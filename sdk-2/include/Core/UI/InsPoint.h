/***********************************************************************
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
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
#define		insPtBlinkInterval	30		
#define		insPtWidth				2

extern void InsPtInitialize (void)
							SYS_TRAP(sysTrapInsPtInitialize);

extern void InsPtSetLocation (SWord x, SWord y)
							SYS_TRAP(sysTrapInsPtSetLocation);

extern void InsPtGetLocation (SWordPtr x, SWordPtr y)
							SYS_TRAP(sysTrapInsPtGetLocation);

extern void InsPtEnable (Boolean enableIt)
							SYS_TRAP(sysTrapInsPtEnable);

extern Boolean InsPtEnabled (void)
							SYS_TRAP(sysTrapInsPtEnabled);

extern void InsPtSetHeight (SWord height)
							SYS_TRAP(sysTrapInsPtSetHeight);

extern SWord InsPtGetHeight (void)
							SYS_TRAP(sysTrapInsPtGetHeight);

extern void InsPtCheckBlink (void)
							SYS_TRAP(sysTrapInsPtCheckBlink);


#endif __INSPOINT_H__
