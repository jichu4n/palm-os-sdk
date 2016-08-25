/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
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


#define noListSelection    		0xffff

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
#ifdef __cplusplus
extern "C" {
#endif

extern void LstDrawList (const ListPtr list)
							SYS_TRAP(sysTrapLstDrawList);

extern void LstEraseList (const ListPtr list)
							SYS_TRAP(sysTrapLstEraseList);

extern Word LstGetSelection (const ListPtr list)
							SYS_TRAP(sysTrapLstGetSelection);

extern CharPtr LstGetSelectionText (const ListPtr list, 
	const Word itemNum)
							SYS_TRAP(sysTrapLstGetSelectionText);

extern Boolean LstHandleEvent (const ListPtr list, const EventPtr event)
							SYS_TRAP(sysTrapLstHandleEvent);

extern void LstSetHeight (const ListPtr pList, Word  visibleItems)
							SYS_TRAP(sysTrapLstSetHeight);

extern void LstSetPosition (const ListPtr pList, const SWord x, 
	const SWord y)
							SYS_TRAP(sysTrapLstSetPosition);

extern void LstSetSelection (const ListPtr list, const Word itemNum)
							SYS_TRAP(sysTrapLstSetSelection);

extern void LstSetListChoices (const ListPtr list, Char ** itemsText, 
	const Word numItems)
							SYS_TRAP(sysTrapLstSetListChoices);

extern void LstSetDrawFunction (const ListPtr pList, 
	const ListDrawDataFuncPtr func)
							SYS_TRAP(sysTrapLstSetDrawFunction);

extern void LstSetTopItem (const ListPtr pList, const Word itemNum)
							SYS_TRAP(sysTrapLstSetTopItem);

extern void LstMakeItemVisible (const ListPtr pList, const Word itemNum)
							SYS_TRAP(sysTrapLstMakeItemVisible);

extern Word LstGetNumberOfItems (const ListPtr pList)
							SYS_TRAP(sysTrapLstGetNumberOfItems);

extern SWord LstPopupList (const ListPtr list)
							SYS_TRAP(sysTrapLstPopupList);

extern Boolean LstScrollList(const ListPtr pList, 
	const enum directions direction, short itemCount)
							SYS_TRAP(sysTrapLstScrollList);

extern Int LstGetVisibleItems (const ListPtr pList)
							SYS_TRAP(sysTrapLstGetVisibleItems);

extern Err LstNewList (VoidPtr *formPP, const Word id, 
	const Word x, const Word y, const Word width, const Word height, 
	const FontID font, const Word visibleItems, const Word triggerId)
							SYS_TRAP(sysTrapLstNewList);



#ifdef __cplusplus 
}
#endif

#endif // __LIST_H__
