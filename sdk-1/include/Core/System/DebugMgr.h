/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
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

#ifdef __GNUC__
static inline
#endif
void 			DbgSrcBreak(void)
							ONEWORD_INLINE(m68kTrapInstr+sysDbgTrapNum);
 
void 			DbgSrcMessage(char* debugStr)
							SYS_TRAP(sysTrapDbgSrcMessage);

#if EMULATION_LEVEL == EMULATION_NONE
#ifdef __GNUC__
static inline
#endif
	void 		DbgBreak(void) ONEWORD_INLINE(m68kTrapInstr+sysDbgTrapNum);
#else
	void 		DbgBreak(void);
#endif

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


