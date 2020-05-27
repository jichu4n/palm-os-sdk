/** 
 * File  PhoneEventReceiver.c
 *
 * Phone Event Receiver
 *
 * Phone Event Receiver Application. Receives phone events from phone library.
 *
 * License
 * 
 * Copyright c) 2006 Handspring Inc., All Rights Reserved
 *
 * $Id:$
 *
 ***************************************************************/

 /** @defgroup PhoneEventReceiver  PhoneEventReceiver Sample Code
 * This sample code showcases how to use the Palm telephony events
 *
 *
 * @{
 * @}	
 */
 /**
 @ingroup PhoneEventReceiver
 */

/**
* 																		  																  
* @file PhoneEventReceiver.c														  												  
*																		  
* @brief The main source file for this sample code.
*																		
*/

#include <PalmOS.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>

#include "PhoneEventReceiver.h"

 /**
 * @name Global variables and internal constants
 * 
 */
 /*@{*/

// -------------------------------------------------------------
// Constants
// -------------------------------------------------------------
#define appFileCreator			   'TRER'		//Treo Event Receiver

// Define the minimum OS version we support
#define MIN_VERSION  sysMakeROMVersion(5,0,0,sysROMStageRelease,0)
#define LAUNCH_FLAGS (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)

/*@}*/

// -------------------------------------------------------------
// Prototypes
// -------------------------------------------------------------
static Err		StartApplication (void);
static void		EventLoop (void);
static void		StopApplication(void);
static Boolean	PhoneEventReceiverEvent(EventPtr event);
static void 	MainFormInit(FormType *frmP);

// -------------------------------------------------------------
// Globals
// -------------------------------------------------------------
UInt16	PhoneLibRefNum;	// Phone library reference number

/****************************************************************/
/* PilotMain */
/**
 * Main entry point
 *
 * \param   cmd			  IN    
 * \param   cmdPBP		  IN	
 * \param	launchFlags	  IN
 *
 * \retval  UInt32
 *
 ****************************************************************/
UInt32  
PilotMain (UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  Err error = 0;

  cmdPBP = cmdPBP;		// Prevent compiler warning of unused var
  
  if (cmd == sysAppLaunchCmdNormalLaunch)
	{
		//error = RomVersionCompatible (MIN_VERSION, launchFlags);
		if (error) 
		{
			return (0);
		}
		error = StartApplication();	// Application start code
		if (error) return error;
		
		EventLoop();					// Event loop
		
		StopApplication ();			// Application stop code
	}

  // Phone event
  else if ( cmd == phnLibLaunchCmdEvent )
  {
	PhnEventPtr phoneEvent = (PhnEventPtr)cmdPBP;

	if (phoneEvent->eventType == phnEvtStartIncomingCall) // If there is an incoming call
		{

		SndPlaySystemSound(sndAlarm); // Play system alart sound first
		phoneEvent->acknowledge = true;

		}
	}


  return 0;
}



/****************************************************************/
/* StartApplication */
/**
 * Application start 
 *
 * \retval  Err
 *
 ****************************************************************/
static Err
StartApplication(void)
{
	Err err;

	err = HsGetPhoneLibrary (&PhoneLibRefNum);
	if (err)
	{
		FrmAlert (resAlertPhoneLibrary);
	}
	else
	{
		// HsGetPhoneLibrary() already opened Phone Lib.  Remember to close though.

		// Register for voice events.
		err = PhnLibRegister(PhoneLibRefNum, 'TRER', phnServiceVoice);
		if (err) 
			FrmAlert (resAlertPhoneLibrary);

		// Load the form
		FrmGotoForm (resFormIDPhoneEventReceiver);
	}

	return (0);
}


/****************************************************************/
/* EventLoop */
/**
 * Main event loop
 *
 * \retval  none
 *
 ****************************************************************/
static void 
EventLoop(void)
{
//  short		err;
  UInt16 		err;
//  int		formID;
  UInt16	formID;
  FormPtr	formP;
  EventType event;

  do
	{

	  EvtGetEvent (&event, sysTicksPerSecond/2);

	  if (SysHandleEvent (&event)) continue;
	  if (MenuHandleEvent ((void *)0, &event, &err)) continue;

	  if (event.eType == frmLoadEvent)
		{
		  formID = event.data.frmLoad.formID;
		  formP = FrmInitForm (formID);
		  FrmSetActiveForm (formP);

		  switch (formID) 
			{
			case resFormIDPhoneEventReceiver:
			  FrmSetEventHandler (formP, (FormEventHandlerPtr)PhoneEventReceiverEvent);
			  break;
			}
		}

	  FrmDispatchEvent(&event);

	 } while(event.eType != appStopEvent);
}


/****************************************************************/
/* StopApplication */
/**
 * Stop the application
 *
 * \retval  none
 *
 ****************************************************************/
static void 
StopApplication (void)
{
  // Close the forms
  FrmCloseAllForms ();

  // Close the phone library
  if (PhoneLibRefNum)
	PhnLibClose (PhoneLibRefNum);
}

/****************************************************************/
/* PhoneEventReceiverEvent */
/**
 * Event handler for the main form
 *
 * \param	event		  IN	Event to process
 *
 * \retval  Boolean
 *
 ****************************************************************/
static Boolean 
PhoneEventReceiverEvent (EventPtr event)
{
  FormPtr formP;
  UInt16  cardNo;
  LocalID dbID;
  DmSearchStateType searchState;
  Boolean handled = false;

  DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);

  switch (event->eType) 
	{
	case frmOpenEvent:
		formP = FrmGetActiveForm();
		FrmDrawForm (formP);
		MainFormInit(formP);
	     	
		handled = true;
		break;
    
	case menuEvent:
		switch (event->data.menu.itemID)
		{
			case resMenuItemAbout:
				MenuEraseStatus (0);
				HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
				handled = true;
				break;
		}
		break;

    case frmUpdateEvent:
		HsStatusUpdateGadgets ();
    	FrmDrawForm(FrmGetActiveForm());
    	handled = true;
       	break;
           	
	  handled = true;
	  break;

	default:
	  break;
	}
  return handled;
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
	FieldType *field;
	const char *wizardDescription;
	UInt16 fieldIndex;

	fieldIndex = FrmGetObjectIndex(frmP, resFieldPhoneEvent);
	field = (FieldType *)FrmGetObjectPtr(frmP, fieldIndex);
	//FrmSetFocus(frmP, fieldIndex);

	wizardDescription =
		"You have set System Alarm to ring before your ringtone start ringing\n"
		"\n"
		"Try to call your Treo and hear the difference!! \n"
		;
				
	/* dont stack FldInsert calls, since each one generates a
	 * fldChangedEvent, and multiple uses can overflow the event queue */
	FldInsert(field, wizardDescription, StrLen(wizardDescription));
}
 
