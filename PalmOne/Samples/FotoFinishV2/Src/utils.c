/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	utils.c
 *
 * Description:
 *	Contains utility functions used throughout the project
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/
#include <PalmOS.h> 
#include "utils.h"
#include "PalmPhoto.h"

/***********************************************************************
 *
 * FUNCTION:    	DisplayJpegAsBitmap
 *
 * DESCRIPTION: 	Displays a given jpeg photo as a bitmap	(This is a hack
 *					and is not needed, once the byte-swapping bug in 
 *					PalmPhotoDisplayImage is fixed)
 *
 * PARAMETERS:  	tempH -> Handle to the window which contains the 
 *							 decoded Jpeg picture
 *					topLeftX -> x Coordinate of the location of display
 *					topLeftY -> y Coordinate of the location of display
 *					density -> density of the screen	
 *
 * RETURNED:    	Err value returned if a BitmapCreate doesn't work	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Err DisplayJpegAsBitmap(WinHandle tempH, Coord	topLeftX, Coord	topLeftY, UInt32 density)
{
	BitmapType *bmpP = NULL;
	BitmapTypeV3 *bmpV3P = NULL;
	Err err = errNone;

	if(density != kCoordinatesStandard)
	{
		bmpP = WinGetBitmap(tempH);

		if(bmpP)
			bmpV3P = BmpCreateBitmapV3(bmpP, kCoordinatesDouble, BmpGetBits(bmpP), NULL);
		else
		{
			err = 0x01;
			goto Done;
		}
		
		if(bmpV3P)
			WinDrawBitmap((BitmapType *)bmpV3P, topLeftX, topLeftY);
		else
		{
			BmpDelete((BitmapType *)bmpV3P);
			bmpV3P = NULL;
			err = 0x02;
			
			goto Done;
		}
		
		BmpDelete((BitmapType *)bmpV3P);
		bmpV3P = NULL;
	} 

	else
	{
		bmpP = WinGetBitmap(tempH);
		
		if(bmpP)
			WinDrawBitmap(bmpP, topLeftX, topLeftY);
	}	

Done:
	return err;

}

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

void SetFieldText(FieldType *fieldP, Char *textP, Boolean blDraw)
{	 
	MemHandle	fieldTextH=NULL;
	Char		*fieldTextP=NULL;
	Int16		textSize=0;
	
	// Get the handle to the text associated with the field
	fieldTextH = FldGetTextHandle(fieldP);

	if(!fieldTextH)
	{
		fieldTextH=MemHandleNew(StrLen(textP));
		if(!fieldTextH)
			return;
	}
	
	// Set the handle to null
	FldSetTextHandle (fieldP, NULL);

	// Get the size of the handle
	textSize=StrLen(textP) + 10;
	
	// Re-size the handle
	if(MemHandleResize(fieldTextH, textSize)==0) {
		// Get the pointer to the handle, by locking
		fieldTextP = (Char *)MemHandleLock(fieldTextH);

		// Copy the text into the pointer
		StrCopy(fieldTextP,textP);
		MemHandleUnlock(fieldTextH);
		FldSetTextHandle (fieldP, fieldTextH);
	}
	// If the option chosen by the user is to draw the field, 
	// draw it!
	if(blDraw)
		FldDrawField(fieldP);
		
}



/***********************************************************************
 *
 * FUNCTION:    	GetSelectedPhotoExtraInformation
 *
 * DESCRIPTION: 	Gets extra information of the selected photo 
 *					(such as date, 	album name, etc)
 *
 * PARAMETERS:  	gLibRefNum -> Reference number of the library
 *					infoType -> Type of information to retrieve
 *					selectedPhotoH -> The handle of the selected photo
 *
 * RETURNED:    	extraInfoP -> Pointer to the extra information	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void *GetSelectedPhotoExtraInformation(UInt16 libRefNum, PalmPhotoExtraInfoType infoType, PalmPhotoHandle selectedPhotoH)
{
	PalmPhotoExtraInfoParam selectedPhotoExtraInfo;
	UInt32 extraInfoSize = 0;
	Err err = errNone;
	void *extraInfoP = NULL;
	
	extraInfoSize = PalmPhotoGetImageExtraInfoSize(libRefNum, 
												   selectedPhotoH, 
												   infoType);
    if(infoType == palmPhotoExtraInfoNotes && extraInfoSize != 0)
        extraInfoSize++;

	
	extraInfoP = MemPtrNew(extraInfoSize);
	MemSet(extraInfoP, extraInfoSize, 0);
	
	selectedPhotoExtraInfo.infoType = infoType;
	selectedPhotoExtraInfo.infoP = extraInfoP;
	selectedPhotoExtraInfo.infoSize = extraInfoSize;	
	
	err = PalmPhotoGetImageExtraInfo(libRefNum, selectedPhotoH, &selectedPhotoExtraInfo);

	if(err)
	{
		if(err != palmPhotoLibErrParam)
		{
			MemPtrFree(extraInfoP);
			return NULL;						
		}
	}

// Returns a pointer to the selected Info	
//Done:
	return selectedPhotoExtraInfo.infoP;
}

/***********************************************************************
 *
 * FUNCTION:    	SetSelectedPhotoExtraInformation
 *
 * DESCRIPTION: 	Sets extra information of the selected photo 
 *					(such as date, 	album name, etc)
 *
 * PARAMETERS:  	gLibRefNum -> Reference number of the library
 *					infoType -> Type of information to retrieve
 *					selectedPhotoH -> The handle of the selected photo
 *					extraInfoP -> Pointer to the extra information	
 *
 * RETURNED:    	err -> Returns the error code generated by 
 *							PalmPhotoSetImageExtraInfo	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Err SetSelectedPhotoExtraInformation(UInt16 libRefNum, PalmPhotoExtraInfoType infoType, PalmPhotoHandle selectedPhotoH, void *extraInfoP)
{
	UInt32 extraInfoSize = 0;
	PalmPhotoExtraInfoParam selectedPhotoExtraInfo;
	Err err = errNone;
	
	extraInfoSize = PalmPhotoGetImageExtraInfoSize(libRefNum, 
												   selectedPhotoH, 
												   infoType);

	// Initializes the structure which is passed a parameter to PalmPhotoSetImageExtraInfo
	selectedPhotoExtraInfo.infoType = infoType;
	selectedPhotoExtraInfo.infoP = extraInfoP;
	selectedPhotoExtraInfo.infoSize = extraInfoSize;	
	
	// Sets the extra information
	err = PalmPhotoSetImageExtraInfo(libRefNum, selectedPhotoH, &selectedPhotoExtraInfo);

	return err;
}


