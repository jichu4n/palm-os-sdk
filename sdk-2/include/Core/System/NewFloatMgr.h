/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		NewFloatMgr.h
 *
 * Description:
 *		New Floating point routines, provided by new IEEE arithmetic
 *		68K software floating point emulator (sfpe) code.
 *
 * History:
 *   	9/23/96 - Created by SCL
 *   11/15/96 - First build of NewFloatMgr.lib
 *   11/26/96 - Added FlpCorrectedAdd and FlpCorrectedSub routines
 *   12/30/96 - Added FlpVersion routine
 *    2/ 4/97 - Fixed FlpDoubleBits definition - sign & exp now SDWords
 *						so total size of FlpCompDouble is 64 bits, not 96. 
 *    2/ 5/97 - Added note about FlpBase10Info reporting "negative" zero.
 *    2/27/97 - Added support for new compiler flag (fp_pilot_traps).
 *
 *******************************************************************/  

#ifndef __NEWFLOATMGR_H__
#define __NEWFLOATMGR_H__

#if defined(__MWERKS__)					// Metrowerks compiler (CodeWarrior or MPW)

	#if __option(IEEEdoubles) == off
		#error "IEEEdoubles ('8-Byte Doubles' option in the 68K Processor preference panel) MUST be on!!"
	#endif

	#ifndef _DONT_USE_FP_TRAPS_
		#if EMULATION_LEVEL != EMULATION_NONE 
			#define _DONT_USE_FP_TRAPS_	1	// For emulator builds - direct link to FP library
		#elif defined(CMD_LINE_BUILD)
			#define _DONT_USE_FP_TRAPS_	0	// For PalmOS 2.0 (MPW) - Use new FP traps
		#elif __option (fp_pilot_traps)
			#define _DONT_USE_FP_TRAPS_	0	// For PalmOS 2.0 (IDE) - Use new FP traps
		#else
			#define _DONT_USE_FP_TRAPS_	1	// For PalmOS 1.0 compatibility - direct link to FP library	
		#endif
	#endif

#else

	#ifndef _DONT_USE_FP_TRAPS_
		#define _DONT_USE_FP_TRAPS_		0	// For PalmOS 2.0 (IDE) - Use new FP traps
	#endif

#endif


#include <Common.h>


/************************************************************************
 * Differences between FloatMgr (PalmOS v1.0) and (this) NewFloatMgr
 ***********************************************************************/
//
// FloatMgr (PalmOS v1.0)		NewFloatMgr (PalmOS v2.0 and NewFloatMgr.lib)
// ----------------------		---------------------------------------------
// FloatType (64-bits)			use FlpFloat (32-bits) or FlpDouble (64-bits)
//
// fplErrOutOfRange				use _fp_get_fpscr() to retrieve errors 
//
// FplInit()						not necessary
// FplFree()						not necessary
//
// FplFToA()						use FlpFToA()
// FplAToF()						use FlpAToF()
// FplBase10Info()				use FlpBase10Info() [*signP returns sign BIT: 1 if negative]
//
// FplLongToFloat()				use _f_itof() or _d_itod()
// FplFloatToLong()				use _f_ftoi() or _d_dtoi()
// FplFloatToULong()				use _f_ftou() or _d_dtou()
//
// FplMul()							use _f_mul() or _d_mul()
// FplAdd()							use _f_add() or _d_add()
// FplSub()							use _f_sub() or _d_sub()
// FplDiv()							use _f_div() or _d_div()



/************************************************************************
 * New Floating point manager constants
 ***********************************************************************/

#define flpVersion		0x02008000	// first version of NewFloatMgr (PalmOS 2.0)

/*
 * These constants are passed to and received from the _fp_round routine.
 */

#define flpToNearest		0
#define flpTowardZero	1
#define flpUpward			3
#define flpDownward		2
#define flpModeMask		0x00000030
#define flpModeShift		4

/*
 * These masks define the fpscr bits supported by the sfpe (software floating point emulator).
 * These constants are used with the _fp_get_fpscr and _fp_set_fpscr routines.
 */

#define flpInvalid		0x00008000
#define flpOverflow		0x00004000
#define flpUnderflow		0x00002000
#define flpDivByZero		0x00001000
#define flpInexact		0x00000800

/*
 * These constants are returned by _d_cmp, _d_cmpe, _f_cmp, and _f_cmpe:
 */

#define flpEqual			0
#define flpLess			1
#define flpGreater		2
#define flpUnordered		3


/************************************************************************
 * New Floating point manager types (private)
 ***********************************************************************/
typedef struct {
	SDWord high;
	SDWord low;
} _sfpe_64_bits;											// for internal use only

typedef _sfpe_64_bits sfpe_long_long;				// for internal use only
typedef _sfpe_64_bits sfpe_unsigned_long_long;	// for internal use only


/************************************************************************
 * New Floating point manager types (public)
 ***********************************************************************/
typedef SDWord FlpFloat;
typedef _sfpe_64_bits FlpDouble;
typedef _sfpe_64_bits FlpLongDouble;

/*
* A double value comprises the fields:
*		0x80000000 0x00000000 -- sign bit (1 for negative)
*		0x7ff00000 0x00000000 -- exponent, biased by 0x3ff == 1023
*		0x000fffff 0xffffffff -- significand == the fraction after an implicit "1."
* So a double has the mathematical form:
*		(-1)^sign_bit * 2^(exponent - bias) * 1.significand
* What follows are some structures (and macros) useful for decomposing numbers.
*/

typedef struct {
	SDWord		sign : 1;
	SDWord		exp  : 11;
	DWord			manH : 20;
	DWord			manL;
} FlpDoubleBits;						// for accessing specific fields

typedef union {
        double				d;			// for easy assignment of values
        FlpDouble			fd;		// for calling New Floating point manager routines
        DWord				ul[2];	// for accessing upper and lower longs
        FlpDoubleBits	fdb;		// for accessing specific fields
} FlpCompDouble;

typedef union {
        float				f;			// for easy assignment of values
        FlpFloat			ff;		// for calling New Floating point manager routines
        DWord				ul;		// for accessing bits of the float
} FlpCompFloat;


/************************************************************************
 * Useful macros...
 ***********************************************************************/
#define BIG_ENDIAN 1
#define __FIRST32(x) *((DWord *) &x)
#define __SECOND32(x) *((DWord *) &x + 1)
#define __ALL32(x) *((DWord *) &x)

#ifdef LITTLE_ENDIAN
#define __LO32(x) *((DWord *) &x)
#define __HI32(x) *((DWord *) &x + 1)
#define __HIX 1
#define __LOX 0
#else
#define __HI32(x) *((DWord *) &x)
#define __LO32(x) *((DWord *) &x + 1)
#define __HIX 0
#define __LOX 1
#endif

#define FlpGetSign(x)			((__HI32(x) & 0x80000000) != 0)
#define FlpIsZero(x)				( ((__HI32(x) & 0x7fffffff) | (__LO32(x))) == 0)

#define FlpGetExponent(x)		(((__HI32(x) & 0x7ff00000) >> 20) - 1023)


// DOLATER - Should we remove FlpNegate? There is an fp (_d_neg) call for this...
#define FlpNegate(x)				((FlpCompDouble *)&x)->ul[__HIX] ^= 0x80000000
#define FlpSetNegative(x)		((FlpCompDouble *)&x)->ul[__HIX] |= 0x80000000
#define FlpSetPositive(x)		((FlpCompDouble *)&x)->ul[__HIX] &= ~0x80000000
	
	
/*******************************************************************
 * New Floating point manager errors
 * The constant fplErrorClass is defined in SystemMgr.h
 *******************************************************************/
#define	flpErrOutOfRange			(flpErrorClass | 1)

	
/************************************************************
 * New Floating point manager trap macros
 *************************************************************/
#define m68kMoveQd2Instr	0x7400

#if _DONT_USE_FP_TRAPS_

#define FLOAT_TRAP(floatSelectorNum)
#define FLOAT_EM_TRAP(floatSelectorNum)

#else

#define FLOAT_TRAP(floatSelectorNum) \
				THREEWORD_INLINE(m68kMoveQd2Instr + floatSelectorNum, \
					m68kTrapInstr+sysDispatchTrapNum, sysTrapFlpDispatch)
#define FLOAT_EM_TRAP(floatEmSelectorNum) \
				THREEWORD_INLINE(m68kMoveQd2Instr + floatEmSelectorNum, \
					m68kTrapInstr+sysDispatchTrapNum, sysTrapFlpEmDispatch)
#endif

/************************************************************
 * New Floating point manager selectors
 *************************************************************/
typedef enum {					// The order of this enum *MUST* match the
									// sysFloatSelector table in NewFloatDispatch.c
	sysFloatBase10Info = 0,	// 0
	sysFloatFToA,				// 1
	sysFloatAToF,				// 2
	sysFloatCorrectedAdd,	// 3
	sysFloatCorrectedSub,	// 4
	sysFloatVersion,			// 5

	flpMaxFloatSelector = sysFloatVersion	// used by NewFloatDispatch.c
} sysFloatSelector;


typedef enum {					// The order of this enum *MUST* match the
									// sysFloatSelector table in NewFloatDispatch.c
	sysFloatEm_fp_round = 0,// 0
	sysFloatEm_fp_get_fpscr,// 1
	sysFloatEm_fp_set_fpscr,// 2

	sysFloatEm_f_utof,		// 3
	sysFloatEm_f_itof,		// 4
	sysFloatEm_f_ulltof,		// 5
	sysFloatEm_f_lltof,		// 6

	sysFloatEm_d_utod,		// 7
	sysFloatEm_d_itod,		// 8
	sysFloatEm_d_ulltod,		// 9
	sysFloatEm_d_lltod,		// 10

	sysFloatEm_f_ftod,		// 11
	sysFloatEm_d_dtof,		// 12
	sysFloatEm_f_ftoq,		// 13
	sysFloatEm_f_qtof,		// 14
	sysFloatEm_d_dtoq,		// 15
	sysFloatEm_d_qtod,		// 16

	sysFloatEm_f_ftou,		// 17
	sysFloatEm_f_ftoi,		// 18
	sysFloatEm_f_ftoull,		// 19
	sysFloatEm_f_ftoll,		// 20

	sysFloatEm_d_dtou,		// 21
	sysFloatEm_d_dtoi,		// 22
	sysFloatEm_d_dtoull,		// 23
	sysFloatEm_d_dtoll,		// 24

	sysFloatEm_f_cmp,			// 25
	sysFloatEm_f_cmpe,		// 26
	sysFloatEm_f_feq,			// 27
	sysFloatEm_f_fne,			// 28
	sysFloatEm_f_flt,			// 29
	sysFloatEm_f_fle,			// 30
	sysFloatEm_f_fgt,			// 31
	sysFloatEm_f_fge,			// 32
	sysFloatEm_f_fun,			// 33
	sysFloatEm_f_for,			// 34

	sysFloatEm_d_cmp,			// 35
	sysFloatEm_d_cmpe,		// 36
	sysFloatEm_d_feq,			// 37
	sysFloatEm_d_fne,			// 38
	sysFloatEm_d_flt,			// 39
	sysFloatEm_d_fle,			// 40
	sysFloatEm_d_fgt,			// 41
	sysFloatEm_d_fge,			// 42
	sysFloatEm_d_fun,			// 43
	sysFloatEm_d_for,			// 44

	sysFloatEm_f_neg,			// 45
	sysFloatEm_f_add,			// 46
	sysFloatEm_f_mul,			// 47
	sysFloatEm_f_sub,			// 48
	sysFloatEm_f_div,			// 49

	sysFloatEm_d_neg,			// 50
	sysFloatEm_d_add,			// 51
	sysFloatEm_d_mul,			// 52
	sysFloatEm_d_sub,			// 53
	sysFloatEm_d_div			// 54
} sysFloatEmSelector;


#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 * New Floating point manager routines
 *************************************************************/

				// Note: FlpBase10Info returns the actual sign bit in *signP (1 if negative)
				// Note: FlpBase10Info reports that zero is "negative".
				//			A workaround is to check (*signP && *mantissaP) instead of just *signP.
Err 			FlpBase10Info(FlpDouble a, DWord* mantissaP, SWord* exponentP, SWord* signP)
					FLOAT_TRAP(sysFloatBase10Info);

Err 			FlpFToA(FlpDouble a, Char* s)
					FLOAT_TRAP(sysFloatFToA);

FlpDouble	FlpAToF(Char* s)
					FLOAT_TRAP(sysFloatAToF);

FlpDouble	FlpCorrectedAdd(FlpDouble firstOperand, FlpDouble secondOperand, SWord howAccurate)
					FLOAT_TRAP(sysFloatCorrectedAdd);

FlpDouble	FlpCorrectedSub(FlpDouble firstOperand, FlpDouble secondOperand, SWord howAccurate)
					FLOAT_TRAP(sysFloatCorrectedSub);

DWord			FlpVersion(void)
					FLOAT_TRAP(sysFloatVersion);

void			FlpSelectorErrPrv (Word flpSelector)
					;		// used only by NewFloatDispatch.c

// The following macros could be useful but are left undefined due to the
// confusion they might cause.  What was called a "float" in PalmOS v1.0 was
// really a 64-bit; in v2.0 "float" is only 32-bits and "double" is 64-bits.
// However, if a v1.0 program is converted to use the NewFloatMgr, these
// macros could be re-defined, or the native _d_ routines could be called.

//#define 		FlpLongToFloat(x)		_d_itod(x)		// similar to 1.0 call, but returns double
//#define 		FlpFloatToLong(f)		_d_dtoi(f)		// similar to 1.0 call, but takes a double
//#define 		FlpFloatToULong(f)	_d_dtou(f)		// similar to 1.0 call, but takes a double


/************************************************************
 * New Floating point emulator functions
 *************************************************************/

/*
 * These three functions define the interface to the (software) fpscr
 * of the sfpe. _fp_round not only sets the rounding mode according
 * the low two bits of its argument, but it also returns those masked
 * two bits. This provides some hope of compatibility with less capable
 * emulators, which support only rounding to nearest. A programmer
 * concerned about getting the rounding mode requested can test the
 * return value from _fp_round; it will indicate what the current mode is.
 *
 * Constants passed to and received from _fp_round are:
 *		flpToNearest, flpTowardZero, flpUpward, or flpDownward 
 */

SDWord		_fp_round(SDWord)			FLOAT_EM_TRAP(sysFloatEm_fp_round);

/*
 * Constants passed to _fp_set_fpscr and received from _fp_get_fpscr are:
 *		flpInvalid, flpOverflow, flpUnderflow, flpDivByZero, or flpInexact 
 */

SDWord		_fp_get_fpscr(void)		FLOAT_EM_TRAP(sysFloatEm_fp_get_fpscr);
void			_fp_set_fpscr(SDWord)	FLOAT_EM_TRAP(sysFloatEm_fp_set_fpscr);


/*
 * The shorthand here can be determined from the context:
 *		i	--> long (SDWord)
 *		u	--> unsigned long (DWord)
 *		ll	--> long long int
 *		ull	--> unsigned long long int
 *		f	--> float
 *		d	--> double
 *		q	--> long double (defaults to double in this implementaton)
 *		XtoY--> map of type X to a value of type Y
 */

FlpFloat			_f_utof(DWord)								FLOAT_EM_TRAP(sysFloatEm_f_utof);
FlpFloat			_f_itof(SDWord)							FLOAT_EM_TRAP(sysFloatEm_f_itof);
FlpFloat			_f_ulltof(sfpe_unsigned_long_long)	FLOAT_EM_TRAP(sysFloatEm_f_ulltof);
FlpFloat			_f_lltof(sfpe_long_long)				FLOAT_EM_TRAP(sysFloatEm_f_lltof);

FlpDouble		_d_utod(DWord)								FLOAT_EM_TRAP(sysFloatEm_d_utod);
FlpDouble		_d_itod(SDWord)							FLOAT_EM_TRAP(sysFloatEm_d_itod);
FlpDouble		_d_ulltod(sfpe_unsigned_long_long)	FLOAT_EM_TRAP(sysFloatEm_d_ulltod);
FlpDouble		_d_lltod(sfpe_long_long)				FLOAT_EM_TRAP(sysFloatEm_d_lltod);


FlpDouble		_f_ftod(FlpFloat)							FLOAT_EM_TRAP(sysFloatEm_f_ftod);
FlpFloat			_d_dtof(FlpDouble)						FLOAT_EM_TRAP(sysFloatEm_d_dtof);

FlpLongDouble	_f_ftoq(FlpFloat)							FLOAT_EM_TRAP(sysFloatEm_f_ftoq);
FlpFloat			_f_qtof(const FlpLongDouble *)		FLOAT_EM_TRAP(sysFloatEm_f_qtof);

FlpLongDouble	_d_dtoq(FlpDouble)						FLOAT_EM_TRAP(sysFloatEm_d_dtoq);
FlpDouble		_d_qtod(const FlpLongDouble *)		FLOAT_EM_TRAP(sysFloatEm_d_qtod);


DWord							_f_ftou(FlpFloat)				FLOAT_EM_TRAP(sysFloatEm_f_ftou);
SDWord						_f_ftoi(FlpFloat)				FLOAT_EM_TRAP(sysFloatEm_f_ftoi);

sfpe_unsigned_long_long	_f_ftoull(FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_ftoull);
sfpe_long_long				_f_ftoll(FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_ftoll);

DWord							_d_dtou(FlpDouble)			FLOAT_EM_TRAP(sysFloatEm_d_dtou);
SDWord						_d_dtoi(FlpDouble)			FLOAT_EM_TRAP(sysFloatEm_d_dtoi);

sfpe_unsigned_long_long	_d_dtoull(FlpDouble)			FLOAT_EM_TRAP(sysFloatEm_d_dtoull);
sfpe_long_long				_d_dtoll(FlpDouble)			FLOAT_EM_TRAP(sysFloatEm_d_dtoll);


/*
 * The comparison functions _T_Tcmp[e] compare their two arguments,
 * of type T, and return one of the four values defined below.
 * The functions _d_dcmpe and _f_fcmpe, in addition to returning
 * the comparison code, also set the invalid flag in the fpscr if
 * the operands are unordered. Two floating point values are unordered
 * when they enjoy no numerical relationship, as is the case when one
 * or both are NaNs.
 *
 * Return values for _d_cmp, _d_cmpe, _f_cmp, and _f_cmpe are:
 *		flpEqual, flpLess, flpGreater, or flpUnordered 
 *
 * The function shorthand is:
 *		eq	--> equal
 *		ne	--> not equal
 *		lt	--> less than
 *		le	--> less than or equal to
 *		gt	--> greater than
 *		ge	--> greater than or equal to
 *		un	--> unordered with
 *		or	--> ordered with (i.e. less than, equal to, or greater than)
 */

SDWord		_f_cmp(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_cmp);
SDWord		_f_cmpe(FlpFloat, FlpFloat)		FLOAT_EM_TRAP(sysFloatEm_f_cmpe);
SDWord		_f_feq(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_feq);
SDWord		_f_fne(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_fne);
SDWord		_f_flt(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_flt);
SDWord		_f_fle(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_fle);
SDWord		_f_fgt(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_fgt);
SDWord		_f_fge(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_fge);
SDWord		_f_fun(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_fun);
SDWord		_f_for(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_for);

SDWord		_d_cmp(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_cmp);
SDWord		_d_cmpe(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_cmpe);
SDWord		_d_feq(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_feq);
SDWord		_d_fne(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_fne);
SDWord		_d_flt(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_flt);
SDWord		_d_fle(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_fle);
SDWord		_d_fgt(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_fgt);
SDWord		_d_fge(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_fge);
SDWord		_d_fun(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_fun);
SDWord		_d_for(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_for);


FlpFloat		_f_neg(FlpFloat)						FLOAT_EM_TRAP(sysFloatEm_f_neg);
FlpFloat		_f_add(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_add);
FlpFloat		_f_mul(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_mul);
FlpFloat		_f_sub(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_sub);
FlpFloat		_f_div(FlpFloat, FlpFloat)			FLOAT_EM_TRAP(sysFloatEm_f_div);

FlpDouble	_d_neg(FlpDouble)						FLOAT_EM_TRAP(sysFloatEm_d_neg);
FlpDouble	_d_add(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_add);
FlpDouble	_d_mul(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_mul);
FlpDouble	_d_sub(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_sub);
FlpDouble	_d_div(FlpDouble, FlpDouble)		FLOAT_EM_TRAP(sysFloatEm_d_div);


#ifdef __cplusplus
}
#endif


#endif //__NEWFLOATMGR_H__


