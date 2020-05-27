/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	MyPlayer.c
 *
 * Description:
 *	Test Application for the Extended Sound API
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/

#include <PalmOS.h>
#include <VFSMgr.h>
#include "PlayForm.h"
#include "RecordForm.h"
#include "MyPlayer.h"
#include "MyPlayerRsc.h"
#include "Common.h"

#if 0
#pragma mark Globals
#endif

/*********************************************************************
 * Globals Initialization
 *********************************************************************/

PlayerInfo		gPlayerInfo;

Char			gErrorCode[32]	= { NULL };
UInt16 			gPlayer_cardNo 	= 0;
LocalID			gPlayer_dbID	= 0;
UInt32			gCurrentTime	= 0;
UInt32			gStartTime		= 0;
UInt32			gPreviousRun	= 0;
UInt8			gBlinkState		= 0;
UInt32			gPreviousBlinkTime = 0;

UInt32			gUISampleRate	= 8000;
SndFormatType	gUIStreamFormat	= sndFormatPCM;
SndStreamWidth	gUIStreamWidth	= sndMono;
SndSampleType	gUISampleType 	= sndUInt8;
FileType		gUIFileType		= VFS_File;
Int32			gUIStreamVolume = 1024;

const FileType		gUIFileTypeTable[] = { VFS_File, Memory_File };
const SndStreamMode	gUIStreamModeTable[] = { sndMono, sndStereo };
const SndFormatType	gUIStreamFormatTable[] = { sndFormatPCM, sndFormatIMA_ADPCM };
const SndSampleType	gUISampleTypeTable[] = { sndUInt8, sndInt16Little };
const UInt32		gUISampleRateTable[] = { 8000, 11025, 16000, 22050, 32000, 44100 };


#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: Main application event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event.
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

static Boolean AppHandleEvent(EventType *eventP)
{
	UInt16 formId = 0;
	FormType *frmP = NULL;

	if(eventP->eType == frmLoadEvent)
	{
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		switch (formId)
		{
		case RecordForm:
			FrmSetEventHandler(frmP, RecordFormHandleEvent);
			break;

		case PlayForm:
			FrmSetEventHandler(frmP, PlayFormHandleEvent);
			break;

		default:
			break;
		}
		return true;
	}

	return false;
}

/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: Main Application event loop.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void AppEventLoop(void)
{
	EventType 	event;
	Err			error = errNone;
	Int32		timeout = evtWaitForever;

	do
	{
		EvtGetEvent(&event, timeout);

		if(!SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				if(!AppHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
			}
		}

		// Timeout is used to fake the time when a stream
		// is recording or playing. Refresh every 1/4 sec.
		switch(gPlayerInfo.playerState)
		{
		case IDLE_REC:
		case IDLE_PLAY:
			timeout = evtWaitForever;
			break;
		case PAUSED_REC:
		case PAUSED_PLAY:
		case RECORDING:
		case PLAYING:
			timeout = kNilTime;
			break;
		}

	} while(event.eType != appStopEvent);
}

/***********************************************************************
 *
 * FUNCTION:    AppStart
 *
 * DESCRIPTION: Called when the application starts
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static Err AppStart(void)
{
	Err 	err = errNone;
	UInt32 	volIterator = vfsIteratorStart;
	VolumeInfoType volInfo;
    UInt16 dirLen = 0;
    
	// Check for Extended Sampled Sound feature
	if( !PalmSGHasExtendedSampledSound() ) {
		FrmAlert(NoSampledSoundAlert);
		return -1;
	}

	// First reset our global player info
	MemSet(&gPlayerInfo, sizeof(PlayerInfo), 0);
	// Make sure the player is in idle state
	gPlayerInfo.playerState = IDLE_REC;

	// Here we'll try to check if a card is present
	while(err == errNone && volIterator != vfsIteratorStop)
	{
		// volIterator will point to NEXT volume after VFSVolumeEnumerate,
		// so cannot use to test for success after breaking out of loop.
		UInt32 	curVolIterator = volIterator;

		err = VFSVolumeEnumerate(&gPlayerInfo.volumeRef, &volIterator);
		if (err) break;

		err = VFSVolumeInfo(gPlayerInfo.volumeRef, &volInfo);
		if (err) break;

		if (!(volInfo.attributes & vfsVolumeAttrNonRemovable))
		{
			// Reset volIterator so it can used accurately in 
			// test for success following this loop.
			volIterator = curVolIterator;
			break;
		}
	}

	//if (err || volIterator == vfsIteratorStop) return 1;
	
	// No SD card insert
	// Dont return 1 because we still want the app to be launched
	if (err == expErrEnumerationEmpty) 
	{
		FrmCustomAlert (InformationAlert,"Please insert a SD card before running the sample code", "", "");
	}

	// If a card is present, create the default directory for wav files
	dirLen = sizeof(gPlayerInfo.directory);

	err = VFSGetDefaultDirectory(gPlayerInfo.volumeRef, ".wav", gPlayerInfo.directory, &dirLen);
	if (err == errNone)
	{
		dirLen = StrLen(gPlayerInfo.directory);
		if(dirLen)
		{
			if (gPlayerInfo.directory[dirLen - 1] == '/')
				gPlayerInfo.directory[dirLen - 1] = '\0';
		} else
			StrCat(gPlayerInfo.directory, "/AUDIO");

		VFSDirCreate(gPlayerInfo.volumeRef, gPlayerInfo.directory);
	}

	// Default file name
	//StrCopy(gPlayerInfo.fileName, "default.wav");
	gPlayerInfo.bufferSize = 0;
	gPlayerInfo.fileType = VFS_File;
	gPlayerInfo.streamWidth = sndMono;
	gPlayerInfo.sampleType = sndUInt8;
	gPlayerInfo.streamFormat = sndFormatPCM;
	gPlayerInfo.sampleRate = 8000;
	gPlayerInfo.streamVolume = 1024;

	// Get the current application path
	SysCurAppDatabase(&gPlayer_cardNo, &gPlayer_dbID);

	// Register our own event so the stream callback can interact with code
	SysNotifyRegister( gPlayer_cardNo, gPlayer_dbID,
		appCreatorID, PlayStreamEventNotify, sysNotifyNormalPriority, &gPlayerInfo );

	return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Called when the application exits
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void AppStop(void)
{
	FrmCloseAllForms();

	// Unregister for our own event
	SysNotifyUnregister( gPlayer_cardNo, gPlayer_dbID,
			appCreatorID, sysNotifyNormalPriority );
}

/* all code from here to end of file should use no global variables */
#pragma warn_a5_access on

/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: Check the if the ROM is compatible with the application
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
    UInt32 romVersion;

    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    if (romVersion < requiredVersion)
    {
        if ((launchFlags &
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
        {
            FrmAlert (RomIncompatibleAlert);

             if (romVersion < kPalmOS20Version)
            {
                AppLaunchWithCommand(
                    sysFileCDefaultApp,
                    sysAppLaunchCmdNormalLaunch, NULL);
            }
        }

        return sysErrRomIncompatible;
    }

    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: Main entry point for the application.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err error;

    error = RomVersionCompatible (ourMinVersion, launchFlags);
    if (error) return (error);

    switch (cmd)
    {
    case sysAppLaunchCmdNormalLaunch:
        error = AppStart();
// Even there is no SD card, we still launch the app.
        if (error)
            return error;

        FrmGotoForm(RecordForm);
        AppEventLoop();

        AppStop();
        break;

    default:
        break;
    }

    return errNone;
}

/* turn a5 warning off to prevent it being set off by C++
 * static initializer code generation */
#pragma warn_a5_access reset
