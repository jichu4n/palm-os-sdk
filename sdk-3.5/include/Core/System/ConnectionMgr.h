/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: ConnectionMgr.h
 *
 * Description:
 *		Connection Manager Interface.  The Connection Manager allows
 * other applications to access, add, and delete connection profiles
 * contained in the Connection Panel.
 *
 * History:
 *   	8/19/98		ADH		Initial Implementation
 *
 *****************************************************************************/

#ifndef __CONNECTIONMGR_H__
#define __CONNECTIONMGR_H__


// Include elementary types
#include <PalmTypes.h>
#include <CoreTraps.h>

//Maximum size for a Connection Profile Name
#define cncProfileNameSize	22


//Error Codes
#define cncErrAddProfileFailed	(cncErrorClass | 1)	// Add profile attempt failed
#define cncErrProfileListFull	(cncErrorClass | 2) // Add attempt failed because the
													// profile list is full.
#define cncErrGetProfileFailed	(cncErrorClass | 3)	// Get profile attempt failed
#define cncErrConDBNotFound		(cncErrorClass | 4)	// Connection database not found
#define cncErrGetProfileListFailed	(cncErrorClass | 5)	// Could not get profile list
#define cncErrProfileReadOnly	(cncErrorClass | 6) // The profile can not be altered
#define cncErrProfileNotFound	(cncErrorClass | 7) // The profile could not be found

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 *
 * FUNCTION:    CncGetProfileList
 *
 * DESCRIPTION: Retrieves a list of available profile names and loads the 
 * list into a list structure.  The calling function may specify the visible
 * items or may specify the visible items to be NULL which will display all 
 * items.  The last item on the list will be -Current- which represents the 
 * currently selected profile in the Connection Panel.
 *
 * PARAMETERS:  nameList	A pointer to a list of CharPtrs.  This will
 *						hold the names of the connection profiles.  The 
 *						calling application can pass this pointer to 
 *						LstSetListChoices to load the list.  Note that
 *						the Connection Manager will allocate the memory 
 *						but the calling application is responsible for 
 *						deallocation.
 *				count	The number of items contained in the list is returned
 *						here.  
 *
 * RETURNED:    Error codes
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			ADH		8/19/98		Initial Revision
 *			ADH		8/28/98		Changed Parameter list
 *
 ***********************************************************************/
Err CncGetProfileList(Char *** nameListP, UInt16 *count)
			SYS_TRAP(sysTrapCncGetProfileList);


/***********************************************************************
 *
 * FUNCTION:    CncGetProfileInfo
 *
 * DESCRIPTION: Retrieves the data for a specified profile.  The name of a
 * specific profile may be passed in.  Fields that the calling app is not 
 * interested in may be set to NULL.  Passing in a NULL to the name parameter
 * will cause the Connection Manager to return the currently selected profile
 * from the Connection Panel.  Passing in -Current- to the name field will 
 * return the current profile.
 *
 * PARAMETERS:  name	The name of the profile that is to be returned.  
 *						Passing in NULL or the string "-Current-" will return
 *						the currently selected profile from the Connection
 *						Panel.
 *				port	The port identifier
 *				baud	The baud rate
 *				volume	The volume of the device (only applicable to modems)
 *				handShake	Flow of Control
 *				initString	The modem initialization string (only applicable to
 *						modems)
 *				resetString Used to configure a modem for use in different countries
 *				isModem	Boolean	(True = Modem, False = Serial)
 *				isPulse	Boolean	(True = Pulse Dial, False = TouchTone)
 *
 * RETURNED:    Error codes
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			ADH		8/19/98		Initial Revision
 *
 ***********************************************************************/
Err CncGetProfileInfo(Char *name, UInt32 *port, UInt32 *baud, UInt16 *volume,
		UInt16 *handShake, Char *initString, Char *resetString,
		Boolean *isModem, Boolean *isPulse)
			SYS_TRAP(sysTrapCncGetProfileInfo);


/***********************************************************************
 *
 * FUNCTION:    CncAddProfile
 *
 * DESCRIPTION: Adds a profile to the Connection Panel database.
 *
 * PARAMETERS:  name	The name of the profile that is to be added to the 
 *						database.  Note that the name may be changed while
 *						it is being added.  All profiles must have unique 
 *						names.   
 *				port	The port identifier
 *				baud	The baud rate
 *				volume	The volume of the device (only applicable to modems,
 *						set to 0 for direct)
 *				handShake	Flow of Control (0 = Automatic, 1 = On, 2 = Off)
 *				initString	The modem initialization string (only applicable to
 *						modems.)
 *				resetString Used to configure a modem for use in different countries
 *				isModem	Boolean	(True = Modem, False = Serial)
 *				isPulse	Boolean	(True = Pulse Dial, False = TouchTone)
 *
 * RETURNED:    Error codes
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			ADH		8/19/98		Initial Revision
 *
 ***********************************************************************/
Err CncAddProfile(Char *name, UInt32 port, UInt32 baud, UInt16 volume,
	 UInt16 handShake, Char *initString, Char *resetString, Boolean isModem,
	 Boolean isPulse)
			SYS_TRAP(sysTrapCncAddProfile);


/***********************************************************************
 *
 * FUNCTION:    CncDeleteProfile
 *
 * DESCRIPTION: Deletes a profile from the Connection Panel database.
 *
 * PARAMETERS:  name	The name of the profile that is to be deleted.
 *						Note that default databases can not be deleted.
 *
 * RETURNED:    Error codes
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			ADH		8/19/98		Initial Revision
 *
 ***********************************************************************/	
Err CncDeleteProfile(Char *name)
			SYS_TRAP(sysTrapCncDeleteProfile);
			

#ifdef __cplusplus 
}
#endif

#endif  // __CONNECTIONMGR_H__

