/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: netinet_tcp.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This module contains the interface definitions that are 
 *	typically found in the unix header <netinet/tcp.h> for use by 
 * Pilot applications that wish to use the sockets API calls.
 *
 *****************************************************************************/

/*      @(#)tcp.h 1.11 88/08/19 SMI; from UCB 7.2 10/28/86      */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef __UNIX_NETINET_TCP_H__
#define __UNIX_NETINET_TCP_H__

#include <PalmTypes.h>

typedef UInt32  tcp_seq;
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr {
        UInt16 th_sport;               /* source port */
        UInt16 th_dport;               /* destination port */
        tcp_seq th_seq;                /* sequence number */
        tcp_seq th_ack;         			/* acknowledgement number */
        UInt8 	th_off:4,               /* data offset */
               th_x2:4;                /* (unused) */
        UInt8  th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
        UInt16 th_win;                 /* window */
        UInt16 th_sum;                 /* checksum */
        UInt16 th_urp;                 /* urgent pointer */
};

#define TCPOPT_EOL      0
#define TCPOPT_NOP      1
#define TCPOPT_MAXSEG   2

/*
 * Default maximum segment size for TCP.
 * With an IP MSS of 576, this is 536,
 * but 512 is probably more convenient.
 */
#ifdef  lint
#define TCP_MSS 536
#else
#define TCP_MSS MIN(512, IP_MSS - sizeof (struct tcpiphdr))
#endif

/*
 * User-settable options (used with setsockopt).
 */
#define TCP_NODELAY     netSocketOptTCPNoDelay   /* don't delay send to coalesce packets */
#define TCP_MAXSEG      netSocketOptTCPMaxSeg    /* set maximum segment size */

#endif /* __UNIX_NETINET_TCP_H__ */
