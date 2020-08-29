/*
 * MiniADPCM.c
 *
 * 
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include "Wave.h"
#include "Common.h"
#include "MiniADPCM.h"
#include "MiniADPCMRsc.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"
#include <PalmOneCodecPluginMgr.h>
#include <PalmOneCodecFormat.h>
#include <PalmSoundMgrExt.h>

#include <HsNav.h>
#include <HsExt.h>


#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#define SRC_SIZE	2048
#define DEST_SIZE	8192

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
	UInt16 			codecMgrRef;	// CPM library library
	WAVInfo			wavInfo;		// Wav info
	FileInfo		fileInfo;		// File info
	SndStreamRef	sndRef;			// Stream reference
	PalmCodecSession session;		// CPM session
	Int32			volume;			// Stream volume 
	Boolean			recording;		// True is recording
	
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

/**
 * Playback Sound Stream callback
 */
static Err PrvSndStreamCallback(void *userDataP, SndStreamRef stream, void *bufferP, UInt32 frameCount)
{
	Err err = errNone;
	UInt32 read = 0;
	UInt32 offset = 0;
	MiniInfo *miniInfoP = (MiniInfo*)userDataP;
	
	// Calculate buffer length
	frameCount *=  2 * miniInfoP->wavInfo.sampleWidth;
	
	// Check current output buffer data size
	if(miniInfoP->destDataSize > frameCount)
	{
		// Copy output buffer
		MemMove(bufferP, miniInfoP->destBufferP + miniInfoP->destOffset, frameCount);
		miniInfoP->destOffset += frameCount;
		miniInfoP->destDataSize -= frameCount;
	}
	else
	{
		UInt32 inSize = 0;
		UInt32 outSize = 0;
		UInt32 endDataSize = 0;
		//UInt32 requiredOutputBufferSize = 0;
		
		// Copy the output left over
		MemMove(bufferP, miniInfoP->destBufferP + miniInfoP->destOffset, miniInfoP->destDataSize);
		offset = miniInfoP->destDataSize;
		miniInfoP->destOffset = 0;
		miniInfoP->destDataSize = 0;
		
		// Check the input buffer
		if(miniInfoP->srcDataSize == 0) {
			
			// Read more data from file
			err = VFSFileRead(miniInfoP->fileInfo.fileRef, miniInfoP->srcBufferSize, miniInfoP->srcBufferP, &read);
			miniInfoP->srcOffset = 0;
					
			// EOF is not an error
			if(err == vfsErrFileEOF)
				err = errNone;
							
			miniInfoP->srcDataSize += read;
		}
		
		
		inSize = miniInfoP->srcDataSize;
		outSize = miniInfoP->destBufferSize; 
		
		
		// Decode data from the input buffer
		err = CodecMgrEncodeDecode(miniInfoP->codecMgrRef, miniInfoP->session,
				miniInfoP->srcBufferP + miniInfoP->srcOffset, &inSize,
				miniInfoP->destBufferP, &outSize);
						
		// Check how much was decoded
		if(err || outSize == 0)
		{
			SysNotifyParamType notifyParam;
			
			// Stop the stream
			SndStreamStop(stream);
			
			// Set the notification info
			notifyParam.notifyType = appFileCreator;
			notifyParam.broadcaster = appFileCreator;
			notifyParam.handled = false;
			notifyParam.notifyDetailsP = NULL;
			
			// Send a notification to turn the screen off
			err = SysNotifyBroadcastDeferred(&notifyParam, 0);
			goto Done;
		}
					
		miniInfoP->srcOffset += inSize;
		miniInfoP->srcDataSize -= inSize;
		miniInfoP->destDataSize = outSize;
		
		endDataSize = frameCount - offset;
		endDataSize = (miniInfoP->destDataSize < endDataSize)?miniInfoP->destDataSize:endDataSize;
		
		// Finish writing to the sound buffer
		MemMove((void*)((Int8*)bufferP + offset), miniInfoP->destBufferP, endDataSize);
			
		miniInfoP->destOffset += endDataSize;
		miniInfoP->destDataSize -= endDataSize;
	}

Done:	
	return err;
}

/**
 * Record Sound Stream callback
 */
static Err PrvSndStreamRecordCallback(void *userDataP, SndStreamRef stream, void *bufferP, UInt32 frameCount)
{
	Err err = errNone;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
	UInt32 written = 0;
	MiniInfo *miniInfoP = (MiniInfo*)userDataP;
	
	// Calculate buffer length
	frameCount *=  2 * miniInfoP->wavInfo.sampleWidth;
		
	inSize = frameCount;
	outSize = DEST_SIZE; 
		
	// Encode data from the input buffer
	err = CodecMgrEncodeDecode(miniInfoP->codecMgrRef, miniInfoP->session, bufferP, &inSize, miniInfoP->destBufferP, &outSize);
						
	// Check for error
	if(err || outSize == 0)
	{
		SysNotifyParamType notifyParam;
		
		// Stop the stream
		SndStreamStop(stream);
		
		// Set the notification info
		notifyParam.notifyType = appFileCreator;
		notifyParam.broadcaster = appFileCreator;
		notifyParam.handled = false;
		notifyParam.notifyDetailsP = NULL;
		
		// Send a notification to turn the screen off
		err = SysNotifyBroadcastDeferred(&notifyParam, 0);
		goto Done;
	}

	// Write the encoded data more data from file
	err = VFSFileWrite(miniInfoP->fileInfo.fileRef, outSize , miniInfoP->destBufferP, &written);
	miniInfoP->fileInfo.fileSize += written;

Done:	
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
		
	// Delete the ADPCM session
	if(miniInfoP->session) {
		err = CodecMgrDeleteSession(miniInfoP->codecMgrRef, &miniInfoP->session);
	}
	
	// Write the file size
	if(gMiniInfo.recording)
	{
		UInt32 dontcare;
		WAVInfo wavInfo;
		wavInfo.sampleRate = 44100;
		wavInfo.sampleWidth = 1;
		
		VFSFileSeek(gMiniInfo.fileInfo.fileRef, vfsOriginBeginning , 0);
		WriteADPCM_WAVEHeader(&wavInfo, gMiniInfo.fileInfo.fileRef, gMiniInfo.fileInfo.fileSize, &dontcare);
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
 * Open a file
 */
static Err PrvFileOpen(Boolean record)
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	UInt16 openMode = vfsModeRead;
	
	// Get first card
	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		StrCopy( gAlertString, "Error - no mounted volumes found. Insert an SD card"); 
		goto Done;
	}
	
	if(record)
		openMode = vfsModeWrite | vfsModeCreate | vfsModeTruncate;
	
	// If we have a file path, open the file
	if(gMiniInfo.fileInfo.filePath[0]!=NULL )
	{
		err = VFSFileOpen(firstVolume, (Char*)gMiniInfo.fileInfo.filePath, openMode, &gMiniInfo.fileInfo.fileRef);
		
		if( err ) 
		{
			StrCopy( gAlertString, "File Open err. Select a ADPCM file"); 
			goto Done;
		}
		
		if(!record)
		{
			// Store the file size
			err = VFSFileSize(gMiniInfo.fileInfo.fileRef , &gMiniInfo.fileInfo.fileSize);
			if( err ) 
			{
				StrCopy( gAlertString, "File Size Error"); 
				goto Done;
			}
			gMiniInfo.fileInfo.fileStartOffset = 0;
			gMiniInfo.fileInfo.fileEndOffset = gMiniInfo.fileInfo.fileSize; 
		}
	}

Done:	

	if(err)
	{
		PrvStopFile(&gMiniInfo);
		PrvAlert( gAlertString );
	}
	return err;
}

/**
 * Parse WAV file
 */
static Err PrvParseWAV()
{
	Err err = errNone;
	Char buffer[128];
	UInt32 byteRead = 0;
	StrCopy( gAlertString, "Error in ParseWAV");

	// Check the WAV header
	err = VFSFileSeek(gMiniInfo.fileInfo.fileRef, vfsOriginBeginning , 0);
	if( err ) goto Done;
	err = VFSFileRead(gMiniInfo.fileInfo.fileRef, 128, &buffer, &byteRead);
	if( err ) 
	{
		StrCopy( gAlertString, "Error accessing the file"); 
		goto Done;
	}
	
	// Decode the WAV header
	err = ReadWAVHeader(buffer, byteRead, &gMiniInfo.wavInfo, &gMiniInfo.fileInfo.fileStartOffset);
	if( err )
	{	
		StrCopy( gAlertString, "File not of type ADPCM"); 
		goto Done;
	}
	 
	
	// Rewind to the start of the data
	err = VFSFileSeek(gMiniInfo.fileInfo.fileRef, vfsOriginBeginning , gMiniInfo.fileInfo.fileStartOffset);
	
Done:
	if(err)
		PrvAlert( gAlertString );
		
	return err;
}

/**
 * Create stream.
 */
static Err PrvCreateStream(Boolean record)
{
	Err err = errNone;
	PalmAudioADPCMParamType *adpcmParamP = NULL;

	adpcmParamP = MemPtrNew(sizeof(PalmAudioADPCMParamType));
	adpcmParamP->sampleRate = Swap32(gMiniInfo.wavInfo.sampleRate);
	adpcmParamP->sampleWidth = Swap32(gMiniInfo.wavInfo.sampleWidth); 
	adpcmParamP->blockSize = Swap32(gMiniInfo.wavInfo.blockAlign);
	
	// Record stream
	if(record) {
		err = SndStreamCreate(&gMiniInfo.sndRef, sndInput, 44100, sndInt16Little, sndMono,
					&PrvSndStreamRecordCallback, &gMiniInfo, 0, false);
	} else {
	// Playback stream
		err = SndStreamCreate(&gMiniInfo.sndRef, sndOutput, gMiniInfo.wavInfo.sampleRate,
					sndInt16Little,	(gMiniInfo.wavInfo.sampleWidth==1)?sndMono:sndStereo,
					&PrvSndStreamCallback, &gMiniInfo, 0, false);
	}

	if(err) 
	{
		StrCopy( gAlertString, "Error creating stream"); 
		goto Done;
	}
	
	// Set the stream volume
	err = SndStreamSetVolume(gMiniInfo.sndRef, gMiniInfo.volume);
	if(err)
	{
		StrCopy( gAlertString, "Error in SetVolume");
		goto Done;
	}

	// Create an ADPCM decoding session
	if(record) {
		err = CodecMgrCreateSession(gMiniInfo.codecMgrRef,
				palmCodecAudioPCM, NULL, palmCodecAudioIMA_ADPCM, adpcmParamP, &gMiniInfo.session);
	} else {
		err = CodecMgrCreateSession(gMiniInfo.codecMgrRef,
				palmCodecAudioIMA_ADPCM, adpcmParamP, palmCodecAudioPCM, NULL, &gMiniInfo.session);
	}

	if(err)
		StrCopy( gAlertString, "Error creating CPM session"); 

Done:
//	if(adpcmParamP)
		MemPtrFree(adpcmParamP);
	
	if(err)
		PrvAlert( gAlertString );
	return err;
}



/**
 * Open the file and start playing.
 */ 
static void PrvPlayFile()
{
	Err err = errNone;
	
	// Check that we are not already playing
	if(gMiniInfo.sndRef)
		PrvStopFile(&gMiniInfo);
	
	// Open the file
	err = PrvFileOpen(false);
	if(err) goto Done;
		
	// Parse the wav file
	err = PrvParseWAV();
	if(err) goto Done;
	
	// Create the stream
	err = PrvCreateStream(false);
	if(err) goto Done;
	
	gMiniInfo.recording = false;
	
	// Start the stream
	err = SndStreamStart(gMiniInfo.sndRef);
		
Done:
	if(err)
		PrvStopFile(&gMiniInfo);
		
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

/**
 * Record File
 */
static void PrvRecordFile()
{
	Err err = errNone;
	WAVInfo wavInfo;
	wavInfo.sampleRate = 44100;
	wavInfo.sampleWidth = 1;
	
	// Check that we are not already playing
	if(gMiniInfo.sndRef)
		PrvStopFile(&gMiniInfo);
		
	// Set the output file
	StrCopy(gMiniInfo.fileInfo.filePath, "/ADPCMRecord.wav");
		
	// Open the file
	err = PrvFileOpen(true);
	if(err) goto Done;
		
	// Write ADPCM header
	err = WriteADPCM_WAVEHeader(&wavInfo, gMiniInfo.fileInfo.fileRef, 0, &gMiniInfo.fileInfo.fileSize);
	if(err) goto Done;
	
	gMiniInfo.wavInfo.sampleRate = 44100;
	gMiniInfo.wavInfo.sampleWidth = 1;
	gMiniInfo.wavInfo.blockAlign = 1024;

	// Create the stream
	err = PrvCreateStream(true);
	if(err) goto Done;

	gMiniInfo.recording = true;
	
	// Start the stream
	err = SndStreamStart(gMiniInfo.sndRef);
		
Done:
	if(err)
		PrvStopFile(&gMiniInfo);
		
	return;
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

	UInt16  cardNo;
	LocalID dbID;
									 
	SysCurAppDatabase(&cardNo, &dbID);  								 

	switch (command)
	{
		case OptionsAbout:
			MenuEraseStatus(0);
			HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
			handled = true;
			break;

		default:
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
			return MainFormDoCommand(eventP->data.menu.itemID);
    
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
        			if(gMiniInfo.sndRef) 
        			{
        				SndStreamSetVolume(gMiniInfo.sndRef, gMiniInfo.volume);
        			}
        			break;
        	}
        	break;
        	
        case ctlSelectEvent:
        	switch(eventP->data.ctlSelect.controlID)
        	{
        		case MainStartButton:
        		    FrmNavObjectTakeFocus(frmP, MainStartButton);
        			PrvPlayFile();
        			handled = true;
        			break;
        			
        		case MainRecordButton:
        		    FrmNavObjectTakeFocus(frmP, MainRecordButton);
        			PrvRecordFile();
        			handled = true;
        			break;
        			
        		case MainStopButton:
        		    FrmNavObjectTakeFocus(frmP, MainStopButton);
        			PrvStopFile(&gMiniInfo);
        			handled = true;
        			break;
        			
        		case MainFileButton:
        			// Sets the callback function and return form
					FileBrowserSetCallback(&PrvGetFileInfoCallback, MainForm);
					FrmGotoForm(FileBrowserForm);
					FrmNavObjectTakeFocus(frmP, MainFileButton);
					handled = true;
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
	err = SysLibFind(kCodecMgrLibName, &gMiniInfo.codecMgrRef);	
	if (err != errNone)
		err = SysLibLoad(kCodecMgrLibType, kCodecMgrLibCreator, &gMiniInfo.codecMgrRef);
	if(err == errNone)
		err = CodecMgrOpen(gMiniInfo.codecMgrRef);
		
	if(err) {
		FrmAlert(NoCPMLibAlert);
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
	err = CodecMgrClose(gMiniInfo.codecMgrRef);
	if (err == errNone)
		err = SysLibRemove(gMiniInfo.codecMgrRef);
	
	// Unregister the notification	
	SysNotifyUnregister( gMiniInfo.cardNo, gMiniInfo.dbID,
			appFileCreator, sysNotifyNormalPriority );
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
		}

    default:
        break;
    }

Done:
    return errNone;
}

#pragma warn_a5_access reset
