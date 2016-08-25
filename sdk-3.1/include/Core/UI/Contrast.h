/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:			Software Contrast Control
 *
 * FILE:				Contrast.h
 *
 * DESCRIPTION:	Contrast control module for devices with software contrast.
 *
 * REVISION HISTORY:
 * 	 1998-02-12 Bob		Initial version
 *
 ********************************************************************/

#ifndef	__CONTRAST_H__
#define	__CONTRAST_H__

#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------------------------------
// FUNCTION:		AdjustContrast
// DESCRIPTION:	Handles user interaction with the contrast dialog.
//						Dialog is modal, and will be dismissed if the user
//						presses the contrast button again, or presses the Done
//						button, or any hardware button to launch an app.					
//-----------------------------------------------------------------------
extern void ContrastAdjust()
		SYS_TRAP(sysTrapContrastAdjust);

#ifdef __cplusplus 
}
#endif

#endif	// __CONTRAST_H__
