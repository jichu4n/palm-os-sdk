/***************************************************************
 *
 *  Project:
 *	  MMSReceiver
 *
 * Copyright info:
 *	  Copyright (c) Handspring 1999 -- All Rights Reserved
 *
 *  FileName:
 *	  MMSReceiver.c
 * 
 *  Description:
 *	  A sample test application to demonstrate to to register
 *    and received SMS, Email, and WEB system helper notifications.
 *
 * History:
 *	  18-apr-2000  dia  Created
 *	  24-may-2000  vmk	Modified for the semi-final Helpers API
 *	  20-jul-2000  jem	Modified from MailWrapper to simulate phone usage.
 *    06-sep-2000  sv   Modified to conform to new Helper Services API.
 *	  14-oct-2003  fhv  Modified to build under Palm OS 5 SDK.
 *	  18-oct-2003  fhv  Added Email and WEB processing to release a sample code
 *                         for DTS (developers).
 ****************************************************************/

#include <PalmOS.h>

#include <68K/Hs.h>

#include <Helper.h>
#include <HelperServiceClass.h>
#include <HsNav.h>

#include "MMSReceiver.h"


/***********************************************************************
 *
 *	Private Equates
 *
 ***********************************************************************/

//Crazy mod to the file to get it to compile - this is actually registered 
//in HsCreators.h, but would cause registration trouble with the other phone
//incs directory

#define MMSReceiverCreator	  'TRMR'

#define prvMyHelperID	  MMSReceiverCreator


/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/


static void 
DoRegister (void)
{
  UInt16 cardNo;
  LocalID dbID;
  Err err;

  // Get information about this application to send to Notification
  // manager calls...
  SysCurAppDatabase (&cardNo, &dbID);

  err = SysNotifyRegister (cardNo, dbID, sysNotifyHelperEvent, 
                           NULL /*callbackP*/, 
                           sysNotifyNormalPriority, 
                           NULL /*userDataP*/);
                           
  // add another scenario: register successfully.
  if (err == errNone)
 	FrmCustomAlert (rscRegisterSuccessAlertID, "YES!", "", "");
  
  if (err == sysNotifyErrDuplicateEntry)
    {
      FrmCustomAlert (rscRegisterFailedAlertID, "App already registered", "", "");
    }
  else if (err)
    {
      Char buffer[32];
      StrIToH (buffer, err);
      FrmCustomAlert (rscRegisterFailedAlertID, "Err: 0x", buffer, "");
    }
}


static void
DoUnregister (void)
{
  UInt16 cardNo;
  LocalID dbID;
  Err err;

  // Get information about this application to send to Notification
  // manager calls...
  SysCurAppDatabase (&cardNo, &dbID);

  err = SysNotifyUnregister (cardNo, dbID, sysNotifyHelperEvent,
                             sysNotifyNormalPriority);
  
  // add another scenario: register successfully.
  if (err == errNone)
  	FrmCustomAlert (rscUnregisterSuccessAlertID, "YES!", "", "");
  
  if (err == sysNotifyErrEntryNotFound)
    {
      FrmCustomAlert (rscUnregisterFailedAlertID, "App was not registered", "", "");
    }
  else if (err)
    {
      Char buffer[32];
      StrIToH (buffer, err);
      FrmCustomAlert (rscUnregisterFailedAlertID, "Err: 0x", buffer, "");
    }
}
 
 
/***************************************************************
 *	Function:	  MainFrmInit
 *
 *	Summary:	  This routine initializes the "Main View" of our app
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	  MainFrmEventHandler()
 *	
 *	Notes:
 *	
 *	History:
 *	  18-apr-2000  dia Ceated 
 *
 ****************************************************************/
static void 
MainFrmInit (FormPtr frmP)
{
  ;
}


/***************************************************************
 *	Function:	  MainFrmDoCommand
 *
 *	Summary:	  Execute a menu command for our main form 
 *
 *	Parameters:
 *	  command	  IN	menu command to execute
 *
 *	Returns:
 *	  true if event was handled
 *	
 *	Called By: 
 *	  MainFrmEventHandler()
 *	
 *	Notes:
 *	
 *	History:
 *	  18-apr-2000  dia Ceated 
 *
 ****************************************************************/
static Boolean 
MainFrmDoCommand (UInt16 command)
{
Boolean handled = false;

UInt16  cardNo;
LocalID dbID;
DmSearchStateType searchState;

DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 MMSReceiverCreator, true, &cardNo, &dbID);

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



/***************************************************************
 *	Function:	  MainFrmHandleControl
 *
 *	Summary:	  Handle a control select event
 *
 *	Parameters:
 *	  eventP	  IN	The control select event...
 *
 *	Returns:
 *	  true if event was handled
 *	
 *	Called By: 
 *	  MainFrmEventHandler()
 *	
 *	Notes:
 *	
 *	History:
 *	  18-apr-2000  dia Ceated 
 *
 ****************************************************************/
static Boolean 
MainFrmHandleControl (EventPtr eventP)
{
  Boolean	handled = false;

  // Figure out which of the two buttons it was...
  switch (eventP->data.ctlSelect.controlID) 
	{
    case rscMainViewEnableButtonID:
      DoRegister();
      handled = true;
      break;
    
    case rscMainViewDisableButtonID:
      DoUnregister();
      handled = true;
      break;

	default:
	  // allow other events to be handled by frmhandleevent
	  handled = false;
	  break;
	
	}	

  return handled;
}



/***************************************************************
 *	Function:	  MainFrmEventHandler
 *
 *	Summary:	  Event handler for our main form. 
 *
 *	Parameters:
 *	  eventP	  IN	pointer to event to handle
 *
 *	Returns:
 *	  true if event was handled
 *	
 *	Called By: 
 *	  FrmDispatchEvent() when event is for our main form. 
 *	  FrmDispatchEvent() is called by our AppEventLoop() 
 *	
 *	Notes:
 *	
 *	History:
 *	  18-apr-2000  dia  Ceated 
 *
 ****************************************************************/
static Boolean 
MainFrmEventHandler (EventPtr eventP)
{
  FormPtr   formP;
  Boolean   handled = false;

  switch (eventP->eType) 
	{
	// -------------------------------------------------------
	// Init the form
	// -------------------------------------------------------
	case frmOpenEvent:
	  formP = FrmGetActiveForm ();
	  MainFrmInit (formP);
	  FrmDrawForm (formP);
	  FrmNavObjectTakeFocus(formP, rscMainViewEnableButtonID);
	  handled = true;

	  break;
    
	// -------------------------------------------------------
	// Menu commands
	// -------------------------------------------------------
	case menuEvent:
	  handled =  MainFrmDoCommand (eventP->data.menu.itemID);
	  break;



	// -------------------------------------------------------
	// Buttons
	// -------------------------------------------------------
	case ctlSelectEvent:
	  handled =  MainFrmHandleControl (eventP);
	  break;

    

	// -------------------------------------------------------
	// Close our form
	// -------------------------------------------------------
	case frmCloseEvent:
	  break;

	default:
	  break;
	}
  return handled;
}



/***************************************************************
 *	Function:	  AppStart
 *
 *	Summary:	  Application initialization when being launched
 *		as a normal application (i.e. not to execute an action code).
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PilotMain when action code is for a normal launch. 
 *	
 *	Notes:
 *	
 *	History:
 *	  26-Feb-1999  RM  Ceated 
 *
 ****************************************************************/

static UInt32 
AppStart(void)
{
  FrmGotoForm (rscMainViewFormID);
  return 0;
}


/***************************************************************
 *	Function:	  AppStop
 *
 *	Summary:	  Application cleanup. This routine cleans up
 *	  all initialization performed by AppStart(). 
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PilotMain, after EventLoop returns
 *	
 *	Notes:
 *	
 *	History:
 *	  26-Feb-1999  RM  Ceated 
 *
 ****************************************************************/
static void 
AppStop (void)
{
	FrmCloseAllForms();
}



/***************************************************************
 *	Function:	  AppEventLoop
 *
 *	Summary:	  This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 *	Parameters:
 *	  eventP	IN	  pointer to event
 *
 *	Returns:
 *	  true if event was handled. 
 *	
 *	Called By: 
 *	  AppEventLoop. 
 *	
 *	Notes:
 *	
 *	History:
 *	  26-Feb-1999  RM  Ceated 
 *
 ****************************************************************/
static Boolean 
AppHandleEvent (EventPtr eventP)
{
  UInt16 formID;
  FormPtr frmP;
  Boolean handled = false;

  if (eventP->eType == frmLoadEvent) 
	{
	  // Load the form resource.
	  formID = eventP->data.frmLoad.formID;
	  frmP = FrmInitForm (formID);
	  FrmSetActiveForm (frmP);		
	  
	  // Set the event handler for the form.  The handler of the currently
	  // active form is called by FrmHandleEvent each time is receives an
	  // event.
	  switch (formID) 
		{
		  case rscMainViewFormID:
			  FrmSetEventHandler(frmP, MainFrmEventHandler);
			  handled = true;
			  break;

		  default:
			  break;
		}
	}
  return handled;
}



/***************************************************************
 *	Function:	  AppEventLoop
 *
 *	Summary:	  Main event loop for the application
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PilotMain after initializing. 
 *	
 *	Notes:
 *	
 *	History:
 *	  26-Feb-1999  RM  Ceated 
 *
 ****************************************************************/
static void 
AppEventLoop(void)
{
  UInt16		err;
  EventType event;

  do
	{
	  EvtGetEvent (&event, evtWaitForever);

	  if (!SysHandleEvent (&event))
		if (!MenuHandleEvent (0, &event, &err))
		  if (!AppHandleEvent (&event))
			FrmDispatchEvent (&event);

	} while(event.eType != appStopEvent);
}



// Executes the helperNotifyActionCodeExecute request if it is intended
//  for this helper
static Boolean
ProcessExecute (HelperNotifyEventType* helperParamP)
{
  HelperNotifyExecuteType* executeP;
  Boolean handled = false;

  executeP = helperParamP->data.executeP;

  // If this is a request for SMS service and it either targets
  //  this specific helper via its helperID or wildcard (0), then
  //  handle it.
  
  if (	(executeP->serviceClassID == kHelperServiceClassIDMMS) 
			&& 
			((executeP->helperAppID == prvMyHelperID) || (executeP->helperAppID == 0)) )
  {
	UInt32 buttonNo;
    HelperServiceMMSDetailsType*  detailsP = executeP->detailsP;

	buttonNo = FrmCustomAlert(rscMMSReceiverMMSAlertID, "Send MMS", executeP->displayedName, executeP->dataP);
	if (buttonNo == 1)
		{
		if (detailsP && detailsP->message && StrLen(detailsP->message))
			{
			FrmCustomAlert(rscMMSReceiverMessageAlertID, "Subject: ", detailsP->subject, detailsP->message);
			}
		else
			{
			FrmCustomAlert(rscMMSReceiverMessageAlertID, "", "", "(empty message)");
			}
		}
	else if (buttonNo == 2)
		{
		if (detailsP && detailsP->object)
			{

			if ( StrCompare( detailsP->object->mimeType, "application/vnd.palm.bmp" ) )
				FrmCustomAlert(rscMMSReceiverObjectAlertID, "", "", "is not an bitmap and is not displayed");
			else
			{
				WinDrawBitmap( (BitmapPtr) detailsP->object->bufferP, 20, 20);
				FrmCustomAlert(rscMMSReceiverObjectAlertID, "", "", "is displayed");
			}
		
			}
		else
			{
			FrmCustomAlert(rscMMSReceiverMessageAlertID, "", "", "(empty object)");
			}
		}


	executeP->err = 0;
	handled = true;
  }
  
  return (handled);
} // ProcessExecute



// Executes the helperNotifyActionCodeValidate request if it is intended
//  for this helper; the purpose of this request is to determine whether
//  any helper (or a specific helper) for a specific service class exists,
//  without incurring the memory management overhead of
//  helperNotifyActionCodeEnumerate.
static Boolean
ProcessValidate (HelperNotifyEventType* helperParamP)
{
  HelperNotifyValidateType* validateP;
  Boolean handled = false;

  validateP = helperParamP->data.validateP;

  // If this is a request for Voice or Fax service and it either targets
  //  this specific helper via its helperID or wildcard (0), then
  //  handle it.
  if (	(validateP->serviceClassID == kHelperServiceClassIDMMS)
		&& (validateP->helperAppID == prvMyHelperID || validateP->helperAppID == 0)  )
    {
      handled = true;
    }

  return (handled);

} // ProcessValidate


// Executes the helperNotifyActionCodeEnumerate request; the "host" makes
//  this request in order to discover which services are available. All
//  helpers are required to handle this call
static void
ProcessEnumerate (HelperNotifyEventType* helperParamP, UInt32 classID)
{
  HelperNotifyEnumerateListType* nodeP;

  // Allocate the enumeration list node
  nodeP = MemPtrNew (sizeof(*nodeP));
  if (!nodeP)
    {
      ErrNonFatalDisplay ("Out of memory");
      return;
    }

  // Change owner to "system" so the memory will not be deleted automatically
  //  when we return from PilotMain
  MemPtrSetOwner (nodeP, 0);	

  // Initialize the node
  MemSet (nodeP, sizeof (*nodeP), 0);

  // DOLATER; should get names from resources for localization.

  // For display in default helper configuration and other UI
  StrNCopy (nodeP->helperAppName, "MMS Receiver", sizeof(nodeP->helperAppName));

  // For possible display in an action pop-up or similar UI gadget;
  //  it's a good idea to keep this short.
  StrNCopy (nodeP->actionName, "Send MMS", sizeof(nodeP->actionName));

  nodeP->helperAppID = prvMyHelperID;					// our unique helper ID

  nodeP->serviceClassID = classID;	// class of service we support

  // Finally, link in the node into the list
  HelperEnumerateEnqueueEntry ((HelperNotifyEnumerateListType*)helperParamP->data.enumerateP, nodeP);	// HelperEnumNodeEnqueue is a macro

  return;
}
  

/***************************************************************
 *	Function:	  PilotMain
 *
 *	Summary:	  Entry point
 *
 *	Parameters:
 *	  cmd		  IN	Action code for the app. This is one of the
 *						  action codes sysAppLaunchCmdXXX defined in
 *						  <SystemMgr.h>
 *	  cmdPBP	  IN	Parameter block pointer for action code. 
 *	  launchFlags IN	Launch flags, one or more of 
 *						  sysAppLaunchFlagXXX defined in <SystemMgr.h>
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PalmOS when launching the app or asking it to execute an
 *		action code like find or goto. 
 *	
 *	Notes:
 *	
 *	
 *	History:
 *	  26-Feb-1999  RM	Ceated by Ron Marianetti
 *	  24-May-2000  vmk	Modified for the "semi-final" Helpers API
 *
 ****************************************************************/
static UInt32  
MMSReceiverPilotMain (UInt16 cmd, void* cmdPBP, UInt16 launchFlags)
{
  UInt32 err;

  switch (cmd)
    {
    case sysAppLaunchCmdNormalLaunch:
	  {
	    err = AppStart();				// Application start code
	    if (err) return err;

	    AppEventLoop();				// Event loop

	    AppStop ();					// Application stop code
	  }
      break;
    case sysAppLaunchCmdNotify:
      {
        SysNotifyParamType* paramP = (SysNotifyParamType*) cmdPBP;
      
        // Look for notifications from the helper manager...
        if (paramP->notifyType == sysNotifyHelperEvent)
          {
            HelperNotifyEventType* helperParamP = (HelperNotifyEventType*) paramP->notifyDetailsP;

            // Just to verify that nothing broke...
            #if (kHelperNotifyCurrentVersion != 1)
              #error "Helper structure changed--are we still OK?"
            #endif

            // Figure out which type of helper manager event...
            if (helperParamP->actionCode == kHelperNotifyActionCodeExecute)
              {
				// Only one helper is allowed to actually handle this request
                if (!paramP->handled)
                  {
                    paramP->handled = ProcessExecute (helperParamP);
					// Must be careful not to set paramP->handled to false because
					//  others may have already handled this notification
                  }
              }

            else if (helperParamP->actionCode == kHelperNotifyActionCodeValidate)
			  {
				// Only one helper is allowed to actually handle this request
                if (!paramP->handled)
                  {
                    paramP->handled = ProcessValidate (helperParamP);
                  }
			  }

            else if (helperParamP->actionCode == kHelperNotifyActionCodeEnumerate)
              {
				// More than one helper may need to handle this request,
				//  so we don't check the "handled" flag here
                
				ProcessEnumerate (helperParamP, kHelperServiceClassIDMMS);
				
                paramP->handled = true;	  // all helpers are required to handle
										  //  the "Enumerate" request.
              }

			else
			  {
				ErrNonFatalDisplay ("unknown helperNotifyActionCode");
			  }
          }
      }
      break;
    case sysAppLaunchCmdSyncNotify:
    case sysAppLaunchCmdSystemReset:
      // Want to register upon reset and upon card insertion...
      // ...need to set "-resetAfterInstall" to get these notification from a card.
      DoRegister();
      break;
    }

  return 0;
}


// Make a call to a more specific functiom

UInt32  
PilotMain (UInt16 cmd, void* cmdPBP, UInt16 launchFlags)
{
  return MMSReceiverPilotMain (cmd, cmdPBP, launchFlags);
}










