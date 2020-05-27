/***********************************************************************
 *
 * Copyright (c) 2003-2004 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	SaveUtils.h
 *
 * Description:
 *	Contains code for saving a photo as a JPEG image on the handheld
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision             (18-aug-2003)
 *          2.0     - remove byte-swapping code    (20-jan-2004)
 *
 ***********************************************************************/

#pragma once

#ifndef	SAVEUTILS_H_
#define	SAVEUTILS_H_

#include "PalmPhoto.h"

typedef struct _ImageData
{
	UInt16	*imgH;
	UInt32	offset;
	UInt16	rowBytes;
	Coord 	width;
	Coord	height;
}ImageData;

Err PalmPhotoReadCallBack(void *bufferP, UInt32 *sizeP, void *userDataP);
Err	PalmPhotoWriteCallBack(void *bufferP, UInt32 *sizeP, void *userDataP);
Err SavePhoto(UInt16 gLibRefNum, WinHandle gBackBufferWinH, Char *fileName, UInt16 albID, PalmPhotoHandle newPhotoH, UInt32 gPhotoWidth, UInt32 gPhotoHeight);

#define Swap16(n)	(((((UInt16) n) << 8) & 0xFF00) | \
					 ((((UInt16) n) >> 8) & 0x00FF))


#endif