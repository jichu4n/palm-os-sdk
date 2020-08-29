/*
 * MiniWAV.c
 *
 * main file for TestPen
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include "RIFF.h"
#include "Common.h"
#include "MiniBuzzer.h"
#include "MiniBuzzerRsc.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"
#include "Common\Libraries\CodecPluginMgr\palmOneCodecFormat.h"
#include "Common\Libraries\CodecPluginMgr\palmOneCodecPluginMgrCommon.h"
#include "68K\Libraries\SndFileStream\palmOneSndFileStream.h"

#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

enum {
	NO_TYPE,
	WAVE_TYPE,
	AMR_TYPE,
	QCELP_TYPE,
	MP3_TYPE,
	APCM_TYPE,
	AAC_TYPE,
	NUM_TYPE
};

/** Our globals. */

static Char		gFileName[64];
static Char		gFilePath[256];
static UInt8	gBitmapFlag = 0; 
static UInt16	gSndVolume = 1024;
//static UInt32	gOldAutoOffTime = 0;
static UInt16	gCurrentVolume = 0;
static UInt16	gRefNum = sysInvalidRefNum;
static UInt8	gType = NO_TYPE;
static FileRef	gFileRef = NULL;
static Char		*gTypeName[NUM_TYPE] = { "No Selection", "WAVE", "AMR", "QCELP", "MP3", "APCM", "AAC" };

static UInt16	cardNo;
static LocalID 	dbID;

static SndFileStreamRef gSndFileStream = NULL;

#if 0
#pragma mark -
#endif


 /** Deals with menu commands issued */
static void HandleMenu(UInt16 itemID)
{
	FormType *frmAboutP = NULL;

	switch(itemID)
	{		
		case AboutForm:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
						
			break;
			
	}
}


/** Playback callback */
static Err MySndFileStreamCallback(SndFileStreamRef stream, SndFileStreamDataPtr streamDataP, Boolean *customDataModified)
{
	Err err = errNone;
	
	// Can do something here!
	
	return err;
}


/** Stop current file. */
static Err PrvStopFile()
{
	Err err = errNone;
	
	if(gSndFileStream)
	{
		err = SndFileStreamClose(gRefNum, gSndFileStream);		
		gSndFileStream = NULL;
	}
	
	return err;
}

/** Play the selected file. */
static Err PrvPlayFile(UInt16 playbackCount)
{
	Err err = errNone;
	SndFileStreamData streamData;
	SndFileStreamFileSpec fileSpec;
	
	err = PrvStopFile();
	if(err) {
		goto Done;
	}		
	
	// Get the first card if no volume selected
	if(!gCurrentVolume) {
		UInt32 volumeIterator = vfsIteratorStart;
		err = VFSVolumeEnumerate(&gCurrentVolume, &volumeIterator);
		if( err == expErrEnumerationEmpty ) {
			gCurrentVolume = 0;
			FrmCustomAlert(HelloAlert, "No Card found", NULL, NULL);
			goto Done;
		}
	}
	
	// Setup structures
	MemSet(&fileSpec, sizeof(fileSpec), 0);
	fileSpec.flags = kSndFileStreamFileSpecFlagVFS;
	fileSpec.volRefNum = gCurrentVolume;
	fileSpec.fileNameP = gFilePath;
	fileSpec.fileCreator = 0;
	fileSpec.fileType = 0;

	// Stream data
	MemSet(&streamData, sizeof(streamData), 0);
	streamData.streamFlags = 0;
	streamData.streamMode = sndOutput;
	streamData.playbackCount = playbackCount;
		
	// Call buzzer
	err = SndFileStreamOpen(gRefNum, &gSndFileStream, &streamData, &fileSpec, MySndFileStreamCallback);
	if(err) {
		FrmCustomAlert(HelloAlert, "Buzzer can't open this file", NULL, NULL);
		goto Done;
	}
	
	// Set the volume
	err = SndFileStreamSetVolume(gRefNum, gSndFileStream, gSndVolume);
	if(err) {
		FrmCustomAlert(HelloAlert, "Buzzer can't set the volume", NULL, NULL);
		goto Done;
	}
	
	// Start playing
	err = SndFileStreamStart(gRefNum, gSndFileStream);
	if(err) {
		FrmCustomAlert(HelloAlert, "Buzzer can't start playing", NULL, NULL);
		goto Done;
	}
		
Done:
	return err;	
}

/** Record to the selected file. */
static Err PrvRecordFile()
{
	Err err = errNone;
	SndFileStreamData streamData;
	SndFileStreamFileSpec fileSpec;
	
	err = PrvStopFile();
	if(err) {
		goto Done;
	}		
	
	// Get the first card if no volume selected
	if(!gCurrentVolume) {
		UInt32 volumeIterator = vfsIteratorStart;
		err = VFSVolumeEnumerate(&gCurrentVolume, &volumeIterator);
		if( err == expErrEnumerationEmpty ) {
			gCurrentVolume = 0;
			FrmCustomAlert(HelloAlert, "No Card found", NULL, NULL);
			goto Done;
		}
	}
	
	// Setup structures
	MemSet(&fileSpec, sizeof(fileSpec), 0);
	fileSpec.flags = kSndFileStreamFileSpecFlagVFS;
	fileSpec.volRefNum = gCurrentVolume;
	fileSpec.fileNameP = gFilePath;
	fileSpec.fileCreator = 0;
	fileSpec.fileType = 0;

	// Stream data
	MemSet(&streamData, sizeof(streamData), 0);
	
	switch (gType)
	{
	case WAVE_TYPE:
		streamData.streamFlags = 0;
		streamData.streamMode = sndInput;
		streamData.fileFormat = kSndFileFormatWAV;
		streamData.sampleFormat = palmCodecAudioPCM;
		streamData.sampleType = sndInt16Little;
		streamData.sampleWidth = sndMono;
		streamData.sampleRate = 8000;
		streamData.sampleBlockAlign = 2; 
		break;
		
	case QCELP_TYPE:
		streamData.streamFlags = 0;
		streamData.streamMode = sndInput;
		streamData.fileFormat = kSndFileFormatTreoQCP;
		streamData.sampleFormat = palmCodecAudioQCELP;
		streamData.sampleType = sndInt16Little;
		streamData.sampleWidth = sndMono;
		streamData.sampleRate = 8000;
		streamData.sampleBlockAlign = 320;
		break;
		
	case AMR_TYPE:
		streamData.streamFlags = 0;
		streamData.streamMode = sndInput;
		streamData.fileFormat = kSndFileFormatTreoAMR;
		streamData.sampleFormat = palmCodecAudioAMR;
		streamData.sampleType = sndInt16Little;
		streamData.sampleWidth = sndMono;
		streamData.sampleRate = 8000;
		streamData.sampleBlockAlign = 0; 
		break;
		
	case APCM_TYPE:
		streamData.streamFlags = 0;
		streamData.streamMode = sndInput;
		streamData.fileFormat = kSndFileFormatWAV;
		streamData.sampleFormat = palmCodecAudioIMA_ADPCM;
		streamData.sampleType = sndInt16Little;
		streamData.sampleWidth = sndMono;
		streamData.sampleRate = 8000;
		streamData.sampleBlockAlign = 256; 
		break;
	
	default:
		FrmCustomAlert(HelloAlert, "Buzzer can't support this file type", NULL, NULL);
		err = kSndFileStreamLibErrUnsupportedFile;
		goto Done;
	}
	
	streamData.maximumMilliseconds = 60000; // Set record time limit to be 1 minute		
		
	// Call buzzer
	err = SndFileStreamCreate(gRefNum, &gSndFileStream, &streamData, &fileSpec, MySndFileStreamCallback);
	if(err) {
		FrmCustomAlert(HelloAlert, "Buzzer can't open this file", NULL, NULL);
		goto Done;
	}
	
	// Set the volume
	err = SndFileStreamSetVolume(gRefNum, gSndFileStream, gSndVolume);
	if(err) {
		FrmCustomAlert(HelloAlert, "Buzzer can't set the volume", NULL, NULL);
		goto Done;
	}
	
	// Start recording
	err = SndFileStreamStart(gRefNum, gSndFileStream);
	if(err) {
		FrmCustomAlert(HelloAlert, "Buzzer can't start recording", NULL, NULL);
		goto Done;
	}
		
Done:
	return err;	
}

/**
 * File info callback.
 * This function is called by the file browser
 */
static void PrvGetFileInfoCallback(UInt16 volume, const Char* path, const Char* file)
{
	// Create the full path name
	StrCopy(gFilePath, path);

	// Internally, we expect full path
	StrCat(gFilePath, file);
	StrCopy(gFileName, file);
}


#if 0
#pragma mark -
#endif

static void MainFormInit(FormType *frmP)
{
	// Check the type
	if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".wav") == 0)
		gType = WAVE_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".amr") == 0)
		gType = AMR_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".qcp") == 0)
		gType = QCELP_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".mp3") == 0)
		gType = MP3_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 5, ".apcm") == 0)
		gType = APCM_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".m4a") == 0)
		gType = AAC_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".mp4") == 0)
		gType = AAC_TYPE;
	else if(StrCaselessCompare(gFileName + StrLen(gFileName) - 4, ".aac") == 0)
		gType = AAC_TYPE;
	else
		gType = NO_TYPE;

	// Update file name
	SetFieldTextFromStr(FrmGetPtr(frmP, MainFileField), gFileName, false);
	
	// Update the type
	SetFieldTextFromStr(FrmGetPtr(frmP, MainTypeField), gTypeName[gType], false);
	
	// update slider
	CtlSetSliderValues(FrmGetPtr(frmP, MainVolumeFeedbackSliderControl), NULL, NULL, NULL, &gSndVolume);
}

/**
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent(EventType * eventP)
{
    Boolean handled = false;
    FormType *frmP = FrmGetActiveForm();
    FieldType *fieldP = NULL;
//    static Char time[20];
    Err err = errNone;

    switch (eventP->eType) 
        {
        case frmOpenEvent:
        	MainFormInit(frmP);
            FrmDrawForm(frmP);
            handled = true;
            break;
            
        case frmUpdateEvent:
            break;
        
        case ctlRepeatEvent:
        	break;
        	
        case ctlSelectEvent:
        	switch(eventP->data.ctlSelect.controlID)
        	{     			
        		case MainFileButton:
        			// Sets the callback function and return form
					FileBrowserSetCallback(&PrvGetFileInfoCallback, MainForm);
					FrmGotoForm(FileBrowserForm);
					handled = true;
        			break;
        			
        		case MainVolumeFeedbackSliderControl:
        			gBitmapFlag = 1 - gBitmapFlag;
        			gSndVolume = eventP->data.ctlSelect.value;
        			CtlSetGraphics(eventP->data.ctlSelect.pControl, (gBitmapFlag) ? CloseMouthBitmapFamily : OpenMouthBitmapFamily, NULL);
        			if(gSndFileStream)
        				err = SndFileStreamSetVolume(gRefNum, gSndFileStream, gSndVolume);
				if(err==errNone) 
					handled = true;
        			break;
        			
        		case MainPlayButton:
        			if(PrvPlayFile(3) == errNone) {
        				FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainPlayButton));
        				FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStopPButton));
        			}
        			handled = true;
        			break; 
        			
        		case MainStopPButton:
        			PrvStopFile();
        			FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStopPButton));
        			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainPlayButton));
        			handled = true;
        			break; 
        			
        		case MainRecordButton:
        			if(PrvRecordFile() == errNone) {
        				FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainRecordButton));
        				FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStopRButton));
        			}
        			handled = true;
        			break; 
        			
        		case MainStopRButton:
        			PrvStopFile();
        			FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStopRButton));
        			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainRecordButton));
        			handled = true;
        			break;  	 			
        	}
        	break;
            
        case menuEvent:
			HandleMenu(eventP->data.menu.itemID);
			FrmDrawForm(frmP);
			handled = true;
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
	
	// Set the current file
	MemSet(gFilePath, 256, 0);
	
	// Open SndFileStream
	SndFileStreamOpenLibrary(gRefNum, err);
	
	// Get the current application path
	SysCurAppDatabase(&cardNo, &dbID);
	
	// Register our own event - Pass the mini info
	SysNotifyRegister(cardNo, dbID, sndFileStreamNotifyStreamComplete,
		NULL, sysNotifyNormalPriority, 0);

	// Quick no-auto off
	// gOldAutoOffTime = SysSetAutoOffTime(0);
	
Done:		
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
    
    // Stop whatever was running...
    PrvStopFile();
    
    // Close the library
	SndFileStreamCloseLibrary(gRefNum, err);
	
	// Unregister the notification	
	SysNotifyUnregister( cardNo, dbID, 
			sndFileStreamNotifyStreamComplete, sysNotifyNormalPriority );
    
    // Restore auto off
	// SysSetAutoOffTime(gOldAutoOffTime);
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
    SndFileStreamDataPtr dataP = NULL;
    FormType *frmP = NULL;
    char dbgMsg[80];

    error = RomVersionCompatible (ourMinVersion, launchFlags);
    if (error) 
    	return (error);

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
		if( notifyParamP == NULL ) 
			goto Done;
		
		switch(notifyParamP->notifyType) 
		{
		case sndFileStreamNotifyStreamComplete:	 
			
			StrPrintF(dbgMsg, "Notify in app: notifyDetailsP: %08lx\n",	notifyParamP->notifyDetailsP);
			DbgMessage(dbgMsg);
			
			PrvStopFile();	
			
			frmP = FrmGetActiveForm();
			dataP = (SndFileStreamDataPtr) notifyParamP->notifyDetailsP;
			
			if(dataP->streamMode == sndOutput)
			{
				FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStopPButton));
				FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainPlayButton));
			}
			else
			{
				FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStopRButton));
        		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainRecordButton));
			}
	
			break;
		}
        
    default:
        break;
    }

Done:
    return errNone;
}

#pragma warn_a5_access reset
