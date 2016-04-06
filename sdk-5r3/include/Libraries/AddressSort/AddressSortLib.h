/******************************************************************************
 *
 * Copyright (c) 2002-2003 PalmSource, Inc. All rights reserved.
 *
 * File: AddressSortLib.h
 *
 * Release: Palm OS 5 SDK (68K) R3.
 *
 * Description:
 *	Include file for the Address Book to access sort routines
 *	found in an ARM library, for performance reasons.
 *
 *****************************************************************************/

#ifndef __ADDRESSSORTLIB_H__
#define	__ADDRESSSORTLIB_H__

#define AddressSortLibTrapAddrDBSort (sysLibTrapCustom)
#define AddressSortLibTrapAddrJDBSort (sysLibTrapCustom + 1)

// Type and creator of library.
#define	addrSortLibType		sysFileTLibrary
#define addrSortLibCreator	'adbs'

Err		AddrDBSort(UInt16 libRefnum, DmOpenRef dbR, Int16 other) 
			SYS_TRAP(AddressSortLibTrapAddrDBSort);

Err		AddrJDBSort(UInt16 libRefnum, DmOpenRef dbR, Int16 other) 
			SYS_TRAP(AddressSortLibTrapAddrJDBSort);


#endif 	//__ADDRESSSORTLIB_H__
