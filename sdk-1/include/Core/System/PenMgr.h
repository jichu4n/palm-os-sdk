/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		PenMgr.h
 *
 * Description:
 *		Include file for Pen manager
 *
 * History:
 *   	6/5/96 Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __PEN_MGR_H
#define __PEN_MGR_H

// Pilot common definitions
#include <Common.h>
#include <SystemMgr.h>


/********************************************************************
 * Pen Manager Errors
 * the constant serErrorClass is defined in SystemMgr.h
 ********************************************************************/
#define	penErrBadParam				(penErrorClass | 1)



/********************************************************************
 * Pen manager Routines
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


// Initializes the Pen Manager
Err	PenOpen(void)
				SYS_TRAP(sysTrapPenOpen);

// Closes the Pen Manager and frees whatever memory it allocated
Err 	PenClose(void)
				SYS_TRAP(sysTrapPenClose);
				

// Put pen to sleep
Err	PenSleep(void)
				SYS_TRAP(sysTrapPenSleep);

// Wake pen
Err	PenWake(void)
				SYS_TRAP(sysTrapPenWake);
				

// Get the raw pen coordinates from the hardware. 
Err 	PenGetRawPen(PointType* penP)
				SYS_TRAP(sysTrapPenGetRawPen);
				
// Reset calibration in preparation for setting it again
Err	PenResetCalibration (void)
				SYS_TRAP(sysTrapPenResetCalibration);

// Set calibration settings for the pen
Err	PenCalibrate (PointType* digTopLeftP, PointType* digBotRightP,
					PointType* scrTopLeftP, PointType* scrBotRightP)
				SYS_TRAP(sysTrapPenCalibrate);

// Scale a raw pen coordinate into screen coordinates
Err 	PenRawToScreen(PointType* penP)
				SYS_TRAP(sysTrapPenRawToScreen);
				
// Scale a screen pen coordinate back into a raw coordinate
Err 	PenScreenToRaw(PointType* penP)
				SYS_TRAP(sysTrapPenScreenToRaw);
				

#ifdef __cplusplus
}
#endif

	
/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_PenGetRawPen		\
				ASM_SYS_TRAP(sysTrapPenGetRawPen)



#endif	//__PEN_MGR_H
