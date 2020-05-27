/******************************************************************************
 * Copyright (c) 2004-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailAccConfig
 */
 
/**
 *  @brief 	Sample code for the VersaMail Setup Applications
 *          
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Added "About" box and 5-way nav support (02/20/07),compiles under PODS and GCC
 *
 *  @file ThirdForm.c
 */
 
#include <PalmOS.h> 
#include "ThirdForm.h"
#include "utils.h"
#include "VMAccConfig.h"
#include "VMAccConfig_Rsc.h"

extern LocalID gLocalID;

extern Boolean MainFormDoCommand(UInt16);

/***********************************************************************
 * FUNCTION:    	ThirdFormHandleEvent
 *
 * DESCRIPTION: 	Event handler for ThirdForm
 *
 * PARAMETERS:  	eventP -> Pointer to the event structure associated
 *								with the event
 *
 * RETURNED:    	handled -> Returns whether the event has been handled 
 *								or not
 *
 * REVISION HISTORY:
 ***********************************************************************/

Boolean ThirdFormHandleEvent(EventType *eventP) 
{
	Boolean 	handled = false;
	FormType 	*frmP = NULL;
	
	switch (eventP->eType) 
	{
		case frmOpenEvent:
			
			frmP = FrmGetActiveForm();
			
			// Set focus on ThirdSaveButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, ThirdSaveButton));
			
			FrmDrawForm(frmP);
			
			FillInFields(THIRDPAGE);
			
			handled = true;
			break;
			
		 case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);
			
         
         case keyDownEvent:
         
         	// Update values associated with the fourth form
			UpdateValuesList(THIRDPAGE);
			
			if (eventP->data.keyDown.keyCode == vchrRockerCenter)
			{
				switch (eventP->data.ctlSelect.controlID)
				{
					case ThirdSaveButton:
					{	
						// Delete flag is set, so for the given account slot, 
						// if records are found, they are deleted
						ParseMMDeviceValues(gLocalID, ACCSLOT, true);
				
						// New records are added to the database
						UpdateMMDeviceDB(ACCSLOT, gLocalID);
				
						FrmAlert(SaveCompleteAlert);
						// App is exited
						ExitApp();
				
						handled = true;
					}
					break;
					
					case ThirdBackButton:
					{
						FrmGotoForm(SecondForm);
						handled = true;
					}
					break;
				}
				
			}
			
          
 		 case ctlSelectEvent:
			
			// Update values associated with the fourth form
			UpdateValuesList(THIRDPAGE);

			if(eventP->data.ctlSelect.controlID == ThirdSaveButton)
			{
				// Delete flag is set, so for the given account slot, 
				// if records are found, they are deleted
				ParseMMDeviceValues(gLocalID, ACCSLOT, true);
				
				// New records are added to the database
				UpdateMMDeviceDB(ACCSLOT, gLocalID);
				
				FrmAlert(SaveCompleteAlert);
				// App is exited
				ExitApp();
				
				handled = true;
			}
			if(eventP->data.ctlSelect.controlID == ThirdBackButton)
			{
				FrmGotoForm(SecondForm);
				handled = true;
			}
			break;	
			
		default:
			break;	
	
 
	}
    return handled;
}
