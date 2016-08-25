/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Launcher.h
 * AUTHOR:	 Roger Flores: April 27, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  These are the routines for the launcher.
 *
 **********************************************************************/
#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#include <Common.h>
#include <DataMgr.h>
#include <Rect.h>
#include <Font.h>
#include <Window.h>

typedef struct {
	FormPtr			form;
   Int  				numItems; 			// numItems of applications available via the launcher
   Int				columns;
   Int				rows;
   Int				topItem;
   SByte				selection;
	VoidHand			appInfoH;			// an array is to be allocated
	TimeFormatType timeFormat;			// Format to display time in
	char				timeString[timeStringLength];
	FormPtr			savedForm;			// the currently active dialog.
} LauncherType;

typedef LauncherType * LauncherPtr;



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

