/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		CMServer.h
 *
 * Description:
 *		Connection Management Protocol server definitions.
 *
 * History:
 *   	9/14/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __CMSERVER_H__
#define __CMSERVER_H__

// Pilot common definitions
#include <Common.h>

//#include <SerialLinkMgr.h>



/************************************************************
 * Connection Manager result codes
 * (cmpErrorClass is defined in SystemMgr.h)
 *************************************************************/

#define cmErrParam			(cmpErrorClass | 1)	// bad parameter
#define cmErrTimedOut		(cmpErrorClass | 2)	// connection initiation/listenning timed out
#define cmErrComm				(cmpErrorClass | 3)	// communication(send/receive) error
#define cmErrCommVersion	(cmpErrorClass | 4)	// incompatible comm version
#define cmErrMemory			(cmpErrorClass | 5)	// insufficient memory error
#define cmErrCommBusy		(cmpErrorClass | 6)	// PAD server is already in use
#define cmErrUserCan			(cmpErrorClass | 7)	// attempt cancelled by user



/********************************************************************
 * Connection Manager Constants 
 ********************************************************************/
 
/********************************************************************
 * Connection Manager Structures
 ********************************************************************/
typedef Int (*CmAbortProc)(DWord userRef);

typedef struct CmParamType {
	UInt				localSocket;	// -> user's socket
	UInt				remoteSocket;	// -> remote socket
	ULong				maxBaud;			// -> maximum baud rate which can be used
	ULong				hwShakeAbove;	// -> enable HW hand-shake above this baud
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

// Initiates connection establishment by sending "wakeup" packets
// and listens for connection handshake(called by server).
Err	CmBroadcast(CmParamPtr paramP)
							SYS_TRAP(sysTrapCmBroadcast);


#ifdef __cplusplus 
}
#endif



/********************************************************************
 * Connection Manager Macros
 ********************************************************************/



#endif	// __CMSERVER_H__
