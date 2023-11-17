/******************************************************************************
 *
 * Copyright (c) 199-1999 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Mail.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines the structures and functions of the Mail Application. 
 *
 * History:
 *		July 30, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#include "MailDB.h"
#include "MailRsc.h"
#include "MailMessage.h"
#include "MailLookup.h"
#include "MailSync.h"
#include "MailTruncate.h"

/***********************************************************************
 *
 * Constants
 *
 ***********************************************************************/

#define mailDBType						'DATA'
#define mailDBName						"MailDB"
#define mailVersionNum					3
#define mailPrefsVersionNum			3


// Mail application's preferences
#define mailPrefID						0
#define mailLocalSyncPrefID			1
#define mailRemoteSyncPrefID			2
#define mailSignaturePrefID			3	


// The default length that retrieved message are truncated to.										
#define syncDefaultMaxMsgLen			4000


#define maxListFromNameLen				40
#define maxFriendlyNameLen				60


/***********************************************************************
 *
 * Structutes
 *
 ***********************************************************************/

// This enumeriated type define the field being edited the the "Edit View"
typedef enum {
	editToField,
	editFromField,
	editCCField,
	editBCCField,
	editSubjectField,
	editBodyField
} EditFieldType;


/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/

extern DmOpenRef				MailDB;							// Mail database
extern UInt16					CurrentRecord;					// record being edited

extern FontID					MessageFont;					// font used in message view
extern Boolean					ShowFullHeader;				// True to display full header in message view

extern MsgFieldType			MessageTopField;
extern UInt16 					MessageScrollPos;
extern MsgSelectFieldType	MessageSelectStartField;
extern MsgSelectFieldType	MessageSelectEndField;
extern UInt16					MessageSelectStartPos;
extern UInt16					MessageSelectEndPos;

extern TimeFormatType		TimeFormat;						// system preference
extern DateFormatType		DateFormat;

extern UInt16					TruncateLen;					// Maximun length of retrieved message
extern MemHandle				SyncUIStateH;					// Save save of the HotSync dialog
extern EditFieldType			EditViewField;					// field to edit in the Edit view
extern Boolean					EditViewEditRecord;			// true if editing a record, false if editing hotsync options

/* HandEra */
extern Boolean                  vgaExists;
/* end HandEra */




/***********************************************************************
 *
 * Routines
 *
 ***********************************************************************/

extern void * GetObjectPtr (UInt16 objectID);

extern void SetDBBackupBit(DmOpenRef dbP);

void    ToolsMoveObject     (FormPtr frmP, UInt16 objIndex, Coord x_diff, Coord y_diff, Boolean draw);
void    ToolsResizeObject   (FormPtr frmP, UInt16 objIndex, Coord x_diff, Coord y_diff, Boolean draw);
UInt16  ToolsFrmGetGSI      (FormPtr frmP);
FontID  ToolsVgaFntSetFont  (FontID select, FontID reference);
Boolean ToolsVgaIsLandscapeMode(void);
void    ToolsVgaAdjustModalForm(FormPtr frmP, Boolean canChangeScreenMode);
