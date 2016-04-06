/******************************************************************************
 *
 * Copyright (c) 2002-2003 PalmSource, Inc. All rights reserved.
 *
 * File: Locale_zhCN.h
 *
 * Release: Palm OS 5 SDK (68K) R3.
 *
 * Description:
 *	This file is used for CodeWarrior projects incorporating Simplified Chinese resources.
 *
 *****************************************************************************/

#ifndef __LOCALE_ZHCN_H__
#define __LOCALE_ZHCN_H__

#define RESOURCE_FILE_PREFIX				"Localized:zhCN:"
#define LOCALE_SUFFIX						"_zhCN"

#define SIMULATOR_CHARENCODING			charEncodingPalmGB
#ifndef SIMULATOR_MEMORYCARD1_SIZE
#define SIMULATOR_MEMORYCARD1_SIZE		0x160000
#endif

// We need to bump the app's storage heap size so that the FEP user/learning
// dictionaries can be created/edited.
#ifndef SIMULATOR_MEMORYCARD0_SIZE
#define SIMULATOR_MEMORYCARD0_SIZE		0x00060000
#endif

#define SIMULATOR_SYSTRES_NAME			"sys_zhCN.tres"

#define SIMULATOR_NONLATIN_SUPPORT		"PalmRez ChinSimp Support"

#endif
