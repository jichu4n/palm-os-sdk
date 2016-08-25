/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot 2.0
 * FILE:     PhoneLookup.h
 * AUTHOR:	 Art Lamb: July 23, 1996
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines phone number lookup structures and routines.
 *
 ***********************************************************************/
#ifndef __PHONE_LOOKUP_H__
#define __PHONE_LOOKUP_H__


#ifdef __cplusplus
extern "C" {
#endif

extern void PhoneNumberLookup (const FieldPtr fld)
			SYS_TRAP(sysTrapPhoneNumberLookup);

#ifdef __cplusplus 
}
#endif


#endif	// __PHONE_LOOKUP_H__
