/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	RecordForm.c
 *
 * Description:
 *	Record form implementation.
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include <HsExt.h>
#include "Common.h"
#include "PalmSoundMgrExt.h"
#include "PalmSoundCustomCtrl.h"
#include "PlayForm.h"
#include "MyPlayer.h"
#include "MyPlayerRsc.h"
#include "Wave.h"

/***********************************************************************
 *
 * FUNCTION:    SndStreamCallback
 *
 * DESCRIPTION: Sound callback for recording.
 *
 * PARAMETERS:	userData	- Pointer to user data.
 *				streamRef	- Reference to the sound stream 
 *				bufferP		- Data buffer
 *				bufferSizeP	- Pointer to the data buffer size
 *
 * RETURNED:	errNone is the function succeeded.
 *
 ***********************************************************************/

static Err SndStreamCallback(void *userDataP, SndStreamRef streamRef,
							 void *bufferP, UInt32 *bufferSizeP)
{
	Err 		err 			= errNone;
	UInt32		dataWritten		= 0;
	PlayerInfo 	*myPlayerInfoP 	= (PlayerInfo *)userDataP;
	
	// We expect the user data to be valid
	if( myPlayerInfoP == NULL ) {
		err = -1;	// 
		goto Done;
	}
	
	// Save the actual buffer size
	myPlayerInfoP->actualSize = *bufferSizeP;
	
	// This is the input section. We are going to receive buffer continually
	// until the user presses the Stop button.
	// The main error to check is a memory error since the device and the card
	// don't have an unlimited amout of memory.
	if( myPlayerInfoP->fileType == VFS_File ) {
		err = VFSFileWrite(myPlayerInfoP->file.fileRef, *bufferSizeP, bufferP, &dataWritten);
	} else {
		dataWritten = FileWrite(myPlayerInfoP->file.fileHandle, bufferP, 1, *bufferSizeP, &err);
	}
	
	myPlayerInfoP->dataSize += dataWritten;
	*bufferSizeP = dataWritten;
	
Done:
	return err;
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    PrvOpenFile
 *
 * DESCRIPTION: Open a file
 *
 * PARAMETERS:	recording	- Trus if recording, false if playing
 *
 * RETURNED:	Ture if succeeded.
 *
 ***********************************************************************/ 

static Boolean PrvOpenFile()
{
	Err 	err = errNone;
	Char	fullFilePath[256] = { NULL };
	Boolean	result = true;
	
	// For VFS we need to recreate the full path of the file location
	if(gPlayerInfo.fileType == VFS_File)
	{
		// Create the full path name for the file
		StrPrintF(fullFilePath, "%s/%s", gPlayerInfo.directory, gPlayerInfo.fileName);
			
		// Open file for writing, create and truncate options
		err = VFSFileOpen(gPlayerInfo.volumeRef, fullFilePath, vfsModeWrite | vfsModeCreate | vfsModeTruncate, (FileRef *)&gPlayerInfo.file.fileRef);
	}
	// Memory file
	else
	{
		// Open file for writing, create and truncate options
		gPlayerInfo.file.fileHandle = FileOpen(0, gPlayerInfo.fileName, sysFileTFileStream, 0, fileModeReadWrite, &err);
	}
	
	// Check for errors
	if(err != errNone)
	{
		result = false;
		// Since the handle might not be null when
		// an error occurs always fix it.
		gPlayerInfo.file.fileHandle = NULL;
		
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error opening the file.", (Char*)gErrorCode, NULL);
	}

	return result;
}

/***********************************************************************
 *
 * FUNCTION:    PrvCloseFile
 *
 * DESCRIPTION: Close a file
 *
 * PARAMETERS:	-
 *
 * RETURNED:	Ture if succeeded.
 *
 ***********************************************************************/ 

static Boolean PrvCloseFile()
{
	Err 	err = errNone;
	Boolean	result = true;
	
	if(gPlayerInfo.fileType == VFS_File) {
		if(gPlayerInfo.file.fileRef)
			err = VFSFileClose(gPlayerInfo.file.fileRef);
	} else {
		if(gPlayerInfo.file.fileHandle)
			err = FileClose(gPlayerInfo.file.fileHandle);
	}
	
	if(err) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error closing the file.", (Char*)gErrorCode, NULL);
		result = false;
	}
	return result;

}


/***********************************************************************
 *
 * FUNCTION:    PrvStartStream
 *
 * DESCRIPTION: Called when record is starting.
 *				1) Open a file
 *				2) Create a stream
 *				3) Set the custom control(s)
 *				4) Write a pseudo wave header
 *				5) Start the stream
 *
 * PARAMETERS:	frmP	- Form pointer.
 *
 * RETURNED:	True if succeeded.
 *
 ***********************************************************************/

static Boolean PrvStartStream(FormType *frmP)
{
	Err 		err = errNone;
	Boolean		result = true;
	FieldType 	*fieldP = FrmGetPtr(frmP, RecordFileField);
	Char 		*fileName = FldGetTextPtr(fieldP);
	
	if( *fileName == NULL ) {
		err = -1;
		StrCopy(gErrorCode, "Error code: NULL Pointer");
		FrmCustomAlert(RecordErrorAlert, "File name cannot be null.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// Copy the current field to the file name
	StrCopy(gPlayerInfo.fileName, FldGetTextPtr(fieldP));
	
	// Read the buffer size
	fieldP = FrmGetPtr(frmP, RecordBufferField);
	gPlayerInfo.bufferSize = StrAToI(FldGetTextPtr(fieldP));
	gPlayerInfo.dataSize = 0;
		
	// Tell the callback the stream direction
	gPlayerInfo.streamMode = sndInput;
	
	// 1. Open a file for recording
	result = PrvOpenFile();
	if(!result)
		goto Done;
		
	// 2. Create the stream
	err = SndStreamCreateExtended(	&gPlayerInfo.streamRef,
									gPlayerInfo.streamMode,
									gPlayerInfo.streamFormat,
									gPlayerInfo.sampleRate,
									(gPlayerInfo.streamFormat==sndFormatIMA_ADPCM)?0:gPlayerInfo.sampleType,
									gPlayerInfo.streamWidth,
									&SndStreamCallback,
									&gPlayerInfo,
									gPlayerInfo.bufferSize,
									false);
									
	// Check for errors
	if(err != errNone) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error creating the stream.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// The volume
	err = SndStreamSetVolume(gPlayerInfo.streamRef, gPlayerInfo.streamVolume);
	if(err != errNone) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error setting the volume of the stream.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// 3. Custom Control
	if(gPlayerInfo.streamFormat==sndFormatIMA_ADPCM)
	{
		SndCodecCustomControlType ct;
		UInt16 blockAlign = 0;
		UInt32 numChannels = (gPlayerInfo.streamWidth == sndMono) ? 1 : 2;
		UInt16 blockSize = 256 * numChannels;
		
		switch(gPlayerInfo.sampleRate)
		{
		case 8000:
		case 11025:
		case 16000:
			break;
		case 22050:
			blockSize *= 2;
			break;
		case 32000:
		case 44100:
		    FrmAlert(FrequencyAlert);
		    err = 1;
		    goto Done;
		}
		
		blockAlign		= Swap16(blockSize);
		
		ct.apiCreator	= Swap32(codecDriverIMAADPCM);
		ct.apiSelector	= Swap32(codecIMAADPCMSetBlockSize);
		ct.valueP		= (void*)Swap32(&blockAlign);
		ct.valueLenP	= NULL;
		
		err = SndStreamDeviceControl(gPlayerInfo.streamRef, sndControlCodecCustomControl, &ct, sizeof(ct));
		if(err) {
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error setting the stream custom control.", (Char*)gErrorCode, NULL);
			goto Done;
		}
	}
	
	// 4. Reserve some space for the WAVE header
	if(gPlayerInfo.streamFormat==sndFormatIMA_ADPCM) {
		err = WriteADPCM_WAVEHeader(&gPlayerInfo);
	} else {
		err = WritePCM_WAVEHeader(&gPlayerInfo);
	}
	
	// 5. Start the stream
	err = SndStreamStart(gPlayerInfo.streamRef);
	if(err)
	{
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error while staring the stream.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
Done:
	if(err) {
		// Close the file
		PrvCloseFile();
		// Delete the stream is needed
		if(gPlayerInfo.streamRef)
			SndStreamDelete(gPlayerInfo.streamRef);
		result = false;
	}
	
	return result;
}

/***********************************************************************
 *
 * FUNCTION:    PrvStopStream
 *
 * DESCRIPTION: Stop the current stream and close the open file.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvStopStream()
{
	Err err = errNone;
	
	// Stop the stream
	err = SndStreamStop(gPlayerInfo.streamRef);
	if(err) {
		gPlayerInfo.playerState = IDLE_REC;
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error stopping the stream.", (Char*)gErrorCode, NULL);
	}
	// Delete the stream
	err = SndStreamDelete(gPlayerInfo.streamRef);
	if(err) {
		gPlayerInfo.playerState = IDLE_REC;
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(RecordErrorAlert, "Error deleting the stream.", (Char*)gErrorCode, NULL);
	}
	
	// Rewind the file and write the real header
	if(gPlayerInfo.fileType == VFS_File) {
		err = VFSFileSeek(gPlayerInfo.file.fileRef, vfsOriginBeginning, 0);
	} else {
		err = FileSeek(gPlayerInfo.file.fileHandle, 0, fileOriginBeginning);
	}
	
	// Write WAVE header
	if(gPlayerInfo.streamFormat==sndFormatIMA_ADPCM) {
		err = WriteADPCM_WAVEHeader(&gPlayerInfo);
	} else {
		err = WritePCM_WAVEHeader(&gPlayerInfo);
	}
	
	// Close the file
	PrvCloseFile();
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    PrvChangeBufferSizeField
 *
 * DESCRIPTION: Set the buffer size text field.
 *
 * PARAMETERS:	frmP	- Pointer to a form
 *				size	- The buffer size
 *
 * RETURNED:	-
 *
 ***********************************************************************/
/*
static void PrvChangeBufferSizeField(FormType *frmP, UInt32 size)
{
	static Char bufferSizeText[10] = { NULL };
	
	// Simply convert the size parameter to a string and display it
	FieldType *fieldP = FrmGetPtr(frmP, RecordBufferField);
	
	if( fieldP ) {
		StrIToA(bufferSizeText, size);
		SetFieldTextFromStr(fieldP, bufferSizeText, true);
	}
}
*/

/***********************************************************************
 *
 * FUNCTION:    PrvBlinkPause
 *
 * DESCRIPTION: Flash the current track time when in pause mode.
 *
 * PARAMETERS:  frmP	- Pointer to the current active form
 *
 * RETURNED:    -
 *
 ***********************************************************************/

static void PrvBlinkPause(FormType *frmP)
{
	UInt16 seconds, minutes;
	Char time[10] = { NULL };
	FieldType* fieldP = FrmGetPtr(frmP, RecordTimeField);
	
	// Blink only if enough time ellapsed
	if( (TimGetTicks() - gPreviousBlinkTime) > kBlinkTime)
	{
		gPreviousBlinkTime = TimGetTicks();
		
		// Two states: time on/timeoff
		if(gBlinkState)
		{
			seconds = gCurrentTime / sysTicksPerSecond;
			minutes = seconds / 60;
			seconds = seconds - (minutes * 60);
			StrPrintF(time, "%02d:%02d", minutes, seconds);
			SetFieldTextFromStr(fieldP, time, true);
		}
		else
		{
			SetFieldTextFromStr(fieldP, "", true);
		}
			
		gBlinkState = 1 - gBlinkState; // Switch flag
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvRefreshUI
 *
 * DESCRIPTION: Refresh the current time and actual buffer.
 *
 * PARAMETERS:  frmP	- Pointer to the current active form
 *
 * RETURNED:    -
 *
 ***********************************************************************/

static void PrvRefreshUI(FormType *frmP)
{
	UInt16 seconds, minutes;
	Char time[10] = { NULL };
	
	// Refresh the time
	FieldType* fieldP = FrmGetPtr(frmP, RecordTimeField);
	if(fieldP) {
		seconds = gCurrentTime / sysTicksPerSecond;
		minutes = seconds / 60;
		seconds = seconds - (minutes * 60);
		StrPrintF(time, "%02d:%02d", minutes, seconds);
		SetFieldTextFromStr(fieldP, time, true);
	}
	
	// Refresh the actual buffer size
	fieldP = FrmGetPtr(frmP, RecordActualField);
	if(fieldP) {
		if( gPlayerInfo.actualSize ) {
			StrIToA(time, gPlayerInfo.actualSize);
			SetFieldTextFromStr(fieldP, time, true);
		} else {
			SetFieldTextFromStr(fieldP, "-", true);
		}
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvSetButtonBitmaps
 *
 * DESCRIPTION: Changes the button bitmaps depending on player state.
 *
 * PARAMETERS:	frmP	- Form pointer.
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvSetButtonBitmaps(FormType *frmP)
{
	// Change the button icon
	ControlType* controlP = FrmGetPtr(frmP, RecordRecordGraphicButton);
	if( controlP )
	{
		switch(gPlayerInfo.playerState)
		{
		case IDLE_REC:
			CtlSetGraphics(controlP, RecordBitmapFamily, RecordSelectedBitmapFamily);
			break;
		case PAUSED_REC:
			CtlSetGraphics(controlP, RecordBitmapFamily, RecordSelectedBitmapFamily);
			break;
		case RECORDING:
			CtlSetGraphics(controlP, PauseBitmapFamily, PauseSelectedBitmapFamily);
			break;
		default:
			break;
		}
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvRecordButtonPressed
 *
 * DESCRIPTION: Handles the player states when record is pressed.
 *
 * PARAMETERS:	frmP	- Form pointer.
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvRecordButtonPressed(FormType *frmP)
{
	Err err = errNone;
	
	// State Machine for Record button
	switch(gPlayerInfo.playerState)
	{
	case IDLE_REC:
		if( PrvStartStream(frmP) )
		{
			gPlayerInfo.playerState = RECORDING;
			gStartTime = TimGetTicks();
			gCurrentTime = 0;
			gPreviousRun = 0;
		}
		break;
		
	case PAUSED_REC:
		// Resume the stream
		err = SndStreamPause(gPlayerInfo.streamRef, false);
		if(err) {
			gPlayerInfo.playerState = IDLE_REC;
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error resuming the stream.", (Char*)gErrorCode, NULL);
		} else {
			gPlayerInfo.playerState = RECORDING;
			gStartTime = TimGetTicks();
			gPreviousRun = gCurrentTime;
		}
		break;
		
	case RECORDING:
		// Pause the stream
		err = SndStreamPause(gPlayerInfo.streamRef, true);
		if(err) {
			gPlayerInfo.playerState = IDLE_REC;
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error pausing the stream.", (Char*)gErrorCode, NULL);
		} else {
			gPlayerInfo.playerState = PAUSED_REC;
			gBlinkState = 0;
		}
		break;
		
	default:
		gPlayerInfo.playerState = IDLE_REC;
		break;
	}

	// Quick refresh
	PrvRefreshUI(frmP);
	PrvSetButtonBitmaps(frmP);
}

/***********************************************************************
 *
 * FUNCTION:    PrvStopButtonPressed
 *
 * DESCRIPTION: Handles the player states when stop is pressed.
 *
 * PARAMETERS:	frmP	- Form pointer.
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvStopButtonPressed(FormType *frmP)
{
	// Reset some values
	gCurrentTime = 0;
	gPreviousRun = 0;
	gPlayerInfo.actualSize = 0;
		
	// State machine for Stop button	
	switch(gPlayerInfo.playerState)
	{
	case IDLE_REC:
		gPlayerInfo.playerState = IDLE_REC;
		break;
		
	case PAUSED_REC:
	case RECORDING:
		gPlayerInfo.playerState = IDLE_REC;
		PrvStopStream();
		// Change state
		FrmGotoForm(PlayForm);
		break;
		
	default:
		break;
	}

	// Quick refresh
	PrvRefreshUI(frmP);
	PrvSetButtonBitmaps(frmP);
}


/***********************************************************************
 *
 * FUNCTION:    PrvHandleNilEvent
 *
 * DESCRIPTION: Handles nil events. We will refresh the record/play time.
 *
 * PARAMETERS:	frmP	- Form pointer.
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvHandleNilEvent(FormType *frmP)
{
	switch(gPlayerInfo.playerState)
	{
	case IDLE_REC:
		break;
	case PAUSED_REC:
		// Blink the current time
		PrvBlinkPause(frmP);
		break;
	case RECORDING:
		// Refresh the time
		gCurrentTime = TimGetTicks() - gStartTime + gPreviousRun;
		PrvRefreshUI(frmP);
		break;
	default:
		break;
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvPlayerFormShowMenu
 *
 * DESCRIPTION: Display the player menu.
 *
 * PARAMETERS:  command		- Menu to handle
 *
 * RETURNED:    True if handled
 *
 ***********************************************************************/

static Boolean PrvPlayerFormShowMenu(UInt16 command)
{
	Boolean handled = false;
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appCreatorID, true, &cardNo, &dbID);
	
	switch (command)
	{
	case RecordOptionsAbout:	
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");		
		handled = true;
		break;
		
	case RecordOptionsPlay:
		FrmGotoForm(PlayForm);
		handled = true;
		break;
	}
	
	return handled;
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    RecordFormInit
 *
 * DESCRIPTION: Form initialiser.
 *
 * PARAMETERS:	frmP	- Pointer to a form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void RecordFormInit(FormType *frmP)
{
	FieldType *fieldP = NULL;
	ControlType *controlP = NULL;
	ListType *listP = NULL;
	UInt16 listPosition = 0;
	Char text[10];
	
	// Initialize the default file string
	if( !*gPlayerInfo.fileName )
		StrCopy(gPlayerInfo.fileName, "default.wav");

	fieldP = FrmGetPtr(frmP, RecordFileField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, gPlayerInfo.fileName, false);
		
	// Initialize the default buffer size
	StrIToA(text, gPlayerInfo.bufferSize);
	fieldP = FrmGetPtr(frmP, RecordBufferField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, text, false);
		
	// Initialize the actual buffer size
	fieldP = FrmGetPtr(frmP, RecordActualField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, "-", false);
		
	// Initialize the default time
	fieldP = FrmGetPtr(frmP, RecordTimeField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, "00:00", false);
		
	// Initialize the file location field (since we might come from the record form)
	controlP = FrmGetPtr(frmP, RecordLocationPopTrigger);
	listP = FrmGetPtr(frmP, RecordLocationList);
	if( controlP && listP ) {
		CtlSetLabel(controlP, LstGetSelectionText(listP, gPlayerInfo.fileType));
		LstSetSelection(listP, gPlayerInfo.fileType);
	}
	
	// Initialize the stream width (mono/stereo)
	controlP = FrmGetPtr(frmP, RecordModePopTrigger);
	listP = FrmGetPtr(frmP, RecordModeList);
	if( controlP && listP ) {
		listPosition = (gPlayerInfo.streamWidth==sndMono)?0:1;
		CtlSetLabel(controlP, LstGetSelectionText(listP, listPosition));
		LstSetSelection(listP, listPosition);
	}
	
	// Initialize the sample type
	controlP = FrmGetPtr(frmP, RecordSampleTypePopTrigger);
	listP = FrmGetPtr(frmP, RecordSampleTypeList); 
	if( controlP && listP ) {
		listPosition = (gPlayerInfo.sampleType==sndUInt8)?0:1;
		CtlSetLabel(controlP, LstGetSelectionText(listP, listPosition));
		LstSetSelection(listP, listPosition);
	}
	
	// Initialize the format
	controlP = FrmGetPtr(frmP, RecordEncodingPopTrigger);
	listP = FrmGetPtr(frmP, RecordEncodingList);
	if( controlP && listP ) {
		listPosition = (gPlayerInfo.streamFormat==sndFormatPCM)?0:1;
		CtlSetLabel(controlP, LstGetSelectionText(listP, listPosition));
		LstSetSelection(listP, listPosition);
	}
	
	// Initialize the sample rate
	controlP = FrmGetPtr(frmP, RecordSampleRatePopTrigger);
	listP = FrmGetPtr(frmP, RecordSampleRateList);
	if( controlP && listP ) {
		switch(gPlayerInfo.sampleRate) {
		case 8000:
			listPosition = 0;
			break;
		case 11025:
			listPosition = 1;
			break;
		case 16000:
			listPosition = 2;
			break;
		case 22050:
			listPosition = 3;
			break;
		case 32000:
			listPosition = 4;
			break;
		case 44100:
			listPosition = 5;
			break;
		}
		CtlSetLabel(controlP, LstGetSelectionText(listP, listPosition));
		LstSetSelection(listP, listPosition);
	}
	
	// Initialize the volume
	controlP = FrmGetPtr(frmP, RecordVolumeFeedbackSliderControl);
	if( controlP ) {
		UInt16 value = 0;
		UInt16 minValue = 0, maxValue = 0;
		CtlGetSliderValues(controlP, &minValue, &maxValue, NULL, NULL);
		value = (gPlayerInfo.streamVolume * (maxValue - minValue)) >> 11;
		CtlSetValue(controlP, value);
	}
		
	// Set the intial state
	gPlayerInfo.playerState = IDLE_REC;
}

/***********************************************************************
 *
 * FUNCTION:    RecordFormDeInit
 *
 * DESCRIPTION: Form de-initialiser.
 *
 * PARAMETERS:	frmP	- Pointer to a form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void RecordFormDeInit(FormType *frmP)
{	
	// Check the current player state
	switch(gPlayerInfo.playerState)
	{
	case PAUSED_REC:
	case RECORDING:
		PrvStopStream();	
		break;
	default:
		break;
	}
}

/***********************************************************************
 *
 * FUNCTION:    RecordFormHandleEvent
 *
 * DESCRIPTION: Form event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

Boolean RecordFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();
	
	switch(eventP->eType)
	{
		
		//
		// Display the menu
		//
        case menuEvent:
            handled = PrvPlayerFormShowMenu(eventP->data.menu.itemID);
            PrvRefreshUI(frmP);
			PrvSetButtonBitmaps(frmP);
            break;
                        
		//
		// Handle the form open events
		//
		case frmOpenEvent:
			RecordFormInit(frmP);
			FrmDrawForm(frmP);
			handled = true;
			break;
		
		//
		// Handle the form close event
		//
		case frmCloseEvent:
			RecordFormDeInit(frmP);
			break;
		
		//
		// Handle the control select events
		//
		case ctlSelectEvent:
			switch(eventP->data.ctlSelect.controlID)
			{
			case RecordRecordGraphicButton:
				PrvRecordButtonPressed(frmP);
				handled = true;
				break;
				
			case RecordStopGraphicButton:
				PrvStopButtonPressed(frmP);
				handled = true;
				break;

			default:
				break;
			}
			break;
		
		//
		// Handle the pop-up trigger event
		//	
		case popSelectEvent:
			switch(eventP->data.popSelect.controlID)
			{
			case RecordLocationPopTrigger:
				gPlayerInfo.fileType = gUIFileTypeTable[eventP->data.popSelect.selection];
				break;
			
			case RecordModePopTrigger:
				gPlayerInfo.streamWidth = gUIStreamModeTable[eventP->data.popSelect.selection];
				break;
				
			case RecordEncodingPopTrigger:
				gPlayerInfo.streamFormat = gUIStreamFormatTable[eventP->data.popSelect.selection];
				break;
				
			case RecordSampleTypePopTrigger:
				gPlayerInfo.sampleType = gUISampleTypeTable[eventP->data.popSelect.selection];
				break;
				
			case RecordSampleRatePopTrigger:
				gPlayerInfo.sampleRate = gUISampleRateTable[eventP->data.popSelect.selection];
				break;
				
			case RecordFormPopTrigger:
				if( eventP->data.popSelect.selection )
					FrmGotoForm(PlayForm);
				break;
				
			default:
				break;
			}
			break;	
				
		//
		// Handle control repeat event
		//
		case ctlRepeatEvent:
			switch(eventP->data.ctlRepeat.controlID)
			{

				case RecordVolumeFeedbackSliderControl:
				{	
					UInt16 minValue = 0, maxValue = 0;
					CtlGetSliderValues(eventP->data.ctlRepeat.pControl, &minValue, &maxValue, NULL, NULL);
					gPlayerInfo.streamVolume = ((UInt32)eventP->data.ctlRepeat.value << 11) / (maxValue - minValue);
					if( gPlayerInfo.streamRef ) {
						SndStreamSetVolume(gPlayerInfo.streamRef, gPlayerInfo.streamVolume);
					}
					break;
				}
			}
			break;
			
		//
		// Handle NilEvent
		// 
		case nilEvent:
			PrvHandleNilEvent(frmP);
			handled = true;
			break;
				
		default:
			break;
	}
	
	return handled;
}
