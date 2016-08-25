/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: sys_time.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This module contains the equivalent of the unix header <sys/time.h>
 *	for use in Pilot applications that wish to use the Berkeley sockets API
 *
 * History:
 *		5/23/96	Created by Ron Marianetti
 *
 *****************************************************************************/

#ifndef __UNIX_SYS_TIME_H__
#define __UNIX_SYS_TIME_H__

#include <PalmTypes.h>


/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */

struct timeval {
	Int32	tv_sec;			/* seconds */
	Int32	tv_usec;			/* and microseconds */
};


struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;			/* type of dst correction */
};

#define	DST_NONE		0		/* not on dst */
#define	DST_USA		1		/* USA style dst */
#define	DST_AUST		2		/* Australian style dst */
#define	DST_WET		3		/* Western European dst */
#define	DST_MET		4		/* Middle European dst */
#define	DST_EET		5		/* Eastern European dst */
#define	DST_CAN		6		/* Canada */
#define	DST_GB		7		/* Great Britain and Eire */
#define	DST_RUM		8		/* Rumania */
#define	DST_TUR		9		/* Turkey */
#define	DST_AUSTALT	10		/* Australian style with shift in 1986 */

	
#endif /* __UNIX_SYS_TIME_H__ */
