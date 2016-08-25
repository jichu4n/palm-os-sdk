/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		ConsoleMgr.h
 *
 * Description:
 *		This module implements simple text in and text out to a console 
 *  application on the other end of the serial port. It talks through
 *  the Serial Link Manager and sends and receives packets of type slkPktTypeConsole.
 *
 * History:
 *   	10/25/94  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __CONSOLEMGR_H__
#define __CONSOLEMGR_H__




/********************************************************************
 * Console Manager Routines
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err		ConPutS(CharPtr message)
				SYS_TRAP(sysTrapConPutS);
				
Err		ConGetS(CharPtr message, Long timeout)
				SYS_TRAP(sysTrapConGetS);


#ifdef __cplusplus 
}
#endif




#endif // __CONSOLEMGR_H__




