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
 * FUNCTION:    PrvReadFile
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static Err PrvReadFile(UInt8 *bufferP, UInt32 *bufferSizeP, PlayerInfo *playerInfoP)
{
	Err err = errNone;
	UInt32 dataRead = 0;
	
	if(playerInfoP->fileType == VFS_File) {
		err = VFSFileRead(playerInfoP->file.fileRef, *bufferSizeP, bufferP, &dataRead);
	} else {
		dataRead = FileRead(playerInfoP->file.fileHandle, bufferP, 1, *bufferSizeP, &err);
	}
	
	*bufferSizeP = dataRead;
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvSeekFile
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static Err PrvSeekFile(UInt32 offset, PlayerInfo *playerInfoP)
{
	Err err = errNone;
	
	if(playerInfoP->fileType == VFS_File) {
		err = VFSFileSeek(playerInfoP->file.fileRef, vfsOriginBeginning, offset);
	} else {
		err = FileSeek(playerInfoP->file.fileHandle, offset, fileOriginBeginning);
	}
		
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    ReadWAVEHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err ReadWAVEHeader(PlayerInfo *playerInfoP)
{
	WaveHeader 	header;
	Err 		err = -1;
	UInt32		type = 0;
	UInt32 		size = 0;
	UInt32		dataRead = 256;
	UInt32		dataUsed = 0;
	UInt32		dataOffset = 0;
	Boolean		dataFound = false;
	UInt8 		*tempP = NULL;
	UInt8 		*bufferP = MemPtrNew(256);
	
	if( !bufferP )
		goto Done;
	
	tempP = bufferP;
	MemSet(&header, sizeof(header), 0);
	
	// Read a chunk
	PrvReadFile(bufferP, &dataRead, playerInfoP);
	
	// Make sure we have enough to read from
	if( dataRead < 44)
		goto Done;
	
	if( MisalignedReadBEUInt32(bufferP, 0) != 'RIFF' )
		goto Done;
		
	// Check Wave
	if( MisalignedReadBEUInt32(bufferP, 8) != 'WAVE' )
		goto Done;
	
	bufferP += 12;
	dataUsed -= 12;
	dataOffset += 20;
	
	// Look for the fmt and data section
	while( !dataFound )
	{
		// Read more if needed
		if( dataRead - dataUsed < 4 ) {
			dataRead = 256;
			dataUsed = 0;
			PrvReadFile(bufferP, &dataRead, playerInfoP);
		}
			
		// Make sure we have enough data
		if( dataRead < 4 )
			goto Done;
			
		type = MisalignedReadBEUInt32(bufferP, 0);
		size = Swap32(MisalignedReadBEUInt32(bufferP, 4));
		
		// Check for the data section
		if( type == 'data' )
		{
			// We're done
			dataFound = true;
		}
		else if( type == 'fmt ')
		{
			UInt16 format = 0;
			UInt16 numChannels = 0;
			UInt32 byteRate = 0;
			UInt16 blockAlign = 0;
			UInt16 bitsPerSample = 0;
			
			// Read the fmt section
			format = Swap16(MisalignedReadBEUInt16(bufferP, 8));
			if( format != WAVE_TYPE_PCM && format != WAVE_TYPE_IMAADPCM )
				goto Done;
				
			playerInfoP->streamFormat = (format==WAVE_TYPE_PCM)?sndFormatPCM:sndFormatIMA_ADPCM;
			
			// Get the number of channels
			numChannels = Swap16(MisalignedReadBEUInt16(bufferP, 10));
			if( numChannels < 1 || numChannels > 2)
				goto Done;
				
			playerInfoP->streamWidth = (numChannels==1)?sndMono:sndStereo;
			
			// Get the sample rate
			playerInfoP->sampleRate = Swap32(MisalignedReadBEUInt32(bufferP, 12));
			if( playerInfoP->sampleRate < 8000 || playerInfoP->sampleRate > 96000 )
				goto Done;
			
			// Get the byte rate
			byteRate = Swap32(MisalignedReadBEUInt32(bufferP, 16));
			
			// Get the block align
			blockAlign = Swap16(MisalignedReadBEUInt16(bufferP, 20));
			if( blockAlign & (blockAlign - 1) )
				goto Done;
			
			// Get the number of bits per sample
			bitsPerSample = Swap16(MisalignedReadBEUInt16(bufferP, 22));
			if( format == WAVE_TYPE_PCM )
			{
				if( blockAlign != ((numChannels * bitsPerSample) / 8))
					goto Done;
					
				if( bitsPerSample != 8 && bitsPerSample != 16 )
					goto Done;
					
				playerInfoP->sampleType = (bitsPerSample==8)?sndUInt8:sndInt16Little;
			}
			else // ADPCM
			{				
				if( blockAlign < 256 )
					goto Done;
				if( bitsPerSample != 4)
					goto Done;
				
				playerInfoP->blockAlign = blockAlign;
			}
			
			bufferP += size + 8;
			dataUsed -= size + 8;
			dataOffset += size + 8;
		}
		else
		{
			dataOffset += 8 + size;
			dataUsed -= 8 + size;
			bufferP += 8 + size;
		}
	}
	
	// Advance the file to the right position
	err = PrvSeekFile(dataOffset, playerInfoP);
	if(err)
		goto Done;
		
	// Made it!
	err = errNone;
	
Done:
	if( tempP )
		MemPtrFree(tempP);
		
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    WritePCM_WAVEHeader
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err WritePCM_WAVEHeader(PlayerInfo *playerInfoP)
{
	WavePCM		pcm;
	Err			err = errNone;
	UInt32		dataWritten = 0;
	UInt16		numChannels = (playerInfoP->streamWidth==sndMono)?1:2;
	UInt32		bitsPerSample = (playerInfoP->sampleType==sndUInt8)?8:16;
	UInt32		blockAlign = numChannels * bitsPerSample / 8;
	
	// RIFF Header
	pcm.header.riff.header.type = TYPE_RIFF;
	pcm.header.riff.header.size	= Swap32(sizeof(WavePCM) + playerInfoP->dataSize - sizeof(ChunkHeader));
	pcm.header.waveType			= TYPE_WAVE;
	
	// fmt sub-chunk
	pcm.header.fmt.header.type	= TYPE_FMT;
	pcm.header.fmt.header.size	= Swap32(sizeof(FmtSubChunk) - sizeof(ChunkHeader));
	pcm.header.fmt.audioFormat	= Swap16(WAVE_TYPE_PCM);
	pcm.header.fmt.channels		= Swap16(numChannels);
	pcm.header.fmt.sampleRate	= Swap32(playerInfoP->sampleRate);
	pcm.header.fmt.avgByteRate	= Swap32(playerInfoP->sampleRate * blockAlign);
	pcm.header.fmt.blockAlign	= Swap16(blockAlign);
	pcm.header.fmt.bitsPerSample= Swap16(bitsPerSample);
	
	// data sub-chunk
	pcm.data.type				= TYPE_DATA;
	pcm.data.size				= Swap32(playerInfoP->dataSize);
	
	if(playerInfoP->fileType == VFS_File) {
		err = VFSFileWrite(playerInfoP->file.fileRef, sizeof(pcm), &pcm, &dataWritten);
	} else {
		dataWritten = FileWrite(playerInfoP->file.fileHandle, &pcm, 1, sizeof(pcm), &err);
	}

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

Err WriteADPCM_WAVEHeader(PlayerInfo *playerInfoP)
{
	WaveIMAADPCM adpcm;
	Err			err = errNone;
	UInt32		dataWritten = 0;
	UInt16		numChannels = (playerInfoP->streamWidth==sndMono)?1:2;
	UInt16 		bitsPerSample = 4;	// ADPCM always encodes to 4-bit samples
	UInt16 		blockAlign = 256 * numChannels;
	UInt32 		samplesPerBlock = 0;

	// Claculate the block size		
	switch(playerInfoP->sampleRate)
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
	adpcm.header.riff.header.size	= Swap32(sizeof(WaveIMAADPCM) + playerInfoP->dataSize - sizeof(ChunkHeader));
	adpcm.header.waveType			= TYPE_WAVE;
	
	// fmt sub-chunk
	adpcm.header.fmt.header.type	= TYPE_FMT;
	adpcm.header.fmt.header.size	= Swap32(sizeof(FmtSubChunk) - sizeof(ChunkHeader) + sizeof(ExtraDataBlock));
	adpcm.header.fmt.audioFormat	= Swap16(WAVE_TYPE_IMAADPCM);
	adpcm.header.fmt.channels		= Swap16(numChannels);
	adpcm.header.fmt.sampleRate		= Swap32(playerInfoP->sampleRate);
	adpcm.header.fmt.avgByteRate	= Swap32(0);
	adpcm.header.fmt.blockAlign		= Swap16(blockAlign);
	adpcm.header.fmt.bitsPerSample	= Swap16(bitsPerSample);
	
	adpcm.extra.size				= Swap16(sizeof(adpcm.extra.samplesPerBlock));
	adpcm.extra.samplesPerBlock		= Swap16(samplesPerBlock);
	
	// data sub-chunk
	adpcm.data.type					= TYPE_DATA;
	adpcm.data.size					= Swap32(playerInfoP->dataSize);
	
	if(playerInfoP->fileType == VFS_File) {
		err = VFSFileWrite(playerInfoP->file.fileRef, sizeof(adpcm), &adpcm, &dataWritten);
	} else {
		dataWritten = FileWrite(playerInfoP->file.fileHandle, &adpcm, 1, sizeof(adpcm), &err);
	}

	return err;
}