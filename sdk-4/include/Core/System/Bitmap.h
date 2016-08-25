/******************************************************************************
 *
 * Copyright (c) 1994-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Bitmap.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *        This file defines bitmap structures and routines.
 *
 * History:
 *		September, 1999	Created by Bertrand Simon
 *			Name	Date		Description
 *			----	----		-----------
 *			BS		9/99		Create
 *			jmp	12/23/99	Fix <> vs. "" problem.
 *
 *****************************************************************************/

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <PalmOptErrorCheckLevel.h>		// #define ERROR_CHECK_LEVEL


//-----------------------------------------------
// The Bitmap Structure.
//-----------------------------------------------

// bitmap version numbers
#define BitmapVersionZero	0
#define BitmapVersionOne	1
#define BitmapVersionTwo	2

// Compression Types for BitmapVersionTwo
typedef enum 
	{
	BitmapCompressionTypeScanLine = 0,
	BitmapCompressionTypeRLE,
	BitmapCompressionTypePackBits,
	
	BitmapCompressionTypeEnd,		// must follow last compression algorithm
	
	BitmapCompressionTypeBest		= 0x64,			
	BitmapCompressionTypeNone = 0xFF
	} BitmapCompressionType;


typedef struct BitmapFlagsType
#ifdef ALLOW_ACCESS_TO_INTERNALS_OF_BITMAPS	// These fields will not be available in the next OS release!
{
	UInt16 	compressed:1;  				// Data format:  0=raw; 1=compressed
	UInt16 	hasColorTable:1;				// if true, color table stored before bits[]
	UInt16 	hasTransparency:1;			// true if transparency is used
	UInt16 	indirect:1;						// true if bits are stored indirectly
	UInt16 	forScreen:1;					// system use only
	UInt16	directColor:1;					// direct color bitmap
	UInt16 	reserved:10;
}
#endif
BitmapFlagsType;
	

// this definition correspond to the 'Tbmp' and 'tAIB' resource types
typedef struct BitmapType
#ifdef ALLOW_ACCESS_TO_INTERNALS_OF_BITMAPS	// These fields will not be available in the next OS release!
{
	Int16  				width;
	Int16  				height;
	UInt16  				rowBytes;
	BitmapFlagsType	flags;
	UInt8					pixelSize;			// bits/pixel
	UInt8					version;				// version of bitmap. This is vers 2
	UInt16	 			nextDepthOffset;	// # of DWords to next BitmapType
													//  from beginnning of this one
	UInt8					transparentIndex;	// v2 only, if flags.hasTransparency is true,
													// index number of transparent color
	UInt8					compressionType;	// v2 only, if flags.compressed is true, this is
													// the type, see BitmapCompressionType
														
	UInt16	 			reserved;			// for future use, must be zero!
	
	// if (flags.hasColorTable)
	//	  ColorTableType	colorTable		// NOTE: Could have 0 entries (2 bytes long)
	//
	// if (flags.directColor)
	//	  BitmapDirectInfoType	directInfo;
	// 
	// if (flags.indirect)
	//	  void*	  bitsP;						// pointer to actual bits
	// else
	//    UInt8	  bits[];					// or actual bits
	//
}
#endif
BitmapType;

typedef BitmapType* BitmapPtr;



// -----------------------------------------------
// This is the structure of a color table. It maps pixel values into
// RGB colors. Each element in the table corresponds to the next
// index, starting at 0.
// -----------------------------------------------
typedef struct RGBColorType
{
	UInt8           index;					// index of color or best match to cur CLUT or unused.
	UInt8           r;						// amount of red, 0->255
	UInt8           g;						// amount of green, 0->255
	UInt8           b;						// amount of blue, 0->255
}
RGBColorType;



// -----------------------------------------------
// For direct color bitmaps (flags.directColor set), this structure follows
//  the color table if one is present, or immediately follows the BitmapType if a
//  color table is not present. 
// The only type of direct color bitmap that is currently supported in version 3
//  of the Window Manager (feature: sysFtrCreator, #sysFtrNumWinVersion) are
//  16 bits/pixel with redBits=5, greenBits=6, blueBits=5. 
// -----------------------------------------------
typedef struct BitmapDirectInfoType
{
	UInt8		  		redBits;					// # of red bits in each pixel
	UInt8		  		greenBits;				// # of green bits in each pixel
	UInt8		  		blueBits;				// # of blue bits in each pixel
	UInt8		  		reserved;				// must be zero
	RGBColorType  transparentColor;		// transparent color (index field ignored) 
}
BitmapDirectInfoType;		  


// -----------------------------------------------
// Color Table
// -----------------------------------------------
typedef struct ColorTableType
{
	// high bits (numEntries > 256) reserved
	UInt16          numEntries;			// number of entries in table
	// RGBColorType entry[];    array 0..numEntries-1 of colors
	//									 starts immediately after numEntries
}
ColorTableType;


// get start of color table entries aray given pointer to ColorTableType
#define ColorTableEntries(ctP)	((RGBColorType *)((ColorTableType *)(ctP)+1))

#ifdef __cplusplus
extern          "C"
{
#endif

// -----------------------------------------------
// Routines relating to bitmap management       
// -----------------------------------------------

extern BitmapType* BmpCreate (Coord width, Coord height, UInt8 depth, ColorTableType * colortableP, UInt16 * error)
					SYS_TRAP (sysTrapBmpCreate);

extern Err      BmpDelete (BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpDelete);

extern Err      BmpCompress (BitmapType * bitmapP, BitmapCompressionType compType)
					SYS_TRAP (sysTrapBmpCompress);

extern void*    BmpGetBits (BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpGetBits);

extern ColorTableType* BmpGetColortable (BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpGetColortable);

extern UInt16   BmpSize (const BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpSize);

extern UInt16   BmpBitsSize (const BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpBitsSize);
					
extern void		 BmpGetSizes (const BitmapType * bitmapP, UInt32 * dataSizeP, UInt32 * headerSizeP)
					SYS_TRAP (sysTrapBmpGetSizes);

extern UInt16   BmpColortableSize (const BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpColortableSize);

extern void     BmpGetDimensions (const BitmapType * bitmapP, Coord * widthP, Coord * heightP, UInt16 * rowBytesP)
					SYS_TRAP (sysTrapBmpGetDimensions);

extern UInt8    BmpGetBitDepth (const BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpGetBitDepth);

extern BitmapType* BmpGetNextBitmap (BitmapType * bitmapP)
					SYS_TRAP (sysTrapBmpGetNextBitmap);

#ifdef __cplusplus
}
#endif


#endif							// __BITMAP_H__
