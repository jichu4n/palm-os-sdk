/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     GriffitifShift.h
 * AUTHOR:   Art Lamb: Aug 24, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines Griffiti shift state indicator routines.
 *
 **********************************************************************/

#ifndef __GRFSHIFTIND_H__
#define __GRFSHIFTIND_H__


// Graffiti lock flags
#define glfCapsLock   0x01
#define glfNumLock	 0x02


typedef enum { gsiShiftNone,				// no indicator
					gsiNumLock,					// numeric lock
					gsiCapsLock,				// capital lock
					gsiShiftPunctuation,		// punctuation shift
					gsiShiftExtended,			// extented punctuation shift
					gsiShiftUpper,				// alpha upper case shift
					gsiShiftLower	 			// alpha lower case
				 } GsiShiftState;


#ifdef __cplusplus
extern "C" {
#endif

extern void GsiInitialize (void)
							SYS_TRAP(sysTrapGsiInitialize);

extern void GsiSetLocation (const SWord x, const SWord y)
							SYS_TRAP(sysTrapGsiSetLocation);

extern void GsiEnable (const Boolean enableIt)
							SYS_TRAP(sysTrapGsiEnable);

extern Boolean GsiEnabled (void)
							SYS_TRAP(sysTrapGsiEnabled);

extern void GsiSetShiftState (const Word lockFlags, const Word tempShift)
							SYS_TRAP(sysTrapGsiSetShiftState);

#ifdef __cplusplus 
}
#endif

#endif __GRFSHIFTIND_H__


