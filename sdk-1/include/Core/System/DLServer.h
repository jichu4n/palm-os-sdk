/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DLServer.h
 *
 * Description:
 *		Desktop Link Protocol(DLP) Server implementation definitions.
 *
 * History:
 *   	7/12/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __DL_SERVER_H__
#define __DL_SERVER_H__

// Pilot common definitions
#include <Common.h>
#include <DataMgr.h>			// for DmOpenRef


// COMMENT OUT TO BUILD WITHOUT THE VIEWER ID OPTION
#define INCLUDE_VIEWER_ID

/************************************************************
 * DLK result codes
 * (dlkErrorClass is defined in SystemMgr.h)
 *************************************************************/

#define dlkErrParam			(dlkErrorClass | 1)	// invalid parameter
#define dlkErrMemory			(dlkErrorClass | 2)	// memory allocation error
#define dlkErrNoSession		(dlkErrorClass | 3)	// could not establish a session	

#define dlkErrSizeErr		(dlkErrorClass | 4)	// reply length was too big

#define dlkErrLostConnection	(dlkErrorClass | 5)	// lost connection
#define dlkErrInterrupted	(dlkErrorClass | 6)	// sync was interrupted (see sync state)
#define dlkErrUserCan		(dlkErrorClass | 7)	// cancelled by user


/********************************************************************
 * Desktop Link system preferences resource for user info
 * id = sysResIDDlkUserInfo, defined in SystemMgr.rh
 ********************************************************************/
#define dlkMaxUserNameLength			40
#define dlkUserNameBufSize				(dlkMaxUserNameLength + 1)

#define dlkMaxLogSize					(2 * 1024)

typedef enum DlkSyncStateType {
	dlkSyncStateNeverSynced = 0,		// never synced
	dlkSyncStateInProgress,				// sync is in progress
	dlkSyncStateLostConnection,		// connection lost during sync
	dlkSyncStateLocalCan,				// cancelled by local user on TD
	dlkSyncStateRemoteCan,				// cancelled by user from desktop
	dlkSyncStateLowMemoryOnTD,			// sync ended due to low memory on TD
	dlkSyncStateAborted,					// sync was aborted for some other reason
	dlkSyncStateCompleted				// sync completed normally
	} DlkSyncStateType;

typedef struct DlkUserInfoHdrType {
	DWord					userID;			// user id
#ifdef INCLUDE_VIEWER_ID
	DWord					viewerID;		// id assigned to viewer by the desktop
#endif
	DWord					lastSyncPC;		// last sync PC id
	ULong					succSyncDate;	// last successful sync date
	ULong					lastSyncDate;	// last sync date
	DlkSyncStateType	lastSyncState;	// last sync status
	Byte					userNameLen;	// length of name field(including null)
	UInt					syncLogLen;		// length of sync log(including null)
	} DlkUserInfoHdrType;

typedef struct DlkUserInfoType {
	DlkUserInfoHdrType	header;			// fixed size header
	Char						nameAndLog[2];	// user name, followed by sync log;
													// both null-terminated(for debugging)
	} DlkUserInfoType;

typedef DlkUserInfoType*		DlkUserInfoPtr;		// user info pointer



/********************************************************************
 * DLK Session Structures
 ********************************************************************/


// DesktopLink event notification callback.  If non-zero is returned,
// sync will be cancelled as soon as a safe point is reached.
typedef enum {
	dlkEventOpeningConduit = 1,			// conduit is being opened -- paramP
													// is null;
	
	dlkEventDatabaseOpened,					// client has opened a database -- paramP
													// points to DlkEventDatabaseOpenedType;

	dlkEventCleaningUp,						// last stage of sync -- cleaning up (notifying apps, etc) --
													// paramP is null
	
	dlkEventSystemResetRequested			// system reset was requested by the desktop client
													// (the normal action is to delay the reset until
													// end of sync) -- paramP is null
	} DlkEventType;

// Prototype for the event notification callback
typedef Int (*DlkEventProcPtr)(DWord userRef, DlkEventType dlkEvent,
		VoidPtr paramP);

// Parameter structure for dlkEventDatabaseOpened
typedef struct DlkEventDatabaseOpenedType {
	CharPtr		dbNameP;				// database name
	ULong			dbCreator;			// database creator
	} DlkEventDatabaseOpenedType;
	

// Prototype for the "user cancel" check callback function
typedef Int (*DlkUserCanProcPtr)(DWord userRef);


//
// List of modified database creators maintained by DLP Server
//
typedef struct DlkDBCreatorList {
	UInt					count;			// number of entries in the list
	Handle				listH;			// chunk handle of the creators list
	} DlkDBCreatorList;


//
// DLP Server session information
//
typedef struct DlkServerSessionType {
 	// Information supplied by user
 	Byte					sls;				// session listenning socket
 	DlkEventProcPtr	eventProcP;		// ptr to DesktopLink event notification proc
 	DWord					userRef;			// user reference value for event proc
	DlkUserCanProcPtr	canProcP;		// ptr to user-cancel function
	DWord					canRef;			// parameter for canProcP()

	// Current database information
 	Byte					dlkDBID;			// Desktop Link database id
 	DmOpenRef			dbP;				// TouchDown database access pointer -- if null, no current db
 	UInt					cardNo;			// memory module number
 	ULong					dbCreator;		// creator id
  	Char					dbName[dmDBNameLength];	// DB name
  	UInt					dbOpenMode;		// database open mode
 	Boolean				created;			// true if the current db was created
 	Boolean				isResDB;			// set to true if resource database
 	Boolean				ramBased;		// true if the db is in RAM storage
 	Boolean				readOnly;		// true if the db is read-only
 	LocalID				dbLocalID;		// TouchDown LocalID of the database
 	ULong					initialModNum;	// initial DB modification number
  	ULong					modRecIndex;	// modified record index for
  												// dlpReadNextModifiedRec(0=beginning)
  	
  	// List of modified database creators maintained by DLP Server
  	DlkDBCreatorList	creatorList;
 
	// Session status information
	DlkSyncStateType	syncState;		// current sync state;
	
 	Boolean				complete;		// set to true when completion request
 												// has been received
 	
 	Boolean				conduitOpened;	// set to true after the first coduit
 												// is opened by remote
 												
 	Boolean				logCleared;		// set to true after sync log has been
 												// cleared during the current session;
			// The log will be cleared before any new entries are added or at
			// the end of sync in case no new entries were added.
			// (we do not clear the log at the beginning of sync in case the
			// user cancels during the "identifying user" phase; in this
			// event, the spec calls for preserving the original log)
 												
 	Boolean				resetPending;	// set to true if system reset is pending;
 												// the reset will be carried out at end
 												// of sync
 												
	// Current request information
 	Boolean				gotCommand;		// set to true when got a request
 	Byte					cmdTID;			// current transaction ID
 	Word					cmdLen;			// size of data in request buffer
 	VoidPtr				cmdP;				// pointer to command
 	VoidHand				cmdH;				// handle of command buffer
 	Byte					remoteSocket;	// remote socket id
 	} DlkServerSessionType;

typedef DlkServerSessionType*	DlkServerSessionPtr;


//
// Parameter passed to DlkStartServer()
//
typedef struct DlkServerParamType {
 	Byte					sls;				// server listenning socket id for PAD
 	DlkEventProcPtr	eventProcP;		// ptr to DesktopLink event notification proc
 	DWord					userRef;			// user reference value for event proc
 	Int					cmdWaitSec;		// maximum number of seconds to wait for
 												// a command (0=default, -1=forever)
	DlkUserCanProcPtr	canProcP;		// ptr to user-cancel function
	DWord					canRef;			// parameter for canProcP()
 	
 	} DlkServerParamType;
 
typedef DlkServerParamType*		DlkServerParamPtr;


/********************************************************************
 * DesktopLink Server Routines
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//
// SERVER API
//

// * RETURNED:	0 if session ended successfully; otherwise: dlkErrParam,
// *				dlkErrNoSession, dlkErrLostConnection, dlkErrMemory,
//	*				dlkErrUserCan
Err	DlkStartServer(DlkServerParamPtr paramP)
							SYS_TRAP(sysTrapDlkStartServer);

Err	DlkGetSyncInfo(ULongPtr succSyncDateP, ULongPtr lastSyncDateP,
			DlkSyncStateType* syncStateP, CharPtr nameBufP,
			CharPtr logBufP, ULongPtr logLenP)
							SYS_TRAP(sysTrapDlkGetSyncInfo);

// DOLATER... this is a temporary function for debugging modem manager.
// remove it when done.
void	DlkSetLogEntry(CharPtr textP, Int textLen, Boolean append)
							SYS_TRAP(sysTrapDlkSetLogEntry);

#ifdef __cplusplus 
}
#endif


/********************************************************************
 * DLK Macros
 ********************************************************************/



#endif	// __DL_SERVER_H__
