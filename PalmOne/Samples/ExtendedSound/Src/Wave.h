/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
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
#include "MyPlayer.h"

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
	DataSubChunk data; 
} WavePCM;

typedef struct {
	WaveHeader header;
	ExtraDataBlock extra;
	DataSubChunk data;
} WaveIMAADPCM;

#define	WAVE_TYPE_PCM			0x0001
#define	WAVE_TYPE_IMAADPCM		0x0011

/************************************************************
 *
 * Function Prototypes
 *
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err ReadWAVEHeader(PlayerInfo *playerInfoP);
Err WritePCM_WAVEHeader(PlayerInfo *playerInfoP);
Err WriteADPCM_WAVEHeader(PlayerInfo *playerInfoP);

#ifdef __cplusplus 
}
#endif

#endif /* WAVE_H_ */