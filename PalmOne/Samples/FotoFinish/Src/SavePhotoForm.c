/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/
/**
 *
 *  @ingroup FotoFinish
 **/

/**
 * @file SavePhotoForm.c
 * This file contains the event-handling code for the SavePhotoForm
 */

/** @name
 *
**/
/*@{*/

/***********************************************************************
 *
 * File:
 *	SavePhotoForm.c
 *
 * Description:
 *	Contains all event-handling code associated with SavePhotoForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/

#include <PalmOS.h>
#include "SavePhotoForm.h"
#include "FotoFinishRsc.h"
//#include "PalmOnePhotoCommon.h"
#include "SaveUtils.h"
#include "FotoFinish.h"

#define	ALBUM_NAME_LENGTH	256

extern 	ListType 	*gLstP;
extern	UInt16 		gLibRefNum;
extern	WinHandle	gBackBufferWinH;
extern	UInt32		gPhotoWidth;
extern 	UInt32		gPhotoHeight;


extern void CommitToBackBuffer();

/***********************************************************************
 *
 * FUNCTION:    	SavePhotoFormHandleEvent
 *
 * DESCRIPTION: 	Event-handler for the SavePhotoForm - handles the Done
 *					and Cancel buttons on the form, as well as the popup list	
 *					If the user presses the Done button, all the operations 
 *					are committed to the back-buffer, else the backbuffer is kept
 *					as it is
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

Boolean SavePhotoFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	PalmPhotoHandle newPhotoH = NULL;
	
	switch (eventP->eType) 
	{
		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == SavePhotoDoneButton)
			{
				UInt16	albumIndex = noListSelection;
				FieldType	*fldFileName;
				Char	*fileName = NULL;
				UInt16	albID = 0;
				UInt16	length = 0;
				
				FormType *frmP = FrmGetActiveForm();
				
				albumIndex = LstGetSelection(gLstP);
				
				
				// Pick an album to save the photo to
				if(albumIndex != noListSelection)
					albID = PalmPhotoGetAlbumID(gLibRefNum, albumIndex);
				else
				{
					FrmAlert(PickAlbumFirstErrorAlert);
					handled = true;
					goto Done;
				}
				
				// Get the pointer to the field which has the name of the file
				fldFileName = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SavePhotoFileNameField));
				
				// If the user has entered a name, assign memory for the name of the file
				if(FldGetTextPtr(fldFileName)) 
				{
					length = StrLen(FldGetTextPtr(fldFileName)) + 6 ;
				}
				else
				{
					FrmAlert(PickFileNameFirstErrorAlert);
					handled = true;
					goto Done;
				}
				
				fileName = (Char *)MemPtrNew(length);
				MemSet(fileName, length, 0);
				StrCopy(fileName, FldGetTextPtr(fldFileName));

				// If the user has not entered a jpg as an extension, 
				// add .jpg to the name of the file
				if(!StrStr(fileName, ".jpg"))
				{  
					StrCat(fileName, ".jpg");		
				}
				
				// Commit all the draw operations that have been performed, to the back-buffer -> 
				// gBackBufferWinH
				CommitToBackBuffer();
				
				// Save the photo to the album specified
				SavePhoto(gLibRefNum, gBackBufferWinH, fileName, albID, newPhotoH, gPhotoWidth, gPhotoHeight);

				MemPtrFree(fileName);

			}	

			break;
			
		default:
		    break;
	
	}
	
Done:
	return handled;
}

/*@}*/
