/******************************************************************************
 * Copyright (c) 2003-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailFontApp
 */
 
/**
 *  @brief 	Sample code for the VersaMail Font Manager API
 *  		
 *
 *
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Usha Shekar (3/15/07) - Added "About" box, 5-way navigation
 *					  and Doxygen comments. Compiles with PODS and GCC.
 *
 *  @file utils.h
 */


#ifndef UTILS_H
#define UTILS_H

void SetFieldText(FieldType *fieldP,Char *textP,Boolean blDraw);
void UpdateListUI(UInt16 formID, UInt16 popupID, UInt16 listID);

#endif