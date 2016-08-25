/*******************************************************************
 * 							 Pilot Software 
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SysAll.h
 *
 * Description:
 *		Includes all system header files in order to help compiler
 *		do pre-compiled headers
 *
 * History:
 *   	11/17/94  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __SYSALL_H__
#define __SYSALL_H__


//*********************************************************************
// If not included from Pilot.h, as is the case when compiling UI modules,
//  use the SysAll pre-compiled header file. 
//*********************************************************************
#ifndef __Pilot_H__
#ifndef __GNUC__
	#ifndef PILOT_PRECOMPILED_HEADERS_OFF	
		#if	!__option(precompile)
			#ifdef CMD_LINE_BUILD
				#include "SysAll.h.obj.mpw"
			#else
				#include "SysAll.h.obj"
			#endif
			#define	__INCLUDED_SYSALL_H_OBJ__
		#endif
	#endif
#endif
#endif


// Fastest compilation speed is obtained by not including these
//  headers when the pre-compiled header is used.
#ifndef __INCLUDED_SYSALL_H_OBJ__
#include <Common.h>
#include <SysTraps.h>
#include <SystemMgr.h>
#include <DebugMgr.h>
#include <ErrorMgr.h>
#include <StringMgr.h>
#include <SysUtils.h>
#include <MemoryMgr.h>
#include <DataMgr.h>

#include <Preferences.h>
#include <TimeMgr.h>
#include <AlarmMgr.h>
#include <SoundMgr.h>
#include <Keyboard.h>
#endif


#endif // __SYSALL_H__
