/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     SelDay.h
 * AUTHOR:	 Roger Flores: November 10, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines the date picker month object's  structures 
 *   and routines.
 *
 ***********************************************************************/
#ifndef	__SELDAY_H__
#define	__SELDAY_H__

#include <Day.h>

#define daySelectorMinYear  firstYear
#define daySelectorMaxYear  lastYear

#ifdef __cplusplus
extern "C" {
#endif

extern Boolean SelectDayV10 (SWord *month, SWord *day, SWord *year, 
	const CharPtr title)
			SYS_TRAP(sysTrapSelectDayV10);

extern Boolean SelectDay (const SelectDayType selectDayBy, SWord *month, 
	SWord *day, SWord *year, const CharPtr title)
			SYS_TRAP(sysTrapSelectDay);

#ifdef __cplusplus 
}
#endif

#endif //__SELDAY_H__
