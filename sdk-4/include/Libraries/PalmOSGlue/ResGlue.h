/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: ResGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Header file for ResGlue library routines.
 *
 *		Glue providing resource utility routines.
 *
 * History:
 *		02/09/00	kwk	Created by Ken Krugler.
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
