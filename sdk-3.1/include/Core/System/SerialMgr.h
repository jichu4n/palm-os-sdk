/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		SerialMgr.h
 *
 * Description:
 *		Include file for Serial manager
 *
 * History:
 *   	2/7/95 Created by Ron Marianetti
 *		7/6/95	vmk	added serDefaultSettings
 *		1/28/98	scl	added Serial Port Definitions
 *
 *******************************************************************/

#ifndef __SERIAL_MGR_H
#define __SERIAL_MGR_H

// Pilot common definitions
#include <Common.h>
#include <SystemMgr.h>


/********************************************************************
 * Serial Manager Errors
 * the constant serErrorClass is defined in SystemMgr.h
 ********************************************************************/
#pragma mark -Error Codes-

#define	serErrBadParam				(serErrorClass | 1)
#define	serErrBadPort				(serErrorClass | 2)
#define	serErrNoMem					(serErrorClass | 3)
#define	serErrBadConnID			(serErrorClass | 4)
#define	serErrTimeOut				(serErrorClass | 5)
#define	serErrLineErr				(serErrorClass | 6)
#define	serErrAlreadyOpen			(serErrorClass | 7)
#define  serErrStillOpen			(serErrorClass | 8)
#define	serErrNotOpen				(serErrorClass | 9)
#define	serErrNotSupported		(serErrorClass | 10)		// functionality not supported


/********************************************************************
 * Serial Port Definitions
 ********************************************************************/
#pragma mark -Port Definitions-

#define	serPortDefault				0x0000	// Use prefDefSerialPlugIn
#define	serPortLocalHotSync		0x8000	// Use physical HotSync port
#define	serPortMaskLocal			0x7FFF	// Mask off HotSync "hint" (for SerialMgr)


/********************************************************************
 * Serial Settings Descriptor
 ********************************************************************/
#pragma mark -Configuration Settings-

typedef struct SerSettingsType {
	ULong			baudRate;			// baud rate
	ULong			flags;				// miscellaneous settings
	Long			ctsTimeout;			// max # of ticks to wait for CTS to become asserted
											// before transmitting; used only when
											// configured with serSettingsFlagCTSAutoM.
	} SerSettingsType;
typedef SerSettingsType*	SerSettingsPtr;
	
#define		serSettingsFlagStopBitsM			0x00000001		// mask for stop bits field
#define		serSettingsFlagStopBits1			0x00000000		//  1 stop bits	
#define		serSettingsFlagStopBits2			0x00000001		//  2 stop bits	
#define		serSettingsFlagParityOnM			0x00000002		// mask for parity on
#define		serSettingsFlagParityEvenM			0x00000004		// mask for parity even
#define		serSettingsFlagXonXoffM				0x00000008		// (NOT IMPLEMENTED) mask for Xon/Xoff flow control
#define		serSettingsFlagRTSAutoM				0x00000010		// mask for RTS rcv flow control
#define		serSettingsFlagCTSAutoM				0x00000020		// mask for CTS xmit flow control
#define		serSettingsFlagBitsPerCharM		0x000000C0		// mask for bits/char
#define		serSettingsFlagBitsPerChar5		0x00000000		//  5 bits/char	
#define		serSettingsFlagBitsPerChar6		0x00000040		//  6 bits/char	
#define		serSettingsFlagBitsPerChar7		0x00000080		//  7 bits/char	
#define		serSettingsFlagBitsPerChar8		0x000000C0		//  8 bits/char	


// Default settings
#define		serDefaultSettings		(serSettingsFlagBitsPerChar8	|		\
												serSettingsFlagStopBits1		|		\
												serSettingsFlagRTSAutoM)
												
#define		serDefaultCTSTimeout		(5*sysTicksPerSecond);

//
// mask values for the lineErrors  from SerGetStatus
//
#pragma mark -Line Errors-

#define	serLineErrorParity		0x0001			// parity error
#define	serLineErrorHWOverrun	0x0002			// HW overrun
#define	serLineErrorFraming		0x0004			// framing error
#define 	serLineErrorBreak			0x0008			// break signal asserted
#define 	serLineErrorHShake		0x0010			// line hand-shake error
#define	serLineErrorSWOverrun	0x0020			// HW overrun




/********************************************************************
 * Type of a wakeup handler procedure which can be installed through the
 *   SerSetWakeupHandler() call.
 ********************************************************************/
typedef void (*SerWakeupHandler)  (DWord refCon);

/********************************************************************
 * Type of an emulator-mode only blocking hook routine installed via
 * SerControl function serCtlEmuSetBlockingHook.  This is supported only
 * under emulation mode.  The argument to the function is the value
 * specified in the SerCallbackEntryType structure.  The intention of the
 * return value is to return false if serial manager should abort the
 * current blocking action, such as when an app quit event has been received;
 * otherwise, it should return true.  However, in the current implementation,
 * this return value is ignored.  The callback can additionally process
 * events to enable user interaction with the UI, such as interacting with the
 * debugger. 
 ********************************************************************/
typedef Boolean (*SerBlockingHookHandler)  (DWord userRef);




/********************************************************************
 * Serial Library Control Enumerations (Pilot 2.0)
 ********************************************************************/
#pragma mark -Control Enumerations-

/********************************************************************
 * Structure for specifying callback routines.
 ********************************************************************/
typedef struct SerCallbackEntryType {
	Ptr		funcP;					// function pointer
	DWord		userRef;					// ref value to pass to callback
	} SerCallbackEntryType;
typedef SerCallbackEntryType*	SerCallbackEntryPtr;

// v2.0 extension
// DOLATER... add DTR control and status
typedef enum SerCtlEnum {
	serCtlFirstReserved = 0,		// RESERVE 0
	
	serCtlStartBreak,					// turn RS232 break signal on:
											// users are responsible for ensuring that the break is set
											// long enough to genearate a valie BREAK!
											// valueP = 0, valueLenP = 0
											
	serCtlStopBreak,					// turn RS232 break signal off:
											// valueP = 0, valueLenP = 0

	serCtlBreakStatus,				// Get RS232 break signal status(on or off):
											// valueP = ptr to Word for returning status(0 = off, !0 = on)
											// *valueLenP = sizeof(Word)
											
	serCtlStartLocalLoopback,		// Start local loopback test
											// valueP = 0, valueLenP = 0
											
	serCtlStopLocalLoopback,		// Stop local loopback test
											// valueP = 0, valueLenP = 0

	serCtlMaxBaud,						// Get maximum supported baud rate:
											// valueP = ptr to DWord for returned baud
											// *valueLenP = sizeof(DWord)
	
	serCtlHandshakeThreshold,		// retrieve HW handshake threshold; this is the maximum baud rate
											// which does not require hardware handshaking
											// valueP = ptr to DWord for returned baud
											// *valueLenP = sizeof(DWord)
	
	serCtlEmuSetBlockingHook,		// Set a blocking hook routine FOR EMULATION
											// MODE ONLY - NOT SUPPORTED ON THE PILOT
											//PASS:
											// valueP = ptr to SerCallbackEntryType
											// *valueLenP = sizeof(SerCallbackEntryType)
											//RETURNS:
											// the old settings in the first argument
											

	serCtlIrDAEnable,					// Enable  IrDA connection on this serial port
											// valueP = 0, valueLenP = 0

	serCtlIrDADisable,				// Disable  IrDA connection on this serial port
											// valueP = 0, valueLenP = 0

	serCtlIrScanningOn,				// Start Ir Scanning mode	
	
	serCtlIrScanningOff,				// Stop Ir Scanning mode
	
	serCtlRxEnable,					// enable receiver  ( for IrDA )
	
	serCtlRxDisable,					// disable receiver ( for IrDA )

	serCtlLAST							// ADD NEW ENTRIES BEFORE THIS ONE
	} SerCtlEnum;



/********************************************************************
 * Serial Library Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/
#pragma mark -API Functions-

#ifdef __cplusplus
extern "C" {
#endif

// Used by mac applications to map the pilot serial port to a particular
// macintosh port.
UInt SerSetMapPort( UInt pilotPort, UInt macPort );

// Acquires and opens a serial port with given baud and default settings.
Err SerOpen(UInt refNum, UInt port, ULong baud)
				SYS_TRAP(sysLibTrapOpen);
				
// Used by debugger to re-initialize serial port if necessary
Err SerDbgAssureOpen(UInt refNum, UInt port, ULong baud);

// Closes the serial connection previously opened with SerOpen.
Err SerClose(UInt refNum)
				SYS_TRAP(sysLibTrapClose);

// Puts serial library to sleep
Err SerSleep(UInt refNum)
				SYS_TRAP(sysLibTrapSleep);

// Wake Serial library
Err SerWake(UInt refNum)
				SYS_TRAP(sysLibTrapWake);

// Get attributes of the serial connection
Err SerGetSettings(UInt refNum, SerSettingsPtr settingsP)
				SYS_TRAP(sysLibTrapCustom);
				
// Set attributes of the serial connection
Err SerSetSettings(UInt refNum, SerSettingsPtr settingsP)
				SYS_TRAP(sysLibTrapCustom+1);

// Return status of serial connection
Word SerGetStatus(UInt refNum, BooleanPtr ctsOnP, 
				BooleanPtr dsrOnP)
				SYS_TRAP(sysLibTrapCustom+2);
				
// Reset error condition of serial connection
Err SerClearErr(UInt refNum)
				SYS_TRAP(sysLibTrapCustom+3);
				
				


// Sends a buffer of data (may queue it up and return).
Err SerSend10(UInt refNum, VoidPtr bufP, ULong size)
				SYS_TRAP(sysLibTrapCustom+4);

// Waits until the serial transmit buffer empties.
// The timeout arg is ignored; CTS timeout is used
Err SerSendWait(UInt refNum, Long timeout)
				SYS_TRAP(sysLibTrapCustom+5);

// Returns how many characters are left in the send queue waiting 
//  for transmission
Err SerSendCheck(UInt refNum, ULongPtr numBytesP)
				SYS_TRAP(sysLibTrapCustom+6);

// Flushes the data out of the transmit buffer
Err SerSendFlush(UInt refNum)
				SYS_TRAP(sysLibTrapCustom+7);




// Receives a buffer of data of the given size.
Err SerReceive10(UInt refNum, VoidPtr bufP, ULong bytes, Long timeout)
				SYS_TRAP(sysLibTrapCustom+8);

// Waits for at least 'bytes' bytes of data to arrive at the serial input.
//  but does not read them in
Err SerReceiveWait(UInt refNum, ULong bytes, Long timeout)
				SYS_TRAP(sysLibTrapCustom+9);

// Returns how many characters are in the receive queue
Err SerReceiveCheck(UInt refNum, ULongPtr numBytesP)
				SYS_TRAP(sysLibTrapCustom+10);

// Flushes any data coming into the serial port, discarding the data.
void SerReceiveFlush(UInt refNum, Long timeout)
				SYS_TRAP(sysLibTrapCustom+11);


// Specify a new input buffer.  To restore the original buffer, pass
// bufSize = 0.
Err SerSetReceiveBuffer(UInt refNum, VoidPtr bufP, UInt bufSize)
				SYS_TRAP(sysLibTrapCustom+12);


// The receive character interrupt service routine, called by kernel when
//  a UART interrupt is detected. DOLATER... generalize ISP's for multiple Serial libs
Boolean SerReceiveISP(void)		
				SYS_TRAP(sysTrapSerReceiveISP);



// "Back Door" into the serial receive queue. Used by applications (like TCP Media layers)
//  that need faster access to received characters
Err	SerReceiveWindowOpen(UInt refNum, BytePtr* bufPP, DWordPtr sizeP)
				SYS_TRAP(sysLibTrapCustom+13);
				
Err	SerReceiveWindowClose(UInt refNum, DWord bytesPulled)
				SYS_TRAP(sysLibTrapCustom+14);
				
// Can be called by applications that need an alternate wakeup mechanism
//  when characters get enqueued by the interrupt routine.  
Err	SerSetWakeupHandler(UInt refNum, SerWakeupHandler procP, 
					DWord refCon)
				SYS_TRAP(sysLibTrapCustom+15);
	
// Called to prime wakeup handler			
Err	SerPrimeWakeupHandler(UInt refNum, Word minBytes)
				SYS_TRAP(sysLibTrapCustom+16);
	
// Called to perform a serial manager control operation			
// (v2.0 extension)
Err	SerControl(UInt refNum, Word op, VoidPtr valueP, WordPtr valueLenP)
				SYS_TRAP(sysLibTrapCustom+17);


// Sends a buffer of data (may queue it up and return).
ULong SerSend(UInt refNum, VoidPtr bufP, ULong count, Err* errP)
				SYS_TRAP(sysLibTrapCustom+18);

// Receives a buffer of data of the given size.
ULong SerReceive(UInt refNum, VoidPtr bufP, ULong count, Long timeout, Err* errP)
				SYS_TRAP(sysLibTrapCustom+19);


#ifdef __cplusplus
}
#endif

	
#endif	//__SERIAL_MGR_H
