/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     font.h
 * AUTHOR:	 Art Lamb: September 13, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines font structures and routines.
 *
 ***********************************************************************/
 
#ifndef __FONT_H__
#define __FONT_H__


typedef struct 
{
	SByte    offset;
	SByte		width;
} FontCharInfoType;

typedef struct{
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
 
enum fontID { stdFont, boldFont, largeFont, symbolFont, symbol11Font, 
				  symbol7Font, ledFont };

#define checkboxFont symbol11Font


typedef enum fontID FontID;


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

extern FontID FntSetFont (FontID font)
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

extern SWord FntCharWidth (Char ch)
							SYS_TRAP(sysTrapFntCharWidth);

extern SWord FntCharsWidth (Char *chars, Word len)
							SYS_TRAP(sysTrapFntCharsWidth);

extern void	FntCharsInWidth(CharPtr string, SWord *stringWidthP, 
		SWord *stringLengthP, Boolean *fitWithinWidth)
							SYS_TRAP(sysTrapFntCharsInWidth);

extern SWord FntDescenderHeight (void)
							SYS_TRAP(sysTrapFntDescenderHeight);

extern SWord FntLineWidth (CharPtr pChars, Word length)
							SYS_TRAP(sysTrapFntLineWidth);


#ifdef __cplusplus
}
#endif


#endif // __FONT_H__



