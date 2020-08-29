/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	DetailsForm.c
 *
 * Description:
 *	Contains all event-handling code associated with DetailsForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *          1.1     - library update   (10/10/03)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include "DetailsForm.h"
#include "utils.h"
#include "PalmPhoto.h"
#include "PalmOneNavigator.h"
#include "FotoFinishRsc.h"
#include "FotoFinishV2.h"

#define	THUMBNAIL_X			3
#define	THUMBNAIL_Y			90
#define THUMBNAIL_WIDTH		48
#define	THUMBNAIL_HEIGHT	36	

extern 	UInt16		gLibRefNum;
extern 	ListType 	*gLstP;
extern 	UInt32 		gNumImagesInAlbum;
extern 	UInt16		gPhotoIndex;
extern 	UInt16		gAlbumCount;
extern 	Char 		**gAlbumList;

PalmPhotoSelectionParam	gDetailsSelectionParam;

static	Char		gFileName[20] = "";
static	Char		gNotes[50] = "";
static	Boolean		gEditFlag = false;

extern UInt32 verPresent;

/***********************************************************************
 *
 * FUNCTION:    	PrvMovePhoto
 *
 * DESCRIPTION: 	Moves a photo from one album to another.
 *
 *
 * PARAMETERS:  	photoToMoveH -> PhotoHandle which is the photo to be
 *									moved	
 *
 * RETURNED:    	Err -> Returns the error code returned by PalmPhotoSetImageExtraInfo	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Err PrvMovePhoto(PalmPhotoHandle photoToMoveH)
{
	Err err = errNone;
	Int16	albumIndex = noListSelection;
	Char	moveToAlbumName[30] = "";
				
	albumIndex = LstGetSelection(gLstP);
				
	if(albumIndex != noListSelection)
		StrCopy(moveToAlbumName, gAlbumList[albumIndex]);
				
	if(photoToMoveH)
	{
		PalmPhotoExtraInfoParam extraInfo;
			
		MemSet(&extraInfo, sizeof(PalmPhotoExtraInfoParam), 0);
						
		extraInfo.infoType = palmPhotoExtraInfoAlbum;
		extraInfo.infoP = moveToAlbumName;
		extraInfo.infoSize = StrLen(moveToAlbumName);
					
		err = PalmPhotoSetImageExtraInfo(gLibRefNum, photoToMoveH, &extraInfo);
	}
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    	PrvExploreAlbum
 *
 * DESCRIPTION: 	Displays each photo from a given album selected
 *
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvExploreAlbum()
{
	FormType *frmP = FrmGetFormPtr(DetailsForm);
	
	gPhotoIndex = 0;

	PalmPhotoFreeSelections(gLibRefNum, &gDetailsSelectionParam);
	DoSelection(GetAlbumID());
				
	gNumImagesInAlbum = gDetailsSelectionParam.selectedImages.imageCount;

	if(gNumImagesInAlbum)
	{
		ToggleLabelsDisplay(frmP, true);
		DisplayImageDetails(gDetailsSelectionParam.selectedImages.imageH[gPhotoIndex]);	
	}
	else
		ToggleLabelsDisplay(frmP, false);
}	

#if 0
#pragma mark -
#endif


/***********************************************************************
 *
 * FUNCTION:    		PrvGetFileSizeString
 *
 * DESCRIPTION: 		Given the absolute value of the size of the file,
 *						function finds the size of the file as a string, 
 *						by appending MB, KB or bytes, whichever is appropriate
 *
 * PARAMETERS:  		fileSizeString -> Pointer to the string which will be
 *						the file size string
 *						fileSize32 -> Size of the file as a UInt32
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvGetFileSizeString(Char *fileSizeString, UInt32 fileSize32)
{
	UInt16	fileSize16 = 0;
 	
	if((fileSize32 >> 10) > 0)
	{
		if((fileSize32 >> 20) > 0)
		{
			fileSize16 = (UInt16)(fileSize32 >> 20);
			StrPrintF(fileSizeString, "%d", fileSize16);
			StrCat(fileSizeString, "MB");
		}
		else
		{ 
			fileSize16 = (UInt16)(fileSize32 >> 10);
			StrPrintF(fileSizeString, "%d", fileSize16);
			StrCat(fileSizeString, "KB");
		}
	}
	else
	{
		fileSize16 = (UInt16)fileSize32;		
		StrPrintF(fileSizeString, "%d", fileSize16);
		StrCat(fileSizeString, " bytes");
	}	
}

/***********************************************************************
 *
 * FUNCTION:    	PrvGetResolutionString
 *
 * DESCRIPTION: 	Returns the resolution of the picture as a string	
 *
 * PARAMETERS:  	resolutionString -> String which contains the resolution
 *					width -> Width of the photo
 *					height -> Height of the photo
 *					bitsPerPixel -> Bits per pixel of the photo (duh!)	
 *
 * RETURNED:    	Nothing	
 *	
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvGetResolutionString(Char *resolutionString, UInt32 width, UInt32 height, UInt32 bitsPerPixel)
{
	UInt16 width16 = (UInt16)width;
	UInt16 height16 = (UInt16)height;
	UInt16 bitsPerPixel16 = (UInt16)bitsPerPixel;
	
	StrPrintF(resolutionString, "%dx%dx%d", width16, height16, bitsPerPixel16);
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	PrvSetImageDetails
 *
 * DESCRIPTION: 	Sets details of the selected photo, such as the name,
 *					as well as any notes associated with the photo	
 *
 * PARAMETERS:  	selectedPhotoH -> Handle to the photo whose details have to 
 *					be displayed	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvSetImageDetails(PalmPhotoHandle selectedPhotoH)
{
	FieldType *fldFileNameP = NULL;
	FieldType *fldNotesP = NULL;
	FormType *frmP = FrmGetFormPtr(DetailsForm);
	PalmPhotoFileLocation *locationP = NULL;
	Char *notes = NULL;
	Err err = errNone;
				
	fldFileNameP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsNameField));
	fldNotesP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsNotesField));
				
	if(FldDirty(fldFileNameP))
	{
		if(FldGetTextPtr(fldFileNameP))
		{
			locationP = (PalmPhotoFileLocation *)GetSelectedPhotoExtraInformation(gLibRefNum, palmPhotoExtraInfoLocation, selectedPhotoH);
			
			if(locationP)
			{
				if(locationP->fileLocationType == palmPhotoStreamLocation)
					StrCopy(locationP->file.StreamFile.name, FldGetTextPtr(fldFileNameP));
				else if(locationP->fileLocationType == palmPhotoVFSLocation)
					StrCopy(locationP->file.VFSFile.name, FldGetTextPtr(fldFileNameP));
			}
			
			SetSelectedPhotoExtraInformation(gLibRefNum, palmPhotoExtraInfoLocation, 
												selectedPhotoH, locationP);
			MemPtrFree(locationP);
		}
	
	}					

	if(FldDirty(fldNotesP))
	{
		if(FldGetTextPtr(fldNotesP))
		{
			PalmPhotoExtraInfoParam	notesParam;
			
			notes = (Char *)MemPtrNew(StrLen(FldGetTextPtr(fldNotesP)) + 1);
			
			StrCopy(notes, FldGetTextPtr(fldNotesP));

			notesParam.infoType = palmPhotoExtraInfoNotes;
			notesParam.infoP = notes;
			notesParam.infoSize = StrLen(notes) + 1;
			
			err = PalmPhotoSetImageExtraInfo(gLibRefNum, selectedPhotoH, &notesParam);
			MemPtrFree(notes);
		}

	}					

	FldReleaseFocus(fldFileNameP);
	FldReleaseFocus(fldNotesP);
		
	DisplayImageDetails(selectedPhotoH);

}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	PrvToggleFieldsDisplay
 *
 * DESCRIPTION: 	Used to hide/show the various fields on the screen	
 *
 * PARAMETERS:  	fieldID -> ID of the field which needs to be modified
 *					text -> Text associated with the field
 *					editMode -> Flag which decides whether the attributes
 *								have to be changed or not
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvToggleFieldsDisplay(UInt16 fieldID, Char *text, Boolean editMode)
{
	FieldAttrType	fldAttr;
	FieldType *fldP = NULL;
	FormType *frmP = FrmGetFormPtr(DetailsForm);
	
	fldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, fieldID));

	FldGetAttributes(fldP, &fldAttr);
	if(editMode)
	{
		fldAttr.underlined = 3;	// Set underlined
		fldAttr.editable = 1; 	// Set editable
	}				
	else
	{
		fldAttr.underlined = 0;
		fldAttr.editable = 0;
	}
		
	FldSetAttributes(fldP, &fldAttr);
					
	SetFieldText(fldP, text, true);			

}		

/***********************************************************************
 *
 * FUNCTION:    	PrvToggleControlsDisplay
 *
 * DESCRIPTION: 	Used to hide/show the various labels on the screen	
 *
 * PARAMETERS:  	editFlag -> Flag which decides whether the elements 
 *						have to be drawn or hidden	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvToggleControlsDisplay(Boolean editFlag)
{
	FormType *frmP = FrmGetFormPtr(DetailsForm);
	
	if(editFlag)
	{
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsNextButton));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsBackButton));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsDoneButton));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsEditButton));
						
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsSaveButton));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsCancelButton));
	}
	else
	{
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsSaveButton));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsCancelButton));

		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsNextButton));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsBackButton));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsDoneButton));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsEditButton));
	}
}

/***********************************************************************
 *
 * FUNCTION:    	ToggleLabelsDisplay
 *
 * DESCRIPTION: 	Used to hide/show the various labels on the screen	
 *
 * PARAMETERS:  	showFlag -> Flag which decides whether the elements 
 *						have to be drawn or hidden	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void ToggleLabelsDisplay(FormPtr frmP, Boolean showFlag)
{
	if(showFlag)
	{
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsNameTagLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsSizeTagLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsResTagLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsNotesTagLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsDateTagLabel));
		
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsNameField));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsSizeField));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsResolutionField));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsNotesField));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsDateField));

	}
	else
	{
		RectangleType rect = { {THUMBNAIL_X, THUMBNAIL_Y}, {THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT}};
		
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsNameTagLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsSizeTagLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsResTagLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsNotesTagLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsDateTagLabel));
	
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsNameField));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsSizeField));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsResolutionField));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsNotesField));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsDateField));

		// Erase the bitmap which is currently drawn on the screen
		WinEraseRectangle(&rect,0);
	
	}

	FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsSaveButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, DetailsCancelButton));
	
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsDoneButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, DetailsEditButton));
				
	
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	PrvHandleFiveWay
 *
 * DESCRIPTION: 	Handles all five-way events generated	
 *
 * PARAMETERS:  	EventType *eventP -> Pointer to the event generated
 *
 * RETURNED:    	Boolean handled -> Indicates whether the event has been
 *										handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvHandleFiveWay(EventType *eventP)
{
	Boolean		handled		= false;
	
	// Get the KeyEvent fields
	WChar 		chr			= eventP->data.keyDown.chr;
	UInt16 		keyCode		= eventP->data.keyDown.keyCode;
	Int16		selection 	= 0;
	
	switch(chr)
	{
		case vchrPageUp:
			
			selection = LstGetSelection(gLstP);
			if(selection > 0)
			{
				LstSetSelection(gLstP, --selection);
				UpdateAlbumListUI(DetailsAlbumListPopTrigger);

				if(!gEditFlag)
					PrvExploreAlbum();
				UpdateArrows();
			}	
			handled = true;
			break;
			
		case vchrPageDown:

			selection = LstGetSelection(gLstP);
			if(selection < LstGetNumberOfItems(gLstP) - 1)
			{
				LstSetSelection(gLstP, ++selection);
				UpdateAlbumListUI(DetailsAlbumListPopTrigger);

				if(!gEditFlag)
					PrvExploreAlbum();
				UpdateArrows();		
			}			
			handled = true;
			break;
			
		case vchrNavChange:
			if(keyCode & navBitRight)
			{
				if(gNumImagesInAlbum)
					if(gPhotoIndex < gNumImagesInAlbum - 1)
						DisplayImageDetails(gDetailsSelectionParam.selectedImages.imageH[++gPhotoIndex]);	

				UpdateArrows();
			}
			else if(keyCode & navBitLeft)
			{
				if(gNumImagesInAlbum)
					if(gPhotoIndex > 0)
						DisplayImageDetails(gDetailsSelectionParam.selectedImages.imageH[--gPhotoIndex]);	

				UpdateArrows();
			}
			break;			
		
		default:
			break;
	}
	
	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    	UpdateArrows
 *
 * DESCRIPTION: 	Changes the state of the arrow buttons which are used
 *					to browse through a given album. When the end of an album
 *					is reached, the right-button becomes gray	
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void UpdateArrows()
{
	FormType 	*frmP = FrmGetFormPtr(DetailsForm);
	
	ControlType *ctlNextP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsNextButton));
	ControlType *ctlBackP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsBackButton));
	Char imageIndex[6] = "";
	
	
	if(!gNumImagesInAlbum)
	{
		CtlSetLabel(ctlNextP, "\6");
		CtlSetLabel(ctlBackP, "\5");
		StrCopy(imageIndex, " -/- ");
	}
	else
	{
		if(gPhotoIndex == 0)
			CtlSetLabel(ctlBackP, "\5");
		else
			CtlSetLabel(ctlBackP, "\2");
		
		if(gPhotoIndex == gNumImagesInAlbum - 1)
			CtlSetLabel(ctlNextP, "\6");
		else
			CtlSetLabel(ctlNextP, "\3");
	
		StrPrintF(imageIndex, "%d/%d", gPhotoIndex + 1, (UInt16)gNumImagesInAlbum);
	}
	FrmCopyLabel(frmP, DetailsImageIndexLabel, imageIndex);
}			

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	DoSelection
 *
 * DESCRIPTION: 	Does the selection process for a given album ID
 *					Invokes PalmPhotoSelectImages
 *
 *
 * PARAMETERS:  	albID -> ID of the album which we have to select	
 *
 * RETURNED:    	Err -> Err returned by PalmPhotoSelectImages	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Err	DoSelection(UInt16 albID)
{
	Err err = errNone;
	
	gDetailsSelectionParam.albumID = albID;
	gDetailsSelectionParam.offset = 0;
	gDetailsSelectionParam.selectionCount = PALM_PHOTO_SELECT_ALL;
	gDetailsSelectionParam.filterCallback = NULL;
	gDetailsSelectionParam.userDataP = NULL;
	gDetailsSelectionParam.selectedImages.imageCount = 0;
	gDetailsSelectionParam.selectedImages.imageH = NULL;
	
	err = PalmPhotoSelectImages(gLibRefNum, &gDetailsSelectionParam);
	
//Done:
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    		GetAlbumID
 *
 * DESCRIPTION: 		Returns the albumID given the index of the album
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		The album ID required
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

UInt16 GetAlbumID()
{
	UInt16 id = 0;
    
	if (verPresent >=  kPalmPhotoLibVersion1 && verPresent < kPalmPhotoLibVersion2)
	{
		id = PalmPhotoGetAlbumID(gLibRefNum, LstGetSelection(gLstP));
	}
	else if (verPresent >=  kPalmPhotoLibVersion2)
	{
		id = PalmPhotoGetAlbumIDV2(gLibRefNum, 0, LstGetSelection(gLstP));
	}
	
	return id;
}

#if 0
#pragma mark -
#endif



/***********************************************************************
 *
 * FUNCTION:    	DisplayImageDetails
 *
 * DESCRIPTION: 	Displays details of the selected photo, such as the name,
 *					date, size, resolution as well as any notes associated
 *					with the photo	
 *
 * PARAMETERS:  	selectedPhotoH -> Handle to the photo whose details have to 
 *					be displayed	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/


void DisplayImageDetails(PalmPhotoHandle	selectedPhotoH)
{
	// Photo-API variables
	PalmPhotoDisplayParam	displayParam ;
	PalmPhotoImageInfo	selectedPhotoInfo;
	PalmPhotoFileLocation	*location = NULL;
	
	Err	err = errNone;
	FieldType *fldName = NULL, *fldSize = NULL, *fldResolution = NULL, 
			  *fldNotes = NULL, *fldDate = NULL;
 
 	
 	//Strings
 	Char fileSizeString[8] = "";
 	Char resolutionString[12] = "";
 	Char *notes = NULL;
 	Char dateString[60] = "";
 	FormType *frmP = FrmGetFormPtr(DetailsForm);
 	
	// Graphics variables
	Coord	width = THUMBNAIL_WIDTH;
	Coord	height = THUMBNAIL_HEIGHT;
	UInt32	density = 0;
 	WinHandle tempH = NULL;
 	RectangleType	jpegAreaRectangle = {{0,0}, {0,0}};
	
	//Date-type date
	DateType *date = NULL;
		
	WinScreenGetAttribute(winScreenDensity, &density);
	
	if(density == kDensityDouble)
	{
		tempH = WinCreateOffscreenWindow(width, height, nativeFormat, &err);

		width *= 2;
		height *= 2;
	}
	else
		tempH = WinCreateOffscreenWindow(width, height, genericFormat, &err);
	
	if(!err)
	{
		MemSet(&displayParam, sizeof(PalmPhotoDisplayParam), 0);
			
		jpegAreaRectangle.topLeft.x = 0;
		jpegAreaRectangle.topLeft.y = 0;
			
		jpegAreaRectangle.extent.x = width;
		jpegAreaRectangle.extent.y = height;
				
		displayParam.winH = tempH; 
		displayParam.rect = jpegAreaRectangle;
		displayParam.displayCallback = NULL;
		displayParam.userDataP = NULL;

		err = PalmPhotoDisplayImage(gLibRefNum, selectedPhotoH, &displayParam);
							
		if(!err)
			DisplayJpegAsBitmap(displayParam.winH, THUMBNAIL_X, THUMBNAIL_Y, density);
	}

	WinDeleteWindow(tempH, false);
	// DISPLAYING INFORMATION ABOUT THE IMAGE ON THE SCREEN
	err = PalmPhotoGetImageInfo(gLibRefNum, selectedPhotoH, &selectedPhotoInfo);
	
	
	PrvGetFileSizeString(fileSizeString, selectedPhotoInfo.filesize);
	fldSize = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsSizeField));
	if(fldSize)
		SetFieldText(fldSize, fileSizeString , true);
	
	PrvGetResolutionString(resolutionString, selectedPhotoInfo.width, 
						selectedPhotoInfo.height, selectedPhotoInfo.bitsPerPixel);
	fldResolution = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsResolutionField));
	if(fldResolution)
		SetFieldText(fldResolution, resolutionString, true);

	
	// DISPLAYING EXTRA INFORMATION ABOUT THE IMAGE ON THE SCREEN	
	
	// Display file name on the screen
	location = (PalmPhotoFileLocation *)GetSelectedPhotoExtraInformation(gLibRefNum, palmPhotoExtraInfoLocation, selectedPhotoH);
	
	if(location)
	{
		fldName = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsNameField));
		if(fldName)
		{
			if(location->fileLocationType == palmPhotoStreamLocation)
			{
				StrCopy(gFileName, location->file.StreamFile.name);
				SetFieldText(fldName, location->file.StreamFile.name, true);
			}
			else if(location->fileLocationType == palmPhotoVFSLocation)
			{
				StrCopy(gFileName, location->file.VFSFile.name);
				SetFieldText(fldName, location->file.VFSFile.name, true);
			}
		}
		MemPtrFree(location);
	}

	// Display date of photo on the screen
	date = (DateType *)GetSelectedPhotoExtraInformation(gLibRefNum,palmPhotoExtraInfoDate, selectedPhotoH);
	if(date)
	{
		DateToAscii (date->month, date->day, date->year + firstYear, 
					(DateFormatType)PrefGetPreference(prefDateFormat), dateString);
		fldDate = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsDateField));
		if(fldDate)
			SetFieldText(fldDate, dateString, true);
		MemPtrFree(date);
	}

	// Display photo notes on the screen
	notes = (Char *)GetSelectedPhotoExtraInformation(gLibRefNum, palmPhotoExtraInfoNotes, selectedPhotoH);
	if(notes)
	{
		fldNotes = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsNotesField));
		if(fldNotes)
		{
			StrCopy(gNotes, notes);
			SetFieldText(fldNotes, notes, true);
		}
		MemPtrFree(notes);
	}		
	else
	{
		Char *noNotes = "-";
		
		fldNotes = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, DetailsNotesField));
		if(fldNotes)
		{
			StrCopy(gNotes, noNotes);
			SetFieldText(fldNotes, noNotes, true);
		}
	}

	FldFreeMemory(fldName);
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	DetailsFormHandleEvent
 *
 * DESCRIPTION: 	Event-handler for the DetailsForm - handles the Done
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

// Event handler for OrganizeForm
Boolean DetailsFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	
	switch(eventP->eType)
	{
		case popSelectEvent:
			
			if(!gEditFlag)
				PrvExploreAlbum();
			UpdateArrows();
			break;
	
		case ctlSelectEvent:
			
			switch(eventP->data.ctlSelect.controlID)
			{
				case DetailsNextButton:
				
					if(gNumImagesInAlbum)
						if(gPhotoIndex < gNumImagesInAlbum - 1)
							DisplayImageDetails(gDetailsSelectionParam.selectedImages.imageH[++gPhotoIndex]);	

					UpdateArrows();
					handled = true;
					break;		
			
				case DetailsBackButton:
				
					if(gNumImagesInAlbum)
						if(gPhotoIndex > 0)
							DisplayImageDetails(gDetailsSelectionParam.selectedImages.imageH[--gPhotoIndex]);	

					UpdateArrows();
					handled = true;
					break;
					
				case DetailsEditButton:

					if(gDetailsSelectionParam.selectedImages.imageH)
					{
						gEditFlag = true;
						
						PrvToggleControlsDisplay(gEditFlag);
						PrvToggleFieldsDisplay(DetailsNameField, gFileName, gEditFlag);
						PrvToggleFieldsDisplay(DetailsNotesField, gNotes, gEditFlag);
					}
					
					handled = true;
					break;
						
				case DetailsSaveButton:
					
					gEditFlag = false;

					PrvSetImageDetails(gDetailsSelectionParam.selectedImages.imageH[gPhotoIndex]);
					PrvMovePhoto(gDetailsSelectionParam.selectedImages.imageH[gPhotoIndex]);
					
					PrvToggleControlsDisplay(gEditFlag);
					PrvToggleFieldsDisplay(DetailsNameField, gFileName, gEditFlag);
					PrvToggleFieldsDisplay(DetailsNotesField, gNotes, gEditFlag);
					
					PrvExploreAlbum();
					
					UpdateArrows();
					handled = true;
					break;
		
				case DetailsCancelButton:
					
					gEditFlag = false;

					PrvToggleControlsDisplay(gEditFlag);
					PrvToggleFieldsDisplay(DetailsNameField, gFileName, gEditFlag);
					PrvToggleFieldsDisplay(DetailsNotesField, gNotes, gEditFlag);

					handled = true;
					break;
			}
			break;
	
		case keyDownEvent:
			handled = PrvHandleFiveWay(eventP);
			break;
			
		default:
			break;
			
	}

//Done:	
	return handled;

}


