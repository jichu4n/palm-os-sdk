/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: PhoneLookup.h
 *
 * Description:
 *	  This file defines phone number lookup structures and routines.
 *
 * History:
 *		July 23, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#ifndef __PHONE_LOOKUP_H__
#define __PHONE_LOOKUP_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <Field.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void PhoneNumberLookup (FieldType *fldP)
			SYS_TRAP(sysTrapPhoneNumberLookup);

#ifdef __cplusplus 
}
#endif


#endif	// __PHONE_LOOKUP_H__

