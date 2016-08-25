/******************************************************************************
 *
 * Copyright (c) 1996 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: sys_socketvar.h
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <sys/socketvar> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 * History:
 *		2/14/96	Created by Ron Marianetti
 *
 *****************************************************************************/

#ifndef __UNIX_SYS_SOCKETVAR_H__
#define __UNIX_SYS_SOCKETVAR_H__


/*
 * Socket state bits.
 */
#define	SS_NOFDREF				0x001	/* no file table ref any more */
#define	SS_ISCONNECTED			0x002	/* socket connected to a peer */
#define	SS_ISCONNECTING		0x004	/* in process of connecting to peer */
#define	SS_ISDISCONNECTING	0x008	/* in process of disconnecting */
#define	SS_CANTSENDMORE		0x010	/* can't send more data to peer */
#define	SS_CANTRCVMORE			0x020	/* can't receive more data from peer */
#define	SS_RCVATMARK			0x040	/* at mark on input */

#define	SS_PRIV					0x080	/* privileged for broadcast, raw... */
#define	SS_NBIO					0x100	/* non-blocking ops */
#define	SS_ASYNC					0x200	/* async i/o notify */
#define 	SS_PIPE					0x400	/* pipe behavior for POSIX & SVID */

#endif	/* __UNIX_SYS_SOCKETVAR_H__ */

