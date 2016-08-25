/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1995 -- All Rights Reserved
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
	Char			name[dmDBNameLength];
	ULong			creator;
	UInt			version;
	LocalID		dbID;
	UInt 			cardNo;
	BitmapPtr	iconP;
} LauncherAppInfoType;
	
typedef struct {
   RectangleType	bounds;
   Int  				numItems; 			// numItems of applications available via the launcher
   Int				columns;
   Int				rows;
   Int				topItem;
	VoidHand			appInfoH;			// an array is to be allocated
	RectangleType	frame;
	WinHandle   	popupWin;
	WinHandle   	savedBits;
	WinHandle   	savedActiveWin;
	WinHandle   	savedDrawWin;
	TimeFormatType timeFormat;			// Format to display time in
	char				timeString[timeStringLength];
	Boolean			needToGetNamesAndIcons;
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

