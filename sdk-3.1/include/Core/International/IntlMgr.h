/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * Project	International
 * File		IntlMgr.h
 * Author	Ken Krugler
 * Purpose	Header file for the international manager.
 *
 * Agenda
 *
 * History
 *	21 Mar 98	kwk	New today.
 *	24 Mar 98	kwk	Filled out full set of routine selectors.
 *	05 Apr 98	kwk	Added static keyword to const in effort to get this
 *					to work w/standard C compilers.
 *	08 Apr 98	kwk	Deleted intlTxtCompareString and intlTxtCompareText
 *					routine selectors.
 *				kwk	Added intlTxtWordBounds routine selector.
 *	17 Apr 98	kwk	Added intlTxtReplaceStr & intlTxtTruncateStr selectors.
 *	27 May 98	kwk	Removed intlTxtToLower and intlTxtTruncateStr selectors
 *					(unused routines).
 *				kwk	Added intlIntlInit selector.
 *				kwk	Added intlTxtCharEncoding & intlTxtStrEncoding selectors.
 *				kwk	Added intlTxtEncodingName selector.
 *	01 Jun 98	kwk	Added intlTxtMaxEncoding selector.
 *	10 Jun 98	kwk	Added intlTxtTransliterate selector.
 *	16 Jun 98	kwk	Added intlTxtIsValidChar selector.
 *	18 Jun 98	kwk	Added intlTxtCompare & intlTxtCaselessCompare selectors.
 *	30 Jun 98	kwk	Moved IntlDispatch declaration here from IntlPrv.h.
 *	28 Jul 98	kwk	Changed TxtGetPrevChar to TxtGetPreviousChar.
 *				kwk	Deleted intlTxtPrevCharSize & intlTxtNextCharSize
 *					selectors.
 *				kwk	Added intlTxtGetChar selector.
 *	26 Aug 98	kwk	Added intlTxtCharWidth selector.
 *	10 Sep 98	kwk	Added intlTxtTruncate selector.
 *	15 Sep 98	kwk	Removed unused IntlQuit() routine declaration.
 *	14 Oct 98	kwk	Added intlIntlGetRoutineAddress selector and
 *					IntlGetRoutineAddress routine declaration.
 *	16 Feb 99	kwk	Changed TxtTruncate to TxtGetTruncationOffset.
 *
 ***********************************************************************/

#ifndef __INTLMGR_H__
#define __INTLMGR_H__

// If nobody has explicitly specified that we should or should not
// use our international trap dispatcher, set it based on the emulation
// level.

#ifndef USE_INTL_TRAPS
	#if EMULATION_LEVEL == EMULATION_NONE
		#define	USE_INTL_TRAPS	1
	#else
		#define	USE_INTL_TRAPS	0
	#endif
#endif

/***********************************************************************
 * Public constants
 ***********************************************************************/

// Bits set for the Intl Mgr feature.

#define	kIntlMgrExists	0x00000001

// International Manager trap macros.

#define m68kMoveQd2Instr	0x7400

#if USE_INTL_TRAPS
	#define INTL_TRAP(intlSelectorNum) \
				THREEWORD_INLINE(m68kMoveQd2Instr + intlSelectorNum, \
					m68kTrapInstr+sysDispatchTrapNum, sysTrapIntlDispatch)
#else
	#define INTL_TRAP(intlSelectorNum)
#endif

// Selectors for routines found in the international manager. The order
// of these selectors MUST match the jump table in IntlDispatch.cp.

typedef enum {
	intlIntlInit = 0,
	intlTxtByteAttr,
	intlTxtCharAttr,
	intlTxtCharXAttr,
	intlTxtCharSize,
	intlTxtGetPreviousChar,
	intlTxtGetNextChar,
	intlTxtGetChar,
	intlTxtSetNextChar,
	intlTxtCharBounds,
	intlTxtPrepFindString,
	intlTxtFindString,
	intlTxtReplaceStr,
	intlTxtWordBounds,
	intlTxtCharEncoding,
	intlTxtStrEncoding,
	intlTxtEncodingName,
	intlTxtMaxEncoding,
	intlTxtTransliterate,
	intlTxtIsValidChar,
	intlTxtCompare,
	intlTxtCaselessCompare,
	intlTxtCharWidth,
	intlTxtGetTruncationOffset,
	intlIntlGetRoutineAddress,
	
	intlMaxSelector = intlIntlGetRoutineAddress,
	intlBigSelector = 0x7FFF	// Force IntlSelector to be 16 bits.
} IntlSelector;

/***********************************************************************
 * Public routines
 ***********************************************************************/

#ifdef __cplusplus
	extern "C" {
#endif

// Return back the address of the routine indicated by <inSelector>. If
// <inSelector> isn't a valid routine selector, return back NULL.

VoidPtr IntlGetRoutineAddress(IntlSelector inSelector)
		INTL_TRAP(intlIntlGetRoutineAddress);

// kwk - Move this, and other routine declarations, into IntlPrv.h which is
// new file in International folder. Rename existing IntlPrv.h in IntlMgr
// extension folder to be something like IntlExtShiftJIS.h

// Dispatcher that uses contents of register D2 to dispatch to the
// appropriate int'l routine. This routine declaration is only used
// when setting up the trap dispatch table; all callers of routines
// accessed via the dispatcher should use the explicit routine declarations
// (e.g. IntlInit below), which set up D2 before calling IntlDispatch.

void IntlDispatch(void);

// Initialization routine, called at system reset time by PalmOS..

void IntlInit(void)
		INTL_TRAP(intlIntlInit);

#ifdef __cplusplus
	}
#endif

#endif // __INTLMGR_H__
