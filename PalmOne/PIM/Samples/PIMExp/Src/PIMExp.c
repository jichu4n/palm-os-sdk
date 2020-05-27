/*
 * PIMExp.c
 *
 * main file for PIMExp
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>

#include "PIMExp.h"
#include "PIMExp_Rsc.h"
#include <Hs.h>

/*********************************************************************
 * Entry Points
 *********************************************************************/

void * GetObjectPtr(UInt16 objectID);
Err SetFieldTextFromStr(UInt16 fieldID, Char *stringPtr, Boolean redraw);
Err GetFieldTextFromStr(UInt16 fieldID, Char *stringPtr);
void PrvEnableEditableField(UInt16 fieldID);

extern Boolean ContactsFormHandleEvent(EventType * eventP);
extern Boolean CalendarFormHandleEvent(EventType * eventP);
extern Boolean TasksFormHandleEvent(EventType * eventP);

/*********************************************************************
 * Global variables
 *********************************************************************/

UInt16 TonesLibRefNum = 0;

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/
void PrvEnableEditableField( UInt16 fieldID)
{
    FieldPtr fieldPtr = GetObjectPtr(fieldID);
    FieldAttrType fieldAttr;
    
    FldGetAttributes(fieldPtr, &fieldAttr);
    fieldAttr.editable = 1;
    fieldAttr.underlined = solidUnderline;
    FldSetAttributes(fieldPtr, &fieldAttr);
    
    FldDrawField(fieldPtr);    
}

Err GetFieldTextFromStr( UInt16      fieldID,
                         Char       *stringPtr)
{
    FieldPtr fieldPtr = GetObjectPtr(fieldID);
    Char *p;
    
    if ((p = FldGetTextPtr(fieldPtr)))
    {
        if (stringPtr)
            StrCopy(stringPtr, p);
        else
            stringPtr = p;
    }
    
    return errNone;
}
						 
Err SetFieldTextFromStr( UInt16      fieldID,
		        		 Char	  	*stringPtr,
				         Boolean  	 redraw )
{
	Err		 	err		  	= errNone;
	FieldPtr    fieldPtr    = GetObjectPtr(fieldID);
	MemHandle	textHandle	= FldGetTextHandle( fieldPtr );

    if (!StrLen(stringPtr)) goto Done;
    
	if (textHandle != (MemHandle)NULL)
	{
		FldSetTextHandle( fieldPtr, NULL );
		err = MemHandleResize( textHandle, ( StrLen( stringPtr ) + 1 ) );
		if (err != errNone)
		{
			FldSetTextHandle( fieldPtr, textHandle );
			goto Done;
		}
	}
	else
	{
		textHandle = MemHandleNew( StrLen( stringPtr ) + 1 );
		if (textHandle == (MemHandle)NULL)
		{
			err = memErrNotEnoughSpace;
			goto Done;
		}
	}
	
	StrCopy( (Char *)MemHandleLock( textHandle ), stringPtr );
	MemHandleUnlock( textHandle );
	
	FldSetTextHandle( fieldPtr, textHandle );
	
	if (redraw == true) {
		FldDrawField( fieldPtr );
	}

Done:	
	return ( err );
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

static void MainFormInit(FormType *frmP)
{
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

	switch (command)
	{

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
			if (eventP->data.ctlSelect.controlID == MainContactListButton)
			{
                FrmGotoForm(ContactsForm);
				handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == MainCalendarButton)
			{
			    FrmGotoForm(CalendarForm);
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == MainTasksButton)
			{
			    FrmGotoForm(TasksForm);
			    handled = true;
			}

			break;
		}
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
				
			case ContactsForm:
			    FrmSetEventHandler(frmP, ContactsFormHandleEvent);
			    break;

			case CalendarForm:
			    FrmSetEventHandler(frmP, CalendarFormHandleEvent);
			    break;
			    
		    case TasksForm:
		        FrmSetEventHandler(frmP, TasksFormHandleEvent);
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

  Err err;

  err = SysLibFind (tonesLibName, &TonesLibRefNum);

  if (err)
	  err = SysLibLoad (tonesLibType, tonesLibCreator, &TonesLibRefNum);

  if (err)
  {
     ErrNonFatalDisplay ("Failed to find/load tones library");
	 return err;
  }


  return err;
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
