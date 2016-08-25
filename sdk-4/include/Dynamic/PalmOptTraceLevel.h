/******************************************************************************
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: PalmOptTraceLevel.Off
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 *****************************************************************************/

#ifndef __PALMOPTTRACELEVEL_H__
#define __PALMOPTTRACELEVEL_H__

#include <BuildDefines.h>

	#ifdef TRACE_OUTPUT_OK_TO_REDEFINE
		#undef TRACE_OUTPUT_OK_TO_REDEFINE
		#undef TRACE_OUTPUT
	#endif
	
	#ifndef TRACE_OUTPUT
		#define TRACE_OUTPUT TRACE_OUTPUT_OFF
	#endif
	
#endif
