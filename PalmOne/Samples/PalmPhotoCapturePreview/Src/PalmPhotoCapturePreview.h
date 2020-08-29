/*
 * PalmPhotoCapturePreview.h
 *
 * header file for PalmPhotoCapturePreview
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef PALMPHOTOCAPTUREPREVIEW_H_
#define PALMPHOTOCAPTUREPREVIEW_H_

/*********************************************************************
 * Internal Structures
 *********************************************************************/

void CapturePicture();
Boolean InitPhotoLib();
Err PhotosLibOpen();
void PhotosLibClose();



/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'STRT'
#define appName					"PalmPhotoCapturePreview"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

#endif /* PALMPHOTOCAPTUREPREVIEW_H_ */
