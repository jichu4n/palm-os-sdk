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
 * @brief This is the header file declaring the data types used by the 
 *	    Telephony Utilities for Palm OS Wireless Telephony Add-on.
 *
 *
 *
 * @version Version 1.0 	- Initial Revision 11/01/2001		Ryan Robertson
 *
 * @file MobileServicesTypes.h
 *
 *
 **/




/******************************************************************************
 *
 * Copyright (c) 1999-2003 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MobileServicesTypes.h
 *
 * Release: 
 *
 * Description:
 *		This is the header file declaring the data types used by the 
 *		Telephony Utilities for Palm OS Wireless Telephony Add-on.
 *
 * History:
 *		11/01/2001		Ryan Robertson		Initial version.
 *
 *****************************************************************************/

#ifndef __MOBILE_SERVICES_TYPES_H__
#define __MOBILE_SERVICES_TYPES_H__

#include <SoundMgr.h>
#include "MobileServices.h"
#include "TelephonyMgrTypes.h"
#include "TelephonyMgrTypesOem.h"

/* Network Registration */
#define kMobileSvcNetworkPlmnMax						32

/* Profile */
#define kMobileSvcProfileInfoNameMax					32

//	----------------------------------------------------------------------
//	Notification
//	----------------------------------------------------------------------

/** 
 * @name Typedefs
 *
 */
/*@{*/

typedef struct _MobileSvcNotificationType
{
	UInt16 		version;
   	UInt32      notificationData; 	/* associated data if any */
	UInt32      notificationData2; 	/* associated data if any */
	UInt32		timeStamp;			/* time stamp */
	UInt16      notificationId;		/* what was the associated telephony event */
	UInt8		priority;			/* notification priority 0 == max, 255 == min */
	UInt8		reserved;
} MobileSvcNotificationType;

/*	----------------------------------------------------------------------	*/
/*	miscellaneous
	----------------------------------------------------------------------	*/

// Error handling
typedef struct _MobileSvcDisplayErrorType {	
	Err 		err;			// IN/OUT: 	The error to handle.  This would be modified if the error handler encountered an error
	UInt32		ioFlags;		// IN/OUT:  The io flags to use.
	Char*		message;		// IN:		Any additional error message to display.
	Int16		pinSize;		// IN/OUT:	The size of the pin code string.
	Char*		pin;			// IN:		The pin code string.  This is set if the error handler was handling a pin or puk error.
} MobileSvcDisplayErrorType;

// Phone number filtering
typedef struct _MobileSvcFilterPhoneType
{
	UInt16		type;		// [in] See TelephonyMgr.h (kTelNwkCDMA, kTelNwkGSM, etc)
	Char*		src;		// [in] The phone number to filter
	UInt16		srcCount;	// [in] The length of the phone number to filter (including null terminator)
	Char*		dst;		// [in/out] The filtered phone number
	UInt16		dstCount;	// [in/out] The length of the filtered phone number (including null terminator)	
} MobileSvcFilterPhoneType;

typedef struct _MobileSvcIsValidCharacterType
{
	UInt16		type;		// [in] See TelephonyMgr.h (kTelNwkCDMA, kTelNwkGSM, etc)
	Char		theChar;	// [in] The character to test.
	Boolean		isValid;	// [out] True if the character is valid
} MobileSvcIsValidCharacterType;

// Volume Control
typedef struct _MobileSvcVolumeControlType
{
	UInt8	type;			// [in] Which item to control Ð kVolumeControlSpeaker or kVolumeControlMicrophone.
	UInt8	levelMin;		// [out] The minimum allowed volume value.
	UInt8   levelMax;		// [out] The maximum allowed volume value.
	UInt8	levelCurrent;	// [out] The volume value choosen by the user.
} MobileSvcVolumeControlType;

// Format Network ID
typedef struct _MobileSvcFormatNetworkIDType
{
	UInt32	ID;			// [in]  Incoming 5-6 digit network id format is XXXYY[Y]
	Boolean	useAlpha;	// [in]  true: uses alpha code (USA-00), false formats numeric (310-00)
	Boolean	foundID;	// [out] Indicates whether country code found in database.
	char   	*results;	// [out] the output string - needs to be 8 chars long.
} MobileSvcFormatNetworkIDType;

/*	----------------------------------------------------------------------	*/
/*	Status
	----------------------------------------------------------------------	*/

// Status Display
typedef struct _MobileSvcDisplayStatusType
{
	UInt16			type;			// [in] The status item to draw.
	RectangleType	rect;			// [in] The location to draw the item at.
} MobileSvcDisplayStatusType;

// Signal Meter Display
typedef struct _MobileSvcDisplaySignalMeterType
{
	UInt8			bars;			// [in] The number of bars to draw (0-4).
	Boolean			drawMeter;  	// [in] True if the Utilities Library should draw the standard signal strength meter.
	RectangleType	rect;			// [in] The location to draw the signal meter at.
} MobileSvcDisplaySignalMeterType;

// Battery Meter Display
typedef struct _MobileSvcDisplayBatteryMeterType
{
	UInt8			batteryPercent;     // [in] The battery percent as returned by SysBatteryInfo.
	Boolean			charging;  			// [in] True if the device is charging from a power source.
	Boolean			drawMeter; 			// [in] True if the Utilities Library should draw the standard battery meter.
	UInt8			reserved;
	RectangleType	rect;  		   		// [in] The location to draw the battery meter at.
} MobileSvcDisplayBatteryMeterType;

/*	----------------------------------------------------------------------	*/
/*	Network Registration
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcNetworkRegistrationType
{
	UInt8		state;        						// [out] The current registration state.  See TelephonyMgrOem.h for more information on the constants.
	Boolean		emergency;							// [out] True if the mobile is currently in emergency call only mode.
	UInt8		networkStatus;						// [out] Network status on denial. See TelephonyMgrOem.h for defines
	Boolean		isCamped;							// [out] True is mobile is camping.
	Boolean		spnDisplayCondition;				// [out] True if the SPN should be displayed, false otherwise.
	UInt8		signal;								// [out] The current signal strength.
	Char		plmn[kMobileSvcNetworkPlmnMax];  	// [out] The plmn name to display from the network.
	Char		spn[kTelOemSimGetSpnMaxStringSize];	// [out] The SPN to display from the sim card.
	Boolean		roaming; 							// [out] True if the user is not on their home network.
	Boolean		divert;								// [out] Is call divert currently active?
	UInt16		profileId; 							// [out] The id of the currently active profile (or zero) if there is not an active profile).
	Boolean		gsmActive;							// [out] True if GSM PDP context is active
	Boolean		gprsActive;							// [out] True if GPRS PDP context is active
} MobileSvcNetworkRegistrationType;

/*	----------------------------------------------------------------------	*/
/*	Emergency Call
	----------------------------------------------------------------------	*/
typedef struct _MobileSvcEmergencyCallType
{
	UInt32			type;						// [in/out] dont know yet.
} MobileSvcEmergencyCallType;

/*	----------------------------------------------------------------------	*/
/*	Profiles
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcProfileType
{
	UInt16			defaultRscId;						// The resource ID of the default settings (internal use only).
	UInt16			reserved;							// Reserved
	UInt32			profileId;							// [in/out] The unique profile ID.
	UInt16			flags;								// [in/out] The flags for this entry
	UInt8			volume;								// [in/out] 0=off, 1=ascending, 2=low, 3=med, 4=high
	UInt8			vibrate;							// [in/out] 0=off, 1=on, 2=vib then ring
	UInt32			tune;								// [in/out] Unique ID of the tune to play.
	UInt8			divert;								// [in/out] kProfileDivertOff, kProfileDivertOn, kProfileDivertVoicemail
	Boolean			activate;							// [in/out] True to auto-activate.  This is only for car/cradle profiles
	Char			name[kMobileSvcProfileInfoNameMax];	// [in/out] The name of this setting entry
	Char			number[kTelMaxPhoneNumberLen];		// [in/out] The divert number.
} MobileSvcProfileType;

typedef struct _MobileSvcGetProfilesType
{
	UInt16					first;		/* [in] first entry database index, zero based */
	UInt16					count;		/* [in] number of name/addresse pairs */
	UInt32					userId;		/* [in] the user id to search for */
	MobileSvcProfileType*	entriesP;	/* [in/out] name/addresse pairs array */
} MobileSvcGetProfilesType;

typedef struct _MobileSvcCountProfilesType
{
	UInt16				count;					/* [out] number of name/addresse pairs */
	UInt32				userId;					/* [in] the user id to search for */
} MobileSvcCountProfilesType;

typedef struct _MobileSvcProfileIndexType
{
	UInt16				profileIndex;			/* [in] the index of the profile */
	UInt32				profileId;				/* [out] the id of the profile */
} MobileSvcProfileIndexType;

/*	----------------------------------------------------------------------	*/
/*	Ringing Tones
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcRingingToneAddType
{
	void*			tune;						// [in] The tune data
	UInt32			tuneLength;					// [in] The length of the tune data.
	Char			name[sndMidiNameLength];	// [in] The name of the tune.
	UInt32			uniqueId;					// [out] The id of the newly added tone.
} MobileSvcRingingToneAddType;

typedef struct _MobileSvcRingingToneCopyType
{
	UInt32			uniqueId;					// [in] The unique id of the tune to copy.
	MemHandle		tune;						// [in/out] The handle with tune data. 
} MobileSvcRingingToneCopyType;

typedef struct _MobileSvcRingingToneConvertType
{
	UInt8			format;						// [in] The tune format kMobileSvcRingingToneNokia
	UInt8			reserved;
	void*			tune;						// [in] The tune data
	UInt32			tuneLength;					// [in] The length of the tune data.
	Char			name[sndMidiNameLength];	// [out] The name of the tune.
	MemHandle		palmTune;					// [in/out] The handle with converted tune data. 	
} MobileSvcRingingToneConvertType;

/*	----------------------------------------------------------------------	*/
/*	Power
	----------------------------------------------------------------------	*/
typedef enum
{
	MobileSvcPowerPhoneOff,
	MobileSvcPowerPhoneOn,
	MobileSvcPowerPhoneOnWithReconnect,			// Special power state used to poweOn the phone after a BBP crash.  (Reserved for internal use only!!!)
	MobileSvcPowerPhoneReset
} MobileSvcPowerStateType;

typedef struct _MobileSvcPowerPhoneType
{
	MobileSvcPowerStateType		state;		// [in] MobileSvcPowerPhoneOn to power on the phone, MobileSvcPowerPhoneOff to power off.
	Boolean						ask;		// [in] True to prompt the user first, false otherwise.
	Boolean						urgent;		// [in] True to do an emergency shutdown if state is also false.
	UInt8						reserved;
} MobileSvcPowerPhoneType;

typedef struct _MobileSvcPowerGetStatusType {	
	Boolean 	powered;	// OUT: 	Set upon power completion.  Set to true if the chip-set is powered on otherwise set to false.
	UInt8		pinState;	// OUT:		Set upon power completion.  Set to the current PIN state of the device
	Err			err;		// OUT:		Set upon power completion.  Set to any errors that occurred during the power on/off process.
} MobileSvcPowerGetStatusType;

/*	----------------------------------------------------------------------	*/
/*	Address Book Utilities
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcCallerIDLookupType
{
	MemHandle 	name;							// OUT:		The name of the matching record if one is found.
	Char 		phone[kTelMaxPhoneNumberLen];	// IN/OUT:	The phone number of the matching record if one is found.
	UInt32 		addrRecordUID;					// OUT:		The unique ID of the matching record.
	Char		phoneLabel;						// OUT:		The address book phone label of the phone number
	Boolean		match;							// OUT:		Set to true if a matching record was found.
} MobileSvcCallerIDLookupType;

typedef struct _MobileSvcDialInfoType
{	
	UInt32 		addrUID;						// IN:		The unique ID of the address book entry.
	Char		phoneLabel;						// IN:		The address book phone label of the phone number
	UInt8		reserved1;
} MobileSvcDialInfoType;

typedef struct _MobileSvcAddToAddressType
{	
	UInt32 		recordId;						// IN:		The record id to add to the address book.
												//			or NULL if the request isn't associated
												//			with a record id.
	Char*		phone;							// IN:		The phone number to display as the default
	UInt16		phoneLength;					// IN:		The length of the phone number to display as default.							
	Char*		lastName;						// IN:		The last name to display as the default.
	UInt16		lastNameLength;					// IN:		The length of the last name
	Char*		firstName;						// IN:		The first name to display as the default.
	UInt16		firstNameLength;				// IN:		The length of the first name.
	UInt32		addrRecordUID;					// OUT:		The unique ID of the new record.
	Char		phoneLabel;						// OUT:		The phone label of the new record.
	UInt8		reserved2;
	MemHandle	name;							// OUT:		The name of the record that was added.
} MobileSvcAddToAddressType;

typedef struct _MobileSvcAddToNoteType
{
	UInt32 		recordId;						// IN:		The record Id to add the note to
												//			or NULL if the request isn't associated
												//			with a record id.
	UInt32 		addrUID;						// IN:		The unique ID of the address book entry to add a note to.
												//			This is ignored if a lineId is passed in.
	Char*		title;							// IN:		The title for the dialog. This is only used
												//			if this note is not associated with a line id.
} MobileSvcAddToNoteType;

/*	----------------------------------------------------------------------	*/
/*	Call State Structure
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcCountLinesType
{	
	UInt16		count;					/* [out] number of lines */
	UInt32		state;					/* [in] The state to filter on */				
} MobileSvcCountLinesType;

typedef struct _MobileSvcGetLineType
{	
	UInt16		index;			// [in] The index of the line.
	UInt16		state;			// [in] The state to search for.
	UInt32		recordId;		// [out] The unique id of the call history record associated with this line.
	UInt8		lineId;			// [out] The id of this line -- from the chipset.
	Boolean		mpty;			// [out] Is this part of a multi-party call
	Char		phoneLabel;		// [out] The label of the phone if an address book match exists	
	UInt8		dir;			// [out] The direction of the line.	
	UInt32		time;			// [out] The start time of the call
	UInt32		addrRecordID;	// [out] The unique ID of the address book record for this call
	Char*		phone;			// [out] The call phone number
	UInt16		phoneLength;	// [in/out] The length of the call phone number
	Char*		name;			// [out] The callee's name
	UInt16		nameLength;		// [in/out] The length of the callee's name							
} MobileSvcGetLineType;

/*	----------------------------------------------------------------------	*/
/*	Call History Structure
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcCountHistoryType
{	
	UInt16		count;					/* [out] number of name/addresse pairs */
	UInt16		dir;					/* [in] The direction to filter on */				
} MobileSvcCountHistoryType;

typedef struct _MobileSvcDeleteHistoryType
{	
	UInt16		index;					/* [in] The index to delete */				
} MobileSvcDeleteHistoryType;

typedef struct _MobileSvcGetHistoryType
{	
	UInt16		index;			// [in] The index of the history record.
	UInt16		dir;			// [in/out] The direction to search for.
	UInt32		recordId;		// [in/out] The unique id of this call history record
	UInt32		time;			// [out] The start time of the call
	UInt32		length;			// [out] The length of the call
	UInt32		addrRecordID;	// [out] The unique ID of the address book record for this call
	Char		phoneLabel;		// [out] The label of the phone if an address book match exists
	UInt8		reserved;
	Char*		phone;			// [out] The call phone number
	UInt16		phoneLength;	// [in/out] The length of the call phone number
	Char*		name;			// [out] The callee's name
	UInt16		nameLength;		// [in/out] The length of the callee's name						
} MobileSvcGetHistoryType;

/*	----------------------------------------------------------------------	*/
/*	Call Management Structure
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcCallIncomingType
{	
	UInt8		lineId;			
} MobileSvcCallIncomingType;

typedef struct _MobileSvcCallDialPhoneType
{	
	Char*		phone;			// [IN] The phone number to call
	Char*		name;			// [IN] The name associated with the phone number.
	UInt32		recordId;		// [IN] The record ID to call.
	Boolean		callerId;		// [IN] True if a caller id lookup should be performed.
	Boolean		command;		// [IN] True if this is a command call (ie: USSD or SSD).
} MobileSvcCallDialPhoneType;

/*	----------------------------------------------------------------------	*/
/*	Control Structure
	----------------------------------------------------------------------	*/

typedef struct _MobileSvcControlType
{	
	UInt8 		type;						// IN:		The type of control data attached
	UInt8		reserved;					//				(kMobileSvcControlTypeXXX constants)
	void*		data;						// IN:		The control data.
} MobileSvcControlType;

/*	----------------------------------------------------------------------	*/
/*	SIM Call Diverts Structures
	----------------------------------------------------------------------	*/
typedef struct _MobileSvcSimGetDivertsType
{
	Boolean		voice;						// OUT:	Voice Diverted
	Boolean		fax;						// OUT:	Fax Diverted
	Boolean		data;						// OUT:	Data Diverted
} MobileSvcSimGetDivertsType;

typedef struct _MobileSvcSimSetDivertsType
{
	UInt16		callType;					// IN:	Call Type to be diverted
											//		kTelOemCfgCallTypeVoice,
											//		kTelOemCfgCallTypeData or
											//		kTelOemCfgCallTypeFax
	Boolean		enable;						// IN:	true to set the divert flag,
											//      false to clear it
} MobileSvcSimSetDivertsType;

/*@}*/


#endif
