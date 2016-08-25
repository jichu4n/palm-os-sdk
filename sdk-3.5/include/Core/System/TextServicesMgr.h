/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TextServicesMgr.h
 *
 * Description:
 *		Header file for Text Services Manager.
 *
 * History:
 *			Created by Ken Krugler
 *		03/05/98	kwk	Created by Ken Krugler.
 *		02/03/99	kwk	Changed name to TextServicesMgr.h, was TextServices.h.
 *		10/20/99	kwk	Moved private stuff into TextServicesPrv.h
 *
 *****************************************************************************/

#ifndef __TEXTSERVICESMGR_H__
#define __TEXTSERVICESMGR_H__

#include <PalmTypes.h>
#include <CoreTraps.h>
#include <SystemMgr.h>

#ifndef USE_TSM_TRAPS
	#if EMULATION_LEVEL == EMULATION_NONE
		#define	USE_TSM_TRAPS	1
	#else
		#define	USE_TSM_TRAPS	0
	#endif
#endif

#if USE_TSM_TRAPS
	#define TSM_TRAP(sel) \
		_SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE, sysTrapTsmDispatch, sel)
#else
	#define TSM_TRAP(tsmSelectorNum)
#endif

/***********************************************************************
 * Public constants
 ***********************************************************************/

// Selectors for routines found in the Text Services manager. The order
// of these selectors MUST match the jump table in TextServicesMgr.c.

#ifdef __GNUC__

#define tsmGetFepMode	0
#define tsmSetFepMode	1
#define tsmHandleEvent	2

#define tsmMaxSelector	tsmHandleEvent

typedef UInt16 TsmSelector;

#else

typedef enum {
	tsmGetFepMode = 0,
	tsmSetFepMode,
	tsmHandleEvent,
	
	tsmMaxSelector = tsmHandleEvent,
	tsmBigSelector = 0x7FFF	// Force TsmSelector to be 16 bits.
} TsmSelector;

#endif


// Input mode - used with TsmGetFepMode.

typedef enum {
	tsmFepModeDefault,		// Default 'on' mode.
	tsmFepModeOff,				// FEP is off
	
	tsmFepModeCustom = 128,	// FEP-specific modes start here.
	
	tsmFepModeBig = 0x7fff	// Force TsmFepModeType to be 16 bits.
} TsmFepModeType;


/***********************************************************************
 * Public types
 ***********************************************************************/

// Structure exchanged with many Fep routines. This is how
// the Fep tells the editing code what to display, and how
// to display it. Note that it's also the context record for the
// Fep, thus additional (private) conversion information will
// typically be appended by the Fep.

typedef struct {
	UInt16		refnum;			// Refnum of Fep shared library.
	
	Char			*inlineText;	// ptr to inline text.
	
	UInt16		convertedLen;	// Length of converted text.
	UInt16		pendingLen;		// Length of unconverted (pending) text.
	
	UInt16		selectStart;	// Start of selection range.
	UInt16		selectEnd;		// End of selection range (can extend past
										// end of inline text)
	
	UInt16		convertStart;	// Start of converted clause highlighting
	UInt16		convertEnd;		// End of converted clause highlighting
} TsmFepStatusType;

/***********************************************************************
 * Public routines
 ***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// Return the current mode for the fep indicated by <inStatusP>, or the
// active fep's current session if <inStatusP> is nil.
TsmFepModeType TsmGetFepMode(const TsmFepStatusType *inStatusP)
		TSM_TRAP(tsmGetFepMode);

// Set the mode for the fep indicated by <inStatusP, or the active fep's
// current session if <inStatusP> is nil. The current mode is returned.
TsmFepModeType TsmSetFepMode(TsmFepStatusType *ioStatusP, TsmFepModeType inNewMode)
		TSM_TRAP(tsmSetFepMode);

#ifdef __cplusplus 
}
#endif

#endif  // __TEXTSERVICESMGR_H__

