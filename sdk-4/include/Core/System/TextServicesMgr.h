/******************************************************************************
 *
 * Copyright (c) 1998-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TextServicesMgr.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Header file for Text Services Manager. This provides the caller with
 *		an API for interacting with various text services, including front-end
 *		processors (FEPs), which are sometimes known as input methods.
 *
 * History:
 *		03/05/98	kwk	Created by Ken Krugler.
 *		02/03/99	kwk	Changed name to TextServicesMgr.h, was TextServices.h.
 *		10/20/99	kwk	Moved private stuff into TextServicesPrv.h
 *		04/19/00	kwk	Use portable typedef for TsmSelector and TsmFepModeType.
 *							Fixed up descriptions for TsmGet/SetFepMode. Added new
 *							selectors for TsmInit, TsmDrawMode, TsmFepHandleEvent,
 *							TsmFepTerminate, and TsmFepCommit.
 *		07/06/00	kwk	Set type of unused status ptr param to be void*, and
 *							moved TsmFepStatusType into the private header file.
 *		08/21/00	kwk	Moved tsmFtrCreator here from TextServicesPrv.h.
 *							Added tsmFtrNumFlags, tsmFtrFlagsHasFep.
 *		11/15/00	kwk	Added tsmGet/SetSystemFep, tsmGet/SetCurrentFep selectors.
 *
 *****************************************************************************/

#ifndef __TEXTSERVICESMGR_H__
#define __TEXTSERVICESMGR_H__

#include <PalmTypes.h>
#include <CoreTraps.h>
#include <SystemMgr.h>

#ifndef USE_TSM_TRAPS
	#if (EMULATION_LEVEL == EMULATION_NONE)
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

// Feature Creators and numbers, for use with the FtrGet() call.
#define	tsmFtrCreator					sysFileCTextServices

// Selector used with call to FtrGet(tsmFtrCreator, xxx) to get the
// Text Services Manager flags.
#define	tsmFtrNumFlags		0

// Flags returned by FtrGet(tsmFtrCreator, tsmFtrNumFlags) call.
#define	tsmFtrFlagsHasFep	0x00000001L		// Bit set if FEP is installed.

// Selectors for routines found in the Text Services manager. The order
// of these selectors MUST match the jump table in TextServicesMgr.c.
typedef UInt16 TsmSelector;

#define	tsmGetFepMode		0
#define	tsmSetFepMode		1
#define	tsmHandleEvent		2
#define	tsmInit				3	// new in 4.0
#define	tsmDrawMode			4	// new in 4.0
#define	tsmGetSystemFep	5	// new in 4.0
#define	tsmSetSystemFep	6	// new in 4.0
#define	tsmGetCurrentFep	7	// new in 4.0
#define	tsmSetCurrentFep	8	// new in 4.0

#define	tsmMaxSelector		tsmSetCurrentFep

// Input mode - used with TsmGet/SetFepMode.
typedef UInt16 TsmFepModeType;

#define tsmFepModeDefault	((TsmFepModeType)0)
#define tsmFepModeOff		((TsmFepModeType)1)
#define tsmFepModeCustom	((TsmFepModeType)128)


/***********************************************************************
 * Public types
 ***********************************************************************/

/***********************************************************************
 * Public routines
 ***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// Return the current mode for the active FEP. The <nullParam> parameter
// is unused and must be set to NULL.
TsmFepModeType TsmGetFepMode(void* nullParam)
		TSM_TRAP(tsmGetFepMode);

// Set the mode for the active FEP to be <inNewMode>. The previous mode
// is returned. The <nullParam> parameter is unused and must be set
// to NULL.
TsmFepModeType TsmSetFepMode(void* nullParam, TsmFepModeType inNewMode)
		TSM_TRAP(tsmSetFepMode);

#ifdef __cplusplus 
}
#endif

#endif  // __TEXTSERVICESMGR_H__
