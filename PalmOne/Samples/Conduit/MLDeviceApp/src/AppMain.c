/******************************************************************************
 *
 * Copyright (c) 2006 Palm, Inc. All rights reserved.
 *
 * File: AppMain.c
 *
 *****************************************************************************/

#include <PalmOS.h>

#include "AppResources.h"
#include "AppUtilities.h"
#include "TableUtilities.h"

/***********************************************************************
 *
 *	Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			'IQJD'	// Registered
#define appPrefID				0x00
#define appPrefVersionNum		0x01
#define TABLE_SIZE                10

char sDbName[]  = "MusicLib database";
	
/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/

/***********************************************************************
 *
 * FUNCTION:    AddFormAddEntry
 * 
 * DESCRIPTION: Add new library entry to table
 * 
 * PARAMETERS:  pForm  - a pointer to Add form
 * 
 *              pTable - table pointer
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
static void AddFormAddEntry(FormType *pForm, TablePtr  pTable)
{
    short   dSelectedRow, dSelectedCol;
    UInt16   dDbRecordCount = DbGetNonDeletedRecCount();
    
 	FieldType *pArtistField = (FieldType *)FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, AddArtistField));
	FieldType *pTitleField  = (FieldType *)FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, AddTitleField));	
    
    char *sArtist = FldGetTextPtr(pArtistField);
	char *sTitle  = FldGetTextPtr(pTitleField);

    if(!sTitle || !sArtist) { 
    	FrmAlert(MissingFieldAlert);
    	return;
    }

	if(TblGetSelection(pTable, &dSelectedRow, &dSelectedCol)) {
		MusicLibAddRecord(dSelectedRow-1, sTitle, sArtist);
	} else if (dDbRecordCount < TABLE_SIZE) {
		MusicLibAddRecord(dDbRecordCount, sTitle, sArtist);
	} 
	
	TblUnhighlightSelection(pTable);
	TableInit(pTable, (dDbRecordCount + 1));	
}


/***********************************************************************
 *
 * FUNCTION:    AddFormHandleEvent
 * 
 * DESCRIPTION: Add event handler for Add form
 * 
 * PARAMETERS:  pEvent
 * 
 * RETURNED:    true if event handled, false if not
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
static Boolean AddFormHandleEvent(EventType* pEvent)
{
	Boolean handled = false;
	FormType *pForm = FrmGetActiveForm();
	TablePtr  pTable;
    	
	switch (pEvent->eType) {
		
		case ctlSelectEvent:
		   switch (pEvent->data.ctlSelect.controlID) {
		       case AddAddButton:
		           pTable = TableGet();
	               AddFormAddEntry(pForm, pTable);
	               FrmEraseForm(pForm);
	               FrmDeleteForm(pForm);
	               handled = true;	           
		           break;
		       case AddCancelButton:
		           FrmEraseForm(pForm);
		           FrmDeleteForm(pForm);
		           handled = true;		       
		           break;
		   }
		   break;
		 
	    default:
		   break;   
	}
    return(handled);
}


/***********************************************************************
 *
 * FUNCTION:    MainFormDeleteEntry
 * 
 * DESCRIPTION: Delete entry from form
 * 
 * PARAMETERS:  
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
static void MainFormDeleteEntry()
{
	short dSelectedRow, dSelectedCol;
	FormType *pForm  = FrmGetActiveForm();
	TablePtr  pTable = AppGetFormObjectPtr(pForm, MainLibTable);
	
	if(TblGetSelection(pTable, &dSelectedRow, &dSelectedCol) && dSelectedRow > 0) {
	   DbDeleteRecord(dSelectedRow-1); 	
	} else { 
	   FrmAlert(DeleteAlert);
	}
	TblUnhighlightSelection(pTable);
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	FormType * pForm;

	switch (command) {
		case MainOptionsAboutStarterApp:
			pForm = FrmInitForm(AboutForm);
			FrmDoDialog(pForm);					// Display the About Box.
			FrmDeleteForm(pForm);
			handled = true;
			break;
	}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    MainFormButtonEvent
 *
 * DESCRIPTION: This routine handles Add/Delete button taps
 *
 * PARAMETERS:  pEvent  - a pointer to an EventType structure
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainFormButtonEvent(EventType* pEvent)
{
    FormType *pForm = FrmGetActiveForm();
    FormType *pAddForm;
    
    switch (pEvent->data.ctlSelect.controlID) {
    	
		case MainAddButton:     
	        
			if(TableIsFull()) {
				FrmAlert(SelectAlert);
			} else {
                pAddForm = FrmInitForm(AddForm);   
                FrmSetEventHandler(pAddForm, AddFormHandleEvent);
                FrmDoDialog(pAddForm);	
                FrmEraseForm(pAddForm);
                FrmDeleteForm(pAddForm);
                FrmSetActiveForm(pForm);
                FrmDrawForm(pForm);
            }
            break;
		    	    
		case MainDeleteButton:
		    MainFormDeleteEntry();	
		    TableInit(TableGet(), DbGetNonDeletedRecCount());	
		    FrmEraseForm(pForm);
		    FrmDrawForm(pForm);
		    break;	
    }
}

/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  pForm
 *
 * RETURNED:    TablePtr
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static TablePtr MainFormInit(FormType *pForm)
{ 
     UInt16   dFrmIndex = FrmGetObjectIndex(pForm, MainLibTable);
     TablePtr pTable    = FrmGetObjectPtr(pForm, dFrmIndex);	   	
     
     return pTable;
}


/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  pEvent  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandleEvent(EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm = FrmGetActiveForm();
    TablePtr    pTable;
    UInt16      row;
    
	switch (pEvent->eType) {
		case menuEvent:
			return MainFormDoCommand(pEvent->data.menu.itemID);

		case frmOpenEvent:           		
		    pTable = MainFormInit(pForm);
		    TableInit(pTable, DbGetNonDeletedRecCount());	
		    FrmDrawForm(pForm);
 			handled = true;
			break;
		
		case ctlSelectEvent:
		    MainFormButtonEvent(pEvent);
		    FrmSetActiveForm(pForm);
		    handled = true;
		    break;
		
		case tblSelectEvent:
		    
		    handled = true;
		    break;
		case tblEnterEvent:
		
		    handled = true;
		    break;
        case penDownEvent:
        {
           TablePtr pTable = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MainLibTable));	   	
           row = TableGetPointRow(pTable, pEvent->screenY);
           if(row != -1)
           {
              TableSelectRow(pTable, row);
              handled = true;
           } else {
           	  handled = false;
           }

           break;
        }
		default:
			break;
	}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventType* pEvent)
{
	UInt16 		formId;
	Boolean		handled = false;
    FormType* 	pMainForm;

	if (pEvent->eType == frmLoadEvent) {
		// Load the form resource.
		formId = pEvent->data.frmLoad.formID;
		
		pMainForm = FrmInitForm(formId);
		FrmSetActiveForm(pMainForm);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId) {
			case MainForm:
			    DbOpenOrCreate(sDbName); 
				FrmSetEventHandler(pMainForm, MainFormHandleEvent);
				break;
				
			default:
				break;
		}
		handled = true;
	}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value errNone if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStart(void)
{
	FrmGotoForm(MainForm);
	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{
    DbClose();
	FrmCloseAllForms();
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	Err			error;
	EventType	event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (SysHandleEvent(&event))
			continue;
			
		if (MenuHandleEvent(0, &event, &error))
			continue;
			
		if (AppHandleEvent(&event))
			continue;

		FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY: 
 *
 *
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error = errNone;

	switch (cmd) {
		case sysAppLaunchCmdNormalLaunch:
			if ((error = AppStart()) == 0) {			
				AppEventLoop();
				AppStop();
			}
			break;

		default:
			break;
	}
	
	return error;
}


