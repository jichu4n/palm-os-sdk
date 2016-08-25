/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		ExgMgr.h
 *
 * Description:
 *		Include file for Exg system functions
 *
 * History:
 *   	5/23/97 Created by Gavin Peacock
 *
 *******************************************************************/

#ifndef __EXGMGR_H
#define __EXGMGR_H

#ifndef __COMMON_H__
#include <Common.h>
#endif

#include <SysTraps.h>

#define exgMemError	 			(exgErrorClass | 1)
#define exgErrStackInit 		(exgErrorClass | 2)  // stack could not initialize
#define exgErrUserCancel 		(exgErrorClass | 3)
#define exgErrNoReceiver 		(exgErrorClass | 4)	// receiver device not found
#define exgErrNoKnownTarget		(exgErrorClass | 5)	// can't find a target app
#define exgErrTargetMissing		(exgErrorClass | 6)  // target app is known but missing
#define exgErrNotAllowed		(exgErrorClass | 7)  // operation not allowed
#define exgErrBadData			(exgErrorClass | 8)  // internal data was not valid
#define exgErrAppError			(exgErrorClass | 9)  // generic application error
#define exgErrUnknown			(exgErrorClass | 10) // unknown general error
#define exgErrDeviceFull		(exgErrorClass | 11) // device is full
#define exgErrDisconnected		(exgErrorClass | 12) // link disconnected
#define exgErrNotFound			(exgErrorClass | 13) // requested object not found
#define exgErrBadParam			(exgErrorClass | 14) // bad parameter to call
#define exgErrNotSupported		(exgErrorClass | 15) // operation not supported by this library
#define exgErrDeviceBusy		(exgErrorClass | 16) // device is busy
#define exgErrBadLibrary		(exgErrorClass | 17) // bad or missing ExgLibrary


typedef struct {
	Word				dbCardNo;			// card number of the database	
	LocalID				dbID;				// LocalID of the database
	Word 				recordNum;			// index of record that contain a match
	DWord				uniqueID;			// postion in record of the match.
	DWord				matchCustom;		// application specific info
} ExgGoToType;	

typedef ExgGoToType * ExgGoToPtr;


typedef struct ExgSocketType {
	Word	libraryRef;	// identifies the Exg library in use
	ULong 	socketRef;	// used by Exg library to identify this connection
	ULong 	target;		// Creator ID of application this is sent to
	ULong	count;		// # of objects in this connection (usually 1)
	ULong	length;		// # total byte count for all objects being sent (optional)
	ULong	time;		// last modified time of object (optional)
	DWord	appData;	// application specific info
	ULong 	goToCreator; // creator ID of app to launch with goto after receive
	ExgGoToType goToParams;	// If launchCreator then this contains goto find info
	Word	localMode:1; // Exchange with local machine only mode 
	Word	packetMode:1; // Use connectionless packet mode (Ultra)
	Word	reserved:14;  // reserved system flags
	CharPtr description;// text description of object (for user)
	CharPtr type;		// Mime type of object (optional)
	CharPtr name;		// name of object, generally a file name (optional)
} ExgSocketType;
typedef ExgSocketType* ExgSocketPtr;


// structures used for sysAppLaunchCmdExgAskUser launch code parameter
// default is exgAskDialog (ask user with dialog...
typedef enum { exgAskDialog,exgAskOk,exgAskCancel } ExgAskResultType;

typedef struct {
	ExgSocketPtr		socketP;
	ExgAskResultType	result;			// what to do with dialog	
} ExgAskParamType;	
typedef ExgAskParamType * ExgAskParamPtr;

#define exgSeparatorChar '\t'		// char used to separate multiple registry entries

#define exgRegLibraryID 0xfffc					// library register thier presence
#define exgRegExtensionID 0xfffd				// filename extenstion registry
#define exgRegTypeID 0xfffe						// MIME type registry
	
#define exgDataPrefVersion 0
#define exgMaxTitleLen     20					// max size for title from exgLibCtlGetTitle

#define exgLibCtlGetTitle	1					// get title for Exg dialogs
#define exgLibCtlSpecificOp 0x8000				// start of range for library specific control codes


typedef Err	(*ExgDBReadProcPtr) 
				(void* dataP, ULong* sizeP, void* userDataP);

typedef Boolean	(*ExgDBDeleteProcPtr)
				(const char* nameP, Word version, Int cardNo,
				LocalID dbID, void* userDataP);

typedef Err	(*ExgDBWriteProcPtr)
				(const void* dataP, ULong* sizeP, void* userDataP);


Err ExgInit(void)  
		SYS_TRAP(sysTrapExgInit);

Err ExgConnect(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgConnect);

Err ExgPut(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgPut);

Err ExgGet(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgGet);

Err ExgAccept(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgAccept);

Err ExgDisconnect(ExgSocketPtr socketP,Err error)
		SYS_TRAP(sysTrapExgDisconnect);

ULong ExgSend(ExgSocketPtr socketP, const void * const bufP, const ULong bufLen,Err* err)
		SYS_TRAP(sysTrapExgSend);

ULong ExgReceive(ExgSocketPtr socketP, VoidPtr bufP, const ULong bufLen, Err* err)
		SYS_TRAP(sysTrapExgReceive);

Err ExgRegisterData(const DWord creatorID, const Word id,const Char * const dataTypesP)
		SYS_TRAP(sysTrapExgRegisterData);

Err ExgNotifyReceive(ExgSocketPtr socketP)
		SYS_TRAP(sysTrapExgNotifyReceive);


Err	ExgDBRead(
		ExgDBReadProcPtr		readProcP,
		ExgDBDeleteProcPtr		deleteProcP,
		void*					userDataP,
		LocalID*				dbIDP,
		Int						cardNo,
		Boolean*				needResetP,
		Boolean					keepDates)
		SYS_TRAP(sysTrapExgDBRead);

Err	ExgDBWrite(
		ExgDBWriteProcPtr		writeProcP,
		void*					userDataP,
		const char*				nameP,
		LocalID					dbID,
		Int						cardNo)
		SYS_TRAP(sysTrapExgDBWrite);



#endif  // __EXGMGR_H
