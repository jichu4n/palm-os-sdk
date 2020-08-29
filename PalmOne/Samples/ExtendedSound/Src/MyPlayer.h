/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	MyPlayer.h
 *
 * Description:
 *	MyPlayer header.
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/

#ifndef TESTEXTENDEDSOUND_H_
#define TESTEXTENDEDSOUND_H_

#include <PalmOS.h>
#include <VFSMgr.h>
#include "PalmSoundMgrExt.h"
#include "PmPalmOSNVFS.h"

/*********************************************************************
 * Constants
 *********************************************************************/

#define	appCreatorID	'eXsD'
 
#define ourMinVersion    sysMakeROMVersion(5,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#define	kBlinkTime	(sysTicksPerSecond / 3)
#define kNilTime	(sysTicksPerSecond / 4)

/*********************************************************************
 * Player states
 *********************************************************************/
 
typedef enum {
	IDLE_REC	= 0,
	IDLE_PLAY,
	PLAYING,
	RECORDING,
	PAUSED_REC,
	PAUSED_PLAY
} PlayerState;

/*********************************************************************
 * File types
 *********************************************************************/

typedef enum {
	VFS_File,
	Memory_File
} FileType;


/*********************************************************************
 * Player info structure
 *********************************************************************/

typedef struct _PlayerInfo{
	UInt32			sampleRate;		// 8000/11025/16000/...
	UInt16			blockAlign;		// Mostly there for ADPCM
	UInt32			dataSize;		// Size of the data chunk (in bytes)
	UInt32			bufferSize;		// Requested buffer size
	UInt32			actualSize;		// Real buffer size
	Int32			streamVolume;	// Volume [0..2048]
	SndStreamMode	streamMode;		// Input/Output
	SndFormatType	streamFormat;	// sndFormatPCM/sndFormatIMA_ADPCM
	SndStreamWidth	streamWidth;	// sndMono/sndStereo
	SndSampleType	sampleType;		// UInt8....
	SndStreamRef	streamRef;		// Our stream reference
	
	Char 			fileName[64];	// File name
	FileType		fileType;		// VFS/Memory
	union {
		FileRef		fileRef;		// File ref for VFS
		FileHand	fileHandle;		// File Handle for File Streaming
	}file;
	
	UInt16			volumeRef;		// Card ref
	Char 			directory[256];	// The SD/MMC card audio directory
	PlayerState		playerState;	// IDLE/PLAYING/PAUSED...
} PlayerInfo;

/***********************************************************************
 *	Player Notification Type
 ***********************************************************************/

typedef enum _PlayerNotification {
	DONE_PLAYING	
} PlayerNotificationType;

/***********************************************************************
 *	Player Notification info
 ***********************************************************************/

typedef struct {
	PlayerInfo *playerInfoP;
	PlayerNotificationType	notificationType;

} PlayerNotification;

/*********************************************************************
 * Globals
 *********************************************************************/
 
extern PlayerInfo	gPlayerInfo;		// Player info
extern UInt32		gCurrentTime;		// Current record/play time
extern UInt32		gStartTime;			// Start of playing/recording sessions
extern UInt32		gPreviousRun;		// Take into account pause.
extern UInt8		gBlinkState;		// Controls time blinking during pause.
extern UInt32		gPreviousBlinkTime;	// Make sure the blinking looks nice
extern Char			gErrorCode[32];		// Error code buffer

/*********************************************************************
 * UI Options - Keep in sync with UI
 *********************************************************************/

extern const FileType		gUIFileTypeTable[];
extern const SndStreamMode	gUIStreamModeTable[];
extern const SndFormatType	gUIStreamFormatTable[];
extern const SndSampleType	gUISampleTypeTable[];
extern const UInt32			gUISampleRateTable[];


#endif // TESTEXTENDEDSOUND_H_