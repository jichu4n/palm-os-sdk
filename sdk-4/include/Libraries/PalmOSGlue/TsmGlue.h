/******************************************************************************
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TsmGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Header file for TsmGlueXXX library routines.
 *
 * History:
 *	06/02/99	kwk	Created by Ken Krugler.
 *	07/06/00	kwk	Since the status ptr must now be NULL, changed the type
 *					to be void*.
 *
 *****************************************************************************/

#ifndef __TSMGLUE_H__
#define __TSMGLUE_H__

#include <TextServicesMgr.h>

// Library versions of all the Text Services Mgr routines:

#ifdef __cplusplus
	extern "C" {
#endif

TsmFepModeType
TsmGlueGetFepMode(void* nullParam);

TsmFepModeType
TsmGlueSetFepMode(void* nullParam, TsmFepModeType inNewMode);

#ifdef __cplusplus
	}
#endif

#endif	// __TSMGLUE_H__
