/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		EmuStubs.h
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
 *
 *
 *******************************************************************/


#ifndef __EMUSTUBS_H__
#define __EMUSTUBS_H__


/************************************************************
 * Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Screen Driver Stubs
void 				StubDrawNotify(void* srcWinP, short scrRowBytes, 
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
void				StubProcessMacEvents(long timeout);
void				StubEnqueuePenPosition(void);
void				StubFlushEvents (void);


// Emulator Stubs
Boolean			StubNilEventPending();
	

// Error Manager stubs
void 				StubErrDisplayFileLineMsg(char* filename, unsigned int lineNo, char* msg);
	

// Sound Manager stubs
#define	stubSndErrBadParam			-1
#define	stubSndErrBadChannel			-2

#define	stubSndCmdFreqDurationAmp	1

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


// Utility functions
unsigned int	StubRandom(void);
	

// Key Manager stubs
unsigned long StubKeyCurrentState(void);

#define stubKeyBitPower			0x0001
#define stubKeyBitPageUp		0x0002
#define stubKeyBitPageDown		0x0004
#define stubKeyBitHard1			0x0008	
#define stubKeyBitHard2			0x0010
#define stubKeyBitHard3			0x0020
#define stubKeyBitHard4			0x0040

#ifdef __cplusplus 
}
#endif


#endif 	// __EMUSTUBS_H__


