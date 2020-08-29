/***********************************************************************
 *
 * FILE:		Source file for AppUtilities library
 *
 * HISTORY: 
 * 		1.2	  jbp	20050205	Removed frmCloseEvent code from
 * 								AppHandleTabKeyEvents().
 *		1.1	  jbp	20041112	Added Date/Time routines.
 *		1.0	  jbp	20040703	Initial version
 *
 *	Copyright © 2006 Palm, Inc. or its subsidiaries.	All rights reserved.
 *	
 *	You may incorporate this sample code (the "Code") into your applications
 *	for Palm OS(R) platform products and may use the Code to develop such 
 *	applications without restriction. The Code is provided to you on an "AS IS" 
 *	basis and the responsibility for its operation is 100% yours.
 *	PALMSOURCE, INC. AND ITS SUBSIDIARIES (COLLECTIVELY, "PALM") DISCLAIM ALL 
 *	WARRANTIES, TERMS AND CONDITIONS WITH RESPECT TO THE CODE, EXPRESS, IMPLIED, 
 *	STATUTORY OR OTHERWISE, INCLUDING WARRANTIES, TERMS OR CONDITIONS OF 
 *	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT AND 
 *	SATISFACTORY QUALITY. You are not permitted to redistribute the Code on a 
 *	stand-alone basis. TO THE FULL EXTENT ALLOWED BY LAW, PALMSOURCE ALSO 
 *	EXCLUDES ANY LIABILITY, WHETHER BASED IN CONTRACT OR TORT (INCLUDING 
 *	NEGLIGENCE), FOR INCIDENTAL, CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE 
 *	DAMAGES OF ANY	KIND, OR FOR LOSS OF REVENUE OR PROFITS, LOSS OF BUSINESS, 
 *	LOSS OF INFORMATION OR DATA, OR OTHER FINANCIAL LOSS ARISING OUT OF OR IN 
 *	CONNECTION WITH THE USE OR PERFORMANCE OF THE CODE.	 The Code is subject to 
 *	Restricted Rights for U.S. government users and export regulations.
 *
 ******************************************************************************/

#include "AppUtilities.h"

// Private structure used to sort form text fields for auto-tab support 
typedef struct PrvFldInfoType {
	UInt16		u16Index;
	Int16		i16XPos;
	Int16		i16YPos;
} PrvFldInfoType;


/***********************************************************************
 *
 * FUNCTION:	AppGetFormObjectPtr
 *
 * DESCRIPTION: Get a pointer to an object in the specified form.
 *
 * PARAMETERS:	-> pForm: form's data
 *				 	-> uwFieldID: field object id
 *
 * RETURNED:	Pointer to object's data
 *
 ***********************************************************************/
void *AppGetFormObjectPtr(FormType *pForm, UInt16 u16ObjectID)
{	
	UInt16	u16ObjectIndex;
	
	if (pForm == NULL)
		pForm = FrmGetActiveForm();
		
	ASSERT(pForm != NULL);
	
	u16ObjectIndex = FrmGetObjectIndex(pForm, u16ObjectID);
	
	return FrmGetObjectPtr(pForm, u16ObjectIndex);
}


/***********************************************************************
 *
 * FUNCTION:	AppUpdateScrollbar
 *
 * DESCRIPTION: Scroll a text field with a scrollbar.
 *
 * PARAMETERS:	-> pForm: the form that owns the field and scrollbar
 *				 	-> u16FieldID: ID of field to scroll
 *				 	-> u16ScrollID: ID of scrollbar that scrolled it
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/
void AppUpdateScrollbar(FormType *pForm, UInt16 u16FieldID, 
								UInt16 u16ScrollID)
{
	ScrollBarType	*pScroll;
	FieldType		*pField;
	UInt16			u16ScrollPos, u16TextHt, u16FieldHt, u16MaxValue;

	pScroll = (ScrollBarType *)AppGetFormObjectPtr(pForm, u16ScrollID);
	pField = (FieldType *)AppGetFormObjectPtr(pForm, u16FieldID);

	FldGetScrollValues(pField, &u16ScrollPos, &u16TextHt, &u16FieldHt);
	
	if (u16TextHt > u16FieldHt)
		u16MaxValue = u16TextHt - u16FieldHt;
	else if (u16ScrollPos)
		u16MaxValue = u16ScrollPos;
	else
		u16MaxValue = 0;

	SclSetScrollBar(pScroll, (Int16)u16ScrollPos, 0, (Int16)u16MaxValue, 
													(Int16)u16FieldHt - 1);
}


/***********************************************************************
 *
 * FUNCTION:	AppGetControlValue
 *
 * DESCRIPTION: Get the current value of a control given the form on which
 *					it resides and the control's ID.
 *
 * PARAMETERS:	-> pForm: the form that owns the control.
 *				 	-> u16ControlID: ID of the control.
 *
 * RETURNED:	A boolean value indicating whether the control is on (true) or
 *					off (false). Note that this function should not be used with
 *					controls such as sliders that return a numeric value.
 *
 ***********************************************************************/
Boolean AppGetControlValue(FormType *pForm, UInt16 u16ControlID)
{
	ControlType *pControl = (ControlType *)AppGetFormObjectPtr(pForm, u16ControlID);
	
	return (Boolean)CtlGetValue(pControl);
}


/***********************************************************************
 *
 * FUNCTION:	AppSetControlValue
 *
 * DESCRIPTION: Set the current value of a control given the form on which
 *					it resides and the control's ID. Note that this function should 
 *					not be used with controls such as sliders that take a numeric
 *					value.
 *
 * PARAMETERS:	-> pForm: the form that owns the control.
 *				 	-> u16ControlID: ID of the control.
 *					-> fValue: supply a value of true to turn the control on, or
 *									false to turn the control off.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/
void AppSetControlValue(FormType *pForm, UInt16 u16ControlID, Boolean fValue)
{
	ControlType *pControl = (ControlType *)AppGetFormObjectPtr(pForm, u16ControlID);
	
	CtlSetValue(pControl, fValue);
}


/***********************************************************************
 *
 * FUNCTION:	PrvScrollLines
 *
 * DESCRIPTION: Scroll a text field with a scrollbar
 *
 * PARAMETERS:	 -> pForm: the form owning the field and scrollbar
 *				 -> u16FieldID: ID of field to scroll
 *				 -> u16ScrollID: ID of scrollbar that scrolled it
 *				 -> u32LinesToScroll: how much to scroll
 *				 -> fDrawNow: update the field now or later?
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/
static void PrvScrollLines(FormType *pForm, UInt16 u16FieldID, 
						   UInt16 u16ScrollID, Int16 i16LinesToScroll, 
						   Boolean fDrawNow)
{
	FieldType	*pField = (FieldType *)AppGetFormObjectPtr(pForm, u16FieldID);

	if (i16LinesToScroll < 0)
		FldScrollField(pField, (UInt16)-i16LinesToScroll, winUp);
	else
		FldScrollField(pField, (UInt16)i16LinesToScroll, winDown);
	
	if ((FldGetNumberOfBlankLines(pField) && i16LinesToScroll < 0) || fDrawNow)
		AppUpdateScrollbar(pForm, u16FieldID, u16ScrollID);
}


/***********************************************************************
 *
 * FUNCTION:	PrvPageScroll
 *
 * DESCRIPTION: Scroll a pagefull of text in a text field
 *
 * PARAMETERS:	
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/
static void PrvPageScroll(FormType *pForm, UInt16 u16FieldID, 
						  UInt16 u16ScrollID, WinDirectionType wdDirection)
{
	FieldType	*pField = (FieldType *)AppGetFormObjectPtr(pForm, u16FieldID);

	if (FldScrollable(pField, wdDirection)) {
		Int16 i16LinesToScroll = (Int16)FldGetVisibleLines(pField) - 1;

		if (wdDirection == winUp)
			i16LinesToScroll = -i16LinesToScroll;

		PrvScrollLines(pForm, u16FieldID, u16ScrollID, i16LinesToScroll, true);
	}
}


/***********************************************************************
 *
 * FUNCTION:	AppHandleFieldScrollingEvents
 *
 * DESCRIPTION: Handle all events relating to the scrolling of multi-line
 *					text fields with scroll bars. 
 * 
 *				The function:
 *					- updates the scrollbar if the text field is altered (either
 *					by adding text or tap-and-drag)
 *					- Updates the field if the scrollbar is scrolled
 *					- Updates both field and scrollbar if customer presses
 *					the case up or down keys
 * 
 *				TO DO: handle Treo 600 and other device nav keys
 *
 * PARAMETERS:	-> pEvent: pointer to the event
 *				 	-> u16FieldID: field object ID
 *				 	-> u16ScrollbarID: scrollbar object ID
 *
 * RETURNED:	True if the event was handled and thus the calling routine
 *					doesn't need to process it.
 *
 ***********************************************************************/
Boolean AppHandleFieldScrollingEvents(EventType *pEvent, UInt16 u16FieldID,
				UInt16 u16ScrollbarID)
{
	Boolean		fHandled = false;
	Int32		i32LinesToScroll;
	FormType	*pForm;

	switch (pEvent->eType) {
		case sclRepeatEvent:
			// If the scroll event isn't for our scrollbar, exit.
			if (pEvent->data.sclRepeat.scrollBarID != u16ScrollbarID)
				return false;
				
			pForm = FrmGetActiveForm();
			i32LinesToScroll = pEvent->data.sclRepeat.newValue
												- pEvent->data.sclRepeat.value;
			PrvScrollLines(pForm, u16FieldID, u16ScrollbarID, 
												(Int16)i32LinesToScroll, true);
			// Must return false so we can get more scroll events!
			fHandled = false;
			break;

		case fldChangedEvent:
			// If the event isn't for our field, exit.
			if (pEvent->data.fldChanged.fieldID != u16FieldID)
				return false;
				
			pForm = FrmGetActiveForm();
			AppUpdateScrollbar(pForm, u16FieldID, u16ScrollbarID);
			fHandled = true;
			break;

		case keyDownEvent:
			// Hard to validate which field is meant to scroll: there could be
			// more than one on a form. We could check for the focus field, 
			// but what if the field is read-only? The only solution may be to 
			// call this in the form's event handler twice: once for
			// sclRepeatEvent and fldChangedEvent, and again for keyDownEvent
			// (where you decide first which field should scroll and call
			// with the proper IDs).
			pForm = FrmGetActiveForm();
			if (pEvent->data.keyDown.chr == pageUpChr) {
				PrvPageScroll(pForm, u16FieldID, u16ScrollbarID, winUp);
				fHandled = true;
			} else if (pEvent->data.keyDown.chr == pageDownChr) {		 
				PrvPageScroll(pForm, u16FieldID, u16ScrollbarID, winDown);
				fHandled = true;
			}
			break;
			
		default:
			break;
	}
	
	return fHandled;
}


/***********************************************************************
 *
 * FUNCTION:	AppSetFieldText
 *
 * DESCRIPTION:	Set field object's text handle.	 Will reuse an existing
 *						text handle, if any.
 *
 * PARAMETERS:	-> pForm: form's data
 *					-> uwFieldID: field object id
 *					-> pNewText: source text (will be copied)
 *				 	-> fDrawNow: if true, field will be redrawn. For readability,
 *									you may want to use kAppDraNow and kAppDrawLater
 *									instead of true and false.
 *
 * RETURNED:	nothing.
 *
 ***********************************************************************/
void AppCopyTextToField(FormType *pForm, UInt16 uwFieldID, 
							char *pNewText, Boolean fDrawNow)
{
	MemHandle		hText = NULL;
	UInt16		u16NewSize;
	char			*pText;
	FieldType		*pField;
	
	// Get pointer to text field
	pField = (FieldType *)AppGetFormObjectPtr(pForm, uwFieldID);
	
	// If no text supplied, clear the text field.
	if (!pNewText || !*pNewText) {
		FldFreeMemory(pField);
		FldSetTextHandle(pField, NULL);
		if (fDrawNow) {
			FldEraseField(pField);
			FldDrawField(pField);
		}
		return;
	}
	
	// We need to resize the text handle (or create a new handle) to 
	// hold the desired text. Save space for the null char at the end.
	
	u16NewSize = StrLen(pNewText) + 1;			// Calculate new handle size
	hText = FldGetTextHandle(pField);			// Get the current text handle
	FldSetTextHandle(pField, NULL);				// Release this handle from field

	if (hText) {
		// Resize the existing handle
		if (MemHandleResize(hText, u16NewSize))
			goto Exit;
	} else {
		// Must create a new handle
		hText = MemHandleNew(u16NewSize);
		if (!hText) 
			return;
	}

	// Copy the text to the locked handle
	pText = (char *)MemHandleLock(hText);
	StrCopy(pText, pNewText);
	MemPtrUnlock(pText);
	
Exit:
	// Tell the text field about the handle
	FldSetTextHandle(pField, hText);
	
	// Draw the field if needed.
	if (fDrawNow) {
		FldEraseField(pField);
		FldDrawField(pField);
	}
}


/***********************************************************************
 *
 * FUNCTION:	AppGetAppVersionStr
 *
 * DESCRIPTION: Get application's version string from its resource database.
 *
 * PARAMETERS:	<- pBuff: buffer to copy string to. The caller is responsible for
 *								managing this buffer.
 *				 	-> u16BuffLen: The size of pBuff, in bytes. If the length of the
 *										version string exceeds this value, only as much
 *										of the version string as will fit in the buffer
 *										will be copied to it.
 *
 * RETURNED:	errNone if the resource was successfully obtained, or a nonzero
 *					value if it was not.
 *
 ***********************************************************************/
Err AppGetAppVersionStr(char *pBuff, UInt16 u16BuffLen)
{
	MemHandle	hVer;
	char		*pVer;
	Err			err = errNone;
	
	// Get application version resource
	hVer = DmGetResource('tver', 1000); // Get handle to resource
	if (hVer) {
		pVer = (char *)MemHandleLock(hVer);		// Lock the handle; get ptr
		MemSet(pBuff, u16BuffLen, 0);
		StrNCopy(pBuff, pVer, (Int16)u16BuffLen - 1);
		pBuff[u16BuffLen - 1] = 0;
		
		MemHandleUnlock(hVer);				// Unlock and release
		DmReleaseResource(hVer);			// the resource
	} else {
		*pBuff = 0;
		err = 1;
	}
	
	return err;
}


/***********************************************************************
 *
 * FUNCTION:	AppSetFormFocus
 *
 * DESCRIPTION:	Set the form's focus to a specified field, and optionally
 *						highlight the field contents.
 *
 * PARAMETERS:	-> pForm: Pointer to the form that contains the field.
 *				 	-> u16FieldID: ID of the field to be given the focus.
 *				 	-> fHiliteText: If true, select the field text. Otherwise, put
 *										the insertion point at the end of the text. For
 *										readability, you may want to use kAppSelectText
 *										and kAppDontSelectText instead of true and false.
 *
 * RETURNED:	nothing.
 *
 ***********************************************************************/
void AppSetFormFocus(FormType *pForm, UInt16 u16FieldID, Boolean fHiliteText)
{
	UInt16	u16FocusIndex;
	FieldType	*pField;
	UInt16	u16TextLen;
	
	ASSERT(pForm != NULL);
	
	u16FocusIndex = FrmGetObjectIndex(pForm, u16FieldID);
	pField = (FieldType *)FrmGetObjectPtr(pForm, u16FocusIndex);
	u16TextLen = FldGetTextLength(pField);
	
	// Set the focus
	FrmSetFocus(pForm, u16FocusIndex);
	
	if (fHiliteText)
		FldSetSelection(pField, 0, u16TextLen);
	else
		FldSetInsPtPosition(pField, u16TextLen);
		// or FldSetSelection(pField, u16TextLen, u16TextLen);
		
}


/***********************************************************************
 *
 * FUNCTION:	AppGetGadgetData
 *
 * DESCRIPTION:	Get the value stored in the data field of a gadget object.
 *
 * PARAMETERS:	-> pForm: Pointer to the form that contains the gadget.
 *				  	-> u16GadgetID: The gadget's ID.
 *
 * RETURNED:	 Pointer to the gadget's data block.
 *
 ***********************************************************************/
MemPtr AppGetGadgetData(FormType *pForm, UInt16 u16GadgetID)
{
	ASSERT(pForm != NULL);

	return (FrmGetGadgetData(pForm, FrmGetObjectIndex(pForm, u16GadgetID)));
}


/***********************************************************************
 *
 * FUNCTION:	AppFreeGadgetData
 *
 * DESCRIPTION:	Free a gadget's data block.
 *
 * PARAMETERS:	-> pForm: Pointer to the form that contains the gadget.
 *				  	-> u16GadgetID: The gadget's ID.
 *
 * RETURNED:	 Nothing.
 *
 ***********************************************************************/
void AppFreeGadgetData(FormType *pForm, UInt16 u16GadgetID)
{
	MemPtr	pData;
	
	pData = AppGetGadgetData(pForm, u16GadgetID);
	if (pData)
		MemPtrFree(pData);
	
	// Clear the pointer in case we check later
	FrmSetGadgetData(pForm, FrmGetObjectIndex(pForm, u16GadgetID), NULL);
}


/***********************************************************************
 *
 * FUNCTION:	AppFieldIsEmpty
 *
 * DESCRIPTION:	Determine whether a text field is empty of characters.
 *
 * PARAMETERS:	-> pForm: Pointer to the form that contains the field.
 *				  	-> u16FieldID: The field's ID.
 *
 * RETURNED:	 true if the field holds no characters; false, otherwise.
 *
 ***********************************************************************/
Boolean AppFieldIsEmpty(FormType *pForm, UInt16 u16FieldID)
{
	return (Boolean)(FldGetTextLength((FieldType*)AppGetFormObjectPtr(pForm, u16FieldID)) == 0);
}


/***********************************************************************
 *
 * FUNCTION:	AppGetFieldTextPtr
 *
 * DESCRIPTION:	Obtain a pointer to a field's text.
 *
 * PARAMETERS:	-> pForm: Pointer to the form that contains the field.
 *				  	-> u16FieldID: The field's ID.
 *
 * RETURNED:	Pointer to the field's text. If the field is empty, this
 *					function returns NULL.
 *
 ***********************************************************************/
 Char * AppGetFieldTextPtr(FormType *pForm, UInt16 u16FieldID)
 {
	Char	*pText;
	
	if (AppFieldIsEmpty(pForm, u16FieldID))
		return NULL;
	else {
		pText = FldGetTextPtr((FieldType*)AppGetFormObjectPtr(pForm, u16FieldID));
		return pText;
	}
 }
 
  
/***********************************************************************
 *
 * FUNCTION:	AppGetFocusObjectPtr
 *
 * DESCRIPTION:	Get a pointer to the form object with the current focus.
 *
 * PARAMETERS:	-> pForm: Pointer to the form.
 *
 * RETURNED:	Pointer to the object that currently has the focus, or NULL
 *					if no object currently has the focus.
 *
 ***********************************************************************/
FieldType *AppGetFocusObjectPtr(FormType *pForm)
{
	UInt16		u16FocusIndex;
	
	u16FocusIndex = FrmGetFocus(pForm);
	
	// If no object has the focus return NULL pointer.
	if (u16FocusIndex == noFocus)
		return (NULL);
		
	// Return a pointer to the object with focus.
	return (FieldType*)FrmGetObjectPtr(pForm, u16FocusIndex);
}


/***********************************************************************
 *
 * FUNCTION:	PrvCompareFields
 *
 * DESCRIPTION:	Sorting callback to compare the positions of two editable text
 *						fields on a form. Fields are sorted from top
 *						to bottom. Fields with the same vertical position
 *						are then sorted left to right.
 *
 * PARAMETERS:	-> pFldA: pointer to first field's position
 *				  	-> pFldB: pointer to the second field's position
 *					-> i32Unused: unused
 *
 * RETURNED:	A negative number if field A is above or to the left of field
 *					B, 0 if they both occupy exactly the same position on the screen,
 *					and a positive number if A is below or to the right of field B.
 *
 ***********************************************************************/
static Int16 PrvCompareFields(void *pFldA, void *pFldB, Int32 i32Unused)
{
	Int16	i16Delta;
	
	i16Delta = ((PrvFldInfoType *)pFldA)->i16YPos 
										- ((PrvFldInfoType *)pFldB)->i16YPos;
	if (i16Delta == 0)
		i16Delta = ((PrvFldInfoType *)pFldA)->i16XPos 
										- ((PrvFldInfoType *)pFldB)->i16XPos;

	return i16Delta;
}


/***********************************************************************
 *
 * FUNCTION:	 PrvTabGadgetHandler
 *
 * DESCRIPTION:	Gadget callback event handler. This handler only handles
 *					delete events, in which case it frees the gadget's data. This
 *					function is useful because frmCloseEvent is
 *					not sent to modal dialogs (via FrmReturnToForm() or
 *					FrmDoDialog()).
 *
 * PARAMETERS:	-> pGadget: Pointer to gadget data and attributes
 *				  	-> u16Cmd: 	Gadget event. Only formGadgetDeleteCmd is acted upon.
 *				  	-> pParams: Unused (normally for enter events).
 *
 * RETURNED:	 true if the function freed the gadget's data, false otherwise.
 *
 ***********************************************************************/
static Boolean PrvTabGadgetHandler(FormGadgetTypeInCallback *pGadget, 
									UInt16 u16Cmd, void *pParams)
{
	Boolean fHandled = false;
	
	if (u16Cmd == formGadgetDeleteCmd) {
		if (pGadget->data)
			MemPtrFree((void *)pGadget->data);
		pGadget->data = NULL;
	}
	
	return fHandled;
}
									

/***********************************************************************
 *
 * FUNCTION:	 AppInitTabList
 *
 * DESCRIPTION:	Create a list of index values, one for each of the editable
 *					text fields on a specified form, sorted by position (top to 
 *					bottom, left to right). This function will optionally set the
 *					focus to the first (in sorted order) field on the form. 
 *	
 *					This function is used in conjunction with AppHandleTabKeyEvents()
 *					and AppSetFirstTabFocus() to allow the user to easily move
 *					between the fields on a form.
 *
 *					The list is stored as the data of a specified gadget, to avoid
 *					using globals that might interfere with the application.
 * 
 *				 	Call this function in response to frmOpenEvent (or before
 *					FrmDoDialog() for modal dialogs). If the number of
 *					visible, editable fields on a form changes, call this
 *					again to update the list.
 *
 * PARAMETERS:	-> pForm: Pointer to the form containing the text fields.
 *				  	-> u16TabGadgetID: Object ID of the gadget that will hold the 
 *						list.
 *				  	-> fSetFocus: If true, set focus to first field
 * 
 * RETURNED:	 Nothing.
 *
 ***********************************************************************/
void AppInitTabList(FormType* pForm, UInt16 u16TabGadgetID, Boolean fSetFocus)
{
	UInt16		i, u16NumFields, u16NumFormObjects, u16GadgetIndex;
	FieldType		*pFld;
	PrvFldInfoType	*pFldList;
	PrvFldInfoType	*pPair;
	FieldAttrType	attr;
	
	// Clear any current list. There are no errors if the list already exists.
	AppFreeGadgetData(pForm, u16TabGadgetID);

	u16NumFormObjects = FrmGetNumberOfObjects(pForm);
	u16NumFields = 0;
	
	// Allocate space for our initial list. There will more space than we
	// need, but this way we make one allocation and only one reallocation.
	pFldList = (PrvFldInfoType *)MemPtrNew(u16NumFormObjects 
													* sizeof(PrvFldInfoType));
	pPair = pFldList;
	
	// Look for visible, editable text fields and add index and position to list
	if (pFldList) {
		for (i = 0; i < u16NumFormObjects; i++) {
			if (FrmGetObjectType(pForm, i) == frmFieldObj) {
				pFld = (FieldType*)FrmGetObjectPtr(pForm, i);
				FldGetAttributes(pFld, &attr);
				
				if (attr.usable && attr.editable) {
					u16NumFields++;
					
					// Store the values for this field: index and form position.
					pPair->u16Index = i;
					FrmGetObjectPosition(pForm, i, &pPair->i16XPos, &pPair->i16YPos);
					
					// Increment the list position pointer.
					++pPair;
				}
			}
		}
	}
	
	// Shrink the list. If there are no qualifiying fields, the memory for
	// the list is freed and pFldList is NULL.
	if (pFldList)
		MemPtrResize(pFldList, u16NumFields * sizeof(PrvFldInfoType));
	
	// Sort the list by y and x position. Fields are sorted top to bottom. 
	// Fields that occur on the same line (y positions are equivalent) are 
	// sorted left to right.
	if (pFldList)
		SysQSort(pFldList, u16NumFields, sizeof(PrvFldInfoType), 
														PrvCompareFields, 0);
	
	// Store the index list with the gadget
	u16GadgetIndex = FrmGetObjectIndex(pForm, u16TabGadgetID);
	FrmSetGadgetData(pForm, u16GadgetIndex, pFldList);
	
	// Set the gadget handler (takes care of freeing list when form closes).
	FrmSetGadgetHandler(pForm, u16GadgetIndex, PrvTabGadgetHandler);
	
	if (fSetFocus)
		AppSetFirstTabFocus(pForm, u16TabGadgetID);
}


/***********************************************************************
 *
 * FUNCTION:	AppHandleTabKeyEvents
 *
 * DESCRIPTION:	Handle tab (and tab-like) keys, moving focus between a form's
 *						fields as appropriate. Optionally select the contents of the
 *						field receiving the focus (otherwise, the insertion point is
 *						placed at the end of the field's text).
 *
 *						This function assumes that you have constructed a "tab list"
 *						- a sorted list of the form's editable text fields - using
 *						AppInitTabList().
 *
 * PARAMETERS:	-> pForm: Pointer to the form.
 *				  	-> u16TabGadgetID: ID of the gadget that contains the tab list.
 *					-> fSelectText: true if the contents of the field receiving the
 *											focus should be selected.
 *
 * RETURNED:	true if at the end of the function the focus has been set to
 *					an editable text field.
 *
 ***********************************************************************/
Boolean AppHandleTabKeyEvents(EventType* pEvent, UInt16 u16TabGadgetID, 
														Boolean fSelectText)
{
	FormType		*pForm;
	Int16			i, i16Direction;
	UInt16			u16NumFields, u16OldFocusIndex, u16NewFocusIndex;
	PrvFldInfoType	*pFldList;
	PrvFldInfoType	*pID;
	
	// Call this event handler before any other form events. It only
	// handles keyDown events.
	if (pEvent->eType != keyDownEvent)
		return false;
			
	// Ignore virtual chars
	if (EvtKeydownIsVirtual(pEvent))
		return false;
		
	// We only care about next, previous, and tab chars
	// TODO - extend to include nav resource events
	if (pEvent->data.keyDown.chr == prevFieldChr)
		i16Direction = -1;
	else if (pEvent->data.keyDown.chr == nextFieldChr)
		i16Direction = 1;
	else if (pEvent->data.keyDown.chr == tabChr)
		i16Direction = 1;
	else
		return false;
	
	// Get the tab list
	pForm = FrmGetActiveForm();	
	pFldList = (PrvFldInfoType *)AppGetGadgetData(pForm, u16TabGadgetID);
	if (!pFldList)
		return false;
	
	// Finally, handle the tabs. If there is only one field, exit.
	u16NumFields = (UInt16)(MemPtrSize(pFldList) / sizeof(PrvFldInfoType));
	if (u16NumFields < 2)
		return false;
		
	// Get the field that has the focus
	u16OldFocusIndex = FrmGetFocus(pForm);
	// DOLATER jbp: if no current focus, set to first one?
	if (u16OldFocusIndex == noFocus)
		return false;
	
	// Find the focus index in the tab list
	pID = pFldList;
	for (i = 0; i < u16NumFields; i++, pID++)
		if (pID->u16Index == u16OldFocusIndex)
			break;
	
	// If we couldn't find the index in the list, set to the first one.
	if (i == u16NumFields)
		u16NewFocusIndex = pFldList->u16Index;
	else {
		i += i16Direction;
		if (i == u16NumFields)
			i = 0;
		else if (i < 0)
			i = (Int16)u16NumFields - 1;
		
		pFldList += i;
		u16NewFocusIndex = pFldList->u16Index;
	}
	
	// Set the new focus and hilite field text.
	AppSetFormFocus(pForm, FrmGetObjectId(pForm, u16NewFocusIndex), fSelectText);
	
	return true;
}


/***********************************************************************
 *
 * FUNCTION:	AppSetFirstTabFocus
 *
 * DESCRIPTION:	Set the focus to the first field in the tab list.
 *
 *						This function assumes that you have constructed a "tab list"
 *						- a sorted list of the form's editable text fields - using
 *						AppInitTabList().
 *
 * PARAMETERS:	-> pForm: Pointer to the form.
 *					-> u16TabGadgetID: ID of the gadget that contains the tab list.
 *
 * RETURNED:	 Nothing.
 *
 ***********************************************************************/
void AppSetFirstTabFocus(FormType* pForm, UInt16 u16TabGadgetID)
{
	PrvFldInfoType	*pFldList;
	
	// Get the tab list
	pFldList = (PrvFldInfoType *)AppGetGadgetData(pForm, u16TabGadgetID);
	
	if (pFldList == NULL)
		return;
	
	AppSetFormFocus(pForm, FrmGetObjectId(pForm, pFldList->u16Index), true);
}


/***********************************************************************
 *
 * FUNCTION:	AppSetDatabaseAttributes
 *
 * DESCRIPTION:	Set a database's attribute bits. This function is particularly
 *						useful when setting the database's backup bit.
 *
 * PARAMETERS:	-> dbRef: A valid reference to an open database.
 *				  	-> u16AttrBits: Bit field of the attributes to set. (For
 *										a complete list, see DataMgr.h.) Note that 
 *										this function only sets the specified attribute
 *										bits. No attributes are cleared by this function.
 *
 * RETURNED:	 Nothing.
 *
 ***********************************************************************/
void AppSetDatabaseAttributes(DmOpenRef dbRef, UInt16 u16NewAttr)
{
	LocalID		dbID;
	UInt16		u16CardNum;
	UInt16		u16Attr;
	Err error;
	// Must pass opened database reference
	if (dbRef == NULL) {
		ErrFatalDisplay("Null dbRef");
		return;
	}
	
	// We need the dbID and card number in order to call DmDatabaseInfo()
	error = DmOpenDatabaseInfo(dbRef, &dbID, NULL, NULL, &u16CardNum, NULL);
	
	// Note that we don't modify any attributes other than
	// the ones that were passed as parameters.
	error = DmDatabaseInfo(u16CardNum, dbID, NULL, &u16Attr, NULL, NULL,
				   NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	
	u16Attr |= u16NewAttr;
	
	error = DmSetDatabaseInfo(u16CardNum, dbID, NULL, &u16Attr, NULL, NULL,
					  NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}


/***********************************************************************
 *
 * FUNCTION:	AppDoubleToStr
 *
 * DESCRIPTION:	Convert a double to a string (poor man's sprintf()). The
 *						generated string is null-terminated. The first character is
 *						reserved for a sign (either " " or "-"), and the resulting
 *						string always contains a decimal point.
 *
 * PARAMETERS:	<- strBuffer: Destination buffer to receive the returned string.
 *										Note that it is the caller's responsibility to
 *										preallocate a buffer of sufficient size to hold
 *										the string.
 *				  	-> dblNumber:	The value to convert.
 *				  	-> u16NumFractDigits:	Number of digits after the decimal point.
 *													Even if this value is zero, the resulting
 *													string contains a decimal point.
 *
 * RETURNED:	 nothing
 *
 ***********************************************************************/
void AppDoubleToStr(char *strBuffer, double dblNumber, UInt16 u16NumFractDigits)
{
	Int32		i32LongNum;
	double		bldIntegerPart, dblZeros, dblRound;
	Int16		i;
	UInt16		u16StrLen, u16Remainder;
	char		sign = ' ';

	if (dblNumber < 0.0) { 
		dblNumber =- dblNumber;
		sign = '-'; 
	}

	dblZeros = 1.0;
	for (i = 0; i < u16NumFractDigits; i++)
		dblZeros *= 10.0;

	dblRound = 0.5 / dblZeros;
	dblNumber += dblRound;
	bldIntegerPart = (Int32)dblNumber;
	dblNumber = dblNumber - bldIntegerPart;
	strBuffer[0] = sign;

	if (sign == '-')
		StrIToA(&strBuffer[1], (Int32)bldIntegerPart);
	else
		StrIToA(&strBuffer[0], (Int32)bldIntegerPart);

	u16StrLen = StrLen(strBuffer);

	strBuffer[u16StrLen] = '.';
	strBuffer[u16NumFractDigits + u16StrLen + 1] = '\0';

	i32LongNum = (Int32)(dblNumber * dblZeros);
		
	for (i = (Int16)(u16NumFractDigits + u16StrLen); i > (Int16)u16StrLen; i--) {
		u16Remainder = (UInt16)(i32LongNum % 10L);
		strBuffer[i] = (char)(u16Remainder + (UInt16)'0');
		i32LongNum /= 10L;
	}
}


/***********************************************************************
 *
 * FUNCTION:	AppStrLCopy
 *
 * DESCRIPTION:	Copies a source string to a destination string, until
 *						the end of either string is reached or a specified maximum
 *						number of characters have been copied.
 *
 * PARAMETERS:	<-> pDstStr: The string into which the source will be copied.
 *				  	-> pSrcStr:	The string to be copied.
 *				  	-> u16Size:	The maximum number of characters to write to the
 *									destination string. Note that this includes the
 *									null terminator that this function always writes
 *									to the destination (unless u16Size is zero), so
 *									that at most u16Size-1 characters are copied from
 *									the source string.
 *
 * RETURNED:	strlen(pSrcStr). Note that if the returned value is greater than 
 *					or equal to u16Size, truncation occurred.
 *
 ***********************************************************************/
UInt16 AppStrLCopy(char *pDstStr, const char *pSrcStr, UInt16 u16Size)
{
	char		*d = pDstStr;
	const char	*s = pSrcStr;
	UInt16		n = u16Size;

	// Copy as many bytes as will fit
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	// Not enough room in pDstStr; add NULL and traverse rest of pSrcStr
	if (n == 0) {
		if (u16Size != 0)
			*d = '\0';		// NULL-terminate pDstStr
		while (*s++)
			;
	}
	
	// Count doesn't include NULL
	return (UInt16)(s - pSrcStr - 1);
}


/***********************************************************************
 *
 * FUNCTION:	AppStrLCat
 *
 * DESCRIPTION:	Appends a source string to a destination string, until
 *						the end of the source string is reached or the destination
 *						buffer becomes full.
 *
 *						At most u16Size-1 characters will be copied. This function
 *						always adds a null terminator unless 
 *						u16Size <= strlen(pDstStr).
 *
 * PARAMETERS:	<-> pDstStr: The string into which the source will be copied.
 *				  	-> pSrcStr:	The string to be copied.
 *				  	-> u16Size:	The full size of the destination buffer. Note that
 *									unlike with strncat(), this is not the space left
 *									in that buffer!
 *
 * RETURNED:	strlen(pSrcStr) + MIN(u16Size, strlen(initial pDstStr)). Note 
 *					that if the returned value is greater than or equal to u16Size, 
 *					truncation occurred.
 *
 ***********************************************************************/
UInt16 AppStrLCat(char *pDstStr, const char *pSrcStr, UInt16 u16Size)
{
	char		*d = pDstStr;
	const char	*s = pSrcStr;
	UInt16		n = u16Size;
	UInt16		dlen;

	/* Find the end of pDstStr and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
		
	dlen = (UInt16)(d - pDstStr);
	n = u16Size - dlen;

	if (n == 0)
		return(dlen + StrLen(s));
		
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	
	*d = '\0';

	// Count doesn't include NULL
	return (UInt16)(dlen + (s - pSrcStr));
}


/***********************************************************************
 *
 * FUNCTION:	AppGetCurrentDateTime
 *
 * DESCRIPTION:	Get current data and time in a DateTimeType struct.
 *
 * PARAMETERS:	<-	pDt: Pointer to structure that receives the date-time info.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/
void AppGetCurrentDateTime(DateTimeType *pDt)
{
	UInt32		u32Secs;
	
	u32Secs = TimGetSeconds();
	TimSecondsToDateTime(u32Secs, pDt);
}


/***********************************************************************
 *
 * FUNCTION:	AppGetCurrentDateStr
 *
 * DESCRIPTION:	Create a date string from the current date based on the user's
 *						prefs.
 *
 * PARAMETERS:	<-	pStrBuffer: Pointer to buffer to receive the date string. Note
 *										that the caller is responsible for ensuring that
 *										this buffer is sufficiently large to hold the
 *										generated string. This buffer must be of length 
 *										dateStringLength for short formats or 
 *										longDateStrLength for long formats.
 *				 	->	fUseLongFormat: If true, use long format (Nov 8, 2004),
 *					   					else, use short format (11/8/04).
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/
void AppGetCurrentDateStr(char *pStrBuffer, Boolean fUseLongFormat)
{
	DateTimeType	dt;
	DateFormatType	format;
	
	// Get current date.
	AppGetCurrentDateTime(&dt);
	
	// Get user's preference for date format
	if (fUseLongFormat)
		format = (DateFormatType)PrefGetPreference(prefLongDateFormat);
	else
		format = (DateFormatType)PrefGetPreference(prefDateFormat);
	
	DateToAscii(dt.month, dt.day, dt.year, format, pStrBuffer);
}


/***********************************************************************
 *
 * FUNCTION:	AppGetCurrentTimeStr
 *
 * DESCRIPTION:	Create a time string from the current time based on the user's
 *						prefs.
 *
 * PARAMETERS:	<-	pStrBuffer: Pointer to buffer to receive the time string. Note
 *										that the caller is responsible for ensuring that
 *										this buffer is sufficiently large to hold the
 *										generated string. This buffer must be of length 
 *										timeStringLength.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/
void AppGetCurrentTimeStr(char *pStrBuffer)
{
	DateTimeType	dt;
	TimeFormatType	format;
	
	// Get current date.
	AppGetCurrentDateTime(&dt);
	
	// Get user's preference for time format
	format = (TimeFormatType)PrefGetPreference(prefTimeFormat);
	
	TimeToAscii(dt.hour, dt.minute, format, pStrBuffer);
}


/***********************************************************************
 *
 * FUNCTION:	PrvSendDB
 *
 * DESCRIPTION:	Callback function for ExgDBWrite/AppSendDatabase
 *
 * PARAMETERS:	-> pData: Pointer to data to send
 *				 	<-> pSize: In - how much data to send; out - how much was sent
 *				 	<-> pSocket: the Exchange Manaager socket
 *
 * RETURNED:	Any errors encountered while sending.
 *
 ***********************************************************************/
static Err PrvSendDB(const void *pData, UInt32 *pSize, void *pSocket)
{
	Err	 err;
	
	*pSize = ExgSend((ExgSocketType *)pSocket, (void *)pData, *pSize, &err);
	
	return err;
}


/***********************************************************************
 *
 * FUNCTION:	AppSendDatabase
 *
 * DESCRIPTION:	Given a database ID, send the database using the Exchange
 *						Manager.
 *
 * PARAMETERS:	-> dbID: Database ID (local ID) of the database to send.
 *				  	-> pName: Name of the database in filename.sfx format.
 *				  	-> pDescription: User-friendly description of database.
 *
 * RETURNED:	errNone if the database was sent without error, or an error
 *					value indicating the error that was encountered.
 *
 ***********************************************************************/
Err AppSendDatabase(LocalID dbID, Char *pName, Char *pDescription)
{
	ExgSocketType	sock;
	Err				err;
	
	// Clear the exchange socket. All unused fields MUST be set to zero.
	MemSet(&sock, sizeof(ExgSocketType), 0);
	
	// Set some of the fields (the minimum)
	sock.name = pName;					// file-name.pdb
	sock.description = pDescription;	// user description
	
	// Send the data. ExgDBWrite() calls the callback routine until all is sent.
	err = ExgPut(&sock);
	err = ExgDBWrite(PrvSendDB, (void *)&sock, NULL, dbID, 0);
	err = ExgDisconnect(&sock, err);
	
	return err;
}
