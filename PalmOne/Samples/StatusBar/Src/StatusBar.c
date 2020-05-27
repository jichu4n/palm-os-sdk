/***********************************************************************
 *
 * Copyright (c) 2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	StatusBar.c
 *
 * Description:
 *	Main file
 *
 *	Version 1.0 	- Initial Revision (03/02/05)
 *
 ***********************************************************************/


#include <PalmOS.h>
#include <PceNativeCall.h>
#include <HsExt.h>
#include "Common.h"
#include "StatusBar.h"
#include "StatusBar_Rsc.h"

#define kRotationLibTrapOpen			sysLibTrapOpen	
#define kRotationLibTrapClose			sysLibTrapClose	

#define kRotationLibTrapGetRotation		(sysLibTrapCustom + 1)

typedef enum RotationTypeTag
{
    Rotation0   = 0,		
    Rotation90  = 90,	
    Rotation180 = 180,	
    Rotation270 = 270	
} RotationType;

Err RotationLibOpen(UInt16 refnum)
				SYS_TRAP(kRotationLibTrapOpen);

Err RotationLibClose(UInt16 refnum)
				SYS_TRAP(kRotationLibTrapClose);
								
RotationType RotationGetRotation(UInt16 refnum)
				SYS_TRAP(kRotationLibTrapGetRotation);


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
	Boolean			 handled			= false;
	FormType		*frmP				= FrmGetActiveForm( );
	Err				 err				= errNone;
//	UInt32	 		 processor			= 0;
	UInt32	 		 result				= 0;
//	void	 		*ftrMem				= NULL;
//	UInt32			 ftrValue			= NULL;
//	UInt16			 orientation		= 0;
	FieldPtr		 fieldSliderP		= (FieldPtr)NULL;
	FieldPtr		 fieldStatusBarP	= (FieldPtr)NULL;
//	FieldPtr		 fieldScreenWidthP	= (FieldPtr)NULL;
//	FieldPtr		 fieldScreenHeightP = (FieldPtr)NULL;
//	UInt32			 ftrBits			= 0;
//	UInt32			 sliderState		= 0;
//	UInt32			 screenWidth		= 0;
//	UInt32			 screenHeight		= 0;
	UInt16			 refNum				= 0;
	RotationType	 rotation			= 0;
	UInt32			*action				= (UInt32 *)NULL;
	NativeFuncType	*ARMletP			= NULL;
	
	
	fieldSliderP		= (FieldPtr)FrmGetPtr( frmP, MainSliderField );
	fieldStatusBarP		= (FieldPtr)FrmGetPtr( frmP, MainStatusBarField );
	
	switch(eventP->eType)
	{
		case frmOpenEvent:
			MainFormInit(frmP);
			
			// Set focus on MainGetStatusButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainGetStatusButton));
			
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
				case MainGetStatusButton:					
					action		= MemPtrNew( sizeof(UInt32) );
					ARMletP 	= (NativeFuncType *)PrvLockResource( 'ARMC', 1 );
					*action 	= Swap32( 4 );
					result 		= PceNativeCall( ARMletP, action );
					PrvUnlockResource( 'ARMC', 1 );
					MemPtrFree( action );				
					
					switch ( result )
					{
						case 0:
							SetFieldTextFromStr( fieldSliderP, "The Slider is Open", true );	
							break;
						case 1:
							SetFieldTextFromStr( fieldSliderP, "The Slider is Closed", true );	
							break;
						default:
							SetFieldTextFromStr( fieldSliderP, "No Slider", true );	
							break;
					}
					
                    err = SysLibFind( "rotmgr", &refNum );

					if ( err != errNone )
					{
						err = SysLibLoad( sysFileTLibrary, 'rotM', &refNum );
					}

					RotationLibOpen( refNum );
					rotation = RotationGetRotation( refNum );
					RotationLibClose( refNum );
					
					switch ( rotation )
					{
						case Rotation0:
							SetFieldTextFromStr( fieldStatusBarP, "0", true );
							break;
						case Rotation90:
							SetFieldTextFromStr( fieldStatusBarP, "90", true );
							break;	
						case Rotation180:
							SetFieldTextFromStr( fieldStatusBarP, "180", true );
							break;	
						case Rotation270:
							SetFieldTextFromStr( fieldStatusBarP, "270", true );
							break;
						default:
							SetFieldTextFromStr( fieldStatusBarP, "This device does not rotate", true );
							break;	
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
