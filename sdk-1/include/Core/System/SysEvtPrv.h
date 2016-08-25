/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SysEvtPrv.h
 *
 * Description:
 *		Private Header for the System Event Manager
 *
 * History:
 *   	8/10/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE
#ifndef __SYSEVTPRV_H__
#define __SYSEVTPRV_H__




/************************************************************
 * Structure of a Pen Capture Queue
 *
 * This structure is used to hold one or more pen strokes
 *   and is filled by the digitizer interrupt routine and emptied
 *   by application level code.
 *
 * A default size queue is allocated by the system and used to capture
 *   strokes for subsequent processing by the Graffiti recognizer
 *   or the hard icon recognizer. This queue pointer is stored in
 *   the System Event Manager Globals.
 *
 * Optionally, applications can replace the system pen queue with
 *   one of their own if it they need to hold larger strokes
 *   or more of them.
 *   
 *
 * Stroke data is stored in compressed form in this queue - refer
 *  to the comments on the routine EvtEnqueuPenPoint  for the format
 *  of the queue data.
 *************************************************************/
typedef struct PenQueueType {
	Int			start;							// start byte offset
	Int			end;								// end byte offset
	Int			size;								// size of queue in bytes
	
	UInt			strokeCount;					// # of complete strokes currently in pen queue
	Boolean		returnedPenDown;				// set after we return a pen-down event for 
														// the next stroke in the queue. Cleared after
														// we return the pen-up for the stroke.

	PointType	addLast;							// last point captured
	Int			addStrokeStart;				// offset in queue to start of stroke
														// currently being added to queue
														// -1  : no stroke being captured
	
	
	UInt			strokesRemoved;				// running count: # of strokes removed from queue.
	UInt			strokesAdded;					// running count: # of strokes added to queue.

	Int			rmvStrokeStage;				// -1  : not started
														//  0  : returned stroke info
														//  1  : returned start point
														
	PointType	rmvLast;							// last point dequeued
	PointType 	rmvStartPt;						// copy of start point of stroke currently
														//  being removed.
	
	Byte			data[1];							// 'size' bytes of data
	} PenQueueType;
typedef PenQueueType*	PenQueuePtr;



/************************************************************
 * Structure of the Key Queue
 *
 * This structure is used to hold one or more keystrokes 
 *   and is filled by numerous places including the low level
 *	  hardware key interrupt routine, the Graffiti recognizer, and
 *	  the digitizer buttons recognizer.
 *
 * A default size queue is allocated by the system. 
 *		This queue pointer is stored in the System Event Manager globals.
 *
 * Optionally, applications can replace the system key queue with
 *   one of their own if it they need to hold more keys in the queue
 *	  at one time. This may be necessary when processing large macro
 *	  strings from Graffiti.
 *
 * Key data is stored in compressed form in this queue. Refer to
 *  the comments for the function EvtEnqueueKeyString for the format
 *  of the compressed data.
 *   
 *************************************************************/
typedef struct KeyQueueType {
	UInt		start;							// start byte offset
	UInt		end;								// end byte offset
	UInt		size;								// size of queue in bytes
	
	Byte		data[1];							// 'size' bytes of data
	} KeyQueueType;
typedef KeyQueueType*	KeyQueuePtr;





/************************************************************
 * Pen button info structure. This structure is used
 *  to hold the bounds of each button on the silk screen and
 *  the ascii code and modifiers byte that each will generate.
 *************************************************************/
typedef struct PenBtnInfoType {
	RectangleType	boundsR;						// bounding rectangle of button
	UInt				asciiCode;					// ascii code for key event
	UInt				keyCode;						// virtual key code for key event
	UInt				modifiers;					// modifiers for key event
	} PenBtnInfoType;
typedef PenBtnInfoType* PenBtnInfoPtr;
	
	
	

/************************************************************
 * Key constants
 *************************************************************/
#define 	evtKeyStringEscape 	0x01		// Escape byte for encoded key strings
													// passed to EvtEnqueueKeyString or stored
													//  in key queue.


#define	evtDefaultPenQSize	0x100
#define	evtDefaultKeyQSize	0x40

#define	evtEffectiveKeyQStorage	(evtDefaultKeyQSize - sizeof(KeyQueueType))


// gremlinsFlags
#define	grmGremlinsOn			0x01
#define  grmEventTraceOn		0x02
#define	grmGremlinsIdle		0x04

/************************************************************
 * Structure of System Event Manager Globals
 *************************************************************/
typedef struct SysEvtMgrGlobalsType {
	Boolean			sendNullEvent;				// set by EvtWakeup 		
	Boolean			enableGraffiti;			// If true, enable Graffiti translation
	Boolean			enableSoftKeys;			// If true, enable digitizer button translation 
												
												
	Boolean			removeTopStroke;			// if true, remove top stroke from queue
														//  next time EvtGetSysEvent is called;
	PenQueuePtr		penQP;						// pointer to current pen queue
	UInt				penQStrokesRemoved;		// saved value of penQP->strokesRemoved. Used to
														// determine if app removed stroke from penUp
														// event or not.
														
	KeyQueuePtr		keyQP;						// pointer to current key queue
	
	RectangleType	writingR;					// enclosing rect for writing area
	UInt				appAreaBottom;				// bottom coordinate of application area.
	
	UInt				penX;							// current pen location and state
	UInt				penY;				
	Boolean			penDown;
	Boolean			displayingBatteryAlert; // to avoid re-entering battery dialog
	
	UInt				lastPenX;					// last pen location and state
	UInt				lastPenY;
	Boolean			lastPenDown;
	
	// This boolean is set by the Remote Screen Driver timer procedure periodically
	//  to force the event manager to call ScrSendUpdateArea which will send the update
	//  area of the screen to the host.
	Boolean			needRemoteScrUpdate;		// If true, call ScrSendUpdateArea 
	
	
	Long				smID;							// Semaphore set by interrupt routines
														//  whenever a low level event is detected

	Byte				gremlinsFlags;				// flag to indicate if gremlins is running and
														// if we'er doing an event trace.

	Boolean			idle;
															
	} SysEvtMgrGlobalsType;
	
typedef SysEvtMgrGlobalsType* SysEvtMgrGlobalsPtr;




/************************************************************
 * System Event Manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {  
#endif



// Return pointer to the pen based button list
PenBtnInfoPtr	EvtGetPenBtnList(UIntPtr numButtons)
						SYS_TRAP(sysTrapEvtGetPenBtnList);

#ifdef __cplusplus
}
#endif


#endif //__SYSEVTPRV_H__

#endif // NON_PORTABLE

