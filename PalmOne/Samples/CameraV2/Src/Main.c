/*
 * MiniCamera.c
 *
 * 
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <VFSMgr.h>
#include <MemGlue.h>
#include <HsExt.h>


#include "Common.h"
#include "MiniCamera.h"
#include <MiniCameraRsc.h>
#include <PalmOneCamera.h>
#include <PalmOneNavigator.h>
#include <PalmLcdOverlay.h>
#include <palmOneResources.h>
#include "CameraSettings.h"


#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)


#define Allocate(n)	MemGluePtrNew( n )
#define TRACK_TIME	(sysTicksPerSecond / 10)

#define ERROR_ALERT(e, x)	\
	if(e){				\
		TurnPreviewOff(gCameraLibRefNum);\
		FrmCustomAlert(ErrorAlert, (x), NULL, NULL);\
		TurnPreviewOn(gCameraLibRefNum);\
		}
		

// Panels
enum {
	MAIN,
	MAIN2,
	ADVANCED,
	SIZE,
	WHITE,
	LIGHT,
	EFFECTS,
	ZOOM,
	QUERY,
	STREAM
};

// Passed to the capture callback
typedef struct {
	WinHandle winH;
	UInt8 *bufferP;
	UInt32 width;	// In pixel
	UInt32 height;	// In pixel
} PhotoCaptureInfo;


#if 0
#pragma mark Globals
#endif

/** Panel Title. */
//static Char *gButton[] = { "ON", "OFF" };
static Char *gButton[] = { "ON", "Capture" };
static Char	*gLabel[] = {	":: Main Controls ::", ":: Advanced Settings ::", ":: Capture Size ::",
								":: White Balance ::", ":: Light Settings ::", ":: Effects Settings ::",
								":: Main Controls 2 ::", ":: Zoom ::", ":: Query ::", ":: Stream ::" };

/** Internal Globals. */
static UInt32	gPreviousScreenDepth = 0;
static UInt32	gScreenDepth = 16;
static UInt16 	gCameraLibRefNum = sysInvalidRefNum;
static UInt16	gLCDOverlayRefNum = sysInvalidRefNum;
static UInt32 	gPreviousControls = MAIN;
static UInt32 	gCurrentControls = MAIN;
static Boolean 	gPreviewOn = false;
static Boolean	gPanning = false;
static Boolean 	gStreamStart = false;
static Boolean  gCannotDisplay = false;
static Boolean  fromQuery = false;
Boolean gStreamOn = false ;
static Int32	gImageX = 0, gImageY = 0; 
static PhotoCaptureInfo	gCaptureInfo = { 0, 0, 0 };
static CamLibStreamType gStreamType;

/** Camera settings */
static CamLibSettingType gZoom = { 0, 0, 0, 0 };
static CamLibSettingType gContrast = { 0, 0, 0, 0 };
static CamLibSettingType gExposure = { 0, 0, 0, 0 };
static CamLibSettingType gSharpness = { 0, 0, 0, 0 };
static CamLibSettingType gSaturation = { 0, 0, 0, 0 };

/** Know formats */
static UInt32 gSize[] = { kCamLibImageSizeQQVGA, kCamLibImageSizeQVGA, kCamLibImageSizeVGA, kCamLibImageSizeSXGA };
static Char *gSizeLabel[] = { "QQVGA", "QVGA", "VGA", "SXGA" };
static UInt32 gFormat[] = { kCamLibCaptureDataFormatRGB565, kCamLibCaptureDataFormatYUV422, kCamLibCaptureDataFormatYCbCr422, kCamLibCaptureDataFormatYUV422Planar, kCamLibCaptureDataFormatYCbCr422Planar };
static Char *gFormatLabel[] = { "RGB565", "YUV422", "YCbCr422", "YUV422P", "YCbCr422P" };

static UInt32 gOverlayFormat[] = { kLcdOverlayLibDataFormatRGB565, kLcdOverlayLibDataFormatYCbCr422, kLcdOverlayLibDataFormatYCbCr422, kLcdOverlayLibDataFormatYCbCr422Planar, kLcdOverlayLibDataFormatYCbCr422Planar };

/** Light settings. */
static UInt32 gLightSetting = kCamLibLightingNormal;

/** Size settings. */
static UInt32 gCaptureSizeSetting = 1; // QVGA
static UInt32 gCaptureFormatSetting = 0; // RGB565;

/** Stream settings. */
static UInt32 gStreamSizeSetting = 1; // QVGA
static UInt32 gStreamFormatSetting = 0; // RGB565;

/** White settings. */
static UInt32 gWhiteSetting = kCamLibWhiteBalanceAuto;

/** Effect settigns. */
static UInt32 gEffectSetting = kCamLibEffectsNormal;

/** Capture and Preview size. */
static RectangleType gPreviewQVGARectangle = { { 0, 0 }, { 160, 120 } };
static UInt32 gCurrentPreviewSize = kCamLibImageSizeQVGA;

#if 0
#pragma mark -
#endif


Boolean IsDevice700P ();

/**
 * Get the Current Camera Settings
 */
/*
static Err PrvGetCurrentSettings()
{
	Err err = errNone;
	
	// Advanced settings
	err = GetContrast(gCameraLibRefNum, &gContrast);
	ERROR_ALERT(err, "Contrast Get err");
	err = GetExposure(gCameraLibRefNum, &gExposure);
	ERROR_ALERT(err, "gExposure Get err");
	err = GetSharpness(gCameraLibRefNum, &gSharpness);
	ERROR_ALERT(err, "Sharpness Get err");
	err = GetSaturation(gCameraLibRefNum, &gSaturation);
	ERROR_ALERT(err, "Saturation Get err");
	err = GetZoom(gCameraLibRefNum, &gZoom);
	ERROR_ALERT(err, "Zoom Get err");
	
	// Lighting
	err = GetLight(gCameraLibRefNum, &gLightSetting);
	ERROR_ALERT(err, "Lighting Get err");
	// White Balance
	err = GetWhiteBalance(gCameraLibRefNum, &gWhiteSetting);
	ERROR_ALERT(err, "WB Get err");
	// Effect
	err = GetEffect(gCameraLibRefNum, &gEffectSetting);
	ERROR_ALERT(err, "Effect Get err");
	
	return err;
}
*/
/**
 * Set the Advanced Controls UI
 */
static void PrvSetAdvancedUI(FormType *frmP)
{
	UInt16 value = 0;
	UInt16 pageSize = 1;
	UInt16 minValue = 0;
	UInt16 maxValue = 0;
	ControlType *ctlP = NULL;
	
	// Always query the latest values
	GetContrast(gCameraLibRefNum, &gContrast);
	GetExposure(gCameraLibRefNum, &gExposure);
	GetSharpness(gCameraLibRefNum, &gSharpness);
	GetSaturation(gCameraLibRefNum, &gSaturation);
	
	// Contrast
	value = gContrast.value;
	minValue = gContrast.minValue;
	maxValue = gContrast.maxValue;
	if(minValue != maxValue) {
		ctlP = FrmGetPtr(frmP, MainContrastFeedbackSliderControl);
		CtlSetSliderValues(ctlP, &minValue, &maxValue, &pageSize, &value);
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainContrastFeedbackSliderControl));
	}

	// Exposure
	value = gExposure.value;
	minValue = gExposure.minValue;
	maxValue = gExposure.maxValue;
	if(minValue != maxValue) {
		ctlP = FrmGetPtr(frmP, MainExposureFeedbackSliderControl);	
		CtlSetSliderValues(ctlP, &minValue, &maxValue, &pageSize, &value);
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainExposureFeedbackSliderControl));
	}
	
	// Sharpness
	value = gSharpness.value;
	minValue = gSharpness.minValue;
	maxValue = gSharpness.maxValue;
	if(minValue != maxValue) {
		ctlP = FrmGetPtr(frmP, MainSharpnessFeedbackSliderControl);
		CtlSetSliderValues(ctlP, &minValue, &maxValue, &pageSize, &value);
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSharpnessFeedbackSliderControl));
	}

	// Saturation
	value = gSaturation.value;
	minValue = gSaturation.minValue;
	maxValue = gSaturation.maxValue;
	if(minValue != maxValue) {
		ctlP = FrmGetPtr(frmP, MainSaturationFeedbackSliderControl);
		CtlSetSliderValues(ctlP, &minValue, &maxValue, &pageSize, &value);
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSaturationFeedbackSliderControl));
	}
}

/**
 * Set the Capture Size UI
 */
static void PrvSetCaptureUI(FormType *frmP)
{
	UInt32 currentSize = 0;
	UInt32 currentFormat = 0;
	
	// Always query the latest values
	GetCaptureSize(gCameraLibRefNum, &currentSize);
	GetCaptureFormat(gCameraLibRefNum, &currentFormat);
	
	gCaptureSizeSetting = 0;
	gCaptureFormatSetting = 0;
	
	switch(currentSize)
	{
		case kCamLibImageSizeSXGA:
			gCaptureSizeSetting++;
		case kCamLibImageSizeVGA:
			gCaptureSizeSetting++;
		case kCamLibImageSizeQVGA:
			gCaptureSizeSetting++;
		case kCamLibImageSizeQQVGA:
			break;
	}
	
	switch(currentFormat)
	{
		case kCamLibCaptureDataFormatYCbCr422Planar:
			gCaptureFormatSetting++;
		case kCamLibCaptureDataFormatYUV422Planar:
			gCaptureFormatSetting++;
		case kCamLibCaptureDataFormatYCbCr422:
			gCaptureFormatSetting++;
		case kCamLibCaptureDataFormatYUV422:
			gCaptureFormatSetting++;
		case kCamLibCaptureDataFormatRGB565:
			break;
	}
	
	CtlSetLabel(FrmGetPtr(frmP, MainCaptureSizePopTrigger), gSizeLabel[gCaptureSizeSetting]);
	CtlSetLabel(FrmGetPtr(frmP, MainCaptureFormatPopTrigger), gFormatLabel[gCaptureFormatSetting]);
}

/**
 * Set the White Balance UI
 */
static void PrvSetWhiteUI(FormType *frmP)
{
	// Always query the latest values
	GetCaptureSize(gCameraLibRefNum, &gWhiteSetting);
	
	CtlSetValue(FrmGetPtr(frmP, MainAutoWBCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainIncandescentCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainFluorescentCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainSunlightCheckbox), false);
	
	switch(gWhiteSetting)
	{
		case kCamLibWhiteBalanceAuto:
			CtlSetValue(FrmGetPtr(frmP, MainAutoWBCheckbox), true);
			break;
					
		case kCamLibWhiteBalanceIncandescent:
			CtlSetValue(FrmGetPtr(frmP, MainIncandescentCheckbox), true);
			break;
			
		case kCamLibWhiteBalanceFluorescent:
			CtlSetValue(FrmGetPtr(frmP, MainFluorescentCheckbox), true);
			break;
		
		case kCamLibWhiteBalanceSunny:
			CtlSetValue(FrmGetPtr(frmP, MainSunlightCheckbox), true);
			break;
	}
}

/**
 * Set the Lighting UI
 */
static void PrvSetLightUI(FormType *frmP)
{
	// Always query the latest values
	GetLight(gCameraLibRefNum, &gLightSetting);
	
	CtlSetValue(FrmGetPtr(frmP, MainNormalCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainLowLightCheckbox), false);
	
	switch(gLightSetting)
	{
		case kCamLibLightingNormal:
			CtlSetValue(FrmGetPtr(frmP, MainNormalLightCheckbox), true);
			break;
					
		case kCamLibLightingLow:
			CtlSetValue(FrmGetPtr(frmP, MainLowLightCheckbox), true);
			break;
	}
}

/**
 * Set the Effect UI
 */
static void PrvSetEffectUI(FormType *frmP)
{
	// Always query the latest values
	GetEffect(gCameraLibRefNum, &gEffectSetting);
	
	CtlSetValue(FrmGetPtr(frmP, MainNormalCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainSepiaCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainBWCheckbox), false);
	CtlSetValue(FrmGetPtr(frmP, MainBlueCheckbox), false);
	
	switch(gEffectSetting)
	{
		case kCamLibEffectsNormal:
			CtlSetValue(FrmGetPtr(frmP, MainNormalCheckbox), true);
			break;
					
		case kCamLibEffectsSepia:
			CtlSetValue(FrmGetPtr(frmP, MainSepiaCheckbox), true);
			break;
			
		case kCamLibEffectsBlackWhite:
			CtlSetValue(FrmGetPtr(frmP, MainBWCheckbox), true);
			break;
					
		case kCamLibEffectsBluish:
			CtlSetValue(FrmGetPtr(frmP, MainBlueCheckbox), true);
			break;
	}
}

/**
 * Set the Zoom UI
 */
static void PrvSetZoomUI(FormType *frmP)
{
	UInt16 value = 0;
	UInt16 pageSize = 1;
	UInt16 minValue = 0;
	UInt16 maxValue = 0;
	ControlType *ctlP = NULL;
	
	// Always query the latest values
	GetZoom(gCameraLibRefNum, &gZoom);
	

	// Contrast
	value = gZoom.value / 1000;
	minValue = gZoom.minValue / 1000;
	maxValue = gZoom.maxValue / 1000;
	if(minValue != maxValue) {
		ctlP = FrmGetPtr(frmP, MainZoomSliderControl);
		CtlSetSliderValues(ctlP, &minValue, &maxValue, &pageSize, &value);
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainZoomSliderControl));
	}
}

/**
 * Set the Stream UI
 */
static void PrvSetStreamUI(FormType *frmP)
{
	UInt32 currentSize = 0;
	UInt32 currentFormat = 0;
	
	// Always query the latest values
	GetStreamSize(gCameraLibRefNum, &currentSize);
	GetStreamFormat(gCameraLibRefNum, &currentFormat);
	
	gStreamSizeSetting = 0;
	gStreamFormatSetting = 0;
	
	switch(currentSize)
	{
		case kCamLibImageSizeSXGA:
			gStreamSizeSetting++;
		case kCamLibImageSizeVGA:
			gStreamSizeSetting++;
		case kCamLibImageSizeQVGA:
			gStreamSizeSetting++;
		case kCamLibImageSizeQQVGA:
			break;
	}
	
	switch(currentFormat)
	{
		case kCamLibCaptureDataFormatYCbCr422Planar:
			gStreamFormatSetting++;
		case kCamLibCaptureDataFormatYUV422Planar:
			gStreamFormatSetting++;
		case kCamLibCaptureDataFormatYCbCr422:
			gStreamFormatSetting++;
		case kCamLibCaptureDataFormatYUV422:
			gStreamFormatSetting++;
		case kCamLibCaptureDataFormatRGB565:
			break;
	}
	
	CtlSetLabel(FrmGetPtr(frmP, MainStreamPopTrigger), gSizeLabel[gStreamSizeSetting]);
	CtlSetLabel(FrmGetPtr(frmP, MainStreamFormatPopTrigger), gFormatLabel[gStreamFormatSetting]);
}

#if 0
#pragma mark -
#endif

/**
 * Fill Contrast details in Query Form
 */
static void PrvFillContrast( FormType* frmP )
{
	Err err = errNone;
	CamLibSettingType contrast = { 0, 0, 0, 0 };
	Char s[20];
	
	err = GetContrast(gCameraLibRefNum, &contrast);
	
	if(! err ){
	
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryCValueField), StrIToA (s, contrast.value), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryCMinField), StrIToA (s, contrast.minValue), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryCMaxField), StrIToA (s, contrast.maxValue), true);
	}
}

/**
 * Fill Zoom details in Query Form
 */
static void PrvFillZoom( FormType* frmP )
{
	Err err = errNone;
	CamLibSettingType zoom = { 0, 0, 0, 0 };
	Char s[20];
	
	err = GetZoom(gCameraLibRefNum, &zoom);
	
	if(!err){
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryZValueField), StrIToA (s, zoom.value), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryZMinField), StrIToA (s, zoom.minValue), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryZMaxField), StrIToA (s, zoom.maxValue), true);
    }
}

/**
 * Fill Exposure details in Query Form
 */
static void PrvFillExposure( FormType* frmP )
{
	Err err = errNone;
	CamLibSettingType exposure = { 0, 0, 0, 0 };
	Char s[20];
	
	err = GetExposure(gCameraLibRefNum, &exposure);
	
	if(!err){
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryEValueField), StrIToA (s, exposure.value), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryEMinField), StrIToA (s, exposure.minValue), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryEMaxField), StrIToA (s, exposure.maxValue), true);
	}
}

/**
 * Fill Light details in Query Form
 */
static void PrvFillLight( FormType* frmP )
{
	Err err = errNone;
	UInt32 light = 0;
	Char s[20];
	
	err = GetLight(gCameraLibRefNum, &light);
	
	if( light == kCamLibLightingNormal )
		StrCopy( s, "Normal");
	else if( light == kCamLibLightingLow)
		StrCopy( s, "LowLight");
	else
		StrCopy( s, "Error");

	SetFieldTextFromStr(FrmGetPtr(frmP, QueryLValueField), s, true);

}

/**
 * Fill Saturation details in Query Form
 */
static void PrvFillSaturation( FormType* frmP )
{
	Err err = errNone;
	CamLibSettingType saturation = { 0, 0, 0, 0 };
	Char s[20];
	
	err = GetSaturation(gCameraLibRefNum, &saturation);
	
	if(!err){
	SetFieldTextFromStr(FrmGetPtr(frmP, QuerySValueField), StrIToA (s, saturation.value), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QuerySMinField), StrIToA (s, saturation.minValue), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QuerySMaxField), StrIToA (s, saturation.maxValue), true);
    }
}

/**
 * Fill Sharpness details in Query Form
 */
static void PrvFillSharpness( FormType* frmP )
{
	Err err = errNone;
	CamLibSettingType sharpness = { 0, 0, 0, 0 };
	Char s[20];
	
	err = GetSharpness(gCameraLibRefNum, &sharpness);
	
	if(!err){
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryShValueField), StrIToA (s, sharpness.value), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryShMinField), StrIToA (s, sharpness.minValue), true);
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryShMaxField), StrIToA (s, sharpness.maxValue), true);
    }
}

/**
 * Fill Special Effects details in Query Form
 */
static void PrvFillEffects( FormType* frmP )
{
	UInt32 effectSetting;
	Char s[20];
	
	GetEffect(gCameraLibRefNum, &effectSetting);	
	
	if( effectSetting == kCamLibEffectsNormal)
		StrCopy( s, "Normal");
	else if( effectSetting == kCamLibEffectsBlackWhite)
		StrCopy( s, "B&W");
	else if( effectSetting == kCamLibEffectsSepia)
		StrCopy( s, "Sepia");
	else if( effectSetting == kCamLibEffectsBluish)
		StrCopy( s, "Blue");
	else
		StrCopy( s, "Error");
	
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryEfValueField), s, true);
	
}

/**
 * Fill WhiteBalance details in Query Form
 */
static void PrvFillWhiteBalance( FormType* frmP )
{
	UInt32 whiteSetting;
	Char s[20];
	
	GetWhiteBalance(gCameraLibRefNum, &whiteSetting);	
	
	if( whiteSetting == kCamLibWhiteBalanceAuto)
		StrCopy( s, "Auto");
	else if( whiteSetting == kCamLibWhiteBalanceIncandescent)
		StrCopy( s, "Incandescent");
	else if( whiteSetting == kCamLibWhiteBalanceFluorescent)
		StrCopy( s, "Fluorescent");
	else if( whiteSetting == kCamLibWhiteBalanceSunny)
		StrCopy( s, "Sunny");
	else
		StrCopy( s, "Error");
	
	SetFieldTextFromStr(FrmGetPtr(frmP, QueryWValueField),s, true);
	
}


#if 0
#pragma mark -
#endif

/**
 * Show the Main controls
 */
static void PrvShowMainControls(FormType *frmP)
{	
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[0]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainPreviewOnButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainAdvancedButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainLightButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSizeButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainWhiteButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainEffectsButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainMain2Button));
	
	// set focus on On Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainPreviewOnButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Main controls
 */
static void PrvHideMainControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainPreviewOnButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainAdvancedButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainLightButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSizeButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainWhiteButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainEffectsButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainMain2Button));
}

/**
 * Show the Main2 controls
 */
static void PrvShowMain2Controls(FormType *frmP)
{	
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[6]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainZoomButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainQueryButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStreamButton));

	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Main controls
 */
static void PrvHideMain2Controls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainZoomButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainQueryButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStreamButton));


}

/**
 * Show the Advanced controls
 */
static void PrvShowAdvancedControls(FormType *frmP)
{
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[1]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainContrastLabel));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainContrastFeedbackSliderControl));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainExposureLabel));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainExposureFeedbackSliderControl));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSaturationLabel));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSaturationFeedbackSliderControl));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSharpnessLabel));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSharpnessFeedbackSliderControl));
	PrvSetAdvancedUI(frmP);
	
	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Advanced controls
 */
static void PrvHideAdvancedControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainContrastLabel));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainContrastFeedbackSliderControl));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainExposureLabel));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainExposureFeedbackSliderControl));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSaturationLabel));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSaturationFeedbackSliderControl));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSharpnessLabel));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSharpnessFeedbackSliderControl));
}

/**
 * Show the Capture Size controls
 */
static void PrvShowCaptureControls(FormType *frmP)
{
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[2]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainCaptureSizePopTrigger));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainCaptureFormatPopTrigger));
	PrvSetCaptureUI(frmP);
	
	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Capture Size controls
 */
static void PrvHideCaptureControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainCaptureSizePopTrigger));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainCaptureFormatPopTrigger));
}

/**
 * Show the White Balance controls
 */
static void PrvShowWhiteControls(FormType *frmP)
{
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[3]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainAutoWBCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFluorescentCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainIncandescentCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSunlightCheckbox));
	PrvSetWhiteUI(frmP);
	
	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide theWhite Balance controls
 */
static void PrvHideWhiteControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainAutoWBCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFluorescentCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainIncandescentCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSunlightCheckbox));
}

/**
 * Show the Light controls
 */
static void PrvShowLightControls(FormType *frmP)
{
	PrvSetLightUI(frmP);
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[4]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainNormalLightCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainLowLightCheckbox));
	
	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Light controls
 */
static void PrvHideLightControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainNormalLightCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainLowLightCheckbox));
}

/**
 * Show the Effects controls
 */
static void PrvShowEffectsControls(FormType *frmP)
{
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[5]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainNormalCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBWCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainSepiaCheckbox));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBlueCheckbox));
	
	PrvSetEffectUI(frmP);
	
	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Effects controls
 */
static void PrvHideEffectsControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainNormalCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBWCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainSepiaCheckbox));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBlueCheckbox));

}

/**
 * Show the Zoom controls
 */
static void PrvShowZoomControls(FormType *frmP)
{
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[7]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainZoomSliderControl));
	PrvSetZoomUI(frmP);
	
	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Zoom controls
 */
static void PrvHideZoomControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainZoomSliderControl));
}

/**
 * Show the Query controls
 */
static void PrvShowQueryControls(FormType *frmP)
{
	FrmGotoForm( QueryForm );
	
}

/**
 * Hide the Query controls
 */
static void PrvHideQueryControls(FormType *frmP)
{
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));

}


/**
 * Show the Stream controls
 */
static void PrvShowStreamControls(FormType *frmP)
{								
	// set this flag so that the select key will respond differently in Stream mode
	gStreamOn = true ;

	PrvSetEffectUI(frmP);
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[9]);
	FldDrawField(FrmGetPtr(frmP, MainPanelTitleField));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStartStreamButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStopStreamButton));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStreamPopTrigger));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStreamList));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStreamFormatPopTrigger));
	FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainStreamFormatList));
	PrvSetStreamUI(frmP);

	// set focus on Back Button
	FrmSetFocus(frmP, noFocus);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmDrawForm(frmP);
}

/**
 * Hide the Stream controls
 */
static void PrvHideStreamControls(FormType *frmP)
{
	Err err = errNone ;
	if (gStreamOn == true)
	{
		if (gStreamStart == true)
		{
			err = StopStream(gCameraLibRefNum, &gStreamType);
			gStreamStart = false;
			ERROR_ALERT(err, "Error Stopping Stream");
		}
		gStreamOn = false ;
	}
		
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainBackButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStartStreamButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStopStreamButton));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStreamPopTrigger));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStreamList));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStreamFormatPopTrigger));
	FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainStreamFormatList));
}

/**
 * Show the Hardware Dialog
 */
/*
static void PrvShowHardwareDlg(FormType *frmP)
{
	Err err = errNone;
	
	// Stop preview
	if(gPreviewOn)
		err = TurnPreviewOff(gCameraLibRefNum);
	
	err = CamLibControl(gCameraLibRefNum, kCamLibCtrlHardwareSettingsDlg, NULL);
	ERROR_ALERT(err, "Hardware Dialog not supported");	
	
	// Resume preview 
	if(gPreviewOn)
		err = TurnPreviewOn(gCameraLibRefNum);
}
*/
/**
 * Hide the current control panel
 */
static void PrvHideCurrentControls(FormType *frmP)
{
	switch(gCurrentControls)
	{
		case MAIN:
			PrvHideMainControls(frmP);
			break;
		case MAIN2:
			PrvHideMain2Controls(frmP);
			break;
		case ADVANCED:
			PrvHideAdvancedControls(frmP);
			break;
		case SIZE:
			PrvHideCaptureControls(frmP);
			break;
		case WHITE:
			PrvHideWhiteControls(frmP);
			break;
		case LIGHT:
			PrvHideLightControls(frmP);
			break;
		case EFFECTS:
			PrvHideEffectsControls(frmP);
			break;
		case ZOOM:
			PrvHideZoomControls(frmP);
			break;
		case QUERY:
			PrvHideQueryControls(frmP);
			break;
		case STREAM:
			PrvHideStreamControls(frmP);
			break;
	}
}

/**
 * Show the current control panel
 */
static void PrvShowCurrentControls(FormType *frmP)
{
	switch(gCurrentControls)
	{
		case MAIN:
			PrvShowMainControls(frmP);
			break;
		case MAIN2:
			PrvShowMain2Controls(frmP);
			break;
		case ADVANCED:
			PrvShowAdvancedControls(frmP);
			break;
		case SIZE:
			PrvShowCaptureControls(frmP);
			break;
		case WHITE:
			PrvShowWhiteControls(frmP);
			break;
		case LIGHT:
			PrvShowLightControls(frmP);
			break;
		case EFFECTS:
			PrvShowEffectsControls(frmP);
			break;
		case ZOOM:
			PrvShowZoomControls(frmP);
			break;
		case QUERY:
			PrvShowQueryControls(frmP);
			break;
		case STREAM:
			PrvShowStreamControls(frmP);
			break;
	}
}

#if 0
#pragma mark -
#endif

/**
 * Photo capture callback
 */
static Err PrvPhotoCaptureCallback(void *bufferP, UInt32 size, void *userDataP)
{	
	PhotoCaptureInfo *captureInfoP = (PhotoCaptureInfo *)userDataP;
	
	// Copy
	MemMove(captureInfoP->bufferP, bufferP, size);
	captureInfoP->bufferP += size;
	
	return errNone;
}

/**
 * Handles 5-Way navigator.
 */
static Boolean PrvHandleFiveWay(EventType *eventP)
{
	Boolean handled = false;
	Err 	err 	= errNone;
		
	// Did we press the Select button
	if( NavSelectPressed(eventP) )
	{
		// check if we are in Stream mode, if yes, select key will trigger capturing stream on or off
		if (gStreamOn == true)
		{
			handled = true;
			
			// There is a known issue of CamLibControl(xx, kCamLibCtrlStreamStart, xx) on 700p family
			// It will still return 0 even the camera is not turned on
			// So we determine if the device is 700p family or not. 
			// If yes, return directly
			if (IsDevice700P ())
			{

				TurnPreviewOff(gCameraLibRefNum);
		  		FrmCustomAlert (InfoAlert, "Please test this functionality on Treo 650 and 680", "", "");
		  		TurnPreviewOn(gCameraLibRefNum);
		  		
		  		goto Done;
			}
			
			if (gStreamStart == false)
			{
				err = StartStream(gCameraLibRefNum, &gStreamType);
								
				if (err != errNone)
				{
					FrmCustomAlert(ErrorAlert, "Err Start Stream", NULL, NULL);
					goto Done;
				}
				
				
				err = CamLibControl(gCameraLibRefNum, kCamLibCtrlStreamWaitForFrame, &gStreamType);
				if (err)
				{
					FrmCustomAlert(ErrorAlert, "Err WaitForFrame", NULL, NULL);
				}
				
				
				err = CamLibControl(gCameraLibRefNum, kCamLibCtrlStreamUnlockFrame, &gStreamType);
				if (err)
				{
					FrmCustomAlert(ErrorAlert, "Err SteamUnlockFrame", NULL, NULL);
				}
						
				gStreamStart = true;
						
				goto Done;
			}
			else 
			{
				if (err)
				{
					FrmCustomAlert(ErrorAlert, "TurnPreviewOn fail", NULL, NULL);
				}
				
				err = StopStream(gCameraLibRefNum, &gStreamType);
				//ERROR_ALERT(err, "Error Stopping Stream");
				if (err)
				{			
					FrmCustomAlert(ErrorAlert, "Err Stopping Stream", NULL, NULL);
					goto Done;
				}
				gStreamStart = false;

				goto Done;
			} 
		}
		else 
		{		
			FormType *frmP = NULL;
			CamLibBitmapInfoType info;
			CamLibCaptureType capture;
			CamLibSettingType captureFormat;
			
			handled = true;
			
			// Set the capture format
			captureFormat.type = gFormat[gCaptureFormatSetting];
			err = CamLibControl(gCameraLibRefNum, kCamLibCtrlCaptureFormatSet, (void*)&captureFormat);
			ERROR_ALERT(err, "Bad capture format");
			if(err)
				goto Done;
			
			// Get bitmap info
			err = CamLibControl(gCameraLibRefNum, kCamLibCtrlCaptureGetBitmapInfo, (void*)&info);
			if(err)
				goto Done;

			// info.width, info.height, info.rowBytes, info.bitsPerPixel
			if(info.width != gCaptureInfo.width || info.height != gCaptureInfo.height || !gCaptureInfo.winH)
			{
				gCaptureInfo.width = info.width;
				gCaptureInfo.height = info.height;
				
				// Delete any old window
				if(gCaptureInfo.winH)
					WinDeleteWindow(gCaptureInfo.winH, false);
					
				// Allocate new one
				gCaptureInfo.winH = WinCreateOffscreenWindow(gCaptureInfo.width/2, gCaptureInfo.height/2, nativeFormat, &err);
				ERROR_ALERT(err, "Unable to create offscreen window");
				if(err)
					goto Done;
			}
			
			// Get the window pointer
			gCaptureInfo.bufferP = BmpGetBits(WinGetBitmap(gCaptureInfo.winH));			
			if(!gCaptureInfo.bufferP)
				goto Done;
			
			// Stop preview
			if(gPreviewOn)
				TurnPreviewOff(gCameraLibRefNum);
			
			frmP = FrmInitForm(SavingForm);
			FrmDrawForm(frmP);
			
			// Capture the image
			capture.userDataP = &gCaptureInfo;
			capture.callbackP = PrvPhotoCaptureCallback;
			err = CamLibControl(gCameraLibRefNum, kCamLibCtrlCapture, &capture);
			ERROR_ALERT(err, "Capture callback failed");
			if(err) {
				if(gPreviewOn)
					TurnPreviewOn(gCameraLibRefNum);
				goto Done;
			}
			
			FrmEraseForm(frmP);
			FrmDeleteForm(frmP);
			
			// Change to the display window
			FrmGotoForm(DisplayForm);
		}
	}
	
Done:
	return handled;
}

#if 0
#pragma mark -
#endif

/**
 * Initialize the form
 */
static void DisplayFormInit(FormType *frmP)
{
	Coord x, y;
	RectangleType rect = { { 0, 0 }, { 0, 0 } };
	
	// LcdOverlay library can be used to display images YUV422P and YCbCr422P
	if( (gFormat[gCaptureFormatSetting] == kCamLibCaptureDataFormatYUV422Planar ||
		gFormat[gCaptureFormatSetting] ==  kCamLibCaptureDataFormatYCbCr422Planar) &&
		gSize[gCaptureSizeSetting] < kCamLibImageSizeVGA )
	{
		Err err = errNone;
		LcdOverlayLibSettingType overlaySetting;
		RectangleType rect = { { 0, 0 }, { 0, 0 } };
		
		gPanning = false;
		gCaptureInfo.bufferP = BmpGetBits(WinGetBitmap(gCaptureInfo.winH));
		
		rect.extent.x = gCaptureInfo.width / 2;
		rect.extent.y = gCaptureInfo.height / 2;
		
		overlaySetting.type = gOverlayFormat[gCaptureFormatSetting];
		err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlFormatSet, (void*)&overlaySetting);
		err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlRectSet, &rect);
		err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlStart, NULL);
		
		err = LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlDraw, gCaptureInfo.bufferP);
		
		return;
	}
	
	if( gFormat[gCaptureFormatSetting] == kCamLibCaptureDataFormatRGB565 ){
	gImageX = 0;
	gImageY = 0;
	
	// Check if we can pan in the image
	if(gCaptureInfo.width > 320 || gCaptureInfo.height > 320)
		gPanning = true;
	else
		gPanning = false;
		
	// Image size
	rect.extent.x = gCaptureInfo.width / 2;
	if(rect.extent.x > 160)	rect.extent.x = 160;
	rect.extent.y = gCaptureInfo.height / 2;
	if(rect.extent.y > 160)	rect.extent.y = 160;
	
	// Center the image
	x = 80 - rect.extent.x / 2;
	y = 80 - rect.extent.y / 2;
		
	// Display the capture image
	WinCopyRectangle(gCaptureInfo.winH, WinGetDisplayWindow(), &rect, x, y, winPaint);
	return;
   }
   
   gCannotDisplay = true; // We cannot display the image.
   FrmGotoForm( MainForm);
   
   
}

/**
 * Track Pendown
 */
static void PrvTrackPenDown(EventType *eventP)
{
	UInt32 startTick = TimGetTicks();
	Int16 oldX, oldY, newX, newY;
	Boolean penIsDown = eventP->penDown;
	oldX = eventP->screenX;
	oldY = eventP->screenY;
	
	while(penIsDown)
	{
		EvtGetPen(&newX, &newY, &penIsDown);
		
		if(newX == oldX && newY == oldY)
			continue;
			
		gImageX += (oldX - newX);
		gImageY += (oldY - newY);
		
		if(gImageX < 0)	gImageX = 0;
		if(gImageX > (gCaptureInfo.width / 2 - 160))	gImageX = gCaptureInfo.width /2 - 160;
		if(gImageY < 0)	gImageY = 0;
		if(gImageY > (gCaptureInfo.height / 2 - 160))	gImageY = gCaptureInfo.height / 2 - 160;
		
		// If panning is enabled
		if(gPanning) {
			RectangleType rect = { { 0, 0 }, { 160, 160 } };
			
			// Image Coord
			rect.topLeft.x = gImageX;
			rect.topLeft.y = gImageY;
			
			// Display the capture image
			WinCopyRectangle(gCaptureInfo.winH, WinGetDisplayWindow(), &rect, 0, 0, winPaint);
		}
		
		oldX = newX;
		oldY = newY;
	}
	
	if(TimGetTicks() - startTick < TRACK_TIME) {
		if( (gFormat[gCaptureFormatSetting] == kCamLibCaptureDataFormatYUV422Planar ||
		gFormat[gCaptureFormatSetting] ==  kCamLibCaptureDataFormatYCbCr422Planar) &&
		gSize[gCaptureSizeSetting] < kCamLibImageSizeVGA )
			LcdOverlayLibControl(gLCDOverlayRefNum, kLcdOverlayLibCtrlStop, NULL);
		FrmGotoForm(MainForm);
	}
}

/**
 * DisplayFormHandleEvent
 */
static Boolean DisplayFormHandleEvent(EventType *eventP)
{
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();

	switch(eventP->eType)
	{
		case frmOpenEvent:
			FrmDrawForm(frmP);
			DisplayFormInit(frmP);
			handled = true;
			break;
			
		case penDownEvent:
			PrvTrackPenDown(eventP);
			handled = true;
			break;
			
		default:
			break;
	}
	
	return handled;	
}

/**
 * QueryFormInit - Initialize all the fields in the form.
 */
static void QueryFormInit(FormType *frmP)
{

	//Fill Contrast
	PrvFillContrast(frmP);
	
	//Fill Exposure
	PrvFillExposure(frmP);
	
	//Fill Zoom
	PrvFillZoom(frmP);
	
	//Fill Light
	PrvFillLight(frmP);
	
	//Fill Saturation
	PrvFillSaturation(frmP);

	//Fill Sharpness
	PrvFillSharpness(frmP);	
	
	//Fill Effects
	PrvFillEffects(frmP);
	
	//Fill White Balance
	PrvFillWhiteBalance( frmP);

}


/**
 * QueryFormHandleEvent
 */
static Boolean QueryFormHandleEvent(EventType *eventP)
{
	Err err = errNone;
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();

	switch(eventP->eType)
	{
		case frmOpenEvent:
			// Turn off the preview first
			err = TurnPreviewOff(gCameraLibRefNum);
			
			// Set focus on OK button
			FrmSetFocus(frmP, noFocus);
		  	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, QueryOKButton));
		  	
			FrmDrawForm(frmP);
			QueryFormInit(frmP);
			fromQuery = true;
			handled = true;
			break;
			
		case ctlSelectEvent:
			if(eventP->data.ctlSelect.controlID == QueryOKButton)
				FrmGotoForm(MainForm);
			
			
		default:
			break;
	}
	
	return handled;	
}


#if 0
#pragma mark -
#endif			
					
/**
 * Initialize the form
 */
static void MainFormInit(FormType *frmP)
{
	SetCaptureSize(gCameraLibRefNum, gSize[gCaptureSizeSetting]);
	FldSetTextPtr(FrmGetPtr(frmP, MainPanelTitleField), gLabel[0]);
	if(gPreviewOn)
		CtlSetLabel(FrmGetPtr(FrmGetActiveForm(), MainPreviewOnButton), gButton[1]);
	PrvShowCurrentControls(frmP);
}


/**
 * @brief MainFormDoCommand
 *	This routine performs the menu command specified.
 *
 *
 * @param command	menu item id
 * @retval true if the event was handled and should not be passed to
 *     FrmHandleEvent
 *
 **/

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appFileCreator, true, &cardNo, &dbID);

	switch (command)
	{
		case OptionsAbout:
			MenuEraseStatus(0);
			HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
			handled = true;
		break;
	}

	return handled;
}



/**
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent(EventType *eventP)
{
	Err err = errNone;
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();
	
	UInt16 fieldIndex = 0;

	switch(eventP->eType)
	{
	
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			MainFormInit(frmP);
			
			// If coming back from Query form, no need to set focus at this point
			if (fromQuery == false)
			{
				/* Set focus on ON button */
				FrmSetFocus(frmP, noFocus);
			  	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainPreviewOnButton));
				FrmDrawForm(frmP);
			}
			else
				fromQuery = false; // come to main form from query form
			
			if(gPreviewOn)
				TurnPreviewOn(gCameraLibRefNum);
				
			if( gCannotDisplay == true )
			{
			 	TurnPreviewOff(gCameraLibRefNum);
				FrmCustomAlert(ErrorAlert,"Cannot Display Format","","");
				gCannotDisplay = false;
				MainFormInit(frmP);				
				TurnPreviewOn(gCameraLibRefNum);
				gPreviewOn = true;
			}
			
			handled = true;
			break;
			
		case keyDownEvent:
				
			fieldIndex = FrmGetFocus(frmP);
			if (fieldIndex ==  FrmGetObjectIndex(frmP, MainPreviewOnButton))
			{
				if (gPreviewOn == true)
					handled = PrvHandleFiveWay(eventP);
			} 
			else if (fieldIndex ==  FrmGetObjectIndex(frmP, MainStartStreamButton) ||
			         fieldIndex ==  FrmGetObjectIndex(frmP, MainStopStreamButton))
				{	
					handled = PrvHandleFiveWay(eventP);
				}

			break;
		
		case popSelectEvent:
			switch(eventP->data.popSelect.controlID)
			{
				case MainCaptureSizePopTrigger:
					gCaptureSizeSetting = eventP->data.popSelect.selection;
					err = SetCaptureSize(gCameraLibRefNum, gSize[gCaptureSizeSetting]);
					ERROR_ALERT(err, "Capture Size not Set");
					break;
					
				case MainCaptureFormatPopTrigger:
					gCaptureFormatSetting = eventP->data.popSelect.selection;
					err = SetCaptureFormat(gCameraLibRefNum, gFormat[gCaptureFormatSetting]);
					ERROR_ALERT(err, "Capture Format not Set");
					break;
					
				case MainStreamPopTrigger:
					gStreamSizeSetting = eventP->data.popSelect.selection;
					err = SetStreamSize(gCameraLibRefNum, gSize[gStreamSizeSetting]);
					ERROR_ALERT(err, "Stream Size not Set");
					break;
					
				case MainStreamFormatPopTrigger:
					gStreamFormatSetting = eventP->data.popSelect.selection;
					err = SetStreamFormat(gCameraLibRefNum, gFormat[gStreamFormatSetting]);
					ERROR_ALERT(err, "Stream Format not Set");
					
					break;
			}
			break;
		
		case frmUpdateEvent:
			break;
				
		case ctlSelectEvent:
			switch(eventP->data.ctlSelect.controlID)
			{
				// Main Controls
				case MainBackButton:
					PrvHideCurrentControls(frmP);
					if(gPreviousControls == MAIN) {
						PrvShowMainControls(frmP);
						gCurrentControls = gPreviousControls;
					}
					else if(gPreviousControls == MAIN2) {
						PrvShowMain2Controls(frmP);
						gCurrentControls = gPreviousControls;
						gPreviousControls = MAIN;
					}	
					
					break;
				
				case MainMain2Button:
					PrvHideCurrentControls(frmP);
					PrvShowMain2Controls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = MAIN2;
					break;
						
				case MainAdvancedButton:
					PrvHideCurrentControls(frmP);
					PrvShowAdvancedControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = ADVANCED;
					break;
					
				case MainSizeButton:
					PrvHideCurrentControls(frmP);
					PrvShowCaptureControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = SIZE;
					break;
					
				case MainWhiteButton:
					PrvHideCurrentControls(frmP);
					PrvShowWhiteControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = WHITE;
					break;
					
				case MainLightButton:
					PrvHideCurrentControls(frmP);
					PrvShowLightControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = LIGHT;
					break;
					
				case MainEffectsButton:
					PrvHideCurrentControls(frmP);
					PrvShowEffectsControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = EFFECTS;
					break;
					
				case MainPreviewOnButton:
					if(gPreviewOn) {
						if(TurnPreviewOff(gCameraLibRefNum) == errNone) {
							CtlSetLabel(FrmGetPtr(frmP, MainPreviewOnButton), gButton[0]);
							gPreviewOn = false;
							
						}
					} else {
						if(TurnPreviewOn(gCameraLibRefNum) == errNone) {
							CtlSetLabel(FrmGetPtr(frmP, MainPreviewOnButton), gButton[1]);
							gPreviewOn = true;

						}
					}
					handled = true;
					break;	
					
				// Main2 Controls
				case MainZoomButton:
					PrvHideCurrentControls(frmP);
					PrvShowZoomControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = ZOOM;
					break;
					
				case MainQueryButton:
					FrmGotoForm( QueryForm );

					break;
					
					
				case MainStreamButton:
					PrvHideCurrentControls(frmP);
					PrvShowStreamControls(frmP);
					gPreviousControls = gCurrentControls;
					gCurrentControls = STREAM;

					break;					
								
				// Light Controls
				case MainNormalLightCheckbox:
					gLightSetting = kCamLibLightingNormal;
					err = SetLight(gCameraLibRefNum, gLightSetting);
					ERROR_ALERT(err, "Normal Light not Set");
					break;
					
				case MainLowLightCheckbox:
					gLightSetting = kCamLibLightingLow;
					err = SetLight(gCameraLibRefNum, gLightSetting);
					ERROR_ALERT(err, "Low Light not Set");
					break;
					
				// White Controls
				case MainAutoWBCheckbox:
					gWhiteSetting = kCamLibWhiteBalanceAuto;
					err = SetWhiteBalance(gCameraLibRefNum, gWhiteSetting);
					ERROR_ALERT(err, "Auto WB not Set");
					break;
					
				case MainIncandescentCheckbox:
					gWhiteSetting = kCamLibWhiteBalanceIncandescent;
					err = SetWhiteBalance(gCameraLibRefNum, gWhiteSetting);
					ERROR_ALERT(err, "Incandescent WB not Set");
					break;
					
				case MainFluorescentCheckbox:
					gWhiteSetting = kCamLibWhiteBalanceFluorescent;
					err = SetWhiteBalance(gCameraLibRefNum, gWhiteSetting);
					ERROR_ALERT(err, "Fluorescent WB not Set");
					break;
					
				case MainSunlightCheckbox:
					gWhiteSetting = kCamLibWhiteBalanceSunny;
					err = SetWhiteBalance(gCameraLibRefNum, gWhiteSetting);
					ERROR_ALERT(err, "Sunlight WB not Set");
					break;
				
				// Effects Controls
				case MainNormalCheckbox:
					gEffectSetting = kCamLibEffectsNormal;
					err = SetEffect(gCameraLibRefNum, gEffectSetting);
					ERROR_ALERT(err, "Normal Effect not Set");
					break;
					
				case MainSepiaCheckbox:
					gEffectSetting = kCamLibEffectsSepia;
					err = SetEffect(gCameraLibRefNum, gEffectSetting);
					ERROR_ALERT(err, "Sepia Effect not Set");
					break;
					
				case MainBWCheckbox:
					gEffectSetting = kCamLibEffectsBlackWhite;
					err = SetEffect(gCameraLibRefNum, gEffectSetting);
					ERROR_ALERT(err, "B&W Effect not Set");
					break;
					
				case MainNegativeCheckbox:
					gEffectSetting = kCamLibEffectsNegative;
					err = SetEffect(gCameraLibRefNum, gEffectSetting);
					ERROR_ALERT(err, "Negative Effect not Set");
					break;
					
				case MainBlueCheckbox:
					gEffectSetting = kCamLibEffectsBluish;
					err = SetEffect(gCameraLibRefNum, gEffectSetting);
					if ( err != errNone)
					{
						ERROR_ALERT(err, "Blue Effect not Set");
					}
					break;
				
				// Stream Controls. Start capturing stream video
				case MainStartStreamButton:
				{
					// There is a known issue of CamLibControl(xx, kCamLibCtrlStreamStart, xx) on 700p family
					// It will still return 0 even the camera is not turned on
					// So we determine if the device is 700p family or not. 
					// If yes, return directly
					if (IsDevice700P ())
					{
					    TurnPreviewOff(gCameraLibRefNum);
				  		FrmCustomAlert (InfoAlert, "Please test this functionality on Treo 650 and 680", "", "");
						TurnPreviewOn(gCameraLibRefNum);
				  		break ;
					}
					
					err = StartStream(gCameraLibRefNum, &gStreamType);
					if (err)
					{
						FrmCustomAlert(ErrorAlert, "Err Start Stream", NULL, NULL);
						break ;
					}
					
						
					err = CamLibControl(gCameraLibRefNum, kCamLibCtrlStreamWaitForFrame, &gStreamType);
					if (err)
					{
						FrmCustomAlert(ErrorAlert, "Err StreamWaitForFrame", NULL, NULL);
					}
					
																
					CamLibControl(gCameraLibRefNum, kCamLibCtrlStreamUnlockFrame, &gStreamType);
					if (err)
					{
						FrmCustomAlert(ErrorAlert, "Err SteamUnlockFrame", NULL, NULL);
					}
					
					gStreamStart = true;
					
				}	
					break;
					
				// Stop capturing stream video	
				case MainStopStreamButton:
					err = StopStream(gCameraLibRefNum, &gStreamType);
					if (err)
					{
						TurnPreviewOff(gCameraLibRefNum);
						FrmCustomAlert(ErrorAlert, "Err Stopping Stream", NULL, NULL);
						TurnPreviewOn(gCameraLibRefNum);
						break;
					}
					gStreamStart = false;
					
					//TurnPreviewOn(gCameraLibRefNum);
		
					break;
					
				case MainZoomSliderControl:
					if(gZoom.value != (Int16)eventP->data.ctlSelect.value * 1000)
					{
						gZoom.value = (Int16)eventP->data.ctlSelect.value * 1000;
						err = SetZoom(gCameraLibRefNum, gZoom.value);
						ERROR_ALERT(err, "Zoom not Set");
					
					}
					break;
			}
			break;
			
		case ctlRepeatEvent:
			switch(eventP->data.ctlRepeat.controlID)
			{
				// Advanced Controls - Repeat style
				case MainContrastFeedbackSliderControl:
					gContrast.value = (Int16)eventP->data.ctlRepeat.value;
					err = SetContrast(gCameraLibRefNum, gContrast.value);
					ERROR_ALERT(err, "Contrast not Set");
					break;
					
				case MainExposureFeedbackSliderControl:
					gExposure.value = (Int16)eventP->data.ctlRepeat.value;
					err = SetExposure(gCameraLibRefNum, gExposure.value, false);
					ERROR_ALERT(err, "Exposure not Set");
					break;
					
				case MainSharpnessFeedbackSliderControl:
					gSharpness.value = (Int16)eventP->data.ctlRepeat.value;
					err = SetSharpness(gCameraLibRefNum, gSharpness.value);
					ERROR_ALERT(err, "Sharpness not Set");
					break;
					
				case MainSaturationFeedbackSliderControl:
					gSaturation.value = (Int16)eventP->data.ctlRepeat.value;
					err = SetSaturation(gCameraLibRefNum, gSaturation.value);
					ERROR_ALERT(err, "Saturation not Set");
					break;
			}
			break;
					
		default:
			break;
	}
	
	return handled;
}

/**
 * AppHandleEvent
 */
static Boolean AppHandleEvent(EventType * eventP)
{
    UInt16 formId;
    FormType * frmP;

    if (eventP->eType == frmLoadEvent)
    {
        formId = eventP->data.frmLoad.formID;
        frmP = FrmInitForm(formId);
        FrmSetActiveForm(frmP);

        switch (formId)
        {
        case MainForm:
            FrmSetEventHandler(frmP, MainFormHandleEvent);
            break;
            
        case DisplayForm:
            FrmSetEventHandler(frmP, DisplayFormHandleEvent);
            break;
            
        case QueryForm:
            FrmSetEventHandler(frmP, QueryFormHandleEvent);
            break;
       
            
        default:
            break;

        }
        return true;
    }

    return false;
}

/**
 * AppEventLoop
 */
static void AppEventLoop(void)
{
	UInt16 error = errNone;
	EventType event;

    do
    {
    	EvtGetEvent(&event, evtWaitForever);
    	
    	if( /*event.eType == nilEvent &&*/ gStreamStart == true )
    	{
    		CamLibControl(gCameraLibRefNum, kCamLibCtrlStreamWaitForFrame, &gStreamType);											
			CamLibControl(gCameraLibRefNum, kCamLibCtrlStreamUnlockFrame, &gStreamType);					
    		EvtSetNullEventTick(10);
    	}
    	
    	if (IsFiveWayNavEvent(&event) || ! SysHandleEvent(&event))
    		if (! MenuHandleEvent(0, &event, &error))
    			if (! AppHandleEvent(&event))
    				FrmDispatchEvent(&event);

    } while (event.eType != appStopEvent);
}

/**
 * AppStart
 * Called when the application starts.
 */
static Err AppStart(void)
{
	Err err = errNone;
	
	// Try to find the hardware utils library
	err = SysLibFind(kCamLibName, &gCameraLibRefNum);	
	if (err != errNone)
		err = SysLibLoad(kCamLibType, kCamLibCreator, &gCameraLibRefNum);
	
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to load Camera library", NULL, NULL);
		goto Done;
	}
	
	// Open the camera manager
	err = CamLibOpen(gCameraLibRefNum);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to load Camera library", NULL, NULL);
		goto Done;
	}
		
	// Try to load LCD Overlay library - This is to display YCbCr Images
	err = SysLibFind(kLcdOverlayLibName, &gLCDOverlayRefNum);	
	if (err != errNone)
	{
		err = SysLibLoad(kLcdOverlayLibType, kLcdOverlayLibCreator, &gLCDOverlayRefNum);
		if(err == errNone)
			err = LcdOverlayLibOpen(gLCDOverlayRefNum);
	}
	// Don't generate an error for the Overlay library
	err = errNone;
	
	// Set Preview
	err = SetPreviewSize(gCameraLibRefNum, gCurrentPreviewSize);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Preview Size Set error", NULL, NULL);
		goto Done;
	}
	err = SetPreviewRect(gCameraLibRefNum, &gPreviewQVGARectangle);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Preview Area Set error", NULL, NULL);
		goto Done;
	}
	
	// Start Camera
	err = TurnCameraOn(gCameraLibRefNum);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to start the camera", NULL, NULL);
		goto Done;
	}

	// Get current camera settings
	/*
	PrvGetCurrentSettings();
	if(err) {
		FrmCustomAlert(ErrorAlert, "Error getting current settings", NULL, NULL);
		goto Done;
	}
	*/
	
	// Switch to 16 bit color
	WinScreenMode(winScreenModeGet, NULL, NULL, &gPreviousScreenDepth, NULL);
	WinScreenMode(winScreenModeSet, NULL, NULL, &gScreenDepth, NULL);
		
Done:			
	return err;
}

/**
 * AppStop
 * Called when the application stops.
 */
static void AppStop(void)
{
	Err err = errNone;
	
	FrmCloseAllForms();
	
	// Delete offscreen window
	if(gCaptureInfo.winH)
		WinDeleteWindow(gCaptureInfo.winH, false);
	
	// Turn preview and camera off
	if(gPreviewOn)
		TurnPreviewOff(gCameraLibRefNum);
	TurnCameraOff(gCameraLibRefNum);
	
	// Close the camera manager
	if(gCameraLibRefNum != sysInvalidRefNum)
	{
		err = CamLibClose(gCameraLibRefNum);
		if(!err)
			SysLibRemove(gCameraLibRefNum);
	}
	
	/*if(gYUVLibRefNum != sysInvalidRefNum)
	{
		err = YuvToBGRLibClose(gYUVLibRefNum);
		if (err == errNone)
			err = SysLibRemove(gYUVLibRefNum);
	}*/
	
	if(gLCDOverlayRefNum != sysInvalidRefNum)
	{
		err = LcdOverlayLibClose(gLCDOverlayRefNum);
		if (err == errNone)
			err = SysLibRemove(gLCDOverlayRefNum);
	}
	
	// Restore previous color mode
	WinScreenMode(winScreenModeSet, NULL, NULL, &gPreviousScreenDepth, NULL);
}

#pragma warn_a5_access on

//
// RomVersionCompatible
//
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
    UInt32 romVersion;

    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    if (romVersion < requiredVersion)
    {
        if ((launchFlags & 
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
        {
            FrmAlert (RomIncompatibleAlert);

            if (romVersion < kPalmOS20Version)
            {
                AppLaunchWithCommand(
                    sysFileCDefaultApp, 
                    sysAppLaunchCmdNormalLaunch, NULL);
            }
        }

        return sysErrRomIncompatible;
    }

    return errNone;
}

//
// PilotMain
//
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err error;
    
    error = RomVersionCompatible (ourMinVersion, launchFlags);
    if (error) return (error);

    switch (cmd)
    {
    case sysAppLaunchCmdNormalLaunch:
        error = AppStart();
        if (error) 
            return error;

        FrmGotoForm(MainForm);
        AppEventLoop();
        
        AppStop();
        break;
        
    default:
        break;
    }

    return errNone;
}

/**
* Check if device is 700p or Torino
*/
Boolean IsDevice700P ()
{
	UInt32 deviceID=0;
	FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
	/* Check for 700p or Torino device ids */
	if (deviceID == kPalmOneDeviceIDVentura || deviceID == kPalmOneDeviceIDTorino || 
		deviceID == kPalmOneDeviceIDGandalf || deviceID == kPalmOneDeviceIDGnome)
    	return true;
  	else 
  		return false;
}


#pragma warn_a5_access reset
