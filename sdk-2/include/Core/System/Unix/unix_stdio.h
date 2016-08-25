/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     unix/stdio.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <stdio.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/


#ifndef __stdio_h
#define __stdio_h

#include <unix_stdarg.h>

#define		EOF					-1
#define		STDIN_FILENO		sysFileDescStdIn


// The following macros are only included if we're building without the
//  help of the standard ANSI C library, i.e. Native Pilot code only.
// Essentially, if you include <stdlib.h> above this header, this stuff
//  does not get used.
#ifndef		_STDLIB
	#define		sprintf				StrPrintF
	#define		vsprintf(x,y,z)	StrVPrintF(x,(const CharPtr)y,z)

#else
	int vsprintf(char *, const char *, _Va_list);

#endif



// The following are macros that simply substitute an error code
//  for functions which have no equivalent in the PilotOS. To
//  ignore these, #define IGNORE_STDIO_STUBS ahead of this header
#ifndef	IGNORE_STDIO_STUBS
#define		stderr				((unsigned char*)-1)
#define		stdout				((unsigned char*)-1)
#define		stdin					((char*)-1)
#define		STDOUT_FILENO		-1	
		 
#define		fclose(x)			(-1)
#define		fopen(x,y)			(0)
#define		tmpnam(x)			(0)
#define		S_ISREG(x)			(0)

#define		fflush(x)			0
#define		fread(a,b,c,d)		(0)
#define		fwrite(a,b,c,d)	(0)
#define		ferror(x)			(-1)

#define		unlink(s)			(-1)
#define		rmdir(x)				(-1)
#define		mkdir(x)				(-1)
#define		rename(x,y)			(-1)
#define		open(x,y)			(-1)
#define		getcwd(x,y)			(0)

#endif	//IGNORE_STDIO_STUBS


// Function prototypes
int sscanf(const char *, const char *, ...);


#endif  /* !__stdio_h */
