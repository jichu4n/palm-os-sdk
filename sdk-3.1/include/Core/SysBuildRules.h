/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		SysBuildRules.h
 *
 * Description:
 *		This header file can beused as a template for
 *  common build options across all applications. To use, simply break 
 *  the pencil and edit.
 *   
 *
 *  Typically, the options specified in this file are used to 
 *  set the language and country for the emulator
 *
 * History:
 *   	6/6/95 RM - Created by Ron Marianetti  
 *
 *******************************************************************/
 
//#define	LANGUAGE					LANGUAGE_FRENCH
//#define	COUNTRY					COUNTRY_FRANCE

// These can be overridden by AppBuildRules.h so we must check to
//   see if they've been defined already or not.


// <RM> 1-16-98, Replaced DYN_MEM_SIZE with DYN_MEM_SIZE_MIN &
//   DYN_MEM_SIZE_MAX
// WAS:
// #ifndef 	DYN_MEM_SIZE
//	  #define		DYN_MEM_SIZE		0x010000		// 64K
// #endif
#ifdef	DYN_MEM_SIZE
	#error	Error! Obsolete define DYN_MEM_SIZE used
#endif

#ifndef 	DYN_MEM_SIZE_MIN
	  #define		DYN_MEM_SIZE_MIN		0x010000		// 64K
#endif
#ifndef 	DYN_MEM_SIZE_MAX
	  #define		DYN_MEM_SIZE_MAX		0x010000		// 64K
#endif
#ifndef 	DYN_MEM_SIZE_EXTRA
	  #define		DYN_MEM_SIZE_EXTRA	0x000000		// 0K
#endif



#ifndef 	DEFAULT_LIB_ENTRIES
	#define		DEFAULT_LIB_ENTRIES	5			// 5 libraries
#endif
