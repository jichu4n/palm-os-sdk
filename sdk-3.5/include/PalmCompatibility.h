/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: CoreCompatibility.h
 *
 * Description:
 *		Constant definitions for atrap compatibility between
 *		new function names and old function names.
 *
 * History:
 *    	4/26/99	BRM		Created.
 *			99-10-22	jwm		Added old-style data types.
 *			10/23/99	jmp		Added more win-to-scr-oriented aliases.
 *			99-10-26	jwm		Added old-style nWORD_INLINE macros.
 *			11/23/99	jmp		Added ScrDisplayModeOperation-to-WinScreenModeOperation
 *									definitions by request from Tool team.
 *
 *****************************************************************************/

// The data types Byte, Word, DWord and so on are now deprecated.  We
// recommend that you use the corresponding new data types: for example,
// use Int16 instead of SWord and UInt32 instead of DWord.  In particular,
// the unfortunate distinction between Handle/VoidHand has been fixed:
// use MemHandle instead.

typedef Int8		SByte;
#if __DEFINE_TYPES_						
typedef UInt8		Byte;
#endif

typedef Int16		SWord;
typedef UInt16		Word;

typedef Int32		SDWord;
typedef UInt32		DWord;


// Logical data types
typedef Int8		SChar;
typedef UInt8		UChar;

typedef Int16		Short;
typedef UInt16		UShort;
  
typedef Int16		Int;
typedef UInt16		UInt;

typedef Int32		Long;
typedef UInt32		ULong;


// Pointer Types
typedef MemPtr		VoidPtr;
typedef MemHandle	VoidHand;

#if __DEFINE_TYPES_
typedef MemPtr		Ptr;
typedef MemHandle	Handle;
#endif


// Because "const BytePtr" means "const pointer to Byte" rather than "pointer
// to const Byte", all these XXXXPtr types are deprecated: you're better off
// just using "Byte *" and so on.  (Even better, use "UInt8 *"!)

typedef SByte*		SBytePtr;
#if __DEFINE_TYPES_						 
typedef Byte*		BytePtr;
#endif

typedef SWord*		SWordPtr;
typedef Word*		WordPtr;
typedef UInt16*	UInt16Ptr;

typedef SDWord*	SDWordPtr;
typedef DWord*		DWordPtr;

// Logical data types
typedef Boolean*	BooleanPtr;

typedef Char*		CharPtr;
typedef SChar*		SCharPtr;
typedef UChar*		UCharPtr;

typedef WChar*		WCharPtr;

typedef Short*		ShortPtr;
typedef UShort*	UShortPtr;

typedef Int*		IntPtr;
typedef UInt*		UIntPtr;

typedef Long*		LongPtr;
typedef ULong*		ULongPtr;


/********************************************************************
 *
 *				Deprecated screen stuff
 *
 ********************************************************************/

#define scrCopy									winPaint
#define scrAND										winErase
#define scrANDNOT									winMask
#define scrXOR										winInvert
#define scrOR										winOverlay
#define scrCopyNOT								winPaintInverse

#define scrDisplayModeGetDefaults			winScreenModeGetDefaults
#define scrDisplayModeGet						winScreenModeGet
#define scrDisplayModeSetToDefaults			winScreenModeSetToDefaults
#define scrDisplayModeSet						winScreenModeSet
#define scrDisplayModeGetSupportedDepths	winScreenModeGetSupportedDepths
#define scrDisplayModeGetSupportsColor		winScreenModeGetSupportsColor

#define ScrOperation								WinDrawOperation

#define ScrDisplayMode(op, widthP, heightP, depthP, enableColorP) \
		  WinScreenMode(op, widthP, heightP, depthP, enableColorP)

#define ScrInit() WinScreenInit()


/********************************************************************
 *
 *				Deprecated SYS_TRAP macro machinery
 *
 ********************************************************************/

// This nWORD_INLINE stuff was never really portable to GCC.  See the
// new SYS_TRAP framework in PalmTypes.h for most uses of these macros.
// Generally if you just want to use CodeWarrior, you can use these
// macros or just use the = { x, y, z} notation directly.  To be
// portable between CodeWarrior and m68k-palmos-gcc, you should use
// the ={x,y,z} notation on the CW side and __attribute__((callseq))
// on the GCC side (see the Palm OS-specific manual).

#if USE_TRAPS == 0
// Disable Palm OS System and Library trap inline code
// Header files become function prototypes for direct-linking
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
// Enable Palm OS System and Library trap inline code
// Header files contain inline glue (opcodes) to be inserted in calling code
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

