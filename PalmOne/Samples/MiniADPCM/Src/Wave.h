/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Wave.h
 *
 * Description:
 *	Basic Wave Utilities
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/

#ifndef WAVE_H_
#define WAVE_H_

#include <PalmTypes.h>
#include <VFSMgr.H>

typedef struct {
	UInt32 type;
	UInt32 size;
} ChunkHeader;

typedef struct {
	ChunkHeader header;
} RiffChunk;

typedef struct {
	ChunkHeader header;
	UInt16 audioFormat;
	UInt16 channels;
	UInt32 sampleRate;
	UInt32 avgByteRate;
	UInt16 blockAlign;
	UInt16 bitsPerSample;
} FmtSubChunk;

typedef struct {
	UInt16 size;
	UInt16 samplesPerBlock;
} ExtraDataBlock;

typedef struct {
	UInt32 type;
	UInt32 size;
} DataSubChunk;

typedef struct {
	RiffChunk riff;
	UInt32 waveType;
	FmtSubChunk fmt;
} WaveHeader;

typedef struct {
	WaveHeader header;
	ExtraDataBlock extra;
	DataSubChunk data;
} WaveIMAADPCM;

#define	WAVE_TYPE_PCM			0x0001
#define	WAVE_TYPE_IMAADPCM		0x0011

typedef struct {
	UInt32	format;			// PCM or ADPCM
	UInt32	sampleType;		// SndInt8...
	UInt32	sampleRate;		// 44kHz...
	UInt32	sampleWidth;	// Mono, Stereo...
	UInt32	blockAlign;		// For ADPCM only
} WAVInfo;

/************************************************************
 *
 * Function Prototypes
 *
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err ReadWAVHeader(void *bufferP, UInt32 bufferSize, WAVInfo *wavInfoP, UInt32 *dataOffsetP);
Err WriteADPCM_WAVEHeader(WAVInfo *wavInfoP, FileRef fileRef,  UInt32 fileLength, UInt32 *written);

#ifdef __cplusplus 
}
#endif

#endif /* WAVE_H_ */