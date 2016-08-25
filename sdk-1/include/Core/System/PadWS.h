/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		PadWS.h
 *
 * Description:
 *		Packet Assembly/Disassembly workstation-side definitions
 *
 * History:
 *   	8/11/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __PAD_WS_H__
#define __PAD_WS_H__



#if EMULATION_LEVEL == 1		// EMULATION_WINDOWS
	
	// Pilot common definitions
	#include "Common.h"
	#include "SerLink.h"

	// PAD Protocol definitions
	#include "PadCommn.h"
	#include "PadUtil.h"
	
#else	// EMULATION_LEVEL != EMULATION_WINDOWS
	
	// Pilot common definitions
	#include <Common.h>
	#include <SerialLinkMgr.h>
	
	// PAD Protocol definitions
	#include <PadCommn.h>
	#include <PadUtil.h>
	
#endif	// EMULATION_LEVEL == EMULATION_WINDOWS

#if EMULATION_WINDOWS != 1
	LOGIC ERROR
#endif


/********************************************************************
 * Session Manager Parameters
 ********************************************************************/
#define pmMaxPktBufSize		( sizeof(PadHeaderType) +	\
										padMaxPktDataSize )

// pmReplyWaitIntervalSec: when waiting for the beginning of a reply,
// the PAD receiver will wait up to this many seconds before before timing
// out on the first packet.  The higher-level routine will perform
// its regular maintenance tasks (such as checking for user cancel) around
// calls to the PAD receiver.
#define pmReplyWaitIntervalSec		2


/************************************************************
 * Session Protocol result codes
 * (padErrorClass is defined in SystemMgr.h)
 *************************************************************/

#define pmErrParam			(padErrorClass | 1)		// invalid parameter
#define pmErrTooManyRetries	(padErrorClass | 2)	// too many retries
#define pmErrTimedOut		(padErrorClass | 4)		// call timed out
#define pmErrInUse			(padErrorClass | 5)		// PAD Server/WS manager is in use
#define pmErrMemory			(padErrorClass | 6)		// not enough memory
#define pmErrUserCan			(padErrorClass | 7)		// cancelled by user
#define pmErrSizeErr			(padErrorClass | 8)		// reply block is too big
#define pmErrBlockFormat	(padErrorClass | 9)		// block format error
#define pmErrBufTooSmall	(padErrorClass | 10)		// buffer is too small
#define pmErrRemoteMemory	(padErrorClass | 11)		// remote memory error




/********************************************************************
 * Session Manager Structures
 ********************************************************************/


//
// Packet Assembly/Disassembly structures
//

// PAD sender-specific parameters
typedef struct PmSendParamType {
	// Send segments specification:
	Int				segCount;		// number of segments to send
	PmSegmentPtr	segP;				// ptr to array of segments
	
	// Statistical information for fine-tuning and debugging:
	ULong				numAttempted;	// number of data packets we attempted to send
	ULong				numSucceeded;	// number of packets successfully sent
	ULong				retries;			// number of retries
	} PmSendParamType;


// PAD receiver-specific parameters
typedef struct PmRcvParamType {
	VoidPtr			rcvBufP;			// ptr to receive buffer
	ULong				rcvBufSize;		// receive buffer size

	// Set by PrvReceiveReplyBlock:
	ULong				blockSize;		// size of block
	ULong				rcvdLen;			// amount of data received so far
	} PmRcvParamType;


// PAD parameter for PrvSendCommandBlock and PrvReceiveReplyBlock:
typedef struct PmPadParamType {
	Byte				localSocket;	// local socket
	Byte				remoteSocket;	// remote socket
	SlkPktHeaderPtr	slkHdrBufP;	// ptr to buffer for serial link header
	PadHeaderPtr	pktBufP;			// ptr to packet buffer of pmMaxPktBufSize bytes
	Int				initTimeoutSec;// initial receiver timeout seconds
	Byte				transID;			// current command/reply transaction ID

	PmSendParamType	send;			// PAD sender-specific params
	PmRcvParamType		rcv;			// PAD receiver-specific params
	} PmPadParamType;

typedef PmPadParamType*	PmPadParamPtr;



//
// PAD Manager Workstation-end globals
//

// Prototype for the "user cancel" check callback function
typedef Int (*PmUserCanProcPtr)(DWord userRef);

typedef struct PmWSGlobalsType {
	Int						replyWaitSec;			// maximum number of seconds to
															// wait for a command reply

	PmUserCanProcPtr		canProcP;				// ptr to user-cancel function
	DWord						userRef;					// parameter for cancelProcP()

	SlkPktHeaderType		slkHdrBuf;				// serial link header buffer
															// packet data buffer
	Word						pktBuf[(pmMaxPktBufSize+1)/2];

	// Transaction id of last request which we originated
	Byte						lastTID;					// last sent transaction id
	
	PmPadParamType			pad;						// PAD routines parameter
	} PmWSGlobalsType;

typedef PmWSGlobalsType*		PmWSGlobalsPtr;



/********************************************************************
 * Session Manager Routines
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


//
// Workstation API
//

// RETURNED:	0 on success; otherwise: pmErrParam, pmErrInUse,
//					pmErrMemory
Err PmInitWS(Byte wsSocket, Byte serverSocket, PmUserCanProcPtr canProcP,
		DWord userRef, Int replyWaitSec);

// MUST be called when done with the session to clean up workstation info
// if PmInitWS() was successful
//
// * RETURNED:	0 on success
Err PmCloseWS(void);

//	* RETURNED:	0 on success;
Err PmSendTickle(void);

//	* RETURNED:	0 on success; otherwise: pmErrParam, pmErrTimedOut,
// *				pmErrBlockFormat, pmErrUserCan, pmErrSizeErr, pmErrBufTooSmall
Err PmSendCommand(PmSegmentPtr cmdSegP, Int cmdSegCount, Int replyWaitSec,
		VoidPtr replyBufP, ULong replyBufSize, WordPtr replyLenP,
		WordPtr actRcvdLenP);

//	* RETURNED:	0 on success; otherwise: pmErrParam, pmErrTimedOut,
// *				pmErrBlockFormat, pmErrUserCan, pmErrSizeErr, pmErrBufTooSmall
Err PmReceiveReply( Int replyWaitSec, Byte refTID, VoidPtr replyBufP,
		ULong replyBufSize, WordPtr replyLenP, WordPtr actRcvdLenP);

#ifdef __cplusplus 
}
#endif


/********************************************************************
 * Session Protocol Macros
 ********************************************************************/



#endif	// __PAD_WS_H__
