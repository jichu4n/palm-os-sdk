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
#include "PalmSoundMgrExt.h"

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

/***********************************************************************
 *
 * FUNCTION:    PalmSGIsTungstenT
 *
 * DESCRIPTION: Check if the device is a Tungsten T.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	True if Tungsten T.
 *
 ***********************************************************************/
 
Boolean	PalmSGIsTungstenT()
{
	Boolean result 		= false;
	UInt32 	companyFtr 	= 0;
	UInt32	deviceFtr	= 0;
	
	// Check for Tungsten T
	FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceFtr);
	FtrGet(sysFtrCreator, sysFtrNumOEMCompanyID, &companyFtr);
	if( companyFtr == 'Palm' && deviceFtr == 'Frg1' )
		result = true;
		
	return result;
}

/***********************************************************************
 *
 * FUNCTION:    PalmSGHasSampledSound
 *
 * DESCRIPTION: Check if a device has the sampled sound API.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	True if sampled sound is available.
 *
 ***********************************************************************/

Boolean PalmSGHasSampledSound()
{
	UInt32	version			= 0;
	Boolean hasSampledSound = false;
	
	// Check if the sound feature is set
	if( FtrGet( sysFileCSoundMgr, sndFtrIDVersion, &version ) == errNone ) {
		hasSampledSound = true;
		goto Done;
	}
	
	if( PalmSGIsTungstenT() )
		hasSampledSound = true;
	
Done:
	return hasSampledSound;
}

/***********************************************************************
 *
 * FUNCTION:    PalmSGHasExtendedSampledSound
 *
 * DESCRIPTION: Check if a device has the extended sampled sound API.
 *
 * PARAMETERS:	-
 *
 * RETURNED:	True if extended sampled sound is available.
 *
 ***********************************************************************/

Boolean PalmSGHasExtendedSampledSound()
{
	Boolean hasExtendedSampledSound = false;
	
	if( SysGetTrapAddress(sysTrapSndStreamCreateExtended) != 
			SysGetTrapAddress(sysTrapSysUnimplemented) )
		hasExtendedSampledSound = true;
	
	return hasExtendedSampledSound;
}
