/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     unix/fcntl.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <fcntl.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/

/*      @(#)fcntlcom.h 1.13 91/06/18 SMI; from UCB fcntl.h 5.2 1/8/86   */

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef __fcntl_h
#define __fcntl_h

/*
 * Rewack the FXXXXX values as _FXXXX so that _POSIX_SOURCE works.
 */
#define _FOPEN          (-1)    /* from sys/file.h, kernel use only */
#define _FREAD          0x0001  /* read enabled */
#define _FWRITE         0x0002  /* write enabled */
#define _FNDELAY        0x0004  /* non blocking I/O (4.2 style) */
#define _FAPPEND        0x0008  /* append (writes guaranteed at the end) */
#define _FSETBLK        0x0010  /* use block offsets */
#define _FASYNC         0x0040  /* signal pgrp when data ready */
#define _FSHLOCK        0x0080  /* BSD flock() shared lock present */
#define _FEXLOCK        0x0100  /* BSD flock() exclusive lock present */
#define _FCREAT         0x0200  /* open with file create */
#define _FTRUNC         0x0400  /* open with truncation */
#define _FEXCL          0x0800  /* error on open if file exists */
#define _FNBIO          0x1000  /* non blocking I/O (sys5 style) */
#define _FSYNC          0x2000  /* do all writes synchronously */
#define _FNONBLOCK      0x4000  /* non blocking I/O (POSIX style) */
#define _FNOCTTY        0x8000  /* don't assign a ctty on this open */
#define _FMARK          0x10000 /* internal; mark during gc() */
#define _FDEFER         0x20000 /* internal; defer for next gc pass */


/*
 * Flags that work for fcntl(fd, F_SETFL, FXXXX)
 */
#define FAPPEND         _FAPPEND
#define FSYNC           _FSYNC
#define FASYNC          _FASYNC
#define FNBIO           _FNBIO
#define FNONBIO         _FNONBLOCK      /* XXX fix to be NONBLOCK everywhere */
#define FNDELAY         _FNDELAY


/* fcntl(2) requests */
#define F_DUPFD         0       /* Duplicate fildes */
#define F_GETFD         1       /* Get fildes flags (close on exec) */
#define F_SETFD         2       /* Set fildes flags (close on exec) */
#define F_GETFL         3       /* Get file flags */
#define F_SETFL         4       /* Set file flags */

/* Prototypes */
#define	fcntl(a,b,c)	NetFCntl(a,b,c)
SWord		NetFCntl(SWord fd, SWord cmd, SWord arg);

#endif  /* !__fcntl_h */
