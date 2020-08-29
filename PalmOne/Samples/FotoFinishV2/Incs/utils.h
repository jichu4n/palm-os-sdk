/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	utils.h
 *
 * Description:
 *	Contains utility functions used throughout the project
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/
 
#ifndef UTILS_H
#define UTILS_H

#include "PalmPhoto.h"

void	SetFieldText(FieldType *fieldP,Char *textP,Boolean blDraw);
Err		DisplayJpegAsBitmap(WinHandle tempH, Coord	topLeftX, Coord	topLeftY, UInt32 density);
void 	*GetSelectedPhotoExtraInformation(UInt16 gLibRefNum, PalmPhotoExtraInfoType infoType, PalmPhotoHandle selectedPhotoH);

Err 	SetSelectedPhotoExtraInformation(UInt16 gLibRefNum, PalmPhotoExtraInfoType infoType, PalmPhotoHandle selectedPhotoH, void *extraInfoP);
 
typedef struct _Text 
{
	Char text[40];
	Coord x, y;
}Text;

typedef struct _StraightLine 
{
	Coord startX, startY;
	Coord endX, endY;
}StraightLine;

typedef struct _FreeLine
{
	Coord *x;
	Coord *y;
	
	UInt16	numPoints;
}FreeLine; 
 	
typedef struct Operation
{
	union OpType
	{
		Text 			textOp;
		StraightLine	stLineOp;
		FreeLine		freeLineOp;
	}Op;
	
	enum{
		NO_OP,
		TEXT_OP,
		STLINE_OP,
		FREELINE_OP
	}Op_Codes;
	
	UInt8 OpCode;
	RGBColorType	color;
	
}Operation;

#define Swap16(n)	(((((UInt16) n) << 8) & 0xFF00) | \
					 ((((UInt16) n) >> 8) & 0x00FF))

#endif