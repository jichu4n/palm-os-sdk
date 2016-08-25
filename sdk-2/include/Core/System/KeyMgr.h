/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		KeyMgr.h
 *
 * Description:
 *		Include file for Key manager
 *
 * History:
 *   	9/13/95 Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __KEYMGR_H__
#define __KEYMGR_H__

// Pilot common definitions
#include <Common.h>
#include <SystemMgr.h>


/********************************************************************
 * Definition of bit field returned from KeyCurrentState
 ********************************************************************/
#define	keyBitPower			0x01			// Power key
#define	keyBitPageUp		0x02			// Page-up
#define	keyBitPageDown		0x04			// Page-down
#define	keyBitHard1			0x08			// App #1
#define	keyBitHard2			0x10			// App #2
#define	keyBitHard3			0x20			// App #3
#define	keyBitHard4			0x40			// App #4
#define	keyBitCradle		0x80			// Button on cradle

#define	keyBitsAll			0xFFFFFFFF	// all keys


#define slowestKeyDelayRate	0xff
#define slowestKeyPeriodRate	0xff


/********************************************************************
 * Key manager Routines
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Set/Get the auto-key repeat rate
Err 		KeyRates(Boolean set, WordPtr initDelayP, WordPtr periodP, 
						WordPtr doubleTapDelayP, BooleanPtr queueAheadP)
							SYS_TRAP(sysTrapKeyRates);
							
// Get the current state of the hardware keys
DWord		KeyCurrentState(void)
							SYS_TRAP(sysTrapKeyCurrentState);
							
// Set the state of the hardware key mask which controls if the key
// generates a keyDownEvent
DWord		KeySetMask(DWord keyMask)
							SYS_TRAP(sysTrapKeySetMask);
							
#ifdef __cplusplus
}
#endif

	
#endif	//__KEYMGR_H__
