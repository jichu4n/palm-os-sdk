/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	PlayForm.c
 *
 * Description:
 *	This file manages teh Player form.
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include <HsExt.h>
#include "Common.h"
#include "Wave.h"
#include "PalmSoundMgrExt.h"
#include "PalmSoundCustomCtrl.h"
#include "PlayForm.h"
#include "MyPlayer.h"
#include "MyPlayerRsc.h"

// The only globals for the special sample rate
static UInt32 gUISampleRate = 8000;
static Boolean gUseSampleRate = false;

/***********************************************************************
 *
 * FUNCTION:    SndStreamCallback
 *
 * DESCRIPTION: Sound callback for playback.
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
	UInt32		dataRead		= 0;
	PlayerInfo 	*myPlayerInfoP 	= (PlayerInfo *)userDataP;
	
	// We expect the user data to be valid
	if( myPlayerInfoP == NULL ) {
		err = -1;	// 
		goto Done;
	}
	
	// Save the actual buffer size
	myPlayerInfoP->actualSize = *bufferSizeP;
	
	// This is the output section. We are sending buffers to the sound thread until
	// we read the complete audio file,
	// A FileRead generates an error when the end of file is reached
	// which we want to avoid since there might be some data not played yet.
	if( myPlayerInfoP->fileType == VFS_File ) {
		err = VFSFileRead(myPlayerInfoP->file.fileRef, *bufferSizeP, bufferP, &dataRead);
	} else {
		dataRead = FileRead(myPlayerInfoP->file.fileHandle, bufferP, 1, *bufferSizeP, &err);
	}
	
	*bufferSizeP = dataRead;
	
	////////////////////////////////////////////////////////////
	// Q: What to do when done reading the file?
	// A: Multiple solutions. Some of them:
	//	1.	When dataRead < bufferSize, fill buffer with silence,
	//		then a) Send an Alarm or b) Send a Notification
	//		=> 	Up to the UI to stop the stream, might leave enough time
	//			for the stream to flush.
	//	2.	When dataRead = 0, stop the stream,
	//		then a) Send an Alarm or b) Send a Notification
	//		=> 	Simple.
	//	3.	When dataRead < bufferSize, fill 2 or 3 buffers with silence,
	//		then stop the stream and a) Send an Alarm or b) Send a Notification
	//		=> 	As 1, but the stream handles it's own stops
	//	4.	When done, use function pointers to inform the player the file
	//		has been played completely.
	//		=> Faster feedback.
	//	5.	Your technique....
	//		=>	Better ? :-)
	//////////////////////////////////////////////////////////
			
	// Also, stop the stream if we're done
	if( dataRead == 0 ) {
		
		SysNotifyParamType notifyParam;
		PlayerNotification notifyDetail;
		
		// Stop the stream
		err = SndStreamStop(streamRef);
		
		// Set the notification detials
		notifyDetail.notificationType = DONE_PLAYING;
		notifyDetail.playerInfoP = myPlayerInfoP;
		
		// Set the notification info
		notifyParam.notifyType = appCreatorID;
		notifyParam.broadcaster = appCreatorID;
		notifyParam.handled = false;
		notifyParam.notifyDetailsP = &notifyDetail;
		
		// Send a notification to turn the screen off
		err = SysNotifyBroadcastDeferred(&notifyParam, sizeof(PlayerNotification));
	}
	
	// Check for the end of file
	if( err == vfsErrFileEOF || err == fileErrEOF ) {
		if( dataRead > 0 )
			err = errNone;
	}

Done:
	// In case of an error, we should inform the player about it.
	// We could have the same notification as above but with a different notification type.
	// This is sample code... :)
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
			
		// Open file for reading
		err = VFSFileOpen(gPlayerInfo.volumeRef, fullFilePath, vfsModeRead, (FileRef *)&gPlayerInfo.file.fileRef);
	}
	// Memory file
	else
	{
		// Open file for reading
		gPlayerInfo.file.fileHandle = FileOpen(0, gPlayerInfo.fileName, sysFileTFileStream, 0, fileModeReadOnly, &err);
	}
	
	// Check for errors
	if(err)
	{
		result = false;
		// Since the handle might not be null when
		// an error occurs always fix it.
		gPlayerInfo.file.fileHandle = NULL;
		
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(PlayErrorAlert, "Error opening the file.", (Char*)gErrorCode, NULL);
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
		FrmCustomAlert(PlayErrorAlert, "Error closing the file.", (Char*)gErrorCode, NULL);
		result = false;
	}
	return result;

}

/***********************************************************************
 *
 * FUNCTION:    PrvDeleteFile
 *
 * DESCRIPTION: Delete a file
 *
 * PARAMETERS:	-
 *
 * RETURNED:	Ture if succeeded.
 *
 ***********************************************************************/ 

static Boolean PrvDeleteFile()
{
	Err 	err = errNone;
	Char	fullFilePath[256] = { NULL };
	Boolean	result = true;
	
	if(gPlayerInfo.fileType == VFS_File)
	{
		// Create the full path name for the file
		StrPrintF(fullFilePath, "%s/%s", gPlayerInfo.directory, gPlayerInfo.fileName);
		
		err = VFSFileDelete(gPlayerInfo.volumeRef, fullFilePath);
	}
	else
	{
		err = FileDelete(0, gPlayerInfo.fileName);
	}
	
	if(err) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(PlayErrorAlert, "Error deleting the file.", (Char*)gErrorCode, NULL);
		result = false;
	}
	return result;
}

/***********************************************************************
 *
 * FUNCTION:    PrvStartStream
 *
 * DESCRIPTION: Function called when user press play.
 *				1) Open a file
 *				2) Parse the wave header
 *				3) Create the sound stream
 *				4) Set the custom control(s)
 *				5) Start the stream
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	True if succeeded.
 *
 ***********************************************************************/

static Boolean PrvStartStream(FormType *frmP)
{
	Err 		err = errNone;
	Boolean		result = true;
	Char 		fileInfoText[128] = {NULL};
	UInt32 		sampleRate = 0;
	UInt16  	bps = 0;
	FieldType 	*fieldP = FrmGetPtr(frmP, PlayFileField);
	Char 		*fileName = FldGetTextPtr(fieldP);
	
	if( *fileName == NULL ) {
		err = -1;
		StrPrintF(gErrorCode, "Error code: NULL Pointer", err);
		FrmCustomAlert(PlayErrorAlert, "File name cannot be null.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// Copy the current field to the file name
	StrCopy(gPlayerInfo.fileName, fileName);
	
	// Read the buffer size
	fieldP = FrmGetPtr(frmP, PlayBufferField);
	gPlayerInfo.bufferSize = StrAToI(FldGetTextPtr(fieldP));
	
	// Tell the callback the stream direction
	gPlayerInfo.streamMode = sndOutput;
	
	// 1. Open a file for recording
	result = PrvOpenFile();
	if(!result)
		goto Done;
		
	// 2. Parse the wave file
	err = ReadWAVEHeader(&gPlayerInfo);
	if(err) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(PlayErrorAlert, "Error parsing the WAVE file.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// Select the sample Rate to use
	sampleRate = (gUseSampleRate)?gUISampleRate:gPlayerInfo.sampleRate;
	bps = (gPlayerInfo.streamFormat==sndFormatIMA_ADPCM)?4:(gPlayerInfo.sampleType==sndUInt8)?8:16;
	
	// Display the wav information
	StrPrintF(fileInfoText, "%d bps - %ld Hz - %s", bps, gPlayerInfo.sampleRate, (gPlayerInfo.streamWidth==sndMono)?"Mono":"Stereo");
	fieldP = FrmGetPtr(frmP, PlayFileInfoField);
	SetFieldTextFromStr(fieldP, fileInfoText, true);
		
	// 3. Create the stream
	err = SndStreamCreateExtended(	&gPlayerInfo.streamRef,
									gPlayerInfo.streamMode,
									gPlayerInfo.streamFormat,
									sampleRate,
									(gPlayerInfo.streamFormat==sndFormatIMA_ADPCM)?0:gPlayerInfo.sampleType,
									gPlayerInfo.streamWidth,
									&SndStreamCallback,
									&gPlayerInfo,
									gPlayerInfo.bufferSize,
									false);
									
	// Check for errors
	if(err != errNone) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(PlayErrorAlert, "Error creating the stream.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// The volume
	err = SndStreamSetVolume(gPlayerInfo.streamRef, gPlayerInfo.streamVolume);
	if(err != errNone) {
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(PlayErrorAlert, "Error setting the volume of the stream.", (Char*)gErrorCode, NULL);
		goto Done;
	}
	
	// 4. Custom Control
	if(gPlayerInfo.streamFormat==sndFormatIMA_ADPCM)
	{
		SndCodecCustomControlType ct;
		UInt16 blockAlign = 0;
	
		blockAlign		= Swap16(gPlayerInfo.blockAlign);
		
		ct.apiCreator	= Swap32(codecDriverIMAADPCM);
		ct.apiSelector	= Swap32(codecIMAADPCMSetBlockSize);
		ct.valueP		= (void*)Swap32(&blockAlign);
		ct.valueLenP	= NULL;
		
		err = SndStreamDeviceControl(gPlayerInfo.streamRef, sndControlCodecCustomControl, &ct, sizeof(ct));
		if(err) {
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(PlayErrorAlert, "Error setting the stream custom control.", (Char*)gErrorCode, NULL);
			goto Done;
		}
	}
		
	// 5. Start the stream
	err = SndStreamStart(gPlayerInfo.streamRef);
	if(err)
	{
		StrPrintF(gErrorCode, "Error code: %x", err);
		FrmCustomAlert(PlayErrorAlert, "Error while staring the stream.", (Char*)gErrorCode, NULL);
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
	FieldType *fieldP = FrmGetPtr(frmP, PlayBufferField);
	
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
	FieldType* fieldP = FrmGetPtr(frmP, PlayTimeField);
	
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
	FieldType* fieldP = FrmGetPtr(frmP, PlayTimeField);
	if(fieldP) {
		seconds = gCurrentTime / sysTicksPerSecond;
		minutes = seconds / 60;
		seconds = seconds - (minutes * 60);
		StrPrintF(time, "%02d:%02d", minutes, seconds);
		SetFieldTextFromStr(fieldP, time, true);
	}
	
	// Refresh the actual buffer size
	fieldP = FrmGetPtr(frmP, PlayActualField);
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
 * DESCRIPTION: Changes the button bitmaps depending on the current state.
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvSetButtonBitmaps(FormType *frmP)
{
	// Change the button icon
	ControlType* controlP = FrmGetPtr(frmP, PlayPlayGraphicButton);
	if( controlP )
	{
		switch(gPlayerInfo.playerState)
		{
		case IDLE_PLAY:
			CtlSetGraphics(controlP, PlayBitmapFamily, PlaySelectedBitmapFamily);
			break;
		case PAUSED_PLAY:
			CtlSetGraphics(controlP, PlayBitmapFamily, PlaySelectedBitmapFamily);
			break;
		case PLAYING:
			CtlSetGraphics(controlP, PauseBitmapFamily, PauseSelectedBitmapFamily);
			break;
		default:
			break;
		}
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvPlayButtonPressed
 *
 * DESCRIPTION: Handles the player states when Play is pressed.
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvPlayButtonPressed(FormType *frmP)
{
	Err err = errNone;
	
	// State Machine for Play button
	switch(gPlayerInfo.playerState)
	{
	case IDLE_PLAY:
		if( PrvStartStream(frmP) )
		{
			gPlayerInfo.playerState = PLAYING;
			gStartTime = TimGetTicks();
			gCurrentTime = 0;
			gPreviousRun = 0;
		}
		break;
		
	case PAUSED_PLAY:
		// Resume the stream
		err = SndStreamPause(gPlayerInfo.streamRef, false);
		if(err) {
			gPlayerInfo.playerState = IDLE_PLAY;
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error resuming the stream.", (Char*)gErrorCode, NULL);
		} else {
			gPlayerInfo.playerState = PLAYING;
			gStartTime = TimGetTicks();
			gPreviousRun = gCurrentTime;
		}
		break;
		
	case PLAYING:
		// Pause the stream
		err = SndStreamPause(gPlayerInfo.streamRef, true);
		if(err) {
			gPlayerInfo.playerState = IDLE_PLAY;
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error pausing the stream.", (Char*)gErrorCode, NULL);
		} else {
			gPlayerInfo.playerState = PAUSED_PLAY;
			gBlinkState = 0;
		}
		break;
		
	default:
		gPlayerInfo.playerState = IDLE_PLAY;
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
 * DESCRIPTION: Handles the player states when Stop is pressed.
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvStopButtonPressed(FormType *frmP)
{
	Err err = errNone;
	
	// Reset some values
	gCurrentTime = 0;
	gPreviousRun = 0;
	gPlayerInfo.actualSize = 0;
		
	// State machine for Stop button	
	switch(gPlayerInfo.playerState)
	{
	case IDLE_PLAY:
		gPlayerInfo.playerState = IDLE_PLAY;
		break;
		
	case PAUSED_PLAY:
	case PLAYING:
		gPlayerInfo.playerState = IDLE_PLAY;
			
		// Stop the stream
		err = SndStreamStop(gPlayerInfo.streamRef);
		if(err) {
			gPlayerInfo.playerState = IDLE_PLAY;
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error stopping the stream.", (Char*)gErrorCode, NULL);
		}
		// Delete the stream
		err = SndStreamDelete(gPlayerInfo.streamRef);
		if(err) {
			gPlayerInfo.playerState = IDLE_PLAY;
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error deleting the stream.", (Char*)gErrorCode, NULL);
		}
		// Close the file
		PrvCloseFile();		
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
 * FUNCTION:    PrvNewButtonPressed
 *
 * DESCRIPTION: Called when the user presses the new button.
 *				Switch to the recoring form.
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvNewButtonPressed(FormType *frmP)
{
	FrmGotoForm(RecordForm);
}

/***********************************************************************
 *
 * FUNCTION:    PrvDeleteButtonPressed
 *
 * DESCRIPTION: Called when the user presses the delete button.
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvDeleteButtonPressed(FormType *frmP)
{
	UInt16 buttonID = FrmAlert(DeleteFileAlert);
	
	if(buttonID == 0) {
		PrvDeleteFile();
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvHandleNilEvent
 *
 * DESCRIPTION: Handles nil events. We will refresh the record/play time.
 *
 * PARAMETERS:	frmP	- Form pointer
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvHandleNilEvent(FormType *frmP)
{
	switch(gPlayerInfo.playerState)
	{
	case IDLE_PLAY:
		break;
	case PAUSED_PLAY:
		// Blink the current time
		PrvBlinkPause(frmP);
		break;
	case PLAYING:
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
	FormType *frmP = NULL;
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appCreatorID, true, &cardNo, &dbID);
	
	switch (command)
	{
	case PlayOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");	
		handled = true;
		break;
	
	case PlayOptionsDelete:
		frmP = FrmGetActiveForm();
		PrvDeleteButtonPressed(frmP);
		handled = true;
		break;
		
	case PlayOptionsRecord:
		FrmGotoForm(RecordForm);
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
 * FUNCTION:    PlayFormInit
 *
 * DESCRIPTION: Form initialiser.
 *
 * PARAMETERS:	frmP	- Pointer to a form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PlayFormInit(FormType *frmP)
{
	FieldType *fieldP = NULL;
	ControlType *controlP = FrmGetPtr(frmP, PlayLocationPopTrigger);
	ListType *listP = FrmGetPtr(frmP, PlayLocationList);
	Char text[10];
	
	// Initialize the default file string
	if( !*gPlayerInfo.fileName )
		StrCopy(gPlayerInfo.fileName, "default.wav");
		
	fieldP = FrmGetPtr(frmP, PlayFileField);
	if( fieldP )
			SetFieldTextFromStr(fieldP, gPlayerInfo.fileName, false);
		
	// Initialize the default buffer size
	StrIToA(text, gPlayerInfo.bufferSize);
	fieldP = FrmGetPtr(frmP, PlayBufferField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, "0", false);
		
	// Initialize the actual buffer size
	fieldP = FrmGetPtr(frmP, PlayActualField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, "-", false);
		
	// Initialize the default time
	fieldP = FrmGetPtr(frmP, PlayTimeField);
	if( fieldP )
		SetFieldTextFromStr(fieldP, "00:00", false);
		
	// Initialize the file location field (since we might come from the record form)
	if( controlP && listP ) {
		CtlSetLabel(controlP, LstGetSelectionText(listP, gPlayerInfo.fileType));
		LstSetSelection(listP, gPlayerInfo.fileType);
	}
	
	// Initialize the default time
	controlP = FrmGetPtr(frmP, PlayUseSampleRateCheckbox);
	if( controlP )
		CtlSetValue(controlP, gUseSampleRate);
	
	// Initialize the sample rate
	controlP = FrmGetPtr(frmP, PlaySampleRatePopTrigger);
	listP = FrmGetPtr(frmP, PlaySampleRateList);
	if( controlP && listP ) {
		UInt16 listPosition = 0;
		switch(gUISampleRate) {
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
	controlP = FrmGetPtr(frmP, PlayVolumeFeedbackSliderControl);
	if( controlP ) {
		UInt16 value = 0;
		UInt16 minValue = 0, maxValue = 0;
		CtlGetSliderValues(controlP, &minValue, &maxValue, NULL, NULL);
		value = (gPlayerInfo.streamVolume * (maxValue - minValue)) >> 11;
		CtlSetValue(controlP, value);
	}
		
	// Set the initial state
	gPlayerInfo.playerState = IDLE_PLAY;
}

/***********************************************************************
 *
 * FUNCTION:    PlayFormDeInit
 *
 * DESCRIPTION: Form de-initialiser.
 *
 * PARAMETERS:	frmP	- Pointer to a form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PlayFormDeInit(FormType *frmP)
{
	Err err = errNone;
	
	// Check the current player state
	switch(gPlayerInfo.playerState)
	{
	case PAUSED_PLAY:
	case PLAYING:
		// Stop the stream
		err = SndStreamStop(gPlayerInfo.streamRef);
		if(err) {
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error stopping the stream.", (Char*)gErrorCode, NULL);
		}
		// Delete the stream
		err = SndStreamDelete(gPlayerInfo.streamRef);
		if(err) {
			StrPrintF(gErrorCode, "Error code: %x", err);
			FrmCustomAlert(RecordErrorAlert, "Error deleting the stream.", (Char*)gErrorCode, NULL);
		}
		// Close the file
		PrvCloseFile();		
		break;
	default:
		break;
	}
}

/***********************************************************************
 *
 * FUNCTION:    PlayFormHandleEvent
 *
 * DESCRIPTION: Form event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

Boolean PlayFormHandleEvent(EventType *eventP)
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
		// Handle the form open event
		//
		case frmOpenEvent:
			PlayFormInit(frmP);
			FrmDrawForm(frmP);
			handled = true;
			break;
			
		//
		// Handle the form close event
		//
		case frmCloseEvent:
			PlayFormDeInit(frmP);
			break;
		
		//
		// Handle the control select events
		//
		case ctlSelectEvent:
			switch(eventP->data.ctlSelect.controlID)
			{
			case PlayPlayGraphicButton:
				PrvPlayButtonPressed(frmP);
				handled = true;
				break;
				
			case PlayStopGraphicButton:
				PrvStopButtonPressed(frmP);
				handled = true;
				break;
			case PlayNewGraphicButton:
				PrvNewButtonPressed(frmP);
				handled = true;
				break;
				
			case PlayDeleteGraphicButton:
				PrvDeleteButtonPressed(frmP);
				handled = true;
				break;
				
			case PlayUseSampleRateCheckbox:
				gUseSampleRate = eventP->data.ctlSelect.on;
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
			case PlayLocationPopTrigger:
				gPlayerInfo.fileType = gUIFileTypeTable[eventP->data.popSelect.selection];
				break;
				
			case PlaySampleRatePopTrigger:
				gUISampleRate = gUISampleRateTable[eventP->data.popSelect.selection];
				break;
				
			case PlayFormPopTrigger:
				if( eventP->data.popSelect.selection )
					FrmGotoForm(RecordForm);
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
				case PlayVolumeFeedbackSliderControl:
				{
					UInt16 minValue = 0, maxValue = 0;
					CtlGetSliderValues(eventP->data.ctlRepeat.pControl, &minValue, &maxValue, NULL, NULL);
					gPlayerInfo.streamVolume = ((UInt32)eventP->data.ctlRepeat.value << 11) / (maxValue - minValue);
					if( gPlayerInfo.streamRef ) {
						SndStreamSetVolume(gPlayerInfo.streamRef, gPlayerInfo.streamVolume);
					}
					break;
				}
				break;
			}
			
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

/***********************************************************************
 *
 * FUNCTION:    PlayStreamEventNotify
 *
 * DESCRIPTION:	Our player own notification system. Used from the callback
 *				to stop the timer and change the player state.
 *
 * PARAMETERS:	notifyParamP	- Notification parameters
 *
 * RETURNED:    Error code.
 *
 ***********************************************************************/

Err PlayStreamEventNotify(SysNotifyParamType *notifyParamP)
{
	Err err = errNone;
	FormType *frmP = FrmGetActiveForm();
	PlayerNotification* detailsP = (PlayerNotification*)(notifyParamP->notifyDetailsP);
	
	// Check the notification type
	switch( detailsP->notificationType ) {
		case DONE_PLAYING:
			// Reset some values
			gCurrentTime = 0;
			gPreviousRun = 0;
			gPlayerInfo.actualSize = 0;
			// Delete the stream
			err = SndStreamDelete(gPlayerInfo.streamRef);
			if(err) {
				gPlayerInfo.playerState = IDLE_PLAY;
				StrPrintF(gErrorCode, "Error code: %x", err);
				FrmCustomAlert(RecordErrorAlert, "Error deleting the stream.", (Char*)gErrorCode, NULL);
			}
			// Close the file
			PrvCloseFile();
			// Change state
			gPlayerInfo.playerState = IDLE_PLAY;
			// Quick refresh
			if( FrmGetActiveFormID() == PlayForm ) {
				PrvRefreshUI(frmP);
				PrvSetButtonBitmaps(frmP);
			}
			break;
		
		default:
			break;
	}
	
	// We handle the notification
	notifyParamP->handled = true;
	
	return errNone;
}

