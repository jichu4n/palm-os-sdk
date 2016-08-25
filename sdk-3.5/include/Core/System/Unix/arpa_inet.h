/******************************************************************************
 *
 * Copyright (c) 1996 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: arpa_inet.h
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <arpa/inet.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

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

#ifndef __UNIX_ARPA_INET_H__
#define __UNIX_ARPA_INET_H__

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

#ifdef __cplusplus
extern "C" {
#endif

// Merge network number and local network address into an IP address
struct	in_addr 	inet_makeaddr(UInt32 net, UInt32 lna);

// Return the NBO network number of the dotted decimal string
UInt32			 		inet_network(Char * s);

// Return the local network address of an IP address
UInt32					inet_lnaof(struct in_addr in);

// Return the network number of an IP address
UInt32					inet_netof(struct in_addr in);

#ifdef __cplusplus 
}
#endif

#endif /* __UNIX_ARPA_INET_H__ */

