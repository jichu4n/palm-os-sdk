/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     unix/string.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <string.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/


#ifndef __string_h
#define __string_h

#include <unix_stdarg.h>


#define		strcpy(x,y)			StrCopy((Char*)x,(Char*)y)
#define		strncpy(x,y,z)		StrNCopy(x,y,z)
#define		strcmp(x,y)			StrCompare(x,y)
#define		strcat(x,y)			StrCat(x,y)
#define		strlen(x)			StrLen((Char*)x)
#define		strchr(x,y)			StrChr(x,y)
#define		strstr(x,y)			StrStr(x,y)

#define		memcpy(x,y,z)		(MemMove(x,(VoidPtr)y,z) ? x : x)
#define		memset(x,y,z)		(MemSet(x,z,y))
#define		memcmp(x,y,z)		(MemCmp(x,y,z))
#define		index(x,y)			StrChr(x,y)



#endif  /* !__string_h */
