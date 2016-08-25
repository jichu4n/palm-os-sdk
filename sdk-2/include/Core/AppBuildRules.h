/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		AppBuildRules.h
 *
 * Description:
 *		This header file can be used as a template for
 *  application specific build options. To use, simple copy this file
 *  and place it into your local directory. 
 *
 *  Typically, the options specified in this file are used to build
 *  with different forms of error-checking, etc.  and are not common to all projects.
 *
 * History:
 *   	6/6/95 RM - Created by Ron Marianetti  
 *
 *******************************************************************/
 
// Run with error checking that forces all handles to be locked before
// they are accessed. Before alpha, all applications should be able to run with this
// option on.
#define	MEMORY_FORCE_LOCK		1


// Set these according to which Shell commands you want to
// link with.
#define	SHELL_COMMAND_DB			0	// Not Available
#define	SHELL_COMMAND_UI			0	// Not Available
#define	SHELL_COMMAND_APP			0	// Not Available
#define	SHELL_COMMAND_EMULATOR	1	// Available

