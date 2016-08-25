/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		PenPrv.h
 *
 * Description:
 *		Private Include file for Pen manager
 *
 * History:
 *   	6/5/96 Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE
#ifndef __PEN_PRV_H
#define __PEN_PRV_H


/********************************************************************
 * Pen Manager Constants
 ********************************************************************/
#define	penCalibrateTop			10
#define	penCalibrateLeft			10
#define	penCalibrateBottom		(hwrDisplayHeight-10)
#define	penCalibrateRight			(hwrDisplayWidth-10)

#define	penInitDelay				30
#define	penInitMaxDDZ				12			// max change allowed in dx or dy between
														//  2 samples.

// We sample the pen 2 times in a row every 20ms and make sure the 2 points are
// within this many pixels. 
#define	penInitMaxSlop				2	

// The max blip we allow on the last point of a stroke as compared to the
//  previous point
#define	penInitMaxEndBlip			5

// Number of samples we keep around for filtering functions.
// Must allow room for at least 2 samples previous to the hot sample
// (i.e. must be >= penHotSample+3
#define	penNumSamples				5

// Index of the sample we return from PenGetRawPen. Must be less than
//  penNumSamples!!
#define	penHotSample				2

// Number of pen-up samples in a row we must see in order to recognize a pen-up.
// Must be <= penHotSample+1 since we must have space in the sample array
// before and including the hot sample to store all these up samples.
#define	penMinUpSamples			3


// This constant is stored in the x field of a point within our sample
//  array to indicate a pen down but with no valid position.
#define	penDownButInvalid			0x7FFF



/********************************************************************
 * Pen Manager Globals
 ********************************************************************/
typedef struct PenGlobalsType {
	// Calibration info
	Boolean			calibrated;
	Boolean			filler;
	UInt				xOffset, yOffset;
	UInt				xScale, yScale;
	
	// Array of pen samples we've taken. The most recent sample is at index #0.
	// We keep this history of samples in order to filter out bad points. 
	// The sample that we return to the caller of PenGetRawPen is stored
	// at index pen
	PointType		sample[penNumSamples];
	
	// Parameters for filter
	UInt				delay;			// delay before reading A/D
	UInt				maxDDZ;			// max 2nd derivative.
	UInt				maxSlop;			// max delta between 2 samples taken immediately
											//  after each other
	UInt				maxEndBlip;		// max blip we allow at the end of a stroke
	} PenGlobalsType;
	
typedef PenGlobalsType*	PenGlobalsPtr;



#endif	//__PEN_MGR_H
#endif 	// NON_PORTABLE
