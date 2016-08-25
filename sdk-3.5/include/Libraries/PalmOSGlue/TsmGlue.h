/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TsmGlue.h
 *
 * Description:
 *         	Header file for TsmGlueXXX library routines.
 *
 * Written by TransPac Software, Inc.
 *
 * History:
 *			Created by Ken Krugler
 *	02 Jun 99	kwk	New today.
 *
 *****************************************************************************/

#ifndef __TSMGLUE_H__
#define __TSMGLUE_H__

// Library versions of all the Text Services Mgr routines:

#ifdef __cplusplus
	extern "C" {
#endif

TsmFepModeType
TsmGlueGetFepMode(const TsmFepStatusType* inStatusP);

TsmFepModeType
TsmGlueSetFepMode(TsmFepStatusType* ioStatusP, TsmFepModeType inNewMode);

#ifdef __cplusplus
	}
#endif

#endif	// __TSMGLUE_H__

