/******************************************************************************
 * Copyright (c) 2004-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailAccConfig
 */
 
/**
 *  @brief 	Sample code for the VersaMail Setup Applications
 *          
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Added "About" box and 5-way nav support (02/20/07),compiles under PODS and GCC
 *
 *  @file utils.h
 */
 
#ifndef UTILS_H
#define UTILS_H

#include "VMAccConfig.h"

void 	SetFieldText(FieldType *fieldP,Char *textP,Boolean blDraw);
Boolean FindMMDeviceDB(LocalID *localIDP, Err *errP);
void 	ParseMMDeviceValues(LocalID localID, UInt16 accSlot, Boolean deleteFlag);
void 	FillInFields(UInt8 displayFlag);
Boolean UpdateValuesList(UInt8 displayFlag);
void 	UpdateMMDeviceDB(UInt16 currentAccSlot, LocalID localID);
void 	ExitApp();

#define	NUM_KEYS	17				   
#define	FIRSTPAGE	0x01
#define	SECONDPAGE	0x02
#define	THIRDPAGE	0x03
#define	FOURTHPAGE	0x04

				  
#endif