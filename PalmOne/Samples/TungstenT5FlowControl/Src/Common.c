/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	Common.c
 *
 * Description:
 *	Some useful macros and functions
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include "Common.h"

/***********************************************************************
 *
 * FUNCTION:    SetFieldTextFromStr
 *
 * DESCRIPTION: Set a field text.
 *
 * PARAMETERS:  fieldP	- Pointer to a field
 *				string	- String to copy to the field
 *				redraw	- If true, forces a redraw
 *
 * RETURNED:    Error Code.
 *
 ***********************************************************************/

Err SetFieldTextFromStr(FieldType *fieldP, Char *string, Boolean redraw)
{
	Err 		err = errNone;
	MemHandle 	textHandle = FldGetTextHandle(fieldP);

	if(textHandle)
	{
		FldSetTextHandle(fieldP, NULL);
		err = MemHandleResize(textHandle, StrLen(string)+1);
		if(err != errNone) {
			FldSetTextHandle(fieldP, textHandle);
			goto Done;
		}
	}
	else
	{
		textHandle = MemHandleNew(StrLen(string)+1);
		if(!textHandle) {
			err = memErrNotEnoughSpace;
			goto Done;
		}
	}
	
	StrCopy((Char*)MemHandleLock(textHandle), string);
	MemHandleUnlock(textHandle);
	
	FldSetTextHandle(fieldP, textHandle);
	
	if(redraw)
		FldDrawField(fieldP);

Done:	
	return err;
}
