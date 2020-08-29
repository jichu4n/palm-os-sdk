/******************************************************************************
 * Copyright (c) 2004-2006 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup Keyguard
 */
 
/**
 *  @brief 	This sample code shows how to activate the keyguard programmatically, and also disable it 
 *          everytime an incoming SMS alert takes place.
 *
 *
 *  @version 1.0	- Initial Revision 
 *  @version 1.1	- Added "About" box (11/24/06),compiles under PODS and GCC
 *
 *  @file Keyguard.c
 */


#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <SystemMgr.h>
#include <NotifyMgr.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>

#include "Keyguard.h"
#include "Keyguard_Rsc.h"

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/

/**
 * @brief MainFormInit
 *	     This function initializes the main form 
 *
 *
 * @param frmP:   IN:  	Pointer to the main form
 * @retval		void:	None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

static void MainFormInit(FormType *frmP)
{
	FieldType *field;
	const char *wizardDescription;
	UInt16 fieldIndex;

	fieldIndex = FrmGetObjectIndex(frmP, MainDescriptionField);
	field = (FieldType *)FrmGetObjectPtr(frmP, fieldIndex);
	FrmSetFocus(frmP, fieldIndex);

	wizardDescription =
		"Keyguard sample app\n"
		"Creator Code: KgTt\n"
		"\n"
		;
				
	/* dont stack FldInsert calls, since each one generates a
	 * fldChangedEvent, and multiple uses can overflow the event queue */
	FldInsert(field, wizardDescription, StrLen(wizardDescription));
}

/**
 * @brief MainFormDoCommand
 *	    This routine performs the menu command specified.
 *
 *
 * @param commnad:   IN:  	Menu item id.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.1
 *
 **/ 

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	
	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);
	
	/* The About button was hit -- display contents of About box */
	switch (command)
	{
	case MainOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
		handled = true;
		break;
	
	default:
		break;
	}

	return handled;
}

/**
 * @brief MainFormHandleEvent
 *	     Main form event handler.
 *
 *
 * @param *eventP :   IN:  	Pointer to an event.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/

static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP;
	UInt32 locked;
	//UInt16 timer = 0;
	
	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			
			// Set focus on MainLockButton button
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainLockButton));
			
			FrmDrawForm(frmP);
			MainFormInit(frmP);
			handled = true;
			break;
            
        case frmUpdateEvent:
			/* 
			 * To do any custom drawing here, first call
			 * FrmDrawForm(), then do your drawing, and
			 * then set handled to true. 
			 */
			break;
			
		case ctlSelectEvent:
		{
			HsAttrGet(hsAttrKeyboardLocked, 0, &locked);

			if (eventP->data.ctlSelect.controlID == MainLockButton)
			{
				if (!locked)
				{
					locked = 1;
					HsAttrSet(hsAttrKeyboardLocked, 0, &locked);
					//timer = SysSetAutoOffTime(0);
				}
			}
			else if (eventP->data.ctlSelect.controlID == MainUnlockButton)
			{
				if (locked)
				{
					locked = 0;
					HsAttrSet(hsAttrKeyboardLocked, 0, &locked);
					//SysSetAutoOffTime(timer);
				}
			}

			break;
		}
		
		default:
		    break;
	}
    
	return handled;
}

/**
 * @brief  AppHandleEvent
 *	     Main application event handler.
 *	     This routine loads form resources and sets the event handler for
 *	     the form loaded.
 *
 *
 * @param *const eventP:   IN:  	Pointer to an event.
 * @retval 			boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/

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

/**
 * @brief AppEventLoop
 *	     Main application event loop.
 *
 *
 * @param void:   IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

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

/**
 * @brief AppStart
 *	     Called when the application starts - gets the current application's preferences.
 *
 *
 * @param void:   IN:  		None
 * @retval 		Err:		Error Code if there is an error else errNone
 *
 *
 * @version This function was added in version 1.0
 *
 **/

UInt16 gLibRefNum;

static Err AppStart(void)
{
	UInt16 cardNo;
	LocalID dbID;
	Err err = errNone;
	
	err = HsGetPhoneLibrary(&gLibRefNum);
	if(err==errNone) {
		PhnLibOpen(gLibRefNum);
		PhnLibRegister(gLibRefNum, 'KgTt', phnServiceSMS);
	
		// Get the current application path
		SysCurAppDatabase(&cardNo, &dbID);
	
		// Register for sleep, to keep playing even when device display is off
		SysNotifyRegister(cardNo, dbID, sysNotifySleepRequestEvent, NULL, sysNotifyNormalPriority, NULL);

		SysNotifyRegister(cardNo, dbID, phnNBSEvent, NULL, sysNotifyNormalPriority, NULL);
	}
	
	return err;
}

/**
 * @brief AppStop
 *	     Called when the application exits - saves the current state of the application.
 *
 *
 * @param void:   IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

static void AppStop(void)
{
	UInt16 cardNo;
	LocalID dbID;
	
	PhnLibClose(gLibRefNum);
	
	// Get the current application path
	SysCurAppDatabase(&cardNo, &dbID);
	
	// Register for sleep, to keep playing even when device display is off
	SysNotifyUnregister(cardNo, dbID, sysNotifySleepRequestEvent, sysNotifyNormalPriority);

	// SysNotifyUnregister(cardNo, dbID, phnNBSEvent, sysNotifyNormalPriority);

	/* Close all the open forms. */
	FrmCloseAllForms();
}

/**
 * @brief RomVersionCompatible
 *	     Check if the ROM is compatible with the application
 *
 *
 * @param requiredVersion:   	IN:  		The minimal required version of the ROM.
 *     						(see sysFtrNumROMVersion in SystemMgr.h for format)
 * @param launchFlags:   	IN:  		Flags that help the application start itself.
 * @retval 				Err:		Error Code if there is an error else errNone
 *
 *
 * @version This function was added in version 1.0
 *
 **/

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

/**
 * @brief PilotMain
 *	     Main entry point for the application.
 *
 *
 * @param cmd:   			IN:  	Launch Code, tells the application how to start itself.
 * @param cmdPBP:   		IN:  	Parameter Block for a launch code.
 * @param launchFlags:   	IN:  	Flags that help the application start itself.
 * @retval 			  UInt32:	Error Code if there is an error else errNone
 *
 *
 * @version This function was added in version 1.0
 *
 **/

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;
	UInt32 locked = 0;
    SysNotifyParamType *notifyParamP = NULL;
    SleepEventParamType *sleepParamP = NULL;
    PhnEventPtr phnEventP = NULL;
        
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
			notifyParamP = (SysNotifyParamType *)cmdPBP;

			switch(notifyParamP->notifyType) {
			
				case sysNotifySleepRequestEvent:
					sleepParamP = (SleepEventParamType*)notifyParamP->notifyDetailsP;
					//sleepParamP->deferSleep++;
			
					break;
					
				case phnNBSEvent:
					HsAttrGet(hsAttrKeyboardLocked, 0, &locked);
					if (locked)
					{
						locked = 0;
						HsAttrSet(hsAttrKeyboardLocked, 0, &locked);
					}
					break;
			}
			
			break;
			
		case phnLibLaunchCmdEvent:
		
			if (launchFlags & sysAppLaunchFlagSubCall)
			{
				phnEventP = (PhnEventPtr)cmdPBP;
				
				if (phnEventP->eventType == phnEvtMessageInd ||
					phnEventP->eventType == phnEvtSegmentInd)
				{
					HsAttrGet(hsAttrKeyboardLocked, 0, &locked);
					if (locked)
					{
						locked = 0;
						HsAttrSet(hsAttrKeyboardLocked, 0, &locked);
					}
				}
				phnEventP->acknowledge = false;
			}
			break;
	}

	return errNone;
}
