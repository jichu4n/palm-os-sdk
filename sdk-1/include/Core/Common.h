/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		Common.h
 *
 * Description:
 *		Common header file for all Pilot routines.
 *		Contains elementary data types
 *
 * History:
 *   	10/19/94  RM - Created by Ron Marianetti  
 *
 *******************************************************************/


#ifndef __COMMON_H__
#define __COMMON_H__


/************************************************************
 * Build Rules
 *************************************************************/
#include <BuildRules.h>


/************************************************************
 * Define whether or not we are direct linking, or going through
 *  traps.
 *
 * When eumulating we use directy linking.
 * When running under native mode, we use traps EXCEPT for the
 *   modules that actually install the routines into the trap table. 
 *   These modules will set the DIRECT_LINK define to 1
 *************************************************************/
#ifndef USE_TRAPS 
	
#if EMULATION_LEVEL == EMULATION_NONE
	#define	USE_TRAPS 1
#else
	#define	USE_TRAPS 0
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
#ifdef __GNUC__
#define ASMWORD(W) asm("dc.w %c0"::"i"(W));
#define ONEWORD_INLINE(trapNum) 	\
	{ASMWORD(trapNum)}
#define TWOWORD_INLINE(w1, w2) 	\
	{ASMWORD(w1)ASMWORD(w2)}
#define THREEWORD_INLINE(w1, w2, w3)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)}
#define FOURWORD_INLINE(w1, w2, w3, w4)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)}
#define FIVEWORD_INLINE(w1, w2, w3, w4, w5)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)}
#define SIXWORD_INLINE(w1, w2, w3, w4, w5, w6)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)}
#define SEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)ASMWORD(w7)}
#define EIGHTWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)ASMWORD(w7)ASMWORD(w8)}
#define NINEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)ASMWORD(w7)ASMWORD(w8)ASMWORD(w9)}
#define TENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)ASMWORD(w7)ASMWORD(w8)ASMWORD(w9)ASMWORD(w10)}
#define ELEVENWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)ASMWORD(w7)ASMWORD(w8)ASMWORD(w9)ASMWORD(w10)ASMWORD(w11)}
#define TWELVEWORD_INLINE(w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12)  \
	{ASMWORD(w1)ASMWORD(w2)ASMWORD(w3)ASMWORD(w4)ASMWORD(w5)ASMWORD(w6)ASMWORD(w7)ASMWORD(w8)ASMWORD(w9)ASMWORD(w10)ASMWORD(w11)ASMWORD(w12)}
#else
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

// Fixed size data types
typedef char				SByte;		// 8 bits
typedef unsigned char 	Byte;

typedef short				SWord;		// 16 bits
typedef unsigned short 	Word;			

typedef long				SDWord;		// 32 bits
typedef unsigned long	DWord;		


// Logical data types
typedef unsigned char	Boolean;

typedef char				Char;			// Used for character strings
typedef unsigned char	UChar;

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
typedef VoidPtr*			VoidHand;


// Fixed size data types
typedef SByte*				SBytePtr;
typedef Byte*				BytePtr;

typedef SWord*				SWordPtr;
typedef Word*				WordPtr;

typedef SDWord*			SDWordPtr;
typedef DWord*				DWordPtr;


// Logical data types
typedef Boolean*			BooleanPtr;

typedef Char*				CharPtr;
typedef UChar*				UCharPtr;

typedef Short*				ShortPtr;
typedef UShort*			UShortPtr;

typedef Int*				IntPtr;
typedef UInt*				UIntPtr;

typedef Long*				LongPtr;
typedef ULong*				ULongPtr;


// Include the following typedefs if types.h wasn't read.
#ifndef __TYPES__

// Generic Pointer types used by Memory Manager
// We have to define Ptr as char* because that's what the Mac includes do.
typedef char*				Ptr;					// global pointer
typedef Ptr*				Handle;				// global handle


// Function types
typedef Long (*ProcPtr)();

#endif /* __TYPES__ */


/************************************************************
 * Common constants
 *************************************************************/
#ifndef NULL
#define NULL	0
#endif	// NULL

#ifndef __TYPES__									// Macintosh Types
enum {false, true};
#define __TYPES__
#endif

#endif //__COMMON_H__
