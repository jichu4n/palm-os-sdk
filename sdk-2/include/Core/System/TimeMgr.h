/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		TimeMgr.h
 *
 * Description:
 *		Time manager functions
 *
 * History:
 *   	1/19/95  roger - Created by Roger Flores
 *
 *
 *******************************************************************/


#ifndef __TIMEMGR_H__
#define __TIMEMGR_H__


// Include elementary types
#include <Common.h>
#include <SysTraps.h>				// Trap Numbers.



/************************************************************
 * Time Manager result codes
 * (timErrorClass is defined in SystemMgr.h)
 *************************************************************/
#define timErrMemory			(timErrorClass | 1)



/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------------------------------------
// Initialization
//-------------------------------------------------------------------
Err			TimInit(void)	SYS_TRAP(sysTrapTimInit);


//-------------------------------------------------------------------
// API
//-------------------------------------------------------------------

// seconds since 1/1/1904
ULong			TimGetSeconds(void)		
						SYS_TRAP(sysTrapTimGetSeconds);
					
// seconds since 1/1/1904
void  		TimSetSeconds(ULong seconds) 	
						SYS_TRAP(sysTrapTimSetSeconds);
					
// ticks since power on
ULong 		TimGetTicks(void)					
						SYS_TRAP(sysTrapTimGetTicks);


#ifdef __cplusplus 
}
#endif



#endif // __TIMEMGR_H__


