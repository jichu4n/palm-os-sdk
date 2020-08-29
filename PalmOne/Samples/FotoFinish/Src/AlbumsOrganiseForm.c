/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/
/**
 *  @ingroup FotoFinish
 **/

/**
 * @file AlbumsOrganiseForm.c
 * This file contains event-handling code for the AlbumsOrganiseForm
 */

/** @name
 *
**/
/*@{*/


/***********************************************************************
 *
 * File:
 *	AlbumsOrganiseForm.c
 *
 * Description:
 *	Contains all event-handling code associated with AlbumsOrganiseForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/
#include <PalmOS.h>
#include "AlbumsOrganiseForm.h"
#include "FotoFinishRsc.h"
//#include "PalmOnePhotoCommon.h"
#include "FotoFinish.h"
#include "utils.h"

extern	UInt16 gLibRefNum;

extern UInt16	gAlbumCount;
extern Char 	**gAlbumList;

Char	*gAlbumName = NULL;

/***********************************************************************
 *
 * FUNCTION:    	PrvHandleRenameButtonEvent
 *
 * DESCRIPTION: 	Handles the event when the Rename Album button is pressed
 *
 * PARAMETERS:  	frmAlbumNameP -> Pointer to the AlbumName dialog
 *					fldAlbumNameP -> Pointer to the text field in the AlbumName dialog
 *					lstP -> Pointer to the list of albums
 *
 *
 * RETURNED:    	Boolean handled -> denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvHandleRenameButtonEvent(FormType *frmAlbumNameP, FieldType *fldAlbumNameP, ListType *lstP)
{
	Int16 sel = noListSelection;
	Char *albumName = NULL;
	Int16 albumIndex = noListSelection;
	UInt16 albumId = 0;
	Err err = errNone;
	Boolean handled = true;
	
	// Gets the name of the selected album
	sel = LstGetSelection(lstP);
	albumName = LstGetSelectionText(lstP, sel);
				
	// The field is set with the album name and is selected	
	SetFieldText(fldAlbumNameP, albumName, false);
	FldSetSelection(fldAlbumNameP, 0, StrLen(albumName));
				
	if(FrmDoDialog(frmAlbumNameP) == AlbumNameDoneButton)
	{
		// If the user presses the done button the dialog, it gets the new album name
		// and renames the album
		if(FldGetTextPtr(fldAlbumNameP))
		{
			gAlbumName = (Char *)FldGetTextPtr(fldAlbumNameP);
			albumIndex = LstGetSelection(lstP);
			if(albumIndex >= 0)
				albumId = PalmPhotoGetAlbumID(gLibRefNum, albumIndex);

			err = PalmPhotoRenameAlbum(gLibRefNum, albumId, gAlbumName);
				
			if(err)
			{
				if(err != palmPhotoLibErrAlbumExist)
				{
					FrmAlert(RenameAlbumErrorAlert);
					goto Done;
				}
			}
			else
			{
				// De-allocates memory for the album name list
				DeInitAlbumList();
				InitAlbumList();
				InitAlbumListUI(lstP);
			}
		}
	}
		
Done:
	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    	PrvHandleDeleteButtonEvent
 *
 * DESCRIPTION: 	Handles the event when the Delete Album button is pressed
 *
 * PARAMETERS:		 lstP -> Pointer to the list of albums
 *
 * RETURNED:    	Boolean handled -> denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvHandleDeleteButtonEvent(ListType *lstP)
{
	Int16 albumIndex = noListSelection;
	Boolean bMoveToUnfiled = false;
	UInt16 albumId = 0;
	Err err = errNone;
	Boolean handled = true;
				
	// Gets the index of the selection made on the list
	albumIndex = LstGetSelection(lstP);
	
	// Gets the album ID from the album index				
	if(albumIndex >= 0)
		albumId = PalmPhotoGetAlbumID(gLibRefNum, albumIndex);
					
	// Checks if the user wants to move photos in the album to the unfiled
	// album
	bMoveToUnfiled = !(FrmAlert(MoveToUnfiledAlert));
	
	// Delete album!				
	err = PalmPhotoDeleteAlbum(gLibRefNum, albumId, bMoveToUnfiled);
	
	if(err)
	{
		FrmAlert(DeleteAlbumErrorAlert);
		handled = true;
		goto Done;
	}
	else
	{
		// De-allocates memory for the album name-list
		DeInitAlbumList();
		InitAlbumList();
		InitAlbumListUI(lstP);
	}

Done:
	return handled;

}

/***********************************************************************
 *
 * FUNCTION:    	PrvHandleNewButtonEvent
 *
 * DESCRIPTION: 	Handles the event when the New Album button is pressed
 *
 * PARAMETERS:  	frmAlbumNameP -> Pointer to the AlbumName dialog
 *					fldAlbumNameP -> Pointer to the text field in the AlbumName dialog
 *					lstP -> Pointer to the list of albums
 *
 *
 * RETURNED:    	Boolean handled -> denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvHandleNewButtonEvent(FormType *frmAlbumNameP, FieldType *fldAlbumNameP, ListType *lstP)
{
	Boolean handled = true;
	UInt16 albumId = 0;
	Err err = errNone;
	
	if(FrmDoDialog(frmAlbumNameP) == AlbumNameDoneButton)
	{
		// If the user presses the done button on the Album Name dialog
		if(FldGetTextPtr(fldAlbumNameP))
		{
			// Gets the name of the album which the user entered....
			gAlbumName = (Char *)FldGetTextPtr(fldAlbumNameP);

			// and creates the album with that name
			albumId = PalmPhotoNewAlbum(gLibRefNum, gAlbumName, &err);
						
			if(err)
			{
				FrmAlert(AlbumInitAlert);
				handled = true;
				goto Done;
			}
			else
			{
				// De-allocates memory for the album name-list
				DeInitAlbumList();
				InitAlbumList();
				InitAlbumListUI(lstP);
			}
		}
	}

Done:
	return handled;

}

/***********************************************************************
 *
 * FUNCTION:    	AlbumsOrganiseFormFormHandleEvent
 *
 * DESCRIPTION: 	Event-handler for the AlbumsOrganiseFormForm - handles the Done
 *					and Cancel buttons on the form, as well as the popup list	
 *
 * PARAMETERS:  	EventType *eventP -> Pointer to the Event structure which is passed
 *					to the event handler
 *
 * RETURNED:    	Boolean handled -> denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/


Boolean AlbumsOrganiseFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	FormType *frmAlbumsOrganiseP = FrmGetFormPtr(AlbumOrganiseForm);

    FormType  *frmAlbumNameP;
    FieldType *fldAlbumNameP;
    
    FormType *frmAddP = FrmGetActiveForm();
																	
	ListType *lstP = FrmGetObjectPtr(frmAlbumsOrganiseP, 
									FrmGetObjectIndex(frmAlbumsOrganiseP, AlbumOrganiseAlbumList));

	switch (eventP->eType) 
	{
		case ctlSelectEvent:
			
			switch(eventP->data.ctlSelect.controlID)
			{
				case AlbumOrganiseRenameAlbumButton:
				    frmAlbumNameP = FrmInitForm(AlbumNameForm);	
	                fldAlbumNameP = (FieldType *)FrmGetObjectPtr(frmAlbumNameP, 
												FrmGetObjectIndex(frmAlbumNameP, AlbumNameAlbumField));
					handled = PrvHandleRenameButtonEvent(frmAlbumNameP, fldAlbumNameP, lstP);
					FrmEraseForm(frmAlbumNameP);
                    FrmDeleteForm(frmAlbumNameP);
                    FrmDrawForm(frmAddP);
					break;
			
				case AlbumOrganiseDeleteAlbumButton:
				    if(!FrmAlert(DeleteAlbumAlert))
					  handled = PrvHandleDeleteButtonEvent(lstP);
					else
					  handled = true;
					  
					FrmDrawForm(frmAddP);
					break;

				case AlbumOrganiseNewAlbumButton:
					frmAlbumNameP = FrmInitForm(AlbumNameForm);	
	                fldAlbumNameP = (FieldType *)FrmGetObjectPtr(frmAlbumNameP, 
												FrmGetObjectIndex(frmAlbumNameP, AlbumNameAlbumField));

					handled = PrvHandleNewButtonEvent(frmAlbumNameP, fldAlbumNameP, lstP);
					FrmEraseForm(frmAlbumNameP);
                    FrmDeleteForm(frmAlbumNameP);
                    FrmDrawForm(frmAddP);
					break;
			}	
			break;
		default:
			break;
	}
	
	return handled;
}
/*@}*/
