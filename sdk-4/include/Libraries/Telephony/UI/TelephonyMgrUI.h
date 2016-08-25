/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TelephonyMgrUI.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *			This is the header
 *
 * History:
 *			May 23, 2000		ARO	Initial Release
 *			May,30, 2000		PPL	Add Pin code notification 
 *			Nov 11, 2000		PPL	Remove unused staff
 *			Dec 03, 2000		LFe add flags
 *
 *****************************************************************************/

#ifndef __TELEPHONYMGRUI_H__
#define __TELEPHONYMGRUI_H__

#include <PalmTypes.h>

/***********************************************************************
 * Structure definition - to move to a public header file
 ***********************************************************************/

#define kTelNotifyErrorDetailsVersion	1

#define telNotifyErrorEvent				'terr'
#define telNotifyEnterCodeEvent			'tpin'

#define kTelTryAgainBit					0x00000001
#define kTelNoSetUpButtonBit			0x00000002
#define kTelAutoTryAgainBit				0x00000004
#define kTelAutoSetUpButtonBit			0x00000008

typedef struct TelNotifyErrorDetailsTag
{
	UInt16	version;
	Err		error;	
	UInt32	ioFlags;	
	Char*	messageP;
} TelNotifyErrorDetailsType, *TelNotifyErrorDetailsPtr;


#endif // __TELEPHONYMGRUI_H__
