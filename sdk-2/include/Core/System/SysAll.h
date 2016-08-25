/*******************************************************************
 * 							 Pilot Software 
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
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
 *		2/24/97   roger - changed to handle c++ versions.
 *		4/16/97   SCL - Added <FeatureMgr.h>
 *
 *******************************************************************/

#ifndef __SYSALL_H__
#define __SYSALL_H__


//*********************************************************************
// If not included from Pilot.h, as is the case when compiling UI modules,
//  use the SysAll pre-compiled header file. 
//*********************************************************************
#ifndef __PILOT_H__
	#ifndef PILOT_PRECOMPILED_HEADERS_OFF
	
		// Use Metrowerk's precompiled headers option
		#if defined(__MWERKS__)
			#if	!__option(precompile)
				#if	__PALMOS_TRAPS__
					#ifdef __cplusplus
						#include "SysAll.h++.mch"
					#else
						#include "SysAll.h.mch"
					#endif
				#else
					#ifdef CMD_LINE_BUILD
						#ifdef __cplusplus
							#include "SysAll.h++.mpw.mch"
						#else
							#include "SysAll.h.mpw.mch"
						#endif
					#else
						#ifdef __cplusplus
							#include "SysAll.h++.sim.mch"
						#else
							#include "SysAll.h.sim.mch"
						#endif
					#endif
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
#include <FeatureMgr.h>

#include <Preferences.h>
#include <Localize.h>
#include <TimeMgr.h>
#include <AlarmMgr.h>
#include <SoundMgr.h>
#include <Keyboard.h>
#include <GraffitiReference.h>
#include <AppLaunchCmd.h>
#endif


#endif // __SYSALL_H__
