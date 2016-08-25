/***********************************************************************
 *
 *      Copyright (c) Palm Computing 1994 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     Event.h
 * AUTHOR:   Art Lamb: September 26, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines window structures and routines.
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
	tblExitEvent,
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

		struct {
			PointType	start;					// display coord. of stroke start
			PointType	end;						// display coord. of stroke start
			} penUp;

		struct {
			Word chr;					// ascii code
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
			Word controlID;
			void *     		pControl;
			} ctlEnter;

		struct ctlSelect {
			Word controlID;
			void *     		pControl;
			Boolean			on;
			} ctlSelect;

		struct ctlRepeat {
			Word controlID;
			void *     		pControl;
			DWord				time;
			} ctlRepeat;

		struct fldEnter {
			Word fieldID;
			void *     		pField;
			} fldEnter;

		struct fldHeightChanged {
			Word fieldID;
			void *     		pField;
			Word				newHeight;
			Word				currentPos;
			} fldHeightChanged;

		struct fldChanged {
			Word fieldID;
			void *     		pField;
			} fldChanged;

		struct fldExit {
			Word fieldID;
			void *     		pField;
			} fldExit;

		struct lstEnter {
			Word listID;
			void *     		pList;
			Word selection;
			} lstEnter;

		struct lstExit {
			Word listID;
			void *     		pList;
			} lstExit;

		struct lstSelect {
			Word listID;
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
			Word tableID;
			void *     		pTable;
			Word row;
			Word column;
			} tblExit;

		struct tblSelect {
			Word tableID;
			void *     		pTable;
			Word row;
			Word column;
			} tblSelect;

		struct frmLoad {
			Word formID;
			} frmLoad;

		struct frmOpen {
			Word formID;
			} frmOpen;

		struct frmGoto {
			Word formID;
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
			Word formID;
			Word updateCode;				// Application specific
			} frmUpdate;

		struct daySelect {
			void *  		   pSelector;
			Word selection;
			Boolean			useThisDate;
			} daySelect;

		struct menu {
			Word itemID;
			} menu;

		struct popSelect {
			Word controlID;
			void *  		   controlP;
			Word listID;
			void *    		listP;
			Word selection;
			Word priorSelection;
			} popSelect;


		} data;
} EventType;

typedef EventType * EventPtr;

//---------------------------------------------------------------------
//	Event Functions
//---------------------------------------------------------------------

extern void EvtInitialize (void)
							SYS_TRAP(sysTrapEvtInitialize);

extern void EvtAddEventToQueue (EventPtr event)
							SYS_TRAP(sysTrapEvtAddEventToQueue);

extern void EvtCopyEvent (EventPtr source, EventPtr dest)
							SYS_TRAP(sysTrapEvtCopyEvent);

extern void EvtGetEvent (EventPtr event, SDWord timeout)
							SYS_TRAP(sysTrapEvtGetEvent);

void			EvtGetPen(SWord *pScreenX, SWord *pScreenY, Boolean *pPenDown)
						SYS_TRAP(sysTrapEvtGetPen);

// For Compatibility.. DOLATER... source modules should use EvtGetPen instead.
#define 		PenGetPoint(a,b,c)	 EvtGetPen(a,b,c)


#endif // __EVENT_H__






