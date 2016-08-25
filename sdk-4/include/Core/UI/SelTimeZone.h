/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SelTimeZone.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines select time zone structures and routines.
 *
 * History:
 *		03/02/2000	peter	Created by Peter Epstein.
 *		08/02/2000	kwk	Updated APIs to include country code.
 *		08/21/2000	kwk	Deleted obsolete SelectDaylightSavingAdjustment.
 *		11/17/2000	CS		Change SelectTimeZone's ioCountryInTimeZoneP parameter
 *								to ioLocaleInTimeZoneP, (and anyCountry to anyLocale,
 *								but that doesn't really matter), since CountryType is
 *								only a UInt8, and this may change someday.
 *						CS		Change GetTimeZoneTriggerText's countryInTimeZone
 *								parameter to localeInTimeZoneP, since CountryType is
 *								only a UInt8, and this may change someday.
 *
 *****************************************************************************/

#ifndef	__SELTIMEZONE_H__
#define	__SELTIMEZONE_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <DateTime.h>
#include <LocaleMgr.h>		// LmLocaleType

#ifdef __cplusplus
extern "C" {
#endif

extern Boolean SelectTimeZone(Int16 *ioTimeZoneP, LmLocaleType* ioLocaleInTimeZoneP,
				const Char *titleP, Boolean showTimes, Boolean anyLocale)
						SYS_TRAP(sysTrapSelectTimeZone);

#ifdef __cplusplus 
}
#endif

#endif // __SELTIMEZONE_H__
