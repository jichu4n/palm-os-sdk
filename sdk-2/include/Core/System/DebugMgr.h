/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DebugMgr.h
 *
 * Description:
 *		Debugging functions
 *
 * History:
 *   	11/03/94  RM - Created by Ron Marianetti
 *   	 5/19/97  SL - Fixed DbgBreak for other environments
 *
 *
 *******************************************************************/


#ifndef __DEBUGMGR_H__
#define __DEBUGMGR_H__

// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.



/************************************************************
 * Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Long 			DbgInit(Ptr spaceP, Ptr dispatchTableP[], Boolean openComm);


// NOTE!!! DbgBreak can NOT be written using the ONEWORD_INLINE macro
//  because it must always resolve to a trap instruction even when
//  USE_TRAPS is set to 0. In particular the SmallROM compiles with
//  USE_TRAPS set to 0 and needs the DbgBreak to resolve as a trap
//  instruction.  Thus we explicitly check USE_TRAPS before using
//  default ONEWORD_INLINE definition.

#ifndef __GNUC__
#if EMULATION_LEVEL == EMULATION_NONE
	#if USE_TRAPS == 0 && defined(__MWERKS__)	// for Palm SmallROM builds
		void 		DbgBreak(void) = {m68kTrapInstr+sysDbgTrapNum};
	#elif USE_TRAPS == 0								// for others who set USE_TRAPS to 0
		void 		DbgBreak(void);
	#else													// default: fall through to ONEWORD_INLINE
		void 		DbgBreak(void)
							ONEWORD_INLINE(m68kTrapInstr+sysDbgTrapNum);
	#endif
#else   /* EMULATION_LEVEL */
	void 		DbgBreak(void);
#endif  /* EMULATION_LEVEL */


void 			DbgSrcBreak(void)
							ONEWORD_INLINE(m68kTrapInstr+sysDbgTrapNum);
#else   /* __GNUC__ */

#define DbgBreak()    ONEWORD_INLINE(m68kTrapInstr+sysDbgTrapNum)
#define DbgSrcBreak() ONEWORD_INLINE(m68kTrapInstr+sysDbgTrapNum)

#endif  /* __GNUC__ */

void 			DbgSrcMessage(char* debugStr)
							SYS_TRAP(sysTrapDbgSrcMessage);

void 			DbgMessage(char* aStr)
							SYS_TRAP(sysTrapDbgMessage);
	
CharPtr 		DbgGetMessage(BytePtr bufferP, Long timeout)
							SYS_TRAP(sysTrapDbgGetMessage);
							
Err			DbgCommSettings(ULongPtr baudP, ULongPtr flagsP)
							SYS_TRAP(sysTrapDbgCommSettings);
	
#ifdef __cplusplus 
}
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_DbgBreak		\
				DC.W	m68kTrapInstr+sysDbgTrapNum
				


#endif // __DEBUGMGR_H__


