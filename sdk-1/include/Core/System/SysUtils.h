/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1995 -- All Rights Reserved
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

#ifdef __cplusplus
}
#endif

#endif // __SYSUTILS_H__

