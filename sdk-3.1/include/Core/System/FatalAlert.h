/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     window.h
 * AUTHOR:       Art Lamb: September 12, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines window structures and routines.
 *
 **********************************************************************/
#ifndef __FATAL_ALERT_H__
#define __FATAL_ALERT_H__

// Value retunred by FatalAlert
#define fatalReset			0
#define fatalEnterDebugger	1
#define fatalDoNothing		0xFFFFU

UInt SysFatalAlert (CharPtr msg)
		SYS_TRAP(sysTrapSysFatalAlert);

#endif 

