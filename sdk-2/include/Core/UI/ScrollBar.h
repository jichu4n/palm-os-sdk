/***********************************************************************
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
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


extern void 	SclGetScrollBar (ScrollBarPtr bar, ShortPtr valueP, ShortPtr minP,
						 ShortPtr maxP, ShortPtr pageSizeP)
							SYS_TRAP(sysTrapSclGetScrollBar);

extern void		SclSetScrollBar (ScrollBarPtr bar, short value, short min, 
						short max, short pageSize)
							SYS_TRAP(sysTrapSclSetScrollBar);

extern void		SclDrawScrollBar (ScrollBarPtr bar)
							SYS_TRAP(sysTrapSclDrawScrollBar);

extern Boolean	SclHandleEvent (ScrollBarPtr bar, EventPtr event)
							SYS_TRAP(sysTrapSclHandleEvent);



#endif __SCROLLBAR_H__
