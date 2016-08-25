/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Init.h
 * AUTHOR:   Art Lamb: Jan 25, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines UI initialization routines.
 *
 **********************************************************************/

#ifndef __INIT_H__
#define __INIT_H__


#ifdef __cplusplus
extern "C" {
#endif

extern void UIInitialize (void)
							SYS_TRAP(sysTrapUIInitialize);

extern void UIReset (void)
							SYS_TRAP(sysTrapUIReset);

#ifdef __cplusplus 
}
#endif


#endif __INIT_H__
