/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
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

extern void RctSetRectangle (RectanglePtr r, SWord left, SWord top, 
	SWord width, SWord height)
							SYS_TRAP(sysTrapRctSetRectangle);

extern void RctCopyRectangle (RectanglePtr srcRect, RectanglePtr  dstRect)
							SYS_TRAP(sysTrapRctCopyRectangle);

extern void RctInsetRectangle (RectanglePtr r, SWord insetAmt)
							SYS_TRAP(sysTrapRctInsetRectangle);

extern void  RctOffsetRectangle (RectanglePtr r, SWord deltaX, SWord deltaY)
							SYS_TRAP(sysTrapRctOffsetRectangle);

extern Boolean RctPtInRectangle (SWord x, SWord y, RectanglePtr r)
							SYS_TRAP(sysTrapRctPtInRectangle);

extern void RctGetIntersection (RectanglePtr r1, RectanglePtr r2, RectanglePtr r3)
							SYS_TRAP(sysTrapRctGetIntersection);


#ifdef __cplusplus
}
#endif


#endif //__RECT_H__
