/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SysConfigPrv.h
 *
 * Description:
 *		Private system configuration defines
 *
 * History:
 *   	8/23/96  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE

#ifndef __SYSCONFIGPRV_H__
#define __SYSCONFIGPRV_H__




/************************************************************
 * Private Memory Manager Constants
 *************************************************************/
// Default size of the dynamic heap
// This macro accepts the total RAM in card 0 as a parameter and returns
//  the number of bytes to assign to the dynamic heap
#ifndef BIG_HEAP
#error	Build option BIG_HEAP not setup
#endif
#if 		BIG_HEAP
	#define	memCalcDynamicRAMSize(totalSize) 		\
		((totalSize > 0x20000) ? (0x010000) : (0x008000))	// leave at 32K until ROM2.0B
#else
	#define	memCalcDynamicRAMSize(totalSize) 		\
		((totalSize > 0x20000) ? (0x008000) : (0x008000))	// leave at 32K until ROM2.0B
#endif

#define	memCalcDynamicHeapSize(totalSize) 		\
		(memCalcDynamicRAMSize(totalSize)-sysLowMemSize)



#endif  // __SYSCONFIGPRV_H__
#endif //NON_PORTABLE
