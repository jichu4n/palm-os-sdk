/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     Day.h
 * AUTHOR:	 Roger Flores: May 31, 1995
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines the date picker month object's  structures 
 *   and routines.
 *
 ***********************************************************************/
#ifndef	__DAY_H__
#define	__DAY_H__


#include	<DateTime.h>

typedef struct {
	RectangleType			bounds;
	Boolean					visible;
	SWord						visibleMonth;		// month actually displayed
	SWord						visibleYear;		// year actually displayed
	DateTimeType			selected;
} DaySelectorType;

typedef DaySelectorType * DaySelectorPtr;


void DayDrawDaySelector (DaySelectorPtr pSelector)
			SYS_TRAP(sysTrapDayDrawDaySelector);

Boolean DayHandleEvent (DaySelectorPtr pSelector, EventPtr pEvent)
			SYS_TRAP(sysTrapDayHandleEvent);

void DayDrawDays (DaySelectorPtr pSelector)
			SYS_TRAP(sysTrapDayDrawDays);


#endif //__DAY_H__
