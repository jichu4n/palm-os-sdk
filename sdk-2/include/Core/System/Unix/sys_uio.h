/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     sys/uio.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <sys/uio.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/
/*	@(#)uio.h 2.9 89/06/27 SMI; from UCB 7.1 6/4/86	*/

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#ifndef __UNIXUIO_H__
#define __UNIXUIO_H__


struct	iovec {
	BytePtr	iov_base;
	Word		iov_len;
};



#endif /*__UNIXUIO_H__*/

