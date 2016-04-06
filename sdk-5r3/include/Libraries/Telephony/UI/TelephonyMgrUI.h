/******************************************************************************
 *
 * Copyright (c) 2000-2003 PalmSource, Inc. All rights reserved.
 *
 * File: TelephonyMgrUI.h
 *
 * Release: Palm OS 5 SDK (68K) R3.
 *
 * Description:
 *			This is the header
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
