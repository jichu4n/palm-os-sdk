/**************************************************************************************************
 *
 * Copyright (c) 2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**************************************************************************************************
 *
 * File:		IRCOMMReceiver.c	
 *
 * Description:	This file contains code that receives IRCOMM data and then displays it
 *
 * Version:		1.0	- Initial Revision (05/31/05)
 * 	
 *************************************************************************************************/

#include <PalmOS.h>
#include <HsNav.h>
#include "Common.h"
#include "IRCOMMReceiver.h"
#include "IRCOMMReceiverRsc.h"					
#include "SerialMgr.h"
#include "FeatureMgr.h"

#define IRCOMM_RECEIVER_RECEIVE_FLUSH_TIMEOUT		( 0 )
#define IRCOMM_RECEIVER_GET_EVENT_TIMEOUT			( 1 )

#define IRCOMM_RECEIVER_BAUD_RATE_INITIAL			( 2400 )
#define IRCOMM_RECEIVER_ERROR_OLD_SERIAL_MANAGER	( 1 )


static Boolean		gReceive					= false;
static UInt16		gPortId						= 0;
static UInt32		gBaudRate					= IRCOMM_RECEIVER_BAUD_RATE_INITIAL;


/***********************************************************************
 *
 * FUNCTION:    		PrvHandleMenu
 *
 * DESCRIPTION: 		Deals with menu commands issued
 *
 * PARAMETERS:  		itemID -> ID of the Menu which has been selected
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void PrvHandleMenu(UInt16 itemID)
{
	FormType *frmAboutP = NULL;
    const RGBColorType black = { 0, 0, 0 ,0 };	
    const RGBColorType white = { 0, 255, 255 ,255 };	
    const RectangleType	currentColorRect = { {27, 142}, { 19, 17} };

	
	switch(itemID)
	{		
		case HelpAboutIRCOMMReceiver:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
			
			WinSetForeColorRGB(&white, NULL);
			WinSetTextColorRGB(&black, NULL);
			WinDrawRectangle(&currentColorRect, 0);
			
			break;
			
	}
}


/**************************************************************************************************
 *
 * FUNCTION:	PrvHexPrint
 *
 * DESCRIPTION:	Converts a char string to ascii values( hex ) and displays it in a field.
 *
 * PARAMETERS:	bufferP		- Char String to be converted.
 *				size		- The size of the string.
 *
 * RETURNED:	none
 *
 *************************************************************************************************/

static void PrvHexPrint( const UInt8		*bufferP,
						 const UInt32		 size )
{
	UInt16		 byteIterator		= 0;
	Char		 bufferHex			[ COMMON_BUFFER_HEX_LEN ];
	Char		 stringHex			[ 6 ];
	FieldPtr	 fieldOutputHexP	= (FieldPtr)NULL;
	FormType	*frmP				= (FormType *)FrmGetActiveForm( );


	fieldOutputHexP	= (FieldPtr)FrmGetPtr( frmP, MainOutputHexField );
	bufferHex[ 0 ] = COMMON_CHAR_NULL;
	for ( byteIterator = 0; byteIterator < size; byteIterator++ )
	{
		StrPrintF( stringHex, "%2X ", (UInt8)( bufferP[ byteIterator ] & COMMON_MASK_8_0XFF ) );
		StrCat( bufferHex, &( stringHex[ 2 ] ) );
	}

	SetFieldTextFromStr( fieldOutputHexP, bufferHex, true );
}

/**************************************************************************************************
 *
 * FUNCTION:	PrvOpenIrCommPort
 *
 * DESCRIPTION:	Opens the IRCOMM Port.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	Error Code if there is an error else errNone
 *
 *************************************************************************************************/

Err	PrvOpenIrCommPort( void )
{
	UInt32		 value				= 0;
	Err			 err				= errNone;
	UInt32		 flags				= 0;
	Int32		 paramSize			= 0;


	// Check to make sure that we have the new serial manager
	err = FtrGet( sysFileCSerialMgr, sysFtrNewSerialPresent, &value );
	if (err != errNone) {
		FrmCustomAlert( RomIncompatibleAlert, "FTR Get Failed", " ", " " );
		return ( err );
	}
	if (value == 0) {
		FrmCustomAlert( RomIncompatibleAlert, "Old Serial Manager", " ", " " );
		return ( IRCOMM_RECEIVER_ERROR_OLD_SERIAL_MANAGER );
	}

	/* open serial port */
	err = SrmOpen( sysFileCVirtIrComm, gBaudRate, &gPortId );
	if (err != errNone)
	{
		FrmCustomAlert( RomIncompatibleAlert, "SrmOpen failed", " ", " " );
		return ( err );
	}
	
	
	flags = (   srmSettingsFlagStopBits1
			  | srmSettingsFlagParityOnM
			  | srmSettingsFlagParityEvenM
			  | srmSettingsFlagBitsPerChar8 );

	paramSize = sizeof( flags );
	err = SrmControl( gPortId,
					  srmCtlSetFlags,
					  (void *)&flags,
					  (UInt16 *)&paramSize );
	if (err != errNone)
	{
		FrmCustomAlert( RomIncompatibleAlert, "srmCtlSetFlags failed", " ", " " );
	}

	return ( errNone );
}


/**************************************************************************************************
 *
 * FUNCTION:	PrvIRCommDataReceive   	
 *
 * DESCRIPTION:	This function receives IRCOMM data and then displays it.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	none
 *
 *************************************************************************************************/

void PrvIRCommDataReceive( void )
{
	UInt32		 numOfBytes			= 0;
	Err			 err				= errNone;
	FormType	*frmP				= (FormType *)FrmGetActiveForm( );
	FieldPtr	 fieldOutputP		= (FieldPtr)NULL;
	FieldPtr	 fieldDebugP		= (FieldPtr)NULL;
	UInt8		 receiveBuffer		[ COMMON_BUFFER_LEN ];
	Char		 tempMessage		[ COMMON_BUFFER_HEX_LEN ];
	Char		 conversionString	[ maxStrIToALen ];
	UInt8		*serialBufferP		= (UInt8 *)NULL;
	UInt32		 freeSpace			= ( sizeof( receiveBuffer ) - 1 );
	UInt32		 numBytesWindow		= 0;
	UInt8		*receiveBufferP		= receiveBuffer;


	fieldOutputP	= (FieldPtr)FrmGetPtr( frmP, MainOutputField );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	

	numOfBytes = 0;
	err = SrmReceiveCheck( gPortId, &numOfBytes );
	if (err != errNone)
	{
		SetFieldTextFromStr( fieldDebugP, "SrmReceiveCheck error", true );
		SrmClearErr( gPortId );
		if(SrmReceiveCheck( gPortId, &numOfBytes )!=errNone) return;
	}
	if (numOfBytes != 0)
	{
		numOfBytes = 0;
		do
		{	
			SrmReceiveWindowOpen( gPortId, &serialBufferP, &numBytesWindow );
			if (numBytesWindow > freeSpace) {
				numBytesWindow = freeSpace;
			}
			freeSpace -= numBytesWindow;
			if (numBytesWindow > 0) {
				MemMove( receiveBufferP, serialBufferP, (Int32)numBytesWindow );
			}
			SrmReceiveWindowClose( gPortId, numBytesWindow );
			receiveBufferP += numBytesWindow;
			numOfBytes += numBytesWindow;
		} while (numBytesWindow > 0);


		/* print results */
		receiveBuffer[ numOfBytes ] = COMMON_CHAR_NULL;

		SetFieldTextFromStr( fieldOutputP, (Char *)receiveBuffer, true );
		PrvHexPrint( receiveBuffer, numOfBytes );
		StrCopy( tempMessage, "Received " );
		StrCat( tempMessage, StrIToA( conversionString, (Int32)numOfBytes ) );

		StrCat( tempMessage, "Byte(s) of IR Data, Baud Rate " );
		StrCat( tempMessage, StrIToA( conversionString, (Int32)gBaudRate ) );

		StrCat( tempMessage, ", Ready to receive IR Data!" );
		SetFieldTextFromStr( fieldDebugP, tempMessage, true );
		
		SrmReceiveFlush( gPortId, IRCOMM_RECEIVER_RECEIVE_FLUSH_TIMEOUT );

	}
}

/**************************************************************************************************
 *
 * FUNCTION:	MainFormInit
 *
 * DESCRIPTION:	This function initializes the main form
 *
 * PARAMETERS:	frmP	- Pointer to the main form
 *
 * RETURNED:	None
 *
 *************************************************************************************************/

/*
void MainFormInit( const FormType	*const frmP )
{
	*frmP; 
}
*/

/**************************************************************************************************
 *
 * FUNCTION:	MainFormDeinit
 *
 * DESCRIPTION:	Thie function deinitializes the main form
 *
 * PARAMETERS:	frmP	- Pointer to the main form
 *
 * RETURNED:	None
 *
 *************************************************************************************************/
/*
void MainFormDeinit( const FormType	*const frmP )
{
	*frmP; 
}
*/

/**************************************************************************************************
 *
 * FUNCTION:	MainFormHandleEvent
 *
 * DESCRIPTION:	Main form event handler.
 *
 * PARAMETERS:	eventP - Pointer to an event.
 *
 * RETURNED:	True if event was handled.
 *
 *************************************************************************************************/

Boolean MainFormHandleEvent( EventType	*eventP )
{
	Boolean		 handled			= false;
	FormType	*frmP				= FrmGetActiveForm( );
	ControlType	*popTrigP			= (ControlType *)NULL;
	Char		*labelP				= (Char *)NULL;
	FieldPtr	 fieldDebugP		= (FieldPtr)NULL;
	FieldPtr	 fieldOutputP		= (FieldPtr)NULL;
	FieldPtr	 fieldOutputHexP	= (FieldPtr)NULL;


	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldOutputP	= (FieldPtr)FrmGetPtr( frmP, MainOutputField );
	fieldOutputHexP	= (FieldPtr)FrmGetPtr( frmP, MainOutputHexField );
	
	switch (eventP->eType)
	{
		case frmOpenEvent:
			//MainFormInit( frmP );
			FrmDrawForm( frmP );
			FrmNavObjectTakeFocus(frmP, MainReceiveButton); // Set navigation focus
			handled = true;
			break;
			
		case frmCloseEvent:
			//MainFormDeinit( frmP );
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainReceiveButton:		
					// The "Receive" button was hit.
					// Receive the IRCOMM data and display it.
					
					popTrigP		= FrmGetPtr( frmP, MainBaudRatePopTrigger );
					labelP			= (Char *)CtlGetLabel( popTrigP );
					gBaudRate		= (UInt32)StrAToI( labelP );
					if (gPortId != 0)
					{
						SrmClose( gPortId );
						gPortId = 0;
					}
					PrvOpenIrCommPort( );
					SrmReceiveFlush( gPortId, IRCOMM_RECEIVER_RECEIVE_FLUSH_TIMEOUT );
					gReceive = true;
					handled = true;
					SetFieldTextFromStr( fieldOutputP, " ", true );
					SetFieldTextFromStr( fieldOutputHexP, " ", true );
					SetFieldTextFromStr( fieldDebugP, "Ready to Receive IRCOMM Data!", true );

					break;
				default:
					break;
			}
			break;

		case popSelectEvent:
			switch (eventP->data.popSelect.controlID)
			{
				case MainBaudRatePopTrigger:

					if (gPortId != 0)
					{
						SrmClose( gPortId );
						gPortId = 0;
					}
					gReceive = false;
					SetFieldTextFromStr( fieldDebugP, " ", true );
					SetFieldTextFromStr( fieldOutputP, " ", true );
					SetFieldTextFromStr( fieldOutputHexP, " ", true );
					break;

				default:
					break;

			}
			break;

		case menuEvent:
			PrvHandleMenu(eventP->data.menu.itemID);
			FrmDrawForm(frmP);
			handled = true;
			break;
			
		case nilEvent:
			if ( gReceive == true )
			{
				PrvIRCommDataReceive( );
			}
			break;

		default:
			break;
	}
	
	return ( handled );
}

#if 0
	#pragma mark -
#endif

/**************************************************************************************************
 *
 * FUNCTION:	AppHandleEvent
 *
 * DESCRIPTION:	Main application event handler.
 *
 * PARAMETERS:	eventP - Pointer to an event.
 *
 * RETURNED:	True if event was handled.
 *
 *************************************************************************************************/

Boolean AppHandleEvent( const EventType	*const eventP )
{
	UInt16		 formId	= 0;
	FormType	*frmP  	= (FormType *)NULL;

	
	if (eventP->eType == frmLoadEvent)
	{
		formId = eventP->data.frmLoad.formID;
		frmP   = FrmInitForm( formId );
		FrmSetActiveForm( frmP );
		
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler( frmP, MainFormHandleEvent );
				break;
			
			default:
				break;
		}

		return ( true );
	}
	
	return ( false );
}

/**************************************************************************************************
 *
 * FUNCTION:	AppEventLoop
 *
 * DESCRIPTION:	Main Application event loop.
 *
 * PARAMETERS:	None
 *
 * RETURNED:	None
 *
 *************************************************************************************************/

void AppEventLoop( void )
{
	EventType	event;
	Err		 	error	= errNone;
	

	do
	{
		EvtGetEvent( &event, IRCOMM_RECEIVER_GET_EVENT_TIMEOUT );
		if (SysHandleEvent( &event ) == false) {
			if (MenuHandleEvent( 0, &event, &error ) == false) {
				if (AppHandleEvent( &event ) == false) {
					FrmDispatchEvent( &event );
				}
			}
		}
				
	} while (event.eType != appStopEvent);
}

/**************************************************************************************************
 *
 * FUNCTION:	AppStart
 *
 * DESCRIPTION:	Called when the application starts
 *
 * PARAMETERS:	None
 *
 * RETURNED:	Error Code if there is an error else errNone
 *
 *************************************************************************************************/

Err AppStart( void )
{
	return ( errNone );
}

/**************************************************************************************************
 *
 * FUNCTION:	AppStop
 *
 * DESCRIPTION:	Called when the application exits
 *
 * PARAMETERS:	- None
 *
 * RETURNED:	- None
 *
 *************************************************************************************************/

void AppStop( void )
{
	if (gPortId != 0)
	{
		SrmClose( gPortId );
		gPortId = 0;
	}
	FrmCloseAllForms( );
}

/* all code from here to end of file should use no global variables */
#pragma warn_a5_access on

/**************************************************************************************************
 *
 * FUNCTION:	RomVersionCompatible
 *
 * DESCRIPTION:	Check if the ROM is compatible with the application
 *
 * PARAMETERS:	requiredVersion	- The minimal required version of the ROM.
 *				launchFlags		- Flags that help the application start itself.
 *
 * RETURNED:	Error Code if there is an error else errNone
 *
 *************************************************************************************************/

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

/**************************************************************************************************
 *
 * FUNCTION:	PilotMain
 *
 * DESCRIPTION:	Main entry point for the application.
 *
 * PARAMETERS:	cmd			- Launch Code, tells the application how to start itself.
 *				cmdPBP		- Parameter Block for a launch code.
 *				launchFlags	- Flags that help the application start itself.
 *
 * RETURNED:	Error code if there is an error else errNone
 *
 *************************************************************************************************/

UInt32 PilotMain( const UInt16	cmd,
				  const MemPtr	cmdPBP,
				  const UInt16	launchFlags )
{
	Err	error	= errNone;


	*cmdPBP; // To avoid unused argument warning
	error = RomVersionCompatible( ourMinVersion, launchFlags );
	if (error) {
		return ( error );
	}

	switch (cmd)
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
#pragma warn_a5_access reset

/* EOF *******************************************************************************************/
