/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     selTime.h
 * AUTHOR:	 Roger Flores: December 6, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines select time structures and routines.
 *
 ***********************************************************************/

#ifndef	__SELTIME_H__
#define	__SELTIME_H__

#define NumericSpaceChar 0x80

//-------------------------------------------------------------------
// structures
//-------------------------------------------------------------------



typedef struct {
	 Byte hours;
	 Byte minutes;
	 Byte seconds;
} HMSTime;


extern Boolean SelectTime (TimePtr startTimeP, TimePtr EndTimeP,
	Boolean untimed, CharPtr title, SWord startOfDay)
						SYS_TRAP(sysTrapSelectTime);

#endif // __SELTIME_H__
