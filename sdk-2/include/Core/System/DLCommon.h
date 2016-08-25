/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DLCommon.h
 *
 * Description:
 *		Desktop Link Protocol(DLP) function id's, parameters, and frame
 *		structures.
 *
 * History:
 *   	6/7/95	vmk	Created by Vitaly Marty Kruglikov
 *		6/15/95	vmk	Changed dlp/Read/Write/UserID to dlp/Read/Write/UserInfo and
 *							added the last sync PC id field to their arguments at Laura's request.
 *		6/15/95	vmk	Added option to dlpDeleteRecord to delete all records in DB. 
 *
 *******************************************************************/


#ifndef __DLCOMMON_H__
#define __DLCOMMON_H__


/************************************************************
 * DesktopLink function codes
 *************************************************************/

// DlpFuncID -- function id's used in request blocks sent to
// the DesktopLink application on Pilot.  The width of the function
// id base is 7 bits.  The high order bit(the 8th bit) is used to
// distinguish requests from their responses --  it is cleared in request
// blocks and is set in response blocks (i.e., the response to a particular
// command will have the same function code base as the command and the
// high order bit set).  See dlpFuncRespFlag defined below.
//
typedef enum DlpFuncID {

	dlpReservedFunc = 0x0F,			// range reserved for internal use

	// DL 1.0 FUNCTIONS START HERE
	dlpReadUserInfo,
	
	dlpWriteUserInfo,
	
	dlpReadSysInfo,
	
	dlpGetSysDateTime,
	
	dlpSetSysDateTime,
	
	dlpReadStorageInfo,
	
	dlpReadDBList,
	
	dlpOpenDB,
	
	dlpCreateDB,
	
	dlpCloseDB,
	
	dlpDeleteDB,
	
	dlpReadAppBlock,
	
	dlpWriteAppBlock,

	dlpReadSortBlock,

	dlpWriteSortBlock,

	dlpReadNextModifiedRec,

	dlpReadRecord,

	dlpWriteRecord,

	dlpDeleteRecord,

	dlpReadResource,

	dlpWriteResource,

	dlpDeleteResource,

	dlpCleanUpDatabase,

	dlpResetSyncFlags,

	dlpCallApplication,

	dlpResetSystem,
	
	dlpAddSyncLogEntry,
	
	dlpReadOpenDBInfo,
	
	dlpMoveCategory,
	
	dlpProcessRPC,					// remote procedure calls interface
	
	dlpOpenConduit,				// this command is sent before each conduit is opened
	
	dlpEndOfSync,					// ends the sync session
	
	dlpResetRecordIndex,			// resets "modified record" index
	
	dlpReadRecordIDList,			// LAST 1.0 FUNCTION
	
	
	// DL 1.1 FUNCTIONS ADDED HERE (Pilot 2.0 Personal, ROM-A)
	dlpReadNextRecInCategory,
	
	dlpReadNextModifiedRecInCategory,
	
	dlpReadAppPreference,
	
	dlpWriteAppPreference,
	
	dlpReadNetSyncInfo,
	
	dlpWriteNetSyncInfo,

	dlpReadFeature,
	
	
	
	dlpLastFunc						// ***ALWAYS KEEP LAST***

	} DlpFuncID;

#define	dlpLastPilotV10FuncID	dlpReadRecordIDList


// Desktop Link function error codes returned in the response errorCode
// field.
typedef enum DlpRespErrorCode {
	dlpRespErrNone = 0,					// reserve 0 for no error
	dlpRespErrSystem,						// general Pilot system error
	dlpRespErrIllegalReq,				// unknown function ID
	dlpRespErrMemory,						// insufficient dynamic heap memory
	dlpRespErrParam,						// invalid parameter
	dlpRespErrNotFound,					// database, record, or resource not found
	dlpRespErrNoneOpen,					// there are no open databases
	dlpRespErrDatabaseOpen,				// database is open by someone else
	dlpRespErrTooManyOpenDatabases,	// there are too many open databases
	dlpRespErrAlreadyExists,			// DB already exists
	dlpRespErrCantOpen,					// couldn't open DB
	dlpRespErrRecordDeleted,			// record is deleted
	dlpRespErrRecordBusy,				// record is in use by someone else
	dlpRespErrNotSupported,				// the requested operation is not supported
												// on the given database type(record or resource)
	dlpRespErrUnused1,					// was dlpRespErrROMBased
	dlpRespErrReadOnly,					// caller does not have write access(or DB is in ROM)
	dlpRespErrNotEnoughSpace,			// not enough space in data store for record/resource/etc.
	dlpRespErrLimitExceeded,			// size limit exceeded
	dlpRespErrCancelSync,				// cancel the sync
	
	dlpRespErrBadWrapper,				// bad arg wrapper(for debugging)
	dlpRespErrArgMissing,				// required arg not found(for debugging)
	dlpRespErrArgSize,					// invalid argument size
	
	dlpRespErrLastReserved = 127		// last error code reserved for Palm
												// *KEEP THIS ENTRY LAST*
	} DlpRespErrorCode;


// Database flags
// NOTE: THESE *MUST* MATCH THE TOUCHDOWN DB ATTRIBUTES(AT LEAST IN THE FIRST VERSION).
// ANY CHANGES MUST BE REFLECTED IN "READ DB LIST" AND 
#define	dlpDBFlagResDB				0x0001	// resource DB if set; record DB if cleared

#define	dlpDBFlagReadOnly			0x0002	// DB is read only if set; read/write if cleared

#define	dlpDBFlagAppInfoDirty	0x0004	// Set if Application Info block is dirty
														// Optionally supported by an App's conduit

#define	dlpDBFlagBackup			0x0008	//	Set if database should be backed up to PC if
														//	no app-specific synchronization conduit has
														//	been supplied.

#define	dlpDBFlagOKToInstallNewer 	0x0010	// This tells the backup conduit that it's OK
															//  for it to install a newer version of this database
															//  with a different name if the current database is
															//  open. This mechanism is used to update the 
															//  Graffiti Shortcuts database, for example. 

#define	dlpDBFlagResetAfterInstall	0x0020 	// Device requires a reset after this database is 
															// installed.

#define	dlpDBFlagOpen					0x8000	// DB is open




// Database record attributes
#define	dlpRecAttrDeleted			0x80	// delete this record next sync
#define	dlpRecAttrDirty			0x40	// archive this record next sync
#define	dlpRecAttrBusy				0x20	// record currently in use
#define	dlpRecAttrSecret			0x10	// "secret" record - password protected
#define	dlpRecAttrArchived		0x08	// archived record


// Date/time will be described in the following format
//			yr(2 bytes), mo(1 byte, 1-12), dy(1 byte, 1-31),
//			hr(1 byte, 0-23), min(1 byte, 0-59), sec(1 byte, 0-59),
//			unused(1 byte).

typedef struct DlpDateTimeType {	// OFFSET
	Word		year;						// 0;		year (high, low)
	Byte		month;					// 2;		month: 1-12
	Byte		day;						// 3;		day: 1-31
	Byte		hour;						// 4;		hour: 0-23
	Byte		minute;					// 5;		minute: 0-59
	Byte		second;					// 6;		second: 0-59
	Byte		unused;					// 7;		unused -- set to null!
	} DlpDateTimeType;				// TOTAL: 8 bytes


 
/************************************************************
 * Request and Response headers.
 *
 * Each DLP request and response data block begins with the
 * corresponding header structure which identifies the function
 * id, argument count, and error code(responses only).
 *************************************************************/

// Request header:
//
typedef struct DlpReqHeaderType {
												// OFFSET
	Byte					id;				// 0;		request function ID
	Byte					argc;				// 2;		count of args that follow this header
												// TOTAL: 2 bytes
	} DlpReqHeaderType;

typedef DlpReqHeaderType*		DlpReqHeaderPtr;

typedef struct DlpReqType {
												// OFFSET
	DlpReqHeaderType	header;			// 0;		request header
												// FIXED SIZE: 2 bytes
	Byte					args[1];			// 2;		request arguments -- var size
	} DlpReqType;

typedef DlpReqType*		DlpReqPtr;
 
// Response header:
//
typedef struct DlpRespHeaderType {
												// OFFSET
	Byte					id;				// 0;		response function ID
	Byte					argc;				// 1;		count of arguments that follow this header
	Word					errorCode;		// 2;		error code
												// TOTAL: 4 bytes
	} DlpRespHeaderType;

typedef DlpRespHeaderType*		DlpRespHeaderPtr;

typedef struct DlpRespType {
												// OFFSET
	DlpRespHeaderType	header;			// 0;		response header
												// FIXED SIZE: 4 bytes
	Byte					args[1];			// 4;		response arguments -- var size
	} DlpRespType;
	
typedef DlpRespType*		DlpRespPtr;


// Generic request/response body type(for utility routines)
//
typedef union DlpGenericBodyType {
	Byte					id;				// request/response id
	DlpReqType			req;				// request body
	DlpRespType			resp;				// response body
	} DlpGenericBodyType;
	
typedef DlpGenericBodyType*		DlpGenericBodyPtr;


// dlpFuncRespFlag is used to form a function response ID from a
// function ID by or'ing it with the function ID.  For example: if
// dlpFuncDeleteResource is the request ID, the correct response ID
// must be (dlpFuncDeleteResource | dlpFuncRespFlag).
//
#define dlpFuncRespFlag				0x80

// dlpFuncIDMask is used to mask out the function ID value
#define dlpFuncIDMask				0x7f

// dlpFirstArgID is the value of the first argument ID to be defined for
// functions.  Values below it are reserved.
//
#define dlpFirstArgID				0x20


/************************************************************
 *
 * Argument headers used to "wrap" request and response arguments
 *
 * IMPORTANT:	ARGUMENT WRAPPERS IN REQUESTS AND RESPONSES MUST
 *					ALWAYS START ON AN EVEN-BYTE BOUNDARY.  The server
 *					implementation expects this to be the case.
 *
 *************************************************************/

// dlpSmallArgFlag is used to identify "small" argument wrappers by
// or'ing it with the argument id in argument header.
//
#define dlpSmallArgFlag			0x080

// dlpShortArgIDMask is used to mask out the argument id value
//
#define dlpShortArgIDMask	0x7F


// dlpLongArgFlag is used to identify "long" argument wrappers by
// or'ing it with the argument id in argument header.
//
#define dlpLongArgFlag			0xC000

// dlpLongArgIDMask is used to mask out the argument id value
//
#define dlpLongArgIDMask		0x3FFF


//-------------------------------------------------------------------------
// Short argument wrappers (v1.0-compatible)
//-------------------------------------------------------------------------

// Maximum short argument size which can be "wrapped"
#define dlpMaxTinyArgSize				0x000000FFL
#define dlpMaxSmallArgSize				0x0000FFFFL
#define dlpMaxShortArgSize				dlpMaxSmallArgSize



// Tiny argument header for data sizes up to 255 bytes(optimization)
//
typedef struct DlpTinyArgWrapperType {
												// OFFSET
	Byte					bID;				// 0;		argument ID
	Byte					bSize;			// 1;		argument size (does NOT include this arg header)
												// TOTAL: 2 bytes
	} DlpTinyArgWrapperType;
	
typedef struct DlpTinyArgType {
	DlpTinyArgWrapperType	wrapper;	// 0;		argument header
	Byte							data[1];	// 2;		argument data -- var size
	} DlpTinyArgType;
	

// Small argument header for data sizes above 255 bytes(*may also be used for
// smaller arguments when convenient*)
//
typedef struct DlpSmallArgWrapperType {
												// OFFSET
	Byte					bID;				// 0;		argument ID
	Byte					unused;			// 1;		unused(for alignment) -- set to null!
	Word					wSize;			// 2;		argument size (does NOT include this arg header)
												// TOTAL: 4 bytes
	} DlpSmallArgWrapperType;

typedef struct DlpSmallArgType {
	DlpSmallArgWrapperType	wrapper;	// 0;		argument header
	Byte							data[1];	// 4;		argument data -- var size
	} DlpSmallArgType;

// Unions of short argument types
typedef union DlpShortArgWrapperType {
	Byte							bID;	// arg id
	DlpTinyArgWrapperType	tiny;	// "tiny" arg wrapper
	DlpSmallArgWrapperType	small;// "tiny" arg wrapper
	} DlpShortArgWrapperType;
typedef DlpShortArgWrapperType*	DlpShortArgWrapperPtr;
	
typedef union DlpShortArgType {
	Byte							bID;			// arg id
	DlpTinyArgType				tiny;			// "tiny" arg
	DlpSmallArgType			small;		// "small" arg
	} DlpShortArgType;

typedef DlpShortArgType*			DlpShortArgPtr;



//-------------------------------------------------------------------------
// Long argument wrapper (v2.0 extension)
//-------------------------------------------------------------------------
// NOTE: Pilot v2.0 will implement the capability to parse long arguments
// but will not originate them.  This will assure backwards compatibility with
// the 1.0 desktop as well as compatibility with the future version of the
// desktop software which may originate the "long" argument wrappers.
//
// Long argument wrappers are identified by the dlpLongArgFlag bits set
// in the argument id field.

// Maximum long argument size which can be "wrapped"
#define dlpMaxLongArgSize				0xFFFFFFFFL

typedef struct DlpLongArgWrapperType {
												// OFFSET
	Word					wID;				// 0;		argument ID
	DWord					dwSize;			// 2;		argument size (does NOT include this arg header)
												// TOTAL: 6 bytes
	} DlpLongArgWrapperType;
typedef DlpLongArgWrapperType*		DlpLongArgWrapperPtr;

typedef struct DlpLongArgType {
												// OFFSET
	DlpLongArgWrapperType	wrapper;	// 0;		argument header
	Byte							data[1];	// 6;		argument data -- var size
	} DlpLongArgType;
typedef DlpLongArgType*	DlpLongArgPtr;


//-------------------------------------------------------------------------
// Unions of all argument and wrapper types
//-------------------------------------------------------------------------

// Union of all argument wrapper types
typedef union DlpGenericArgWrapperType {
	DlpShortArgWrapperType	shortWrap;	// "short" arg wrapper(tiny and small)
	DlpLongArgWrapperType	longWrap;	// "long" arg wrapper
	} DlpGenericArgWrapperType;
typedef DlpGenericArgWrapperType*		DlpGenericArgWrapperPtr;


// Union of all argument types
typedef union DlpGenericArgType {
	DlpShortArgType			shortArg;	// "short" arg(tiny and small)
	DlpLongArgType				longArg;		// "long" arg
	} DlpGenericArgType;
typedef DlpGenericArgType*			DlpGenericArgPtr;


/********************************************************************
 * Desktop Link Protocol Parameters
 ********************************************************************/

// dlpCmdTimeoutSec -- this is the number of seconds to wait for a command
// to begin coming in before timing out
//
#define dlpCmdTimeoutSec				30



/************************************************************
 * DLP function argument structures
 *************************************************************/


//////////////////////////////////////////////////////////////////////////
// dlpReadUserInfo
//////////////////////////////////////////////////////////////////////////
//		Request arguments: none
//
//		Response arguments:
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory

// Request argument structure:
//
//		none.
	
// Response argument structure:
//
#define dlpReadUserInfoRespArgID		dlpFirstArgID

typedef struct DlpReadUserInfoRespHdrType {
												// OFFSET
	DWord					userID;			//	0;		user ID number (0 if none)
	DWord					viewerID;		// 4;		id assigned to viewer by the desktop
	DWord					lastSyncPC;		// 8;		last sync PC id (0 if none)
	DlpDateTimeType	succSyncDate;	//	12;	last successful sync (year = 0 if none)
	DlpDateTimeType	lastSyncDate;	//	20;	last sync date(year = 0 if none)
	Byte					userNameLen;	//	28;	length of user name field,
												//			including null (0 = no user name)
	Byte					passwordLen;	//	29;	length of encrypted password
												//			(0 = no password set)
												// TOTAL: 30 bytes										
	} DlpReadUserInfoRespHdrType;

typedef struct DlpReadUserInfoRespType {
												// OFFSET
	DlpReadUserInfoRespHdrType			//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 30 bytes										
	Byte					nameAndPassword[1];	// 30;	user name -- var size
	// User name begins at the nameAndPassword field and is null-terminated.
	// The encrypted password follows the user name and is NOT null-terminated.
	// The encrypted password may contain any byte values(0-255).
	} DlpReadUserInfoRespType;


	
//////////////////////////////////////////////////////////////////////////
//	dlpWriteUserInfo
//////////////////////////////////////////////////////////////////////////
//		Request arguments:
//			user id (4 bytes)
//			last sync PC id(4 bytes)
//			user name
//
//		Response arguments: none
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrNotEnoughSpace,
//			dlpRespErrParam

// Request argument structure:
//
#define dlpWriteUserInfoReqArgID		dlpFirstArgID

#define dlpMaxUserNameSize		41		// max user name size, including null byte

typedef struct DlpWriteUserInfoReqHdrType {
												// OFFSET
	DWord					userID;			//	0;		user ID number
	DWord					viewerID;		// 4;		id assigned to viewer by the desktop
	DWord					lastSyncPC;		// 8;		last sync PC id
	DlpDateTimeType	lastSyncDate;	//	12;	last sync date(year = 0 if none)
	Byte					modFlags;		// 20;	flags indicating which values are being
												//			modified; see the dlpUserInfoMod...
												//			flags defined below
	Byte					userNameLen;	//	21;	user name length, including null
												// TOTAL: 22 bytes										
	} DlpWriteUserInfoReqHdrType;

// Flags indicating which values are being changed by the dlpWriteUserInfo
// request.  These flags are used in the modFlags field of DlpWriteUserInfoReqHdrType.
// These flags are additive.
//
#define dlpUserInfoModUserID			0x80	// changing the user id
#define dlpUserInfoModSyncPC			0x40	// changing the last sync PC id
#define dlpUserInfoModSyncDate		0x20	// changing sync date
#define dlpUserInfoModName				0x10	// changing user name
#define dlpUserInfoModViewerID		0x08	// changing the viewer id

typedef struct DlpWriteUserInfoReqType {
												// OFFSET
	DlpWriteUserInfoReqHdrType
							header;			//	0;		fixed-size header
												// FIXED SIZE: 22 bytes										
	Byte					userName[1];	// 22;	user name -- var size
	// User name begins at the userName field and is null-terminated.
	} DlpWriteUserInfoReqType;

	
// Response argument structure:
//
//		none.


	
//////////////////////////////////////////////////////////////////////////
//	dlpReadSysInfo
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem
//

// Request argument structure:
//
//		none.
	
// Response argument structure:
//
enum {
	dlpReadSysInfoRespArgID		= dlpFirstArgID
	};

// dlpReadSysInfoRespArgID arg structure:
//
typedef struct DlpReadSysInfoRespType {
												// OFFSET
	DWord					romSWVersion;	// 0;		ROM-based sys software version
	DWord					localizationID;// 4;		localization ID
	Byte					unused;			//	8;		unused(for alignment) -- set to null!
	Byte					prodIDSize;		// 9;		size of productID/model field
	DWord					prodID;			// 10;	product id (was variable size)
												// TOTAL: 14 bytes
	} DlpReadSysInfoRespType;
	


//////////////////////////////////////////////////////////////////////////
//	dlpGetSysDateTime
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes: none

// Request argument structure:
//
//		none.
	
// Response argument structure:
//
#define dlpGetSysDateTimeRespArgID		dlpFirstArgID

typedef struct DlpGetSysDateTimeRespType {
												// OFFSET
	DlpDateTimeType	dateTime;		// 0;		system date/time
												// TOTAL: 8 bytes
	} DlpGetSysDateTimeRespType;



//////////////////////////////////////////////////////////////////////////
//	dlpSetSysDateTime
//////////////////////////////////////////////////////////////////////////
//		Request arguments:
//			new system date/time
//
//		Response arguments: none
//
//		Possible error codes
//			dlpRespErrParam

// Request argument structure:
//
#define dlpSetSysDateTimeReqArgID		dlpFirstArgID

typedef struct DlpSetSysDateTimeReqType {
												// OFFSET
	DlpDateTimeType	dateTime;		// 0;		new system date/time
												// TOTAL: 8 bytes
	} DlpSetSysDateTimeReqType;

typedef DlpSetSysDateTimeReqType*	DlpSetSysDateTimeReqPtr;
	
// Response argument structure:
//
//		none.


	
//////////////////////////////////////////////////////////////////////////
//	dlpReadStorageInfo
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrNotFound

// Request argument structure:
//
enum {
	dlpReadStorageInfoReqArgID		= dlpFirstArgID
	};

typedef struct DlpReadStorInfoReqType {
												// OFFSET
	Byte					startCardNo;	// 0;		card number to start at
												//			(0 = first)
	Byte					unused;			//	1;		unused -- set to null!
												// TOTAL: 2 bytes
	} DlpReadStorInfoReqType;


// Response argument structure:
//
enum {
	dlpReadStorageInfoRespArgID		= dlpFirstArgID,
	dlpReadStorageInfoExRespArgID		// v1.1 extension
	};

//
// dlpReadStorageInfoRespArgID:
//

// Card info structure of variable size
typedef struct DlpCardInfoHdrType {
												// OFFSET
	Byte					totalSize;		// 0;		total size of this card info
												//			*ROUNDED UP TO EVEN SIZE*
	Byte					cardNo;			// 1;		card number
	Word					cardVersion;	// 2;		card version
	DlpDateTimeType	crDate;			// 4;		creation date/time
	DWord					romSize;			// 12;		ROM size
	DWord					ramSize;			// 16;	RAM size
	DWord					freeRam;			// 20;	free RAM size
	Byte					cardNameSize;	// 24;	size of card name string
	Byte					manufNameSize;	// 25;	size of manuf. name string
												// TOTAL: 26 bytes;
	} DlpCardInfoHdrType;

typedef struct DlpCardInfoType {
												// OFFSET
	DlpCardInfoHdrType					//
							header;			// 0;		fixed-size header
												// FIXED SIZE: 26 bytes;
	Byte					cardNameAndManuf[1];
												// 26;	card name and manuf. text -- var size
	// Card name is the cardNameSize bytes of text at cardNameAndManuf,
	// followed immediately by manufNameSize bytes of manufacturer name.
	} DlpCardInfoType;

	
typedef struct DlpReadStorInfoRespHdrType {
												// OFFSET
	Byte					lastCardNo;		// 0;		card number of last card retrieved
	Byte					more;				//	1;		non-zero if there are more cards
	Byte					unused;			//	2;		unused -- set to null!
	Byte					actCount;		// 3;		actual count of structures returned
												// TOTAL: 4 bytes
	} DlpReadStorInfoRespHdrType;

typedef struct DlpReadStorInfoRespType {
												// OFFSET
	DlpReadStorInfoRespHdrType			//
							header;			// 0;		fixed-size header
												// FIXED SIZE: 4 bytes
	DlpCardInfoType	cardInfo[1];	// 4;		actCount of card info structures -- var size
	} DlpReadStorInfoRespType;


//
// EXTENDED ARGUMENTS(DL v1.1): dlpReadStorageInfoExRespArgID
//
typedef struct DlpReadStorInfoExRespType {
												// OFFSET
	Word					romDBCount;		// 0;		ROM database count
	Word					ramDBCount;		// 2;		RAM database count
	DWord					dwReserved1;	// 4;		RESERVED -- SET TO NULL!
	DWord					dwReserved2;	// 8;		RESERVED -- SET TO NULL!
	DWord					dwReserved3;	// 12;	RESERVED -- SET TO NULL!
	DWord					dwReserved4;	// 16;	RESERVED -- SET TO NULL!
												// TOTAL: 20 bytes
	} DlpReadStorInfoExRespType;
	
	
	
	
//////////////////////////////////////////////////////////////////////////
//	dlpReadDBList
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrNotFound

// Request argument structure:
//
#define dlpReadDBListReqArgID		dlpFirstArgID

typedef struct DlpReadDBListReqType {
												// OFFSET
	Byte					srchFlags;		// 0;		search flags
	Byte					cardNo;			// 1;		card number -- 0-based
	Word					startIndex;		// 2;		DB index to start at
												//			(0 = from beginning)
												// TOTAL: 4 bytes
	} DlpReadDBListReqType;

#define dlpReadDBListFlagRAM		0x80		// Search for RAM-based
#define dlpReadDBListFlagROM		0x40		// Search for ROM-based


// Response argument structure:
//
#define dlpReadDBListRespArgID		dlpFirstArgID

// Database info structure of variable size
typedef struct DlpDBInfoHdrType {
												// OFFSET
	Byte					totalSize;		// 0;		total size of the DB info
												//			*ROUNDED UP TO EVEN SIZE*
	Byte					miscFlags;		//	1;		flags(v1.1) -- set all unused bits to null!
	Word					dbFlags;			// 2;		DB flags: dlpDBFlagReadOnly,
												//			dlpDBFlagResDB,
												//			dlpDBFlagAppInfoDirty, dlpDBFlagOpen,
												//			dlpDBFlagBackup;
	DWord					type;				// 4;		database type
	DWord					creator;			// 8;		database creator
	Word					version;			// 12;	database version
	DWord					modNum;			// 14;	modification number
	DlpDateTimeType	crDate;			// 18;	creation date
	DlpDateTimeType	modDate;			// 26;	latest modification date
	DlpDateTimeType	backupDate;		// 34;	latest backup date
	Word					dbIndex;			//	42;	DB index
												// TOTAL: 44 bytes;
	} DlpDBInfoHdrType;
#define dlpDbInfoMiscFlagExcludeFromSync	0x80

typedef struct DlpDBInfoType {
												// OFFSET
	DlpDBInfoHdrType	header;			// 0;		fixed-size header
												// FIXED SIZE: 44 bytes;
	Byte					name[1];			// 44;	databse name text -- var size and
												//			null-terminated
	} DlpDBInfoType;

	
typedef struct DlpReadDBListRespHdrType {
												// OFFSET
	Word					lastIndex;		// 0;		DB index of last entry retrieved
	Byte					flags;			// 2;		flags: dlpReadDBListRespFlagMore
	Byte					actCount;		// 3;		actual count of structures returned
												// TOTAL: 4 bytes
	} DlpReadDBListRespHdrType;

// dlpReadDBListRespFlagMore flag: if set, indicates that there are more
// databases to list -- this enables the server to send the listing
// incrementally, reducing server memory requirements if necessary
#define dlpReadDBListRespFlagMore	0x80
	
typedef struct DlpReadDBListRespType {
												// OFFSET
	DlpReadDBListRespHdrType			//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 4 bytes
	DlpDBInfoType		dbInfo[1];		// 4;		actCount of DB info structures -- var size
	} DlpReadDBListRespType;
	

//////////////////////////////////////////////////////////////////////////
//	dlpOpenDB
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrParam,
//			dlpRespErrNotFound
//			dlpRespErrTooManyOpenDatabases
//			dlpRespErrCantOpen

// Request argument structure:
//
#define dlpOpenDBReqArgID		dlpFirstArgID

typedef struct DlpOpenDBReqHdrType {
												// OFFSET
	Byte					cardNo;			// 0;		memory module number
	Byte					mode;				// 1;		open mode
												// TOTAL: 2 bytes;
	} DlpOpenDBReqHdrType;

#define dlpOpenDBModeRead			0x80
#define dlpOpenDBModeWrite			0x40
#define dlpOpenDBModeExclusive	0x20
#define dlpOpenDBModeShowSecret	0x10


typedef struct DlpOpenDBReqType {
												// OFFSET
	DlpOpenDBReqHdrType					//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 2 bytes;
	Byte					name[1];			// 2;		databse name text -- var size
												//			zero-terminated
	} DlpOpenDBReqType;

typedef DlpOpenDBReqType*		DlpOpenDBReqPtr;

	
// Response argument structure:
//
#define dlpOpenDBRespArgID		dlpFirstArgID
//
// The response argument is the 1-byte database ID to be passed in
// subsequent read/write requests.

	
	
//////////////////////////////////////////////////////////////////////////
//	dlpCreateDB
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrParam,
//			dlpRespErrAlreadyExists,
//			dlpRespErrCantOpen,
//			dlpRespErrNotEnoughSpace,
//			dlpRespErrTooManyOpenDatabases

// Request argument structure:
//
#define dlpCreateDBReqArgID		dlpFirstArgID

typedef struct DlpCreateDBReqHdrType {
												// OFFSET
	DWord					creator;			// 0;		DB creator
	DWord					type;				// 4;		DB type
	Byte					cardNo;			// 8;		memory module number
	Byte					unused;			//	9;		unused -- set to null
	Word					dbFlags;			// 10;	allowed flags: dlpDBFlagResDB,
												//			dlpDBFlagBackup, dlpDBFlagOKToInstallNewer,
												//			dlpDBFlagResetAfterInstall
	Word					version;			// 12;	DB version #
												// TOTAL: 14 bytes;
	} DlpCreateDBReqHdrType;

typedef struct DlpCreateDBReqType {
												// OFFSET
	DlpCreateDBReqHdrType				//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 12 bytes;
	Byte					name[1];			// 14;	DB name text -- var size
												//			zero-terminated
	} DlpCreateDBReqType;

typedef DlpCreateDBReqType*		DlpCreateDBReqPtr;

	
// Response argument structure:
//
#define dlpCreateDBRespArgID		dlpFirstArgID

// The response argument is the 1-byte database ID to be passed in
// subsequent read/write requests.


	
//////////////////////////////////////////////////////////////////////////
//	dlpCloseDB
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrParam,
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrNoneOpen

// Request argument structure:
//
typedef enum {
	dlpCloseDBReqDBIDArgID = dlpFirstArgID,		// close a specific database
	dlpCloseDBReqAllArgID								// close all databases
	} DlpCloseDBReqArgID;

// Argument structure to close a specific database(dlpCloseDBReqDBIDArgID):
//
// The request argument is the 1-byte database ID returned in open/create
// DB responses.

// Argument structure to close all databases(dlpCloseDBReqAllArgID):
//
// This request argument contains no data


// Response argument structure:
//
//		none.


	
//////////////////////////////////////////////////////////////////////////
//	dlpDeleteDB
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrNotFound,
//			dlpRespErrCantOpen,
//			dlpRespErrDatabaseOpen

// Request argument structure:
//
#define dlpDeleteDBReqArgID		dlpFirstArgID

typedef struct DlpDeleteDBReqHdrType {
												// OFFSET
	Byte					cardNo;			// 0;		memory module number
	Byte					unused;			// 1;		unused -- set to null!
												// TOTAL: 2 bytes;
	} DlpDeleteDBReqHdrType;


typedef struct DlpDeleteDBReqType {
												// OFFSET
	DlpDeleteDBReqHdrType				//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 2 bytes;
	Byte					name[1];			// 2;		databse name text -- var size
												//			zero-terminated
	} DlpDeleteDBReqType;

typedef DlpDeleteDBReqType*		DlpDeleteDBReqPtr;


// Response argument structure:
//
//		none.


//////////////////////////////////////////////////////////////////////////
//	dlpReadOpenDBInfo
//////////////////////////////////////////////////////////////////////////
//		Get information on an open database
//
//		Possible error codes
//			dlpRespErrNoneOpen
//			dlpRespErrParam

// Request argument structure:
//
#define dlpReadOpenDBInfoArgID			dlpFirstArgID

// The request argument is the 1-byte database ID returned in open/create
// DB responses.
	

// Response argument structure:
//
#define dlpReadOpenDBInfoRespArgID		dlpFirstArgID


typedef struct DlpReadOpenDBInfoRespType {
												// OFFSET
	Word					numRec;			//	0;		number of records or resources
												// TOTAL: 2 bytes
	} DlpReadOpenDBInfoRespType;

	
//////////////////////////////////////////////////////////////////////////
//	dlpMoveCategory
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrNoneOpen
//			dlpRespErrParam
//			dlpRespErrNotSupported
//			dlpRespErrReadOnly

// Request argument structure:
//
#define dlpMoveCategoryReqArgID			dlpFirstArgID

typedef struct DlpMoveCategoryReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					fromCategory;	//	1;		id of the "from" category
	Byte					toCategory;		//	2;		id of the "to" category
	Byte					unused;			//	3;		unused -- set to null!
												// TOTAL: 4 bytes;
	} DlpMoveCategoryReqType;
	

// Response argument structure:
//
//		none.

	
//////////////////////////////////////////////////////////////////////////
//	dlpReadAppBlock
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrNotFound
//			dlpRespErrNoneOpen
//			dlpRespErrParam

// Request argument structure:
//
#define dlpReadBlockReqArgID			dlpFirstArgID

typedef struct DlpReadBlockReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		unused -- set to null!
	Word					offset;			//	2;		offset into the block
	Word					numBytes;		//	4;		number of bytes to read starting
												//			at offset(-1 = to the end)
												// TOTAL: 6 bytes;
	} DlpReadBlockReqType;
	

// Response argument structure:
//
#define dlpReadBlockRespArgID			dlpFirstArgID

typedef struct DlpReadBlockRespHdrType {
												// OFFSET
	Word					blockSize;		//	0;		actual block size -- may be greater
												//			than the amount of data returned
												//	TOTAL: 2 bytes
	} DlpReadBlockRespHdrType;

typedef struct DlpReadBlockRespType {
												// OFFSET
	DlpReadBlockRespHdrType				//
							header;			//	0;		fixed size header
												//	FIXED SIZE: 2 bytes
	Byte					data[1];			// 2;		block data -- var size
	} DlpReadBlockRespType;

	
//////////////////////////////////////////////////////////////////////////
//	dlpWriteAppBlock
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrReadOnly
//			dlpRespErrNotEnoughSpace
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpWriteBlockReqArgID			dlpFirstArgID

typedef struct DlpWriteBlockReqHdrType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		unused -- set to null!
	Word					blockSize;		//	2;		total block size(0 = free existing block)
												// TOTAL: 4 bytes;
	} DlpWriteBlockReqHdrType;


typedef struct DlpWriteBlockReqType {
												// OFFSET
	DlpWriteBlockReqHdrType				//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 4 bytes;
	Byte					data[1];			// 4;		block data -- var size
	} DlpWriteBlockReqType;


// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpReadSortBlock
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory
//			dlpRespErrNotFound
//			dlpRespErrNoneOpen

// Request argument structure:
//
// see dlpReadAppBlock


// Response argument structure:
//
// see dlpReadAppBlock


//////////////////////////////////////////////////////////////////////////
//	dlpWriteSortBlock
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrParam,
//			dlpRespErrNoneOpen

// Request argument structure:
//
// see dlpWriteAppBlock


// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpReadNextModifiedRec
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			see dlpReadRecord
//

// Request argument structure:
//
#define dlpReadNextModRecReqArgID		dlpFirstArgID
// The request argument is the 1-byte database ID returned in open/create
// DB responses.


// Response argument structure:
//
// Response argument id = dlpReadRecordRespArgID

//	Response argument structure = DlpReadRecordRespType



//////////////////////////////////////////////////////////////////////////
//	dlpResetRecordIndex
//////////////////////////////////////////////////////////////////////////
//	Resets the "next modified record" index to the beginning
//
//		Possible error codes
//			dlpRespErrParam
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpResetRecordIndexReqArgID		dlpFirstArgID

// The request argument is the 1-byte database ID returned in open/create
// DB responses.

// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpReadRecord
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrNotSupported,
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrParam,
//			dlpRespErrNotFound,
//			dlpRespErrRecordBusy,
//			dlpRespErrNoneOpen

typedef enum {
	dlpReadRecordIdArgID = dlpFirstArgID,
	dlpReadRecordIndexArgID
	} DlpReadRecordReqArgID;

// dlpReadRecordIdArgID request argument structure:
//
typedef struct DlpReadRecordByIDReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		unused -- set to null!
	DWord					recordID;		// 2;		unique record id
	Word					offset;			//	6;		offset into the record
	Word					numBytes;		//	8;		number of bytes to read starting
												//			at the offset(-1 = "to the end")
												// TOTAL: 10 bytes;
	} DlpReadRecordByIDReqType;

// dlpReadRecordIndexArgID request argument structure:
//
typedef struct DlpReadRecordByIndexReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		unused -- set to null!
	Word					index;			// 2;		record index
	Word					offset;			//	4;		offset into the record
	Word					numBytes;		//	6;		number of bytes to read starting
												//			at the offset(-1 = "to the end")
												// TOTAL: 8 bytes;
	} DlpReadRecordByIndexReqType;


// Response argument structure:
//
#define dlpReadRecordRespArgID		dlpFirstArgID

typedef struct DlpReadRecordRespHdrType {
												// OFFSET
	DWord					recordID;		// 0;		unique record id
	Word					index;			//	4;		record index
	Word					recSize;			//	6;		total record size in bytes
	Byte					attributes;		// 8;		record attributes
	Byte					category;		// 9;		record category index
												// TOTAL: 10 bytes;
	} DlpReadRecordRespHdrType;

typedef struct DlpReadRecordRespType {
												// OFFSET
	DlpReadRecordRespHdrType			//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 8 bytes;
	Byte					data[1];			// 8;		record data -- var size
	} DlpReadRecordRespType;



//////////////////////////////////////////////////////////////////////////
//	dlpReadRecordIDList
//////////////////////////////////////////////////////////////////////////
//	Returns a list of unique record id's.  May need to call more than once
// to get the entire list.  dlpRespErrNotFound is returned when "start"
// is out of bounds
//
//		Possible error codes
//			dlpRespErrNotSupported,
//			dlpRespErrParam,
//			dlpRespErrNotFound,
//			dlpRespErrNoneOpen

#define dlpReadRecordIDListReqArgID		dlpFirstArgID

// dlpReadRecordIDListReqArgID request argument structure:
//
typedef struct DlpReadRecordIDListReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					flags;			//	1;		request flags
	Word					start;			// 2;		starting record index (0-based)
	Word					maxEntries;		//	4;		maximum number of entries, or
												//			0xFFFF to return as many as possible
												// TOTAL: 6 bytes;
	} DlpReadRecordIDListReqType;

// dlpReadRecordIDListFlagSortDB: if set, DL Server will call the creator
// application to resort the database before returning the list.
#define dlpReadRecordIDListFlagSortDB	0x80


// Response argument structure:
//
#define dlpReadRecordIDListRespArgID		dlpFirstArgID

typedef struct DlpReadRecordIDListRespHdrType {
												// OFFSET
	Word					numEntries;		// 0;		number of entries returned
												// TOTAL: 2 bytes;
	} DlpReadRecordIDListRespHdrType;

typedef struct DlpReadRecordIDListRespType {
												// OFFSET
	DlpReadRecordIDListRespHdrType	//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 2 bytes;
	DWord					recID[1];		// 8;		list of record id's -- var size
	} DlpReadRecordIDListRespType;

	

//////////////////////////////////////////////////////////////////////////
//	dlpWriteRecord
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrNotFound
//			dlpRespErrNotEnoughSpace
//			dlpRespErrNotSupported
//			dlpRespErrReadOnly
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpWriteRecordReqArgID		dlpFirstArgID

// dlpWriteRecordReqArgID -- required
typedef struct DlpWriteRecordReqHdrType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					flags;			//	1;		set unused bits to null!
	DWord					recordID;		// 2;		unique record id or null
	Byte					attributes;		// 6;		record attributes -- only
												//			dlpRecAttrSecret is allowed here
												//			v1.1 extension:
												//			dlpRecAttrDeleted, dlpRecAttrArchived and
												//			dlpRecAttrDirty are also allowed.
	Byte					category;		// 7;		record category
												// TOTAL: 8 bytes;
	} DlpWriteRecordReqHdrType;

#define dlpWriteRecordReqFlagDataIncluded	0x80	// original implementer of destop software always
																// set this bit.  Define it here for compatibility

typedef struct DlpWriteRecordReqType {
												// OFFSET
	DlpWriteRecordReqHdrType			//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 8 bytes;
	Byte					data[1];			// 8;		record data -- var size
	} DlpWriteRecordReqType;


// Response argument structure:
//
#define dlpWriteRecordRespArgID		dlpFirstArgID

typedef struct DlpWriteRecordRespType {
												// OFFSET
	DWord					recordID;		// 0;		record ID
												// TOTAL: 4 bytes
	} DlpWriteRecordRespType;



//////////////////////////////////////////////////////////////////////////
//	dlpDeleteRecord
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrNotFound
//			dlpRespErrNotSupported
//			dlpRespErrReadOnly
//			dlpRespErrNoneOpen
//

// Request argument structure:
//
#define dlpDeleteRecordReqArgID	dlpFirstArgID

// Argument structure to delete by record ID(dlpDeleteRecordReqIDArgID):
typedef struct DlpDeleteRecordReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					flags;			//	1;		flags (dlpDeleteRecFlagDeleteAll)
	DWord					recordID;		// 2;		unique record id (see flags)
												// TOTAL: 6 bytes;
	} DlpDeleteRecordReqType;

// dlpDeleteRecFlagDeleteAll: if this flag is set, the reocordID field
// is ignored and all database records will be deleted
#define dlpDeleteRecFlagDeleteAll		0x80

// dlpDeleteRecFlagByCategory: if this flag is set, the least significant byte
// of the reocordID field contains the category id of records to be deleted
#define dlpDeleteRecFlagByCategory		0x40

// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpReadResource
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrNotFound
//			dlpRespErrNoneOpen

typedef enum {
	dlpReadResourceIndexArgID = dlpFirstArgID,
	dlpReadResourceTypeArgID
	} DlpReadResourceReqArgID;

// dlpReadResourceIndexArgID request argument structure:
//
typedef struct DlpReadResourceByIndexReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		unused -- set to null!
	Word					index;			// 2;		resource index
	Word					offset;			//	4;		offset into the resource
	Word					numBytes;		//	6;		number of bytes to read starting
												//			at the offset(-1 = "to the end")
												// TOTAL: 8 bytes;
	} DlpReadResourceByIndexReqType;

// dlpReadResourceTypeArgID request argument structure:
//
typedef struct DlpReadResourceByTypeReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		unused -- set to null!
	DWord					type;				// 2;		resource type
	Word					id;				// 6;		resource id
	Word					offset;			//	8;		offset into the resource
	Word					numBytes;		//	10;	number of bytes to read starting
												//			at the offset(-1 = "to the end")
												// TOTAL: 12 bytes;
	} DlpReadResourceByTypeReqType;
	

// Response argument structure:
//
#define dlpReadResourceRespArgID		dlpFirstArgID

typedef struct DlpReadResourceRespHdrType {
												// OFFSET
	DWord					type;				// 0;		resource type
	Word					id;				// 4;		resource id
	Word					index;			//	6;		resource index
	Word					resSize;			//	8;		total resource size in bytes
												// TOTAL: 10 bytes;
	} DlpReadResourceRespHdrType;

typedef struct DlpReadResourceRespType {
												// OFFSET
	DlpReadResourceRespHdrType			//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 6 bytes;
	Byte					resData[1];		// 6;		resource data -- var size
	} DlpReadResourceRespType;



//////////////////////////////////////////////////////////////////////////
//	dlpWriteResource
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrNotEnoughSpace,
//			dlpRespErrParam,
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpWriteResourceReqArgID		dlpFirstArgID

typedef struct DlpWriteResourceReqHdrType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					unused;			//	1;		inused -- set to null!
	DWord					type;				// 2;		resource type
	Word					id;				// 6;		resource id
	Word					resSize;			//	8;		total resource size
												// TOTAL: 10 bytes;
	} DlpWriteResourceReqHdrType;

typedef struct DlpWriteResourceReqType {
												// OFFSET
	DlpWriteResourceReqHdrType			//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 10 bytes;
	Byte					data[1];			// 10;		resource data -- var size
	} DlpWriteResourceReqType;

// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpDeleteResource
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrNotFound
//			dlpRespErrNotSupported
//			dlpRespErrReadOnly
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpDeleteResourceReqArgID		dlpFirstArgID

typedef struct DlpDeleteResourceReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					flags;			//	1;		flags (dlpDeleteResFlagDeleteAll)
	DWord					type;				// 2;		resource type
	Word					id;				// 6;		resource id
												// TOTAL: 8 bytes;
	} DlpDeleteResourceReqType;

// dlpDeleteResFlagDeleteAll: if set, all resources in the db will be deleted
#define dlpDeleteResFlagDeleteAll	0x80


// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpCleanUpDatabase
//////////////////////////////////////////////////////////////////////////
//		Deletes all records which are marked as archived or deleted in the
//		record database
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrReadOnly,
//			dlpRespErrNotSupported
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpCleanUpDatabaseReqArgID		dlpFirstArgID

// The request argument is the 1-byte database ID returned in open/create
// DB responses.

// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpResetSyncFlags
//////////////////////////////////////////////////////////////////////////
//		For record databases, reset all dirty flags.
//		For both record and resource databases, set the last sync time to NOW
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam
//			dlpRespErrReadOnly,
//			dlpRespErrNoneOpen

// Request argument structure:
//
#define dlpResetSyncFlagsReqArgID		dlpFirstArgID

// The request argument is the 1-byte database ID returned in open/create
// DB responses.


// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpCallApplication
//////////////////////////////////////////////////////////////////////////
//		Call an application entry point via an action code
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrParam,
//			dlpRespErrNotFound

// Request argument structure:
//
enum	{
	dlpCallApplicationReqArgIDV10 = dlpFirstArgID,	// req id for Pilot v1.0
	dlpCallAppReqArgID										// req id for Pilot v2.0 and later
	};

// dlpCallApplicationReqArgIDV10:
typedef struct DlpCallApplicationReqHdrTypeV10 {
												// OFFSET
	DWord					creator;			// 0;		app DB creator id
	Word					action;			// 4;		action code
	Word					paramSize;		// 6;	custom param size
												// TOTAL: 8 bytes
	} DlpCallApplicationReqHdrTypeV10;

typedef struct DlpCallApplicationReqTypeV10 {
												// OFFSET
	DlpCallApplicationReqHdrTypeV10	//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 8 bytes
	Byte					paramData[1];	// 8;	custom param data -- var size
	} DlpCallApplicationReqTypeV10;


// dlpCallAppReqArgID (Pilot v2.0):
typedef struct DlpCallAppReqHdrType {
												// OFFSET
	DWord					creator;			// 0;		DB creator id of target executable
	DWord					type;				// 4;		DB type id of target executable
	Word					action;			// 8;		action code
	DWord					dwParamSize;	// 10;	custom param size in number of bytes
	DWord					dwReserved1;	// 14;	RESERVED -- set to NULL!!!
	DWord					dwReserved2;	// 18;	RESERVED -- set to NULL!!!
												// TOTAL: 22 bytes
	} DlpCallAppReqHdrType;

typedef struct DlpCallAppReqType {
												// OFFSET
	DlpCallAppReqHdrType					//
							hdr;				//	0;		fixed-size header
												// FIXED SIZE: 22 bytes
	Byte					paramData[1];	// 22;	custom param data -- var size
	} DlpCallAppReqType;



// Response argument structure:
//
enum	{
	dlpCallApplicationRespArgIDV10 = dlpFirstArgID,		// resp id for Pilot v1.0
	dlpCallAppRespArgID											// resp id for Pilot v2.0 and later
	};

// dlpCallApplicationRespArgIDV10:
typedef struct DlpCallApplicationRespHdrTypeV10 {
												// OFFSET
	Word					action;			// 0;		action code which was called
	Word					resultCode;		// 2;		result error code returned by action
	Word					resultSize;		// 4;		custom result data size
												// TOTAL: 6 bytes
	} DlpCallApplicationRespHdrTypeV10;

typedef struct DlpCallApplicationRespTypeV10 {
												// OFFSET
	DlpCallApplicationRespHdrTypeV10	//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 6 bytes
	Byte					resultData[1];	// 6;		custom result data -- var size
	} DlpCallApplicationRespTypeV10;

// dlpCallAppRespArgID:
typedef struct DlpCallAppRespHdrType {
												// OFFSET
	DWord					dwResultCode;	// 0;		result error code returned by handler
	DWord					dwResultSize;	// 4;		custom result data size
	DWord					dwReserved1;	// 8;		RESERVED -- SET TO NULL!!!
	DWord					dwReserved2;	// 12;	RESERVED -- SET TO NULL!!!
												// TOTAL: 16 bytes
	} DlpCallAppRespHdrType;

typedef struct DlpCallAppRespType {
												// OFFSET
	DlpCallAppRespHdrType				//
							hdr;				//	0;		fixed-size header
												// FIXED SIZE: 16 bytes
	Byte					resultData[1];	// 16;		custom result data -- var size
	} DlpCallAppRespType;



//////////////////////////////////////////////////////////////////////////
//	dlpResetSystem
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem

// Request argument structure:
//
//		none.

// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
// dlpAddSyncLogEntry
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrNotEnoughSpace,
//			dlpRespErrLimitExceeded,
//			dlpRespErrParam

// Request argument structure:
//
#define dlpAddSyncLogEntryReqArgID		dlpFirstArgID

typedef struct DlpAddSyncLogEntryReqType {
												// OFFSET
	Byte					text[1];			// 0;		entry text -- var size and
												//			null-terminated
	} DlpAddSyncLogEntryReqType;

// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
// dlpProcessRPC
//////////////////////////////////////////////////////////////////////////
//		Remote Procedure Call interface
//
//		Request arguments:
//			RPC command block
//
//		Response arguments:
//			RPC reply block
//
//		Possible error codes
//			0 on success; otherwise error code from the TouchDown
//			RPC executor
//
// NOTE: this is a low-level system command which does not use arg wrappers.

// Request argument structure:
//
// Block of RPC command data (no arg wrapper)

// Response argument structure:
//
// Block of RPC reply data of same length as command block(no arg wrapper)

	

//////////////////////////////////////////////////////////////////////////
// dlpOpenConduit
//////////////////////////////////////////////////////////////////////////
//		This command is sent before each conduit is opened by the desktop.
//		If the viewer has a cancel pending, it will return dlpRespErrCancelSync
//		in the response header's errorCode field.
//
//		Request arguments:	none.
//
//		Response arguments:	none.
//
//		Possible error codes
//			dlpRespErrCancelSync

// Request argument structure:
//
//		none.

// Response argument structure:
//
//		none.


	
//////////////////////////////////////////////////////////////////////////
// dlpEndOfSync
//////////////////////////////////////////////////////////////////////////
//		This command is sent by the desktop to end the sync.
//
//		Request arguments:	termination code: 0 = normal termination;
//									otherwise the client is aborting the sync
//
//		Possible error codes
//			0

// Request argument structure:
//
#define dlpEndOfSyncReqArgID		dlpFirstArgID

typedef enum DlpSyncTermCode {
	dlpTermCodeNormal = 0,				// normal termination
	dlpTermCodeOutOfMemory,				// termination due to low memory on TD
	dlpTermCodeUserCan,					// user cancelled from desktop
	dlpTermCodeOther						// catch-all abnormal termination code
	} DlpSyncTermCode;
	
	
typedef struct DlpEndOfSyncReqType {
												// OFFSET
	Word					termCode;		// 0;		termination code
												// TOTAL: 2 bytes
	} DlpEndOfSyncReqType;


// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
//	dlpReadNextRecInCategory
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			see dlpReadRecord

#define dlpReadNextRecInCategoryReqArgID	dlpFirstArgID

// dlpReadNextRecInCategoryReqArgID request argument structure:
//
typedef struct DlpReadNextRecInCategoryReqType {
												// OFFSET
	Byte					dbID;				//	0;		database id
	Byte					category;		//	1;		category id
												// TOTAL: 2 bytes;
	} DlpReadNextRecInCategoryReqType;


// Response argument structure:
//
// Response argument id = dlpReadRecordRespArgID

//	Response argument structure = DlpReadRecordRespType



//////////////////////////////////////////////////////////////////////////
//	dlpReadNextModifiedRecInCategory
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			see dlpReadNextRecInCategory

// Request argument structure:
//
// same as dlpReadNextRecInCategory

// Response argument structure:
//
// same as dlpReadNextRecInCategory



//////////////////////////////////////////////////////////////////////////
//	dlpReadAppPreference
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory,
//			dlpRespErrParam,
//			dlpRespErrNotFound,

#define dlpReadAppPreferenceReqArgID	dlpFirstArgID

// dlpReadAppPreferenceReqArgID request argument structure:
//
typedef struct DlpReadAppPreferenceReqType {
												// OFFSET
	DWord					creator;			//	0;		application creator type
	Word					id;				// 4;		preference id
	Word					reqBytes;		//	6;		max. number of preference bytes requested;
												//			pass 0xFFFF for actual size
	Byte					flags;			// 8;		command flags: dlpAppPrefReqFlagBackedUp - if set, use backed-up pref db
	Byte					unused;			// 9;		reserved/padding -- set to NUL!
												// TOTAL: 10 bytes;
	} DlpReadAppPreferenceReqType;

#define dlpReadAppPrefActualSize		0xFFFF
#define dlpAppPrefReqFlagBackedUp	0x80

// Response argument structure:
//
#define dlpReadAppPreferenceRespArgID		dlpFirstArgID

typedef struct DlpReadAppPreferenceRespHdrType {
												// OFFSET
	Word					version;			// 0;		version number of the application
	Word					actualSize;		// 2;		actual preference data size
	Word					retBytes;		// 4;		number of preference bytes returned
												// TOTAL: 6 bytes
	} DlpReadAppPreferenceRespHdrType;

typedef struct DlpReadAppPreferenceRespType {
												// OFFSET
	DlpReadAppPreferenceRespHdrType	//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 6 bytes
	Byte					data[1];			// 6;		custom result data -- var size
	} DlpReadAppPreferenceRespType;

	
//////////////////////////////////////////////////////////////////////////
//	dlpWriteAppPreference
//////////////////////////////////////////////////////////////////////////
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrParam,
//			dlpRespErrNotEnoughSpace

// Request argument structure:
//
#define dlpWriteAppPreferenceReqArgID		dlpFirstArgID

typedef struct DlpWriteAppPreferenceReqHdrType {
												// OFFSET
	DWord					creator;			//	0;		application creator type
	Word					id;				// 4;		preference id
	Word					version;			// 6;		version number of the application
	Word					prefSize;		// 8;		preference size(in number of bytes)
	Byte					flags;			// 10;	command flags: dlpAppPrefReqFlagBackedUp - if set, use backed-up pref db
	Byte					unused;			// 11;	reserved/padding -- set to NUL!
												// TOTAL: 12 bytes;
	} DlpWriteAppPreferenceReqHdrType;

typedef struct DlpWriteAppPreferenceReqType {
												// OFFSET
	DlpWriteAppPreferenceReqHdrType	//
							header;			//	0;		fixed-size header
												// FIXED SIZE: 12 bytes;
	Byte					data[1];			// 12;	record data -- var size
	} DlpWriteAppPreferenceReqType;


// Response argument structure:
//
// none.



//////////////////////////////////////////////////////////////////////////
// dlpReadNetSyncInfo
//////////////////////////////////////////////////////////////////////////
//		Request arguments: none
//
//		Response arguments:
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrMemory

// Request argument structure:
//
//		none.
	
// Response argument structure:
//
#define dlpReadNetSyncInfoRespArgID		dlpFirstArgID

typedef struct DlpReadNetSyncInfoRespHdrType {
												// OFFSET
	Byte					lanSyncOn;		//	0;		non-zero if Lan Sync is enabled
	Byte					bReserved1;		// 1;		reserved -- SET TO NULL!
	DWord					dwReserved1;	// 2;		reserved -- SET TO NULL!
	DWord					dwReserved2;	// 6;		reserved -- SET TO NULL!
	DWord					dwReserved3;	// 10;	reserved -- SET TO NULL!
	DWord					dwReserved4;	// 14;	reserved -- SET TO NULL!
	Word					syncPCNameSize;//	18;	length of sync PC host name,
												//			including null (0 = no host name)
	Word					syncPCAddrSize;//	20;	length of sync PC address,
												//			including null (0 = no address)
	Word					syncPCMaskSize;//	22;	length of sync PC subnet mask,
												//			including null (0 = no mask)
												// TOTAL: 24 bytes										
	} DlpReadNetSyncInfoRespHdrType;

typedef struct DlpReadNetSyncInfoRespType {
												// OFFSET
	DlpReadNetSyncInfoRespHdrType		//
							hdr;				//	0;		fixed-size header
												// FIXED SIZE: 24 bytes										
	Byte					syncAddr[1];	// 24;	sync IP address/host name -- var size,
												//			null-terminated
	} DlpReadNetSyncInfoRespType;


	
//////////////////////////////////////////////////////////////////////////
//	dlpWriteNetSyncInfo
//////////////////////////////////////////////////////////////////////////
//		Request arguments:
//
//		Response arguments: none
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrNotEnoughSpace,
//			dlpRespErrParam

// Request argument structure:
//
#define dlpWriteNetSyncInfoReqArgID		dlpFirstArgID

#define dlpMaxHostAddrLength	255	// maximum PC host name length, not including NULL

typedef struct DlpWriteNetSyncInfoReqHdrType {
												// OFFSET
	Byte					modFlags;		// 0;		flags indicating which values are being
												//			modified; see the dlpNetSyncInfoMod...
												//			flags defined below
	Byte					lanSyncOn;		//	1;		non-zero if Lan Sync is enabled
	DWord					dwReserved1;	// 2;		reserved -- SET TO NULL!
	DWord					dwReserved2;	// 6;		reserved -- SET TO NULL!
	DWord					dwReserved3;	// 10;	reserved -- SET TO NULL!
	DWord					dwReserved4;	// 14;	reserved -- SET TO NULL!
	Word					syncPCNameSize;//	18;	length of sync PC host name,
												//			including null (0 = no address/host name)
	Word					syncPCAddrSize;//	20;	length of sync PC address,
												//			including null (0 = no address)
	Word					syncPCMaskSize;//	22;	length of sync PC subnet mask,
												//			including null (0 = no mask)
												// TOTAL: 24 bytes										
	} DlpWriteNetSyncInfoReqHdrType;

// Flags indicating which values are being changed by the dlpWriteNetSyncInfo
// request.  These flags are used in the modFlags field of DlpWriteNetSyncInfoReqHdrType.
// These flags are additive.
//
#define dlpNetSyncInfoModLanSyncOn	0x80	// changing the "lan sync on" setting
#define dlpNetSyncInfoModSyncPCName	0x40	// changing the sync PC host name
#define dlpNetSyncInfoModSyncPCAddr	0x20	// changing the sync PC address
#define dlpNetSyncInfoModSyncPCMask	0x10	// changing the sync PC subnet mask

typedef struct DlpWriteNetSyncInfoReqType {
												// OFFSET
	DlpWriteNetSyncInfoReqHdrType
							hdr;				//	0;		fixed-size header
												// FIXED SIZE: 24 bytes										
	Byte					syncAddr[1];	// 24;	sync IP address/host name -- var size,
												//			null-terminated
	} DlpWriteNetSyncInfoReqType;

	
// Response argument structure:
//
//		none.



//////////////////////////////////////////////////////////////////////////
// dlpReadFeature
//////////////////////////////////////////////////////////////////////////
//		Request arguments:
//
//		Response arguments:
//
//		Possible error codes
//			dlpRespErrSystem,
//			dlpRespErrNotFound,
//			dlpRespErrParam

// Request argument structure:
//
#define dlpReadFeatureReqArgID		dlpFirstArgID

typedef struct DlpReadFeatureReqType {
												// OFFSET
	DWord					dwFtrCreator;	// 0;		feature creator
	Word					wFtrNum;			// 4;		feature number
												// TOTAL: 6 bytes										
	} DlpReadFeatureReqType;
	
	
// Response argument structure:
//
#define dlpReadFeatureRespArgID		dlpFirstArgID

typedef struct DlpReadFeatureRespType {
												// OFFSET
	DWord					dwFeature;		//	0;		feature value
												// TOTAL: 4 bytes
	} DlpReadFeatureRespType;


/************************************************************
 * Macros
 *************************************************************/


#endif // __DLCOMMON_H__


