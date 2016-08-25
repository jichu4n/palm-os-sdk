/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: UIResDefs.r
 *
 * Description:
 *		Type Declarations for Rez and DeRez
 *
 * Currently supported types:
 *		cnty, tAIB, taif, tAIN, tAIS, Talt, Tbmp, tbmf, tBTN, tCBX,
 *		tFBM, tFLD, tFRM, tgbn, tGDT, tgpb, tgrb, tGSI, tLBL, tLST,
 *		tPBN, tPUL, tPUT, tREP, tSCL, tSTL, tSTR, tTBL, tTTL, tver,
 *		tint, xprf, silk.
 *
 *	Note: only *all lowercase* entries are reserved.  We're OK with
 * 		the existing ones, but newly added resource types should be
 *		all downcased.
 *
 * Conditionally supported types from MPW's SysTypes.r:
 *		NFNT, FONT
 *		(If you need SysTypes.r file, include it BEFORE UIResDefs.r)
 *
 * Conditionally supported types from MPW's Types.r:
 *		ICON, MBAR, MENU, PICT
 *		(If you need Types.r file, include it BEFORE UIResDefs.r)
 *
 * Other known but UNsupported types:
 *		 feat (TMPL in ResEdit Preferences)
 *		 tkbd (in keyboard.rsrc, created by MakeKbd.c.)
 *
 * Usage:
 *		 Resource files can be DeRez'ed with the following command:
 *			derez MyRsrcFile.rsrc UIResDefs.r >MyRezSource.r
 *
 *		 Source files can be Rez'ed with the following command:
 *			rez UIResDefs.r MyRezSource.r -t rsrc -c RSED -o MyRsrcFile.rsrc
 *
 * Padding:
 *		 For some reason, some cstring types are defined in the ResEdit
 *		 templates as ECST and others are CSTR.  It would appear that
 *		 Rez aligns relative to the start of the resource, which doesn't
 *		 quite jive with ResEdit's even padded cstring type.  It's not clear
 *		 that PalmOS (RezConvert.cp for example) requires even padding.
 *		 All of the affected strings occur at the end of the resource type.
 *		 If the ECST's are important, these rez templates should be tweaked.
 *
 * History:
 *		08/29/96	SCL	Created
 *   	09/03/96	SCL	Added "Unique Currency Symbol" field to "cnty" resource
 *   	10/25/96 SCL	Changed "tFLD" resource to new 2.0 definition
 *   	02/07/99	SCL	Added largeBoldFont (new for Palm OS v3.0)
 *   	10/21/98 Bob	Added tgbn, tgpb, rbmf, and taif types
 *   	12/23/98 Bob	Added tAIB and Tbmp types
 *		06/29/99 CS 	Added tint type to support ResLoadConstant
 *		06/30/99 CS		Added measurementSystem field to "cnty" type,
 *							and added align statements to remove compiler
 *							dependencies.
 *		07/15/99	kwk	Added 'silk' resource typedef.
 *		09/20/99	gap	added additional cXXXX country values.
 *		09/20/99	gap	cPRC -> cRepChina.
 *
 *****************************************************************************/

#define alertType		informationAlert=0, confirmationAlert=1,		\
						warningAlert=2, errorAlert=3

#define palmFont		stdFont=0, boldFont=1, largeFont=2,				\
						symbolFont=3, checkboxFont=4, symbol11Font=4,	\
						symbol7Font=5, ledFont=6, largeBoldFont=7


/*======================================================================================*/
#ifndef __SYSTYPES_R__	/* If MPW's SysTypes.r was NOT included, */
						/* then define NFNT and FONT */

/*----------------------------FONT ¥ Font Description-----------------------------------*/
/* PROBLEMS: the offset to the offset/width table has been changed to a longint, with the
			 high word stored in the neg descent field (if its not -1).  Rez can't handle
			 this. */
type 'FONT' {
		/* Font Type Flags */
FontType:
		boolean = 1;											/* Reserved				*/
		boolean		doExpandFont, dontExpandFont;
		boolean		proportionalFont, fixedWidthFont;
		boolean = 1;											/* Reserved				*/
		unsigned bitstring[2] = 0;								/* Reserved				*/
		boolean		blackFont, colorFont;
		boolean		notSynthetic, isSynthetic;
		boolean		nofctbRsrc, hasfctbRsrc;
		unsigned bitstring[3] = 0;								/* Reserved				*/
		unsigned bitstring[2]	oneBit, twoBit,					/* Font depth			*/
								fourBit, eightBit;
		boolean		noCharWidthTable, hasCharWidthTable;
		boolean		noImageHeightTable, hasImageHeightTable;

	FirstChar:
		integer;												/* first char			*/
	LastChar:
		integer;												/* last char			*/
		integer;												/* width max			*/
		integer;												/* kern max				*/
		integer;												/* neg descent			*/
		integer;												/* font rect width		*/
	Height:
		integer;												/* font rect height		*/
	Offset:
		unsigned integer = ((WidthTable-Offset)/16);			/* offset to off/wid tab*/
		integer;												/* ascent				*/
		integer;												/* descent				*/
		integer;												/* leading				*/
	RowWords:
		integer;												/* row width (in words)	*/

		/* Tables */
		/* Bit image */
		hex string [($$Word(RowWords) <<($$BitField(FontType, 12, 2)
			& 3) + 1) * $$Word(Height)];

		/* Location Table */
		array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
			integer;
		};

		/* Offset/Width Table */
	WidthTable:
		array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
			integer;
		};

		/* Optional Character Width Table */
		Array [($$Word(FontType) & 0x0002) != 0] {
			array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
				integer;
			};
		};

		/* Optional Image Height Table */
		Array [($$Word(FontType) & 0x0001) != 0] {
			array [$$Word(LastChar) - $$Word(FirstChar) + 3] {
				integer;
			};
		};
};

/*----------------------------NFNT ¥ Font Description-----------------------------------*/
type 'NFNT' as 'FONT';

#endif // __SYSTYPES_R__


/*======================================================================================*/
#ifndef __TYPES_R__		/* If MPW's Types.r was NOT included, */
						/* then define ICON, MENU, MBAR, and PICT */

/*----------------------------ICON ¥ Icon-----------------------------------------------*/
type 'ICON' {
		hex string[128];										/* Icon data			*/
};


/*----------------------------MENU ¥ Menu-----------------------------------------------*/
type 'MENU' {
		integer;												/* Menu ID				*/
		fill word[2];
		integer 		textMenuProc = 0;						/* ID of menu def proc	*/
		fill word;
		unsigned hex bitstring[31]
						allEnabled = 0x7FFFFFFF;				/* Enable flags 		*/
		boolean 		disabled, enabled;						/* Menu enable			*/
		pstring 		apple = "\0x14";						/* Menu Title			*/
		wide array {
				pstring;										/* Item title			*/
				byte			noIcon; 						/* Icon number			*/
				char			noKey = "\0x00",				/* Key equivalent or	*/
								hierarchicalMenu = "\0x1B";		/* hierarchical menu	*/
				char			noMark = "\0x00",				/* Marking char or id	*/
								check = "\0x12";				/* of hierarchical menu	*/
				fill bit;
				unsigned bitstring[7]
								plain;							/* Style				*/
		};
		byte = 0;
};


/*----------------------------MBAR ¥ Menu Bar-------------------------------------------*/
type 'MBAR' {
		integer = $$CountOf(MenuArray); 						/* Number of menus		*/
		wide array MenuArray{
				integer;										/* Menu resource ID 	*/
		};
};

#ifndef __PICT_R__
/*----------------------------PICT ¥ Quickdraw Picture----------------------------------*/
type 'PICT' {
		unsigned integer;										/* Length				*/
		rect;													/* Frame				*/
		hex string; 											/* Data 				*/
};
#endif // __PICT_R__
#endif // __TYPES_R__


#ifndef __PALMTYPES_R__
#define __PALMTYPES_R__


/*======================================================================================*/
type 'cnty' {
	wide array CountryArray {
		byte;											/* Number */
		align word;
		cstring[0x14];								/* Name */
		byte;											/* Date Format */
		byte;											/* Long Date Format */
		byte;											/* Week Start Day */
		byte;											/* Time Format */
		byte;											/* Number Format */
		align word;
		cstring[0x14];								/* Currency Name */
		cstring[0x06];								/* Currency Symbol */
// Unique Currency Symbol field added 9/3/96 at Art's request...
		cstring[0x06];								/* Unique Currency Symbol */
		byte;											/* Currency Decimal Places */
		byte;											/* Daylight Savings Type */
		longint;										/* Minutes West of GMT */
		byte;											/* Measurement system (e.g., English, Metric) */
		align word;
	};
};	/* end cnty */


/*--------------------------------------------------------------------------------------*/
type 'tAIB' {
	array Images {
				switch {
					case Uncompressed:
						integer;  						/* width;						*/
	Height:			integer; 						/* height;						*/
	RowBytes:		integer; 						/* rowBytes;					*/
						key integer = 0; 				/* flags; 						*/
						byte;								/* pixelSize; bits/pixel 	*/
						byte;								/* version;						*/
	NextOffset:			integer;						/* nextDepthOffset - # of DWords to next BitmapType */
						fill byte;						/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						hex string[$$Word(RowBytes[$$ArrayIndex(Images)]) * $$Word(Height[$$ArrayIndex(Images)])];
						align LONG;
					
					case Compressed:
						integer;  						/* width;						*/
	HeightComp:		integer; 						/* height;						*/
	RowBytesComp:	integer; 						/* rowBytes;					*/
						key integer	= -32768; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel 	*/
						byte;								/* version;						*/
	NextOffsetComp:		integer;					/* nextDepthOffset - # of DWords to next BitmapType */
						fill byte;						/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						hex string[(($$Word(NextOffsetComp[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;

					case Uncompressed_HasTransparency:
						integer;  						/* width;						*/
	HeightT:			integer; 						/* height;						*/
	RowBytesT:		integer; 						/* rowBytes;					*/
						key integer = 8192; 			/* flags;						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetT:	integer;							/* nextDepthOffset - # of DWords to next BitmapType */
						unsigned byte;					/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						hex string[$$Word(RowBytesT[$$ArrayIndex(Images)]) * $$Word(HeightT[$$ArrayIndex(Images)])];
						align LONG;
					
					case Compressed_HasTransparency:
						integer;  						/* width;						*/
	HeightCompT:		integer; 					/* height;						*/
	RowBytesCompT:	integer; 						/* rowBytes;					*/
						key integer	= -24576; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel 	*/
						byte;								/* version;						*/
	NextOffsetCompT:integer;						/* nextDepthOffset - # of DWords to next BitmapType */
						unsigned byte;					/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						hex string[(($$Word(NextOffsetCompT[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;

					case Uncompressed_HasColorTable:
						integer;  						/* width;						*/
	HeightCT:		integer; 						/* height;						*/
	RowBytesCT:		integer; 						/* rowBytes;					*/
						key integer = 16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCT:	integer;							/* nextDepthOffset (dword)	*/
						fill byte;						/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[$$Word(RowBytesCT[$$ArrayIndex(Images)]) * $$Word(HeightCT[$$ArrayIndex(Images)])];
						align LONG;

					case Compressed_HasColorTable:
						integer;  						/* width;						*/
	HeightCompCT:	integer; 						/* height;						*/
	RowBytesCompCT:integer; 						/* rowBytes;					*/
						key integer = -16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCompCT:	integer;						/* nextDepthOffset (dword)	*/
						unsigned byte;					/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[(($$Word(NextOffsetCompCT[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;

					case Uncompressed_HasColorTable_HasTransparency:
						integer;  						/* width;						*/
	HeightCTT:		integer; 						/* height;						*/
	RowBytesCTT:		integer; 					/* rowBytes;					*/
						key integer = 16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCTT:	integer;							/* nextDepthOffset (dword)	*/
						unsigned byte;					/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[$$Word(RowBytesCTT[$$ArrayIndex(Images)]) * $$Word(HeightCTT[$$ArrayIndex(Images)])];
						align LONG;

					case Compressed_HasColorTable_HasTransparency:
						integer;  						/* width;						*/
	HeightCompCTT:	integer; 						/* height;						*/
	RowBytesCompCTT:integer; 						/* rowBytes;					*/
						key integer = -16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCompCTT:	integer;					/* nextDepthOffset (dword)	*/
						unsigned byte;					/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[(($$Word(NextOffsetCompCTT[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;
				};
	};
};	/* end tAIB */


/*--------------------------------------------------------------------------------------*/
type 'taif' {
	integer;											/* App Icon ID			*/
	integer;											/* width 				*/
	integer;											/* height 				*/
	integer = $$CountOf(PICTArray); 					/* Number of images		*/
	wide array PICTArray {
		integer;										/* depth 				*/
		byte		uncompressed=0, compressed=1;		/* compression flag		*/
		fill byte;
		byte		noTransprency=0, hasTransparency=1;	/* transparency flag	*/
		fill byte;
		integer;										/* PICT id for data 	*/
		integer;										/* compression type 	*/
		integer;										/* transparent index 	*/
		integer		reserved=0;							/* reserved, must be 0 (colorTable) */
	};
};	/* end taif */


/*--------------------------------------------------------------------------------------*/
type 'tAIN' {
	cstring;										/* App Icon Name */
	align word;										/* App Icon Name is defined in */
													/* TMPL as an even-padded cString. */
};	/* end tAIN */


/*--------------------------------------------------------------------------------------*/
type 'tAIS' {
													/* dmRecNumCategories = 16 categories */
	cstring;										/* Localize app info string[0] */
	cstring;										/* Localize app info string[1] */
	cstring;										/* Localize app info string[2] */
	cstring;										/* Localize app info string[3] */
	cstring;										/* Localize app info string[4] */
	cstring;										/* Localize app info string[5] */
	cstring;										/* Localize app info string[6] */
	cstring;										/* Localize app info string[7] */
	cstring;										/* Localize app info string[8] */
	cstring;										/* Localize app info string[9] */
	cstring;										/* Localize app info string[10] */
	cstring;										/* Localize app info string[11] */
	cstring;										/* Localize app info string[12] */
	cstring;										/* Localize app info string[13] */
	cstring;										/* Localize app info string[14] */
	cstring;										/* Localize app info string[15] */
};	/* end tAIS */


/*--------------------------------------------------------------------------------------*/
type 'Talt' {
	integer			alertType;						/* Alert Type */
	integer;										/* Help Rsc ID */
	integer;										/* # Buttons */
	integer;										/* Default Button ID */
	cstring;										/* Title */
	cstring;										/* Message */

	array ButtonArray {
		cstring;									/* Button Text */
	};
};	/* end Talt */


/*--------------------------------------------------------------------------------------*/
type 'Tbmp' {
	array Images {
				switch {
					case Uncompressed:
						integer;  						/* width;						*/
	Height:			integer; 						/* height;						*/
	RowBytes:		integer; 						/* rowBytes;					*/
						key integer = 0; 				/* flags; 						*/
						byte;								/* pixelSize; bits/pixel 	*/
						byte;								/* version;						*/
	NextOffset:			integer;						/* nextDepthOffset - # of DWords to next BitmapType */
						fill byte;						/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						hex string[$$Word(RowBytes[$$ArrayIndex(Images)]) * $$Word(Height[$$ArrayIndex(Images)])];
						align LONG;
					
					case Compressed:
						integer;  						/* width;						*/
	HeightComp:		integer; 						/* height;						*/
	RowBytesComp:	integer; 						/* rowBytes;					*/
						key integer	= -32768; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel 	*/
						byte;								/* version;						*/
	NextOffsetComp:		integer;					/* nextDepthOffset - # of DWords to next BitmapType */
						fill byte;						/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						hex string[(($$Word(NextOffsetComp[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;

					case Uncompressed_HasTransparency:
						integer;  						/* width;						*/
	HeightT:			integer; 						/* height;						*/
	RowBytesT:		integer; 						/* rowBytes;					*/
						key integer = 8192; 			/* flags;						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetT:	integer;							/* nextDepthOffset - # of DWords to next BitmapType */
						unsigned byte;					/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						hex string[$$Word(RowBytesT[$$ArrayIndex(Images)]) * $$Word(HeightT[$$ArrayIndex(Images)])];
						align LONG;
					
					case Compressed_HasTransparency:
						integer;  						/* width;						*/
	HeightCompT:		integer; 					/* height;						*/
	RowBytesCompT:	integer; 						/* rowBytes;					*/
						key integer	= -24576; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel 	*/
						byte;								/* version;						*/
	NextOffsetCompT:integer;						/* nextDepthOffset - # of DWords to next BitmapType */
						unsigned byte;					/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						hex string[(($$Word(NextOffsetCompT[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;

					case Uncompressed_HasColorTable:
						integer;  						/* width;						*/
	HeightCT:		integer; 						/* height;						*/
	RowBytesCT:		integer; 						/* rowBytes;					*/
						key integer = 16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCT:	integer;							/* nextDepthOffset (dword)	*/
						fill byte;						/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[$$Word(RowBytesCT[$$ArrayIndex(Images)]) * $$Word(HeightCT[$$ArrayIndex(Images)])];
						align LONG;

					case Compressed_HasColorTable:
						integer;  						/* width;						*/
	HeightCompCT:	integer; 						/* height;						*/
	RowBytesCompCT:integer; 						/* rowBytes;					*/
						key integer = -16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCompCT:	integer;						/* nextDepthOffset (dword)	*/
						unsigned byte;					/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[(($$Word(NextOffsetCompCT[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;

					case Uncompressed_HasColorTable_HasTransparency:
						integer;  						/* width;						*/
	HeightCTT:		integer; 						/* height;						*/
	RowBytesCTT:		integer; 					/* rowBytes;					*/
						key integer = 16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCTT:	integer;							/* nextDepthOffset (dword)	*/
						unsigned byte;					/* transparent index 		*/
						fill byte;						/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[$$Word(RowBytesCTT[$$ArrayIndex(Images)]) * $$Word(HeightCTT[$$ArrayIndex(Images)])];
						align LONG;

					case Compressed_HasColorTable_HasTransparency:
						integer;  						/* width;						*/
	HeightCompCTT:	integer; 						/* height;						*/
	RowBytesCompCTT:integer; 						/* rowBytes;					*/
						key integer = -16384; 		/* flags; 						*/
						byte;								/* pixelSize; bits/pixel	*/
						byte;								/* version;						*/
	NextOffsetCompCTT:	integer;					/* nextDepthOffset (dword)	*/
						unsigned byte;					/* transparent index 		*/
						byte;								/* compression type			*/
						fill word;						/* reserved						*/
						integer noColorTable = 0;	/* color table					*/
						hex string[(($$Word(NextOffsetCompCTT[$$ArrayIndex(Images)]) - 4) << 2)];
						align LONG;
				};
	};
}; /* end Tbmp */


/*--------------------------------------------------------------------------------------*/
type 'tbmf' {
	integer;											/* Bitmap ID			*/
	integer;											/* width 				*/
	integer;											/* height 				*/
	integer = $$CountOf(PICTArray); 			/* Number of images		*/
	wide array PICTArray {
		integer;										/* depth 				*/
		byte		uncompressed=0, compressed=1;		/* compression flag		*/
		fill byte;
		byte		noTransprency=0, hasTransparency=1;	/* transparency flag	*/
		fill byte;
		integer;										/* PICT id for data 	*/
		integer;										/* compression type 	*/
		integer;										/* transparent index 	*/
		integer		reserved=0;					/* reserved, must be 0 (colorTable) */
	};
};	/* end tbmf */


/*--------------------------------------------------------------------------------------*/
type 'tBTN' {
	integer;										/* Button ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			rightAnchor=0, leftAnchor=1;	/* Left Anchor */
	fill byte;
	byte			noFrame=0, frame=1;				/* Frame */
	fill byte;
	byte			boldFrame=0, nonBoldFrame=1;	/* Non-bold frame */
	fill byte;
	byte			palmFont;						/* Font ID */
	cstring;										/* Button Label */
};	/* end tBTN */


/*--------------------------------------------------------------------------------------*/
type 'tCBX' {
	integer;										/* Check Box ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			notSelected=0, selected=1;		/* Selected */
	fill byte;
	byte;											/* Group ID */
	byte			palmFont;						/* Font ID */
	cstring;										/* Check Box Label */
	align word;										/* Check Box Label is defined in */
													/* TMPL as an even-padded cString. */
};	/* end tCBX */


/*--------------------------------------------------------------------------------------*/
type 'tclt' {
	unsigned integer;								/* numEntries		*/
	wide array {
		unsigned byte;								/* index/reserved	*/
		unsigned hex byte;							/* red				*/
		unsigned hex byte;							/* green			*/
		unsigned hex byte;							/* blue				*/
	};
};	/* end tclt */


/*--------------------------------------------------------------------------------------*/
type 'tFBM' {
	integer;										/* X position */
	integer;										/* Y position */
	integer;										/* Bitmap Rsc ID */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
};	/* end tFBM */


/*--------------------------------------------------------------------------------------*/
type 'tFLD' {
	integer;										/* Field ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			notEditable=0, editable=1;		/* Editable */
	fill byte;
	byte			notUnderlined=0, underlined=1;	/* Underlined */
	fill byte;
	fill word;										/* Solid Underline (???) */
	byte			notSingleLine=0, singleLine=1;	/* Single Line */
	fill byte;
	byte			notDynamicSize=0, dynamicSize=1;		/* Dynamic Size */
	fill byte;
	byte			notLeftJustified=0, leftJustified=1;	/* Left Justified */
	fill byte;
	integer;										/* Max chars */
	byte			palmFont;						/* Font ID */
	fill byte;
	byte			notAutoShift=0, autoShift=1;		/* Autoshift */
	fill byte;
	byte			notHasScrollbar=0, hasScrollbar=1;	/* Has Scrollbar */
	fill byte;
	byte			notNumeric=0, numeric=1;			/* Numeric */
	fill byte;
};	/* end tFLD */


/*--------------------------------------------------------------------------------------*/
type 'tFRM' {
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			notModal=0, modal=1;			/* Modal */
	fill byte;
	byte			noSaveBehind=0, saveBehind=1;	/* Save behind */
	fill byte;
	fill word;
	fill word;
	integer;										/* Form ID */
	integer;										/* Help Rsc ID */
	integer;										/* Menu Rsc ID */
	integer;										/* Default Button ID */
	fill word;
	fill word;

	integer = $$Countof(ObjectArray);				/* Item count */
	wide array ObjectArray {
		integer;										/* Object ID */
		string[4];										/* Object Type */
	};
};	/* end tFRM */


/*--------------------------------------------------------------------------------------*/
type 'tgbn' {
	integer;										/* Graphic Button ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			rightAnchor=0, leftAnchor=1;	/* Left Anchor */
	fill byte;
	byte			noFrame=0, frame=1;				/* Frame */
	fill byte;
	byte			boldFrame=0, nonBoldFrame=1;	/* Non-bold frame */
	fill byte;
	integer;										/* Bitmap (Tbmp) ID */
	integer;										/* Selected Bitmap ID (0 for none) */
};	/* end tgbn */


/*--------------------------------------------------------------------------------------*/
type 'tGDT' {
	integer;										/* Gadget ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
};	/* end tGDT */


/*--------------------------------------------------------------------------------------*/
type 'tgpb' {
	integer;										/* Graphic Push Button ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte;											/* Group ID */
	fill byte;
	integer;										/* Bitmap (Tbmp) ID */
	integer;										/* Selected Bitmap ID (0 for none) */
};	/* end tgpb */


/*--------------------------------------------------------------------------------------*/
type 'tgrb' {
	integer;										/* Graphic Repeating Button ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			rightAnchor=0, leftAnchor=1;	/* Left Anchor */
	fill byte;
	byte			noFrame=0, frame=1;				/* Frame */
	fill byte;
	byte			boldFrame=0, nonBoldFrame=1;	/* Non-bold frame */
	fill byte;
	integer;										/* Bitmap (Tbmp) ID */
	integer;										/* Selected Bitmap ID (0 for none) */
};	/* end tgrb */


/*--------------------------------------------------------------------------------------*/
type 'tGSI' {
	integer;										/* X position */
	integer;										/* Y position */
};	/* end tGSI */


/*--------------------------------------------------------------------------------------*/
type 'tLBL' {
	integer;										/* Label ID */
	integer;										/* Left */
	integer;										/* Top */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			palmFont;						/* Font ID */
	cstring;										/* Label */
};	/* end tLBL */


/*--------------------------------------------------------------------------------------*/
type 'tLST' {
	integer;										/* Label ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			palmFont;						/* Font ID */
	fill byte;
	integer;										/* Visible Items */
	integer = $$Countof(StringArray);				/* Item count */
	array StringArray {
		cstring;										/* Label */
	};
};	/* end tLST */


/*--------------------------------------------------------------------------------------*/
type 'tPBN' {
	integer;										/* Push Button ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte;											/* Group ID */
	byte			palmFont;						/* Font ID */
	cstring;										/* Button Label */
};	/* end tPBN */


/*--------------------------------------------------------------------------------------*/
type 'tPUL' {
	integer;										/* Control ID */
	integer;										/* List ID */
};	/* end tPUL */


/*--------------------------------------------------------------------------------------*/
type 'tPUT' {
	integer;										/* Popup Trigger */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			rightAnchor=0, leftAnchor=1;	/* Left Anchor */
	fill byte;
	byte			palmFont;						/* Font ID */
	cstring;										/* Popup Label */
	align word;										/* Popup Label is defined in */
													/* TMPL as an even-padded cString. */
};	/* end tPUT */


/*--------------------------------------------------------------------------------------*/
type 'tREP' {
	integer;										/* Button ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	byte			rightAnchor=0, leftAnchor=1;	/* Left Anchor */
	fill byte;
	byte			noFrame=0, frame=1;				/* Frame */
	fill byte;
	byte			boldFrame=0, nonBoldFrame=1;	/* Non-bold frame */
	fill byte;
	byte			palmFont;						/* Font ID */
	cstring;										/* Button Label */
};	/* end tREP */


/*--------------------------------------------------------------------------------------*/
type 'tSCL' {
	integer;										/* Scroll Bar ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;			/* Usable */
	fill byte;
	integer;										/* Value */
	integer;										/* Minimum Value */
	integer;										/* Maximum Value */
	integer;										/* Page Size */
};	/* end tSCL */


/*--------------------------------------------------------------------------------------*/
type 'tsld' {
	integer;										/* Slider ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;	/* Usable */
	fill byte;
	integer;										/* Value */
	integer;										/* Minimum Value */
	integer;										/* Maximum Value */
	integer;										/* Page Size */
	byte			horizontal=0, vertical=1;	/* Horizontal? */
	fill byte;
	integer;										/* Thumb (Tbmp) ID, 0 for default */
	integer;										/* Background (Tbmp) ID, 0 for default */
};	/* end tSCL */


/*--------------------------------------------------------------------------------------*/
type 'tslf' {
	integer;										/* Feedback Slider ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notUsable=0, usable=1;	/* Usable */
	fill byte;
	integer;										/* Value */
	integer;										/* Minimum Value */
	integer;										/* Maximum Value */
	integer;										/* Page Size */
	byte			horizontal=0, vertical=1;	/* Horizontal? */
	fill byte;
	integer;										/* Thumb (Tbmp) ID, 0 for default */
	integer;										/* Background (Tbmp) ID, 0 for default */
};	/* end tSCL */


/*--------------------------------------------------------------------------------------*/
type 'tSTL' {		// Beware - Item count is one-based, but the system routine
					// must be passed a zero-based value to retrieve strings...
	cstring;										/* Prefix */

	integer = $$Countof(StringArray);				/* Item count */
	array StringArray {
		cstring;										/* Text */
	};
};	/* end tSTL */


/*--------------------------------------------------------------------------------------*/
type 'tSTR' {
	cstring;										/* The String */
													/* TMPL allows additional hex data */
};	/* end tSTR */


/*----------------------------tint ¥ Soft Constant--------------------------------------*/
type 'tint' {
	unsigned longint;								/* The Constant */
};	/* end tint */


/*--------------------------------------------------------------------------------------*/
type 'tTBL' {
	integer;										/* Table ID */
	integer;										/* Left */
	integer;										/* Top */
	integer;										/* Width */
	integer;										/* Height */
	byte			notEditable=0, editable=1;		/* Editable */
	fill byte;
	fill word;										/* reserved 1 */
	fill word;										/* reserved 2 */
	fill word;										/* reserved 3 */
	integer;										/* Rows */
	integer = $$Countof(ColumnArray);				/* Columns */
	array ColumnArray {
		integer;										/* Column Width */
	};
};	/* end tTBL */


/*--------------------------------------------------------------------------------------*/
type 'tTTL' {
	cstring;										/* Title */
	align word;										/* Title string is defined in */
													/* TMPL as an even-padded cString. */
};	/* end tTTL */


/*--------------------------------------------------------------------------------------*/
type 'tver' {
	cstring;										/* Version String */
	align word;										/* Version String is defined in */
													/* TMPL as an even-padded cString. */
};	/* end tver */


/*--------------------------------------------------------------------------------------*/
type 'xprf' {
	integer = 0x0001;								/* Version number */
	fill bit[31];									/* Reserved flags */
	boolean	allowOverlays, disableOverlays;			/* T->disable overlays */
};	/* end xprf */


/*--------------------------------------------------------------------------------------*/
#define	alphaGraffitiSilkscreenArea			0
#define	numericGraffitiSilkscreenArea		1

type 'silk' {
	integer = 0x0001;			// version
	
	literal longint;			// vendor creator ('psys' for 3Com)
	
	integer						// locale language
		lEnglish=0,
		lFrench,
		lGerman,
		lItalian,
		lSpanish,
		lJapanese=6;
	
	integer						// locale country
		cAustralia=0,
		cAustria,
		cBelgium,
		cBrazil,
		cCanada,
		cDenmark,
		cFinland,
		cFrance,
		cGermany,
		cHongKong,
		cIceland,
		cIreland,
		cItaly,
		cJapan,
		cLuxembourg,
		cMexico,
		cNetherlands,
		cNewZealand,
		cNorway,
		cSpain,
		cSweden,
		cSwitzerland,
		cUnitedKingdom,
		cUnitedStates,
		cIndia,
		cIndonesia,
		cKorea,
		cMalaysia,
		cRepChina,
		cPhilippines,
		cSingapore,
		cThailand,
		cTaiwan;
	
	integer = $$Countof(RectArray);	// rectangle count
	wide array RectArray {
		point;						// topLeft of bounds.
		point;						// extent of bounds.
		
		literal longint				// area type
			screenArea = 'scrn',
			graffitiArea = 'graf';
		
		integer;					// area index
	};
	
	integer = $$Countof(ButtonArray);
	wide array ButtonArray {
		point;						// topLeft of bounds.
		point;						// extent of bounds.
		
		hex integer					// keyDown.chr
			vchrMenu = 0x0105,
			vchrLaunch = 0x0108,
			vchrKeyboard = 0x0109,
			vchrFind = 0x010a,
			vchrCalc = 0x010b,
			vchrKeyboardAlpha = 0x0110,
			vchrKeyboardNumeric = 0x0111,
			vchrTsm1 = 0x0118,
			vchrTsm2 = 0x0119,
			vchrTsm3 = 0x011a,
			vchrTsm4 = 0x011b;
		hex integer;				// keyDown.keyCode
		hex integer					// keyDown.modifiers
			commandKeyMask = 0x0008;
	};
};

#endif // __PALMTYPES_R__

