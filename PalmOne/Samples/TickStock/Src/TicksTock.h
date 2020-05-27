/*
 * TicksTock.h
 *
 * header file for Tick sTock
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef TICKSTOCK_H_
#define TICKSTOCK_H_

#include <HsPhone.h>

/*********************************************************************
 * Internal Structures
 *********************************************************************/

typedef struct TicksTockPreferenceType
{
    Boolean auto_update;
	Boolean blocking;
	Int32 timeout;
} TicksTockPreferenceType;

typedef enum _HTTPStateEnum
{
    HTTPStateNone = 0,
    HTTPStateError,
    HTTPStateSuspended,
    HTTPStateInit,
    HTTPStateConnect,
    HTTPStateSendRequest,
    HTTPStateRecvHeader,
    HTTPStateRecvContent,
    HTTPStateClose,
    HTTPStateDone

} HTTPStateEnum;

typedef UInt8 HTTPStateType;

typedef enum _CompanyEnum
{
    Palm,
    Yahoo,
    Google,
    Microsoft,
    Intel,
    CompanyCount
    
} CompanyEnum;

typedef UInt8 CompanyType;

typedef struct StockDataType
{
    Char price[16];
    Char time[32];
} StockDataType;

typedef struct NetLibInfoType
{
    UInt16 refNum;
    Boolean loaded;
    NetSocketRef socket;
} NetLibInfoType;

typedef struct ReceiveBufferType
{
    Char *ptr;
    UInt16 size;
    UInt16 offset;
} ReceiveBufferType;

typedef struct MyAppGlobalType
{
    Err error;
    TicksTockPreferenceType prefs;
    CompanyType company;
    NetLibInfoType netLib;
    HTTPStateType HTTPState;
    UInt16 stateCount;
    StockDataType data[CompanyCount];
    ReceiveBufferType recvBuf;

} MyAppGlobalType;

/*********************************************************************
 * Global variables
 *********************************************************************/

extern TicksTockPreferenceType g_prefs;

/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'ttOc'
#define appName					"TicksTock"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

#define QUOTE_SERVER            "finance.yahoo.com"  
#define QUOTE_CMD               "GET /q?s=%s&d=t HTTP/1.0\r\n\n"
#define QUOTE_PORT              80
#define QUOTE_RESPTERM          "</html>"
#define QUOTE_RESPSIZE          64000
#define QUOTE_FTR_GLOBAL		0
#define SYMBOL_SIZE				7


/*********************************************************************
 * Internal Functions
 *********************************************************************/

Boolean InitNetLib(UInt16 *refNum, Err *error);
Err CloseNetLib(UInt16 refNum);
Err OpenSocket(UInt16 refNum, NetSocketRef *socket, Boolean use_non_blocking);
Err ConnectSocket(UInt16 refNum, NetSocketRef socket, Char *url);
Err CloseSocket(UInt16 refNum, NetSocketRef socket);
Boolean RecvData(UInt16 refNum, NetSocketRef socket, void **buf, UInt16 *offset, UInt16 *size, Err *error);
Boolean SendData(UInt16 refNum, NetSocketRef socket, void *buf, UInt16 *size, Err *error);

Boolean HTTPHandleEvent();
void PhoneHandleEvent(PhnEventType *pEvent);

#endif /* TICKSTOCK_H_ */
