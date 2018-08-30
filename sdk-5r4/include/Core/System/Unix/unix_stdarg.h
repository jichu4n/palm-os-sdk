/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: unix_stdarg.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *	  This module contains the interface definitions that are 
 * typically found in the unix header <stdarg.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

#if defined(EMULATION_LEVEL) && (EMULATION_LEVEL != EMULATION_NONE)

#include <stdarg.h>				// use the native stdarg

#else

#ifndef __stdarg_h
#define __stdarg_h

#ifndef __va_list__
	#define __va_list__
	typedef char * va_list;		// from <va_list.h>
#endif /* __va_list__ */

#define __va_start(parm)	(va_list) (&parm + 1)

#define va_start(ap, parm)	ap = __va_start(parm)
#define va_end(ap)

#define va_arg(ap, type)	(*       (((type *) (ap += sizeof(type))) - 1)        )

#endif  /* !__stdarg_h */
#endif	/* EMULATION_LEVEL != EMULATION_NONE */
