/***********************************************************************
 *
 * Copyright (c) 2006 Palm, Inc.
 * All rights reserved.
 *
 * PROJECT:  NavExample
 * FILE:     NavExample.c
 *
 *
 * DESCRIPTION: This sample code demonstrates how the 5-way navigation
 * 				key works programmatically
 *	  
 **********************************************************************/

#include <PalmOS.h> // to dismiss GCC build complain
#include <HsExt.h>
//#include <HsNav.h>
//#include <Form.h>
#include "NavExample.h"



// Define the minimum OS version we support
#define ourMinVersion	sysMakeROMVersion(2,0,0,sysROMStageRelease,0)

/***********************************************************************
 *
 *	Private Enums & Types
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/
static AppPrefs gAppPrefs;

static MemHandle gDbArrayH = 0;
static UInt32 gDbArrayCount = 0;
static Boolean TableEditState = false;

/***********************************************************************
 *
 *	Protos
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Private Functions
 *
 ***********************************************************************/


/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
#define LAUNCHFLAGMASK (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)

static Err 
PrvRomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags)
{
  UInt32 romVersion;
  
  // See if we're on in minimum required version of the ROM or later.
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
  if (romVersion < requiredVersion)
	{
	  if ((launchFlags & LAUNCHFLAGMASK) == LAUNCHFLAGMASK)
		{
		  FrmAlert(rscRomIncompatibleAlert);
	  
		  // Pilot 1.0 will continuously relaunch this app 
		  // unless we switch to another safe one.
		  if (romVersion < sysMakeROMVersion(2, 0, 0, 
				  sysROMStageRelease, 0))
			{
			  AppLaunchWithCommand(sysFileCDefaultApp, 
				  sysAppLaunchCmdNormalLaunch, NULL);
			}
		}
	  
	  return sysErrRomIncompatible;
	}
  
  return 0;
}

/***************************************************************
 *	Function:	  PrvGetPrefs
 *
 *	Summary:	  This routine initializes the application 
 *				  preferences structure.
 *
 *	Parameters:
 *	  none
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ****************************************************************/
static Err 
PrvGetPrefs (AppPrefs *prefsP)
{
  AppPrefs		buf;
  UInt16		bufSize;

  // Read the saved preferences / saved-state information.
  bufSize = sizeof(buf);
  if (PrefGetAppPreferences (appFileCreator, appPrefID, 
	  &buf, &bufSize, true) != noPreferenceFound)
	{
	  MemMove (prefsP, &buf, sizeof (AppPrefs));
	}
  else
	{
	  MemSet (prefsP, sizeof (AppPrefs), 0);

	  prefsP->showItem1 = true;
	  prefsP->showItem2 = true;
	  prefsP->showItem3 = true;
	  prefsP->showItem4 = true;
	  prefsP->showItem5 = true;

	  prefsP->type = 'appl';
	}

  return 0;
}

/***************************************************************
 *	Function:	  PrvSetPrefs
 *
 *	Summary:	  This routine stores the application 
 *				  preferences in the preferences database.
 *
 *	Parameters:
 *	  none
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ****************************************************************/
static Err 
PrvSetPrefs (AppPrefs *prefsP)
{
  PrefSetAppPreferences (appFileCreator, appPrefID, 
		  appPrefVersionNum, prefsP, sizeof(AppPrefs), true);
  return 0;
}

/***************************************************************
 *	Function:	  PrvUpdatePrefControls
 *
 *	Summary:	  This function updates the preferences checkboxes.
 *
 *	Parameters:
 *	  prefsP	  IN	applicaiton preferences structure
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ****************************************************************/
static void
PrvUpdatePrefControls (AppPrefs *prefsP)
{
  FormPtr frmP;

  frmP = FrmGetActiveForm ();

  SetControlVal(frmP,rscPreferencesItem1Checkbox,prefsP->showItem1);
  SetControlVal(frmP,rscPreferencesItem2Checkbox,prefsP->showItem2);
  SetControlVal(frmP,rscPreferencesItem3Checkbox,prefsP->showItem3);
  SetControlVal(frmP,rscPreferencesItem4Checkbox,prefsP->showItem4);
  SetControlVal(frmP,rscPreferencesItem5Checkbox,prefsP->showItem5);

  FrmDrawForm (frmP);
}

/***************************************************************
 *	Function:	  PrvGetPrefControlSatus
 *
 *	Summary:	  This function gets the status of the 
 *				  preferences checkboxes.
 *
 *	Parameters:
 *	  prefsP	  OUT	applicaiton preferences structure
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ****************************************************************/
static void
PrvGetPrefControlSatus (AppPrefs *prefsP)
{
  FormPtr frmP;

  frmP = FrmGetActiveForm ();

  prefsP->showItem1 = (GetControlVal(frmP,rscPreferencesItem1Checkbox) != 0);
  prefsP->showItem2 = (GetControlVal(frmP,rscPreferencesItem2Checkbox) != 0);
  prefsP->showItem3 = (GetControlVal(frmP,rscPreferencesItem3Checkbox) != 0);
  prefsP->showItem4 = (GetControlVal(frmP,rscPreferencesItem4Checkbox) != 0);
  prefsP->showItem5 = (GetControlVal(frmP,rscPreferencesItem5Checkbox) != 0);
}

/***************************************************************
 *	Function:	  PrefFormEventHandler
 *
 *	Summary:	  The event handler for the preferences form.
 *
 *	Parameters:
 *	  eventP	  IN	event pointer
 *
 *	Returns:
 *	  true if handled.
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ****************************************************************/
static Boolean
PrefFormEventHandler (EventPtr eventP)
{
  Boolean handled = false;

  switch (eventP->eType)
	{
	  case frmOpenEvent:
		PrvUpdatePrefControls (&gAppPrefs);
		handled = true;
		break;

	  case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
		  {
			case rscPreferencesOKButton:
			  //
			  // save the app preferences to the prefs database.
			  //
			  PrvGetPrefControlSatus (&gAppPrefs);
			  PrvSetPrefs (&gAppPrefs);
			  FrmReturnToForm (0);
			  handled = true;
			  break;

			case rscPreferencesCancelButton:
			  FrmReturnToForm (0);
			  handled = true;
			  break;

			default:
			  break;
		  }
		break;

	  default:
		break;
	}

  return handled;
}

static Int16
PrvSaveDatabaseListSelection (ListPtr lstP)
{
  Int16 curSel;
  Int32 len;
  SysDBListItemType* arrayP;

  curSel = LstGetSelection (lstP);
  if (curSel != noListSelection)
	{
	  arrayP = MemHandleLock (gDbArrayH);
	  len = min (StrLen (arrayP[curSel].name), sizeof (gAppPrefs.name)-1);
	  StrNCopy (gAppPrefs.name, arrayP[curSel].name, len);
	  gAppPrefs.name[len] = 0;
	  MemHandleUnlock (gDbArrayH);
	}

  return curSel;
}

/***********************************************************************
 *	Function:	  PrvSetDatabaseListSelection
 *
 *	Summary:	  Set the selected item in the Database List view based
 *				  name or partial match.
 *
 *	Parameters:
 *	  lstP		IN	  list pointer
 *
 *	Returns:
 *	  Index of selected item.
 *	
 *	Called By: 
 *	  PrvSetPopupSelection ()
 *	  HandlePopup ()
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static Int16
PrvSetDatabaseListSelection (ListPtr lstP, Char *name, Int32 len, Boolean caseSensitive)
{
  SysDBListItemType* arrayP;
  Int16 selection;
  Int16 val;
  
  selection = 0;

  if (len == 0)
	{
	  len = StrLen (name);
	  if (len == 0)
		{
		  goto Exit;
		}
	}

  arrayP = MemHandleLock (gDbArrayH);

  if (caseSensitive)
	{
	  while (selection < (gDbArrayCount-1))
		{
		  val = StrNCompare (name, arrayP[selection].name, len);
		  if(val == 0)
			{
			  break;
			}
		  else if (val < 0)
			{
			  selection--;
			  break;
			}
		  selection++;
		}
	}
  else
	{
	  while (selection < (gDbArrayCount-1))
		{
		  val = StrNCaselessCompare (name, arrayP[selection].name, len);
		  if(val == 0)
			{
			  break;
			}
		  else if (val < 0)
			{
			  selection--;
			  break;
			}
		  selection++;
		}
	}

  MemHandleUnlock (gDbArrayH);

Exit:

  LstSetSelection (lstP, selection);

  return selection;
}

/***********************************************************************
 *
 * FUNCTION:    PrvDrawListFunc
 *
 * DESCRIPTION: Draws the text within a list object.  It prevents text
 *				from overrunning the bounding box.
 *
 * PARAMETERS:  itemNum -- the item number in the list.
 *				bounds -- the size of the bounding box.
 *				itemsText -- not used.
 *
 * RETURNED:    void
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static void
PrvDrawListFunc(Int16 itemNum, RectangleType *boundsP, Char **itemsText)
{
  Int16 length;
  Int16 width;
  SysDBListItemType *arrayP;
  
  arrayP = MemHandleLock (gDbArrayH);
  
  // Truncate the item with an ellipsis if it doesnt fit 
  // in the list width. Get the item text length.
  length = StrLen(arrayP[itemNum].name);
  
  // Get the width of the text
  width = FntCharsWidth(arrayP[itemNum].name, length);
  
  // Does it fit?
  if (width <= boundsP->extent.x)
	{
	  // Draw entire item text as is
	  WinDrawChars(arrayP[itemNum].name, length,
	  		  boundsP->topLeft.x, boundsP->topLeft.y);
	}
  else
	{
	  Boolean ignored;
	  Char ellipsis;
	  
	  ellipsis = chrEllipsis;
	  
	  // Set the new max item width
	  width = boundsP->extent.x - FntCharWidth(ellipsis);
  
	  // Find the item length that fits in the bounds minus the ellipsis
	  FntCharsInWidth(arrayP[itemNum].name, &width, &length, &ignored);
  
	  // Draw item text that fits
	  WinDrawChars(arrayP[itemNum].name, length, 
			  boundsP->topLeft.x, boundsP->topLeft.y);
  
	  // Draw ellipsis char
	  WinDrawChars(&ellipsis, 1, 
			  boundsP->topLeft.x + width, boundsP->topLeft.y);
	}

  MemHandleUnlock (gDbArrayH);
  
  return;
}


/***********************************************************************
 *	Function:	  PrvCompareDatabseListElementFunc
 *
 *	Summary:	  Comparison function used for sorting the database
 *				  array.
 *
 *	Parameters:
 *	  p1	 IN	  item 1.
 *	  p2	 IN	  item 2.
 *	  weight IN	  integer weighting
 *
 *	Returns:
 *	  returns 0 if p1 == p2, weight if p1 > p2, or -weight otherwise.
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created
 *    2007-11-29  CGW modified to satisfy coverity
 *
 ***********************************************************************/
static Int16 
PrvCompareDatabseListElementFunc (void *p1, void *p2, Int32 weight)
{
  SysDBListItemType* item1P;
  SysDBListItemType* item2P;
  Int16	rel = 0 ; 

  item1P = (SysDBListItemType*)p1;
  item2P = (SysDBListItemType*)p2;
  
  rel = StrCompare (item1P->name, item2P->name);
  if (rel == 0) return 0;
  return rel * weight;
}

/***********************************************************************
 *	Function:	  PrvDeleteDatabaseList
 *
 *	Summary:	  Deallocated the database array
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  returns 0 if no error
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static Err
PrvDeleteDatabaseList (void)
{
  if (gDbArrayH)
	{
	  MemHandleFree (gDbArrayH);
	  gDbArrayH = 0;
	  gDbArrayCount = 0;
	}

  return 0;
}

/***********************************************************************
 *	Function:	  PrvCreateDatabaseList
 *
 *	Summary:	  Create the database array based on the type of
 *				  database we want to view.
 *
 *	Parameters:
 *	  type		IN	  type of resource database to list
 *
 *	Returns:
 *	  returns 0 if no error
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static Err
PrvCreateDatabaseList (UInt32 type)
{
  Err err = 0;
  SysDBListItemType* dbInfoP;
  SysDBListItemType* dbArrayP;
  MemHandle dbInfoH;
  UInt16 i;
  UInt16 count;
  UInt16 attr;

  PrvDeleteDatabaseList ();

  if (!SysCreateDataBaseList(type, 0, &count, &dbInfoH, false))
	{
	  return memErrNotEnoughSpace;
	}

  gDbArrayH = MemHandleNew (sizeof (SysDBListItemType) * count);
  if (!gDbArrayH)
	{
	  MemHandleFree (dbInfoH);
	  return memErrNotEnoughSpace;
	}

  dbInfoP = MemHandleLock (dbInfoH);
  dbArrayP = MemHandleLock (gDbArrayH);
  
  for (i = 0; i < count; i++)
	{
	  UInt32 recType;

	  err = DmDatabaseInfo (dbInfoP[i].cardNo, dbInfoP[i].dbID, 
				NULL, &attr, NULL, NULL, NULL, NULL, NULL, NULL, 
				NULL, &recType, NULL);

	  if (err || !(attr & dmHdrAttrResDB))
		{
		  // If there was an error getting the database info, 
		  // or if it is the wrong kind of resource, 
		  // skip it and continue.
		  continue;
		}

	  if ((type == 'ovly') && (type != recType))
		{
		  // This is to get all overlays.
		  continue;
		}

	  MemMove (&dbArrayP[gDbArrayCount], &dbInfoP[i], sizeof (SysDBListItemType));
	  gDbArrayCount++;
	}

  MemHandleUnlock (dbInfoH);
  MemHandleFree (dbInfoH);

  SysQSort (dbArrayP, gDbArrayCount, sizeof (SysDBListItemType), 
	  PrvCompareDatabseListElementFunc, 1);

  MemHandleUnlock (gDbArrayH);

  return err;
}

/***********************************************************************
 *	Function:	  PrvScrollDatabaseList
 *
 *	Summary:	  Scroll the database list to show selected item.
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static void
PrvScrollDatabaseList (FormPtr frmP)
{
  ListPtr lstP;
  Int16 curSel;

  lstP = GetObjectPtr (frmP, rscMainDBList);

  curSel = LstGetSelection (lstP);
  LstMakeItemVisible (lstP, curSel);
}

/***********************************************************************
 *	Function:	  PrvDisplayDatabaseList
 *
 *	Summary:	  Show the database.
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static Err
PrvDisplayDatabaseList (FormPtr frmP)
{
  ListPtr lstP;
  Int16 curSel;

  lstP = GetObjectPtr (frmP, rscMainDBList);

  if (FrmVisible (frmP))
	LstEraseList(lstP);
  if (gDbArrayCount)
	{
	  LstSetListChoices(lstP, NULL, gDbArrayCount);
	  LstSetDrawFunction(lstP, PrvDrawListFunc);
	  curSel = PrvSetDatabaseListSelection (lstP, gAppPrefs.name, 0, true);
	  if (FrmVisible (frmP))
		LstMakeItemVisible(lstP, curSel);
	}
  else
	{
	  FrmAlert(rscNoDatabasesAlert);
	  LstSetListChoices(lstP, NULL, 0);
	  LstSetSelection(lstP, noListSelection);
	}

  return 0;
}

/***********************************************************************
 *	Function:	  PrvUpdateDatabaseList
 *
 *	Summary:	  Update the list of resource databases in the list-view.
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	  PrvSetPopupSelection ()
 *	  HandlePopup ()
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static Err
PrvUpdateDatabaseList (FormPtr frmP)
{
  ControlPtr ctlP;

  ctlP = GetObjectPtr (frmP, rscMainOpenButton);
  
  // Clear out the display while
  // we update the lists.
  CtlEraseControl(ctlP);
  CtlSetEnabled(ctlP, false);

  PrvCreateDatabaseList (gAppPrefs.type);
  PrvDisplayDatabaseList (frmP);

  CtlSetEnabled(ctlP, true);
  if (FrmVisible (frmP))
	CtlShowControl(ctlP);

  return 0;
}

/***********************************************************************
 *	Function:	  PrvDoDatabaseListAction
 *
 *	Summary:	  Do an action based on a particular key-press.
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	  MainFrmEventHandler ()
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static Boolean
PrvDoDatabaseListAction (FormPtr frmP, UInt16 c)
{
  Boolean handled = false;
  ListPtr lstP;
  Int16 selection;
  Char buf[2] = {'c', '0'};

  lstP = GetObjectPtr (frmP, rscMainDBList);

  switch (c)
	{
	  case vchrRockerUp:
	  case pageUpChr:
		selection = LstGetSelection (lstP);
		if (selection == noListSelection)
		  {
			// If no item is selected, then select
			// the last one.
			selection = gDbArrayCount - 1;
		  }
		else
		  {
			// Select the previous list item.
			if (selection > 0)
			  {
				LstSetSelection (lstP, selection - 1);
			  }
		  }
		handled = true;
		break;

	  case vchrRockerDown:
	  case pageDownChr:
		selection = LstGetSelection (lstP);
		if (selection == noListSelection)
		  {
			// If no item is selection, then select
			// the first one.
			selection = 0;
		  }
		else
		  {
			// Select the next list item.
			if (selection < (gDbArrayCount - 1))
			  {
				LstSetSelection (lstP, selection + 1);
			  }
		  }
		handled = true;
		break;

	  case vchrRockerCenter:
		FrmAlert (rscAlertOpen);  // this is where we would open the database.
		handled = true;
		break;

	  default:
		//
		// Jump to a selection based on an acsii character.
		//
		if ((((Char)c & ~0x20) >= 'A') && (((Char)c & ~0x20)<= 'Z'))
		  {
			//Char buf[2] = {c, 0};
			PrvSetDatabaseListSelection (lstP, buf, 1, false);
			PrvSaveDatabaseListSelection (lstP);
			handled = true;
		  }
		break;
	}

  return handled;
}

/***********************************************************************
 *
 * FUNCTION:    HandlePopup
 *
 * DESCRIPTION: This function manages the state of the popup button and
 *				its associated list.
 *
 * PARAMETERS:  eventPtr -- a reference to the event structure.
 *				frmPtr -- a reference to the main form.
 *
 * RETURNED:    Returns true if the event was handled.
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static Boolean
HandlePopup (EventPtr eventP, FormPtr frmP)
{
  ListPtr lstP;
  ListPtr popP;
  UInt16 selected;
  UInt16 oldSelected;
  
  // add return check for coverity check
  lstP = GetObjectPtr (frmP, rscMainDBList);
  if (lstP == NULL)
  	return false;
 	
  // add return check for coverity check
  popP = GetObjectPtr (frmP, rscMainTypeList);
  if (popP == NULL)
  	return false;
  
  oldSelected = LstGetSelection(popP);
  selected = LstPopupList(popP);
  
  // Make we sure we selected something, and if
  // so, make sure it's something new.
  if ( (selected != -1 ) && 
	   (selected != oldSelected) )
	{
	  Char *txtP;
	  ControlPtr ctlP;
	  
	  txtP = LstGetSelectionText(popP, selected);
	  ctlP = eventP->data.ctlSelect.pControl;
	  CtlSetLabel(ctlP, txtP);

	  // If 'All' was selected, treat it
	  // in a 'special' way.
	  if ( !StrCompare(txtP, "All") )
		{
		  gAppPrefs.type = 0;
		}
	  else
		{
		  StrNCopy((Char *)&gAppPrefs.type, txtP, 4);
		}
	}

  PrvUpdateDatabaseList (frmP);
  FrmDrawForm (frmP);
			    
  return true;
}

/***********************************************************************
 *	Function:	  PrvSetPopupSelection
 *
 *	Summary:	  Set the selected item in the popup list
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	  MainFormInit ()
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static void
PrvSetPopupSelection (FormPtr frmP)
{
  ListPtr lstP;
  ControlPtr ctlP;
  Boolean found;
  UInt16 count;
  Char *strP; 	
  Char string[5];

  count = 0;
  found = false;
  
  lstP = GetObjectPtr (frmP, rscMainTypeList);

  if (gAppPrefs.type != 0)
	{
	  StrNCopy(string, (Char *)&gAppPrefs.type, 4);
	  string[4] = '\0';
  
	  // Search the popup list for the type of the
	  // resource viewed during the previous session.
	  // If found, select that type of resource and
	  // display them in the list-view.  If not, use
	  // the default type, which in this case is All.
	  count = LstGetNumberOfItems(lstP);
	  while (count--)
		{
		  strP = LstGetSelectionText(lstP, count);
		  if (!StrCompare(strP, string))
		  	{
		  	  LstSetSelection(lstP, count);
		  	  found = true;
		  	  break;
		  	}
		}
	}
  
  if (!found)
	{
	  gAppPrefs.type = 0;
	  LstSetSelection(lstP, 0);
	  count = 0;
	}
  
  ctlP = GetObjectPtr(frmP, rscMainTypePopTrigger);
  
  strP = LstGetSelectionText(lstP, count);
  CtlSetLabel(ctlP, strP);
}

/***********************************************************************
 *	Function:	  MainFrmInit
 *
 *	Summary:	  This routine initializes the "Main View" of our app
 *
 *	Parameters:
 *	  frmP		IN	  form pointer
 *
 *	Returns:
 *	  void
 *	
 *	Called By: 
 *	  MainFrmEventHandler()
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ***********************************************************************/
static void 
MainFrmInit (FormPtr frmP)
{
  PrvSetPopupSelection (frmP);
  PrvUpdateDatabaseList (frmP);
  FrmSetMenu (frmP, rscMainMenu);
}

/***********************************************************************
 *	Function:	 TableFrmEventHandler
 *
 *	Summary:	  Event handler for the table form. 
 *
 *	Parameters:
 *	  eventP	  IN	pointer to event to handle
 *
 *	Returns:
 *	  true if event was handled
 *	
 *	Called By: 
 *	  FrmDispatchEvent() when event is for our main form. 
 *	  FrmDispatchEvent() is called by our AppEventLoop() 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV  created 
 *
 ***********************************************************************/
static Boolean 
TableFrmEventHandler (EventPtr eventP)
{
  FormPtr   frmP;
  Boolean   handled = false;
  TablePtr  tableP = NULL;
  Int16     row = 0, numRows = 0;  
  UInt16    focusedIndex = 0;
  FieldType *fld = NULL;
  Int16     column = 0;
  UInt16  	cardNo;
  LocalID	dbID;
  DmSearchStateType searchState;
    	    	          
  switch (eventP->eType) 
	{
	  // -------------------------------------------------------
	  // Init the form
	  // -------------------------------------------------------
	  case frmOpenEvent:
		frmP = FrmGetActiveForm ();
		tableP = GetObjectPtr (frmP, rscTableSampleTable);		
        numRows = TblGetNumberOfRows(tableP);
        
        TblSetColumnUsable(tableP, 0, true);
        TblSetColumnUsable(tableP, 1, true);
        TblSetColumnUsable(tableP, 2, true);
        
        for (row = 0; row < numRows; row++)
        {
            TblSetRowUsable(tableP, row, true);

            TblSetItemStyle(tableP, row, 0, numericTableItem);
            TblSetItemInt(tableP, row, 0, row);
            
            TblSetItemStyle(tableP, row, 1, textTableItem);
            TblSetItemStyle(tableP, row, 2, textTableItem);
        }


		FrmDrawForm (frmP);
		FrmSetNavEntry(frmP, rscTableSampleTable, 0, 0, rscTableMainButton, kFrmNavObjectFlagsForceInteractionMode);
		FrmSetNavEntry(frmP, rscTableMainButton, rscTableSampleTable, rscTableSampleTable, 0, kFrmNavObjectFlagsIsBigButton);
		FrmNavObjectTakeFocus(frmP, rscTableSampleTable)	;
		handled = true;
		break;

	  // -------------------------------------------------------
	  // Menu commands
	  // -------------------------------------------------------
	  case menuEvent:
		{
		  switch (eventP->data.menu.itemID) 
			{
			  case rscMenuItemPreferences:
				FrmPopupForm (rscPreferencesForm);
				handled = true;
				break;

			  case rscMenuItemAbout:
				DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appFileCreator, true, &cardNo, &dbID);
				MenuEraseStatus(0);
				HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
				handled = true;
	
				break;

			  default:
				break;
			}
		  }
		break;

	  // -------------------------------------------------------
	  // Buttons
	  // -------------------------------------------------------
	  case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
		  {
			case rscTableMainButton:
              FrmGotoForm(rscMainForm);
			  handled = true;
			  break;

			default:
			  break;
		  }
		break;

	  case frmUpdateEvent:
		  break;

	  // -------------------------------------------------------
	  // Close our form
	  // -------------------------------------------------------
	  case frmCloseEvent:
		break;
		
	  case frmObjectFocusTakeEvent:
	    if (eventP->data.frmObjectFocusTake.objectID == rscTableSampleTable)
	    {
	        RectangleType rect = {{2, 18}, {156, 121}};
	        
    	    frmP = FrmGetActiveForm();
    	    FrmNavDrawFocusRing(frmP, rscTableSampleTable, frmNavFocusRingNoExtraInfo, &rect, frmNavFocusRingStyleObjectTypeDefault, true);
       	    FrmSetFocus(frmP, FrmGetObjectIndex(frmP, rscTableSampleTable));
    	    handled = true;
    	}
	    break;
	  
	  case frmObjectFocusLostEvent:
	    //handled = true;
	    break;
	    
	  case keyDownEvent:
	    
        frmP = FrmGetActiveForm();
        tableP = GetObjectPtr (frmP, rscTableSampleTable);
        focusedIndex = FrmGetFocus(frmP);

   	    if (eventP->data.keyDown.chr == vchrRockerCenter)
   	    {
            if (focusedIndex == FrmGetObjectIndex(frmP, rscTableSampleTable))
            {
    	        if (!TableEditState)
    	        {
    	            TableEditState = true;
    	            TblGrabFocus (tableP, 0, 1);
    	            fld = TblGetCurrentField (tableP); 
                    FldSetInsPtPosition (fld, 0); 
                    FldGrabFocus (fld);
                    
                    FrmNavRemoveFocusRing(frmP);
           	        handled = true;
    	        }
    	    }
    	}
    	else if (TableEditState)
    	{
    	    if(!TblGetSelection(tableP, &row, &column)){
	    	    if (eventP->data.keyDown.chr == vchrRockerLeft)
	        	{
	        	    TblReleaseFocus(tableP);
	        	    if (column == 1)
	        	    {
	        	        TableEditState = false;
	        	    }
	        	    else
	        	    {
	        	        TblGrabFocus(tableP, row, column - 1);
	           	        handled = true;
	        	    }
	        	}
	        	else if (eventP->data.keyDown.chr == vchrRockerRight)
	        	{
	        	    TblReleaseFocus(tableP);
	        	    if (column == 2)
	        	    {
	           	        TableEditState = false;
	        	    }
	        	    else
	        	    {
	        	        TblGrabFocus(tableP, row, column + 1);
	           	        handled = true;
	        	    }
	        	}
	        	else if (eventP->data.keyDown.chr == vchrRockerUp)
	        	{
	        	    TblReleaseFocus(tableP);
	        	    if (row == 0)
	        	    {
	        	        TableEditState = false;
	        	    }
	        	    else
	        	    {
	        	        TblGrabFocus(tableP, row - 1, column);
	           	        handled = true;
	        	    }
	        	}
	        	else if (eventP->data.keyDown.chr == vchrRockerDown)
	        	{
	        	    TblReleaseFocus(tableP);
	        	    if (row == 10)
	        	    {
	        	        TableEditState = false;
	        	    }
	        	    else
	        	    {
	        	        TblGrabFocus(tableP, row + 1, column);
	           	        handled = true;
	        	    }
	        	}
			}        	
        	if (handled)
        	{
    	        fld = TblGetCurrentField (tableP); 
                FldSetInsPtPosition (fld, 0); 
                FldGrabFocus (fld); 
            }        	
        }
	    break;

	  default:
		break;
	}

  return handled;
}



/***********************************************************************
 *	Function:	  MainFrmEventHandler
 *
 *	Summary:	  Event handler for our main form. 
 *
 *	Parameters:
 *	  eventP	  IN	pointer to event to handle
 *
 *	Returns:
 *	  true if event was handled
 *	
 *	Called By: 
 *	  FrmDispatchEvent() when event is for our main form. 
 *	  FrmDispatchEvent() is called by our AppEventLoop() 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV  created 
 *
 ***********************************************************************/
static Boolean 
MainFrmEventHandler (EventPtr eventP)
{
  FormPtr   frmP;
  Boolean   handled = false;
  UInt16  	cardNo;
  LocalID	dbID;
  DmSearchStateType searchState;

  switch (eventP->eType) 
	{
	  // -------------------------------------------------------
	  // Init the form
	  // -------------------------------------------------------
	  case frmOpenEvent:
		frmP = FrmGetActiveForm ();
		MainFrmInit (frmP);
		FrmDrawForm (frmP);
		PrvScrollDatabaseList (frmP);
		handled = true;
		break;

	  // -------------------------------------------------------
	  // Menu commands
	  // -------------------------------------------------------
	  case menuEvent:
		{
		  switch (eventP->data.menu.itemID) 
			{
			  case rscMenuItemPreferences:
				FrmPopupForm (rscPreferencesForm);
				handled = true;
				break;

			  case rscMenuItemAbout:
				DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appFileCreator, true, &cardNo, &dbID);
				MenuEraseStatus(0);
				HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
				
				handled = true;
				break;

			  default:
				break;
			}
		  }
		break;

	  // -------------------------------------------------------
	  // Key press
	  // -------------------------------------------------------
	  case keyDownEvent:
		frmP = FrmGetActiveForm ();
		if (ObjectHasFocus (frmP, rscMainDBList))
		  {
			handled = PrvDoDatabaseListAction (frmP, eventP->data.keyDown.chr);
		  }
		break;

	  // -------------------------------------------------------
	  // Buttons
	  // -------------------------------------------------------
	  case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
		  {
			case rscMainOpenButton:
			  //frmP = FrmGetActiveForm ();
			  FrmGetActiveForm ();
			  FrmAlert (rscAlertOpen);  // this is where we would open the database...
			  handled = true;
			  break;

		    case rscMainTypePopTrigger:
			  frmP = FrmGetActiveForm ();
			  handled = HandlePopup(eventP, frmP);
			  break;
			  
			case rscMainTableButton:
			  FrmGotoForm(rscTableForm);
			  handled = true;
			  break;  

			default:
			  break;
		  }
		break;

	  case frmUpdateEvent:
		  frmP = FrmGetActiveForm ();
		  FrmSetMenu (frmP, rscMainMenu);
		  break;

	  // -------------------------------------------------------
	  // Close our form
	  // -------------------------------------------------------
	  case frmCloseEvent:
		PrvDeleteDatabaseList ();
		break;

	  default:
		break;
	}

  return handled;
}


/***********************************************************************
 *	Function:	  AppStart
 *
 *	Summary:	  Application initialization when being launched
 *		as a normal application (i.e. not to execute an action code).
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PilotMain when action code is for a normal launch. 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV  created 
 *
 ***********************************************************************/
static Err 
AppStart(void)
{
  PrvGetPrefs (&gAppPrefs);
  FrmGotoForm (rscMainForm);

  return 0;
}

/***********************************************************************
 *	Function:	  AppStop
 *
 *	Summary:	  Application cleanup. This routine cleans up
 *	  all initialization performed by AppStart(). 
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PilotMain, after EventLoop returns
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV  created 
 *
 ***********************************************************************/
static void 
AppStop (void)
{
  PrvSetPrefs (&gAppPrefs);
  FrmCloseAllForms();
}

/***********************************************************************
 *	Function:	  AppEventLoop
 *
 *	Summary:	  This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 *	Parameters:
 *	  eventP	IN	  pointer to event
 *
 *	Returns:
 *	  true if event was handled. 
 *	
 *	Called By: 
 *	  AppEventLoop. 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV  created 
 *
 ***********************************************************************/
static Boolean 
AppHandleEvent (EventPtr eventP)
{
  UInt16 formID;
  FormPtr frmP;
  Boolean handled = false;

  switch (eventP->eType) 
	{
	  case frmLoadEvent:
		// Load the form resource.
		formID = eventP->data.frmLoad.formID;
		frmP = FrmInitForm (formID);
		FrmSetActiveForm (frmP);		
		
		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formID) 
		  {
			case rscMainForm:
			  FrmSetEventHandler (frmP, MainFrmEventHandler);
			  handled = true;
			  break;

			case rscTableForm:
			  FrmSetEventHandler (frmP, TableFrmEventHandler);
			  handled = true;
			  break;
			  
			case rscPreferencesForm:
			  FrmSetEventHandler (frmP, PrefFormEventHandler);
			  handled = true;
			  break;

			default:
			  break;
		  }
		break;

	  default:
		break;
	}

  return handled;
}

/***********************************************************************
 *	Function:	  AppEventLoop
 *
 *	Summary:	  Main event loop for the application
 *
 *	Parameters:
 *	  void
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PilotMain after initializing. 
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV  Ceated 
 *
 ***********************************************************************/
static void 
AppEventLoop(void)
{
  UInt16	err;
  EventType event;

  do
	{
	  EvtGetEvent (&event, evtWaitForever);

	  if (!SysHandleEvent (&event))
		if (!MenuHandleEvent (0, &event, &err))
		  if (!AppHandleEvent (&event))
			FrmDispatchEvent (&event);
	} 
  while(event.eType != appStopEvent);
}


/***********************************************************************
 *	Function:	  PilotMain
 *
 *	Summary:	  Entry point
 *
 *	Parameters:
 *	  cmd		  IN	Action code for the app. This is one of the
 *						  action codes sysAppLaunchCmdXXX defined in
 *						  <SystemMgr.h>
 *	  cmdPBP	  IN	Parameter block pointer for action code. 
 *	  launchFlags IN	Launch flags, one or more of 
 *						  sysAppLaunchFlagXXX defined in <SystemMgr.h>
 *
 *	Returns:
 *	  0 if no error
 *	
 *	Called By: 
 *	  PalmOS when launching the app or asking it to execute an
 *		action code like find or goto. 
 *	
 *	Notes:
 *	
 *	
 *	History:
 *	  2003-02-04  SLV  created
 *
 ***********************************************************************/
UInt32
PilotMain (UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  UInt32 err = 0;

  err = PrvRomVersionCompatible (ourMinVersion, launchFlags);
  if (err) goto Exit;

  switch (cmd)
    {
	  case sysAppLaunchCmdNormalLaunch:
		{
		  err = AppStart();				// Application start code
		  if (err) goto Exit;
		  AppEventLoop();				// Event loop
		  AppStop ();					// Application stop code
		}
		break;
	}

Exit:

  return err;
}



