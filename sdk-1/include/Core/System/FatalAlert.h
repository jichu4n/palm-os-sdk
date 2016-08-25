/***********************************************************************
 *
 *      Copyright (c) Palm Computing 1994 -- All Rights Reserved
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
#define fatalContinue	0
#define fatalReset		1

UInt SysFatalAlert (CharPtr msg)
		SYS_TRAP(sysTrapSysFatalAlert);

#endif 

