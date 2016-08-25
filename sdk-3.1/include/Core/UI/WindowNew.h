/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     WindowNew.h
 * AUTHOR:   Art Lamb: September 12, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines window structures and routines.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			RM		11/21/96	Updated to support color.
 *			kwk	07/03/98	Added WinDrawChar & WinDrawTruncChars.
 *
 **********************************************************************/
#ifndef __WINDOW_H__
#define __WINDOW_H__


enum directions { up = 0, down, left, right };
typedef enum directions DirectionType;


enum windowFormats { screenFormat = 0, genericFormat };
typedef enum windowFormats WindowFormatType;


typedef enum {scrCopy, scrAND, scrANDNOT, scrXOR, scrOR, scrCopyNOT} ScrOperation;


enum patterns { blackPattern, whitePattern, grayPattern, customPattern };
typedef enum patterns PatternType;

#define grayHLinePattern	0xAA

// grayUnderline means dotted black
// solidUnderline means solid black
// colorUnderline means solid, in whatever the foreground color is set to
enum underlineModes { noUnderline, grayUnderline, solidUnderline, colorUnderline };
typedef enum underlineModes UnderlineModeType;



typedef struct {
	Word compressed:1;  		// Data format:  0=raw; 1=compressed
	Word hasColorTable:1;	// if true, color table stored before bits[]
	Word reserved :14;
} BitmapFlagsType;


typedef struct {
	Word  					width;
	Word  					height;
	Word  					rowBytes;
	BitmapFlagsType		flags;
	Byte						pixelSize;			// bits/pixel
	Byte						version;				// version of bitmap. This is vers 1
	Word	 					nextDepthOffset;	// # of DWords to next BitmapType
														//  from beginnning of this one
	Word	 					reserved [2];
	
	// Byte					bits[]				// If pixelSize == 1:
	                                       //   this is the start of the pixels
	                                       // If pixelSize >= 2, we have:
	                                       //   ColorTableType followed by pixels. 
	                                       //   In most cases the ColorTableType will
	                                       //   have 0 entries however and be only 1
	                                       //   word long. 
} BitmapType;

typedef BitmapType * BitmapPtr;

typedef Word CustomPatternType [4];


// This is the structure of a color table. It maps pixel values into
//  RGB colors. Each element in the table corresponds to the next
//  index, starting at 0. A pointer to this table is stored in
//  the WindowExtType structure and a pointer to the WindowExtType is
//  stored in the WindowType. 
typedef struct RGBColorType {
	Byte	unused;									// unused, must be 0
	Byte	r;											// amount of red, 0->255
	Byte	g;											// amount of green, 0->255
	Byte	b;											// amount of blue, 0->255
	} RGBColorType;
	
typedef struct ColorTableType {
	Word							numEntries;		// number of entries in table
	RGBColorType				entry[1];		// array 0->numEntries-1 of colors
	} ColorTableType;
typedef ColorTableType* 	ColorTablePtr;



typedef struct {
	ScrOperation		grafMode;
	Boolean				patternMode;
	PatternType			pattern;
	CustomPatternType	customPtn;
	FontID				fontId;
	Byte					padding1;
	FontPtr           font;
	UnderlineModeType	underlineMode;
	RGBColorType		foreColor;
	RGBColorType		backColor;
} GraphicStateType;

typedef GraphicStateType * GraphicStatePtr;


typedef union {
	struct {
		Word cornerDiam   : 8;            // corner diameter, max 38
		Word reserved_3   : 3; 
		Word threeD			: 1;				 // Draw 3D button    
		Word shadowWidth  : 2;            // Width of shadow
		Word width        : 2;            // Width frame
	} bits;
	Word word;									// IMPORTANT: INITIALIZE word to zero before setting bits!
} FrameBitsType;

typedef Word FrameType;

//  Standard Frame Types
#define noFrame         0
#define simpleFrame     1
#define rectangleFrame  1
#define simple3DFrame   0x0012			 // 3d, frame = 2
#define roundFrame      0x0401          // corner = 7, frame = 1
#define boldRoundFrame  0x0702          // corner = 7, frame = 2
#define popupFrame      0x0205          // corner = 2,  frame = 1, shadow = 1
#define dialogFrame     0x0302          // corner = 3,  frame = 2
#define menuFrame       popupFrame


typedef struct {
	Word format:1;      // window format:  0=screen mode; 1=generic mode
	Word offscreen:1;   // offscreen flag: 0=onscreen ; 1=offscreen
	Word modal:1;       // modal flag:     0=modeless window; 1=modal window
	Word focusable:1;   // focusable flag: 0=non-focusable; 1=focusable
	Word enabled:1;     // enabled flag:   0=disabled; 1=enabled
	Word visible:1;     // visible flag:   0-invisible; 1=visible
	Word dialog:1;      // dialog flag:    0=non-dialog; 1=dialog
	Word reserved :9;
} WindowFlagsType;


// This structure is for use by the Window Manager and Screen Driver only!! It 
// may change size in the future. It contains information about
// the hardware device we're drawing to such as the base address,
// size, rowBytes, bit depth, etc. 
// A pointer to this structure is stored in the extendP field
// of a WindowType structure. This field used to be called viewOrigin
// in the B&W Window Manager and was unused. 
typedef struct GDeviceType {
	VoidPtr						baseAddr;		// base address
	Word							width;			// width in pixels
	Word							height;			// height in pixels
	Word							rowBytes;	 	// rowBytes of display
	Byte							pixelSize;		// bits/pixel
	Byte							version;			// version of GDeviceType
														//  - this is version 0
	// Flags -----
	Word							forDriver:1;	// true if this is the GDeviceType structure
														//  used by the screen driver.
	Word							dynamic:1;		// true if this structure was allocated
														//  using MemPtrNew and needs to be freed.
	Word							compressed:1;	// true if pixels are compressed.
	Word							flags:13;		// unused flags - all 0 for now

	// Color Table ----
	ColorTablePtr				cTableP;			// color table. If nil, then
														//  a default color table will be used. 
	} GDeviceType;
typedef GDeviceType* GDevicePtr;
	

// The Window structure.
typedef struct WinTypeStruct {
  Word							displayWidthV20;		// use WinGetDisplayExtent instead
  Word							displayHeightV20;		// use WinGetDisplayExtent instead
  VoidPtr						displayAddrV20;		// use the drawing functions instead
  WindowFlagsType				windowFlags;
  RectangleType				windowBounds;
  AbsRectType					clippingBounds;
  GDevicePtr					gDeviceP;
  FrameBitsType   			frameType;
  GraphicStatePtr				gstate;
  struct WinTypeStruct *	nextWindow;
} WindowType;

typedef WindowType * WinPtr;
typedef WinPtr   		WinHandle;

 



//-----------------------------------------------
//  Macros
//-----------------------------------------------

// For now, the window handle is a pointer to a window structure,
// this however may change, so use the following macros. 

#define WinGetWindowPointer(winHandle) ((WinPtr) winHandle)

#define WinGetWindowHandle(winPtr) ((WinHandle) winPtr)

#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
#define ECWinValidateHandle(winHandle) WinValidateHandle(winHandle)
#else
#define ECWinValidateHandle(winHandle) 
#endif

//-----------------------------------------------
// Routines relating to windows management       
//-----------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern Boolean WinValidateHandle (const WinHandle winHandle)
							SYS_TRAP(sysTrapWinValidateHandle);
							
extern WinHandle WinCreateWindow (RectanglePtr bounds, FrameType frame, 
	Boolean modal, Boolean focusable, WordPtr error)
							SYS_TRAP(sysTrapWinCreateWindow);

extern WinHandle WinCreateOffscreenWindow (SWord width, SWord height, 
	WindowFormatType format, WordPtr error)
							SYS_TRAP(sysTrapWinCreateOffscreenWindow);

extern void WinDeleteWindow (WinHandle winHandle, Boolean eraseIt)
							SYS_TRAP(sysTrapWinDeleteWindow);

extern void WinInitializeWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinInitializeWindow);

extern void WinAddWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinAddWindow);

extern void WinRemoveWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinRemoveWindow);

extern void WinMoveWindowAddr (WinPtr oldLocationP, WinPtr newLocationP)
							SYS_TRAP(sysTrapWinMoveWindowAddr);

extern void WinSetActiveWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinSetActiveWindow);

extern WinHandle WinSetDrawWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinSetDrawWindow);

extern WinHandle WinGetDrawWindow (void)
							SYS_TRAP(sysTrapWinGetDrawWindow);

extern WinHandle WinGetActiveWindow (void)
							SYS_TRAP(sysTrapWinGetActiveWindow);

extern WinHandle WinGetDisplayWindow (void)
							SYS_TRAP(sysTrapWinGetDisplayWindow);

extern WinHandle WinGetFirstWindow (void)
							SYS_TRAP(sysTrapWinGetFirstWindow);

extern void WinEnableWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinEnableWindow);

extern void WinDisableWindow (WinHandle winHandle)
							SYS_TRAP(sysTrapWinDisableWindow);

extern void WinGetWindowFrameRect (WinHandle winHandle, RectanglePtr r)
							SYS_TRAP(sysTrapWinGetWindowFrameRect);

extern void WinDrawWindowFrame (void)
							SYS_TRAP(sysTrapWinDrawWindowFrame);

extern void WinEraseWindow (void)
							SYS_TRAP(sysTrapWinEraseWindow);

extern WinHandle WinSaveBits (RectanglePtr source, WordPtr error)
							SYS_TRAP(sysTrapWinSaveBits);

extern void WinRestoreBits (WinHandle winHandle, SWord destX, SWord destY)
							SYS_TRAP(sysTrapWinRestoreBits);

extern void WinCopyRectangle (WinHandle srcWin, WinHandle dstWin, 
	RectanglePtr srcRect, SWord destX, SWord destY, ScrOperation mode)
							SYS_TRAP(sysTrapWinCopyRectangle);

extern void WinScrollRectangle
 (RectanglePtr r, enum directions direction, SWord distance, 
  RectanglePtr vacated)
							SYS_TRAP(sysTrapWinScrollRectangle);

extern void WinGetDisplayExtent (SWordPtr extentX, SWordPtr extentY)
							SYS_TRAP(sysTrapWinGetDisplayExtent);

extern void WinGetWindowBounds (RectanglePtr r)
							SYS_TRAP(sysTrapWinGetWindowBounds);

extern void WinSetWindowBounds (WinHandle winHandle, RectanglePtr r)
							SYS_TRAP(sysTrapWinSetWindowBounds);

extern void WinGetWindowExtent (SWordPtr extentX, SWordPtr extentY)
							SYS_TRAP(sysTrapWinGetWindowExtent);

extern void WinDisplayToWindowPt (SWordPtr extentX, SWordPtr extentY)
							SYS_TRAP(sysTrapWinDisplayToWindowPt);

extern void WinWindowToDisplayPt (SWordPtr extentX, SWordPtr extentY)
							SYS_TRAP(sysTrapWinWindowToDisplayPt);

extern void WinGetClip (RectanglePtr r)
							SYS_TRAP(sysTrapWinGetClip);

extern void WinSetClip (RectanglePtr r)
							SYS_TRAP(sysTrapWinSetClip);

extern void WinResetClip (void)
							SYS_TRAP(sysTrapWinResetClip);

extern void WinClipRectangle (RectanglePtr r)
							SYS_TRAP(sysTrapWinClipRectangle);

extern Boolean WinModal (WinHandle winHandle)
							SYS_TRAP(sysTrapWinModal);

//-----------------------------------------------
// Routines to draw shapes or frames shapes      
//-----------------------------------------------

extern void WinDrawLine (SWord x1, SWord y1, SWord x2, SWord y2)
							SYS_TRAP(sysTrapWinDrawLine);

extern void WinDrawGrayLine (SWord x1, SWord y1, SWord x2, SWord y2)
							SYS_TRAP(sysTrapWinDrawGrayLine);

extern void WinEraseLine (SWord x1, SWord y1, SWord x2, SWord y2)
							SYS_TRAP(sysTrapWinEraseLine);

extern void WinInvertLine (SWord x1, SWord y1, SWord x2, SWord y2)
							SYS_TRAP(sysTrapWinInvertLine);

extern void WinFillLine (SWord x1, SWord y1, SWord x2, SWord y2)
							SYS_TRAP(sysTrapWinFillLine);

extern void WinDrawRectangle (RectanglePtr r, Word cornerDiam)
							SYS_TRAP(sysTrapWinDrawRectangle);

extern void WinEraseRectangle (RectanglePtr r, Word cornerDiam)
							SYS_TRAP(sysTrapWinEraseRectangle);

extern void WinInvertRectangle (RectanglePtr r, Word cornerDiam)
							SYS_TRAP(sysTrapWinInvertRectangle);

extern void WinFillRectangle (RectanglePtr r, Word cornerDiam)
							SYS_TRAP(sysTrapWinFillRectangle);

extern void WinDrawRectangleFrame (FrameType frame, RectanglePtr r)
							SYS_TRAP(sysTrapWinDrawRectangleFrame);

extern void WinDrawGrayRectangleFrame (FrameType frame, RectanglePtr r)
							SYS_TRAP(sysTrapWinDrawGrayRectangleFrame);

extern void WinEraseRectangleFrame (FrameType frame, RectanglePtr r)
							SYS_TRAP(sysTrapWinEraseRectangleFrame);

extern void WinInvertRectangleFrame (FrameType frame, RectanglePtr r)
							SYS_TRAP(sysTrapWinInvertRectangleFrame);

extern void WinGetFramesRectangle (FrameType  frame, RectanglePtr r, 
	RectanglePtr obscuredRect)
							SYS_TRAP(sysTrapWinGetFramesRectangle);



//-----------------------------------------------
// Routines relating to characters               
//-----------------------------------------------

extern void WinDrawChar (WChar theChar, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawChar);

extern void WinDrawChars (const Char * const chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawChars);

extern void WinDrawInvertedChars (const Char * const chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawInvertedChars);

extern void WinDrawTruncChars(ConstCharPtr pChars, Word length, SWord x, SWord y, SWord maxWidth)
							SYS_TRAP(sysTrapWinDrawTruncChars);

extern void WinEraseChars (const Char * const chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinEraseChars);

extern void WinInvertChars (const Char * const chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinInvertChars);

extern UnderlineModeType WinSetUnderlineMode (UnderlineModeType mode)
							SYS_TRAP(sysTrapWinSetUnderlineMode);


//-----------------------------------------------
// Routines to patterns and colors                 
//-----------------------------------------------

extern void WinGetPattern (CustomPatternType)
							SYS_TRAP(sysTrapWinGetPattern);

extern void WinSetPattern (CustomPatternType)
							SYS_TRAP(sysTrapWinSetPattern);

//extern void WinSetPatternXferMode (enum patternXferModes mode);

//extern enum patternXferModes WinGetPatternXferMode (void);

#define WinSetForeColor(newColorP, oldColorP)		\
	WinSetColors(newColorP, oldColorP, 0, 0)
	
#define WinSetBackColor(newColorP, oldColorP)		\
	WinSetColors(0, 0, newColorP, oldColorP)
	
extern void WinSetColors(const RGBColorType* const newForeColorP, RGBColorType* oldForeColorP,
					const RGBColorType* const newBackColorP, RGBColorType* oldBackColorP)
							SYS_TRAP(sysTrapWinSetColors);


//-----------------------------------------------
// Routines relating to bitmaps            
//-----------------------------------------------
extern void WinDrawBitmap (BitmapPtr bitmapP, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawBitmap);


#ifdef __cplusplus 
}
#endif


#endif //__WINDOW_H__

