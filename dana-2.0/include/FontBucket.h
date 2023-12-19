/***********************************************************************

FontBucket.h

Purpose: 	Include this header file in any source files that work
			with FontBucket. FontBucket enables you to easily include
			custom fonts in your application, and allow users to
			pick their own fonts from popular font collections.	

Copyright © 2000 Hands High Software
All Rights Reserved

The code in this file may be freely copied, distributed and used without 
compensation to  Hands High Software, Inc. or its permission.


API Version: 1.2

File Version: 1.11

See FontBucket.c for history

************************************************************************/


#ifndef _FONTBUCKET_H_
#define _FONTBUCKET_H_

#include "ErrorBase.h"

// The following version number gets incremented whenever an API change happens that
// requires the user to install a new version of FontBucket. If an older version is
// detected, FmInit will return errFontObsoleteFBVersion and FontBucket will act as 
// if it was not installed.

#define kFontBucketVersion         2	


#define PalmFontNameString              9500	
#define VGAFontNameString               9504	
#define BoldStyleString					9550

#define kMaxFontNameSize                64	// 63 + null
#define kMaxFontStyleSize				32  // 31 + null
#define kFontBucketNotInstalled         0

#define kDefaultFontStart				((FontID)fntAppFontCustomBase + ((FontID) 2))
#define kDefaultFontMax					((FontID)255)
#define kNoFontRangeSpecified           ((FontID)0)

#define kNoFmFontID                     ((FmFontID)0)

#define errFontTableOutOfSlots       ((Err)(appErrorClass | 0x01))
#define errFontNotFree               ((Err)(appErrorClass | 0x02))
#define errFontNotFound          	 ((Err)(appErrorClass | 0x03))
#define errFontInvalidRange          ((Err)(appErrorClass | 0x04))
#define errFontBucketNotInitialized	 ((Err)(appErrorClass | 0x05))
#define errFontIndexInvalid          ((Err)(appErrorClass | 0x06))
#define errFontInvalidFmFontID       ((Err)(appErrorClass | 0x07))
#define errFontObsoleteFBVersion	 ((Err)(appErrorClass | 0x08))
#define errFontBadFontData			 ((Err)(appErrorClass | 0x09))

typedef UInt32 FmFontID;

typedef enum {
	FmInitFontLaunchCode = sysAppLaunchCmdCustomBase,
	FmSelectFontLaunchCode,
	FmUseFontLaunchCode,
	FmValidateFontLaunchCode,
	FmFreeFontLaunchCode,
	FmCloseLaunchCode,
	FmNameLaunchCode,
	FmIdLaunchCode,
	FmFontIdLaunchCode,
	FmNumberOfFontsLaunchCode,
	FmInfoLaunchCode,
	FmInfoByIDLaunchCode,
	FMFaceListLaunchCode,
	FMDrawModeLaunchCode
} FmLaunchCodes;

typedef enum {
	fmStd,
	fmHandera,
	fmSony
} FMDeviceType;

typedef struct {
	UInt16	  		card;
	LocalID   		localID;
	FMDeviceType 	device;
#ifdef FB_SONY_SUPPORT
	UInt16			sonyHiResLib;	// sony hi res lib, if present
	Boolean			preventSonyHiRes; // uses the standard OS selector when on a Sony AND the screen is in hi-res mode
#endif
} FmType, *FmPtr;

#if __cplusplus
extern "C" {
#endif

Err FmInit(FmPtr fmPtr,  FontID	firstFont,  FontID    lastFont, Boolean fromSubLaunch);
Boolean FmSelectFont (FmPtr fmPtr, FmFontID* fmFontId);
Err    FmUseFont(FmPtr fmPtr, FmFontID fmFontId, FontID *fontID);
Err    FmFreeFont(FmPtr fmPtr, FontID fontID);
Err    FmClose (FmPtr fmPtr);
Err    FmValidFont(FmPtr fmPtr, FmFontID fmFontId);
Err    FmFontName (FmPtr fmPtr, FmFontID fmFontId, Char *name);
Err    FmGetFMFontIdFromName (FmPtr fmPtr, const Char *name, FmFontID* fmFontId, const Char* style, UInt16 size);
Err    FmGetFMFontID(FmPtr fmPtr, FontID fontID, FmFontID* fmFontId);
UInt16 FmGetFontCount(FmPtr fmPtr);
Err    FmGetIndexedFontInfo(FmPtr fmPtr, UInt16 index, Char* fontName, Char *fontStyle,
			UInt8 *fontSize, FmFontID *fmFontID);
Err    FmGetFontInfo (FmPtr fmPtr, FmFontID fmFontID, Char* fontName, Char *fontStyle, UInt8 *fontSize);
MemHandle	   FmGetFamilyNames (FmPtr fmPtr);
		
// Return true if FontBucket is installed
// Normally, you shouldn't need to call this, since the API works whether or not
// FontBucket is installed. However, if you need to know for some reason, call this after
// calling FmInit.			
__inline Boolean FmIsInstalled (FmPtr fmPtr) {
	return fmPtr->localID != 0;
}

#ifdef FB_SONY_SUPPORT

// Prevent FontBucket from working when in sony hi-res mode.
// This is useful if the user forces hi-res assist mode and your app doesn't handle that, 
// or you only want to use standard fonts in hi-res mode, since Sony can't handle custom fonts
// in fields or form objects when in hi-res mode.
// default is off.

__inline void FmPreventSonyHiRes (FmPtr fmPtr, Boolean prevent) {
	fmPtr->preventSonyHiRes = prevent;
}

__inline Boolean FmIsSonyHiResPrevented (FmPtr fmPtr) {
	return fmPtr->preventSonyHiRes;
}

#endif

#if __cplusplus
}
#endif


#ifdef __cplusplus
// c++ implementation of the fontbucket interface

class FontBucket {
protected:
	FmType m_params;
	
public:	
	FontBucket () {
		m_params.card = 0;
		m_params.localID = 0;
	}
	~FontBucket () { } // need to explicitly close
	
	Err init (FontID first = kNoFontRangeSpecified, 
			FontID last = kNoFontRangeSpecified,
			Boolean fromSubLaunch = false) {
		return FmInit (&m_params, first, last, fromSubLaunch);
	}
	
	Err close (void) {
		return FmClose (&m_params);
	}
	
	Boolean isInstalled (void) {
		return FmIsInstalled (&m_params);
	}

	Boolean select (FmFontID *fmFontId) {
		return FmSelectFont (&m_params, fmFontId);
	}
	
	Err  use (FmFontID fmFontId, FontID *fontID) {
		return FmUseFont(&m_params, fmFontId, fontID);
	}
	
	Err  free (FontID fontID) {
		return FmFreeFont(&m_params, fontID);
	}
	
	Err    valid (FmFontID fmFontId) {
		return FmValidFont(&m_params, fmFontId);
	}
	
	Err    name (FmFontID fmFontId, Char *name) {
		return FmFontName (&m_params, fmFontId, name);
	}
	
	Err    id (const Char *name, FmFontID* fmFontId, const Char* style = NULL, UInt16 size = 0) {
		return FmGetFMFontIdFromName (&m_params, name, fmFontId, style, size);
	}

	Err    id (FontID fontID, FmFontID* fmFontId) {
		return FmGetFMFontID (&m_params, fontID, fmFontId);
	}
	
	UInt16 getFontCount(void) {
		return FmGetFontCount(&m_params);
	}
	
	Err getIndexedFontInfo(UInt16 index, 
								Char * fontName, 
								Char* fontStyle,
								UInt8 *fontSize, 
								FmFontID *fmFontID) {
		return FmGetIndexedFontInfo(&m_params, index, fontName, fontStyle, fontSize, fmFontID);
	}
	
	Err info (FmFontID fmFontId, Char * fontName, 
								Char* fontStyle,
								UInt8 *fontSize) {
		return FmGetFontInfo(&m_params, fmFontId, fontName, fontStyle, fontSize);
	}		
	
	MemHandle familyNames (void) {
		return FmGetFamilyNames (&m_params);
	}
	
#ifdef FB_SONY_SUPPORT
	void preventSonyHiRes (Boolean prevent) {
		FmPreventSonyHiRes (&m_params, prevent);
	}
	
	Boolean isSonyHiResPrevented (void) {
		return FmIsSonyHiResPrevented(&m_params);
	}
#endif
	
	
	
};

#endif // _cplusplus

#endif // _FONTBUCKET_H_