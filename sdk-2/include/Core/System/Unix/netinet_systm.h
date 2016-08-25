/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     netinet/in_systm.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <netinet/in_systm.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/
/*      @(#)in_systm.h 1.8 88/08/19 SMI; from UCB 7.1 6/5/86    */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * Miscellaneous internetwork
 * definitions for kernel.
 */

#ifndef __UNIX_NETINET_IN_SYSTM_H
#define __UNIX_NETINET_IN_SYSTM_H

/*
 * Network types.
 *
 * Internally the system keeps counters in the headers with the bytes
 * swapped so that VAX instructions will work on them.  It reverses
 * the bytes before transmission at each protocol level.  The n_ types
 * represent the types with the bytes in ``high-ender'' order.
 */
typedef u_short n_short;                /* short as received from the net */
typedef u_long  n_long;                 /* long as received from the net */

typedef u_long  n_time;                 /* ms since 00:00 GMT, byte rev */

#endif /*!__UNIX_NETINET_IN_SYSTM_H*/
