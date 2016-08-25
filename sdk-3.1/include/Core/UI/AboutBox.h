/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     AboutBox.h
 * AUTHOR:	 Christopher Raff: October 25th, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines About Box routines
 *
 ***********************************************************************/
#ifndef __ABOUTBOX_H__
#define __ABOUTBOX_H__

#ifdef __cplusplus
extern "C" {
#endif

extern void AbtShowAbout (const DWord creator)
		SYS_TRAP(sysTrapAbtShowAbout);
		
#ifdef __cplusplus 
}
#endif

#endif // __ABOUTBOX_H__
