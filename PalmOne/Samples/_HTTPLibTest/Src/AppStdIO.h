/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Any Pilot app that wants a stdio window
 * FILE:     AppStdIO.h
 * AUTHOR:	 Ron Marianetti 8/20/96
 *
 * DESCRIPTION:
 *	  This module implements stdin/stdout support for a pilot application.
 * It requires that the application provide a form with a text field and
 * up and down repeating buttons. 
 *
 *   To use this module, the application must do the following:
 *
 * 1.) Call StdInit during app initialization. StdInit() will save away
 *		the object ID of the form that contains the stdio field, the field
 *		itself, and the up and down buttons in the field. It also saves
 *		the pointer to the procedure in the application which is responsible
 *		for executing command lines. This procedure will be called when the user
 *		enters a return character in the field.
 *
 *	2.) Call StdHandleEvent from the form's event handler before doing application
 *		specific processing of the event.
 *		In other words, the form event handler that the application
 *		installs with FrmSetEventHandler() should call StdHandleEvent before
 *		it does anything else with the event. 
 *
 * 3.) Call StdFree() during app shutdown.
 *
 *
 * Other than that, the app is free to call StdPutS, StdPrintF, etc anytime it wants
 *		between the StdInit() and StdFree() calls. If the current form is not the
 *		StdIO form when these calls are made, they will record changes to the
 *		active text and display it next time the form becomes active. 
 *
 *----------------------------------------------------------------------------
 *
 * A "typical" application will have the following routine called
 *		ApplicationHandleEvent which gets called from it's main event loop
 *		after SysHandleEvent() and MenuHandleEvent():
 *
 *----------------------------------------------------------------------------
	static Boolean ApplicationHandleEvent (EventPtr event)
	{
		FormPtr frm;
		Word formId;

		if (event->eType == frmLoadEvent) {
			formId = event->data.frmLoad.formID;
			frm = FrmInitForm (formId);
			FrmSetActiveForm (frm);		
			
			switch (formId) {
				.....
				case myViewWithStdIO:
					FrmSetEventHandler (frm, MyViewHandleEvent);
					break;
				}
			return (true);
			}	 

		return (false);
		}	 
 *
 *
 *----------------------------------------------------------------------------
 *
 * A "typical" application form event handler will look like this:
 *
 *----------------------------------------------------------------------------
	static Boolean MyViewHandleEvent (EventPtr event)
	{
		FormPtr frm;
		Boolean handled = false;


		// Let StdIO handler to it's thing first.
		if (StdHandleEvent(event)) return true;


		// If StdIO did not completely handle the event...
		if (event->eType == keyDownEvent) {
			if ( (event->data.keyDown.modifiers & commandKeyMask) &&
					(event->data.keyDown.chr == hardCradleChr) ) {
				FrmGotoForm( kMainFormID );					// exit to main view
				EvtAddEventToQueue( event );					// ... and repost the key event
				handled = true;
				}
			
		else if (event->eType == ctlSelectEvent) {		
			switch (event->data.ctlSelect.controlID) {
				case myViewDoneButtonID:
					FrmGotoForm (networkFormID);
					handled = true;
					break;
				}
			}


		else if (event->eType == menuEvent) 
			return MyMenuDoCommand( event->data.menu.itemID );

			
		else if (event->eType == frmUpdateEvent) {
			MyViewDraw( FrmGetActiveForm() );
			handled = true;
			}
			
			
		else if (event->eType == frmOpenEvent) {
			frm = FrmGetActiveForm();
			MyViewInit( frm );
			MyViewDraw( frm );
			handled = true;
			}
			
			
		else if (event->eType == frmCloseEvent) {
			frm = FrmGetActiveForm();
			MyViewClose(frm);
			}
			
		return (handled);
	}
 *
 *
 **********************************************************************/

Err		StdInit(Word formID, Word fieldID, Word scrollerID,	void (*appProcessCmd)(CharPtr cmdP) );
//Err	StdInit(UInt16 formID, UInt16 fieldID, UInt16 scrollerID, void (*appProcessCmd)(Char * cmdP) );
					
Err		StdFree(void);
Boolean 	StdHandleEvent (EventPtr event);


void 		StdColumn (Word column);
//void StdColumn (UInt16 column)

void 		StdBS (void);
void 		StdPutS (CharPtr str);
void 		StdClear(void);
int		StdPrintF(const CharPtr formatStr, ...);
int		StdFPrintF(unsigned char* fd, const CharPtr formatStr, ...);
//int		StdVPrintF(const CharPtr formatStr, va_list args);
Int		StdGetS(CharPtr strP, Boolean echo);
Int		StdGetChar(Boolean echo);


#ifndef	__DONT_MAP_STDIO__
#define	printf				StdPrintF
#define	fprintf				StdFPrintF
#define	puts					StdPutS
#endif


