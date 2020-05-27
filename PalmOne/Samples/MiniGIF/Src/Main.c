/*
 * MiniGIF.c
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
#include <MemGlue.h>
#include "Common.h"
#include "MiniGIFRsc.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"
#include <PalmOneCodecPluginMgr.h>
#include <PalmOneCodecFormat.h>
#include <HsNav.h>

#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#define TRACK_TIME	(sysTicksPerSecond / 10)

/**
 * GIF info.
 */
typedef struct
{
	UInt32		top;
	UInt32		left;
	UInt32 		width;
	UInt32 		height;
	UInt32		delay;
	WinHandle	winHandle;
	
	struct GIFFrame *nextP;
	
} GIFFrame;

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

/**
 * Mini info structure.
 * 
 */
typedef struct
{
	UInt16 			codecMgrLibRefNum;
	PalmCodecSession session;		// 

	FileInfo		fileInfo;		// File info
	UInt32		imageWidth;
	UInt32		imageHeight;	
	UInt32		rowByte;
	GIFFrame	*gifImageP;

	UInt16	cardNo;
	LocalID dbID;
} MiniInfo;

/**
 * Globals
 */
static MiniInfo gMiniInfo;
static UInt32	gScreenDepth = 16;
static UInt32	gPreviousScreenDepth = 0;
static UInt32	gLastTick = 0;
static GIFFrame	*gCurrentImageP = NULL;
static Char gAlertString[85];
static Boolean gPen = false;

/**
 * Byte-swapping function
 */
static void PrvSwapStructure(UInt32 *structP, UInt32 numElement)
{	
	while(numElement--) {
		*structP = Swap32(*structP);
		structP++;
	}
}


/**
 * Load the GIF file
 */
static Err PrvLoadGIF()
{
	Err err = errNone;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
//	UInt32 written = 0;
	UInt8 *bufferP = NULL;
	UInt32 bufferSize = 0;
	GIFFrame **gifFramePP = NULL;
	PalmCodecSession gifSession = NULL;
	PalmImageParamType *imageParamP = NULL;
	PalmFileFormatParamType *fileParamP = NULL;
	PalmImageGIFFrameParamType *gifParamP = NULL;
		
	// Always use MemPtrNew, so that we are sure the pointers are 4 byte aligned
	fileParamP = MemPtrNew(sizeof(PalmFileFormatParamType)) ;
	imageParamP = MemPtrNew(sizeof(PalmImageParamType));
	gifParamP = MemPtrNew(sizeof(PalmImageGIFFrameParamType));
	
	if(!fileParamP || !imageParamP || !gifParamP) {
		FrmCustomAlert(InfoAlert, "Not Enough Memory !", NULL, NULL);
		err = 1;
		goto Done;
	}
	
	// Set File parameters
	fileParamP->fileLocationType = Swap32(palmCodecVFSFile);
	fileParamP->vfsFileRef = (FileRef)Swap32(gMiniInfo.fileInfo.fileRef);
		
	imageParamP->width = Swap32(0);
	imageParamP->height = Swap32(0);
	imageParamP->rowByte = Swap32(0);
	imageParamP->endianess = Swap32(palmCodecLITTLE_ENDIAN);
		
	// Create Session
	err = CodecMgrCreateSession(gMiniInfo.codecMgrLibRefNum, 
			palmCodecImageGIF89aFile, fileParamP, 
			palmCodecImageBGR565, imageParamP,
			&gifSession);
			
	if(err) {
		FrmCustomAlert(InfoAlert, "Unable to create codec session !", NULL, NULL);
		goto Done;
	}
	
	// Get the width and height of the image sent back
	gMiniInfo.imageWidth = Swap32(imageParamP->width);
	gMiniInfo.imageHeight = Swap32(imageParamP->height);
	gMiniInfo.rowByte = Swap32(imageParamP->rowByte);
	
	gifFramePP = &gMiniInfo.gifImageP;
	
	// Allocate buffer
	bufferSize = gMiniInfo.rowByte * gMiniInfo.imageHeight;
	bufferP = MemPtrNew(bufferSize); //MemGluePtrNew(bufferSize);
	if(!bufferP) {
		FrmCustomAlert(InfoAlert, "Not Enough Memory !", NULL, NULL);
		err = 1;
		goto Done;
	}
			
	// Decode the image
	while(true)
	{
		// Set the size for the input and output
		inSize = sizeof(PalmFileFormatParamType);
		outSize = sizeof(PalmImageGIFFrameParamType); //outSize = bufferSize;
		
		// Gif Params
		gifParamP->imageDataSize = Swap32(bufferSize);
		gifParamP->imageDataP = (UInt8*)Swap32(bufferP);

		// Decode 1 frame
		err = CodecMgrEncodeDecode(gMiniInfo.codecMgrLibRefNum,
				gifSession, fileParamP, &inSize, gifParamP, &outSize);
		
		// If no more to read
		if(!inSize || err)
		{
			if(outSize == 0)
				err = errNone;//Hack, because the last encode decode may return error
			break;
		}
			
		// We have a frame!			
		if(outSize) {
			UInt8 *winBufferP = NULL;
			BitmapType *winBmpP = NULL;
			
			// Swap the returned structure - only the first 11 elements
			PrvSwapStructure((UInt32*)gifParamP, 11);
			gifParamP->imageDataSize = Swap32(gifParamP->imageDataSize);
			
			// Create new gif frame in out list
			*gifFramePP = MemPtrNew(sizeof(GIFFrame));
			MemSet(*gifFramePP, sizeof(GIFFrame), 0);
			
			(*gifFramePP)->top = gifParamP->topPosition;
			(*gifFramePP)->left = gifParamP->leftPosition;
			(*gifFramePP)->width = gifParamP->imageWidth;
			(*gifFramePP)->height = gifParamP->imageHeight;
			(*gifFramePP)->delay = gifParamP->delayTime * 100 / sysTicksPerSecond;
			
			// Create offscreen buffer and copy data into it
			(*gifFramePP)->winHandle = WinCreateOffscreenWindow((gifParamP->imageWidth+1) / 2, (gifParamP->imageHeight+1) / 2, nativeFormat, &err);
			winBmpP = WinGetBitmap((*gifFramePP)->winHandle);
			winBufferP = BmpGetBits(winBmpP);
			MemMove(winBufferP, bufferP, gifParamP->imageDataSize);

			gifFramePP = (GIFFrame**)&((*gifFramePP)->nextP);
		}
	}
	
Done:
	gCurrentImageP = gMiniInfo.gifImageP;
	
	if(gifSession)
		CodecMgrDeleteSession(gMiniInfo.codecMgrLibRefNum, &gifSession);
	if(fileParamP)
		MemPtrFree(fileParamP);
	if(imageParamP)
		MemPtrFree(imageParamP);
	if(gifParamP)
		MemPtrFree(gifParamP);
	if(bufferP)
		MemPtrFree(bufferP);

	return err;
}

/**
 * Draw image on the screen
 */
static void PrvDrawCurrentImage()
{
	Coord x, y;
	RectangleType rect = { { 0, 0 }, { 0, 0 } };
	RectangleType rect2;
		
	// Image size
	rect.extent.x = gMiniInfo.imageWidth / 2;
	if(rect.extent.x > 160)	rect.extent.x = 160;
	rect.extent.y = gMiniInfo.imageHeight / 2;
	if(rect.extent.y > 160)	rect.extent.y = 160;
	
	// Center the image
	x = 80 - rect.extent.x / 2;
	y = 80 - rect.extent.y / 2;
	
	MemMove(&rect2, &rect, sizeof(RectangleType));
	rect2.topLeft.x = x;
	rect2.topLeft.y = y;
		
	// Display the capture image
	if(gCurrentImageP && gCurrentImageP->winHandle)
		WinCopyRectangle(gCurrentImageP->winHandle, WinGetDisplayWindow(), &rect, x, y, winPaint);
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
static Err PrvFileOpen()
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	

	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		StrCopy( gAlertString, "Error enumerating Volume"); 
		goto Done;
	}
	
	// If we have a file path, open the file
	if( gMiniInfo.fileInfo.filePath[0]!=NULL )
	{
		err = VFSFileOpen(firstVolume, (Char*)gMiniInfo.fileInfo.filePath, vfsModeRead, &gMiniInfo.fileInfo.fileRef);
		if( err ) 
		{
			StrCopy( gAlertString, "File Open err. Select a gif file");
			goto Done;
		}
		
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
		
Done:	

	if(err)	
		PrvAlert( gAlertString );
	return err;
}


/**
 * Memory clean up after we are done displaying the images
 * 
 */
static void PrvCleanImages()
{
	GIFFrame *gifFrameP = gMiniInfo.gifImageP;
	
	while(gifFrameP)
	{
		GIFFrame *nextP = (GIFFrame *)gifFrameP->nextP;
		if(gifFrameP->winHandle)
			WinDeleteWindow(gifFrameP->winHandle, false);
		MemPtrFree(gifFrameP);
		gifFrameP = nextP;
	}
	
	// Seek the file to beginning, if folks wanna click on Display again.
	VFSFileSeek ( gMiniInfo.fileInfo.fileRef,vfsOriginBeginning, 0);
	gMiniInfo.gifImageP = NULL;
	gCurrentImageP = NULL;
}

/**
 * File info callback.
 * This function is called by the file browser
 */
static void PrvGetInputFileInfoCallback(UInt16 volume, const Char* path, const Char* file)
{
	Err err = errNone;
	
	// Create the full path name
	StrCopy(gMiniInfo.fileInfo.filePath, path);

	// Internally, we expect full path
	StrCat(gMiniInfo.fileInfo.filePath, file);
	
	// Open the selected file
	err = PrvFileOpen();
	if(err) {
		// Oops!
		FrmCustomAlert(InfoAlert, "Error opening Input file", "" ,"");
	} 
	
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
		case HelpAboutMiniGIF:
			frmAboutP = FrmInitForm(HelpAboutMiniGIF);
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
	
	switch (eventP->eType)
	{
		case menuEvent:
			handled = MainFormDoCommand(eventP->data.menu.itemID);
            break;
            
		case frmOpenEvent:
			
			FrmDrawForm(frmP);
			FrmNavObjectTakeFocus(frmP, MainFileButton);
			handled = true;			
			break;
			
			
		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == MainFileButton)
			{
				FileBrowserSetCallback(&PrvGetInputFileInfoCallback, MainForm);
				FrmGotoForm(FileBrowserForm);
				handled = true;
			
			}
			else if(eventP->data.ctlSelect.controlID == MainDisplayButton)
			{				
				FrmGotoForm( DisplayForm );
				handled = true;
			}	
			break;
			
		
			
		default:
			break;
	}

Done:	
	return handled;
}

/**
 * DisplayFormHandleEvent
 */
static Boolean DisplayFormHandleEvent(EventType *eventP)
{
	Err err = errNone;
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();

	switch(eventP->eType)
	{
		case frmOpenEvent:
			FrmDrawForm(frmP);
			err = PrvLoadGIF();
			if( !err )
				PrvDrawCurrentImage();
			else
				FrmGotoForm( MainForm );
			
			gLastTick = TimGetTicks();
				
			handled = true;
			break;
			
		case frmCloseEvent:			
			PrvCleanImages();  
			handled = true;  
			break;
		
		case penDownEvent:
			gPen = true;
			break;
			
		case penUpEvent:		
			if( gPen == true ){
				gPen = false;
				FrmGotoForm(MainForm);
			}
				
			handled = true;
			break;
		
		case nilEvent:
			if(gCurrentImageP && (TimGetTicks() - gLastTick >= gCurrentImageP->delay))
			{
				gCurrentImageP = (GIFFrame*)gCurrentImageP->nextP;
				if(!gCurrentImageP)
					gCurrentImageP = gMiniInfo.gifImageP;
				PrvDrawCurrentImage();
				gLastTick = TimGetTicks();
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
         
        case DisplayForm:
            FrmSetEventHandler(frmP, DisplayFormHandleEvent);
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
    	EvtGetEvent(&event, 0);

    	if (! SysHandleEvent(&event)) {
    		if (! MenuHandleEvent(0, &event, &error)) {
    		   if (! AppHandleEvent(&event)) {
    			  FrmDispatchEvent(&event);
    		   }
            }
        }
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
	err = SysLibFind(kCodecMgrLibName, &gMiniInfo.codecMgrLibRefNum);	
	if (err != errNone)
	{
		err = SysLibLoad(kCodecMgrLibType, kCodecMgrLibCreator, &gMiniInfo.codecMgrLibRefNum);
		if(err == errNone)
			err = CodecMgrOpen(gMiniInfo.codecMgrLibRefNum);
	}
	
	if(err) {
		FrmCustomAlert(InfoAlert, "Codec Plugin Manager not found !", NULL, NULL);
		goto Done;
	}
	
	
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
       	
 	// Close the library	
	err = CodecMgrClose(gMiniInfo.codecMgrLibRefNum);
	if (err == errNone)
		err = SysLibRemove(gMiniInfo.codecMgrLibRefNum);
	
			
	// Restore previous color mode
	WinScreenMode(winScreenModeSet, NULL, NULL, &gPreviousScreenDepth, NULL);
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
        
    
    default:
        break;
    }

Done:
    return errNone;
}

#pragma warn_a5_access reset
