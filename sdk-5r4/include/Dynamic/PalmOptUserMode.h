/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: PalmOptUserMode.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 *****************************************************************************/

#include <BuildDefines.h>
#ifdef USER_MODE
#undef USER_MODE
#endif
#define USER_MODE USER_MODE_NORMAL
