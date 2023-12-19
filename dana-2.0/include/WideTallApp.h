/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            WideTallApp.h
 *
 * DESCRIPTION: This file contains the trap interface for Wide Tall Applications.
 *
 ****************************************************************************/
#ifndef _OEMTYPES_H_
#define _OEMTYPES_H_

#include <PalmTypes.h>
#include <CoreTraps.h>

#define ON			1
#define OFF			0

#define AlphaSmartSysFtrID             'WTAP'

#ifdef __GNUC__

#define _AS_CALL_WITH_16BIT_SELECTOR(table, vector, selector)\
        __attribute__ ((__callseq__ (\
            "move.w #" _Str(selector) ",%%d2; "\
            "trap #" _Str(table) "; dc.w " _Str(vector) )))

#elif defined (__MWERKS__)	/* The equivalent in CodeWarrior syntax */

#define _AS_CALL_WITH_16BIT_SELECTOR(table, vector, selector) \
    = { 0x343C, selector, 0x4E40 + table, vector }

#endif

#define AS_TRAP(sel) \
        _AS_CALL_WITH_16BIT_SELECTOR(_SYSTEM_TABLE, sysTrapOEMDispatch, sel)

#ifdef BUILDING_EXTENSION
    #define EXTERNAL_TRAP(x)
#else
    #define EXTERNAL_TRAP(x) AS_TRAP(x)
#endif


#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
	}
#endif


#endif
