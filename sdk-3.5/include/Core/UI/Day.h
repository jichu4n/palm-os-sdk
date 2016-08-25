/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Day.h
 *
 * Description:
 *	  This file defines the date picker month object's  structures 
 *   and routines.
 *
 * History:
 *		May 31, 1995	Created by Roger Flores
 *
 *****************************************************************************/

#ifndef	__DAY_H__
#define	__DAY_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include	<DateTime.h>
#include <Event.h>

typedef enum 
	{
	selectDayByDay,		// return d/m/y
	selectDayByWeek,		// return d/m/y with d as same day of the week
	selectDayByMonth		// return d/m/y with d as same day of the month
	} SelectDayType;

typedef struct DaySelectorType
	{
	RectangleType			bounds;
	Boolean					visible;
	UInt8 					reserved1;
	Int16						visibleMonth;		// month actually displayed
	Int16						visibleYear;		// year actually displayed
	DateTimeType			selected;
	SelectDayType			selectDayBy;
	UInt8 					reserved2;
	} DaySelectorType;

typedef DaySelectorType *DaySelectorPtr;

#ifdef __cplusplus
extern "C" {
#endif

void DayDrawDaySelector (const DaySelectorPtr selectorP)
			SYS_TRAP(sysTrapDayDrawDaySelector);

Boolean DayHandleEvent (const DaySelectorPtr selectorP,
	const EventType *pEvent)
			SYS_TRAP(sysTrapDayHandleEvent);

void DayDrawDays (const DaySelectorPtr selectorP)
			SYS_TRAP(sysTrapDayDrawDays);

#ifdef __cplusplus 
}
#endif

#endif //__DAY_H__

