/******************************************************************************
 *
 * Copyright (c) 1997-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: FontSelect.h
 *
 * Description:
 *	  This file defines the font selector routine.
 *
 * History:
 *		September 10, 1997	Created by Art Lamb
 *
 *****************************************************************************/

#ifndef	__FONTSELECT_H__
#define	__FONTSELECT_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <Font.h>

#ifdef __cplusplus
extern "C" {
#endif

extern FontID FontSelect (FontID fontID)
						SYS_TRAP(sysTrapFontSelect);


#ifdef __cplusplus 
}
#endif

#endif // __FONTSELECT_H__

