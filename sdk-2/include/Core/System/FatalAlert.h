/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		FatalAlert.h
 *
 * Description:
 *		This file defines the fatal alert routine.
 *
 * History:
 *   	9/12/94  art - Created by Art Lamb
 *
 *******************************************************************/

#ifndef __FATAL_ALERT_H__
#define __FATAL_ALERT_H__

// Value retunred by FatalAlert
#define fatalContinue	0
#define fatalReset		1

UInt SysFatalAlert (CharPtr msg)
		SYS_TRAP(sysTrapSysFatalAlert);

#endif 

