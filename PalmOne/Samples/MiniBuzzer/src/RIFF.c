/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Wave.c
 *
 * Description:
 *	WAV functions. Very bad code. But good enough fo the sample code.
 *
 * Author:
 *	November 22, 2002	Created by Nicolas Pabion
 *
 ***********************************************************************/

#include <PalmOS.h>
#include "RIFF.h"
#include "Common.h"
#include "PalmSoundMgrExt.h"

#define MisalignedReadBEUInt16(pointer, offset)											\
			(((UInt16)(*(((UInt8 *)(pointer)) + offset + sizeof(UInt8)))) |				\
			 (((UInt16)(*(((UInt8 *)(pointer)) + offset))) << 8))

#define MisalignedReadBEUInt32(pointer, offset)															\
			(((UInt32)(MisalignedReadBEUInt16((((UInt8 *)(pointer)) + sizeof(UInt16)), offset))) |		\
			 (((UInt32)(MisalignedReadBEUInt16(pointer, offset))) << 16))

#define	TYPE_RIFF	'RIFF'
#define	TYPE_WAVE	'WAVE'
#define	TYPE_FMT	'fmt '
#define	TYPE_DATA	'data'
#define TYPE_QLCM	'QLCM'
#define TYPE_VRAT	'vrat'

extern Err PrvFileWrite(MiniInfo *miniInfoP, UInt32 numBytes, const void *bufP, UInt32 *numBytesWrittenP);

static const UInt8 qcpStructDef[] = { 	0x41, 0x14, 0x41,									// RIFF
										0x41, 0x14, 0x21, 0x14, 0x22, 0x81, 0x12,			// fmt
										0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1,
										0x52, 0x14, 0x81, 0x81, 0x54,
										0x41, 0x34,											// vrat
										0x41, 0x14,											// data
										0x00 };
										
static const UInt8 fmtChunkStructDef[] = {	0x41, 0x14, 0x21, 0x14, 0x22, 0x81, 0x12,
											0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1,
											0x52, 0x14, 0x81, 0x81, 0x54 };

/***********************************************************************
 *
 * FUNCTION:    ReadQCPHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err ReadQCPHeader(void *bufferP, UInt32 bufferSize, WAVInfo *wavInfoP, UInt32 *dataOffsetP)
{
	Err 		err = -1;
	UInt32		type = 0;
	UInt32 		size = 0;
	UInt32		dataOffset = 0;
//	Boolean		dataFound = false;
	UInt8 		*tempP = bufferP;
	
	/*
	 * Just skip the RIFF packaging for now
	 */
	wavInfoP->format		= palmCodecAudioQCELP;
	wavInfoP->sampleType	= sndInt16Little;
	wavInfoP->sampleRate	= 8000;
	wavInfoP->sampleWidth	= 1;
	wavInfoP->blockAlign	= 320;
	*dataOffsetP = 0;
	return errNone;



	
	// Make sure we have enough to read from
	if( bufferSize < 44)
		goto Done;
	
	// Check for the RIFF tag
	if( MisalignedReadBEUInt32(tempP, 0) != TYPE_RIFF )
		goto Done;
		
	// Check for the QLCM tag
	if( MisalignedReadBEUInt32(tempP, 8) != TYPE_QLCM )
		goto Done;
	
	tempP += 12;
	bufferSize -= 12;
	dataOffset += 20;
	
	// Look for the fmt and data section
	while( true )
	{
		// Make sure we have enough data
		if( bufferSize < 4 )
			goto Done;
			
		type = MisalignedReadBEUInt32(tempP, 0);
		size = Swap32(MisalignedReadBEUInt32(tempP, 4));
		
		// Check for the data section
		if( type == 'data' )
		{
			// We're done
			err = errNone;
			*dataOffsetP = dataOffset;
			goto Done;
		}
		
		if( type == 'fmt ')
		{
			FmtSubChunkQcp fmtChunk;
			
			if (size != sizeof(fmtChunk) - sizeof(ChunkHeader))
				goto Done;
			
			// Read the fmt section
			MemMove (&fmtChunk, tempP, sizeof(fmtChunk));
			err = SwapStructureInPlace (&fmtChunk, fmtChunkStructDef, sizeof(fmtChunk));
	
			// Check the version
			if (fmtChunk.mjv != QCP_FMT_MJV || fmtChunk.mnv != QCP_FMT_MNV)
				goto Done;

			wavInfoP->format = palmCodecAudioQCELP;

			switch (fmtChunk.bitsPerSample)
			{
				case 8:		wavInfoP->sampleType = sndUInt8;		break;
				case 16:	wavInfoP->sampleType = sndInt16Little;	break;
				default:	goto Done;
			}

			wavInfoP->sampleRate = fmtChunk.samplesPerSec;
			
			wavInfoP->sampleWidth = 1; // mono
			
			wavInfoP->blockAlign = fmtChunk.samplesPerBlock * fmtChunk.bitsPerSample/8;
		}
			
		dataOffset += 8 + size;
		bufferSize -= 8 + size;
		tempP += 8 + size;
	}
	
Done:
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    WriteQCPHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
Err WriteQCPHeader(MiniInfo *miniInfoP, UInt32 fileLength, UInt32 *written)
{
	RiffQCP		qcp;
	Err			err = -1;
	UInt32		dataWritten = 0;
	UInt32		bytesPerSample = miniInfoP->wavInfo.sampleType & 0x0F;
//	UInt32		bytesPerSecond = bytesPerSample * miniInfoP->wavInfo.sampleWidth * miniInfoP->wavInfo.sampleRate;
	UInt8		rate;

	/*
	 * Just skip the RIFF packaging for now
	 */
	*written = 0;
	return errNone;
	
	
	
	
	if (miniInfoP->wavInfo.format != palmCodecAudioQCELP)
		goto Done;
		
	if (miniInfoP->wavInfo.sampleType != sndInt16Little)
		goto Done;
		
	if (miniInfoP->wavInfo.sampleRate != 8000)
		goto Done;
		
	if (miniInfoP->wavInfo.sampleWidth != 1)
		goto Done;
		
	if (miniInfoP->wavInfo.blockAlign != 160 * 2)
		goto Done;
		
	// RIFF Header
	qcp.header.riff.header.type = TYPE_RIFF;
	qcp.header.riff.header.size	= sizeof(RiffQCP) + fileLength - sizeof(ChunkHeader);
	qcp.header.qlcmType			= TYPE_QLCM;
	
	// fmt sub-chunk
	MemSet (&qcp.header.fmt, sizeof(qcp.header.fmt), 0);
	qcp.header.fmt.header.type		= TYPE_FMT;
	qcp.header.fmt.header.size		= sizeof(qcp.header.fmt) - sizeof(ChunkHeader);
	qcp.header.fmt.mjv				= QCP_FMT_MJV;
	qcp.header.fmt.mnv				= QCP_FMT_MNV;
	//qcp.header.fmt.id
	//qcp.header.fmt.codecVersion
	//qcp.header.fmt.name
	qcp.header.fmt.avgBitsPerSec	= 0; //!!!
	//qcp.header.fmt.bytesPerPacket
	qcp.header.fmt.samplesPerBlock	= miniInfoP->wavInfo.blockAlign / bytesPerSample;
	qcp.header.fmt.samplesPerSec	= miniInfoP->wavInfo.sampleRate;
	qcp.header.fmt.bitsPerSample	= bytesPerSample * 8;
	qcp.header.fmt.variableRate.numOfRates = 5;
	for (rate = 0; rate <= 4; rate++)
	{
		static const UInt8 qcelpPacketSize[5] = { 0, 3, 7, 16, 34 };
		qcp.header.fmt.variableRate.bytesPerPacket[4-rate].rate = rate;
		qcp.header.fmt.variableRate.bytesPerPacket[4-rate].size = qcelpPacketSize[rate];
	}
	
	// vrat sub-chunk
	qcp.header.vrat.header.type 	= TYPE_VRAT;
	qcp.header.vrat.header.size		= sizeof(qcp.header.vrat) - sizeof(ChunkHeader);
	qcp.header.vrat.variableRate	= 1;	//!!!
	qcp.header.vrat.sizeInPackets	= 0; 			//!!!

	// data sub-chunk
	qcp.data.type					= TYPE_DATA;
	qcp.data.size					= fileLength;

	err = SwapStructureInPlace (&qcp, qcpStructDef, sizeof(qcp));
	err = PrvFileWrite(miniInfoP, sizeof(qcp), &qcp, &dataWritten);

Done:
	return err;
}
