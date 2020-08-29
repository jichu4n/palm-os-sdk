/*
 * NVFSTester.c
 *
 * main file for NVFSTester
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <PmPalmOSNVFS.h>
#include <HsNav.h>

#include "NVFSTester.h"
#include "NVFSTester_Rsc.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

/*********************************************************************
 * Global variables
 *********************************************************************/

#define testDBName "testDB"
#define testDBType 'DATA'

DmOpenRef gDBRef = 0;

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/

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
	MemHandle recH;
	Char *p;
	
	FieldType *field;
	UInt16 fieldIndex;
	
	fieldIndex = FrmGetObjectIndex(frmP, MainDescriptionField);
	field = (FieldType *)FrmGetObjectPtr(frmP, fieldIndex);
	FrmSetFocus(frmP, fieldIndex);
		
	if (DmNumRecords(gDBRef))
	{
		recH = DmQueryRecord(gDBRef, 0);
		p = MemHandleLock(recH);
		FldInsert(field, p, StrLen(p));
		MemHandleUnlock(recH);
	}
}


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
    const RGBColorType black = { 0, 0, 0 ,0 };	
    const RGBColorType white = { 0, 255, 255 ,255 };	
    const RectangleType	currentColorRect = { {27, 142}, { 19, 17} };
	
	switch(itemID)
	{		
		case HelpAboutNVFSTester:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
			
			WinSetForeColorRGB(&white, NULL);
			WinSetTextColorRGB(&black, NULL);
			WinDrawRectangle(&currentColorRect, 0);
			
			break;
			
	}
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

/*
static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	switch (command)
	{

	}

	return handled;
}
*/


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
	Err error = errNone;
	Boolean handled = false;
	FormType * frmP;
	FieldType *field;
	UInt16 fieldIndex;

	UInt16 index;
	UInt16 recSize;
	MemHandle recH;
	Char *p;
				
	frmP = FrmGetActiveForm();
	fieldIndex = FrmGetObjectIndex(frmP, MainDescriptionField);
	field = (FieldType *)FrmGetObjectPtr(frmP, fieldIndex);

	switch (eventP->eType) 
	{
		case menuEvent:
			HandleMenu(eventP->data.menu.itemID);
			FrmDrawForm(frmP);
			handled = true;
			break;

		case frmOpenEvent:
			FrmDrawForm(frmP);
			MainFormInit(frmP);
			FrmNavObjectTakeFocus(frmP, MainDescriptionField);
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
			if (eventP->data.ctlSelect.controlID == MainClearTextButton)
			{
				recSize = StrLen(FldGetTextPtr(field)) + 1;
				
				index = DmNumRecords(gDBRef);
				
				if (index)
				{
					recH = DmResizeRecord(gDBRef, 0, recSize);
					index = 0;
				}
				else
					recH = DmNewRecord(gDBRef, &index, recSize);				

				p = MemHandleLock(recH);

				DmWrite(p, 0, FldGetTextPtr(field), recSize);

				MemHandleUnlock(recH);

				DmReleaseRecord(gDBRef, index, true);
				
				handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == MainClearSyncButton)
			{
				error = DmSyncDatabase(gDBRef);
				if (error) FrmAlert (RomIncompatibleAlert);
				
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
	gDBRef = DmOpenDatabaseByTypeCreator(testDBType, 'VFSt', dmModeReadWrite);
	if (!gDBRef) return DmGetLastErr();
	
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

	if (gDBRef) DmCloseDatabase(gDBRef);
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
	DmOpenRef dbRef;
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
			
		case sysAppLaunchCmdSyncNotify:
			dbRef = DmOpenDatabaseByTypeCreator(testDBType, 'VFSt', dmModeReadWrite);
			if (!dbRef)
			{
				error = DmCreateDatabase(0, testDBName, 'VFSt', testDBType, false);
				if (error) return error;
			}
			else
				DmCloseDatabase(dbRef);
			break;
	}

	return errNone;
}
