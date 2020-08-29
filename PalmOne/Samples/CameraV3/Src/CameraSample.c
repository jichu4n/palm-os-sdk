/******************************************************************************
 *
 * Copyright (c) 1999-2000 Handspring, Inc. All rights reserved.
 *
 * File: CameraSample.c
 *
 * Release: Palm OS 5.0 (111904)
 *
 *****************************************************************************/

#include <PalmOS.h>

#include <68K\Hs.h>

#include <palmOneCamera.h>

#include <palmOneCreators.h>

#include "CameraSample_Rsc.h"

#include "CameraSettings.h"


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/

// Define the minimum OS version we support (2.0 for now).
#define ourMinVersion		sysMakeROMVersion(2,0,0,sysROMStageRelease,0)
#define kPalmOS10Version	sysMakeROMVersion(1,0,0,sysROMStageRelease,0)


#define previewStartChr	's'
#define previewEndChr	'e'
#define previewChr		'p'
#define captureChr		'c'
#define sleepChr		'x'
#define wakeChr			'w'
#define testPatternChr	't'
#define eraseChr		'r'

#define previewLeft		0
#define previewTop		36
#define previewWidth	160
#define previewHeight	120
#define previewDepth	16

#define statusLeft		80
#define statusTop       1
#define statusWidth     80  
#define statusHeight	12


#define camMaskRegisterSpaceIFP						0x000001	  // Image flow processor registers
#define camAddressSpaceIFP							(((UInt32)camMaskRegisterSpaceIFP) << 16)


/***********************************************************************
 *
 *	Internal Structures
 *
 ***********************************************************************/

/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/

static UInt16			gLibRefNum = sysInvalidRefNum;

static Boolean			Preview			= false;

static Boolean			TestPattern		= false;

static Boolean			OldLibrary		= true;

// change for GCC build by tcui
FormPtr frmP;

/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/

typedef struct PhotoCaptureInfo
{
  WinHandle winH;
  UInt8 *bufferP;
} PhotoCaptureInfo;

Err PhotoCaptureCallback(void *bufferP, UInt32 size, void *userDataP);

/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
  UInt32 romVersion;

  // See if we're on in minimum required version of the ROM or later.
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
  if (romVersion < requiredVersion)
	{
	  if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
		  (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
	  {
		FrmAlert (RomIncompatibleAlert);

		// Palm OS 1.0 will continuously relaunch this app unless we switch to 
		// another safe one.
		if (romVersion <= kPalmOS10Version)
		  {
			AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
		  }
	  }

	  return sysErrRomIncompatible;
	}

  return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    DisplayError
 *
 * DESCRIPTION: This routine displays an error dialog.
 *
 * PARAMETERS:  msg - error message
 *				err - error code
 *
 * RETURNED:    void
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void DisplayError (Char * msg, Err err)
{
  Char		  errStr[32];


  StrPrintF (errStr, "0x%x", err);
  FrmCustomAlert (LibraryLoadErrorAlert, msg, errStr, NULL);
}



#if 0 
#pragma mark ------------------------------------ 
#endif



/***********************************************************************
 *
 * FUNCTION:    MainFormUpdateStatus
 *
 * DESCRIPTION: This routine draws a camera frame.
 *
 * PARAMETERS:  frmP    - pointer to the MainForm form.
 *				message - message to display
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainFormUpdateStatus (FormPtr frmP, Char * message)
{
  RectangleType r;

  RctSetRectangle (&r, statusLeft, statusTop, statusWidth, statusHeight);
  WinEraseRectangle (&r, 0);

  WinDrawChars (message, StrLen(message), statusLeft, statusTop);
}

// add for GCC build by tcui
static void TestMainFormUpdateStatus (Char * message)
{
  RectangleType r;

  RctSetRectangle (&r, statusLeft, statusTop, statusWidth, statusHeight);
  WinEraseRectangle (&r, 0);

  WinDrawChars (message, StrLen(message), statusLeft, statusTop);
}


Err PhotoCaptureCallback(void *bufferP, UInt32 size, void *userDataP)
{
	PhotoCaptureInfo *captureInfoP = (PhotoCaptureInfo *) userDataP;
	
	MemMove(captureInfoP->bufferP, bufferP, size);
	captureInfoP->bufferP += size;
	
	return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    WindowPreview
 *
 * DESCRIPTION: This routine draws a camera frame.
 *
 * PARAMETERS:  none
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err WindowPreview (void)
{
  UInt32		i;
  Err			err = 0;
  UInt16 	  * bits;
  BitmapType  * bitmapP;
  
  // New library stuff
  CamLibBitmapInfoType info;
  CamLibCaptureType capture;
  CamLibSettingType captureFormat;
  PhotoCaptureInfo captureInfo;
  
  CamLibSettingType settingType;

  if (OldLibrary)
  {
	  bitmapP = BmpCreate (previewWidth, previewHeight, previewDepth, 0, &err);

	  if (bitmapP)
		{
		  bits = (UInt16 *) BmpGetBits (bitmapP);
		  
		  for (i = 0; i < previewWidth * previewHeight; i++)
			bits[i] = 0x07e0;

		  err = CameraLibPreview (gLibRefNum, bits, 
			  kCameraImageFormatRGB16, previewWidth, previewHeight);

		  if (! err)
			{
			  WinDrawBitmap (bitmapP, previewLeft, previewTop);
			}

		  BmpDelete (bitmapP);
		}
  }
  else
  {

  	// add for bug fix #210850 by tcui
  	CamLibControl(gLibRefNum, kCamLibCtrlCaptureSizeGet, &settingType);
	settingType.type = kCamLibImageSizeQVGA;
	err = CamLibControl(gLibRefNum, kCamLibCtrlCaptureSizeSet, &settingType);
  	// end of add
  	
  	// set the capture format
  	captureFormat.type = kCamLibCaptureDataFormatRGB565;
  	CamLibControl(gLibRefNum, kCamLibCtrlCaptureFormatSet, (void *) &captureFormat);
  	
  	// get bitmap info
  	CamLibControl(gLibRefNum, kCamLibCtrlCaptureGetBitmapInfo, (void *) &info);
  	
  	// prepare offscreen window
  	captureInfo.winH = WinCreateOffscreenWindow(info.width / 2, info.height / 2, nativeFormat, &err);
  	captureInfo.bufferP = BmpGetBits(WinGetBitmap(captureInfo.winH));
  	
  	capture.userDataP = &captureInfo;
  	capture.callbackP = PhotoCaptureCallback;
  	
  	// take a picture
  	CamLibControl(gLibRefNum, kCamLibCtrlCapture, &capture);
  	
  	// display the picture
	WinDrawBitmap(WinGetBitmap(captureInfo.winH), previewLeft, previewTop);
	
	WinDeleteWindow(captureInfo.winH, true);
	
  }
  
  return err;
}

/***********************************************************************
 *
 * FUNCTION:    WindowEraseImage
 *
 * DESCRIPTION: This routine erases the image.
 *
 * PARAMETERS:  none
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void WindowEraseImage (void)
{
  RectangleType	r;
  RGBColorType		rgbColor;
  IndexedColorType	color;



  RctSetRectangle (&r, previewLeft, previewTop, previewWidth, previewHeight);


  rgbColor.index = 0;	// index of color or best match to cur CLUT or unused.
  rgbColor.r = 255;		// amount of red, 0->255
  rgbColor.g = 0;		// amount of green, 0->255
  rgbColor.b = 0;		// amount of blue, 0->255
  color = WinRGBToIndex (&rgbColor);

  WinDrawRectangle (&r, 0);

  color =  WinSetForeColor (color);
}





/***********************************************************************
 *
 * FUNCTION:    MainFormHandleKeyEvent
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandleKeyEvent(FormPtr frmP, EventPtr eventP)
{
  Err		err = 0;
  UInt16    chr;
  Boolean	handled = false;

  
  chr = eventP->data.keyDown.chr;

  if (TxtCharIsValid(chr))
	{
	  switch (chr)
		{
		  case previewStartChr:
			MainFormUpdateStatus (frmP, "Preview started");
			err = TurnPreviewOn(gLibRefNum, OldLibrary);
			Preview = true;
			break;

		  case previewEndChr:
			MainFormUpdateStatus (frmP, "Preview stopped");
		    err = TurnPreviewOff(gLibRefNum, OldLibrary);
			Preview = false;
			break;
			
		  case captureChr:
		  case previewChr:
			if (Preview)
			  err = TurnPreviewOff(gLibRefNum, OldLibrary);
			Preview = false;
			MainFormUpdateStatus (frmP, "Capture ...");
		    err = WindowPreview ();
			MainFormUpdateStatus (frmP, "Capture done");
		   break;

		  case sleepChr:
			MainFormUpdateStatus (frmP, "Sleep");
		    err = CameraLibrarySleep (gLibRefNum, OldLibrary);
			break;

		  case wakeChr:
			MainFormUpdateStatus (frmP, "Wake");
			err = CameraLibraryWake (gLibRefNum, OldLibrary);
			break;

		  case eraseChr:
			WindowEraseImage ();
			break;

		  case testPatternChr:
			if (! TestPattern)
			  {
				TestPattern = true;
				MainFormUpdateStatus (frmP, "Test on");
			    err = TurnPreviewOn(gLibRefNum, OldLibrary);
				Preview = true;

				err = CameraLibRegisterSet (gLibRefNum, camAddressSpaceIFP | 0x48, 0x04);
				if (err)
				  {
					DisplayError ("Get error:", err);
				  }
			  }
			else
			  {
				TestPattern = false;
				MainFormUpdateStatus (frmP, "Test off");
				err = CameraLibRegisterSet (gLibRefNum, 0x48, 0x00);
				if (err)
				  {
					DisplayError ("Get error:", err);
				  }
			  }

			if (err)
			  {
				DisplayError ("Get error:", err);
				break;
			  }

			break;

		}
	} 


  if (err) 
	{
	  FrmCustomAlert(LibraryLoadErrorAlert, "Error:", NULL, NULL);
	}

  return handled;
}


// Implement Menu/About function
#define appFileCreator			   'TRCS'
/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean 
MainFormDoCommand (
		UInt16 							command )
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

// About function end

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  eventP  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandleEvent (EventPtr eventP)
{
	Boolean 	handled = false;
	FormPtr 	frmP 	= FrmGetActiveForm();


	switch (eventP->eType) 
	  {
		case menuEvent:
		  {
		    if (Preview)
		    {
	  		  TurnPreviewOff(gLibRefNum, OldLibrary);
	  		  Preview = false;
	  		}			
			handled = MainFormDoCommand(eventP->data.menu.itemID); 
			if (!Preview)
		    {
		      MainFormUpdateStatus (frmP, "Preview started");
	  		  TurnPreviewOn(gLibRefNum, OldLibrary);
	  		  Preview = true;
	  		}
	  		break;  
		  }

		case frmUpdateEvent:
		case frmOpenEvent:
		  {
		  	FrmSetFocus(frmP, noFocus);
		  	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainPreviewButton));	
			FrmDrawForm (frmP);
			handled = true;
			MainFormUpdateStatus (frmP, "Preview started");
			TurnPreviewOn(gLibRefNum, OldLibrary);
			Preview = true;
			break;
		  }

		case frmCloseEvent:
		  {
		    if (Preview)
		    {
	  		  TurnPreviewOff(gLibRefNum, OldLibrary);
	  		  Preview = false;
	  		}
	  		
			break;
		  }


		case ctlSelectEvent:
		  {
			switch (eventP->data.ctlEnter.controlID)
			  {
				case MainPreviewButton:     
				  //  The Capture button is pressed.
				  // change for GCC build by tcui
				  //frmP = FrmGetActiveForm();
				  if (Preview)
			  		TurnPreviewOff(gLibRefNum, OldLibrary);
				  Preview = false;

				  // change for GCC build by tcui
				  // lost the correct frmP value when return. codewarrior doesn't compile the second parameter
				  //MainFormUpdateStatus (frmP, "Capture ..."); 
				  //TestMainFormUpdateStatus ("Capture ...");
				  TestMainFormUpdateStatus ("Capture done");
		         
		          WindowPreview ();
		          
				  // change for GCC build by tcui
				  // lost the correct frmP value when return. codewarrior doesn't compile the second parameter
				  //MainFormUpdateStatus (frmP, "Capture done"); 
				  // this call will cause crash in GCC build
				  //TestMainFormUpdateStatus ("Capture done");

				  handled = true;
				  break;
			  }
			break;
		  }

		case keyDownEvent:
		  {
			handled = MainFormHandleKeyEvent (frmP, eventP);
		  }

		default:
			break;	
	  }
	
	return handled;
}




/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventPtr eventP)
{
  UInt16 formId;
  FormPtr frmP;

  if (eventP->eType == frmLoadEvent)
	{
	  // Load the form resource.
	  formId = eventP->data.frmLoad.formID;
	  frmP = FrmInitForm(formId);
	  FrmSetActiveForm(frmP);

	  // Set the event handler for the form.  The handler of the currently
	  // active form is called by FrmHandleEvent each time is receives an
	  // event.
	  switch (formId)
		{
		  case MainForm:
			FrmSetEventHandler(frmP, MainFormHandleEvent);
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
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	UInt16	  error;
	EventType event;
	UInt32	  timeout = evtWaitForever;

	do {
		EvtGetEvent(&event, timeout);

		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value 0 if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStart(void)
{
  Err 		error = errNone;
  UInt32	toDepth = 16;
  UInt32	currentDepth;
  UInt32	supportedDepths;
  
  UInt16 cardNo	= 0;
  LocalID dbID	= 0;



  // Get current display depth.
  WinScreenMode (winScreenModeGet, NULL, NULL, &currentDepth, NULL);

  // Get supported display depths.
  WinScreenMode (winScreenModeGetSupportedDepths, NULL, NULL, &supportedDepths, NULL);

  if ((currentDepth != toDepth) && (supportedDepths & (1 << (toDepth - 1))) )
    {
	  error = WinScreenMode (winScreenModeSet, NULL, NULL, &toDepth, NULL);	

	  if (error) 
		{
		  FrmCustomAlert(LibraryLoadErrorAlert, "WinScreenMode error:", NULL, NULL);
		  return error;
		}
    }

  // Library load (Try loading library from Treo 600)
  if( gLibRefNum == sysInvalidRefNum ) 
	{
	  error = SysLibFind(kCameraLibName, &gLibRefNum);
	  if (error) 
		{
		  error = SysLibLoad(sysFileTLibrary, kCameraLibCreator, &gLibRefNum);

		  if (error) 
			  gLibRefNum = sysInvalidRefNum;
		}
	}

  // Library load (Try loading library from Ace)
  if (gLibRefNum == sysInvalidRefNum)
	{
	  error = SysLibFind(kCamLibName, &gLibRefNum);
	  if (error != errNone)
	  	error = SysLibLoad(kCamLibType, kCamLibCreator, &gLibRefNum);
	  	
	  if (error)
	    {
	      FrmCustomAlert(LibraryLoadErrorAlert, "Unable to load Camera library", NULL, NULL);
	      return error;
	    }
	    
	  OldLibrary = false;
    }
    

  SysCurAppDatabase(&cardNo, &dbID);    
  SysNotifyRegister(cardNo, dbID, kPalmCreatorIDLcdState, 0, sysNotifyNormalPriority, 0);
    
  error = CameraLibraryOpen(gLibRefNum, OldLibrary);
	  
  return error;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{
	UInt16 cardNo	= 0;
	LocalID dbID	= 0;

	// Close all the open forms.
	FrmCloseAllForms ();

	SysCurAppDatabase(&cardNo, &dbID);
	SysNotifyUnregister(cardNo, dbID, kPalmCreatorIDLcdState, sysNotifyNormalPriority);
	
	// Unload the Camera library
	if( gLibRefNum != sysInvalidRefNum ) 
	  {
		CameraLibraryClose(gLibRefNum, OldLibrary);
		SysLibRemove(gLibRefNum);
	  }

}


/***********************************************************************
 *
 * FUNCTION:    AppMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with 
 *                      the launch code. 
 *              launchFlags - word value providing extra information 
 *                            about the launch.
 *
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static UInt32 AppMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  Err error;

  error = RomVersionCompatible (ourMinVersion, launchFlags);
  if (error) return (error);

  switch (cmd)
	{
	  case sysAppLaunchCmdNormalLaunch:
		{
		  error = AppStart();
		  if (error) 
			return error;
			  
		  FrmGotoForm(MainForm);
		  AppEventLoop();
		  AppStop();
		  break;
		}
		case sysAppLaunchCmdNotify:
		{
			SysNotifyParamType* pNotify = (SysNotifyParamType *) cmdPBP;
			switch (pNotify->notifyType)
			{
				case kPalmCreatorIDLcdState:
				{
					if (pNotify->notifyDetailsP != 0)
					{
						// LCD is turning on
						
						UInt16 frmID 	= FrmGetActiveFormID();
						if (frmID == MainForm)
						{
				
							// if keyguard is on, turn it off, as 
							// the camera preview goes straight to screen by communicating
							// with the camera driver, therefore any forms will be hidden
							// by it if displayed. 
							
							Err err;
							UInt32 locked = 0;
							UInt32 value;
							
						 	if (FtrGet(hsFtrCreator, hsFtrIDVersion, &value) == errNone)
							{
								err = HsAttrGet(hsAttrKeyboardLocked, 0, &locked);
								if ( (err == errNone) && (locked) )
								{
									locked = 0; 
									err = HsAttrSet(hsAttrKeyboardLocked, 0, &locked);
								}
							}

							FrmUpdateForm(MainForm, frmRedrawUpdateCode);

						}
						
					}
					
					break;
				}
				
				default:
					break;
			}
		}

	  default:
		break;
	}
  
  return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  return AppMain(cmd, cmdPBP, launchFlags);
}
