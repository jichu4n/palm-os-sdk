/***********************************************************************
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
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


extern Boolean SelectDayV10 (SWord *month, SWord *day, SWord *year, CharPtr title)
			SYS_TRAP(sysTrapSelectDayV10);

extern Boolean SelectDay (SelectDayType selectDayBy, SWord *month, 
	SWord *day, SWord *year, CharPtr title)
			SYS_TRAP(sysTrapSelectDay);


#endif //__SELDAY_H__
