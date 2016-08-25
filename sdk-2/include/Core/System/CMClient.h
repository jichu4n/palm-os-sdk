/*******************************************************************
 * 							 Touchdown Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		CMClient.h
 *
 * Description:
 *		Connection Management Protocol client definitions.
 *
 * History:
 *   	9/14/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __CMCLIENT_H__
#define __CMCLIENT_H__

#if EMULATION_LEVEL == 1		// EMULATION_WINDOWS
	
	// Touchdown common definitions
	#include "Common.h"
	#include "SerLink.h"
	
#else	// EMULATION_LEVEL != EMULATION_WINDOWS
	
	// Touchdown common definitions
	#include <Common.h>
	#include <SerialLinkMgr.h>
	
#endif	// EMULATION_LEVEL == EMULATION_WINDOWS

#if EMULATION_WINDOWS != 1
	LOGIC ERROR
#endif



/************************************************************
 * Connection Manager result codes
 * (cmpErrorClass is defined in SystemMgr.h)
 *************************************************************/

#define cmErrParam			(cmpErrorClass | 1)	// bad parameter
#define cmErrTimeOut			(cmpErrorClass | 2)	// connection listenning/initialization timed out
#define cmErrComm				(cmpErrorClass | 3)	// communication(send/receive) error
#define cmErrCommVersion	(cmpErrorClass | 4)	// incompatible comm version
#define cmErrMemory			(cmpErrorClass | 5)	// insufficient memory error
#define cmErrCommBusy		(cmpErrorClass | 6)	// PAD server is already in use
#define cmErrUserCan			(cmpErrorClass | 7)	// attempt cancelled by user



/********************************************************************
 * Connection Manager Constants 
 ********************************************************************/

#define cmWakeupTransactionID		0xFF
 
/********************************************************************
 * Connection Manager Structures
 ********************************************************************/
typedef Int (*CmAbortProc)(DWord userRef);

typedef struct CmParamType {
	UInt				localSocket;	// -> user's socket
	UInt				remoteSocket;	// -> remote socket
	ULong				maxBaud;			// -> maximum baud rate which can be used
	Boolean			viaModem;		// -> set to true if connecting via modem
	CmAbortProc		abortProc;		// -> ptr to abort procedure
	DWord				userRef;			// -> reference value for abort procedure
	} CmParamType;

typedef CmParamType*			CmParamPtr;


typedef struct CmSessionType {
	// Parameters passed by user
	CmParamType		param;			// user parameters
	
	UInt				serRefNum;		// serial library reference number
	} CmSessionType;

typedef CmSessionType*	CmSessionPtr;




/********************************************************************
 * Connection Manager API Routines
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// Listens for a "wakeup" message from the broadcaster
// and completes the connection handshake by sending the "init"
// or "abort" message(called by client).
Err	CmListen(CmParamPtr paramP);


#ifdef __cplusplus 
}
#endif



/********************************************************************
 * Connection Manager Macros
 ********************************************************************/



#endif	// __CMCLIENT_H__
