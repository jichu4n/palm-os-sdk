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
 *  @file MainForm.c
 */
 
#include <PalmOS.h> 
#include "MainForm.h"
#include "utils.h"
#include "VMAccConfig.h"
#include "VMAccConfig_Rsc.h"

// Pointer to the list of values-strings
Char 		**gValues = NULL;
// Pointer to the form which is active
FormType 	*gFrmP = NULL;
// Pointer to the pop-up list
ListType 	*gLstP = NULL;
// Flag which denotes whether __MMDevice database is present
Boolean		gFoundDB = false;
// ID of the database __MMDevice
LocalID 	gLocalID = 0;

static Boolean 	gFirstTime = true;

extern Boolean MainFormDoCommand(UInt16);

/**
 * @brief InitValuesList
 *	      Initializes the list of strings which will hold the values
 *		  entered by the user
 *
 *
 * @param   none 
 * @retval 	void
 *
 *
 * @version This function was added in version 1.0
 *
 **/	

void InitValuesList()
{
	UInt16 count = 0;

	if(gValues)
	{
		for(count = 0; count < NUM_KEYS; count++)
			if(gValues[count])
			{
				MemPtrFree(gValues[count]);
				gValues[count] = NULL;
			}
		MemPtrFree(gValues);
		gValues = NULL;
	}
}


/**
 * @brief MainFormHandleEvent
 *	      Main form event handler.
 *
 *
 * @param *eventP :   IN:  	Pointer to an event.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/	

Boolean MainFormHandleEvent(EventType *eventP) 
{
	Boolean handled = false;
	Err 	err = errNone;
	UInt16 count = 0;
	
	switch (eventP->eType) 
	{
		case frmOpenEvent:
			gFrmP = FrmGetActiveForm();
			
			// Set focus on MainNextButton button
			FrmSetFocus(gFrmP, noFocus);
			FrmSetFocus(gFrmP, FrmGetObjectIndex(gFrmP, MainNextButton));
			
			
			FrmDrawForm(gFrmP);

			if(gFirstTime)
			{
				InitValuesList();
				gValues = MemPtrNew(NUM_KEYS * sizeof(Char *));

				for(count = 0; count < NUM_KEYS; count++)
					gValues[count] = NULL;

				gFoundDB = FindMMDeviceDB(&gLocalID, &err);

				if(gFoundDB)
					ParseMMDeviceValues(gLocalID, ACCSLOT, false);
				gFirstTime = false;
			}
			
			if(gFoundDB)
				FillInFields(FIRSTPAGE);

			handled = true;
			break;
			
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);
		
		case keyDownEvent:
			if (eventP->data.keyDown.keyCode == vchrRockerCenter)
			{
				switch (eventP->data.ctlSelect.controlID)
				{
					case MainNextButton:
					{
						Boolean errorFlag = true;
				// Update values associated with the first form
						errorFlag = UpdateValuesList(FIRSTPAGE);
						if(!errorFlag)
							FrmGotoForm(SecondForm);
						else
						{
							FrmAlert(TitleEmptyAlert);
						}
					handled = true;
					}
					break;
					
					case MainCancelButton:
					{
						DmDeleteDatabase(0, gLocalID);
						ExitApp();
						handled = true;
					}	
					break;	
				}
			}
		
		
			            
 		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == MainNextButton)
			{
				Boolean errorFlag = true;
				// Update values associated with the first form
				errorFlag = UpdateValuesList(FIRSTPAGE);
				if(!errorFlag)
					FrmGotoForm(SecondForm);
				else
				{
					FrmAlert(TitleEmptyAlert);
				}
				handled = true;
			}
			
			if(eventP->data.ctlSelect.controlID == MainCancelButton)
			{
				DmDeleteDatabase(0, gLocalID);
				ExitApp();
				handled = true;
			}	
			break;	
			
		default:
			break;	
	}
    

    return handled;
}


