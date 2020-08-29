/******************************************************************************
 * Copyright (c) 2003-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailFontApp
 */
 
/**
 *  @brief 	Sample code for the VersaMail Font Manager API
 *  		
 *
 *
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Usha Shekar (3/15/07) - Added "About" box, 5-way navigation
 *					  and Doxygen comments. Compiles with PODS and GCC.
 *
 *  @file VMFontApp.c
 */



#include <PalmOS.h>

#include "VMFontApp.h"
#include "VMFontAppRsc.h"
#include "MainForm.h"
#include <PalmOneVMFontLib.h>
#include "utils.h"


UInt16 	gLibRefNum	= 0;
UInt32 	gContext	= 0;

/***********************************************************************
 *
 * FUNCTION:    	AppHandleEvent
 *
 * DESCRIPTION: 	Handles all events as they happen -
 *					Registers all event handlers to the respective forms	
 *
 * PARAMETERS:  	eventP -> Pointer to the structure of the event which is 
 *								being handled
 *
 * RETURNED:    	Boolean handled -> Returns whether the event has been
 *										handled or not
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean AppHandleEvent(EventType * eventP)	/*  */
{
	UInt16 formId;
	FormType *frmP;

	if (eventP->eType == frmLoadEvent)
	{
		/* Load the form resource. */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

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

/***********************************************************************
 *
 * FUNCTION:    	AppStart
 *
 * DESCRIPTION: 	Initialization before the app 	
 *
 * PARAMETERS:  	None
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
 
static Err AppStart(void)		
{
	Err err = errNone;
	
	// Finds the Palm VM Font library and loads its
	err = SysLibFind(fontPickerLibName, &gLibRefNum);
	if(err)
		err = SysLibLoad(fontPickerType, fontPickerCreatorID , &gLibRefNum);

	if(err)
		FrmAlert(LibErrAlert);
	else
		err = VMFontOpen(gLibRefNum, &gContext);
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    	AppEventLoop
 *
 * DESCRIPTION: 	The engine of FontoMatic (If you didn't get that,
 *					read the Palm OS Bible)
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void AppEventLoop(void)	
{
	UInt16 error = errNone;
	EventType event;

	do 
	{ 
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event)) {
		
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
 * FUNCTION:    	AppStop
 *
 * DESCRIPTION: 	Cleanup after the app is exited 	
 *
 * PARAMETERS:  	None
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void AppStop(void)
{
    Err err = errNone;
    
    err = VMFontClose(gLibRefNum, gContext);
    FrmCloseAllForms();
}

/***********************************************************************
 *
 * FUNCTION:    	PilotMain
 *
 * DESCRIPTION: 	The Pilot which drives the engine ( If you didn't get 
 *					that too, read the Palm OS Bible)	
 *
 * PARAMETERS:  	cmd -> Command launch flag
 *					cmdPBP -> Pointer to some app-launch parameters	
 *					launchFlags -> Special launch flags
 *
 * RETURNED:    	Err value	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags) /*Entry to app*/
{
	Err error = errNone;

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
	}

	return errNone;
}
