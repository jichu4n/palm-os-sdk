 /********************************************************************* 
 *Copyright (c) 2000-2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *********************************************************************/

 /** @defgroup GSMStatus GSMStatus Sample Code 
 *	This sample code shows how to retrieve radio connection status, operator name, and voicemail number on
 *	GSM phones.
 *  
 *	@note Please see additional sample codes that demonstrate other telephony library capabilities
 * 
 * @{
 * @}
 */ 
 
 /**	
 @ingroup GSMStatus
 */
 
 /** 
 * @brief The main source file for GSMStatus
 *
 * @file GSMStatus.c
 */

 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <HsNav.h>      // Include for HsExt.h
#include <HsExt.h>      // Include for HsGetPhoneLibrary()
#include <HsPhone.h>    // Include for Phone library API
#include <HsNavCommon.h>

#include "Common.h"
#include "GSMStatus.h"
#include "GSMStatus_Rsc.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

/**
*	@name Global variables and internal constants
*
*/
/*	@{ */

/*********************************************************************
 * Global variables
 *********************************************************************/

UInt16 gLibRef = 0;

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*	@} */

 extern void MainFormInit(FormType *frmP, Boolean redraw);
 Boolean MainFormDoCommand(UInt16 command);
 Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags);
 
 
 /***********************************************************************
 *
 * FUNCTION:    PrvCommonInitGauge
 *
 * DESCRIPTION:	Call this function on form open to initialize the radio and
 *				 battery status gadgets.
 *			.
 *
 * PARAMETERS:  frm
 *
 * RETURNED:    nothing
 *
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/30/02		Created -- based on David Matiskella's code
 *								 in Soho.
 *
 ***********************************************************************/
static void PrvCommonInitGauge( FormPtr frm )
{
  UInt32 hsStatusVersion;
  
  if (FtrGet (hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion) == 0)
	  {
		HsStatusSetGadgetType (frm, NetworkBatteryGadget, hsStatusGadgetBattery);
		HsStatusSetGadgetType (frm, NetworkSignalGadget, hsStatusGadgetSignal);
	  }
} // CommonInitGauge

 
 
/**
 * @brief MainFormDoCommand
 *	This routine performs the menu command specified.
 *
 *
 * @param command	menu item id
 * @retval true if the event was handled and should not be passed to
 *     FrmHandleEvent
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
		case OptionsAbout:
			MenuEraseStatus(0);
			HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
			handled = true;
		break;
	}

	return handled;
}

/**
 * @brief MainFormHandleEvent
 * This routine is the event handler for the "MainForm" of this 
 * application.
 *
 * @param eventP: a pointer to an EventType structure
 *
 * @retval true if the event was handled and should not be passed to
 *     FrmHandleEvent
 *
 **/

static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP = FrmGetActiveForm();;

	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			DbgMessage ("frmOpenEvent \n");
			
			/* Set focus on Refresh button */
			FrmSetFocus(frmP, noFocus);
		  	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainRefreshButton));
			
			FrmDrawForm(frmP);
			
			MainFormInit(frmP, true);
			
			PrvCommonInitGauge(frmP);
			
			handled = true;
			break;
            
        case frmUpdateEvent:
			/* 
			 * To do any custom drawing here, first call
			 * FrmDrawForm(), then do your drawing, and
			 * then set handled to true. 
			 */
			break;

		case nilEvent:
			
			DbgMessage ("nilEvent \n");
			MainFormInit(frmP, true);
		    handled = true;
		    
		    break;
		     
		case keyDownEvent:	 
		case ctlSelectEvent:
		{
			if ((eventP->data.ctlSelect.controlID == MainRefreshButton)||
				(eventP->data.keyDown.keyCode == vchrRockerCenter))
			{
			    PhnPowerType power = phnPowerOff;
			     
			    power = PhnLibModulePowered(gLibRef);
			    
			    if (power == phnPowerOff)   
			        PhnLibSetModulePower(gLibRef, true);
			        
			    DbgMessage ("Refresh \n");
				MainFormInit(frmP, true);
			}

			break;
		}
		
		default:
		    break;
	}
    
	return handled;
}

/**
 * @brief AppHandleEvent
 *	     Main application event handler.
 *
 *
 * @param 			eventP:   IN:  	Pointer to an event.
 * @retval 			boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/
 
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
 
/**
 * @brief AppEventLoop
 *	     Main application event loop.
 *
 *
 * @param 		IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do 
	{
		/* change timeout if you need periodic nilEvents */
		EvtGetEvent(&event, SysTicksPerSecond() / 2);

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

/**
 * @brief AppStart
 * Get the current application's preferences.
 *
 * @retval errNone - if nothing went wrong
 *
 **/

static Err AppStart(void)
{
	Err err=errNone;
	
	err=HsGetPhoneLibrary(&gLibRef);
	if(err==errNone) {
		PhnLibOpen(gLibRef);

	    /* Register for phone event:
    		 We're mainly interested in phnEvtRegistration indication */
		PhnLibRegister(gLibRef, appFileCreator, phnServiceAll);
	}
	return err;
}

/**
 * @brief AppStop
 * Save the current state of the application.
 *
 **/

static void AppStop(void)
{
    /* Unregister for phone event: */
    PhnLibRegister(gLibRef, appFileCreator, 0);

	PhnLibClose(gLibRef);
	
	/* Close all the open forms. */
	FrmCloseAllForms();
}

/**
 * @brief RomVersionCompatible
 * This routine checks that a ROM version is meet your minimum 
 * requirement.
 *
 * @param requiredVersion: minimum rom version required
 *     (see sysFtrNumROMVersion in SystemMgr.h for format)
 * @param launchFlags: flags that indicate if the application UI is initialized
 *     These flags are one of the parameters to your app's PilotMain
 * @retval   error code or zero if ROM version is compatible
 *
 *
 * @version This function was added in version 1.0
 *
 **/

Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
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

/**
 * @brief PilotMain
 * This is the main entry point for the application.
 *
 * @param cmd: word value specifying the launch code. 
 * @param cmdPBP: pointer to a structure that is associated with the launch code
 * @param launchFlags: word value providing extra information about the launch.
 *
 * @retval    Result of launch, errNone if all went OK
 *
 **/

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
			
		case phnLibLaunchCmdEvent:
		{
		    PhnEventPtr pEvent = (PhnEventPtr)cmdPBP;
		    
		    if (pEvent->eventType == phnEvtRegistration)
		    {
		        if (pEvent->data.registration.status != registrationNone)
		        {
		            /* Use this for demo only:
		             Do not block while the phone is being activated */
		            
		            // FrmAlert(RegistrationAlert);
		        }
		    }
		}
	}

	return errNone;
}

/* EOF *******************************************************************************************/