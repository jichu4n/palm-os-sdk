/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     InsPoint.h
 * AUTHOR:   Art Lamb: Jan 25, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines insertion point routines.
 *
 **********************************************************************/

#ifndef __INSPOINT_H__
#define __INSPOINT_H__

// Blink interval is half of a second
#define		insPtBlinkInterval	(sysTicksPerSecond / 2)		
#define		insPtWidth				2

#ifdef __cplusplus
extern "C" {
#endif

extern void InsPtInitialize (void)
							SYS_TRAP(sysTrapInsPtInitialize);

extern void InsPtSetLocation (const SWord x, const SWord y)
							SYS_TRAP(sysTrapInsPtSetLocation);

extern void InsPtGetLocation (const SWordPtr x, const SWordPtr y)
							SYS_TRAP(sysTrapInsPtGetLocation);

extern void InsPtEnable (const Boolean enableIt)
							SYS_TRAP(sysTrapInsPtEnable);

extern Boolean InsPtEnabled (void)
							SYS_TRAP(sysTrapInsPtEnabled);

extern void InsPtSetHeight (const SWord height)
							SYS_TRAP(sysTrapInsPtSetHeight);

extern SWord InsPtGetHeight (void)
							SYS_TRAP(sysTrapInsPtGetHeight);

extern void InsPtCheckBlink (void)
							SYS_TRAP(sysTrapInsPtCheckBlink);

#ifdef __cplusplus 
}
#endif

#endif __INSPOINT_H__
