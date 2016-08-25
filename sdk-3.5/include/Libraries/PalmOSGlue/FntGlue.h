/******************************************************************************
 *
 * Copyright (c) 1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: FntGlue.h
 *
 * Description:
 *              Glue providing compatibility for applications that wish
 *		to make calls to the Font Mgr, but which might actually be running
 *		on a system which does not support newer calls.
 *
 * History:
 *		06/18/99	kwk	Created by Ken Krugler.
 *		06/27/99	kwk	Added defaultBoldFont.
 *		09/25/99	kwk	Added FntGlueWidthToOffset.
 *
 *****************************************************************************/

#ifndef __FNTGLUE_H__
#define __FNTGLUE_H__

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

Int16 FntGlueWidthToOffset(const Char* charsP, UInt16 length, Int16 pixelWidth,
			Boolean* leadingEdge, Int16* truncWidth);

#ifdef __cplusplus
	}
#endif

#endif	// __FNTGLUE_H__

