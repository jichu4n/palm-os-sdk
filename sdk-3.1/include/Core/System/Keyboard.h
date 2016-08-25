/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Keyboard.h
 * AUTHOR:	 Roger Flores: March 29, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines the keyboard's  structures 
 *   and routines.
 *
 ***********************************************************************/


#ifndef __KBD_H__
#define __KBD_H__



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

#endif // __KBD_H__
