/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Common.h
 *
 * Description:
 *	Some useful macros and functions
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
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
 
#define MisalignedReadBEUInt16(pointer, offset)											\
			(((UInt16)(*(((UInt8 *)(pointer)) + offset + sizeof(UInt8)))) |				\
			 (((UInt16)(*(((UInt8 *)(pointer)) + offset))) << 8))

#define MisalignedReadBEUInt32(pointer, offset)															\
			(((UInt32)(MisalignedReadBEUInt16((((UInt8 *)(pointer)) + sizeof(UInt16)), offset))) |		\
			 (((UInt32)(MisalignedReadBEUInt16(pointer, offset))) << 16))

#define Swap16(n)	(((((UInt16) n) << 8) & 0xFF00) | \
					 ((((UInt16) n) >> 8) & 0x00FF))
					 

#define Swap32(n)	(((((UInt32) n) << 24) & 0xFF000000) | \
					 ((((UInt32) n) <<  8) & 0x00FF0000) | \
					 ((((UInt32) n) >>  8) & 0x0000FF00) | \
					 ((((UInt32) n) >>  24) & 0x000000FF))


/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err SetFieldTextFromStr(FieldType *fieldP, Char *string, Boolean redraw);

Boolean	PalmSGIsTungstenT();
Boolean PalmSGHasSampledSound();
Boolean PalmSGHasExtendedSampledSound();

#ifdef __cplusplus 
}
#endif
 
#endif COMMON_H_