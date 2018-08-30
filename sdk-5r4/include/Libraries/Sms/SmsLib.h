/******************************************************************************
 *
 * Copyright (c) 1997-2004 PalmSource, Inc. All rights reserved.
 *
 * File: SmsLib.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		Include file for PalmOS SMS Library
 *
 *****************************************************************************/

#ifndef __SMSLIB_H__
#define __SMSLIB_H__

#include <PalmTypes.h>
#include <LibTraps.h>
#include <ExgMgr.h>
#include <TelephonyMgr.h>
#include <TelephonyMgrTypes.h>

// The Sms library is used as an Exchange library. ExgLib.h defines all the
// primary entrypoints into the library. The rest of this include file defines the
// specials opCodes used in the ExgControl function and the structure used in the
// socketRef field of the Exchange Manager Socket structure.

#include <ExgLib.h>

/****************************************************************************
 *
 * ExgLibControl opcodes
 *
 ****************************************************************************/

/* Those Op codes take SmsPrefType as parameter */
#define exgLibSmsPrefGetOp				(exgLibCtlSpecificOp | 1)
#define exgLibSmsPrefGetDefaultOp		(exgLibCtlSpecificOp | 2)
#define exgLibSmsPrefSetOp				(exgLibCtlSpecificOp | 3)

/* This Op code takes a network type constant as parameter kSmsNetworkXxx */
#define exgLibSmsPrefDisplayOp			(exgLibCtlSpecificOp | 4)

/* This Op code takes an UInt16 as parameter */
#define exgLibSmsIncompleteGetCountOp	(exgLibCtlSpecificOp | 5)

/* This OpCode takes a SmsID (UInt16) as parameter */
#define exgLibSmsIncompleteDeleteOp		(exgLibCtlSpecificOp | 6)

/****************************************************************************
 *
 * Types and Constants
 *
 ****************************************************************************/

#define kSmsLibName 				"SMS Library"	// name of Sms library

// Feature Creators and numbers, for use with the FtrGet() call. This
// feature can be obtained to get the current version of the Sms Library
#define kSmsFtrNumVersion			((UInt16)0)

// 0xMMmfsbbb, where MM is major version, m is minor version
// f is bug fix, s is stage: 3-release, 2-beta, 1-alpha, 0-development,
// bbb is build number for non-releases 
// V1.12b3   would be: 0x01122003
// V2.00a2   would be: 0x02001002
// V1.01     would be: 0x01013000

#define kSmsMessageRegExtensionType	"sms"
#define kSmsReportRegExtensionType	"rps"
#define kSmsExtensionTypeLength		3

#define kSmsScheme					"_sms"

#define	kSmsMaxPhoneSize			(kTelMaxPhoneNumberLen + 1)	// Max length for Phone number

#define kSmsNBSConverter			((UInt8)0)	// NBS header will be add to the data
#define kSmsNoConverter				((UInt8)1)	// No header will be add to the data.

#define kSmsRowDataEncoding			((UInt8)0)	// 8 bit encoding scheme
#define kSmsTextEncoding			((UInt8)1)	// UCS2 encoding scheme
#define kSmsGsmTextEncoding			((UInt8)2)	// GSM 7 bit text encoding scheme

#define kSmsNetworkAuto				((UInt8)-1)
#define kSmsNetworkCDMA				((UInt8)kTelNwkCDMA)
#define kSmsNetworkGSM				((UInt8)kTelNwkGSM)
#define kSmsNetworkTDMA				((UInt8)kTelNwkTDMA)
#define kSmsNetworkPDC				((UInt8)kTelNwkPDC)

#define kSmsMessageType				((UInt8)0)
#define kSmsIncompleteType			((UInt8)1)
#define kSmsReportType				((UInt8)2)

/****************************************************************************
 *
 * Sms Error
 *
 ****************************************************************************/
#pragma mark Sms Error

#define kSmsErrMaxSizeExceeded		(smsErrorClass | 0x01)	/* Message exceeds maximum size supported by network */

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

#pragma mark Report Param
/****************************************************************************
 * Report parameters
 ****************************************************************************/

/* Report Parameters Type
   ---------------------- */
typedef struct SmsReportParamsTag
{
	UInt32	timeStamp;			// TimeStamp of the report (when delivered, or last attempt, ...)
	UInt16	index;				// SMS index on the phone storage
	UInt8	reportType;			// Delivery report type
	UInt8	report;				// Status report indicator
	Char*	originatingAddress;	// Phone number to which belong the report (was sent)
	
} SmsReportParamsType, *SmsReportParamsPtr;


#pragma mark Send Param
/****************************************************************************
 * Send parameters
 ****************************************************************************/

/* Advanced Parameters Type for TDMA & CDMA network
   ------------------------------------------------ */
typedef struct SmsSendCDMAParamsTag
{
	UInt8				messageType;			// Message Type

	TelSmsDateTimeType	deferredDate;	

	UInt8				priority;			
	UInt8				privacy;			

	UInt8				alertOnDelivery:1;
	UInt8				manualAckRequest:1;		
	UInt8				reserved:6;

	Char*				callbackNumber;			// Address to reply
	
} SmsSendCDMAParamsType, *SmsSendCDMAParamsPtr;

typedef SmsSendCDMAParamsType SmsSendTDMAParamsType, *SmsSendTDMAParamsPtr;


/* Advanced Parameters Type for GSM network
   ---------------------------------------- */

typedef struct SmsSendGSMParamsTag
{
	UInt16		protocolId;				// Reserved - not supported (Fax, paging, . . .)
	Char*		serviceCenterNumber;	// SMS Center number - Optionel

	Boolean		rejectDuplicated;		// Network must reject msg if the same exists
	Boolean		replyPath;				// Use reply specified path

} SmsSendGSMParamsType, *SmsSendGSMParamsPtr;


/* SMS Send Parameters Type
   ------------------------ */
   
typedef struct SmsSendParamsTag
{
	TelSmsDateTimeType	validityPeriod;				// SMS validity Period
	Char*				destinationAddress;			// recipient number -> to send the sms to

	UInt8				networkDeliveryRequested:1;	// Sms report wanted
	UInt8				ignoreDefaultValue:1;		// If false, the field validityPeriod, ackRequested, reportRequested, smsCenter are ignored.
	UInt8				reserved:6;

	UInt16				partCount;					// number of parts of the SMS	(output)
	UInt16				lastPart;					// if error, the last part sent (output)	

	UInt8				converter;					// How the data are going to be typed (0 = NBS, 1 = None)
	
	union
	{
		SmsSendGSMParamsType	gsm;
		SmsSendCDMAParamsType	cdma;
		SmsSendTDMAParamsType	tdma;
		
	} protocol;
		
} SmsSendParamsType, *SmsSendParamsPtr;


#pragma mark Receive Param
/****************************************************************************
 * Receive parameters
 ****************************************************************************/

/* Advanced Parameters Type for TDMA & CDMA network
   ----------------------------------------------- */
typedef struct SmsReceiveCDMAParamsTag
{
	UInt8				messageType;			// Delivery Message Type

	TelSmsDateTimeType	validityPeriod;			

	UInt8				priority;				
	UInt8				privacy;				
	
	Boolean 			alertOnDeliveryRequest;	
	Boolean 			manualAckRequest;		

	UInt8	 			voiceMessageNumber;		
	UInt8				languageIndicator;

	Char*				callbackNumberAddress;	// Store callback address */
	
} SmsReceiveCDMAParamsType, *SmsReceiveCDMAParamsPtr;

typedef SmsReceiveCDMAParamsType SmsReceiveTDMAParamsType, *SmsReceiveTDMAParamsPtr;

/* Advanced Parameters Type for GSM network
   ---------------------------------------- */
typedef struct SmsReceiveGSMParamsTag
{
	UInt16	protocolId;				// reserved - not supported
	Char*	serviceCenterNumber;

	Boolean	replyPath;				// Must use specified reply path
	
} SmsReceiveGSMParamsType, *SmsReceiveGSMParamsPtr;

/* SMS Receive Parameters Type
   --------------------------- */
   
typedef struct SmsReceiveParamsTag
{
	UInt32		timeStamp;			// Palm
	Char*		originatingAddress;	// originating number -> to send the sms to
	
	UInt8		leaveOnPhone:1;		// Received messages won't be deleted on the phone (Input)
	UInt8		forceSlotMode:1;	// Force parsing methode to Slot Mode (default is Block mode) (Input)
	UInt8		reserved:6;

	UInt16		index;				// Index of the storage in the mobile where the message is stored

	Boolean		otherToReceive;
	Boolean		reportDeliveryIndicator;

	union
	{
		SmsReceiveGSMParamsType		gsm;
		SmsReceiveCDMAParamsType	cdma;
		SmsReceiveTDMAParamsType	tdma;
		
	} protocol;
	
} SmsReceiveParamsType, *SmsReceiveParamsPtr;


#pragma mark Sms Param
/****************************************************************************
 * Sms parameters
 ****************************************************************************/
/* SMS Parameters Type
   ------------------- */

typedef struct SmsParamsTag
{
	UInt32		creator;			// MUST ALWAYS BE SET TO sysFileCSmsLib
	UInt16		smsID;				// ID of the SMS (output)

	Char*		extension;			// Extension type of the data - Optionel		(Output)
	Char*		mimeTypes;			// Mime type of object - Optionel				(Output)
	UInt32		appCreator;			// Application Creator of the target - Optionel	(Output)

	UInt8		dataCodingScheme;	// How SMS are going to convert the data (0 = 8 bit, 1 = 7 bit)
	UInt8		networkType;		// Indicates the type of advanced parameters (input - output)
	UInt8		dataType;			// Indicates the kind of message: Sms, incomplete Sms, Report (Output)
	
	UInt16		nbsDestPort;		// NBS port to use to encode the data (input) - port used in received Sms (output)
	UInt16		nbsSrcPort;			// NBS port to use to encode the data (input) - port used in received Sms (output)

	union
	{
		SmsSendParamsType		send;
		SmsReceiveParamsType	receive;
		SmsReportParamsType		report;

	} data;

} SmsParamsType, *SmsParamsPtr;


/* Preferences Type
   ------------------- */
   
typedef struct SmsPrefTag
{
	UInt32	validity;								// Validity period of SMS (relatif) in seconds
	UInt16	warnOver;								// Display an alert if sending more Sms than this value.
	Boolean leave;									// Leave SMS on Phone
	Boolean report;									// Ask for a network delivery report
	Boolean	autoSMSC;								// If set, don't use the value stored in smscNumber field
	Char	smscNumber[kSmsMaxPhoneSize];			// SMS Service Center. Could be null
	
} SmsPrefType, *SmsPrefPtr;


/****************************************************************************/

#endif  // SMS_LIB_H
