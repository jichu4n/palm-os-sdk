/******************************************************************************
 * Copyright (c) 2004-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailAccConfig
 */
 
/**
 *  @brief 	Sample code for the VersaMail Setup Applications
 *          
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Added "About" box and 5-way nav support (02/20/07),compiles under PODS and GCC
 *
 *  @file VMAccConfig.c
 */



#include <PalmOS.h>
#include <HsExt.h>

#include "MainForm.h"
#include "SecondForm.h"
#include "ThirdForm.h"

#include "VMAccConfig_Rsc.h"
#include "utils.h"

Boolean MainFormDoCommand(UInt16);

// Global variable which denotes the different keys
Char *gKeys[NUM_KEYS] = {"incomingserver",
				         "username",
				         "useencryptedpassword",
				         "password",
				         "emailaddress",
				         "outgoingserver",
				         "title",
				         "incomingport",
				         "rootmailbox",
				         "outgoingport",
				         "replyto",
				         "apn",
				         "useesmtp",
				         "samcreatorid",
				         "samtypeid",
				         "useincomingssl",
				         "useoutgoingssl"
				        };
				        
				        

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
	case MainOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
		handled = true;
		break;
	
	default:
		break;
	
	}

	return handled;
}


				       
/**
 * @brief AppHandleEvent
 *	      Handles all events as they happen -
 *		  Registers all event handlers to the respective forms	
 *
 *
 * @param *eventP :   IN:  	Pointer to an event.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/

 
static Boolean AppHandleEvent(EventType * eventP)	
{
	UInt16 		formId = 0;
	FormType 	*frmP = NULL;
	Boolean		handled = false;

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
				handled = true;
				break;

			case SecondForm:
				FrmSetEventHandler(frmP, SecondFormHandleEvent);
				handled = true;
				break;

			case ThirdForm:
				FrmSetEventHandler(frmP, ThirdFormHandleEvent);
				handled = true;
				break;

		}
	}

	return handled;
}


/**
 * @brief AppStart
 *	      Initialization before the app 
 *
 *
 * @param void:   IN:  		None
 * @retval 		Err:		Error Code if there is an error else errNone
 *
 *
 * @version This function was added in version 1.0
 *
 **/
 
static Err AppStart(void)		/* Initialization before the app */
{
	return errNone;
}


/**
 * @brief AppEventLoop
 *	      Main application event loop.
 *
 *
 * @param void:   IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/
 
static void AppEventLoop(void)	/* Motor of the application */
{
	UInt16 error = 0;
	EventType event;

	do 
	{
		/* Timeout is not necessary as the palm doesnt do anything while
		   waiting for an event */
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);
	} while (event.eType != appStopEvent);
}


/**
 * @brief AppStop
 *	      Cleanup after the app is exited 
 *
 *
 * @param void:   IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/
  
static void AppStop(void)		/* Cleanup after the app */
{
    FrmCloseAllForms();
	InitValuesList();
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
