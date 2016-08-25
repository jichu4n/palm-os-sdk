/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: UIControls.h
 *
 * Description:
 *             	Contrast & brightness control for devices with
 *						software contrast.
 *
 * History:
 *			Name	Date		Description
 *			----	----		-----------
 *			bob	02/12/98	Initial version
 *			bob	03/15/99	Added brightness
 *			bob	08/27/99	Added UIPickColor, renamed UIControls.h
 *
 *****************************************************************************/

#ifndef	__UICONTROLS_H__
#define	__UICONTROLS_H__

#include <CoreTraps.h>

// for UIPickColor
#define UIPickColorStartPalette	0
#define UIPickColorStartRGB		1

typedef UInt16 UIPickColorStartType;



#ifdef __cplusplus
extern "C" {
#endif

extern void UIContrastAdjust()
		SYS_TRAP(sysTrapUIContrastAdjust);

extern void UIBrightnessAdjust()
		SYS_TRAP(sysTrapUIBrightnessAdjust);

Boolean UIPickColor(IndexedColorType *indexP, RGBColorType *rgbP,
					     UIPickColorStartType start, const Char *titleP,
					     const Char *tipP)
		SYS_TRAP(sysTrapUIPickColor);


#ifdef __cplusplus 
}
#endif

#endif	// __UICONTROLS_H__

