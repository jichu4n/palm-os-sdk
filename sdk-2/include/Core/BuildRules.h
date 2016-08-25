/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		BuildRules.h
 *
 * Description:
 *		Build Rules for Pilot. This file is necessary for
 *  the CodeWarrior environment since it doesn't permit #defines
 *  in the "Makefile" (project settings).
 *    
 *    This file is included from within Common.h
 *
 * History:
 *   	12/13/94  RM - Created by Ron Marianetti  
 *   	 5/19/97  SL - Cleaned up CMD_LINE_BUILD and __PALMOS_TRAPS__ 
 *								for other build environments
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
#define	ENVIRONMENT_CW			0		// CodeWarrior compiler/linker (IDE or MPW)
#define	ENVIRONMENT_MPW		1		// Apple's MPW compiler/linker


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


// The makefile should set the define CONSOLE_SERIAL_LIB to one of
// the following.  THIS DEFINE IS ONLY USED BY A COUPLE MODULES SO WE
//  DON'T GIVE IT A DEFAULT VALUE BELOW...
// ANY MODULE THAT USES THIS DEFINE SHOULD VERIFY THAT IT IS DEFINED!!
#define	CONSOLE_SERIAL_LIB_68328	1	// SerialMgr68328 used by Console app
#define	CONSOLE_SERIAL_LIB_68681	2	// SerialMgr68681 used by Console app


// The makefile should set the define COUNTRY to one of the 
// following constants:
#define	COUNTRY_UNITED_STATES	0
#define	COUNTRY_FRANCE				1
#define	COUNTRY_GERMANY			2
#define	COUNTRY_ITALY				3
#define	COUNTRY_SPAIN				4


// The makefile should set the define LANGUAGE to one of the 
// following constants:
#define	LANGUAGE_ENGLISH			0
#define	LANGUAGE_FRENCH			1
#define	LANGUAGE_GERMAN			2
#define	LANGUAGE_ITALIAN			3
#define	LANGUAGE_SPANISH			4



// The makefile should set the define USER_MODE to one of the 
// following constants:
#define	USER_MODE_NORMAL			0	// normal operation
#define	USER_MODE_DEMO				1	// demo mode - Graffiti and pop-up keyboard disabled


// The makefile should set the define INTERNAL_COMMANDS to one of the 
// following constants:
#define	INTERNAL_COMMANDS_EXCLUDE		0
#define	INTERNAL_COMMANDS_INCLUDE		1	// Include internal shell commands

// The makefile should set the define INCLUDE_DES to one of the 
// following constants:
#define	INCLUDE_DES_OFF					0
#define	INCLUDE_DES_ON						1	// include it


/************************************************************
 * Include the following when running under the CodeWarrior
 *  IDE so that default build options can be overriden. The default
 *  version of this file is in the Incs: directory. If an app wishes
 *  to override the default options, it should create a copy of this
 *  include file in its own local directory. This include file
 *  is never used when building from MPW since MPW can specify compiler
 *  defines from the command line.
 *
 * Other environments can override all of these settings by simply
 *  pre-defining CMD_LINE_BUILD and any other desired settings.
 *************************************************************/

#ifndef CMD_LINE_BUILD				// typically pre-defined only from MPW

	#if !defined(__MWERKS__) && !defined(__PALMOS_TRAPS__)

		// assume other environments generally build only PalmOS executables
		#define __PALMOS_TRAPS__ 	1

	#endif
	

	#if	__PALMOS_TRAPS__			// defined by CodeWarrior IDE or above

		// Settings to build a PalmOS executable
		#define EMULATION_LEVEL		EMULATION_NONE		// building Pilot executable
		#define USE_TRAPS				1						// use Pilot traps
		
	#endif


	// Include default definitions - either from Incs: directory or from
	// local project directory (if present, to override system defaults).
	#include "AppBuildRules.h"
	#include <SysBuildRules.h>
#endif



/************************************************************
 * Settings that can be overriden in the makefile (for MPW)
 *	OR (for CodeWarrior) in "AppBuildRules.h".  If there is no
 * local copy of "AppBuildRules.h" within the project directory,
 * the one in the Incs directory will be used instead.
 *************************************************************/ 
#ifndef	EMULATION_LEVEL
	#define	EMULATION_LEVEL		EMULATION_MAC
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
	#define	MEMORY_FORCE_LOCK		MEMORY_FORCE_LOCK_ON
#endif

#ifndef	COUNTRY
	#define	COUNTRY					COUNTRY_UNITED_STATES
#endif

#ifndef	LANGUAGE
	#define	LANGUAGE					LANGUAGE_ENGLISH
#endif

#ifndef	USER_MODE
	#define	USER_MODE				USER_MODE_NORMAL
#endif

#ifndef	INTERNAL_COMMANDS
	#define	INTERNAL_COMMANDS		INTERNAL_COMMANDS_EXCLUDE
#endif

#ifndef	INCLUDE_DES
	#define	INCLUDE_DES				INCLUDE_DES_OFF
#endif


/**********************************************************************
 * By putting these here (once), we don't need them in each :Src:ÅRsc.c
 **********************************************************************/ 
#if 		LANGUAGE == LANGUAGE_ENGLISH
#define RESOURCE_FILE_PREFIX ""

#elif 	LANGUAGE == LANGUAGE_FRENCH
#define RESOURCE_FILE_PREFIX "Localized:Fr."

#elif 	LANGUAGE == LANGUAGE_GERMAN
#define RESOURCE_FILE_PREFIX "Localized:Ge."

#elif 	LANGUAGE == LANGUAGE_ITALIAN
#define RESOURCE_FILE_PREFIX "Localized:It."

#elif 	LANGUAGE == LANGUAGE_SPANISH
#define RESOURCE_FILE_PREFIX "Localized:Sp."
#endif



#endif // __BUILDRULES_H__
