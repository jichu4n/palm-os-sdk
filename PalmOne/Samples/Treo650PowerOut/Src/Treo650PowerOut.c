/***********************************************************************
 *
 * Copyright (c) 2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Treo650PowerOut.c
 *
 * Description:
 *	Treo 650 Power Out file
 *
 *	Version 1.0 	- Initial Revision (03/06/05)
 *
 ***********************************************************************/


#include <PalmOS.h>
#include <PceNativeCall.h>
#include <HsExt.h>
#include "Common.h"
#include "Treo650PowerOut.h"
#include "Treo650PowerOutRsc.h"

typedef enum
{
	POWER_OFF_PERIPHERAL = 0,
	POWER_ON_PERIPHERAL	 = 1
} PowerType_e;

// Global pointer
static NativeFuncType *gARMletP = NULL;

/***********************************************************************
 *
 * FUNCTION:	PrvLockResource
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 ***********************************************************************/
static MemPtr PrvLockResource(DmResType type, DmResID resID)
{
	MemHandle 	resH = NULL;
	MemPtr		resP = NULL;
	
	resH = DmGetResource(type, resID);
	resP = MemHandleLock(resH);
	DmReleaseResource(resH);
	
	return resP;
}

/***********************************************************************
 *
 * FUNCTION:	PrvUnlockResource
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 ***********************************************************************/

static void PrvUnlockResource(DmResType type, DmResID resID)
{
	MemHandle resH = NULL;
	
	resH = DmGetResource(type, resID);
	MemHandleUnlock(resH);
	DmReleaseResource(resH);
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


/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id
 */

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
	Boolean		 handled	= false;
	FormType	*frmP		= FrmGetActiveForm( );
//	Err			 err		= errNone;
	UInt32	 	 processor	= 0;
	UInt32	 	 result		= 0;
//	void	 	*ftrMem		= NULL;
//	UInt32		 ftrValue	= NULL;
	UInt32		*action		= MemPtrNew( sizeof(UInt32) );
	
	switch(eventP->eType)
	{
		case frmOpenEvent:
			MainFormInit(frmP);
			FrmDrawForm(frmP);
			handled = true;
			break;
			
		case frmCloseEvent:
			MainFormDeinit(frmP);
			break;
			
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);
			
		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainPowerOutPowerOffButton:		
					FtrGet(sysFtrCreator, sysFtrNumProcessorID, &processor);
					if(sysFtrNumProcessorIsARM(processor))
					{
						gARMletP = (NativeFuncType *)PrvLockResource( 'ARMC', 1 );
						*action = Swap32( POWER_OFF_PERIPHERAL );
						result = PceNativeCall( gARMletP, action );		
						PrvUnlockResource( 'ARMC', 1 );					
					}
					handled = true;					
					break;
				case MainPowerOutPowerOnButton:
					FtrGet(sysFtrCreator, sysFtrNumProcessorID, &processor);
					if(sysFtrNumProcessorIsARM(processor))
					{
						gARMletP = (NativeFuncType *)PrvLockResource( 'ARMC', 1 );
						*action = Swap32( POWER_ON_PERIPHERAL );
						result = PceNativeCall( gARMletP, action );
						PrvUnlockResource( 'ARMC', 1 );					
					}
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
