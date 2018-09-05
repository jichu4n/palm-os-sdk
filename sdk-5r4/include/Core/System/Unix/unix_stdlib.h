/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: unix_stdlib.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <stdlib.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

#ifndef __stdlib_h
#define __stdlib_h

// The following macros are only included if we're building without the
//  help of the standard ANSI C library, i.e. Native Pilot code only.
// Essentially, if you include <stdlib.h> above this header, this stuff
//  does not get used.
#ifndef		_STDLIB

#define		malloc(x)			MemPtrNew(x)
#define		calloc(x,y)			MemCalloc(x,y)
#define		realloc(x,y)		MemRealloc(x,y)
#define		free(x)				MemPtrFree(x)
#define		atoi(x)				StrAToI(x)
#define		atol(x)				StrAToI(x)

#endif

#endif  /* !__stdlib_h */
