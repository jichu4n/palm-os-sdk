/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		StringMgr.h
 *
 * Description:
 *		String manipulation functions
 *
 * History:
 *   	11/09/94  RM - Created by Ron Marianetti
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
CharPtr	StrCopy(CharPtr dst, CharPtr src)
							SYS_TRAP(sysTrapStrCopy);

CharPtr	StrCat(CharPtr dst, CharPtr src)
							SYS_TRAP(sysTrapStrCat);
							
UInt		StrLen(CharPtr src)
							SYS_TRAP(sysTrapStrLen);
							
Int		StrCompare(CharPtr s1, CharPtr s2)
							SYS_TRAP(sysTrapStrCompare);

Int 		StrCaselessCompare(CharPtr s1, CharPtr s2)
							SYS_TRAP(sysTrapStrCaselessCompare);

CharPtr	StrToLower(CharPtr dst, CharPtr src)
							SYS_TRAP(sysTrapStrToLower);

CharPtr 	StrIToA(CharPtr s, Long i)
							SYS_TRAP(sysTrapStrIToA);

CharPtr 	StrIToH(CharPtr s, ULong i)
							SYS_TRAP(sysTrapStrIToH);

CharPtr	StrChr (CharPtr str, Int chr)
							SYS_TRAP(sysTrapStrChr);

CharPtr	StrStr (CharPtr str, CharPtr token)
							SYS_TRAP(sysTrapStrStr);

Int		StrAToI (CharPtr str)
							SYS_TRAP(sysTrapStrAToI);

#ifdef __cplusplus 
}
#endif




#endif //__STRINGMGR_H
