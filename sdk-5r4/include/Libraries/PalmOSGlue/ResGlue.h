/******************************************************************************
 *
 * Copyright (c) 2000-2004 PalmSource, Inc. All rights reserved.
 *
 * File: ResGlue.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		Header file for ResGlue library routines.
 *
 *		Glue providing resource utility routines.
 *
 *****************************************************************************/

#ifndef __RESGLUE_H__
#define __RESGLUE_H__

#include <PalmTypes.h>

#ifdef __cplusplus
	extern "C" {
#endif

UInt32 ResGlueLoadConstant(UInt16 rscID);

#ifdef __cplusplus
	}
#endif

#endif	// __RESGLUE_H__
