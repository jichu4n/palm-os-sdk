/*
 * REMTimer.c
 *
 * main file for REM Timer
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <HsNav.h>              // Include before HsExt.h
#include <HsExt.h>              // For REM sleep notification
#include <palmOneCreators.h>    // For LCD notification

#include "REMTimer.h"
#include "REMTimer_Rsc.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

#define NoSleep         0x00
#define NormalSleep     0x01
#define	RemSleep        0x02
#define DeferredSleep   0x03

/*********************************************************************
 * Global variables
 *********************************************************************/

UInt16 gSleep = NoSleep;
UInt16 gDeferCount = 0;
UInt16 gTimer = 0;
Char   gTimerLabel[256] = "";

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/

void RegisterForNotification(UInt32 notifyType)
{
	UInt16 cardNo	= 0;
	LocalID dbID	= 0;

	SysCurAppDatabase(&cardNo, &dbID);
	SysNotifyRegister(cardNo, dbID, notifyType, 0, sysNotifyNormalPriority, 0);

    return;
}

void UnregisterForNotification(UInt32 notifyType)
{
	UInt16 cardNo	= 0;
	LocalID dbID	= 0;

	SysCurAppDatabase(&cardNo, &dbID);
	SysNotifyUnregister(cardNo, dbID, notifyType, sysNotifyNormalPriority);

    return;
}

void TurnDeviceOff(void)
{
   	EvtEnqueueKey(vchrAutoOff, 0, commandKeyMask);
	return;
}


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

void * GetObjectPtr(UInt16 objectID)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}

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

void MainFormInit(FormType *frmP)
{
	FieldType *field;
	const char *wizardDescription;
	UInt16 fieldIndex;

	fieldIndex = FrmGetObjectIndex(frmP, MainDescriptionField);
	field = (FieldType *)FrmGetObjectPtr(frmP, fieldIndex);
	FrmSetFocus(frmP, fieldIndex);

	wizardDescription =
		"Tap on each button and observe "
		"the different behaviors of the timer and count.\n"
		"\n"
		"Device should go to sleep every 3 seconds "
		"after sleep is initiated.\n"
		"\n"
		"Interesting note: If timer is running but defer count "
		"does not increase, something else in the system is "
		"requesting for REM sleep"
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

Boolean MainFormDoCommand(UInt16 command)
{
    Boolean handled = false;
	FormType *frmAboutP = NULL;
    const RGBColorType black = { 0, 0, 0 ,0 };	
    const RGBColorType white = { 0, 255, 255 ,255 };	
    const RectangleType	currentColorRect = { {27, 142}, { 19, 17} };
	
	switch(command)
	{		
		case HelpAboutREMTimer:
			frmAboutP = FrmInitForm(HelpAboutREMTimer);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
			
			WinSetForeColorRGB(&white, NULL);
			WinSetTextColorRGB(&black, NULL);
			WinDrawRectangle(&currentColorRect, 0);
			handled = true;
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

Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP;

	switch (eventP->eType) 
	{
		case menuEvent:
		    frmP = FrmGetActiveForm();
			handled = MainFormDoCommand(eventP->data.menu.itemID);
			FrmDrawForm(frmP);
			break;

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);
			MainFormInit(frmP);
			handled = true;
			break;
            
		case ctlSelectEvent:
		{
			handled = true;
			
			if (eventP->data.ctlSelect.controlID == MainSleepButton)
			{
				gSleep = DeferredSleep;
                RegisterForNotification(sysNotifySleepRequestEvent);
				FrmPopupForm(SleepForm);
			}
			else if (eventP->data.ctlSelect.controlID == MainRemSleepButton)
			{
				gSleep = RemSleep;
                RegisterForNotification(hsNotifyRemSleepRequestEvent);
				FrmPopupForm(SleepForm);
			}
			else if (eventP->data.ctlSelect.controlID == MainOffButton)
			{
				gSleep = NormalSleep;
				FrmPopupForm(SleepForm);
			}
			else
				handled = false;

			break;
		}
		
		default:
			break;
	}
    
	return handled;
}

/*
 * FUNCTION: SleepFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "SleepForm" of this 
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

Boolean SleepFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	Boolean closeForm = false;
	FormType * frmP;
	RectangleType eraseRect = {{48, 73}, {64, 14}};
	
	switch (eventP->eType) 
	{
		case frmOpenEvent:

            // Initialize timer form:
            // 1. Timer start from 1
            // 2. Display sleep status (deferred, rem, normal)
            
			gTimer = 1;
			
			if (gSleep == DeferredSleep)
    			StrPrintF(gTimerLabel, "Sleep deferred. Timer: %d Count: %d", gTimer, gDeferCount);
    		else if (gSleep == RemSleep)
    		    StrPrintF(gTimerLabel, "REM sleep. Timer: %d  Count: %d", gTimer, gDeferCount);
    		else if (gSleep == NormalSleep)
    		    StrPrintF(gTimerLabel, "Normal sleep. Timer: %d Count: %d", gTimer, gDeferCount);

			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);
		
			WinEraseRectangle(&eraseRect, 0);
			WinDrawChars(gTimerLabel, StrLen(gTimerLabel), 5, 5);

			handled = true;
			break;
            
		case nilEvent:
		
            // Update timer form:
            // 1. Increment timer only if sleep has been initiated
            // 2. Display sleep status (deferred, rem, normal)
            // 3. Close form if device is awake (gSleep == NoSleep)
            
            if (gSleep != NoSleep)		    
       			gTimer++;

			if (gSleep == DeferredSleep)
    			StrPrintF(gTimerLabel, "Sleep deferred. Timer: %d Count: %d", gTimer, gDeferCount);
    		else if (gSleep == RemSleep)
    		    StrPrintF(gTimerLabel, "REM sleep. Timer: %d  Count: %d", gTimer, gDeferCount);
    		else if (gSleep == NormalSleep)
    		    StrPrintF(gTimerLabel, "Normal sleep. Timer: %d Count: %d", gTimer, gDeferCount);
    		else
            {
                closeForm = true;
                handled = true;
                break;
            }
			frmP = FrmGetActiveForm();
			            			
			FrmDrawForm(frmP);
			
			WinEraseRectangle(&eraseRect, 0);
			WinDrawChars(gTimerLabel, StrLen(gTimerLabel), 5, 5);
			
			if (gTimer == 3 || (gTimer != 5 && gTimer % 5 == 0))
				TurnDeviceOff();
				
			handled = true;
			break;
				
		case penDownEvent:
		case keyDownEvent:
            closeForm = true;
			handled = true;
    		break;
		
		default:
			break;
	}
	
    if (closeForm)
    {
		gTimer = 0;
		gDeferCount = 0;
		gSleep = NoSleep;
		
       	UnregisterForNotification(sysNotifySleepRequestEvent);
       	UnregisterForNotification(hsNotifyRemSleepRequestEvent);

		FrmReturnToForm(MainForm);
    }
    
	return handled;
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

Boolean AppHandleEvent(EventType * eventP)
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
				
			case SleepForm:
				FrmSetEventHandler(frmP, SleepFormHandleEvent);
				break;
				
			default:
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

void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do 
	{
		/* change timeout if you need periodic nilEvents */
		EvtGetEvent(&event, SysTicksPerSecond());

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

Err AppStart(void)
{
    RegisterForNotification(kPalmCreatorIDLcdState);

	return errNone;
}

/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

void AppStop(void)
{
	UnregisterForNotification(sysNotifySleepRequestEvent);
	UnregisterForNotification(hsNotifyRemSleepRequestEvent);
	UnregisterForNotification(kPalmCreatorIDLcdState);

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

Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
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
	SysNotifyParamType *pNotify = NULL;
	SleepEventParamType *pSleep = NULL;
	Boolean appIsActive = launchFlags & sysAppLaunchFlagSubCall;
	
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
			
		case sysAppLaunchCmdNotify:

        	if (!appIsActive)
        	{
                ErrNonFatalDisplay("App is not active but is accessing globals");
                return 1;
            }

			pNotify = (SysNotifyParamType *) cmdPBP;
			
			switch (pNotify->notifyType)
			{
				case sysNotifySleepRequestEvent:
					pSleep = (SleepEventParamType *) pNotify->notifyDetailsP;
					pSleep->deferSleep++;
					gDeferCount++;
					break;
					
				case hsNotifyRemSleepRequestEvent:
					pSleep = (SleepEventParamType *) pNotify->notifyDetailsP;
					pSleep->deferSleep++;
					gDeferCount++;
					break;
					
				case kPalmCreatorIDLcdState:
				    if (pNotify->notifyDetailsP == 0)
				    {
				        // LCD is off
				    }
				    else
				    {
				    	// Disable keyguard
				        UInt32 active = 0;
                    	HsAttrSet(hsAttrKeyboardLocked, 0, (UInt32 *)&active);

                        // Stop timer and alert user
				        FrmAlert(TimerAlert);
				    }
				    break;
			}
		
			break;
			
		default:
			break;
	}

	return errNone;
}
