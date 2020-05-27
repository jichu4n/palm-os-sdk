/*
 * MiniMP3.c
 *
 * Main File for MiniMp3
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK
 *
 * Copyright (c) 1999-2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include <Hs.h>

#include "mp3.h"
#include "id3.h"
#include "MiniMP3.h"
#include "MiniMP3Rsc.h"
#include "Common.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"
#include <PalmOneCodecPluginMgr.h>
#include <PalmOneCodecFormat.h>


#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#define SRC_SIZE	1024
#define DEST_SIZE	32768

/**
 * MP3 Info
 */
typedef struct
{	
	Boolean 	hasID3v1;		// True if has ID3v1 tag
	Boolean 	hasID3v2;		// True if has ID3v2 tag
	Boolean		hasXing;		// True if has Xing header

	ID3v1Tag	id3;			// ID3v1 tag
	MP3Version	version;		// MP3 version
	MP3Layer	layer;			// MP3 Layer
	UInt32		bitRate;		// Average bit rate
	UInt16		frameSize;		// Average Frame Size
	UInt16		sampleRate;		// Sample rate
	Boolean		VBR;			// True is VBR file
	MP3Mode		channelMode;	// Channel mode (STEREO, JOINT_STEREO...)
					
} MP3Info;

/**
 * File Info
 */
typedef struct
{
	Char	filePath[256];	// Contains filename + full path

	FileRef	fileRef;		// File ref
	UInt32	fileSize;		// File size
	UInt32	fileDataSize;	// Dile data size (=fileEndOffset-fileStartOffset)
	UInt32	fileStartOffset;// File start offset
	UInt32	fileEndOffset;	// File end offset
	
} FileInfo; 

/** Mini info structure. */
typedef struct _MiniInfo MiniInfo;

/**
 * Mini info structure.
 * Stores all the needed info for the sound callback.
 */
struct _MiniInfo
{
	UInt16 			CodecMgrLibRefNum;	// MP3 library
	MP3Info			mp3Info;		// MP3 info
	FileInfo		fileInfo;		// File info
	SndStreamRef	sndRef;			// Stream reference
	PalmCodecSession session;		// MP3 session
	Int32			volume;			// Stream volume 
	
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
};

/**
 * MiniInfo global
 */
static MiniInfo gMiniInfo;
static UInt16	gOldAutoOffTime = 0;
static Char gAlertString[80];
UInt32 temp = 0;
/***********************************************************************
 *
 * FUNCTION:    PrvNewStreamCallback
 *
 * DESCRIPTION: New callback.
 *
 * PARAMETERS:	
 *
 * RETURNED:	Error code
 *
 ***********************************************************************/

#define THRESHOLD (1450)	// That's the size of the biggest possible frame
static Err PrvNewStreamCallback(void *userData, SndStreamRef stream, void *buffer, UInt32 frameCount)
{
	Err err = errNone;
	UInt32 read = 0;
	UInt32 offset = 0;
	MiniInfo *miniInfoP = (MiniInfo*)userData;
	UInt32 numChannels = (miniInfoP->mp3Info.channelMode==MP3_SINGLE_CHANNEL)?1:2;
	
	// Calculate buffer length
	frameCount *=  2 * numChannels;
	
	// Check current output buffer data size
	if(miniInfoP->destDataSize > frameCount)
	{
		// Copy output buffer
		MemMove(buffer, miniInfoP->destBufferP + miniInfoP->destOffset, frameCount);
		miniInfoP->destOffset += frameCount;
		miniInfoP->destDataSize -= frameCount;
	}
	else
	{
		UInt32 inSize = 0;
		UInt32 outSize = 0;
		UInt32 endDataSize = 0;
		
		// Copy the output left over
		MemMove(buffer, miniInfoP->destBufferP + miniInfoP->destOffset, miniInfoP->destDataSize);
		offset = miniInfoP->destDataSize;
		miniInfoP->destOffset = 0;
		miniInfoP->destDataSize = 0;
		
		// Check the input buffer
		if(miniInfoP->srcDataSize < THRESHOLD) {
			// Move the end of the buffer to the front
			MemMove(miniInfoP->srcBufferP, miniInfoP->srcBufferP + miniInfoP->srcOffset, miniInfoP->srcDataSize);
			miniInfoP->srcOffset = 0;
			
			// Read more data from file
			err = VFSFileRead(miniInfoP->fileInfo.fileRef,
					miniInfoP->srcBufferSize - miniInfoP->srcDataSize,
					miniInfoP->srcBufferP + miniInfoP->srcDataSize, &read);
					
			// EOF is not an error
			if(err == vfsErrFileEOF)
				err = errNone;
							
			miniInfoP->srcDataSize += read;
		}
		
		inSize = miniInfoP->srcDataSize;
		outSize = miniInfoP->destBufferSize; 
		
		// Decode data from the input buffer
		err = CodecMgrEncodeDecode(miniInfoP->CodecMgrLibRefNum, miniInfoP->session,
				miniInfoP->srcBufferP + miniInfoP->srcOffset, &inSize,
				miniInfoP->destBufferP, &outSize);
						
		// Check how much was decoded
		if(err || outSize == 0)
		{
			SysNotifyParamType notifyParam;
			
			// Set the notification info
			notifyParam.notifyType = appFileCreator;
			notifyParam.broadcaster = appFileCreator;
			notifyParam.handled = false;
			notifyParam.notifyDetailsP = NULL;
			
			// Send a notification to stop playing
			err = SysNotifyBroadcastDeferred(&notifyParam, 0);
			SndStreamStop(stream);
		}
					
		miniInfoP->srcOffset += inSize;
		miniInfoP->srcDataSize -= inSize;
		miniInfoP->destDataSize = outSize;
		
		endDataSize = frameCount - offset;
		endDataSize = (miniInfoP->destDataSize < endDataSize)?miniInfoP->destDataSize:endDataSize;
		
		// Finish writing to the sound buffer
		MemMove((void*)((Int8*)buffer + offset), miniInfoP->destBufferP, endDataSize);
			
		miniInfoP->destOffset += endDataSize;
		miniInfoP->destDataSize -= endDataSize;
	}
	
	return err;
}

#if 0
#pragma mark -
#endif

/**
 * Shows Alert Box
 */
static void PrvAlert( Char* string)
{

	FrmCustomAlert( InfoAlert, string,"","");

}

/**
 * Open a file
 */
static Err PrvFileOpen(MiniInfo* miniInfoP)
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	
	// Get first card
	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		StrCopy( gAlertString, "Error enumerating Volume"); 
		goto Done;
	}
	
	// If we have a file path, open the file
	if(miniInfoP->fileInfo.filePath[0]!=NULL)
	{
		err = VFSFileOpen(firstVolume, (Char*)miniInfoP->fileInfo.filePath, vfsModeRead, &miniInfoP->fileInfo.fileRef);
		if( err ) 
		{
			StrCopy( gAlertString, "File Open err. Select a Mp3/2 file");
			goto Done;
		}
		
		// Store the file size
		err = VFSFileSize(miniInfoP->fileInfo.fileRef , &miniInfoP->fileInfo.fileSize);
		if( err ) 
		{
			StrCopy( gAlertString, "File Size Error"); 
			goto Done;
		}
		miniInfoP->fileInfo.fileStartOffset = 0;
		miniInfoP->fileInfo.fileEndOffset = miniInfoP->fileInfo.fileSize; 
	}

Done:	

	if(err)	
		PrvAlert( gAlertString );
	
	return err;
}

/**
 * Parse MP3 file
 */
static Err PrvParseMP3(MiniInfo* miniInfoP)
{
	Err err = errNone;
	Char buffer[128];
	FrameInfo frameInfo;
	UInt32 headerSize = 0, header = 0, byteRead = 0;
	StrCopy( gAlertString, "Error in ParseMP3, Corrupt Mp3/2 file");
	
	// Check for ID3v1
	err = VFSFileSeek(miniInfoP->fileInfo.fileRef, vfsOriginEnd , -128);
	if( err ) goto Done;
	err = VFSFileRead(miniInfoP->fileInfo.fileRef, 128, &buffer, &byteRead);
	if( err ) goto Done;
	miniInfoP->mp3Info.hasID3v1 = CheckID3v1((Char*)buffer, byteRead, &miniInfoP->mp3Info.id3);

	// Set the file offset (we don't want to send the tags to the decoder)
	if( miniInfoP->mp3Info.hasID3v1 ) {
		miniInfoP->fileInfo.fileEndOffset -= 128;
	}
	
	// Check for ID3v2
	err = VFSFileSeek(miniInfoP->fileInfo.fileRef, vfsOriginBeginning , 0);
	if( err ) goto Done;
	err = VFSFileRead(miniInfoP->fileInfo.fileRef, 10, buffer, &byteRead);
	if( err ) goto Done;
	miniInfoP->mp3Info.hasID3v2 = CheckID3v2((Char*)buffer, byteRead, &headerSize);

	// Set the file offset (we don't want to send the tags to the decoder)
	if( miniInfoP->mp3Info.hasID3v2 ) {
		miniInfoP->fileInfo.fileStartOffset = headerSize;
	}
		
	// Set the file data size
	miniInfoP->fileInfo.fileDataSize = miniInfoP->fileInfo.fileEndOffset - miniInfoP->fileInfo.fileStartOffset;
		
	//
	// Now decode the first header (no VBR!!!)
	//

	// Rewind to the beginning of the data
	err = VFSFileSeek(miniInfoP->fileInfo.fileRef, vfsOriginBeginning , miniInfoP->fileInfo.fileStartOffset);
	if( err ) goto Done;
	
	// Read the frame header
	err = VFSFileRead(miniInfoP->fileInfo.fileRef, 4, &header, &byteRead);
	if( err ) goto Done;
		
	// Decode the header
	if( DecodeMP3Header( header, &frameInfo) )
	{
		miniInfoP->mp3Info.version = frameInfo.version;
		miniInfoP->mp3Info.layer = frameInfo.layer;
		miniInfoP->mp3Info.bitRate = frameInfo.bitRate;
		miniInfoP->mp3Info.channelMode = frameInfo.channelMode;
		miniInfoP->mp3Info.sampleRate = frameInfo.sampleRate;
		temp = frameInfo.length;
	}
	else
	{
		err = -1;
		goto Done;
	}

	// Rewind to the start of the data
	err = VFSFileSeek(miniInfoP->fileInfo.fileRef, vfsOriginBeginning , miniInfoP->fileInfo.fileStartOffset);
	if( err ) goto Done;

Done:
	if( err )
		PrvAlert( gAlertString );
	return err;
}

/**
 * Create stream.
 *
 *
 */
static Err PrvCreateStream(MiniInfo* miniInfoP)
{
	Err err = errNone;
	UInt8 numChannel = (miniInfoP->mp3Info.channelMode==MP3_SINGLE_CHANNEL)?1:2;
	
	// Use regualr stream
	err = SndStreamCreate(&miniInfoP->sndRef, sndOutput, miniInfoP->mp3Info.sampleRate,
						sndInt16Little,	(numChannel==1)?sndMono:sndStereo,
						&PrvNewStreamCallback, miniInfoP, 2048 / numChannel, false);
		
	if(err)
	{
		StrCopy( gAlertString, "Error creating stream"); 
		goto Done;
	}
	
	if( miniInfoP->mp3Info.layer == LAYER3 )
	{
	
	// Create an MP3 session
		err = CodecMgrCreateSession(miniInfoP->CodecMgrLibRefNum,
			palmCodecAudioMP3, NULL, palmCodecAudioPCM, NULL, &miniInfoP->session);
	}
	
	if(err)
		StrCopy( gAlertString, "Error creating CPM session"); 
	
Done:
	if(err)
		PrvAlert( gAlertString );
			
	return err;
}


/**
 * Stop the current playing file.
 */ 
static void PrvStopFile(MiniInfo *miniInfoP)
{
	Err err = errNone;
	
	// Stop the stream
	if(miniInfoP->sndRef)
		err = SndStreamStop(miniInfoP->sndRef);
	
	// Delete the stream
	if(miniInfoP->sndRef) {
		err = SndStreamDelete(miniInfoP->sndRef);
		miniInfoP->sndRef = NULL;
	}
		
	// Delete the MP3 session
	if(miniInfoP->session) {
		err = CodecMgrDeleteSession(miniInfoP->CodecMgrLibRefNum, &miniInfoP->session);
	}
		
	// Close the file
	if(miniInfoP->fileInfo.fileRef) {
		VFSFileClose(miniInfoP->fileInfo.fileRef);
		miniInfoP->fileInfo.fileRef = NULL;
	}
	
	// Clear the buffers...
	miniInfoP->srcDataSize = 0;
	miniInfoP->srcOffset = 0;
	miniInfoP->destDataSize = 0;
	miniInfoP->destOffset = 0;
}

/**
 * Open the file and start playing.
 */ 
static void PrvPlayFile(MiniInfo* miniInfoP)
{
	Err err = errNone;
	
	// Check that we are not already playing
	if(miniInfoP->sndRef)
		PrvStopFile(miniInfoP);
	
	// Open the file
	err = PrvFileOpen(miniInfoP);
	if(err) goto Done;
		
	// Parse the mp3 file
	err = PrvParseMP3(miniInfoP);
	if(err) goto Done;
	
	// Create the stream
	err = PrvCreateStream(miniInfoP);
	if(err) goto Done;
	
	SndStreamSetVolume(gMiniInfo.sndRef, gMiniInfo.volume);
	
	// Start the stream
	err = SndStreamStart(miniInfoP->sndRef);
		
Done:
	if(err)
		PrvStopFile(miniInfoP);
		
	return;
}

/**
 * File info callback.
 * This function is called by the file browser
 */
static void PrvGetFileInfoCallback(UInt16 volume, const Char* path, const Char* file)
{
	// Create the full path name
	StrCopy(gMiniInfo.fileInfo.filePath, path);

	// Internally, we expect full path
	StrCat(gMiniInfo.fileInfo.filePath, file);
}

#if 0
#pragma mark -
#endif

/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id
 */

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
    FormType *frmAboutP = NULL;
    
	switch (command)
	{
		case HelpAboutMiniMP3:
			frmAboutP = FrmInitForm(HelpAboutMiniMP3);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
						 
			handled = true;
			break;
	}

	return handled;
}


/**
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent(EventType * eventP)
{
    Boolean handled = false;
    FormType *frmP = FrmGetActiveForm();
//    FieldType *fieldP = NULL;
//    static Char time[20];

    switch (eventP->eType) 
        {
        case menuEvent:
            handled = MainFormDoCommand(eventP->data.menu.itemID);
            break;
            
        case frmOpenEvent:
            FrmDrawForm(frmP);
            handled = true;
            break;
            
        case frmUpdateEvent:
            break;
        
        case ctlRepeatEvent:
        	switch(eventP->data.ctlSelect.controlID)
        	{
        		case MainVolumeFeedbackSliderControl:
        			gMiniInfo.volume = eventP->data.ctlRepeat.value;
        			// Set the volume
        			if(gMiniInfo.sndRef) {
        				SndStreamSetVolume(gMiniInfo.sndRef, gMiniInfo.volume);
        			}
        			break;
        	}
        	break;
        	
        case ctlSelectEvent:
        	switch(eventP->data.ctlSelect.controlID)
        	{
        		case MainStartButton:
        			PrvPlayFile(&gMiniInfo);
        			break;
        			
        			
        		case MainStopButton:
        			PrvStopFile(&gMiniInfo);
        			break;
        			
        		case MainFileButton:
        			// Sets the callback function and return form
					FileBrowserSetCallback(&PrvGetFileInfoCallback, MainForm);
					FrmGotoForm(FileBrowserForm);
        			break;        			
        	}
        	break;
            
        default:
            break;
        }
    
    return handled;
}

/**
 * AppHandleEvent
 */
static Boolean AppHandleEvent(EventType * eventP)
{
    UInt16 formId;
    FormType * frmP;

    if (eventP->eType == frmLoadEvent)
    {
        formId = eventP->data.frmLoad.formID;
        frmP = FrmInitForm(formId);
        FrmSetActiveForm(frmP);

        switch (formId)
        {
        case MainForm:
            FrmSetEventHandler(frmP, MainFormHandleEvent);
            break;
            
        case FileBrowserForm:
            FrmSetEventHandler(frmP, FileBrowserFormHandleEvent);
            break;

        default:
            break;

        }
        return true;
    }

    return false;
}

/**
 * AppEventLoop
 */
static void AppEventLoop(void)
{
	UInt16 error = errNone;
	EventType event;

    do
    {
    	EvtGetEvent(&event, evtWaitForever);
    	
    	if (! SysHandleEvent(&event))
    		if (! MenuHandleEvent(0, &event, &error))
    			if (! AppHandleEvent(&event))
    				FrmDispatchEvent(&event);

    } while (event.eType != appStopEvent);
}

/**
 * AppStart
 * Called when the application starts.
 */
static Err AppStart(void)
{
	Err err = errNone;
	
	// Clear the global
	MemSet(&gMiniInfo, sizeof(MiniInfo), 0);

	// Try to find the hardware utils library
	err = SysLibFind(kCodecMgrLibName, &gMiniInfo.CodecMgrLibRefNum);	
	if (err != errNone)
	{
		err = SysLibLoad(kCodecMgrLibType, kCodecMgrLibCreator, &gMiniInfo.CodecMgrLibRefNum);
		if(err == errNone)
			err = CodecMgrOpen(gMiniInfo.CodecMgrLibRefNum);
	}
	
	if(err) {
		FrmAlert(NoMP3LibAlert);
		goto Done;
	}
	
	// Allocate memory for the buffers
	gMiniInfo.srcBufferP = MemPtrNew(SRC_SIZE);
	if(!gMiniInfo.srcBufferP) {
		err = memErrNotEnoughSpace;
		goto Done;
	}
	gMiniInfo.srcBufferSize = SRC_SIZE;
	gMiniInfo.srcDataSize = 0;
	gMiniInfo.srcOffset = 0;
	
		
	gMiniInfo.destBufferP = MemPtrNew(DEST_SIZE);	
	if(!gMiniInfo.destBufferP) {
		err = memErrNotEnoughSpace;
		goto Done;
	}
	
	gMiniInfo.destBufferSize = DEST_SIZE;
	gMiniInfo.destDataSize = 0;
	gMiniInfo.destOffset = 0;
	
	// Volume
	gMiniInfo.volume = 1024;
		
	// Get the current application path
	SysCurAppDatabase(&gMiniInfo.cardNo, &gMiniInfo.dbID);
	
	// Register our own event - Pass the mini info
	SysNotifyRegister(gMiniInfo.cardNo, gMiniInfo.dbID,
		appFileCreator, NULL, sysNotifyNormalPriority, &gMiniInfo );
	
	// Register for REM sleep, to keep playing even when device display is off
	SysNotifyRegister(gMiniInfo.cardNo, gMiniInfo.dbID,
	    hsNotifyRemSleepRequestEvent, NULL, sysNotifyNormalPriority, &gMiniInfo);

	// Quick no-auto off
	gOldAutoOffTime = SysSetAutoOffTime(0);
	
Done:
	if(err)
	{
		if(gMiniInfo.srcBufferP)
			MemPtrFree(gMiniInfo.srcBufferP);
		
		if(gMiniInfo.destBufferP)
			MemPtrFree(gMiniInfo.destBufferP);
	}
			
	return err;
}

/**
 * AppStop
 * Called when the application stops.
 */
static void AppStop(void)
{
	Err err = errNone;
	
    FrmCloseAllForms();
    
    // Restore auto off
	SysSetAutoOffTime(gOldAutoOffTime);
    
    // Make sure the file is stopped
    PrvStopFile(&gMiniInfo);
    
    // Clean the buffers
    if(gMiniInfo.srcBufferP)
		MemPtrFree(gMiniInfo.srcBufferP);
		
	if(gMiniInfo.destBufferP)
		MemPtrFree(gMiniInfo.destBufferP);
	
	// Close the library	
	err = CodecMgrClose(gMiniInfo.CodecMgrLibRefNum);
	if (err == errNone)
		err = SysLibRemove(gMiniInfo.CodecMgrLibRefNum);
	
	// Unregister the notification	
	SysNotifyUnregister( gMiniInfo.cardNo, gMiniInfo.dbID,
			appFileCreator, sysNotifyNormalPriority );
			
	// Unregister REM Sleep
	SysNotifyUnregister(gMiniInfo.cardNo, gMiniInfo.dbID,
	        hsNotifyRemSleepRequestEvent, sysNotifyNormalPriority);
}

#pragma warn_a5_access on

//
// RomVersionCompatible
//
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

//
// PilotMain
//
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err error;
    SysNotifyParamType *notifyParamP = NULL;
//    SleepEventParamType *sleepParamP = NULL;

    error = RomVersionCompatible (ourMinVersion, launchFlags);
    if (error) return (error);

    switch (cmd)
    {
    case sysAppLaunchCmdNormalLaunch:
        error = AppStart();
        if (error) 
            return error;

        FrmGotoForm(MainForm);
        AppEventLoop();
        
        AppStop();
        break;
        
    case sysAppLaunchCmdNotify:
		notifyParamP = (SysNotifyParamType *)cmdPBP;
		if( notifyParamP == NULL ) goto Done;
		
		switch(notifyParamP->notifyType) {
			case appFileCreator:
				PrvStopFile((MiniInfo*)notifyParamP->userDataP);
				break;
			
			case hsNotifyRemSleepRequestEvent:
				// sleepParamP = (SleepEventParamType*)notifyParamP->notifyDetailsP;
				// sleepParamP->deferSleep++;
				
				// Start playing music when it goes to sleep (got to have a file selected first):
				// PrvPlayFile((MiniInfo*)notifyParamP->userDataP);

				break;
				
		}

    default:
        break;
    }

Done:
    return errNone;
}

#pragma warn_a5_access reset
