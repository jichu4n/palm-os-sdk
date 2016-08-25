/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Font.h
 * AUTHOR:	 Art Lamb: September 13, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines font structures and routines.
 *
 * HISTORY:
 *		05/05/98	art	Add structures for font mapping table.
 *		07/03/98	kwk	Added FntWidthToOffset.
 *		10/23/98	kwk	Changed fontMapTable to 0xC000 (was 0xFFFF).
 *
 ***********************************************************************/
 
#ifndef __FONT_H__
#define __FONT_H__


// Font types in FontType structure.
#define fontMapTable				0xC000

#define  fntTabChrWidth      20
#define	fntMissingChar		  -1


typedef struct {
	SByte		offset;
	SByte		width;
} FontCharInfoType, * FontCharInfoPtr;

typedef struct {
	SWord fontType; 			// font type
	SWord firstChar; 			// ASCII code of first character
	SWord lastChar; 			// ASCII code of last character
	SWord maxWidth; 			// maximum character width
	SWord kernMax; 			// negative of maximum character kern
	SWord nDescent; 			// negative of descent
	SWord fRectWidth; 		// width of font rectangle
	SWord fRectHeight; 		// height of font rectangle
	SWord owTLoc; 				// offset to offset/width table
	SWord ascent; 				// ascent
	SWord descent; 			// descent
	SWord leading; 			// leading
	SWord rowWords; 			// row width of bit image / 2
} FontType;

typedef FontType * FontPtr;

typedef FontPtr * FontTablePtr;


typedef struct {
	DWord		rscType;
	Word		rscID;
} FontIndexEntryType, * FontIndexEntryPtr;


// Font mapping state table.
#define	fntStateIsChar			1
#define	fntStateNextIsChar	2

typedef struct {
	Byte	flags;
	Byte	state;
	Word	value;
} FontMapType, * FontMapPtr;



enum fontID { 
	stdFont = 0x00,					// Small font used for the user's writing.  Shows a good amount
	boldFont, 							// Small font.  Bold for easier reading.  Used often for ui.
	largeFont,							// Larger font for easier reading.  Shows a lot less.
	symbolFont,							// Various ui images like check boxes and arrows
	symbol11Font, 						// Larger various ui images
	symbol7Font,						// Smaller various ui images
	ledFont,								// Calculator specific font
	largeBoldFont,						// A thicker version of the large font.  More readable.
	fntAppFontCustomBase = 0x80	// First available application-defined font ID
};

typedef enum fontID FontID;

#define checkboxFont symbol11Font

#define FntIsAppDefined(fnt) (fnt >= fntAppFontCustomBase)



//--------------------------------------------------------------------
//
// Font Function
//
//--------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern FontID FntGetFont (void)
							SYS_TRAP(sysTrapFntGetFont);

extern FontID FntSetFont (const FontID font)
							SYS_TRAP(sysTrapFntSetFont);

extern FontPtr FntGetFontPtr (void)
							SYS_TRAP(sysTrapFntGetFontPtr);

extern SWord FntBaseLine (void)
							SYS_TRAP(sysTrapFntBaseLine);

extern SWord FntCharHeight (void)
							SYS_TRAP(sysTrapFntCharHeight);

extern SWord FntLineHeight (void)
							SYS_TRAP(sysTrapFntLineHeight);

extern SWord FntAverageCharWidth (void)
							SYS_TRAP(sysTrapFntAverageCharWidth);

extern SWord FntCharWidth (const Char ch)
							SYS_TRAP(sysTrapFntCharWidth);

extern SWord FntCharsWidth (const Char * chars, Word len)
							SYS_TRAP(sysTrapFntCharsWidth);

extern Word FntWidthToOffset(ConstCharPtr pChars, Word length,
	SWord pixelWidth, Boolean *leadingEdge, SWordPtr truncWidth)
							SYS_TRAP(sysTrapFntWidthToOffset);

extern void	FntCharsInWidth(const Char * string, SWord *stringWidthP,
	SWord *stringLengthP, Boolean *fitWithinWidth)
							SYS_TRAP(sysTrapFntCharsInWidth);

extern SWord FntDescenderHeight (void)
							SYS_TRAP(sysTrapFntDescenderHeight);

extern SWord FntLineWidth (const Char * pChars, Word length)
							SYS_TRAP(sysTrapFntLineWidth);

Word FntWordWrap (Char * const chars, const Word maxWidth)
							SYS_TRAP(sysTrapFntWordWrap);

void FntWordWrapReverseNLines (const Char * const chars, 
	const Word maxWidth, const WordPtr linesToScrollP, 
	const WordPtr scrollPosP)
							SYS_TRAP(sysTrapFntWordWrapReverseNLines);

void FntGetScrollValues (const Char * const chars, const Word width, 
	const Word scrollPos, const WordPtr linesP, 
	const WordPtr topLine)
							SYS_TRAP(sysTrapFntGetScrollValues);

extern Err FntDefineFont (FontID font, FontPtr fontP)
							SYS_TRAP(sysTrapFntDefineFont);

#ifdef __cplusplus
}
#endif


#endif // __FONT_H__



