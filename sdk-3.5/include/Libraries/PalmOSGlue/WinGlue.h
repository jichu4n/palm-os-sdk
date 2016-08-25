/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: WinGlue.h
 *
 * Description:
 *         	Header file for WinGlue library routines.
 *
 * WinGlue provides compatibility for applications that wish to make
 *	calls to Window routines, but which might actually be running on devices
 *	with roms that do not have newer routines available.
 *
 * Written by TransPac Software, Inc.
 *
 * History:
 *			Created by Ken Krugler
 *		06/18/99	kwk	New today.
 *
 *****************************************************************************/

#ifndef __WINGLUE_H__
#define __WINGLUE_H__

#ifdef __cplusplus
	extern "C" {
#endif

void WinGlueDrawChar(WChar theChar, Int16 x, Int16 y);

void WinGlueDrawTruncChars(const Char* pChars, UInt16 length, Int16 x, Int16 y, Int16 maxWidth);

#ifdef __cplusplus
	}
#endif

#endif	// __WINGLUE_H__

