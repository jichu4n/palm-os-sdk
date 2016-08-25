/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     unix/stdarg.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 * typically found in the unix header <stdarg.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/


#ifndef __stdarg_h
#define __stdarg_h

typedef char * va_list;		// from <va_list.h>

#define __va_start(parm)	(__std(va_list)) (&parm + 1)

#define va_start(ap, parm)	ap = __va_start(parm)
#define va_end(ap)

#define va_arg(ap, type)	(*       (((type *) (ap += sizeof(type))) - 1)        )

#endif  /* !__stdarg_h */
