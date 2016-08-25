/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved  
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

/********************************************************************
 * Pen Manager Globals
 ********************************************************************/
typedef struct PenGlobalsType {
	UInt				xOffset, yOffset;
	UInt				xScale, yScale;
	
	// This variable is incremented every time we detect the pen up,
	//  until we've seen it enough times to believe it.
	UInt				penRising;
	
	// We keep this information around in order to throw out invalid
	//  points. Invalid points are most likey to occur as the pen is lifted
	//  and we filter them by throwing out points that show a drastic
	//  change in velocity in either the x or y direction
	Int				lastX;			// x of last point (-1 if no previous point)
	Int				lastY;			// y of last point
	Int				lastDX;			// delta y of previous 2 points
	Int				lastDY;			// delta x of previous 2 points
	
	
	// Parameters for filter
	UInt				delay;			// delay before reading A/D
	UInt				maxDDZ;			// max 2nd derivative.
	
	} PenGlobalsType;
	
typedef PenGlobalsType*	PenGlobalsPtr;



#endif	//__PEN_MGR_H
#endif 	// NON_PORTABLE
