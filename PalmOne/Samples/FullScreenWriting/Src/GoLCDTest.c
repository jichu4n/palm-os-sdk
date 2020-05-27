/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	GoLCDTest.c
 *
 * Description:
 *	Test Application for GoLCD
 *
 *	Version 1.0 	- Initial Revision (03/12/03)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include "Common.h"
#include "GoLCDUtils.h"
#include "GoLCDTest.h"
#include "GoLCDTestRsc.h"
#include "ColorButtonGadget.h"

/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#if 0
#pragma mark Globals
#endif

/*********************************************************************
 * Internal Globals
 *********************************************************************/

static UInt16 				gGoLcdLibRefNum 	= sysInvalidRefNum;
static GoLcdState			gGoLcdState			= OFF;

// Application selected GoLCD states
static GoLcdStatusType		gGoLcdStatus 		= goLcdNotAvailable;
static GoLcdInkStateType	gGoLcdInkState		= goLcdInkDisabled;
static GoLcdColorModeType 	gGoLcdColorMode		= goLcdColorOverride;
static RGBColorType			gGoLcdInkColor		= { 0, 0, 0, 0 };
static GoLcdGsiStateType	gGsiState			= goLcdGsiOverride;
static GoLcdColorModeType 	gGsiColorMode		= goLcdColorDefault;
static RGBColorType			gGsiColor			= { 0, 0, 0, 0 };
static RectangleType		gGoLcdBounds		= { { 0, 0 }, { 0, 0 } };
static UInt32				gGoLcdTimeout		= 0;

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:	PrvGetGoLcdState
 *
 * DESCRIPTION: Get the current GoLCD state and settings
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvGetGoLcdState()
{
	// Get GoLCD settings
	gGoLcdStatus = GoLcdGetStatus(gGoLcdLibRefNum);
	GoLcdGetInkState(gGoLcdLibRefNum, &gGoLcdInkState, &gGoLcdColorMode, &gGoLcdInkColor);
	GoLcdGetGsiState(gGoLcdLibRefNum, &gGsiState, &gGsiColorMode, &gGsiColor);
	GoLcdGetBounds(gGoLcdLibRefNum, &gGoLcdBounds);
	gGoLcdTimeout = GoLcdGetTimeout(gGoLcdLibRefNum, goLcdPenTapMode);
	gGadgetInfo.timeout = gGoLcdTimeout;
	
	// Set color mode to override so we can use the color we did setup
	// The problem occurs when using the Writing Are Preference Panel:
	// The Panel will override the current color with the one saved in
	// its preferences database.
	gGoLcdColorMode = goLcdColorOverride;
	// We do the same with the GSI, but it seems that the Writing Area panel
	// doesn't modifiy this value. Let's not take a chance...
	gGsiColorMode = goLcdColorOverride;
	
	// Check GoLcdStatus
	if( gGoLcdStatus == goLcdEnabled ) {
		gGoLcdState = ENABLED;
		GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, gGoLcdColorMode, &gGoLcdInkColor);
	} else {
		if( gGsiState == goLcdInkDisabled ) {
			gGoLcdState = OFF;
		} else {
			gGoLcdState = DISABLED;
			GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, gGoLcdColorMode, &gGoLcdInkColor);
		}
	}
}

/***********************************************************************
 *
 * FUNCTION:	PrvSetGoLcdState
 *
 * DESCRIPTION: Get the current GoLCD state and settings
 *
 * PARAMETERS:	goLcdState	- State to switch to
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvSetGoLcdState(GoLcdState goLcdState)
{
	gGoLcdState = goLcdState;
	
	switch(goLcdState)
	{
		case OFF:
			// Set the ink state for GoLCD
			gGoLcdInkState = goLcdInkDisabled;
			GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, gGoLcdColorMode, &gGoLcdInkColor);
			// Set the ink state for GSI
			gGsiState = goLcdInkDisabled;
			GoLcdSetGsiState(gGoLcdLibRefNum, gGsiState, gGsiColorMode, &gGsiColor);
			// Turn off GoLCD
			gGoLcdStatus = goLcdDisabled;
			GoLcdSetStatus(gGoLcdLibRefNum, gGoLcdStatus);
			break;
		
		case ENABLED:		
			// Enable GoLCD
			gGoLcdStatus = goLcdEnabled;
			GoLcdSetStatus(gGoLcdLibRefNum, gGoLcdStatus);
			// Set the ink state for GoLCD
			gGoLcdInkState = goLcdInkEnabled;
			GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, gGoLcdColorMode, &gGoLcdInkColor);
			// Set the ink state for GSI
			gGsiState = goLcdInkEnabled;
			GoLcdSetGsiState(gGoLcdLibRefNum, gGsiState, gGsiColorMode, &gGsiColor);
			break;
			
		case DISABLED:		
			// Disable GoLCD
			gGoLcdStatus = goLcdDisabled;
			GoLcdSetStatus(gGoLcdLibRefNum, gGoLcdStatus);
			// Set the ink state for GoLCD
			gGoLcdInkState = goLcdInkEnabled;
			GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, gGoLcdColorMode, &gGoLcdInkColor);
			// Set the ink state for GSI
			gGsiState = goLcdInkEnabled;
			GoLcdSetGsiState(gGoLcdLibRefNum, gGsiState, gGsiColorMode, &gGsiColor);
			break;	
	}
}

/***********************************************************************
 *
 * FUNCTION:	PrvChangeGoLCDBounds
 *
 * DESCRIPTION: Read the bounds fields and set the new GoLCD bounds
 *
 * PARAMETERS:	frmP	- Pointer to main form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void PrvChangeGoLCDBounds(FormType *frmP)
{
	Coord width, height;
	FieldType *fieldP = NULL;

	// Get the X field
	fieldP = FrmGetPtr(frmP, MainXField);
	gGoLcdBounds.topLeft.x = StrAToI(FldGetTextPtr(fieldP));
	
	// Get the Y field
	fieldP = FrmGetPtr(frmP, MainYField);
	gGoLcdBounds.topLeft.y = StrAToI(FldGetTextPtr(fieldP));
	
	// Get the Width field
	fieldP = FrmGetPtr(frmP, MainWidthField);
	gGoLcdBounds.extent.x = StrAToI(FldGetTextPtr(fieldP));
	
	// Get the Height field
	fieldP = FrmGetPtr(frmP, MainHeightField);
	gGoLcdBounds.extent.y = StrAToI(FldGetTextPtr(fieldP));
	
	// Get the display Extent	
	WinGetDisplayExtent(&width, &height);
	
	// Set the new GoLCD Bounds
	GoLcdSetBounds(gGoLcdLibRefNum, &gGoLcdBounds);
}

/***********************************************************************
 *
 * FUNCTION:	PrvFormShowMenu
 *
 * DESCRIPTION: Main form Initialization.
 *
 * PARAMETERS:	frmP	- Pointer to main form
 *
 * RETURNED:	-
 *
 ***********************************************************************/
 
static Boolean PrvFormShowMenu(UInt16 command)
{
	Boolean handled = false;
	FormType *frmP = NULL;
	
	switch (command)
	{
	case OptionsAbout:
		MenuEraseStatus(0);
		
		frmP = FrmInitForm(AboutForm);
		FrmDoDialog(frmP);
		FrmDeleteForm(frmP);
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
 * FUNCTION:	MainFormInit
 *
 * DESCRIPTION: Main form Initialization.
 *
 * PARAMETERS:	frmP	- Pointer to main form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void MainFormInit(FormType *frmP)
{
	Char text[32];
	FieldType *fieldP = NULL;
	ControlType *controlP = NULL;
	
	// Get the GoLCD status
	PrvGetGoLcdState();

	// Set the current GoLCD status
	controlP = FrmGetPtr(frmP, MainOffPushButton);
	CtlSetValue(controlP, (gGoLcdState == OFF));
	controlP = FrmGetPtr(frmP, MainEnablePushButton);
	CtlSetValue(controlP, (gGoLcdState == ENABLED));
	controlP = FrmGetPtr(frmP, MainDisablePushButton);
	CtlSetValue(controlP, (gGoLcdState == DISABLED));
	
	// Set the Ink color
	MemMove(&gGadgetInfo.inkColor, &gGoLcdInkColor, sizeof(RGBColorType));
	
	// Set the Ink Color Mode Type
	controlP = FrmGetPtr(frmP, MainInvertCheckbox);
	CtlSetValue(controlP, (gGoLcdColorMode == goLcdColorInverted));
	
	// Set the GSI color
	MemMove(&gGadgetInfo.gsiColor, &gGsiColor, sizeof(RGBColorType));
		
	// Set the bounds
	fieldP = FrmGetPtr(frmP, MainXField);
	StrIToA(text, gGoLcdBounds.topLeft.x);
	SetFieldTextFromStr(fieldP, text, false);
		
	fieldP = FrmGetPtr(frmP, MainYField);
	StrIToA(text, gGoLcdBounds.topLeft.y);
	SetFieldTextFromStr(fieldP, text, false);

	fieldP = FrmGetPtr(frmP, MainWidthField);
	StrIToA(text, gGoLcdBounds.extent.x);
	SetFieldTextFromStr(fieldP, text, false);

	fieldP = FrmGetPtr(frmP, MainHeightField);
	StrIToA(text, gGoLcdBounds.extent.y);
	SetFieldTextFromStr(fieldP, text, false);
	
	// Set the timeout
	fieldP = FrmGetPtr(frmP, MainTimeoutField);
	StrPrintF(text, "%ld ms", gGoLcdTimeout * sysTicksPerSecond / 1000);
	SetFieldTextFromStr(fieldP, text, false);

}

/***********************************************************************
 *
 * FUNCTION:	MainFormHandleEvent
 *
 * DESCRIPTION: Main form event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

static Boolean MainFormHandleEvent(EventType *eventP)
{
	Char text[32];
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();
	ControlType *controlP = NULL;
	GoLcdStatusType currentStatus = gGoLcdStatus;
	
	switch(eventP->eType)
	{
		//
		// Form Open Event
		//
		case frmOpenEvent:
			MainFormInit(frmP);
			FrmDrawForm(frmP);			
			handled = true;
			break;
			
		case menuEvent:
			handled = PrvFormShowMenu(eventP->data.menu.itemID);
			break;
			
		//
		// Nil Events
		//
		case nilEvent:
			currentStatus = GoLcdGetStatus(gGoLcdLibRefNum);
			if( currentStatus != gGoLcdStatus )
			{
				gGoLcdStatus = currentStatus;
				
				// Set the current GoLCD status
				controlP = FrmGetPtr(frmP, MainOffPushButton);
				CtlSetValue(controlP, (gGoLcdStatus == goLcdNotAvailable));
				controlP = FrmGetPtr(frmP, MainEnablePushButton);
				CtlSetValue(controlP, (gGoLcdStatus == goLcdEnabled));
				controlP = FrmGetPtr(frmP, MainDisablePushButton);
				CtlSetValue(controlP, (gGoLcdStatus == goLcdDisabled));
			}
			break;
		
		//
		// Control Select Event
		//
		case ctlSelectEvent:
			switch(eventP->data.ctlSelect.controlID)
			{
				IndexedColorType colorIndex;
				
				case MainTestButton:
					FrmGotoForm(TestForm);
					handled = true;
					break;
									
				case MainApplyButton:
					PrvChangeGoLCDBounds(frmP);
					handled = true;
					break;
									
				case MainInvertCheckbox:
					if( eventP->data.ctlSelect.on )
						gGoLcdColorMode = goLcdColorInverted;
					else
						gGoLcdColorMode = goLcdColorOverride;
						
					GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, gGoLcdColorMode, &gGoLcdInkColor);
					handled = true;
					break;
										
				case MainOffPushButton:
					PrvSetGoLcdState(OFF);
					FrmDrawForm(frmP);
					handled = true;
					break;

				case MainEnablePushButton:
					PrvSetGoLcdState(ENABLED);
					FrmDrawForm(frmP);
					handled = true;
					break;
					
				case MainDisablePushButton:
					PrvSetGoLcdState(DISABLED);
					FrmDrawForm(frmP);
					handled = true;
					break;
					
				case MainInkColorGadget:
					colorIndex = WinRGBToIndex(&gGoLcdInkColor);
					if( UIPickColor(&colorIndex, NULL, UIPickColorStartPalette, "Choose Ink Color", NULL) )
					{
						WinIndexToRGB(colorIndex, &gGoLcdInkColor);
						MemMove(&gGadgetInfo.inkColor, &gGoLcdInkColor, sizeof(RGBColorType));
						GoLcdSetInkState(gGoLcdLibRefNum, gGoLcdInkState, goLcdColorOverride, &gGoLcdInkColor);
						FrmDrawForm(frmP);
					}
					handled = true;
					break;
				
				case MainGsiColorGadget:
					colorIndex = WinRGBToIndex(&gGsiColor);
					if( UIPickColor(&colorIndex, NULL, UIPickColorStartPalette, "Choose GSI Color", NULL) )
					{
						WinIndexToRGB(colorIndex, &gGsiColor);
						MemMove(&gGadgetInfo.gsiColor, &gGsiColor, sizeof(RGBColorType));
						GoLcdSetGsiState(gGoLcdLibRefNum, gGsiState, goLcdColorOverride, &gGsiColor);
						FrmDrawForm(frmP);
					}
					handled = true;
			}
			break;
			
		//
		// Control Repeat Event
		//
		case ctlRepeatEvent:
			switch(eventP->data.ctlRepeat.controlID)
			{
				case MainScrollUpRepeating:
					gGoLcdTimeout += 10;
					handled = true;
					break;
									
				case MainScrollDownRepeating:
					gGoLcdTimeout -= 10;
					if( gGoLcdTimeout < 10)
						gGoLcdTimeout = 10;
					handled = true;
					break;
			}
			
			// Save the timeout in our global structure
			gGadgetInfo.timeout = gGoLcdTimeout;
			// Set the timeout
			GoLcdSetTimeout(gGoLcdLibRefNum, goLcdPenTapMode, gGoLcdTimeout);
			// Update the field
			StrPrintF(text, "%ld ms", gGoLcdTimeout * sysTicksPerSecond / 1000);
			SetFieldTextFromStr(FrmGetPtr(frmP, MainTimeoutField), text, true);
			handled = false;
			break;
			
		default:
			break;
	}
	
	return handled;
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:	TestFormInit
 *
 * DESCRIPTION: Test form Initialization.
 *
 * PARAMETERS:	frmP	- Pointer to main form
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void TestFormInit(FormType *frmP)
{
	RGBColorType oldForeColor;
	RectangleType rect;
	
	// Copy the Bounds
	RctCopyRectangle(&gGoLcdBounds, &rect);
	
	// Substract frame
	RctInsetRectangle(&rect, 1); 
	
	WinSetForeColorRGB(&gGoLcdInkColor, &oldForeColor);
	WinDrawRectangleFrame(simpleFrame, &rect);
	
	WinSetForeColorRGB(&oldForeColor, NULL);
	
	// Set focus to the field
	FrmSetFocusID(frmP, TestTestField);
}

/***********************************************************************
 *
 * FUNCTION:    TestFormHandleEvent
 *
 * DESCRIPTION: Test form event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

static Boolean TestFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();
	
	switch(eventP->eType)
	{
		case frmOpenEvent:
			FrmDrawForm(frmP);
			TestFormInit(frmP);
			handled = true;
			break;
		
		case ctlSelectEvent:
			FrmGotoForm(MainForm);
			handled = true;
			break;	
		
		case frmUpdateEvent:
			FrmDrawForm(frmP);
			TestFormInit(frmP);
			handled = true;
			break;
			
		default:
			break;
	}
	
	return handled;
}

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
		case MainForm:
			FrmSetEventHandler(frmP, MainFormHandleEvent);
			
			// Set Color Button Gadgets handler
			FrmSetGadgetHandler(frmP,  FrmGetObjectIndex(frmP, MainInkColorGadget), ColorButtonGadgetHandler);
			FrmSetGadgetHandler(frmP,  FrmGetObjectIndex(frmP, MainGsiColorGadget), ColorButtonGadgetHandler);
			break;
			
		case TestForm:
			FrmSetEventHandler(frmP, TestFormHandleEvent);
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
 * FUNCTION:    ApplEventLoop
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
	EventType event;
	Err error = errNone;
	
	do
	{
		EvtGetEvent(&event, sysTicksPerSecond / 10);
				
		if(!SysHandleEvent(&event))
		{
			if(!MenuHandleEvent(0, &event, &error) )
			{
				if(!AppHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
			}
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
 * RETURNED:	Err
 *
 ***********************************************************************/

static Err AppStart(void)
{
	Err 	err = errNone;
	//UInt32 	value = 0;
	UInt16 	prefsSize = 0;
	
	// Try loading the GoLCD library
	err = LoadGoLCDLib(&gGoLcdLibRefNum);
	if( err ) {
		FrmAlert(NotAvailableAlert);
		goto Done;
	}
	
	// Load Preferences
	// I added this feature because if you launch the preference panel
	// for the Writing Area, it would override the current colors. So
	// Now, all you have to do is re-start GoLcdTest to restore your colors.
	// Also useful after a reset...
	prefsSize = sizeof(GadgetInfo);
	if(PrefGetAppPreferences(appFileCreator, appPrefID, &gGadgetInfo, &prefsSize, true) ==  appPrefVersionNum)
	{
		// Read current settings
		PrvGetGoLcdState();
		
		if( gGadgetInfo.inverted )
			gGoLcdColorMode = goLcdColorInverted;
			
		gGsiColor = gGadgetInfo.gsiColor;
		gGoLcdInkColor = gGadgetInfo.inkColor;
		gGoLcdTimeout = gGadgetInfo.timeout;
		GoLcdSetTimeout(gGoLcdLibRefNum, goLcdPenTapMode, gGoLcdTimeout);
		
		// Set the preference color settings
		PrvSetGoLcdState(gGoLcdState);
	}

Done:		
	return err;
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
	
	// Unloading the GoLCD library
	UnloadGoLCDLib(gGoLcdLibRefNum);
	
	// Save Preferences
	PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, &gGadgetInfo, sizeof(GadgetInfo), true);
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
        if (error) 
            return error;

        FrmGotoForm(MainForm);
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
