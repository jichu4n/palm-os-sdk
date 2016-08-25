/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Control.h
 * AUTHOR:	 Art Lamb: August 29, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines check box structures and routines.
 *
 ***********************************************************************/
#ifndef __CONTROL_H__
#define __CONTROL_H__



typedef struct {
	Byte usable         :1;		// set if part of ui 
	Byte enabled        :1;		// set if interactable (not grayed out)
	Byte visible        :1;		// set if drawn (set internally)
	Byte on             :1;		// set if on (checked)
	Byte leftAnchor     :1;		// set if bounds expand to the right
	                           // clear if bounds expand to the left
   Byte frame          :3;
} ControlAttrType;


enum controlStyles {buttonCtl, pushButtonCtl, checkboxCtl, popupTriggerCtl,
							 selectorTriggerCtl, repeatingButtonCtl};
typedef enum controlStyles ControlStyleType;

enum buttonFrames {noButtonFrame, standardButtonFrame, boldButtonFrame,
						 rectangleButtonFrame};
typedef enum buttonFrames ButtonFrameType;


typedef struct {
	Word						id;
	RectangleType			bounds;
	CharPtr				   text;	
	ControlAttrType		attr;
	ControlStyleType		style;
	FontID					font;
	Byte						group;
} ControlType;


typedef ControlType * ControlPtr;



//----------------------------------------------------------
//	Control Functions
//----------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern void CtlDrawControl (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlDrawControl);

extern void CtlEraseControl (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlEraseControl);

extern void CtlHideControl (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlHideControl);

extern void CtlShowControl (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlShowControl);

extern Boolean CtlEnabled (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlEnabled);

extern void CtlSetEnabled (const ControlPtr pControl, const Boolean usable)
							SYS_TRAP(sysTrapCtlSetEnabled);

extern void CtlSetUsable (const ControlPtr pControl, const Boolean usable)
							SYS_TRAP(sysTrapCtlSetUsable);

extern SWord CtlGetValue (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlGetValue);

extern void CtlSetValue (const ControlPtr pControl, const SWord newValue)
							SYS_TRAP(sysTrapCtlSetValue);

extern CharPtr CtlGetLabel (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlGetLabel);

extern void CtlSetLabel (const ControlPtr pControl, const CharPtr newLabel)
							SYS_TRAP(sysTrapCtlSetLabel);

extern void CtlHitControl (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlHitControl);

extern Boolean CtlHandleEvent (const ControlPtr pControl, 
	const EventPtr pEvent)
							SYS_TRAP(sysTrapCtlHandleEvent);

extern Boolean CtlValidatePointer (const ControlPtr pControl)
							SYS_TRAP(sysTrapCtlValidatePointer);

extern ControlPtr CtlNewControl (VoidPtr *formPP, const Word ID, 
	const ControlStyleType style, const CharPtr textP, 
	const Word x, const Word y, const Word width, const Word height, 
	const FontID font, Byte group, Boolean leftAnchor)
							SYS_TRAP(sysTrapCtlNewControl);


#ifdef __cplusplus 
}
#endif


#endif //__CONTROL_H__



