/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		BuildRules.h
 *
 * Description:
 *		Build Rules for Pilot. This file is necessary for
 *  the CodeWarrior environment since it doesn't permit #defines
 *  in the "Makefile".
 *    
 *    This file is included from within Common.h
 *
 * History:
 *   	12/13/94  RM - Created by Ron Marianetti  
 *
 *******************************************************************/

#ifndef 	__BUILDRULES_H__
#define	__BUILDRULES_H__

/************************************************************
 * Compilation Control Options
 *************************************************************/

// The makefile should set the define EMULATION_LEVEL to one of the following
//  constants:
#define	EMULATION_NONE			0		// native environment
#define	EMULATION_WINDOWS		1		// emulate on Windows
#define	EMULATION_DOS			2		// emulate on DOS
#define	EMULATION_MAC		   3		// emulate on Macintosh


// If emulation is not EMULATION_NONE, the following define controls
// whether we are talking to a touchdown device over the serial port,
// or if we are emulating a memory card in local RAM.
//
// The makefile should set the define MEMORY_TYPE to one of the following
//  constants
#define	MEMORY_LOCAL			0		// Emulate using local memory card image
#define	MEMORY_REMOTE			1		// Pass calls through serial port to the device


// The makefile should set the define ENVIRONMENT to one of the following:
#define	ENVIRONMENT_CW			0		// CodeWarrior on the Mac
#define	ENVIRONMENT_MPW		1		// MPW on the Mac


// The makefile should set the define ERROR_CHECK_LEVEL to one of the 
//  following constants:
#define	ERROR_CHECK_NONE		0		// compile no error checking code
#define	ERROR_CHECK_PARTIAL	1		// display for fatal errors only
#define	ERROR_CHECK_FULL		2		// display fatal or non-fatal errors


// The makefile should set the define CPU_TYPE to one of the 
//  following constants:
#define	CPU_68K					0		// Motorola 68K type
#define	CPU_x86					1		// Intel x86 type


// The makefile should set the define HW_TARGET to one of the 
// following constants:
#define	HW_TARGET_NONE			0		// Emulating
#define	HW_TARGET_302			1		// Motorola 302 ADS board
#define	HW_TARGET_328EMU		2		// Motorola 328 Emulator ADS board
#define	HW_TARGET_328			3		// Motorola 328 ADS board
#define	HW_TARGET_TD1			4		// Pilot hardware


// The makefile should set the define MEMORY_FORCE_LOCK to one of the
// following.
#define	MEMORY_FORCE_LOCK_OFF	0	// Don't force all handles to be locked
#define	MEMORY_FORCE_LOCK_ON		1	// Force all handles to be locked before usage


// The makefile should set the define DEBUG_LEVEL to one of the
// following. THIS DEFINE IS ONLY USED BY A COUPLE MODULES SO WE
//  DON'T GIVE IT A DEFAULT VALUE BELOW...
// ANY MODULE THAT USES THIS DEFINE SHOULD VERIFY THAT IT IS DEFINED!!
#define	DEBUG_LEVEL_NONE			1	// None: Does not auto-launch Console or Debugger
#define	DEBUG_LEVEL_PARTIAL		2	// Partial: Auto-Launches Console but skips debugger
#define	DEBUG_LEVEL_FULL			3	// Full: Waits in debugger on reset


// The makefile should set the define COUNTRY to one of the 
// following constants:
#define	COUNTRY_UNITED_STATES	0
#define	COUNTRY_FRANCE				1
#define	COUNTRY_GERMANY			2


// The makefile should set the define LANGUAGE to one of the 
// following constants:
#define	LANGUAGE_ENGLISH			0
#define	LANGUAGE_FRENCH			1
#define	LANGUAGE_GERMAN			2



/************************************************************
 * Include the following file when running under the CodeWarriror
 *  IDE so that default build options can be overriden. The default
 *  version of this file is in the Incs: directory. If an app wishes
 *  to override the default options, it should create a copy of this
 *  include file in it's own local directory. This include file
 *  is never used when building from MPW since MPW can specify compiler
 *  defines from the command line.
 *************************************************************/
#ifndef CMD_LINE_BUILD						// pre-defined only from MPW
	#include "AppBuildRules.h"
	
	#include <SysBuildRules.h>
#endif



/************************************************************
 * Settings, these can be overriden in the makefile
 *************************************************************/ 
#ifndef	EMULATION_LEVEL
	#define	EMULATION_LEVEL		EMULATION_NONE
#endif

#if EMULATION_LEVEL == EMULATION_NONE
	#define	MEMORY_TYPE				MEMORY_LOCAL
#endif

#ifndef MEMORY_TYPE
	#define	MEMORY_TYPE 			MEMORY_LOCAL
#endif

#ifndef ENVIRONMENT
	#define	ENVIRONMENT				ENVIRONMENT_CW
#endif

#ifndef	ERROR_CHECK_LEVEL
	#define	ERROR_CHECK_LEVEL		ERROR_CHECK_FULL
#endif

#ifndef CPU_TYPE
	#define	CPU_TYPE					CPU_68K
#endif

#ifndef HW_TARGET
	#define	HW_TARGET				HW_TARGET_NONE
#endif

#ifndef	MEMORY_FORCE_LOCK
	#define	MEMORY_FORCE_LOCK		MEMORY_FORCE_LOCK_OFF
#endif

#ifndef	COUNTRY
	#define	COUNTRY					COUNTRY_UNITED_STATES
#endif

#ifndef	LANGUAGE
	#define	LANGUAGE					LANGUAGE_ENGLISH
#endif



#endif // __BUILDRULES_H__
