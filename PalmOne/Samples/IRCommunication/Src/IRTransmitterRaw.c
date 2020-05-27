/**************************************************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**************************************************************************************************
 *
 * File:		IRTransmitterRaw.c
 *
 * Description:	This file contains code that retrieves the raw data from the field and then sends
 *				it using Raw IR	
 *
 * Version:		1.0	- Initial Revision (09/05/03)
 * 	
 *************************************************************************************************/

#include <PalmOS.h>
#include <HsNav.h>
#include "Common.h"
#include "IRTransmitterRaw.h"									
#include "IRTransmitterRawRsc.h"						
#include "SerialMgr.h"
#include "FeatureMgr.h"

#define IR_TRANSMITTER_RAW_PORT			serPortIrPort
#define IR_TRANSMITTER_RAW_ERROR_CODE	( 1 )


/**************************************************************************************************
 *
 * FUNCTION:	PrvCharToInteger				
 *
 * DESCRIPTION:	Converts a hexadecimal character to it's equivalent integer value.
 *
 * PARAMETERS:	character	- Hexadecimal character to be converted.
 *
 * RETURNED:	The integer value corresponding to the hexadecimal character inputted	
 *
 *************************************************************************************************/

static UInt8 PrvCharToInteger( const Char	character )
{
	UInt8		integer = 0;
	

	switch ( character )
	{
		case '1':
			integer = 1;
			break;
		case '2':
			integer = 2;
			break;
		case '3':
			integer = 3;
			break;
		case '4':
			integer = 4;
			break;
		case '5':
			integer = 5;
			break;
		case '6':
			integer = 6;
			break;
		case '7':
			integer = 7;
			break;
		case '8':
			integer = 8;
			break;
		case '9':
			integer = 9;
			break;
		case 'A':
		case 'a':
			integer = 10;
			break;
		case 'B':
		case 'b':
			integer = 11;
			break;
		case 'C':
		case 'c':
			integer = 12;
			break;
		case 'D':
		case 'd':
			integer = 13;
			break;
		case 'E':
		case 'e':
			integer = 14;
			break;
		case 'F':
		case 'f':
			integer = 15;
			break;
		default:
			integer = 0;
			break;
	}

	return ( integer );
}

/**************************************************************************************************
 *
 * FUNCTION:	PrvIRDataSend				
 *
 * DESCRIPTION:	This function retrieves the raw data from the field and then sends it using Raw IR.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	Error code if there is an error else errNone
 *
 *************************************************************************************************/

static Err PrvIRDataSend( void )
{
	UInt32		 value				= 0;
	UInt8		 byte				= 0;
	Err			 err				= errNone;
	UInt16		 portId				= 0;
	UInt32		 numBytesToSend		= 0;
	UInt32		 numSent			= 0;
	Char		 message			[ COMMON_BUFFER_LEN ]	= "NO USER INPUT!\n";
	Char		 messageHex			[ COMMON_BUFFER_HEX_LEN ];
	UInt8		 messageHexLength	= 0;
	Char		 tempMessage		[ COMMON_BUFFER_HEX_LEN ];
	Char		 conversionString	[ maxStrIToALen ];
	Char		 stringHex			[ 6 ];
	UInt8		 byteIterator		= 0;
	Char		*fieldTextP			= (Char *)NULL;
	Char		*fieldTextHexP		= (Char *)NULL;
	FieldPtr	 fieldInputP		= (FieldPtr)NULL;
	FieldPtr	 fieldInputHexP		= (FieldPtr)NULL;
	FieldPtr	 fieldDebugP		= (FieldPtr)NULL;
	FormType	*frmP				= (FormType *)FrmGetActiveForm( );
	UInt32		 flags				= 0;
	Int32		 paramSize			= 0;
	UInt32		 baudRate			= 0;
	ControlType	*popTrigP			= (ControlType *)NULL;
	Char		*labelP				= (Char *)NULL;


	MemSet( messageHex, sizeof( messageHex ), 0 );
	MemSet( tempMessage, sizeof( tempMessage ), 0 );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldInputP		= (FieldPtr)FrmGetPtr( frmP, MainInputField );
	fieldInputHexP	= (FieldPtr)FrmGetPtr( frmP, MainInputHexField );

	popTrigP	= FrmGetPtr( frmP, MainBaudRatePopTrigger );
	labelP		= (Char *)CtlGetLabel( popTrigP );
	baudRate	= (UInt32)StrAToI( labelP );
	
	SetFieldTextFromStr( fieldDebugP, "Send IR Data Function", true );
	fieldTextP		= FldGetTextPtr( fieldInputP );
	fieldTextHexP	= FldGetTextPtr( fieldInputHexP );
	if ( (fieldTextP != (Char *)NULL) && (StrLen( fieldTextP ) > 0) )
	{
		SetFieldTextFromStr( fieldDebugP, "Input", true );
		StrCopy( message, fieldTextP );

		messageHex[ 0 ] = '\0';
		numBytesToSend = StrLen( message );
		for ( byteIterator = 0; byteIterator < numBytesToSend; byteIterator++ )
		{
			StrPrintF( stringHex, "%02X ", (UInt8)message[ byteIterator ] );
			StrCat( messageHex, &( stringHex[ 2 ] ) );
		}

		SetFieldTextFromStr( fieldInputHexP, messageHex, true );
	}
	else if ( (fieldTextHexP != (Char *)NULL) && (StrLen( fieldTextHexP ) > 0) )
	{
		SetFieldTextFromStr( fieldDebugP, "Input Hex", true );
		StrCopy( messageHex, fieldTextHexP );

		message[ 0 ]		= '\0';
		messageHexLength	= (UInt8)StrLen( messageHex );
		for ( byteIterator = 0; byteIterator < messageHexLength; byteIterator += 3 )
		{
			SetFieldTextFromStr( fieldDebugP, "Input Hex", true );
			byte = PrvCharToInteger( messageHex[ byteIterator ] );
			byte <<= 4;
			byte = (UInt8)( byte | PrvCharToInteger( messageHex[ byteIterator + (UInt8)( 1 ) ] ) );
			message[ byteIterator / 3 ] = (Char)byte;
			numBytesToSend++;
		}
		message[ byteIterator / 3 ] = '\0';

		SetFieldTextFromStr( fieldInputP, message, true );
	}

	// Check to make sure that we have the new serial manager
	err = FtrGet( sysFileCSerialMgr, sysFtrNewSerialPresent, &value );
	if (err != errNone) {
		SetFieldTextFromStr( fieldDebugP, "FTR Get Failed", true );
		return ( err );
	}
	if (value == 0) {
		SetFieldTextFromStr( fieldDebugP, "Old Serial Manager", true );
		return ( IR_TRANSMITTER_RAW_ERROR_CODE );
	}

	SetFieldTextFromStr( fieldDebugP, "New Serial Manager", true );

	// Open a connection to the IR Port to send Raw Data
	err = SrmOpen( IR_TRANSMITTER_RAW_PORT,
				   baudRate,
				   &portId );
	if (err != errNone) {
		SetFieldTextFromStr( fieldDebugP, "IR Port Open Failed", true );
		return ( err );
	}

	// Comment added by Ryan Lim.
	// This TX app turns off/on the IR port everytime the "Send Raw IR" button
	// is pressed. It depends on the serial driver implementation, but most
	// likely the IR port gets turned off by SrmOpen (during init) and gets
	// turned on by SrmControl() call. There probably is not enough time delay
	// for the IR port to warm up between the SrmControl() call and the
	// SrmSend() call. The serial driver needs to make sure that the IR port is
	// ready to transfer before the SrmSend() is called. Watch out for this when
	// raw IR data look partially corrupt.
	err = SrmControl( portId, srmCtlIrDAEnable, (void *)NULL, (UInt16 *)NULL );
	if (err != errNone )
	{
		SetFieldTextFromStr( fieldDebugP, "IrDA Enable Failed", true );
		SrmClose( portId );
		return ( err );
	}

	err = SrmControl( portId, srmCtlRxDisable, (void *)NULL, (UInt16 *)NULL );
	if (err != errNone ) {
		SetFieldTextFromStr( fieldDebugP, "IrDA Rx Disable Failed", true );
	}
		
	flags = (   srmSettingsFlagStopBits1
			  | srmSettingsFlagParityOnM
			  | srmSettingsFlagParityEvenM
			  | srmSettingsFlagBitsPerChar8 );

	paramSize = sizeof( flags );
	err = SrmControl( portId,
					  srmCtlSetFlags,
					  (void *)&flags,
					  (UInt16 *)&paramSize );
	if (err != errNone )
	{
		SetFieldTextFromStr( fieldDebugP, "Set Flags Failed", true );
		SrmClose( portId );
		return ( err );
	}
	
	numSent = SrmSend( portId, message, numBytesToSend, &err );
	if (err != errNone)
	{
		switch (err)
		{
			case serErrBadPort:
				StrCopy( message, "Err Bad Port" );
				break;
			case serErrNotOpen:
				StrCopy( message, "Err Port Not Open" );
				break;
			case serErrTimeOut:
				StrCopy( message, "Err Time Out" );
				break;
			case serErrConfigurationFailed:
				StrCopy( message, "Err Config Failed" );
				break;
			case serErrNotSupported:
				StrCopy( message, "Err Port Not foreground" );
				break;
			case serErrNoDevicesAvail:
				StrCopy( message, "Err No Device Avail" );
				break;
			default:
				StrCopy( message, "Unknown Error" );
				break;
		}
		SetFieldTextFromStr( fieldDebugP, "Send IR Data Failed", true );
		SetFieldTextFromStr( fieldDebugP, message, true );
		SrmClose( portId );
		return ( err );
	}

	SrmSendWait( portId );

	err = SrmClose( portId );
	if (err != errNone)
	{
		SetFieldTextFromStr( fieldDebugP, "IR Port Close Failed", true );
		return ( err );
	}

	StrCopy( tempMessage, "IR Data Send OK, " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)numBytesToSend ) );
	StrCat( tempMessage, " bytes to send. " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)numSent ) );
	StrCat( tempMessage, "bytes sent at Baud " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)baudRate ) );
	SetFieldTextFromStr( fieldDebugP, tempMessage, true );

	return ( errNone );
}


/**************************************************************************************************
 *
 * FUNCTION:	PrvHandleMenu
 *
 * DESCRIPTION:	Handle menu
 *
 * PARAMETERS:	itemID
 *
 * RETURNED:	None
 *
 *************************************************************************************************/
static void PrvHandleMenu(UInt16 itemID)
{
	//Err err = errNone;
	FormType *frmAboutP = NULL;
	
	switch(itemID)
	{
	
		case HelpAboutRawIRTransmitter:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
						
			break;
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

void MainFormInit( const FormType	*const frmP )
{
}

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

void MainFormDeinit( const FormType	*const frmP )
{
}

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
	Boolean		 handled				= false;
	Err			 err					= errNone;
	FormType	*frmP					= FrmGetActiveForm( );
//	UInt8		 repetitionsIterator	= 0;
//	FieldPtr	 fieldRepsP				= (FieldPtr)NULL;
//	FieldPtr	 fieldInterDataGapP		= (FieldPtr)NULL;
//	UInt32		 repetitions			= 1;
//	Char		*repetitionsP			= (Char *)NULL;
//	Char		*interDataGapP			= (Char *)NULL;
//	UInt32		 interDataGap			= 1;


	switch (eventP->eType)
	{
		case frmOpenEvent:
			MainFormInit( frmP );
			FrmDrawForm( frmP );
			FrmNavObjectTakeFocus(frmP, MainInputField); // Set navigation focus			

			handled = true;
			break;
			
		case frmCloseEvent:
			MainFormDeinit( frmP );
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainSendButton:		
					// The "Send" button was hit.
					// Get the Raw IR data from the field and send it using Raw IR
					err = PrvIRDataSend( );
					if (err != errNone) {
						return ( false );
					}
					handled = true;					
					break;

				default:
					break;
			}

			break;

		case popSelectEvent:
			break;

		case menuEvent:
			PrvHandleMenu(eventP->data.menu.itemID);
			handled = true;
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
		formId	= eventP->data.frmLoad.formID;
		frmP	= FrmInitForm( formId );
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
	Err			error	= errNone;
	

	do
	{
		EvtGetEvent( &event, evtWaitForever );
					
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
