
/**************************************************************************************************
 *
 * Copyright (c) 2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**************************************************************************************************
 *
 * File:		SerialTransRecv.c
 *
 * Description:	This file contains code that sends and receives serial data.
 *				This file also shows how to	detect a serial peripheral on Tungsten T5 and how to
 *				power on/off the POWER_OUT pin on Tungsten T5.
 *
 * Version:		1.0	- Initial Revision (02/22/05)
 *				2.0 - Added Class Notifications
 * 	
 *************************************************************************************************/

#include <PalmOS.h>
#include <PceNativeCall.h>
#include "Common.h"
#include "SerialTransRecvRsc.h"						
#include "SerialMgr.h"
#include "FeatureMgr.h"
#include "HsNav.h"
#include <PmConnectorLib.h>


#define ourMinVersion		sysMakeROMVersion( 4, 0, 0, sysROMStageDevelopment, 0 )
#define kPalmOS20Version	sysMakeROMVersion( 2, 0, 0, sysROMStageDevelopment, 0 )
			
#define DEBUG_FILE									( 1 )

#define DEBUG										( 1 )
#define SERIAL_TRANSRECV_PORT						serPortCradleRS232Port
#define SERIAL_TRANSRECV_ERROR_CODE					( 1 )

#define SERIAL_TRANSRECV_RECEIVE_FLUSH_TIMEOUT		( 0 )
#define SERIAL_TRANSRECV_GET_EVENT_TIMEOUT			( 1 )

#define SERIAL_TRANSRECV_BAUD_RATE_INITIAL			( 2400 )
#define SERIAL_TRANSRECV_ERROR_OLD_SERIAL_MANAGER	( 1 )

#define SERIAL_TRANSRECV_SER_DET_FTR_NUM			( 1 )
#define SERIAL_TRANSRECV_SER_DET_FILE_NAME			"SerialDetect.log"

#define SERIAL_TRANSRECV_SER_PORT_ID_FTR_NUM		( 2 )
#define SERIAL_TRANSRECV_SER_RECEIVE_FTR_NUM		( 3 )
#define SERIAL_TRANSRECV_SER_BAUD_RATE_FTR_NUM		( 4 )

#define SERIAL_TRANSRECV_SER_DET_T5_FTR_NUM			( 5 )

#define SERIAL_TRANSRECV_SER_DET_TIME_MS			( 600 )

#define SERIAL_TRANSRECV_DET_SERIAL					0x0040
#define SERIAL_TRANSRECV_DET_USB					0x0010


#define	SERIAL_TRANSRECV_CREATOR_ID					'sRtR'

#define SERIAL_TRANSRECV_DEBUG_FILE_NAME			"SerialDebug.log"

#define FrmGetPtr(formP, id)			FrmGetObjectPtr( formP, FrmGetObjectIndex( formP, id ) )

static Boolean		gReceive			= false;
static Boolean		gSerialPortOn		= false;
static UInt16		gPortId				= 0;
static UInt32		gBaudRate			= SERIAL_TRANSRECV_BAUD_RATE_INITIAL;
//static UInt32		gEcho				= false;
static UInt32		gTotalBytes			= 0;

static Char			gBuffer				[ 2048 ];
static Char			gDebugMessage		[ 100 ];

static Err PrvInformationDisplay( const Char		*const dataP,
								  const Boolean		 fileWrite,
								  const Char		*const csvFileNameP );


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

/**************************************************************************************************
 *
 * FUNCTION:	PrvOpenSerialPort
 *
 * DESCRIPTION:	Opens the Serial Port.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	Error Code if there is an error else errNone
 *
 *************************************************************************************************/

Err	PrvOpenSerialPort( void )
{
	UInt32			 value				= 0;
	Err				 err				= errNone;
//	Int32			 paramSize			= 0;
//	UInt32			 flags				= 0;
	UInt32			*action				= (UInt32 *)NULL;
	NativeFuncType	*ARMletP			= NULL;
	UInt32			 deviceID			= 0;
	UInt32	 		 result				= 0;

	
	// Check to make sure that we have the new serial manager
	err = FtrGet( sysFileCSerialMgr, sysFtrNewSerialPresent, &value );
	if ( err != errNone ) {
		FrmCustomAlert( InformationAlert, "FTR Get Failed", " ", " " );
		return ( err );
	}
	if ( value == 0 ) {
		FrmCustomAlert( InformationAlert, "Old Serial Manager", " ", " " );
		return ( SERIAL_TRANSRECV_ERROR_OLD_SERIAL_MANAGER );
	}
	
	if ( gBaudRate == 0 )
	{
		gBaudRate = SERIAL_TRANSRECV_BAUD_RATE_INITIAL;
	}

	/* open serial port */
	err = SrmOpen( SERIAL_TRANSRECV_PORT, gBaudRate, &gPortId );
	if (err != errNone)
	{
		switch ( err )
		{
			case serErrAlreadyOpen:
				FrmCustomAlert( InformationAlert, "Serial port already open!", " ", " " );
				break;

			case serErrBadPort:
				FrmCustomAlert( InformationAlert, "Serial Port does not exist",
								" ", " " );
				break;
			case memErrNotEnoughSpace:
				FrmCustomAlert( InformationAlert, "Mem not enough", " ", " " );
				break;
			default:
				FrmCustomAlert( InformationAlert, "Unknown Error!", " ", " " );
				break;
		}
		FrmCustomAlert( InformationAlert, "SrmOpen failed", " ", " " );
		return ( err );
	}
	
	FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID );
	// Flow Control Workaround for Tungsten T3
	// If we are on Tungsten T3 we disable flow control by calling the armlet.
	if ( deviceID == 'Arz1' )
	{
		action		= MemPtrNew( sizeof(UInt32) );
		ARMletP 	= (NativeFuncType *)PrvLockResource( 'ARMC', 1 );
		*action 	= Swap32( 1 );
		result 		= PceNativeCall( ARMletP, action );
		PrvUnlockResource( 'ARMC', 1 );
		MemPtrFree( action );				

	}

	return ( errNone );
}

static Err PrvInformationDisplay( const Char		*const dataP,
								  const Boolean		 fileWrite,
								  const Char		*const csvFileNameP )

{
	Err						error 			= errNone;
	FileHand				file;


	csvFileNameP;  // To avoid unused warning
	dataP;		   // To avoid unused warning
	fileWrite;	   // To avoid unused warning
	file;          // To avoid unused warning
	
	if ( fileWrite == true )
	{
#ifdef DEBUG_FILE
		if ( csvFileNameP == (Char *)NULL )
		{
			FrmCustomAlert( InformationAlert, "No CSV File Name specified", " ", " " );
			return ( 1 );
		}
		if ( dataP == (Char *)NULL )
		{
			FrmCustomAlert( InformationAlert, "No Data specified", " ", " " );
			return ( 1 );
		}
	
		file = FileOpen( 0, csvFileNameP, sysFileTFileStream, 0, fileModeAppend, &error );
		if ( error != errNone )
		{
			FrmCustomAlert( InformationAlert, "Error opening", csvFileNameP, " " );
			return ( error );
		}
	
		// do something with the record's contents here
		FileWrite( file, dataP, StrLen( dataP ), 1, &error );
		FileWrite( file, "\r\n", StrLen( "\r\n" ), 1, &error );
	
		FileClose( file );
#endif
	}
	else
	{
#ifdef DEBUG_SCREEN
		FrmCustomAlert( InformationAlert, dataP, " ", " " );
#endif

	}
	return ( errNone );
}

/**************************************************************************************************
 *
 * FUNCTION:	PrvSerialDataReceive		
 *
 * DESCRIPTION:	This function receives Serial data and then displays it.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	none
 *
 *************************************************************************************************/

void PrvSerialDataReceive( void )
{
	UInt32		 numOfBytes			= 0;
	Err			 err				= errNone;
	FormType	*frmP				= (FormType *)FrmGetActiveForm( );
	FieldPtr	 fieldOutputP		= (FieldPtr)NULL;
	FieldPtr	 fieldDebugP		= (FieldPtr)NULL;

	UInt8		 receiveBuffer		[ 1024 ];
	Char		 tempMessage		[ 1024 ];
	
	Char		 conversionString	[ maxStrIToALen ];
	UInt8		*serialBufferP		= (UInt8 *)NULL;
	UInt32		 freeSpace			= ( sizeof( receiveBuffer ) - 1 );
	UInt32		 numBytesWindow		= 0;
	UInt8		*receiveBufferP		= receiveBuffer;
	Int32		 baudRate			= 0;
	Int16		 paramSize			= 0;
	
	fieldOutputP	= (FieldPtr)FrmGetPtr( frmP, MainOutputField );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	
	
	numOfBytes = 0;
	err = SrmReceiveCheck( gPortId, &numOfBytes );
	
	
	if (err != errNone)
	{
		SetFieldTextFromStr( fieldDebugP, "SrmReceiveCheck error", true );
		PrvInformationDisplay( "SrmReceiveCheck error", true, SERIAL_TRANSRECV_DEBUG_FILE_NAME );
		PrvInformationDisplay( StrIToH( conversionString, (UInt32)numOfBytes ), true,
							   SERIAL_TRANSRECV_DEBUG_FILE_NAME );
		SrmClearErr( gPortId );
		if ( SrmReceiveCheck( gPortId, &numOfBytes ) != errNone )
		{
			PrvInformationDisplay( "SrmReceiveCheck error again", true, SERIAL_TRANSRECV_DEBUG_FILE_NAME );
			PrvInformationDisplay( StrIToH( conversionString, (UInt32)numOfBytes ),
								   true, SERIAL_TRANSRECV_DEBUG_FILE_NAME );
		}
	}

	if (numOfBytes != 0)
	{
		numOfBytes = 0;
		do
		{
		
		
		
			err = SrmReceiveWindowOpen( gPortId, &serialBufferP, &numBytesWindow );

			if (numBytesWindow > freeSpace) {
				numBytesWindow = freeSpace;
			}
			freeSpace -= numBytesWindow;
			if (numBytesWindow > 0) {
				MemMove( receiveBufferP, serialBufferP, (Int32)numBytesWindow );
			}
			err = SrmReceiveWindowClose( gPortId, numBytesWindow );
			receiveBufferP += numBytesWindow;
			numOfBytes += numBytesWindow;
			
			

		} while (numBytesWindow > 0);

		gTotalBytes += numOfBytes;
		receiveBuffer[ numOfBytes - 1 ] = COMMON_CHAR_NULL;

		if ( ( StrLen( gBuffer ) + numOfBytes ) <= 2046 )
		{
			StrCat( (Char *)gBuffer, (Char *)receiveBuffer );
		}

		/* print results */

		SetFieldTextFromStr( fieldOutputP, (Char *)receiveBuffer, true );
		StrCopy( tempMessage, "Received " );
		StrCat( tempMessage, StrIToA( conversionString, (Int32)numOfBytes ) );

		StrCat( tempMessage, " Total " );
		StrCat( tempMessage, StrIToA( conversionString, (Int32)gTotalBytes ) );

		StrCat( tempMessage, "Byte(s) of Serial Data, Baud Rate " );
		StrCat( tempMessage, StrIToA( conversionString, (Int32)gBaudRate ) );

		{
			UInt16  numTicks = SysTicksPerSecond( );
			StrCat( tempMessage, ", Ticks " );
			StrCat( tempMessage, StrIToA( conversionString, numTicks ) );
		}
		paramSize = sizeof( baudRate );
		SrmControl( gPortId,
					srmCtlGetBaudRate,
					&baudRate,
					(UInt16 *)&paramSize );

		StrCat( tempMessage, ", Baud Rate(R) " );
		StrCat( tempMessage, StrIToA( conversionString, baudRate ) );

		StrCat( tempMessage, ", Ready to receive Serial Data!" );
		SetFieldTextFromStr( fieldDebugP, tempMessage, true );
		
		SrmReceiveFlush( gPortId, SERIAL_TRANSRECV_RECEIVE_FLUSH_TIMEOUT );	
		
		
	}
}

Err PrvInformationDisplayError( const Err			err,
								const Char		   *functionNameP,
								const UInt32		lineNumber )
{
	Char		message				[ 300 ];
	Char		conversionString	[ 30 ];


	if ( err != errNone )
	{
		StrCopy( message, "Status = Error!, Class = " );
		StrCat( message, StrIToH( conversionString, (UInt32)( err & 0xFF00 ) ) );
		StrCat( message, " , Code = " );
		StrCat( message, StrIToH( conversionString, (UInt32)( err & 0x00FF ) ) );
		StrCat( message, " , Name = " );
		switch ( err )
		{
			case serErrBadParam:
				StrCat( message, "serErrBadParam!" );
				break;
			case serErrBadPort:
				StrCat( message, "serErrBadPort!" );
				break;
			case serErrNoMem:
				StrCat( message, "serErrNoMem!" );
				break;
			case serErrBadConnID:
				StrCat( message, "serErrBadConnID!" );
				break;
			case serErrTimeOut:
				StrCat( message, "serErrTimeOut!" );
				break;
			case serErrLineErr:
				StrCat( message, "serErrLineErr!" );
				break;
			case serErrAlreadyOpen:
				StrCat( message, "serErrAlreadyOpen!" );
				break;
			case serErrStillOpen:
				StrCat( message, "serErrStillOpen!" );
				break;
			case serErrNotOpen:
				StrCat( message, "serErrNotOpen!" );
				break;
			case serErrNotSupported:
				StrCat( message, "serErrNotSupported!" );
				break;
			case serErrNoDevicesAvail:
				StrCat( message, "serErrNoDevicesAvail!" );
				break;
			case serErrConfigurationFailed:
				StrCat( message, "serErrConfigurationFailed!" );
				break;
			case memErrNotEnoughSpace:
				StrCat( message, "memErrNotEnoughSpace!" );
				break;
			default:
				StrCat( message, "Unknown!" );
				break;
		}
		StrCat( message, ", " );
		StrCat( message, functionNameP );
		StrCat( message, ", Line # = " );
		StrCat( message, StrIToH( conversionString, (UInt32)lineNumber ) );
		PrvInformationDisplay( message, true, SERIAL_TRANSRECV_DEBUG_FILE_NAME );
	}
	else
	{
#ifdef DEBUG
		StrCopy( message, "Status = Successful!, " );
		StrCat( message, functionNameP );
		StrCat( message, " Line # = " );
		StrCat(	message, StrIToH( conversionString, (UInt32)lineNumber ) );
		PrvInformationDisplay( message, true, SERIAL_TRANSRECV_DEBUG_FILE_NAME );
#endif
	}
	return ( err );
}

/**************************************************************************************************
 *
 * FUNCTION:	PrvSerialDataSend				
 *
 * DESCRIPTION:	This function retrieves the data from the field and then sends it using the serial
 *				port.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	Error code if there is an error else errNone
 *
 *************************************************************************************************/
/*
static Err PrvSerialDataSend( void )
{
//	UInt32			 value				= 0;
//	UInt8			 byte				= 0;
	Err				 err				= errNone;
	UInt32			 numBytesToSend		= 0;
	UInt32			 numSent			= 0;
	Char			 message			[ 1100 ];
	Char			 tempMessage		[ 1100 ];
	Char			 conversionString	[ maxStrIToALen ];
//	UInt8			 byteIterator		= 0;
	Char			*fieldTextP			= (Char *)NULL;
//	Char			*fieldTextHexP		= (Char *)NULL;
	FieldPtr		 fieldInputP		= (FieldPtr)NULL;
	FieldPtr		 fieldDebugP		= (FieldPtr)NULL;
	FormType		*frmP				= (FormType *)FrmGetActiveForm( );
//	UInt32			 flags				= 0;
//	Int32			 paramSize			= 0;
//	UInt32			 baudRate			= 0;
//	ControlType		*popTrigP			= (ControlType *)NULL;
//	Char			*labelP				= (Char *)NULL;
	Err				 error				= errNone;


	MemSet( tempMessage, sizeof( tempMessage ), 0 );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldInputP		= (FieldPtr)FrmGetPtr( frmP, MainInputField );


	SetFieldTextFromStr( fieldDebugP, "Send Serial Data Function", true );
	fieldTextP		= FldGetTextPtr( fieldInputP );
	if ( (fieldTextP != (Char *)NULL) && (StrLen( fieldTextP ) > 0) )
	{
		SetFieldTextFromStr( fieldDebugP, "Input", true );
		StrCopy( message, fieldTextP );

		message [ StrLen ( fieldTextP ) ] = COMMON_CHAR_NULL;

		numBytesToSend = StrLen( message );
	}

	numSent = SrmSend( gPortId, message, numBytesToSend, &err );
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
		SetFieldTextFromStr( fieldDebugP, "Send Serial Data Failed!", true );
		SetFieldTextFromStr( fieldDebugP, message, true );
		return ( err );
	}



	error = SrmSendWait( gPortId );
	if ( error != errNone )
	{
		switch ( error )
		{
            case serErrTimeOut:
				FrmCustomAlert( InformationAlert,
								"Unable to send data within the CTS timeout period.",
								" ", " " );
				break;

			default:
				FrmCustomAlert( InformationAlert, "Unknown Error", " ", " " );
				break;
		}
	}

	StrCopy( tempMessage, "Serial Data Send OK, Bytes " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)numBytesToSend ) );
	StrCat( tempMessage, ", Sent " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)numSent ) );
	StrCat( tempMessage, ", Baud " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)gBaudRate ) );
	SetFieldTextFromStr( fieldDebugP, tempMessage, true );

	return ( errNone );
}

*/

// use memory dynamic allocation to avoid coverity error
static Err PrvSerialDataSend( void )
{
//	UInt32			 value				= 0;
//	UInt8			 byte				= 0;
	Err				 err				= errNone;
	UInt32			 numBytesToSend		= 0;
	UInt32			 numSent			= 0;
//	Char			 message			[ 1100 ];
//	Char			 tempMessage		[ 1100 ];
	UInt32			 messageSize = 1100;
	Char*			 messagePtr = NULL;
	Char*			 tempMessagePtr = NULL;
	Char			 conversionString	[ maxStrIToALen ];
//	UInt8			 byteIterator		= 0;
	Char			*fieldTextP			= (Char *)NULL;
//	Char			*fieldTextHexP		= (Char *)NULL;
	FieldPtr		 fieldInputP		= (FieldPtr)NULL;
	FieldPtr		 fieldDebugP		= (FieldPtr)NULL;
	FormType		*frmP				= (FormType *)FrmGetActiveForm( );
//	UInt32			 flags				= 0;
//	Int32			 paramSize			= 0;
//	UInt32			 baudRate			= 0;
//	ControlType		*popTrigP			= (ControlType *)NULL;
//	Char			*labelP				= (Char *)NULL;
	Err				 error				= errNone;

	messagePtr = (Char *) MemPtrNew(messageSize);
	tempMessagePtr = (Char *) MemPtrNew(messageSize);

	MemSet( tempMessagePtr, messageSize, 0 );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldInputP		= (FieldPtr)FrmGetPtr( frmP, MainInputField );


	SetFieldTextFromStr( fieldDebugP, "Send Serial Data Function", true );
	fieldTextP		= FldGetTextPtr( fieldInputP );
	if ( (fieldTextP != (Char *)NULL) && (StrLen( fieldTextP ) > 0) )
	{
		SetFieldTextFromStr( fieldDebugP, "Input", true );
		StrCopy( messagePtr, fieldTextP );

		*(Char *)(messagePtr + StrLen(fieldTextP)) = COMMON_CHAR_NULL;

		numBytesToSend = StrLen( messagePtr );
	}

	numSent = SrmSend( gPortId, messagePtr, numBytesToSend, &err );
	if (err != errNone)
	{
		switch (err)
		{
			case serErrBadPort:
				StrCopy( messagePtr, "Err Bad Port" );
				break;
			case serErrNotOpen:
				StrCopy( messagePtr, "Err Port Not Open" );
				break;
			case serErrTimeOut:
				StrCopy( messagePtr, "Err Time Out" );
				break;
			case serErrConfigurationFailed:
				StrCopy( messagePtr, "Err Config Failed" );
				break;
			case serErrNotSupported:
				StrCopy( messagePtr, "Err Port Not foreground" );
				break;
			case serErrNoDevicesAvail:
				StrCopy( messagePtr, "Err No Device Avail" );
				break;
			default:
				StrCopy( messagePtr, "Unknown Error" );
				break;
		}
		SetFieldTextFromStr( fieldDebugP, "Send Serial Data Failed!", true );
		SetFieldTextFromStr( fieldDebugP, messagePtr, true );

		MemPtrFree(messagePtr);
		MemPtrFree(tempMessagePtr);
	
		return ( err );
	}



	error = SrmSendWait( gPortId );
	if ( error != errNone )
	{
		switch ( error )
		{
            case serErrTimeOut:
				FrmCustomAlert( InformationAlert,
								"Unable to send data within the CTS timeout period.",
								" ", " " );
				break;

			default:
				FrmCustomAlert( InformationAlert, "Unknown Error", " ", " " );
				break;
		}
	}

	StrCopy( tempMessagePtr, "Serial Data Send OK, Bytes " );
	StrCat( tempMessagePtr, StrIToA( conversionString, (Int32)numBytesToSend ) );
	StrCat( tempMessagePtr, ", Sent " );
	StrCat( tempMessagePtr, StrIToA( conversionString, (Int32)numSent ) );
	StrCat( tempMessagePtr, ", Baud " );
	StrCat( tempMessagePtr, StrIToA( conversionString, (Int32)gBaudRate ) );
	SetFieldTextFromStr( fieldDebugP, tempMessagePtr, true );

	MemPtrFree(messagePtr);
	MemPtrFree(tempMessagePtr);
	
	return ( errNone );
}


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
		case HelpAboutSerialCommunication:
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
	frmP;
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
	frmP;
}

void PowerOnPowerOut( void )
{
	Char		debugMessage		[ 100 ];
	Err			err					= errNone;
	UInt16		refNum				= 0;
	
	
	// Try to find the  library
	err = SysLibFind(kPmConnectorLibName, &refNum);	
	if (err != errNone)
		err = SysLibLoad(kPmConnectorLibType, kPmConnectorLibCreator, &refNum);

	if(err) {
		FrmCustomAlert(InformationAlert, "Unable to load connector library", NULL, NULL);
	}
	else
	{
		err = PmConnectorLibOpen( refNum );
		if ( err != errNone )
		{
			FrmCustomAlert( InformationAlert, "Unable to open the library", " ", "" );
		}
		err = PmConnectorLibControl( refNum,
									 kPmConnectorLibCtrlPowerOn,
									 (void *)NULL);

		if ( err != errNone )
		{
			FrmCustomAlert( InformationAlert, "Unable to power on the power_out", " ", "" );
		}

		
		StrCopy( debugMessage, "Power on the power_out" );
		StrCat( debugMessage, ":" );
		PrvInformationDisplay( debugMessage, 1, SERIAL_TRANSRECV_SER_DET_FILE_NAME );

		err	= PmConnectorLibClose( refNum );

		if ( err != errNone )
		{
			FrmCustomAlert( InformationAlert, "Unable to close the library", " ", "" );
		}

	}
}

void PowerOffPowerOut( void )
{
	Char		debugMessage		[ 100 ];
	Err			err					= errNone;
	UInt16		refNum				= 0;
	
	
	// Try to find the  library
	err = SysLibFind(kPmConnectorLibName, &refNum);	
	if (err != errNone)
		err = SysLibLoad(kPmConnectorLibType, kPmConnectorLibCreator, &refNum);

	if(err) {
		FrmCustomAlert(InformationAlert, "Unable to load connector library", NULL, NULL);
	}
	else
	{
		err = PmConnectorLibOpen( refNum );

		if ( err != errNone )
		{
			FrmCustomAlert( InformationAlert, "Unable to open the library", " ", "" );
		}
		err = PmConnectorLibControl( refNum,
									 kPmConnectorLibCtrlPowerOff,
									 (void *)NULL);

		if ( err != errNone )
		{
			FrmCustomAlert( InformationAlert, "Unable to power off the power_out", " ", "" );
		}

		StrCopy( debugMessage, "Power off the power_out" );
		StrCat( debugMessage, ":" );
		PrvInformationDisplay( debugMessage, 1, SERIAL_TRANSRECV_SER_DET_FILE_NAME );

		err	= PmConnectorLibClose( refNum );

		if ( err != errNone )
		{
			FrmCustomAlert( InformationAlert, "Unable to close the library", " ", "" );
		}
	}
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
	Boolean		 handled			= false;
	Err			 err				= errNone;
	FormType	*frmP				= FrmGetActiveForm( );
	ControlType	*popTrigP			= (ControlType *)NULL;
	Char		*labelP				= (Char *)NULL;
	FieldPtr	 fieldDebugP		= (FieldPtr)NULL;
	FieldPtr	 fieldOutputP		= (FieldPtr)NULL;
//	UInt16		 portId				= 0;
	UInt16	   	 cardNo				= 0;
	LocalID	   	 dbId				= 0;
	Char		 tempMessage		[ 60 ];
	Char		 conversionString	[ maxStrIToALen ];
	ControlType *controlP 			= (ControlType *)NULL;
	ListType 	*listP				= (ListType *)NULL;
	UInt32 dev = 0;


	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldOutputP	= (FieldPtr)FrmGetPtr( frmP, MainOutputField );

	switch (eventP->eType)
	{
		case frmOpenEvent:

			MainFormInit( frmP );
			FrmDrawForm( frmP );
			
			// Set 5-way navigation focus to input field
			FrmNavObjectTakeFocus(frmP, MainInputField);
			
			if ( gPortId == 0 )
			{
				FrmHideObject( frmP, FrmGetObjectIndex( frmP, MainSerialLabel ) );
			}
			else
			{
				FrmShowObject( frmP, FrmGetObjectIndex( frmP, MainSerialLabel ) );
			}
			if ( gReceive == true )
			{
				FrmShowObject( frmP, FrmGetObjectIndex( frmP, MainReceiveLabel ) );
			}
			else
			{
				FrmHideObject( frmP, FrmGetObjectIndex( frmP, MainReceiveLabel ) );
			}
			
			// Initialize the baud rate
			controlP = FrmGetPtr(frmP, MainBaudRatePopTrigger);
			listP = FrmGetPtr(frmP, MainBaudRateList);
			if ( controlP && listP )
			{
				Int16 listPosition = 0;
				switch( gBaudRate )
				{
					case 2400:
						listPosition = 0;
						break;
					case 4800:
						listPosition = 1;
						break;
					case 9600:
						listPosition = 2;
						break;
					case 19200:
						listPosition = 3;
						break;
					case 38400:
						listPosition = 4;
						break;
					case 57600:
						listPosition = 5;
						break;
					case 115200:
						listPosition = 6;
						break;
					case 300:
						listPosition = 7;
						break;
				}
				CtlSetLabel( controlP, LstGetSelectionText( listP, listPosition ) );
				LstSetSelection( listP, listPosition );
			}

			handled = true;
			break;
			
		case frmCloseEvent:
			MainFormDeinit( frmP );
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainSerialOpenButton:
					if ( gPortId == 0 )
					{
						popTrigP		= FrmGetPtr( frmP, MainBaudRatePopTrigger );
						labelP			= (Char *)CtlGetLabel( popTrigP );
						gBaudRate		= (UInt32)StrAToI( labelP );
						
						err = PrvOpenSerialPort( );
						if ( err != errNone )
						{
							return ( false );
						}
						FrmShowObject( frmP, FrmGetObjectIndex( frmP, MainSerialLabel ) );
						StrCopy( tempMessage, "Opened Serial Port @ Baud = " );
						StrCat( tempMessage, StrIToA( conversionString, (Int32)gBaudRate ) );
						SetFieldTextFromStr( fieldDebugP, tempMessage, true );
						SrmReceiveFlush( gPortId, SERIAL_TRANSRECV_RECEIVE_FLUSH_TIMEOUT );
					}
					handled = true;
					break;
				
				case MainSerialCloseButton:
					if ( gPortId != 0 )
					{
						SrmClose( gPortId );
						gPortId = 0;
						FrmHideObject( frmP, FrmGetObjectIndex( frmP, MainSerialLabel ) );
						StrCopy( tempMessage, "Closed Serial Port" );
						SetFieldTextFromStr( fieldDebugP, tempMessage, true );
					}
					else
					{
						StrCopy( tempMessage, "Serial Port was not open" );
						SetFieldTextFromStr( fieldDebugP, tempMessage, true );
					}
					gReceive = false;
					FrmHideObject( frmP, FrmGetObjectIndex( frmP, MainReceiveLabel ) );
					handled = true;
					break;
					
				case MainSendButton:		
					// The "Send" button was hit.
					// Get the data from the field and send it using the serial port
					err = PrvSerialDataSend( );
					if ( err != errNone )
					{
						return ( false );
					}
					handled = true;
										
					break;


				case MainReceiveButton:		
					// The "Receive" button was hit.
					// Receive the serial data and display it.	
										
					gBuffer[ 0 ] = COMMON_CHAR_NULL;
					if ( gPortId != 0 )
					{
						gReceive = true;
					
						FrmShowObject( frmP, FrmGetObjectIndex( frmP, MainReceiveLabel ) );
						SetFieldTextFromStr( fieldOutputP, " ", true );
						StrCopy( tempMessage, "Ready to Receive Serial @ Baud = " );
						StrCat( tempMessage, StrIToA( conversionString, (Int32)gBaudRate ) );
						SetFieldTextFromStr( fieldDebugP, tempMessage, true );
						
						/* For Centro devices, receive data without waiting for nil event.
						   Subsequent nil events can also cause data to be received */	
						FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &dev);
						if( dev == 'D062' || // Centro Sprint
						    dev == 'D061') // Centro ATT
						{
							PrvSerialDataReceive( );
							
						}
										
					 }
					else
					{
						SetFieldTextFromStr( fieldDebugP, "Serial port is not open", true );
					}
					
					
					handled = true;
					
					break;
					
				case MainEnableDetectButton:
					err = SysCurAppDatabase( &cardNo, &dbId );
					SysNotifyRegister( cardNo,
									   dbId,
									   sysExternalConnectorAttachEvent,
									   0,
									   sysNotifyNormalPriority,
									   0 );

					SysNotifyRegister( cardNo,
									   dbId,
									   sysExternalConnectorDetachEvent,
									   0,
									   sysNotifyNormalPriority,
									   0 );
					
					SysNotifyRegister( cardNo,
									   dbId,
									   kPmConnectorNotifyType,
									   0,
									   sysNotifyNormalPriority,
									   0 );
					handled = true;
					break;
				
				case MainDisableDetectButton:
					err = SysCurAppDatabase( &cardNo, &dbId );
					SysNotifyUnregister( cardNo,
										 dbId,
										 sysExternalConnectorAttachEvent,
										 sysNotifyNormalPriority );

					SysNotifyUnregister( cardNo,
										 dbId,
										 sysExternalConnectorDetachEvent,
										 sysNotifyNormalPriority );
					
					SysNotifyUnregister( cardNo,
										 dbId,
										 kPmConnectorNotifyType,
										 sysNotifyNormalPriority );
					handled = true;
					break;
				
				case MainPOnButton:
					PowerOnPowerOut( );
					handled = true;
					break;
					
				case MainPOffButton:
					PowerOffPowerOut( );
					break;
					
				case MainCButton:
					gDebugMessage[ 0 ] = COMMON_CHAR_NULL;
					SetFieldTextFromStr( fieldDebugP, gDebugMessage, true );
					
					FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
					handled = true;
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
						FrmHideObject( frmP, FrmGetObjectIndex( frmP, MainSerialLabel ) );
						gPortId = 0;
						
					}
					gReceive = false;
					FrmHideObject( frmP, FrmGetObjectIndex( frmP, MainReceiveLabel ) );
					SetFieldTextFromStr( fieldDebugP, " ", true );
					SetFieldTextFromStr( fieldOutputP, " ", true );
					break;

				default:
					break;

			}
			break;


		case nilEvent:
		
			if ( gReceive == true && gPortId != 0 )
			{
				PrvSerialDataReceive( );
			}
			break;

		case menuEvent:
			PrvHandleMenu(eventP->data.menu.itemID);
			FrmDrawForm(frmP);
			handled = true;
			break;


		default:
			break;
	}
	
	if ( gPortId != 0 )
	{
		gSerialPortOn = true;
	}
	else
	{	
		gSerialPortOn = false;
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
	UInt32		value = 0;			
	
	gBuffer[ 0 ] = COMMON_CHAR_NULL;
	gDebugMessage[ 0 ] = COMMON_CHAR_NULL;
	
	FtrGet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_PORT_ID_FTR_NUM, &value );
	gPortId = (UInt16)value;
	FtrGet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_RECEIVE_FTR_NUM, &value );
	gReceive = (Boolean)value;
	FtrGet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_BAUD_RATE_FTR_NUM, &value );
	gBaudRate = (UInt32)value;
	if ( gBaudRate == 0 )
	{
		gBaudRate = SERIAL_TRANSRECV_BAUD_RATE_INITIAL;
	}
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
	UInt32		value = 0;
	
	value = (UInt32)gPortId;
	FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_PORT_ID_FTR_NUM, value );
	value = (UInt32)gReceive;
	FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_RECEIVE_FTR_NUM, value );
	value = (UInt32)gBaudRate;
	FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_BAUD_RATE_FTR_NUM, value );
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
	Err						 error				= errNone;
	Err						 err 				= errNone;
	SysNotifyParamType		*notifyP			= (SysNotifyParamType *)NULL;
	UInt16					 userData			= 0;
	Char					 conversionString	[ 32 ];
//	UInt16					 portId				= 0;
	Char					 debugMessage		[ 100 ];
	UInt32					 value				= 0;
//	UInt16					 refNum				= 0;
	UInt32					 deviceID			= 0;

//	UInt32					 numBytes			= 0;
	
	
	debugMessage[ 0 ] = COMMON_CHAR_NULL;
	cmdPBP;
	error = RomVersionCompatible( ourMinVersion, launchFlags );
	if (error) {
		return ( error );
	}

	switch (cmd)
	{
		case sysAppLaunchCmdSystemReset:
			FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_PORT_ID_FTR_NUM, 0 );
			FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_RECEIVE_FTR_NUM, 0 );
			FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_BAUD_RATE_FTR_NUM, 0 );
			FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
			FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_DET_T5_FTR_NUM, 0 );
			break;
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart( );
			if (error) {
				return ( error );
			}

			FrmGotoForm( MainForm );
			AppEventLoop( );

			AppStop( );
			break;

		
		case sysAppLaunchCmdNotify:
			notifyP		= (SysNotifyParamType *)( cmdPBP );
			userData	= ((UInt16)(notifyP->notifyDetailsP));
			switch ( notifyP->notifyType )
			{
				case kPmConnectorNotifyType:
					FrmCustomAlert( InformationAlert, "Class Detect Notifications", " ", " " );
					{
						Char 						conversionString		[ 32 ];
						UInt16 						code 					= kPmConnectorClassNoDevice;
						UInt16 						cmdId 					= kPmConnectorLibCtrlGetDeviceClass;
						UInt16          			refNumFlash				= 0;
						PmConnectorNotifyDataType  *connectorNotifyDataP 	= (PmConnectorNotifyDataType  *)(notifyP->notifyDetailsP);
						
						err = SysLibFind( kPmConnectorLibName, &refNumFlash);
						if (err)
						{
							UInt32          type 	   = kPmConnectorLibType;
							UInt32          libCreator = kPmConnectorLibCreator;
							
							
							err = SysLibLoad (type, libCreator, &refNumFlash);
							if (err)
							{
								FrmCustomAlert( InformationAlert, "Cannot find PmConnectorLib\n", " ", " ");
							}
						}
						err = PmConnectorLibControl ( refNumFlash, cmdId, (void *)&code);
						if (err)
							{
								FrmCustomAlert( InformationAlert, "PmConnectorLibControl error\n", " ", " ");
							}
						FrmCustomAlert( InformationAlert, "Code = ",
										StrIToH( conversionString, (UInt32)code ),
										" " );
						FrmCustomAlert( InformationAlert,
										"Class",
										StrIToH( conversionString, (UInt32)( connectorNotifyDataP->deviceClass & kPmConnectorClassMask ) ),
										" " );
						if ( connectorNotifyDataP->deviceClass & kPmConnectorDeviceAttach )
						{
							FrmCustomAlert( InformationAlert,
											"Attach",
											" ",
											" " );
						}
						
						if ( connectorNotifyDataP->deviceClass & kPmConnectorDeviceDetach )
						{
							FrmCustomAlert( InformationAlert,
											"Detach",
											" ",
											" " );
						}					
						
					}
					break;
				case sysExternalConnectorAttachEvent:
					StrCat( debugMessage, "At" );
					StrCat( debugMessage, StrIToH( conversionString, (UInt32)userData) );
					StrCat( debugMessage, ":" );
					PrvInformationDisplay( debugMessage, 1, SERIAL_TRANSRECV_SER_DET_FILE_NAME );
				
					FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
					if ( deviceID == 'TnT5' )
					{
						FtrGet( SERIAL_TRANSRECV_CREATOR_ID,
								SERIAL_TRANSRECV_SER_DET_FTR_NUM,
								&value );
						
						value = value + 1;
						switch ( value )
						{
							case 1:
								if ( userData == SERIAL_TRANSRECV_DET_USB )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM,
											value );
									PrvInformationDisplay( "1: At 0x0010", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
									PowerOffPowerOut( );
								}
								else
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM,
											0 );
								}
								break;
							case 3:
								if ( userData == SERIAL_TRANSRECV_DET_SERIAL )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM,
											value );
									PrvInformationDisplay( "3: At 0x0040", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
									PrvInformationDisplay( "Tungsten T5 serial Attached", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );	
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_T5_FTR_NUM,
											1 );
								}
								else if ( userData == SERIAL_TRANSRECV_DET_USB )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM,
											1 );
									PrvInformationDisplay( "1: At 0x0010", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
									PowerOffPowerOut( );
									
								}
								else
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
								}
								break;
							case 5:
								if ( userData == SERIAL_TRANSRECV_DET_USB )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM, value );
									PrvInformationDisplay( "5: At 0x0010", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
								}
								else
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
								}
								break;
							default:
								FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
										SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
								break;
						
						}
					}
					break;
				
				case sysExternalConnectorDetachEvent:
					
					StrCat( debugMessage, "Dt" );
					StrCat( debugMessage, StrIToH( conversionString, (UInt32)userData) );
					StrCat( debugMessage, ":" );
					PrvInformationDisplay( debugMessage, 1, SERIAL_TRANSRECV_SER_DET_FILE_NAME );

					
					FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
					if ( deviceID == 'TnT5' )
					{
						FtrGet( SERIAL_TRANSRECV_CREATOR_ID,
								SERIAL_TRANSRECV_SER_DET_FTR_NUM, &value );
						value = value + 1;
						
						switch ( value )
						{
							case 2:
								if ( userData == SERIAL_TRANSRECV_DET_USB )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM, value );
									PrvInformationDisplay( "2: Dt 0x0010", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
								}
								else
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID, SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
								}
								break;
							case 4:
								if ( userData == SERIAL_TRANSRECV_DET_SERIAL )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM, value );
									PrvInformationDisplay( "4: Dt 0x0040", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
								}
								else
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
								}
								break;
							case 6:
								if ( userData == SERIAL_TRANSRECV_DET_USB )
								{
									FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
											SERIAL_TRANSRECV_SER_DET_T5_FTR_NUM,
											0 );
									PrvInformationDisplay( "6: Dt 0x0010", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );
									PrvInformationDisplay( "Tungsten T5 serial Detached", 1,
														   SERIAL_TRANSRECV_SER_DET_FILE_NAME );	
								}
							default:
								FtrSet( SERIAL_TRANSRECV_CREATOR_ID,
										SERIAL_TRANSRECV_SER_DET_FTR_NUM, 0 );
								break;
						}
					}

					break;

				default:
					break;
			}
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
