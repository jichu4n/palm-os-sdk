/******************************************************************************
 * Copyright (c) 2003-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailFontApp
 */
 
/**
 *  @brief 	Sample code for the VersaMail Font Manager API
 *  		
 *
 *
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Usha Shekar (3/15/07) - Added "About" box, 5-way navigation
 *					  and Doxygen comments. Compiles with PODS and GCC.
 *
 *  @file MainForm.c
 */


#include <PalmOS.h> 
#include <HsExt.h>

#include "MainForm.h"
#include "VMFontAppRsc.h"
#include "utils.h"
#include "PalmOneVMFontLib.h"

#include "VMFontApp.h"

// Font ID of the current font
UInt8	gCurrentFontID = 0;
// Reference number for the VM Font library
extern UInt16 	gLibRefNum;
// Context of the VM Font library
extern UInt32 	gContext;
// Current rules associated with the font picker
static VMFontUIRules gRules = { true , true , true , allSize , allStyle};

/***********************************************************************
 *
 * FUNCTION:    	PrvMenu
 *
 * DESCRIPTION: 	Handles menu events 	
 *
 * PARAMETERS:  	menuID -> ID of the menu selected
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/
 
static void PrvMenu(UInt16 menuID)
{
	//FormType 	*frmAbout = NULL;
	FormType 	*frmOptions = NULL;

	ControlType *ctlSize = NULL;
	ControlType *ctlStyle = NULL;
	ControlType *ctlFace = NULL;
	
	ListType 	*lstSizeP = NULL;
	ListType 	*lstStyleP = NULL;
	
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;
//	UInt16 sel_size = 0, sel_style = 0;
	
	switch(menuID)
	{
		case HelpAboutFontoMatic:
							
			DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);

			MenuEraseStatus(0);
			HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
					
			break;
	
		case EditOptions:
			
			// Changes the rules associated with font picker
			frmOptions = FrmInitForm(OptionsForm);
			
			lstSizeP = FrmGetObjectPtr(frmOptions, 
											FrmGetObjectIndex(frmOptions, OptionsSizeList));
			lstStyleP = FrmGetObjectPtr(frmOptions, 
											FrmGetObjectIndex(frmOptions, OptionsStyleList));
											
			// Show saved values; show default values if not changed
			
			LstSetSelection(lstSizeP, gRules.sizeRule);
			
			LstSetSelection(lstStyleP, gRules.styleRule);
				
			
			UpdateListUI(OptionsForm, OptionsSizePopTrigger,OptionsSizeList);
			
			UpdateListUI(OptionsForm, OptionsStylePopTrigger, OptionsStyleList);
			
			
			// Show saved values; show default values if not changed
			
			FrmSetControlValue(frmOptions, FrmGetObjectIndex(frmOptions, OptionsShowFontFaceCheckbox),
							   gRules.fontFaceVisible);
			FrmSetControlValue(frmOptions, FrmGetObjectIndex(frmOptions, OptionsShowFontSizeCheckbox),
							   gRules.fontSizeVisible);
			FrmSetControlValue(frmOptions, FrmGetObjectIndex(frmOptions, OptionsShowFontStyleCheckbox),
							   gRules.fontStyleVisible);
			
			if(FrmDoDialog(frmOptions))
			{
				// Get the values associated with different fields
				ctlFace = 	FrmGetObjectPtr(frmOptions, 
											FrmGetObjectIndex(frmOptions, OptionsShowFontFaceCheckbox));
				ctlSize  = 	FrmGetObjectPtr(frmOptions, 
											FrmGetObjectIndex(frmOptions, OptionsShowFontSizeCheckbox));
				ctlStyle = 	FrmGetObjectPtr(frmOptions, 
											FrmGetObjectIndex(frmOptions, OptionsShowFontStyleCheckbox));
				
				// Set the values associated with the different fields
				gRules.fontFaceVisible = CtlGetValue(ctlFace);
				gRules.fontSizeVisible = CtlGetValue(ctlSize);
				gRules.fontStyleVisible = CtlGetValue(ctlStyle);
				
				if(LstGetSelection(lstSizeP) != noListSelection)
					gRules.sizeRule = LstGetSelection(lstSizeP);

				if(LstGetSelection(lstStyleP) != noListSelection)
					gRules.styleRule = LstGetSelection(lstStyleP);
					
			}
			FrmEraseForm(frmOptions);
			FrmDeleteForm(frmOptions);
			
			break;
	}
}

/***********************************************************************
 * FUNCTION:    	ExitStop
 *
 * DESCRIPTION: 	Exits the app by generating an appStopEvent 	
 *
 * PARAMETERS:  	None
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/

static void ExitApp()
{
	EventType event;
	
	event.eType = appStopEvent;
	EvtAddEventToQueue(&event);
}

/***********************************************************************
 * FUNCTION:    	SetValues
 *
 * DESCRIPTION: 	Sets values associated with a given form
 *
 * PARAMETERS:  	fieldID -> ID of the given field
 *					text -> Pointer to the text which has to be set
 *					numText -> The integer which has to be set to the field 	
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/
 
static void SetValues(UInt16 fieldID, Char *text, UInt16 numText)
{
	FormType 	*frmP = FrmGetActiveForm();
	FieldType 	*fldValue = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, fieldID)); 
	
	if(text)
		SetFieldText(fldValue, text, true);
	else if(numText)
	{
		Char convertedText[3] = "";
		StrPrintF(convertedText, "%d", (UInt16)numText);
		SetFieldText(fldValue, convertedText, true);
	}			 
}	

/***********************************************************************
 * FUNCTION:    	ToggleLabelsDisplay
 *
 * DESCRIPTION: 	Hides/shows UI elements on the form, depending on the flag
 *
 * PARAMETERS:  	Boolean showFlag -> Parameter which decides whether the UI
 *										elements have to be drawn or hidden	
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/

static void ToggleLabelsDisplay(Boolean showFlag)
{
	FormType *frmP = FrmGetActiveForm();
	
	if(!showFlag)
	{
			FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFontIDTagLabel));
			FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFontIDField));
			
			FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFontTagLabel));
			FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFontField));
	}
	else
	{
			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFontIDTagLabel));
			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFontIDField));
			
			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFontTagLabel));
			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFontField));
	}
}

/***********************************************************************
 * FUNCTION:    	MainFormHandleEvent
 *
 * DESCRIPTION: 	Event handler for MainForm
 *
 * PARAMETERS:  	eventP -> Pointer to the event structure associated
 *								with the event
 *
 * RETURNED:    	handled -> Returns whether the event has been handled 
 *								or not
 *
 * REVISION HISTORY:
 ***********************************************************************/

Boolean MainFormHandleEvent(EventType *eventP) 
{
	Boolean handled = false;
	FormType  *frmP = FrmGetActiveForm();
	Char fontString[50] = "";
	UInt8	previousFontID;
	
	switch (eventP->eType) 
	{
		case frmOpenEvent:
					
			// Set focus on MainChangeFontButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainChangeFontButton));
			
			FrmDrawForm(frmP);
			ToggleLabelsDisplay(false);
						
			handled = true;
			break;
      
            
 		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == MainChangeFontButton)
			{
				// Initialization of the UI for the font picker
				InitFontUI(gLibRefNum);
				// Set the rules for the font picker
				SetFontUIRules(gLibRefNum, gRules);

				previousFontID = gCurrentFontID;
				if(DoFontDialog(gLibRefNum, &gCurrentFontID, stdFont))
				{	   
					FieldType *fldText = FrmGetObjectPtr(frmP, 
														 FrmGetObjectIndex(frmP, MainTextAreaField));
					// Display all UI elements
					ToggleLabelsDisplay(true);
				
					// If a font is selected
					if(previousFontID != gCurrentFontID)
					{
						// Initialize memory for the font pointer
						VMFontPtr currentFntPtr = MemPtrNew(sizeof(VMFontType));
						// Get the pointer to the font from the font ID
						VMFontGetFontByID(gLibRefNum, currentFntPtr, gCurrentFontID); 
						
						// Convert the font to a string
						VMFontToString(gLibRefNum, currentFntPtr, fontString);
						
						// Set the values of the fields with the given information
						SetValues(MainFontField, fontString, 0);					
						SetValues(MainFontIDField, NULL, gCurrentFontID);
						
						
						ToggleLabelsDisplay(true);

						// Set the font to the current font
						FldSetFont(fldText, gCurrentFontID);

						// De-initialize memory for the font pointer
						MemPtrFree(currentFntPtr);
					}
					// If there has been text entered, redraw the field with the given
					// text
					if(FldGetTextPtr(fldText))
						SetFieldText(fldText, FldGetTextPtr(fldText), true);
				}	
				handled = true;
			}
	
			if(eventP->data.ctlSelect.controlID == MainDoneButton)
			{
				ExitApp();
				handled = true;
			}
			
			break;			
	
		case menuEvent:
			
			PrvMenu(eventP->data.menu.itemID);
			break;
			
		default:
			break;
	}
    return handled;
}
