/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:	Pilot
 *
 * FILE:		ScrDriverNew.h
 *
 * AUTHOR:	Ron Marianetti
 *
 * DECLARER:
 *
 * DESCRIPTION:	Screen driver that supports color. Initially, only
 *		2 bit gray is implemented
 *	  
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			RM		11/21/96	Created
 *
 **********************************************************************/

#ifndef	__SCRDRIVER_H__
#define 	__SCRDRIVER_H__

#include <SystemMgr.h>


/**************************************************************************
 * Internal constants
 ***************************************************************************/
#define		scrMaxLineBytes		40			// Max # of bytes in a scan line
														// used in compression/decompression calls
#define		scrMaxDepth				2			// Same as hwrDisplayMaxDepth
														// used to reserve space for expanded patterns														
														// only used by WindowNew.c
														

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
 * Internal Drawing info structure used by Screen Driver. This structure is
 * used to hold local variables used by the drawing operations. 
 *
 * At the beginning of each drawing or blit operation, the basic fields
 *  in this structure are intialized using PrvGetBltInfo() 
 *  (for ScrCopyRectangle and ScrDrawChars) or PrvGetDrawInfo() (for
 *  ScrLineRoutine and ScrRectangleRoutine.
 *
 * Then the masks for the left and right edge and other line specific
 *  information is filled in by calling PrvGetLineInfo(). Most high
 *  level calls call PrvGetLineInfo() only once. The exception is 
 *  ScrDrawChars() because it essentially does a different bitblt
 *  operation for every character in the string. 
 * 
 ***************************************************************************/
typedef struct ScrBltInfoType {


	//-----------------------------------------------------------------------------
	// COMMON BASE INFO
	//............................................................................
	// The following fields are used by EVERY screen driver routine and are filled 
	//  in by both PrvGetBltInfo and  PrvGetDrawInfo.
	//-----------------------------------------------------------------------------
	ScrOperation		op;						// which drawing mode to use
	SWord					height;					// height of operation

	WordPtr				dstBaseP;				// base address of window we're blitting to
	DWord					dstRowBytes;			// rowBytes of window
	Word					dstDepth;				// bits/pixel
	Word					dstShift;				// log base 2 of dstDepth
	Word					dstPixelsInWordZ;		// 0x0F for 1 bit, 0x07 for 2 bit, etc. 
	
	
	Word					forePixels;				// packed indices of foreground color
														//  in destination bitmap 
	Word					backPixels;				// packed indices of background color
														//  in destination bitmap
	WordPtr				oneBitTableP;			// pointer to table that maps 1 bit source
														//  into destination depth sans color
	

	//-----------------------------------------------------------------------------
	// BLIT ONLY
	//............................................................................
	// The following fields are only used by blitting operations (ScrCopyRectangle
	//  and ScrDrawChars and are filled in by PrvGetBltInfo only
	//-----------------------------------------------------------------------------
	WordPtr				srcBaseP;				// base address of window we're blitting from
	DWord					srcRowBytes;			// rowBytes of window
	Word					srcDepth;				// bits/pixel
	Word					srcShift;				// log base 2 of srcDepth
	Word					srcPixelsInWordZ;		// 0x0F for 1 bit, 0x07 for 2 bit, etc. 

	WordPtr				convSrcP;				// pointer to buffer that can hold 1
														//  scanline of source bitmap converted
														//  to destination bitmap format. 
	Word					dynConvSrcP:1;			// true if convSrcP was dynamically allocated
														//  and needs to be freed
	
	
	//-----------------------------------------------------------------------------
	// DRAW ONLY
	//............................................................................
	// The following fields are only used by drawing operations (ScrDrawLineRoutine
	//  and ScrRectangleRoutine) and are filled in by PrvGetDrawInfo only
	//-----------------------------------------------------------------------------
	BytePtr				patternP;				// pointer to pattern to use				
	Boolean				simple;					// true if scrCopy with solid color
	
	
	
	//-----------------------------------------------------------------------------
	// COMMON LINE INFO 
	//............................................................................
	// The following fields are are filled in by PrvGetLineInfo. These are fields that
	//   would change with every character drawn in ScrDrawChars() but common to the 
	//   entire operation in the other routines (ScrCopyRectangle, ScrLineRouting, 
	//   ScrRectangleRoutine).
	//-----------------------------------------------------------------------------
	WordPtr				dstP;						// byte address of first dst line
	SDWord				dstRowDelta;			// delta bytes from row to row

	Word					leftMask;				// mask for leftmost word of row in dest
	Word					rightMask;				// mask for rightmost word of row in dest
	SWord					midWords;				// # of words between left and right mask in dest

	// The srcP and srcRowDelta fields are only filled in for blit operations 
	//  (when infoP->srcBaseP is not nil). 
	WordPtr				srcP;						// byte address of first source line
	SDWord				srcRowDelta;			// delta bytes from row to row
	Word					forward:1;				// true if copying forward, false if
														//  backward. 
	// The srcBytesInLine field is normally initialized by PrvGetDepthConvertInfo()
	//  and used by the appropriate depth conversion routine. It contains the number
	// of bytes of source needed to fill 1 scan line of destination
	Word					srcBytesInLine;

	} ScrBltInfoType;
typedef ScrBltInfoType* ScrBltInfoPtr;


/************************************************************
 * Function pointer to routine used to convert source bitmap into
 *  destination bitmap depth. It converts infoP->srcBytesInLine bytes
 *  of pixels from srcWP into the destination depth and stores them
 *  into infoP->convSrcP.
 *************************************************************/
typedef void (*ScrConvertDepthProcP)(WordPtr srcWP, ScrBltInfoPtr infoP);



/**************************************************************************
 * Structure of Screen Driver globals
 ***************************************************************************/
typedef struct ScrGlobalsType {
	GDeviceType			gDevice;					// GDevice info (rowBytes, depth, etc)
														// a pointer to this field is stored in
														// the gDeviceP field of onscreen windows
	CustomPatternType	grayPat;					// Gray pattern
	Boolean				doDrawNotify;			// call ScrDrawNotify after drawing
	AbsRectType			updateR;					// update rect when in remote mode
	SDWord				lastUpdate;				// Tickcount of last update

	// This buffer is used to hold a scan line of a source blit that
	// has been bit depth converted to match the destination bitmap.
	Word					expSrcSize;
	BytePtr				expSrcP;
	
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
					const Char * const chars, Word len, FontPtr fontPtr)
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
							
Err ScrDisplayMode(ScrDisplayModeOperation operation, DWordPtr widthP,
					DWordPtr heightP, DWordPtr depthP, BooleanPtr enableColorP)
				SYS_TRAP(sysTrapScrDisplayMode);


#ifdef __cplusplus 
}
#endif



#endif // __SCRDRIVER_H__
