/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: GraffitiReference.h
 *
 * Description:
 *	  This file defines the Graffiti Reference routines.
 *
 * History:
 *		June 25, 1996	Created by Roger Flores
 *		06/25/96	rsf	Created by Roger Flores
 *		07/30/99	kwk	Moved all reference types other than referenceDefault
 *							into GraffitiReference.c
 *
 *****************************************************************************/

#ifndef __GRAFFITIREFERENCE_H__
#define __GRAFFITIREFERENCE_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

typedef enum 
	{
	referenceDefault = 0xff		// based on graffiti mode
	} ReferenceType;

/************************************************************
 * Graffiti Reference procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


extern void SysGraffitiReferenceDialog (ReferenceType referenceType)
							SYS_TRAP(sysTrapSysGraffitiReferenceDialog);


#ifdef __cplusplus
}
#endif

#endif // __GRAFFITIREFERENCE_H__

