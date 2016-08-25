/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: BmpGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *    Glue providing compatibility for applications that wish to make calls to
 *    some recent bitmap functions, but which might actually be running on a
 *    system which does not support newer calls.
 *
 * History:
 *		2000-12-22	jwm	Created.
 *
 *****************************************************************************/

#ifndef __BMPGLUE_H__
#define __BMPGLUE_H__

#include <Bitmap.h>

#ifdef __cplusplus
extern "C" {
#endif

void BmpGlueGetDimensions (const BitmapType *bitmapP,
                           Coord *widthP, Coord *heightP, UInt16 *rowBytesP);

UInt8 BmpGlueGetBitDepth (const BitmapType *bitmapP);

BitmapType *BmpGlueGetNextBitmap (BitmapType *bitmapP);

#ifdef __cplusplus
}
#endif

#endif
