/******************************************************************************
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: FntGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Glue providing compatibility for applications that wish
 *		to make calls to the Font Mgr, but which might actually be running
 *		on a system which does not support newer calls.
 *
 * History:
 *		06/18/99	kwk	Created by Ken Krugler.
 *		06/27/99	kwk	Added defaultBoldFont.
 *		09/25/99	kwk	Added FntGlueWidthToOffset.
 *		05/12/00	kwk	Added FntGlueWCharWidth.
 *
 *****************************************************************************/

#ifndef __FNTGLUE_H__
#define __FNTGLUE_H__

#include <Font.h>

enum fontDefaults {
	defaultSmallFont = 0,
	defaultLargeFont,
	defaultBoldFont,
	defaultSystemFont
};
typedef enum fontDefaults FontDefaultType;

#ifdef __cplusplus
	extern "C" {
#endif

FontID FntGlueGetDefaultFontID(FontDefaultType inFontType);

Int16 FntGlueWCharWidth(WChar iChar);

Int16 FntGlueWidthToOffset(const Char* charsP, UInt16 length, Int16 pixelWidth,
			Boolean* leadingEdge, Int16* truncWidth);

#ifdef __cplusplus
	}
#endif

#endif	// __FNTGLUE_H__
