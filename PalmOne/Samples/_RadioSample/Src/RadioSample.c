/******************************************************************************
 * Copyright (c) 2004-2006 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup RadioSample
 */
 
/**
 *  @brief 	This file contains code that is common to all Palm OS based programs
 *  		and is used by RadioSampleMain.c
 *
 *
 *
 *
 *  @version 1.0	- Initial Revision (02/18/04)
 *  @version 1.1	- Separated common functions from specific RadioSample related ones (11/02/06)
 *
 *  @file RadioSample.c
 */


#include <PalmOS.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>
#include "Common.h"
#include "RadioSample.h"	 					
#include "RadioSample_Rsc.h"
#include <TelephonyMgr.h>
#include "MobileServices.h"


extern Boolean AppHandleEvent( const EventType	*const eventP );

typedef struct
{
	Char		String	[ 500 ];
} DBRecordType;

/**
 * @brief MainFormInit
 *	     This function initializes the main form 
 *
 *
 * @param frmP:   IN:  	Pointer to the main form
 * @retval		void:	None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

void MainFormInit(void )
{
	
}


/**
 * @brief MainFormDeInit
 *	     This function deinitializes the main form 
 *
 *
 * @param frmP:   IN:  	Pointer to the main form
 * @retval		void:	None
 *
 *
 * @version This function was added in version 1.0
 *
 **/
/***
void MainFormDeinit( const FormType	*const frmP )
***/
void MainFormDeinit( void )
{
	
}


/**
 * @brief AppEventLoop
 *	     Main application event loop.
 *
 *
 * @param void:   IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

void AppEventLoop( void )
{
	EventType	event;
	Err		 	error	= errNone;
	

	do
	{
		EvtGetEvent( &event, evtWaitForever );
        if ( event.eType == winEnterEvent )
		{
			if (    event.data.winEnter.enterWindow == (WinHandle)FrmGetFormPtr(MainForm)
				 && event.data.winEnter.enterWindow == (WinHandle)FrmGetFirstForm ( ) )
			{
				FtrSet( appCreatorID, 10, 20 );
			}
		}
		if (SysHandleEvent( &event ) == false) {
			if (MenuHandleEvent( 0, &event, &error ) == false) {
				if (AppHandleEvent( &event ) == false) {
					FrmDispatchEvent( &event );
				}
			}
		}
				
	} while (event.eType != appStopEvent);
}


/**
 * @brief AppStart
 *	     Called when the application starts
 *
 *
 * @param void:   IN:  		None
 * @retval 		Err:		Error Code if there is an error else errNone
 *
 *
 * @version This function was added in version 1.0
 *
 **/

Err AppStart( void )
{
	Err						error			= errNone;

	return ( error );
}


/**
 * @brief AppStop
 *	     Called when the application exits
 *
 *
 * @param void:   IN:  		None
 * @retval 		void:		None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

void AppStop( void )
{
	FrmCloseAllForms( );
}

/* all code from here to end of file should use no global variables */



/**
 * @brief RomVersionCompatible
 *	     Check if the ROM is compatible with the application
 *
 *
 * @param requiredVersion:   	IN:  		The minimal required version of the ROM.
 * @param launchFlags:   	IN:  		Flags that help the application start itself.
 * @retval 				Err:		Error Code if there is an error else errNone
 *
 *
 * @version This function was added in version 1.0
 *
 **/

Err RomVersionCompatible( const UInt32	requiredVersion,
						  const UInt16	launchFlags )
{
	UInt32	romVersion	= 0;


	FtrGet( sysFtrCreator, sysFtrNumROMVersion, &romVersion );
	if (romVersion < requiredVersion)
	{
		if (    (launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			 == (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp) )
		{
			FrmAlert( RomIncompatibleAlert );

			if (romVersion < kPalmOS20Version) {
				AppLaunchWithCommand( sysFileCDefaultApp,
									  sysAppLaunchCmdNormalLaunch,
									  NULL );
			}
		}

		return ( sysErrRomIncompatible );
	}

	return ( errNone );
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

UInt32 PilotMain( const UInt16	cmd,
				  const MemPtr	cmdPBP,
				  const UInt16	launchFlags )
{
	Err		   					 error				= errNone;
	
	/* to fix compiler error in CW, comment out for PODS */
	cmdPBP;
		
	error = RomVersionCompatible( ourMinVersion, launchFlags );
	if (error) {
		return ( error );
	}

	switch ( cmd )
	{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart( );
			if (error) {
				return ( error );
			}

			FrmGotoForm( MainForm );
			AppEventLoop( );

			AppStop( );
			break;
		
		
		default:
			break;
	}

	return ( errNone );
}

/* turn a5 warning off to prevent it being set off by C++
 * static initializer code generation */


/* EOF *******************************************************************************************/
