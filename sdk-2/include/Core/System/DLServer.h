/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DLServer.h
 *
 * Description:
 *		Desktop Link Protocol(DLP) Server implementation definitions.
 *
 * History:
 *   	vmk	7/12/95	Created by Vitaly Marty Kruglikov
 *   	vmk	7/12/96	Converted to HTAL architecture
 *
 *******************************************************************/


#ifndef __DL_SERVER_H__
#define __DL_SERVER_H__

// Pilot common definitions
#include <Common.h>
#include <DataMgr.h>			// for DmOpenRef



/************************************************************
 * DLK result codes
 * (dlkErrorClass is defined in SystemMgr.h)
 *************************************************************/
#pragma mark *Error Codes*

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
#pragma mark *User Info Preference*

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

#define dlkUserInfoPrefVersion	0x0102	// current user info pref version: 1.2

typedef struct DlkUserInfoHdrType {
	Word					version;			// pref version number
	DWord					userID;			// user id
	DWord					viewerID;		// id assigned to viewer by the desktop
	DWord					lastSyncPC;		// last sync PC id
	ULong					succSyncDate;	// last successful sync date
	ULong					lastSyncDate;	// last sync date
	DlkSyncStateType	lastSyncState;	// last sync status
	UInt					lanSyncEnabled;// if non-zero, LAN Sync is enabled
	DWord					hsTcpPortNum;	// TCP/IP port number of Desktop HotSync
	DWord					dwReserved1;	// RESERVED -- set to NULL!
	DWord					dwReserved2;	// RESERVED -- set to NULL!
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
 * Desktop Link system preferences resource for the Conduit Filter Table
 * id = sysResIDDlkCondFilterTab, defined in SystemMgr.rh
 ********************************************************************/
#pragma mark *Conduit Filter Preference*

//
// Table for specifying conduits to "filter out" during HotSync
//

// This table consists of DlkCondFilterTableHdrType header followed by a
// variable number of DlkCondFilterEntryType entries

typedef struct DlkCondFilterTableHdrType {
	Word			entryCount;
	} DlkCondFilterTableHdrType;
typedef DlkCondFilterTableHdrType*	DlkCondFilterTableHdrPtr;
	
typedef struct DlkCondFilterEntryType {
	DWord			creator;
	DWord			type;
	} DlkCondFilterEntryType;
typedef DlkCondFilterEntryType*	DlkCondFilterEntryPtr;

typedef struct DlkCondFilterTableType {
	DlkCondFilterTableHdrType
							hdr;				// table header
	DlkCondFilterEntryType
							entry[1];		// variable number of entries
	} DlkCondFilterTableType;
typedef DlkCondFilterTableType*	DlkCondFilterTablePtr;



/********************************************************************
 * DLK Session Structures
 ********************************************************************/
#pragma mark *Session Structures*


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
typedef Int (*DlkEventProcPtr)(DWord eventRef, DlkEventType dlkEvent,
		VoidPtr paramP);

// Parameter structure for dlkEventDatabaseOpened
// Added new fields for Pilot v2.0		vmk	12/24/96
typedef struct DlkEventDatabaseOpenedType {
	DmOpenRef	dbR;					// open database ref (v2.0)
	CharPtr		dbNameP;				// database name
	ULong			dbType;				// databse type (v2.0)
	ULong			dbCreator;			// database creator
	} DlkEventDatabaseOpenedType;
	

// Prototype for the "user cancel" check callback function
typedef Int (*DlkUserCanProcPtr)(DWord canRef);


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
 	UInt					htalLibRefNum;		// HTAL library reference number - the library has a live connection
 	ULong					maxHtalXferSize;	// Maximum transfer block size

 	// Information supplied by user
 	DlkEventProcPtr	eventProcP;		// ptr to DesktopLink event notification proc
 	DWord					eventRef;			// user reference value for event proc
	DlkUserCanProcPtr	canProcP;		// ptr to user-cancel function
	DWord					canRef;			// parameter for canProcP()
	Handle				condFilterH;	// handle of conduit filter table(DlkCondFilterTableHdrPtr) or 0 for none

	// Current database information
 	Byte					dlkDBID;			// Desktop Link database id
 	DmOpenRef			dbR;				// TouchDown database access pointer -- if null, no current db
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
  	ULong					curRecIndex;	// current record index for enumeration functions
  												// (0=beginning)
  	
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
 	} DlkServerSessionType;

typedef DlkServerSessionType*	DlkServerSessionPtr;


/********************************************************************
 * DLK Function Parameter Structures
 ********************************************************************/
#pragma mark *Function Parameter Structures*

//
// Parameter passed to DlkControl()
//
typedef enum DlkCtlEnum {
	dlkCtlFirst = 0,				// reserve 0
	
	//
	// Pilot v2.0 control codes:
	//
	dlkCtlGetPCHostName,			// param1P = ptr to text buffer; (can be null if *(UIntPtr)param2P is 0)
										// param2P = ptr to buffer size(UInt);
										// returns actual length, including null, in *(UIntPtr)param2P which may be bigger than # of bytes copied.
									
	dlkCtlSetPCHostName,			// param1P = ptr to host name(zero-terminated) or NULL if *param2 is 0
										// param2P = ptr to length(UInt), including NULL (if length is 0, the current name is deleted)
	
	dlkCtlGetCondFilterTable,	// param1P =	ptr to destination buffer for filter table, or NULL if *param2 is 0
										// param2P =	on entry, ptr to size of buffer(UInt) (the size may be 0)
										// 				on return, size, in bytes, of the actual filter table
	
	dlkCtlSetCondFilterTable,	// param1P =	ptr to to conduit filter table, or NULL if *param2 is 0
										// param2P =	ptr to size of filter table(UInt) (if size is 0, the current table will be deleted)
	
	dlkCtlGetLANSync,				// param1P =	ptr to store for the LANSync setting(UInt): 0 = off, otherwise on
										// param2P =	not used, set to NULL
	
	dlkCtlSetLANSync,				// param1P =	ptr to the LANSync setting(UInt): 0 = off, otherwise on
										// param2P =	not used, set to NULL
	
	dlkCtlGetHSTCPPort,			// param1P =	ptr to store for the Desktop HotSync TCP/IP port number(DWord) -- zero if not set
										// param2P =	not used, set to NULL
	
	dlkCtlSetHSTCPPort,			// param1P =	ptr to the Desktop HotSync TCP/IP port number(DWord)
										// param2P =	not used, set to NULL
	
	dlkCtlSendCallAppReply,		// param1P =	ptr to DlkCallAppReplyParamType structure
										// param2P =	not used, set to NULL
										//
										// RETURNS: send error code; use this error code
										// as return value from the action code handler


	dlkCtlGetPCHostAddr,			// param1P = ptr to text buffer; (can be null if *(UIntPtr)param2P is 0)
										// param2P = ptr to buffer size(UInt);
										// returns actual length, including null, in *(UIntPtr)param2P which may be bigger than # of bytes copied.
									
	dlkCtlSetPCHostAddr,			// param1P = ptr to host address string(zero-terminated) or NULL if *param2 is 0
										// param2P = ptr to length(UInt), including NULL (if length is 0, the current name is deleted)


	dlkCtlGetPCHostMask,			// param1P = ptr to text buffer; (can be null if *(UIntPtr)param2P is 0)
										// param2P = ptr to buffer size(UInt);
										// returns actual length, including null, in *(UIntPtr)param2P which may be bigger than # of bytes copied.
									
	dlkCtlSetPCHostMask,			// param1P = ptr to subnet mask string(zero-terminated) or NULL if *param2 is 0
										// param2P = ptr to length(UInt), including NULL (if length is 0, the current name is deleted)
	
	
	dlkCtlLAST						// *KEEP THIS ENTRY LAST*
	
} DlkCtlEnum;


//
// Parameter passed to DlkStartServer()
//
typedef struct DlkServerParamType {
 	UInt					htalLibRefNum;	// HTAL library reference number - the library has a live connection
 	DlkEventProcPtr	eventProcP;		// ptr to DesktopLink event notification proc
 	DWord					eventRef;		// user reference value for event proc
	DWord					reserved1;		// reserved - set to NULL
	DWord					reserved2;		// reserved - set to NULL
	Handle				condFilterH;	// handle of conduit filter table(DlkCondFilterTableHdrPtr) or 0 for none
 	} DlkServerParamType;
 
typedef DlkServerParamType*		DlkServerParamPtr;



//
// Parameter passed with DlkControl()'s dlkCtlSendCallAppReply code
//
typedef struct DlkCallAppReplyParamType {
	Word					pbSize;			// size of this parameter block (set to sizeof(DlkCallAppReplyParamType))
	DWord					dwResultCode;	// result code to be returned to remote caller
	VoidPtr				resultP;			// ptr to result data
	DWord					dwResultSize;	// size of reply data in number of bytes
	VoidPtr				dlRefP;			// DesktopLink reference pointer from
												// SysAppLaunchCmdHandleSyncCallAppType
	DWord					dwReserved1;	// RESERVED -- set to null!!!
	} DlkCallAppReplyParamType;


/********************************************************************
 * DesktopLink Server Routines
 ********************************************************************/
#pragma mark *Function Prototypes*

#ifdef __cplusplus
extern "C" {
#endif

//
// SERVER API
//

// * RETURNED:	0 if session ended successfully; otherwise: dlkErrParam,
// *				dlkErrNoSession, dlkErrLostConnection, dlkErrMemory,
//	*				dlkErrUserCan
extern Err	DlkStartServer(DlkServerParamPtr paramP)
							SYS_TRAP(sysTrapDlkStartServer);

extern Err	DlkGetSyncInfo(ULongPtr succSyncDateP, ULongPtr lastSyncDateP,
			DlkSyncStateType* syncStateP, CharPtr nameBufP,
			CharPtr logBufP, ULongPtr logLenP)
							SYS_TRAP(sysTrapDlkGetSyncInfo);

// DOLATER... this is a temporary function for debugging modem manager.
// remove it when done.
extern void	DlkSetLogEntry(CharPtr textP, Int textLen, Boolean append)
							SYS_TRAP(sysTrapDlkSetLogEntry);

// Dispatch a DesktopLink request (exposed for patching)
extern Err DlkDispatchRequest(DlkServerSessionPtr sessP)
							SYS_TRAP(sysTrapDlkDispatchRequest);

extern Err DlkControl(DlkCtlEnum op, VoidPtr param1P, VoidPtr param2P)
				SYS_TRAP(sysTrapDlkControl);

#ifdef __cplusplus 
}
#endif


/********************************************************************
 * DLK Macros
 ********************************************************************/



#endif	// __DL_SERVER_H__
