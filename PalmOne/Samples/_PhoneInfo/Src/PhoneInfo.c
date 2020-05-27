/******************************************************************************
 * Copyright (c) 2004 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/
 
/**
 @ingroup _PhoneInfo
 */

/************************************************************** 
 * @file	PhoneInfo.c
 *
 * Phone Info
 *
 * @brief Phone Info Application. Display some information about the phone.
 *
 ***************************************************************/

#include <PalmOS.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>
#include <PmSysGadgetLibCommon.h>
#include <PmSysGadgetLib.h>

#define NON_PORTABLE
#include <HwrMiscFlags.h>  // For hwrOEMCompanyIDHandspring
#undef NON_PORTABLE

#include "PhoneInfo.h"
#include "PhoneUtils.h"

// -------------------------------------------------------------
// Constants
// -------------------------------------------------------------

#define appFileCreator			   'TRIF'

// -------------------------------------------------------------
// Prototypes
// -------------------------------------------------------------

static Err		StartApplication (void);
static void		EventLoop (void);
static void		StopApplication(void);
static Boolean	PhoneInfoEvent(EventPtr event);

static void		DisplayPhoneInfo (FormPtr frmP);
static void 	CommonInitGauge( FormPtr frm );

// Define the minimum OS version we support
#define MIN_VERSION  sysMakeROMVersion(5,0,0,sysROMStageRelease,0)
#define LAUNCH_FLAGS (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)

// -------------------------------------------------------------
// Globals
// -------------------------------------------------------------

UInt16	PhoneLibRefNum;			// Phone library reference number
UInt32	phnType;
Boolean	gALSSupported = false;	// True if device supports Alternate Line Service
								//	(GSM Only)
Int16	gALSLineNumber = 1;		// If devices do not support Alternate Line 
								// Service, then their line number is ALWAYS 1

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
  launchFlags = launchFlags;
  
  if (cmd == sysAppLaunchCmdNormalLaunch)
	{
		error = StartApplication();	// Application start code
		if (error) return error;
		
		EventLoop();				// Event loop
		
		StopApplication ();			// Application stop code
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
	PhnRadioStateType	radioState;

	err = HsGetPhoneLibrary (&PhoneLibRefNum);
	if (err)
	{
		FrmAlert (resAlertPhoneLibrary);
	}

	err = PhnLibGetRadioState (PhoneLibRefNum, &radioState);
  	if (err == errNone)
	{
	  gALSSupported = radioState.alsSupported;
	  if (radioState.activeLineNumber != kLineNumberUnknown)
		{
		  gALSLineNumber = radioState.activeLineNumber;
		}
	}
	
    (void)HsAttrGet (hsAttrPhoneType, 0, &phnType);
	FrmGotoForm (resFormIDPhoneInfo);

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
  UInt16 	err;
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
			case resFormIDPhoneInfo:
			  FrmSetEventHandler (formP, (FormEventHandlerPtr)PhoneInfoEvent);
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
  PhnLibClose (PhoneLibRefNum);
}

/****************************************************************/
/* PhoneInfoEvent */
/**
 * Event handler for the main form
 *
 * \param	event		  IN	Event to process
 *
 * \retval  Boolean
 *
 ****************************************************************/
static Boolean 
PhoneInfoEvent (EventPtr event)
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
		
		/* Set focus on Vibrate button */
		FrmSetFocus(formP, noFocus);
		FrmSetFocus(formP, FrmGetObjectIndex(formP, resButtonVibrate));
			
		DisplayPhoneInfo (formP);
		FrmDrawForm (formP);
     	CommonInitGauge(formP);			
	     	
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

	case keyDownEvent:
	case ctlSelectEvent:
		if ((event->data.ctlSelect.controlID == resButtonVibrate)||
				(event->data.keyDown.keyCode == vchrRockerCenter))
		{
			UInt16 vibrate = kIndicatorAlertAlert;
			UInt16 vibrateCount = 2 ;
			HsIndicatorState (vibrateCount, kIndicatorTypeVibrator, &vibrate);
			handled = true;
			break;
		}	

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



/****************************************************************/
/* DisplayPhoneInfo */
/**
 * Display the phone info on the given form. The form must have
 * the appropriate resources in it.
 *
 * \param	frmP		  IN	Form to display info on.
 *
 * \retval  Boolean
 *
 ****************************************************************/
static void 
DisplayPhoneInfo (FormPtr frmP)
{
	ShowPhoneInfo (PhoneLibRefNum, frmP);
}


/***********************************************************************
 *
 * FUNCTION:    CommonInitGauge
 *
 * DESCRIPTION:	Call this function on form open to initialize the radio and
 *				 battery status gadgets.
 *			.
 *
 * PARAMETERS:  frm
 *
 * RETURNED:    nothing
 *
 * CALLED BY:	NetworkFormInit
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/30/02		Created -- based on David Matiskella's code
 *								 in Soho.
 *
 ***********************************************************************/
static void CommonInitGauge( FormPtr frm )
{
  UInt32 hsStatusVersion;
  UInt16 libRefNum;
  Err error = errNone;

  if (FtrGet (hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion) == 0)
	  {
		HsStatusSetGadgetType (frm, resGadgetMainBatteryLevel, hsStatusGadgetBattery);
		
		HsStatusSetGadgetType (frm, resGadgetSignalLevel, hsStatusGadgetSignal);
		
		error = SysLibFind(kPmSysGadgetLibName, &libRefNum);
		if (error) 
		{
			error = SysLibLoad(kPmSysGadgetLibType, kPmSysGadgetLibCreator, &libRefNum);
		}
			
		if (error == errNone)
		{
		
			PmSysGadgetLibOpen(libRefNum);
			PmSysGadgetStatusGadgetTypeSet (libRefNum, frm, resGadgetBluetooth, pmSysGadgetStatusGadgetBt);
			PmSysGadgetLibClose(libRefNum);
		}
		
	  }
} // CommonInitGauge
 