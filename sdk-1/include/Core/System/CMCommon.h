/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		CMCommon.h
 *
 * Description:
 *		Connection Management Protocol common definitions.
 *
 * History:
 *   	9/14/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __CMCOMMON_H__
#define __CMCOMMON_H__

#if EMULATION_LEVEL == 1		// EMULATION_WINDOWS
	
	// Pilot common definitions
	#include "Common.h"
	
#else	// EMULATION_LEVEL != EMULATION_WINDOWS
	
	// Pilot common definitions
	#include <Common.h>
	
#endif	// EMULATION_LEVEL == EMULATION_WINDOWS

#if EMULATION_WINDOWS != 1
	LOGIC ERROR
#endif



/********************************************************************
 * CMP Packet Format
 ********************************************************************/

// The CMP packets are used for transmitting data for the Pilot CMP protocol layer.
// The structure of the CMP packet is built into the data part of the SLP packet and is,
// therefore, constrained by the SLP packet data size limitations.


// Packet type values used in the CMP packet body "type" field:
//
typedef enum CmpType {
	cmpWakeup	= 1,			// CMP Wakeup packet sent from server to client
	cmpInit,						// CMP Init packet sent from client to server
	cmpAbort						// CMP Abort packet sent from client to server
	} CmpType;


// cmpInit and cmpWakeup packet body structure:
//
typedef struct CmpBodyType {
	Byte	type;								// CMP packet type

	Byte	flags;							// in cmpInit packets: indicates which values were adjusted by
												// the client(defined below); in cmpWakeup pakcets: null;
												// in cmpAbort packets: reason for abort.

	DWord	commVersion;					// in cmpWakeup: server comm software version number;
												// in cmpInit: client comm version

	DWord	baudRate;						// in cmpWakeup: max server baud rate;
												// in cmpInit packets: the prescribed baud rate
	} CmpBodyType;
	
typedef CmpBodyType*		CmpBodyPtr;

// Flags used in the packet body "flags" field of cmpInit
// packets.  These values are additive in the sense that they can be
// added or or'ed to form a composite value:
#define cmpInitFlagChangeBaudRate	0x80		// signals that the baud rate was adjusted by the client


// Flags used in the packet body "flags" field of cmpAbort packets.
// These values are additive in the sense that they can be
// added or or'ed to form a composite value:
#define cmpAbortFlagVersionError		0x80		// signals incompatible comm versions



/********************************************************************
 * CMP Protocol Parameters
 ********************************************************************/

// This transaction id is used by cmpWakeup packets.  cmpInit and cmpAbort
// packets MUST use a different transaction id.
//
#define cmpWakeupTransactionID		0xFF

// This is the baud rate at which CMP connections are initiated
//
#define cmpInitialBaudRate				9600L

// This is the time limit for trying to initiate a connection
//
#define cmpMaxInitiateSec				20


#endif	// __CMCOMMON_H__
