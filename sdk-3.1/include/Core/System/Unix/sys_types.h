/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot TCP/IP Library
 * FILE:     sys/types.h
 * AUTHOR:	 Ron Marianetti 2/14/96
 *
 * DESCRIPTION:
 *	  This module contains the Pilot equivalent of the "standard" unix
 * header file <sys/types.h>
 *
 **********************************************************************/
#ifndef 	__UNIXTYPES_H__
#define	__UNIXTYPES_H__

#include	<NetMgr.h>						// Include Pilot Net Manager equates



// Common Unix types
typedef	Byte		u_char;
typedef	Word		u_short;
typedef	UInt		u_int;
typedef	DWord		u_long;

typedef	Word		ushort;				/* System V compatibility */
typedef	UInt		uint;					/* System V compatibility */

/* Don't let PilotStdio.h break us */
#undef FILE
#undef size_t

// Define this so we don't get duplicate define warnings. 
#define	__size_t__
typedef	DWord 	size_t;				// must be signed for some unix apps 
//typedef	DWord 	time_t;			
	
typedef  Byte		FILE;

// File Descriptor Utilities, Pilot native forms in <NetMgr.h>
typedef	NetFDSetType	fd_set;
#define	FD_SETSIZE		netFDSetSize

#define	FD_SET(n,p)		netFDSet(n,p)
#define	FD_CLR(n,p)		netFDClr(n,p)
#define	FD_ISSET(n,p)	netFDIsSet(n,p)
#define	FD_ZERO(p)		netFDZero(p)


// Basic functions
#define	bcopy(b1,b2,len)		\
				MemMove(b2,b1,len)

#define	bzero(b,len)		\
				MemSet(b,len,0)
				
#define	bcmp(b1,b2,len) 	\
				MemCmp(b1,b2,len)
				
				

#endif  //__UNIXTYPES_H__
