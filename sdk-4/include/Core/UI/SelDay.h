/******************************************************************************
 *
 * Copyright (c) 1994-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SelDay.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines the date picker month object's  structures 
 *   and routines.
 *
 * History:
 *		November 10, 1994	Created by Roger Flores
 *
 *****************************************************************************/

#ifndef	__SELDAY_H__
#define	__SELDAY_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

#include <Day.h>

#define daySelectorMinYear  firstYear
#define daySelectorMaxYear  lastYear

#ifdef __cplusplus
extern "C" {
#endif

extern Boolean SelectDayV10 (Int16 *month, Int16 *day, Int16 *year, 
	const Char *title)
			SYS_TRAP(sysTrapSelectDayV10);

extern Boolean SelectDay (const SelectDayType selectDayBy, Int16 *month, 
	Int16 *day, Int16 *year, const Char *title)
			SYS_TRAP(sysTrapSelectDay);

#ifdef __cplusplus 
}
#endif

#endif //__SELDAY_H__
