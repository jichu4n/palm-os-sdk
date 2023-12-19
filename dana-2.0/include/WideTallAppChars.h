/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            WideTallAppChars.h
 *
 * DESCRIPTION: This file contains the definitions and constants 
 *				for the keyboard characters.
 *
 ****************************************************************************/
#ifndef WIDETALLAPPCHARS_H
#define WIDETALLAPPCHARS_H

//------------------------------------------------------------------------------
// All alphasmart key events are a virtual key
//------------------------------------------------------------------------------
#define	AS_VIRTUAL_KEY				0x2000

//------------------------------------------------------------------------------
// New event type for detecting modifiers keys up and down
// Palm has allocated AlphaSmart 0x4000 - 0x4007 event type numbers
// for use on their device
//------------------------------------------------------------------------------
#define AlphaSmartfirstOEMEvent		(0x4000)		// (firstOEMEvent + 0)
#define MODIFIER_KEY_EVENT			(AlphaSmartfirstOEMEvent + 0)
// the following two events are defined in USBEmu.h -- DON'T USE THESE EVENT VALUES!
//#define asUSBHidConnectedEvent	(AlphaSmartfirstOEMEvent + 1)
//#define asUSBHidDisconnectedEvent	(AlphaSmartfirstOEMEvent + 2)


//------------------------------------------------------------------------------
// ICode values
//------------------------------------------------------------------------------
#define ICODE_INVALID				0xFF
#define ICODE_LAST					0x50

#define ICODE_DOWN_ARROW			0x0D
#define ICODE_UP_ARROW				0x4B
#define ICODE_RIGHT_ARROW			0x4A
#define ICODE_LEFT_ARROW			0x49
#define ICODE_BACKSPACE				0x03
#define ICODE_LEFT_SHIFT			0x08
#define ICODE_RIGHT_SHIFT			0x45
#define ICODE_LEFT_OPTION			0x26
#define ICODE_PC_DELETE				0x3C
#define ICODE_COMMAND				0x0C
#define ICODE_CONTROL				0x4D
#define ICODE_CAPS_LOCK				0x05
#define ICODE_RETURN				0x40
#define ICODE_TAB					0x06
#define ICODE_ESC					0x48
#define ICODE_SPACEBAR				0x4C


#define ICODE_POWER_KEY				0x50
#define ICODE_F1_KEY				0x2D
#define ICODE_F2_KEY				0x2C
#define ICODE_F3_KEY				0x04
#define ICODE_F4_KEY				0x0F
#define ICODE_F5_KEY				0x0E
#define ICODE_F6_KEY				0x0A
#define ICODE_F7_KEY				0x01
#define ICODE_F8_KEY				0x27
#define ICODE_F9_KEY				0x2B
#define ICODE_F10_KEY				0x35
#define ICODE_F11_KEY				0x3F
#define ICODE_F12_KEY				0x34
#define ICODE_F13_KEY				0x1F
#define ICODE_F14_KEY				0x3E
#define ICODE_F15_KEY				0x29
#define ICODE_F16_KEY				0x2A


#define ICODE_APP_1					ICODE_F9_KEY
#define ICODE_APP_2					ICODE_F10_KEY
#define ICODE_APP_3					ICODE_F11_KEY
#define ICODE_APP_4					ICODE_F12_KEY
#define ICODE_FIND					ICODE_F13_KEY
#define ICODE_MENU					ICODE_F14_KEY
#define ICODE_APPS					ICODE_F15_KEY
#define ICODE_SEND					ICODE_F16_KEY

#define ICODE_FN_NEW_BUTTON			ICODE_F9_KEY
#define ICODE_FN_CANCEL_BUTTON		ICODE_F10_KEY
#define ICODE_FN_DONE_BUTTON		ICODE_F11_KEY
#define ICODE_FN_CONTRAST			ICODE_F13_KEY
#define ICODE_FN_NOTE_BUTTON		ICODE_F12_KEY
#define ICODE_FN_CALC				ICODE_F14_KEY
#define ICODE_FN_SHORTCUT			ICODE_F15_KEY
#define ICODE_FN_HOT_SYNC			ICODE_F16_KEY
#define ICODE_FN_OK_BUTTON			ICODE_RETURN
#define ICODE_FN_DELETE_BUTTON		ICODE_PC_DELETE
#define ICODE_FN_SHOW_BUTTON		ICODE_LEFT_ARROW
#define ICODE_FN_DETAILS_BUTTON		ICODE_RIGHT_ARROW


#define ICODE_FN_PAGE_UP			ICODE_UP_ARROW
#define ICODE_FN_PAGE_DOWN			ICODE_DOWN_ARROW

#define ICODE_TICK_TILDE			0x2E				// ` and ~ key
#define ICODE_QUOTE					0x0B				// ' and " key
#define ICODE_SEMICOLON				0x14				// ; and : key

#define ICODE_1						0x38
#define ICODE_2						0x37
#define ICODE_3						0x36
#define ICODE_4						0x39
#define ICODE_5						0x2F
#define ICODE_6_HAT					0x30				// 6 and ^ key
#define ICODE_7						0x3A
#define ICODE_8						0x31
#define ICODE_9						0x32				// 9 key
#define ICODE_ZERO					0x33

#define ICODE_A						0x18
#define ICODE_B						0x4E
#define ICODE_C						0x41
#define ICODE_D						0x16
#define ICODE_E						0x20
#define ICODE_F						0x19
#define ICODE_G						0x10
#define ICODE_H						0x11
#define ICODE_I						0x1C
#define ICODE_J						0x1B
#define ICODE_K						0x12
#define ICODE_L						0x13
#define ICODE_M						0x46
#define ICODE_N						0x4F
#define ICODE_O						0x1D
#define ICODE_P						0x1E
#define ICODE_Q						0x22
#define ICODE_R						0x23
#define ICODE_S						0x17
#define ICODE_T						0x07
#define ICODE_U						0x24
#define ICODE_V						0x44
#define ICODE_W						0x21
#define ICODE_X						0x42
#define ICODE_Y						0x09
#define ICODE_Z						0x43

#define ICODE_PLUS_EQUAL			0x25
#define ICODE_COMMA					0x3B
#define ICODE_MINUS					0x28
#define ICODE_PERIOD				0x3D
#define ICODE_QUESTION_MARK			0x47
#define ICODE_LEFT_BRACKET			0x02
#define ICODE_RIGHT_BRACKET			0x00
#define ICODE_VERTICAL_BAR			0x15
#define ICODE_EXTRA_ISO				0x1A


//------------------------------------------------------------------------------
// New Types
// For a palm keydown event, if the chr field is AS_VIRTUAL_KEY then:
//	keyCode = an alphasmart keyboard event
//	modifiers = commandMask
//
// The alphasmart keyboard event has the icode in the lower 8 bits and the
// modifers in the upper 8 bits.  
//------------------------------------------------------------------------------
	// upper 8 bits of modifier info 
	// lower 8 bits are is the icode
typedef	UInt16			KeyboardEvent;
typedef	KeyboardEvent	*KeyboardEvent_p;

//------------------------------------------------------------------------------
// Modifier bits in the upper 8 bits of the alphsmart event.
// If the bit is set then the key on the keyboard is down.
//------------------------------------------------------------------------------

#define KEYBOARD_PATCH_IGNORE_EVENT		0x8000
#define KEYBOARD_MODIFIER_COMMAND		0x4000
#define KEYBOARD_MODIFIER_FUNCTION		0x2000
#define KEYBOARD_MODIFIER_RIGHT_SHIFT	0x1000
#define KEYBOARD_MODIFIER_LEFT_SHIFT	0x0800
#define KEYBOARD_MODIFIER_CAPS_LOCK		0x0400
#define KEYBOARD_MODIFIER_CONTROL		0x0200

//------------------------------------------------------------------------------
// Useful macros for manipulating the keyboard event
//------------------------------------------------------------------------------
										
#define KEYBOARD_MODIFIER_SHIFT_MASK	(KEYBOARD_MODIFIER_LEFT_SHIFT | \
										KEYBOARD_MODIFIER_RIGHT_SHIFT)
	// if this bit is set then the key went up not down									
#define KEYBOARD_MODIFIER_KEY_UP		0x0080

#define KEYBOARD_ICODE_MASK				0x00FF
#define KEYBOARD_MODIFIER_MASK			0x7e00

#define KEYBOARD_CMD_CONTROL_MASK		(KEYBOARD_MODIFIER_CONTROL | \
										KEYBOARD_MODIFIER_COMMAND)

#define KEYBOARD_ACTION_CODES_MASK		(KEYBOARD_MODIFIER_CONTROL | \
										KEYBOARD_MODIFIER_COMMAND | \
										KEYBOARD_MODIFIER_FUNCTION)
										
#define KEYBOARD_STICKY_CODES_MASK		(KEYBOARD_MODIFIER_CONTROL | \
										KEYBOARD_MODIFIER_COMMAND | \
										KEYBOARD_MODIFIER_FUNCTION | \
										KEYBOARD_MODIFIER_SHIFT_MASK)

	// pass an alphasmart keyboard event (16 bit), not a palm event
#define KEYBOARD_GET_ICODE(keyboardEvent)		((UInt8 )((keyboardEvent) & \
										KEYBOARD_ICODE_MASK))
											
#define KEYBOARD_MAKE_EVENT(icode)		(((KeyboardEvent)(icode)) & 0x00FF)
					

#endif
