/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		Keyboard.h
 *
 * Description:
 *		This file defines the keyboard's structures and routines.
 *
 * History:
 *   	3/29/95  rsf - Created by Roger Flores
 *
 *******************************************************************/


#ifndef __KBD_H__
#define __KBD_H__



#define kbdReturnKey		linefeedChr
#define kbdTabKey			tabChr
#define kbdBackspaceKey	backspaceChr
#define kbdShiftKey		2
#define kbdCapsKey		1
#define kbdNoKey			-1


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
