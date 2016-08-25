/******************************************************************************
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: DateGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Header file for DateGlue library routines.
 *
 *		DateGlue provides compatibility for applications that wish to make
 *		calls to date routines, but which might actually be running on devices
 *		with roms that do not have newer routines available.
 *
 * History:
 *		06/18/99	kwk	Created by Ken Krugler.
 *		01/31/00	kwk	Removed unimplemented DateGlueToAscii.
 *		12/14/00	kwk	Added DateGlueTemplateToAscii.
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
