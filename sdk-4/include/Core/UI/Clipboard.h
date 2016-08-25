/******************************************************************************
 *
 * Copyright (c) 1994-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: clipBoard.h
 *
 * Release: Palm OS SDK 4.0 (63220)
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

typedef struct ClipboardItemTag
#ifdef ALLOW_ACCESS_TO_INTERNALS_OF_CLIPBOARDS	// These fields will not be available in the next OS release!
{
	MemHandle		item;
	UInt16		length;
}
#endif
ClipboardItem;

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
