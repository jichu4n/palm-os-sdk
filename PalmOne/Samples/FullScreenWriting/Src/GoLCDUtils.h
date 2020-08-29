/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	GoLCD.h
 *
 * Description:
 *	Utility functions for GoLCD.
 *
 * Version 1.0 	- Initial Revision (03/12/03)
 *
 ***********************************************************************/

#ifndef GOLCDUTILS_H_
#define GOLCDUTILS_H_

#include "PalmGoLCD.h"

/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err		LoadGoLCDLib(UInt16 *refNumP);
Err		UnloadGoLCDLib(UInt16 refNum);

#ifdef __cplusplus 
}
#endif
 
#endif // GOLCDUTILS_H_