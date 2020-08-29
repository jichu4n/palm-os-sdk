/**************************************************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**************************************************************************************************
 *
 * File:		IRUnframer.c
 *
 * Description:	This file contains code that receives an IrDA Packet. It then extracts the raw data
 *				from the IrDA packet and displays the raw data.
 *
 * Version:		1.0	- Initial Revision (09/05/03)
 * 	
 *************************************************************************************************/

#include <PalmOS.h>
#include "Common.h"
#include "IRUnframer.h"
#include "IRUnframerRsc.h"				
#include "SerialMgr.h"
#include "FeatureMgr.h"

#define IR_UNFRAMER_PORT						serPortIrPort
	
typedef enum                                	
{                                           	
	IR_UNFRAMER_ERROR_CRC_INCORRECT				= 1,
	IR_UNFRAMER_ERROR_PACKET_CORRUPT			= 2,
	IR_UNFRAMER_ERROR_PACKET_PTR_INVALID		= 3,
	IR_UNFRAMER_ERROR_LENGTH_PTR_INVALID		= 4,
	IR_UNFRAMER_ERROR_UNKNOWN
} IRUNFRAMERErrorCode;

// IrDA payload contains Address byte, Control byte, 2 bytes of CRC and any number of information
// bytes
#define IR_UNFRAMER_IRDA_PAYLOAD_LEN_MIN		( 4 )
	
#define IR_UNFRAMER_IRDA_CRC_SIZE				( 2 )
#define IR_UNFRAMER_IRDA_ADDR_CONTROL_SIZE		( 2 )
	
#define IR_UNFRAMER_RECEIVE_FLUSH_TIMEOUT		( 0 )
#define IR_UNFRAMER_GET_EVENT_TIMEOUT			( 1 )

#define IR_UNFRAMER_BAUD_RATE_INITIAL			( 2400 )
#define IR_UNFRAMER_ERROR_OLD_SERIAL_MANAGER	( 1 )

static Boolean		gReceive					= false;
static UInt16		gPortId						= 0;
static UInt32		gBaudRate					= IR_UNFRAMER_BAUD_RATE_INITIAL;


static UInt16 fcsTable[ 256 ] = {
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

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
//	Err err = errNone;
	FormType *frmAboutP = NULL;
	
	switch(itemID)
	{
	
		case HelpAboutIRUnframer:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
						
			break;
	}
}

/**************************************************************************************************
 *
 * FUNCTION:    PrvFcsCalculate		
 *
 * DESCRIPTION: Calculate a new FCS given the current FCS and the new data.
 *
 * PARAMETERS:	fcs			- The current FCS value.
 *				dataP		- The new data.
 *				length		- length of the new data string.
 *
 * RETURNED:	The new FCS value
 *
 *************************************************************************************************/

static inline UInt16 PrvFcsCalculate( register UInt16			 fcs,
									  register UInt8			*dataP,
									  register UInt32			 length )
{
	while (length != 0)
	{
		fcs = ( fcs >> COMMON_SHIFT_8 ) ^ fcsTable[ ( fcs ^ *dataP++ ) & COMMON_MASK_8_0XFF ];
		length--;
	}

	return ( fcs );
}

/**************************************************************************************************
 *
 * FUNCTION:    PrvHexPrint				
 *
 * DESCRIPTION:	Converts a char string to ascii values( hex ) and displays it in a field.
 *				if requested, returns the converted string.
 *
 * PARAMETERS:	bufferP			- Char String to be converted.
 *				size			- The size of the string.
 *				objectId		- ID of an object in the form.
 *				tempHexBufferP	- Pointer to the location where the converted string will
 *								  be copied.
 *
 * RETURNED:	none
 *
 *************************************************************************************************/

static void PrvHexPrint( const UInt8		*bufferP,
						 const UInt32		 size,
						 const UInt16		 objectId,
						 const UInt8		*tempHexBufferP )
{
	UInt16		 byteIterator	= 0;
	Char		 bufferHex		[ COMMON_BUFFER_HEX_LEN ];
	Char		 stringHex		[ 6 ];
	FieldPtr	 fieldP			= (FieldPtr)NULL;
	FormType	*frmP			= (FormType *)FrmGetActiveForm( );


	fieldP	= (FieldPtr)FrmGetPtr( frmP, objectId );
	bufferHex[ 0 ] = COMMON_CHAR_NULL;
	for ( byteIterator = 0; byteIterator < size; byteIterator++ )
	{
		StrPrintF( stringHex, "%2X ", (UInt8)( bufferP[ byteIterator ] & 0x00ff ) );
		StrCat( bufferHex, &( stringHex[ 2 ] ) );
	}

	if ( tempHexBufferP != (UInt8 *)NULL )
	{
		StrCopy( (Char *)tempHexBufferP, bufferHex );
	}


	SetFieldTextFromStr( fieldP, bufferHex, true );
}
			
/**************************************************************************************************
 *
 * FUNCTION:    PrvIrDAUnframe	
 *
 * DESCRIPTION: Extracts the raw data from an IrDA Packet.
 *
 * PARAMETERS:	irDaPacketP			- An IrDA Packet.
 *				irDaPacketLength	- Length of the IrDA Packet.
 *				rawDataP			- Location where the Raw Data will be stored
 *				rawDataLengthP		- This parameter is used to return the length of the raw data
 *									  unframed from an IrDA Packet.
 *
 * RETURNED:	Error code if there is an error else errNone
 *
 *************************************************************************************************/

static Err PrvIrDAUnframe( const UInt8	*const irDaPacketP,
						   UInt32		 irDaPacketLength,
						   UInt8		*const rawDataP,
						   UInt32		*const rawDataLengthP )
{
	UInt16	 state				= COMMON_STATE_START;
	UInt32	 packetSize			= irDaPacketLength;
	UInt8	*dataP				= (UInt8 *)irDaPacketP;
	UInt8	*dataRawP			= (UInt8 *)rawDataP;
	UInt32	 payloadLength		= 0;
	UInt8	 bufferHex			[ COMMON_BUFFER_HEX_LEN ];
	UInt16	 crcCalculated		= COMMON_INIT_FCS;
	UInt16	 crcPayload			= 0;
	Err		 errorStatus		= errNone;
	
	
	if (irDaPacketP == (UInt8 *)NULL) {
		return ( IR_UNFRAMER_ERROR_PACKET_PTR_INVALID );
	}

	if (rawDataP == (UInt8 *)NULL) {
		return ( IR_UNFRAMER_ERROR_PACKET_PTR_INVALID );
	}

	if (rawDataLengthP == (UInt32 *)NULL)
	{
		return ( IR_UNFRAMER_ERROR_LENGTH_PTR_INVALID );
	}

	while (packetSize > 0)
	{
		switch (state)
		{
			case COMMON_STATE_START:

				if (*dataP == COMMON_IRDA_EXTRA_BOF) {
					state = COMMON_STATE_XBOF;
				}
				else if (*dataP == COMMON_IRDA_BOF)
				{
					state = COMMON_STATE_PAYLOAD;
					dataP++;
					packetSize--;
					payloadLength = 0;
				}
				else if (*dataP == COMMON_IRDA_EOF) {
					state = COMMON_STATE_FRAME_END;
				}
				else {
					state = COMMON_STATE_XBOF;
				}
				break;


			case COMMON_STATE_XBOF:
				dataP++;
				packetSize--;

				if (*dataP == COMMON_IRDA_EXTRA_BOF) {
					state = COMMON_STATE_XBOF;
				}
				else if (*dataP == COMMON_IRDA_BOF)
				{
					if ( *( dataP + 1 ) == COMMON_IRDA_BOF )
					{
						state = COMMON_STATE_XBOF;
					}
					else
					{
						state = COMMON_STATE_PAYLOAD;
					
						payloadLength 	=  0;
						dataP 		  	+= 1;
						packetSize		-= 1;
					}
				}
				else if (*dataP == COMMON_IRDA_EOF) {
					state = COMMON_STATE_FRAME_END;
				}
				else {
					state = COMMON_STATE_XBOF;
				}
				break;

			case COMMON_STATE_FRAME_END:
				packetSize = 0;
				*dataRawP  = COMMON_CHAR_NULL;
				break;

			case COMMON_STATE_PAYLOAD:	//all other cases
				
				if (*dataP == COMMON_IRDA_EOF) {	
					state = COMMON_STATE_FRAME_END;
					break;
				}
				else if (*dataP == COMMON_IRDA_ESC)
				{
					state = COMMON_STATE_IR_ESC;
					packetSize--;
					if (packetSize > 0) {
						dataP++;
					}

					break;
				}
				*dataRawP = *dataP;

				dataP++;
				dataRawP++;
				packetSize--;
				payloadLength++;

				break;

			case COMMON_STATE_IR_ESC:
				*dataRawP = (UInt8)(*dataP ^ (UInt8)COMMON_IRDA_ESC_XOR);
				state = COMMON_STATE_PAYLOAD;
				dataP++;
				dataRawP++;
				packetSize--;
				payloadLength++;
				break;

		}	
	}

	dataRawP = (UInt8 *)rawDataP;
	if ( payloadLength >= IR_UNFRAMER_IRDA_PAYLOAD_LEN_MIN )
	{
		crcCalculated = PrvFcsCalculate( crcCalculated,
										 dataRawP,
										 ( payloadLength - IR_UNFRAMER_IRDA_CRC_SIZE ) );
		crcCalculated = ~( crcCalculated );
		crcCalculated = (   ( ( crcCalculated & COMMON_MASK_8_0XFF ) << COMMON_SHIFT_8 )
						  | ( ( crcCalculated >> COMMON_SHIFT_8    ) &  COMMON_MASK_8_0XFF ) );

		crcPayload = (UInt16)*( dataRawP + ( payloadLength - IR_UNFRAMER_IRDA_CRC_SIZE ) );

		crcPayload <<= COMMON_SHIFT_8;
		
		crcPayload |= (UInt16)*( dataRawP + ( payloadLength - IR_UNFRAMER_IRDA_CRC_SIZE ) + 1 );
		
		if ( crcCalculated != crcPayload )
		{
			errorStatus = IR_UNFRAMER_ERROR_CRC_INCORRECT;
		}

		// Remove the CRC
		payloadLength = payloadLength - IR_UNFRAMER_IRDA_CRC_SIZE;

		// Remove the Address and Control
		payloadLength = payloadLength - IR_UNFRAMER_IRDA_ADDR_CONTROL_SIZE;
		if ( payloadLength != 0 )
		{
			MemMove( bufferHex, &dataRawP[ IR_UNFRAMER_IRDA_ADDR_CONTROL_SIZE ], (Int32)payloadLength );
			MemMove( dataRawP, bufferHex, (Int32)payloadLength );
		}

		*rawDataLengthP = payloadLength;

	}
	else
	{
		errorStatus = IR_UNFRAMER_ERROR_PACKET_CORRUPT;
	}

	return ( errorStatus );
}

/**************************************************************************************************
 *
 * FUNCTION:	PrvOpenIrPort
 *
 * DESCRIPTION:	Opens the IR Port in the raw mode.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	Error Code if there is an error else errNone
 *
 *************************************************************************************************/

Err	PrvOpenIrPort( void )
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
		return ( IR_UNFRAMER_ERROR_OLD_SERIAL_MANAGER );
	}

	/* open serial port */
	err = SrmOpen( serPortIrPort, gBaudRate, &gPortId );
	if (err != errNone)
	{
		FrmCustomAlert( RomIncompatibleAlert, "SrmOpen failed", " ", " " );
		return ( err );
	}

	err = SrmControl( gPortId, srmCtlIrDAEnable, (void *)NULL, (UInt16 *)NULL );
	if (err != errNone)
	{
		FrmCustomAlert( RomIncompatibleAlert, "srmCtlIrDAEnable failed", " ", " " );
		SrmClose( gPortId );
		gPortId = 0;
		return ( err );
	}

	err = SrmControl( gPortId, srmCtlRxEnable, (void *)NULL, (UInt16 *)NULL );
	if (err != errNone)
	{
		FrmCustomAlert( RomIncompatibleAlert, "srmCtlRxEnable failed", " ", " " );
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
 * FUNCTION:	PrvIRDataReceive		
 *
 * DESCRIPTION:	This function receives an IrDA Packet. It then extracts the raw data from the IrDA
 *				packet and displays the raw data.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	none
 *
 *************************************************************************************************/

void PrvIRDataReceive( void )
{
	UInt32		 numOfBytes			= 0;
	UInt32		 rawDataLength		= 0;
	Err			 err				= errNone;
	Err			 errorStatus		= errNone;
	FormType	*frmP				= (FormType *)FrmGetActiveForm( );
	FieldPtr	 fieldOutputP		= (FieldPtr)NULL;
	FieldPtr	 fieldDebugP		= (FieldPtr)NULL;
	UInt8		 receiveBuffer		[ COMMON_BUFFER_LEN ];
	UInt8		 rawData			[ COMMON_BUFFER_LEN ];
	Char		 tempMessage		[ COMMON_BUFFER_HEX_LEN ];
	Char		 conversionString	[ maxStrIToALen ];
	UInt8		*serialBufferP		= (UInt8 *)NULL;
	UInt32		 freeSpace			= ( sizeof( receiveBuffer ) - 1 ) ;
	UInt32		 numBytesWindow		= 0;
	UInt8		*receiveBufferP		= receiveBuffer;


	fieldOutputP	= (FieldPtr)FrmGetPtr( frmP, MainOutputField );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	
	MemSet( rawData, COMMON_BUFFER_LEN, 0 );
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
		errorStatus = PrvIrDAUnframe( receiveBuffer, numOfBytes, rawData, &rawDataLength );
		switch (errorStatus)
		{
			case errNone:
				StrCopy( tempMessage, "IR Data Receive OK, Bytes " );
				break;
			case IR_UNFRAMER_ERROR_CRC_INCORRECT:
				StrCopy( tempMessage, "IR Data Receive Error ( CRC Incorrect ),	Bytes " );
				break;
			case IR_UNFRAMER_ERROR_PACKET_CORRUPT:
				StrCopy( tempMessage, "IR Data Receive Error ( Packet Corrupt ), Bytes " );
				break;
			case IR_UNFRAMER_ERROR_PACKET_PTR_INVALID:
				StrCopy( tempMessage, "IR Data Receive Error ( Ptr Invalid ), Bytes " );
				break;
			case IR_UNFRAMER_ERROR_LENGTH_PTR_INVALID:
				StrCopy( tempMessage, "IR Data Receive Error ( Len Ptr Invalid ), Bytes " );
				break;
			default:
				StrCopy( tempMessage, "IR Data Receive Error ( Unknown ), Bytes " );
				break;
		}

		rawData[ rawDataLength ] = COMMON_CHAR_NULL;

		if ( rawDataLength == 0 )
		{
			SetFieldTextFromStr( fieldOutputP, " ", true );
		}
		else
		{
			SetFieldTextFromStr( fieldOutputP, (Char *)rawData, true );
		}
		PrvHexPrint( receiveBuffer, numOfBytes, MainOutputHexField, (UInt8 *)NULL );

		StrCat( tempMessage, StrIToA( conversionString, (Int32)numOfBytes ) );

		StrCat( tempMessage, ", Baud Rate " );
		StrCat( tempMessage, StrIToA( conversionString, (Int32)gBaudRate ) );
		StrCat( tempMessage, ", Ready to receive IrDA!"	);
		SetFieldTextFromStr( fieldDebugP, tempMessage, true );
		SrmReceiveFlush( gPortId, IR_UNFRAMER_RECEIVE_FLUSH_TIMEOUT );
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

void MainFormInit(void )
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

void MainFormDeinit(void)
{
	
}

/**************************************************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: Main form event handler.
 *
 * PARAMETERS:	eventP - Pointer to an event.
 *
 * RETURNED:	True if event was handled.
 *
 *************************************************************************************************/

Boolean MainFormHandleEvent( EventType	*eventP )
{
	Boolean		 handled			= false;
	//Err			 err				= errNone;
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
			MainFormInit();
			FrmDrawForm(frmP);
			handled = true;
			break;
			
		case frmCloseEvent:
			MainFormDeinit();
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainReceiveButton:		
					// The "Receive" button was hit.
					// Receive the IrDA packet. Unframe the raw data from the packet
					// and display it.
					popTrigP		= FrmGetPtr( frmP, MainBaudRatePopTrigger );
					labelP			= (Char *)CtlGetLabel( popTrigP );
					gBaudRate		= (UInt32)StrAToI( labelP );
					if (gPortId != 0)
					{
						SrmClose( gPortId );
						gPortId = 0;
					}
					PrvOpenIrPort( );
					SrmReceiveFlush( gPortId, IR_UNFRAMER_RECEIVE_FLUSH_TIMEOUT );
					gReceive = true;
					handled = true;

					SetFieldTextFromStr( fieldOutputP, " ", true );
					SetFieldTextFromStr( fieldOutputHexP, " ", true );
					SetFieldTextFromStr( fieldDebugP, "Ready to Receive IrDA!", true );

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
					SetFieldTextFromStr( fieldOutputP, " ", true );
					SetFieldTextFromStr( fieldOutputHexP, " ", true );
					SetFieldTextFromStr( fieldDebugP, " ", true );
					break;
				default:
					break;

			}
			break;

		case nilEvent:
			if ( gReceive == true )
			{
				PrvIRDataReceive( );
			}
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
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: Main application event handler.
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
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: Main Application event loop.
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
		EvtGetEvent( &event, IR_UNFRAMER_GET_EVENT_TIMEOUT );
					
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
 * FUNCTION:    AppStart
 *
 * DESCRIPTION: Called when the application starts
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
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Called when the application exits
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
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: Check if the ROM is compatible with the application
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
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: Main entry point for the application.
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
