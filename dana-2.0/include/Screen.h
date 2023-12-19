/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            Screen.h
 *
 * DESCRIPTION: This file contains the API to the screen.
 *
 ****************************************************************************/

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "WideTallApp.h"

#define ScrnFtrNum            2

/*--------------------------------------------------------------------------
 * Screen result codes
 * (oemErrorClass is reserved for Palm licensees).
 *-------------------------------------------------------------------------*/
#define scrnErrorClass               (oemErrorClass | 0x200)

#define scrnErrUnimplemented         (scrnErrorClass | 0)    // function not implemented (on this hardware)
#define scrnErrBadParam              (scrnErrorClass | 1)    // invalid parameter
#define scrnErrModeUnsupported       (scrnErrorClass | 2)    // does not support screen mode
#define scrnErrScreenLocked          (scrnErrorClass | 3)    // could not lock the screen

/*---------------------------------------------------------------------------
 * Extension Installed
 *--------------------------------------------------------------------------*/
#define _ScreenFeaturePresent(versionPtr) \
        (Boolean)(FtrGet(AlphaSmartSysFtrID, ScrnFtrNum, versionPtr) == 0)

/*---------------------------------------------------------------------------
 * Rotation modes
 *--------------------------------------------------------------------------*/
typedef enum
{
    rotateScrnMode0 = 0,
    rotateScrnMode90, 
    rotateScrnMode270 
} ScrnRotateModeType;


/*---------------------------------------------------------------------------
 * Screen modes
 *--------------------------------------------------------------------------*/
typedef enum
{
    screenModeWideTall = 0,
    screenModeCentered      
} ScrnScreenModeType;

/*---------------------------------------------------------------------------
 * Screen System State
 *--------------------------------------------------------------------------*/
typedef struct ScrnSystemStateType
{
    ScrnScreenModeType  screenMode;				// current state of screen ext.
	ScrnScreenModeType	curAppScreenMode;		// current state of app 
    Int16               offset_X;
    Int16               offset_Y;
    UInt8               draw_override;			// > 0, no offsetting

    UInt8               reserved[8];			// not used
} ScrnSystemStateType;



#ifdef __cplusplus
extern "C" {
#endif

#define     scrnMinFunction          0x0200
#define     scrnGetRotateMode        (scrnMinFunction +  0)
#define     scrnSetRotateMode        (scrnMinFunction +  1)

#define     scrnEnable               (scrnMinFunction +  2)
#define     scrnDisable              (scrnMinFunction +  3)
#define     scrnIsEnabled            (scrnMinFunction +  4)

#define     scrnTableSetFontID       (scrnMinFunction +  5)
#define     scrnGetSystemState       (scrnMinFunction +  6)

/********************************************************************
 *              SCREEN API Prototypes
 ********************************************************************/

/*---------------------------------------------------------------------------
 * ScrnGetRotateMode - Returns the current rotation settings
 *--------------------------------------------------------------------------*/
extern void ScrnGetRotateMode(ScrnRotateModeType *rotation)
                    EXTERNAL_TRAP(scrnGetRotateMode);

/*---------------------------------------------------------------------------
 * ScrnSetRotateMode - Set current screen mode.             
 *--------------------------------------------------------------------------*/
extern Err ScrnSetRotateMode(ScrnRotateModeType rotation)
                    EXTERNAL_TRAP(scrnSetRotateMode);

/*---------------------------------------------------------------------------
 * ScrnTableSetFontID - Use small Palm font when drawing tableItemStyles 
 * excluding customTableItem which determines its.
 *--------------------------------------------------------------------------*/
extern void ScrnTableSetFontID(TablePtr  table, FontID fontID)
                    EXTERNAL_TRAP(scrnTableSetFontID);

/*---------------------------------------------------------------------------
 * ScrnEnable
 *--------------------------------------------------------------------------*/
extern void ScrnEnable(void)
                    EXTERNAL_TRAP(scrnEnable);

/*---------------------------------------------------------------------------
 * ScrnDisable
 *--------------------------------------------------------------------------*/
extern void ScrnDisable(void)
                    EXTERNAL_TRAP(scrnDisable);

/*---------------------------------------------------------------------------
 * ScrnIsEnabled
 *--------------------------------------------------------------------------*/
extern Boolean ScrnIsEnabled(void)
                    EXTERNAL_TRAP(scrnIsEnabled);

/*---------------------------------------------------------------------------
 * ScrnGetSystemState
 *--------------------------------------------------------------------------*/
extern void ScrnGetSystemState(ScrnSystemStateType *pState)
                    EXTERNAL_TRAP(scrnGetSystemState);


#ifdef __cplusplus 
}
#endif


#endif  // __SCREEN_H__
