/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/
/** 
 *  @ingroup FotoFinish
 **/

/**
 * @file SaveUtils.h
 * This file contains code for saving a photo as a JPEG image on the device
 */

/** @name
 *
**/
/*@{*/

/***********************************************************************
 *
 * File:
 *	SaveUtils.h
 *
 * Description:
 *	Contains code for saving a photo as a JPEG image on the handheld
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/
 
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

Err SavePhoto(UInt16 gLibRefNum, WinHandle gBackBufferWinH, Char *fileName, UInt16 albID, PalmPhotoHandle newPhotoH, UInt32 gPhotoWidth, UInt32 gPhotoHeight);
Err	PalmPhotoWriteCallBack(void *bufferP, UInt32 *sizeP, void *userDataP);

#define Swap16(n)	(((((UInt16) n) << 8) & 0xFF00) | \
					 ((((UInt16) n) >> 8) & 0x00FF))


#endif

/*@}*/
