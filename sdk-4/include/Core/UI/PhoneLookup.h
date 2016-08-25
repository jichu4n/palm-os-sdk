/******************************************************************************
 *
 * Copyright (c) 1994-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: PhoneLookup.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines phone number lookup structures and routines.
 *
 * History:
 *		July 23, 1996	Created by Art Lamb
 *		March 24, 2000 Ludovic Ferrandis: Add custom API
 *
 *****************************************************************************/

#ifndef __PHONE_LOOKUP_H__
#define __PHONE_LOOKUP_H__

#include <PalmTypes.h>
#include <CoreTraps.h>
#include <AppLaunchCmd.h>

#include <Field.h>
#include <DataMgr.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void PhoneNumberLookup (FieldType *fldP)
			SYS_TRAP(sysTrapPhoneNumberLookup);

extern void PhoneNumberLookupCustom (FieldType *fldP, AddrLookupParamsType* params, Boolean useClipboard)
			SYS_TRAP(sysTrapPhoneNumberLookupCustom);

#ifdef __cplusplus 
}
#endif


#endif	// __PHONE_LOOKUP_H__
