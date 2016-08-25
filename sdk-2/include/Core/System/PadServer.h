/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		PadServer.h
 *
 * Description:
 *		Packet Assembly/Disassembly server-side definitions
 *
 * History:
 *   	7/17/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __PAD_SERVER_H__
#define __PAD_SERVER_H__

// Pilot common definitions
#include <Common.h>
#include <SerialLinkMgr.h>

// Data Manager definitions
#include <DataMgr.h>			// for DmOpenRef

// Common Session Protocol definitions
#include <PadCommn.h>

#include <PadUtil.h>


/********************************************************************
 * PAD Server Parameters
 ********************************************************************/
#define psrMaxPktBufSize		( sizeof(PadHeaderType) +	\
										padMaxPktDataSize )

// psrMinSerRcvBufSize: minimum serial receive buffer size needed
// for PAD Server.  The extra 32 bytes are for beginning/end of queue
// and other serial manager overhead
#define psrMinSerRcvBufSize	( sizeof(SlkPktHeaderType) +	\
										psrMaxPktBufSize +				\
										sizeof(SlkPktFooterType) +		\
										32 )

// psrCmdWaitIntervalSec: when waiting for the beginning of a command,
// the PAD receiver will wait up to this many seconds before before timing
// out on the first packet.  The higher-level routine will perform
// its regular maintenance tasks (such as checking for user cancel) around
// calls to the PAD receiver.
#define psrCmdWaitIntervalSec		2


/************************************************************
 * Session Protocol result codes
 * (padErrorClass is defined in SystemMgr.h)
 *************************************************************/

#define psrErrParam			(padErrorClass | 1)		// invalid parameter
#define psrErrTooManyRetries	(padErrorClass | 2)	// too many retries
#define psrErrTimeOut		(padErrorClass | 4)		// call timed out
#define psrErrInUse			(padErrorClass | 5)		// PAD Server/WS manager is in use
#define psrErrMemory			(padErrorClass | 6)		// not enough memory
#define psrErrUserCan		(padErrorClass | 7)		// cancelled by user
#define psrErrSizeErr			(padErrorClass | 8)		// reply block is too big
#define psrErrBlockFormat	(padErrorClass | 9)		// block format error




/********************************************************************
 * Session Manager Structures
 ********************************************************************/


//
// Packet Assembly/Disassembly structures
//


// PAD sender-specific parameters
typedef struct PsrSendParamType {
	Byte				remoteSocket;	// remote socket id
	Byte				transID;			// transaction ID to use on block packets
	
	// Send segments specification:
	Int				segCount;		// number of segments to send
	PmSegmentPtr	segP;				// ptr to array of segments
	
	// Statistical information for fine-tuning and debugging:
	ULong				numAttempted;	// number of data packets we attempted to send
	ULong				numAcked;		// number of packets which were acked
	ULong				retries;			// number of retries
	} PsrSendParamType;


// PAD receiver-specific parameters
typedef struct PsrRcvParamType {
	Byte				filterTransID;	// tranasaction id to filter out
	DmOpenRef		dbP;				// database reference for allocating memory
	
	// Set by PrvReceiveCmdBlock:
	VoidPtr			cmdP;				// ptr to command in dynamic heap chunk
	VoidHand			cmdBufH;			// handle of command buffer chunk in data heap;
	ULong				blockSize;		// size of block
	ULong				rcvdLen;			// amount of data received so far
	Byte				transID;			// received transaction id
	Byte				remoteSocket;	// source socket
	Boolean			gotTickle;		// Set to true if Tickle had been received
	} PsrRcvParamType;


// PAD parameter for PrvSendReplyBlock and PrvReceiveCmdBlock:
typedef struct PsrPadParamType {
	Byte				localSocket;	// local socket
	SlkPktHeaderPtr	slkHdrBufP;	// ptr to buffer for serial link header
	PadHeaderPtr	pktBufP;			// ptr to packet buffer of psrMaxPktBufSize bytes
	Int				initTimeoutSec;// initial receiver timeout seconds

	PsrSendParamType	send;			// PAD sender-specific params
	PsrRcvParamType	rcv;			// PAD receiver-specific params
	} PsrPadParamType;

typedef PsrPadParamType*	PsrPadParamPtr;


//
// PAD Server globals
//

// Prototype for the "user cancel" check callback function
typedef Int (*PsrUserCanProcPtr)(DWord userRef);

// Server globals
typedef struct PsrGlobalsType {
	//Boolean					inUse;				// true when session is in use
	//Boolean					open;					// true when session is open
	Byte						serverSocket;			// session listenning socket
	Int						cmdWaitSec;				// max seconds to wait for commands
															// (-1 = forever)
	PsrUserCanProcPtr		canProcP;				// ptr to user-cancel function
	DWord						userRef;					// parameter for cancelProcP()
	SlkPktHeaderType		slkHdrBuf;				// Serial Link header buffer
															// packet data buffer
	Word						pktBuf[(psrMaxPktBufSize+1)/2];
	Byte						lastRcvdCmdTID;		// transaction id of last command received
															// (for duplicate filtration)
	
	PsrPadParamType		pad;						// PAD routines parameter
	} PsrGlobalsType;

typedef PsrGlobalsType*		PsrGlobalsPtr;


/********************************************************************
 * Session Manager Routines
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


//
// Server API
//

// RETURNED:	0 on success; otherwise: psrErrParam, psrErrInUse,
//					psrErrMemory
Err PsrInit(Byte serverSocket, PsrUserCanProcPtr canProcP, DWord userRef,
		Int cmdWaitSec)
							SYS_TRAP(sysTrapPsrInit);

// MUST be called when done with the session to clean up server info
// if PsrInit() was successful
//
// * RETURNED:	0 on success; otherwise: psrErrParam
Err PsrClose(void)	SYS_TRAP(sysTrapPsrClose);

// * RETURNED:	0 on success; otherwise: psrErrParam, psrErrTimeOut,
// *				psrErrBlockFormat, psrErrUserCan
Err PsrGetCommand(DmOpenRef refDBP, VoidPtr* cmdPP, VoidHand* cmdBufHP,
		WordPtr rcvdCmdLenP, BytePtr tidP, BytePtr remoteSocketP)
							SYS_TRAP(sysTrapPsrGetCommand);

// * RETURNED:	0 on success; otherwise: psrErrParam, psrErrTooManyRetries,
// *				psrErrSizeErr, psrErrUserCan
Err PsrSendReply(Byte remoteSocket, Byte refTID, PmSegmentPtr segP, Int segCount)
							SYS_TRAP(sysTrapPsrSendReply);


#ifdef __cplusplus 
}
#endif


/********************************************************************
 * Session Protocol Macros
 ********************************************************************/



#endif	// __PAD_SERVER_H__
