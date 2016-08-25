/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		SysEvtMgr.h
 *
 * Description:
 *		Header for the System Event Manager
 *
 * History:
 *   	03/22/95	RM		Created by Ron Marianetti
 *		07/23/98	kwk	Changed UInt param in EvtEnqueueKey to WChar.
 *
 *******************************************************************/

#ifndef __SYSEVTMGR_H__
#define __SYSEVTMGR_H__

#include <Common.h>

/************************************************************
 * System Event Manager Errors
 *************************************************************/
#define	evtErrParamErr			(evtErrorClass | 1)
#define	evtErrQueueFull		(evtErrorClass | 2)
#define	evtErrQueueEmpty		(evtErrorClass | 3)


/************************************************************
 * System Event Manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {  
#endif


//-----------------------------------------------------------------
// High Level Calls
//------------------------------------------------------------------
Err				EvtSysInit(void)
						SYS_TRAP(sysTrapEvtSysInit);

// Return next "System" event. This routine will send strokes to Graffiti as necessary
//  and return a key event. Otherwise, it will return a simple pen down or pen
//  up event, or put the processor to sleep for a max time of 'timeout' if 
// no events are available. 
void				EvtGetSysEvent(EventPtr eventP, Long timeout)
						SYS_TRAP(sysTrapEvtGetSysEvent);


// Return true if there is a low level system event (pen or key) available
Boolean			EvtSysEventAvail(Boolean ignorePenUps)
						SYS_TRAP(sysTrapEvtSysEventAvail);



// Translate a stroke in the silk screen area to a key event
Err				EvtProcessSoftKeyStroke(PointType* startPtP, PointType* endPtP)
						SYS_TRAP(sysTrapEvtProcessSoftKeyStroke);


//-----------------------------------------------------------------
// Pen Queue Utilties
//------------------------------------------------------------------

// Replace current pen queue with another of the given size
Err				EvtSetPenQueuePtr(Ptr penQueueP, ULong size)
						SYS_TRAP(sysTrapEvtSetPenQueuePtr);

// Return size of current pen queue in bytes
ULong				EvtPenQueueSize(void)
						SYS_TRAP(sysTrapEvtPenQueueSize);

// Flush the pen queue
Err				EvtFlushPenQueue(void)
						SYS_TRAP(sysTrapEvtFlushPenQueue);


// Append a point to the pen queue. Passing -1 for x and y means 
//  pen-up (terminate the current stroke). Called by digitizer interrupt routine
Err				EvtEnqueuePenPoint(PointType* ptP)
						SYS_TRAP(sysTrapEvtEnqueuePenPoint);


// Return the stroke info for the next stroke in the pen queue. This MUST
//  be the first call when removing a stroke from the queue
Err				EvtDequeuePenStrokeInfo(PointType* startPtP, PointType* endPtP)
						SYS_TRAP(sysTrapEvtDequeuePenStrokeInfo);

// Dequeue the next point from the pen queue. Returns non-0 if no
//  more points. The point returned will be (-1,-1) at the end
//  of the stroke.
Err				EvtDequeuePenPoint(PointType* retP)
						SYS_TRAP(sysTrapEvtDequeuePenPoint);


// Flush the entire stroke from the pen queue and dispose it
Err				EvtFlushNextPenStroke()
						SYS_TRAP(sysTrapEvtFlushNextPenStroke);




//-----------------------------------------------------------------
// Key Queue Utilties
//------------------------------------------------------------------

// Replace current key queue with another of the given size. This routine will
//  intialize the given key queue before installing it
Err				EvtSetKeyQueuePtr(Ptr keyQueueP, ULong size)
						SYS_TRAP(sysTrapEvtSetKeyQueuePtr);

// Return size of current key queue in bytes
ULong				EvtKeyQueueSize(void)
						SYS_TRAP(sysTrapEvtKeyQueueSize);

// Flush the key queue
Err				EvtFlushKeyQueue(void)
						SYS_TRAP(sysTrapEvtFlushKeyQueue);


// Append a key to the key queue. 
Err				EvtEnqueueKey(WChar ascii, UInt keycode, UInt modifiers)
						SYS_TRAP(sysTrapEvtEnqueueKey);

// Return true of key queue empty.
Boolean			EvtKeyQueueEmpty(void)
						SYS_TRAP(sysTrapEvtKeyQueueEmpty);


// Pop off the next key event from the key queue and fill in the given
//  event record structure. Returns non-zero if there aren't any keys in the
//  key queue. If peek is non-zero, key will be left in key queue.
Err				EvtDequeueKeyEvent(EventPtr eventP, Word peek)
						SYS_TRAP(sysTrapEvtDequeueKeyEvent);


//-----------------------------------------------------------------
// General Utilities
//------------------------------------------------------------------
// Force the system to wake-up. This will result in a null event being
//  sent to the current app.
Err				EvtWakeup(void)
						SYS_TRAP(sysTrapEvtWakeup);

// Reset the auto-off timer. This is called by the SerialLink Manager in order
//  so we don't auto-off while receiving data over the serial port.
Err				EvtResetAutoOffTimer(void)
						SYS_TRAP(sysTrapEvtResetAutoOffTimer);


// Set Graffiti enabled or disabled.
void	EvtEnableGraffiti(Boolean enable)
						SYS_TRAP(sysTrapEvtEnableGraffiti);

#ifdef __cplusplus
}
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_EvtEnqueuePenPoint		\
				ASM_SYS_TRAP(sysTrapEvtEnqueuePenPoint)



#endif //__SYSEVTMGR_H__



