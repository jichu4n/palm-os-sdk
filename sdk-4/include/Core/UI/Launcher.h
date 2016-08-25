/******************************************************************************
 *
 * Copyright (c) 1995-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Launcher.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  These are the routines for the launcher.
 *
 * History:
 *		April 27, 1995	Created by Roger Flores
 *		July 21, 2000	Deleted (mostly) by Bob Ebert, old launcher dialog not supported
 *
 *****************************************************************************/

#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#include <PalmTypes.h>
#include <CoreTraps.h>


/************************************************************
 * Launcher procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// We're leaving the trap in place for now, but it just does a SysUIAppSwitch to
// launch the real launcher.  --Bob 21-Jul-00

void 		SysAppLauncherDialog()
					SYS_TRAP(sysTrapSysAppLauncherDialog);


#ifdef __cplusplus
}
#endif

#endif // __LAUNCHER_H__
