/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
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

extern void CtlDrawControl (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlDrawControl);

extern void CtlEraseControl (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlEraseControl);

extern void CtlHideControl (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlHideControl);

extern void CtlShowControl (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlShowControl);

extern Boolean CtlEnabled (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlEnabled);

extern void CtlSetEnabled (ControlPtr pControl, Boolean usable)
							SYS_TRAP(sysTrapCtlSetEnabled);

extern void CtlSetUsable (ControlPtr pControl, Boolean usable)
							SYS_TRAP(sysTrapCtlSetUsable);

extern SWord CtlGetValue (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlGetValue);

extern void CtlSetValue (ControlPtr pControl, SWord newValue)
							SYS_TRAP(sysTrapCtlSetValue);

extern CharPtr CtlGetLabel (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlGetLabel);

extern void CtlSetLabel (ControlPtr pControl, CharPtr newLabel)
							SYS_TRAP(sysTrapCtlSetLabel);

extern void CtlHitControl (ControlPtr pControl)
							SYS_TRAP(sysTrapCtlHitControl);

extern Boolean CtlHandleEvent (ControlPtr pControl, EventPtr pEvent)
							SYS_TRAP(sysTrapCtlHandleEvent);

#ifdef __cplusplus 
}
#endif


#endif //__CONTROL_H__



