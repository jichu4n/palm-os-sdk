/**************************************************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**************************************************************************************************
 *
 * File:		IRFramerZire71.c
 *
 * Description:	This file contains code that retrieves raw data from a field and encapsulates it in
 *				an IrDA frame and then sends it using Raw IR. This code is intended for Zire 71.
 *
 * Version:		1.0	- Initial Revision (10/06/03)
 * 	
 *************************************************************************************************/

#include <PalmOS.h>
#include <HsNav.h>
#include "Common.h"
#include "IRFramer.h"	 							
#include "IRFramerZire71Rsc.h"		
#include "SerialMgr.h"
#include "FeatureMgr.h"

#define IR_FRAMER_PORT						serPortIrPort
#define IR_FRAMER_ERROR_CODE				( 1 )

#define IR_FRAMER_ADD_BOF_PARAM_VALUE		( 12 )

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
 * FUNCTION:	PrvFcsCalculate		
 *
 * DESCRIPTION:	Calculate a new FCS given the current FCS and the new data.
 *
 * PARAMETERS:	fcs			- The current FCS value.
 *				dataP		- The new data.
 *				length		- length of the new data string.
 *
 * RETURNED:	The new FCS value
 *
 *************************************************************************************************/

static inline UInt16 PrvFcsCalculate( register UInt16			 fcs,
									  register unsigned char	*dataP,
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
 * FUNCTION:	PrvHexPrint				
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
/****
static void PrvHexPrint( const UInt8		*const bufferP,
						 const UInt32		 size,
						 const UInt16		 objectId,
						 UInt8				*const tempHexBufferP )
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
		StrPrintF( stringHex, "%2X ", (UInt8)( bufferP[ byteIterator ] & COMMON_MASK_8_0XFF ) );
		StrCat( bufferHex, &( stringHex[ 2 ] ) );
	}

	if ( tempHexBufferP != (UInt8 *)NULL )
	{
		StrCopy( (Char *)tempHexBufferP, bufferHex );
	}


	SetFieldTextFromStr( fieldP, bufferHex, true );
}
****/
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
/****
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
****/
/**************************************************************************************************
 *
 * FUNCTION:	PrvIrDAFrame
 *
 * DESCRIPTION:	Encapsulates the raw data in an IrDA Frame and splits the IrDA Frame into four
 *				parts - Header, Payload, CRC and Trailer.
 *
 * PARAMETERS:	headerP				- Location where the header part of the IrDA Packet will be
 *									  stored.
 *				headerLengthP		- This parameter returns the length of the header.
 *				payloadP			- Location where the payload part of the IrDA Packet will be
 *									  stored
 *				payloadLengthP		- This parameter returns the length of the payload.
 *				crcP				- Location where the CRC part of the IrDA Packet will be
 *									  stored.
 *				crcLengthP			- This parameter returns the length of the CRC.
 *				trailerP			- Location where the trailer part of the IrDA Packet will be
 *									  stored.
 *				trailerLengthP		- This parameter returns the length of the trailer part.
 *				rawDataP			- Raw Data to be encapsulated.
 *				rawDataLength		- The length of the Raw Data.
 *				baudRate			- The Baud Rate for packet transmission.
 *				additionalBofValue	- Additional BOFs parameter value, helps determine how many
 *									  additional BOFs are needed.
 *
 * RETURNED:	Error code if there is an error else errNone
 *
 *************************************************************************************************/

static Err PrvIrDAFrame( UInt8			*const headerP,
						 UInt32			*const headerLengthP,
						 UInt8			*const payloadP,
						 UInt32			*const payloadLengthP,
						 UInt8			*const crcP,
						 UInt32			*const crcLengthP,
						 UInt8			*const trailerP,
						 UInt32			*const trailerLengthP,
						 const UInt8	*rawDataP,
						 const UInt32	 rawDataLength,
						 UInt32			 baudRate,
						 UInt8			 additionalBofValue )
{
	UInt32	 rawDataSize		= rawDataLength;
	UInt32	 payloadSize		= 0;
	UInt8	 numberOfXBof		= 0;
	UInt8	 byteIterator		= 0;
	UInt16	 crc				= COMMON_INIT_FCS;
	UInt8	*tempHeaderP		= headerP;
	UInt8	*tempPayloadP		= payloadP;
	UInt8	*startOfPayloadP	= payloadP;
	UInt8	*tempCrcP			= crcP;
	UInt8	*tempTrailerP		= trailerP;
	UInt8	*tempRawDataP		= (UInt8 *)rawDataP;


	if (headerP == (UInt8 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	if (headerLengthP == (UInt32 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	else {
		(*headerLengthP) = 0;
	}
	if (payloadP == (UInt8 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	if ( payloadLengthP == (UInt32 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	else {
		(*payloadLengthP) = 0;
	}
	if (crcP == (UInt8 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	if ( crcLengthP == (UInt32 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	else {
		(*crcLengthP) = 0;
	}
	if (trailerP == (UInt8 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	if (trailerLengthP == (UInt32 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}
	else {
		(*trailerLengthP) = 0;
	}

	if (rawDataP == (UInt8 *)NULL) {
		return ( IR_FRAMER_ERROR_CODE );
	}

	// Calculate the number of extra beginning of frame characters to be added based on the baud
	// rate
	switch (baudRate)
	{
		case 2400:
			numberOfXBof = (UInt8)( additionalBofValue / 48 );
			break;
		case 9600:
			numberOfXBof = (UInt8)( additionalBofValue / 12 );
			break;
		case 19200:
			numberOfXBof = (UInt8)( additionalBofValue / 6 );
			break;
		case 38400:
			numberOfXBof = (UInt8)( additionalBofValue / 3 );
			break;
		case 57600:
			numberOfXBof = (UInt8)( additionalBofValue / 2 );
			break;
		case 115200:
			numberOfXBof = (UInt8)( additionalBofValue / 1 );
			break;
		default:
			return ( IR_FRAMER_ERROR_CODE );
	}

	// Add the extra beginning of frame character based on the baud rate
	for ( byteIterator = 0; byteIterator < numberOfXBof; byteIterator++ )
	{
		*tempHeaderP = COMMON_IRDA_EXTRA_BOF;
		tempHeaderP++;
		(*headerLengthP)++;
	}

	// Add the beginning of frame character
	*tempHeaderP = COMMON_IRDA_BOF;
	tempHeaderP++;
	(*headerLengthP)++;
	
	*tempHeaderP = COMMON_CHAR_NULL;

	// Address - Broadcast a command frame
	*tempPayloadP = COMMON_IRDA_ADDR_BROADCAST_CMD;
	tempPayloadP++;
	payloadSize++;
	(*payloadLengthP)++;
	// Control - Test Command			     		
	*tempPayloadP = COMMON_IRDA_CONTROL_TEST_CMD;
	tempPayloadP++;
	payloadSize++;
	(*payloadLengthP)++;

	// Add the raw data as a payload to the frame
	for ( byteIterator = 0; byteIterator < rawDataSize; byteIterator++ )
	{
		// if there are special characters then escape the special characters
		if (    (UInt8)*tempRawDataP == COMMON_IRDA_ESC
			 || (UInt8)*tempRawDataP == COMMON_IRDA_BOF
			 || (UInt8)*tempRawDataP == COMMON_IRDA_EOF )
		{
			*tempPayloadP = COMMON_IRDA_ESC;
			tempPayloadP++;	
			payloadSize++;
			(*payloadLengthP)++;
			*tempPayloadP = (UInt8)((UInt8)( *tempRawDataP ) ^ (UInt8)COMMON_IRDA_ESC_XOR);
			tempRawDataP++;
			tempPayloadP++;
			payloadSize++;
			(*payloadLengthP)++;
		}
		else
		{
			*tempPayloadP = *tempRawDataP;
			tempPayloadP++;
			payloadSize++;
			(*payloadLengthP)++;
			tempRawDataP++;
		}
	}
	
	*tempPayloadP = COMMON_CHAR_NULL;
	
	// Calculate the CRC and add to the frame.
	
	// Calculate the partial CRC for the Address and Control Bytes
	crc = PrvFcsCalculate( crc, (UInt8 *)startOfPayloadP, 2 );
	
	// Calculate the full CRC using the CRC for Address and Control Bytes and
	// using the raw data
	crc = PrvFcsCalculate( crc, (UInt8 *)rawDataP, rawDataSize );
													  	
	crc = ~( crc );

	if (    (UInt8)( crc ) == COMMON_IRDA_ESC
		 || (UInt8)( crc ) == COMMON_IRDA_BOF
		 || (UInt8)( crc ) == COMMON_IRDA_EOF )
	{
		*tempCrcP = COMMON_IRDA_ESC;
		tempCrcP++;
		(*crcLengthP)++;
		*tempCrcP = (UInt8)( (UInt8)( (Char)( crc ) ) ^ (UInt8)COMMON_IRDA_ESC_XOR );
		tempCrcP++;
		(*crcLengthP)++;
	}
	else
	{
		*tempCrcP = (UInt8)( crc );
		tempCrcP++;
		(*crcLengthP)++;
	}

	if (    (UInt8)( crc >> COMMON_SHIFT_8 ) == COMMON_IRDA_ESC
		 || (UInt8)( crc >> COMMON_SHIFT_8 ) == COMMON_IRDA_BOF
		 || (UInt8)( crc >> COMMON_SHIFT_8 ) == COMMON_IRDA_EOF )
	{
		*tempCrcP = COMMON_IRDA_ESC;
		tempCrcP++;
		(*crcLengthP)++;
		*tempCrcP = (UInt8)(   (UInt8)( (Char)( crc >> COMMON_SHIFT_8 ) )
							 ^ (UInt8)COMMON_IRDA_ESC_XOR );
		tempCrcP++;
		(*crcLengthP)++;
	}
	else
	{
		*tempCrcP = (UInt8)( crc >> COMMON_SHIFT_8 );
		tempCrcP++;
		(*crcLengthP)++;
	}
	
	*tempCrcP = COMMON_CHAR_NULL;

	// Add the end of frame character
	*tempTrailerP = COMMON_IRDA_EOF;
	tempTrailerP++;
	(*trailerLengthP)++;

	*tempTrailerP = COMMON_CHAR_NULL;

	return ( errNone );
}

/**************************************************************************************************
 *
 * FUNCTION:	PrvIRDataSend				
 *
 * DESCRIPTION:	This function retrieves the raw data from the field and encapsulates it in
 *				an IrDA frame and then sends it using Raw IR.
 *
 * PARAMETERS:	none.
 *
 * RETURNED:	Error code if there is an error else errNone
 *
 *************************************************************************************************/

static Err PrvIRDataSend( void )
{
	UInt32		 value					= 0;
//	UInt8		 byte					= 0;
	Err			 err					= errNone;
	UInt16		 portId					= 0;
	UInt32		 numBytesToSend			= 0;
	UInt32		 numSent				= 0;
	Char		 message        		[ COMMON_BUFFER_LEN ] = "NO USER INPUT!\n";
	UInt32		 messageLength			= 0;
	Char		 messageHex     		[ COMMON_BUFFER_HEX_LEN ];
	UInt8		 payloadBuffer			[ COMMON_BUFFER_LEN ];
	UInt32		 payloadLength			= 0;
	UInt8		 headerBuffer			[   IR_FRAMER_ADD_BOF_PARAM_VALUE
										  + COMMON_IRDA_SIZEOF_BOF
										  + COMMON_SIZEOF_NULL ];
	UInt32		 headerLength			= 0;
	UInt8		 crcBuffer				[   COMMON_IRDA_SIZEOF_CRC
										  + COMMON_IRDA_SIZEOF_ESC
										  + COMMON_IRDA_SIZEOF_ESC
										  + COMMON_SIZEOF_NULL ];
	UInt32		 crcLength				= 0;
	UInt8		 trailerBuffer			[ COMMON_IRDA_SIZEOF_EOF + COMMON_SIZEOF_NULL ];
	UInt32		 trailerLength			= 0;
//	UInt16		 messageHexLength		= 0;
	Char		 tempMessage    		[ COMMON_BUFFER_HEX_LEN ];
	Char		 conversionString		[ maxStrIToALen ];
	Char		 stringHex      		[ 6 ];
	UInt8		 byteIterator			= 0;
//	UInt8		 messageByteIterator	= 0;
	Char		*fieldTextP				= (Char *)NULL;
//	Char		*fieldTextHexP			= (Char *)NULL;
	FieldPtr	 fieldInputP			= (FieldPtr)NULL;
	FieldPtr	 fieldInputHexP			= (FieldPtr)NULL;
	FieldPtr	 fieldDebugP			= (FieldPtr)NULL;
	FormType	*frmP					= (FormType *)FrmGetActiveForm( );
	UInt32		 flags					= 0;
	Int32		 paramSize				= 0;
	UInt32		 baudRate				= 0;
	ControlType	*popTrigP				= (ControlType *)NULL;
	Char		*labelP					= (Char *)NULL;


	MemSet( messageHex, sizeof( messageHex ), 0 );
	MemSet( headerBuffer, sizeof( headerBuffer ), 0 );
	MemSet( payloadBuffer, sizeof( payloadBuffer ), 0 );
	MemSet( crcBuffer, sizeof( crcBuffer ), 0 );
	MemSet( trailerBuffer, sizeof( trailerBuffer ), 0 );
	MemSet( tempMessage, sizeof( tempMessage ), 0 );
	fieldDebugP		= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldInputP		= (FieldPtr)FrmGetPtr( frmP, MainInputField );
	fieldInputHexP	= (FieldPtr)FrmGetPtr( frmP, MainInputHexField );

	popTrigP	= FrmGetPtr( frmP, MainBaudRatePopTrigger );
	labelP		= (Char *)CtlGetLabel( popTrigP );
	baudRate	= (UInt32)StrAToI( labelP );
	
	SetFieldTextFromStr( fieldDebugP, "Send IR Data Function", true );
	fieldTextP		= FldGetTextPtr( fieldInputP );
	FldGetTextPtr( fieldInputHexP );

	if ( (fieldTextP != (Char *)NULL) && (StrLen( fieldTextP ) > 0) )
	{
		SetFieldTextFromStr( fieldDebugP, "Input", true );
		StrCopy( message, fieldTextP );

		messageLength = StrLen( message );

		PrvIrDAFrame( headerBuffer,
					  &headerLength,
					  payloadBuffer,
					  &payloadLength,
					  crcBuffer,
					  &crcLength,
					  trailerBuffer,
					  &trailerLength,
					  (UInt8 *)message,
					  messageLength,
					  baudRate,
					  IR_FRAMER_ADD_BOF_PARAM_VALUE );
		messageHex[ 0 ] = COMMON_CHAR_NULL;
		numBytesToSend =   headerLength
						 + payloadLength
						 + crcLength
						 + trailerLength;
		for ( byteIterator = 0;
			  byteIterator < headerLength;
			  byteIterator++ )
		{
			StrPrintF( stringHex, "%02X ", (UInt8)headerBuffer[ byteIterator ] );
			StrCat( messageHex, &( stringHex[ 2 ] ) );
		}	
		
		for ( byteIterator = 0;
			  byteIterator < payloadLength;
			  byteIterator++ )
		{
			StrPrintF( stringHex, "%02X ", (UInt8)payloadBuffer[ byteIterator ] );
			StrCat( messageHex, &( stringHex[ 2 ] ) );
		}
		
		for ( byteIterator = 0;
			  byteIterator < crcLength;
			  byteIterator++ )
		{
			StrPrintF( stringHex, "%02X ", (UInt8)crcBuffer[ byteIterator ] );
			StrCat( messageHex, &( stringHex[ 2 ] ) );
		}
		
		for ( byteIterator = 0;
			  byteIterator < trailerLength;
			  byteIterator++ )
		{
			StrPrintF( stringHex, "%02X ", (UInt8)trailerBuffer[ byteIterator ] );
			StrCat( messageHex, &( stringHex[ 2 ] ) );
		}	
	
		SetFieldTextFromStr( fieldInputHexP, messageHex, true );
	}
	
	// Check to make sure that we have the new serial manager
	err = FtrGet( sysFileCSerialMgr, sysFtrNewSerialPresent, &value );
	if (err != errNone) {
		SetFieldTextFromStr( fieldDebugP, "FTR Get Failed", true );
		return ( err );
	}
	if (value == 0) {
		SetFieldTextFromStr( fieldDebugP, "Old Serial Manager", true );
		return ( IR_FRAMER_ERROR_CODE );
	}

	SetFieldTextFromStr( fieldDebugP, "New Serial Manager", true );

	// Open a connection to the IR Port to send Raw Data
	err = SrmOpen( IR_FRAMER_PORT,
				   baudRate,
				   &portId );
	if (err != errNone) {
		SetFieldTextFromStr( fieldDebugP, "IR Port Open Failed", true );
		return ( err );
	}

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
	
	// Send the header part of the IrDA packet	
	numSent = SrmSend( portId, headerBuffer, headerLength, &err );
	if (err != errNone)
	{

		switch (err)
		{
			case serErrBadPort:
				StrCopy( message, "Header: Err Bad Port" );
				break;
			case serErrNotOpen:
				StrCopy( message, "Header: Err Port Not Open" );
				break;
			case serErrTimeOut:
				StrCopy( message, "Header: Err Time Out" );
				break;
			case serErrConfigurationFailed:
				StrCopy( message, "Header: Err Config Failed" );
				break;
			case serErrNotSupported:
				StrCopy( message, "Header: Err Port Not foreground" );
				break;
			case serErrNoDevicesAvail:
				StrCopy( message, "Header: Err No Device Avail" );
				break;
			default:
				StrCopy( message, "Header: Unknown Error" );
				break;
		}
		SetFieldTextFromStr( fieldDebugP, "Header: Send IR Data Failed", true );
		SetFieldTextFromStr( fieldDebugP, message, true );
		SrmClose( portId );
		return ( err );
	}
	
	// Send the payload part of the IrDA packet
	numSent += SrmSend( portId, payloadBuffer, payloadLength, &err );
	if (err != errNone)
	{

		switch (err)
		{
			case serErrBadPort:
				StrCopy( message, "Payload: Err Bad Port" );
				break;
			case serErrNotOpen:
				StrCopy( message, "Payload: Err Port Not Open" );
				break;
			case serErrTimeOut:
				StrCopy( message, "Payload: Err Time Out" );
				break;
			case serErrConfigurationFailed:
				StrCopy( message, "Payload: Err Config Failed" );
				break;
			case serErrNotSupported:
				StrCopy( message, "Payload: Err Port Not foreground" );
				break;
			case serErrNoDevicesAvail:
				StrCopy( message, "Payload: Err No Device Avail" );
				break;
			default:
				StrCopy( message, "Payload: Unknown Error" );
				break;
		}
		SetFieldTextFromStr( fieldDebugP, "Payload: Send IR Data Failed", true );
		SetFieldTextFromStr( fieldDebugP, message, true );
		SrmClose( portId );
		return ( err );
	}

	// Send the CRC part of the IrDA Packet
	numSent += SrmSend( portId, crcBuffer, crcLength, &err );
	if (err != errNone)
	{

		switch (err)
		{
			case serErrBadPort:
				StrCopy( message, "CRC: Err Bad Port" );
				break;
			case serErrNotOpen:
				StrCopy( message, "CRC: Err Port Not Open" );
				break;
			case serErrTimeOut:
				StrCopy( message, "CRC: Err Time Out" );
				break;
			case serErrConfigurationFailed:
				StrCopy( message, "CRC: Err Config Failed" );
				break;
			case serErrNotSupported:
				StrCopy( message, "CRC: Err Port Not foreground" );
				break;
			case serErrNoDevicesAvail:
				StrCopy( message, "CRC: Err No Device Avail" );
				break;
			default:
				StrCopy( message, "CRC: Unknown Error" );
				break;
		}
		SetFieldTextFromStr( fieldDebugP, "CRC: Send IR Data Failed", true );
		SetFieldTextFromStr( fieldDebugP, message, true );
		SrmClose( portId );
		return ( err );
	}
	
	// Send the Trailer part of the IrDA Packet
	numSent += SrmSend( portId, trailerBuffer, trailerLength, &err );
	if (err != errNone)
	{

		switch (err)
		{
			case serErrBadPort:
				StrCopy( message, "Trailer: Err Bad Port" );
				break;
			case serErrNotOpen:
				StrCopy( message, "Trailer: Err Port Not Open" );
				break;
			case serErrTimeOut:
				StrCopy( message, "Trailer: Err Time Out" );
				break;
			case serErrConfigurationFailed:
				StrCopy( message, "Trailer: Err Config Failed" );
				break;
			case serErrNotSupported:
				StrCopy( message, "Trailer: Err Port Not foreground" );
				break;
			case serErrNoDevicesAvail:
				StrCopy( message, "Trailer: Err No Device Avail" );
				break;
			default:
				StrCopy( message, "Trailer: Unknown Error" );
				break;
		}
		SetFieldTextFromStr( fieldDebugP, "Trailer: Send IR Data Failed", true );
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

	StrCopy( tempMessage, "IR Data Send OK, Bytes " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)numBytesToSend ) );
	StrCat( tempMessage, ", Sent " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)numSent ) );
	StrCat( tempMessage, ", Baud " );
	StrCat( tempMessage, StrIToA( conversionString, (Int32)baudRate ) );
	SetFieldTextFromStr( fieldDebugP, tempMessage, true );

	return ( errNone );
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
}
*/
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
	
		case HelpAboutIRFramerZire71:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
						
			break;
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
			//MainFormInit( frmP );
			FrmDrawForm( frmP );
			FrmSetNavEntry(frmP, MainSendButton, MainInputHexField, MainInputHexField, MainBaudRatePopTrigger, kFrmNavObjectFlagsForceInteractionMode);
			FrmSetNavEntry(frmP, MainInputHexField, MainInputField, MainInputField, MainSendButton, kFrmNavObjectFlagsForceInteractionMode);
			FrmSetNavEntry(frmP, MainInputField, 0, 0, MainInputHexField, 0);
			FrmNavObjectTakeFocus(frmP, MainInputField); // Set navigation focus
			
			handled = true;
			break;
			
		case frmCloseEvent:
			//MainFormDeinit( frmP );
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
