/******************************************************************************
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TelephonyMgr.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		This is the header file for the Telephony Manager
 *      for Palm OS Wireless Telephony Add-on.
 *		It defines the Telephony Manager public functions.
 *
 * History:
 *			Created by Gilles Fabre
 *		08/05/99		gfa			Initial version.
 *		05/02/00		gfa			Shared lib, 2nd API version.
 *
 *****************************************************************************/

#ifndef __TELEPHONYMGR_H__
#define __TELEPHONYMGR_H__

#include <PalmTypes.h>
#include <Rect.h>
#include <Font.h>
#include <Window.h>
#include <Event.h>
#include <LibTraps.h>

#include "TelephonyMgrTypes.h"

/* sysMakeROMVersion(major, minor, fix, stage, buildNum) */
#define kTelMgrVersionMajor	1
#define kTelMgrVersionMinor	0
#define kTelMgrVersionFix	3
#define kTelMgrVersionBuild 0

/* telephony manager shared lib version */ 
#define kTelMgrVersion sysMakeROMVersion(kTelMgrVersionMajor, kTelMgrVersionMinor, kTelMgrVersionFix, sysROMStageBeta, kTelMgrVersionBuild) 

/* TelMgr shared lib internal name */
#define kTelMgrLibName 				"Telephony Library"

/* TelMgr shared lib name and creator */
#define kTelMgrDatabaseCreator		 sysFileCTelMgrLib	
#define kTelMgrDatabaseType			 sysFileTLibrary	

#define kTelTelephonyNotification 	 'tmgr'				/* telephony notification */
#define kTelTelephonyEvent			 0x1200				/* telephony event */

/* Telephony notification IDs */
enum	{kTelSmsLaunchCmdIncomingMessage,		/* an incoming SMS */
		 kTelSpcLaunchCmdIncomingCall,			/* an incoming voice call */
		 kTelSpcLaunchCmdCallerIDAvailable,		/* the caller ID is available */
		 kTelSpcLaunchCmdCallReleased,		 	/* the call has been released */
		 kTelSpcLaunchCmdCallBusy,				/* the called equipment is busy */
		 kTelSpcLaunchCmdCallConnect,			/* the line is opened */
		 kTelSpcLaunchCmdCallError,				/* the call has encountered an error */
		 kTelEmcLaunchCmdCallReleased, 			/* the call has been released */
		 kTelEmcLaunchCmdCallBusy,				/* the called equipment is busy */
		 kTelEmcLaunchCmdCallConnect,			/* the line is opened */
		 kTelEmcLaunchCmdCallError,				/* the call has encountered an error */
		 kTelLastLaunchCode};	

/* notification priorities */
#define kTelCallNotificationPriority		0  /* higher priority */
#define kTelSmsNotificationPriority			1  

/* error codes */
#define telErrMsgAllocation			(telErrorClass | 0x01)				/* couldn't allocate message */	
#define telErrUnknown				(telErrorClass | 0x02)				/* unknown Tel internal error */	
#define telErrMemAllocation	    	(telErrorClass | 0x03)				/* memory allocation error */

#define telErrResultTimeOut			(telErrorClass | 0x04)				/* time-out was reached */
#define telErrResultUserCancel		(telErrorClass | 0x05)				/* user cancelled action */
#define	telErrResultBusyResource	(telErrorClass | 0x06)				/* resource is busy */
#define telErrInvalidAppId			(telErrorClass | 0x07)				/* don't know that application */
#define telErrTooManyApps			(telErrorClass | 0x08)				/* applications table is full */
#define telErrSecurity 				(telErrorClass | 0x09)				/* access to ME has not been granted */
#define telErrBufferSize		    (telErrorClass | 0x0A)				/* buffer used to retrieve data is too small */
#define telErrFeatureNotSupported  	(telErrorClass | 0x0B)				/* the feature is not supported by phone/network */

#define telErrPhoneComm				(telErrorClass | 0x0C)				/* the communication link with the phone is down */
#define telErrPhoneReply			(telErrorClass | 0x0D)				/* the phone reply syntax is incorrect, check the phone driver! */
#define telErrCommandFailed			(telErrorClass | 0x0E)				/* the phone couldn't achieve the associated command, check the phone driver!  */

#define	telErrSpcLineIsBusy			(telErrorClass | 0x0F)				/* spc call failure events, error field values */

#define telErrPhoneCodeRequired		(telErrorClass | 0x10)				/* phone code required */
#define telErrNoSIMInserted			(telErrorClass | 0x11)				/* no SIM inserted */
#define telErrPINRequired			(telErrorClass | 0x12)				/* PIN is required */
#define telErrPUKRequired			(telErrorClass | 0x13)				/* PUK is required */
#define telErrSIMFailure			(telErrorClass | 0x14)				/* the SIM is not working properly */
#define	telErrSIMBusy				(telErrorClass | 0x15)				/* the SIM couldn't reply */
#define telErrSIMWrong				(telErrorClass | 0x16)				/* the SIM is not accepted by the phone */
#define telErrPassword				(telErrorClass | 0x17)				/* incorrect password */
#define telErrPIN2Required			(telErrorClass | 0x18)				/* PIN2 is required */
#define telErrPUK2Required			(telErrorClass | 0x19)				/* PUK2 is required */
#define telErrPhoneMemAllocation	(telErrorClass | 0x1A)				/* phone memory is full */
#define telErrInvalidIndex			(telErrorClass | 0x1B)				/* invalid index when accessing a storage */
#define telErrEntryNotFound			(telErrorClass | 0x1C)				/* entry not found */
#define telErrPhoneMemFailure		(telErrorClass | 0x1D)				/* the phone encountered a memory error */
#define telErrInvalidString			(telErrorClass | 0x1E)				/* bad character in text string */
#define telErrInvalidDial			(telErrorClass | 0x1F)				/* bad character in dial string */
#define telErrNoNetwork				(telErrorClass | 0x20)				/* no network available */
#define telErrNetworkTimeOut		(telErrorClass | 0x21)				/* the network didn't reply within 'normal' time delay */
#define telErrInvalidParameter		(telErrorClass | 0x22)				/* bad parameter passed to an API */
#define telErrValidityPeriod		(telErrorClass | 0x23)				/* the specified short message validity period is invalid */
#define telErrCodingScheme			(telErrorClass | 0x24)				/* the specified short message coding scheme is invalid */
#define telErrPhoneNumber			(telErrorClass | 0x25)				/* the specified short message smsc or destination phone number is invalid */
#define telErrValueStale			(telErrorClass | 0x26)				/* information couldn't be retrieved, a copy of last retrieved value was returned */
#define telErrTTaskNotRunning		(telErrorClass | 0x27)				/* the Telephony Task is not running */
#define telErrPhoneToSIMPINRequired (telErrorClass | 0x28)				/* Phone 2 SIM PIN is required */

#define telErrSpecificDrvNotFound 	(telErrorClass | 0x29)				/* the specified driver was not found */
#define telErrGenericDrvNotFound	(telErrorClass | 0x2A)				/* the generic driver was not found */
#define telErrNoSpecificDrv			(telErrorClass | 0x2B)				/* no specific driver was specified */
	
#define	telErrSpcLineIsReleased		(telErrorClass | 0x2C)				/* the call has been released */
#define	telErrSpcCallError			(telErrorClass | 0x2D)				/* the call has encountered an error */

#define	telErrNotInstalled			(telErrorClass | 0x2E)				/* the shared lib couldn't be installed */
#define	telErrVersion				(telErrorClass | 0x2F)				/* the shared lib version doesn't match the application one */
#define	telErrSettings				(telErrorClass | 0x30)				/* bad telephony settings: Phone Panel Prefs doesn't exist or Telephony Profile not (correctly) set */

#define	telErrUnavailableValue		(telErrorClass | 0x31)				/* the asked value can't be retrieved at that time (i.e.: TelSpcGetCallerNumber and no active line) */

#define	telErrLimitedCompatibility	(telErrorClass | 0x32)				/* the current driver is partially compatible with the connected phone */

#define	telErrProfileConflict		(telErrorClass | 0x33)				/* the currently used profile conflicts with the requested profile */

#define	telErrLibStillInUse			(telErrorClass | 0x34)				/* the shared lib is currently being used by another app, don't unload it! */

#define	telErrTTaskNotFound			(telErrorClass | 0x35)				/* couldn't find the specified (by phone driver) telephony task */

/* constants */
#define kTelInvalidAppId			((TelAppID)-1)	/* this value can't be returned on TelMgr attachement */

#define	kTelInfiniteDelay			0xFFFFFFFF		/* infinite time-out delay */

#define kTelLocationSeparator 		';'				/* this symbol is used to separate location string tokens */

#define kTelNwkAutomaticSearch		0				/* network search mode */
#define kTelNwkManualSearch			1

#define kTelNwkCDMA					0				/* network type */
#define kTelNwkGSM					1		
#define kTelNwkTDMA					2				
#define kTelNwkPDC					3				

#define kTelPowBatteryPowered		0				/* battery status */
#define kTelPowBatteryNotPowered	1
#define kTelPowNoBattery			2
#define kTelPowBatteryFault			3

#define kTelSpcCallingLineId		-1				/* ID of a calling line. We can't provide a real ID knowing that an error might occur after
														TelSpcCallNumber return... So use this one to 'close' the line */
/* Messages types */
#define kTelSmsMessageTypeDelivered		0
#define kTelSmsMessageTypeReport		1
#define kTelSmsMessageTypeSubmitted		2
#define kTelSmsMessageTypeManualAck		3
#define kTelSmsMessageAllTypes			4

#define kTelSmsMultiPartExtensionTypeId 0x00 	/* Multipart short messages */
#define kTelSmsNbsExtensionTypeId 		0x04 	/* NBS message, with port number in short */
#define kTelSmsNbs2ExtensionTypeId 		0x05 	/* NBS message, with port number in long */

#define kTelSmsDefaultProtocol		0				/* sms message transport protocol */
#define kTelSmsFaxProtocol			1
#define kTelSmsX400Protocol			2
#define kTelSmsPagingProtocol		3
#define kTelSmsEmailProtocol		4
#define kTelSmsErmesProtocol		5
#define kTelSmsVoiceProtocol		6

#define kTelSmsAPIVersion			0x0001		/* SMS api version */

#define kTelSmsStorageSIM			0			/* SMS storage IDs */
#define kTelSmsStoragePhone			1
#define kTelSmsStorageAdaptor		2
#define kTelSmsStorageFirstOem		3

#define kTelSmsCMTMessageType		0	/* Cellular Messaging Teleservice message */
#define kTelSmsCPTMessageType		1	/* Cellular Paging Teleservice message */
#define kTelSmsVMNMessageType		2	/* Voice Mail Notification message */


/* Delivery report Type (UInt8) - Only used in CDMA & TDMA advanced parameters */
#define kTelSmsStatusReportDeliveryType	0	/* Status report or delivery acknowledge */
#define kTelSmsManualAckDeliveryType		1	/* Manual acknowledge delivery */


/* Data coding scheme (UInt8) */
#define kTelSms8BitsEncoding		0
#define kTelSmsBitsASCIIEncoding	1	/* ANSI X3.4 */
#define kTelSmsIA5Encoding			2	/* CCITTT T.50 */
#define kTelSmsIS91Encoding			3	/* TIA/EIA/IS-91 section 3.7.1 */
#define kTelSmsUCS2Encoding 		4	/* Only supported by GSM */
#define kTelSmsDefaultGSMEncoding	5	/* Only supported by GSM */


/* Message urgency / priority (UInt8) - Only used in CDMA & TDMA advanced parameters */
#define kTelSmsUrgencyNormal 		0
#define kTelSmsUrgencyUrgent		1
#define kTelSmsUrgencyEmergency		2
/*Bulk (CDMA) & Interactive mode (TDMA) are not supported */


/* Privacy message indicator (UInt8) - Only used in CDMA & TDMA advanced parameters */
#define kTelSmsPrivacyNotRestricted  0	/* Privacy level 0 */
#define kTelSmsPrivacyRestricted	 1	/* Privacy level 1 */
#define kTelSmsPrivacyConfidential	 2	/* Privacy level 2 */
#define kTelSmsPrivacySecret		 3	/* Privacy level 3 */


/* Delivery status report (UInt8) */
#define kTelSmsDSRSuccess					0
#define kTelSmsDSRMessageReplaced			1
#define kTelSmsDSRMessageForwarded			2 /* unknown delivery result */
#define kTelSmsDSRTempCongestion			3
#define kTelSmsDSRTempSMEBusy				4
#define kTelSmsDSRTempServiceRejected		5
#define kTelSmsDSRTempServiceUnavailable 	6
#define kTelSmsDSRTempSMEError				7
#define kTelSmsDSRTempOther					8
#define kTelSmsDSRPermRPError				9
#define kTelSmsDSRPermBadDestination		10
#define kTelSmsDSRPermUnobtainable			11
#define kTelSmsDSRPermServiceUnavailable	12
#define kTelSmsDSRPermInternetworkError		13
#define kTelSmsDSRPermValidityExpired		14
#define kTelSmsDSRPermDeletedByOrigSME		15
#define kTelSmsDSRPermDeleteByAdm			16
#define kTelSmsDSRPermSMNotExist			17
#define kTelSmsDSRPermOther					18

#define kTelSpeechCallClass				0			/* call classes */
#define kTelDataCallClass				1
#define kTelFaxCallClass				2
	
#define kTelPhbFixedPhonebook   		0	/* phonebooks */
#define kTelPhbSimPhonebook    			1
#define kTelPhbPhonePhonebook     		2
#define kTelPhbLastDialedPhonebook   	3
#define kTelPhbSimAndPhonePhonebook   	4
#define kTelPhbAdaptorPhonebook    		5
#define kTelPhbFirstOemPhonebook    	6

#define kTelCallIdle					0	/* call states */
#define kTelCallConnecting				1
#define kTelCallConnected				2
#define kTelCallRedial					3
#define kTelCallIncoming				4
#define kTelCallIncomingAck				5
#define kTelCallDisconnecting			6

#define kTelCallTypeOutgoing			0	/* call type */
#define kTelCallTypeIncoming			1

#define kTelCallServiceVoice			0 	/* call service type */
#define kTelCallServiceData				1

#define kTelStyReady					0	/* no more security code expected */
#define kTelStyPin1CodeId				1	/* authentication code IDs */
#define kTelStyPin2CodeId				2
#define kTelStyPuk1CodeId				3
#define kTelStyPuk2CodeId				4
#define kTelStyPhoneToSimCodeId			5
#define kTelStyFirstOemCodeId			6

#define kTelInfPhoneBrand				0	/* phone information type */
#define kTelInfPhoneModel				1
#define	kTelInfPhoneRevision			2

/* TelMgr library call ID's */
/* first entry points are reserved for internal use only */
#define telLibTrapReserved1				(sysLibTrapCustom)
#define telLibTrapReserved2				(sysLibTrapCustom+1)
#define telLibTrapReserved3				(sysLibTrapCustom+2)
#define telLibTrapReserved4				(sysLibTrapCustom+3)
#define telLibTrapReserved5				(sysLibTrapCustom+4)
#define telLibTrapReserved6				(sysLibTrapCustom+5)
#define telLibTrapReserved7				(sysLibTrapCustom+6)
#define telLibTrapReserved8				(sysLibTrapCustom+7)
#define telLibTrapReserved9				(sysLibTrapCustom+8)
#define telLibTrapReserved10			(sysLibTrapCustom+9)

#define telLibTrapGetEvent				(sysLibTrapCustom+10)
#define telLibTrapGetTelephonyEvent		(sysLibTrapCustom+11)

#define telLibTrapOpenPhoneConnection	(sysLibTrapCustom+12)
#define telLibTrapIsPhoneConnected		(sysLibTrapCustom+13)
#define telLibTrapClosePhoneConnection	(sysLibTrapCustom+14)

#define telLibTrapIsServiceAvailable	(sysLibTrapCustom+15)
#define telLibTrapIsFunctionSupported	(sysLibTrapCustom+16)

#define telLibTrapSendCommandString		(sysLibTrapCustom+17)

#define telLibTrapCancel				(sysLibTrapCustom+18)

#define telLibTrapMatchPhoneDriver		(sysLibTrapCustom+19)

#define telLibTrapGetCallState			(sysLibTrapCustom+20)

#define telLibTrapOemCall				(sysLibTrapCustom+21)

#define telLibTrapNwkGetNetworks		(sysLibTrapCustom+22)
#define telLibTrapNwkGetNetworkName		(sysLibTrapCustom+23)
#define telLibTrapNwkGetLocation		(sysLibTrapCustom+24)
#define telLibTrapNwkSelectNetwork		(sysLibTrapCustom+25)
#define telLibTrapNwkGetSelectedNetwork	(sysLibTrapCustom+26)
#define telLibTrapNwkGetNetworkType		(sysLibTrapCustom+27)
#define telLibTrapNwkGetSignalLevel		(sysLibTrapCustom+28)
#define telLibTrapNwkGetSearchMode		(sysLibTrapCustom+29)
#define telLibTrapNwkSetSearchMode		(sysLibTrapCustom+30)

#define telLibTrapStyChangeAuthenticationCode	(sysLibTrapCustom+31)
#define telLibTrapStyGetAuthenticationState		(sysLibTrapCustom+32)
#define telLibTrapStyEnterAuthenticationCode	(sysLibTrapCustom+33)

#define telLibTrapPowGetPowerLevel		(sysLibTrapCustom+34)
#define telLibTrapPowGetBatteryStatus	(sysLibTrapCustom+35)
#define telLibTrapPowSetPhonePower		(sysLibTrapCustom+36)

#define telLibTrapCfgSetSmsCenter		(sysLibTrapCustom+37)
#define telLibTrapCfgGetSmsCenter		(sysLibTrapCustom+38)
#define telLibTrapCfgGetPhoneNumber		(sysLibTrapCustom+39)

#define telLibTrapSmsGetUniquePartId	(sysLibTrapCustom+40)
#define telLibTrapSmsGetDataMaxSize		(sysLibTrapCustom+41)
#define telLibTrapSmsSendMessage		(sysLibTrapCustom+42)
#define telLibTrapSmsSendManualAcknowledge	(sysLibTrapCustom+43)
#define telLibTrapSmsReadMessage		(sysLibTrapCustom+44)
#define telLibTrapSmsReadMessages		(sysLibTrapCustom+45)
#define telLibTrapSmsReadReport			(sysLibTrapCustom+46)
#define telLibTrapSmsReadReports		(sysLibTrapCustom+47)
#define telLibTrapSmsReadSubmittedMessage	(sysLibTrapCustom+48)
#define telLibTrapSmsReadSubmittedMessages	(sysLibTrapCustom+49)
#define telLibTrapSmsGetMessageCount	(sysLibTrapCustom+50)
#define telLibTrapSmsDeleteMessage		(sysLibTrapCustom+51)
#define telLibTrapSmsGetAvailableStorage	(sysLibTrapCustom+52)
#define telLibTrapSmsGetSelectedStorage	(sysLibTrapCustom+53)
#define telLibTrapSmsSelectStorage		(sysLibTrapCustom+54)

#define telLibTrapEmcCall				(sysLibTrapCustom+55)
#define telLibTrapEmcCloseLine			(sysLibTrapCustom+56)
#define telLibTrapEmcGetNumberCount		(sysLibTrapCustom+57)
#define telLibTrapEmcGetNumber			(sysLibTrapCustom+58)
#define telLibTrapEmcSetNumber			(sysLibTrapCustom+59)
#define telLibTrapEmcSelectNumber		(sysLibTrapCustom+60)

#define telLibTrapSpcCallNumber			(sysLibTrapCustom+61)
#define telLibTrapSpcCloseLine			(sysLibTrapCustom+62)
#define telLibTrapSpcHoldLine			(sysLibTrapCustom+63)
#define telLibTrapSpcRetrieveHeldLine	(sysLibTrapCustom+64)
#define telLibTrapSpcConference			(sysLibTrapCustom+65)
#define telLibTrapSpcSelectLine			(sysLibTrapCustom+66)
#define telLibTrapSpcAcceptCall			(sysLibTrapCustom+67)
#define telLibTrapSpcRejectCall			(sysLibTrapCustom+68)
#define telLibTrapSpcGetCallerNumber	(sysLibTrapCustom+69)
#define telLibTrapSpcSendBurstDTMF		(sysLibTrapCustom+70)
#define telLibTrapSpcStartContinuousDTMF	(sysLibTrapCustom+71)
#define telLibTrapSpcStopContinuousDTMF	(sysLibTrapCustom+72)
#define telLibTrapSpcPlayDTMF			(sysLibTrapCustom+73)

#define telLibTrapPhbGetEntryCount		(sysLibTrapCustom+74)
#define telLibTrapPhbGetEntry			(sysLibTrapCustom+75)
#define telLibTrapPhbGetEntries			(sysLibTrapCustom+76)
#define telLibTrapPhbAddEntry			(sysLibTrapCustom+77)
#define telLibTrapPhbDeleteEntry		(sysLibTrapCustom+78)
#define telLibTrapPhbGetAvailablePhonebooks	(sysLibTrapCustom+79)
#define telLibTrapPhbSelectPhonebook	(sysLibTrapCustom+80)
#define telLibTrapPhbGetSelectedPhonebook	(sysLibTrapCustom+81)
#define telLibTrapPhbGetEntryMaxSizes	(sysLibTrapCustom+82)

#define telLibTrapSndPlayKeyTone		(sysLibTrapCustom+83)
#define telLibTrapSndStopKeyTone		(sysLibTrapCustom+84)
#define telLibTrapSndMute				(sysLibTrapCustom+85)

#define telLibTrapInfGetInformation		(sysLibTrapCustom+86)

#define telLibTrapDtcCallNumber			(sysLibTrapCustom+87)
#define telLibTrapDtcCloseLine			(sysLibTrapCustom+88)
#define telLibTrapDtcSendData			(sysLibTrapCustom+89)
#define telLibTrapDtcReceiveData		(sysLibTrapCustom+90)

#define telLibTrapUnblockNotifications	(sysLibTrapCustom+91)

#define telLibTrapOpenProfile			(sysLibTrapCustom+92)

#define telLibTrapLast					(sysLibTrapCustom+93)

#ifdef __cplusplus
extern "C" {
#endif

/* function traps */
Err TelOpen(UInt16 		iRefnum, 
			UInt32		iVersnum,
		    TelAppID 	*oAppIdP)
		SYS_TRAP(sysLibTrapOpen);

Err TelClose(UInt16 	iRefnum, 
			 TelAppID 	iAppId)
		SYS_TRAP(sysLibTrapClose);

/* events management */
void TelGetEvent(UInt16 	iRefnum,
				 TelAppID 	iAppId, 
				 EventPtr	oEventP, 
				 Int32 		iTimeOut)
		SYS_TRAP(telLibTrapGetEvent);

void TelGetTelephonyEvent(UInt16 	iRefnum,
						  TelAppID 	iAppId, 
						  EventPtr 	oEventP, 
						  Int32 	iTimeOut)
		SYS_TRAP(telLibTrapGetTelephonyEvent);
		
/* phone connection management */
Err	TelOpenPhoneConnection(UInt16		iRefnum,
					  	   TelAppID 	iAppId, 
						   UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapOpenPhoneConnection);

Err	TelIsPhoneConnected(UInt16		iRefnum,
						TelAppID	iAppId, 
						UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapIsPhoneConnected);

Err	TelClosePhoneConnection(UInt16		iRefnum,
						  	TelAppID 	iAppId, 
						  	UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapClosePhoneConnection);

/* service/function availability */
Err	TelIsServiceAvailable(UInt16	iRefnum,
						  TelAppID	iAppId, 
						  UInt16	serviceId,
						  UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapIsServiceAvailable);

Err	TelIsFunctionSupported(UInt16	 iRefnum,
					       TelAppID  iAppId, 
						   UInt16	 functionId,
						   UInt16*	 ioTransIdP)
		SYS_TRAP(telLibTrapIsFunctionSupported);

/* sending commands to the phone */
Err	TelSendCommandString(UInt16	  					iRefnum,
					     TelAppID 					iAppId, 
						 TelSendCommandStringType* 	ioParamP ,
						 UInt16*  					ioTransIdP)
		SYS_TRAP(telLibTrapSendCommandString);

/* cancelling asynchronous calls */
Err	TelCancel(UInt16	iRefnum,
			  TelAppID 	iAppId, 
			  UInt16  	iTransId,
			  UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapCancel);

/* checking whether phone and driver match */
Err	TelMatchPhoneDriver(UInt16	 iRefnum,
			  			TelAppID iAppId, 
			  			UInt16*  ioTransIdP)
		SYS_TRAP(telLibTrapMatchPhoneDriver);

/* getting phone status */
Err	TelGetCallState(UInt16	 			 iRefnum,
			  		TelAppID 			 iAppId, 
			  		TelGetCallStateType* ioParamP,
			  		UInt16*  			 ioTransIdP)
		SYS_TRAP(telLibTrapGetCallState);

/* OEM support */
Err	TelOemCall(UInt16 	 	   iRefnum,
			   TelAppID 	   iAppId, 
			   TelOemCallType* ioParamP,
			   UInt16*		   ioTransIdP)
		SYS_TRAP(telLibTrapOemCall);

/* network */
Err	TelNwkGetNetworks(UInt16 	 				iRefnum,
				  	  TelAppID	 				iAppId, 
				  	  TelNwkGetNetworksType*	ioParamP,
				  	  UInt16*	 				ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetNetworks);

Err	TelNwkGetNetworkName(UInt16 	 				iRefnum,
				  		 TelAppID	 				iAppId, 
				  		 TelNwkGetNetworkNameType*	ioParamP,
				  		 UInt16*	 				ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetNetworkName);

Err	TelNwkGetLocation(UInt16 					iRefnum,
				  	  TelAppID					iAppId, 
					  TelNwkGetLocationType*	ioParamP,
					  UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetLocation);

Err	TelNwkSelectNetwork(UInt16 		iRefnum,
				  		TelAppID	iAppId, 
				  		UInt32	 	iNetworkId,
				  		UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapNwkSelectNetwork);

Err	TelNwkGetSelectedNetwork(UInt16 	iRefnum,
				  			 TelAppID	iAppId, 
					  		 UInt32* 	oNetworkIdP,
					  		 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetSelectedNetwork);

Err	TelNwkGetNetworkType(UInt16 	iRefnum,
				  		 TelAppID	iAppId, 
					  	 UInt8* 	oTypeP,
					  	 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetNetworkType);

Err	TelNwkGetSignalLevel(UInt16 	iRefnum,
				  		 TelAppID	iAppId, 
					  	 UInt8* 	oSignalP,
					  	 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetSignalLevel);

Err	TelNwkGetSearchMode(UInt16 		iRefnum,
				  		TelAppID	iAppId, 
					  	UInt8* 		oModeP,
					  	UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapNwkGetSearchMode);

Err	TelNwkSetSearchMode(UInt16 		iRefnum,
				  		TelAppID	iAppId, 
					  	UInt8 		iMode,
					  	UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapNwkSetSearchMode);

/* security */
Err	TelStyGetAuthenticationState(UInt16		iRefnum,
						  		TelAppID	iAppId, 
							  	UInt8* 		oStateP,
							  	UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapStyGetAuthenticationState);

Err	TelStyEnterAuthenticationCode(UInt16		iRefnum,
						  		  TelAppID		iAppId, 
							  	  const Char*	iCodeP,
							  	  UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapStyEnterAuthenticationCode);

Err	TelStyChangeAuthenticationCode(UInt16							iRefnum,
						  		   TelAppID							iAppId, 
							  	   TelStyChangeAuthenticationType*	iParamP,
							  	   UInt16*							ioTransIdP)
		SYS_TRAP(telLibTrapStyChangeAuthenticationCode);

/* power */
Err	TelPowGetPowerLevel(UInt16		iRefnum,
						TelAppID	iAppId, 
						UInt8* 		oPowerP,
						UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapPowGetPowerLevel);

Err	TelPowGetBatteryStatus(UInt16	iRefnum,
						   TelAppID	iAppId, 
						   UInt8*	oStatusP,
						   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapPowGetBatteryStatus);

Err	TelPowSetPhonePower(UInt16		iRefnum,
						TelAppID	iAppId, 
						Boolean		iPowerOn)
		SYS_TRAP(telLibTrapPowSetPhonePower);

/* configuration */
Err	TelCfgSetSmsCenter(UInt16		iRefnum,
					   TelAppID		iAppId, 
					   const Char* 	iDialNumberP,
					   UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapCfgSetSmsCenter);

Err	TelCfgGetSmsCenter(UInt16					iRefnum,
					   TelAppID					iAppId, 
					   TelCfgGetSmsCenterType* 	ioParamP,
					   UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapCfgGetSmsCenter);

Err	TelCfgGetPhoneNumber(UInt16						iRefnum,
					     TelAppID					iAppId, 
					     TelCfgGetPhoneNumberType* 	ioParamP,
					     UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapCfgGetPhoneNumber);

/* sms */
Err	TelSmsGetUniquePartId(UInt16	iRefnum,
					     TelAppID	iAppId, 
					     UInt16	 	*oUniqueIdP,
					     UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSmsGetUniquePartId);

Err	TelSmsGetDataMaxSize(UInt16		iRefnum,
					     TelAppID	iAppId, 
					     UInt16*	oSizeP,
					     UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSmsGetDataMaxSize);

Err	TelSmsSendMessage(UInt16					iRefnum,
					  TelAppID					iAppId, 
					  TelSmsSendMessageType*	ioMessageP,
					  UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapSmsSendMessage);

Err	TelSmsSendManualAcknowledge(UInt16					iRefnum,
							   TelAppID					iAppId, 
							   TelSmsManualAckType*		ioAckP,
							   UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapSmsSendManualAcknowledge);

Err	TelSmsReadMessage(UInt16						iRefnum,
					  TelAppID						iAppId, 
					  TelSmsDeliveryMessageType*	ioMessageP,
					  UInt16*						ioTransIdP)
		SYS_TRAP(telLibTrapSmsReadMessage);

Err	TelSmsReadMessages(UInt16					iRefnum,
					   TelAppID					iAppId, 
					   TelSmsReadMessagesType*	ioParamP,
					   UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapSmsReadMessages);

Err	TelSmsReadReport(UInt16				iRefnum,
					 TelAppID			iAppId, 
					 TelSmsReportType*	ioReportP,
					 UInt16*			ioTransIdP)
		SYS_TRAP(telLibTrapSmsReadReport);

Err	TelSmsReadReports(UInt16					iRefnum,
					  TelAppID					iAppId, 
					  TelSmsReadReportsType*	ioParamP,
					  UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapSmsReadReports);

Err	TelSmsReadSubmittedMessage(UInt16						iRefnum,
							   TelAppID						iAppId, 
							   TelSmsSubmittedMessageType*	ioMessageP,
							   UInt16*						ioTransIdP)
		SYS_TRAP(telLibTrapSmsReadSubmittedMessage);

Err	TelSmsReadSubmittedMessages(UInt16								iRefnum,
							    TelAppID							iAppId, 
							    TelSmsReadSubmittedMessagesType*	ioParamP,
							    UInt16*								ioTransIdP)
		SYS_TRAP(telLibTrapSmsReadSubmittedMessages);

Err	TelSmsGetMessageCount(UInt16						iRefnum,
						   TelAppID						iAppId, 
						   TelSmsGetMessageCountType*	ioParamP,
						   UInt16*						ioTransIdP)
		SYS_TRAP(telLibTrapSmsGetMessageCount);

Err	TelSmsDeleteMessage(UInt16						iRefnum,
						TelAppID					iAppId, 
						TelSmsDeleteMessageType*	ioParamP,
						UInt16*						ioTransIdP)
		SYS_TRAP(telLibTrapSmsDeleteMessage);

Err	TelSmsGetAvailableStorage(UInt16							iRefnum,
							  TelAppID							iAppId, 
							  TelSmsGetAvailableStorageType*	ioParamP,
							  UInt16*							ioTransIdP)
		SYS_TRAP(telLibTrapSmsGetAvailableStorage);

Err	TelSmsGetSelectedStorage(UInt16		iRefnum,
							 TelAppID	iAppId, 
							 UInt8*		oStorageIdP,
							 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSmsGetSelectedStorage);

Err	TelSmsSelectStorage(UInt16		iRefnum,
						TelAppID	iAppId, 
						UInt8		iStorageId,
						UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapSmsSelectStorage);

/* emergency calls */
Err	TelEmcCall(UInt16	iRefnum,
			   TelAppID	iAppId, 
			   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapEmcCall);

Err	TelEmcCloseLine(UInt16		iRefnum,
					TelAppID	iAppId, 
					UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapEmcCloseLine);

Err	TelEmcGetNumberCount(UInt16	iRefnum,
						  TelAppID	iAppId, 
						  UInt8*	oCountP,
						  UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapEmcGetNumberCount);

Err	TelEmcGetNumber(UInt16					iRefnum,
					TelAppID				iAppId, 
					TelEmcGetNumberType*	ioParamP,
					UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapEmcGetNumber);

Err	TelEmcSetNumber(UInt16					iRefnum,
					TelAppID				iAppId, 
					TelEmcSetNumberType*	iParamP,
					UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapEmcSetNumber);

Err	TelEmcSelectNumber(UInt16	iRefnum,
					   TelAppID	iAppId, 
					   UInt8  	iIndex,
					   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapEmcSelectNumber);

/* speech call */
Err	TelSpcCallNumber(UInt16			iRefnum,
					 TelAppID		iAppId, 
					 const Char*	iDialNumberP,
					 UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapSpcCallNumber);

Err	TelSpcCloseLine(UInt16		iRefnum,
					TelAppID	iAppId, 
					UInt8	  	iLineId,
					UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapSpcCloseLine);

Err	TelSpcHoldLine(UInt16	iRefnum,
				   TelAppID	iAppId, 
				   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcHoldLine);

Err	TelSpcRetrieveHeldLine(UInt16	iRefnum,
				   		   TelAppID	iAppId, 
				   		   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcRetrieveHeldLine);

Err	TelSpcConference(UInt16		iRefnum,
				   	 TelAppID	iAppId, 
				   	 UInt8*		oLineIdP,
				   	 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcConference);

Err	TelSpcSelectLine(UInt16		iRefnum,
				   	 TelAppID	iAppId, 
				   	 UInt8		iLineId,
				   	 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcSelectLine);

Err	TelSpcAcceptCall(UInt16		iRefnum,
				   	 TelAppID	iAppId, 
				   	 UInt8*		oLineIdP,
				   	 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcAcceptCall);

Err	TelSpcRejectCall(UInt16		iRefnum,
				   	 TelAppID	iAppId, 
				   	 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcRejectCall);

Err	TelSpcGetCallerNumber(UInt16						iRefnum,
				   	 	  TelAppID						iAppId, 
				   	 	  TelSpcGetCallerNumberType* 	ioParamP,
				   	      UInt16*						ioTransIdP)
		SYS_TRAP(telLibTrapSpcGetCallerNumber);

Err	TelSpcSendBurstDTMF(UInt16		iRefnum,
					 	TelAppID	iAppId, 
					 	const Char*	iDTMFStringP,
					 	UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapSpcSendBurstDTMF);

Err	TelSpcStartContinuousDTMF(UInt16	iRefnum,
							  TelAppID	iAppId, 
							  UInt8		iKeyCode,
							  UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcStartContinuousDTMF);

Err	TelSpcStopContinuousDTMF(UInt16		iRefnum,
							 TelAppID	iAppId, 
							 UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSpcStopContinuousDTMF);

Err	TelSpcPlayDTMF(UInt16				iRefnum,
				   TelAppID				iAppId, 
				   TelSpcPlayDTMFType*	iParamP,
				   UInt16*				ioTransIdP)
		SYS_TRAP(telLibTrapSpcPlayDTMF);

/* phonebook */
Err TelPhbGetEntryCount(UInt16					 iRefnum,
						TelAppID				 iAppId, 
						TelPhbGetEntryCountType* oParamP,
						UInt16*					 ioTransIdP)
		SYS_TRAP(telLibTrapPhbGetEntryCount);

Err TelPhbGetEntry(UInt16				iRefnum,
				   TelAppID				iAppId, 
				   TelPhbEntryType* 	ioEntryP,
				   UInt16*				ioTransIdP)
		SYS_TRAP(telLibTrapPhbGetEntry);

Err TelPhbGetEntries(UInt16					iRefnum,
				     TelAppID				iAppId, 
				     TelPhbGetEntriesType* 	ioParamP,
				     UInt16*				ioTransIdP)
		SYS_TRAP(telLibTrapPhbGetEntries);

Err TelPhbAddEntry(UInt16				iRefnum,
				   TelAppID				iAppId, 
				   TelPhbEntryType* 	iEntryP,
				   UInt16*				ioTransIdP)
		SYS_TRAP(telLibTrapPhbAddEntry);

Err TelPhbDeleteEntry(UInt16	iRefnum,
				   	  TelAppID	iAppId, 
				      UInt16 	iEntryIndex,
				      UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapPhbDeleteEntry);

Err TelPhbGetAvailablePhonebooks(UInt16								iRefnum,
						   	     TelAppID							iAppId, 
						         TelPhbGetAvailablePhonebooksType* 	ioParamP,
						         UInt16*							ioTransIdP)
		SYS_TRAP(telLibTrapPhbGetAvailablePhonebooks);

Err TelPhbSelectPhonebook(UInt16	iRefnum,
				   	  	  TelAppID	iAppId, 
				      	  UInt8 	iPhbId,
				      	  UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapPhbSelectPhonebook);

Err TelPhbGetSelectedPhonebook(UInt16	iRefnum,
				   	 	 	   TelAppID	iAppId, 
				      		   UInt8* 	oPhbIdP,
					      	   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapPhbGetSelectedPhonebook);

Err TelPhbGetEntryMaxSizes(UInt16						iRefnum,
				   	 	   TelAppID						iAppId, 
				      	   TelPhbGetEntryMaxSizesType* 	oParamP,
					       UInt16*						ioTransIdP)
		SYS_TRAP(telLibTrapPhbGetEntryMaxSizes);

/* sound */
Err	TelSndPlayKeyTone(UInt16					iRefnum,
				   	  TelAppID					iAppId, 
				      TelSndPlayKeyToneType* 	iParamP,
					  UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapSndPlayKeyTone);

Err	TelSndStopKeyTone(UInt16					iRefnum,
				   	  TelAppID					iAppId, 
					  UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapSndStopKeyTone);

Err	TelSndMute(UInt16	iRefnum,
			   TelAppID	iAppId, 
			   Boolean	iMuteOn,
			   UInt16*	ioTransIdP)
		SYS_TRAP(telLibTrapSndMute);

/* information */
Err	TelInfGetInformation(UInt16						iRefnum,
					     TelAppID					iAppId, 
					     TelInfGetInformationType* 	ioParamP,
					     UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapInfGetInformation);

/* data */
Err	TelDtcCallNumber(UInt16					iRefnum,
					 TelAppID				iAppId, 
					 TelDtcCallNumberType* 	ioParamP,
					 UInt16*				ioTransIdP)
		SYS_TRAP(telLibTrapDtcCallNumber);

Err	TelDtcCloseLine(UInt16		iRefnum,
					TelAppID	iAppId, 
					UInt8	 	iLineId,
					UInt16*		ioTransIdP)
		SYS_TRAP(telLibTrapDtcCloseLine);

Err	TelDtcSendData(UInt16				iRefnum,
			   	   TelAppID				iAppId, 
			   	   TelDtcSendDataType* 	iParamP,
			   	   UInt16*				ioTransIdP)
		SYS_TRAP(telLibTrapDtcSendData);

Err	TelDtcReceiveData(UInt16					iRefnum,
			   	   	  TelAppID					iAppId, 
			   	   	  TelDtcReceiveDataType* 	ioParamP,
			   	      UInt16*					ioTransIdP)
		SYS_TRAP(telLibTrapDtcReceiveData);

Err	TelUnblockNotifications(UInt16 iRefnum)
		SYS_TRAP(telLibTrapUnblockNotifications);

/* open telephony using a particular connection profile */
Err TelOpenProfile(UInt16 		iRefnum, 
				   UInt32		iVersnum,
				   UInt32 		profileId,
			       TelAppID 	*oAppIdP)
		SYS_TRAP(telLibTrapOpenProfile);

#ifdef __cplusplus
}
#endif

/* MACROS for checking service availability */
#define TelIsNwkServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelNwkServiceId, transIdP)

#define TelIsStyServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelStyServiceId, transIdP)

#define TelIsPowServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelPowServiceId, transIdP)

#define TelIsCfgServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelCfgServiceId, transIdP)

#define TelIsSmsServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelSmsServiceId, transIdP)

#define TelIsEmcServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelEmcServiceId, transIdP)

#define TelIsSpcServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelSpcServiceId, transIdP)

#define TelIsDtcServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelDtcServiceId, transIdP)

#define TelIsPhbServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelPhbServiceId, transIdP)

#define TelIsOemServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelOemServiceId, transIdP)

#define TelIsSndServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelSndServiceId, transIdP)

#define TelIsInfServiceAvailable(refnum, appId, transIdP) \
	TelIsServiceAvailable(refnum, appId, kTelInfServiceId, transIdP)

/* MACROS to check function availability */
#define TelIsSendCommandStringSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSendCommandStringMessage, transIdP)

#define TelIsCancelSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelUrqCancelMessage, transIdP)

#define TelIsMatchPhoneDriverSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelUrqMatchPhoneDriverMessage, transIdP)

#define TelIsGetCallStateSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelGetCallStateMessage, transIdP)

#define TelIsOemCallSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCallMessage, transIdP)

#define TelIsNwkGetNetworksSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetNetworksMessage, transIdP)

#define TelIsNwkGetNetworkNameSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetNetworkNameMessage, transIdP)

#define TelIsNwkGetLocationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetLocationMessage, transIdP)

#define TelIsNwkSelectNetworkSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkSelectNetworkMessage, transIdP)

#define TelIsNwkGetSelectedNetworkSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetSelectedNetworkMessage, transIdP)

#define TelIsNwkGetNetworkTypeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetNetworkTypeMessage, transIdP)

#define TelIsNwkGetSignalLevelSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetSignalLevelMessage, transIdP)

#define TelIsNwkGetSearchModeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkGetSearchModeMessage, transIdP)

#define TelIsNwkSetSearchModeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelNwkSetSearchModeMessage, transIdP)

#define TelIsStyGetAuthenticationStateSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelStyGetAuthenticationStateMessage, transIdP)

#define TelIsStyEnterAuthenticationCodeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelStyEnterAuthenticationCodeMessage, transIdP)

#define TelIsStyChangeAuthenticationCodeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelStyChangeAuthenticationCodeMessage, transIdP)

#define TelIsPowGetPowerLevelSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPowGetPowerLevelMessage, transIdP)

#define TelIsPowGetBatteryStatusSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPowGetBatteryStatusMessage, transIdP)

#define TelIsPowSetPhonePowerSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPowSetPhonePowerMessage, transIdP)

#define TelIsCfgSetSmsCenterSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelCfgSetSmsCenterMessage, transIdP)

#define TelIsCfgGetSmsCenterSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelCfgGetSmsCenterMessage, transIdP)

#define TelIsCfgGetPhoneNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelCfgGetPhoneNumberMessage, transIdP)

#define TelIsSmsGetUniquePartIdSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelUrqSmsGetUniquePartIdMessage, transIdP)

#define TelIsSmsGetDataMaxSizeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsGetDataMaxSizeMessage, transIdP)

#define TelIsSmsSendMessageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsSendMessageMessage, transIdP)

#define TelIsSmsSendManualAcknowledgeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsSendManualAcknowledgeMessage, transIdP)

#define TelIsSmsReadMessageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsReadMessageMessage, transIdP)

#define TelIsSmsReadMessagesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsReadMessagesMessage, transIdP)

#define TelIsSmsReadReportSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsReadReportMessage, transIdP)

#define TelIsSmsReadReportsSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsReadReportsMessage, transIdP)

#define TelIsSmsReadSubmittedMessageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsReadSubmittedMessageMessage, transIdP)

#define TelIsSmsReadSubmittedMessagesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsReadSubmittedMessagesMessage, transIdP)

#define TelIsSmsGetMessageCountSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsGetMessageCountMessage, transIdP)

#define TelIsSmsDeleteMessageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsDeleteMessageMessage, transIdP)

#define TelIsSmsGetAvailableStorageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsGetAvailableStorageMessage, transIdP)

#define TelIsSmsGetSelectedStorageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsGetSelectedStorageMessage, transIdP)

#define TelIsSmsSelectStorageSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSmsSelectStorageMessage, transIdP)

#define TelIsEmcCallSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelEmcCallMessage, transIdP)

#define TelIsEmcCloseLineSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelEmcCloseLineMessage, transIdP)

#define TelIsEmcGetNumberCountSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelEmcGetNumberCountMessage, transIdP)

#define TelIsEmcGetNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelEmcGetNumberMessage, transIdP)

#define TelIsEmcSetNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelEmcSetNumberMessage, transIdP)

#define TelIsEmcSelectNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelEmcSelectNumberMessage, transIdP)

#define TelIsSpcCallNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcCallNumberMessage, transIdP)

#define TelIsSpcCloseLineSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcCloseLineMessage, transIdP)

#define TelIsSpcHoldLineSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcHoldLineMessage, transIdP)

#define TelIsSpcRetrieveHeldLineSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcRetrieveHeldLineMessage, transIdP)

#define TelIsSpcConferenceSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcConferenceMessage, transIdP)

#define TelIsSpcSelectLineSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcSelectLineMessage, transIdP)

#define TelIsSpcAcceptCallSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcAcceptCallMessage, transIdP)

#define TelIsSpcRejectCallSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcRejectCallMessage, transIdP)

#define TelIsSpcGetCallerNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcGetCallerNumberMessage, transIdP)

#define TelIsSpcSendBurstDTMFSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcSendBurstDTMFMessage, transIdP)

#define TelIsSpcStartContinuousDTMFSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcStartContinuousDTMFMessage, transIdP)

#define TelIsSpcStopContinuousDTMFSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcStopContinuousDTMFMessage, transIdP)

#define TelIsSpcPlayDTMFSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSpcPlayDTMFMessage, transIdP)

#define TelIsPhbGetEntryCountSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbGetEntryCountMessage, transIdP)

#define TelIsPhbGetEntrySupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbGetEntryMessage, transIdP)

#define TelIsPhbGetEntriesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbGetEntriesMessage, transIdP)

#define TelIsPhbAddEntrySupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbAddEntryMessage, transIdP)

#define TelIsPhbDeleteEntrySupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbDeleteEntryMessage, transIdP)

#define TelIsPhbGetAvailablePhonebooksSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbGetAvailablePhonebooksMessage, transIdP)

#define TelIsPhbSelectPhonebookSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbSelectPhonebookMessage, transIdP)

#define TelIsPhbGetSelectedPhonebookSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbGetSelectedPhonebookMessage, transIdP)
		
#define TelIsPhbGetEntryMaxSizesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelPhbGetEntryMaxSizesMessage, transIdP)

#define TelIsSndPlayKeyToneSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSndPlayKeyToneMessage, transIdP)

#define TelIsSndStopKeyToneSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSndStopKeyToneMessage, transIdP)

#define TelIsSndMuteSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelSndMuteMessage, transIdP)

#define TelIsInfGetInformationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelInfGetInformationMessage, transIdP)

#define TelIsDtcCallNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelDtcCallNumberMessage, transIdP)

#define TelIsDtcCloseLineSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelDtcCloseLineMessage, transIdP)

#define TelIsDtcSendDataSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelDtcSendDataMessage, transIdP)

#define TelIsDtcReceiveDataSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelDtcReceiveDataMessage, transIdP)

#endif 
