/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		DebugMgr.h
 *
 * Description:
 *		Debugging functions
 *
 * History:
 *   	11/03/94  RM - Created by Ron Marianetti
 *
 *		2/1/98	vmk  - Defined DbgControl
 *
 *
 *******************************************************************/


#ifndef __DEBUGMGR_H__
#define __DEBUGMGR_H__

// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.


//
// Constants and structures used in conjunction with DbgControl
//
#define dbgCtlNotHandled		false			// debug control operation was not handled
#define dbgCtlHandled			true			// debug control operation was handled

#define dbgCtlAllHandlersID		0				// indiacates that the operation is to be handled
												// by all handlers

#define dbgCtlHandlerNameLen	31				// maximum handler name length, not including null
#define dbgCtlHandlerVerLen		15				// maximum handler version string length, not including null

#define dbgCtlFirstCustomOp		0x8000			// debug handlers begin numbering their op[erations]
												// with this number; the system reserves all operation
												// number values below this one
// Typed of the DbgControl function
typedef Boolean DbgControlFuncType(DWord handlerID, Word op, VoidPtr paramP, DWordPtr dwParamP);

typedef struct DbgCtlHandlerInfoType {
	DbgControlFuncType*	handlerFuncP;			// ptr to handler's DbgControl function
	DWord	version;							// numeric version number (hander-defined)
	Boolean	enabled;							// true if handler is enabled; false if not
	Char	name[dbgCtlHandlerNameLen+1];		// null-terminated handler name
	Char	ver[dbgCtlHandlerVerLen+1];			// null-terminated handler version string
	DWord	dwReserved;							// RESERVED -- CALLER MUST INITIALIZE TO ZERO!
	} DbgCtlHandlerInfoType;

typedef void DbgCtlEnumCallbackFunc(VoidPtr callbackDataP, DWord handlerID, DbgControlFuncType*	handlerFuncP);

typedef struct DbgCtlEnumInfoType {
	DbgCtlEnumCallbackFunc*	enumFuncP;
	VoidPtr					callbackDataP;
	} DbgCtlEnumInfoType;

// System-defined debug control operations
enum {
	
	dbgCtlOpEnumHandlers = 1,					// handlerID = dbgCtlAllHandlersID (applies to all handlers)
												// paramP = ptr to DbgCtlEnumInfoType
												// returns dbgCtlHandled if handled
												
	dbgCtlOpGetHandlerInfo,						// handlerID = desired handler creator
												// paramP = ptr to DbgCtlHandlerInfoType
												// returns dbgCtlHandled if handled

	dbgCtlOpEnableHandler,						// handlerID = desired handler creator or dbgCtlAllHandlersID
												// returns dbgCtlHandled if handled

	dbgCtlOpDisableHandler,						// handlerID = desired handler creator or dbgCtlAllHandlersID
												// returns dbgCtlHandled if handled

	dbgCtlOpGetEnabledStatus,					// handlerID = desired handler creator
												// dwParamP = ptr to DWord type variable to be filled in with
												// enabled status: non-zero = enabled, zero = disabled
												// returns dbgCtlHandled if handled

	dbgCtlOpGetVersion,							// handlerID = desired handler creator
												// dwParamP = ptr to DWord type variable to be filled in with
												// handler-specific version number
												// returns dbgCtlHandled if handled
												
												
	dbgCtlOpLAST
	};

#ifdef __GNUC__
#define _DEBUGGER_TRAP  __attribute__ ((__callseq__ ("trap #8")))
#elif defined (__MWERKS__) /* The equivalent in CodeWarrior syntax */
#define _DEBUGGER_TRAP  = 0x4E40 + 8
#endif


/************************************************************
 * Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Long 			DbgInit(Ptr spaceP, Ptr dispatchTableP[], Boolean openComm);

void 			DbgSrcBreak(void)
#if USE_TRAPS
							_DEBUGGER_TRAP
#endif
;
 
void 			DbgSrcMessage(CharPtr debugStr)
							SYS_TRAP(sysTrapDbgSrcMessage);


// Debug control function for implementing debug tracing, etc. via debug handlers.
// The default implementation does nothing, leaving the real work up to "debug handlers".
// Debug handlers will be implemented as system extensions.  As they are loaded, extensions
// will override this function call (DbgControl) and chain to those handlers loaded before
// them.  When a debug control call is made by the client, a handler id of the handler that
// implements the functionality will be passed in as the first parameter.  The handler id is
// the unique creator id of the handler.  When a handler is called, it will first examine
// the handler ID -- if it matches its own, the handler will execute the command and return;
// if the handler id does not match, the handler must pass the call down the chain and return
// the value from that call.  The operation to be performed is indicated by the parameter "op".
// op is specific to each handler this means that the same op values may be used by different
// handlers, since handler id's are unique. op values defined by handlers must begin at
// dbgCtlFirstCustomOp.  Handler ID of dbgCtlAllHandlersID applies to all handlers.  When the
// handler id of zero is passed, each handler is responsible for executing the requested action
// and passing the call down the chain.  The last two parameters are defined by each handler
// for its own operations.
//typedef Boolean DbgControlFuncType(DWord handlerID, Word op, VoidPtr paramP, DWordPtr dwParamP);
extern DbgControlFuncType DbgControl
							SYS_TRAP(sysTrapDbgControl);



// NOTE!!! DbgBreak can NOT be written using the ONEWORD_INLINE macro
//  because it must always resolve to a trap instruction even when
//  USE_TRAPS is set to 0. In particular the SmallROM compiles with
//  USE_TRAPS set to 0 and needs the DbgBreak to resolve as a trap
//  instruction. 
#if EMULATION_LEVEL == EMULATION_NONE
	void 		DbgBreak(void) _DEBUGGER_TRAP;
#else
	void 		DbgBreak(void);
#endif

void 			DbgMessage(CharPtr aStr)
							SYS_TRAP(sysTrapDbgMessage);
	
CharPtr 		DbgGetMessage(BytePtr bufferP, Long timeout)
							SYS_TRAP(sysTrapDbgGetMessage);
							
Err			DbgCommSettings(ULongPtr baudP, ULongPtr flagsP)
							SYS_TRAP(sysTrapDbgCommSettings);
	
#ifdef __cplusplus 
}
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_DbgBreak		\
				DC.W	m68kTrapInstr+sysDbgTrapNum		//lint !e773
				


#endif // __DEBUGMGR_H__


