/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * Project	Instant Karma
 * File		JapaneseFEP.h
 * Author	Art LAmb
 * Purpose	Header file for Text Services Manager User Dictionary support.
 *
 ***********************************************************************/

// This file only contains support for the UserDictRegister macro.


#define sysFileCUserDict       'udic' /* creator of UserDict panel */

/* call user dictionary panel */
enum
{
	userDictLaunchCmdRegister = sysAppLaunchCmdCustomBase
};

/* parameter block passed with the userDictLaunchCmdRegister command. */
typedef struct
{
	/* current focused field or NULL */
	void *field; /* FieldPtr */
} UserDictRegisterParamsType;
typedef UserDictRegisterParamsType *UserDictRegisterParamsPtr;

#define UserDictRegister(fld) \
do \
{ \
	DmSearchStateType searchState; \
	LocalID dbID; \
	UInt cardNo; \
	UserDictRegisterParamsType params; \
\
	params.field = (fld); \
\
	/* Get the card number and database id of the UserDict panel. */ \
	if (DmGetNextDatabaseByTypeCreator (true, &searchState, \
													sysFileTPanel, sysFileCUserDict, \
													true, &cardNo, &dbID) == 0) \
	{ \
		DWord result; \
		SysAppLaunch (cardNo, dbID, 0, \
						  userDictLaunchCmdRegister, (Ptr) &params, &result); \
	} \
} while (0)

