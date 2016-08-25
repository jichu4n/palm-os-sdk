/******************************************************************************
 *
 * Copyright (c) 1994-1998 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: EmuStubs.h
 *
 * Description:
 *		Stub routines used only when emulating.
 *		
 *		Note: All of these routines must be declared using generic
 *		C types so that this header file can be included in both
 *		Pilot source code and Mac or PC source code. 
 *
 *		The module that contains these routines "MacStubs.c" or
 *		"PCStubs.c" is the glue between the Pilot routines and the OS and
 *		does not include any Pilot headers.
 *
 * History:
 *   	11/03/94  RM - Created by Ron Marianetti
 *		08/05/98 dia - Added function to check whether gremlins is on.
 *		10/26/98  vsm - Changed return type of StubProcessMacEvents.
 *
 *****************************************************************************/

#ifndef __EMUSTUBS_H__
#define __EMUSTUBS_H__

#include <HostControl.h>

typedef enum 
	{
	emuSendNoEvent,
	emuSendNilEvent,
	emuSendPenOrKeyEvent
	} StubNilEventIsPendingResults;

/************************************************************
 * Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Screen Driver Stubs
void 				StubDrawNotify(void* srcWinP, short scrRowBytes, short scrDepth, 
						short fromX, short fromY, short width, short height);

// Console stubs
int 				StubConPutS(char* cstring);
int 				StubConGetS(char* cstring);

// Debugger Stubs
void 				StubDbgSrcBreak(void);
void 				StubDbgSrcMessage(char* debugStr);
void 				StubDbgBreak(void);
void 				StubDbgMessage(char* aStr);

// Time Manager Stubs
unsigned long	StubTimGetTicks(void);
unsigned long	StubTimGetSeconds(void);
void				StubTimSetSeconds(unsigned long seconds);


// Event Manager Stubs
long				StubProcessMacEvents(long timeout);
void				StubEnqueuePenPosition(void);
void				StubFlushEvents (void);

// Gremlin Utility Functions
unsigned char	StubGremlinsIsOn(void);



// Emulator Stubs
StubNilEventIsPendingResults			StubNilEventIsPending();
	

// Error Manager stubs
void 				StubErrDisplayFileLineMsg(const char * const filename, 
						unsigned int lineNo, const char * const msg);
	

// Sound Manager stubs
#define	stubSndErrBadParam			-1
#define	stubSndErrBadChannel			-2

#define	stubSndCmdFreqDurationAmp	1		// param1 = freq in hz, param2 = duration in millisec,
														// param3 = relative amplitidue 0-64;
#define	stubSndCmdNoteOn				2		// param1 = MIDI key # (0-127),
														// param2 = 0,
														// param3 = key velocity (0-127)
#define	stubSndCmdQuiet				3		// param1-3 = 0;

#define	stubSndCmdMakeGlobalChannel	4

#define	stubSndCmdFreeGlobalChannel	5

#define	stubSndCmdFrqOn				6		// param1 = freq in hz,
														// param2 = 0,
														// param3 = relative amplitidue 0-64;

short 			StubSndCommand(void* channelP, unsigned short defAmp,
							unsigned short cmd, unsigned long param1,
							unsigned short param2, unsigned short param3,
							char noWait);
							

// Alarm Manager stubs
short				StubAlmSetAlarm(unsigned long alarmType,
						long (*callback)(), unsigned long alarmTime);
unsigned long	StubAlmGetAlarm(unsigned long alarmType);
void				StubAlmDisplayAlarm(void);


// Emulator Utility Functions
void				StubEventTrace(void *eventP);			// Pilot Emulator Event Trace call

// Resource Utility Functions
void**			StubGet1Resource(unsigned long resType, int resID);	

// Graphics Utility functions. 
void 				StubInitCTable(void* cTableP, unsigned int depth);

// Utility functions
unsigned int	StubRandom(void);
	
// File System functions
short				CreateMacOutputFile(char* name, unsigned long fileType,
							unsigned long fileCreator);
	

// Key Manager stubs
unsigned long StubKeyCurrentState(void);

#define stubKeyBitPower			0x0001
#define stubKeyBitPageUp		0x0002
#define stubKeyBitPageDown		0x0004
#define stubKeyBitHard1			0x0008	
#define stubKeyBitHard2			0x0010
#define stubKeyBitHard3			0x0020
#define stubKeyBitHard4			0x0040

char				StubCancelKey(void);

#ifdef __cplusplus 
}
#endif


#endif 	// __EMUSTUBS_H__

