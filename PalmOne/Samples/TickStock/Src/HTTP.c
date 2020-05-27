/******************************************************************************
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/
/**
 * @file  HTTP.c
 * @brief Source code for basic socket operations.
 *
 */
 
#include <PalmOS.h>
#include "TicksTock.h"

Boolean InitNetLib(UInt16 *refNum, Err *error)
{
    UInt16 ifErrs = 0;
    UInt16 pre_count = 0, post_count = 0;
    Boolean libOpened = false;
    Err err = errNone;

    err = SysLibFind("Net.lib", refNum);    
    if (err)
    	return false;
    	
    NetLibOpenCount(*refNum, &pre_count);
    
    *error = NetLibOpen(*refNum, &ifErrs);
    
    if (*error != errNone && *error != netErrAlreadyOpen)
        goto Error;

    if (ifErrs)
    {
        *error = netErrInternal;
        goto Error;
    }
    
    if (*error == errNone) libOpened = true;
    goto Done;

Error:
    NetLibOpenCount(*refNum, &post_count);
    
    if (pre_count < post_count)
        NetLibClose(*refNum, true);
        
Done:
    return libOpened;
}

Err CloseNetLib(UInt16 refNum)
{
   return NetLibClose(refNum, false);
}

Err OpenSocket(UInt16 refNum, NetSocketRef *socket, Boolean use_blocking)
{
    Err error = errNone;
    Int16 result = 0;
    
	*socket = NetLibSocketOpen(refNum, netSocketAddrINET,
                netSocketTypeStream, netSocketProtoIPTCP,
                -1, &error);
                
    if (error == errNone)
    {
        NetSocketLingerType linger;
        Boolean non_blocking = true;

		linger.onOff = true;
		linger.time = 0;

		result = NetLibSocketOptionSet(refNum, *socket,
    		netSocketOptLevelSocket, netSocketOptSockLinger,
			&linger, sizeof(NetSocketLingerType), -1, &error);

        if (!use_blocking)
        {
    		result = NetLibSocketOptionSet(refNum, *socket,
                netSocketOptLevelSocket, netSocketOptSockNonBlocking,
    			(void *)&non_blocking, sizeof(Boolean), -1, &error);
        }
        
        result = NetLibSocketOptionSet(refNum, *socket,
			netSocketOptLevelTCP, netSocketOptTCPNoDelay,
			(void *)&non_blocking, sizeof(Boolean), -1, &error);
    }
    
    return error;
}

Err ConnectSocket(UInt16 refNum, NetSocketRef socket, Char *url)
{
    Err error = errNone;
    NetSocketAddrINType s_addr;
    NetHostInfoBufType *pHostInfo_buf;
    NetHostInfoType *pHostInfo = NULL;
    NetIPAddr ip = 0;
    Int16 result = 0;
    
    pHostInfo_buf = MemPtrNew(sizeof(NetHostInfoBufType));

    pHostInfo = NetLibGetHostByName(refNum, url, pHostInfo_buf, -1, &error);

    if (!pHostInfo)
    {
        MemPtrFree(pHostInfo_buf);
        return -1;
    }

    ip = *((NetIPAddr *)pHostInfo->addrListP[0]);

    MemPtrFree(pHostInfo_buf);
    
    MemSet(&s_addr, sizeof(NetSocketAddrINType), 0);
    s_addr.family = netSocketAddrINET;
    s_addr.port = NetHToNS(80);
    s_addr.addr = NetNToHL(ip);

	result = NetLibSocketConnect(refNum, socket, (NetSocketAddrType *)&s_addr,
		sizeof(s_addr), -1, &error);

    if (!result || !error)
    {
        error = error;
    }
    		
	if (error == netErrSocketAlreadyConnected)
	    error = errNone;
    
    return error;
}

Err CloseSocket(UInt16 refNum, NetSocketRef socket)
{
    Err error = errNone;
    
    NetLibSocketShutdown(refNum, socket,
			netSocketDirBoth, -1, &error) ;

    NetLibSocketClose(refNum, socket, -1, &error);

    return error;
}

Boolean RecvData(UInt16 refNum, NetSocketRef socket, void **buf, UInt16 *offset, UInt16 *size, Err *error)
{
    Boolean done = false;
    Int16 bytesRecvd = -1;
    
    if (*buf == NULL)
    {
        *buf = (Char *) MemPtrNew(32000);
        MemSet(*buf, 32000, 0);

        *size = 32000;
        *offset = 0;
    }

    if (*buf == NULL) ErrFatalDisplay("Error allocating receive buffer");
    
    bytesRecvd = NetLibReceive(refNum, socket, (void*)(((Char *)(*buf)) + *offset), *size, 0, NULL, NULL, -1, error);
    
    if (bytesRecvd > 0)
    {
        *offset = *offset + bytesRecvd;
        *size = *size - bytesRecvd;
    }
    else if (bytesRecvd == 0)
        done = true;

    return done;
}

Boolean SendData(UInt16 refNum, NetSocketRef socket, void *buf, UInt16 *size, Err *error)
{
    Boolean done = false;
    Int16 bytesSent = -1;
    
    bytesSent = NetLibSend(refNum, socket, buf, *size, 0, NULL, NULL, -1, error);
    
    if (bytesSent == *size)
        done = true;

    if (bytesSent)
    {
        buf = (Char *)buf + bytesSent;
        *size = *size - bytesSent;
    }
    
    return done;
}