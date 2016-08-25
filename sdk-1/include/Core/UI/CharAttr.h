/***********************************************************************
 *
 * Copyright (c) Palm Computing 1994 -- All Rights Reserved
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
// IsAscii should be used first.
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


WordPtr GetCharAttr (void)
			SYS_TRAP(sysTrapGetCharAttr);

BytePtr GetCharSortValue (void)
			SYS_TRAP(sysTrapGetCharSortValue);

BytePtr GetCharCaselessValue (void)
			SYS_TRAP(sysTrapGetCharCaselessValue);

#endif /* _CHAR_ATTR_H */
