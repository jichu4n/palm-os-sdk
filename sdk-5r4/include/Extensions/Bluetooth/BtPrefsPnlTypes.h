/******************************************************************************
 *
 * Copyright (c) 2000-2004 PalmSource, Inc. All rights reserved.
 *
 * File: BtPrefsPnlTypes.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		Include file for Bluetooth Preferences Panel
 *
 *****************************************************************************/

#ifndef __BTPREFSPNLTYPES_H__
#define __BTPREFSPNLTYPES_H__

#include "BtLibTypes.h"

#define kBluetoothPanelPrefID			0x01
#define kBluetoothPanelPrefVersion		0x01

#ifndef sysFileCBluetoothPanel
#define	sysFileCBluetoothPanel			'abtp'	// Creator type for Bluetooth Panel
#endif


// default panel setting
#define BTPREFS_ALLOWWAKEUP_NO		false
#define BTPREFS_ALLOWWAKEUP_YES		true
#define BTPREFS_ALLOWWAKEUP_SCHEDULE	2

#define BTPREF_SCHEDULED_DAY_SUNDAY		0x0001
#define BTPREF_SCHEDULED_DAY_MONDAY		0x0002
#define BTPREF_SCHEDULED_DAY_TUESDAY	0x0004
#define BTPREF_SCHEDULED_DAY_WEDNESDAY	0x0008
#define BTPREF_SCHEDULED_DAY_THURSDAY	0x0010
#define BTPREF_SCHEDULED_DAY_FRIDAY		0x0020
#define BTPREF_SCHEDULED_DAY_SATURDAY	0x0040
#define BTPREF_SCHEDULED_DAY_BIT8		0x0080

#define BTLIB_DEFAULT_ACCESS			true
#define BTLIB_DEFAULT_DISCOVERABLE		true
#define BTLIB_DEFAULT_ALLOWWAKEUP		BTPREFS_ALLOWWAKEUP_NO
#define BTLIB_DEFAULT_USECACHE			true
#define BTLIB_DEFAULT_DISPLAYADDRESS	false
#define BTLIB_DEFAULT_DISPLAYPAIRED 	true
#define BTLIB_DEFAULT_DISPLAYLASTUSED 	false

#define BTPREFS_DEFAULT_SCHEDULED_START_HOURS	8
#define BTPREFS_DEFAULT_SCHEDULED_START_MIN		0
#define BTPREFS_DEFAULT_SCHEDULED_END_HOURS		18
#define BTPREFS_DEFAULT_SCHEDULED_END_MIN		0
#define BTPREFS_DEFAULT_SCHEDULED_DAYS			(BTPREF_SCHEDULED_DAY_MONDAY | BTPREF_SCHEDULED_DAY_TUESDAY | BTPREF_SCHEDULED_DAY_WEDNESDAY | BTPREF_SCHEDULED_DAY_THURSDAY | BTPREF_SCHEDULED_DAY_FRIDAY)

typedef struct _BluetoothPanelPrefsType {
	Boolean					bBlueTooth;		// Bluetooth On/Off
	Boolean					bDiscoverable;	// Can be discovered
	Char					friendlyName[btLibMaxDeviceNameLength];	// Device name, -HotSync- name as default
	Boolean					bAllowWakeup;	// Allow to wake-up
	UInt16					WakeupAlert;	// Alert type
	Boolean					bUseCache; // Method of getting the name from the cache or the remote device
	Boolean 					bDisplayAddress; // display device address instead of string unnamed <device type>
	Boolean 					bDisplayPaired; // display paired device at discovery start
	Boolean 					bDisplayLastUsed; // display last device use at discovery start
	UInt8 					numberOfLastUsed; // display last device use at discovery start
	UInt8					reserved1; 
	UInt8					reserved2;
	UInt8					reserved3;
	UInt8					reserved4;
	UInt8					reserved5;
	UInt8					reserved6;
} BluetoothPanelPrefsType;

// Service Panel parameter block structure for sysAppLaunchCmdPanelCalledFromApp
// The parameter block must be allocated using MemPtrNew, with the owner set to 0 (zero)
// using MemPtrSetOwner(void * p, UInt16 owner).  All data must be contained within the single
// memory chunk.
//
typedef enum SvcCalledFromAppCmdEnum {
	svcCFACmdSetBtOnOff = 1,				// Set prefs bluetooth on/off
	svcCFACmdLaunchTrustForm,				// Launch the Trusted Device Form
	svcCFACmdSetBtDiscoveryOnOff,			// Set prefs bluetooth discovery on/off

	// Add new commands here before svcCFACmdLast
	
	svcCFACmdLast
} SvcCalledFromAppCmdEnum;

typedef struct SvcCalledFromAppPBType {
	SvcCalledFromAppCmdEnum	cmd;					// command
	// Union of command data structures
	union {
		Boolean	bValue;
	} data;	
} SvcCalledFromAppPBType;

typedef SvcCalledFromAppPBType*	SvcCalledFromAppPBPtr;


#endif
