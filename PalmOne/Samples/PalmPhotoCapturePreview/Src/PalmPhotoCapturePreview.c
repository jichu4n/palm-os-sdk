/*
 * PalmPhotoCapturePreview.c
 *
 * main file for PalmPhotoCapturePreview
 *
 *
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
 
  /** @defgroup PhotoCapture PalmPhotoCapturePreview Sample Code
 * This sample code showcases how to use the Palm Photo Library to launch the Camera UI, capture a photo, preview that photo,
 * and save this photo to an album on the device.
 *
 * @note Please see additional sample codes that showcase further Photo Library capabilities.
 *
 * @{
 * @}	
 */
 /**
 @ingroup PhotoCapture
 */
 
/**
* 																		  																  
* @file PalmPhotoCapturePreview.c														  												  
*																		  
* @brief The main source file for this sample code.
*																		
*/
 
#include <PalmOS.h>
//#include <PalmOSGlue.h>
#include "StringMgr.h"
#include "ErrorMgr.h"
#include <BuildDefines.h>   // included for definition of ERROR_CHECK_FULL

#include "PalmPhotoCapturePreview.h"
#include "../Rsc/PalmPhotoCapturePreview_Rsc.h"
#include "PalmPhoto.h"
#include <HsExt.h>
#include <HsNav.h>




/*********************************************************************
 * Global variables and internal constants
 *********************************************************************/
 
 /**
 * @name Global variables and internal constants
 * 
 */
 /*@{*/

// vary this parameter for debug vs release versions
#ifndef ERROR_CHECK_LEVEL
	#define ERROR_CHECK_LEVEL ERROR_CHECK_FULL
#endif

UInt16 	PhotosLibRefNum = sysInvalidRefNum;

//temporary jpeg file name. Used for saving file from file stream to memory.				 
#define tempJPEGFile	"tempJPEGFile.jpg"


/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(5,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*@}*/

/*********************************************************************
 * Internal Functions
 *********************************************************************/

/*
 * FUNCTION: GetObjectPtr
 *
 * DESCRIPTION:
 *
 * This routine returns a pointer to an object in the current form.
 *
 * PARAMETERS:
 *
 * formId
 *     id of the form to display
 *
 * RETURNED:
 *     address of object as a void pointer
 */

// uncomment it out if this function is to be used
/*
static void * GetObjectPtr(UInt16 objectID)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}
*/

/*
 * FUNCTION: MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void MainFormInit(FormType *frmP)
{
	FieldType *field;
	const char *wizardDescription;
	UInt16 fieldIndex;

	fieldIndex = FrmGetObjectIndex(frmP, MainDescriptionField);
	field = (FieldType *)FrmGetObjectPtr(frmP, fieldIndex);
	FrmSetFocus(frmP, fieldIndex);

	wizardDescription =
		"Palm Sample Code\n"
		"Shows how to Capture, Preview, and \n"
		"add a Photo to an album.\n"		
		"\n"
		"Please see: \n"
		"               source code \n"
		"               Developer Guide \n"		
		"           and API Guide \n"
		"\n"
		"for more details.\n"
		;
				
	/* dont stack FldInsert calls, since each one generates a
	 * fldChangedEvent, and multiple uses can overflow the event queue */
	FldInsert(field, wizardDescription, StrLen(wizardDescription));
}

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
	DmSearchStateType searchState;
	UInt16 versionP = 1.0;

	
	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);


	 

	switch (command)
	{
	case AboutPhotoCapturePreview:
		MenuEraseStatus(0);
		DmSetDatabaseInfo (cardNo, dbID, NULL, NULL, &versionP, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   		HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
		handled = true;
		break;
	
	default:
		break;
	
	}

	return handled;

}

/*
 * FUNCTION: MainFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "MainForm" of this 
 * application.
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP;
	

	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			MainFormInit(frmP);
			FrmDrawForm(frmP);		
			FrmNavObjectTakeFocus(frmP, MainDescriptionField); // Set navigation focus	
			handled = true;
			break;
           
			
		case frmUpdateEvent:
			FrmDrawForm(FrmGetFormPtr(eventP->data.frmUpdate.formID));
			handled = true;
			break;	
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == MainLaunchCameraButton)
			{
			
				CapturePicture(); 
						   	
				handled = true;					
			}

			break;
		}
		
		default:
		    break;
	}
    
	return handled;
}


/*
 * FUNCTION: CapturePicture
 *
 * DESCRIPTION:
 *
 * This is the main routine of this sample code, which captures the picture,
 * and saves this picture to the photo album.
 *
 * PARAMETERS:
 *     none
 *
 * RETURNED:
 *     nothing
 */


void CapturePicture()
{
	PalmPhotoCaptureParamV2 captureParam;
	Err err = errNone;

	UInt32 displayDensity = kDensityLow;
	UInt32 winApiVersion = 0;

	PalmPhotoHandle	imageH = NULL;
	
 	PalmPhotoHandle	newImageH = NULL;
	UInt16 albumID = 0;	
		
	UInt32 height = 120;
	UInt32 width = 120;
	
	UInt16 errorCode = palmPhotoLibErrUnknown;
	char errorCodeString[maxStrIToALen];

	
	FtrGet (sysFtrCreator, sysFtrNumWinVersion, &winApiVersion);
	if (winApiVersion >= 4)
	{
		WinScreenGetAttribute(winScreenDensity, &displayDensity);					
	}
	
	//unscale the image bounds to native coordinates	
	if(displayDensity > kDensityLow )
	{
		UInt16 oldCoordSys = WinSetCoordinateSystem(displayDensity);
		height = WinScaleCoord(height, false);
		width = WinScaleCoord(width, false);
		WinSetCoordinateSystem(oldCoordSys);			
	}	
	    	
	
	//check if photolib is open, if not, open it
  if (PhotosLibRefNum == sysInvalidRefNum)
    {
	 	Boolean photoLibSupported = false;
	 	photoLibSupported = InitPhotoLib();
	 	
	 	if (photoLibSupported == false)
	 	{
	 	   return;
	 	}	 	
	 	
	}		
	

	//make sure to delete old copies of file
	FileDelete (0, tempJPEGFile );

	//prepare parameters to launch camera and let user capture a picture
	MemSet(&captureParam, sizeof(PalmPhotoCaptureParamV2), 0);

	//only format supported in memory is RGB, so working around that:
	//    -- capturing photo in a stream location as a jpeg
	//    -- adding this photo to an album on the device
	captureParam.fileLocation.fileLocationType = palmPhotoStreamLocation;

	StrCopy(captureParam.fileLocation.file.StreamFile.name, tempJPEGFile);
	captureParam.fileLocation.file.StreamFile.type = appFileCreator;
	captureParam.fileLocation.file.StreamFile.creator = appFileCreator;

	captureParam.imageInfo.width = width;
	captureParam.imageInfo.height = height;
	captureParam.imageInfo.bitsPerPixel = 16;
	captureParam.imageInfo.fileFormat = palmPhotoJPEGFileFormat; 	
	
	imageH = PalmPhotoCaptureImageV2(PhotosLibRefNum, &captureParam );

	
	if (imageH == NULL)
	{
	    FrmCustomAlert(DebugAlert, "In CapturePicture:", " imageH is NULL after photo capture.", " ");
        PhotosLibClose();
	    return;
	}
	
	
	err = PalmPhotoGetLastErrV2(PhotosLibRefNum);
	if (err != errNone)
	{
	   //error definitions can be found in palmOnePhotoCommon.h
	
	    StrIToA(errorCodeString, err);
	        if (err == errorCode)
		  FrmCustomAlert(DebugAlert, "In CapturePicture:", " An error occured: PalmPhotoCaptureImageV2, unknown error.", " ");
		else
		  FrmCustomAlert(DebugAlert, "In CapturePicture: ", errorCodeString, ". ");

    	PalmPhotoCloseImageV2(PhotosLibRefNum, imageH);		    
        PhotosLibClose();		    
		return;
	}
	
    // if we made it this far, there were no Photo Lib errors, and 
    // we have a valid photo handle. now we can proceed with adding our photo
    // from the stream to some album. 


	// get the album id of the album on the device in index 0
	albumID = PalmPhotoGetAlbumIDV2(PhotosLibRefNum, 0, 0);
	
	// add our captured image to the photo album that we just got the ID for
	err = PalmPhotoAddImageV2(PhotosLibRefNum, albumID, imageH, &newImageH);
	
	if (err != errNone)
	{
	   	StrIToA(errorCodeString, err);
	        if (err == errorCode)
		  FrmCustomAlert(DebugAlert, "In CapturePicture:", " An error occured: PalmPhotoAddImageV2, unknown error.", " ");
		else
		  FrmCustomAlert(DebugAlert, "In CapturePicture: ", errorCodeString, ". ");

	    PalmPhotoCloseImageV2(PhotosLibRefNum, imageH);
        PhotosLibClose();		    
		return;
	}
	
	// Freeing image handles
    PalmPhotoCloseImageV2(PhotosLibRefNum, imageH);
    PalmPhotoCloseImageV2(PhotosLibRefNum, newImageH);
	
	
	PhotosLibClose();
	
    //don't leave the file hanging around
	FileDelete (0, tempJPEGFile );

	return;
}


/*
 * FUNCTION: InitPhotoLib
 *
 * DESCRIPTION:
 *
 * This routine initializes the library by checking to make sure it can be 
 * opened. If it cannot be opened, it alerts the user that the photo library is
 * not present.  
 *
 * PARAMETERS:
 *    none
 *
 * RETURNED:
 *    true if the library opened successfully, false if it didn't.
 */
 
Boolean InitPhotoLib()
{
		
	//if Photo Lib is not present then we cannot support pictures
	if(PhotosLibOpen() != errNone)
	{ 
	    FrmCustomAlert(DebugAlert, "In InitPhotoLib:", " The photo library is not present!", " ");
	    return false;
	}	


	return true;
}


/*
 * FUNCTION: PhotosLibOpened
 *
 * DESCRIPTION:
 *
 * This routine loads and opens the Photo Library.
 *
 * PARAMETERS:
 *    none
 *
 * RETURNED:
 *    errNone if everything succeeded, or the error of the last unsuccessful
 *    operation. 
 */

Err PhotosLibOpen()
{
	Err err = errNone;
	Boolean loaded = false;
	
	if(PhotosLibRefNum != sysInvalidRefNum)
		return err;
	
		
	//open Photo library
	err = SysLibFind(PalmPhotoLibName , &PhotosLibRefNum);
	if(err)
	{
		err = SysLibLoad(PalmPhotoLibTypeID,PalmPhotoLibCreatorID, &PhotosLibRefNum);
		
		if(err)
		{
		   return err;
		} 
       
		loaded = true;
	}
	
	err = PalmPhotoLibOpen(PhotosLibRefNum);	
	
	if (err != errNone && loaded)
	{
		SysLibRemove(PhotosLibRefNum);			
	}

	return err;
}


/*
 * FUNCTION: PhotosLibClose
 *
 * DESCRIPTION: 
 *
 * This routine closes and removes the library.
 *
 * PARAMETERS:
 *     none
 *
 * RETURNED:
 *     nothing
 */

void PhotosLibClose()
{
	if (PhotosLibRefNum != sysInvalidRefNum)
	{
		if (PalmPhotoLibClose(PhotosLibRefNum) == errNone)
		{
			SysLibRemove(PhotosLibRefNum);
		}
		PhotosLibRefNum = sysInvalidRefNum;
	}	
}



/*
 * FUNCTION: AppHandleEvent
 *
 * DESCRIPTION: 
 *
 * This routine loads form resources and set the event handler for
 * the form loaded.
 *
 * PARAMETERS:
 *
 * event
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed
 *     to a higher level handler.
 */

static Boolean AppHandleEvent(EventType * eventP)
{
	UInt16 formId;
	FormType * frmP;

	if (eventP->eType == frmLoadEvent)
	{
		/* Load the form resource. */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		/* 
		 * Set the event handler for the form.  The handler of the
		 * currently active form is called by FrmHandleEvent each
		 * time is receives an event. 
		 */
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

		}
		return true;
	}

	return false;
}

/*
 * FUNCTION: AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.
 */

static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do 
	{
		/* change timeout if you need periodic nilEvents */
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				if (! AppHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
			}
		}
	} while (event.eType != appStopEvent);
}

/*
 * FUNCTION: AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * RETURNED:
 *     errNone - if nothing went wrong
 */

static Err AppStart(void)
{

	return errNone;
}

/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

static void AppStop(void)
{
        
	/* Close all the open forms. */
	FrmCloseAllForms();

}

/*
 * FUNCTION: RomVersionCompatible
 *
 * DESCRIPTION: 
 *
 * This routine checks that a ROM version is meet your minimum 
 * requirement.
 *
 * PARAMETERS:
 *
 * requiredVersion
 *     minimum rom version required
 *     (see sysFtrNumROMVersion in SystemMgr.h for format)
 *
 * launchFlags
 *     flags that indicate if the application UI is initialized
 *     These flags are one of the parameters to your app's PilotMain
 *
 * RETURNED:
 *     error code or zero if ROM version is compatible
 */

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	/* See if we're on in minimum required version of the ROM or later. */
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
	{
		if ((launchFlags & 
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
		{
			FrmAlert (RomIncompatibleAlert);

			/* Palm OS versions before 2.0 will continuously relaunch this
			 * app unless we switch to another safe one. */
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

/*
 * FUNCTION: PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * 
 * PARAMETERS:
 *
 * cmd
 *     word value specifying the launch code. 
 *
 * cmdPB
 *     pointer to a structure that is associated with the launch code
 *
 * launchFlags
 *     word value providing extra information about the launch.
 *
 * RETURNED:
 *     Result of launch, errNone if all went OK
 */

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;

	error = RomVersionCompatible (ourMinVersion, launchFlags);
	if (error) return (error);

	switch (cmd)
	{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error) 
				return error;

			/* 
			 * start application by opening the main form
			 * and then entering the main event loop 
			 */
			FrmGotoForm(MainForm);
			AppEventLoop();

			AppStop();
			break;
	}

	return errNone;
}
