/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: netdb.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <netdb.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 * History:
 *		2/14/96	Created by Ron Marianetti
 *
 *****************************************************************************/

/*	@(#)netdb.h 1.11 88/08/19 SMI from UCB 5.9 4/5/88	*/
/*
 * Copyright (c) 1980,1983,1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 */



#ifndef __UNIX_NETDB_H__
#define __UNIX_NETDB_H__

#include <PalmTypes.h>


/*
 * Structures returned by network data base library.
 * All addresses are supplied in host order, and
 * returned in network order (suitable for use in system calls).
 */

struct	hostent {
	Char	*h_name;						/* official name of host */
	Char	**h_aliases;				/* alias list */
	UInt16 h_addrtype;				/* host address type */
	UInt16 h_length;					/* length of address */
	Char	**h_addr_list;				/* list of addresses from name server */
#define	h_addr	h_addr_list[0]	/* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct	netent {
	Char		*n_name;					/* official name of net */
	Char		**n_aliases;			/* alias list */
	UInt16	n_addrtype;				/* net address type */
	UInt32	n_net;					/* network # */
};

struct	servent {
	Char	*s_name;						/* official service name */
	Char	**s_aliases;				/* alias list */
	UInt16 s_port;						/* port # */
	Char	*s_proto;					/* protocol to use */
};

struct	protoent {
	Char	*p_name;						/* official protocol name */
	Char	**p_aliases;				/* alias list */
	UInt16 p_proto;					/* protocol # */
};

struct rpcent {
	Char	*r_name;						/* name of server for this rpc program */
	Char	**r_aliases;				/* alias list */
	Int16	r_number;					/* rpc program number */
};


// Supported calls
#define	gethostbyname(nameP) 	\
				((struct hostent*)NetLibGetHostByName(AppNetRefnum, nameP,&AppHostInfo,AppNetTimeout,&h_errno))
			
#define	gethostbyaddr(addr,len,type) 	\
				((struct hostent*)NetLibGetHostByAddr(AppNetRefnum, (UInt8 *)addr,len,type,&AppHostInfo,AppNetTimeout,&h_errno))

#define	getservbyname(serv,proto)		\
				((struct servent*)NetLibGetServByName(AppNetRefnum, serv,proto,&AppServInfo,AppNetTimeout,&h_errno))
				

// Unsupported calls
// struct hostent	 *gethostent();
// struct netent	*getnetbyname(), *getnetbyaddr(), *getnetent();
// struct servent	*getservbyport(), *getservent();
// struct protoent	*getprotobyname(), *getprotobynumber(), *getprotoent();
// struct rpcent	*getrpcbyname(), *getrpcbynumber(), *getrpcent();

/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (when using the resolver)
 */

extern  Err 		h_errno;				

#define	HOST_NOT_FOUND	1 				/* Authoritive Answer Host not found */
#define	TRY_AGAIN		2 				/* Non-Authoritive Host not found, or SERVERFAIL */
#define	NO_RECOVERY		3 				/* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define	NO_DATA			4 				/* Valid name, no data record of requested type */
#define	NO_ADDRESS		NO_DATA		/* no address, look for MX record */


#endif	/* __UNIX_NETDB_H__ */
