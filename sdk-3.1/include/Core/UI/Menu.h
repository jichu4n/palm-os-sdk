/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     menu.h
 * AUTHOR:	 Roger Flores: November 18, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines menu structures and routines.
 *
 ***********************************************************************/

#ifndef __MENU_H__
#define __MENU_H__


#define noMenuSelection			-1
#define noMenuItemSelection	-1
#define separatorItemSelection	-2


// To match Apple's ResEdit the first byte of a menu item's text can
// be a special char indicating a special menu item.
#define MenuSeparatorChar			'-'



typedef struct {
	Word						id;				// id of the menu item
	Char						command;			// command key
	CharPtr					itemStr;			// string to be displayed
} MenuItemType;


typedef struct {
	WinHandle				menuWin;			// window of pull-down menu
	RectangleType			bounds;			// bounds of the pulldown
	WinHandle				bitsBehind;		// saving bits behind pull-down menu
	RectangleType			titleBounds;	// bounds of the title in menu bar
	CharPtr					title;			// menu title displayed in menu bar
	Word						numItems;		// number of items in the menu
	MenuItemType *			items;			// array of menu items
} MenuPullDownType;

typedef MenuPullDownType * MenuPullDownPtr;

typedef struct {
	Word visible			:1;			// Set if menu bar is drawn
	Word commandPending	:1;			// Set if next key is a command
	Word insPtEnabled	:1;			// Set if insPt was on when menu was drawn
} MenuBarAttrType;


typedef struct {
	WinHandle				barWin;			// window of menu bar
	WinHandle				bitsBehind;		// saving bits behind menu bar
	WinHandle				savedActiveWin;
	WinHandle				bitsBehindStatus;
	MenuBarAttrType 		attr;
	SWord						curMenu;			// current menu or -1 if none
	SWord						curItem;			// current item in curMenu, -1 if none
	SDWord					commandTick;	//
	SWord						numMenus;		// number of menus
	MenuPullDownPtr  		menus;			// array of menus
} MenuBarType;

typedef MenuBarType * MenuBarPtr;


#ifdef __cplusplus
extern "C" {
#endif

extern MenuBarPtr MenuInit (const Word resourceId)
							SYS_TRAP(sysTrapMenuInit);

extern MenuBarPtr MenuGetActiveMenu (void)
							SYS_TRAP(sysTrapMenuGetActiveMenu);

extern MenuBarPtr MenuSetActiveMenu (const MenuBarPtr pMenu)
							SYS_TRAP(sysTrapMenuSetActiveMenu);

extern void MenuDispose (const MenuBarPtr pMenu)
							SYS_TRAP(sysTrapMenuDispose);

extern Boolean MenuHandleEvent (MenuBarPtr pMenu, const EventPtr event,
	const WordPtr error)
							SYS_TRAP(sysTrapMenuHandleEvent);

extern void MenuDrawMenu (const MenuBarPtr pMenu)
							SYS_TRAP(sysTrapMenuDrawMenu);

extern void MenuEraseStatus (const MenuBarPtr pMenu)
							SYS_TRAP(sysTrapMenuEraseStatus);

extern void MenuSetActiveMenuRscID (const Word resourceId)
							SYS_TRAP(sysTrapMenuSetActiveMenuRscID);

#ifdef __cplusplus 
}
#endif

#endif //__MENU_H__
