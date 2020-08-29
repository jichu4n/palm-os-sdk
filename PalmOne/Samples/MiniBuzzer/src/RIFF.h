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
#include <VFSMgr.H>

/**
 * Common RIFF structures
 **/
typedef struct {
	UInt32 type;
	UInt32 size;
} ChunkHeader;

typedef struct {
	ChunkHeader header;
} RiffChunk;

typedef struct {
	UInt32	data1;
	UInt16	data2;
	UInt16	data3;
	UInt8	data4[8];
} GUIDType;

/**
 * WAV Files
 **/
typedef struct {
	ChunkHeader header;
	UInt16 audioFormat;
	UInt16 channels;
	UInt32 sampleRate;
	UInt32 avgByteRate;
	UInt16 blockAlign;
	UInt16 bitsPerSample;
} FmtSubChunkWav;

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
	FmtSubChunkWav fmt;
} WaveHeader;

typedef struct {
	WaveHeader header;
	ExtraDataBlock extra;
	DataSubChunk data;
} WaveIMAADPCM;

/**
 * QCP Files
 **/
typedef struct {
	UInt32	numOfRates;
	struct {
		UInt8	size;
		UInt8	rate;
	}		bytesPerPacket[8];
} QPLVarRate;

#define QCP_FMT_MJV	1
#define QCP_FMT_MNV	0

typedef struct {
	ChunkHeader header;
	UInt8		mjv;	// major version of this format (always 1)
	UInt8		mnv;	// minor version of this format (always 0)
	GUIDType	id;
	UInt16		codecVersion;
	Char		name[80];
	UInt16		avgBitsPerSec;
	UInt16		bytesPerPacket;
	UInt16		samplesPerBlock;
	UInt16		samplesPerSec;
	UInt16		bitsPerSample;
	QPLVarRate	variableRate;
	UInt32		reserved[5];
} FmtSubChunkQcp;

typedef struct {
	ChunkHeader header;
	UInt32		variableRate;
	UInt32		sizeInPackets;
} VratSubChunk;

typedef struct {
	RiffChunk		riff;
	UInt32  		qlcmType;
	FmtSubChunkQcp 	fmt;
	VratSubChunk	vrat;
} QCPHeader;

typedef struct {
	QCPHeader header;
	DataSubChunk data;
} RiffQCP;

/**
 * Sound data
 */
#define	WAVE_TYPE_PCM			0x0001
#define	WAVE_TYPE_IMAADPCM		0x0011

#define palmCodecAudioQCELP 'QCLP'

typedef struct {
	UInt32	format;			// QCP
	UInt32	sampleType;		// SndInt8...
	UInt32	sampleRate;		// 44kHz...
	UInt32	sampleWidth;	// Mono, Stereo...
	UInt32	blockAlign;		// For ADPCM only
} WAVInfo;

/**
 * File Info
 */
typedef struct
{
	Char	filePath[256];	// Contains filename + full path

	FileRef	fileRef;		// File ref
	FileHand	fileHand;		// File ref
	UInt32	fileSize;		// File size
	UInt32	fileDataSize;	// Dile data size (=fileEndOffset-fileStartOffset)
	UInt32	fileStartOffset;// File start offset
	UInt32	fileEndOffset;	// File end offset
	
} FileInfo; 

/** Mini info structure. */
typedef struct _MiniInfo MiniInfo;

#define CALLBACK_MAX	100

typedef struct
{
	UInt32	deltaTick;
	UInt32	processTicks;
	UInt32	codecBytesIn;
	UInt32	codecBytesOut;

} CallbackStatsType;

/**
 * Mini info structure.
 * Stores all the needed info for the sound callback.
 */
struct _MiniInfo
{
	UInt16 			codecMgrRef;	// CPM library library
	WAVInfo			wavInfo;		// Wav info
	FileInfo		fileInfo;		// File info
	SndStreamRef	sndRef;			// Stream reference
	Int32			volume;			// Stream volume 
	Boolean			recording;		// True is recording
	
	Boolean			pcm;			// True to record/playback raw PCM
	
	Int8	*srcBufferP;	// The input buffer.
	UInt32	srcBufferSize;	// Size of the input buffer
	UInt32	srcDataSize;	// Data in source buffer
	UInt32	srcOffset;		// Offset in source buffer
	
	Int8	*destBufferP;	// Decoded buffer output
	UInt32	destBufferSize;	// Size the destination buffer
	UInt32	destDataSize;	// Data in decoded buffer
	UInt32	destOffset;		// Offset in the destination buffer
	
	UInt16	cardNo;
	LocalID dbID;
	
	// Stream callback debugging
	UInt16	cbCount;
	UInt32	cbLastTick;
	UInt32	cbDeltaTick[CALLBACK_MAX];
	UInt32	cbProcessTicks[CALLBACK_MAX];
	UInt32	cbBytesIn[CALLBACK_MAX];
	UInt32	cbBytesOut[CALLBACK_MAX];	
};

/************************************************************
 *
 * Function Prototypes
 *
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err ReadQCPHeader(void *bufferP, UInt32 bufferSize, WAVInfo *wavInfoP, UInt32 *dataOffsetP);
Err WriteQCPHeader(MiniInfo *miniInfoP, UInt32 fileLength, UInt32 *written);

#ifdef __cplusplus 
}
#endif

#endif /* WAVE_H_ */