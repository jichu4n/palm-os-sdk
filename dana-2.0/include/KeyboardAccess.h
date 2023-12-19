/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            KeyboardAccess.h
 *
 * DESCRIPTION: This file contains the API to the keyboard related functions.
 *
 ****************************************************************************/
#ifndef KEYBOARDACCESS_H
#define KEYBOARDACCESS_H

#include "WideTallApp.h"
#include "WideTallAppChars.h"

#define WTAPKeyboardAccessFtrNum           10

/*---------------------------------------------------------------------------
 * Check whether US or ISO keyboard is on the Dana
 *--------------------------------------------------------------------------*/
typedef enum
{
    kybdTypeUS = 1,	
    kybdTypeISO
} KeyboardKeyType;

#define KeyboardTypeFtrNum					14

#define _KybdType(keyboardType) \
        (Boolean)(FtrGet(AlphaSmartSysFtrID, KeyboardTypeFtrNum, keyboardType) == 0)


//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
typedef enum
{
    kybdLayoutQwerty = 0,	
    kybdLayoutDvorak,
    kybdLayoutLeft,       
    kybdLayoutRight
} KeyboardLayoutType;

/*---------------------------------------------------------------------------
 * Extension Installed
 *--------------------------------------------------------------------------*/
#define _KybdFeaturePresent(version) \
        (Boolean)(FtrGet(AlphaSmartSysFtrID, WTAPKeyboardAccessFtrNum, version) == 0)

/*---------------------------------------------------------------------------
 * KeyboardAccess API result codes
 *--------------------------------------------------------------------------*/
#define kybdErrorClass               (oemErrorClass  | 0x800)
#define kybdErrBadParam              (kybdErrorClass | 0)    // invalid parameter


#ifdef __cplusplus
extern "C" {
#endif

#define		kybdMinFunction         		0x0800

#define     kybdGetLayout					(kybdMinFunction +  0)
#define     kybdGetModifiers				(kybdMinFunction +  1)
#define     kybdGetKeyboardEmulation		(kybdMinFunction +  2)
#define     kybdSetKeyboardEmulation		(kybdMinFunction +  3)

#define     kybdLastFunction				(kybdSetKeyboardEmulation)    

/********************************************************************
 *              Keyboard Access API Prototypes
 ********************************************************************/

/*---------------------------------------------------------------------------
 * Get the current layout of the keyboard.
 * Note: All non US versions will return kybdLayoutQwerty.
 *--------------------------------------------------------------------------*/
extern KeyboardLayoutType KybdGetLayout(void)
                    EXTERNAL_TRAP(kybdGetLayout);

/*---------------------------------------------------------------------------
 * Get the current state of the keyboard modifiers.
 * Caps lock, Right shift, Left shift, Right shift, Option/Alt/Function, Cmd
 * Upper 8 bits of the event represent the modifiers.
 * Look for the bit definitions in WideTallAppChars.h such as:
 * KEYBOARD_MODIFIER_COMMAND 
 *--------------------------------------------------------------------------*/
extern KeyboardEvent KybdGetModifiers(void)
                    EXTERNAL_TRAP(kybdGetModifiers);

/*---------------------------------------------------------------------------
 * Get the current state of keyboard enable.
 * For system use only.
 *--------------------------------------------------------------------------*/
extern Boolean KybdGetKeyboardEmulation(void)
                    EXTERNAL_TRAP(kybdGetKeyboardEmulation);

/*---------------------------------------------------------------------------
 * Set the keyboard enable value.
 * For system use only.
 *--------------------------------------------------------------------------*/
extern void KybdSetKeyboardEmulation(Boolean value)
                    EXTERNAL_TRAP(kybdSetKeyboardEmulation);


#ifdef __cplusplus 
}
#endif


#endif  // KEYBOARDACCESS_H
