/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	TestFiveWay.c
 *
 * Description:
 *	Test Application for Five Way
 *
 *	Version 1.0 	- Initial Revision (09/19/02)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include <KeyMgr.h>
#include <68K/Hs.h>
#include <68K/Libraries/PmKeyLib/PmKeyLib.h>
#include <common/system/HsKeyCodes.h>

#include "Common.h"
#include "PalmOneNavigator.h"
#include "TestFiveWay.h"
#include "TestFiveWayRsc.h"

/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

static Char* keyName[]  = { "Select", "Up", "Down", "Right", "Left" };
static Char* keyState[] = { "Pressed", "Repeat", "Released", "Not Pressed" };

static UInt16 gLibRefNum;
static Boolean OldLibrary = false;

Boolean MainFormDoCommand(UInt16 command);

/***********************************************************************
 *
 * FUNCTION:    DisplayKeyState
 *
 * DESCRIPTION: Display the 5 Way states.
 *
 * PARAMETERS:	frmP	- Pointer to a form
 *				eventP	- Event to process
 *
 * RETURNED:	True is the key was handled.
 *
 ***********************************************************************/

static Boolean NewDisplayKeyState(FormType *frmP, EventType *eventP)
{
	FieldType 	*fieldP		= NULL;
	ControlType *ctlP		= NULL;
	eventsEnum  eType		= eventP->eType;
	
	WChar 		chr			= NULL;
	UInt16 		keyCode		= NULL;
	UInt16 		modifiers	= NULL;
	
	// Get the KeyEvent fields
	switch(eType)
	{
		case keyUpEvent:
			chr			= eventP->data.keyUp.chr;
			keyCode		= eventP->data.keyUp.keyCode;
			modifiers	= eventP->data.keyUp.modifiers;
			break;

		case keyDownEvent:
			chr			= eventP->data.keyDown.chr;
			keyCode		= eventP->data.keyDown.keyCode;
			modifiers	= eventP->data.keyDown.modifiers;
			break;

		case keyHoldEvent:
			chr			= eventP->data.keyHold.chr;
			keyCode		= eventP->data.keyHold.keyCode;
			modifiers	= eventP->data.keyHold.modifiers;
			break;
		
		default:
			break;			
	}


	//
	// Macro section
	// 

	// Use macro to determine is a Up has been pressed
	ctlP = FrmGetPtr(frmP, MainUpCheckbox);
	if(ctlP) CtlSetValue(ctlP, chr == vchrPageUp && !(eType == keyUpEvent));
	
	// Use macro to determine is a Down has been pressed
	ctlP = FrmGetPtr(frmP, MainDownCheckbox);
	if(ctlP) CtlSetValue(ctlP, chr == vchrPageDown && !(eType == keyUpEvent));
	
	// Use macro to determine is a Right has been pressed
	ctlP = FrmGetPtr(frmP, MainRightCheckbox);
	if(ctlP) CtlSetValue(ctlP, chr == vchrRockerRight && !(eType == keyUpEvent));

	// Use macro to determine is a Left has been pressed
	ctlP = FrmGetPtr(frmP, MainLeftCheckbox);
	if(ctlP) CtlSetValue(ctlP, chr == vchrRockerLeft && !(eType == keyUpEvent));

	// Use macro to determine is a Select has been pressed
	ctlP = FrmGetPtr(frmP, MainSelectCheckbox);
	if(ctlP) CtlSetValue(ctlP, keyCode == keyRockerCenter && (eType == keyUpEvent));
	
	//
	// Chr section
	//
	
	fieldP = FrmGetPtr(frmP, MainChrField);
	
	switch(keyCode)
	{
		case keyRockerUp:
		case keyPageUp:
			SetFieldTextFromStr(fieldP, keyName[1] , true);		
			break;		

		case keyRockerDown:
		case keyPageDown:
			SetFieldTextFromStr(fieldP, keyName[2] , true);		
			break;
					
		case keyRockerRight:
			SetFieldTextFromStr(fieldP, keyName[3] , true);		
			break;
					
		case keyRockerLeft:
			SetFieldTextFromStr(fieldP, keyName[4] , true);		
			break;
					
		case keyRockerCenter:
			SetFieldTextFromStr(fieldP, keyName[0] , true);
			break;
	}

	//
	// Key state section
	//
		
	// Up
	fieldP = FrmGetPtr(frmP, MainUpField);
	if(chr == vchrPageUp)
	{
	  if (eType == keyHoldEvent
	      || (eType == keyDownEvent && (modifiers & autoRepeatKeyMask)))
	    SetFieldTextFromStr(fieldP, keyState[1], true);
	  
	  else if (eType == keyDownEvent)
	    SetFieldTextFromStr(fieldP, keyState[0], true);
	  
	  else if (eType == keyUpEvent)
	    SetFieldTextFromStr(fieldP, keyState[2], true);
	}
    else
      SetFieldTextFromStr(fieldP, keyState[3], true);

	// Down
	fieldP = FrmGetPtr(frmP, MainDownField);
	if(chr == vchrPageDown)
	{
	  if (eType == keyHoldEvent
	      || (eType == keyDownEvent && (modifiers & autoRepeatKeyMask)))
	    SetFieldTextFromStr(fieldP, keyState[1], true);
	  
	  else if (eType == keyDownEvent)
	    SetFieldTextFromStr(fieldP, keyState[0], true);
	  
	  else if (eType == keyUpEvent)
	    SetFieldTextFromStr(fieldP, keyState[2], true);
	}
    else
      SetFieldTextFromStr(fieldP, keyState[3], true);
    
    // Left
    fieldP = FrmGetPtr(frmP, MainLeftField);
	if(chr == vchrRockerLeft)
	{
	  if (eType == keyHoldEvent
	      || (eType == keyDownEvent && (modifiers & autoRepeatKeyMask)))
	    SetFieldTextFromStr(fieldP, keyState[1], true);
	  
	  else if (eType == keyDownEvent)
	    SetFieldTextFromStr(fieldP, keyState[0], true);
	  
	  else if (eType == keyUpEvent)
	    SetFieldTextFromStr(fieldP, keyState[2], true);
	}
    else
      SetFieldTextFromStr(fieldP, keyState[3], true);

	// Right
	fieldP = FrmGetPtr(frmP, MainRightField);
	if(chr == vchrRockerRight)
	{
	  if (eType == keyHoldEvent
	      || (eType == keyDownEvent && (modifiers & autoRepeatKeyMask)))
	    SetFieldTextFromStr(fieldP, keyState[1], true);
	  
	  else if (eType == keyDownEvent)
	    SetFieldTextFromStr(fieldP, keyState[0], true);
	  
	  else if (eType == keyUpEvent)
	    SetFieldTextFromStr(fieldP, keyState[2], true);
	}
    else
      SetFieldTextFromStr(fieldP, keyState[3], true);
	
    // Select
    fieldP = FrmGetPtr(frmP, MainSelectField);
	if(keyCode == vchrRockerCenter)
	{
	  if (eType == keyHoldEvent
	      || (eType == keyDownEvent && (modifiers & autoRepeatKeyMask)))
	    SetFieldTextFromStr(fieldP, keyState[1], true);
	  
	  else if (eType == keyDownEvent)
	    SetFieldTextFromStr(fieldP, keyState[0], true);
	  
	  else if (eType == keyUpEvent)
	    SetFieldTextFromStr(fieldP, keyState[2], true);
	}
    else
      SetFieldTextFromStr(fieldP, keyState[3], true);
		
    // Draw a line for the select button
    WinDrawLine(76, 49, 105, 34);
    
    return true;
}

static Boolean OldDisplayKeyState(FormType *frmP, EventType *eventP)
{
	FieldType 	*fieldP		= NULL;
	ControlType *ctlP		= NULL;
	
	// Get the KeyEvent fields
	WChar 		chr			= eventP->data.keyDown.chr;
	UInt16 		keyCode		= eventP->data.keyDown.keyCode;
	
	//
	// Macro section
	// 
	
	// Use macro to determine is a Select has been pressed
	ctlP = FrmGetPtr(frmP, MainSelectCheckbox);
	if(ctlP) CtlSetValue(ctlP, NavKeyPressed(eventP, Select)?true:false);
	
	// Use macro to determine is a Up has been pressed
	ctlP = FrmGetPtr(frmP, MainUpCheckbox);
	if(ctlP) CtlSetValue(ctlP, NavKeyPressed(eventP, Up)?true:false);
	
	// Use macro to determine is a Down has been pressed
	ctlP = FrmGetPtr(frmP, MainDownCheckbox);
	if(ctlP) CtlSetValue(ctlP, NavKeyPressed(eventP, Down)?true:false);
	
	// Use macro to determine is a Right has been pressed
	ctlP = FrmGetPtr(frmP, MainRightCheckbox);
	if(ctlP) CtlSetValue(ctlP, NavKeyPressed(eventP, Right)?true:false);
	
	// Use macro to determine is a Left has been pressed
	ctlP = FrmGetPtr(frmP, MainLeftCheckbox);
	if(ctlP) CtlSetValue(ctlP, NavKeyPressed(eventP, Left)?true:false);
	
	
	//
	// Chr section
	//
	
	fieldP = FrmGetPtr(frmP, MainChrField);
	switch(chr)
	{
		case vchrPageUp:
			SetFieldTextFromStr(fieldP, keyName[1] , true);
			break;
		case vchrPageDown:
			SetFieldTextFromStr(fieldP, keyName[2] , true);
			break;
		case vchrNavChange:
			if(keyCode & navBitRight)
				SetFieldTextFromStr(fieldP, keyName[3] , true);
			else if(keyCode & navBitLeft)
				SetFieldTextFromStr(fieldP, keyName[4] , true);
			else if(keyCode & navBitSelect)
				SetFieldTextFromStr(fieldP, keyName[0] , true);
			break;
	}
	
	//
	// Key state section
	//
		
	// Up
	fieldP = FrmGetPtr(frmP, MainUpField);
	if( keyCode & navBitUp ) {
		if( keyCode & navChangeUp )
			SetFieldTextFromStr(fieldP, keyState[0], true);	// Press
		else
			SetFieldTextFromStr(fieldP, keyState[1], true); // Repeat
	} else {
		if( keyCode & navChangeUp )
			SetFieldTextFromStr(fieldP, keyState[2], true);	// Released
		else
			SetFieldTextFromStr(fieldP, keyState[3], true);	// Not Pressed
	}
		
		
	////////////////////////////////////////////////////////////
	// Other Way, but doesn't handle multiple keys correctly
	////////////////////////////////////////////////////////////
	// 	if( chr == vchrPageUp ) {
	//		if( modifiers & autoRepeatKeyMask )
	//			SetFieldTextFromStr(fieldP, keyState[1], true);
	//		else
	//			SetFieldTextFromStr(fieldP, keyState[0], true);
	// 	} else {
	// 		SetFieldTextFromStr(fieldP, keyState[2], true);
	// 	}
	////////////////////////////////////////////////////////////
	
	
	
	// Down
	fieldP = FrmGetPtr(frmP, MainDownField);
	if( keyCode & navBitDown ) {
		if( keyCode & navChangeDown )
			SetFieldTextFromStr(fieldP, keyState[0], true);	// Press
		else
			SetFieldTextFromStr(fieldP, keyState[1], true);	// Repeat
	} else {
		if( keyCode & navChangeDown )
			SetFieldTextFromStr(fieldP, keyState[2], true);	// Released
		else
			SetFieldTextFromStr(fieldP, keyState[3], true);	// Not Pressed
	}

    ////////////////////////////////////////////////////////////
	// Other Way, but doesn't handle multiple keys correctly
	////////////////////////////////////////////////////////////
	//
	// if( chr == vchrPageDown ) {
	//    	if( modifiers & autoRepeatKeyMask )
	//    		SetFieldTextFromStr(fieldP, keyState[1], true);
	//   	else
	//    		SetFieldTextFromStr(fieldP, keyState[0], true);
	// } else {
	//   	SetFieldTextFromStr(fieldP, keyState[2], true);
	// }
	////////////////////////////////////////////////////////////
    
    // Left
    fieldP = FrmGetPtr(frmP, MainLeftField);
    if( keyCode & navBitLeft ) {
		if( keyCode & navChangeLeft )
			SetFieldTextFromStr(fieldP, keyState[0], true);	// Press
		else
			SetFieldTextFromStr(fieldP, keyState[1], true);	// Repeat
	} else {
		if( keyCode & navChangeLeft )
			SetFieldTextFromStr(fieldP, keyState[2], true);	// Released
		else
			SetFieldTextFromStr(fieldP, keyState[3], true);	// Not Pressed
	}

	////////////////////////////////////////////////////////////
	// Other Way, but doesn't handle multiple keys correctly
	////////////////////////////////////////////////////////////
	//
	//	if( chr == vchrNavChange && (keyCode & navBitLeft)) {
	//		if( modifiers & autoRepeatKeyMask )
	//			SetFieldTextFromStr(fieldP, keyState[1], true);
	//		else
	//			SetFieldTextFromStr(fieldP, keyState[0], true);
	//	} else {
	//		SetFieldTextFromStr(fieldP, keyState[2], true);
	//	}
	////////////////////////////////////////////////////////////
	
	// Right
	fieldP = FrmGetPtr(frmP, MainRightField);
	if( keyCode & navBitRight ) {
		if( keyCode & navChangeRight )
			SetFieldTextFromStr(fieldP, keyState[0], true);	// Press
		else
			SetFieldTextFromStr(fieldP, keyState[1], true);	// Repeat
	} else {
		if( keyCode & navChangeRight )
			SetFieldTextFromStr(fieldP, keyState[2], true);	// Released
		else
			SetFieldTextFromStr(fieldP, keyState[3], true);	// Not Pressed
	}
		
    ////////////////////////////////////////////////////////////
	// Other Way, but doesn't handle multiple keys correctly
	////////////////////////////////////////////////////////////
	//
	//	if( chr == vchrNavChange && (keyCode & navBitRight)) {
	//		if( modifiers & autoRepeatKeyMask )
	//			SetFieldTextFromStr(fieldP, keyState[1], true);
	//		else
	//			SetFieldTextFromStr(fieldP, keyState[0], true);
	//	} else {
	//		SetFieldTextFromStr(fieldP, keyState[2], true);
	//	}
	////////////////////////////////////////////////////////////
    
    // Select
    fieldP = FrmGetPtr(frmP, MainSelectField);
    if( keyCode & navBitSelect ) {
		if( keyCode & navChangeSelect )
			SetFieldTextFromStr(fieldP, keyState[0], true);	// Press
		else
			SetFieldTextFromStr(fieldP, keyState[1], true);	// Repeat
	} else {
		if( keyCode & navChangeSelect )
			SetFieldTextFromStr(fieldP, keyState[2], true);	// Released
		else
			SetFieldTextFromStr(fieldP, keyState[3], true);	// Not Pressed
	}
		
    ////////////////////////////////////////////////////////////
	// Other Way, but doesn't handle multiple keys correctly
	////////////////////////////////////////////////////////////
	//
	//	if( chr == vchrNavChange && (keyCode & navBitSelect)) {
	//		if( modifiers & autoRepeatKeyMask )
	//			SetFieldTextFromStr(fieldP, keyState[1], true);
	//		else
	//			SetFieldTextFromStr(fieldP, keyState[0], true);
	//	} else {
	//		SetFieldTextFromStr(fieldP, keyState[2], true);
	//	}
	////////////////////////////////////////////////////////////
    
    
    // Draw a line for the select button
    WinDrawLine(76, 49, 105, 34);
    
    return true;
}

/**
 * @brief MainFormDoCommand
 *	This routine performs the menu command specified.
 *
 *
 * @param command	menu item id
 * @retval true if the event was handled and should not be passed to
 *     FrmHandleEvent
 *
 **/

Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appFileCreator, true, &cardNo, &dbID);

	switch (command)
	{
		case OptionsAbout:
			MenuEraseStatus(0);
			HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
			handled = true;
		break;
	}

	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: Form event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

static Boolean MainFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();
	
	EventType cleanEvent;
	cleanEvent.eType = keyDownEvent;
	cleanEvent.data.keyDown.chr = 0;
	cleanEvent.data.keyDown.keyCode = 0;
	cleanEvent.data.keyDown.modifiers = 0;

	
	switch(eventP->eType)
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			FrmDrawForm(frmP);	
			
			// Refresh the display
			if (OldLibrary)
				OldDisplayKeyState(frmP, &cleanEvent);
			else
				NewDisplayKeyState(frmP, &cleanEvent);
			handled = true;
			break;
		
		case keyUpEvent: if (OldLibrary) break;	
		case keyHoldEvent: if (OldLibrary) break;
		case keyDownEvent:
			if (OldLibrary)
				handled = OldDisplayKeyState(frmP, eventP);
			else
				handled = NewDisplayKeyState(frmP, eventP);
			break;
			
		default:
			break;
	}
	
	return handled;
}

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
	UInt16 error;
	Boolean err;
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if(EvtKeydownIsVirtual(&event) && (event.data.keyDown.chr == vchrNavChange)
			&& ((event.data.keyDown.keyCode & (navChangeBitsAll | navBitsAll)) == (navChangeSelect | navBitSelect))
			&& (event.data.keyDown.modifiers & poweredOnKeyMask) )
		{
			// If you want your application to turn on and not to show the pop-up clock
			// Use the following code:
			// // Simply reset the auto off...
			// EvtResetAutoOffTimer();
			
			// Pop the regular clock dialog
			err = SysHandleEvent(&event);
			// Skip the loop
			continue;
		}
		
		// If the event is a five way event, pass it to the form
		// event handler.
		if(IsFiveWayNavEvent(&event) || !SysHandleEvent(&event))
		{
			if (! MenuHandleEvent (0, &event, &error))
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
 * RETURNED:	-
 *
 ***********************************************************************/

static Err AppStart(void)
{
	/*
	// Check for 5 Way
	UInt32 	version = 0;
	Err		err = FtrGet(navFtrCreator, navFtrVersion, &version);
	
	if(err)
		FrmAlert(NotAvailableAlert);
	*/
	
	Err error = errNone;
	
	error = SysLibFind(kPmKeyLibName, &gLibRefNum);
	if (error)
	{
		error = SysLibLoad(sysFileTLibrary, kPmKeyLibCreator, &gLibRefNum);
		
		if (error)
		{
			gLibRefNum = sysInvalidRefNum;
			OldLibrary = true;
			error = errNone;
		}
	}

	return error;
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
