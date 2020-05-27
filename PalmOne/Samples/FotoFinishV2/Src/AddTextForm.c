/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	AddTextForm.c
 *
 * Description:
 *	Contains all event-handling code associated with AddTextForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/
#include <PalmOS.h> 
#include "AddTextForm.h"
#include "FotoFinishRsc.h"
#include "PalmPhoto.h"

extern	Coord	gTextX;
extern 	Coord	gTextY;

Char *gTextToAdd = NULL;

/***********************************************************************
 *
 * FUNCTION:    	AddTextFormHandleEvent
 *
 * DESCRIPTION: 	Event-handler for the AddTextForm - handles the Done
 *					and Cancel buttons on the form, as well as the popup list	
 *
 * PARAMETERS:  	EventType *eventP -> Pointer to the Event structure which 
 *					is passed to the event handler
 *
 * RETURNED:    	Boolean handled -> denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Boolean AddTextFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	//FormType *frmP = NULL;
	
	switch (eventP->eType) 
	{
		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == AddTextDoneButton)
			{
				FieldType *fldTextToAdd = NULL;
				FormType  *frmP = FrmGetActiveForm();
				UInt16	length = 0;
				
				// Get the pointer to the text-field which is part of the form
				fldTextToAdd = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, AddTextTextField));
				
				if(fldTextToAdd)
				{
					// If the user has entered some text to be added,
					// assign memory for the text and copy it from the field
					if(FldGetTextPtr(fldTextToAdd))
					{
						length = StrLen(FldGetTextPtr(fldTextToAdd)) + 1;
						if(gTextToAdd)
						{
							MemPtrFree(gTextToAdd);
							gTextToAdd = NULL;
						}	
						gTextToAdd = (Char *)MemPtrNew(length);
						StrCopy(gTextToAdd, (Char *)FldGetTextPtr(fldTextToAdd));
					}
				}
			}	
	
			break;
		
		default:
			break;
	
	}
	
//Done:
	return handled;
}
