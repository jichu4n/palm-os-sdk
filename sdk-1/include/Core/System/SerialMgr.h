/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SerialMgr.h
 *
 * Description:
 *		Include file for Serial manager
 *
 * History:
 *   	2/7/95 Created by Ron Marianetti
 *		7/6/95	vmk	added serDefaultSettings
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
#define	serErrBadParam				(serErrorClass | 1)
#define	serErrBadPort				(serErrorClass | 2)
#define	serErrNoMem					(serErrorClass | 3)
#define	serErrBadConnID			(serErrorClass | 4)
#define	serErrTimeOut				(serErrorClass | 5)
#define	serErrLineErr				(serErrorClass | 6)
#define	serErrAlreadyOpen			(serErrorClass | 7)
#define  serErrStillOpen			(serErrorClass | 8)
#define	serErrNotOpen				(serErrorClass | 9)


/********************************************************************
 * Serial Settings Descriptor
 ********************************************************************/
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
#define		serSettingsFlagXonXoffM				0x00000008		// mask for Xon/Xoff flow control
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
#define	serLineErrorParity		0x0001			// parity error
#define	serLineErrorHWOverrun	0x0002			// HW overrun
#define	serLineErrorFraming		0x0004			// framing error
#define 	serLineErrorBreak			0x0008			// break signal asserted
#define 	serLineErrorHShake		0x0010			// line hand-shake error
#define	serLineErrorSWOverrun	0x0020			// HW overrun




/********************************************************************
 * Serial Library Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


// Acquires and opens a serial port with given baud and default settings.
Err SerOpen(UInt refNum, UInt port, ULong baud)
				SYS_TRAP(sysLibTrapOpen);

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
Err SerSend(UInt refNum, VoidPtr bufP, ULong size)
				SYS_TRAP(sysLibTrapCustom+4);

// Waits until the serial transmit buffer empties.
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
Err SerReceive(UInt refNum, VoidPtr bufP, ULong bytes, Long timeout)
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


#ifdef __cplusplus
}
#endif

	
#endif	//__SERIAL_MGR_H
