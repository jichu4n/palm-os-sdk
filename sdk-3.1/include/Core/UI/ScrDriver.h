/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:	Pilot [EMULATOR]
 *
 * FILE:	ScrDriver.h
 *
 * AUTHOR:	Ain McKendrick
 *
 * DECLARER:
 *
 * DESCRIPTION:	Screen driver function prototypes.  For use by TD applications.
 *	  
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			Ain		1/25/95		Initial creation
 *			Ain		2/3/95		Revised to use new WinPtr type due to name
 *								changes in window.h
 *
 **********************************************************************/

// If using the version 2 Graphics, include the new Window.h
//  and ignore this one. Eventually, we will throw out support for
//  building version 1 and rename WindowNew.h as Window.h, etc.
#if 	GRAPHICS_VERSION == GRAPHICS_VERSION_2 
#include	<ScrDriverNew.h>
#else

#ifndef	__SCRDRIVER_H__
#define 	__SCRDRIVER_H__

#include <SystemMgr.h>


/**************************************************************************
 * Internal constants
 ***************************************************************************/
#define		scrMaxLineBytes		32			// Max # of bytes in a scan line
														// used in compression/decompression calls
														
/**************************************************************************
 * Operations for the ScrDisplayMode function
 ***************************************************************************/
typedef enum {
	scrDisplayModeGetDefaults,
	scrDisplayModeGet,
	scrDisplayModeSetToDefaults,
	scrDisplayModeSet,
	scrDisplayModeGetSupportedDepths,
	scrDisplayModeGetSupportsColor
} ScrDisplayModeOperation;


/**************************************************************************
 * Internal Drawing info structure used by Screen Driver
 ***************************************************************************/
//typedef enum {scrCopy, scrAND, scrANDNOT, scrXOR, scrOR, scrCopyNOT} ScrOperation;


typedef struct ScrBltInfoType {

	// Common fields.
	ScrOperation		op;						// which bit operation to do
	SWord					height;					// height of blit operation

	WordPtr				dstBaseP;				// base address of window we're blitting to
	Word					dstRowBytes;			// rowBytes of window

	WordPtr				srcBaseP;				// base address of window we're blitting from
	Word					srcRowBytes;			// rowBytes of window


	// Info for each line
	Word					leftMask;				// mask for leftmost byte of row
	Word					rightMask;				// mask for rightmost byte of row
	SWord					midWords;				// # of words between left and right mask

	SWord					dstRowDelta;			// delta bytes from row to row
	WordPtr				dstP;						// byte address of first dst line

	SWord					srcRowDelta;			// delta bytes from row to row
	WordPtr				srcP;						// byte address of first source line


	
	// These fields only used for drawing operations.
	Word*					patternP;				// pointer to pattern to use				
	Word					solidPat;				// color word if no pattern
														//DOLATER.. used to byte byte
	Boolean				simple;					// true if scrCopy with solid pattern
	CustomPatternType	pattern;					// copy of modified pattern
	
	} ScrBltInfoType;
typedef ScrBltInfoType* ScrBltInfoPtr;




/**************************************************************************
 * Structure of Screen Driver globals
 ***************************************************************************/
typedef struct ScrGlobalsType {
	Word					width;					// width of display in pixels
	Word					height;					// height of display in pixels
	Word					rowBytes;				// rowBytes of display
	Ptr					baseAddr;				// base address of display
	CustomPatternType	grayPat;					// Gray pattern
	Boolean				doDrawNotify;			// call ScrDrawNotify after drawing
	AbsRectType			updateR;					// update rect when in remote mode
	SDWord					lastUpdate;				// Tickcount of last update
	} ScrGlobalsType;
typedef ScrGlobalsType* ScrGlobalsPtr;




/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Initialization
void	ScrInit(void)
				SYS_TRAP(sysTrapScrInit);
				

// BitBlt Functions
Err	ScrCopyRectangle(WinPtr sourceWindow, WinPtr destWindow,
	 				SWord fromX, SWord fromY, SWord toX, SWord toY,
	 				SWord bitCount, SWord lineCount)
	 			SYS_TRAP(sysTrapScrCopyRectangle);
						

						
// Character Functions
void	ScrDrawChars(WinPtr pWindow, SWord xLoc, SWord yLoc, SWord xExtent, SWord yExtent,
					SWord clipTop, SWord clipLeft, SWord clipBottom, SWord clipRight,
					CharPtr chars, Word len, FontPtr fontPtr)
				SYS_TRAP(sysTrapScrDrawChars);

// Line Draw Functions
void 	ScrLineRoutine(WinPtr pWindow, SWord x1, SWord y1, SWord x2, SWord y2)
				SYS_TRAP(sysTrapScrLineRoutine);
				
void	ScrRectangleRoutine(WinPtr pWindow, SWord x, SWord y, SWord extentX, 
					SWord extentY)
				SYS_TRAP(sysTrapScrRectangleRoutine);

// Utility Functions
void 	ScrScreenInfo (WinPtr pWindow)
				SYS_TRAP(sysTrapScrScreenInfo);
				
void	ScrDrawNotify(SWord updLeft, SWord updTop,  SWord updWidth, SWord updHeight)
				SYS_TRAP(sysTrapScrDrawNotify);
				
void	ScrSendUpdateArea(Boolean force)		
				SYS_TRAP(sysTrapScrSendUpdateArea);

Word ScrCompressScanLine(BytePtr lineP, BytePtr prevLineP, Word width, 
					BytePtr dstParamP, Boolean firstLine)
				SYS_TRAP(sysTrapScrCompressScanLine);

Word ScrDeCompressScanLine(BytePtr srcP, BytePtr dstP, Word width)
				SYS_TRAP(sysTrapScrDeCompressScanLine);
							
Err ScrDisplayMode(ScrDisplayModeOperation operation,
 					DWordPtr widthP, DWordPtr heightP, DWordPtr depthP, BooleanPtr enableColorP)
				SYS_TRAP(sysTrapScrDisplayMode);


#ifdef __cplusplus 
}
#endif



#endif // __SCRDRIVER_H__
#endif // GRAPHICS_VERSION_2
