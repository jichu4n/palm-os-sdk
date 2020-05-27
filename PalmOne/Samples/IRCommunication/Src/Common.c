/**************************************************************************************************
 *
 * Copyright (c) 2007 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/** @ingroup IRCommunication
 *
**/

/** @file Common.c
 * This file contains utility macros and functions for the IR Communication sample applications
**/

/** @name
 *
**/
/*@{*/


/**************************************************************************************************
 *
 *	File:			Common.c
 *
 *	Description:	Some useful macros and functions
 *
 *	Version:		1.0 - Initial Revision (03/04/03)
 *
 *************************************************************************************************/

#include <PalmOS.h>
#include "Common.h"

/**************************************************************************************************
 *
 * FUNCTION:    SetFieldTextFromStr
 *
 * DESCRIPTION: Set a field text.
 *
 * PARAMETERS:  fieldPtr  - Pointer to a field
 *				stringPtr - String to copy to the field
 *				redraw	  - If true, forces a redraw
 *
 * RETURNED:    Error Code.
 *
 *************************************************************************************************/

Err SetFieldTextFromStr( FieldType	*fieldPtr,
						 Char	  	*stringPtr,
						 Boolean  	 redraw )
{
	Err		 	err		  	= errNone;
	MemHandle	textHandle	= FldGetTextHandle( fieldPtr );


	if (textHandle != (MemHandle)NULL)
	{
		FldSetTextHandle( fieldPtr, NULL );
		err = MemHandleResize( textHandle, ( StrLen( stringPtr ) + 1 ) );
		if (err != errNone)
		{
			FldSetTextHandle( fieldPtr, textHandle );
			goto Done;
		}
	}
	else
	{
		textHandle = MemHandleNew( StrLen( stringPtr ) + 1 );
		if (textHandle == (MemHandle)NULL)
		{
			err = memErrNotEnoughSpace;
			goto Done;
		}
	}
	
	StrCopy( (Char *)MemHandleLock( textHandle ), stringPtr );
	MemHandleUnlock( textHandle );
	
	FldSetTextHandle( fieldPtr, textHandle );
	
	if (redraw == true) {
		FldDrawField( fieldPtr );
	}

Done:	
	return ( err );
}

/* EOF *******************************************************************************************/

/*@}*/
