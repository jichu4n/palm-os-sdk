/*
 * Camera Settings.c
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 */
#include "CameraSettings.h"
#include "PalmOneCamera.h"


/** Set the Saturation */
Err SetSaturation(UInt16 libRefNum, Int32 saturation)
{
	CamLibSettingType settingType;
	settingType.type = kCamLibSaturationManual;
	settingType.value = saturation;
	return CamLibControl(libRefNum, kCamLibCtrlSaturationSet, (void*)&settingType);
}
 
/** Get the Saturation */
Err GetSaturation(UInt16 libRefNum, CamLibSettingType *saturationP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlSaturationGet, (void*)&settingType);
	if(err) goto Done;
	
	saturationP->type = settingType.type;
	saturationP->value = settingType.value;
	
	err = CamLibControl(libRefNum, kCamLibCtrlSaturationQuery, (void*)&settingType);
	if(err) goto Done;
	
	saturationP->minValue = settingType.minValue;
	saturationP->maxValue = settingType.maxValue;

Done:
	return err;
}

/** Set the Contrast */
Err SetContrast(UInt16 libRefNum, Int32 contrast)
{
	CamLibSettingType settingType;
	settingType.type = kCamLibContrastManual;
	settingType.value = contrast;
	return CamLibControl(libRefNum, kCamLibCtrlContrastSet, (void*)&settingType);
}
 
/** Get the Contrast */
Err GetContrast(UInt16 libRefNum, CamLibSettingType *contrastP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlContrastGet, (void*)&settingType);
	if(err) goto Done;
	
	contrastP->type = settingType.type;
	contrastP->value = settingType.value;
	
	err = CamLibControl(libRefNum, kCamLibCtrlContrastQuery, (void*)&settingType);
	if(err) goto Done;
	
	contrastP->minValue = settingType.minValue;
	contrastP->maxValue = settingType.maxValue;

Done:
	return err;
}

/** Set the Exposure */
Err SetExposure(UInt16 libRefNum, Int32 exposure, Boolean autoExposure)
{
	CamLibSettingType settingType;
	settingType.type = (autoExposure)?kCamLibExposureAuto:kCamLibExposureManual;
	settingType.value = exposure;
	return CamLibControl(libRefNum, kCamLibCtrlExposureSet, (void*)&settingType);
}
 
/** Get the Exposure */
Err GetExposure(UInt16 libRefNum, CamLibSettingType *exposureP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlExposureGet, (void*)&settingType);
	if(err) goto Done;
	
	exposureP->type = settingType.type;
	exposureP->value = settingType.value;
	
	err = CamLibControl(libRefNum, kCamLibCtrlExposureQuery, (void*)&settingType);
	if(err) goto Done;
	
	exposureP->minValue = settingType.minValue;
	exposureP->maxValue = settingType.maxValue;

Done:
	return err;
}
 
/** Set the Sharpness */
Err SetSharpness(UInt16 libRefNum, Int32 sharpness)
{
	CamLibSettingType settingType;
	settingType.type = kCamLibSharpnessManual;
	settingType.value = sharpness;
	return CamLibControl(libRefNum, kCamLibCtrlSharpnessSet, (void*)&settingType);
}
 
/** Get the Sharpness */
Err GetSharpness(UInt16 libRefNum, CamLibSettingType *sharpnessP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlSharpnessGet, (void*)&settingType);
	if(err) goto Done;
	
	sharpnessP->type = settingType.type;
	sharpnessP->value = settingType.value;
	
	err = CamLibControl(libRefNum, kCamLibCtrlSharpnessQuery, (void*)&settingType);
	if(err) goto Done;
	
	sharpnessP->minValue = settingType.minValue;
	sharpnessP->maxValue = settingType.maxValue;

Done:
	return err;
}

/** Set the Zoom */
Err SetZoom(UInt16 libRefNum, Int32 zoom)
{
	CamLibSettingType settingType;
	settingType.type = kCamLibZoomManual;
	settingType.value = zoom;
	return CamLibControl(libRefNum, kCamLibCtrlZoomSet, (void*)&settingType);
}
 
/** Get the Zoom */
Err GetZoom(UInt16 libRefNum, CamLibSettingType *zoomP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlZoomGet, (void*)&settingType);
	if(err) goto Done;
	
	zoomP->type = settingType.type;
	zoomP->value = settingType.value;
	
	err = CamLibControl(libRefNum, kCamLibCtrlZoomQuery, (void*)&settingType);
	if(err) goto Done;
	
	zoomP->minValue = settingType.minValue;
	zoomP->maxValue = settingType.maxValue;

Done:
	return err;
}

/** Set the Light */
Err SetLight(UInt16 libRefNum, UInt32 light)
{
	CamLibSettingType settingType;
	settingType.type = light;
	return CamLibControl(libRefNum, kCamLibCtrlLightingSet, (void*)&settingType);
}

/** Get the Capture Size */
Err GetLight(UInt16 libRefNum, UInt32 *lightP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlLightingGet, (void*)&settingType);
	if(!err)
		*lightP = settingType.type;
	return err;
}

/** Set the Capture Size */
Err SetCaptureSize(UInt16 libRefNum, UInt32 captureSize)
{
	CamLibSettingType settingType;
	settingType.type = captureSize;
	return CamLibControl(libRefNum, kCamLibCtrlCaptureSizeSet, (void*)&settingType);
}

/** Get the Capture Size */
Err GetCaptureSize(UInt16 libRefNum, UInt32 *captureSizeP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlCaptureSizeGet, (void*)&settingType);
	if(!err)
		*captureSizeP = settingType.type;
	return err;
}

/** Set the Capture Format */
Err SetCaptureFormat(UInt16 libRefNum, UInt32 captureFormat)
{
	CamLibSettingType settingType;
	settingType.type = captureFormat;
	return CamLibControl(libRefNum, kCamLibCtrlCaptureFormatSet, (void*)&settingType);
}

/** Get the Capture Format */
Err GetCaptureFormat(UInt16 libRefNum, UInt32 *captureFormatP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlCaptureFormatGet, (void*)&settingType);
	if(!err)
		*captureFormatP = settingType.type;
	return err;
}


/** Set the effect */
Err SetEffect(UInt16 libRefNum, UInt32 effect)
{
	CamLibSettingType settingType;
	settingType.type = effect;
	return CamLibControl(libRefNum, kCamLibCtrlEffectsSet, (void*)&settingType);
}

/** Get the effect */
Err GetEffect(UInt16 libRefNum, UInt32 *effectP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlEffectsGet, (void*)&settingType);
	if(!err)
		*effectP = settingType.type;
	return err;
}


/** Set the white balance */
Err SetWhiteBalance(UInt16 libRefNum, UInt32 whiteBalance)
{
	CamLibSettingType settingType;
	settingType.type = whiteBalance;
	return CamLibControl(libRefNum, kCamLibCtrlWhiteBalanceSet, (void*)&settingType);
}

/** Get the white balance */
Err GetWhiteBalance(UInt16 libRefNum, UInt32 *whiteBalanceP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlWhiteBalanceGet, (void*)&settingType);
	if(!err)
		*whiteBalanceP = settingType.type;
	return err;
}

/** Set the preview size */
Err SetPreviewSize(UInt16 libRefNum, UInt32 previewSize)
{
	CamLibSettingType settingType;
	settingType.type = previewSize;
	return CamLibControl(libRefNum, kCamLibCtrlPreviewSizeSet, &settingType);
}

/** Get the preview size */
Err GetPreviewSize(UInt16 libRefNum, UInt32 *previewSizeP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlPreviewSizeGet, &settingType);
	if(!err)
		*previewSizeP = settingType.type;
	return err;
}

/** Set the preview rect. */
Err SetPreviewRect(UInt16 libRefNum, RectangleType *rectP)
{
	return CamLibControl(libRefNum, kCamLibCtrlPreviewRectSet, rectP);
}

/** Get the preview rect. */
Err GetPreviewRect(UInt16 libRefNum, RectangleType *rectP)
{
	return CamLibControl(libRefNum, kCamLibCtrlPreviewRectGet, rectP);
}

/** Set the stream size */
Err SetStreamSize(UInt16 libRefNum, UInt32 streamSize)
{
	CamLibSettingType settingType;
	settingType.type = streamSize;
	return CamLibControl(libRefNum, kCamLibCtrlStreamSizeSet, &settingType);
}

/** Get the stream size */
Err GetStreamSize(UInt16 libRefNum, UInt32 *streamSizeP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlStreamSizeGet, &settingType);
	if(!err)
		*streamSizeP = settingType.type;
	return err;
}

/** Set the stream format */
Err SetStreamFormat(UInt16 libRefNum, UInt32 streamFormat)
{
	CamLibSettingType settingType;
	settingType.type = streamFormat;
	return CamLibControl(libRefNum, kCamLibCtrlStreamFormatSet, &settingType);
}

/** Get the stream format */
Err GetStreamFormat(UInt16 libRefNum, UInt32 *streamFormatP)
{
	CamLibSettingType settingType;
	Err err = CamLibControl(libRefNum, kCamLibCtrlStreamFormatGet, &settingType);
	if(!err)
		*streamFormatP = settingType.type;
	return err;
}

/** Start the video stream. */
Err StartStream(UInt16 libRefNum, CamLibStreamType *streamTypeP)
{
	return CamLibControl(libRefNum, kCamLibCtrlStreamStart, streamTypeP);
}

/** Stop the video stream. */
Err StopStream(UInt16 libRefNum, CamLibStreamType *streamTypeP)
{
	return CamLibControl(libRefNum, kCamLibCtrlStreamStop, streamTypeP);
}

/** Turn the camera on. */
Err TurnCameraOn(UInt16 libRefNum)
{
	return CamLibControl(libRefNum, kCamLibCtrlPowerOn, NULL);
}

/** Turn the camera off. */
Err TurnCameraOff(UInt16 libRefNum)
{
	return CamLibControl(libRefNum, kCamLibCtrlPowerOff, NULL);
}

/** Turn the preview on. */
Err TurnPreviewOn(UInt16 libRefNum)
{
	return CamLibControl(libRefNum, kCamLibCtrlPreviewStart, NULL);
}


/** Turn the preview off. */
Err TurnPreviewOff(UInt16 libRefNum)
{
	return CamLibControl(libRefNum, kCamLibCtrlPreviewStop, NULL);
}
