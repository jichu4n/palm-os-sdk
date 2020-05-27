/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	GoLCDUtils.c
 *
 * Description:
 *	Utility functions for GoLCD.
 *
 *	Version 1.0 	- Initial Revision (03/12/03)
 *
 ***********************************************************************/

#include <PalmOS.h>
#include <PalmTypes.h>
#include "GoLCDUtils.h"

/***********************************************************************
 *
 * FUNCTION:	LoadGoLCDLib
 *
 * DESCRIPTION: Load the GoLCD lib, open it
 *
 * PARAMETERS:	refNumP			- Library reference number
 *
 * RETURNED:	Err
 *
 ***********************************************************************/
 
Err LoadGoLCDLib(UInt16 *refNumP)
{
	Err 	err = errNone; 
	UInt16 	refNum = 0;
	Boolean loaded = false;
	//UInt32 	libVersion = 0;
	
	err = SysLibFind(goLcdLibName, &refNum);
	if (err != errNone)
	{
		err = SysLibLoad(goLcdLibType, goLcdLibCreator, &refNum);
		loaded = true;
	}
	if (err)
		goto Done;

	err = GoLcdLibOpen(refNum);
	if (err != errNone)
	{
		if (loaded)	{
			// Only remove it if it was just loaded
			SysLibRemove(refNum);
		}
		goto Done;
	}
			
Done:
	*refNumP = refNum;
	return err;
}

/***********************************************************************
 *
 * FUNCTION:	UnloadGoLCDLib
 *
 * DESCRIPTION: Onload the GoLCd lib
 *
 * PARAMETERS:	refNum		- Library reference number
 *
 * RETURNED:	Err
 *
 ***********************************************************************/
 
Err UnloadGoLCDLib(UInt16 refNum)
{
	Err err = errNone;
	if (refNum == sysInvalidRefNum)
		return -1;
		
	err = GoLcdLibClose(refNum);
	if (err == errNone)
	{
		err = SysLibRemove(refNum);
		ErrNonFatalDisplayIf(err, "SysLibRemove failed");
	}
	
	return err;
}

