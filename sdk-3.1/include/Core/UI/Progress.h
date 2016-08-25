/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot Net Library PPP Network Interface
 * FILE:     Progress.h
 * AUTHOR:	 Gavin Peacock 6/4/97 from Ron Marianetti's net dialog stuff
 *
 * DESCRIPTION:
 *	  This header file defines a generic progress dialog interface
 *
 **********************************************************************/

#ifndef	__PROGRESS_H__
#define	__PROGRESS_H__

#include <SysTraps.h>


#define progressMaxMessage	128
#define progressMaxTitle	20   // max size for title of progress dialog

// Progress callback function
// The progress dialog will call this function to get the text to display for the
// current status. 
// stage - the current stage of progess as defined by your app
// message - text that can be sent from the protocol 
// cancel - true if the dialog is in cancel mode
// error - current error (func should return an error message in this case...
typedef struct
{
	Word stage;					// <= current stage
	CharPtr textP;				// => buffer to hold text to display
	Word textLen;				// <= length of text buffer
	CharPtr message;			// <= additional text for display
	Err error;					// <= current error
	Word bitmapId;				// => resource ID of bitmap to display
	Word canceled:1;			// <= true if user has pressed the cancel button			
	Word showDetails:1;			// <= true if user pressed down arrow for more details
	Word textChanged:1;			// => if true then update text (defaults to true)
	Word timedOut:1;			// <= true if update caused by a timeout
	ULong timeout;				// <> timeout in ticks to force next update (for animation)
	
	//progress bar info (Not Implemented)
	DWord barMaxValue;			// the maximum value for the progress bar, if = 0 then the bar is
								// not visible
	DWord barCurValue;			// the current value of the progress bar, the bar will be drawn 
								// filled the percentage of maxValue \ value
	CharPtr barMessage;			// additional text for display below the progress bar.
	Word	barFlags;			// reserved for future use.
} PrgCallbackData, *PrgCallbackDataPtr;

//typedef Boolean (*PrgCallbackFunc)  (Word stage,Boolean showDetails,CharPtr message,Boolean cancel,Word error,CharPtr textP, Word maxtextLen,Word *bitmapID);
typedef Boolean (*PrgCallbackFunc)  (PrgCallbackDataPtr cbP);



//---------------------------------------------------------------------------
// Structure of the Progress Info structure. This structure should be stored
//  in the interface's globals. Each of the routines in SerNetIFCommon.c 
//  take a pointer to this structure. 
//---------------------------------------------------------------------------


typedef struct {

	// This field contains a pointer to the open progress dialog
	FormPtr				frmP;						// Our progress dialog ptr
	
	// This field is set a maximum time for the action to take place. A cancel
	// will be generated if this timeout is reached
	ULong				timeout;					// max time to wait in ticks
	
	
	// This boolean is set by either the protocol (through PrgUpdateDialog()) or UI 
	//  task to inform the UI that it needs to update it's progress dialog with new 
	//  information as stored in the error, stage, and message fields. 
	Word					needUpdate:1;			// true if UI update required.


	// The following boolean is set by the UI task when the user hits the cancel button.
	// When the user cancels, the UI changes to display "Cancelling..." and then waits
	// for the protocol task to notice the user cancel and set the error field to
	//  netErrUserCancel before disposing the dialog. The SerIFUserCancel() which is
	//  called from the protocol task checks this boolean.
	Word					cancel:1;				// true if cancelling 
	
	
	// This boolean is set by PrvCheckEvents() after we've displayed an error message
	//  in the progress dialog and changed the "cancel" button to an "OK" button. 
	//  This tells the dialog event handling code in PrvCheckEvents() that it should
	//  dispose of the dialog on the next hit of the cancel/OK button.
	Word					waitingForOK:1;		// true if waiting for OK button hit.


	// This boolean gets set if the user hits the down button while the UI is up. It
	//  causes more detailed progress to be shown
	Word					showDetails:1;			// show progress details. 
	
	// This is set to true whenever the message text is changed. This allows the
	// display to be more efficient by not redrawing when not needed
	Word    messageChanged: 1;
	
	
	//-----------------------------------------------------------------------
	// The following fields are set by PrgUpdateDialog() and used by PrgHandleEvent()
	//  to figure out what to display in the progress dialog
	//-----------------------------------------------------------------------
	
	// This word is set by the protocol task (through PrgUpdateDialog()) when an 
	//  error occurs during connection establishment. If this error is non-nil 
	//  and not equal to netErrUserCancel, the UI task will display the appropriate
	//  error message and change the cancel button to an OK button, set the waitingForOK
	//  boolean and wait for the user to  hit the OK button before disposing 
	//  the dialog. 
	Word					error;					// error set by interface

	// This enum is set by the protocol task (through PrgUpdateDialog()) as it 
	//  progresses through the  connection establishment and is checked by 
	//  PrgHandleEvent() when needUpate is true. It is used to determine what 
	//  string to display in the progress dialog.
	Word	stage;					// which stage of the connection we're in


	// This is an additional string that is displayed in the progress dialog for
	//  certain stages. The netConStageSending stage for example uses this string
	//  for holding the text string that it is sending. It is set by
	//  PrgUpdateDialog().
	Char					message[progressMaxMessage+1];	// connection stage message.
	
	// Used to cache current icon number so we don't unnecessarily redraw it
	Word					lastBitmapID;
	
	// Text array used to hold control title for the OK/Cancel button. This
	//  must be kept around while the control is present in case of updates.
	Char					ctlLabel[8];
	
	CharPtr 				serviceNameP;

	//progress bar stuff (Sot implemented)
	DWord lastBarMaxValue;
	DWord lastBarCurValue;
	
	// stuff for saving old window state
	WinHandle 		oldDrawWinH;
	WinHandle 		oldActiveWinH;
	FormPtr			oldFrmP;
	Boolean 		oldInsPtState;
	PointType 		oldInsPtPos;

	PrgCallbackFunc textCallback;

	char 			title[progressMaxTitle+1];
	} ProgressType, *ProgressPtr;






// macro to test if the user has canceled
#define PrgUserCancel(prgP) (prgP)->cancel

//-----------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------
ProgressPtr	PrgStartDialogV10(CharPtr title,PrgCallbackFunc textCallback)
		SYS_TRAP(sysTrapPrgStartDialogV10);

void PrgStopDialog(ProgressPtr prgP,Boolean force)
		SYS_TRAP(sysTrapPrgStopDialog);

void	PrgUpdateDialog(ProgressPtr prgGP, Word err, Word stage,
					CharPtr	messageP,Boolean updateNow)
		SYS_TRAP(sysTrapPrgUpdateDialog);

Boolean	PrgHandleEvent(ProgressPtr prgGP,EventPtr eventP)
		SYS_TRAP(sysTrapPrgHandleEvent);

#endif //__PROGRESS_H__






