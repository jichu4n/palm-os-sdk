/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: sys_socket.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <sys/socket.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 * History:
 *		2/14/96	Created by Ron Marianetti
 *
 *****************************************************************************/

#ifndef __UNIX_SYS_SOCKET_H__
#define __UNIX_SYS_SOCKET_H__

// Include System headers
#include <PalmTypes.h>

// Include the Pilot Net Manager defines
#include <NetMgr.h>

// Include other Unix headers
#include	<sys_types.h>
#include <sys_time.h>
#include <netinet_in.h>
#include <arpa_inet.h>
#include <sys_socketvar.h>
#include <unix_netdb.h>
#include <sys_uio.h>
#include <sys_errno.h>
#include <netinet_tcp.h>
#include <unix_fcntl.h>
#include <unix_string.h>
#include <unix_stdio.h>
#include <unix_stdlib.h>

// This is a set of unix calls that get stubbed out or replaced by
//  PilotOS calls.
//
// Although these don't really "belong" here, it seems like the best solution
// rather than having a ton of other unix like header files that don't
// provide all the functionality that unix does. 
#define		sleep(x)				SysTaskDelay(x*sysTicksPerSecond)
#define		isatty(x)			1
#define		fork()				-1			// can't support fork
#define		abort(x)

// The following macros are only included if we're building without the
//  help of the standard ANSI C library, i.e. Native Pilot code only.
// Essentially, if you include <stdlib.h> above this header, this stuff
//  does not get used.
#ifndef		_STDLIB
#define		getenv(x)			0
#endif

/*===================================================================
 * Definitions related to sockets: types, address families, options.
 ===================================================================*/

/*
 * Types
 */
#define	SOCK_STREAM		netSocketTypeStream			/* stream socket */
#define	SOCK_DGRAM		netSocketTypeDatagram		/* datagram socket */
#define	SOCK_RAW			netSocketTypeRaw				/* raw-protocol interface */
#define  SOCK_RDM			netSocketTypeReliableMsg	/* reliable message */

/*
 * Option flags per-socket.
 */
#define	SO_DEBUG			netSocketOptSockDebug			/* turn on debugging info recording */
#define	SO_ACCEPTCONN	netSocketOptSockAcceptConn		/* socket has had listen() */
#define	SO_REUSEADDR	netSocketOptSockReuseAddr		/* allow local address reuse */
#define	SO_KEEPALIVE	netSocketOptSockKeepAlive		/* keep connections alive */
#define	SO_DONTROUTE	netSocketOptSockDontRoute		/* just use interface addresses */
#define	SO_BROADCAST	netSocketOptSockBroadcast		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK	netSocketOptSockUseLoopback	/* bypass hardware when possible */
#define	SO_LINGER		netSocketOptSockLinger			/* linger on close if data present */
#define	SO_OOBINLINE	netSocketOptSockOOBInLine		/* leave received OOB data in line */
/*
 * N.B.: The following definition is present only for compatibility
 * with release 3.0.  It will disappear in later releases.
 */
#define	SO_DONTLINGER	(~SO_LINGER)	/* ~SO_LINGER */

/*
 * Additional options, not kept in so_options.
 */
#define	SO_SNDBUF		netSocketOptSockSndBufSize			/* send buffer size */
#define	SO_RCVBUF		netSocketOptSockRcvBufSize			/* receive buffer size */
#define	SO_SNDLOWAT		netSocketOptSockSndLowWater		/* send low-water mark */
#define	SO_RCVLOWAT		netSocketOptSockRcvLowWater		/* receive low-water mark */
#define	SO_SNDTIMEO		netSocketOptSockSndTimeout			/* send timeout */
#define	SO_RCVTIMEO		netSocketOptSockRcvTimeout			/* receive timeout */
#define	SO_ERROR			netSocketOptSockErrorStatus		/* get error status and clear */
#define	SO_TYPE			netSocketOptSockSocketType			/* get socket type */

/*
 * Structure used for manipulating linger option.
 */
struct	linger {
	Int16	l_onoff;													/* option on/off */
	Int16	l_linger;												/* linger time */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET	netSocketOptLevelSocket				/* options for socket level */

/*
 * Address families.
 */
#define	AF_INET		netSocketAddrINET				/* internetwork: UDP, TCP, etc. */
#define	AF_RAW		netSocketAddrRaw				/* raw access to data-link interface */

/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr {
	Int16	sa_family;										/* address family */
	UInt8	sa_data[14];									/* up to 14 bytes of direct address */
};

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto {
	UInt16	sp_family;										/* address family */
	UInt16	sp_protocol;									/* protocol */
};

/*
 * Protocol families, same as address families for now.
 */  
#define	PF_INET		AF_INET
#define	PF_RAW		AF_RAW

/*
 * Maximum queue length specifiable by listen.
 */
#define	SOMAXCONN	5

/*
 * Message header for recvmsg and sendmsg calls.
 */
struct msghdr {
	Char *	msg_name;									/* optional address */
	Int16		msg_namelen;								/* size of address */
	struct	iovec *msg_iov;							/* scatter/gather array */
	Int16		msg_iovlen;									/* # elements in msg_iov */
	Char *	msg_accrights;								/* access rights sent/received */
	Int16		msg_accrightslen;
	};

#define	MSG_OOB			netIOFlagOutOfBand		/* process out-of-band data */
#define	MSG_PEEK			netIOFlagPeek				/* peek at incoming message */
#define	MSG_DONTROUTE	netIOFlagDontRoute		/* send without using routing tables */

#define	MSG_MAXIOVLEN	netIOVecMaxLen


//===========================================================================
// Globals used by the Sockets API. These are declard in the Socket.c module
// which must be linked in with any application that uses the sockets API.
//===========================================================================
extern 	Err						errno;
extern	Int32						AppNetTimeout;
extern	Char						AppINETString[];
extern	NetHostInfoBufType	AppHostInfo;
extern	NetServInfoBufType	AppServInfo;
extern	UInt16					AppNetRefnum;


//===========================================================================
// Macros that convert BSD Sockets calls into native Pilot calls.
//===========================================================================
#define	accept(socket,addr,addrlen)							\
				NetLibSocketAccept(AppNetRefnum,socket,(NetSocketAddrType*)addr,(Int16 *)addrlen,AppNetTimeout,&errno)
				
#define	bind(socket,localaddr,addrlen)						\
				NetLibSocketBind(AppNetRefnum, socket,(NetSocketAddrType*)localaddr,addrlen,AppNetTimeout,&errno)
				
#define	close(socket) 												\
				NetLibSocketClose(AppNetRefnum, socket,AppNetTimeout,&errno)
				
#define	connect(socket,addr,addrlen)							\
				NetLibSocketConnect(AppNetRefnum, socket,(NetSocketAddrType*)addr,addrlen,AppNetTimeout,&errno)
				
#define	getsockopt(socket,level,opt,optvalP,optlenP)		\
				NetLibSocketOptionGet(AppNetRefnum, socket,level,opt,optvalP,(UInt16 *)optlenP,AppNetTimeout,&errno)
				
#define	listen(socket,queuelen)									\
				NetLibSocketListen(AppNetRefnum, socket,queuelen,AppNetTimeout,&errno)
				
#define	read(socket,buffer,buflen)								\
				NetLibReceive(AppNetRefnum, socket,buffer,buflen,0,0,0,AppNetTimeout,&errno)
				
#define	recv(socket,buffer,buflen,flags)						\
				NetLibReceive(AppNetRefnum, socket,buffer,buflen,flags,0,0,AppNetTimeout,&errno)
				
#define	recvfrom(socket,buffer,buflen,flags,from,fromlen) \
				NetLibReceive(AppNetRefnum, socket,buffer,buflen,flags,from,(UInt16 *)fromlen,AppNetTimeout,&errno)
				
#define	recvmsg(socket,msg,flags)								\
				NetLibReceivePB(AppNetRefnum, socket,(NetIOParamType*)msg,flags,AppNetTimeout,&errno)
				
#define	send(socket,buf,buflen,flags)							\
				NetLibSend(AppNetRefnum, socket,buf,buflen,flags,0,0,AppNetTimeout,&errno)
				
#define	sendmsg(socket,msg,flags)								\
				NetLibSendPB(AppNetRefnum, socket,(NetIOParamType*)msg,flags,AppNetTimeout,&errno)
				
#define	sendto(socket,buf,buflen,flags,to,tolen)			\
				NetLibSend(AppNetRefnum, socket,buf,buflen,flags,to,tolen,AppNetTimeout,&errno)
				
#define	setsockopt(socket,level,opt,optvalP,optlen)		\
				NetLibSocketOptionSet(AppNetRefnum, socket,level,opt,optvalP,optlen,AppNetTimeout,&errno)
				
#define	shutdown(socket,direction)								\
				NetLibSocketShutdown(AppNetRefnum, socket,direction,AppNetTimeout,&errno)
				
#define	socket(domain,type,protocol)							\
				NetLibSocketOpen(AppNetRefnum, domain,type,protocol,AppNetTimeout,&errno)

#define	write(socket,buf,buflen)								\
				NetLibSend(AppNetRefnum, socket,(void *) buf,buflen,0,0,0,AppNetTimeout,&errno)
				
#define	getpeername(socket,nameP,nameLenP)					\
				NetLibSocketAddr(AppNetRefnum, socket, 0, 0, (NetSocketAddrType*)nameP, (Int16 *)nameLenP, AppNetTimeout, &errno)			
				
#define	getsockname(socket,nameP,nameLenP)					\
				NetLibSocketAddr(AppNetRefnum, socket, (NetSocketAddrType*)nameP, (Int16 *)nameLenP, 0, 0, AppNetTimeout, &errno)			
				

//===========================================================================
// Sockets API calls that go through glue
//===========================================================================

#ifdef __cplusplus
extern "C" {
#endif

Int16		getdomainname(Char * nameP, Int16 namelen);

Int16		setdomainname(Char * nameP, Int16 namelen);

Int16		gethostname(Char * nameP, Int16 namelen);

Int16		sethostname(Char * nameP, Int16 namelen);

Int16		select(Int16 width, fd_set* readfds, fd_set* writefds,
					fd_set* exceptfds, struct timeval* timeout);

Int16		gettimeofday(struct timeval* tp, struct timezone* tzp);

Int16		settimeofday(struct timeval* tp, struct timezone* tzp);


void *	MemCalloc(UInt32 nmemb, UInt32 size);

void *	MemRealloc(void * p, UInt32 size);

#ifdef __cplusplus 
}
#endif



//===========================================================================
// These are high level convenience functions.
//===========================================================================

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the NetSocket glue code globals if not already initialized.
// In particular, this sets up the AppNetRefnum global with the refnum of
//  the NetLib. 
Err 				NetUInit (void);

// Open up a TCP socket and connect to the given host. If port is 0, the port
//  number will be looked up by the serviceName parameter. Returns socket
//  descriptor or -1 on error
NetSocketRef	NetUTCPOpen(const Char * hostName, const Char * serviceName, Int16 port);

// Read N bytes from a descriptor. This call automatically makes repeated read
//  calls to the socket until all N bytes have been received
// Returns number of bytes read or -1 if error
Int32				NetUReadN(NetSocketRef fd, UInt8 * bufP, UInt32 numBytes);

		
// Write N bytes to a descriptor. This call automatically makes repeated write
//  calls to the socket until all N bytes have been sent
// Returns number of bytes written or -1 if error
Int32				NetUWriteN(NetSocketRef fd, UInt8 * bufP, UInt32 numBytes);

#ifdef __cplusplus 
}
#endif


#endif /* __UNIX_SYS_SOCKET_H__ */
