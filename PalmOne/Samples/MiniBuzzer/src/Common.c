/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Common.c
 *
 * Description:
 *	Useful functions
 *
 * Author:
 *	September 19, 2002	Created by Nicolas Pabion
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include "Common.h"

/***********************************************************************
 *
 * FUNCTION:    SetFieldTextFromStr
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    N/A
 *
 ***********************************************************************/

Err SetFieldTextFromStr(FieldPtr field, Char *s, Boolean redraw)
{
	MemHandle h;
	
	h = FldGetTextHandle(field);
	if(h)
	{
		Err err;
		FldSetTextHandle(field, NULL);
		err = MemHandleResize(h, StrLen(s)+1);
		if(err!=errNone)
		{
			FldSetTextHandle(field, h);
			return err;
		}
	} else {
		h = MemHandleNew(StrLen(s)+1);
		if(!h)
			return memErrNotEnoughSpace;
	}
	
	StrCopy((Char *)MemHandleLock(h), s);
	MemHandleUnlock(h);
	
	FldSetTextHandle(field, h);
	if(redraw)
		FldDrawField(field);
	return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    SwapStructureInPlace
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    N/A
 *
 ***********************************************************************/

Err SwapStructureInPlace (void* structP, const UInt8* structDefP, Int32 structSize)
{
	void* endP = (UInt8*)structP + structSize;
	UInt8 d = 0;
	UInt8 count;
	UInt8 size;
	
	while (true)
	{
		d = *structDefP++;
		count = 0x0F & (d >> 4);
		size	= 0x0F & d;
		
		if (!d)
			break;
		
		while (count--)
		{
			switch (size)
			{
				case 1:
					((UInt8*)structP)++;
				break;
					
				case 2:
				{
					UInt16 val = Swap16(*(UInt16*)structP);
					*((UInt16*)structP)++ = val;
				}
				break;
					
				case 4:
				{
					UInt32 val = Swap32(*(UInt32*)structP);
					*((UInt32*)structP)++ = val;
				}
				break;
				
				default:
					ErrFatalDisplay ("Bad structure definition");
					return 1;
			}
			
			structSize -= size;
		}
	}
	
	ErrFatalDisplayIf (structP != endP, "Wrong structure size");

	return errNone;
}
