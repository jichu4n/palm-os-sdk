/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	mp3.h
 *
 * Description:
 *	MPEG tables and header decoding functions
 *
 *
 ***********************************************************************/

#ifndef MP3_H_
#define MP3_H_

#include <PalmOS.h>

/***********************************************************************
 *
 *	Enums
 *
 ***********************************************************************/

// MPEG Version
typedef enum _MP3Version
{
	MPEG2_5 	= 0,
	MPEG_RSVD,
	MPEG2,
	MPEG1
	
} MP3Version;

// MPEG Layer
typedef enum _MP3Layer
{
	LAYER_RSVD 	= 0,
	LAYER3,
	LAYER2,
	LAYER1
	
} MP3Layer;

// MPEG Channel Mode
typedef enum _MP3Mode
{
	MP3_STEREO		= 0,
	MP3_JOINT_STEREO,
	MP3_DUAL_CHANNEL,
	MP3_SINGLE_CHANNEL
	
} MP3Mode;

/***********************************************************************
 *
 *	Structures
 *
 ***********************************************************************/

typedef struct
{
	MP3Version	version;
	MP3Layer	layer;
	Boolean		CRC;
	UInt32		bitRate;
	UInt16		sampleRate;
	Boolean 	padding;
	MP3Mode		channelMode;
	Boolean		copyright;
	Boolean		original;
	
	UInt32		length;	// In bytes	
	UInt32		duration;	// In milliseconds
	
} FrameInfo;

/************************************************************
 *
 * Function Prototypes
 *
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Boolean DecodeMP3Header(UInt32 header, FrameInfo* info);
Err		QuickFrameSync(Char* buffer, UInt32 bufLen, UInt32* syncPos);	

#ifdef __cplusplus 
}
#endif

#endif /* MP3_H_ */