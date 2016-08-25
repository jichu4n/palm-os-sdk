/***********************************************************************
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     window.h
 * AUTHOR:       Art Lamb: September 12, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines window structures and routines.
 *
 **********************************************************************/

#ifndef __WINDOW_H__
#define __WINDOW_H__


enum directions { up = 0, down, left, right };
typedef enum directions DirectionType;


enum windowFormats { screenFormat = 0, genericFormat };
typedef enum windowFormats WindowFormatType;


//enum graphicModes { drawMode = 0, eraseMode, invertMode, mergeMode};
//typedef enum graphicModes GraphicModeType;
typedef enum {scrCopy, scrAND, scrANDNOT, scrXOR, scrOR, scrCopyNOT} ScrOperation;


enum patterns { blackPattern, whitePattern, grayPattern, customPattern };
typedef enum patterns PatternType;

#define grayHLinePattern	0xAA

enum underlineModes { noUnderline, grayUnderline, solidUnderline };
typedef enum underlineModes UnderlineModeType;



typedef struct {
	Word compressed:1;  	// Data format:  0=raw; 1=compressed
	Word reserved :15;
} BitmapFlagsType;


typedef struct {
	Word  					width;
	Word  					height;
	Word  					rowBytes;
	BitmapFlagsType		flags;
	Word	 					reserved [4];
} BitmapType;



typedef BitmapType * BitmapPtr;

typedef Word CustomPatternType [4];

typedef struct {
	ScrOperation		grafMode;
	Boolean				patternMode;
	PatternType			pattern;
	CustomPatternType	customPtn;
	FontID				fontId;
	Byte					padding1;
	FontPtr           font;
	UnderlineModeType	underlineMode;
} GraphicStateType;

typedef GraphicStateType * GraphicStatePtr;


typedef union {
	struct {
		Word cornerDiam   : 8;            // corner diameter, max 38
		Word reserved     : 4;     
		Word shadowWidth  : 2;            // Width of shadow
		Word width        : 2;            // Width frame
	} bits;
	Word word;
} FrameBitsType;

typedef Word FrameType;

//  Standard Frame Types
#define noFrame         0
#define simpleFrame     1
#define rectangleFrame  1
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
	Word compressed:1;	// compressed flag:  0=uncompressed; 1=compressed
	Word reserved :8;
} WindowFlagsType;


typedef struct WinTypeStruct {
  Word							displayWidth;
  Word							displayHeight;
  VoidPtr						displayAddr;
  WindowFlagsType				windowFlags;
  RectangleType				windowBounds;
  AbsRectType					clippingBounds;
  PointType						viewOrigin;
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


//-----------------------------------------------
// Routines relating to windows management       
//-----------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

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

extern void WinDrawChars (CharPtr chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawChars);

extern void WinDrawInvertedChars (CharPtr chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawInvertedChars);

extern void WinEraseChars (CharPtr chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinEraseChars);

extern void WinInvertChars (CharPtr chars, Word len, SWord x, SWord y)
							SYS_TRAP(sysTrapWinInvertChars);

extern UnderlineModeType WinSetUnderlineMode (UnderlineModeType mode)
							SYS_TRAP(sysTrapWinSetUnderlineMode);


//-----------------------------------------------
// Routines to control patterns                  
//-----------------------------------------------

extern void WinGetPattern (CustomPatternType)
							SYS_TRAP(sysTrapWinGetPattern);

extern void WinSetPattern (CustomPatternType)
							SYS_TRAP(sysTrapWinSetPattern);

//extern void WinSetPatternXferMode (enum patternXferModes mode);

//extern enum patternXferModes WinGetPatternXferMode (void);


//-----------------------------------------------
// Routines relating to bitmaps            
//-----------------------------------------------
extern void WinDrawBitmap (BitmapPtr bitmapP, SWord x, SWord y)
							SYS_TRAP(sysTrapWinDrawBitmap);


#ifdef __cplusplus 
}
#endif


#endif		// __WINDOW_H__

