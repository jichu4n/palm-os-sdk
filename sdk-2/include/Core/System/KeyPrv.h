/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		KeyPrv.h
 *
 * Description:
 *		Private Include file for Key manager
 *
 * History:
 *   	9/13/95 Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE
#ifndef __KEYPRV_H
#define __KEYPRV_H


/********************************************************************
 * Key Manager Constants
 ********************************************************************/
#define	keyMinUpTicks					(sysTicksPerSecond/20)	// Min# of ticks key must be up to recognize it

#define	keyMinBacklightTicks			(sysTicksPerSecond)		// Min# of ticks key must be held down to
																				// turn on backlight

#define	keyBitMask						0x007F	// Which bits in port D we use for keys

/********************************************************************
 * Key Manager Globals
 ********************************************************************/
typedef struct KeyGlobalsType {
	ULong		lastKeyTicks;					// GHwrCurTicks last time we enqueued
													//  a key event - cleared as soon as
													//  key is released.
													

	Byte		keyState;						// current state of hardware keys - returned
													// by KeyCurrentState
	Byte		keyStateMask;					// which keys generate an event
	Byte		keyUpCount;						// # of consecutive times we've detected
													// no keys down - used for debouncing
	Byte		initDelay;						// key repeat init delay in ticks
	Byte		period;							// key repeat period in ticks
	
	DWord		turnOnTicks;					// Tickcount for key that turned
													// device on. Used for
													// detecting when we should turn on backlight
	
	// These variables used for sensing double-taps
	ULong		doubleTapTicks;				// GHwrCurTicks last tiem we enqueued
													//  a key event - never cleared. 
	Word		doubleTapDelay;				// double-tap timeout in ticks
	Byte		doubleTapState;				// state of hardware keys last time
	
	// Belongs next to 'period' but put here for alignment
	Boolean	queueAhead;						// if true, enqueue auto-repeat keys even if 
													// queue is not empty.
	} KeyGlobalsType;
	
typedef KeyGlobalsType*	KeyGlobalsPtr;





/********************************************************************
 * Key manager Routines
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Called early on during boot-up to get status of hard reset
//  and debugger key options. Returns DWord with bits set for each
//  key down.
DWord		KeyBootKeys(void);
				

// Initializes the Key Manager and allocates it's globals
Err		KeyInit(void)
				SYS_TRAP(sysTrapKeyInit);


// Called as a result of a key-down interrupt or periodically by
//  by timer task while key is down. Returns # of ticks before
//  it should be called again by timer task.
ULong		KeyHandleInterrupt(Boolean periodic, DWord status)
				SYS_TRAP(sysTrapKeyHandleInterrupt);
				
			
// Called by SysEvtMgr when it gets a pen event in order to
//  reset the key manager's double-tap detection.
Err		KeyResetDoubleTap(void)
				SYS_TRAP(sysTrapKeyResetDoubleTap);


// Called before system goes to sleep
Err		KeySleep(Boolean untilReset, Boolean emergency)
				SYS_TRAP(sysTrapKeySleep);


// Called when the system wakes up
Err		KeyWake(void)
				SYS_TRAP(sysTrapKeyWake);



#ifdef __cplusplus
}
#endif

	
#endif	//__KEYPRV_H
#endif 	// NON_PORTABLE
