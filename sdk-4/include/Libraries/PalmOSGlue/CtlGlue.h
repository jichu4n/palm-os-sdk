/******************************************************************************
 *
 * Copyright (c) 2001 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: CtlGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Glue providing compatibility for applications that wish
 *		to make calls to the Control Manager, but which might be running
 *		on a system which does not support newer calls.
 *
 * History:
 *		01/08/00	tlw	Created by Tim Wiegman.
 *
 *****************************************************************************/

#ifndef __CTLGLUE_H__
#define __CTLGLUE_H__

#include <Control.h>

#ifdef __cplusplus
	extern "C" {
#endif

ControlStyleType CtlGlueGetControlStyle(const ControlType *ctlP);

#ifdef __cplusplus
	}
#endif

#endif	// __CTLGLUE_H__
