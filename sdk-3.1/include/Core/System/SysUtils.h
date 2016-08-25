/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     SysUtils.h
 * AUTHOR:	 Roger Flores: April 27, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  These are miscellaneous routines.
 *
 **********************************************************************/
#ifndef __SYSUTILS_H__
#define __SYSUTILS_H__

// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.



//typedef Int _comparF (const void *, const void *, Int other);
typedef Int _comparF (VoidPtr, VoidPtr, Long other);
typedef _comparF * CmpFuncPtr;

typedef Int _searchF (ConstVoidPtr searchData, ConstVoidPtr arrayData, Long other);
typedef _searchF * SearchFuncPtr;


typedef enum
	{
	GremlinIsOn				// return non zero if gremlins is currently running
	} GremlinFunctionType;
	
typedef struct 
	{
	DWord unused;
	} GremlinParamsType;


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

Boolean	SysBinarySearch (ConstVoidPtr baseP, const UInt numOfElements, const UInt width, 
				SearchFuncPtr searchF, ConstVoidPtr searchData, 
				const Long other, ULongPtr position, const Boolean findFirst)
						SYS_TRAP(sysTrapSysBinarySearch);

void 		SysInsertionSort (VoidPtr baseP, UInt numOfElements, UInt width, 
				const CmpFuncPtr comparF, const Long other)
						SYS_TRAP(sysTrapSysInsertionSort);

void 		SysQSort (VoidPtr baseP, UInt numOfElements, UInt width, 
				const CmpFuncPtr comparF, const Long other)
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

DWord SysGremlins(GremlinFunctionType selector, GremlinParamsType *params)
						SYS_TRAP(sysTrapSysGremlins);


#ifdef __cplusplus
}
#endif

#endif // __SYSUTILS_H__

