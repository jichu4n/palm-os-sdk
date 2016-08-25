/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		Common.h
 *
 * Description:
 *		Common header file for all Pilot routines.
 *		Contains elementary data types
 *
 * History:
 *   	10/19/94  RM - Created by Ron Marianetti  
 *   	 4/24/97  SL - Changes for PalmOS 2.0 SDK
 *		 5/27/98	kwk - Added WChar & WCharPtr typedefs.
 *		 5/30/98	kwk - Added ConstCharPtr typedef.
 *
 *******************************************************************/


#ifndef __COMMON_H__
#define __COMMON_H__


/************************************************************
 * Build Rules
 *************************************************************/
#include <BuildRules.h>


/************************************************************
 * Metrowerks will substitute strlen and strcpy with inline
 * 68K assembly code.  Prevent this.
 *************************************************************/
 
#ifdef __MC68K__
#define _NO_FAST_STRING_INLINES_ 0
#endif


/************************************************************
 * Define whether or not we are direct linking, or going through
 *  traps.
 *
 * When eumulating we use directy linking.
 * When running under native mode, we use traps EXCEPT for the
 *   modules that actually install the routines into the trap table. 
 *   These modules will set the DIRECT_LINK define to 1
 *************************************************************/
#ifndef EMULATION_LEVEL
#error "This should not happen!"
#endif

#ifndef USE_TRAPS 
	#if EMULATION_LEVEL == EMULATION_NONE
		#define	USE_TRAPS 1						// use Pilot traps
	#else
		#define	USE_TRAPS 0						// direct link (Simulator)
	#endif
#endif

#if USE_TRAPS == 0

#define ONEWORD_INLINE(trapNum)
#define TWOWORD_INLINE(w1, w2)
#define THREEWORD_INLINE(w1, w2, w3)
#define FOURWORD_INLINE(w1, w2, w3, w4)
#define FIVEWORD_INLINE(w1, w2, w3, w4, w5)
#define SIXWORD_INLINE(w1, w2, w3, w4, w5, w6)
#define SEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7)
#define EIGHTWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8)
#define NINEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9)
#define TENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10)
#define ELEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11)
#define TWELVEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12)

#else

// Define the traps differently depending on the compiler

#ifdef __GNUC__
#define _ALAS  __attribute__ ((__callseq__ ("** Sorry; calling this function with these header files doesn't work yet.  Life will be much better with 3.5 headers")))
#define ONEWORD_INLINE(trapNum)  _ALAS
#define TWOWORD_INLINE(w1, w2)  _ALAS
#define ONEWORD_INLINE(trapNum)  _ALAS
#define THREEWORD_INLINE(w1, w2, w3)  _ALAS
#define FOURWORD_INLINE(w1, w2, w3, w4)  _ALAS
#define FIVEWORD_INLINE(w1, w2, w3, w4, w5)  _ALAS
#define SIXWORD_INLINE(w1, w2, w3, w4, w5, w6)  _ALAS
#define SEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7)  _ALAS
#define EIGHTWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8)  _ALAS
#define NINEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9)  _ALAS
#define TENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10)  _ALAS
#define ELEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11)  _ALAS
#define TWELVEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12)  _ALAS

#else

// Define the traps for Metrowerks IDE or MPW 68K compiler

#define ONEWORD_INLINE(trapNum) 	\
	= trapNum
#define TWOWORD_INLINE(w1, w2) 	\
	= {w1,w2}
#define THREEWORD_INLINE(w1, w2, w3)  \
	= {w1,w2,w3}
#define FOURWORD_INLINE(w1, w2, w3, w4)  \
	= {w1,w2,w3,w4}
#define FIVEWORD_INLINE(w1, w2, w3, w4, w5)  \
	= {w1,w2,w3,w4,w5}
#define SIXWORD_INLINE(w1, w2, w3, w4, w5, w6)  \
	= {w1,w2,w3,w4,w5,w6}
#define SEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7)  \
	= {w1,w2,w3,w4,w5,w6,w7}
#define EIGHTWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8)  \
	= {w1,w2,w3,w4,w5,w6,w7,w8}
#define NINEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9)  \
	= {w1,w2,w3,w4,w5,w6,w7,w8,w9}
#define TENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10)  \
	= {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10}
#define ELEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11)  \
	= {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11}
#define TWELVEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12)  \
	= {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12}

#endif

#endif


/********************************************************************
 * Elementary data types
 ********************************************************************/
// Determine if we need to define our basic types or not
#ifndef	__TYPES__					// (Already defined in CW11)
#ifndef	__MACTYPES__				// (Already defined in CWPro3)
#define	__DEFINE_TYPES_	1
#endif
#endif


// Fixed size data types
typedef char				SByte;		// 8 bits
#if __DEFINE_TYPES_						
typedef unsigned char 	Byte;
typedef unsigned short  UInt16;
typedef unsigned long   UInt32;
#endif

typedef short				SWord;		// 16 bits
typedef short				Int16;
typedef unsigned short 	Word;			

typedef long				SDWord;		// 32 bits
typedef long				Int32;
typedef unsigned long	DWord;		


// Logical data types
#if __DEFINE_TYPES_						 
typedef unsigned char	Boolean;
#endif

typedef char				Char;			// Used for character strings
typedef unsigned char	UChar;
typedef unsigned short	WChar;		// 'wide' int'l character type.

typedef short				Short;		// >= Byte
typedef unsigned short	UShort;

typedef short				Int;			// >= Word (use short so MPW and CW agree)
typedef unsigned short	UInt;			   

typedef long				Long;			// >= DWord
typedef unsigned long	ULong;

typedef short				Err;

typedef DWord				LocalID;		// local (card relative) chunk ID



/************************************************************
 * Pointer Types
 *************************************************************/ 
typedef void*				VoidPtr;
typedef void const *		ConstVoidPtr;
typedef VoidPtr*			VoidHand;
typedef ConstVoidPtr*	ConstVoidHand;


// Fixed size data types
typedef SByte*				SBytePtr;
typedef SByte const *	ConstSBytePtr;
#if __DEFINE_TYPES_						 
typedef Byte*				BytePtr;
typedef Byte const *		ConstBytePtr;
#endif

typedef SWord*				SWordPtr;
typedef Word*				WordPtr;
typedef unsigned short* UInt16Ptr;
typedef SWord const *	ConstSWordPtr;
typedef Word const *		ConstWordPtr;
typedef unsigned short const * ConstUInt16Ptr;

typedef SDWord*			SDWordPtr;
typedef DWord*				DWordPtr;
typedef SDWord const *	ConstSDWordPtr;
typedef DWord const *	ConstDWordPtr;


// Logical data types
typedef Boolean*			BooleanPtr;
typedef Boolean const *	ConstBooleanPtr;

typedef Char*				CharPtr;
typedef UChar*				UCharPtr;
typedef WChar*				WCharPtr;
typedef const Char* 		ConstCharPtr;

typedef Short*				ShortPtr;
typedef UShort*			UShortPtr;
typedef Short const *	ConstShortPtr;
typedef UShort*			ConstUShortPtr;

typedef Int*				IntPtr;
typedef UInt*				UIntPtr;
typedef Int const *		ConstIntPtr;
typedef UInt const *		ConstUIntPtr;

typedef Long*				LongPtr;
typedef ULong*				ULongPtr;
typedef Long const *		ConstLongPtr;
typedef ULong const *	ConstULongPtr;

// Include the following typedefs if types.h wasn't read.
#if __DEFINE_TYPES_						 

// Generic Pointer types used by Memory Manager
// We have to define Ptr as char* because that's what the Mac includes do.
typedef char*				Ptr;					// global pointer
typedef Ptr*				Handle;				// global handle
typedef char const *		ConstPtr;			// global pointer to constant data
typedef ConstPtr*			ConstHandle;		// global handle to constant data

// Function types
typedef Long (*ProcPtr)();

#endif /* __DEFINE_TYPES_ */



/************************************************************
 * Common constants
 *************************************************************/
#ifndef NULL
#define NULL	0
#endif	// NULL

// Include the following typedefs if types.h wasn't read.
#if  __DEFINE_TYPES_
	#ifdef __MWERKS__
		#if !__option(bool)
			#ifndef true
				#define true			1
			#endif
			#ifndef false
				#define false			0
			#endif
		#endif
	#else
      #ifndef __cplusplus
		  #ifndef true
				enum {false, true};
		  #endif
	   #endif
	#endif
#endif /* __TYPES__ */


#endif //__COMMON_H__
