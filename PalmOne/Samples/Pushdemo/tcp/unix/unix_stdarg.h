/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: unix_stdarg.h
 *
 * Release: 
 *
 * Description:
 *	  This module contains the interface definitions that are 
 * typically found in the unix header <stdarg.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

#ifndef __UNIX_STDARG_H__
#define __UNIX_STDARG_H__

#ifndef __stdarg_h
#define __stdarg_h

#ifndef __va_list__
	#define __va_list__
	typedef char * va_list;		// from <va_list.h>
#endif /* __va_list__ */


/* HFa, ABa, PPo Change stadarg macro to take care about platforms
not ligned on 16 bits */

#define _ONE_MORE_STEP_(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  ( ap = (va_list)&v + _ONE_MORE_STEP_(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _ONE_MORE_STEP_(t)) - _ONE_MORE_STEP_(t)) )
#define va_end(ap)      ( ap = (va_list)0 )


/* Was
#define __va_start(parm)	(va_list) (&parm + 1)

#define va_start(ap, parm)	ap = __va_start(parm)
#define va_end(ap)

#define va_arg(ap, type)	(*       (((type *) (ap += sizeof(type))) - 1)        )
*/

#endif  /* !__stdarg_h */

#endif /* __UNIX_STDARG_H__ */
