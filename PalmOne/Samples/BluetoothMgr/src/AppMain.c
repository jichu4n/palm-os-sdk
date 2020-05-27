/****************************************************************************
 * Copyright (c) 2007 Palm, Inc. All rights reserved.
 *
 * File: AppMain.c
 *
*****************************************************************************/

#include <PalmOS.h>
#include <HsNav.h>

#include "AppResources.h"
#include "BtUtil.h"

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
	Boolean     libOpened = false;
	FormType*   pForm = FrmGetActiveForm();;
	
	switch (pEvent->eType) {
		case menuEvent:
			return MainFormDoCommand(pEvent->data.menu.itemID);

		case frmOpenEvent:
		    libOpened = BtUtilLibInit();
	        FrmDrawForm(pForm);
	        FrmNavObjectTakeFocus(pForm, MainConnectButton); // Set 5-way
			handled = true;
			break;
		
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID) {
				
				case MainConnectButton: 
				    // Connect to Bluetooth enabled device within range
					BtUtilDiscoverandConnectToDevice();	
			        FrmDrawForm(pForm);
				    FrmNavObjectTakeFocus(pForm, MainConnectButton);			        
                    handled = true;
                    break;
                    
                case MainDisconnectButton:
                    // Disconnect from device
                    FrmNavObjectTakeFocus(pForm, MainDisconnectButton);
                    BtUtilSocketClose();
                    BtUtilDisconnect();
                    handled = true;
                    break;
                    
                case MainDeviceNameButton: 
                    // Get name of device connected to
                    FrmNavObjectTakeFocus(pForm, MainDeviceNameButton);
                    BtUtilGetDeviceName();
                    handled = true;
             		break;   	
             		
                case MainSendDataButton: 
                    // Send data over RFCOMM socket
                    FrmNavObjectTakeFocus(pForm, MainSendDataButton);
                    BtUtilSendData();
                    handled = true;
             		break;   	
			
			    case MainReceiveButton: 
			        // Listen for data
			        FrmNavObjectTakeFocus(pForm, MainReceiveButton);
			        BtUtilSocketListen();
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
	BtUtilSocketClose();
	BtUtilLibUninit();
	
	// Close all the open forms.
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
		    if (BtUtilDeviceHasBt(launchFlags) == errNone) {
			   if ((error = AppStart()) == 0) {			
				  AppEventLoop();
				  AppStop();
			   }
		    }
			break;
		default:
			break;
	}
	
	return error;
}