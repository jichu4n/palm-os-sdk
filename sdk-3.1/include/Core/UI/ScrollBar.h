/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     ScrollBar.h
 * AUTHOR:	 Art Lamb: Feb 6, 1996
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines scroll bar structures and routines.
 *
 ***********************************************************************/

#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

typedef enum { sclUpArrow, sclDownArrow, sclUpPage, sclDownPage, sclCar } 
	ScrollBarRegionType;


typedef struct {
	Word usable			:1;		// Set if part of ui 
	Word visible		:1;		// Set if drawn, used internally
	Word hilighted		:1;		// Set if region is hilighted
	Word shown			:1;		// Set if drawn and maxValue > minValue
	Word activeRegion	:4;		// ScrollBarRegionType
} ScrollBarAttrType;


typedef struct {
	RectangleType		bounds;
	Word					id;
	ScrollBarAttrType	attr;
	Short					value;
	Short					minValue;
	Short					maxValue;
	Short					pageSize;
	Short					penPosInCar;
	Short					savePos;
} ScrollBarType;

typedef ScrollBarType * ScrollBarPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern void 	SclGetScrollBar (const ScrollBarPtr bar, const ShortPtr valueP, 
	const ShortPtr minP, const ShortPtr maxP, const ShortPtr pageSizeP)
							SYS_TRAP(sysTrapSclGetScrollBar);

extern void		SclSetScrollBar (const ScrollBarPtr bar, Short value, 
						const Short min, const Short max, const Short pageSize)
							SYS_TRAP(sysTrapSclSetScrollBar);

extern void		SclDrawScrollBar (const ScrollBarPtr bar)
							SYS_TRAP(sysTrapSclDrawScrollBar);

extern Boolean	SclHandleEvent (const ScrollBarPtr bar, const EventPtr event)
							SYS_TRAP(sysTrapSclHandleEvent);

#ifdef __cplusplus 
}
#endif


#endif __SCROLLBAR_H__
