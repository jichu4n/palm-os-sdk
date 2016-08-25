/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: DateGlue.h
 *
 * Description:
 *         	Header file for DateGlue library routines.
 *
 * DateGlue provides compatibility for applications that wish to make
 *	calls to date routines, but which might actually be running on devices
 *	with roms that do not have newer routines available.
 *
 * Written by TransPac Software, Inc.
 *
 * History:
 *			Created by Ken Krugler
 *		06/18/99	kwk	New today.
 *
 *****************************************************************************/

#ifndef __DATEGLUE_H__
#define __DATEGLUE_H__

#ifdef __cplusplus
	extern "C" {
#endif

void DateGlueToAscii(UInt8 months, UInt8 days, UInt16 years, DateFormatType dateFormat,
						Char* pString);

void DateGlueToDOWDMFormat(UInt8 month, UInt8 day, UInt16 year, DateFormatType dateFormat,
							Char* pString);


#ifdef __cplusplus
	}
#endif

#endif	// __DATEGLUE_H__

