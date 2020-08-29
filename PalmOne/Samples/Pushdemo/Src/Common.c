/******************************************************************************
 * Copyright (c) 2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup Pushdemo
 */
 
/** 
 *  @brief Some useful macros and functions for the Pushdemo app
 *
 *  @version 1.0	- Initial Revision (07/31/07)
 *
 *  @file Common.c
 */



#include <PalmOS.h>
#include "Common.h"

/**
 * @brief SetFieldTextFromStr
 *        
 *
 *
 * @param *fieldPtr:   IN:  Pointer to FieldType structure
 * @param *stringPtr:  IN:  Pointer to char
 * @param redraw:      IN:  Boolean, redraw yes or no?   
 *
 * @version This function was added in version 1.0
 *
 **/

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
