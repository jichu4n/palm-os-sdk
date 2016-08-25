/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		ExgLib.h
 *
 * Description:
 *		Include file the Exchange Library interface. The Exchange Library is a
 *		generic interface to any number of librarys. Any Exchange Library
 *		MUST have entrypoint traps in exactly the order listed here.
 *		The System Exchange manager functions call these functions when 
 *		applications make calls to the Exchange manager. Applications will
 *		usually not make direct calls to this API.
 *
 * History:
 *   	5/23/97 Created by Gavin Peacock
 *
 *******************************************************************/

#ifndef __EXG_LIB_H
#define __EXG_LIB_H

#include <Common.h>
#include <SysTraps.h>
#include <ExgMgr.h>

// special exchange mgr event key
#define exgIntDataChr 0x01ff

//-----------------------------------------------------------------------------
// 	Obx library call ID's. Each library call gets the trap number:
//   exgTrapXXXX which serves as an index into the library's dispatch table.
//   The constant sysLibTrapCustom is the first available trap number after
//   the system predefined library traps Open,Close,Sleep & Wake.
//
// WARNING!!! This order of these traps MUST match the order of the dispatch
//  table in and Exchange library!!!
//-----------------------------------------------------------------------------
#pragma mark Traps
typedef enum {
	exgLibTrapHandleEvent = sysLibTrapCustom,
	exgLibTrapConnect,
	exgLibTrapAccept,
	exgLibTrapDisconnect,
	exgLibTrapPut,
	exgLibTrapGet,
	exgLibTrapSend,
	exgLibTrapReceive,
	exgLibTrapControl,
	exgLibReserved1,
	exgLibTrapLast
	} ExgLibTrapNumberEnum;


/************************************************************
 * Net Library procedures.
 *************************************************************/ 
#pragma mark Functions
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------
// Library initialization, shutdown, sleep and wake
//--------------------------------------------------
// Open the library - enable server for receiving data.
Err		ExgLibOpen (Word libRefnum)
			SYS_TRAP(sysLibTrapOpen);
					
Err		ExgLibClose (Word libRefnum)
			SYS_TRAP(sysLibTrapClose);
					
Err		ExgLibSleep (Word libRefnum)
			SYS_TRAP(sysLibTrapSleep);
					
Err		ExgLibWake (Word libRefnum)
			SYS_TRAP(sysLibTrapWake);
					
//	Handle events that this library needs. This will be called by
//	sysHandle event when certain low level events are triggered.					
Err		ExgLibHandleEvent(Word libRefnum,VoidPtr eventP)
			SYS_TRAP(exgLibTrapHandleEvent);
						
//  Establish a new connection 						
Err	 	ExgLibConnect(UInt libRefNum,ExgSocketPtr exgSocketP)
			SYS_TRAP(exgLibTrapConnect);

// Accept a connection request from remote end
Err		ExgLibAccept(Word libRefnum,ExgSocketPtr exgSocketP)
			SYS_TRAP(exgLibTrapAccept);

// Disconnect
Err		ExgLibDisconnect(Word libRefnum,ExgSocketPtr exgSocketP,Err error)
			SYS_TRAP(exgLibTrapDisconnect);

// Initiate a Put command. This passes the name and other information about
// an object to be sent
Err		ExgLibPut(Word libRefnum,ExgSocketPtr exgSocketP)
			SYS_TRAP(exgLibTrapPut);

// Initiate a Get command. This requests an object from the remote end.
Err		ExgLibGet(UInt libRefNum,ExgSocketPtr exgSocketP)
			SYS_TRAP(exgLibTrapGet);

// Send data to remote end - called after a Put command
ULong 	ExgLibSend(UInt libRefNum, ExgSocketPtr exgSocketP, const void * const bufP, const ULong bufLen, Err *errP)
			SYS_TRAP(exgLibTrapSend);

// Receive data from remote end -- called after Accept
ULong 	ExgLibReceive(UInt libRefNum, ExgSocketPtr exgSocketP, VoidPtr bufP, const ULong bufSize, Err *errP)
			SYS_TRAP(exgLibTrapReceive);

// Send various option commands to the Exg library
Err 	ExgLibControl(UInt libRefNum, Word op, VoidPtr valueP, WordPtr valueLenP)
			SYS_TRAP(exgLibTrapControl);

						
#ifdef __cplusplus
}
#endif

#endif  // EXG_LIB_H
