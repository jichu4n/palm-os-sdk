/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	mp3.c
 *
 * Description:
 *	MPEG tables
 *
 *
 ***********************************************************************/
 
#include "MP3.h"
#include "Common.h"

/***********************************************************************
 *
 * FUNCTION:    GetMPEG1Table
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

//static UInt32 const gMPEG1Table[3][15] = {
//	{ 0, 32000, 64000, 96000, 128000, 160000, 192000, 224000, 256000, 288000, 320000, 352000, 384000, 416000, 448000 }, // Layer 1
//	{ 0, 32000, 48000, 56000,  64000,  80000,  96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000, 384000 }, // Layer 2
//	{ 0, 32000, 40000, 48000,  56000,  64000,  80000,  96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000 }  // Layer 3
//};
 
static  UInt32 GetMPEG1Table(UInt8 layer, UInt8 bits)
{
	if( layer == LAYER3 )
	{
		UInt32 bitRate[15] = { 0, 32000, 40000, 48000,  56000,  64000,  80000,  96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000 };  // Layer 3
		return bitRate[bits];
	}
	else if( layer == LAYER2 )
	{
		UInt32 bitRate[15] = { 0, 32000, 48000, 56000,  64000,  80000,  96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000, 384000 }; // Layer 2
		return bitRate[bits];
	}
	else
	{
		UInt32 bitRate[15] = { 0, 32000, 64000, 96000, 128000, 160000, 192000, 224000, 256000, 288000, 320000, 352000, 384000, 416000, 448000 }; // Layer 1
		return bitRate[bits];
	}
}

/***********************************************************************
 *
 * FUNCTION:    GetMPEG2Table
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

//static UInt32 const gMPEG2Table[3][15] = {
//	{ 0, 32000, 48000, 56000, 64000, 80000, 96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000, 384000 }, // Layer 1
//	{ 0,  8000, 16000, 24000, 32000, 40000, 48000,  56000,  64000,  80000,  96000, 112000, 128000, 144000, 160000 }, // Layer 2
//	{ 0,  8000, 16000, 24000, 32000, 40000, 48000,  56000,  64000,  80000,  96000, 112000, 128000, 144000, 160000 }  // Layer 3
//};

static  UInt32 GetMPEG2Table(UInt8 layer, UInt8 bits)
{
	if( layer == LAYER1 )
	{
		UInt32 bitRate[15] = { 0, 32000, 48000, 56000, 64000, 80000, 96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000, 384000 };  // Layer 1
		return bitRate[bits];
	}
	else
	{
		UInt32 bitRate[15] = { 0,  8000, 16000, 24000, 32000, 40000, 48000,  56000,  64000,  80000,  96000, 112000, 128000, 144000, 160000 }; // Layer 2 & 3
		return bitRate[bits];
	}
}

/***********************************************************************
 *
 * FUNCTION:    GetSampleRateTable
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

//static UInt16 const gSampleRateTable[3][3] = {
//	{ 44100, 48000, 32000 }, // MPEG1
//	{ 22050, 24000, 16000 }, // MPEG2
//	{ 11025, 12000,  8000 }  // MPEG2.5
//};

static  UInt32 GetSampleRateTable(UInt8 version, UInt8 bits)
{
	UInt32 sampleTable[3] = { 44100, 48000, 32000 };
	UInt32 sampleRate = sampleTable[bits];
	
	return (sampleRate >> version);
}

/***********************************************************************
 *
 * FUNCTION:    DecodeMPEGHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Boolean DecodeMP3Header(UInt32 header, FrameInfo* info)
{
	UInt16 temp = 0 ;
	
	// Check the frame sync
	if( (header >> 21) != 0x7FF )
		return false;
	
	// Get the MPEG version
	info->version = (header & 0x180000) >> 19;
	if( info->version == MPEG_RSVD )
		return false;
	
	// Get the Layer
	info->layer = (header & 0x60000) >> 17;
	if( info->layer == LAYER_RSVD )
		return false;
	
	// Get the CRC bit
	info->CRC = (header & 0x1FFFF)?true:false;
	
	// Get the bit rate
	temp = (header & 0xF000) >> 12;
	if( temp == 0xF )
		return false;
	
	switch( info->version )
	{
	case MPEG1:
		info->bitRate = GetMPEG1Table(info->layer, temp);
		break;
	case MPEG2:
	case MPEG2_5:
		info->bitRate = GetMPEG2Table(info->layer, temp);
		break;
	default:
		return false;
		break;
	}
	
	// Get the sample rate
	temp = (header & 0xC00) >> 10;
	if( temp == 0x3 )
		return false;
	
	if( info->version == MPEG2_5 )
		info->sampleRate = GetSampleRateTable(2, temp);
	else
		info->sampleRate = GetSampleRateTable(3 - info->version, temp);
	
	// Get the padding bit
	info->padding = (header & 0x200)?true:false;

	// Get the channel mode
	info->channelMode  = (header & 0xC0) >> 6;
		
	// Get the copyright
	info->copyright = (header & 0x8)?true:false;
	
	// Check if original
	info->original = (header & 0x4)?true:false;

	// Calculate the frame length and duration
	if( info->layer == LAYER1 ) {
		info->length = (( 12 * info->bitRate) / info->sampleRate + info->padding ) * 4;
		info->duration = 384000 / info->sampleRate;
	} else {
		info->length = ( 144 * info->bitRate) / info->sampleRate + info->padding;
		info->duration = 1152000 / info->sampleRate;
	}
	
					
	return true;
}

/***********************************************************************
 *
 * FUNCTION:    QuickFrameSync
 *
 * DESCRIPTION: Tries to find the next correct mp3 frame.
 *				Tests MPEG version, layer, bitrate and sample rate.
 *				There is still a slight chance not to find a sync frame.
 *
 * PARAMETERS:  buffer	- Pointer to a buffer (should be at least 2x frame size)
 *				bufLen	- Buffer length
 *				syncPos	- Returns the position in the buffer where sync
 *							frame ws found.
 *
 * RETURNED:    Error Code	- 0 if OK, 1 if no sync frame found.
 *
 ***********************************************************************/

Err	QuickFrameSync(Char* buffer, UInt32 bufLen, UInt32* syncPos)
{
	FrameInfo frameInfo;
	UInt32 header = 0;
	UInt32 pos = 0;
//	UInt32 version = 0;
//	UInt32 layer = 0;
	
	bufLen = bufLen - 4; // Protection
	while( pos < bufLen )
	{
		header = MisalignedReadBEUInt32(buffer+pos, 0);
		// Check if it is a frame
		if( DecodeMP3Header(header, &frameInfo) )
		{
			// Check next frame to make sure... (99.999%)
			header = MisalignedReadBEUInt32(buffer+pos+frameInfo.length, 0);
			if( DecodeMP3Header(header, &frameInfo) )
			{
				*syncPos = pos;
				return errNone;
			}
			else
				pos++;
		}
		else
			pos++;
	}
	
	*syncPos = 0;
	return 1;
}



