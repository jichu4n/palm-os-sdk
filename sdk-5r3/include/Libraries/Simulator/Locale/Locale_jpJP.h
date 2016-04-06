/******************************************************************************
 *
 * Copyright (c) 1994-2003 PalmSource, Inc. All rights reserved.
 *
 * File: Locale_jpJP.h
 *
 * Release: Palm OS 5 SDK (68K) R3.
 *
 * Description:
 *	This file is used for CodeWarrior projects incorporating Japanese resources.
 *
 *****************************************************************************/

#ifndef __LOCALE_JPJP_H__
#define __LOCALE_JPJP_H__

#define RESOURCE_FILE_PREFIX				"Localized:jpJP:"
#define LOCALE_SUFFIX						"_jpJP"

#define SIMULATOR_CHARENCODING			charEncodingPalmSJIS
#ifndef SIMULATOR_MEMORYCARD1_SIZE
#define SIMULATOR_MEMORYCARD1_SIZE		0x160000
#endif
#define SIMULATOR_SYSTRES_NAME			"sys_jpJP.tres"

#define SIMULATOR_NONLATIN_SUPPORT		"PalmRez Japanese Support"

#endif
