/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1995 -- All Rights Reserved
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


extern void AbtShowAbout (DWord creator)
		SYS_TRAP(sysTrapAbtShowAbout);


#endif // __ABOUTBOX_H__
