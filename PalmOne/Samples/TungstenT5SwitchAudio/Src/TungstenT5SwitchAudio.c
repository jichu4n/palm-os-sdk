/***********************************************************************
 *
 * Copyright (c) 2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	TungstenT5SwitchAudio.c
 *
 * Description:
 *	Main file
 *
 *	Version 1.0 	- Initial Revision (03/20/05)
 *
 ***********************************************************************/


#include <PalmOS.h>
#include <PceNativeCall.h>
#include <HsExt.h>  //Include for About box
#include "Common.h"
#include "TungstenT5SwitchAudio.h"
#include "TungstenT5SwitchAudioRsc.h"


static MemPtr PrvLockResource(DmResType type, DmResID resID)
{
	MemHandle 	resH = NULL;
	MemPtr		resP = NULL;
	
	resH = DmGetResource(type, resID);
	resP = MemHandleLock(resH);
	DmReleaseResource(resH);
	
	return resP;
}

static void PrvUnlockResource(DmResType type, DmResID resID)
{
	MemHandle resH = NULL;
	
	resH = DmGetResource(type, resID);
	MemHandleUnlock(resH);
	DmReleaseResource(resH);
}

Boolean IsDeviceT5 ()
{
	UInt32 deviceID=0;
	FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
	if (deviceID == 'TnT5')
    	return true;
  	else 
  		return false;
}

/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION:
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void MainFormInit(FormType *frmP)
{
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDeinit
 *
 * DESCRIPTION:
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void MainFormDeinit(FormType *frmP)
{
}

/**********************
* Menu handler
**********************/
Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appCreatorID, true, &cardNo, &dbID);

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

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: Main form event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event.
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

static Boolean MainFormHandleEvent(EventType *eventP)
{
	Boolean			 handled			= false;
	FormType		*frmP				= FrmGetActiveForm( );
//	Err				 err				= errNone;
	UInt32	 		 result				= 0;
//	UInt16			 refNum				= 0;
	UInt32			*action				= (UInt32 *)NULL;
	NativeFuncType	*ARMletP			= NULL;
	
		
	switch(eventP->eType)
	{
		//case menuEvent:
			//return MainFormDoCommand(eventP->data.menu.itemID);
	
		case frmOpenEvent:
			MainFormInit(frmP);
			FrmDrawForm(frmP);
			handled = true;
			
			
			break;
			
		case frmCloseEvent:
			MainFormDeinit(frmP);
			break;
			
		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainSwitchAudioToMultiConnectorButton:					
					action		= MemPtrNew( sizeof(UInt32) );
					ARMletP 	= (NativeFuncType *)PrvLockResource( 'ARMC', 1 );
					*action 	= Swap32( 1 );
					result 		= PceNativeCall( ARMletP, action );
					PrvUnlockResource( 'ARMC', 1 );
					MemPtrFree( action );				
					
					handled = true;					
					break;
					
				case MainSwitchAudioToSpeakerButton:					
					action		= MemPtrNew( sizeof(UInt32) );
					ARMletP 	= (NativeFuncType *)PrvLockResource( 'ARMC', 1 );
					*action 	= Swap32( 2 );
					result 		= PceNativeCall( ARMletP, action );
					PrvUnlockResource( 'ARMC', 1 );
					MemPtrFree( action );				
					
					handled = true;					
					break;
				default:
					break;
			}
			break;

		default:
			break;
	}
	
	return handled;
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: Main application event handler.
 *
 * PARAMETERS:	eventP		- Pointer to an event.
 *
 * RETURNED:	True if event was handled.
 *
 ***********************************************************************/

static Boolean AppHandleEvent(EventType *eventP)
{
	UInt16 formId = 0;
	FormType *frmP = NULL;
	
	if(eventP->eType == frmLoadEvent)
	{
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);
		
		switch (formId)
		{
		case MainForm:
			FrmSetEventHandler(frmP, MainFormHandleEvent);
			break;
			
		default:
			break;
		}
		return true;
	}
	
	return false;
}

/***********************************************************************
 *
 * FUNCTION:    ApplEventLoop
 *
 * DESCRIPTION: Main Application event loop.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void AppEventLoop(void)
{
	EventType 	event;
	Err			error = errNone;
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
					
		if(!SysHandleEvent(&event))
		{
			if (!MenuHandleEvent(0, &event, &error))
			{
				if(!AppHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}
			}
		}
				
	} while(event.eType != appStopEvent);
}

/***********************************************************************
 *
 * FUNCTION:    AppStart
 *
 * DESCRIPTION: Called when the application starts
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static Err AppStart(void)
{	
	//check if the device is Tungsten T5 or not
	if (!IsDeviceT5())
	{
		FrmCustomAlert(InformationAlert, "Please run this sample code on Tungsten T5", "", "");
		return 1;
	}
			
	return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Called when the application exits
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static void AppStop(void)
{
	FrmCloseAllForms();
}

/* all code from here to end of file should use no global variables */
#pragma warn_a5_access on

/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: Check the if the ROM is compatible with the application
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
    UInt32 romVersion;

    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    if (romVersion < requiredVersion)
    {
        if ((launchFlags &
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
        {
            FrmAlert (RomIncompatibleAlert);

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

/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: Main entry point for the application.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	-
 *
 ***********************************************************************/

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

		FrmGotoForm(MainForm);
		AppEventLoop();

        AppStop();
        break;

    default:
        break;
    }

    return errNone;
}

/* turn a5 warning off to prevent it being set off by C++
 * static initializer code generation */
#pragma warn_a5_access reset
