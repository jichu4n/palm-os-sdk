/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Rect.h
 * AUTHOR:	 Roger Flores: November 3, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines rectangle structures and routines.
 *
 ***********************************************************************/

#ifndef __RECT_H__
#define __RECT_H__

typedef struct {
  SWord left;
  SWord top;
  SWord right;
  SWord bottom;
} AbsRectType;


typedef struct {
  SWord x;
  SWord y;
} PointType;


typedef struct {
  PointType  topLeft;
  PointType  extent;
} RectangleType;

typedef RectangleType * RectanglePtr;



#ifdef __cplusplus
extern "C" {
#endif

extern void RctSetRectangle (const RectanglePtr r, const SWord left, 
	const SWord top, const SWord width, const SWord height)
							SYS_TRAP(sysTrapRctSetRectangle);

extern void RctCopyRectangle (const RectanglePtr srcRect, 
	const RectanglePtr  dstRect)
							SYS_TRAP(sysTrapRctCopyRectangle);

extern void RctInsetRectangle (const RectanglePtr r, SWord insetAmt)
							SYS_TRAP(sysTrapRctInsetRectangle);

extern void  RctOffsetRectangle (const RectanglePtr r, const SWord deltaX, 
	const SWord deltaY)
							SYS_TRAP(sysTrapRctOffsetRectangle);

extern Boolean RctPtInRectangle (const SWord x, const SWord y, 
	const RectanglePtr r)
							SYS_TRAP(sysTrapRctPtInRectangle);

extern void RctGetIntersection (const RectanglePtr r1, const RectanglePtr r2, 
	RectanglePtr r3)
							SYS_TRAP(sysTrapRctGetIntersection);


#ifdef __cplusplus
}
#endif


#endif //__RECT_H__
