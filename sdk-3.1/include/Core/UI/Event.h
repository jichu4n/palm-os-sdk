/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Event.h
 * AUTHOR:   Art Lamb: September 26, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines event structures and routines.
 *
 * HISTORY:
 *			05/05/98	art	Add Text Services event.
 *			07/23/98	kwk	Changed Word field in keyDown event to WChar.
 *			08/20/98	kwk	Split tsmEvent into tsmConfirmEvent & tsmFepButtonEvent.
 *			09/07/98	kwk	Added EvtPeekEvent routine declaration.
 *			10/13/98	kwk	Removed EvtPeekEvent until API can be finalized.
 *
 **********************************************************************/

#ifndef __EVENT_H__
#define __EVENT_H__


// Types of events
enum events {
	nilEvent = 0,
	penDownEvent,
	penUpEvent,
	penMoveEvent,
	keyDownEvent,
	winEnterEvent,
	winExitEvent,
	ctlEnterEvent,
	ctlExitEvent,
	ctlSelectEvent,
	ctlRepeatEvent,
	lstEnterEvent,
	lstSelectEvent,
	lstExitEvent,
	popSelectEvent,
	fldEnterEvent,
	fldHeightChangedEvent,
	fldChangedEvent,
	tblEnterEvent,
	tblSelectEvent,
	daySelectEvent,
	menuEvent,
	appStopEvent,
	frmLoadEvent,
	frmOpenEvent,
	frmGotoEvent,
	frmUpdateEvent,
	frmSaveEvent,
	frmCloseEvent,
	frmTitleEnterEvent,
	frmTitleSelectEvent,
	tblExitEvent,
	sclEnterEvent,
	sclExitEvent,
	sclRepeatEvent,
	tsmConfirmEvent,
	tsmFepButtonEvent,
	
	firstUserEvent = 32767
};

// keyDownEvent modifers
#define shiftKeyMask			0x0001
#define capsLockMask			0x0002
#define numLockMask			0x0004
#define commandKeyMask		0x0008
#define optionKeyMask		0x0010
#define controlKeyMask		0x0020
#define autoRepeatKeyMask	0x0040		// True if generated due to auto-repeat
#define doubleTapKeyMask	0x0080		// True if this is a double-tap event
#define poweredOnKeyMask	0x0100		// True if this is a double-tap event
#define appEvtHookKeyMask	0x0200		// True if this is an app hook key
#define libEvtHookKeyMask	0x0400		// True if this is a library hook key
	
// Event timeouts
#define 	evtWaitForever		-1

// The event record.
typedef struct {
	enum events    eType;
	Boolean        penDown;
	SWord          screenX;
	SWord          screenY;
	union data {
		struct generic {
			Word data1;
			Word data2;
			Word data3;
			Word data4;
			Word data5;
			Word data6;
			Word data7;
			Word data8;
			} generic;

		struct penUp {
			PointType	start;					// display coord. of stroke start
			PointType	end;						// display coord. of stroke start
			} penUp;

		struct keyDown {
			WChar chr;					// ascii code
			Word keyCode;				// virtual key code
			Word modifiers;			
			} keyDown;

		struct winEnter {
			WinHandle    enterWindow; 
			WinHandle    exitWindow;
			} winEnter;

		struct winExit {
			WinHandle    enterWindow; 
			WinHandle    exitWindow;
			} winExit;

		struct ctlEnter {
			Word 				controlID;
			void *     		pControl;
			} ctlEnter;

		struct ctlSelect {
			Word controlID;
			void *     		pControl;
			Boolean			on;
			} ctlSelect;

		struct ctlRepeat {
			Word 				controlID;
			void *     		pControl;
			DWord				time;
			} ctlRepeat;

		struct fldEnter {
			Word 				fieldID;
			void *     		pField;
			} fldEnter;

		struct fldHeightChanged {
			Word 				fieldID;
			void *     		pField;
			Word				newHeight;
			Word				currentPos;
			} fldHeightChanged;

		struct fldChanged {
			Word 				fieldID;
			void *     		pField;
			} fldChanged;

		struct fldExit {
			Word 				fieldID;
			void *     		pField;
			} fldExit;

		struct lstEnter {
			Word 				listID;
			void *     		pList;
			Word selection;
			} lstEnter;

		struct lstExit {
			Word 				listID;
			void *     		pList;
			} lstExit;

		struct lstSelect {
			Word 				listID;
			void *     		pList;
			Word selection;
			} lstSelect;

		struct tblEnter {
			Word tableID;
			void *     		pTable;
			Word row;
			Word column;
			} tblEnter;

		struct tblExit {
			Word 				tableID;
			void *     		pTable;
			Word 				row;
			Word 				column;
			} tblExit;

		struct tblSelect {
			Word 				tableID;
			void *     		pTable;
			Word				row;
			Word 				column;
			} tblSelect;

		struct frmLoad {
			Word 				formID;
			} frmLoad;

		struct frmOpen {
			Word 				formID;
			} frmOpen;

		struct frmGoto {
			Word 				formID;
			Word 				recordNum;		// index of record that contain a match
			Word				matchPos;		// postion in record of the match.
			Word				matchLen;		// length of match.
			Word				matchFieldNum;	// field number string was found int
			DWord				matchCustom;	// application specific info
			} frmGoto;

		struct frmClose {
			Word formID;
			} frmClose;

		struct frmUpdate {
			Word 			formID;
			Word 			updateCode;				// Application specific
			} frmUpdate;

		struct frmTitleEnter {
			Word formID;
			} frmTitleEnter;

		struct frmTitleSelect {
			Word formID;
			} frmTitleSelect;

		struct daySelect {
			void *		pSelector;
			Word			selection;
			Boolean		useThisDate;
			} daySelect;

		struct menu {
			Word itemID;
			} menu;

		struct popSelect {
			Word			controlID;
			void *		controlP;
			Word			listID;
			void *    	listP;
			Word			selection;
			Word			priorSelection;
			} popSelect;

		struct sclEnter {
			Word 			scrollBarID;
			void *		pScrollBar;
			} sclEnter;

		struct sclExit {
			Word 			scrollBarID;
			void *		pScrollBar;
			Short			value;
			Short			newValue;
			} sclExit;

		struct sclRepeat {
			Word 			scrollBarID;
			void *		pScrollBar;
			Short			value;
			Short			newValue;
			Long			time;
			} sclRepeat;

		struct tsmConfirm {
			CharPtr		yomiText;
			Word			formID;
		} tsmConfirm;
		
		struct tsmFepButton {
			Word			buttonID;
			} tsmFepButton;

		} data;
		
} EventType;

typedef EventType * EventPtr;


// Events are stored in the event queue with some extra fields:
typedef  struct {
	EventType	event;
	DWord			id;						// used to support EvtAddUniqueEvent
	} EventStoreType;


//---------------------------------------------------------------------
//	Event Functions
//---------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void 		EvtInitialize (void)
							SYS_TRAP(sysTrapEvtInitialize);

void 		EvtAddEventToQueue (const EventPtr event)
							SYS_TRAP(sysTrapEvtAddEventToQueue);
							
void 		EvtAddUniqueEventToQueue(const EventPtr eventP, const DWord id, 
				const Boolean inPlace)
							SYS_TRAP(sysTrapEvtAddUniqueEventToQueue);

void 		EvtCopyEvent (const EventPtr source, const EventPtr dest)
							SYS_TRAP(sysTrapEvtCopyEvent);

void 		EvtGetEvent (const EventPtr event, SDWord timeout)
							SYS_TRAP(sysTrapEvtGetEvent);

Boolean 	EvtEventAvail (void)
							SYS_TRAP(sysTrapEvtEventAvail);

void		EvtGetPen(SWord *pScreenX, SWord *pScreenY, Boolean *pPenDown)
						SYS_TRAP(sysTrapEvtGetPen);

#ifdef __cplusplus 
}
#endif


// For Compatibility.. DOLATER... source modules should use EvtGetPen instead.
#define 		PenGetPoint(a,b,c)	 EvtGetPen(a,b,c)


// zzz make into a routine
#define EvtSetNullEventTick(tick)	\
	if (NeedNullTickCount == 0 || 	\
	    NeedNullTickCount > tick ||	\
	    NeedNullTickCount <= TimGetTicks ())	\
		NeedNullTickCount = tick;



#endif // __EVENT_H__






