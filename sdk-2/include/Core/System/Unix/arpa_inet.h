/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     arpa/inet.h
 *
 * DESCRIPTION:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <arpa/inet.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 **********************************************************************/

/*	@(#)inet.h 1.8 88/08/19 SMI; from UCB 5.1 5/30/85	*/
/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * External definitions for
 * functions in inet(3N)
 */

#ifndef __UNIXINET_H__
#define __UNIXINET_H__

#include <netinet_in.h>

//===========================================================================
// Macros that convert Unix calls into native Pilot calls.
//===========================================================================
// Convert a dotted decimal string representation into an IP address.
// return -1 on error 
#define	inet_addr(s)			\
						NetLibAddrAToIN(AppNetRefnum, s)
						
// Return a dotted decimal string representation of an IP address
#define	inet_ntoa(in)			\
						NetLibAddrINToA(AppNetRefnum, (in).s_addr, AppINETString)
				

//===========================================================================
// Functions implemented through glue code 
//===========================================================================

// Merge network number and local network address into an IP address
struct	in_addr 	inet_makeaddr(DWord net, DWord lna);

// Return the NBO network number of the dotted decimal string
DWord			 		inet_network(CharPtr s);

// Return the local network address of an IP address
DWord					inet_lnaof(struct in_addr in);

// Return the network number of an IP address
DWord					inet_netof(struct in_addr in);

#endif /*__UNIXINET_H__*/





