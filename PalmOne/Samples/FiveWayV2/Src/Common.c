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
 *	Version 1.0 	- Initial Revision (09/19/02)
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
