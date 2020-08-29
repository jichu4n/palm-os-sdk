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
#include <PalmSoundMgrExt.h>
#include "Wave.h"

#define MisalignedReadBEUInt16(pointer, offset)											\
			(((UInt16)(*(((UInt8 *)(pointer)) + offset + sizeof(UInt8)))) |				\
			 (((UInt16)(*(((UInt8 *)(pointer)) + offset))) << 8))

#define MisalignedReadBEUInt32(pointer, offset)															\
			(((UInt32)(MisalignedReadBEUInt16((((UInt8 *)(pointer)) + sizeof(UInt16)), offset))) |		\
			 (((UInt32)(MisalignedReadBEUInt16(pointer, offset))) << 16))

#define	Swap16(n)	(((((UInt16) n) << 8) & 0xFF00) | \
                     ((((UInt16) n) >> 8) & 0x00FF))

#define Swap32(n)	(((((UInt32) n) << 24) & 0xFF000000) |	\
                     ((((UInt32) n) <<  8) & 0x00FF0000) |	\
                     ((((UInt32) n) >>  8) & 0x0000FF00) |	\
                     ((((UInt32) n) >> 24) & 0x000000FF))

#define	TYPE_RIFF	(Swap32('FFIR'))
#define	TYPE_WAVE	(Swap32('EVAW'))
#define	TYPE_FMT	(Swap32(' tmf'))
#define	TYPE_DATA	(Swap32('atad'))

/***********************************************************************
 *
 * FUNCTION:    ReadWAVHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err ReadWAVHeader(void *bufferP, UInt32 bufferSize, WAVInfo *wavInfoP, UInt32 *dataOffsetP)
{
	Err 		err = -1;
	UInt32		type = 0;
	UInt32 		size = 0;
	UInt32		dataOffset = 0;
	Boolean		dataFound = false;
	UInt8 		*tempP = bufferP;
	
	// Make sure we have enough to read from
	if( bufferSize < 44)
		goto Done;
	
	// Check for the RIFF tag
	if( MisalignedReadBEUInt32(tempP, 0) != 'RIFF' )
		goto Done;
		
	// Check for the Wave tag
	if( MisalignedReadBEUInt32(tempP, 8) != 'WAVE' )
		goto Done;
	
	tempP += 12;
	bufferSize -= 12;
	dataOffset += 20;
	
	// Look for the fmt and data section
	while( !dataFound )
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
			dataFound = true;
			err = errNone;
		}
		else if( type == 'fmt ')
		{
			UInt16 format = 0;
			UInt16 numChannels = 0;
			UInt32 byteRate = 0;
			UInt16 blockAlign = 0;
			UInt16 bitsPerSample = 0;
			
			// Read the fmt section
			format = Swap16(MisalignedReadBEUInt16(tempP, 8));
			if( format != WAVE_TYPE_IMAADPCM )
				goto Done;
			else
				wavInfoP->format = sndFormatIMA_ADPCM;	
			
			// Get the number of channels
			numChannels = Swap16(MisalignedReadBEUInt16(tempP, 10));
			if( numChannels < 1 || numChannels > 2)
				goto Done;
				
			wavInfoP->sampleWidth = numChannels;
			
			// Get the sample rate
			wavInfoP->sampleRate = Swap32(MisalignedReadBEUInt32(tempP, 12));
			if( wavInfoP->sampleRate < 8000 || wavInfoP->sampleRate > 96000 )
				goto Done;
			
			// Get the byte rate
			byteRate = Swap32(MisalignedReadBEUInt32(tempP, 16));
			
			// Get the block align
			blockAlign = Swap16(MisalignedReadBEUInt16(tempP, 20));
			if( blockAlign & (blockAlign - 1) )
				goto Done;
			
			// Get the number of bits per sample
			bitsPerSample = Swap16(MisalignedReadBEUInt16(tempP, 22));
//DEADCODE, not reachable
/*
			if( format == WAVE_TYPE_PCM )
			{
				if( blockAlign != ((numChannels * bitsPerSample) / 8))
					goto Done;
					
				if( bitsPerSample != 8 && bitsPerSample != 16 )
					goto Done;
					
				wavInfoP->sampleType = (bitsPerSample==8)?sndUInt8:sndInt16Little;
			}
*/
//			else // ADPCM
//			{				
				if( blockAlign < 256 )
					goto Done;
				if( bitsPerSample != 4)
					goto Done;
				
				wavInfoP->blockAlign = blockAlign;
//			}
			
			tempP += size + 8;
			bufferSize -= size + 8;
			dataOffset += size + 8;
		}
		else
		{
			dataOffset += 8 + size;
			bufferSize -= 8 + size;
			tempP += 8 + size;
		}
	}
	
	*dataOffsetP = dataOffset;

Done:
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    WriteADPCM_WAVEHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err WriteADPCM_WAVEHeader(WAVInfo *wavInfoP, FileRef fileRef, UInt32 fileLength, UInt32 *written)
{
	WaveIMAADPCM adpcm;
	Err			err = errNone;
	UInt32		dataWritten = 0;
	UInt16		numChannels = wavInfoP->sampleWidth;
	UInt16 		bitsPerSample = 4;	// ADPCM always encodes to 4-bit samples
	UInt16 		blockAlign = 256 * numChannels;
	UInt32 		samplesPerBlock = 0;

	// Claculate the block size		
	switch(wavInfoP->sampleRate)
	{
	case 8000:
	case 11025:
	case 16000:
		break;
	case 22050:
	case 32000:
		blockAlign *= 2;
		break;
	case 44100:
		blockAlign *= 4;
		break;
	}
	
	samplesPerBlock = ((blockAlign - (4 * numChannels)) * 8) / 
											(bitsPerSample * numChannels) + 1;
	
	// RIFF Header
	adpcm.header.riff.header.type 	= TYPE_RIFF;
	adpcm.header.riff.header.size	= Swap32(sizeof(WaveIMAADPCM) + fileLength - sizeof(ChunkHeader));
	adpcm.header.waveType			= TYPE_WAVE;
	
	// fmt sub-chunk
	adpcm.header.fmt.header.type	= TYPE_FMT;
	adpcm.header.fmt.header.size	= Swap32(sizeof(FmtSubChunk) - sizeof(ChunkHeader) + sizeof(ExtraDataBlock));
	adpcm.header.fmt.audioFormat	= Swap16(WAVE_TYPE_IMAADPCM);
	adpcm.header.fmt.channels		= Swap16(numChannels);
	adpcm.header.fmt.sampleRate		= Swap32(wavInfoP->sampleRate);
	adpcm.header.fmt.avgByteRate	= Swap32(0);
	adpcm.header.fmt.blockAlign		= Swap16(blockAlign);
	adpcm.header.fmt.bitsPerSample	= Swap16(bitsPerSample);
	
	adpcm.extra.size				= Swap16(sizeof(adpcm.extra.samplesPerBlock));
	adpcm.extra.samplesPerBlock		= Swap16(samplesPerBlock);
	
	// data sub-chunk
	adpcm.data.type					= TYPE_DATA;
	adpcm.data.size					= Swap32(fileLength);
	
	err = VFSFileWrite(fileRef, sizeof(adpcm), &adpcm, &dataWritten);

//Done:
	return err;
}
