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
 * @brief This is the header file for the Telephony Manager Extension for Palm OS 
 *	    Wireless Telephony Add-on.
 *	    It defines the new Telephony Manager public functions.
 *
 *
 *
 * @version Version 1.0 	11/21/00	Thierry Langlais	Initial version.
 * @version	Version 2.0		07/18/01	Hatem Oueslati		GPRS API Update.
 * @version Version 3.0		08/16/01	Thierry Langlais	Added Palm GSM specific errors.
 * @version Version 4.0		09/12/01	Christophe Guiraud	Added USSD API
 * @version Version 5.0		10/05/01	Christophe Guiraud	Added Alert API
 * @version Version 6.0		10/09/01	Hatem Oueslati		Voice Recognition API update
 * @version Version 7.0		10/15/01	Thierry Langlais	I Added TelOemCfgSetPhoneRTC() and
 *										TelOemCfgGetPhoneRTC() APIs.
 * @version Version 8.0		01/08/02	Hatem Oueslati		Added 
 *										TelOemSmsIsMessageGsmAlphabetCompatible 
 *										API
 * @version Version 9.0		01/25/02	Thierry Langlais	Reorganised new functions added before
 *										and added their corresponding 
 *										"TelIsFunctionSupported"
 *
 *
 *
 * @file TelephonyMgrOem.h
 *
 *
 **/






/* HOW TO
Add a new API:
	- Add its corresponding structure if any in TelephonyMgrTypesOem.h
	- Add its corresponding message (look for Message) and trap (look for 
		telLibTrap) below.
	-Add its corresponding "Is function supported" definition. 
		look for TelIsFunctionSupported below.
	- Go to TelOemMgrDispatch.c and Add the function itself.
*/

#ifndef __TELEPHONY_MGR_OEM_H__
#define __TELEPHONY_MGR_OEM_H__

#include "TelephonyMgrTypesOem.h"
#include "TelephonyMgr.h"


/** 
 * @name Defines
 *
 */
/*@{*/


/* sysMakeROMVersion(major, minor, fix, stage, buildNum) */
#define	kTelOemMgrVersionMajor				1
#define	kTelOemMgrVersionMinor				0
#define	kTelOemMgrVersionFix				0
#define	kTelOemMgrVersionStage				sysROMStageRelease
#define	kTelOemMgrVersionBuild 				0

/* telephony manager shared lib version */ 
#define	kTelOemMgrVersion sysMakeROMVersion(kTelOemMgrVersionMajor \
											, kTelOemMgrVersionMinor \
											, kTelOemMgrVersionFix \
											, kTelOemMgrVersionStage \
											, kTelOemMgrVersionBuild) 

/* TelMgrExt shared lib internal name */
#define	kTelOemMgrLibName 					"OemPhoneLib"

/* TelMgr shared lib name and creator */
#define	kTelOemMgrDatabaseCreator			'tmge'
#define	kTelOemMgrDatabaseType				sysFileTLibrary

#define	kTelOemLibId						'TOIE'

/* TelMgr compilation directives */
#define kTEL_OEM_SMS_RETRIEVE_IN_BACKGROUND		/* This compilation directive chooses the default SMS incoming retrieve mechanism:
													- if constant defined: incoming SMS are retrieved in background. It's faster since SIM is
													not used anymore to store incoming messages, they are stored in TT dynamic storage instead.
													- if constant not defined: incoming SMS are retrieved in SIM. It's slower since incoming
													messages are stored in SIM but more safe. */

#define kTEL_OEM_IGNORE_AND_REJECT_NON_VOICE_INCOMING_CALLS	/* This compilation directive activates the automatic reject of the non voice incoming calls, 
															and prevents the broadcast of corresponding notifications.
															*/

/*@}*/



/*	----------------------------------------------------------------------	*/
/* 	new notification IDs
	----------------------------------------------------------------------	*/

/** 
 * @name Enum
 *
 */
/*@{*/

enum TTOemNotificationTypeEnum
{
	kTelOemSpcLaunchCmdCallActive = kTelSpcLaunchCmdCallConnect, /* specific */
	kTelOemSpcLaunchCmdCallDialling = kTelLastLaunchCode,
	kTelOemSpcLaunchCmdCallAlerting,
	kTelOemSpcLaunchCmdCallWaiting,
	kTelOemSpcLaunchCmdCallHold,
	kTelOemSpcLaunchCmdCallOnHoldReleased,
	kTelOemSpcLaunchCmdCallRejected,
	
	kTelOemNwkLaunchCmdRegistrationChange,

	/* It appears when the network signal quality has significantly changed */
	kTelOemNwkLaunchCmdSignalQualityChange,
	
	/* USSD unsolicited result code (USSD response from the network/network 
	initiated operation) */
	kTelOemUSSDLaunchCmdResultCode,
	
	kTelOemGPRSLaunchCmdEventReporting,			/* GPRS Event Reporting */
	kTelOemGPRSLaunchCmdNwkRegistration,		/* GPRS Network Registration Event */
	
	kTelOemGPRSLaunchCmdSessionStarted, 		/* GPRS session has started */
	kTelOemGPRSLaunchCmdSessionClosed,			/* GPRS session was closed */
	
	kTelOemPhbLaunchCmdServiceState,			/* Phonebook service state 0 means BUSY or NOT READY, 1 means READY */
	kTelOemSupLaunchCmdUnknown,
	
	/* Project Specific */
	kTelOemLaunchCmdTelephonyIsRunning,
	kTelOemLaunchCmdTelephonyIsStopping,
	kTelOemLaunchCmdPhoneIsOn,					/* TelNotificationType::notificationData: true: new event; false: read state */
	kTelOemLaunchCmdPhoneIsOff,
	kTelOemLaunchCmdTelephonyRestrictedAccess,	/* TelNotificationType::notificationData: true: new event; false: read state */
	kTelOemLaunchCmdTelephonyGrantedAccess,		/* TelNotificationType::notificationData: true: new event; false: read state */
												/*kTelOemLaunchCmdChipsetInitialized, SMS, phonebook services are ready to be used */
												/* It can includes the SIM Application toolkit service if driver is present */
	kTelOemChipsetLaunchCmdExtButtonPressed,
	kTelOemChipsetLaunchCmdExtButtonReleased,
	kTelOemChipsetLaunchCmdExtButtonRepeated,
	kTelOemChipsetLaunchCmdPhoneButtonPressed,
	kTelOemChipsetLaunchCmdPhoneButtonReleased,
	kTelOemChipsetLaunchCmdEarBudPlugged,
	kTelOemChipsetLaunchCmdEarBudUnplugged,
	kTelOemChipsetLaunchCmdSimCardInserted,
	kTelOemChipsetLaunchCmdSimCardRemoved,

	/* SIM Application Toolkit */
	kTelOemSatLaunchCmdInitializationFinished,	/* TelNotificationType::notificationData: true: Profile Download supported; false: SIM card isn't SAT enabled */
	kTelOemSatLaunchCmdProactiveCommand,		/* TelNotificationType::notificationData: standard proactive command type */
												/* TelNotificationType::notificationData2: proactive command serial number */
	kTelOemSatLaunchCmdEndOfProactiveSession,
	kTelOemSatLaunchCmdCtrlBySimResponse,		/* TelNotificationType::notificationData: operation type */
												/* TelNotificationType::notificationData2: response serial number */
	kTelOemSatLaunchCmdInternalRequest,			/* TelNotificationType::notificationData: internal task code */
	
	kTelOemAsyncReplyMessage,

	kTelOemGSMLaunchCmdSessionStarted, 			/* GSM session has started */
	kTelOemGSMLaunchCmdSessionClosed,			/* GSM session was closed */
	
	kTelOemLaunchCmdSIMStatus,					/* Status 0 indicateds the SIM was removed */
	kTelOemLaunchCmdNITZUpdate,					/* Unsolicited NITZ update from network */
	kTelOemSpnFromCbm							/* Cell Broadcast notification for SPN update */
};

/*@}*/




/*	----------------------------------------------------------------------	*/
/* 	new notification priorities
	----------------------------------------------------------------------	*/
/** 
 * @name Defines
 *
 */
/*@{*/
	
#define kTelOemHardwareEventNotificationPriority	0  	/* higher priority */
#define kTelOemSatNotificationPriority				1	/* same as SMS */
#define kTelOemNwkNotificationPriority				1	/* same as SMS */
#define kTelOemSSDNotificationPriority				1	/* same as SMS */
#define kTelOemUSSDNotificationPriority				1	/* same as SMS */
#define kTelOemPhbNotificationPriority				1	
#define kTelOemSpnFromCbmNotificationPriority		1

/*@}*/

/*	----------------------------------------------------------------------	*/
/* 	Error codes: 
	new error format:   telOemErr<service><description>

 	Source Insight 3.1 MACROS: Thierry Langlais 2001 

 	Macros used to easily re number all following lines
	To use this MACRO, you have to be under Source Insight editor, 
	then just put the cursor at the begining of the line 
	that contain the macro name and run macro via 
	the menu "Run Macro". It this command is not yet defined,
	then do it via the Key or Menu assigments...

RenumberErrorLinesInHeaderFile()
stop
*/

/** 
 * @name Error codes
 * @brief new error format:   telOemErr<service><description>
 * Source Insight 3.1 MACROS: Thierry Langlais 2001 

 	Macros used to easily re number all following lines
	To use this MACRO, you have to be under Source Insight editor, 
	then just put the cursor at the begining of the line 
	that contain the macro name and run macro via 
	the menu "Run Macro". It this command is not yet defined,
	then do it via the Key or Menu assigments...


 */
/*@{*/


/*	----------------------------------------------------------------------	*/
#define telOemErrBase										telErrOperationNotAllowed			
#define telOemErrPhoneOff									(telOemErrBase + 1)
#define telOemErrLowBattery									(telOemErrBase + 2)
#define telOemErrDatabaseFull								(telOemErrBase + 3)

/* SIM card errors - 3GPP TS 11.11 */
#define telOemErrSimMemoryProblem							(telOemErrBase + 4)
#define telOemErrSimNoElementaryFileSelected				(telOemErrBase + 5)
#define telOemErrSimOutOfRange								(telOemErrBase + 6)
#define telOemErrSimFileNotFound							(telOemErrBase + 7)
#define telOemErrSimFileCommandInconsistency				(telOemErrBase + 8)
#define telOemErrSimNoChvInitialized						(telOemErrBase + 9)
#define telOemErrSimNoAccess								(telOemErrBase + 10)
#define telOemErrSimConflictWithChvStatus					(telOemErrBase + 11)
#define telOemErrSimConflictWithInvalidationStatus			(telOemErrBase + 12)
#define telOemErrSimAccessBlocked							(telOemErrBase + 13)
#define telOemErrSimUnableToIncrease						(telOemErrBase + 14)
#define telOemErrSimIncorrectParameter						(telOemErrBase + 15)
#define telOemErrSimUnknownInstruction						(telOemErrBase + 16)
#define telOemErrSimWrongInstructionClass					(telOemErrBase + 17)
#define telOemErrSimTechnicalProblem						(telOemErrBase + 18)
/* SIM card errors - Palm specific */
#define telOemErrSimNotAccessible							(telOemErrBase + 19)
#define telOemErrSimUnexpectedData							(telOemErrBase + 20)
#define telOemErrSimSecurity								(telOemErrBase + 21)
#define telOemErrSimNotEnoughSpace							(telOemErrBase + 22)
#define telOemErrSimGenericError							(telOemErrBase + 23)

/* UPGRADE_GPRS */
#define telOemErrGPRSIllegalMS								(telOemErrBase + 24)
#define telOemErrGPRSIllegalME								(telOemErrBase + 25)
#define telOemErrGPRSServicesNotAllowed						(telOemErrBase + 26)
#define telOemErrGPRSPLMNNotAllowed							(telOemErrBase + 27)
#define telOemErrGPRSLocationAreaNotAllowed					(telOemErrBase + 28)
#define telOemErrGPRSRoamingNotAllowedInThisLocationArea	(telOemErrBase + 29)
#define telOemErrGPRSServiceOptionNotSupported				(telOemErrBase + 30)
#define telOemErrGPRSRequestedServiceOptionNotSubscribed	(telOemErrBase + 31)
#define telOemErrGPRSServiceOptionTemporarilyOutOfOrder 	(telOemErrBase + 32)
#define telOemErrGPRSUnspecifiedError						(telOemErrBase + 33)
#define telOemErrGPRSPDPAuthenticationFailure				(telOemErrBase + 34)
#define telOemErrGPRSInvalidMobileClass						(telOemErrBase + 35)

/* MUX */
#define telOemErrMUXAlreadyActive							(telOemErrBase + 36)

/* POWER */
#define telOemErrPowerHwrPhoneOnFailed						(telOemErrBase + 37)
#define telOemErrPowerHwrPhoneOffFailed						(telOemErrBase + 38)

/* UPGRADE_VGIII */
#define telOemErrCBErrorUnknown								(telOemErrBase + 39)
#define telOemErrNetworkNotAllowedEmergencyCallsOnly		(telOemErrBase + 40)

/* UPGRADE from the 07.07 GSM standard */
#define telOemErrNetworkPersonalisationPINRequired			(telOemErrBase + 41)
#define telOemErrNetworkPersonalisationPUKRequired			(telOemErrBase + 42)
#define telOemErrNetworkSubsetPersonalisationPINRequired	(telOemErrBase + 43)
#define telOemErrNetworkSubsetPersonalisationPUKRequired	(telOemErrBase + 44)
#define telOemErrNetworkProviderPersonalisationPINRequired	(telOemErrBase + 45) // Bad error name, the following is the good one.
#define telOemErrServiceProviderPersonalisationPINRequired	(telOemErrBase + 45)
#define telOemErrServiceProviderPersonalisationPUKRequired	(telOemErrBase + 46)
#define telOemErrCorporatePersonalisationPINRequired		(telOemErrBase + 47)
#define telOemErrCorporatePersonalisationPUKRequired		(telOemErrBase + 48)
#define telOemErrFixedNumberDialingRestriction				(telOemErrBase + 49)
#define telOemErrNotOpen									(telOemErrBase + 50)
#define telOemErrSmsNormalRelease							(telOemErrBase + 51)
#define telOemErrGPRSOperatorResourceInsufficient			(telOemErrBase + 52)
#define telOemErrGPRSUnknowOrMissingAPN						(telOemErrBase + 53)
#define telOemErrGPRSPdpActivationRejectedGGSN				(telOemErrBase + 54)
#define telOemErrGPRSPdpActivationRejectedUnspecified		(telOemErrBase + 55)
#define telOemErrGPRSPdpDeactivationRegular					(telOemErrBase + 56)
#define telOemErrGPRSPdpDeactivationNetworkFailure			(telOemErrBase + 57)
#define telOemErrPhoneToVeryFirstSIMPINRequired				(telOemErrBase + 58)
#define telOemErrPhoneToVeryFirstSIMPUKRequired				(telOemErrBase + 59)

			
// Hey!  Keep this (telOemErrLastError) below updated if you add more errors above this line.
#define telOemErrLastError	telOemErrPhoneToVeryFirstSIMPUKRequired

// IMPORTANT!!!!!!
// IF YOU ADD A NEW ERROR CODE, PLEASE LET RYAN KNOW SO THAT THE ERROR STRING
// LIST IN MOBILE SERVICES IS KEPT IN SYNC!

/*@}*/


/*	----------------------------------------------------------------------	*/
/* 	Constants
	----------------------------------------------------------------------	*/

/** 
 * @name Constants
 *
 */
/*@{*/


/* 
These constants allow an easier process of press duration for these buttons 
*/
#define kPhoneButtonShortPress				1
#define kPhoneButtonLongPress				2
#define kPhoneEarBudButtonShortPress		3
#define kPhoneEarBudButtonLongPress			4
#define kPhoneEarBudButtonRepeat          	5

/* 	Information service --------------------------------------------------	*/
/*  Extended code */
/* international mobile subscriber identity */
#define kTelOemInfInternationalMobileSubscriberId	3	

/* product serial number identification */
#define kTelOemInfPhoneSerialNumber					4	

/*	Calls duration --------------------------------------------------------	*/

/* 	Phonebook service ----------------------------------------------------	*/
/*
Here is a reminder of phonebooks that already exist in telephonyMgr.h file.

#define kTelPhbFixedPhonebook   			0	
#define kTelPhbSimPhonebook    				1	
#define kTelPhbPhonePhonebook     			2
#define kTelPhbLastDialedPhonebook   		3
#define kTelPhbSimAndPhonePhonebook   		4
#define kTelPhbAdaptorPhonebook    			5
#define kTelPhbFirstOemPhonebook    		6
*/

/*	New phonebooks */
#define kTelPhbMissed						6	
#define kTelPhbEmergency		   			7
#define kTelPhbOwn							8
#define kTelPhbReceived			    		9
#define kTelPhbDialed						10
#define kTelPhbSimBarrDialing				11
#define kTelPhbSimSrvNumber					12
#define kTelPhbSimAbbrAndFixed				13
#define kTelPhbLastOemPhonebook				13

/* 	Power service --------------------------------------------------------	*/
/*  Functionalities options */
#define kTelOemPowFunctionalityMini			0	/* minimum functionality */
#define kTelOemPowFunctionalityMaxi			1	/* full functionality */
#define kTelOemPowTransmitterDisabled		2	/* disable phone transmit RF circuits only */
#define kTelOemPowReceiverDisabled			3	/* disable phone receive RF circuits only */
#define kTelOemPowBothDisabled				4	/* disable phone both transmit and receive RF circuits */
	
/* 	Security service -----------------------------------------------------	*/
/*  Extended code */
#define kTelOemStyAOCodeId	kTelStyFirstOemCodeId		/* "AO" - BAOC (Barr All Outgoing Calls) */
#define kTelOemStyOICodeId	kTelStyFirstOemCodeId + 1	/* "OI" - BOIC (Barr Outgoing International Calls) */
#define kTelOemStyOXCodeId	kTelStyFirstOemCodeId + 2	/* "OX" - BOIC-exHC (Barr Outgoing International Calls except to Home Country) */
#define kTelOemStyAICodeId	kTelStyFirstOemCodeId + 3	/* "AI" - BAIC (Barr All Incoming Calls) */
#define kTelOemStyIRCodeId	kTelStyFirstOemCodeId + 4	/* "IR" - BIC-Roam (Barr Incoming Calls when Roaming outside the home country) */
#define kTelOemStyABCodeId	kTelStyFirstOemCodeId + 5	/* "AB" - All Barring services */
#define kTelOemStyAGCodeId	kTelStyFirstOemCodeId + 6	/* "AG" - All outGoing barring services */
#define kTelOemStyACCodeId	kTelStyFirstOemCodeId + 7	/* "AC" - All inComing barring services */


#define kTelOemStyPhoneToVeryFirstSimPINCodeId				kTelStyFirstOemCodeId + 16 	/* ME is waiting phone-to-very first SIM card password to be given */
#define kTelOemStyPhoneToVeryFirstSimPUKCodeId				kTelStyFirstOemCodeId + 17 	/* ME is waiting phone-to-very first SIM card unblocking password to be given */
#define kTelOemStyNetworkPersonalisationPINCodeId			kTelStyFirstOemCodeId + 18 	/* ME is waiting network personalisation password to be given*/
#define kTelOemStyNetworkPersonalisationPUKCodeId			kTelStyFirstOemCodeId + 19 	/* ME is waiting network personalisation unblocking password to be given */
#define kTelOemStyNetworkSubsetPersonalisationPINCodeId 	kTelStyFirstOemCodeId + 20 	/* ME is waiting network subset personalisation password to be given */
#define kTelOemStyNetworkSubsetPersonalisationPUKCodeId 	kTelStyFirstOemCodeId + 21 	/* ME is waiting network subset personalisation unblocking password to be given */
#define kTelOemStyServiceProviderPersonalisationPINCodeId	kTelStyFirstOemCodeId + 22 	/* ME is waiting service provider personalisation password to be given */
#define kTelOemStyServiceProviderPersonalisationPUKCodeId	kTelStyFirstOemCodeId + 23 	/* ME is waiting service provider personalisation unblocking password to be given */
#define kTelOemStyCorporatePersonalisationPINCodeId			kTelStyFirstOemCodeId + 24 	/* ME is waiting corporate personalisation password to be given */
#define kTelOemStyCorporatePersonalisationPUKCodeId			kTelStyFirstOemCodeId + 25 	/* ME is waiting corporate personalisation unblocking password to be given */

/*	Locks */
#define kTelOemStyLockPhoneToSim			1	/* "PS" - PH-SIM PIN ME    phone to SIM card password */
#define kTelOemStyLockFirstSim				2	/* "PF" - PH-FSIM PIN ME   phone to very first SIM card password */
#define kTelOemStyLockSim					3	/* "SC" - SIM PIN          Sim card password */

#define kTelOemStyLockNetwork				4	/* "PN" - Network Personalisation (refer GSM 02.22 ) */
#define kTelOemStyLockNetworkSubset			5	/* "PU" - network sUbset Personalisation (refer GSM 02.22 */
#define kTelOemStyLockServiceProvider		6	/* "PP" - service Provider Personalisation (refer GSM 02.22) */
#define kTelOemStyLockCorporate				7	/* "PC" - Corporate Personalisation (refer GSM 02.22) */

#define kTelOemStyLockAO					8	/* "AO" - BAOC (Barr All Outgoing Calls) (refer GSM 02.88*/
#define kTelOemStyLockOI					9	/* "OI" - BOIC (Barr Outgoing International Calls) (refer GSM 02.88) */
#define kTelOemStyLockOX					10	/* "OX" - BOIC-exHC (Barr Outgoing International Calls except to Home Country) (refer GSM 02.88) */
#define kTelOemStyLockAI					11	/* "AI" - BAIC (Barr All Incoming Calls) (refer GSM 02.88) */
#define kTelOemStyLockIR					12	/* "IR" - BIC-Roam (Barr Incoming Calls when Roaming outside the home country) (refer GSM 02.88) */
#define kTelOemStyLockAB					13	/* "AB" - All Barring services (refer GSM 02.30) (applicable only for <mode>=0) */
#define kTelOemStyLockAG					14	/* "AG" - All outGoing barring services (refer GSM 02.30) (applicable only for <mode>=0) */
#define kTelOemStyLockAC					15	/* "AC" - All inComing barring services (refer GSM 02.30) (applicable only for <mode>=0) */
#define kTelOemStyLockFD					16	/* "FD" - SIM fixed dialling memory feature (if PIN2 authentication has not been done during
															 the current session, PIN2 is required as <passwd>) */
#define kTelOemStyLockME					17	/* "ME" - ME phonebook */

#define kTelOemStyLockAll					18	/* Six Kinds of SIM Lock functions simultaneously: "PS", "PF", "PN", "PU","PP" and "PC" */


/*  Configuration --------------------------------------------------------	*/
/*	Call barring, call waiting and call diverting types */
#define	kTelOemCfgCallTypeVoice					1	/* voice (telephony) 			*/
#define	kTelOemCfgCallTypeData					2 	/* data 						*/
#define	kTelOemCfgCallTypeFax					4 	/* fax (facsimile services)		*/
#define	kTelOemCfgCallTypeSMS					8 	/* SMS (short message service) 	*/

#define	kTelOemCfgCallTypeDataCircuitSync		16	/* data circuit sync */
#define	kTelOemCfgCallTypeDataCircuitAsync		32	/* data circuit async */
#define	kTelOemCfgCallTypeDedicatedPacketAccess 64  /* dedicated packet access */
#define	kTelOemCfgCallTypeDedicatedPADAccess  	128 /* dedicated PAD access */

#define	kTelOemCfgCallTypeLastClass				7	/* combination of voice, data, and fax */


/*	Barring type */
#define	kTelOemCfgBarrAOC					1	/* Barr All Outgoing Calls 		*/
#define	kTelOemCfgBarrOIC 					2	/* Barr Outgoing International Calls */
#define	kTelOemCfgBarrOX					3	/* Barr Outgoing International Calls except to Home Country */
#define	kTelOemCfgBarrAIC 					4	/* Barr All Incoming Calls 		*/
#define	kTelOemCfgBarrICRoam				5	/* Barr Incoming Calls when Roaming outside the home country */
#define	kTelOemCfgBarrNTa					6	/* Barr incoming calls from numbers Not stored to TA memory  */
#define	kTelOemCfgBarrNMe					7	/* Barr incoming calls from numbers Not stored to ME memory  */
#define	kTelOemCfgBarrNSim					8	/* Barr incoming calls from numbers Not stored to SIM memory */
#define	kTelOemCfgBarrNAny					9	/* Barr incoming calls from numbers Not stored in Any memory */
#define	kTelOemCfgBarrAll					10	/* All Barring services (applicable only for deactivation) */
#define	kTelOemCfgBarrAllOutgoing			11	/* All outgoing barring services (applicable only for deactivation) */
#define	kTelOemCfgBarrAllIncoming			12	/* All inComing barring services (applicable only for deactivation) */
#define	kTelOemCfgBarrSimFD					13	/* SIM fixed dialling memory feature */

/* Diverting reason	*/
#define	kTelOemCfgDivUnconditional			0	/* Call forwarding unconditional */
#define	kTelOemCfgDivMobileBusy				1	/* Call forwarding on mobile subscriber busy */
#define	kTelOemCfgDivNoReply				2	/* Call forwarding on no reply */
#define	kTelOemCfgDivNotReachable			3	/* Call forwarding on mobile subscriber not reachable */
#define	kTelOemCfgDivAllCall				4	/* Call forwarding to be defined */
#define	kTelOemCfgDivAllConditionalCall		5	/* Call forwarding to be defined */
#define	kTelOemCfgDivLastReason				kTelOemCfgDivAllConditionalCall

/* Diverting mode */
#define	kTelOemCfgDivDisable				0	/* Disable constant for call diverting service */
#define	kTelOemCfgDivEnable					1	/* Enable constant for call diverting service */
#define	kTelOemCfgDivRegistration			3	/* Registration constant for call diverting service */
#define	kTelOemCfgDivErasure				4	/* Erasure constant for call diverting service */
#define	kTelOemCfgDivLastMode				kTelOemCfgDivErasure

#define	kTelOemCfgDivStatusNotActive		0	/* Status: not active for this diverting mode */
#define	kTelOemCfgDivStatusActive			1	/* Status: active for this diverting mode */

/* Call Waiting */
#define	kTelOemCfgCallWaitingDeactivate		0	/* Deactivate constant for call waiting service */
#define	kTelOemCfgCallWaitingActivate		1	/* Activate constant for call waiting service */

/* Voice mail modes constants */
#define	kTelOemCfgVoiceMailModeDeactivate	0	/* Deactivate constant for voice mail service */
#define	kTelOemCfgVoiceMailModeActivate		1	/* Activate constant for voice mail service   */

/* Call line identification restriction (parameter sets the adjustment for outgoing calls)*/
#define	kTelOemCfgCallLineIdRestrictionDefault		0	/* presentation indicator is used according to the subscription of the CLIR service */
#define	kTelOemCfgCallLineIdRestrictionInvocated	1	/* the subscriber number is hidden to the called party */
#define	kTelOemCfgCallLineIdRestrictionSuppressed	2	/* the subscriber number is showed to the called party */

/* Call line identification restriction (parameter shows the subscriber CLIR service status in the network)*/
#define	kTelOemCfgCallLineIdRestrictionNotProvisioned		0 /* CLIR not provisioned */
#define	kTelOemCfgCallLineIdRestrictionProvisioned			1 /* CLIR provisioned in permanent mode */
#define	kTelOemCfgCallLineIdRestrictionUnknown				2 /* unknown (e.g. no network, etc) */
#define	kTelOemCfgCallLineIdRestrictionTemporaryRestricted  3 /* CLIR temporary mode presentation restricted */
#define	kTelOemCfgCallLineIdRestrictionTemporaryAllowed 	4 /* CLIR temporary mode presentation allowed */

/* Call line identification presentation (parameter indicates the result code presentation status */
#define	kTelOemCfgCallLineIdPresentationDisabled		0	/* result code presentation is enabled */
#define	kTelOemCfgCallLineIdPresentationEnabled			1	/* result code presentation is disabled */

/* Call line identification presentation (parameter indicates the subscriber CLIP service status in the network) */
#define	kTelOemCfgCallLineIdPresentationNotProvisioned		0	/* CLIP not provisioned */
#define	kTelOemCfgCallLineIdPresentationProvisioned			1	/* CLIP provisioned in permanent mode */
#define	kTelOemCfgCallLineIdPresentationUnknown				2	/* unknown (e.g. no network, etc) */

/* Bearer service:<speed>*/
#define	kTelOemCfgBearerDataRateAuto			0 /* autobauding (automatic selection of the speed; 
													this setting is possible in case of 3.1 kHz modem	
													and non-transparent service) */
#define	kTelOemCfgBearerDataRate300bpsV21		1 /* 300 bps (V.21)*/
#define	kTelOemCfgBearerDataRate1200bpsV22		2 /* 1200 bps (V.22)*/
#define	kTelOemCfgBearerDataRate1200_75bpsV23	3 /* 1200/75 bps (V.23)*/
#define	kTelOemCfgBearerDataRate2400bpsV22bis	4 /* 2400 bps (V.22bis)*/
#define	kTelOemCfgBearerDataRate2400bpsV26ter	5 /* 2400 bps (V.26ter)*/
#define	kTelOemCfgBearerDataRate4800bpsV32		6 /* 4800 bps (V.32)*/
#define	kTelOemCfgBearerDataRate9600bpsV32		7 /* 9600 bps (V.32)*/
#define	kTelOemCfgBearerDataRate9600bpsV34		12 /* 9600 bps (V.34)*/
#define	kTelOemCfgBearerDataRate14400bpsV34		14 /* 14400 bps (V.34)*/
#define	kTelOemCfgBearerDataRate19200bpsV34		15 /* 19200 bps (V.34)*/
#define	kTelOemCfgBearerDataRate28800bpsV34		16 /* 28800 bps (V.34)*/
#define	kTelOemCfgBearerDataRate1200bpsV120		34 /* 1200 bps (V.120)*/
#define	kTelOemCfgBearerDataRate2400bpsV120		36 /* 2400 bps (V.120)*/
#define	kTelOemCfgBearerDataRate4800bpsV120		38 /* 4800 bps (V.120)*/
#define	kTelOemCfgBearerDataRate9600bpsV120		39 /* 9600 bps (V.120)*/
#define	kTelOemCfgBearerDataRate14400bpsV120	43 /* 14400 bps (V.120)*/
#define	kTelOemCfgBearerDataRate19200bpsV120	47 /* 19200 bps (V.120)*/
#define	kTelOemCfgBearerDataRate28800bpsV120	48 /* 28800 bps (V.120)*/
#define	kTelOemCfgBearerDataRate38400bpsV120	49 /* 38400 bps (V.120)*/
#define	kTelOemCfgBearerDataRate48000bpsV120	50 /* 48000 bps (V.120)*/
#define	kTelOemCfgBearerDataRate56000bpsV120	51 /* 56000 bps (V.120)*/
#define	kTelOemCfgBearerDataRate300bpsV110		65 /* 300 bps (V.110)*/
#define	kTelOemCfgBearerDataRate1200bpsV110		66 /* 1200 bps (V.110)*/
#define	kTelOemCfgBearerDataRate2400bpsV110		68 /* 2400 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate4800bpsV110		70 /* 4800 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate9600bpsV110		71 /* 9600 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate14400bpsV110	75 /* 14400 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate19200bpsV110	79 /* 19200 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate28800bpsV110	80 /* 28800 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate38400bpsV110	81 /* 38400 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate48000bpsV110	82 /* 48000 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate56000bpsV110	83 /* 56000 bps (V.110 or X.31 flag stuffing)*/
#define	kTelOemCfgBearerDataRate56000bpsTrans	115 /* 56000 bps (bit transparent)*/
#define	kTelOemCfgBearerDataRate64000bpsTrans	116 /* 64000 bps (bit transparent)*/

/* Bearer service:	<service> */
#define	kTelOemCfgBearerDataAsynchronousUDI				0 /* data circuit asynchronous (UDI or 3.1 kHz modem) */
#define	kTelOemCfgBearerDataSynchronousUDI				1 /* data circuit synchronous (UDI or 3.1 kHz modem) */
#define	kTelOemCfgBearerPADAccessAsynchronousUDI		2 /* PAD Access (asynchronous) (UDI) */
#define	kTelOemCfgBearerPacketAccessSynchronousUDI		3 /* Packet Access (synchronous) (UDI) */
#define	kTelOemCfgBearerDataAsynchronousRDI				4 /* data circuit asynchronous (RDI) */
#define	kTelOemCfgBearerDataSynchronousRDI				5 /* data circuit synchronous (RDI) */
#define	kTelOemCfgBearerPADAccessAsynchronousRDI		6 /* PAD Access (asynchronous) (RDI) */
#define	kTelOemCfgBearerPacketAccessSynchronousRDI		7 /* Packet Access (synchronous) (RDI) */

/* Bearer service:	<connection> */
#define	kTelOemCfgBearerConnectionTransparent					0 /* transparent */
#define	kTelOemCfgBearerConnectionNonTransparent				1 /* non-transparent */
#define	kTelOemCfgBearerConnectionBothTransparentPreferred		2 /* both, transparent preferred */
#define	kTelOemCfgBearerConnectionBothNonTransparentPreferred	3 /* both, non-transparent preferred */

/* Network -----------------------------------------------------------------*/
#define	kTelOemNwkFirstFreeLocation			0xFFFF	/* this is an index facility */
#define	kTelOemNwkNotifNotRegistered		0		/* not registered, mobile is not currently searching a new operator to register to */
#define	kTelOemNwkNotifRegisteredHome		1		/* registered, home network */
#define	kTelOemNwkNotifSearchToRegister		2		/* not registered, but mobile is currently searching a new operator to register to*/
#define	kTelOemNwkNotifRegistrationDenied	3		/* registration denied */
#define	kTelOemNwkNotifUnknown				4		/* unknown */
#define	kTelOemNwkNotifRegisteredRoaming	5		/* registered, roaming */

#define	kTelOemNwkServiceStatusFull 		0		/* full network service */
#define	kTelOemNwkServiceStatusLimited		1		/* limited network service */
#define	kTelOemNwkServiceStatusNone			2		/* no network service */

/* Signal strength value */
#define	kTelOemNwkSavedSignalLevel			0xFF 	/* This constant can be used as an input parameter of the 'TelNwkGetSignalLevel(...)'
													   API to retrieve the signal level value stored by the Telephony Task and updated 
													   with the information given in the 'kTelOemNwkLaunchCmdSignalQualityChange'
													   unsolicited notifications. */

/* Alert preferences */
/* Incoming event type */
#define	kAlertIncomingDefault				1		/* all notification or incoming call not specifically defined */
#define	kAlertIncomingCall					2		/* all incoming calls */
//#define	kAlertIncomingCallWaiting		3		/* call waiting */
#define	kAlertIncomingSMS					3		/* all SMS notifications */

/* Alert command options */
#define	kAlertCmdRing						1		/* set or get the ring tone and volume */
#define	kAlertCmdVibrator					2		/* set or get the vibrator */
#define	kAlertCmdBehaviour					3		/* set or get the alert behaviour */

/* Vibrator options */
#define	kAlertVibratorOff					0 		/* the vibrator will be activated on an incoming event */
#define	kAlertVibratorOn					1		/* the vibrator won't be activated on an incoming event */

/* Alert behaviour */
#define	kAlertNormal						1		/* play defined tone and vibrate */
#define	kAlertIncreaseVolume				2		/* the tone volume increase from low to high during alert */
#define	kAlertVibrateThenRing				3 		/* vibrate then play the defined tone */


/* Speech ----------------------------------------------------------------	*/
/* Call close */
#define kTelOemSpcCallCloseCurrent			250	/* this special line ID closes the current call */
#define kTelOemSpcCallCloseAllHold			251	/* this special line ID releases all calls on hold */
#define kTelOemSpcCallCloseAllActive		252	/* this special line ID releases all active calls */
#define kTelOemSpcCallCloseAll				253	/* this special line ID closes all call */

/* Call Information state */
#define	kTelOemSpcCallInfoStateActive		0	/* normal case when speech is possible */
#define	kTelOemSpcCallInfoStateHeld			1	/* call is on hold */
#define	kTelOemSpcCallInfoStateDialing		2	/* mobile originated call (before ring)*/
#define	kTelOemSpcCallInfoStateAlerting		3	/* mobile originated call (during ring)*/
#define	kTelOemSpcCallInfoStateIncoming		4	/* mobile terminated call (first call) */
#define	kTelOemSpcCallInfoStateWaiting		5	/* mobile terminated call (when there is at least one call) */
#define	kTelOemSpcCallInfoStateReleased		6	/* a call has been released */

/* Call Information type */
#define	kTelOemSpcCallInfoTypeVoice			0	/* It is a voice call type */
#define	kTelOemSpcCallInfoTypeData			1	/* It is a data call type */



/* GSM ----------------------------------------------------------------	*/

/* GSM Speed */
#define kTelOemGsmSpeedAutobauding 		0		/* autobauding */
#define kTelOemGsmSpeedV21				1		/* V.21 - 300 bps*/
#define kTelOemGsmSpeedV22				2		/* V.22 - 1200 bps*/
#define kTelOemGsmSpeedV23				3		/* V.23 - 1200/75 bps*/
#define kTelOemGsmSpeedV22bis			4		/* V.22 bis - 2400 bps*/
#define kTelOemGsmSpeedV26ter			5		/* V.26 ter - 2400 bps*/
#define kTelOemGsmSpeedV32_4800			6		/* V.32 - 4800 bps*/
#define kTelOemGsmSpeedV32_9600			7		/* V.32 - 9600 bps*/
#define kTelOemGsmSpeedV34_9600			12		/* V.34 - 9600 bps*/
#define kTelOemGsmSpeedV34_14400		14		/* V.34 - 14400 bps*/
#define kTelOemGsmSpeedV110_300			65 		/* V.110 or X.31 - flag stuffing */
#define kTelOemGsmSpeedV110_1200		66		/* V.110 or X.31 - flag stuffing */
#define kTelOemGsmSpeedV110_2400		68		/* V.110 or X.31 - flag stuffing */
#define kTelOemGsmSpeedV110_4800		70		/* V.110 or X.31 - flag stuffing */
#define kTelOemGsmSpeedV110_9600		71		/* V.110 or X.31 - flag stuffing */
#define kTelOemGsmSpeedV110_14400		75		/* V.110 or X.31 - flag stuffing */

/* GSM Bearer Service */
#define kTelOemGsmAsynchronousModem		0		
#define kTelOemGsmAsynchronousPADAccess	2						

/* GSM Connection Element */
#define kTelOemGsmConnectionTransparentMode		0		
#define kTelOemGsmConnectionNonTransparentMode	1		



/* GPRS ----------------------------------------------------------------	*/

/* GPRS Packet Data Protocol  */
#define	kTelOemGprsPdpIP					0	/* Internet Protocol */
#define	kTelOemGprsPdpPPP					1	/* Point to Point Protocol */
#define	kTelOemGprsPdpOSPIH					2	/* Internet Hosted Octet Stream Protocol (IHOSP)*/

/* GPRS Compression */
#define	kTelOemGprsDataCompressionSetOff  	0	/* No data compression */
#define	kTelOemGprsDataCompressionSetOn  	1	/* V42 bis data compression */
#define	kTelOemGprsHdrCompressionSetOff  	0	/* No header compression*/
#define	kTelOemGprsHdrCompressionSetOn  	1	/* V42 bis header compression */

/* GPRS OSPIH Protocol */
#define	kTelOemGprsOSPIHProtocolUDP	  	0	/* UDP used over IP on GPRS OSPIH */
#define	kTelOemGprsOSPIHProtocolTCP	  	1	/* TCP used over IP on GPRS OSPIH */

/* GPRS Attachment */
#define	kTelOemGprsDetached	  			0	/* GPRS Detached */
#define	kTelOemGprsAttached			  	1	/* GPRS Attached */

/* GPRS PDP Activation State */
#define	kTelOemGprsPdpDeactivated		0	/* PDP Deactivated */
#define	kTelOemGprsPdpActivated			1	/* PDP Activated */

/* GPRS Network registration */
#define	kTelOemGprsNwkRegistrationDisable						0	/* Diasble notifications on GPRS network Registration status change */
#define	kTelOemGprsNwkRegistrationNwkEnable						1	/* Notifications on GPRS network Registration status change */
#define	kTelOemGprsNwkRegistrationCellEnable					2	/* Notifications on GPRS network and Cell change */
#define	kTelOemGprsNwkRegistrationCellSupportingStatusEnable	3	/* Notifications on GPRS network registration status change, cell change, GPRS supporting status of service cell change */

/* GPRS Network registration status */
#define	kTelOemGprsNwkRegistrationStatusNotRegistered	0	/* Not currently searching a new operator to register to */
#define	kTelOemGprsNwkRegistrationStatusRegistered		1	/* Home Network */
#define	kTelOemGprsNwkRegistrationStatusSearching		2	/* Not registered but currently searching a new operator to register to */
#define	kTelOemGprsNwkRegistrationStatusDenied			3	/* Registration denied */
#define	kTelOemGprsNwkRegistrationStatusUnknown			4	/* Unknown */
#define	kTelOemGprsNwkRegistrationStatusRoaming			5	/* Registered roaming */

/* GPRS SMS service */
#define	kTelOemGprsSmsGprsOnly			0	/* SMS Over GPRS only */
#define	kTelOemGprsSmsGsmOnly			1	/* SMS Over GSM only */
#define	kTelOemGprsSmsGprsPreferred		2	/* SMS Over GPRS preferred (use GSM if GPRS not available) */
#define	kTelOemGprsSmsGsmPreferred		3	/* SMS Over GSM preferred (use GPRS if GSM not available) */

/* GPRS Layer 2 Protocol */
#define	kTelOemGprsLayer2ProtocolPPP		0	/* PPP for a PDP such as IP */
#define	kTelOemGprsLayer2ProtocolNull		1	/* none, for PDP type OSP:IHOSS */

/* GPRS Quality of Service */
#define	kTelOemGprsQosPrecedenceDefault		0
#define	kTelOemGprsQosPrecedenceHigh		1
#define	kTelOemGprsQosPrecedenceNormal		2
#define	kTelOemGprsQosPrecedenceLow			3
#define	kTelOemGprsQosDelayDefault			0
#define	kTelOemGprsQosDelayClass1			1	/* <2 seconds for a 1024 SDU size */
#define	kTelOemGprsQosDelayClass2			2	/* <15 seconds for a 1024 SDU size */
#define	kTelOemGprsQosDelayClass3			3	/* <75 seconds for a 1024 SDU size */
#define	kTelOemGprsQosDelayBestEffort		4	/* Best Effort */
#define	kTelOemGprsQosReliabilityDefault	0	
#define	kTelOemGprsQosReliabilityClass1		1	/*  GTP Mode Acknowledged, LLC Mode Acknowledged, LLC Data Protected, RLC Block Acknowledged */
#define	kTelOemGprsQosReliabilityClass2		2	/*  GTP Mode Unacknowledged, LLC Mode Acknowledged, LLC Data Protected, RLC Block Acknowledged */
#define	kTelOemGprsQosReliabilityClass3		3	/*  GTP Mode Unacknowledged, LLC Mode Unacknowledged, LLC Data Protected, RLC Block Acknowledged */
#define	kTelOemGprsQosReliabilityClass4		4	/*  GTP Mode Unacknowledged, LLC Mode Unacknowledged, LLC Data Protected, RLC Block Unacknowledged */
#define	kTelOemGprsQosReliabilityClass5		5	/*  GTP Mode Unacknowledged, LLC Mode Unacknowledged, LLC Data Unprotected, RLC Block Unacknowledged */
#define	kTelOemGprsQosPeakDefault			0
#define	kTelOemGprsQosPeakClass1			1	/* Up to 1 000 octets/s (8 kbit/s) */
#define	kTelOemGprsQosPeakClass2			2	/* Up to 2 000 octets/s (16 kbit/s) */
#define	kTelOemGprsQosPeakClass3			3	/* Up to 4 000 octets/s (32 kbit/s) */
#define	kTelOemGprsQosPeakClass4			4	/* Up to 8 000 octets/s (64 kbit/s) */
#define	kTelOemGprsQosPeakClass5			5	/* Up to 16 000 octets/s (128 kbit/s) */
#define	kTelOemGprsQosPeakClass6			6	/* Up to 32 000 octets/s (256 kbit/s) */
#define	kTelOemGprsQosPeakClass7			7	/* Up to 64 000 octets/s (512 kbit/s) */
#define	kTelOemGprsQosPeakClass8			8	/* Up to 128 000 octets/s (1024 kbit/s) */
#define	kTelOemGprsQosPeakClass9			9	/* Up to 256 000 octets/s (2048 kbit/s) */
#define	kTelOemGprsQosMeanDefault			0
#define	kTelOemGprsQosMeanClass1			1	/* 100 octets/hour (~0.22 bit/s) */
#define	kTelOemGprsQosMeanClass2			2	/* 200 octets/hour (~0.44 bit/s) */
#define	kTelOemGprsQosMeanClass3			3	/* 500 octets/hour (~1.11 bit/s) */
#define	kTelOemGprsQosMeanClass4			4	/* 1 000 octets/hour (~2.2 bit/s) */
#define	kTelOemGprsQosMeanClass5			5	/* 2 000 octets/hour (~4.4 bit/s) */
#define	kTelOemGprsQosMeanClass6			6	/* 5 000 octets/hour (~11.1 bit/s) */
#define	kTelOemGprsQosMeanClass7			7	/* 10 000 octets/hour (~22 bit/s) */
#define	kTelOemGprsQosMeanClass8			8	/* 20 000 octets/hour (~44 bit/s) */
#define	kTelOemGprsQosMeanClass9			9	/* 50 000 octets/hour (~111 bit/s) */
#define	kTelOemGprsQosMeanClass10			10	/* 100 000 octets/hour (~0.22 kbit/s) */
#define	kTelOemGprsQosMeanClass11			11	/* 200 000 octets/hour (~0.44 kbit/s) */
#define	kTelOemGprsQosMeanClass12			12	/* 500 000 octets/hour (~1.11 kbit/s) */
#define	kTelOemGprsQosMeanClass13			13	/* 1 000 000 octets/hour (~2.2 kbit/s) */
#define	kTelOemGprsQosMeanClass14			14	/* 2 000 000 octets/hour (~4.4 kbit/s) */
#define	kTelOemGprsQosMeanClass15			15	/* 5 000 000 octets/hour (~11.1 kbit/s) */
#define	kTelOemGprsQosMeanClass16			16	/* 10 000 000 octets/hour (~22 kbit/s) */
#define	kTelOemGprsQosMeanClass17			17	/* 20 000 000 octets/hour (~44 kbit/s) */
#define	kTelOemGprsQosMeanClass18			18	/* 50 000 000 octets/hour (~111 kbit/s) */
#define	kTelOemGprsQosMeanClassBestEffort	31	/* Best Effort */

/* GPRS Event Reporting */
#define	kTelOemGprsEventReject					0	/* PDP context activation rejected */
#define	kTelOemGprsEventNwReact					1	/* PDP context activation reactivated by network */
#define	kTelOemGprsEventNwDeact					2	/* PDP context activation deactivated by network */
#define	kTelOemGprsEventMeDeact					3	/* PDP context activation deactivated by mobile */
#define	kTelOemGprsEventNwDetach				4	/* GPRS detached by network */
#define	kTelOemGprsEventMeDetach				5	/* GPRS detached by mobile */
#define	kTelOemGprsEventNwClass					6	/* MS Class changed by network */
#define	kTelOemGprsEventMeClass					7	/* MS Class changed by mobile */
#define	kTelOemGprsEventReportingDisabledMode	0	/* No event reporting forwarded */
#define	kTelOemGprsEventReportingEnabledMode	1	/* Event reporting forwarded if link OK */
#define	kTelOemGprsEventReportingBufferedMode	2	/* Event reporting forwarded if link OK or buffered and then forwared when link is OK again */
#define	kTelOemGprsEventReportingClearBuffer	0	/* Mobile buffer of unsolicited result code is cleared when kTelOemGprsEventReportingEnabledMode or kTelOemGprsEventReportingBufferedMode is chosen */
#define	kTelOemGprsEventReportingFlushBuffer	1	/* Mobile buffer of unsolicited result code is flushed to Telephony when kTelOemGprsEventReportingEnabledMode or kTelOemGprsEventReportingBufferedMode is chosen */

/* USSD */
/* Data coding scheme (UInt8) */
#define kTelUssd8BitsEncoding		0
#define kTelUssdBitsASCIIEncoding	1	/* ANSI X3.4 */
#define kTelUssdIA5Encoding			2	/* CCITTT T.50 */
#define kTelUssdIS91Encoding		3	/* TIA/EIA/IS-91 section 3.7.1 */
#define kTelUssdUCS2Encoding 		4	/* Only supported by GSM */
#define kTelUssdDefaultGSMEncoding	5	/* Only supported by GSM */

/* Commands identifiers */
#define	kTelOemUssdCmdDisableUnsolicitedResultCodePresentation	0
#define	kTelOemUssdCmdEnableUnsolicitedResultCodePresentation	1
#define	kTelOemUssdCmdCancelSession								2

/* Result Codes */
#define	kTelOemUssdCmdResultCodeNoFurtherUserActionRequired		0
#define	kTelOemUssdCmdResultCodeFurtherUserActionRequired		1
#define	kTelOemUssdCmdResultCodeUSSDTerminatedByNetwork			2
#define	kTelOemUssdCmdResultCodeOtherLocalClientHasResponded	3
#define	kTelOemUssdCmdResultCodeOperationNotSupported			4
#define	kTelOemUssdCmdResultCodeNetworkTimeOut					5

/* Alert */
#define	kTelOemAlertSilenceRinger			0x01
#define	kTelOemAlertSilenceVibrator			0x02
#define	kTelOemAlertSilenceAll				(kTelOemAlertSilenceRinger | kTelOemAlertSilenceVibrator)		

/* Voice Recognition ------------------------------------------------------	*/
#define	kTelOemAvrCmdUserPromptRecogniseDigitDialingStart		1
#define	kTelOemAvrCmdUserPromptRecogniseDigitDialingRepeat		2
#define	kTelOemAvrCmdUserPromptRecogniseDigitDialingFailed		3
#define	kTelOemAvrCmdUserPromptRecogniseDigitDialingVerify		4
#define	kTelOemAvrCmdUserPromptRecogniseDigitDialingNotTrained	5
#define	kTelOemAvrCmdUserPromptTrainingDigitDialingStart		6
#define	kTelOemAvrCmdUserPromptTrainingDigitDialingNumbers		7
#define	kTelOemAvrCmdUserPromptTrainingDigitDialingRepeat		8
#define	kTelOemAvrCmdUserPromptTrainingDigitDialingMore			9
#define	kTelOemAvrCmdUserPromptTrainingDigitDialingSuccess		10
#define	kTelOemAvrCmdUserPromptTrainingDigitDialingFailed		11
#define	kTelOemAvrCmdUserPromptRecogniseNameDialingStart		12
#define	kTelOemAvrCmdUserPromptRecogniseNameDialingRepeat		13
#define	kTelOemAvrCmdUserPromptRecogniseNameDialingFailed		14
#define	kTelOemAvrCmdUserPromptRecogniseNameDialingVerify		15
#define	kTelOemAvrCmdUserPromptRecogniseNameDialingNotTrained	16
#define	kTelOemAvrCmdUserPromptTrainingNameDialingStart			17
#define	kTelOemAvrCmdUserPromptTrainingNameDialingRepeat		18
#define	kTelOemAvrCmdUserPromptTrainingNameDialingSimilar		19
#define	kTelOemAvrCmdUserPromptTrainingNameDialingSimilarFail	20
#define	kTelOemAvrCmdUserPromptTrainingNameDialingDifferent		21
#define	kTelOemAvrCmdUserPromptTrainingNameDialingDifferentFail	22
#define	kTelOemAvrCmdUserPromptTrainingNameDialingSuccess		23
#define	kTelOemAvrCmdUserPromptTrainingNameDialingFailed		24

/* SIM card --------------------------------------------------------------	*/
/* Elementary File reading mode */
#define kTelOemSimReadFileModeInfo			0	/* get information about the Elementary File */
#define kTelOemSimReadFileModeRead			1	/* retrieve the entire Elementary File body */
#define kTelOemSimReadFileModePart			2	/* retrieve a part of the Elementary File body */
#define kTelOemSimReadFileModeRecord		3	/* retrieve a record of the Elementary File */

/* Elementary File structure */
#define kTelOemSimFileStructureTransparent	0x00	/* Transparent Elementary File (binary body) - Value from ETSI GSM 11.11 */
#define kTelOemSimFileStructureLinear		0x01	/* Linear Fixed Elementary File (record table) - Value from ETSI GSM 11.11 */
#define kTelOemSimFileStructureCyclic		0x03	/* Cyclic Elementary File (record table with cyclic overwriting) - Value from ETSI GSM 11.11 */

/* Text formats */
#define kTelOemSimTextFormatPlatform		1	/* Palm OS format (multibyte) */
#define kTelOemSimTextFormatGSM7Bits		2	/* refer to 3GPP TS 03.38 */
#define kTelOemSimTextFormatGSM7BitsPacked	3	/* refer to 3GPP TS 03.38 §6.1.2.1 */
#define kTelOemSimTextFormatUCS2			4	/* refer to ISO 2DIS 10646 */
#define kTelOemSimTextFormatDCSBased		5	/* refer to 3GPP TS 11.14 §12.15 */
#define kTelOemSimTextFormatAlphaIdBased	6	/* refer to 3GPP TS 11.11 §10.5.1 */

/* SIM Application Toolkit -----------------------------------------------	*/
/* Proactive command identifiers - Standard codes */
#define kTelOemSatCmdRefresh				0x01
#define kTelOemSatCmdSetUpEventList			0x05
#define kTelOemSatCmdSetUpCall				0x10
#define kTelOemSatCmdSendSS					0x11
#define kTelOemSatCmdSendUSSD				0x12
#define kTelOemSatCmdSendShortMessage		0x13
#define kTelOemSatCmdSendDTMF				0x14
#define kTelOemSatCmdLaunchBrowser			0x15
#define kTelOemSatCmdPlayTone				0x20
#define kTelOemSatCmdDisplayText			0x21
#define kTelOemSatCmdGetInkey				0x22
#define kTelOemSatCmdGetInput				0x23
#define kTelOemSatCmdSelectItem				0x24
#define kTelOemSatCmdSetUpMenu				0x25
#define kTelOemSatCmdSetUpIdleModeText		0x28
#define kTelOemSatCmdRunATCommand			0x34
#define kTelOemSatCmdOpenChannel			0x40
#define kTelOemSatCmdCloseChannel			0x41
#define kTelOemSatCmdReceiveData			0x42
#define kTelOemSatCmdSendData				0x43

/* Proactive command response General Result codes */
/* Refer to the Standard for applicability */
#define kTelOemSatResSuccess				0x00
#define kTelOemSatResOkPartialComprehension	0x01
#define kTelOemSatResOkMissingInfo			0x02
#define kTelOemSatResOkAdditionalEfsRead	0x03
#define kTelOemSatResOkIconNotDisplayed		0x04
#define kTelOemSatResOkModifiedBySim		0x05
#define kTelOemSatResOkLimitedService		0x06
#define kTelOemSatResOkWithModification		0x07
#define kTelOemSatResUserTermination		0x10
#define kTelOemSatResBackwardMove			0x11
#define kTelOemSatResNoResponseFromUser		0x12
#define kTelOemSatResHelpInfoRequest		0x13
#define kTelOemSatResTransactionTermination	0x14
#define kTelOemSatResMeUnableNow			0x20
#define kTelOemSatResNetworkUnableNow		0x21
#define kTelOemSatResUserDismissal			0x22
#define kTelOemSatResCallClearedByUser		0x23
#define kTelOemSatResTimerContradiction		0x24
#define kTelOemSatResSimControlInteraction	0x25
#define kTelOemSatResBrowserGenericError	0x26
#define kTelOemSatResBeyondMeCapabilities	0x30
#define kTelOemSatResCmdTypeNotUnderstood	0x31
#define kTelOemSatResCmdDataNotUnderstood	0x32
#define kTelOemSatResUnknownCmdNumber		0x33
#define kTelOemSatResSuppSvcReturnError		0x34
#define kTelOemSatResSmsRpError				0x35
#define kTelOemSatResMissingValues			0x36
#define kTelOemSatResUssdReturnError		0x37
#define kTelOemSatResMultipleCardError		0x38
#define kTelOemSatResSimControlFault		0x39
#define kTelOemSatResBearerIndProtocolError	0x3A

/* Additional Information codes for miscellaneous errors */
#define kTelOemSatAddGeNoSpecificCause		0x00

/* Additional Information codes for Launch Browser generic error */
#define kTelOemSatAddLbBearerUnavailable	0x01
#define kTelOemSatAddLbBrowserUnavailable	0x02
#define kTelOemSatAddLbDataReadError		0x03

/* Additional Information codes for "ME currently unable to process command" error */
#define kTelOemSatAddUnScreenBusy			0x01
#define kTelOemSatAddUnMeBusyOnCall			0x02
#define kTelOemSatAddUnMeBusyOnSuppSvc		0x03
#define kTelOemSatAddUnNoService			0x04
#define kTelOemSatAddUnAccessControlBar		0x05
#define kTelOemSatAddUnNoRadioResource		0x06
#define kTelOemSatAddUnNotInSpeechCall		0x07
#define kTelOemSatAddUnMeBusyOnUssd			0x08
#define kTelOemSatAddUnMeBusyOnSendDtmf		0x09

/* Additional Information codes for "Interaction with Call Control by SIM */
/* or MO Short Message Control by SIM, permanent problem" error (SIM Control fault) */
#define kTelOemSatAddCsActionNotAllowed		0x01
#define kTelOemSatAddCsRequestTypeChange	0x02

/* Additional Information codes for "Bearer Independent Protocol error" */
#define kTelOemSatAddBiNoChannelAvailable	0x01
#define kTelOemSatAddBiChannelClosed		0x02
#define kTelOemSatAddBiInvalidChannelId		0x03
#define kTelOemSatAddBiBufSizeUnavailable	0x04
#define kTelOemSatAddBiSecurityError		0x05
#define kTelOemSatAddBiTransportUnavailable	0x06

/* Response type codes */
#define kTelOemSatResponseTypeYesOrNo			1	/* applies to Get Inkey */
#define kTelOemSatResponseTypeDigitsGSM			2	/* applies to Get Inkey, Get Input */
#define kTelOemSatResponseTypeDigitsGSMPacked	3	/* applies to Get Input */
#define kTelOemSatResponseTypeDigitsUCS2		4	/* applies to Get Inkey, Get Input */
#define kTelOemSatResponseTypeTextGSM			5	/* applies to Get Inkey, Get Input */
#define kTelOemSatResponseTypeTextGSMPacked 	6	/* applies to Get Input */
#define kTelOemSatResponseTypeTextUCS2			7	/* applies to Get Inkey, Get Input */

/* Operation codes for Refresh */
#define kTelOemSatRefreshInitialization			0x03
#define kTelOemSatRefreshInitAndFileChange		0x02
#define kTelOemSatRefreshInitAndFullFileChange	0x00
#define kTelOemSatRefreshFileChange				0x01
#define kTelOemSatRefreshHardReset				0x04

/* Tone codes for Play Tone */
#define kTelOemSatSoundStdDial					0x01
#define kTelOemSatSoundStdCalledPartyBusy		0x02
#define kTelOemSatSoundStdCongestion			0x03
#define kTelOemSatSoundStdRadioPathAck			0x04
#define kTelOemSatSoundStdCallDropped			0x05
#define kTelOemSatSoundStdError					0x06
#define kTelOemSatSoundStdCallWaiting			0x07
#define kTelOemSatSoundStdRing					0x08
#define kTelOemSatSoundGeneralBeep				0x10
#define kTelOemSatSoundPositiveAck				0x11
#define kTelOemSatSoundError					0x12

/* Bearer codes */
#define kTelOemSatBearerCSD						1	/* applies to Launch Browser, Open Channel */
#define kTelOemSatBearerGPRS					2	/* applies to Launch Browser, Open Channel */
#define kTelOemSatBearerSMS						3	/* applies to Launch Browser */
#define kTelOemSatBearerUSSD					4	/* applies to Launch Browser */

/* Menu Selection event codes */
#define kTelOemSatMenuSelectionApplicationLaunch 1	/* Menu Selection: the user wants to launch a SAT application */
#define kTelOemSatMenuSelectionHelpInfoRequest	 2	/* the user wants to see the Help Information associated with a SAT application */

/* Event Download event codes */
#define kTelOemSatEvtDownloadUserActivity		 0x04
#define kTelOemSatEvtDownloadIdleScreenAvailable 0x05
#define kTelOemSatEvtDownloadLanguageSelection	 0x07
#define kTelOemSatEvtDownloadBrowserTermination	 0x08

/* Browser Termination event cause codes */
#define kTelOemSatBrowserTerminationUser		1
#define kTelOemSatBrowserTerminationError		2

/* Phone action codes */
#define kTelOemSatPhoneActionCallEstablishment	1

/* Control by SIM response codes */
#define kTelOemSatCtrlBySimResponseAllowed				1
#define kTelOemSatCtrlBySimResponseAllowedWithChange	2
#define kTelOemSatCtrlBySimResponseRejected				3

/* Control by SIM operation codes */
#define kTelOemSatCtrlBySimOperationCall		1
#define kTelOemSatCtrlBySimOperationSS			2
#define kTelOemSatCtrlBySimOperationUSSD		3
#define kTelOemSatCtrlBySimOperationSMS			4

/* Miscellaneous constants */
#define kTelOemSatMaxMessageTextLength			274	/* maximum number of characters that may be sent by the SIM in a text string (doesn't include a terminating 0). Reachable with "Display Text" and the GSM 7bits packed format. */
#define kTelOemSatMaxResponseTextLength			239	/* maximum number of characters that may be sent to the SIM (doesn't include a terminating 0). Regards "Get Input" and reachable with the GSM 7bits packed format. */

// enw [7/2/2003] Duplicated defines from MobPan/MobileServ/Dialer gives us one stop shopping.
#define kTelOemPINMaxLen						8
#define kTelOemPINMinLen						4
#define kTelOemPUKMaxLen						8
#define kTelOemPUKMinLen						8
#define kTelOemBarPasswdMaxLen					4
#define kTelOemBarPasswdMinLen					4


/* Async reply mode codes -------------------------------------------------	*/
#define kTelOemAsyncReplyModeByEvent			1
#define kTelOemAsyncReplyModeByNotification		2


/*@}*/


/*****************************************************************************/
/* Source Insight 3.1 MACROS: Thierry Langlais 2001 
 *****************************************************************************/
/* Macros used to easily re number all following lines
To use this MACRO, you have to be under Source Insight editor, then just put the cursor at the 
begining of the line and run macro via the menu "Run Macro". It this command is not yet defined,
then do it via the Key or Menu assigments...

RenumberLinesInHeaderFile()
stop
*/

/*	----------------------------------------------------------------------	*/
/* 	new messages
	----------------------------------------------------------------------	*/

/** 
 * @name New messages
 *
 */
/*@{*/


#define kTelOemInfGetEarBudStatusMessage						(kTelLastMessage + 1)
#define kTelOemCfgSetPhoneRTCMessage							(kTelLastMessage + 2)
#define kTelOemCfgGetPhoneRTCMessage							(kTelLastMessage + 3)
#define kTelOemCfgSetLoudspeakerVolumeMessage					(kTelLastMessage + 4)
#define kTelOemCfgGetLoudspeakerVolumeMessage					(kTelLastMessage + 5)
#define kTelOemCfgGetLoudspeakerVolumeRangeMessage				(kTelLastMessage + 6)
#define kTelOemCfgSetCallWaitingMessage							(kTelLastMessage + 7)
#define kTelOemCfgGetCallWaitingMessage							(kTelLastMessage + 8)
#define kTelOemCfgSetCallBarringMessage							(kTelLastMessage + 9)
#define kTelOemCfgGetCallBarringMessage							(kTelLastMessage + 10)
#define kTelOemCfgSetCallDivertingMessage						(kTelLastMessage + 11)
#define kTelOemCfgGetCallDivertingMessage						(kTelLastMessage + 12)
#define kTelOemCfgGetCallsDivertingMessage						(kTelLastMessage + 13)
#define kTelOemCfgSetCallDeflectionNumberMessage				(kTelLastMessage + 14)
#define kTelOemCfgSetCallLineIdRestrictionMessage 				(kTelLastMessage + 15)
#define kTelOemCfgGetCallLineIdRestrictionMessage 				(kTelLastMessage + 16)
#define kTelOemCfgGetCallLineIdPresentationMessage 				(kTelLastMessage + 17)
#define kTelOemCfgSetAlertPreferencesMessage	 				(kTelLastMessage + 18)
#define kTelOemCfgGetAlertPreferencesMessage 					(kTelLastMessage + 19)
#define kTelOemCfgSetVoiceMailNumberMessage	 					(kTelLastMessage + 20)
#define kTelOemCfgGetVoiceMailNumberMessage 					(kTelLastMessage + 21)
#define kTelOemCfgSetBearerServiceMessage 						(kTelLastMessage + 22)
#define kTelOemPowGetPhonePowerStatusMessage					(kTelLastMessage + 23)
#define kTelOemInfGetCallsDurationMessage						(kTelLastMessage + 24)
#define kTelOemInfResetCallsDurationMessage						(kTelLastMessage + 25)
#define kTelOemNwkGetHomeNetworkIdMessage						(kTelLastMessage + 26)
#define kTelOemNwkGetNetworkNameMessage							(kTelLastMessage + 27)
#define kTelOemNwkGetPreferredNetworksMessage					(kTelLastMessage + 28)
#define kTelOemNwkSetPreferredNetworkMessage					(kTelLastMessage + 29)
#define kTelOemNwkDeletePreferredNetworkMessage					(kTelLastMessage + 30)
#define kTelOemNwkGetNetworksMessage							(kTelLastMessage + 31)
#define kTelOemNwkGetRegistrationMessage						(kTelLastMessage + 32)
#define kTelOemNwkGetCurrentRegistrationServiceStatusMessage	(kTelLastMessage + 33)
#define kTelOemStySetLockMessage								(kTelLastMessage + 34)
#define kTelOemStyGetLockMessage								(kTelLastMessage + 35)
#define kTelOemStyUnblockCodeMessage							(kTelLastMessage + 36)
#define kTelOemStyGetRemainingAttemptNumberMessage				(kTelLastMessage + 37)

#define kTelOemSpcGetCallsInformationMessage					(kTelLastMessage + 38)
#define kTelOemSpcGetCallInformationMessage						(kTelLastMessage + 39)
#define kTelOemSpcDialMessage									(kTelLastMessage + 40)
#define kTelOemSpcExplicitCallTransferMessage					(kTelLastMessage + 41)
#define kTelOemSpcTtyMessage									(kTelLastMessage + 42)

#define kTelOemAvrPlaySampleMessage								(kTelLastMessage + 43)
#define kTelOemAvrDeleteSampleMessage							(kTelLastMessage + 44)
#define kTelOemAvrGetSamplesMessage								(kTelLastMessage + 45)
#define kTelOemAvrStopMessage									(kTelLastMessage + 46)
#define kTelOemAvrGetSampleStatusMessage						(kTelLastMessage + 47)
#define kTelOemAvrEnterVoiceRecognitionModeMessage				(kTelLastMessage + 48)
#define kTelOemAvrRecordSampleMessage							(kTelLastMessage + 49)

#define kTelOemGprsSetContextMessage							(kTelLastMessage + 50)
#define kTelOemGprsGetContextMessage							(kTelLastMessage + 51)
#define kTelOemGprsGetDefinedCidsMessage						(kTelLastMessage + 52)
#define kTelOemGprsSetAttachMessage								(kTelLastMessage + 53)
#define kTelOemGprsGetAttachMessage								(kTelLastMessage + 54)
#define kTelOemGprsSetPdpActivationMessage						(kTelLastMessage + 55)
#define kTelOemGprsGetPdpActivationMessage						(kTelLastMessage + 56)
#define kTelOemGprsGetPdpAddressMessage							(kTelLastMessage + 57)
#define kTelOemGprsSetNwkRegistrationMessage					(kTelLastMessage + 58)
#define kTelOemGprsGetNwkRegistrationMessage					(kTelLastMessage + 59)
#define kTelOemGprsSetSmsServiceMessage							(kTelLastMessage + 60)
#define kTelOemGprsGetSmsServiceMessage							(kTelLastMessage + 61)
#define kTelOemGprsActivateDataMessage							(kTelLastMessage + 62)
#define kTelOemGprsSetQosRequestedMessage						(kTelLastMessage + 63)
#define kTelOemGprsGetQosRequestedMessage						(kTelLastMessage + 64)
#define kTelOemGprsSetQosMinimumMessage							(kTelLastMessage + 65)
#define kTelOemGprsGetQosMinimumMessage							(kTelLastMessage + 66)
#define kTelOemGprsGetQosCurrentMessage							(kTelLastMessage + 67)
#define kTelOemGprsSetEventReportingMessage						(kTelLastMessage + 68)
#define kTelOemGprsGetEventReportingMessage						(kTelLastMessage + 69)
#define kTelOemGprsGetDataCounterMessage						(kTelLastMessage + 70)
#define kTelOemGprsSetAttachTimedMessage						(kTelLastMessage + 71)

#define kTelOemUssdSendCommandMessage							(kTelLastMessage + 72)

#define kTelOemSetAsyncReplyModeMessage							(kTelLastMessage + 73)

#define kTelOemSmsIsMessageGsmAlphabetCompatibleMessage			(kTelLastMessage + 74)

#define kTelOemSendCommandStringMessage							(kTelLastMessage + 75)

#define kTelOemStySimLockActivateMessage						(kTelLastMessage + 76)
#define kTelOemStySimLockDeactivateMessage						(kTelLastMessage + 77)
#define kTelOemStySimLockStatusMessage							(kTelLastMessage + 78)
#define kTelOemStySimLockGetAttemptCounterMessage				(kTelLastMessage + 79)
#define kTelOemStySimLockResetAttemptCounterMessage				(kTelLastMessage + 80)

#define kTelOemAlertSilenceMessage								(kTelLastMessage + 81)			

#define kTelOemSimReadElementaryFileMessage						(kTelLastMessage + 82)
#define kTelOemSimGetIconPropertiesMessage						(kTelLastMessage + 83)
#define kTelOemSimGetIconMessage								(kTelLastMessage + 84)
#define kTelOemSimConvertTextMessage							(kTelLastMessage + 85)
#define kTelOemSatInitializeMessage								(kTelLastMessage + 86)
#define kTelOemSatGetProactiveCommandParametersMessage			(kTelLastMessage + 87)
#define kTelOemSatSendProactiveCommandResponseMessage			(kTelLastMessage + 88)
#define kTelOemSatNotifyMenuSelectionMessage					(kTelLastMessage + 89)
#define kTelOemSatNotifyPhoneActionMessage						(kTelLastMessage + 90)
#define kTelOemSatSendEventNotificationMessage					(kTelLastMessage + 91)
#define kTelOemSatPerformInternalTaskMessage					(kTelLastMessage + 92)
#define kTelOemSimGetSpnMessage									(kTelLastMessage + 93)
#define kTelOemHandleTimeChangeEvent                     		(kTelLastMessage + 94)
#define kTelOemSimUpdateMessage									(kTelLastMessage + 95)		
#define kTelOemCfgSetAudioPathGainMessage						(kTelLastMessage + 96)		
#define kTelOemCfgGetAudioPathGainMessage						(kTelLastMessage + 97)				
#define kTelOemSmsGetCbmDataMessage								(kTelLastMessage + 98)

/* WARNING: this message MUST BE the last one processed in the OEM SIM driver */
/* If you have to add new functions in OEM SIM Driver, then insert their 
corresponding message above */
#define kTelOemLastMessage			                        	kTelOemSmsGetCbmDataMessage

/*@}*/


/*	----------------------------------------------------------------------	*/
/*  TelephonyMgrOem library call ID's
	----------------------------------------------------------------------	*/

/** 
 * @name TelephonyMgrOem library call ID's
 *
 */
/*@{*/



#define telLibTrapOemGetEvent									(sysLibTrapCustom)
#define telLibTrapOemGetTelephonyEvent							(sysLibTrapCustom + 1)
#define telLibTrapOemInfGetEarBudStatus							(sysLibTrapCustom + 2)
#define telLibTrapOemCfgSetPhoneRTC								(sysLibTrapCustom + 3)
#define telLibTrapOemCfgGetPhoneRTC								(sysLibTrapCustom + 4)
#define telLibTrapOemCfgSetLoudspeakerVolume					(sysLibTrapCustom + 5)
#define telLibTrapOemCfgGetLoudspeakerVolume					(sysLibTrapCustom + 6)
#define telLibTrapOemCfgGetLoudspeakerVolumeRange				(sysLibTrapCustom + 7)
#define telLibTrapOemCfgSetCallWaiting							(sysLibTrapCustom + 8)
#define telLibTrapOemCfgGetCallWaiting							(sysLibTrapCustom + 9)
#define telLibTrapOemCfgSetCallBarring							(sysLibTrapCustom + 10)
#define telLibTrapOemCfgGetCallBarring							(sysLibTrapCustom + 11)
#define telLibTrapOemCfgSetCallDiverting						(sysLibTrapCustom + 12)
#define telLibTrapOemCfgGetCallDiverting						(sysLibTrapCustom + 13)
#define telLibTrapOemCfgGetCallsDiverting						(sysLibTrapCustom + 14)
#define telLibTrapOemCfgSetCallDeflectionNumber					(sysLibTrapCustom + 15)
#define telLibTrapOemCfgSetCallLineIdRestriction				(sysLibTrapCustom + 16)
#define telLibTrapOemCfgGetCallLineIdRestriction				(sysLibTrapCustom + 17)
#define telLibTrapOemCfgGetCallLineIdPresentation				(sysLibTrapCustom + 18)
#define telLibTrapOemCfgSetAlertPreferences						(sysLibTrapCustom + 19)
#define telLibTrapOemCfgGetAlertPreferences						(sysLibTrapCustom + 20)
#define telLibTrapOemCfgSetVoiceMailNumber						(sysLibTrapCustom + 21)
#define telLibTrapOemCfgGetVoiceMailNumber						(sysLibTrapCustom + 22)
#define telLibTrapOemCfgSetBearerService						(sysLibTrapCustom + 23)
#define telLibTrapOemPowGetPhonePowerStatus						(sysLibTrapCustom + 24)
#define telLibTrapOemInfGetCallsDuration						(sysLibTrapCustom + 25)
#define telLibTrapOemInfResetCallsDuration						(sysLibTrapCustom + 26)
#define telLibTrapOemNwkGetHomeNetworkId						(sysLibTrapCustom + 27)
#define telLibTrapOemNwkGetNetworkName							(sysLibTrapCustom + 28)
#define telLibTrapOemNwkGetPreferredNetworks					(sysLibTrapCustom + 29)
#define telLibTrapOemNwkSetPreferredNetwork						(sysLibTrapCustom + 30)
#define telLibTrapOemNwkDeletePreferredNetwork					(sysLibTrapCustom + 31)
#define telLibTrapOemNwkGetNetworks								(sysLibTrapCustom + 32)
#define telLibTrapOemNwkGetRegistration							(sysLibTrapCustom + 33)
#define telLibTrapOemNwkGetCurrentRegistrationServiceStatus		(sysLibTrapCustom + 34)
#define telLibTrapOemStySetLock									(sysLibTrapCustom + 35)
#define telLibTrapOemStyGetLock									(sysLibTrapCustom + 36)
#define telLibTrapOemStyUnblockCode								(sysLibTrapCustom + 37)
#define telLibTrapOemStyGetRemainingAttemptNumber				(sysLibTrapCustom + 38)
#define telLibTrapOemSpcGetCallsInformation						(sysLibTrapCustom + 39)
#define telLibTrapOemSpcGetCallInformation						(sysLibTrapCustom + 40)
#define telLibTrapOemSpcDial									(sysLibTrapCustom + 41)
#define telLibTrapOemSpcExplicitCallTransfer					(sysLibTrapCustom + 42)
#define telLibTrapOemSpcTty										(sysLibTrapCustom + 43)

#define telLibTrapOemAvrPlaySample								(sysLibTrapCustom + 44)
#define telLibTrapOemAvrDeleteSample							(sysLibTrapCustom + 45)
#define telLibTrapOemAvrGetSamples								(sysLibTrapCustom + 46)
#define telLibTrapOemAvrStop									(sysLibTrapCustom + 47)
#define telLibTrapOemAvrGetSampleStatus							(sysLibTrapCustom + 48)
#define telLibTrapOemAvrEnterVoiceRegognitionMode				(sysLibTrapCustom + 49)
#define telLibTrapOemAvrRecordSample							(sysLibTrapCustom + 50)
#define telLibTrapOemGprsSetContext								(sysLibTrapCustom + 51)
#define telLibTrapOemGprsGetContext								(sysLibTrapCustom + 52)
#define telLibTrapOemGprsGetDefinedCids							(sysLibTrapCustom + 53)
#define telLibTrapOemGprsSetAttach								(sysLibTrapCustom + 54)
#define telLibTrapOemGprsGetAttach								(sysLibTrapCustom + 55)
#define telLibTrapOemGprsSetPdpActivation						(sysLibTrapCustom + 56)
#define telLibTrapOemGprsGetPdpActivation						(sysLibTrapCustom + 57)
#define telLibTrapOemGprsGetPdpAddress							(sysLibTrapCustom + 58)
#define telLibTrapOemGprsSetNwkRegistration						(sysLibTrapCustom + 59)
#define telLibTrapOemGprsGetNwkRegistration						(sysLibTrapCustom + 60)
#define telLibTrapOemGprsSetSmsService							(sysLibTrapCustom + 61)
#define telLibTrapOemGprsGetSmsService							(sysLibTrapCustom + 62)
#define telLibTrapOemGprsActivateData							(sysLibTrapCustom + 63)
#define telLibTrapOemGprsSetQosRequested						(sysLibTrapCustom + 64)
#define telLibTrapOemGprsGetQosRequested						(sysLibTrapCustom + 65)
#define telLibTrapOemGprsSetQosMinimum							(sysLibTrapCustom + 66)
#define telLibTrapOemGprsGetQosMinimum							(sysLibTrapCustom + 67)
#define telLibTrapOemGprsGetQosCurrent							(sysLibTrapCustom + 68)
#define telLibTrapOemGprsSetEventReporting						(sysLibTrapCustom + 69)
#define telLibTrapOemGprsGetEventReporting						(sysLibTrapCustom + 70)
#define telLibTrapOemGprsGetDataCounter							(sysLibTrapCustom + 71)
#define telLibTrapOemGprsSetAttachTimedMessage					(sysLibTrapCustom + 72)

#define telLibTrapOemUssdSendCommand							(sysLibTrapCustom + 73)
#define telLibTrapOemSetAsyncReplyMode							(sysLibTrapCustom + 74)
#define telLibTrapOemSmsIsMessageGsmAlphabetCompatible			(sysLibTrapCustom + 75)
#define telLibTrapOemSendCommandString							(sysLibTrapCustom + 76)

#define telLibTrapOemStySimLockActivate							(sysLibTrapCustom + 77)
#define telLibTrapOemStySimLockDeactivate						(sysLibTrapCustom + 78)
#define telLibTrapOemStySimLockStatus							(sysLibTrapCustom + 79)
#define telLibTrapOemStySimLockGetAttemptCounter				(sysLibTrapCustom + 80)
#define telLibTrapOemStySimLockResetAttemptCounter				(sysLibTrapCustom + 81)

#define telLibTrapOemAlertSilence								(sysLibTrapCustom + 82)

#define telLibTrapOemSimReadElementaryFile						(sysLibTrapCustom + 83)
#define telLibTrapOemSimGetIconProperties						(sysLibTrapCustom + 84)
#define telLibTrapOemSimGetIcon									(sysLibTrapCustom + 85)
#define telLibTrapOemSimConvertText								(sysLibTrapCustom + 86)
#define telLibTrapOemSatInitialize								(sysLibTrapCustom + 87)
#define telLibTrapOemSatGetProactiveCommandParameters			(sysLibTrapCustom + 88)
#define telLibTrapOemSatSendProactiveCommandResponse			(sysLibTrapCustom + 89)
#define telLibTrapOemSatNotifyMenuSelection						(sysLibTrapCustom + 90)
#define telLibTrapOemSatNotifyPhoneAction						(sysLibTrapCustom + 91)
#define telLibTrapOemSatSendEventNotification					(sysLibTrapCustom + 92)
#define telLibTrapOemSatPerformInternalTask						(sysLibTrapCustom + 93)
#define telLibTrapOemSimGetSpnQuery								(sysLibTrapCustom + 94)
#define telLibTrapOemRemoveOemCall								(sysLibTrapCustom + 95)
#define telLibTrapOemHandleTimeChangeEvent               		(sysLibTrapCustom + 96)
#define telLibTrapOemSimUpdate									(sysLibTrapCustom + 97)
#define telLibTrapOemCfgSetAudioPathGain						(sysLibTrapCustom + 98)
#define telLibTrapOemCfgGetAudioPathGain						(sysLibTrapCustom + 99)
#define telLibTrapOemSmsTelOemSmsGetCbmData						(sysLibTrapCustom + 100)
#define telLibTrapOemLastOne									telLibTrapOemSmsTelOemSmsGetCbmData

#ifdef BUILDING_TEL_OEM_MGR
	#define TEL_OEM_LIB_TRAP(trapNum)
#else
	#define TEL_OEM_LIB_TRAP(trapNum) SYS_TRAP(trapNum)
#endif


/*@}*/


/*	----------------------------------------------------------------------	*/
/* 	standard functions
	----------------------------------------------------------------------	*/

/** 
 * @name Functions
 *
 */
/*@{*/

Err TelOemOpen(		UInt16 							refnum,
					UInt32							iVersnum)
		TEL_OEM_LIB_TRAP(sysLibTrapOpen);

Err TelOemClose(	UInt16 							refnum)
		TEL_OEM_LIB_TRAP(sysLibTrapClose);

Err TelOemSleep(UInt16 refnum)
		TEL_OEM_LIB_TRAP(sysLibTrapSleep);

Err TelOemWake(UInt16 refnum)
		TEL_OEM_LIB_TRAP(sysLibTrapWake);

/*	----------------------------------------------------------------------	*/
/* 	event management
	----------------------------------------------------------------------	*/
void TelOemGetEvent(UInt16 							refnum,
				 	TelAppID						appId, 
				 	EventPtr						oEventP, 
				 	Int32							iTimeOut)
		TEL_OEM_LIB_TRAP(telLibTrapOemGetEvent);

void TelOemGetTelephonyEvent(UInt16					refnum,
				 	TelAppID						appId, 
				 	TelEventType*					telEventP, 
				 	Int32							iTimeOut)
		TEL_OEM_LIB_TRAP(telLibTrapOemGetTelephonyEvent);

void TelOemRemoveOemCall(UInt16					refnum,
				 	UInt16						transId)
		TEL_OEM_LIB_TRAP(telLibTrapOemRemoveOemCall);

/*	----------------------------------------------------------------------	*/
/* 	new functions
	----------------------------------------------------------------------	*/
/*  misc */
/* sending commands to the phone */
Err	TelOemSendCommandString(UInt16	  				iRefnum,
					     TelAppID 					iAppId, 
						 TelOemSendCommandStringType* 	ioParamP ,
						 UInt16*  					ioTransIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSendCommandString);


Err	TelOemSetAsyncReplyMode(UInt16					refnum,
					TelAppID						appId, 
					UInt8							AsyncReplyMode
					)
		TEL_OEM_LIB_TRAP(telLibTrapOemSetAsyncReplyMode);

/* extended SMS */
Err	TelOemSmsIsMessageGsmAlphabetCompatible(UInt16	refnum,
					TelAppID						appId, 
					TelOemSmsGsmAlphabetCompatibleType*	compatibilityP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSmsIsMessageGsmAlphabetCompatible);
		
Err	TelOemSmsGetCbmData (UInt16						refnum,
					TelAppID						appId, 
					TelOemSmsGetCbmType*			cbmMessageP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSmsTelOemSmsGetCbmData);
		

/* 	configuration */
Err	TelOemCfgSetPhoneRTC(UInt16						refnum,
					TelAppID						appId, 
					TelOemCfgPhoneRTCType*			timeP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetPhoneRTC);		
		
Err	TelOemCfgGetPhoneRTC(UInt16						refnum,
					TelAppID						appId, 
					TelOemCfgPhoneRTCType*			timeP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetPhoneRTC);		
		
Err	TelOemCfgSetLoudspeakerVolume(UInt16			refnum,
				  	TelAppID	 					appId, 
				  	UInt8							volume,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetLoudspeakerVolume);

Err	TelOemCfgGetLoudspeakerVolume(UInt16			refnum,
				  	TelAppID	 					appId, 
				  	UInt8*							volumeP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetLoudspeakerVolume);

Err	TelOemCfgGetLoudspeakerVolumeRange(UInt16		refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgGetLoudspeakerVolumeRangeType* volumeP,
				  	UInt16*	 						transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetLoudspeakerVolumeRange);

Err	TelOemCfgSetCallWaiting(UInt16 					refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallWaitingType*		p,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetCallWaiting);
		
Err	TelOemCfgGetCallWaiting(UInt16 					refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallWaitingType*		p,
				  	UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetCallWaiting);
		
Err	TelOemCfgSetCallBarring(UInt16 					refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallBarringType*		callP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetCallBarring);
		
Err	TelOemCfgGetCallBarring(UInt16 					refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallBarringType*		statusP,
				  	UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetCallBarring);
		

Err	TelOemCfgSetCallDiverting(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallDivertingType*		callP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetCallDiverting);
		
Err	TelOemCfgGetCallDiverting(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallDivertingType*		statusP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetCallDiverting);

Err	TelOemCfgGetCallsDiverting(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallsDivertingType*	statusP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetCallsDiverting);


Err	TelOemCfgSetCallDeflectionNumber(UInt16			refnum,
				  	TelAppID	 					appId, 
				  	char* 							numberP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetCallDeflectionNumber);
		
	
Err	TelOemCfgSetCallLineIdRestriction(UInt16		refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgCallLineIDRestrictionType* restrictionP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetCallLineIdRestriction);
		
Err	TelOemCfgGetCallLineIdRestriction(UInt16		refnum,
				  	TelAppID	 					appId,
				  	TelOemCfgCallLineIDRestrictionType* restrictionP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetCallLineIdRestriction);

Err	TelOemCfgGetCallLineIdPresentation(UInt16		refnum,
				  	TelAppID	 					appId,
				  	TelOemCfgCallLineIDPresentationType* presentationP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetCallLineIdPresentation);

Err	TelOemCfgSetAlertPreferences(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgAlertPreferencesType*	prefsP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetAlertPreferences);
		
Err	TelOemCfgGetAlertPreferences(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgAlertPreferencesType*	prefsP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetAlertPreferences);

Err	TelOemCfgSetVoiceMailNumber(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgVoiceMailType*			vmP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetVoiceMailNumber);
		
Err	TelOemCfgGetVoiceMailNumber(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgVoiceMailType*			vmP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetVoiceMailNumber);

Err	TelOemCfgSetBearerService(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemCfgBearerServiceType*		bearerP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetBearerService);

/*	power */
Err	TelOemPowGetPhonePowerStatus(UInt16				refnum,
					TelAppID						appId, 
					UInt8*							statusP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemPowGetPhonePowerStatus);		
		
/*	information */
Err	TelOemInfGetEarBudStatus(UInt16	 				refnum,
				  	TelAppID	 					appId, 
				  	Boolean*						statusP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemInfGetEarBudStatus);

Err	TelOemInfGetCallsDuration(UInt16				refnum,
					TelAppID						appId, 
					TelOemInfGetCallsDurationType*	paramP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemInfGetCallsDuration);		
		
Err	TelOemInfResetCallsDuration(UInt16				refnum,
					TelAppID						appId, 
					Char*							passwordP,
					UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemInfResetCallsDuration);		
		
/*	network */
Err	TelOemNwkGetHomeNetworkId(UInt16				refnum,
					TelAppID						appId, 
					UInt32* 						networkIdP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkGetHomeNetworkId);

Err	TelOemNwkGetNetworkName(UInt16					refnum,
					TelAppID						appId, 
					TelNwkGetNetworkNameType*		networkP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkGetNetworkName);

Err	TelOemNwkGetPreferredNetworks(UInt16			refnum,
					TelAppID						appId, 
					TelOemNwkGetPreferredNetworksType* paramP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkGetPreferredNetworks);

Err	TelOemNwkSetPreferredNetwork(UInt16				refnum,
					TelAppID						appId, 
					TelOemNwkPreferredNetworkType* 	ioParampP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkSetPreferredNetwork);

Err	TelOemNwkDeletePreferredNetwork(UInt16			refnum,
					TelAppID						appId, 
					UInt32							networkId,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkDeletePreferredNetwork);

Err	TelOemNwkGetNetworks(UInt16						refnum,
					TelAppID						appId, 
					TelOemNwkGetNetworksType*		networkP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkGetNetworks);

Err	TelOemNwkGetRegistration(UInt16					refnum,
					TelAppID						appId, 
					UInt8*							registrationP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkGetRegistration);


Err	TelOemNwkGetCurrentRegistrationServiceStatus(UInt16	refnum,
					TelAppID							appId, 
					UInt8*								registrationStatusP,
					UInt16*								transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemNwkGetCurrentRegistrationServiceStatus);

/*	security */
Err	TelOemStySetLock(UInt16							refnum,
					TelAppID						appId, 
					TelOemStyLockType*				securityP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStySetLock);

Err	TelOemStyGetLock(UInt16							refnum,
					TelAppID						appId, 
					TelOemStyLockType*				securityP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStyGetLock);

Err	TelOemStyUnblockCode(UInt16						refnum,
					TelAppID						appId, 
					TelOemStyCodeUnblockType*		securityP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStyUnblockCode);

Err TelOemStyGetRemainingAttemptNumber(UInt16 		refnum, 
					TelAppID 						appId, 
					TelOemStyRemainingAttemptNumberType *securityP, 
					UInt16							*transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStyGetRemainingAttemptNumber);

Err TelOemStySimLockActivate (UInt16 				refnum, 
					TelAppID 						appId, 
					TelOemStySimLockType*			simLockP, 
					UInt16* 						transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStySimLockActivate);

Err TelOemStySimLockDeactivate (UInt16 				refnum, 
					TelAppID 						appId, 
					TelOemStySimLockType*			simLockP, 
					UInt16* 						transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStySimLockDeactivate);

Err TelOemStySimLockStatus (UInt16 					refnum, 
					TelAppID 						appId, 
					TelOemStySimLockType*			simLockP, 
					UInt16* 						transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStySimLockStatus);

Err TelOemStySimLockGetAttemptCounter (UInt16		refnum, 
					TelAppID 						appId, 
					TelOemStySimLockType*			simLockP, 
					UInt16* 						transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStySimLockGetAttemptCounter);

Err TelOemStySimLockResetAttemptCounter (UInt16 	refnum, 
					TelAppID 						appId, 
					TelOemStySimLockType*			simLockP, 
					UInt16* 						transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemStySimLockResetAttemptCounter);
	
/*	speech service */
Err	TelOemSpcGetCallsInformation(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemSpcGetCallsInformationType* paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSpcGetCallsInformation);
		
Err	TelOemSpcGetCallInformation(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemSpcCallInformationType*	paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSpcGetCallInformation);
		
Err	TelOemSpcDial(UInt16							refnum,
					TelAppID						appId, 
					TelOemSpcDialType*				dialNumberP,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSpcDial);

Err	TelOemSpcExplicitCallTransfer(UInt16			refnum,
					TelAppID						appId, 
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSpcExplicitCallTransfer);

Err	TelOemSpcTty(UInt16								refnum,
					TelAppID						appId, 
					Boolean							enable,
					UInt16*							transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSpcTty);


/*	VOICE RECOGNITION */
Err	TelOemAvrPlaySample(UInt16						refnum,
				  	TelAppID	 					appId, 
				  	TelOemAvrSampleType*			sampleP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrPlaySample);
		
Err	TelOemAvrDeleteSample(UInt16					refnum,
				  	TelAppID	 					appId, 
				  	TelOemAvrSampleType*			sampleP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrDeleteSample);
		
Err	TelOemAvrGetSamples(UInt16						refnum,
				  	TelAppID	 					appId, 
				  	TelOemAvrSampleIdType*			sampleP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrGetSamples);
		
Err	TelOemAvrStop(UInt16							refnum,
				  	TelAppID	 					appId, 
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrStop);
		
Err	TelOemAvrGetSampleStatus(UInt16					refnum,
				  	TelAppID	 					appId, 
				  	TelOemAvrSampleStatusType*		sampleP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrGetSampleStatus);
		
Err	TelOemAvrEnterVoiceRecognitionMode(UInt16		refnum,
				  	TelAppID	 					appId, 
				  	UInt8							contextId,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrEnterVoiceRegognitionMode);
		
Err	TelOemAvrRecordSample(UInt16					refnum,
				  	TelAppID	 					appId, 
				  	TelOemAvrRecordSampleType*		sampleP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAvrRecordSample);
		
/* GPRS */
Err	TelOemGprsSetContext(UInt16						refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsContextType*			iContextP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetContext);

Err	TelOemGprsGetContext(UInt16						refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsContextType*			ioContextP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetContext);

Err	TelOemGprsGetDefinedCids(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsDefinedCidsType*		ioCidsP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetDefinedCids);

Err	TelOemGprsSetAttach(UInt16						refNum,
				  	TelAppID	 					appId, 
				  	UInt8							iAttach,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetAttach);

Err	TelOemGprsGetAttach(UInt16						refNum,
				  	TelAppID	 					appId, 
				  	UInt8*							oAttach,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetAttach);

Err	TelOemGprsSetPdpActivation(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsPdpActivationType*	iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetPdpActivation);

Err	TelOemGprsGetPdpActivation(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsPdpActivationType*	ioParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetPdpActivation);

Err	TelOemGprsGetPdpAddress(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsPdpAddressType*		ioParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetPdpAddress);

Err	TelOemGprsSetNwkRegistration(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	UInt8							iRegistrationType,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetNwkRegistration);

Err	TelOemGprsGetNwkRegistration(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsNwkRegistrationType*	ioRegistrationP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetNwkRegistration);

Err	TelOemGprsSetSmsService(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	UInt8							iSMSService,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetSmsService);

Err	TelOemGprsGetSmsService(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	UInt8*							oSMSService,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetSmsService);

Err	TelOemGprsActivateData(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsActivateDataType*		iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsActivateData);

Err	TelOemGprsSetQosRequested(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsQosType*				iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetQosRequested);

Err	TelOemGprsGetQosRequested(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsQosType*				ioParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetQosRequested);

Err	TelOemGprsSetQosMinimum(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsQosType*				iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetQosMinimum);

Err	TelOemGprsGetQosMinimum(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsQosType*				ioParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetQosMinimum);

Err	TelOemGprsGetQosCurrent(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsQosType*				ioParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetQosCurrent);

Err	TelOemGprsSetEventReporting(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsEventReportingType*	iEvtReportP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetEventReporting);

Err	TelOemGprsGetEventReporting(UInt16				refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsEventReportingType*	oEvtReportP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetEventReporting);

Err	TelOemGprsGetDataCounter(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemGprsDataCounterType*		oDataCntP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsGetDataCounter);
		
Err	TelOemGprsSetAttachTimed(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	UInt16							timeout,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemGprsSetAttachTimedMessage);

/* USSD */
Err	TelOemUssdSendCommand(UInt16					refNum,
				  	TelAppID	 					appId, 
				  	TelOemUssdCommandParamType*		oParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemUssdSendCommand);

/* Alert */
Err	TelOemAlertSilence(UInt16						refNum,
				  	TelAppID	 					appId, 
				  	UInt8							iAlertType,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemAlertSilence);
		
/* SIM card */
Err	TelOemSimReadElementaryFile(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemSimReadElementaryFileType* paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSimReadElementaryFile);
		
Err	TelOemSimGetIconProperties(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemSimGetIconPropertiesType*	paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSimGetIconProperties);
		
Err	TelOemSimGetIcon(UInt16							refnum,
				  	TelAppID	 					appId, 
				  	TelOemSimGetIconType*			paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSimGetIcon);
		
Err	TelOemSimConvertText(UInt16						refnum,
				  	TelAppID	 					appId, 
				  	TelOemSimConvertTextType*		paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSimConvertText);
		
/* SIM Application Toolkit */
Err	TelOemSatInitialize(UInt16						refnum,
				  	TelAppID	 					appId, 
				  	TelOemSatInitializeType*		iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatInitialize);

Err	TelOemSatGetProactiveCommandParameters(UInt16	refnum,
				  	TelAppID	 					appId, 
				  	TelOemSatGetProactiveCmdParamType*	paramP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatGetProactiveCommandParameters);
		
Err	TelOemSatSendProactiveCommandResponse(UInt16	refnum,
				  	TelAppID	 					appId, 
				  	TelOemSatSendProactiveCmdResponseType*	iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatSendProactiveCommandResponse);
		
Err	TelOemSatNotifyMenuSelection(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	TelOemSatNotifyMenuSelectionType*	iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatNotifyMenuSelection);
		
Err	TelOemSatNotifyPhoneAction(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	UInt8							actionCode,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatNotifyPhoneAction);

Err	TelOemSatSendEventNotification(UInt16			refnum,
				  	TelAppID	 					appId, 
				  	TelOemSatSendEventNotificationType*	iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatSendEventNotification);

Err	TelOemSatPerformInternalTask(UInt16				refnum,
				  	TelAppID	 					appId, 
				  	UInt32							taskCode,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSatPerformInternalTask);
	
Err	TelOemSimGetSpn(UInt16							refnum,
				  	TelAppID	 					appId, 
				  	TelOemSimGetSpnType*			iParamP,
				  	UInt16*		 					transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSimGetSpnQuery);
	
Err   TelOemHandleTimeChangeEvent(UInt16     refnum,
               TelAppID                appId,
               Int32                   timeDelta,
               UInt16*                 transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemHandleTimeChangeEvent);
		
Err   TelOemSimUpdate(UInt16     refnum,
               TelAppID                appId,
               TelOemSimUpdateType*    update,
               UInt16*                 transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemSimUpdate);

Err   TelOemCfgSetAudioPathGain(UInt16     refnum,
               TelAppID                appId,
               TelOemAudioPathType*    iParamP,
               UInt16*                 transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgSetAudioPathGain);

Err   TelOemCfgGetAudioPathGain(UInt16     refnum,
               TelAppID                appId,
               TelOemAudioPathType*    iParamP,
               UInt16*                 transIdP)
		TEL_OEM_LIB_TRAP(telLibTrapOemCfgGetAudioPathGain);

/*@}*/

		
/*	----------------------------------------------------------------------	*/
/*	MACROS to check function availability											*/
/*	----------------------------------------------------------------------	*/

/** 
 * @name Macros
 *
 */
/*@{*/


#define TelIsOemSmsIsMessageGsmAlphabetCompatible(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSmsIsMessageGsmAlphabetCompatibleMessage, transIdP)

#define TelIsOemSmsGetCbmDataSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSmsGetCbmDataMessage, transIdP)

#define TelIsOemSetAsyncReplyModeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSetAsyncReplyModeMessage, transIdP)

#define TelIsOemSendCommandStringSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSendCommandStringMessage, transIdP)

#define TelIsOemCfgSetPhoneRTCSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetPhoneRTCMessage, transIdP)

#define TelIsOemCfgGetPhoneRTCSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetPhoneRTCMessage, transIdP)

#define TelIsOemCfgSetLoudspeakerVolumeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetLoudspeakerVolumeMessage, transIdP)

#define TelIsOemCfgGetLoudspeakerVolumeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetLoudspeakerVolumeMessage, transIdP)

#define TelIsOemCfgGetLoudspeakerVolumeRangeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetLoudspeakerVolumeRangeMessage, transIdP)

#define TelIsOemCfgSetCallWaitingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetCallWaitingMessage, transIdP)

#define TelIsOemCfgGetCallWaitingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetCallWaitingMessage, transIdP)

#define TelIsOemCfgSetCallBarringSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetCallBarringMessage, transIdP)

#define TelIsOemCfgGetCallBarringSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetCallBarringMessage, transIdP)

#define TelIsOemCfgSetCallDivertingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetCallDivertingMessage, transIdP)

#define TelIsOemCfgGetCallDivertingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetCallDivertingMessage, transIdP)

#define TelIsOemCfgGetCallsDivertingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetCallsDivertingMessage, transIdP)

#define TelIsOemCfgSetCallDeflectionNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetCallDeflectionNumberMessage, transIdP)

#define TelIsOemCfgSetCallLineIdRestrictionSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetCallLineIdRestrictionMessage, transIdP)

#define TelIsOemCfgGetCallLineIdRestrictionSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetCallLineIdRestrictionMessage, transIdP)

#define TelIsOemCfgGetCallLineIdPresentationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetCallLineIdPresentationMessage, transIdP)

#define TelIsOemCfgSetAlertPreferencesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetAlertPreferencesMessage, transIdP)

#define TelIsOemCfgGetAlertPreferencesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetAlertPreferencesMessage, transIdP)

#define TelIsOemCfgSetVoiceMailNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetVoiceMailNumberMessage, transIdP)

#define TelIsOemCfgGetVoiceMailNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetVoiceMailNumberMessage, transIdP)

#define TelIsOemCfgGetLocalTimestampSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetLocalTimestampMessage, transIdP)

#define TelIsOemCfgSetBearerServiceSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetBearerServiceMessage, transIdP)


#define TelIsOemPowGetPhonePowerStatusSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemPowGetPhonePowerStatusMessage, transIdP)


#define TelIsOemInfGetEarBudStatusSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemInfGetEarBudStatusMessage, transIdP)

#define TelIsOemInfGetCallsDurationIsSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemInfGetCallsDurationMessage, transIdP)

#define TelIsOemInfResetCallsDurationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemInfResetCallsDurationMessage, transIdP)


#define TelIsOemNwkGetHomeNetworkIdSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkGetHomeNetworkIdMessage, transIdP)

#define TelIsOemNwkGetNetworkNameSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkGetNetworkNameMessage, transIdP)

#define TelIsOemNwkGetPreferredNetworksSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkGetPreferredNetworksMessage, transIdP)

#define TelIsOemNwkSetPreferredNetworkSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkSetPreferredNetworkMessage, transIdP)

#define TelIsOemNwkDeletePreferredNetworkSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkDeletePreferredNetworkMessage, transIdP)

#define TelIsOemNwkGetNetworksSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkGetNetworksMessage, transIdP)

#define TelIsOemNwkGetRegistrationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkGetRegistrationMessage, transIdP)

#define TelIsOemNwkGetCurrentRegistrationServiceStatusSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemNwkGetCurrentRegistrationServiceStatusMessage, transIdP)

#define TelIsOemStySetLockSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStySetLockMessage, transIdP)

#define TelIsOemStyGetLockSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStyGetLockMessage, transIdP)

#define TelIsOemStyUnblockCodeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStyUnblockCodeMessage, transIdP)

#define TelIsOemStyGetRemainingAttemptNumberSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStyGetRemainingAttemptNumberMessage, transIdP)

#define TelIsOemSpcGetCallsInformationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSpcGetCallsInformationMessage, transIdP)

#define TelIsOemSpcGetCallInformationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSpcGetCallInformationMessage, transIdP)

#define TelIsOemSpcDialSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSpcDialMessage, transIdP)
	
#define TelIsOemSpcExplicitCallTransferSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSpcExplicitCallTransferMessage, transIdP)
	
#define TelIsOemSpcTtySupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSpcTtyMessage, transIdP)


#define TelIsOemAvrPlaySampleSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrPlaySampleMessage, transIdP)

#define TelIsOemAvrDeleteSampleSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrDeleteSampleMessage, transIdP)

#define TelIsOemAvrGetSamplesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrGetSamplesMessage, transIdP)

#define TelIsOemAvrStop(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrStopMessage, transIdP)

#define TelIsOemAvrGetSampleStatus(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrGetSampleStatusMessage, transIdP)

#define TelIsOemAvrEnterVoiceRecognitionModeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrEnterVoiceRecognitionModeMessage, transIdP)

#define TelIsOemAvrRecordSampleSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAvrRecordSampleMessage, transIdP)


#define TelIsOemGprsSetContextSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetContextMessage, transIdP)

#define TelIsOemGprsGetContextSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetContextMessage, transIdP)

#define TelIsOemGprsGetDefinedCidsSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetDefinedCidsMessage, transIdP)

#define TelIsOemGprsSetAttachSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetAttachMessage, transIdP)

#define TelIsOemGprsGetAttachSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetAttachMessage, transIdP)

#define TelIsOemGprsSetPdpActivationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetPdpActivationMessage, transIdP)

#define TelIsOemGprsGetPdpActivationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetPdpActivationMessage, transIdP)

#define TelIsOemGprsGetPdpAddressSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetPdpAddressMessage, transIdP)

#define TelIsOemGprsSetNwkRegistrationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetNwkRegistrationMessage, transIdP)

#define TelIsOemGprsGetNwkRegistrationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetNwkRegistrationMessage, transIdP)

#define TelIsOemGprsSetSmsServiceSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetSmsServiceMessage, transIdP)

#define TelIsOemGprsGetSmsServiceSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetSmsServiceMessage, transIdP)

#define TelIsOemGprsActivateDataSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsActivateDataMessage, transIdP)

#define TelIsOemGprsSetQosRequestedSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetQosRequestedMessage, transIdP)

#define TelIsOemGprsGetQosRequestedSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetQosRequestedMessage, transIdP)

#define TelIsOemGprsSetQosMinimumSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetQosMinimumMessage, transIdP)

#define TelIsOemGprsGetQosMinimumSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetQosMinimumMessage, transIdP)

#define TelIsOemGprsGetQosCurrentSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetQosCurrentMessage, transIdP)

#define TelIsOemGprsSetEventReportingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsSetEventReportingMessage, transIdP)

#define TelIsGprsGetEventReportingSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemGprsGetEventReportingMessage, transIdP)


#define TelIsUssdSendCommandSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemUssdSendCommandMessage, transIdP)


#define TelIsAlertSilenceSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemAlertSilenceMessage, transIdP)

#define TelIsStySimLockDeactivateSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStySimLockDeactivateMessage, transIdP)

#define TelIsStySimLockStatusSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStySimLockStatusMessage, transIdP)

#define TelIsStySimLockResetAttemptCounterSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemStySimLockResetAttemptCounterMessage, transIdP)

#define TelIsOemSimReadElementaryFileSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSimReadElementaryFileMessage, transIdP)

#define TelIsOemSimGetIconPropertiesSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSimGetIconPropertiesMessage, transIdP)

#define TelIsOemSimGetIconSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSimGetIconMessage, transIdP)

#define TelIsOemSatInitializeSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatInitializeMessage, transIdP)

#define TelIsOemSatGetProactiveCommandParametersSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatGetProactiveCommandParametersMessage, transIdP)

#define TelIsOemSatSendProactiveCommandResponseSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatSendProactiveCommandResponseMessage, transIdP)

#define TelIsOemSatNotifyMenuSelectionSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatNotifyMenuSelectionMessage, transIdP)

#define TelIsOemSatNotifyPhoneActionSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatNotifyPhoneActionMessage, transIdP)

#define TelIsOemSatSendEventNotificationSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatSendEventNotificationMessage, transIdP)

#define TelIsOemSatPerformInternalTaskSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSatPerformInternalTaskMessage, transIdP)

#define TelIsOemSimGetSpnSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSimGetSpnMessage, transIdP)

#define TelIsOemHandleTimeChangeEventSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemHandleTimeChangeEvent, transIdP)
	
#define TelIsOemSimUpdateSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemSimUpdateMessage, transIdP)

#define TelOemCfgSetAudioPathGainSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgSetAudioPathGainMessage, transIdP)

#define TelOemCfgGetAudioPathGainSupported(refnum, appId, transIdP) \
	TelIsFunctionSupported(refnum, appId, kTelOemCfgGetAudioPathGainMessage, transIdP)

/*@}*/


#endif
