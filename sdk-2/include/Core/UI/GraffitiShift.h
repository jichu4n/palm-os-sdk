/***********************************************************************
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
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



extern void GsiInitialize (void)
							SYS_TRAP(sysTrapGsiInitialize);

extern void GsiSetLocation (SWord x, SWord y)
							SYS_TRAP(sysTrapGsiSetLocation);

extern void GsiEnable (Boolean enableIt)
							SYS_TRAP(sysTrapGsiEnable);

extern Boolean GsiEnabled (void)
							SYS_TRAP(sysTrapGsiEnabled);

extern void GsiSetShiftState (Word lockFlags, Word tempShift)
							SYS_TRAP(sysTrapGsiSetShiftState);


#endif __GRFSHIFTIND_H__


