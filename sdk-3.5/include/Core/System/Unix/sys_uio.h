/******************************************************************************
 *
 * Copyright (c) 1996 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: sys_uio.h
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <sys/uio.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

/*	@(#)uio.h 2.9 89/06/27 SMI; from UCB 7.1 6/4/86	*/

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#ifndef __UNIX_SYS_UIO_H__
#define __UNIX_SYS_UIO_H__


struct	iovec {
	UInt8 *	iov_base;
	UInt16	iov_len;
};



#endif /*__UNIXUIO_H__*/

