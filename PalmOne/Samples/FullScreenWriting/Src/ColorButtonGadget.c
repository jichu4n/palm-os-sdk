/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	ColorButtonGadget.c
 *
 * Description:
 *	Color button with push effect and shadow.
 *
 * Version 1.0 	- Initial Revision (03/12/03)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include <UIColor.h>

#include "GoLCDTest.h"
#include "GoLCDTestRsc.h"
#include "ColorButtonGadget.h"

static RGBColorType black = { 0, 0, 0 ,0 };
static RGBColorType grey = { 0, 128, 128, 128 };

GadgetInfo gGadgetInfo = { 15, false, { 0, 0xDD, 0xEE, 0xFF }, { 0, 0, 0, 0 } };

/***********************************************************************
 *
 * FUNCTION:    DrawGadget
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

static void DrawGadget(FormGadgetTypeInCallback* gadgetP)
{
	GadgetInfo *gadgetInfoP = (GadgetInfo*)gadgetP->data;
	RectangleType rect;
	RGBColorType color;
	
	// Check for pointers
	if( gadgetInfoP == NULL ) return;
	
	// Save the current draw state
	WinPushDrawState();
	
	// Copy the gadget bounds
	RctCopyRectangle(&gadgetP->rect, &rect);
	// Erase gadget
	RctInsetRectangle(&rect, -1);
	WinSetBackColor(UIColorGetTableEntryIndex(UIFormFill));
	WinFillRectangle(&rect, 0);
	// Make the gadbet a tid bit smaller
	RctInsetRectangle(&rect, 1);
	rect.extent.x--;
	rect.extent.y--;
	
	switch(gadgetP->id)
	{
		case MainInkColorGadget:
			MemMove(&color, &gadgetInfoP->inkColor, sizeof(RGBColorType));
			break;
			
		case MainGsiColorGadget:
			MemMove(&color, &gadgetInfoP->gsiColor, sizeof(RGBColorType));
			break;
	}
	
	rect.topLeft.x++;
	rect.topLeft.y++;
	
	if( !gadgetInfoP->inverted )
	{
		RctInsetRectangle(&rect, -1);
		WinSetBackColorRGB(&grey, NULL);
		WinFillRectangle(&rect, 4);
		RctInsetRectangle(&rect, 1);
		rect.topLeft.x--;
		rect.topLeft.y--;
	}		

	WinSetBackColorRGB(&color, NULL);
	WinFillRectangle(&rect, 4);
	
	// Draw the frame
	WinSetForeColorRGB(&black, NULL);
	WinDrawRectangleFrame(roundFrame, &rect);

	// Restore the previous draw state
	WinPopDrawState();
}

/***********************************************************************
 *
 * FUNCTION:    ColorButtonGadgetHandler
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Boolean ColorButtonGadgetHandler(FormGadgetTypeInCallback* gadgetP, UInt16 cmd, void* paramP)
{
	//FormType* frmP = NULL;
	Boolean handled = false;
	EventType* pToEvent = (EventType*) paramP;
	
	//   The gadget handler is called with one of 4 cmd:
	//
	//   formGadgetDrawCmd to draw and initialize the gadget
	//   formGadgetHandleEventCmd in response to a gadget tap or an	event keyed by your application
	//   formGadgetEraseCmd if there is any custom screen erasing that needs to be done when it is erased
	//   formGadgetDeletionCmd to clean up any memory used for the gadget
	
	switch (cmd)
	{
	case formGadgetDrawCmd:
		if( gadgetP->data == NULL ) {
			gadgetP->data = &gGadgetInfo;
		}
		DrawGadget(gadgetP);
		handled = true;
		break;
		
	case formGadgetHandleEventCmd:

		if(pToEvent->eType == frmGadgetEnterEvent)
		{
			Boolean isPenDown = true;
			Boolean isInside = true, isOldInside = true;
			Int16 newPointX, newPointY;
			EventType newEvent;
			//UInt16 ticksPerSec = SysTicksPerSecond();
			
			// Draw inverted button
			gGadgetInfo.inverted = true;
			DrawGadget(gadgetP);

			while(isPenDown)
			{
				EvtGetPen(&newPointX, &newPointY, &isPenDown);
				
				if(RctPtInRectangle (newPointX, newPointY, &(gadgetP->rect))) {
					isInside = true;
					gGadgetInfo.inverted = true;
				} else {
					isInside = false;
					gGadgetInfo.inverted = false;
				}
				
				// If there is a state change
				if( isOldInside != isInside )
				{
					isOldInside = isInside;
					DrawGadget(gadgetP);
				}
			}
			
			// Redraw the released button
			gGadgetInfo.inverted = false;
			DrawGadget(gadgetP);
			
			if( isInside ) {
				//Send a control select event
				newEvent.eType = ctlSelectEvent;
				newEvent.data.ctlSelect.controlID = gadgetP->id;
				newEvent.data.ctlSelect.pControl = (ControlType*)gadgetP; //Hummm....
				EvtAddEventToQueue(&newEvent);
			}
		}
		
		handled = true;
		break;
	
	case formGadgetEraseCmd:
	 	handled = false;
	 	break;
	 	
	 case formGadgetDeleteCmd:
	 	handled = true;
	 	break;

	default:
		break;
	}

	return handled;
}
