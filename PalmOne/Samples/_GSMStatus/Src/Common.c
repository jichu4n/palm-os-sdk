/**************************************************************************************************
 *
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**
 @ingroup GSMStatus
 */

/** 
 *	@file			Common.c
 *
 *	@brief			Some useful macros and functions
 *
 *	@version		1.0 - Initial Revision (03/04/03)
 *
 */

#include <PalmOS.h>
#include "Common.h"

/**
 *	@name Internal Functions
 */
 /*	@{ */
 
Err SetFieldTextFromStr( FieldType	*fieldPtr, Char	*stringPtr, Boolean redraw );

/*	@} */

/**
 * @brief SetFieldTextFromStr
 *        
 *
 *
 * @param *fieldPtr:   IN:  Pointer to FieldType structure
 * @param *stringPtr:  IN:  Pointer to char
 * @param redraw:      IN:  Boolean, redraw yes or no?   
 *
 * @retval Error Code
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
	MemPtr		memP		= NULL ;


	if (textHandle != (MemHandle)NULL)
	{
		FldSetTextHandle( fieldPtr, NULL );
		err = MemHandleResize( textHandle, ( StrLen( stringPtr ) + 1 ) );
		if (err != errNone)
		{
			FldSetTextHandle( fieldPtr, textHandle );
			goto Done;
		}
		DbgMessage ("  SetTextHandle \n");
		
	}
	else
	{
		textHandle = MemHandleNew( StrLen( stringPtr ) + 1 );
		if (textHandle == (MemHandle)NULL)
		{
			err = memErrNotEnoughSpace;
			goto Done;
		}
		DbgMessage ("  allocate dynamic memory \n");
	}
	
	
	memP = MemHandleLock(textHandle );
	StrCopy ((Char *)memP, (const char*)stringPtr);
	//StrCopy( (Char *)MemHandleLock( textHandle ), stringPtr );
	DbgMessage ("  copy succeed \n");

	MemHandleUnlock( textHandle );
	DbgMessage ("  unlock memory \n");
		
	FldSetTextHandle( fieldPtr, textHandle );
	DbgMessage ("  settext \n");
	
	if (redraw == true) {
		FldDrawField( fieldPtr );
	}

Done:	
	return ( err );
}

/* EOF *******************************************************************************************/
