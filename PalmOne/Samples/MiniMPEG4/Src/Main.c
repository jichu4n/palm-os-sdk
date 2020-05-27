/*
 * MiniMPEG4.c
 *
 * Main file for MPEG4
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include <MemGlue.h>
#include <HsNav.h>
#include "Common.h"
#include "MiniMPEG4.h"
#include "MiniMPEG4Rsc.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"
#include "PalmOneCodecPluginMgr.h"
#include "PalmOneCodecFormat.h"
#include "ColorButtonGadget.h"
#include "PalmLcdOverlay.h"
#include "PalmOneCamera.h"

#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#define Allocate(n)	MemPtrNew(n)

#define DATA_BUFFER_SIZE	16384
#define USE_RATE_CONTROL	1

/** Holds basic encoder/decoder info. */
typedef struct {
	Char	inputFileName[256];
	Char	outputFileName[256];
	
	FileRef inputFileRef;
	FileRef outputFileRef;
	
	UInt32 	width;
	UInt32 	height;
	UInt32 	frameRate;
	UInt32 	bitRate;
	UInt32 	keyFrame;
	UInt32 	IQuantization;
	UInt32	searchRange;
	
	UInt16 	codecMgrLibRefNum;
	PalmCodecSession session;
	UInt32 imageFormat;

	Boolean loadFileInMemory;
	Boolean outputToSD;
	Boolean encoding;
	Boolean	loop;
	
} MPEG4Info;

#if 0
#pragma mark Globals
#endif

/** Internal Globals. */
static MPEG4Info 	gMPEG4Info;
static UInt32		gFrameCount	= 0;
static UInt32		gStartTicks	= 0;
static UInt32		gLastTick	= 0;
static UInt32		gRateControl= 0;

static Char			*gGOButtonLabel[] = { "Go", "Stop" };
static Char			gLoadingText[] = "Loading";

static UInt32	gScreenDepth = 16;
static UInt32	gPreviousScreenDepth = 0;

RGBColorType	gStartColor = { 0, 255, 255, 255 };
RGBColorType	gEndColor = { 0, 0, 0, 255 };

static const RGBColorType	gWhite = { 0, 255, 255, 255 };
static const RGBColorType	gBlack = { 0, 0, 0, 0 };

// Extra
static UInt16	gLCDOverlayRefNum = sysInvalidRefNum;
static Boolean	gUseOverlay = true;
//static UInt16	gCameraRefNum = sysInvalidRefNum;

#if 0
#pragma mark -
#endif


/***********************************************************************
 *
 * FUNCTION:    		HandleMenu
 *
 * DESCRIPTION: 		Deals with menu commands issued
 *
 * PARAMETERS:  		itemID -> ID of the Menu which has been selected
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
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


/**
 * Read and Encode the parameters
 */
static Err PrvReadParameters(Boolean encoding)
{
	Err err = errNone;
	FieldType *fldP = NULL;
	FormType *frmP = FrmGetActiveForm();
	
	// Get the width & height
	fldP = FrmGetPtr(frmP, MainWidthField);
	if(FldGetTextPtr(fldP))
		gMPEG4Info.width = StrAToI(FldGetTextPtr(fldP));
	fldP = FrmGetPtr(frmP, MainHeightField);
	if(FldGetTextPtr(fldP))
		gMPEG4Info.height = StrAToI(FldGetTextPtr(fldP));
		
	// Read the file names
	
	fldP = FrmGetPtr(frmP, MainInputField);
	if (FldGetTextPtr(fldP))
		StrCopy(gMPEG4Info.inputFileName, FldGetTextPtr(fldP));
	else
		StrCopy(gMPEG4Info.inputFileName, "");
	fldP = FrmGetPtr(frmP, MainOutputField);
	
		StrCopy(gMPEG4Info.outputFileName, FldGetTextPtr(fldP));
	// Get the SearchRange & BitRate
	fldP = FrmGetPtr(frmP, MainSearchRangeField);
	if(FldGetTextPtr(fldP))
		gMPEG4Info.searchRange = StrAToI(FldGetTextPtr(fldP));
	fldP = FrmGetPtr(frmP, MainBitRateField);
	if(FldGetTextPtr(fldP))
		gMPEG4Info.bitRate = StrAToI(FldGetTextPtr(fldP));
		
	// Get the KeyFrame & IQuantization
	fldP = FrmGetPtr(frmP, MainKeyFrameField);
	if(FldGetTextPtr(fldP))
		gMPEG4Info.keyFrame = StrAToI(FldGetTextPtr(fldP));
	fldP = FrmGetPtr(frmP, MainIQuantizationField);
	if(FldGetTextPtr(fldP))
		gMPEG4Info.IQuantization = StrAToI(FldGetTextPtr(fldP));
	
//Done:	
	return err;
}

/**
 * Open input file
 */
static Err PrvFileOpenInput()
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	
	// Get first card
	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		goto Done;
	}
	
	// If we have a file path, open the file
	if(gMPEG4Info.inputFileName[0]!=NULL)
	{
		err = VFSFileOpen(firstVolume, gMPEG4Info.inputFileName, vfsModeRead, &gMPEG4Info.inputFileRef);
		if( err ) goto Done;
	}

Done:	
	return err;
}

/**
 * Open output file
 */
static Err PrvFileOpenOutput()
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	
	// Get first card
	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		goto Done;
	}
	
	// If we have a file path, open the file
	if(gMPEG4Info.outputFileName[0]!=NULL)
	{
		err = VFSFileOpen(firstVolume, gMPEG4Info.outputFileName, vfsModeWrite|vfsModeCreate|vfsModeTruncate, &gMPEG4Info.outputFileRef);
		if( err ) goto Done;
	}

Done:	
	return err;
}

/**
 * File info callback.
 * This function is called by the file browser
 */
static void PrvGetInputFileInfoCallback(UInt16 volume, const Char* path, const Char* file)
{
	// Create the full path name
	StrCopy(gMPEG4Info.inputFileName, path);

	// Internally, we expect full path
	StrCat(gMPEG4Info.inputFileName, file);
}

/**
 * Load a file in memory.
 * Draws a fill rectangle with specified colors.
 *
 */
static UInt32 PrvLoadFileInMemory(FileRef srcFile, UInt32 inputBufferSize, UInt8 *fileBufferP, Boolean useFtr)
{
	UInt32 read = 0;
	UInt32 total = 0;
	UInt32 current = 0;
	UInt32 xPosition = 80 - FntCharsWidth(gLoadingText, StrLen(gLoadingText)) / 2;
	RGBColorType oldColor;
	EventType event;
	UInt8 *tempBufferP = NULL;
	RectangleType goButtonBounds;
	WinDrawOperation oldOperation;
	UInt32 toRead = inputBufferSize / 100;
	RectangleType rect = { { 30, 147 }, { 1, 10 } };
	RGBColorType color = { 0, 0, 0, 0 };
	Int32 R, G, B;
	Int32 sR, sG, sB;
	Err err=errNone;
	
	
	// FtrPtrNew - Allocate temporary buffer
	if(useFtr) {
		tempBufferP = MemPtrNew(4096);
	}
	
	// Save old color
	WinSetBackColorRGB(&color, &oldColor);
	
	R = (UInt32)gStartColor.r << 8; G = (UInt32)gStartColor.g << 8; B = (UInt32)gStartColor.b << 8;
	
	// Color steps
	sR = (((Int32)gEndColor.r - (Int32)gStartColor.r) << 8) / 101;
	sG = (((Int32)gEndColor.g - (Int32)gStartColor.g) << 8) / 101;
	sB = (((Int32)gEndColor.b - (Int32)gStartColor.b) << 8) / 101;
	
	// Get the bounds for the Go button
	FrmGetObjectBounds(FrmGetActiveForm(), FrmGetObjectIndex(FrmGetActiveForm(),MainGoButton), &goButtonBounds);
	
	while(1)
	{
		EvtGetEvent(&event, 0);
		if(event.eType == penDownEvent && RctPtInRectangle(event.screenX, event.screenY, &goButtonBounds)) {
			total = 0;
			goto Done;
		}
			
		if(useFtr) {
			err=VFSFileRead(srcFile, 4096, tempBufferP, &read);
			DmWrite(fileBufferP, total, tempBufferP, read);
		} else {
			err=VFSFileRead(srcFile, toRead, fileBufferP, &read);
			fileBufferP += read;
		}

		if(err!=errNone) break;
		
		if(!read)
			break;
		
		total += read;
	
		current = (total * 100) / inputBufferSize + 31;
		
		if( current > rect.topLeft.x)
		{
			R += sR; G += sG; B += sB;
			if(R < 0) R = 0; if(R > 65536) R = 65536;
			if(G < 0) G = 0; if(G > 65536) G = 65536;
			if(B < 0) B = 0; if(B > 65536) B = 65536;
			color.r = R >> 8;
			color.g = G >> 8;
			color.b = B >> 8;
					
			WinSetBackColorRGB(&color, NULL);
			WinFillRectangle(&rect, 0);
			oldOperation = WinSetDrawMode(winOverlay);
			WinPaintChars(gLoadingText, StrLen(gLoadingText), xPosition, 146);
			WinSetDrawMode(oldOperation);		
			rect.topLeft.x = current;
		}
	}

Done:
	if(tempBufferP)
		MemPtrFree(tempBufferP);
	WinSetBackColorRGB(&oldColor, NULL);
	return total;
}

/**
 * Update the current UI
 * Displays the frmae count, the average FPS and the current FPS
 */
static void PrvUpdateUI()
{
	Char text[32];
	FieldType *fldP = NULL;
	UInt32 curTicks =  TimGetTicks();
	UInt32 fps = curTicks - gStartTicks;
	
	if(fps)
		fps = (sysTicksPerSecond * gFrameCount) / fps;
	
	// Average FPS
	StrIToA(text, fps);
	fldP = FrmGetPtr(FrmGetActiveForm(), MainAverageFPSField);
	SetFieldTextFromStr(fldP, text, true);
	
	// Current FPS
	fps = ( curTicks - gLastTick );
	if(fps)
		fps = sysTicksPerSecond / fps;
	
	StrIToA(text, fps);
	fldP = FrmGetPtr(FrmGetActiveForm(), MainCurrentFPSField);
	SetFieldTextFromStr(fldP, text, true);
	
	gLastTick = curTicks;
	
	// Current Frame Count
	StrPrintF(text, "%04ld", gFrameCount);
	fldP = FrmGetPtr(FrmGetActiveForm(), MainFramesField);
	SetFieldTextFromStr(fldP, text, true);
}

/**
 * DrawLaoding rectangle
 */
static void PrvDrawLoadingRect()
{
	RGBColorType oldColor;
	RectangleType rect = { { 30, 147 }, { 101, 10 } };
	
	// Draw the progress bar
	WinSetForeColorRGB(&gWhite, &oldColor);
	WinDrawRectangle(&rect, 0);
	WinSetForeColorRGB(&gBlack, NULL);
	WinDrawRectangleFrame(rectangleFrame, &rect);
	
	// Restore
	WinSetForeColorRGB(&oldColor, NULL);
}

#if 0
#pragma mark -
#endif

/**
 * Load a frame from memory.
 */
static void PrvLoadFrameFromMemory(UInt8 *bufferP, PalmImagePlanarType *planarImage, UInt32 imageFormat)
{
	switch(imageFormat)
	{
		case palmCodecImageYCbCr420Planar:
			planarImage->planeP[0] = bufferP;
			bufferP += planarImage->width * planarImage->height;
			planarImage->planeP[1] = bufferP;
			bufferP += planarImage->width * planarImage->height / 4;
			planarImage->planeP[2] = bufferP;
			break;
			
		case palmCodecImageYCbCr422Planar:
			planarImage->planeP[0] = bufferP;
			bufferP += planarImage->width * planarImage->height;
			planarImage->planeP[1] = bufferP;
			bufferP += planarImage->width * planarImage->height / 2;
			planarImage->planeP[2] = bufferP;
			break;
	}	
}

/**
 * Laos a frame from file
 */
static Err PrvLoadFrameFromFile(FileRef fileRef, PalmImagePlanarType *planarImage, UInt32 imageFormat)
{
	Err 	err = errNone;
	UInt32	read = 0;
//	UInt32 	width = 0;
	UInt32 	height = 0;
	UInt32	YHeight = 0;
	UInt32	YWidth = 0;
	UInt32	CHeight = 0;
	UInt32	CWidth = 0;
	UInt8	*planeP = NULL;	
	
	switch(imageFormat)
	{
		case palmCodecImageYCbCr420Planar:
			YWidth 	= planarImage->width;
			YHeight = planarImage->height;
			CWidth 	= planarImage->width / 2;
			CHeight = planarImage->height / 2;
			break;
			
		case palmCodecImageYCbCr422Planar:
			YWidth 	= planarImage->width;
			YHeight = planarImage->height;
			CWidth	= planarImage->width / 2;
			CHeight	= planarImage->height;
			break;
		
		default:
			err = appErrorClass;
			goto Done;
	}
	
	// Load the Y components
	planeP = planarImage->planeP[0];
	for(height = 0; height < YHeight; height++)
	{
		err = VFSFileRead(fileRef, YWidth, planeP, &read);
		if(err || read != YWidth) {
			err = vfsErrFileEOF; 
			goto Done;
		}	
		planeP += YWidth;
	}
	
	// Load the Cb components
	planeP = planarImage->planeP[1];
	for(height = 0; height < CHeight; height++)
	{
		err = VFSFileRead(fileRef,CWidth, planeP, &read);
		if(err || read != CWidth) {
			err = vfsErrFileEOF;
			goto Done;
		}
		planeP += CWidth;
	}
	
	// Load the Cr components
	planeP = planarImage->planeP[2];
	for(height = 0; height < CHeight; height++)
	{
		err = VFSFileRead(fileRef, CWidth, planeP, &read);
		if(err || read != CWidth) {
			err = vfsErrFileEOF;
			goto Done;
		}
		planeP += CWidth;
	}
	
Done:
	return err;
}

#if 0
#pragma mark -
#endif


/**
 * Run the MPEG4 Decoder
 *
 * The loop option is done by resetting the decoder since the
 * next loop will have to read again the VO & VOL header.
 */
static Err PrvStartDecoder()
{
	Err err = errNone;
	EventType event;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
	Boolean useFtr = false;
	UInt32 inputBufferSize = 0;
	UInt8 *inputBufferP = NULL;
	UInt8 *currentBufferP = NULL;
//	UInt8 *offScreenP = NULL;
//	WinHandle offScreenH = NULL;
	RectangleType goButtonBounds;
	PalmImagePlanarType planarImage;
	PalmImagePlanarType *swapImageP = NULL;
	PalmImageParamType *imageParamP = NULL;
	UInt32 read = 0;
	
	// Make sure atack data is clean
	MemSet(&planarImage, sizeof(PalmImagePlanarType), 0);
	
	// Read parameters and check
	err = PrvReadParameters(false);
	if(err)
		goto Done;
		
	// Open the input and output files
	err = PrvFileOpenInput();
	if(err)
		goto Done;
	
	if(gMPEG4Info.outputToSD) {	
		err = PrvFileOpenOutput();
		if(err)
			goto Done;
	}
	
	// Allocate image structure	
	imageParamP = (PalmImageParamType*)MemPtrNew(sizeof(PalmImageParamType));
	if(!imageParamP) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	MemSet(imageParamP, sizeof(PalmImageParamType), 0);
	
	// Allocate swap image	
	swapImageP = (PalmImagePlanarType*)MemPtrNew(sizeof(PalmImagePlanarType));
	if(!swapImageP) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	MemSet(swapImageP, sizeof(PalmImagePlanarType), 0);
	
	// Set Image Params
	imageParamP->width = 0; // Don't care
	imageParamP->height = 0; // Don't care
	imageParamP->rowByte = 0; // Don't care
	imageParamP->endianess = 0; // Don't care
	
	// Create the MPEG4 Decoding session
	//err = CodecMgrCreateSession(gMPEG4Info.codecMgrLibRefNum,  palmCodecVideoMPEG4, NULL, gMPEG4Info.imageFormat, imageParamP, &gMPEG4Info.session);	
	err = CodecMgrCreateSessionByID(gMPEG4Info.codecMgrLibRefNum, palmCodecVideoMPEG4, NULL, gMPEG4Info.imageFormat, imageParamP, 'mp4d', 0, &gMPEG4Info.session);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to create session", NULL, NULL);
		goto Done;
	}
	
	// Set the planar image
	planarImage.width = gMPEG4Info.width;
	planarImage.height = gMPEG4Info.height;
	planarImage.planeCount = 3;
	
	switch(gMPEG4Info.imageFormat)
	{
		case palmCodecImageYCbCr420Planar:
			planarImage.planeP[0] = Allocate( (3 * planarImage.width * planarImage.height) / 2);
			planarImage.planeP[1] = (UInt8*)planarImage.planeP[0] + planarImage.width * planarImage.height;
			planarImage.planeP[2] = (UInt8*)planarImage.planeP[1] + (planarImage.width * planarImage.height / 4);
			planarImage.planeWidth[0] = planarImage.width;
			planarImage.planeWidth[1] = planarImage.width / 2;
			planarImage.planeWidth[2] = planarImage.width / 2;
			break;
			
		default:
			FrmCustomAlert(ErrorAlert, "Bad image format", NULL, NULL);
			goto Done;
			break;
	}
	
	if(!planarImage.planeP[0]) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	
	// Use a byte-swapped copy for the CPM
	swapImageP->width = Swap32(planarImage.width);
	swapImageP->height = Swap32(planarImage.height);
	swapImageP->planeCount = Swap32(planarImage.planeCount);
	swapImageP->planeP[0] = (void*)Swap32(planarImage.planeP[0]);
	swapImageP->planeP[1] = (void*)Swap32(planarImage.planeP[1]);
	swapImageP->planeP[2] = (void*)Swap32(planarImage.planeP[2]);	
	swapImageP->planeWidth[0] = Swap32(planarImage.planeWidth[0]);
	swapImageP->planeWidth[1] = Swap32(planarImage.planeWidth[1]);
	swapImageP->planeWidth[2] = Swap32(planarImage.planeWidth[2]);
	
	// Running from file
	if(gMPEG4Info.loadFileInMemory)
	{	
		// Get the file size
		err = VFSFileSize(gMPEG4Info.inputFileRef, &inputBufferSize);	
		if(err || !inputBufferSize)
			goto Done;
				
		currentBufferP = inputBufferP = (UInt8*)Allocate(inputBufferSize);
		if(!inputBufferP) {
			// Try with FtrPtr
			err = FtrPtrNew(appFileCreator, 0, inputBufferSize, (void*)&inputBufferP);
			if(err) {
				err = memErrNotEnoughSpace;
				FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
				goto Done;
			}
			
			currentBufferP = inputBufferP;
			useFtr = true;
		}
		
		// Load the file
		if(!PrvLoadFileInMemory(gMPEG4Info.inputFileRef, inputBufferSize, inputBufferP, useFtr))
			goto Done;
	}
	else
	{
		// Allocate buffer for VFS
		inputBufferP = (UInt8*)Allocate(DATA_BUFFER_SIZE);
		
		// Load first input buffer
		currentBufferP = inputBufferP;
		err = VFSFileRead(gMPEG4Info.inputFileRef, DATA_BUFFER_SIZE, inputBufferP, &inputBufferSize);
		if(err)
			goto Done;
	}
	
	// Get the bounds for the Go button
	FrmGetObjectBounds(FrmGetActiveForm(), FrmGetObjectIndex(FrmGetActiveForm(),MainGoButton), &goButtonBounds);
	
		
	// LCD Overlay
	if( gUseOverlay)
	{
		LcdOverlayLibSettingType overlaySetting;
		RectangleType rect = { { 0, 0 }, { 0, 0 } };
		
		rect.extent.x = planarImage.width / 2;
		rect.extent.y = planarImage.height / 2;
		
		overlaySetting.type = kLcdOverlayLibDataFormatYCbCr420Planar;
		err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlFormatSet, (void*)&overlaySetting);
		err |= LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlRectSet, &rect);
		err |= LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlStart, NULL);
		
		if(err) {
			FrmCustomAlert(ErrorAlert, "LCD Overlay Init failed!!!", NULL, NULL);
			goto Done;
		}
	}

	
	// Get the current time here we we go!
	gFrameCount = 0;
	gRateControl = sysTicksPerSecond / 15; // Rate control: 15 FPS
	gStartTicks = gLastTick = TimGetTicks();

	// Decode Frames
	while(true)
	{
		// Set input params		
		inSize = inputBufferSize;
		outSize = sizeof(PalmImagePlanarType);
		
		// Check the event queue
		EvtGetEvent(&event, 0);
		if(event.eType == penDownEvent && RctPtInRectangle(event.screenX, event.screenY, &goButtonBounds))
			goto Done;
		
		// Decode a frame
		err = CodecMgrEncodeDecode(gMPEG4Info.codecMgrLibRefNum, gMPEG4Info.session, currentBufferP, &inSize, swapImageP, &outSize);
		
		
		
		switch(err)
		{	
			//UInt32 read = 0;
			
			case kCodecSyncNotFound:
				if(gMPEG4Info.loadFileInMemory) {
					if(gMPEG4Info.loop)
					{
						err=VFSFileSize(gMPEG4Info.inputFileRef, &inputBufferSize);	
						if(err!=errNone) return err;
						currentBufferP = inputBufferP;
						err = CodecMgrResetSession(gMPEG4Info.codecMgrLibRefNum, gMPEG4Info.session);
					}
					else
					{
						FrmCustomAlert(InfoAlert, "Decoding done!", NULL, NULL);
						goto Done;
					}
				} else {
					// Reload the buffer
					MemMove(inputBufferP, currentBufferP, inputBufferSize);
					err = VFSFileRead(gMPEG4Info.inputFileRef, DATA_BUFFER_SIZE - inputBufferSize, inputBufferP + inputBufferSize, &read);
					if(!read) {
						if(gMPEG4Info.loop)
						{
							VFSFileSeek(gMPEG4Info.inputFileRef, vfsOriginBeginning, 0);	
							currentBufferP = inputBufferP;
							err=VFSFileRead(gMPEG4Info.inputFileRef, DATA_BUFFER_SIZE, inputBufferP, &inputBufferSize);
							if(err!=errNone) return err;
							err = CodecMgrResetSession(gMPEG4Info.codecMgrLibRefNum, gMPEG4Info.session);
						}
						else
						{
							if(gUseOverlay)
								err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlStop, NULL);
							FrmCustomAlert(InfoAlert, "Decoding done!", NULL, NULL);
							goto Done;
						}
					}
					// Set current buffer
					inputBufferSize += read;
					currentBufferP = inputBufferP;
				}				
				
				break;
				
			// Real error
			case kCodecMgrLibNotSupported:
			case kCodecErrBadParam:
				if(gUseOverlay)
					err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlStop, NULL);
				FrmCustomAlert(InfoAlert, "Decoding done!", NULL, NULL);
				goto Done;
				
			default:
				break;
		}
				
		// Update the input buffer size
		inputBufferSize -= inSize;
		currentBufferP += inSize;
		
		// Update the Frame counter
		if(outSize)
		{
			gFrameCount++;
			
			if(gUseOverlay)
			{
				err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlDraw, planarImage.planeP[0]);
			}
			
			if(gMPEG4Info.outputToSD)
			{
				UInt32 written = 0;
				UInt32 YSize = planarImage.width * planarImage.height;
				UInt32 CSize = planarImage.width * planarImage.height / 4;
				
				// Write the Y components
				err = VFSFileWrite(gMPEG4Info.outputFileRef, YSize, planarImage.planeP[0], &written);
				if(err || written != YSize) {
					FrmCustomAlert(ErrorAlert, "Error during file write!", NULL, NULL);
					goto Done;
				}
				// Write the Cb components
				err = VFSFileWrite(gMPEG4Info.outputFileRef, CSize, planarImage.planeP[1], &written);
				if(err || written != CSize) {
					FrmCustomAlert(ErrorAlert, "Error during file write!", NULL, NULL);
					goto Done;
				}
				// Write the Cr components
				err = VFSFileWrite(gMPEG4Info.outputFileRef, CSize, planarImage.planeP[2], &written);
				if(err || written != CSize) {
					FrmCustomAlert(ErrorAlert, "Error during file write!", NULL, NULL);
					goto Done;
				}
			}
		}
	
#if USE_RATE_CONTROL == 1
		if((TimGetTicks() - gLastTick) < gRateControl)
			SysTaskDelay(gRateControl - TimGetTicks() + gLastTick);
#endif
		
		// Update the UI frame count
		PrvUpdateUI();
	}
			
Done:
//DEADCODE, not reachable
//	if(offScreenH)
//		WinDeleteWindow(offScreenH, false);
	if(inputBufferP){
		if(useFtr)	
			FtrPtrFree(appFileCreator, 0);
		else
			MemPtrFree(inputBufferP);
	}
	if(planarImage.planeP[0])
		MemPtrFree(planarImage.planeP[0]);

	if(imageParamP)
		MemPtrFree(imageParamP);

	if(swapImageP)
		MemPtrFree(swapImageP);
		
	// Close overlay
	if( gUseOverlay)
		err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlStop, NULL);
				
	// Delete the session
	if(gMPEG4Info.session)
		CodecMgrDeleteSession(gMPEG4Info.codecMgrLibRefNum, &gMPEG4Info.session);
	
	// Close files
	if(gMPEG4Info.inputFileRef) {
		VFSFileClose(gMPEG4Info.inputFileRef);
		gMPEG4Info.inputFileRef = NULL;
	}
	
    if(gMPEG4Info.outputFileRef) {
    	VFSFileClose(gMPEG4Info.outputFileRef);
    	gMPEG4Info.outputFileRef = NULL;
    }
    
    // Quick refresh
    if(gUseOverlay)
	    FrmDrawForm(FrmGetActiveForm());
    		
	return err;
}

/**
 * Run the MPEG4 Encoder
 *
 * The loop option is done by simply rewinding the file or stream.
 */
static Err PrvStartEncoder()
{
	Err err = errNone;
	EventType event;
	Boolean useFtr = false;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
	UInt32 inputBufferSize = 0;
	UInt32 fileOffset = 0;
	UInt8 *inputBufferP = NULL;
	UInt8 *outputBufferP = NULL;
	UInt8 *savedPlaneP[3] = { NULL, NULL, NULL };
	RectangleType goButtonBounds;
	PalmImagePlanarType planarImage;
	PalmImagePlanarType *swapImageP = NULL;
	PalmImageParamType *imageParamP = NULL;
	PalmVideoMPEG4EncodeParamType *mpeg4ParamP = NULL;
	
	// Make sure atack data is clean
	MemSet(&planarImage, sizeof(PalmImagePlanarType), 0);
	
	// Read parameters and check
	err = PrvReadParameters(false);
	if(err)
		goto Done;
		
	// Open the input and output files
	err = PrvFileOpenInput();
	if(err)
		goto Done;
	
	if(gMPEG4Info.outputToSD) {	
		err = PrvFileOpenOutput();
		if(err)
			goto Done;
	}
	
	// Allocate image structure	
	imageParamP = (PalmImageParamType*)MemPtrNew(sizeof(PalmImageParamType));
	if(!imageParamP) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	MemSet(imageParamP, sizeof(PalmImageParamType), 0);
	
	// Allocate swap image	
	swapImageP = (PalmImagePlanarType*)MemPtrNew(sizeof(PalmImagePlanarType));
	if(!swapImageP) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	MemSet(swapImageP, sizeof(PalmImagePlanarType), 0);
	
	// Allocate MPEG4 param	
	mpeg4ParamP = (PalmVideoMPEG4EncodeParamType*)MemPtrNew(sizeof(PalmVideoMPEG4EncodeParamType));
	if(!mpeg4ParamP) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	MemSet(mpeg4ParamP, sizeof(PalmVideoMPEG4EncodeParamType), 0);
	
	// Set Image Params
	imageParamP->width = Swap32(gMPEG4Info.width);
	imageParamP->height = Swap32(gMPEG4Info.height);
	imageParamP->rowByte = 0; // Don't care
	imageParamP->endianess = 0; // Don't care
	
	// Set MPEG4 Params 
	mpeg4ParamP->profile = 0; // Don't care for now
	mpeg4ParamP->volWidth = Swap32(gMPEG4Info.width);
	mpeg4ParamP->volHeight = Swap32(gMPEG4Info.height);
	mpeg4ParamP->keyFrame =  Swap32(gMPEG4Info.keyFrame);
	mpeg4ParamP->frameRate = Swap32(gMPEG4Info.frameRate);
	mpeg4ParamP->bitRate = Swap32(gMPEG4Info.bitRate);
	mpeg4ParamP->algorithm = Swap32(palmCodecMPEG4MVFAST);
	mpeg4ParamP->IVOPQuantization = Swap32(gMPEG4Info.IQuantization);
	mpeg4ParamP->VOVOLVersionID = Swap32(2);
	
	// Set the planar image
	planarImage.width = gMPEG4Info.width;
	planarImage.height = gMPEG4Info.height;
	switch(gMPEG4Info.imageFormat)
	{
		case palmCodecImageYCbCr420Planar:
			planarImage.planeP[0] = Allocate(planarImage.width * planarImage.height);
			planarImage.planeP[1] = Allocate(planarImage.width * planarImage.height / 4);
			planarImage.planeP[2] = Allocate(planarImage.width * planarImage.height / 4);
			planarImage.planeWidth[0] = planarImage.width;
			planarImage.planeWidth[1] = planarImage.width / 2;
			planarImage.planeWidth[2] = planarImage.width / 2;
			break;
			
		case palmCodecImageYCbCr422Planar:
			planarImage.planeP[0] = Allocate(planarImage.width * planarImage.height);
			planarImage.planeP[1] = Allocate(planarImage.width * planarImage.height / 2);
			planarImage.planeP[2] = Allocate(planarImage.width * planarImage.height / 2);
			planarImage.planeWidth[0] = planarImage.width;
			planarImage.planeWidth[1] = planarImage.width / 2;
			planarImage.planeWidth[2] = planarImage.width / 2;
			break;
			
		default:
			FrmCustomAlert(ErrorAlert, "Bad image format", NULL, NULL);
			goto Done;
			break;
	}
	
	// Check for memory 
	if(!planarImage.planeP[0] || !planarImage.planeP[1] || !planarImage.planeP[2]) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	
	// Save plane pointers since they can be modified
	savedPlaneP[0] = planarImage.planeP[0];
	savedPlaneP[1] = planarImage.planeP[1];
	savedPlaneP[2] = planarImage.planeP[2];
	
	// Create a copy for the CPM
	swapImageP->width = Swap32(planarImage.width);
	swapImageP->height = Swap32(planarImage.height);
	swapImageP->planeP[0] = (void*)Swap32(planarImage.planeP[0]);
	swapImageP->planeP[1] = (void*)Swap32(planarImage.planeP[1]);
	swapImageP->planeP[2] = (void*)Swap32(planarImage.planeP[2]);
	swapImageP->planeWidth[0] = Swap32(planarImage.planeWidth[0]);
	swapImageP->planeWidth[1] = Swap32(planarImage.planeWidth[1]);
	swapImageP->planeWidth[2] = Swap32(planarImage.planeWidth[2]);
		
	// Create the MPEG4 Encoding session
	err = CodecMgrCreateSession(gMPEG4Info.codecMgrLibRefNum, gMPEG4Info.imageFormat, imageParamP, palmCodecVideoMPEG4, mpeg4ParamP, &gMPEG4Info.session);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to create session", NULL, NULL);
		goto Done;
	}
	
	// Allocate output buffer	
	outputBufferP = (UInt8*)Allocate(planarImage.width * planarImage.height * 2);
	if(!outputBufferP) {
		err = memErrNotEnoughSpace;
		FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
		goto Done;
	}
	MemSet(mpeg4ParamP, sizeof(PalmVideoMPEG4EncodeParamType), 0);
	
	// Running from file
	if(gMPEG4Info.loadFileInMemory)
	{	
		// Get the file size
		err = VFSFileSize(gMPEG4Info.inputFileRef, &inputBufferSize);	
		if(err || !inputBufferSize)
			goto Done;
		
		// Might work....	
		inputBufferP = (UInt8*)Allocate(inputBufferSize);
		if(!inputBufferP) {
		
			// Try with FtrPtr
			err = FtrPtrNew(appFileCreator, 0, inputBufferSize, (void*)&inputBufferP);
			if(err) {
				err = memErrNotEnoughSpace;
				FrmCustomAlert(ErrorAlert, "Not enough memory", NULL, NULL);
				goto Done;
			}
			
			useFtr = true;
		}
		
		
		// Load the file
		if(!PrvLoadFileInMemory(gMPEG4Info.inputFileRef, inputBufferSize, inputBufferP, useFtr))
			goto Done;
	}
	
	// Get the bounds for the Go button
	FrmGetObjectBounds(FrmGetActiveForm(), FrmGetObjectIndex(FrmGetActiveForm(),MainGoButton), &goButtonBounds);
	
	// Get the current time here we we go!
	gFrameCount = 0;
	gStartTicks = gLastTick = TimGetTicks();

	// Decode Frames
	while(true)
	{
		// Load raw data from input file
		if(gMPEG4Info.loadFileInMemory)
		{
			if( gMPEG4Info.loop && fileOffset < inputBufferSize)
				fileOffset = 0;
			
			if( fileOffset < inputBufferSize )
			{
				PrvLoadFrameFromMemory(inputBufferP + fileOffset, &planarImage, gMPEG4Info.imageFormat);
				swapImageP->planeP[0] = (void*)Swap32(planarImage.planeP[0]);
				swapImageP->planeP[1] = (void*)Swap32(planarImage.planeP[1]);
				swapImageP->planeP[2] = (void*)Swap32(planarImage.planeP[2]);
				switch(gMPEG4Info.imageFormat)
				{
					case palmCodecImageYCbCr420Planar:
						fileOffset += ( 3 * planarImage.width * planarImage.height ) / 2;
						break;
						
					case palmCodecImageYCbCr422Planar:
						fileOffset += ( 2 * planarImage.width * planarImage.height );
						break;
				}
			}
			else
			{
				FrmCustomAlert(InfoAlert, "Encoding done!", NULL, NULL);
				goto Done;
			}
		}
		else
		{
			// Load from file
			err = PrvLoadFrameFromFile(gMPEG4Info.inputFileRef, &planarImage, gMPEG4Info.imageFormat);
			if(err) {
				if(gMPEG4Info.loop) {
					VFSFileSeek(gMPEG4Info.inputFileRef, vfsOriginBeginning, 0);	
					err = PrvLoadFrameFromFile(gMPEG4Info.inputFileRef, &planarImage, gMPEG4Info.imageFormat);
				} else {
					FrmCustomAlert(InfoAlert, "Encoding done!", NULL, NULL);
					goto Done;
				}
			}
			swapImageP->planeP[0] = (void*)Swap32(planarImage.planeP[0]);
			swapImageP->planeP[1] = (void*)Swap32(planarImage.planeP[1]);
			swapImageP->planeP[2] = (void*)Swap32(planarImage.planeP[2]);
		}
		
		inSize = sizeof(PalmImagePlanarType);
		outSize = planarImage.width * planarImage.height * 2;
		
		// Check the event queue
		EvtGetEvent(&event, 0);
		if(event.eType == penDownEvent && RctPtInRectangle(event.screenX, event.screenY, &goButtonBounds))
			goto Done;
	
		// Encode one frame
		err = CodecMgrEncodeDecode(gMPEG4Info.codecMgrLibRefNum, gMPEG4Info.session, swapImageP, &inSize, outputBufferP, &outSize);
		
		// Update the Frame counter
		if(outSize)
		{
			gFrameCount++;
			
			if(gMPEG4Info.outputToSD)
			{
				UInt32 written = 0;
				
				// Write the Buffer
				err = VFSFileWrite(gMPEG4Info.outputFileRef, outSize, outputBufferP, &written);
				if(err || written != outSize) {
					FrmCustomAlert(ErrorAlert, "Error during file write!", NULL, NULL);
					goto Done;
				}
			}
		}
		
		// Update the UI frame count
		PrvUpdateUI();
	}
			
Done:
	if(inputBufferP) {
		if(useFtr)	
			FtrPtrFree(appFileCreator, 0);
		else
			MemPtrFree(inputBufferP);
	}
	if(outputBufferP)
		MemPtrFree(outputBufferP);	
	if(savedPlaneP[0])
		MemPtrFree(savedPlaneP[0]);
	if(savedPlaneP[1])
		MemPtrFree(savedPlaneP[1]);
	if(savedPlaneP[2])
		MemPtrFree(savedPlaneP[2]);
	if(imageParamP)
		MemPtrFree(imageParamP);
	if(swapImageP)
		MemPtrFree(swapImageP);
		
	// Delete the session
	if(gMPEG4Info.session)
		CodecMgrDeleteSession(gMPEG4Info.codecMgrLibRefNum, &gMPEG4Info.session);
	
	// Close files
	if(gMPEG4Info.inputFileRef) {
		VFSFileClose(gMPEG4Info.inputFileRef);
		gMPEG4Info.inputFileRef = NULL;
	}
	
    if(gMPEG4Info.outputFileRef) {
    	VFSFileClose(gMPEG4Info.outputFileRef);
    	gMPEG4Info.outputFileRef = NULL;
    }
    		
	return err;
}

#if 0
#pragma mark -
#endif

/**
 * Initialize the form
 */
static void MainFormInit(FormType *frmP)
{
	Char text[32];
	FieldType *fldP = NULL;
	ControlType *ctlP = NULL;
	
	// Set the current mode
	if(gMPEG4Info.encoding)	{
		ctlP = FrmGetPtr(frmP, MainEncodePushButton);
		CtlSetValue(ctlP, true);
	} else {
		ctlP = FrmGetPtr(frmP, MainDecodePushButton);
		CtlSetValue(ctlP, true);
	}
	
	// Set encoder specfic params
	StrIToA(text, gMPEG4Info.searchRange);
	fldP = FrmGetPtr(frmP, MainSearchRangeField);
	SetFieldTextFromStr(fldP, text, false);
	StrIToA(text, gMPEG4Info.bitRate);
	fldP = FrmGetPtr(frmP, MainBitRateField);
	SetFieldTextFromStr(fldP, text, false);
	StrIToA(text, gMPEG4Info.keyFrame);
	fldP = FrmGetPtr(frmP, MainKeyFrameField);
	SetFieldTextFromStr(fldP, text, false);
	StrIToA(text, gMPEG4Info.IQuantization);
	fldP = FrmGetPtr(frmP, MainIQuantizationField);
	SetFieldTextFromStr(fldP, text, false);
	
	// Set Width and Height
	StrIToA(text, gMPEG4Info.width);
	fldP = FrmGetPtr(frmP, MainWidthField);
	SetFieldTextFromStr(fldP, text, false);
	StrIToA(text, gMPEG4Info.height);
	fldP = FrmGetPtr(frmP, MainHeightField);
	SetFieldTextFromStr(fldP, text, false);
	
	// Set files
	if(gMPEG4Info.inputFileName[0]!=NULL) {
		fldP = FrmGetPtr(frmP, MainInputField);
		SetFieldTextFromStr(fldP, gMPEG4Info.inputFileName, false);
	}
	
	if(gMPEG4Info.outputFileName[0]!=NULL) {
		fldP = FrmGetPtr(frmP, MainOutputField);
		SetFieldTextFromStr(fldP, gMPEG4Info.outputFileName, false);
	}
		
	// Set the checkbox
	ctlP = FrmGetPtr(frmP, MainLoadInMemCheckbox);
	CtlSetValue(ctlP, gMPEG4Info.loadFileInMemory);
	ctlP = FrmGetPtr(frmP, MainToSDCheckbox);
	CtlSetValue(ctlP, gMPEG4Info.outputToSD);
	ctlP = FrmGetPtr(frmP, MainLoopCheckbox);
	CtlSetValue(ctlP, gMPEG4Info.loop);
		
	
	// Overlay
	if(gLCDOverlayRefNum == sysInvalidRefNum)
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainOverlayPushButton));
	else
	{
		ctlP = FrmGetPtr(frmP, MainOverlayPushButton);
		CtlSetValue(ctlP, gUseOverlay);
	}
}

/**
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();
	FieldType *fldP = NULL;


	switch(eventP->eType)
	{
		case frmOpenEvent:
			MainFormInit(frmP);
			FrmDrawForm(frmP);
			PrvDrawLoadingRect();
			FrmNavObjectTakeFocus(frmP, MainInputField);
			handled = true;
			break;
			
		case frmUpdateEvent:
			break;
			
		case popSelectEvent:
			switch(eventP->data.popSelect.controlID)
			{
				case MainFormatPopTrigger:
					switch(eventP->data.popSelect.selection) {
						case 0:
							gMPEG4Info.imageFormat = palmCodecImageYCbCr420Planar;
							break;
						case 1:
							gMPEG4Info.imageFormat = palmCodecImageYCbCr422Planar;
							break;
					}
					break;
				
				case MainQuickPickPopTrigger:
					switch(eventP->data.popSelect.selection) {
						case 0:
							gMPEG4Info.width = 320;
							gMPEG4Info.height = 240;
							break;
						case 1:
							gMPEG4Info.width = 176;
							gMPEG4Info.height = 144;
							break;
						case 2:
							gMPEG4Info.width = 352;
							gMPEG4Info.height = 288;
							break;
					}
					{
						Char text[32];
						// Set Width and Height
						StrIToA(text, gMPEG4Info.width);
						fldP = FrmGetPtr(frmP, MainWidthField);
						SetFieldTextFromStr(fldP, text, true);
						StrIToA(text, gMPEG4Info.height);
						fldP = FrmGetPtr(frmP, MainHeightField);
						SetFieldTextFromStr(fldP, text, true);
					}
					break;
			}
			break;
			
		case ctlSelectEvent:
			switch(eventP->data.ctlSelect.controlID)
			{
				case MainInputGraphicButton:
					// Sets the callback function and return form
					if(PrvReadParameters(gMPEG4Info.encoding)==errNone) { // Store before switching form
						FileBrowserSetCallback(&PrvGetInputFileInfoCallback, MainForm);
						FrmGotoForm(FileBrowserForm);
						handled = true;
					}
					break;
					
				case MainEncodePushButton:
					gMPEG4Info.encoding = true;
					break;
					
				case MainDecodePushButton:
					gMPEG4Info.encoding = false;
					break;
					
				case MainLoadInMemCheckbox:
					gMPEG4Info.loadFileInMemory = CtlGetValue(eventP->data.ctlSelect.pControl);
					break;
					
				case MainToSDCheckbox:
					gMPEG4Info.outputToSD = CtlGetValue(eventP->data.ctlSelect.pControl);
					break;
					
				case MainLoopCheckbox:
					gMPEG4Info.loop = CtlGetValue(eventP->data.ctlSelect.pControl);
					break;
					
				case MainGoButton:
					CtlSetLabel(FrmGetPtr(FrmGetActiveForm(), MainGoButton), gGOButtonLabel[1]);
					if(gMPEG4Info.encoding)
						PrvStartEncoder();
					else
						PrvStartDecoder();
					CtlSetLabel(FrmGetPtr(FrmGetActiveForm(), MainGoButton), gGOButtonLabel[0]);
					PrvDrawLoadingRect();
					handled = true;
					break;
					
				case MainStartColorGadget:
				{
					IndexedColorType colorIndex = WinRGBToIndex(&gStartColor);
					if( UIPickColor(&colorIndex, NULL, UIPickColorStartPalette, "End Color", NULL) )
						WinIndexToRGB(colorIndex, &gStartColor);
					FrmDrawForm(FrmGetActiveForm());
					PrvDrawLoadingRect();
					handled = true;
				}
					break;
					
				case MainEndColorGadget:
				{
					IndexedColorType colorIndex = WinRGBToIndex(&gEndColor);
					if( UIPickColor(&colorIndex, NULL, UIPickColorStartPalette, "End Color", NULL) )
						WinIndexToRGB(colorIndex, &gEndColor);
					FrmDrawForm(FrmGetActiveForm());
					PrvDrawLoadingRect();
					handled = true;
				}
					break;
								
					
				case MainOverlayPushButton:
					gUseOverlay = eventP->data.ctlSelect.on;
					FrmDrawForm(FrmGetActiveForm());
					PrvDrawLoadingRect();
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
            FrmSetGadgetHandler(frmP,  FrmGetObjectIndex(frmP, MainStartColorGadget), ColorButtonGadgetHandler);
			FrmSetGadgetHandler(frmP,  FrmGetObjectIndex(frmP, MainEndColorGadget), ColorButtonGadgetHandler);
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
	MemSet(&gMPEG4Info, sizeof(MPEG4Info), 0);

	// Try to find the hardware utils library
	err = SysLibFind(kCodecMgrLibName, &gMPEG4Info.codecMgrLibRefNum);	
	if (err != errNone)
	{
		err = SysLibLoad(kCodecMgrLibType, kCodecMgrLibCreator, &gMPEG4Info.codecMgrLibRefNum);
		if(err == errNone)
			err = CodecMgrOpen(gMPEG4Info.codecMgrLibRefNum);
	}
	
	if(err) {
		FrmAlert(NoCPMLibAlert);
		goto Done;
	}


	// Try to load LCD Overlay library
	err = SysLibFind(kLcdOverlayLibName, &gLCDOverlayRefNum);	
	if (err != errNone)
	{
		err = SysLibLoad(kLcdOverlayLibType, kLcdOverlayLibCreator, &gLCDOverlayRefNum);
		if(err == errNone)
			err = LcdOverlayLibOpen(gLCDOverlayRefNum);
	}
	// Don't generate an error for the Overlay library
	err = errNone;
	
	
	// Set default values
	gMPEG4Info.inputFileRef = NULL;
	gMPEG4Info.outputFileRef = NULL;
	gMPEG4Info.session = NULL;
	
	gMPEG4Info.width = 320;
	gMPEG4Info.height = 240;
	gMPEG4Info.frameRate = 15;
	gMPEG4Info.bitRate = 0;
	gMPEG4Info.keyFrame = 20;
	gMPEG4Info.IQuantization = 16;
	gMPEG4Info.imageFormat = palmCodecImageYCbCr420Planar;
	gMPEG4Info.searchRange = 10;
	
	gMPEG4Info.loadFileInMemory = true;
	gMPEG4Info.outputToSD = false;
	gMPEG4Info.encoding = true;
	gMPEG4Info.loop = false;
	
	StrCopy(gMPEG4Info.outputFileName, "/default.out");
	
	// Switch to 16 bit color
	WinScreenMode(winScreenModeGet, NULL, NULL, &gPreviousScreenDepth, NULL);
	WinScreenMode(winScreenModeSet, NULL, NULL, &gScreenDepth, NULL);
		
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
    
    // Close files
	if(gMPEG4Info.inputFileRef)
		VFSFileClose(gMPEG4Info.inputFileRef);
	if(gMPEG4Info.outputFileRef)
		VFSFileClose(gMPEG4Info.outputFileRef);
	
	// Close the library
	if(gMPEG4Info.codecMgrLibRefNum != sysInvalidRefNum)
	{
		err = CodecMgrClose(gMPEG4Info.codecMgrLibRefNum);
		if (err == errNone)
			err = SysLibRemove(gMPEG4Info.codecMgrLibRefNum);
	}
	
	
	if(gLCDOverlayRefNum != sysInvalidRefNum)
	{
		err = LcdOverlayLibClose(gLCDOverlayRefNum);
		if (err == errNone)
			err = SysLibRemove(gLCDOverlayRefNum);
	}
	
	// Restore previous color mode
	WinScreenMode(winScreenModeSet, NULL, NULL, &gPreviousScreenDepth, NULL);
}

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
//    SysNotifyParamType *notifyParamP = NULL;

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
        
    default:
        break;
    }

//Done:
    return errNone;
}

