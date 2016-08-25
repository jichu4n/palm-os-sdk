/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     SelTime.h
 * AUTHOR:	 Roger Flores: December 6, 1994
 * CHANGES:  Nick Twyman 8/4/98. Added SelectOneTime trap
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines select time structures and routines.
 *
 ***********************************************************************/

#ifndef	__SELTIME_H__
#define	__SELTIME_H__

//-------------------------------------------------------------------
// structures
//-------------------------------------------------------------------


typedef struct {
	 Byte hours;
	 Byte minutes;
	 Byte seconds;
} HMSTime;

#ifdef __cplusplus
extern "C" {
#endif

extern Boolean SelectTime (const TimePtr startTimeP, const TimePtr EndTimeP,
	const Boolean untimed, const CharPtr title, SWord startOfDay)
						SYS_TRAP(sysTrapSelectTime);

extern Boolean SelectOneTime(SWord *hour,SWord *minute,const CharPtr title)
						SYS_TRAP(sysTrapSelectOneTime);

#ifdef __cplusplus 
}
#endif

#endif // __SELTIME_H__
