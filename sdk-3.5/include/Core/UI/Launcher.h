/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Launcher.h
 *
 * Description:
 *	  These are the routines for the launcher.
 *
 * History:
 *		April 27, 1995	Created by Roger Flores
 *
 *****************************************************************************/

#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <DataMgr.h>
#include <Rect.h>
#include <Font.h>
#include <Form.h>
#include <Window.h>

typedef struct {
	FormPtr			form;
   Int16  			numItems; 			// numItems of applications available via the launcher
   Int16				columns;
   Int16				rows;
   Int16				topItem;
   Int8				selection;
	UInt8				reserved;
	MemHandle		appInfoH;			// an array is to be allocated
	TimeFormatType timeFormat;			// Format to display time in
	char				timeString[timeStringLength];
	FormPtr			savedForm;			// the currently active dialog.
} LauncherType;

typedef LauncherType *LauncherPtr;



/************************************************************
 * Launcher procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void 		SysAppLauncherDialog()
					SYS_TRAP(sysTrapSysAppLauncherDialog);


#ifdef __cplusplus
}
#endif

#endif // __LAUNCHER_H__

