/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Keyboard.h
 *
 * Description:
 *	  This file defines the keyboard's  structures 
 *   and routines.
 *
 * History:
 *		March 29, 1995	Created by Roger Flores
 *
 *****************************************************************************/

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#define kbdReturnKey		linefeedChr
#define kbdTabKey			tabChr
#define kbdBackspaceKey	backspaceChr
#define kbdShiftKey		2
#define kbdCapsKey		1
#define kbdNoKey			0xff


typedef enum 
	{
	kbdAlpha = 0, 
	kbdNumbersAndPunc = 1, 
	kbdAccent = 2,
	kbdDefault = 0xff		// based on graffiti mode (usually alphaKeyboard)
	} KeyboardType;


/************************************************************
 * Keyboard procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// At some point the Graffiti code will need access to the
// shift and caps lock info.  Either export the structures
// or provide calls to the info.

extern void SysKeyboardDialogV10 ()
							SYS_TRAP(sysTrapSysKeyboardDialogV10);

extern void SysKeyboardDialog (KeyboardType kbd)
							SYS_TRAP(sysTrapSysKeyboardDialog);


#ifdef __cplusplus
}
#endif

#endif // __KEYBOARD_H__

