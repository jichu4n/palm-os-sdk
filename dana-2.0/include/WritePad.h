/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            WritePad.h
 *
 * DESCRIPTION: This file contains the API to the WritePad.
 *
 ****************************************************************************/
#ifndef __WRITEPAD_H__
#define __WRITEPAD_H__

#include "WideTallApp.h"


#define WritePadFtrNum           7

/*---------------------------------------------------------------------------
 * Billboard feature number
 *--------------------------------------------------------------------------*/
#define BillboardFtrNum            3

/*---------------------------------------------------------------------------
 * NotifyMgr: Notification of WritePad minimize/maximize events
 *--------------------------------------------------------------------------*/
#define notifyWritePadEvent      'wrtp'
#define notifyBillboardDrawEvent 'bill'
typedef struct 
{
    Boolean writePad_minimized;
    Boolean writePad_onTheRight;
} writePadEventType;


/*---------------------------------------------------------------------------
 * Extension Installed
 *--------------------------------------------------------------------------*/
#define _WritePadFeaturePresent(version) \
        (Boolean)(FtrGet(AlphaSmartSysFtrID, WritePadFtrNum, version) == 0)

/*---------------------------------------------------------------------------
 * WritePad API result codes
 * (oemErrorClass is reserved for Palm licensees).
 *--------------------------------------------------------------------------*/
#define wrtpErrorClass               (oemErrorClass  | 0x700)
#define wrtpErrBadParam              (wrtpErrorClass | 0)    // invalid parameter


#ifdef __cplusplus
extern "C" {
#endif

#define     wrtMinFunction               		0x0700

#define     wrtGetWritePadWindow              	(wrtMinFunction +  0)
#define     wrtGetWritePadTemplate            	(wrtMinFunction +  1)
#define     wrtWritePadRestoreDefaultTemplate 	(wrtMinFunction +  2)
#define     wrtDrawWritePadWindow             	(wrtMinFunction +  3)
#define     wrtEraseWritePadWindow            	(wrtMinFunction +  4)
#define     wrtGetAreas   						(wrtMinFunction +  5)
#define     wrtSetAreas                   		(wrtMinFunction +  6)
#define     wrtGetButtons                 		(wrtMinFunction +  7)
#define     wrtSetButtons                 		(wrtMinFunction +  8)
#define     wrtGetGraffitiPersistence     		(wrtMinFunction +  9)
#define     wrtSetGraffitiPersistence     		(wrtMinFunction + 10)
#define     wrtSetWritePadTemplate            	(wrtMinFunction + 11)
#define     wrtGetButtonListSize          		(wrtMinFunction + 12)
#define     wrtWritePadWindowShown            	(wrtMinFunction + 13)
#define     wrtSetPenEnabled              		(wrtMinFunction + 14)
#define     wrtGetPenEnabled              		(wrtMinFunction + 15)
#define     wrtWindowRight                      (wrtMinFunction + 16)
#define     wrtGetBillboardWin                  (wrtMinFunction + 17)

#define     wrtMaxFunction                		(wrtMinFunction + 17)    

/********************************************************************
 *              WritePad API Prototypes
 ********************************************************************/

/*---------------------------------------------------------------------------
 * Show the WritePad window.
 *--------------------------------------------------------------------------*/
extern void WrtpMaximizeWindow(void)
                    EXTERNAL_TRAP(wrtDrawWritePadWindow);

/*---------------------------------------------------------------------------
 * Hide the WritePad window.
 *--------------------------------------------------------------------------*/
extern void WrtpMinimizeWindow(void)
                    EXTERNAL_TRAP(wrtEraseWritePadWindow);

/*---------------------------------------------------------------------------
 * Is the WritePad window maximized or minimized?.
 *--------------------------------------------------------------------------*/
extern Boolean WrtpWindowMaximized(void)
                    EXTERNAL_TRAP(wrtWritePadWindowShown);

/*---------------------------------------------------------------------------
 * Return a WinHandle to the WritePad Template to allow drawing directly to the 
 * onscreen window
 *--------------------------------------------------------------------------*/
extern WinHandle WrtpGetWindow(void)
                    EXTERNAL_TRAP(wrtGetWritePadWindow);

/*---------------------------------------------------------------------------
 * Returns pointers to the template bitmaps
 *--------------------------------------------------------------------------*/
extern void WrtpGetTemplateBitmaps(BitmapPtr *maxWritePadTemplate, BitmapPtr *selectedMaxWritePadTemplate, 
                                   BitmapPtr *minWritePadTemplate, BitmapPtr *selectedMinWritePadTemplate)
                    EXTERNAL_TRAP(wrtGetWritePadTemplate);

/*---------------------------------------------------------------------------
 * Provide bitmaps for the WritePad template.
 *--------------------------------------------------------------------------*/
extern Err WrtpSetTemplateBitmaps(BitmapPtr maxWritePadTemplate, BitmapPtr selectedMaxWritePadTemplate, 
                                  BitmapPtr minWritePadTemplate, BitmapPtr selectedMinWritePadTemplate)
                    EXTERNAL_TRAP(wrtSetWritePadTemplate);

/*---------------------------------------------------------------------------
 * Restore the WritePad Template to default.
 *--------------------------------------------------------------------------*/
extern Err WrtpRestoreDefaultTemplates(void)
                    EXTERNAL_TRAP(wrtWritePadRestoreDefaultTemplate);

/*---------------------------------------------------------------------------
 * Get the WritePad screen button list size.  Caller should call this first, 
 * allocate a buffer of this size, then call WritePadGetButtons().
 *--------------------------------------------------------------------------*/
extern UInt16 WrtpGetButtonListSize(Boolean maximized)
                    EXTERNAL_TRAP(wrtGetButtonListSize);

/*---------------------------------------------------------------------------
 * Get the WritePad screen button list
 *--------------------------------------------------------------------------*/
extern Err WrtpGetButtonList(PenBtnListType *buttonList, Boolean maximized)
                    EXTERNAL_TRAP(wrtGetButtons);

/*---------------------------------------------------------------------------
 * Set the WritePad screen button list
 *--------------------------------------------------------------------------*/
extern Err WrtpSetButtonList(PenBtnListType *buttonList, Boolean maximized)
                    EXTERNAL_TRAP(wrtSetButtons);                    

/*---------------------------------------------------------------------------
 * Get the WritePad screen alpha and numeric areas
 *--------------------------------------------------------------------------*/
extern Err WrtpGetAreas(RectangleType *alphaEntry, RectangleType *numericEntry)
                    EXTERNAL_TRAP(wrtGetAreas);

/*---------------------------------------------------------------------------
 * Set the WritePad alpha and numeric areas
 *--------------------------------------------------------------------------*/
extern Err WrtpSetAreas(RectangleType *alphaEntry, RectangleType *numericEntry)
                    EXTERNAL_TRAP(wrtSetAreas);

/*---------------------------------------------------------------------------
 * Get number of timer ticks graffiti remains on the WritePad area
 *--------------------------------------------------------------------------*/
extern UInt32 WrtpGetGraffitiPersistence(void)
                    EXTERNAL_TRAP(wrtGetGraffitiPersistence);

/*---------------------------------------------------------------------------
 * Set number of timer ticks graffiti remains on the WritePad area
 *--------------------------------------------------------------------------*/
extern void WrtpSetGraffitiPersistence(UInt32 ticks)
                    EXTERNAL_TRAP(wrtSetGraffitiPersistence);

/*---------------------------------------------------------------------------
 * Get the window handle to the billboard window.
 * Make sure to check if the handle is NULL!
 *--------------------------------------------------------------------------*/
extern WinHandle WrtpGetBillboardWin(void)
                    EXTERNAL_TRAP(wrtGetBillboardWin);

/*---------------------------------------------------------------------------
 * System use only
 *--------------------------------------------------------------------------*/
extern void WrtpSetPenEnabled(Boolean enabled)
                    EXTERNAL_TRAP(wrtSetPenEnabled);


extern Boolean WrtpGetPenEnabled(void)
                    EXTERNAL_TRAP(wrtGetPenEnabled);

extern Boolean WrtpWindowRight(void)
                    EXTERNAL_TRAP(wrtWindowRight);

#ifdef __cplusplus 
}
#endif


#endif  // __WRITEPAD_H__
