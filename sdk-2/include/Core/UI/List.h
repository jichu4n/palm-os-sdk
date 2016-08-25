/***********************************************************************
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     list.h
 * AUTHOR:	 Roger Flores: November 3, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines list structures and routines.
 *
 ***********************************************************************/

#ifndef __LIST_H__
#define __LIST_H__


#define noListSelection    		0x0ffff

//-------------------------------------------------------------------
// List structures
//-------------------------------------------------------------------

typedef struct {
	Word usable         :1;			// set if part of ui 
	Word enabled        :1;			// set if interactable (not grayed out)
	Word visible        :1;			// set if drawn
   Word poppedUp       :1;     	// set if choices displayed in popup win.
	Word hasScrollBar	  :1;			// set if the list has a scroll bar
	Word search			  :1;			// set if incremental search is enabled
   Word reserved       :2;
} ListAttrType;


// Load data callback routine prototype
typedef void ListDrawDataFuncType (Word itemNum, RectanglePtr bounds, 
	CharPtr *itemsText);
	
typedef ListDrawDataFuncType * ListDrawDataFuncPtr;


typedef struct {
	Word						id;
   RectangleType			bounds;
   ListAttrType			attr;
   CharPtr					*itemsText;
   Word						numItems;        		// number of choices in the list
   Word   					currentItem;     		// currently display choice
   Word   					topItem;         		// top item visible when poped up
   FontID           		font;						// font used to draw list
	WinHandle   			popupWin;				// used only by popup lists
   ListDrawDataFuncPtr	drawItemsCallback;	// 0 indicates no function
} ListType;

typedef ListType * ListPtr;


//-------------------------------------------------------------------
// List routines
//-------------------------------------------------------------------
extern void LstDrawList (ListPtr list)
							SYS_TRAP(sysTrapLstDrawList);

extern void LstEraseList (ListPtr list)
							SYS_TRAP(sysTrapLstEraseList);

extern Word LstGetSelection (ListPtr list)
							SYS_TRAP(sysTrapLstGetSelection);

extern CharPtr LstGetSelectionText (ListPtr list, Word itemNum)
							SYS_TRAP(sysTrapLstGetSelectionText);

extern Boolean LstHandleEvent (ListPtr list, EventPtr event)
							SYS_TRAP(sysTrapLstHandleEvent);

extern void LstSetHeight (ListPtr pList, Word  visibleItems)
							SYS_TRAP(sysTrapLstSetHeight);

extern void LstSetPosition (ListPtr pList, SWord x, SWord y)
							SYS_TRAP(sysTrapLstSetPosition);

extern void LstSetSelection (ListPtr list, Word itemNum)
							SYS_TRAP(sysTrapLstSetSelection);

extern void LstSetListChoices (ListPtr list, Char ** itemsText, 
	Word numItems)
							SYS_TRAP(sysTrapLstSetListChoices);

extern void LstSetDrawFunction (ListPtr pList, ListDrawDataFuncPtr func)
							SYS_TRAP(sysTrapLstSetDrawFunction);

extern void LstSetTopItem (ListPtr pList, Word itemNum)
							SYS_TRAP(sysTrapLstSetTopItem);

extern void LstMakeItemVisible (ListPtr pList, Word itemNum)
							SYS_TRAP(sysTrapLstMakeItemVisible);

extern Word LstGetNumberOfItems (ListPtr pList)
							SYS_TRAP(sysTrapLstGetNumberOfItems);

extern SWord LstPopupList (ListPtr list)
							SYS_TRAP(sysTrapLstPopupList);

extern Boolean LstScrollList(ListPtr pList, enum directions direction, short itemCount)
							SYS_TRAP(sysTrapLstScrollList);

extern Int LstGetVisibleItems (ListPtr pList)
							SYS_TRAP(sysTrapLstGetVisibleItems);
#endif // __LIST_H__
