/***********************************************************************
 *
 * Copyright (c) 2001 TRG, All Rights Reserved
 *
 * FileName:
 *		TRG.h
 *
 * Description:
 *
 *
 * History:
 *      7-10-00  -- original version
 *
 **********************************************************************/
#ifndef _TRG_H_
#define _TRG_H_

#include <PalmTypes.h>
#include <CoreTraps.h>

#define TRGSysFtrID             'TRG '

#ifdef __GNUC__

#define _TRG_CALL_WITH_16BIT_SELECTOR(table, vector, selector)\
        __attribute__ ((__callseq__ (\
            "move.w #" _Str(selector) ",%%d2; "\
            "trap #" _Str(table) "; dc.w " _Str(vector) )))

#elif defined (__MWERKS__)	/* The equivalent in CodeWarrior syntax */

#define _TRG_CALL_WITH_16BIT_SELECTOR(table, vector, selector) \
    = { 0x343C, selector, 0x4E40 + table, vector }

#endif

#define TRG_TRAP(sel) \
        _TRG_CALL_WITH_16BIT_SELECTOR(_SYSTEM_TABLE, sysTrapOEMDispatch, sel)

#ifdef BUILDING_EXTENSION
    #define EXT_TRAP(x)
#else
    #define EXT_TRAP(x) TRG_TRAP(x)
#endif


#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
	}
#endif


#endif
