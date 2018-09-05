/******************************************************************************
 *
 * Copyright (c) 1994-2004 PalmSource, Inc. All rights reserved.
 *
 * File: CPMLib.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *    This module contains the interface definition for the Cryptographic
 *    Provider Manager library on Pilot.
 *
 *****************************************************************************/

#ifndef CPMLIB_H_
#define	CPMLIB_H_
#include <SystemPublic.h>

#if defined(_WIN32) || defined(__arm) ||CPU_TYPE == CPU_ARM
#include "CPMLibARMInterface.h"
#else
#include "CPMLib68KInterface.h"
#endif

#endif /* CPMLIB_H_ */
