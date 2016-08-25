/******************************************************************************
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TelephonyMgrTypes.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		This is the header file declaring the data types used by the 
 *		Telephony Manager for Palm OS Wireless Telephony Add-on.
 *
 * History:
 *			Created by Gilles Fabre
 *		08/02/99		gfa			Initial version.
 *		05/02/00		gfa			Shared lib, 2nd API version.
 *
 *****************************************************************************/

#ifndef __TELEPHONYMGRTYPES_H__
#define __TELEPHONYMGRTYPES_H__

#include <PalmTypes.h>
#include <ErrorBase.h>
#include <Event.h>

#ifndef telErrorClass
#	define telErrorClass (appErrorClass + 0x100) /* DOLATER: remove this constant */
#endif

#define	kTelMaxPhoneNumberLen	30

/* managers id */
enum TelServices {kTelNwkServiceId,	
				  kTelStyServiceId,
				  kTelPowServiceId,
				  kTelCfgServiceId,
				  kTelSmsServiceId,
				  kTelEmcServiceId,
				  kTelSpcServiceId,
				  kTelDtcServiceId,
				  kTelPhbServiceId,
				  kTelOemServiceId,
				  kTelSndServiceId,
				  kTelInfServiceId};

/* messages */
enum TelMessages {kTelGetCallStateMessage,

				  kTelSendCommandStringMessage,

				  kTelNwkGetNetworkNameMessage,
				  kTelNwkGetNetworkTypeMessage,
				  kTelNwkGetSignalLevelMessage,
				  kTelNwkGetSearchModeMessage,
				  kTelNwkSetSearchModeMessage,
				  kTelNwkGetNetworksMessage,
				  kTelNwkSelectNetworkMessage,
				  kTelNwkGetSelectedNetworkMessage,
				  kTelNwkGetLocationMessage,

				  kTelStyGetAuthenticationStateMessage,
				  kTelStyEnterAuthenticationCodeMessage,
				  kTelStyChangeAuthenticationCodeMessage,

				  kTelPowGetPowerLevelMessage,
				  kTelPowGetBatteryStatusMessage,
				  kTelPowSetPhonePowerMessage,

				  kTelCfgSetSmsCenterMessage,
				  kTelCfgGetSmsCenterMessage,
				  kTelCfgGetPhoneNumberMessage,
				   
				  kTelSmsSendMessageMessage,
				  kTelSmsSendManualAcknowledgeMessage,
				  kTelSmsGetMessageCountMessage,
				  kTelSmsReadMessageMessage,
				  kTelSmsReadMessagesMessage,
				  kTelSmsReadSubmittedMessageMessage,
				  kTelSmsReadSubmittedMessagesMessage,
				  kTelSmsReadReportMessage,
				  kTelSmsReadReportsMessage,
				  kTelSmsDeleteMessageMessage,
				  kTelSmsGetAvailableStorageMessage,
				  kTelSmsSelectStorageMessage,
				  kTelSmsGetSelectedStorageMessage,
				  kTelSmsGetDataMaxSizeMessage,

				  kTelEmcGetNumberCountMessage,
				  kTelEmcGetNumberMessage,
				  kTelEmcSetNumberMessage,
				  kTelEmcSelectNumberMessage,
				  kTelEmcCallMessage,
				  kTelEmcCloseLineMessage,

				  kTelSpcAcceptCallMessage,
				  kTelSpcRejectCallMessage,
				  kTelSpcCallNumberMessage,
				  kTelSpcCloseLineMessage,
				  kTelSpcHoldLineMessage,
				  kTelSpcRetrieveHeldLineMessage,
				  kTelSpcGetCallerNumberMessage,
				  kTelSpcSendBurstDTMFMessage,
				  kTelSpcStartContinuousDTMFMessage,
				  kTelSpcStopContinuousDTMFMessage,
				  kTelSpcConferenceMessage,
				  kTelSpcSelectLineMessage,
				  kTelSpcPlayDTMFMessage,

				  kTelDtcCallNumberMessage,
				  kTelDtcCloseLineMessage,
				  kTelDtcSendDataMessage,
				  kTelDtcReceiveDataMessage,

				  kTelPhbGetEntryCountMessage,
				  kTelPhbGetEntryMessage,
				  kTelPhbGetEntriesMessage,
				  kTelPhbAddEntryMessage,
				  kTelPhbDeleteEntryMessage,
				  kTelPhbGetAvailablePhonebooksMessage,
				  kTelPhbSelectPhonebookMessage,
				  kTelPhbGetSelectedPhonebookMessage,
				  kTelPhbGetEntryMaxSizesMessage,

				  kTelSndPlayKeyToneMessage,
				  kTelSndStopKeyToneMessage,
				  kTelSndMuteMessage,
				   
				  kTelUrqSmsGetUniquePartIdMessage,

				  kTelUrqClosePhoneConnectionMessage,	/* urgent query: ask TT to close connection with phone */
				  kTelUrqOpenPhoneConnectionMessage,	/* urgent query: ask TT to open connection with phone */
				  kTelUrqIsPhoneConnectedMessage,		/* urgent query: ask TT the connection state with phone */
				  kTelUrqMatchPhoneDriverMessage,		/* urgent query: ask TT to check whether the driver matches the phone */
				  kTelUrqCancelMessage,					/* urgent query: ask TT to cancel an asynchronous call */
				  kTelUrqIsServiceAvailableMessage,  	/* urgent query: ask TT whether a service set is available */
				  kTelUrqIsFunctionSupportedMessage,  	/* urgent query: ask TT whether a function is supported */

				  kTelUrqGetTTStatusMessage,			/* urgent query: ask TT is status */
				  kTelUrqSleepMessage,					/* urgent query: warn TT the palm is going asleep */
				  kTelUrqWakeMessage,					/* urgent query: warn TT the palm is awaking */
				  kTelUrqDetachMessage,					/* urgent query: warn TT associated application is about to quit */
				  kTelUrqStopTTMessage,					/* urgent query: ask TT to stop */

				  kTelInfGetInformationMessage,
				   
				  kTelOemCallMessage,					/* oem calls */

				  kTelLastMessage = kTelOemCallMessage
};

/* basic types */
typedef UInt32	  TelAppID;

/* notification structure */
typedef struct _TelNotificationType
{
   	UInt32      notificationData; 	/* associated data if any */
	UInt32      notificationData2; 	/* associated data if any */
	UInt32		timeStamp;			/* time stamp */
	UInt16      notificationId;		/* what was the associated telephony event */
	UInt8		priority;			/* notification priority 0 == max, 255 == min */
} TelNotificationType;

/* event structure */
typedef struct _TelEventType
{
   	eventsEnum	 eType;
   	Boolean      penDown;
   	UInt8        tapCount;
   	Int16        screenX;
   	Int16        screenY;

	UInt16		 functionId;	/* ID of the message associated to the asynchronous function call */
	UInt16		 transId;		/* transId returned on asynchronous function call return */
	MemPtr		 *paramP;		/* parameter passed at asynchronous function call */
	Err			 returnCode;	/* function return code, errNone if ok, else an error */
} TelEventType;

/* command string */
typedef struct _TelSendCommandStringType
{
	Char*	commandString; 	/* command string to be sent */
	Char*	resultString;  	/* result string */
	UInt16  resultSize;		/* result string buffer size/max bytes retrieved on result */
	UInt32	timeOut;		/* milliseconds time out for command processing (before phone starts replying) */
} TelSendCommandStringType;

/* call state */
typedef struct _TelGetCallStateType
{
	UInt8	state;				/* call state, see kTelCall<State> constants in TelMgr.h */
	UInt8	callType;			/* incoming or outgoing */
	UInt8	callServiceType;	/* voice or data */
	
	/* outgoing or incoming call number */
	UInt8	numberSize;			/* size of number (in), length of number + 1 (out) */	
	Char*	number;				/* called or calling number */
} TelGetCallStateType;

/* network support */
typedef struct _TelNwkGetNetworkNameType
{
	UInt32	id; 	/* network ID */
	Char*	value;	/* name */
	UInt16	size; 	/* size of name (in), name len including '\0' (out) */
} TelNwkGetNetworkNameType;

typedef struct _TelNwkGetNetworksType
{
	UInt32*	networkIdP; /* network IDs array */
	UInt8	size; 		/* size of networkIdP (in), number of network IDs (out) */
} TelNwkGetNetworksType;

/* phone location within network web */
typedef struct _TelNwkGetLocationType
{
	Char*	value;	/* current location string */
	UInt16	size; 	/* size of value (in), location len including '\0' (out) */
} TelNwkGetLocationType;

/* change security code */
typedef struct _TelStyChangeAuthenticationType
{
	UInt8	codeId;		/* code to be changed */	
	Char* 	oldCode;	/* old code value */
	Char*	newCode;	/* new code value */
} TelStyChangeAuthenticationType;

/* SMS center */
typedef struct _TelCfgGetSmsCenterType
{
	UInt8	size;	/* size of value (in), SMS dial number len including '\0' (out) */
	Char*	value;	/* SMS center dial number */
} TelCfgGetSmsCenterType;

/* phone number */
typedef struct _TelCfgGetPhoneNumberType
{
	UInt8	size;	/* size of value (in), phone dial number len including '\0' (out) */
	Char*	value;	/* phone dial number */
} TelCfgGetPhoneNumberType;

/* SMS */
/* SMS time */
typedef struct _TelSmsDateTimeType 
{
	Boolean 	absolute;
	UInt32		dateTime;	/* relative time from now, or Palm absolute time */
} TelSmsDateTimeType;

/* SMS extensions */
typedef struct _TelSmsMultiPartExtensionType
{
	UInt16	bytesSent;

	UInt16	partCurrent,
		    partCount,
		    partId;
} TelSmsMultiPartExtensionType;

typedef struct _TelSmsNbsExtensionType
{
 	UInt16 	destPort, 	/* destination NBS port */
 			srcPort; 	/* source NBS port */
} TelSmsNbsExtensionType;

typedef struct _TelSmsUserExtensionType
{
 	UInt8* 	extHeader; /* user defined extended header */
 	UInt8 	extHeaderSize;
} TelSmsUserExtensionType;

typedef struct _TelSmsExtensionType
{
	UInt8	extensionTypeId;	/* what does this extension describe? */
	
	union 
	{
		TelSmsMultiPartExtensionType 	mp;  	/* multi-part SMS extension */
	   	TelSmsNbsExtensionType      	nbs; 	/* NBS SMS extension */
	   	TelSmsUserExtensionType    		user; 	/* User Defined SMS extension */
	} extension;
} TelSmsExtensionType;

/* Advanced parameters for GSM */
typedef struct _TelSmsSubmitAdvancedGSMType
{
	UInt16	protocolId;					/* Reserved - not supported (Fax, paging, . . .) GSM only */

	Boolean	rejectDuplicatedRequest;	/* GSM - Network must reject msg if the same exists */
	Boolean	replyPath;					/* GSM - use reply specified path */

	Char*	serviceCenterNumber;		/* SMS service center number */
	UInt8	serviceCenterNumberSize;	/* Used for decoding only */
} TelSmsSubmitAdvancedGSMType;

/* Advanced parameters for CDMA */
typedef struct _TelSmsSubmitAdvancedCDMAType
{
	Boolean		manualAckRequest;		
	UInt8		messageType;		/* Message Type */

	TelSmsDateTimeType	deferredDate;	/* GSM & CDMA only Absolute or relative */

	UInt8		priority;			/* CDMA & TDMA only */
	UInt8		privacy;			/* CDMA & TDMA only */

	Boolean		alertOnDeliveryRequest;	/* CDMA & TDMA(?) */

	Char*		callbackNumber;		/* CDMA & TDMA only - address to reply */
	UInt8		callbackNumberSize;
} TelSmsSubmitAdvancedCDMAType;

/* Advanced parameters for TDMA - currently identical to CDMA */
typedef TelSmsSubmitAdvancedCDMAType TelSmsSubmitAdvancedTDMAType;

/* Submit message structure */
typedef struct _TelSmsSubmitMessageType 
{
	UInt16	version;				/* SMS API version */

	Boolean	networkDeliveryRequest;	/* All - Ask a network delivery report / status report */

	Char*	destinationAddress;		/* length : GSM 12bytes, CDMA up to 2x64 bytes */
	UInt8	destinationAddressSize;	/* Used for decoding only */

	UInt16	dataSize;				/* Length of data being sent */
	UInt8*	data;					/* All */
	UInt8	dataCodingScheme;	

	TelSmsDateTimeType	validityPeriod;	/* All - Absolute or relative */

	/* Advanced parameters */
	UInt8	standardType;			/* Indicates the type of advanced parameters */
	union 
	{
		TelSmsSubmitAdvancedGSMType		advancedGSM;
		TelSmsSubmitAdvancedCDMAType	advancedCDMA;
		TelSmsSubmitAdvancedTDMAType	advancedTDMA;
	} advancedParams;
	
	UInt8	extensionsCount;			/* how many extensions in this message */
	TelSmsExtensionType* extensionsP;	/* SMS extensions array: NBS, Multi-part, etc. */
} TelSmsSubmitMessageType;

/* Submit message structure parameter */
typedef struct _TelSmsSendMessageType
{
	UInt32					messageId;	/* Output parameter, filled by the telephony implementation */
	TelSmsSubmitMessageType	message;
} TelSmsSendMessageType;

/* Submitted message structure parameter */
typedef struct _TelSmsSubmittedMessageType
{
	UInt16					index;	/* Message's index on the phone */
	TelSmsSubmitMessageType	message;
} TelSmsSubmittedMessageType;

/* Manual acknowledge structure */
typedef struct _TelSmsManualAckType
{
	UInt16 	version;			/* SMS API version */
	Char*	destinationAddress;	/* length : GSM 12bytes, CDMA up to 2x64 bytes */
	UInt32	messagesId;			/* Message Id of message to be acknowledged */

	UInt16	dataSize;			/* Length of data being sent */
	UInt8*	data;				/* All */
	UInt8	dataCodingScheme;

	UInt8	responseCode;		/* Value is network dependant */
} TelSmsManualAckType;

typedef struct _TelSmsDeliveryAdvancedGSMType
{
	UInt16	protocolId;		/* reserved - not supported - GSM only */

	Boolean	replyPath;		/* GSM - must use specified reply path */
	Char*	serviceCenterNumber;
    UInt8	serviceCenterNumberSize;
} TelSmsDeliveryAdvancedGSMType;

typedef struct _TelSmsDeliveryAdvancedCDMAType
{
	UInt8		messageType;			/* Delivery Message Type */

	TelSmsDateTimeType	validityPeriod;		/* CDMA & TDMA only */

	UInt8		priority;				/* CDMA & TDMA only */
	UInt8		privacy;				/* CDMA & TDMA only */
	
	Boolean 	alertOnDeliveryRequest;	/* CDMA & TDMA only */
	Boolean 	manualAckRequest;		/* CDMA */

	UInt8	 	voiceMessageNumber;		/* CDMA, TDMA, GSM */
	
	UInt8		callbackNumberSize;
	Char*		callbackNumberAddress; /* Store callback address */
	
	UInt8		languageIndicator;		/* reserved - not supported - CDMA only */
} TelSmsDeliveryAdvancedCDMAType;

typedef struct _TelSmsDeliveryAdvancedTDMAType 
{
	UInt8			messageType;		/* Delivery Message Type */

	TelSmsDateTimeType validityPeriod;		/* CDMA & TDMA only */
	
	UInt8			priority;			/* CDMA & TDMA only */
	UInt8			privacy;			/* CDMA & TDMA only */
	
	Boolean 		manualAckRequest;		/* CDMA */

	Boolean 		alertOnDeliveryRequest;	/* CDMA & TDMA only */
	UInt8	 		voiceMessageNumber;	/* CDMA, TDMA, GSM */
	
	UInt8			callbackNumberSize;
	Char*			callbackNumberAddress;	/* Store callback address */
} TelSmsDeliveryAdvancedTDMAType;

/*   Delivery message structure */
typedef struct _TelSmsDeliveryMessageType
{
	UInt16	version;		/* SMS API version */
	UInt16	index;			/* SMS index on the phone storage */

	UInt32	 messageIdentifier;

	TelSmsDateTimeType	timeStamp;

	UInt16	dataSize;			/* Length of data being sent */
	UInt8*	data;
	UInt8	dataCodingScheme;	/* enum All */

	UInt8	originatingAddressSize;
	Char*	originatingAddress;	/* Store originating address (delivery message) */
	
	Boolean otherToReceive;		/* GSM & CDMA & TDMA(?) */
	
	Boolean reportDeliveryIndicator;	/* All */
	
	/* Advanced parameters */
	UInt8	standardType;
	union 
	{
		TelSmsDeliveryAdvancedGSMType	advancedGSM;
		TelSmsDeliveryAdvancedCDMAType	advancedCDMA;
		TelSmsDeliveryAdvancedTDMAType	advancedTDMA;
	} advancedParams;

	UInt8	extensionsCount;			/* how many extensions in this message */
	TelSmsExtensionType* extensionsP;	/* SMS extensions array: NBS, Multi-part, etc. */
} TelSmsDeliveryMessageType;

/*  Report message structure */
typedef struct _TelSmsReportType
{
	UInt16	version;
	UInt16	index;				/* SMS index on the phone storage */
	UInt8	reportType;			/* Delivery report type */
	UInt32	messageId;

	UInt16	dataSize;			/* Length of data being sent */
	UInt8*	data;
	UInt8	dataCodingScheme; 

	Char*	originatingAddress;	/* Store originating address */
	UInt8	originatingAddressSize;

	UInt8	report;

	TelSmsDateTimeType timeStamp;	/* Time when SC received the corresponding sent message */
} TelSmsReportType;

/* multiple messages read */
typedef struct _TelSmsReadMessagesType
{
	UInt16					  first;		/* first message physical index, zero based */
	UInt16				  	  count;		/* number of messages to read */
	TelSmsDeliveryMessageType *messagesP;	/* messages array */
} TelSmsReadMessagesType;

/* multiple reports read */
typedef struct _TelSmsReadReportsType
{
	UInt16			  first;				/* first report physical index, zero based */
	UInt16		  	  count;				/* number of reports to read */
	TelSmsReportType *reportsP;				/* reports array */
} TelSmsReadReportsType;

/* multiple submitted messages read */
typedef struct _TelSmsReadSubmittedMessagesType
{
	UInt16			  			first;		/* first sent message physical index, zero based */
	UInt16		  	  			count;		/* number of sent messages to read */
	TelSmsSubmittedMessageType 	*submittedsP; /* sent messages array */
} TelSmsReadSubmittedMessagesType;

/* get messages number/slots in selected storage */
typedef struct _TelSmsGetMessageCountType
{
	UInt8	messageType;		/* report, submitted, etc. */
	UInt16	slots;				/* number of entries in the selected storage */
	UInt16	count;				/* number of messages present in the selected storage */
} TelSmsGetMessageCountType;

/* delete message selected storage */
typedef struct _TelSmsDeleteMessageType
{
	UInt8	messageType;		/* report, submitted, etc. */
	UInt16	index;				/* physical index in storage, zero based */
} TelSmsDeleteMessageType;

/* available storage list */
typedef struct _TelSmsGetAvailableStorageType
{
	UInt16	count;				/* storage IDs array size (in), retrieved IDs number (out) */
	UInt8	*storagesP;			/*  storage IDs array */
} TelSmsGetAvailableStorageType;

/* emergency call support */
typedef struct _TelEmcGetNumberType
{
	UInt8	index; 	/* EMC number index, zero based */
	UInt8	size; 	/* EMC dial number len including '\0' (out), value size (in) */
	Char*	value;	/* EMC dial number */
} TelEmcGetNumberType;

typedef struct _TelEmcSetNumberType
{
	UInt8	index; 	/* EMC number index, zero based */
	Char*	value;	/* EMC dial number */
} TelEmcSetNumberType;

/* speech call support */
typedef struct _TelSpcGetCallerNumberType
{
	UInt8	size; 	/* dial number len including '\0' (out), value size (in) */
	Char*	value;	/* dial number */
} TelSpcGetCallerNumberType;

typedef struct _TelSpcPlayDTMFType
{
	UInt8	keyTone;	/* keytone to be played */
	UInt32	duration;	/* play duration in 10 ms multiple */
} TelSpcPlayDTMFType;

/* phonebook support */
typedef struct _TelPhbEntryType
{
	UInt16 	phoneIndex;  	/* entry's index in the phonebook, zero based */
	Char*  	fullName;
	UInt8	fullNameSize; 	/* name len including '\0' (out), name size (in) */
	Char*  	dialNumber;		
	UInt8	dialNumberSize;	/* dial number len including '\0' (out), dialNumber size (in) */
} TelPhbEntryType;

typedef struct _TelPhbGetEntryCountType
{
	UInt16	slots;	/* number of entries in the selected phonebook */	
	UInt16	count;	/* number of name/addresse pairs present in the selected phonebook */
} TelPhbGetEntryCountType;

typedef struct _TelPhbGetEntriesType
{
	UInt16				first;		/* first entry physical index, zero based */
	UInt16				count;		/* number of name/addresse pairs */
	TelPhbEntryType*	entriesP;	/* name/addresse pairs array */
} TelPhbGetEntriesType;

typedef struct _TelPhbGetAvailablePhonebooksType
{
	UInt16	count;			/* size of array (in), number of IDs (out) */
	UInt8*	phonebooksP; 	/* phonebook IDs array */
} TelPhbGetAvailablePhonebooksType;

typedef struct _TelPhbGetEntryMaxSizesType
{
	UInt8	fullNameMaxSize; 	/* name len including '\0' */
	UInt8	dialNumberMaxSize;	/* dial number len including '\0' */
} TelPhbGetEntryMaxSizesType;

/* sound support */
typedef struct _TelSndPlayKeyToneType
{
	UInt8	keycode;		/* what key */
	UInt8	type;			/* what tone type */
} TelSndPlayKeyToneType;

/* information support */
typedef struct _TelInfGetInformationType
{
	UInt8	infoType; 	/* expected information, can be up to 2Kb (!) */
	UInt16	size;		/* value len including '\0' (out), value size (in) */
	Char*	value;		/*  returned information string */
} TelInfGetInformationType;

/* data call support */
typedef struct _TelDtcCallNumberType
{
	Char* 	dialNumberP;	/* number to dial */
	UInt8	lineId;			/* resulting line id, sent back in event */
} TelDtcCallNumberType;

typedef struct _TelDtcSendDataType
{
	UInt8* 	data;			/* data to be sent */
	UInt32	size;			/* data size (in), data sent (out)*/
} TelDtcSendDataType;

typedef struct _TelDtcReceiveDataType
{
	UInt8* 	data;			/* buffer to receive data */
	UInt32	size;			/* size of buffer (in), data size (out)*/
	UInt32	timeOut;		/* milliseconds to wait before exiting */
} TelDtcReceiveDataType;

/* OEM support */
typedef struct _TelOemCallType
{
	UInt32	OemId; 	/* unique ID of OEM function set */
	UInt8	funcId; /* function ID */
	void*	paramP; /* parameters block */
} TelOemCallType;

#endif
