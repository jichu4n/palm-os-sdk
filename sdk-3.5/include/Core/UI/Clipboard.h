/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: clipBoard.h
 *
 * Description:
 *	  This file defines clipboard structures and routines.
 *
 * History:
 *		September 1, 1994	Created by Art Lamb
 *
 *****************************************************************************/

#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#define numClipboardForamts 	3
#define numClipboardFormats	numClipboardForamts
#define cbdMaxTextLength		1000


// Clipboard standard formats
enum clipboardFormats { clipboardText, clipboardInk, clipboardBitmap };

typedef enum clipboardFormats ClipboardFormatType;

typedef struct {
	MemHandle		item;
	UInt16		length;
} ClipboardItem;

//----------------------------------------------------------
//	Clipboard Functions
//----------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern void ClipboardAddItem 
	(const ClipboardFormatType format, const void *ptr, UInt16 length)
							SYS_TRAP(sysTrapClipboardAddItem);

extern Err ClipboardAppendItem 
	(const ClipboardFormatType format, const void *ptr, UInt16 length)
							SYS_TRAP(sysTrapClipboardAppendItem);

extern MemHandle ClipboardGetItem (const ClipboardFormatType format, 
	UInt16 *length)
							SYS_TRAP(sysTrapClipboardGetItem);

#ifdef __cplusplus 
}
#endif

#endif // __CLIPBOARD_H__

