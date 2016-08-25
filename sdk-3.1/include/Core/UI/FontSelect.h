/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     FontSelect.h
 * AUTHOR:	 Art Lamb: September 10, 1997
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines the font selector routine.
 *
 ***********************************************************************/

#ifndef	__SELFONT_H__
#define	__SELFONT_H__


#ifdef __cplusplus
extern "C" {
#endif

extern FontID FontSelect (FontID fontID)
						SYS_TRAP(sysTrapFontSelect);


#ifdef __cplusplus 
}
#endif

#endif // __SELFONT_H__
