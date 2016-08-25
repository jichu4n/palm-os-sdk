/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		M68KHwr.h
 *
 * Description:
 *		Pilot debugger remote hardware/system info 
 *
 * History:
 *   	1/18/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/  

#ifndef __M68KHWR_H
#define __M68KHWR_H

// Pilot common definitions
#include <Common.h>


/***********************************************************************
 * Breakpoint words we use
 ***********************************************************************/
#define m68kTrapInstr			0x4E40
#define m68kTrapVectorMask		0x000F

/***********************************************************************
 * 68000 Exception Vector table
 ***********************************************************************/
typedef struct M68KExcTableType {
	DWord		initStack;								// initial stack pointer
	DWord		initPC;									// initial PC
	
	DWord		busErr;									// 08 
	DWord		addressErr;								// 0C  
	DWord		illegalInstr;							// 10  
	DWord		divideByZero;							// 14  
	DWord		chk;									// 18
	DWord		trap;									// 1C
	DWord		privilege;								// 20
	DWord		trace;									// 24
	DWord		aTrap;									// 28
	DWord		fTrap;									// 2C
	DWord		reserved12;								// 30
	DWord		coproc;									// 34
	DWord		formatErr;								// 38
	DWord		unitializedInt;							// 3C
	
	DWord		reserved[8];							// 40-5C
	
	DWord		spuriousInt;							// 60
	DWord		autoVec1;								// 64
	DWord		autoVec2;								// 68
	DWord		autoVec3;								// 6C
	DWord		autoVec4;								// 70
	DWord		autoVec5;								// 74
	DWord		autoVec6;								// 78
	DWord		autoVec7;								// 7C
	
	DWord		trapN[16];								// 80 - BC
	
	DWord		unassigned[16];							// C0 - FC
	} M68KExcTableType;
 
 

/**************************************************************************************
 *  structure for the Motorolla 68000 processor registers (variables).
 *
 *	WARNING:
 *	This structure is used as the body of the 'read regs' command response
 *  packet.  Any changes to it will require changes in the nub's code.
 *
 **************************************************************************************/
typedef struct M68KRegsType {
	DWord	d[8];							/*  data registers  */
	DWord	a[7];							/*  address registers  */
	DWord	usp;							/*  user stack pointer  */
	DWord	ssp;							/*  supervisor stack pointer  */
	DWord	pc;							/*  program counter  */
	Word	sr;							/*  status register  */
} M68KRegsType;



 
/**************************************************************************************
 *  bit masks for testing M68000 status register fields
 **************************************************************************************/

/* trace mode */
#define	m68kSrTraceMask			0x08000
#define	m68kSrTraceBit				15

/* supervisor state */
#define	m68kSrSupervisorMask		0x02000

/* interrupt mask */
#define	m68kSrInterruptMask		0x00700
#define	m68kSrInterruptOffset	8			/* offset for right-shifting interrupt mask */

/* condition codes */
#define	m68kSrExtendMask			0x00010
#define	m68kSrNegativeMask		0x00008
#define	m68kSrZeroMask				0x00004
#define	m68kSrOverflowMask		0x00002
#define	m68kSrCarryMask			0x00001



#endif	//__M68KHWR_H

