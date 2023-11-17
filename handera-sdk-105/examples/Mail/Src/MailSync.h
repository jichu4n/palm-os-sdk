/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailSync.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines the structures and functions of the Mail Sync 
 *   options.
 *
 * History:
 *		July 26, 1996	Created by Art Lamb
 *
 *****************************************************************************/

// The mail sync options are stored in the following format.
//		SyncFilterType
//		To filter		- null terminated string
//		From filter		- null terminated string
//		Subject filter	- null terminated string

//mgmg added syncUnread for unreadOnly Sync feature
typedef enum { syncAll, syncSendOnly, syncFilter, syncUnread } MailSyncType;
//typedef enum { syncAll, syncSendOnly, syncFilter } MailSyncType;

typedef enum { ingoreContaining, retrieveContaining } MailFilterType;


typedef struct {

	MailSyncType	syncType;				// all, send only, filter

	Boolean			retrieveHighPriority;// If true then high priority messages are
													// always retrieved.

	MailFilterType	messageContaing;		// If true then messages containing the "to",
													// "from" and "subject" are retrived.  If false
													// then messages containing the "to", "from" and
													//  "subject" are ignored.

	UInt8				reserved;
	
	UInt16				maxLength;				// The length that retrieved message should be 
													// truncated to.
} SyncPreferencesType;

typedef SyncPreferencesType * SyncPreferencesPtr;




// This structure is used to save the current HotSync options ui settings
// before we go the the Edit View to edit the to, from or subject filters.  
// The HotSync Options dialog is dismiss when we edit the filter so we need
// to save the UI state.
typedef struct {

	Boolean			local;					// true of local, false for remote

	UInt8				syncType;				// index of selected sync type push button
													// (all, send only, filter).

	Boolean			retrieveHighPriority;// If true then high priority messages are
													// always retrieved.

	MailFilterType	messageContaing;		// If true then messages containing the "to",
													// "from" and "subject" are retrived.  If false
													// then messages containing the "to", "from" and
													//  "subject" are ignored.

	UInt16				maxLength;				// The length that retrieved message should be 
													// truncated to.

	MemHandle			to;
	
	MemHandle			from;

	MemHandle			subject;

} SyncUIStateType;

typedef SyncUIStateType * SyncUIStatePtr;


extern Boolean SyncHandleEvent (EventPtr event);
