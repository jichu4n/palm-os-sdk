/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	CameraSettings.h
 *
 * Description:
 *	Useful functions
 *
 ***********************************************************************/

#ifndef CAMERA_SETTINGS_H_
#define CAMERA_SETTINGS_H_

#include <palmOneCamera.h>

/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Camera advanced settings
Err SetSaturation(UInt16 libRefNum, Int32 saturation);
Err GetSaturation(UInt16 libRefNum, CamLibSettingType *saturationP);
Err SetContrast(UInt16 libRefNum, Int32 contrast);
Err GetContrast(UInt16 libRefNum, CamLibSettingType *contrastP);
Err SetExposure(UInt16 libRefNum, Int32 exposure, Boolean autoExposure);
Err GetExposure(UInt16 libRefNum, CamLibSettingType *exposureP);
Err SetSharpness(UInt16 libRefNum, Int32 sharpness);
Err GetSharpness(UInt16 libRefNum, CamLibSettingType *sharpnessP);

// Zoom settings
Err SetZoom(UInt16 libRefNum, Int32 zoom);
Err GetZoom(UInt16 libRefNum, CamLibSettingType *zoomP);

// Lighting settings
Err SetLight(UInt16 libRefNum, UInt32 light);
Err GetLight(UInt16 libRefNum, UInt32 *lightP);

// Effect settings
Err SetEffect(UInt16 libRefNum, UInt32 effect);
Err GetEffect(UInt16 libRefNum, UInt32 *effectP);

// White balance settings
Err SetWhiteBalance(UInt16 libRefNum, UInt32 whiteBalance);
Err GetWhiteBalance(UInt16 libRefNum, UInt32 *whiteBalanceP);

// Capture size settings
Err SetCaptureSize(UInt16 libRefNum, UInt32 captureSize);
Err GetCaptureSize(UInt16 libRefNum, UInt32 *captureSizeP);
Err SetCaptureFormat(UInt16 libRefNum, UInt32 captureFormat);
Err GetCaptureFormat(UInt16 libRefNum, UInt32 *captureFormatP);

// Preview size settings
Err SetPreviewSize(UInt16 libRefNum, UInt32 previewSize);
Err GetPreviewSize(UInt16 libRefNum, UInt32 *previewSizeP);
Err SetPreviewRect(UInt16 libRefNum, RectangleType *rectP);
Err GetPreviewRect(UInt16 libRefNum, RectangleType *rectP);

// Stream settings
Err SetStreamSize(UInt16 libRefNum, UInt32 streamSize);
Err GetStreamSize(UInt16 libRefNum, UInt32 *streamSizeP);
Err SetStreamFormat(UInt16 libRefNum, UInt32 streamFormat);
Err GetStreamFormat(UInt16 libRefNum, UInt32 *streamFormatP);

// Stream controls
Err StartStream(UInt16 libRefNum, CamLibStreamType *streamTypeP);
Err StopStream(UInt16 libRefNum, CamLibStreamType *streamTypeP);

// Camera/Preview ON & OFF
Err TurnCameraOn(UInt16 libRefNum);
Err TurnCameraOff(UInt16 libRefNum);
Err TurnPreviewOn(UInt16 libRefNum, Boolean oldLibrary);
Err TurnPreviewOff(UInt16 libRefNum, Boolean oldLibrary);

// Camera library open & close
Err CameraLibraryOpen(UInt16 libRefNum, Boolean oldLibrary);
Err CameraLibraryClose(UInt16 libRefNum, Boolean oldLibrary);

// Camera library sleep & wake
Err CameraLibrarySleep(UInt16 libRefNum, Boolean oldLibrary);
Err CameraLibraryWake(UInt16 libRefNum, Boolean oldLibrary);

#ifdef __cplusplus 
}
#endif
 
#endif // CAMERA_SETTINGS_H_