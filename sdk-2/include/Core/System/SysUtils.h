/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		SysUtils.h
 *
 * Description:
 *		These are miscellaneous routines.
 *
 * History:
 *		4/27/95		roger		Initial version.
 *
 *******************************************************************/  

#ifndef __SYSUTILS_H__
#define __SYSUTILS_H__

// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.



//typedef Int _comparF (const void *, const void *, Int other);
typedef Int _comparF (VoidPtr, VoidPtr, Long other);
typedef _comparF * CmpFuncPtr;

typedef Int _searchF (const VoidPtr, const VoidPtr, Long other);
typedef _searchF * SearchFuncPtr;

/************************************************************
 * Constants
 *************************************************************/
#define	sysRandomMax		0x7FFF			// Max value returned from SysRandom()


/************************************************************
 * Macros
 *************************************************************/
#define Abs(a) ((a >= 0) ? a : (-a))

/************************************************************
 * procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Boolean	SysBinarySearch (VoidPtr baseP, Int numOfElements, Int width, 
				 SearchFuncPtr searchF, const VoidPtr searchData, 
				const Long other, ULongPtr position, Boolean findFirst)
						SYS_TRAP(sysTrapSysBinarySearch);

void 		SysInsertionSort (VoidPtr baseP, Int numOfElements, Int width, 
				_comparF *comparF, const Long other)
						SYS_TRAP(sysTrapSysInsertionSort);

void 		SysQSort (VoidPtr baseP, Int numOfElements, Int width, 
				_comparF *comparF, const Long other)
						SYS_TRAP(sysTrapSysQSort);

void		SysCopyStringResource (CharPtr string, UInt theID)
						SYS_TRAP(sysTrapSysCopyStringResource);

VoidHand SysFormPointerArrayToStrings(CharPtr c, Int stringCount)
						SYS_TRAP(sysTrapSysFormPointerArrayToStrings);
						
						
// Return a random number ranging from 0 to sysRandomMax.
// Normally, 0 is passed unless you want to start with a new seed.
Int		SysRandom(ULong newSeed)
						SYS_TRAP(sysTrapSysRandom);


CharPtr SysStringByIndex(Word resID, Word index, CharPtr strP, Word maxLen)
						SYS_TRAP(sysTrapSysStringByIndex);

CharPtr 	SysErrString(Err err, CharPtr strP, Word maxLen)
						SYS_TRAP(sysTrapSysErrString);


#ifdef __cplusplus
}
#endif

#endif // __SYSUTILS_H__

