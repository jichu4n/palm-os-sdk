/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailSync.c
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  The file contains routines the manage the Mail application's 
 *   HotSync options.
 *
 * History:
 *		July 29, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#include <PalmOS.h>

#include "Mail.h"
/* HandEra */
#include "vga.h"
/* end HandEra */

/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/

#define localHotSyncItem			0
#define remoteHotSyncItem			1

static void SyncApply (void);
static void SyncChangeSyncType (UInt16 controlID);
static MemHandle SyncSaveUIState (Boolean local);
static void SyncSavePreferences (Boolean local);
static void SyncLoadPreferences (Boolean local);
static void SyncInitFilter (UInt16 fieldID, Char * filter, UInt16 len);
static void SyncInit (void);
static void SyncRestoreUIState (MemHandle stateH);


/* HandEra */
static  VgaScreenStateType      gScreenState;
/* end HandEra */

/***********************************************************************
 *
 * FUNCTION:    ShowObject
 *
 * DESCRIPTION: This routine set an object usable and draws the object if
 *              the form it is in is visible.
 *
 * PARAMETERS:  frm      - pointer to a form
 *              objectID - id of the object to set usable
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	2/21/95	Initial Revision
 *
 ***********************************************************************/
static void ShowObject (FormPtr frm, UInt16 objectID)
{
	FrmShowObject (frm, FrmGetObjectIndex (frm, objectID));
}


/***********************************************************************
 *
 * FUNCTION:    HideObject
 *
 * DESCRIPTION: This routine set an object not-usable and erases it
 *              if the form it is in is visible.
 *
 * PARAMETERS:  frm      - pointer to a form
 *              objectID - id of the object to set not usable
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	2/21/95	Initial Revision
 *
 ***********************************************************************/
static void HideObject (FormPtr frm, UInt16 objectID)
{
	FrmHideObject (frm, FrmGetObjectIndex (frm, objectID));
}


/***********************************************************************
 *
 * FUNCTION:    SyncInitFilter
 *
 * DESCRIPTION: This routine initialize the text value of the specified
 *              filter field with the string passed.
 *
 * PARAMETERS:  fieldID  id of field to initialize
 *              filter   filter string
 *              len      length of the filter string
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/1/96	Initial Revision
 *
 ***********************************************************************/
static void SyncInitFilter (UInt16 fieldID, Char * filter, UInt16 len)
{
	MemHandle h;
	Char * p;
	FieldPtr fld;

	fld = GetObjectPtr (fieldID);
	FldFreeMemory (fld);

	if (len)
		{
		// Copy the filter string into a new handle.
		h = MemHandleNew (len+1);
		p = MemHandleLock (h);
		StrCopy (p, filter);
		MemPtrUnlock (p);

		FldSetTextHandle (fld, h);
		}

	if (FrmVisible(FrmGetActiveForm ()))
		FldDrawField (fld);
}


/***********************************************************************
 *
 * FUNCTION:    SyncChangeSyncType
 *
 * DESCRIPTION: This routine changes the ui gadgets in the Sync Options
 *              Dialog Box such that they match the newly selected sync
 *              type.  The routine is called when one of the sync type 
 *              push buttons are pushed.
 *
 * PARAMETERS:  controlID - id of the newly selected sync type push button
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art		7/30/96	Initial Revision
 *			MGolden	7/7/97	added syncUnread option
 *
 ***********************************************************************/
static void SyncChangeSyncType (UInt16 controlID)
{
	FormPtr frm;
	
	frm = FrmGetActiveForm ();

	switch (controlID)
		{
		case SyncAllPushButton:
			HideObject (frm, SyncSendOnlyLabel);
			HideObject (frm, SyncRetrieveAllCheckbox);
			HideObject (frm, SyncMessageContaingTrigger);
			HideObject (frm, SyncToButton);
			HideObject (frm, SyncToField);
			HideObject (frm, SyncFromButton);
			HideObject (frm, SyncFromField);
			HideObject (frm, SyncSubjectButton);
			HideObject (frm, SyncSubjectField);
//mgmg added for syncUnread
			HideObject(frm, SyncUnreadLabel);

			ShowObject (frm, SyncAllLabel);
			break;

		
		case SyncSendOnlyPushButton:
			HideObject (frm, SyncAllLabel);
			HideObject (frm, SyncRetrieveAllCheckbox);
			HideObject (frm, SyncMessageContaingTrigger);
			HideObject (frm, SyncToButton);
			HideObject (frm, SyncToField);
			HideObject (frm, SyncFromButton);
			HideObject (frm, SyncFromField);
			HideObject (frm, SyncSubjectButton);
			HideObject (frm, SyncSubjectField);
//mgmg added for syncUnread
			HideObject(frm, SyncUnreadLabel);

			ShowObject (frm, SyncSendOnlyLabel);
			break;


		case SyncFilterPushButton:			
			HideObject (frm, SyncAllLabel);
			HideObject (frm, SyncSendOnlyLabel);
//mgmg added for syncUnread
			HideObject(frm, SyncUnreadLabel);

			ShowObject (frm, SyncRetrieveAllCheckbox);
			ShowObject (frm, SyncMessageContaingTrigger);
			ShowObject (frm, SyncToButton);
			ShowObject (frm, SyncToField);
			ShowObject (frm, SyncFromButton);
			ShowObject (frm, SyncFromField);
			ShowObject (frm, SyncSubjectButton);
			ShowObject (frm, SyncSubjectField);
			break;

//mgmg added for syncUnread
		case SyncUnreadPushButton:
			HideObject (frm, SyncAllLabel);
			HideObject (frm, SyncSendOnlyLabel);
			HideObject (frm, SyncRetrieveAllCheckbox);
			HideObject (frm, SyncMessageContaingTrigger);
			HideObject (frm, SyncToButton);
			HideObject (frm, SyncToField);
			HideObject (frm, SyncFromButton);
			HideObject (frm, SyncFromField);
			HideObject (frm, SyncSubjectButton);
			HideObject (frm, SyncSubjectField);
//mgmg added for syncUnread
			ShowObject(frm, SyncUnreadLabel);
			break;
		}
}


/***********************************************************************
 *
 * FUNCTION:    SyncSaveUIState
 *
 * DESCRIPTION: This routine save the current ui settings. It is call
 *              before go the the Edit View to edit the to, from or 
 *              subject filters.  We will dismiss the HotSync Options 
 *              dialog so we need to save the UI state.
 *
 * PARAMETERS:  local - true for local sync preferences, false for 
 *                      remote sync preferences.
 *
 * RETURNED:    handle of SyncUIStateType structure
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/20/96	Initial Revision
 *
 ***********************************************************************/
static MemHandle SyncSaveUIState (Boolean local)
{
	FormPtr frm;
	ListPtr lst;
	FieldPtr fld;
	ControlPtr ctl;
	MemHandle stateH;
	SyncUIStatePtr	stateP;


	frm = FrmGetActiveForm ();

	// Allocate a handle to hold the UI state.
	stateH = MemHandleNew (sizeof (SyncUIStateType));
	if (! stateH) return (NULL);
	stateP = MemHandleLock (stateH);

	// Local or remote HotSync options?
	stateP->local = local;

	// Get the sync type (all, send only, or filter or Unread).
	stateP->syncType = FrmGetControlGroupSelection (frm, SyncTypeGroup);

	// Get the "Retrieve All High Priority" checkbox setting.
	ctl = GetObjectPtr (SyncRetrieveAllCheckbox);
	stateP->retrieveHighPriority = (CtlGetValue (ctl) == true);

	// Get the "Ignore / Retrive Messages Containing" setting.
	lst = GetObjectPtr (SyncMessageContaingList);
	stateP->messageContaing = (MailFilterType) LstGetSelection (lst);

	// Get length that retrieved message should be truncated to.
	stateP->maxLength = TruncateLen;

	// Get the to, from and subject filters.
	fld = GetObjectPtr (SyncToField);
	stateP->to = FldGetTextHandle (fld);
	FldSetTextHandle (fld, 0);

	fld = GetObjectPtr (SyncFromField);
	stateP->from = FldGetTextHandle (fld);
	FldSetTextHandle (fld, 0);

	fld = GetObjectPtr (SyncSubjectField);
	stateP->subject = FldGetTextHandle (fld);
	FldSetTextHandle (fld, 0);

	MemHandleUnlock (stateH);
	
	return (stateH);
}


/***********************************************************************
 *
 * FUNCTION:    SyncRestoreUIState
 *
 * DESCRIPTION: This routine restores the ui settings of the HotSync 
 *              options dialog.  It is call after we return from
 *              editing the to, from or subject filters, in the Edit
 *              View.  We dismissed the HotSync Options before going
 *              to the Edit View  so we need to restore the UI state.
 *
 * PARAMETERS:  handle of SyncUIStateType structure
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/20/96	Initial Revision
 *
 ***********************************************************************/

static void SyncRestoreUIState (MemHandle stateH)
{
	UInt16 item;
	UInt16 controlID;
	Char * label;
	ListPtr lst;
	FormPtr frm;
	ControlPtr ctl;
	SyncUIStatePtr	stateP;


	frm = FrmGetActiveForm ();

	stateP = MemHandleLock (stateH);

	// Set the local / remote settings trigger.
	ctl = GetObjectPtr (SyncSettingsTrigger);
	lst = GetObjectPtr (SyncSettingsList);
	if (stateP->local)
		item = localHotSyncItem;
	else
		item = remoteHotSyncItem;
	label = LstGetSelectionText (lst, item);
	CtlSetLabel (ctl, label);
	LstSetSelection (lst, item);


	// Set the Sync tye (All, Send Only, or Filter) push button.
	controlID = FrmGetObjectId (frm, stateP->syncType);
	FrmSetControlGroupSelection (frm, SyncTypeGroup, controlID);

	// Enable / Disable the proper ui gadgets.
	SyncChangeSyncType (controlID);

	// Set the "Retrieve All High Priority" checkbox.
	ctl = GetObjectPtr (SyncRetrieveAllCheckbox);
	CtlSetValue (ctl, (stateP->retrieveHighPriority == true));

	
	// Set the "Ignore / Retrive Messages Containing" trigger and poup
	// list.
	ctl = GetObjectPtr (SyncMessageContaingTrigger);
	lst = GetObjectPtr (SyncMessageContaingList);
	label = LstGetSelectionText (lst, stateP->messageContaing);
	CtlSetLabel (ctl, label);
	LstSetSelection (lst, stateP->messageContaing);
	
	// Set length that retrieved message should be truncated to.
	TruncateLen = stateP->maxLength;

	// Set the to, from and subject filters.
	FldSetTextHandle (GetObjectPtr (SyncToField), stateP->to);
	FldSetTextHandle (GetObjectPtr (SyncFromField), stateP->from);
	FldSetTextHandle (GetObjectPtr (SyncSubjectField), stateP->subject);
		
	MemPtrFree (stateP);
}


/***********************************************************************
 *
 * FUNCTION:    SyncLoadPreferences
 *
 * DESCRIPTION: This routine load the specified HotSync preferences 
 *              (local or remote) and initial the dialog's ui.
 *
 * PARAMETERS:  local - true for local sync preferences, false for 
 *                      remote sync preferences.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	7/30/96	Initial Revision
 * 			MGolden	7/7/97	added syncUnread option
 *
 ***********************************************************************/
static void SyncLoadPreferences (Boolean local)
{
	UInt16 len;
	UInt16 rscID;
	UInt16 prefsSize;
	UInt16 controlID;
	Int16 version;
	Char * label;
	Char * filter;
	ListPtr lst;
	FormPtr frm;
	ControlPtr ctl;
	SyncPreferencesType prefs;
	SyncPreferencesPtr  prefsP;

	frm = FrmGetActiveForm ();

	if (local)
		rscID = mailLocalSyncPrefID;
	else
		rscID = mailRemoteSyncPrefID;
		

	// Get the HotSync preferences.
	prefsSize = sizeof (SyncPreferencesType);
	version = PrefGetAppPreferences (sysFileCMail, rscID, &prefs, &prefsSize, true);
	if (version == noPreferenceFound)
		{
		controlID = SyncAllPushButton;
		prefs.retrieveHighPriority = true;
		prefs.messageContaing = ingoreContaining;

		TruncateLen = syncDefaultMaxMsgLen;
		}

	else 
		{
//mgmg syncUnread
		switch (prefs.syncType)
			{
			case syncAll:
				controlID = SyncAllPushButton;
				break;
				
			case syncSendOnly:
				controlID = SyncSendOnlyPushButton;
				break;

			case syncFilter:
				controlID = SyncFilterPushButton;
				break;

			case syncUnread:
				controlID = SyncUnreadPushButton;
				break;
			
			default:
				//just in case...set it to all
				controlID = SyncAllPushButton;
				break;
			}//switch
/* old
		if (prefs.syncType == syncAll)
			{
			controlID = SyncAllPushButton;
			}
			
		else if (prefs.syncType == syncSendOnly)
			{
			controlID = SyncSendOnlyPushButton;
			}
	
		else
			{
			controlID = SyncFilterPushButton;
			}
*/

		TruncateLen = prefs.maxLength;
		}


	// Set the Sync tye (All, Send Only, or Filter) push button.
	FrmSetControlGroupSelection (frm, SyncTypeGroup, controlID);


	// Enable / Disable the proper ui gadgets.
	SyncChangeSyncType (controlID);

	// Set the "Retrieve All High Priority" checkbox.
	ctl = GetObjectPtr (SyncRetrieveAllCheckbox);
	CtlSetValue (ctl, (prefs.retrieveHighPriority == true));

	
	// Set the "Ignore / Retrive Messages Containing" trigger and poup
	// list.
	ctl = GetObjectPtr (SyncMessageContaingTrigger);
	lst = GetObjectPtr (SyncMessageContaingList);
	label = LstGetSelectionText (lst, prefs.messageContaing);
	CtlSetLabel (ctl, label);
	LstSetSelection (lst, prefs.messageContaing);
	

	// Get the rest of the preferences, the filter strings are at the 
	// of the preferences.
	if (version != noPreferenceFound)
		{
		prefsP = MemPtrNew (prefsSize);
		if (! prefsP) return;
		
		PrefGetAppPreferences (sysFileCMail, rscID, prefsP, &prefsSize, true);

		filter = ((Char *)prefsP) + sizeof (SyncPreferencesType);
		len = StrLen (filter);
		SyncInitFilter (SyncToField, filter, len);
		
		filter += len + 1;
		len = StrLen (filter);
		SyncInitFilter (SyncFromField, filter, len);
		
		filter += len + 1;
		len = StrLen (filter);
		SyncInitFilter (SyncSubjectField, filter, len);
		
		MemPtrFree (prefsP);
		}

	else
		{
		SyncInitFilter (SyncToField, NULL, 0);
		SyncInitFilter (SyncFromField, NULL, 0);
		SyncInitFilter (SyncSubjectField, NULL, 0);
		}
}


/***********************************************************************
 *
 * FUNCTION:    SyncSavePreferences
 *
 * DESCRIPTION: This routine save the specified HotSync preferences 
 *              (local or remote) based on the dialog's ui settings
 *
 * PARAMETERS:  local - true for local sync preferences, false for 
 *                      remote sync preferences.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/1/96	Initial Revision
 * 			MGolden	7/7/97	added syncUnread option
 *
 ***********************************************************************/
static void SyncSavePreferences (Boolean local)
{
	UInt16 prefID;
	UInt16 prefsSize;
	UInt16 controlID;
	Char * ptr;
	Char * filter;
	FormPtr frm;
	ListPtr lst;
	FieldPtr toFld;
	FieldPtr fromFld;
	FieldPtr subjectFld;
	ControlPtr ctl;
	SyncPreferencesPtr  prefsP;


	// Local or remote HotSync options?
	if (local)
		prefID = mailLocalSyncPrefID;
	else
		prefID = mailRemoteSyncPrefID;


	// Allocate a handle to hold the preferences.
	toFld = GetObjectPtr (SyncToField);
	fromFld = GetObjectPtr (SyncFromField);
	subjectFld = GetObjectPtr (SyncSubjectField);
		
	prefsSize = sizeof (SyncPreferencesType) + 
					FldGetTextLength (toFld) + 
					FldGetTextLength (fromFld) + 
					FldGetTextLength (subjectFld) + 3;
	
	prefsP = MemPtrNew (prefsSize);
	if (! prefsP) return;


	// Get the sync type (all, send only, or filter).
	frm = FrmGetActiveForm ();
	controlID = FrmGetObjectId (frm, 
		FrmGetControlGroupSelection (frm, SyncTypeGroup));
//new syncUnread
	switch(	controlID )
		{
		case SyncAllPushButton:
			prefsP->syncType = syncAll;		
			break;
		
		case SyncSendOnlyPushButton:
			prefsP->syncType = syncSendOnly;
			break;
		
		case SyncFilterPushButton:
			prefsP->syncType = syncFilter;
			break;
		
		case SyncUnreadPushButton:
			prefsP->syncType = syncUnread;
			break;
		
		default:
			//just in case...set it to all...
			prefsP->syncType = syncAll;		
			break;
				
		}//switch
/* old
	if (controlID == SyncAllPushButton)
		prefsP->syncType = syncAll;		
	else if (controlID == SyncSendOnlyPushButton)
		prefsP->syncType = syncSendOnly;
	else
		prefsP->syncType = syncFilter;
*/

	// Get the "Retrieve All High Priority" checkbox setting.
	ctl = GetObjectPtr (SyncRetrieveAllCheckbox);
	prefsP->retrieveHighPriority = (CtlGetValue (ctl) == true);


	// Get the "Ignore / Retrive Messages Containing" setting.
	lst = GetObjectPtr (SyncMessageContaingList);
	prefsP->messageContaing = (MailFilterType) LstGetSelection (lst);


	prefsP->maxLength = TruncateLen;


	// Copy the filter strings to the end of the preferences.
	filter = ((Char *)prefsP) + sizeof (SyncPreferencesType);
	ptr = FldGetTextPtr (toFld);
	if (ptr)
		{
		StrCopy (filter, ptr);
		filter += StrLen(ptr) + 1;
		}
	else
		{
		*filter = 0;
		filter++;
		}

	ptr = FldGetTextPtr (fromFld);
	if (ptr)
		{
		StrCopy (filter, ptr);
		filter += StrLen(ptr) + 1;
		}
	else
		{
		*filter = 0;
		filter++;
		}


	ptr = FldGetTextPtr (subjectFld);
	if (ptr)
		{
		StrCopy (filter, ptr);
		}
	else
		{
		*filter = 0;
		}
		
	
	// Save the preferences.
	PrefSetAppPreferences (sysFileCMail, prefID, mailVersionNum, prefsP, prefsSize, true);

	MemPtrFree (prefsP);
}


/***********************************************************************
 *
 * FUNCTION:    SyncApply
 *
 * DESCRIPTION: This routine applies the changes made in the Sync
 *              Option Dialog.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	7/30/96	Initial Revision
 *
 ***********************************************************************/
static void SyncApply (void)
{
	ListPtr lst;

	// Local or remote HotSync options?
	lst = GetObjectPtr (SyncSettingsList);
	SyncSavePreferences (LstGetSelection (lst) == localHotSyncItem);
}


/***********************************************************************
 *
 * FUNCTION:    SyncInit
 *
 * DESCRIPTION: This routine initializes the Sync Dialog.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	7/29/96	Initial Revision
 *
 ***********************************************************************/
static void SyncInit (void)
{
	Char * label;
	ListPtr lst;
	FormPtr frm;
	ControlPtr ctl;

	frm = FrmGetActiveForm ();

	// Set the local / remote settings trigger.
	ctl = GetObjectPtr (SyncSettingsTrigger);
	lst = GetObjectPtr (SyncSettingsList);
	label = LstGetSelectionText (lst, localHotSyncItem);
	CtlSetLabel (ctl, label);
	LstSetSelection (lst, localHotSyncItem);


	// Get the local HotSync preferences and initial the dialog's ui.
	SyncLoadPreferences (true);
}



/***********************************************************************
 *
 * FUNCTION:    SyncHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Sync
 *              Options Dialog Box".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	7/29/96	Initial Revision
 * 			MGolden	7/7/97	added syncUnread option
 *
 ***********************************************************************/
Boolean SyncHandleEvent (EventPtr event)
{
	UInt16 controlID;
	FormPtr frm;
	Boolean local;
	Boolean handled = false;

	if (event->eType == ctlSelectEvent)
		{
		controlID = event->data.ctlSelect.controlID;
		if (controlID == SyncOkButton)
			{
				SyncApply ();
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
			}
		else if (controlID == SyncCancelButton)
			{
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
			}
		else if (controlID == SyncTruncateButton)
			{
				FrmPopupForm (TruncateDialog);
			}
		else if ( (controlID == SyncAllPushButton) 		||
					 (controlID == SyncSendOnlyPushButton) 	||
					 (controlID == SyncFilterPushButton)   	||
					 (controlID == SyncUnreadPushButton))
			{
			SyncChangeSyncType (event->data.ctlSelect.controlID);
			}

		else if ( (controlID == SyncToButton) 		||
					 (controlID == SyncFromButton) 	||
					 (controlID == SyncSubjectButton))
			{
				if (controlID == SyncToButton)
					EditViewField = editToField;
				else if (controlID ==  SyncFromButton)
					EditViewField = editFromField;
				else
					EditViewField = editSubjectField;

				EditViewEditRecord = false;
				local = (LstGetSelection (GetObjectPtr (SyncSettingsList)) == localHotSyncItem);
				SyncUIStateH = SyncSaveUIState (local);
				FrmGotoForm (EditView);
			}

#ifdef USE_SWITCH_STATEMENT
		switch (controlID)
			{
			case SyncOkButton:
				SyncApply ();
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
				break;

			case SyncCancelButton:
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
				break;

			case SyncTruncateButton:
				FrmPopupForm (TruncateDialog);
				break;

			case SyncAllPushButton:
			case SyncSendOnlyPushButton:
			case SyncFilterPushButton:			

//mgmg syncUnread
			case SyncUnreadPushButton:			
				SyncChangeSyncType (event->data.ctlSelect.controlID);
				break;

			case SyncToButton:
			case SyncFromButton:
			case SyncSubjectButton:
				if (controlID == SyncToButton)
					EditViewField = editToField;
				else if (controlID ==  SyncFromButton)
					EditViewField = editFromField;
				else
					EditViewField = editSubjectField;

				EditViewEditRecord = false;
				local = (LstGetSelection (GetObjectPtr (SyncSettingsList)) == localHotSyncItem);
				SyncUIStateH = SyncSaveUIState (local);
				FrmGotoForm (EditView);
				break;
			}
#endif
		}


	else if (event->eType == popSelectEvent)
		{
		if (event->data.popSelect.controlID == SyncSettingsTrigger &&
			 event->data.popSelect.selection != event->data.popSelect.priorSelection)
			{
			local = event->data.popSelect.selection == localHotSyncItem;
			SyncSavePreferences (! local);
			SyncLoadPreferences (local);
			}
		}


	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
        if(vgaExists)
            VgaSaveScreenState(&gScreenState);
        ToolsVgaAdjustModalForm(frm, true);
/* end HandEra */
		if ( ! SyncUIStateH)
			SyncInit ();
		else
			{
			SyncRestoreUIState (SyncUIStateH);
			SyncUIStateH = 0;
			}
		FrmDrawForm (frm);
		handled = true;
		}

	return (handled);
}
