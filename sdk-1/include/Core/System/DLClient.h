/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DLClient.h
 *
 * Description:
 *		Desktop Link Protocol(DLP) client implementation definitions.
 *
 * History:
 *   	7/12/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/

#define INCLUDE_VIEWER_ID

#ifndef __DL_CLIENT_H__
#define __DL_CLIENT_H__


#if EMULATION_LEVEL == 1		// EMULATION_WINDOWS

// Pilot common definitions
#include "Common.h"

// Desktop Link common definitions
#include	"DLCommon.h"

#else	// EMULATION_LEVEL != EMULATION_WINDOWS

// Pilot common definitions
#include <Common.h>

// Desktop Link common definitions
#include	<DLCommon.h>

#endif	// EMULATION_LEVEL == EMULATION_WINDOWS

#if EMULATION_WINDOWS != 1
	LOGIC ERROR
#endif




/************************************************************
 * DLK result codes
 * (dlkErrorClass is defined in SystemMgr.h)
 *************************************************************/

#define dlkErrParam			(dlkErrorClass | 1)	// invalid parameter
#define dlkErrSessClosed	(dlkErrorClass | 2)	// session closed unexpectedly
#define dlkErrMemory			(dlkErrorClass | 3)	// memory allocation error
#define dlkErrLocalCan		(dlkErrorClass | 4)	// cancelled by local user
#define dlkErrNoSession		(dlkErrorClass | 5)	// could not establish a session	
#define dlkErrSizeErr		(dlkErrorClass | 6)	// reply length is too big
#define dlkErrTooManyClients	(dlkErrorClass | 7)	// reply length is too big
#define dlkErrBufTooSmall	(dlkErrorClass | 8)	// reply buffer is too small
#define dlkErrCreate			(dlkErrorClass | 9)	// error creating database
#define dlkErrNotEnoughSpace	(dlkErrorClass | 10)	// not enough space on remote
#define dlkErrAlreadyExists	(dlkErrorClass | 11)	// cannot create a duplicate
#define dlkErrNotFound		(dlkErrorClass | 12)	// database/record/resource/block not found
#define dlkErrDelete			(dlkErrorClass | 13)	// delete error
#define dlkErrClose			(dlkErrorClass | 14)	// close error
#define dlkErrAdd				(dlkErrorClass | 15)	// add record/resource error
#define dlkErrRead			(dlkErrorClass | 16)	// read record/resource/block error
#define dlkErrUserInfo		(dlkErrorClass | 17)	// error reading/writing user info
#define dlkErrReplyFormat	(dlkErrorClass | 18)	// reply format error
#define dlkErrDeleted		(dlkErrorClass | 19)	// record was deleted
#define dlkErrOpen			(dlkErrorClass | 20)	// record was deleted



/********************************************************************
 * DLK Session Structures
 ********************************************************************/
 	
//
// DLP Client session information
//

typedef Int (*DlkAbortProcPtr)(DWord userRef);


typedef struct DlkClientSessionType {
 	// Information supplied by user
 	Byte					remoteSocket;	// server socket
 	Byte					localSocket;	// workstation socket
 	DlkAbortProcPtr	abortProcP;		// ptr to abort proc
 	DWord					userRef;			// user reference value for abort proc
 
	// Session status information
	Int					numClients;		// Number of clients
	Byte					dlpRefNum;		// DLP session reference number

 	// Reply buffer
 	WordPtr				replyBufP;
 	ULong					replyBufSize;
 	} DlkClientSessionType;
 
typedef DlkClientSessionType*	DlkClientSessionPtr;


//
// Parameter passed to DlkInitClient()
//
typedef struct DlkClientParamType {
 	Byte					remoteSocket;	// server socket for SP
 	Byte					localSocket;	// workstation socket
 	DlkAbortProcPtr	abortProcP;
 	DWord					userRef;
 	} DlkClientParamType;
 
typedef DlkClientParamType*		DlkClientParamPtr;


/********************************************************************
 * DLK Routines
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//
// CLIENT API
//

// * RETURNED:	0 if session ended successfully; otherwise: dlkErrParam,
// *				dlkErrNoSession, dlkErrMemory, dlkErrTooManyClients
Err DlkInitClient(DlkClientParamPtr paramP, BytePtr refNumP);

Err DlkClientDone(Byte refNum);

Err DlkSendTickle(Byte refNum);

Err DlkOpenDatabase(Byte refNum, Byte cardNo, CharPtr nameP, Byte mode,
		BytePtr dlkDBIDP);

Err DlkCreateDatabase(Byte refNum, Byte cardNo, CharPtr name, Word dbFlags,
		DWord creator, DWord type, BytePtr dbIDP);

Err DlkDeleteDatabase(Byte refNum, UInt cardNo, CharPtr nameP);

Err DlkCloseDatabase(Byte refNum, Byte dbID);

Err DlkReadOpenDBInfo(Byte refNum, Byte dbID, WordPtr numRecP);

Err DlkReadAppBlock(Byte refNum, Byte dbID, UInt offset, UInt numBytes,
		VoidPtr bufP, UInt bufSize, UIntPtr rcvdLenP, UIntPtr blkSizeP);

Err DlkAddRecord(Byte refNum, Byte dbID, VoidPtr recP,
		Word recSize, DWordPtr recIDP);

Err DlkReadRecord(Byte refNum, Byte dbID, UInt index, UInt offset,
		UInt numBytes, VoidPtr bufP, UInt bufSize, DWordPtr recIDP,
		BytePtr attrP, BytePtr categoryP, UIntPtr rcvdLenP, UIntPtr recSizeP);

Err DlkDeleteAllRecords(Byte RefNum, Byte dbID);

Err DlkWriteUserInfo(Byte refNum, CharPtr userNameP, DWord userID,
		DWord lastSyncPC);

Err DlkReadUserInfo(Byte refNum, CharPtr userNameBufP, DWordPtr userIDP,
		DWordPtr lastSyncPCP);

Err DlkNotifyOpenConduit(Byte refNum, BooleanPtr cancelP);

Err DlkNotifyEndOfSync(Byte refNum, DlpSyncTermCode termCode);

Err DlkResetRemote(Byte refNum);

Err DlkGetDBNameViaCallAppTest(Byte refNum, Word cardNo, Word index, CharPtr bufP, Word bufSize);


#ifdef __cplusplus 
}
#endif


/********************************************************************
 * DLK Macros
 ********************************************************************/



#endif	// __DL_CLIENT_H__
