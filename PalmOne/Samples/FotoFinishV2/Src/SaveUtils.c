/***********************************************************************
 *
 * Copyright (c) 2003-2004 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	SaveUtils.c
 *
 * Description:
 *	Contains code for saving a photo as a JPEG image on the handheld
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision          (18-aug-2003)
 *          1.1     - library update            (23-sep-2003)
 *          2.0     - require Photo Lib v2.0    (20-jan-2004)
 *
 ***********************************************************************/
#include <PalmOS.h> 
#include "SaveUtils.h"
#include "PalmPhoto.h"
#include "MainForm.h"
#include "FotoFinishRsc.h"
#include "FotoFinishV2.h"

//#include "Global.h"

extern UInt32 verPresent;

/***********************************************************************
 *
 * FUNCTION:    	PrvPalmPhotoReadCallback
 *
 * DESCRIPTION: 	Callback is used by the Photo API to read from a 
 *					picture and save it a RGB buffer of our choice.
 *
 * CAUTION:			This callback function is not industrial-strength.
 *                  If you use it in your program, you deserve.what happens.
 *
 * PARAMETERS:  	bufferP   -> Pointer to the buffer in the Photo library
 *                               that contains image data decoded into
 *                               RGB format (Source pointer)
 *					sizeP     -> Pointer to the size of the data
 *					userDataP -> Pointer to the data including the buffer
 *                               into which the RGB data is copied.
 *					             (the destination pointer)
 *
 * RETURNED:    	Err -> If read operation fails
 *                         Return palmPhotoLibErrAbortReadWrite to terminate the
 *                         read operation.
 *
 * REVISION HISTORY:
 *   18 aug 2003    v1.0    Original version
 *   23 sep 2003    v1.1    byte-swap to work around a bug in the library
 *   20 jan 2004    v2.0    library v2.0 no longer requires byte-swapping
 *
 *
 ***********************************************************************/

Err PalmPhotoReadCallBack(void *bufferP, UInt32 *sizeP, void *userDataP)
{
	// bufferP -> Pointer to the chunk of memory which is passed to the user
	// sizeP -> Pointer to the size of the buffer
	// userDataP -> Pointer which can be cast in any way by the user

	Err err = errNone;
	UInt32 size = *sizeP;
	UInt8 *myBuffP = (UInt8 *)bufferP;
	ImageData *dataP = (ImageData *)userDataP;

	while(size--)
	{
		*dataP->imgH = *myBuffP;
		dataP->imgH++;
		dataP->offset++;
		myBuffP++;
	}

	return err;
}


/***********************************************************************
 *
 * FUNCTION:    	PrvPalmPhotoWriteCallback
 *
 * DESCRIPTION: 	Callback is used by the Photo API to write to the 
 *					JPEG picture and save it a location of our choice. In this situation,
 *					data from the JPEG picture is saved to backBufferWinH
 *
 * NOTE:            The back-buffer is little-endian, so the pixel data must
 *                  be byte-swapped when copying into 68K space.
 *
 *
 * CAUTION:			This callback function is not industrial-strength.
 *                  If you use it in your program, you deserve.what happens.
 *
 * PARAMETERS:  	bufferP   -> Pointer to the Photo library buffer from which
 *                               the data will be converted to the Jpeg format 
 *								 (Destination pointer)
 *					sizeP     -> Pointer to the size of the data written
 *                               Set *sizeP to 0 to halt the write operation.
 *					userDataP -> Pointer to the data which will be written to bufferP
 *                               (in this case, this will be backBufferWinH)
 *                               (which is the source pointer)
 *
 * RETURNED:    	Err -> If read operation fails	
 *
 * REVISION HISTORY:
 *   18 aug 2003    v1.0    Original version
 *   23 sep 2003    v1.1    byte-swap to work around a bug in the library
 *   20 jan 2004    v2.0    
 *
 *
 ***********************************************************************/


Err PalmPhotoWriteCallBack(void *bufferP, UInt32 *sizeP, void *userDataP)
{
	// bufferP -> Pointer to the chunk of memory which is passed to the user
	// sizeP -> Pointer to the size of the buffer
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
		if (verPresent >=  kPalmPhotoLibVersion1 && verPresent < kPalmPhotoLibVersion2)
		{
			newPhotoH = PalmPhotoCreateImage(libRefNum, &createParam, &err);
		} 
		else if (verPresent >=  kPalmPhotoLibVersion2)
		{
			newPhotoH = PalmPhotoCreateImageV2(libRefNum, &createParam);
		}
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
	if(!err)
	{
		if (verPresent >=  kPalmPhotoLibVersion1 && verPresent < kPalmPhotoLibVersion2)
		{
			// This call is to workaround a known issue where the image is being added to the album twice
		    count = PalmPhotoImageCount(libRefNum, albID);  
			err = PalmPhotoAddImage(libRefNum,  albID, newPhotoH, addedPhotoHandleP, palmPhotoLowQuality);
		} 
		else if (verPresent >=  kPalmPhotoLibVersion2)
		{
			// This call is to workaround a known issue where the image is being added to the album twice
		    count = PalmPhotoImageCount(libRefNum, albID);  
			err = PalmPhotoAddImageV2(libRefNum,  albID, newPhotoH, addedPhotoHandleP);
		}
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
	
	FrmEraseForm(frmP);
	FrmDeleteForm(frmP);

Done:
	FileDelete(0, createParam.fileLocation.file.StreamFile.name);
	return err;	
}
