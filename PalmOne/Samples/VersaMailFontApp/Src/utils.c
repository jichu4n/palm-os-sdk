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
 *  @file utils.c
 */



#include <PalmOS.h>
#include "utils.h"


/***********************************************************************
 *
 * FUNCTION:    	SetFieldText
 *
 * DESCRIPTION: 	Sets a given text pointer as the text of a field	
 *
 * PARAMETERS:  	fieldP -> Pointer to the field to be drawn
 *					textP -> Pointer to the text that has to be drawn	
 *					blDraw -> Flag which will draw the text on the field		
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void	SetFieldText(FieldType *fieldP,Char *textP,Boolean blDraw)
{	
	MemHandle	hFieldText=NULL;
	Char	*		fieldText=NULL;
	Int16		iTextSize=0;
	hFieldText = FldGetTextHandle(fieldP);

	if(!hFieldText)
	{
		hFieldText=MemHandleNew(StrLen(textP));
		if(!hFieldText)
			return;
	}
	
	FldSetTextHandle (fieldP, NULL);
	iTextSize=StrLen(textP) + 10;
	
	if(MemHandleResize(hFieldText,iTextSize)==0) {
		fieldText=(Char *)MemHandleLock(hFieldText);

		StrCopy(fieldText,textP);
		MemHandleUnlock(hFieldText);
		FldSetTextHandle (fieldP, hFieldText);
	}
	
	if(blDraw)
		FldDrawField(fieldP);
		
}

/***********************************************************************
 *
 * FUNCTION:    	UpdateListUI
 *
 * DESCRIPTION: 	Updates the popup list with the name of the file	
 *
 * PARAMETERS:  	formID -> Form ID of the active form
 *					popupID -> ID of the popup of the form	
 *					listID -> ID of the list
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void UpdateListUI(UInt16 formID, UInt16 popupID, UInt16 listID)
{
	FormType *frmP = FrmGetFormPtr(formID);
	ListType *lstP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, listID));
	
	// Sets the caption of the popup list to the album name
	CtlSetLabel( (ControlType *)FrmGetObjectPtr(frmP, 
												FrmGetObjectIndex(frmP, popupID)),
												LstGetSelectionText(lstP,LstGetSelection(lstP))
											 	);
}