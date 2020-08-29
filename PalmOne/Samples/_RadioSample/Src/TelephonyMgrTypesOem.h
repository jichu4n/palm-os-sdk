/**************************************************************************************************
 *
 * Copyright (c) 2004-2006 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**
 @ingroup RadioSample
 */


/**
 * @brief 	This is the header file declaring the data types used by the 
 *		Telephony Manager for Palm OS Wireless Telephony Add-on.
 *
 *
 *
 * @version Version 1.0 	11/21/00	Thierry Langlais	Initial version.
 * @version Version 2.0		07/18/01	Hatem Oueslati		GPRS API Update.
 * @version	Version 3.0		09/12/01	Christophe Guiraud	Added USSD API
 * @version Version 4.0		10/09/01	Hatem Oueslati		Voice Recognition API update
 *					
 *
 * @file TelephonyMgrTypesOem.h
 *
 *
 **/


#ifndef __TELEPHONYMGRTYPESOEM_H__
#define __TELEPHONYMGRTYPESOEM_H__

#include "TelephonyMgrTypes.h"
/* managers id */
#define kTelOemSupServiceId					50

/* miscellaneous constants */
#define kTelOemSatMaxBearerCapabilitySize	10	/* in bytes */


/** 
 * @name Typedefs
 *
 */
/*@{*/


typedef enum
{
	cliValid,
	cliWithheld,
	cliUnknown
} TelOemCliValidType;

#define	kSimUpdateBinary	214
#define kSimUpdateRecord	220

/*	----------------------------------------------------------------------	*/
/* 	new types
	----------------------------------------------------------------------	*/
/*	information -----------------------------------------------------------	*/
/* Calls duration */
typedef struct _TelOemInfGetCallsDurationType
{
	UInt32	lastReceived;		/* [out] last received call duration in second */
	UInt32	lastDialled;		/* [out] last dialled call duration in second */
	UInt32	received;			/* [out] received calls duration in second */
	UInt32	dialled;			/* [out] dialled calls duration in second */
	UInt32	lastResetTimestamp;	/* [out] last reset date time in second */
	UInt32	lifetime;			/* [out] lifetime in second */

} TelOemInfGetCallsDurationType;

/*	----------------------------------------------------------------------	*/
/*	configuration
	----------------------------------------------------------------------	*/
/* Phone real-time clock */
typedef struct _TelOemCfgPhoneRTCType
{
	UInt32	datetime;		/* [in-out] date and time in second */
	Int16	timeZone;		/* [in-out] "Time zone", difference expressed in quarter of an hour,
							   between "current time" and GMT one. */
	Int16	reserved;		/* reserved */
} TelOemCfgPhoneRTCType;

/* loudspeaker volume level range */
typedef struct _TelOemCfgGetLoudspeakerVolumeRangeType
{
	UInt8	levelMin;		/* [out] minimum level */
	UInt8	levelMax;		/* [out] maximum level */
	Int16	reserved;		/* reserved */
	
} TelOemCfgGetLoudspeakerVolumeRangeType;

/* call waiting */
typedef struct _TelOemCfgCallWaitingType
{							
	UInt8	state;			/* [in out] in for enable or disable this service. out for getting status */
	UInt8	callType;		/* [in] call type , see kTelOemCfgCallType<Call Type>
							   constants in TelephonyMgrOem.h */
	Int16	reserved;		/* reserved */
} TelOemCfgCallWaitingType;

/* call barring */
typedef struct _TelOemCfgCallBarringType
{							/* password for barring authorization */
	Char*	password;		/* [out] password not necessary for status */
	UInt8	barrType;		/* [in out] call barring type, see kTelOemCfgBarr<Barring Type>
							   constants in TelephonyMgrOem.h */
	UInt8	callType;		/* [in] call type , see kTelOemCfgCallType<Call Type>
							   constants in TelephonyMgrOem.h */
	Boolean	activate;		/* [in out] true means activate and false means deactivate */
	UInt8	reserved;		
							
} TelOemCfgCallBarringType;

/* call diverting */
typedef struct _TelOemCfgCallDivertingType
{
	Char*	number;			/* [in out] diverting number */
	UInt8	numberSize;		/* [in out] size of number (in), length of number + 1 (out) */
	UInt8	reason;			/* [in out] call diverting reason, see kTelOemCfgDiv<reason> */
							/* constants in TelephonyMgrOem.h */
	UInt8	mode;			/* [in out] call diverting mode , see kTelOemCfgDiv<mode> */
							/* constants in TelephonyMgrOem.h */
	UInt8	callType;		/* [in] call type , see kTelOemCfgCallType<Call Type> */
							/* constants in TelephonyMgrOem.h */
	UInt8	time;			/* [in out] 1...30 when "no reply" is enabled or queried, 
								this gives the time in seconds to wait before 
								call is forwarded, default value 20 */
	Boolean	status;			/* [out]true means activated and false means deactivated */
	UInt8	reserved1;		
	UInt8	reserved2;		
							
} TelOemCfgCallDivertingType;

typedef struct _TelOemCfgCallsDivertingType
{
	TelOemCfgCallDivertingType	*callsDivertP;	/* [in out] class(s) of call diverting   */
	UInt8						classCount;		/* [in]	number of class in callsDivertingP */
	UInt8						allClassReason;		
	UInt16						reserved2;		
							
} TelOemCfgCallsDivertingType;


/* call line identification presentation status */
typedef struct _TelOemCfgCallLineIDPresentationType
{
	UInt8	resultCodePresentation;		/* [out] indicates result code presentation status */
	UInt8	subscriberNetworkService;	/* [out] indicates the subscriber CLIP service status in the network */
										/* see constants in TelephonyMgrOem.h */
	UInt8	reserved1;		
	UInt8	reserved2;		
	
} TelOemCfgCallLineIDPresentationType;


/* call line identification restriction */
typedef struct _TelOemCfgCallLineIDRestrictionType
{
	UInt8	indicator;		/* [in out] parameter sets the adjustment for outgoing calls */
	UInt8	netStatus;		/* [out] parameter shows the subscriber CLIR service status in the network */
							/* netStatus is read only */
							/* see constants in TelephonyMgrOem.h */
	UInt8	reserved1;		
	UInt8	reserved2;		
	
} TelOemCfgCallLineIDRestrictionType;

/* Alert preferences */
typedef struct _TelOemCfgAlertPreferencesType
{
	UInt16	incomingEventType;	/* [in] Call, SMS, ... see defined constants (in)*/
	void	*paramP;			/* [in out] depend on alert type (in-out)*/
	UInt32	extraData;			/* [in out] depend on alert type (in-out)*/
	UInt8	alertType;			/* [in] ring, vibrator, LED, ... (in) */
	UInt8	reserved1;		
	UInt8	reserved2;		
	UInt8	reserved3;		
	
} TelOemCfgAlertPreferencesType;

/* Voice mail number */
typedef struct _TelOemCfgVoiceMailType
{
	Char	*number;		/* [in out] number */ 
	UInt8	numberSize;		/* [in] available number size */
							/* [out] number length + 1 */
	UInt8	mode;			/* [in out] mode ( enable or disable ) */
	UInt8	reserved2;
	UInt8	reserved3;

} TelOemCfgVoiceMailType;

/* Bearer service  */
typedef struct _TelOemCfgBearerServiceType
{
	UInt8	speed;			/* [in-out] bearer service data rate */
	UInt8	service;		/* [in-out] bearer service type */
	UInt8	connection;		/* [in-out] bearer service connection type */
	UInt8	reserved;
	
} TelOemCfgBearerServiceType;

/*	----------------------------------------------------------------------	*/
/*	network
	----------------------------------------------------------------------	*/
typedef struct _TelOemNwkPreferredNetworkType
{
	UInt32	id; 			/* [in] network ID */
	UInt16	index;			/* [in] list index of the network ID */
	UInt8	reserved1;
	UInt8	reserved2;
	
} TelOemNwkPreferredNetworkType;

typedef struct _TelOemNwkGetPreferredNetworksType
{
	TelOemNwkPreferredNetworkType	*networkP;	/* pointer on a preferred network type array */
	UInt16							count;		/* [in out] in - number of available structures for network  */
												/* out - number of networks */
	UInt8							reserved1;
	UInt8							reserved2;

} TelOemNwkGetPreferredNetworksType;

typedef struct _TelOemNwkNetworkType
{
	UInt32	id; 			/* [in out] network ID */
	Char	*name;			/* [in out] name */
	UInt16	nameSize;		/* [in out] in - available name size */
							/* out - name length + 1 */
	UInt8	reserved1;
	UInt8	reserved2;

} TelOemNwkNetworkType;

typedef struct _TelOemNwkGetNetworksType
{
	TelOemNwkNetworkType	*networkP;	/* [in out] pointer on a network type array */
	UInt16					count;		/* [in out] in - number of available structures for network  */
										/* out - number of networks */

} TelOemNwkGetNetworksType;

/*	----------------------------------------------------------------------	*/
/*	security
	----------------------------------------------------------------------	*/
typedef struct _TelOemStyLockType
{
	Char	*password;		/* [in] password */
	UInt8	lockType;		/* [in] lock type, see kTelOemStyLock<lockType> */
							/* constants in TelephonyMgrOem.h */
	Boolean	activate;		/* [in out] true means activate and false means deactivate */
	UInt8	reserved1;
	UInt8	reserved2;
							
} TelOemStyLockType;

typedef struct _TelOemStyCodeUnblockType
{
	Char	*unblockCode;		/* [in] can be a PUK,PUK2 code */
	Char	*newPinCode;		/* [in] new pin,pin2 */
	
} TelOemStyCodeUnblockType;

typedef struct _TelOemStyRemainingAttemptNumberType
{
	UInt8 	pin1; 			/* [out] pin1 remaining attempt number */
	UInt8 	pin2; 			/* [out] pin2 remaining attempt number */
	UInt8 	puk1; 			/* [out] puk1 remaining attempt number */
	UInt8 	puk2; 			/* [out] puk2 remaining attempt number */
} TelOemStyRemainingAttemptNumberType;

typedef struct _TelOemStySimLockType
{
	Char	*controlKey; 	/* [in] SimLock facility Control Key (applicable when activate and deactivate only) */
	UInt8 	facility; 		/* [in]  facility for active, deactive or query a SimLock */
	UInt8	status;			/* [out] SimLock facility status (applicable when query only) : 0 -> not active, 1 -> active */
	UInt8 	attemptCount;	/* [out] SimLock attempt counter (applicable when get attempt counter only) */
	UInt8	attemptMax;		/* [out] SimLock facility status (applicable when get attempt counter only) */

} TelOemStySimLockType;

/*	----------------------------------------------------------------------	*/
/*	speech
	----------------------------------------------------------------------	*/
/* get current calls informations */
typedef struct _TelOemSpcCallInformationType
{
	Char				*number;		/* [in out] call number */
	Char				*name;			/* [in out] call name */
	UInt8				numberSize;		/* [in out] in - size of number, out - length of number + 1 */
	UInt8				nameSize;		/* [in out] in -size of name, out -length of name + 1 */
	UInt8				lineId;			/* [in out] call identification number */
	UInt8				dir;			/* [out] mobile originated (MO) call or mobile terminated call */
										/* kTelCallTypeOutgoing or kTelCallTypeIncoming */
	UInt8				state;			/* [out] state of the call see kTelCallInfoState<state>*/
	UInt8				callType;		/* [out] voice, data,... see kTelCallInfoType<type>*/
										/* kTelCallServiceVoice or kTelCallServiceData */
	Boolean				mpty;			/* [out] false : call is not one of multiparty (conference) call parties*/
										/* true  : call is one of multiparty (conference) call parties */
	TelOemCliValidType	cliValidity;	/* 0=Valid, 1=Withheld, 2=Unknown */
	UInt8				reserved;
} TelOemSpcCallInformationType;

typedef struct _TelOemSpcGetCallsInformationType
{
	TelOemSpcCallInformationType	*callsP;	/* [in out] pointer on a current calls array */
	UInt8							callCount;	/* [in out] in - number of available structures for call information */
												/* out - number of current calls*/
	UInt8							reserved;
	UInt8							reserved1;
	UInt8							reserved2;
	
} TelOemSpcGetCallsInformationType;

/* dial command supporting USSD (*#<SC>*<SI>#) */
typedef struct _TelOemSpcDialType
{
	Char	*number;		/* [in] call number or MMI string */
	Char	*alphaNum;		/* [in out] result if exists */
	UInt16	alphaNumSize;	/* [in out] in - available alphaNum size
							   out - length of result + 1 */
							/* alphaNumSize is different of 0 if alphaNum 
							   correspond to a supported MMI string */
	UInt8	lineId;			/* [out] call identification number */
	UInt8	reserved;
							
} TelOemSpcDialType;

typedef struct _TelOemAudioPathType
{
	UInt8	uplink;
	UInt8	sidetone;
	UInt8	downlink;
	UInt8	volume;
	UInt8	echoCancelation;
	UInt8	save;
} TelOemAudioPathType;

/*	----------------------------------------------------------------------	*/
/* Voice recognition
	----------------------------------------------------------------------	*/
typedef struct _TelOemAvrSampleType
{
	UInt8	sampleId;		/* range 1..254   0 and 255 are reserved */
	UInt8	contextId;		/* up to 7 contexts can be defined */
	UInt8	reserved1;
	UInt8	reserved2;
							
} TelOemAvrSampleType;

typedef struct _TelOemAvrSampleIdType
{
	UInt8	*sampleP;		/* pointer on a sample ids array */
	UInt8	sampleCount;	/* in - number of available structures for id */
							/* out - number of samples*/
	UInt8	reserved1;
	UInt8	reserved2;
} TelOemAvrSampleIdType;

typedef struct _TelOemAvrSampleStatusType
{
	UInt8	sampleId;		/* range 1..254   0 and 255 are reserved */
	UInt8	contextId;		/* up to 7 contexts can be defined */
	Boolean	trained;		/* out - true if sample trained or false if not trained */
	UInt8	reserved;
	
} TelOemAvrSampleStatusType;

typedef struct _TelOemAvrRecordSampleType
{
	UInt8	sampleId;		/* range 1..254   0 and 255 are reserved */
	UInt8	contextId;		/* up to 7 contexts can be defined */
	UInt8	maxRetries;		/* range application defined (<=10) */
	UInt8	reserved;
	
} TelOemAvrRecordSampleType;

/*	----------------------------------------------------------------------	*/
/* SIM card
	----------------------------------------------------------------------	*/
typedef struct _TelOemSimReadElementaryFileType
{
	const UInt16	*filePath;		// IN: absolute path of the file to read in the SIM. Example: { 0x3F00, 0x7F20, 0x6F21 }
									// consists of file identifiers following the path order, beginning with the Master File and ending with the Elementary File to read
	MemPtr			bytes;			// IN: buffer to be filled in with requested file body bytes. Can be 0 if bufferSize==0
									// OUT: requested file body bytes
	UInt16			bufferSize;		// IN: size of the bytes buffer, in bytes
	UInt16			partOffset;		// IN: offset of requested file body part
	UInt16			partSize;		// IN: size of requested file body part
	UInt16			byteCount;		// OUT: actual applicable byte amount, or size of the body of the Elementary File, depending on the mode
	Boolean			forceAccess;	// IN: true means ignore the cache, actually retrieve all the data from the SIM card
	UInt8			filePathLength;	// IN: number of file identifiers (UInt16) in filePath
	UInt8			recordNumber;	// IN: number of the record to be read. Range 1..254
	UInt8			recordSize;		// OUT: size of a record, in bytes. 0 if the file is not a Linear Fixed or a Cyclic Elementary File
	UInt8			mode;			// IN: mode indicating which data is requested, see kTelOemSimReadFileMode<mode>
	UInt8			structure;		// OUT: Elementary File structure, see kTelOemSimFileStructure<structure>

} TelOemSimReadElementaryFileType;

typedef struct _TelOemSimGetIconPropertiesType
{
	Coord	width;			// OUT: icon width, in pixels. Range 1..255
	Coord	height;			// OUT: icon height, in pixels. Range 1..255
	UInt16	density;		// OUT: recommended icon density. See DensityType
	UInt8	iconId;			// IN: icon identifier
	UInt8	depth;			// OUT: icon depth, in bits per pixel. Range 1..8

} TelOemSimGetIconPropertiesType;

typedef struct _TelOemSimGetIconType
{
	MemPtr	buffer;			// IN: allocated buffer
							// OUT: the same buffer, containing a Bitmap
	UInt32	bufferSize;		// IN: buffer size (bytes)
							// OUT: space needed in buffer (bytes). Used space if less or equal than input value
	UInt8	iconId;			// IN: icon identifier

} TelOemSimGetIconType;

typedef struct _TelOemSimConvertTextType
{
	const UInt8	*inputText;			// IN: text to be converted; doesn't need to be null-terminated
	UInt8		*resultText;		// OUT: buffer to be filled in with converted text. Can be NULL if resultSize==0
	UInt32		*firstBadChar;		// IN/OUT: NULL to replace inconvertible characters with spaces
									// or pointer to an integer to be set to the offset of the first inconvertible char 
	UInt32		inputSize;			// IN: size of input text, in bytes, not including the chrNull if any
	UInt32		resultSize;			// IN: size of the resultText buffer, in bytes
									// OUT: size of the resulting text, in bytes, including the chrNull if any
	UInt8		inputFormat;		// IN: format of input text, one kTelOemSimTextFormat<format> constant
	UInt8		dataCodingScheme;	// IN: only for kTelOemSimTextFormatDCSBased format: the Data Coding Scheme
	UInt8		resultFormat;		// IN: requested format
	UInt8		reserved;

} TelOemSimConvertTextType;

#define kTelOemSimUpdateMaxBufferSize 128
typedef struct _TelOemSimUpdateType
{
	// Taken from section 3.4.13 of Chi Mei AT command specification.
	// Parameters of this structure are not in the same order as the command (fileId and command are reversed for alignment)
	UInt16	fileId;
	UInt8	command; // Intended to be either kSimUpdateBinary or kSimUpdateRecord
	UInt8	p1;
	UInt8	p2;
	UInt8	p3;
	UInt8	*binaryBuf;	
} TelOemSimUpdateType;

typedef struct _TelOemSimGetSpnType
{
	Char	*spnName;		// Your allocated memory at least kTelOemSimGetSPNMaxStringSize long to store the null-terminated result.
	UInt16	spnNameSize;	// The size of your allocated memory.
	UInt8	displayByte;	// The SPN display byte. 
} TelOemSimGetSpnType;

#define kTelOemSimGetSpnMaxStringSize	20 // More than enough
/*	----------------------------------------------------------------------	*/
/* SIM Application Toolkit
	----------------------------------------------------------------------	*/
typedef struct _TelOemSatRefreshParametersType
{
	const UInt16	*filePaths;		// concatenated absolute paths of the SIM Elementary Files that have been modified. NULL if no specific file mentioned. See TelOemSimReadElementaryFile()
	UInt8			fileIdCount;	// number of file identifiers to be given in filePaths. This isn't the number of Elementary Files. Range 0..120. 0 if no specific file mentioned
	UInt8			operationCode;	// operation type. a kTelOemSatRefresh<type> code
	
} TelOemSatRefreshParametersType;

typedef struct _TelOemSatSetUpEventListParametersType
{
	const UInt8	*events;	// IN: codes of the SIM events to be monitored. NULL if eventCount == 0
	UInt8		eventCount;	// IN: number of events to be given in events, or 0 to stop monitoring SIM events

} TelOemSatSetUpEventListParametersType;

typedef struct _TelOemSatSetUpCallParametersType
{
	UInt8		capability[kTelOemSatMaxBearerCapabilitySize];	// bearer capability configuration parameters. first byte gives number of following bytes and may be 0 (see GSM 04.08 5.3.0 section 10.5.4.5)
	const Char	*dialingNumberString;							// dialing number string
	const Char	*userConfirmationText;							// text to display during the user confirmation phase. NULL if not provided
	const Char	*callEstablishmentText;							// text to display during the call establishment phase. NULL if not provided
	Boolean		automaticRedial;								// true if the SIM requests automatic redial when call set-up attempt is unsuccessful
	Boolean		userConfirmationExplicitIcon;					// false means the icon, if present, can't be displayed without the text
	Boolean		callEstablishmentExplicitIcon;					// false means the icon, if present, can't be displayed without the text
	UInt8		userConfirmationIconId;							// icon identifier. 0 if no icon
	UInt8		callEstablishmentIconId;						// icon identifier. 0 if no icon

} TelOemSatSetUpCallParametersType;

typedef struct _TelOemSatGenericMessageParametersType
{
	const Char	*text;			// text to display. NULL if not provided
	Boolean		concealCommand;	// true means the ME shouldn't inform the user about this action
	Boolean		explicitIcon;	// false means the icon, if present, can't be displayed without the text
	UInt8		iconId;			// icon identifier. 0 if no icon

} TelOemSatGenericMessageParametersType;

typedef struct _TelOemSatLaunchBrowserParametersType
{
	const Char		*text;					// text to be displayed when asking the user's consent. NULL if not provided
	const Char		*url;					// Uniform Resource Locator. Not empty but NULL if there is to use the default URL
	const Char		*gatewayAddress;		// Gateway name / Proxy identity to be used for connecting to the URL. NULL if there is to use the default values
	const UInt16	*provisioningFilePaths;	// concatenated absolute paths of provisioning files in the SIM. The list is prioritized, the first path takes precedence over the others. NULL if provisioningFileCount == 0. See TelOemSimReadElementaryFile()
	const UInt8		*preferredBearers;		// prioritized list of kTelOemSatBearer<bearer> bearer codes, beginning with the preferred one. NULL if preferredBearerCount == 0
	Boolean			explicitIcon;			// false means the icon, if present, can't be displayed without the text
	UInt8			iconId;					// icon identifier. 0 if no icon
	UInt8			provisioningFileIdCount;// number of file identifiers to be given in provisioningFilePaths. This isn't the number of Elementary Files. Can be 0
	UInt8			preferredBearerCount;	// number of bearer codes in preferredBearers. Can be 0
	UInt8			commandQualifier;		// Command Qualifier. See 3GPP TS 11.14
	UInt8			browserIdentity;		// Browser Identity. 0x00 if not provided. See 3GPP TS 11.14
	
} TelOemSatLaunchBrowserParametersType;

typedef struct _TelOemSatPlayToneParametersType
{
	const Char	*text;			// text to display. NULL if not provided
	UInt32		soundDuration;	// in milliseconds. 0 for default duration, or range 100..15300000
	Boolean		concealCommand;	// true means the ME shouldn't show a message to the user about this action
	Boolean		explicitIcon;	// false means the icon, if present, can't be displayed without the text
	UInt8		soundCode;		// one of the kTelOemSatSound<sound> codes
	UInt8		iconId;			// icon identifier. 0 if no icon

} TelOemSatPlayToneParametersType;

typedef struct _TelOemSatDisplayTextParametersType
{
	const Char	*text;						// text to display
	Boolean		highPriority;				// priority level: true means high, false means normal
	Boolean		clearAfterDelay;			// true means it's not mandatory to wait for a user's reaction
	Boolean		explicitIcon;				// false means the icon, if present, can't be displayed without the text
	Boolean		immediateResponseExpected;	// true means a response shall be sent to the SIM ASAP and the message shall then be sustained
	UInt8		iconId;						// icon identifier. 0 if no icon

} TelOemSatDisplayTextParametersType;

typedef struct _TelOemSatGetInkeyParametersType
{
	const Char	*text;						// text to display
	Boolean		helpInformationAvailable;	// indicates whether the SIM provides an help or not
	Boolean		explicitIcon;				// false means the icon, if present, can't be displayed without the text
	UInt8		iconId;						// icon identifier. 0 if no icon
	UInt8		responseType;				// expected response type. a kTelOemSatResponseType<type> code
	
} TelOemSatGetInkeyParametersType;

typedef struct _TelOemSatGetInputParametersType
{
	const Char	*text;						// text to display
	const Char	*defaultResponse;			// default response text to propose. NULL if not provided
	Boolean		hideUserInput;				// true means data entered by the user shall be masked
	Boolean		helpInformationAvailable;	// indicates whether the SIM provides an help or not
	Boolean		explicitIcon;				// false means the icon, if present, can't be displayed without the text
	UInt8		iconId;						// icon identifier. 0 if no icon
	UInt8		minimumResponseLength;		// minimum response length, in characters. 0 if no minimum specified
	UInt8		maximumResponseLength;		// maximum response length, in characters. 0 if no maximum specified
	UInt8		responseType;				// expected response format. a kTelOemSatResponseType<type> code
	
} TelOemSatGetInputParametersType;

typedef struct _TelOemSatMenuItemParametersType
{
	const Char	*name;					// item name. not null but empty if not provided
	Boolean		explicitIcon;			// false means the icon, if present, can't be displayed without the item name
	UInt8		identifier;				// item identifier. not 0
	UInt8		iconId;					// icon identifier. 0 if no icon
	UInt8		nextActionIndicator;	// identifier of the next command for this item. 0 if not provided

} TelOemSatMenuItemParametersType;

typedef struct _TelOemSatSelectItemParametersType
{
	TelOemSatMenuItemParametersType	*items;						// item details
	const Char						*text;						// text to be displayed before the item list. NULL if not provided
	Boolean							softKeySelectionPreferred;	// true means user should be able to select an item by tapping on its icon
	Boolean 						helpInformationAvailable;	// indicates whether the SIM provides an help for each item
	Boolean							explicitIcon;				// false means the icon, if present, can't be displayed without the text
	UInt8							itemCount;					// number of items. at least 1
	UInt8							iconId;						// icon identifier. 0 if no icon
	UInt8							defaultItemId;				// identifier of the item that should be pre-selected. 0 if no default item

} TelOemSatSelectItemParametersType;

typedef struct _TelOemSatAppParametersType
{
	const Char	*name;					// SAT application name. not null but empty if not provided
	Boolean		explicitIcon;			// false means the icon, if present, can't be displayed without the application name
	UInt8		identifier;				// application identifier. not 0
	UInt8		iconId;					// application icon. 0 if no icon
	UInt8		nextActionIndicator;	// identifier of the first command of the application. 0 if not provided

} TelOemSatAppParametersType;

typedef struct _TelOemSatSetUpMenuParametersType
{
	TelOemSatAppParametersType	*applications;				// SAT application items
	const Char					*title;						// application menu title text
	Boolean						explicitIcon;				// false means the icon, if present, can't be displayed without the title text
	Boolean						helpInformationAvailable;	// indicates whether the SIM provides an help for each application
	Boolean						softKeySelectionPreferred;	// true means user should be able to select an application by tapping on its icon
	UInt8						applicationCount;			// number of SAT applications proposed by the SIM. 0 means the menu shall be removed
	UInt8						iconId;						// application menu title icon identifier. 0 if no icon

} TelOemSatSetUpMenuParametersType;

typedef struct _TelOemSatOpenChannelParametersType
{
	const Char	*text;					// text to display. NULL if not provided
	Boolean		explicitIcon;			// false means the icon, if present, can't be displayed without the text
	Boolean		onDemandEstablishment;	// true means the link isn't established immediately
	UInt8		bearerCode;				// one of the applicable kTelOemSatBearer<bearer> codes, or 0 for default bearer
	UInt8		iconId;					// icon identifier. 0 if no icon

} TelOemSatOpenChannelParametersType;

typedef struct _TelOemSatDataTransferParametersType
{
	const Char	*text;			// text to display. NULL if not provided
	UInt32		requestSize;	// number of bytes that are requested (Receive Data) or transmitted (Send Data)
	Boolean		explicitIcon;	// false means the icon, if present, can't be displayed without the text
	UInt8		iconId;			// icon identifier. 0 if no icon

} TelOemSatDataTransferParametersType;

typedef struct _TelOemSatGetProactiveCmdParamType
{
	Boolean		noResponseExpected;	// OUT: true means this proactive command doesn't require an explicit response, or has already been answered
	UInt8		cmdId;				// IN: proactive command type
	UInt16		cmdSerialNb;		// IN: proactive command serial number
	union ProactiveCmdParams
	{
		TelOemSatRefreshParametersType			refresh;
		TelOemSatSetUpEventListParametersType	setUpEventList;
		TelOemSatSetUpCallParametersType		setUpCall;
		TelOemSatGenericMessageParametersType	genericMessage; // for Send SS, Send USSD, Send Short Message, Send DTMF, Set Up Idle Mode Text, Run AT Command, Close Channel
		TelOemSatLaunchBrowserParametersType	launchBrowser;
		TelOemSatPlayToneParametersType			playTone;
		TelOemSatDisplayTextParametersType		displayText;
		TelOemSatGetInkeyParametersType			getInkey;
		TelOemSatGetInputParametersType			getInput;
		TelOemSatSelectItemParametersType		selectItem;
		TelOemSatSetUpMenuParametersType		setUpMenu;
		TelOemSatOpenChannelParametersType		openChannel;
		TelOemSatDataTransferParametersType		dataTransfer; // for Receive Data, Send Data
	}			cmdParams;			// OUT: proactive command parameters
	MemPtr		buffer;				// IN: allocated buffer
									// OUT: the same buffer, containing the pointer-referred parameters
	UInt32		bufferSize;			// IN: buffer size (bytes)
									// OUT: space needed in buffer (bytes). Used space if less or equal than input value
	UInt32		internalField;		// internal variable. don't use it
							
} TelOemSatGetProactiveCmdParamType;

typedef struct _TelOemSatSendProactiveCmdResponseType
{
	const Char	*response;		// IN: associated text response; null-terminated string. Only applies to some result codes
	Boolean		yesResponse;	// IN: true means "Yes" response, false means "No" response. Only applies to one "Get Inkey" case
	Boolean		justValidate;	// IN: true to just check parameters and return, without actually sending the response
	UInt32		firstBadChar;	// OUT: byte index of the first character that can't be converted
	UInt16		cmdSerialNb;	// IN: proactive command serial number
	UInt8		cmdId;			// IN: proactive command type
	UInt8		resultCode;		// IN: one of the result codes applicable to this proactive command
	UInt8		additionalInfo;	// IN: additional information code. Only applies to some result codes
	UInt8		itemId;			// IN: associated item identifier. Only applies to some result codes
	UInt8		responseType;	// IN: expected response format, given by the SIM; a kTelOemSatResponseType<type> code. Only applies to some result codes
							
} TelOemSatSendProactiveCmdResponseType;

typedef struct _TelOemSatNotifyMenuSelectionType
{
	UInt8	eventCode;		// IN: one of the codes applicable to Menu Selection
	UInt8	applicationId;	// IN: associated application identifier. Does not apply to all events
							
} TelOemSatNotifyMenuSelectionType;

typedef struct _TelOemSatInitializeType
{
	const UInt8	*profile;		 // IN: Terminal Profile standard parameters
	UInt8		profileSize;	 // IN: profile data size, in bytes
	Char		languageCode[2]; // IN: ISO 639 language code

} TelOemSatInitializeType;

typedef struct _TelOemSatSendEventNotificationType
{
	Char	languageCode[2];			// IN: for Language Selection event, the ISO 639 language code
	UInt8	eventCode;					// IN: one of the Event Download event codes
	UInt8	browserTerminationCause;	// IN: for Browser Termination event, one of the cause codes

} TelOemSatSendEventNotificationType;

typedef struct _TelOemNITZNotificationType
{
   	UInt32      notificationData; 	/* associated data if any */
	UInt32      notificationData2; 	/* associated data if any */
	UInt32		timeStamp;			/* time stamp */
	UInt16      notificationId;		/* what was the associated telephony event */
	UInt8		priority;			/* notification priority 0 == max, 255 == min */

	// Above here: Snarf the standard Telephony notification
	
	Char		shortName[32];
	Char		longName[32];
	UInt32		networkTime;		
	Int16		networkTimezone;
} TelOemNITZNotificationType;

typedef struct _TelOemSatGetCtrlBySimResponseType
{
	MemPtr		buffer;						// IN: allocated buffer
											// OUT: the same buffer, containing the pointer-referred parameters
	const Char	*text;						// OUT: text to be displayed to the user. NULL if not provided
	const Char	*newAddress;				// OUT: new connection (for instance dialstring) information. NULL if not provided
	const Char	*newSCRPDestinationAddress; // OUT: new Service Center Relay Protocol Destination Address. NULL if not provided
	const Char	*newTPDestinationAddress; 	// OUT: new Transfer Protocol Destination Address. NULL if not provided
	UInt32		bufferSize;					// IN: buffer size (bytes)
											// OUT: space needed in buffer (bytes). Used space if less or equal than input value
	Boolean		newRequestUnsupported; 		// OUT: true means the Phone can't process the modified request
	Boolean		concealRequest;				// true means the ME shouldn't inform the user about this action
	UInt16		transactionId;				// IN: transaction identifier provided with kTelOemSatLaunchCmdCtrlBySimResponse notification
	UInt8		result;						// OUT: a kTelOemSatCtrlBySimResponse<response> code
	UInt8		operation;					// OUT: initial operation; a kTelOemSatCtrlBySimOperation<operation> code
	UInt8		newOperation;				// OUT: new operation in case of a modified request; a kTelOemSatCtrlBySimOperation<operation> code

} TelOemSatGetCtrlBySimResponseType;

/*	----------------------------------------------------------------------	*/
/* GPRS
	----------------------------------------------------------------------	*/
typedef struct _TelOemGprsContextType
{
	UInt8	contextID;			/* Context ID */
	UInt8	PdpType;			/* PDP Type: IP, PPP, OSPIH */
	
	Char	*accessPointName;	/* if accessPointNameSize == 0, then default APN requested from network */
	UInt8	accessPointNameSize;
	
	Char	*PdpAddress;		/* if PdpAddressSize == 0, then PDP Address requested from network */
	UInt8	PdpAddressSize;
	
	UInt8	dataCompression;	/* Data compression or none */
	UInt8	headerCompression;	/* Header compression or none */
	
	Char	*OSPIHHost; 		/* only required if pdpType OSPIH is chosen: Internet Host */
	UInt8	OSPIHHostSize; 		/* only required if pdpType OSPIH is chosen: Internet Host Size */
	UInt16	OSPIHPort; 			/* only required if pdpType OSPIH is chosen: TCP or UDP port on Internet Host */
	UInt8	OSPIHProtocol;		/* only required if pdpType OSPIH is chosen: Protocol used over IP, TCP or UDP */

} TelOemGprsContextType;

typedef struct _TelOemGprsDefinedCidsType
{
	UInt8	cidCount;			/* Number of element in the array, output number if cids count */
	UInt8	*cidsP;				/* output : array of cids */

} TelOemGprsDefinedCidsType;

typedef struct _TelOemGprsPdpActivationType
{
	UInt8	contextID;		/* context ID */
	UInt8	state;			/* state */

} TelOemGprsPdpActivationType;

typedef struct _TelOemGprsPdpAddressType
{
	UInt8	contextID;		/* context ID */
	Char	*PdpAddr;		/* PDP address */
	UInt8	PdpAddrSize;	/* size of PDP address */

} TelOemGprsPdpAddressType;

typedef struct _TelOemGprsNwkRegistrationType
{
	UInt8	registrationType;		/* Type of registration: Disable, NwkEnable, CellEnable */
	UInt8	registrationStatus;		/* Registration status */
	UInt8	cellSupportingStatus;	/* Indicates if cell is supporting GPRS or not (0 - GPRS not supported, 1 - GPRS supported, 255 - unknown) */
	UInt8	reserved;
	UInt16	locationAreaCode;		/* Location Information */
	UInt16	cellId;					/* Cell ID */

} TelOemGprsNwkRegistrationType;

typedef struct _TelOemGprsActivateDataType
{
	UInt8	contextID;			/* Context ID */
	UInt8	layer2Protocol;		/* Layer 2 protocol: NULL or PPP */

} TelOemGprsActivateDataType;

typedef struct _TelOemGprsQosType
{
	UInt8	contextID;			/* Context ID */
	UInt8	precedence;			/* Precedence */
	UInt8	delay;				/* Delay Class */
	UInt8	reliability;		/* Reliability Class */
	UInt8	peak;				/* Peak Troughput Class */
	UInt8	mean;				/* Mean Troughput Class */

} TelOemGprsQosType;

typedef struct _TelOemGprsEventReportingType
{
	UInt8	mode;		/* Event reporting mode */
	UInt8	buffer;		/* optional: buffer of unsolicited result codes */

} TelOemGprsEventReportingType;

typedef struct _TelOemGprsDataCounterType
{
	UInt8	contextID;	/* input: Context ID */
	UInt32	ulBytes;	/* output: Represents the uplink data amount in bytes that has been transmitted by MS to network */
	UInt32	dlBytes;	/* output: Represents the downlink data amount in bytes that has been received by MS from network */
	UInt32	ulPackets;	/* output: Represents the uplink data amount in packets (NPDUs) that has been transmitted by MS to network */
	UInt32	dlPackets;	/* output: Represents the downlink data amount in packets (NPDUs) that has been received by MS from network */

} TelOemGprsDataCounterType;

/*	----------------------------------------------------------------------	*/
/*  USSD
	----------------------------------------------------------------------	*/
typedef struct _TelOemUssdCommandParamType
{
	Char	*ussdStr;				/* USSD string */
	UInt16	ussdStrSize;			/* USSD string Length */
	UInt8	cmd;					/* Command ID */
	UInt8	dcs;					/* Data Coding Scheme */
	
} TelOemUssdCommandParamType;

#define kTelOemMaxUssdStrSize		100 // 7Bits:93 / 8Bits:82

typedef struct _TelOemUssdUnsolicitedResultCodeType
{
	Char	ussdStr[kTelOemMaxUssdStrSize];	/* USSD string */
	UInt16	ussdStrSize;					/* USSD string Length */
	UInt8	resultCode;						/* Result Code */
	UInt8	dcs;							/* Data Coding Scheme */

} TelOemUssdUnsolicitedResultCodeType;


/* command string */
typedef struct _TelOemSendCommandStringType
{
	Char	*commandString; 	/* command string to be sent */
	Char	*resultString;  	/* result string */
	UInt16  resultSize;			/* result string buffer size/max bytes retrieved on result */
	UInt32	timeOut;			/* milliseconds time out for command processing (before phone starts replying) */
} TelOemSendCommandStringType;


/*	----------------------------------------------------------------------	*/
/* 	Extended SMS
	----------------------------------------------------------------------	*/
typedef struct _TelOemSmsGsmAlphabetCompatibleType
{
	UInt8	*message;						/* Message to analyse GSM Alphabet compatibility with */
	UInt16	messageLen;						/* Message Len (without null terminated character) - just do a StrLen of the text entered */
	Boolean	compatible;						/* Output: whether message is compatible with GSM Alphabet or not */
	UInt8	reserved;

} TelOemSmsGsmAlphabetCompatibleType;

/*	----------------------------------------------------------------------	*/
/* 	SPN from CBM Cell Broadcast Message updates the SPN
	----------------------------------------------------------------------	*/
typedef struct _TelOemSmsGetCbmType
{
	UInt16	serialNum;
	UInt16	messageId;
	UInt8	pageParam;
	UInt8	*data;							/* Message to update the SPN with */
	UInt16	dataSize;						/* Message Len */
} TelOemSmsGetCbmType;

/*	----------------------------------------------------------------------	*/
/* 	Sound Task
	----------------------------------------------------------------------	*/
#include <SoundMgr.h>

typedef struct SoundTaskCmdType
{
	void				*smfH;				/* [in] pointer to SMF stream handle */
	SndSmfOptionsType	*smfOptP;			/* [in] pointer to options */										
} SoundTaskCmdType;
	
/*@}*/

#endif
