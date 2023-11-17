/****************************************************************************
 *
 *      Copyright (c) 2001, TRG, All Rights Reserved
 *
 *---------------------------------------------------------------------------
 * FileName:
 *              Vga.h
 *
 * Description:
 *              VGA API definitions.
 *
 *
 ****************************************************************************/

#ifndef __VGA_H__
#define __VGA_H__

#include "Trg.h"

#define TRGVgaFtrNum            2

/*--------------------------------------------------------------------------
 * Vga result codes
 * (oemErrorClass is reserved for Palm licensees).
 *-------------------------------------------------------------------------*/
#define vgaErrorClass               (oemErrorClass | 0x200)

#define vgaErrUnimplemented         (vgaErrorClass | 0)    // function not implemented (on this hardware)
#define vgaErrBadParam              (vgaErrorClass | 1)    // invalid parameter
#define vgaErrModeUnsupported       (vgaErrorClass | 2)    // does not support screen mode
#define vgaErrScreenLocked          (vgaErrorClass | 3)    // ???

/*---------------------------------------------------------------------------
 * Extension Installed
 *--------------------------------------------------------------------------*/
#define _TRGVGAFeaturePresent(versionPtr) \
        (Boolean)(FtrGet(TRGSysFtrID, TRGVgaFtrNum, versionPtr) == 0)


/*---------------------------------------------------------------------------
 * Notification that the screen area changed (silk minimize/maximized, rotation)
 *--------------------------------------------------------------------------*/
#define displayExtentChangedEvent  ((eventsEnum)(firstUserEvent-2))
typedef struct displayExtentChangedDataType
{
    RectangleType oldDim;
    RectangleType newDim;
} displayExtentChangedDataType;

/*---------------------------------------------------------------------------
 * Macro to simplify getting the data out of the event structure.
 * Example:
 * yDiff = displayExtentChangedData(eventP)->newDim->extent.y - 
 *         displayExtentChangedData(eventP)->oldDim->extent.y;
 *--------------------------------------------------------------------------*/
#define  displayExtentChangedData(eventP) ((displayExtentChangedDataType *)(&((eventP)->data.generic)))




/*---------------------------------------------------------------------------
 * Screen and rotation modes
 *--------------------------------------------------------------------------*/
typedef enum
{
    screenModeScaleToFit = 0,
    screenMode1To1,
    screenModeOffset        //System Use Only
} VgaScreenModeType;

typedef enum
{
    rotateModeNone = 0,
    rotateMode90, 
    rotateMode180,
    rotateMode270 
} VgaRotateModeType;

/*---------------------------------------------------------------------------
 * Legacy app display modes.  
 *--------------------------------------------------------------------------*/
typedef enum
{
    offsetModeTopLeft = 0,
    offsetModeTopCenter,
    offsetModeTopRight,
    offsetModeCenterLeft,
    offsetModeCenterCenter,
    offsetModeCenterRight,
    offsetModeBottomLeft,
    offsetModeBottomCenter,
    offsetModeBottomRight
} VgaOffsetModeType;

/*---------------------------------------------------------------------------
 * Font Select Types
 *--------------------------------------------------------------------------*/
typedef enum
{
    vgaFontSelectPalm = 0,
    vgaFontSelectVgaText
} VgaFontSelectType;

/*---------------------------------------------------------------------------
 * Form Modify Types
 *--------------------------------------------------------------------------*/
typedef enum
{
    vgaFormModify160To240 = 0
} VgaFormModifyType;

/*---------------------------------------------------------------------------
 * Vga Screen State
 *--------------------------------------------------------------------------*/
typedef struct VgaScreenStateType
{
    VgaScreenModeType   mode;
    VgaRotateModeType   rotate;
    VgaOffsetModeType   offset;
} VgaScreenStateType;

#ifdef __cplusplus
extern "C" {
#endif

#define     trgMinVgaFunction          0x0200
#define     trgGetScreenMode           (trgMinVgaFunction +  0)
#define     trgSetScreenMode           (trgMinVgaFunction +  1)

#define     trgBitmapExpandedExtent    (trgMinVgaFunction +  2)
#define     trgNOP                     (trgMinVgaFunction +  3) //Remove
#define     trgWinDrawBitmapExpanded   (trgMinVgaFunction +  4)

#define     trgBaseToVgaFont           (trgMinVgaFunction +  5)
#define     trgFontSelect              (trgMinVgaFunction +  6)
#define     trgGetFrmTitleHeight       (trgMinVgaFunction +  7)
#define     trgIsVgaFont               (trgMinVgaFunction +  8)
#define     trgVgaEnable               (trgMinVgaFunction +  9)
#define     trgVgaDisable              (trgMinVgaFunction +  10)
#define     trgVgaIsEnabled            (trgMinVgaFunction +  11)
#define     trgGetLegacyMode           (trgMinVgaFunction +  12)
#define     trgSetLegacyMode           (trgMinVgaFunction +  13)

#define     trgTableUseBaseFont        (trgMinVgaFunction +  14)

#define     trgFrmModify               (trgMinVgaFunction +  15)

#define     trgVgaToBaseFont           (trgMinVgaFunction +  16)

#define     trgReloadAppPrefs          (trgMinVgaFunction +  17)

#define     trgSaveScreenState         (trgMinVgaFunction +  18)
#define     trgRestoreScreenState      (trgMinVgaFunction +  19)

#define     trgRotateSelect            (trgMinVgaFunction +  20)

#define     trgMaxVgaFunction          0x0214    


/********************************************************************
 *              VGA API Prototypes
 ********************************************************************/

/*---------------------------------------------------------------------------
 * Returns the current mode and rotation settings
 *--------------------------------------------------------------------------*/
extern void VgaGetScreenMode(VgaScreenModeType *mode, VgaRotateModeType *rotation)
                    EXT_TRAP(trgGetScreenMode);

/*---------------------------------------------------------------------------
 * Set current screen mode.             
 *--------------------------------------------------------------------------*/
extern Err VgaSetScreenMode(VgaScreenModeType mode, VgaRotateModeType rotation)
                    EXT_TRAP(trgSetScreenMode);

/*---------------------------------------------------------------------------
 * Pass in non-expanded bitmap and destination, returns the X & Y extent 
 * the expanded bitmap.
 *--------------------------------------------------------------------------*/
extern void VgaBitmapExpandedExtent(BitmapPtr bitmapP, Coord *extentX, Coord *extentY)
                    EXT_TRAP(trgBitmapExpandedExtent);

/*---------------------------------------------------------------------------
 * Draw the bitmap at 1.5 scale.  This is needed for non 160x160 applications
 * that need to expand existing small bitmaps.  (IE Launcher)
 *--------------------------------------------------------------------------*/
extern void VgaWinDrawBitmapExpanded(BitmapPtr bitmapP, Coord x, Coord y)
                    EXT_TRAP(trgWinDrawBitmapExpanded);

/*---------------------------------------------------------------------------
 * Take the Palm font and return the 1.5 scale font
 *--------------------------------------------------------------------------*/
extern FontID VgaBaseToVgaFont(FontID font)
                    EXT_TRAP(trgBaseToVgaFont);

/*---------------------------------------------------------------------------
 * Take the Vga font and return the Palm font
 *--------------------------------------------------------------------------*/
extern FontID VgaVgaToBaseFont(FontID font)
                    EXT_TRAP(trgVgaToBaseFont);

/*---------------------------------------------------------------------------
 * Display the TRG Select Font dialog which shows the 4 Palm text fonts 
 * plus the 4 equivalent TRG larger fonts.
 *--------------------------------------------------------------------------*/
extern FontID VgaFontSelect(VgaFontSelectType selectFormType, FontID fontID)
                    EXT_TRAP(trgFontSelect);

/*---------------------------------------------------------------------------
 * Is font a 1.5 expanded font.
 *--------------------------------------------------------------------------*/
extern Boolean VgaIsVgaFont(FontID font)
                    EXT_TRAP(trgIsVgaFont);

/*---------------------------------------------------------------------------
 * Use small Palm font when drawing tableItemStyles excluding customTableItem
 * which determines its 
 *--------------------------------------------------------------------------*/
extern void VgaTableUseBaseFont(TablePtr  table, Boolean on)
                    EXT_TRAP(trgTableUseBaseFont);

/*---------------------------------------------------------------------------
 * Return the heigth of the Titlebar. Necessary for placement of form text
 * and objects
 *--------------------------------------------------------------------------*/
extern UInt16 VgaGetFrmTitleHeight(void)
                    EXT_TRAP(trgGetFrmTitleHeight);

/*---------------------------------------------------------------------------
 * VgaEnable
 *--------------------------------------------------------------------------*/
extern void VgaEnable(Boolean redraw)
                    EXT_TRAP(trgVgaEnable);

/*---------------------------------------------------------------------------
 * VgaDisable
 *--------------------------------------------------------------------------*/
extern void VgaDisable(Boolean redraw)
                    EXT_TRAP(trgVgaDisable);

/*---------------------------------------------------------------------------
 * VgaIsEnabled
 *--------------------------------------------------------------------------*/
extern Boolean VgaIsEnabled(void)
                    EXT_TRAP(trgVgaIsEnabled);

/*---------------------------------------------------------------------------
 * VgaGetLegacyMode
 *--------------------------------------------------------------------------*/
extern void VgaGetLegacyMode(VgaScreenModeType *viewMode, VgaOffsetModeType *offsetMode)
                    EXT_TRAP(trgGetLegacyMode);

/*---------------------------------------------------------------------------
 * VgaSetLegacyMode
 *--------------------------------------------------------------------------*/
extern void VgaSetLegacyMode(VgaScreenModeType viewMode, VgaOffsetModeType offsetMode)
                    EXT_TRAP(trgSetLegacyMode);

/*---------------------------------------------------------------------------
 * VgaFormModify
 *--------------------------------------------------------------------------*/
extern Err VgaFormModify(FormType *frmP, VgaFormModifyType type)
                    EXT_TRAP(trgFrmModify);

/*---------------------------------------------------------------------------
 * VgaFormModify
 *
 *  #define to allow the old VgaFrmModify routine. Please change your code to
 *  use the new VgaFormModify routine above. This #define will be removed in
 *  the future.
 *--------------------------------------------------------------------------*/
#define VgaFrmModify(frmP, type)				VgaFormModify(frmP, type)

/*---------------------------------------------------------------------------
 * VgaLoadAppPrefs
 *--------------------------------------------------------------------------*/
extern void VgaReloadAppPrefs(void)
                    EXT_TRAP(trgReloadAppPrefs);

/*---------------------------------------------------------------------------
 * VgaSaveScreenState
 *--------------------------------------------------------------------------*/
extern void VgaSaveScreenState(VgaScreenStateType *state)
                    EXT_TRAP(trgSaveScreenState);

/*---------------------------------------------------------------------------
 * VgaRestoreScreenState
 *--------------------------------------------------------------------------*/
extern Err VgaRestoreScreenState(VgaScreenStateType *state)
                    EXT_TRAP(trgRestoreScreenState);

/*---------------------------------------------------------------------------
 * VgaRestoreScreenState
 *--------------------------------------------------------------------------*/
extern Err VgaRestoreScreenState(VgaScreenStateType *state)
                    EXT_TRAP(trgRestoreScreenState);

/*---------------------------------------------------------------------------
 * VgaRotateSelect
 *--------------------------------------------------------------------------*/
extern VgaRotateModeType VgaRotateSelect (VgaRotateModeType rotateMode)
                    EXT_TRAP(trgRotateSelect);


#ifdef __cplusplus 
}
#endif


#endif  // __VGA_H__
