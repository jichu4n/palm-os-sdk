/******************************************************************************
 *
 * Copyright (c) 1998-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: OmGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Header file for OmGlue library routines.
 *
 *		Glue providing overlay manager utility routines.
 *
 * History:
 *		02/03/00	kwk	Created by Ken Krugler.
 *
 *****************************************************************************/

#ifndef __OMGLUE_H__
#define __OMGLUE_H__

#include <OverlayMgr.h>

#ifdef __cplusplus
	extern "C" {
#endif

void OmGlueGetCurrentLocale(OmLocaleType *currentLocale);

void OmGlueGetSystemLocale(OmLocaleType *systemLocale);

#ifdef __cplusplus
	}
#endif

#endif	// __OMGLUE_H__
