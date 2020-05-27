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
 * @file SaveUtils.c
 * This file contains code for saving a photo as a JPEG image on the device
 */

/** @name
 *
**/
/*@{*/

/***********************************************************************
 *
 * File:
 *	SaveUtils.c
 *
 * Description:
 *	Contains code for saving a photo as a JPEG image on the handheld
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/

#include <PalmOS.h>
#include "SaveUtils.h"
//#include "PalmOnePhotoCommon.h"
#include "MainForm.h"
#include "FotoFinishRsc.h"


/***********************************************************************
 *
 * FUNCTION:    	PrvPalmPhotoWriteCallback
 *
 * DESCRIPTION: 	Callback is used by the Photo API to write to the 
 *					JPEG picture and save it a location of our choice. In this situation,
 *					data from the JPEG picture is saved to backBufferWinH
 *
 * PARAMETERS:  	bufferP -> Pointer to the data which will be converted to the Jpeg format 
 *								(Source pointer)
 *					sizeP -> Pointer to the size of the data
 *					userDataP ->Pointer to the data which will be written to bufferP (in this case,
 *					this will be backBufferWinH) (which is the destination pointer)
 *
 * RETURNED:    	Err -> If read operation fails	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/


Err PalmPhotoWriteCallBack(void *bufferP, UInt32 *sizeP, void *userDataP)
{
	//bufferP -> Pointer to the chunk of memory which is passed to the user
	//sizeP -> Pointer to the size of the buffer
	// userDataP -> Pointer which can be cast in any way by the user
	
	Err err = errNone;
	UInt32 size = *sizeP / 2;
	UInt16 *myBuffP = (UInt16 *)bufferP;
	ImageData *dataP = (ImageData *)userDataP;
	
	while(size--)
	{
		*myBuffP = Swap16(*dataP->imgH);
		dataP->imgH++;
		dataP->offset++;
		myBuffP++;
	}	
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    	SavePhoto
 *
 * DESCRIPTION: 	Saves a photo to a given album	
 *
 * PARAMETERS:  	libRefNum -> Reference number for the library
 *					backBufferWinH -> Back-buffer which will be saved to the jpeg
 *										picture
 *					fileName -> File name for the picture
 *					albID -> ID of the album to which the picture will be saved
 *					newPhotoH -> Handle to the jpeg to which the picture will be saved
 *					photoHeight -> Height of the picture
 *					photoWidth -> Width of the picture	
 *
 * RETURNED:    	Err -> Err code returned by PalmPhotoWriteImage	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Err SavePhoto(UInt16 libRefNum, WinHandle backBufferWinH, Char *fileName, UInt16 albID, PalmPhotoHandle newPhotoH, UInt32 photoWidth, UInt32 photoHeight)
{
	PalmPhotoCreateParam 	createParam;
	PalmPhotoReadWriteParam writeParam;
	ImageData 				imageD;
	UInt32					density = 0;
	Err 					err = errNone;
	PalmPhotoHandle			*addedPhotoHandleP = NULL;
	FormType				*frmP = NULL;
	UInt16 count = 0;
	
	// Get the bitmap associated with the window
	BitmapType *bmpP = WinGetBitmap(backBufferWinH);
	// Get the bits associated with the bitmap
	imageD.imgH = (UInt16 *)BmpGetBits(bmpP);
	imageD.offset = 0;

	WinScreenGetAttribute(winScreenDensity, &density);

	if(!newPhotoH)
	{
		createParam.fileLocation.fileLocationType = palmPhotoStreamLocation;
		StrCopy(createParam.fileLocation.file.StreamFile.name, fileName);
		createParam.fileLocation.file.StreamFile.type = 'Foto';
		createParam.fileLocation.file.StreamFile.creator = 'Foto';
		
		if(density == kDensityDouble)
		{
			createParam.imageInfo.width = photoWidth << 1;
			createParam.imageInfo.height = photoHeight << 1;
		}
		else
		{
			createParam.imageInfo.width = photoWidth;
			createParam.imageInfo.height = photoHeight;
		}
		
		createParam.imageInfo.bitsPerPixel = 16;
		createParam.imageInfo.fileFormat = palmPhotoJPEGFileFormat;

		// Create a new image and get the photo handle
		newPhotoH = PalmPhotoCreateImage(libRefNum, &createParam, &err);
	}	
	
	if(err)
	{
		if(err == palmPhotoLibErrImageExist)
			FrmAlert(ImageExistErrorAlert);
		goto Done;
	}
	
	
	MemSet(&writeParam, sizeof(PalmPhotoReadWriteParam), 0);
	
	writeParam.imageFormat = palmPhotoRGB565FileFormat;
	writeParam.rwCallback = PalmPhotoWriteCallBack;
	writeParam.userDataP = &imageD;
	
	frmP = FrmInitForm(PleaseWaitForm);
	FrmDrawForm(frmP);
	
	// This is where the encoding to JPEG takes place. The callback
	// is used to transfer chunks of data from the bitmap and is
	// encoded into the JPEG picture
	err = PalmPhotoWriteImage(libRefNum, newPhotoH, &writeParam);

	// Once the photo is written, it is added to the album database
	if(!err) {
	    // This call is to workaround a known issue where the image is being added to the album twice
		count = PalmPhotoImageCount  (libRefNum, albID);   
		err = PalmPhotoAddImage(libRefNum,  albID, newPhotoH, addedPhotoHandleP, palmPhotoLowQuality);
	    
	}  
	// If the name is already exists in the database
	if(err)
	{
		if(err == palmPhotoLibErrImageExist)
			FrmAlert(ImageExistErrorAlert);
		
		err = PalmPhotoDeleteImage(libRefNum, newPhotoH);
		goto Done;
	}

	// Image is closed
	PalmPhotoCloseImage(libRefNum, newPhotoH);
	//DeInitOpQueue();
	
Done:
    FrmEraseForm(frmP);
	FrmDeleteForm(frmP);
	FileDelete(0, createParam.fileLocation.file.StreamFile.name);
	return err;	
}

/*@}*/
