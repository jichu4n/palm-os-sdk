/******************************************************************************
 *
 * Copyright (c) 1995-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: GraffitiShift.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *        This file defines Griffiti shift state indicator routines.
 *
 * History:
 *		Aug 24, 1995	Created by Art Lamb
 *      mm/dd/yy   initials - brief revision comment
 *
 *****************************************************************************/

#ifndef __GRAFFITISHIFT_H__
#define __GRAFFITISHIFT_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

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

extern void GsiSetLocation (const Int16 x, const Int16 y)
							SYS_TRAP(sysTrapGsiSetLocation);

extern void GsiEnable (const Boolean enableIt)
							SYS_TRAP(sysTrapGsiEnable);

extern Boolean GsiEnabled (void)
							SYS_TRAP(sysTrapGsiEnabled);

extern void GsiSetShiftState (const UInt16 lockFlags, const UInt16 tempShift)
							SYS_TRAP(sysTrapGsiSetShiftState);

#ifdef __cplusplus 
}
#endif

#endif //__GRAFFITISHIFT_H__
