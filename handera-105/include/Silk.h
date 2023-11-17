/****************************************************************************
 *
 *      Copyright (c) 2001, TRG, All Rights Reserved
 *
 *---------------------------------------------------------------------------
 * FileName:
 *              Silk.h
 *
 * Description:
 *              Silkscreen API definitions.
 *
 *
 ****************************************************************************/

#ifndef __SILK_H__
#define __SILK_H__

#include "Trg.h"

#define TRGSilkFtrNum           7


/*---------------------------------------------------------------------------
 * NotifyMgr: Notification of Silk minimize/maximize events
 * NOTE:  If your app is form based, it is much easier to 
 * handle the qvgaChangedEvent in your form handle event instead.
 *--------------------------------------------------------------------------*/
#define trgNotifySilkEvent   'slkx'
typedef struct 
{
    Boolean silk_minimized;
} silkEventType;


/*---------------------------------------------------------------------------
 * Extension Installed
 *--------------------------------------------------------------------------*/
#define _TRGSilkFeaturePresent(version) \
        (Boolean)(FtrGet(TRGSysFtrID, TRGSilkFtrNum, version) == 0)

/*---------------------------------------------------------------------------
 * Silk API result codes
 * (oemErrorClass is reserved for Palm licensees).
 *--------------------------------------------------------------------------*/
#define silkErrorClass               (oemErrorClass  | 0x700)
#define silkErrBadParam              (silkErrorClass | 0)    // invalid parameter


#ifdef __cplusplus
extern "C" {
#endif

#define     trgMinSilkFunction         0x0700

#define     trgGetSilkWindow              (trgMinSilkFunction +  0)
#define     trgGetSilkTemplate            (trgMinSilkFunction +  1)
#define     trgSilkRestoreDefaultTemplate (trgMinSilkFunction +  2)
#define     trgDrawSilkWindow             (trgMinSilkFunction +  3)
#define     trgEraseSilkWindow            (trgMinSilkFunction +  4)
#define     trgGetAreas                   (trgMinSilkFunction +  5)
#define     trgSetAreas                   (trgMinSilkFunction +  6)
#define     trgGetButtons                 (trgMinSilkFunction +  7)
#define     trgSetButtons                 (trgMinSilkFunction +  8)
#define     trgGetGraffitiPersistence     (trgMinSilkFunction +  9)
#define     trgSetGraffitiPersistence     (trgMinSilkFunction + 10)
#define     trgSetSilkTemplate            (trgMinSilkFunction + 11)
#define     trgGetButtonListSize          (trgMinSilkFunction + 12)
#define     trgSilkWindowShown            (trgMinSilkFunction + 13)
#define     trgSetPenEnabled              (trgMinSilkFunction + 14)
#define     trgGetPenEnabled              (trgMinSilkFunction + 15)

#define     trgMaxSilkFunction            (trgMinSilkFunction + 15)    

/********************************************************************
 *              Silk API Prototypes
 ********************************************************************/

/*---------------------------------------------------------------------------
 * Show the Silk window.
 *--------------------------------------------------------------------------*/
extern void SilkMaximizeWindow(void)
                    EXT_TRAP(trgDrawSilkWindow);

/*---------------------------------------------------------------------------
 * Hide the silk window.
 *--------------------------------------------------------------------------*/
extern void SilkMinimizeWindow(void)
                    EXT_TRAP(trgEraseSilkWindow);

/*---------------------------------------------------------------------------
 * Is the silk window maximized or minimized?.
 *--------------------------------------------------------------------------*/
extern Boolean SilkWindowMaximized(void)
                    EXT_TRAP(trgSilkWindowShown);

/*---------------------------------------------------------------------------
 * Return a WinHandle to the Silk Template to allow drawing directly to the 
 * onscreen window
 *--------------------------------------------------------------------------*/
extern WinHandle SilkGetWindow(void)
                    EXT_TRAP(trgGetSilkWindow);

/*---------------------------------------------------------------------------
 * Returns pointers to the template bitmaps
 *--------------------------------------------------------------------------*/
extern void SilkGetTemplateBitmaps(BitmapPtr *maxSilkTemplate, BitmapPtr *selectedMaxSilkTemplate, 
                                   BitmapPtr *minSilkTemplate, BitmapPtr *selectedMinSilkTemplate)
                    EXT_TRAP(trgGetSilkTemplate);

/*---------------------------------------------------------------------------
 * Provide bitmaps for the silk template.
 *--------------------------------------------------------------------------*/
extern Err SilkSetTemplateBitmaps(BitmapPtr maxSilkTemplate, BitmapPtr selectedMaxSilkTemplate, 
                                  BitmapPtr minSilkTemplate, BitmapPtr selectedMinSilkTemplate)
                    EXT_TRAP(trgSetSilkTemplate);

/*---------------------------------------------------------------------------
 * Restore the Silk Template to default.
 *--------------------------------------------------------------------------*/
extern Err SilkRestoreDefaultTemplates(void)
                    EXT_TRAP(trgSilkRestoreDefaultTemplate);

/*---------------------------------------------------------------------------
 * Get the silk screen button list size.  Caller should call this first, 
 * allocate a buffer of this size, then call SilkGetButtons().
 *--------------------------------------------------------------------------*/
extern UInt16 SilkGetButtonListSize(Boolean maximized)
                    EXT_TRAP(trgGetButtonListSize);

/*---------------------------------------------------------------------------
 * Get the silk screen button list
 *--------------------------------------------------------------------------*/
extern Err SilkGetButtonList(PenBtnListType *buttonList, Boolean maximized)
                    EXT_TRAP(trgGetButtons);

/*---------------------------------------------------------------------------
 * Set the silk screen button list
 *--------------------------------------------------------------------------*/
extern Err SilkSetButtonList(PenBtnListType *buttonList, Boolean maximized)
                    EXT_TRAP(trgSetButtons);                    

/*---------------------------------------------------------------------------
 * Get the silk screen alpha and numeric areas
 *--------------------------------------------------------------------------*/
extern Err SilkGetAreas(RectangleType *alphaEntry, RectangleType *numericEntry)
                    EXT_TRAP(trgGetAreas);

/*---------------------------------------------------------------------------
 * Set the silk screen alpha and numeric areas
 *--------------------------------------------------------------------------*/
extern Err SilkSetAreas(RectangleType *alphaEntry, RectangleType *numericEntry)
                    EXT_TRAP(trgSetAreas);

/*---------------------------------------------------------------------------
 * Get number of timer ticks graffiti remains on the silk area
 *--------------------------------------------------------------------------*/
extern UInt32 SilkGetGraffitiPersistence(void)
                    EXT_TRAP(trgGetGraffitiPersistence);

/*---------------------------------------------------------------------------
 * Set number of timer ticks graffiti remains on the silk area
 *--------------------------------------------------------------------------*/
extern void SilkSetGraffitiPersistence(UInt32 ticks)
                    EXT_TRAP(trgSetGraffitiPersistence);

/*---------------------------------------------------------------------------
 * System use only
 *--------------------------------------------------------------------------*/
extern void SilkSetPenEnabled(Boolean enabled)
                    EXT_TRAP(trgSetPenEnabled);


extern Boolean SilkGetPenEnabled(void)
                    EXT_TRAP(trgGetPenEnabled);

#ifdef __cplusplus 
}
#endif


#endif  // __SILK_H__
