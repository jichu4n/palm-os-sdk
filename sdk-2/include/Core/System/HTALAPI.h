/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		HTALAPI.h
 *
 * Description:
 *		HotSync Transport Adaptation Layer(HTAL) library API.
 *
 * History:
 *   	7/3/96	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __HTAL_API_H__
#define __HTAL_API_H__

// Pilot common definitions
#include <Common.h>
#include <SystemMgr.rh>


/********************************************************************
 * Type and creator of HTAL Library databases
 ********************************************************************/
#pragma mark -Creator and Type-

// Creator. Used for both the database that contains the HTAL Library and
//  it's preferences database.
#define		htalCreatorPAD			sysFileCPADHtal		// Our PAD HTAL Library creator
#define		htalCreatorTCP			sysFileCTCPHtal		// Our TCP HTAL Library creator

// Types. Used to identify the HTAL Library from it's prefs.
#define		htalLibType				sysFileTHtalLib		// Our HTAL Code Resources Database type
#define		htalPrefsType			'rsrc'		// Our HTAL Preferences Database type


/************************************************************
 * HTAL result codes
 * (htalErrorClass is defined in SystemMgr.h)
 *************************************************************/
#pragma mark -Result Codes-

#define htalErrTimeOut			(htalErrorClass | 1)		// call timed out(INTERNAL ERROR CODE)
#define htalErrParam				(htalErrorClass | 2)		// invalid parameter
#define htalErrInUse				(htalErrorClass | 3)		// HTAL library is already in use
#define htalErrMemory			(htalErrorClass | 4)		// not enough memory
#define htalErrUserCan			(htalErrorClass | 5)		// cancelled by user
#define htalErrSizeErr			(htalErrorClass | 6)		// reply block is too big
#define htalErrNotConnected	(htalErrorClass | 7)		// lost/could not establish connection
#define htalErrNotOpen			(htalErrorClass | 8)		// library is not open
#define htalErrInterfaceBusy	(htalErrorClass | 9)		// comm interface(serial port, TCP/IP, etc.) is busy
#define htalErrCommSetup		(htalErrorClass | 10)	// comm setup error

#define htalErrRemoteConnect	(htalErrorClass | 11)	// could not establish remote connection and error
																		// was already reported to the user(ie., modem dial
																		// failed, etc.)

#define htalErrCommVersion		(htalErrorClass | 12)	// comm version is incompatible with the desktop

#define htalErrRmtMemory		(htalErrorClass | 13)	// not enough memory on remote device

#define htalErrDestHostName	(htalErrorClass | 14)	// invalid destination host name, or host name not
																		// found on the network

#define htalErrGotTickle		(htalErrorClass | 15)	// INTERNAL error code indicating that a keep-alive
																		// tickle packet was received -- this error code
																		// should never be returned to the client


#define htalErrExtLoopback		(htalErrorClass | 16)	// External loopback was detected -- this error code
																		// is returned when an HTAL library detects that a
																		// loopback test is being performed.


//-----------------------------------------------------------------------------
// HTAL library call ID's. Each library call gets the trap number:
//   htalLibTrapXXXX which serves as an index into the library's dispatch table.
//   The constant sysLibTrapCustom is the first available trap number after
//   the system predefined library traps Open,Close,Sleep & Wake.
//
// WARNING!!! The order of these traps MUST match the order of the dispatch
//  table in xxxHTALDispatch.c (where xxx is PAD, TCP, etc.)!!!
//-----------------------------------------------------------------------------
#pragma mark -Traps-

typedef enum {
	htalLibTrapSetOption = sysLibTrapCustom,
	htalLibTrapGetOption,
	htalLibTrapControl,
	htalLibTrapConnectPhase1,
	htalLibTrapDisconnect,
	htalLibTrapGetCommand,
	htalLibTrapSendReply,
	htalLibTrapCancel,
	
	// New Traps, re-order these later..
	htalLibTrapMaxBlockSize,
	htalLibTrapVersion,
	htalLibTrapAddSegmentSizes,
	htalLibTrapConnectPhase2,
	
	htalLibTrapLast
	} HtalLibTrapNumberEnum;



/********************************************************************
 * HTAL Session Structures
 ********************************************************************/
#pragma mark -Session Structures-

 
//-----------------------------------------------------------------------------
// Option id's used by HtalLib/Set/Get/Option
//-----------------------------------------------------------------------------
#pragma mark ---Option Enums-

typedef enum HtalOptEnum {
	htalOptFirst = 0,
	
	htalOptBlockingHook,		// param1P = ptr to callback func ptr; param2P = ptr to user ref value(DWord)
	
	htalOptEventHook,			// param1P = ptr to callback func ptr; param2P = ptr to user ref value(DWord)
	
	htalOptMemAllocHook,		// param1P = ptr to callback func ptr; param2P = ptr to user ref value(DWord)
	
	htalOptCmdWaitTimeOut,	// param1P = ptr to command timeout seconds(Int) -1 = forever; param2P = nil
	
	htalOptConnMgmtEnabled,	// param1P = ptr to Boolean(true = enabled,false=disabled); param2P = nil
	
	htalOptKeepAliveEnabled,// param1P = ptr to Boolean(true = enabled,false=disabled); param2P = nil
									// keep-alive is ON by default

	// Add option id's here
	htalOptMakeAsyncReqCallback,// param1P = ptr to callback func ptr; param2P = ptr to user ref value(DWord)
	

	
	htalOptLast
	
} HtalOptEnum;

#define htalOptFirstCustom		0xA800

 
//-----------------------------------------------------------------------------
// Control id's used by HtalLibControl
//-----------------------------------------------------------------------------
#pragma mark ---Control Enums-

typedef enum HtalCtlEnum {
	htalCtlFirst = 0,
	
	htalCtlGetServiceList,	// Get a list of supported services
									// paramP		-- ptr to array of HtalServiceEntryType (can be
									//						null if *(UIntPtr)paramLenP is 0)
									// *paramLenP	-- number of elements in array (if *paramLenP == 0,
									//						number of supported services is returned here)
	
	htalCtlPrintDebugInfo,	// Print debug info to the Pilot's display
									// paramP		-- on entry, ptr to Word variable containing the vertical
									//						display offset in # of pixels where printing is to begin;
									//						on return, the variable is filled in with the vertical pixel
									//						offset where printing may continue.
									// *paramLenP	-- set to sizeof(Word);

	
	htalCtlLAST					// *KEEP THIS ENTRY LAST*
	
} HtalCtlEnum;

#define htalCtlFirstCustom		0xA800

 
//-----------------------------------------------------------------------------
// Temporary data structures which will need to be defined elsewhere
//-----------------------------------------------------------------------------
#pragma mark ---Temporary Data Structures-

// parameter for sysDialLaunchCmdDial
typedef struct DialLaunchCmdDialType {
	DWord			svcID;						// service id
	Boolean		dontDisplayUI;				// if true, the dialer must avoid UI of any kind
	Byte			bReserved;					// reserved -- SET TO NULL!
	UInt			serRefNum;					// serial library reference number
	Err			modemError;					// error code returned from Modem Manager
	DWord			dwReserved;					// reserved -- SET TO NULL!
	} DialLaunchCmdDialType;
typedef DialLaunchCmdDialType*		DialLaunchCmdDialPtr;

#define dialErrPhoneSetup		(appErrorClass | 1)			// phone setup is not complete
#define dialErrParam				(appErrorClass | 2)			// invalid parameter
#define dialErrNotSupported	(appErrorClass | 3)			// the requested dial option is not supported
#define dialErrMemory			(appErrorClass | 4)			// insufficient memory
#define dialErrModem				(appErrorClass | 5)			// when this error is returned, look in the mdmError field
																			// for the cause of error
#define dialErrUserCan			(appErrorClass | 6)			// user cancelled



//-----------------------------------------------------------------------------
// The HtalDataSegmentType structure enables the caller to
// specify the block's data in multiple non-contiguous segments.  For example, when
// sending a record, DesktopLink will specify the function header, the argument wrapper,
// the fixed record info, and the record data as two or more segments(one or more segments
// for the fixed structures and one segment for the record data).  In such an instance,
// it is not practical(and sometimes not possible) to allocate a single data buffer to
// contain all that information contiguously.
//-----------------------------------------------------------------------------
#pragma mark ---Data Segment Descriptor-

typedef struct HtalDataSegmentType {
	VoidPtr		dataP;				// ptr to segment data
	DWord			dwDataSize;			// segment data size in bytes
	Boolean		wordAlign;			// if true, this segments data will be sent
											// at an even offset from the start of the
											// block, padding the previous segment's
											// data with a null byte, if necessary
	} HtalDataSegmentType;

typedef HtalDataSegmentType*		HtalDataSegmentPtr;



//-----------------------------------------------------------------------------
// HTAL callback function prototypes
//-----------------------------------------------------------------------------
#pragma mark ---Callback Prototypes-


// Prototype for the "blocking hook"  callback function
typedef void HtalBlockingCallbackType(DWord userRef);
typedef HtalBlockingCallbackType* HtalBlockingCallbackPtr;


// Prototype for the "event notification"  callback function
typedef enum HtalEventType {
	htalEventDialing = 1,				// dataP = ptr to dial string
	htalEventWaitingForCallback,
	htalEventEstablishingConnection,
	htalEventDisconnecting,
	htalEventAborting
	} HtalEventType;
typedef ULong HtalEventCallbackType(DWord userRef, HtalEventType eventType, VoidPtr dataP);
typedef HtalEventCallbackType* HtalEventCallbackPtr;


// Prototype for the "memory allocation"  callback function
typedef enum HtalMemAllocType {
	htalMemTypeNone = 0,				// command buffer could not be allocated
	htalMemTypeInternalPointer,	// pointer to an internal buffer(client must not attempt to free this buffer)
	htalMemTypeDynamicHeapHandle,	// client-allocated dynamic heap handle(client responsible for freeing)
	htalMemTypeDataHeapHandle,		// client-allocated data heap handle(client responsible for freeing)
	htalMemTypeLast
	} HtalMemAllocType;
typedef DWord HtalMemAllocCallbackType(DWord userRef, ULong size, HtalMemAllocType* memTypeP);
typedef HtalMemAllocCallbackType* HtalMemAllocCallbackPtr;


// Prototypes for the "request asynchronous callback" function
// THIS FUNCTION IS CALLED FROM A TIMER PROC OR FROM ANOTHER THREAD.
// IT MUST NOT ALLOCATE MEMORY OR CONSUME AN EXCESSIVE AMOUNT OF TIME.
// WHEN CALLED FROM A TIMER PROC, IT MUST NOT BLOCK!!!
//
// NULL reqDataP cancels the request.
// Returns: 0 on success
typedef Err HtalAsyncReqHandlerType(UInt refNum, VoidPtr reqDataP);
typedef HtalAsyncReqHandlerType*	HtalAsyncReqHandlerPtr;

typedef struct HtalAsyncReqParamType {
	Boolean			fromTimer;				// true if called from timer proc
	DWord				userRef;					// user reference value
	UInt				htalRefNum;				// HTAL lib reference number
	VoidPtr			reqDataP;				// request data pointer (NULL to cancel request)
	HtalAsyncReqHandlerPtr
						handlerP;				// ptr to request handler function
	} HtalAsyncReqParamType;
	
typedef HtalAsyncReqParamType*	HtalAsyncReqParamPtr;

// Returns true if handled
typedef Boolean HtalMakeAsyncReqCallbackType(HtalAsyncReqParamPtr paramP);
typedef HtalMakeAsyncReqCallbackType* HtalMakeAsyncReqCallbackPtr;



#pragma mark ---Function Parameter Structures-

//-----------------------------------------------------------------------------
// HtalLibConnectPhase1 function parameter
//-----------------------------------------------------------------------------

//#define htalTempDirectCableConnection	1
//#define htalTempDirectModemConnection	2

typedef struct HtalConnectParamType {
	DWord				svcOwnCreator;		// creator of the module which owns the connection id
	DWord				svcOwnType;			// type of the module which owns the connection id
	DWord				svcID;				// connection id
	//CharPtr			tempPhoneNumP;		// phone num string(temporary - will be part of connection template)
	DWord				miscFlags;			// miscellaneous flags(to be defined) -- SET UNUSED BITS TO ZERO!
	DWord				dwReserved1;		// reserved -- SET TO NULL!
	} HtalConnectParamType;
typedef HtalConnectParamType*		HtalConnectParamPtr;



//-----------------------------------------------------------------------------
// HtalLibGetCommand function parameter
//-----------------------------------------------------------------------------
	
typedef struct HtalGetCommandParamType {
	Word	tid;							// transaction id of command returned here
	ULong	cmdLen;						// length of received command returned here
	DWord	cmdBufRef;					// reference to command buffer returned here(actual
											// data type depends on cmdBufType)
	HtalMemAllocType	cmdBufType;	// command buffer type returned here
	} HtalGetCommandParamType;
	
typedef HtalGetCommandParamType*	HtalGetCommandParamPtr;



//-----------------------------------------------------------------------------
// HtalLibSendReply function parameter
//-----------------------------------------------------------------------------
typedef struct HtalSendReplyParamType {
	Word				tid;				// -> transaction id of reply
	HtalDataSegmentPtr	segP;		// -> ptr to HTAL data segment array
	Int				segCount;		// -> segment count
	} HtalSendReplyParamType;
	
typedef HtalSendReplyParamType*	HtalSendReplyParamPtr;



//-----------------------------------------------------------------------------
// HtalLibControl function parameters
//-----------------------------------------------------------------------------
typedef struct HtalServiceEntryType {
	ULong		type;								// service type
	ULong		creator;							// service creator
	DWord		dwReserved;						// RESERVED -- SET TO NULL!
	} HtalServiceEntryType;
typedef HtalServiceEntryType*		HtalServiceEntryPtr;



#pragma mark -API Prototypes-

#ifdef __cplusplus
extern "C" {
#endif


//--------------------------------------------------
// Library initialization, shutdown, sleep and wake
//--------------------------------------------------

typedef Err HtalLibOpenFunc(UInt refNum);
extern HtalLibOpenFunc HtalLibOpen
				SYS_TRAP(sysLibTrapOpen);
				
typedef Err HtalLibCloseFunc(UInt refNum);
extern HtalLibCloseFunc HtalLibClose
				SYS_TRAP(sysLibTrapClose);

typedef Err HtalLibSleepFunc(UInt refNum);
extern HtalLibSleepFunc HtalLibSleep
				SYS_TRAP(sysLibTrapSleep);

typedef Err HtalLibWakeFunc(UInt refNum);
extern HtalLibWakeFunc HtalLibWake
				SYS_TRAP(sysLibTrapWake);

//--------------------------------------------------
// Custom routines
//--------------------------------------------------
	
// Set an option
typedef Err HtalLibSetOptionFunc(UInt refNum, HtalOptEnum optionID, VoidPtr param1P, VoidPtr param2P);
extern HtalLibSetOptionFunc HtalLibSetOption
				SYS_TRAP(htalLibTrapSetOption);
				
// Get an option value
typedef Err HtalLibGetOptionFunc(UInt refNum, HtalOptEnum optionID, VoidPtr param1P, VoidPtr param2P);
extern HtalLibGetOptionFunc HtalLibGetOption
				SYS_TRAP(htalLibTrapGetOption);
				
// Control
typedef Err HtalLibControlFunc(UInt refNum, HtalCtlEnum ctl, VoidPtr paramP, WordPtr paramLenP);
extern HtalLibControlFunc HtalLibControl
				SYS_TRAP(htalLibTrapControl);
				
// Phase 1 of connection establishment potentially involving UI
typedef Err HtalLibConnectPhase1Func(UInt refNum, HtalConnectParamPtr paramP);
extern HtalLibConnectPhase1Func HtalLibConnectPhase1
				SYS_TRAP(htalLibTrapConnectPhase1);
				
// Phase 2 of connection establishment not involving UI
typedef Err HtalLibConnectPhase2Func(UInt refNum);
extern HtalLibConnectPhase2Func HtalLibConnectPhase2
				SYS_TRAP(htalLibTrapConnectPhase2);
				
// Shut down the connection
typedef Err HtalLibDisconnectFunc(UInt refNum, Boolean abort);
extern HtalLibDisconnectFunc HtalLibDisconnect
				SYS_TRAP(htalLibTrapDisconnect);
				
// Receive the next command block
typedef Err HtalLibGetCommandFunc(UInt refNum, HtalGetCommandParamPtr paramP);
extern HtalLibGetCommandFunc HtalLibGetCommand
				SYS_TRAP(htalLibTrapGetCommand);
				
// Send a reply block
typedef Err HtalLibSendReplyFunc(UInt refNum, HtalSendReplyParamPtr paramP);
extern HtalLibSendReplyFunc HtalLibSendReply
				SYS_TRAP(htalLibTrapSendReply);
				
// Cancel the current blocking operation (can be called from blocking hook)
typedef Err HtalLibCancelFunc(UInt refNum);
extern HtalLibCancelFunc HtalLibCancel
				SYS_TRAP(htalLibTrapCancel);

//
// HTAL LIB UTILITY ROUTINES
//
				
// Get the maximum block size which can be sent in one transaction
typedef ULong HtalLibMaxBlockSizeFunc(UInt refNum);
extern HtalLibMaxBlockSizeFunc HtalLibMaxBlockSize
				SYS_TRAP(htalLibTrapMaxBlockSize);

// Get HTAL library version
typedef DWord HtalLibVersionFunc(UInt refNum);
extern HtalLibVersionFunc HtalLibVersion
				SYS_TRAP(htalLibTrapVersion);
				
// Total the sizes of HTAL data segments
typedef ULong HtalLibAddSegmentSizesFunc(UInt refNum, Int segCount, HtalDataSegmentType seg[]);
extern HtalLibAddSegmentSizesFunc HtalLibAddSegmentSizes
				SYS_TRAP(htalLibTrapAddSegmentSizes);


#ifdef __cplusplus 
}
#endif


/********************************************************************
 * HTAL Macros
 ********************************************************************/
#pragma mark -Macros-




#endif	// __HTAL_API_H__
