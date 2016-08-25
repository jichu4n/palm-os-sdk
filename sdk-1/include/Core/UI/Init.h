/***********************************************************************
 *
 *      Copyright (c) Palm Computing 1995 -- All Rights Reserved
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

extern void UIInitialize (void)
							SYS_TRAP(sysTrapUIInitialize);

extern void UIReset (void)
							SYS_TRAP(sysTrapUIReset);

#endif __INIT_H__
