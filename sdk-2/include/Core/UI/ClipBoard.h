/***********************************************************************
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
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

extern void ClipboardAddItem 
	(ClipboardFormatType format, VoidPtr ptr, Word length)
							SYS_TRAP(sysTrapClipboardAddItem);

extern VoidHand ClipboardGetItem (ClipboardFormatType format, WordPtr length)
							SYS_TRAP(sysTrapClipboardGetItem);

#endif // __CLIPBOARD_H__
