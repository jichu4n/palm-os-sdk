/*
 * GoLCDTest.h
 *
 * header file for GoLCDTest
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 2003 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef GOLCDTEST_H_
#define GOLCDTEST_H_

/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'goTs'
#define appName					"GoLCDTest"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

#define appFeatureNum			0		

/************************************************************
 * Structures
 *************************************************************/
 
typedef enum {
	OFF,
	ENABLED,
	DISABLED
} GoLcdState;

typedef struct
{
	UInt32			timeout;
	Boolean 		inverted;
	RGBColorType	inkColor;
	RGBColorType	gsiColor;
	
} GadgetInfo;

extern GadgetInfo gGadgetInfo;		

#endif // GOLCDTEST_H_
