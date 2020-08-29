/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Common.h
 *
 * Description:
 *	Useful functions
 *
 * Author:
 *	September 24, 2002	Created by Nicolas Pabion
 *
 ***********************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

/***********************************************************************
 *
 *	Defines
 *
 ***********************************************************************/

#define FrmGetPtr(formP, id) FrmGetObjectPtr(formP, FrmGetObjectIndex(formP, id))
#define FrmHideID(formP, id) FrmHideObject(formP, FrmGetObjectIndex(formP, id))
#define FrmShowID(formP, id) FrmShowObject(formP, FrmGetObjectIndex(formP, id))
#define FrmGetPosition(formP, id, xP, yP) \
 FrmGetObjectPosition(formP, FrmGetObjectIndex(formP, id), xP, yP);
#define FrmGetBounds(formP, id, boundsP) \
 FrmGetObjectBounds(formP, FrmGetObjectIndex(formP, id), boundsP);
#define FrmSetBounds(formP, id, boundsP) \
 FrmSetObjectBounds(formP, FrmGetObjectIndex(formP, id), boundsP);

/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err SetFieldTextFromStr(FieldPtr field, Char *s, Boolean redraw);

#ifdef __cplusplus 
}
#endif
 
#endif COMMON_H_