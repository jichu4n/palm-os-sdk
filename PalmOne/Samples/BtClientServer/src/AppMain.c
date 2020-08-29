 /****************************************************************************
 *
 * Copyright (c) 2007 Palm, Inc. All rights reserved.
 *
 * File: AppMain.c
 *
 *****************************************************************************/

#include <PalmOS.h>

#include "AppResources.h"
#include "SrmUtil.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"
#include "Common.h"
#include "LogUtil.h"

/***********************************************************************
 *
 *	Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			'BTDA'	// register your own at http://www.palmos.com/dev/creatorid/
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

static Char		gFileName[64];
static Char		gFilePath[256];
static Boolean  gIsClient;

/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/


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
static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	FormType * pForm;

	switch (command) {
		case MainOptionsAboutStarterApp:
			pForm = FrmInitForm(AboutForm);
			FrmDoDialog(pForm);					// Display the About Box.
			FrmDeleteForm(pForm);
			handled = true;
			break;

	}
	
	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    MainGetFileInfoCallback
 *
 * DESCRIPTION: File info callback. This function is called by the file browser
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainGetFileInfoCallback(UInt16 volume, const Char* path, const Char* file)
{
	// Create the full path name
	StrCopy(gFilePath, path);

	// Internally, we expect full path
	StrCat(gFilePath, file);
	StrCopy(gFileName, file);
}


/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  pEvent  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandleEvent(EventType* pEvent)
{
	Boolean 	handled   = false;
	FormType*   pForm = FrmGetActiveForm();;
	
	switch (pEvent->eType) {
		case menuEvent:
			return MainFormDoCommand(pEvent->data.menu.itemID);

		case frmOpenEvent:

            // Show currently selected filepath
			if(gFileName) {
			   SetFieldTextFromStr(FrmGetPtr(pForm, MainFileField), gFilePath, false);
			}
	        FrmDrawForm(pForm);
	        
	        // Highlight client button when returning from file browser form
	        if(gIsClient) {
	           FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainClientButton),  1);
	        }
			handled = true;
			break;
		
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID) {
                    
                case MainClientButton:
                    SrmUtilSetupClient();
                    FrmDrawForm(pForm);
                    // Highlight client button when in client mode
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainServerButton),  0);
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainClientButton),  1);
                    gIsClient = true;
                    handled = true;
                    break; 
                    
                case MainServerButton:
                    SrmUtilSetupServer();
                    // Highlight server button when in server mode
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainClientButton),  0);
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainServerButton),  1);                    
                    handled = true;
                    break;
                    
                case MainSendDataButton: // Send data over RFCOMM socket
                    SrmUtilClientSend(gFilePath);
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainClientButton),  0);
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainServerButton),  0);
                    handled = true;
             		break;   	
			
			    case MainReceiveButton: // Listen for data			        
			        SrmUtilServerReceive();
			        FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainClientButton),  0);
                    FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainServerButton),  0);
			        handled = true;
			        break;
			    
			    case MainClosePortButton:
			    	SrmUtilUninit();
			    	// Reset client and server buttons
			    	FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainClientButton),  0);
			    	FrmSetControlValue (pForm,  FrmGetObjectIndex(pForm, MainServerButton),  0);
			    	gIsClient = false;
			    	handled = true;
			    	break;
			    
			    case MainFileBrowseButton:
        			// Set the callback function and return form
					FileBrowserSetCallback(&MainGetFileInfoCallback, MainForm);
					FrmGotoForm(FileBrowserForm);
					handled = true;
        			break;	
        			
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
static Boolean AppHandleEvent(EventType* pEvent)
{
	UInt16 		formId;
	FormType* 	pForm;
	Boolean		handled = false;

	if (pEvent->eType == frmLoadEvent) {
		// Load the form resource.
		formId = pEvent->data.frmLoad.formID;
		pForm = FrmInitForm(formId);
		FrmSetActiveForm(pForm);
        
		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId) {
			case MainForm:
				FrmSetEventHandler(pForm, MainFormHandleEvent);
				break;
            
	        case FileBrowserForm:
	            FrmSetEventHandler(pForm, FileBrowserFormHandleEvent);
	            break;
	            
			default:
				break;
		}
		handled = true;
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
 * RETURNED:     Err value errNone if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStart(void)
{
	FrmGotoForm(MainForm);

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
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{	
	SrmUtilUninit();
	LogUtilUninit();
	FrmCloseAllForms();
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
	Err			error;
	EventType	event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (SysHandleEvent(&event))
			continue;
			
		if (MenuHandleEvent(0, &event, &error))
			continue;
			
		if (AppHandleEvent(&event))
			continue;

		FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
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
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error = errNone;

	switch (cmd) {
		case sysAppLaunchCmdNormalLaunch:
			   if ((error = AppStart()) == 0) {			
				  AppEventLoop();
				  AppStop();
			   }
			   break;
		default:
			break;
	}
	
	return error;
}