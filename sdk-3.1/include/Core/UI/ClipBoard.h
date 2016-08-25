/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     clipBoard.h
 * AUTHOR:	 Art Lamb: September 1, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines clipboard structures and routines.
 *
 ***********************************************************************/

#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__


#define numClipboardForamts 	3
#define cbdMaxTextLength		1000


// Clipboard standard formats
enum clipboardFormats { clipboardText, clipboardInk, clipboardBitmap };

typedef enum clipboardFormats ClipboardFormatType;

typedef struct {
	VoidHand		item;
	Word			length;
} ClipboardItem;

//----------------------------------------------------------
//	Clipboard Functions
//----------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern void ClipboardAddItem 
	(const ClipboardFormatType format, const VoidPtr ptr, Word length)
							SYS_TRAP(sysTrapClipboardAddItem);

extern VoidHand ClipboardGetItem (const ClipboardFormatType format, 
	const WordPtr length)
							SYS_TRAP(sysTrapClipboardGetItem);

#ifdef __cplusplus 
}
#endif

#endif // __CLIPBOARD_H__
