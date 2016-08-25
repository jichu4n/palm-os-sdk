/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     CharAttr.h
 * AUTHOR:   Art Lamb: April 21, 1995
 *
 * DECLARER: Toolbox
 *
 * DESCRIPTION:
 *        This file defines character classification and character
 *        conversion macros
 *
 **********************************************************************/

#ifndef _CHAR_ATTR_H
#define _CHAR_ATTR_H

#ifdef NON_INTERNATIONAL
// Remember that sizeof(0x0D) == 2 because 0x0D is treated like an int
#define sizeofchar(c)	sizeof((char) (c))
#define lastAsciiChr			0x00FF
#endif

#define	sizeOf7BitChar(c)	1

// Character attribute code bits
#define _XA		0x200 	// extra alphabetic
#define _XS		0x100 	// extra space
#define _BB		0x80 		// BEL, BS, etc.
#define _CN		0x40 		// CR, FF, HT, NL, VT
#define _DI		0x20 		// '0'-'9'
#define _LO		0x10 		// 'a'-'z' and lowercase extended chars.
#define _PU		0x08 		// punctuation
#define _SP		0x04 		// space
#define _UP		0x02 		// 'A'-'Z' and uppercase extended chars.
#define _XD		0x01 		// '0'-'9', 'A'-'F', 'a'-'f'


// Character classification macros.
// If these are used to test characters from events (keyDownEvent)
// IsAscii should be used first. Note that the TxtXXX() versions
// should be used instead; these are only included for backwards
// compatibility.

#ifdef NON_INTERNATIONAL

#define IsAscii(c)			(c <= 255)
#define IsAlNum(attr,c)		(attr[(Byte)(c)] & (_DI|_LO|_UP|_XA))
#define IsAlpha(attr,c)		(attr[(Byte)(c)] & (_LO|_UP|_XA))
#define IsCntrl(attr,c)		(attr[(Byte)(c)] & (_BB|_CN))
#define IsDigit(attr,c)		(attr[(Byte)(c)] & _DI)
#define IsGraph(attr,c)		(attr[(Byte)(c)] & (_DI|_LO|_PU|_UP|_XA))
#define IsLower(attr,c)		(attr[(Byte)(c)] & _LO)
#define IsPrint(attr,c)		(attr[(Byte)(c)] & (_DI|_LO|_PU|_SP|_UP|_XA))
#define IsPunct(attr,c)		(attr[(Byte)(c)] & _PU)
#define IsSpace(attr,c)		(attr[(Byte)(c)] & (_CN|_SP|_XS))
#define IsUpper(attr,c)		(attr[(Byte)(c)] & _UP)
#define IsHex(attr,c)		(attr[(Byte)(c)] & _XD)
#define IsDelim(attr,c)		(attr[(Byte)(c)] & _SP|_PU)

#endif

// This macro is deprecated because it relies on character code ranges, versus checking
// to ensure that the keydown event has the command bit set in the modifiers field.  Use
// the TxtCharIsHardKey macro found in TextMgr.h

#ifdef NON_INTERNATIONAL
#define	ChrIsHardKey(c)		((((c) >= hardKeyMin) && ((c) <= hardKeyMax)) || ((c) == calcChr))
#else
#define	ChrIsHardKey(c)	_Obsolete__use_TxtCharIsHardKey
#endif


#ifdef __cplusplus
extern "C" {
#endif

// These routines have been replaced by new Text Manager routines found
// in TextMgr.h.

#ifdef NON_INTERNATIONAL

WordPtr GetCharAttr (void)
			SYS_TRAP(sysTrapGetCharAttr);

BytePtr GetCharSortValue (void)
			SYS_TRAP(sysTrapGetCharSortValue);

BytePtr GetCharCaselessValue (void)
			SYS_TRAP(sysTrapGetCharCaselessValue);

#endif

#ifdef __cplusplus 
}
#endif


#endif /* _CHAR_ATTR_H */
