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
 *  @file SecondForm.c
 */


#include <PalmOS.h> 
#include "SecondForm.h"
#include "utils.h"
#include "VMAccConfig.h"
#include "VMAccConfig_Rsc.h"

extern Boolean MainFormDoCommand(UInt16);

/***********************************************************************
 * FUNCTION:    	SecondFormHandleEvent
 *
 * DESCRIPTION: 	Event handler for SecondForm
 *
 * PARAMETERS:  	eventP -> Pointer to the event structure associated
 *								with the event
 *
 * RETURNED:    	handled -> Returns whether the event has been handled 
 *								or not
 *
 * REVISION HISTORY:
 ***********************************************************************/

Boolean SecondFormHandleEvent(EventType *eventP) 
{
	Boolean 	handled = false;
	FormType 	*frmP = NULL;

	switch (eventP->eType) 
	{
		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			
			// Set focus on SecondNextButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, SecondNextButton));
			
			FrmDrawForm(frmP);
			FillInFields(SECONDPAGE);
			
			handled = true;
			break;
			
        
        case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);
			
		
		case keyDownEvent:
			if (eventP->data.keyDown.keyCode == vchrRockerCenter)
			{
				switch (eventP->data.ctlSelect.controlID)
				{
					case SecondNextButton:
					{	
						FrmGotoForm(ThirdForm);
						handled = true;
					}
					break;
					
					case SecondBackButton:
					{
						FrmGotoForm(MainForm);
						handled = true;
					}
					break;
				}
				
				// Update values associated with the second form
				UpdateValuesList(SECONDPAGE);
			}
			
		    
 		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == SecondNextButton)
			{
				FrmGotoForm(ThirdForm);
				handled = true;
			}
			if(eventP->data.ctlSelect.controlID == SecondBackButton)
			{
				FrmGotoForm(MainForm);
				handled = true;
			}

			// Update values associated with the second form
			UpdateValuesList(SECONDPAGE);
			break;	
			
		default:
			break;	
	}
    return handled;
}
