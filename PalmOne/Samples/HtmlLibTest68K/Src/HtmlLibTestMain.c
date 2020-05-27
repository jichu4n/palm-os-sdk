/******************************************************************************
 * HtmlLibTest68K
 *
 * HtmlLibTestMain.c
 *
 * Brief:This sample code shows how to render text and image data using HTML library on
 *		 GSM phones and T|X handheld.
 *
 * Copyright (c) 2004-2006 Palm, Inc. or its subsidiaries.
 *
 * All rights reserved.
 *****************************************************************************/

#include <PalmOS.h>
#include <HsExt.h>
#include <HsNav.h>

#include "HtmlLibTestRsc.h"

/* Application Creator */
#define appFileCreator			'ApkF'

/* global variable */
Int32 gEventWait = 0;
MemHandle gTextHandle = NULL;

/* extern */
Boolean RendererInitialize(void);
void RendererFinalize(void);


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
 ***********************************************************************/
Boolean MainFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP;

	switch (eventP->eType) {
		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);
			FrmNavObjectTakeFocus(frmP, MainContentField);
			handled = true;
			break;

		case frmUpdateEvent:
			FrmDrawForm(FrmGetFormPtr(eventP->data.frmUpdate.formID));
			handled = true;
			break;

		case frmCloseEvent:
            break;
			
		case menuEvent:
			if (eventP->data.menu.itemID == OptionsAboutHtmlLibTest)
			{
				
				UInt16  cardNo;
				LocalID dbID;
				DmSearchStateType searchState;

				DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
										 appFileCreator, true, &cardNo, &dbID);
		
				HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
				
				handled = true;
			}

			break;

		case ctlSelectEvent:
			if (eventP->data.ctlSelect.controlID == MainRenderButton) {
				FrmPopupForm(RenderForm);
			}
			break;

		case popSelectEvent:
			if (eventP->data.popSelect.selection >= 0) {
				MemHandle resH = NULL;
				if (eventP->data.popSelect.selection == 0) {
					resH = DmGetResource(strRsc, Test1String);
				} else {
					resH = DmGetResource(strRsc, Test2String);
				}
				if (resH) {
					FormType *frmP = FrmGetActiveForm();
					FieldType *fldP;
					Char *textP;
					Char *strP = MemHandleLock(resH);
					if (strP) {
						fldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainContentField));
						if (gTextHandle) {
							FldSetTextHandle(fldP, NULL);
							MemHandleFree(gTextHandle);
						}
						gTextHandle = MemHandleNew(MemHandleSize(resH));
						if (gTextHandle) {
							textP = MemHandleLock(gTextHandle);
							StrCopy(textP, strP);
							MemHandleUnlock(gTextHandle);
							FldSetTextHandle(fldP, gTextHandle);
							FldDrawField(fldP);
						}
						MemHandleUnlock(resH);
					}
					DmReleaseResource(resH);
				}
			}
			break;

		default:
			break;
	}

	return handled;
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
 ***********************************************************************/
static Err AppStart(void)
{
	gEventWait = 60000;
	RendererInitialize();
    return errNone;
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
 ***********************************************************************/
static void AppStop(void)
{
    /* Close all the open forms.*/
	RendererFinalize();
    FrmCloseAllForms();
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
 ***********************************************************************/
static Boolean AppHandleEvent(EventPtr eventP)
{
	UInt16 formId;
	FormPtr frmP;

	if (eventP->eType == frmLoadEvent) {
		/* Load the form resource */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		if (!frmP) 
			return false;
		FrmSetActiveForm(frmP);

		/* Set the event handler for the form.  The handler of the currently*/
		/* active form is called by FrmHandleEvent each time is receives an event*/
		switch (formId) {
		case MainForm:
        {
			extern Boolean MainFormHandleEvent(EventPtr eventP);
			FrmSetEventHandler(frmP, MainFormHandleEvent);
        }
        break;
		case RenderForm:
		{
			extern Boolean RenderFormHandleEvent(EventPtr eventP);
			FrmSetEventHandler(frmP, RenderFormHandleEvent);
		}
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
 ***********************************************************************/
static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do {
		EvtGetEvent(&event, gEventWait);

		if (! SysHandleEvent(&event)) {
			/* handle some menu events */
			if (! MenuHandleEvent(0, &event, &error)) {
				if (! AppHandleEvent(&event)) {
					FrmDispatchEvent(&event);
				}
			}
		} 
	} while (event.eType != appStopEvent);
}

/***********************************************************************
 *
 * FUNCTION:    StarterPalmMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code.
 *              cmdPB - pointer to a structure that is associated with the launch code.
 *              launchFlags -  word value providing extra information about the launch.
 *
 * RETURNED:    Result of launch
 *
 ***********************************************************************/
static UInt32 StarterPalmMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;
	switch (cmd) {
    case sysAppLaunchCmdNormalLaunch:
		error = AppStart();
		if (error)
			return error;
		FrmGotoForm(MainForm);
		AppEventLoop();
		AppStop();
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
 ***********************************************************************/
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	return StarterPalmMain(cmd, cmdPBP, launchFlags);
}
