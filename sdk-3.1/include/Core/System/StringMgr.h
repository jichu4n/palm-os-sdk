/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		StringMgr.h
 *
 * Description:
 *		String manipulation functions
 *
 * History:
 *   	11/09/94  RM - Created by Ron Marianetti
 *		08/26/98	kwk	Changed chr param in StrChr to WChar (was Int)
 *
 *******************************************************************/



#ifndef __STRINGMGR_H__
#define __STRINGMGR_H__


// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.


#ifdef __cplusplus
extern "C" {
#endif

// String Manipulation routines
CharPtr	StrCopy(CharPtr dst, const Char* src)
							SYS_TRAP(sysTrapStrCopy);

CharPtr	StrNCopy(CharPtr dst, const Char* src, Word n)
							SYS_TRAP(sysTrapStrNCopy);

CharPtr	StrCat(CharPtr dst, const Char* src)
							SYS_TRAP(sysTrapStrCat);
							
CharPtr	StrNCat(CharPtr dst, const Char* src, Word n)
							SYS_TRAP(sysTrapStrNCat);
							
UInt		StrLen(const Char* src)
							SYS_TRAP(sysTrapStrLen);
							
Int		StrCompare(const Char* s1, const Char* s2)
							SYS_TRAP(sysTrapStrCompare);

Int		StrNCompare(const Char* s1, const Char* s2, DWord n)
							SYS_TRAP(sysTrapStrNCompare);

Int 		StrCaselessCompare(const Char* s1, const Char* s2)
							SYS_TRAP(sysTrapStrCaselessCompare);

Int		StrNCaselessCompare(const Char* s1, const Char* s2, DWord n)
							SYS_TRAP(sysTrapStrNCaselessCompare);

CharPtr	StrToLower(CharPtr dst, const Char* src)
							SYS_TRAP(sysTrapStrToLower);

CharPtr 	StrIToA(CharPtr s, Long i)
							SYS_TRAP(sysTrapStrIToA);

CharPtr 	StrIToH(CharPtr s, ULong i)
							SYS_TRAP(sysTrapStrIToH);

CharPtr StrLocalizeNumber(CharPtr s, Char thousandSeparator, Char decimalSeparator)
							SYS_TRAP(sysTrapStrLocalizeNumber);

CharPtr StrDelocalizeNumber(CharPtr s, Char thousandSeparator, Char decimalSeparator)
							SYS_TRAP(sysTrapStrDelocalizeNumber);

CharPtr	StrChr (const Char* str, WChar chr)
							SYS_TRAP(sysTrapStrChr);

CharPtr	StrStr (const Char* str, const Char* token)
							SYS_TRAP(sysTrapStrStr);

Long		StrAToI (const Char* str)
							SYS_TRAP(sysTrapStrAToI);
							
SWord 	StrPrintF(CharPtr s, const Char* formatStr, ...)
							SYS_TRAP(sysTrapStrPrintF);
							
SWord 	StrVPrintF(CharPtr s, const Char* formatStr, const VoidPtr arg)
							SYS_TRAP(sysTrapStrVPrintF);

#ifdef __cplusplus 
}
#endif




#endif //__STRINGMGR_H
