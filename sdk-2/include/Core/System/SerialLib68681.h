/*******************************************************************
 * 							 Touchdown Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SerialLib68328.h
 *
 * Description:
 *		Include file for 68328 Serial Library
 *
 * History:
 *   	2/7/95 Created by Ron Marianetti
 *		vmk	8/21/95	Added defQData field to serial globals
 *
 *******************************************************************/

#ifndef __SERIAL_LIB_68681_H
#define __SERIAL_LIB_68681_H


// Touchdown common definitions
#include <SerialMgr.h>

/********************************************************************
 * Debug Info stored in serial globals 
 ********************************************************************/
typedef struct Ser681DebugInfoType {
		Word					lineErrorCount;	// number of line errors since opening

	#ifdef INCLUDE_BASIC_681_DEBUG_INFO
		Word					numReceived;		// number of bytes received since opening
		Word					numSent;				// number of bytes sent
		Byte					lastReceived;		// last byte received
		Byte					lastErrorStatus;	// receiver status of last line error
		
		// Include extensive debug info only if basic info is included
		#ifdef INCLUDE_EXTENSIVE_681_DEBUG_INFO
			Word					hwOverrunCount;	// number of hardware overrun errors since opening
			Word					parityErrorCount;	// number of parity errors since opening
			Word					framingErrorCount;// number of framing errors since opening
		#endif	//#ifdef INCLUDE_EXTENSIVE_681_DEBUG_INFO
	#endif	//#ifdef INCLUDE_BASIC_681_DEBUG_INFO

	} Ser681DebugInfoType;
	
typedef Ser681DebugInfoType*	Ser681DebugInfoPtr;



/********************************************************************
 * Serial Globals 
 ********************************************************************/
typedef struct Ser681GlobalsType {
	DWord					openSignature;		// hack to detect if we have been opened when
													// booting from the 68328-based debugger
	UInt					openCount;			// # of times we've been opened
	Int					port;
	SerSettingsType	settings;
	Word					lineErrors;			// line errors bitfield
	

	VoidPtr				chanP;				// pointer to this channel base in UART
	
#ifndef BUILDING_FOR_DEBUGGER
	// Receive Queue. When the queue is empty, qStart == qEnd. When the queue
	//  is full, qStart = qEnd-1 (taking wrap-around into account if necessary).
	Word					qStart;				// start offset of queue
	Word					qEnd;					// end offset of queue
	Word					qSize;				// size of queue
	Ptr					qData;				// pointer to queue data
	Ptr					defQData;			// pointer to default queue data
	
	// Receive Queue semaphore info
	Word					smSignalSize;		// How much the semaphore is waiting for
	DWord					smID;					// semaphore ID 

	DWord					lastRcvTicks;		// tick count of last received char

	Byte					currentIMR;			// current value of interrupt mask register
	Ptr					oldIRQ3P;			// saved pointer to previously-installed IRQ3 handler

	Boolean				breakIsOn;			// set to true when break signal is started, false when it is stopped
#endif	//#ifndef BUILDING_FOR_DEBUGGER

	// Serial Library debugging information
	Ser681DebugInfoType	debugInfo;
	
	} Ser681GlobalsType;

typedef Ser681GlobalsType*	Ser681GlobalsPtr;

#define	serLibDefaultRcvBufferSize	0x200

#endif	//__SERIAL_LIB_68681_H
