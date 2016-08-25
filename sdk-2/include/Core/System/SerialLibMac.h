/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SerialLibMac.h
 *
 * Description:
 *		Include file for Mac Serial Library
 *
 * History:
 *   	6/30/95 Created by Vitaly Kruglikov
 *
 *******************************************************************/

#ifndef __SERIAL_LIB_MAC_H
#define __SERIAL_LIB_MAC_H


// Pilot common definitions
#include "Common.h"
#include "SerialMgr.h"

// Maximum size of our receive window. We have to emulate this
//  functionality on the Mac by reading out of the Mac's serial
//  driver into a temporary buffer maintained by the Serial Library
// in the emulator.
#define	serFakeIntQueueSize			0x10


/********************************************************************
 * Serial Globals
 ********************************************************************/
typedef struct SerMacGlobalsType {
	UInt					refNum;				// port reference number
	UInt					openCount;			// number of times we have been opened
	UInt					port;
	SerSettingsType	settings;
	Word					lineErrors;			// line errors bitfield
	DWord					maxBaud;				// maximum supported baud rate
	Boolean				breakIsOn;			// set to true when break signal is started, false when it is stopped
	
	short					inRefNum;			// input driver reference number
	short					outRefNum;			// output driver reference number
	Ptr					inBufP;				// custom input buffer ptr
	
	// THe following fields are used to emulate the SerReceiveWindowOpen
	//  and SerReceiveWindowClose() routines in the Emulator. 
	Boolean				windowOpen;			// non-zero if window currently open
													//   used for error checking.
	Word					windowOffset;		// next available byte in intQueue;
	Word					windowSize;			// # of bytes read into "int queue"
	Byte					intQueue[serFakeIntQueueSize];	// fake Int. queue"
	SerCallbackEntryType
							blockingHook;		// blocking hook callback

	// Receive Queue semaphore info
	SerWakeupHandler		wakeupHandler;		// Wakeup handler, if installed
	DWord					wakeupRefcon;		// refcon for wakeup handler
	Word					smSignalSize;		// How much the semaphore is waiting for
	ULong					emTaskID;			// Task ID of interrupt simulator 
	} SerMacGlobalsType;
typedef SerMacGlobalsType*	SerMacGlobalsPtr;	

#define	serDefaultRcvBufferSize	(30*1024)

Err		SerPrvInstallDispatcher(UInt refNum, SysLibTblEntryPtr entryP);
#endif	//__SERIAL_LIB_MAC_H
