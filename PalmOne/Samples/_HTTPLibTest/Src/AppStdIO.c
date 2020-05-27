/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Any Pilot app that wants a stdio window
 * FILE:     AppStdIO.c
 *
 * DESCRIPTION:
 *	  This module implements stdin/stdout support for a pilot application.
 * It requires that the application provide a form with a text field and
 * scroll bar
 *
 *   The application code must call StdInit() and pass the resource ID's
 * of the form, field, and scroll bar along with other settings
 * before using any of the other calls.
 *
 *   See "AppStdIO.h" for a complete description of how to use this
 *	module. 
 *
 **********************************************************************/

#include <PalmOS.h>				
#include <68K\Hs.h>

//#include <SysAll.h>
//#include <UIAll.h>
#include <KeyMgr.h>

#if EMULATION_LEVEL == EMULATION_NONE
	#include <unix_stdarg.h>
#else
	#include <stdarg.h>
#endif

#include "AppStdIO.h"



#if EMULATION_LEVEL == EMULATION_NONE

/*********************************************************************
 * This stuff is needed if building for the device.
 *******************************************************************/
extern MenuBarPtr		CurrentMenuP;


/*********************************************************************
 * Globals
 * These are initialized by parameters passed to StdInit().
 *******************************************************************/
static VoidHand PrvTextH = NULL;  	// holds latest text

static Word		 PrvFormID = 0;		// Form ID that contains text field
static Word		 PrvFieldID = 0;		// Field ID 
static Word		 PrvScrollerID = 0;

// Routine provided by app for processing commands
void 				(*PrvAppProcessCommand)(Char* cmdP);



/***********************************************************************
 *
 * FUNCTION:		PrvCloneStr
 *
 * DESCRIPTION:	Allocates a handle and stuffs srcStr into it.
 *
 * CALLED BY:		 
 *
 * PARAMETERS:		srcStr - pointer to string to initialize handle with
 *
 * RETURNED:    	 
 *
 ***********************************************************************/
static Handle PrvCloneStr(CharPtr srcStr)
{
	Word		len;
	Handle 	stringH;
	CharPtr 	stringP;
	
	len = StrLen(srcStr);
	if (!len) return NULL;
	
	stringH = MemHandleNew(len+1);
	if (stringH) {
		stringP = MemHandleLock(stringH);
		StrCopy(stringP,srcStr);
		MemPtrUnlock(stringP);
		}
	return stringH;
}



/***********************************************************************
 *
 * FUNCTION:		PrvHandleStrCat
 *
 * DESCRIPTION:	Catenates a string into an existing handle
 *
 * CALLED BY:		 
 *
 * PARAMETERS:		stringH 	- existing handle with text
 *						srcP		- pointer to text to catenate
 *
 * RETURNED:    	 
 *
 ***********************************************************************/
static void PrvHandleStrCat(VoidHand stringH, CharPtr srcP)
{
	Word newSize,oldLength;
	CharPtr stringP;
	
	newSize = StrLen(srcP) + 1;					// initialize new size
	
	// Resize the existing handle, if any
	if ( stringH ) {
		stringP = MemHandleLock( stringH );
		oldLength = StrLen(stringP);
		newSize += oldLength;
		MemPtrUnlock( stringP );
		if ( MemHandleResize(stringH, newSize) == 0) {	// Append the new text
			stringP = MemHandleLock( stringH );
			StrCopy( stringP + oldLength, srcP );			// copy the new text
			MemPtrUnlock( stringP );
			}
		}
}



/***********************************************************************
 *
 * FUNCTION:		StdVPrintF
 *
 * DESCRIPTION:	Utility used to implement printf and fprintf
 *
 * CALLED BY:		StdPrintF, StdFPrintF
 *
 * PARAMETERS:		formatStr - format string
 *						args		 - pointer to argument list.
 *
 * RETURNED:    	# of characters sent to stdout.
 *
 ***********************************************************************/
#define textLen 0x1FF
int	StdVPrintF(const CharPtr formatStr, va_list args);

int	StdVPrintF(const CharPtr formatStr, va_list args)
{
	SWord				result;
	//const	UInt		textLen = 0x1FF;
	static Char		text[textLen+1];		// static so we don't eat up stack space
	
	// Crude check for overflow on the formatStr
	if (StrLen(formatStr) < textLen/2) {
		result = StrVPrintF(text, formatStr, args);
		StdPutS(text);
		}
	else {
		StdPutS(formatStr);
		result = StrLen(formatStr);
		}
		
	return result;
}


/***********************************************************************
 *
 * FUNCTION:    PrvGetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  objectID - which object.
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static VoidPtr PrvGetObjectPtr (Word objectID)
{
	FormPtr frm;
	VoidPtr obj;
	
	frm = FrmGetActiveForm ();
	obj = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, objectID));

	return obj;
}




/***********************************************************************
 *
 * FUNCTION:	SetFieldText
 *
 * DESCRIPTION:	Set field object's text handle.  Will reuse an existing
 *						text handle, if any
 *
 * PARAMETERS:	fldID		-- field object id
 *					srcP		-- source text pointer
 *					append	-- if true, the new text will be appended
 *					redraw	-- if true, field will be redrawn
 *
 * RETURNED:    nothing.
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static void PrvSetFieldText(Word fldID, CharPtr srcP, Boolean append,
								Boolean redraw)
{
	Handle 		stringH;
	UInt			oldLength = 0;
	UInt			newSize;
	CharPtr 		stringP;
	FieldPtr		fldP;

	fldP = (FieldPtr)PrvGetObjectPtr(fldID);

	if (!srcP || !*srcP) {						// handle clearing field as well
		FldFreeMemory(fldP);
		if ( redraw )	 {
			FldEraseField( fldP );
			FldDrawField( fldP );
			}
		return;
		}
	
	
	newSize = StrLen(srcP) + 1;					// initialize new size
	stringH = FldGetTextHandle( fldP );			// get the current text handle
	FldSetTextHandle( fldP, 0 );					// release this handle from field

	// Resize the existing handle, if any
	if ( stringH ) {
		if ( append ) {
			stringP = MemHandleLock( stringH );
			oldLength = StrLen(stringP);
			newSize += oldLength;
			MemPtrUnlock( stringP );
			}
		if ( MemHandleResize(stringH, newSize) )
			goto Exit;
		} // Resize the existing handle, if any
	
	// Otherwise, allocate a new handle
	else {
		stringH = MemHandleNew( newSize );		// allocate a new chunk
		if ( !stringH )	return;
		}

	// Append the new text
	stringP = MemHandleLock( stringH );
	StrCopy( stringP + oldLength, srcP );		// copy the new text
	MemPtrUnlock( stringP );
	
Exit:
	FldSetTextHandle( fldP, stringH );			// set the new text handle
	if ( redraw )	 {
		FldEraseField( fldP );
		FldDrawField( fldP );
		}
}



/***********************************************************************
 *
 * FUNCTION:    PrvFormUpdateScroller
 *
 * DESCRIPTION: This routine draws or erases the status view scrollbar
 *
 * PARAMETERS:  frm             -  pointer to the status form
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			roger	8/7/95	Initial Revision
 *
 ***********************************************************************/
static void PrvFormUpdateScroller (FormPtr frm)
{
	Word 				scrollPos;
	Word 				textHeight;
	Word 				fieldHeight;
	Short 			maxValue;
	FieldPtr 		fld;
	ScrollBarPtr 	bar;

	fld = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, PrvFieldID));
	bar = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, PrvScrollerID));
	
	FldGetScrollValues (fld, &scrollPos, &textHeight,  &fieldHeight);

	if (textHeight > fieldHeight)
		maxValue = textHeight - fieldHeight;
	else
		maxValue = 0;

	SclSetScrollBar (bar, scrollPos, 0, maxValue, fieldHeight-1);
}



/***********************************************************************
 *
 * FUNCTION:    PrvFieldScroll
 *
 * DESCRIPTION: Scrolls the status view a page or a 
 *              line at a time.
 *
 * PARAMETERS:  direction - up or dowm
 *              oneLine   - true if scrolling a single line
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			roger	8/7/95	Initial Revision
 *
 ***********************************************************************/
static void PrvFieldScroll (SWord linesToScroll)
{
	/*
	fld = PrvGetObjectPtr (PrvFieldID);
	if (FldScrollable (fld, direction)) {
		if (oneLine)
			linesToScroll = 1;
		else
			linesToScroll = FldGetVisibleLines (fld) - 1;
			
		FldScrollField (fld, linesToScroll, direction);
		PrvFormUpdateScroller (FrmGetActiveForm());
		}
	*/


	Word				blankLines;
	Short				min;
	Short				max;
	Short				value;
	Short				pageSize;
	FieldPtr			fld;
	ScrollBarPtr	bar;
	
	fld = PrvGetObjectPtr (PrvFieldID);

	if (linesToScroll < 0) {
		blankLines = FldGetNumberOfBlankLines (fld);
		FldScrollField (fld, -linesToScroll, winUp);
		
		// If there were blank lines visible at the end of the field
		// then we need to update the scroll bar.
		if (blankLines) {
			// Update the scroll bar.
			bar = PrvGetObjectPtr (PrvScrollerID);
			SclGetScrollBar (bar, &value, &min, &max, &pageSize);
			if (blankLines > -linesToScroll)
				max += linesToScroll;
			else
				max -= blankLines;
			SclSetScrollBar (bar, value, min, max, pageSize);
			}
		}

	else if (linesToScroll > 0)
		FldScrollField (fld, linesToScroll, winDown);

}

/***********************************************************************
 *
 * FUNCTION:    PrvFieldPageScroll
 *
 * DESCRIPTION: Scroll the owner message a page up or down.
 *
 * PARAMETERS:  direction - up or down
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			rsf	8/7/96	Initial Revision
 *
 ***********************************************************************/
/*
static void PrvFieldPageScroll (WinDirectionType direction)
{
	Short value;
	Short min;
	Short max;
	Short pageSize;
	Word linesToScroll;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = PrvGetObjectPtr (PrvFieldID);
	
	if (FldScrollable (fld, direction)) {
		linesToScroll = FldGetVisibleLines (fld) - 1;
		FldScrollField (fld, linesToScroll, direction);

		// Update the scroll bar.
		bar = PrvGetObjectPtr (PrvScrollerID);
		SclGetScrollBar (bar, &value, &min, &max, &pageSize);

		if (direction == winUp)
			value -= linesToScroll;
		else
			value += linesToScroll;
		
		SclSetScrollBar (bar, value, min, max, pageSize);
		return;
		}
}
*/

/***********************************************************************
 *
 * FUNCTION:    PrvFormInit
 *
 * DESCRIPTION: This routine initializes the "Status View"
 *
 * PARAMETERS:  frm  - a pointer to the Status form
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			gavin	7/15/96	Initial Revision
 *
 ***********************************************************************/
static void PrvFormInit (FormPtr frm)
{
	FieldAttrType 	attr;
	FieldPtr 		fldP;


	if (!PrvTextH)
		PrvTextH = PrvCloneStr(" ");
		
	// Get the field ptr
	fldP = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, PrvFieldID));
	
	// Indicate that it has a scroll bar
	FldGetAttributes (fldP, &attr);
	attr.hasScrollBar = true;
	FldSetAttributes (fldP, &attr);
	
	// Initialize it's text
	FldSetTextHandle( fldP, PrvTextH );			// set the new text handle
	
	// No longer need background copy
	PrvTextH = 0;
	
}


/***********************************************************************
 *
 * FUNCTION:    PrvFormClose
 *
 * DESCRIPTION: Closes down the stdio form
 *
 * PARAMETERS:  frm  - a pointer to the Status form
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			gavin	7/15/96	Initial Revision
 *
 ***********************************************************************/
static void PrvFormClose (FormPtr frm)
{
	FieldPtr fldP;
	
	fldP = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, PrvFieldID));
	
	// Save the current handle
	if (PrvTextH) MemHandleFree(PrvTextH);
	PrvTextH = FldGetTextHandle(fldP);
	
	// Take it out of the field object.
	//FldSetTextHandle( fldP, NULL ); // remove the handle so it 
									 //  isn't freed
	// Commented out line to fix memory leak									  
}


/***********************************************************************
 *
 * FUNCTION:    PrvFormDraw
 *
 * DESCRIPTION: This routine draws the "Sync View"
 *
 * PARAMETERS:  frm - pointer to the view form.
 *
 * RETURNED:    nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk	10/16/95	Initial Revision
 *
 ***********************************************************************/
static void PrvFormDraw(FormPtr frm)
{
	PrvFormUpdateScroller ( frm );
}



/***********************************************************************
 *
 * FUNCTION:    PrvProcessCommand
 *
 * DESCRIPTION: This routine grabs the current line of the text field
 *			and sends it to the application's command processor.
 *
 * PARAMETERS:  void
 *
 * RETURNED:    nothing.
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
/*
static void PrvProcessCommand(void)
{
	CharPtr 		textP,p;
	SWord 		i,j;
	const int 	cmdBufSize = 50; 			// max allowed lenght for a command line
   char 			cmdBuf[cmdBufSize+1];   // buffer for holding command line
   Handle		textH;
	FieldPtr		fldP=0;


	// Get the text handle
	if (FrmGetActiveFormID() == PrvFormID) {
		fldP = PrvGetObjectPtr(PrvFieldID);
		textH = FldGetTextHandle(fldP);

		}
	else {
		textH = PrvTextH;
		}

   // parse text for command line interface here....
   if (!textH) return;
	textP = MemHandleLock(textH);
   
   if (fldP)
   	i = FldGetInsPtPosition(fldP) - 2;
   else
   	i = StrLen(textP)-2;  		// skip null and final linefeed
   
   
   // scan back to previous line or start of text
   while (i>0 && textP[i] != linefeedChr) i--;
   
   if (textP[i] == linefeedChr) i++;  		// do not include the linefeed...
   if (textP[i] == '>') i++;  				// skip over prompt...
   
   // copy into command buffer leaving out final linefeed
	for (p=&textP[i],j=0; *p && *p!=linefeedChr && j<cmdBufSize;)
		cmdBuf[j++] = *p++;
	cmdBuf[j] = 0;
	MemHandleUnlock(textH);
	
	
	// Call app's command handler
	if (PrvAppProcessCommand)
		(*PrvAppProcessCommand)(cmdBuf);
}
*/


/***********************************************************************
 *
 * FUNCTION:		StdColumn
 *
 * DESCRIPTION:	Utility used to move the insertion point to the given column
 *
 * CALLED BY:		Applications.
 *
 * PARAMETERS:		col - which column to move to
 *
 * RETURNED:    	void
 *
 ***********************************************************************/
void StdColumn (Word column)
{	

}


/***********************************************************************
 *
 * FUNCTION:		StdPutS
 *
 * DESCRIPTION:	Utility used to print a string to stdout
 *
 * CALLED BY:		Applications.
 *
 * PARAMETERS:		str - string to print
 *
 * RETURNED:    	void
 *
 ***********************************************************************/
void StdPutS (CharPtr str)
{	
	FieldPtr 	fldP;
	Word			maxSize;
	CharPtr		textP;
	Word			pos;
	
	if (FrmGetActiveFormID() == PrvFormID) {
		fldP = (FieldPtr)PrvGetObjectPtr(PrvFieldID);
		
		// Make sure there's room for this new text
		maxSize = FldGetMaxChars(fldP);
		textP = FldGetTextPtr(fldP);
		
		// If not enough room, lop off the top.
		if (textP) {
			if (StrLen(textP) + StrLen(str) >= maxSize) {
				RectangleType	clipR, savedClipR;

				// Clip off everything so that field package doesn't
				//  do a redraw during FldDelete()
				WinGetClip (&savedClipR);
				clipR = savedClipR;
				clipR.extent.x = clipR.extent.y = 0;
				WinSetClip (&clipR);

				// Lop off the top
				pos = FldGetInsPtPosition(fldP);
				FldDelete(fldP, 0, maxSize/4);
				FldSetInsPtPosition(fldP, pos);

				// Restore clip region.
				WinSetClip (&savedClipR);
				}
			}
		
		// Insert the new text
		FldInsert(fldP, str, StrLen(str));
		PrvFormUpdateScroller (FrmGetActiveForm());
		}
		
	else  {	
		if (PrvTextH)
			PrvHandleStrCat(PrvTextH, str);
		else
			PrvTextH = PrvCloneStr(str);
		}
}



/***********************************************************************
 *
 * FUNCTION:		StdClear
 *
 * DESCRIPTION:	Clear the "screen"
 *
 * CALLED BY:		Applications.
 *
 * PARAMETERS:		void
 *
 * RETURNED:    	void
 *
 ***********************************************************************/
void StdClear(void)
{
	if (FrmGetActiveFormID() == PrvFormID) {
		PrvSetFieldText(PrvFieldID, NULL, false, true);
		PrvFormUpdateScroller (FrmGetActiveForm());
		}
	else {
		if (PrvTextH) MemHandleFree(PrvTextH);
		PrvTextH = NULL;
		}
}






/******************************************************************************
 * The "printf" routine
 *****************************************************************************/
int	StdPrintF(const CharPtr formatStr, ...)
{
	va_list			args;
	SWord				result;
	
	va_start(args, formatStr);
	result = StdVPrintF(formatStr, args);
	va_end(args);
	
	return result;
}


/******************************************************************************
 * The "fprintf" routine
 *****************************************************************************/
int	StdFPrintF(unsigned char* fd, const CharPtr formatStr, ...)
{
	va_list			args;
	SWord				result;
	
	va_start(args, formatStr);
	result = StdVPrintF(formatStr, args);
	va_end(args);
	
	return result;
}


/***********************************************************************
 *
 * FUNCTION:    StdHandleEvent
 *
 * DESCRIPTION: Handles events in the form that contains the
 *		stdio text field and scroll arrows if the event belongs to
 *		the text field or scroll arrows.
 *
 *		This routine should be called from the form event handler
 *		before it does it's own processing with any of the non stdio 
 *		objects in the form.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be 
 *						processed by the app's own form event handler. 
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			roger	8/7/95	Initial Revision
 *
 ***********************************************************************/
Boolean StdHandleEvent (EventPtr event)
{
	FormPtr 			frmP;
	Boolean 			handled = false;

    /*
	if (event->eType == keyDownEvent) {

		if ( event->data.keyDown.chr == pageUpChr ) {
			PrvFieldPageScroll (winUp);
			handled = true;
			}
		
		else if ( event->data.keyDown.chr == pageDownChr ) {
			PrvFieldPageScroll (winDown);
			handled = true;
			}

		else {	
			frmP = FrmGetActiveForm ();
			FrmHandleEvent (frmP, event);
			PrvFormUpdateScroller (frmP);
			if ( event->data.keyDown.chr == linefeedChr )	
				PrvProcessCommand();
			if(event->data.keyDown.chr != vchrRockerCenter && event->data.keyDown.chr != vchrHardRockerCenter) {
			   handled = true;			
			}
		}
    }

	else*/ if (event->eType == sclRepeatEvent)
		{
		PrvFieldScroll (event->data.sclRepeat.newValue - 
			event->data.sclRepeat.value);
		}



	else if (event->eType == frmUpdateEvent) {
		PrvFormDraw( FrmGetActiveForm() );
		}
		
	else if (event->eType == fldChangedEvent) {
		PrvFormUpdateScroller( FrmGetActiveForm() );
		handled = true;
		}
		
		
	else if (event->eType == frmOpenEvent) {
		frmP = FrmGetActiveForm();
		PrvFormInit( frmP );
		PrvFormDraw( frmP );
		FrmSetFocus( frmP, FrmGetObjectIndex (frmP, PrvFieldID)) ;
		}
		
		
	else if (event->eType == frmCloseEvent) {
		frmP = FrmGetActiveForm();
		PrvFormClose(frmP);
		}
		
	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:		StdGetChar
 *
 * DESCRIPTION:	Waits till the user types a character. Ignores all
 *			other types of events. Returns character or -1 if an 
 *			appStop event is encountered or if the user enters the
 *			'~' character.
 *
 * CALLED BY:		Applications.
 *
 * PARAMETERS:		 
 *
 * RETURNED:    	 
 *
 ***********************************************************************/
Int	StdGetChar(Boolean echo)
{
	EventType	event;
	Word			key;
	Char			str[2];
	Word		 	error;
		
	str[1] = 0;
	
	while (1) {
		// Check for system events
		EvtGetEvent(&event, evtWaitForever);
		if (SysHandleEvent(&event)) continue;
		if (MenuHandleEvent (0, &event, &error)) continue;
		
		// On appStop, return end of file
		if (event.eType == appStopEvent) return -1;
		
		// If not a key event, pass to form handler
		if (event.eType != keyDownEvent) {
			FrmDispatchEvent(&event);
			continue;
			}
		
		// Get the key
		key = event.data.keyDown.chr;
		
		// Treat ~ as end-of-file 
		if (key == '~') return -1;
		
		// If echo, display it
		if (echo) {
			str[0] = key;
			if (key == 8)
				StdBS();
			else
				StdPutS(str);
			}
			
		// Return the key
		return key;
		}
		
	
}




/***********************************************************************
 *
 * FUNCTION:		StdInit
 *
 * DESCRIPTION:	Initialize the standard IO manager.
 *
 * CALLED BY:		Application during it's startup.
 *
 * PARAMETERS:		 
 *
 * RETURNED:    	 
 *
 ***********************************************************************/
Err	StdInit(Word formID, Word fieldID, Word scrollerID, 
					void (*appProcessCmd)(CharPtr cmdP) )
{
	
	// Save globals
	PrvFormID = formID;
	PrvFieldID = fieldID;
	PrvScrollerID = scrollerID;
	PrvAppProcessCommand = appProcessCmd;

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:		StdFree
 *
 * DESCRIPTION:	Closes down the standard IO manager.
 *
 * CALLED BY:		Application during it's shutdown.
 *
 * PARAMETERS:		 
 *
 * RETURNED:    	 
 *
 ***********************************************************************/
Err	StdFree(void)
{
	// Free the text handle
	if (PrvTextH) MemHandleFree(PrvTextH);
	PrvTextH = 0;
	
	// Clear globals
	PrvFormID = 0;
	PrvFieldID = 0;
	PrvScrollerID = 0;
	PrvAppProcessCommand = 0;
	
	
	return 0;	
	
}

#endif	// EMULATION_LEVEL == EMULATION_NONE




/***********************************************************************
 *
 * FUNCTION:		StdBS
 *
 * DESCRIPTION:	Utility used to backspace from stdout
 *
 * CALLED BY:		Applications.
 *
 * PARAMETERS:		
 *
 * RETURNED:    	void
 *
 ***********************************************************************/
void StdBS (void)
{	
	EventType	event;
	
	event.eType = keyDownEvent;
	event.data.keyDown.chr = 8;
	StdHandleEvent(&event);
	
}


/***********************************************************************
 *
 * FUNCTION:		StdGetS
 *
 * DESCRIPTION:	Waits till the user types a string. Ignores all
 *			other types of events. Returns -1 of end-of-file is encountered
 *			or 0 otherwise.
 *
 * CALLED BY:		Applications.
 *
 * PARAMETERS:		 
 *
 * RETURNED:    	 
 *
 ***********************************************************************/
Int	StdGetS(CharPtr strP, Boolean echo)
{
	Int			key;
	UInt			index = 0;
	
	// Terminate the string
	strP[0] = 0;
		
	while (1) {
		key = StdGetChar(echo);
	
		// Look for backspace
		if (key == 8 && index==0)
		{
			printf("\nftp> ");
			strP[0] = 0;
			continue;
			}
	
	
		
		// Check for end of file
		if (key == -1) return -1;
		
		// If carriage return, exit
		if (key == '\r' || key == '\n') return 0;
		
		// Look for backspace
		if (key == 8 ) {
			index--;
			strP[index] = 0;
			continue;
			}
		
		// Append to the string
		strP[index++] = key;
		strP[index] = 0;
		}
		
	
}

