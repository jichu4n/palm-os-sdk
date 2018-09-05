/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: DateGlue.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		Header file for DateGlue library routines.
 *
 *		DateGlue provides compatibility for applications that wish to make
 *		calls to date routines, but which might actually be running on devices
 *		with roms that do not have newer routines available.
 *
 *****************************************************************************/

#ifndef __DATEGLUE_H__
#define __DATEGLUE_H__

#include <DateTime.h>

#ifdef __cplusplus
	extern "C" {
#endif

UInt16 DateGlueTemplateToAscii(const Char* templateP, UInt8 months, UInt8 days,
	UInt16 years, Char* stringP, Int16 stringSize);

void DateGlueToDOWDMFormat(UInt8 month, UInt8 day, UInt16 year, DateFormatType dateFormat,
							Char* pString);


#ifdef __cplusplus
	}
#endif

#endif	// __DATEGLUE_H__
