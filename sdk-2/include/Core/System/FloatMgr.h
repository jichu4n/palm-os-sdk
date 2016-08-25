/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		FloatMgr.h
 *
 * Description:
 *		Floating point routines
 *
 * History:
 *   	3/9/95 - Created by Ron Marianetti
 *
 *******************************************************************/  


#ifndef __FLOATMGR_H__
#define __FLOATMGR_H__

#include <Common.h>

/************************************************************************
 * Constants
 ***********************************************************************/
#define	fMaxLong	((Long)0x7FFFFFFF)
#define	fMinLong	((Long)0x80000000)


/************************************************************************
 * Floating point type
 * An exponent of kExpInf represents infinity
 ***********************************************************************/
typedef struct {	
	DWord			man;		// unsigned 32 bit
	SWord			exp;		// signed 16 bit
	SByte			sign;		// +/- 1	
	} FloatType;
#define	kExpInf		16000

	
	
/*******************************************************************
 * Floating pointer Library  Errors
 * The constant fplErrorClass is defined in SystemMgr.h
 *******************************************************************/
#define	fplErrOutOfRange			(fplErrorClass | 1)

	
/************************************************************
 * Floating point manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err			FplInit(void)
					SYS_TRAP(sysTrapFplInit);
					
void			FplFree(void)
					SYS_TRAP(sysTrapFplFree);

Err 			FplFToA(FloatType a, char* s)
					SYS_TRAP(sysTrapFplFToA);
					
FloatType 	FplAToF(char* s)
					SYS_TRAP(sysTrapFplAToF);
					
Err 			FplBase10Info(FloatType a, ULong* mantissaP, Int* exponentP, 
					Int* signP)
					SYS_TRAP(sysTrapFplBase10Info);

FloatType 	FplLongToFloat(Long x)
					SYS_TRAP(sysTrapFplLongToFloat);
					
Long 			FplFloatToLong(FloatType f)
					SYS_TRAP(sysTrapFplFloatToLong);
					
ULong 		FplFloatToULong(FloatType f)
					SYS_TRAP(sysTrapFplFloatToULong);
					

FloatType 	FplMul(FloatType a, FloatType b)
					SYS_TRAP(sysTrapFplMul);
					
FloatType 	FplAdd(FloatType a, FloatType b)
					SYS_TRAP(sysTrapFplAdd);
					
FloatType 	FplSub(FloatType a, FloatType b)
					SYS_TRAP(sysTrapFplSub);
					
FloatType	FplDiv(FloatType dividend, FloatType divisor)
					SYS_TRAP(sysTrapFplDiv);
					

#ifdef __cplusplus
}
#endif


#endif //__FLOATMGR_H__


