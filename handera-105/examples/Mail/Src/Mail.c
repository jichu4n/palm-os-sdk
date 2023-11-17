/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Mail.c
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This is the Mail application's main module.
 *
 * History:
 *		May 30, 1996	Created by Art Lamb
 *		Feb 2,  2001	Added localeChanged notification support
 *
 *****************************************************************************/

#include <PalmOS.h>

#include <PalmUtils.h>

#include "Mail.h"
 
/* HandEra */
#include "vga.h"
/* end HandEra */

/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/

// Define the minimum OS version we support
#define ourMinVersion		sysMakeROMVersion(4,0,0,sysROMStageDevelopment,0)

#define noRecordSelected				-1
#define noNewRecord						-1
#define noDate								0

// Categories
#define inboxCategory					0
#define outboxCategory					1
#define deletedCategory					2
#define filedCategory					3
#define draftCategory					4
//mgmg
#define ignoreCategory					13
// this category is used to store a "Revert" record. AS needed. 
// It will be marked as deleted, and stored at the end. So if the user Sync's it
// will properly dissapear.


// Column widths in List View
/* HandEra */
#define listDateWidth					(vgaExists ? (VgaIsVgaFont(ListFont) ? 38 : 25) : 25)
#define listScrollBarWidth				(vgaExists ? 11 : 9)
#define listColumnSpacing				(vgaExists ? 6 : 4)
#define listIconWidth					(vgaExists ? 23 : 15)
//#define listDateWidth					25
//#define listScrollBarWidth				9
//#define listColumnSpacing				4
//#define listIconWidth					15
/* end HandEra */

// Options dialog sort order list items.
#define sortByDateItem					0
#define sortByFromItem					1
#define sortBySubjectItem				2


// Update codes, used to determine how the to do list should 
// be redrawn.
#define updateDisplayOptsChanged		0x01
#define updatePurge						0x02
#define updateFontChanged				0x04
#define updateSelectCurrentRecord	0x80


// Maximum length and width of the Mesage View's title.
#define maxMessageTitleLen				40
#define maxMessageTitleLenWidth		130

// Maximum length and width of the Edit View's title.
#define maxEditTitleLen					40
#define maxEditTitleLenWidth			110


// Edit view table's rows and columns
#define newToRow							0
#define newCCRow							1
#define newBCCRow							2
#define newSubjectRow					3
#define newBodyRow						4

#define maxNewRows						5

#define newLabelColumn					0
#define newTextColumn					1

//mgmg add some space after the label
#define spaceAfterLabel					3

// Max lines per field in new view
#define maxNewAddressLines				2
#define maxNewSubjectLines				2

//mgmg
//#define maxNewFieldChars				4000
#define maxNewFieldChars				10000

// Update codes
#define updateRestoreFocus				1
#define updateReinitialize				2

// Space between columns in find results dialog.
#define findColumnSpacing				4

// Max name length for addresses in the find results dialog.
#define maxFindFromNameLen				40

// Maximum width of a commented message (message where each line starts
// with a >).
#define commentedMsgWidth				119

// Length the buffer used to format a reply message.
#define replayBufferLen					200


// Maximin input length for new message fields
#define maxAddressLength 				4096
#define maxSubjectLength				256
#define maxBodyLength 					4096
#define maxFilterLength					256

//distance from the title to draw the Message x of y text in the message view
#define kMessageTitleOffset				9

/***********************************************************************
 *
 *	Internal Structutes
 *
 ***********************************************************************/

// This is the structure of the data stored in the state file.
typedef struct {
	FontID			messageFont;
	FontID			listFont;
	FontID			editFont;
	Boolean			showDates;
	Boolean			includeOriginalText;
	Boolean			commentOriginalText;	
	Boolean			confirmDelete;
	Boolean			showFullHeader;
	Boolean			attachSignature;
	Boolean			confirmRead;
	Boolean			confirmDelivery;
	UInt8				reserved;		
	UInt16			revertCategory;
} MailPreferenceType;

#define kDefaultListFont stdFont
#define kDefaultMessageFont stdFont
#define kDefaultEditFont stdFont

typedef struct {
    VgaRotateModeType rotation;
    Boolean           rememberRotation;
} MailRotationPrefType;


/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/

		 DmOpenRef			MailDB;									// Mail database

		 TimeFormatType	TimeFormat;								// system preference
       DateFormatType	DateFormat;

static char					CategoryName [dmCategoryLength];	// name of the current categorg

// The following global variables are used to keep track of the edit
// state of the application.
	    UInt16				CurrentRecord = -1;					// record being edited
static UInt16				NewRecord = -1;						// index of unsend new message
static UInt16				TopVisibleRecord = 0;				// top visible record in list view
static UInt16				CurrentCategory = 0;					// currently displayed category
static Int16				MessagesInCategory;					// number of messages in the current category
static UInt16				UnreadMessages = 0;					// number of unread messages

static UInt16				NewViewCurrentRowID = 0;
static Boolean				NewViewShowBCC = false;
static UInt16				NewScrollPosition [maxNewRows];	// Scroll position of each field in the New View.

static Boolean				ItemSelected = false;				// true if a list view item is selected
static UInt16				EditPosition = 0;						// position of the insertion point in the current field
static UInt16				EditBodyPosition = 0;				// position of the insertion point in the body field
static UInt16				EditSelectionLength = 0;			// length of the current selection.
	    EditFieldType		EditViewField;							// field to edit in the Edit view
	    Boolean				EditViewEditRecord;					// true if editing a record, false if editing hotsync options
static Boolean 				RestoreFromEditView = false;	// true if we a re returning from the edit view			
static Boolean				StashedNewViewShowBCC = false;

// Global variable used by Message View
		 MsgFieldType		MessageTopField;					// top visible field
		 UInt16 			MessageScrollPos;						// position of first visible character in top visible field 
		 MsgSelectFieldType	MessageSelectStartField;
		 MsgSelectFieldType	MessageSelectEndField;
		 UInt16				MessageSelectStartPos;
		 UInt16				MessageSelectEndPos;
		
//mgmg
		MemHandle			MessageOriginalRecordData = NULL;
		UInt16				MessageRevertCategory = 0;

// globals for the New view
// *			mgolden	10/6/97		changed for the rocky fonts
		UInt16				gNewViewTableHeight;		// the original height of the table object, used to calculate the 
																// correct new height based on the font.
		
// The following global variables are saved to a state file.
		 FontID				MessageFont = kDefaultListFont;	// font used in message view
		 FontID				ListFont = kDefaultMessageFont;	// font used in message view
		 FontID				EditFont = kDefaultEditFont;		// font used in message view

static Boolean				ShowDates = true;						// true if dates are displayed in the list view
static Boolean				IncludeOriginalText = true;		// true to include original message in reply
static Boolean				CommentOriginalText = false;		// true to comment original message in reply
static Boolean				ConfirmDelete = true;				// true if we display an alert before deleting a message

static MemHandle			MessageTitleH;							// Title of message view.
	    Boolean				ShowFullHeader = false;				// True to display full header in message view
static Boolean				AttachSignature = false;			// True to always attach signature
static Boolean				ConfirmRead = false;					// True to always confirm that a message was read
static Boolean				ConfirmDelivery = false;			// True to always confirm that a message was delivered

static MemHandle			EditTitleH;								// Title of edit view.

		 UInt16				TruncateLen;							// Maximun length of retrieved message
		 MemHandle			SyncUIStateH = 0;							// Save save of the HotSync dialog


static UInt16				ReturnToView = ListView;			// View to return to when a message is sent
static	UInt32					*crIDs = NULL;	// The creator IDs for default
static	Char					*appNames = NULL;	// App names for default
static 	Char					**appNamesArray = NULL;
static	UInt32					appCnt = 0;		// Num of possible defaults

/* HandEra */
static  VgaScreenStateType      gScreenState;
Boolean                         vgaExists = false;
/* end HandEra */
/***********************************************************************
 *
 *	Internal Routines
 *
 ***********************************************************************/

static UInt16 NewViewGetFieldHeight (UInt16 recordNum, Int16 rowID, UInt16 width);
static void NewViewInitRow (TablePtr table, Int16 row, Int16 rowID, UInt16 height);
static void DrawString (Char * str, Int16 x, Int16 y, Int16 width);
static void MessageViewDrawMessage (void);
static void MessageViewToggleFont (void);
static void EditViewToggleFont (void);
static UInt16 NewViewGetLineCount (UInt16 recordNum, Int16 rowID, UInt16 width);
static Boolean MessageUndelete(void);
static void MessageViewMarkUnRead (void);
static void MessageViewEnableButtons(FormPtr frm);
static void ListViewSelectRecord (FormPtr frm, UInt16 recordNum, Boolean scrollToSelect);
static void Search (FindParamsPtr findParams);
static Err MessageViewRevert(DmOpenRef dbP, UInt16 * index);
static void MessageViewInitializRevert(void);
static void MessageViewCleanUpRevert(void);
static void SaveRevert(void);
static void RestoreRevert(Boolean keepIt);
static void ListViewLoadTable (FormPtr frm, UInt16 recordNum);
static void ListViewLoadRecords (FormPtr frm);
#ifdef DO_ROM_CHECK
static Err RomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags);
#endif
static FontID SelectFont (FontID currFontID);
static UInt16 ListViewNumberOfRows (TablePtr table);
static void NewViewToggleFont (void);
static UInt16 NewViewNumberOfRows (TablePtr table);
static void NewViewLoadTable(FormPtr frm);

static void MailLoadPrefs(void);		// BGT
static void MailSavePrefs(void);		// BGT
static Err ParseMailtoURL(MailAddRecordParamsPtr newMsgP, MemPtr cmdPBP,
						  Char *buf);
static Boolean PrvGetNameValuePair(Char **strP, Int16 * strLen, Char **nameP, 
								   Int16 * nameLen, Char **valueP,
								   Int16 * valueLen);
static void PrvSetMailAppsList(Int32 listSelection);
static void RegisterLocaleChangedNotification(void);
static Err CreateDefaultDatabase(void);
static void EventLoop (void);

#ifdef DO_ROM_CHECK
/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: Check that the ROM version meets the
 *              minimum requirement.  Warn if the app was switched to.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags indicating how the application was
 *											 launched.  A warning is displayed only if
 *                                these flags indicate that the app is 
 *											 launched normally.
 *
 * RETURNED:    zero if rom is compatible else an error code
 *                             
 ***********************************************************************/
static Err RomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;
	
	
	// See if we're on in minimum required version of the ROM or later.
	// The system records the version number in a feature.  A feature is a
	// piece of information which can be looked winUp by a creator and feature
	// number.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		// If the user launched the app from the launcher, explain
		// why the app shouldn't run.  If the app was contacted for something
		// else, like it was asked to find a string by the system find, then
		// don't bother the user with a warning dialog.  These flags tell how
		// the app was launched to decided if a warning should be displayed.
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			FrmAlert (RomIncompatibleAlert);
		
			// Pilot 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.  The sysFileCDefaultApp is considered "safe".
			if (romVersion < 0x02000000)
				{
				Err err;
				
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
				}
			}
		
		return (sysErrRomIncompatible);
		}

	return 0;
}
#endif

/***********************************************************************
 *
 * FUNCTION:     SetDBBackupBit
 *
 * DESCRIPTION:  This routine sets the backup bit on the given database.
 *					  This is to aid syncs with non Palm software.
 *					  If no DB is given, open the app's default database and set
 *					  the backup bit on it.
 *
 * PARAMETERS:   dbP -	the database to set backup bit,
 *								can be NULL to indicate app's default database
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			grant	4/1/99	Initial Revision
 *
 ***********************************************************************/
void SetDBBackupBit(DmOpenRef dbP)
{
	DmOpenRef localDBP;
	LocalID dbID;
	UInt16 cardNo;
	UInt16 attributes;

	// Open database if necessary. If it doesn't exist, simply exit (don't create it).
	if (dbP == NULL)
		{
		localDBP = DmOpenDatabaseByTypeCreator (mailDBType, sysFileCMail, dmModeReadWrite);
		if (localDBP == NULL)  return;
		}
	else
		{
		localDBP = dbP;
		}
	
	// now set the backup bit on localDBP
	DmOpenDatabaseInfo(localDBP, &dbID, NULL, NULL, &cardNo, NULL);
	DmDatabaseInfo(cardNo, dbID, NULL, &attributes, NULL, NULL,
				NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	attributes |= dmHdrAttrBackup;
	DmSetDatabaseInfo(cardNo, dbID, NULL, &attributes, NULL, NULL,
				NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	
	// close database if necessary
	if (dbP == NULL) 
		{
		DmCloseDatabase(localDBP);
		}
}


/***********************************************************************
 *
 * FUNCTION:     StartApplication
 *
 * DESCRIPTION:  This routine opens the application's database, laods the 
 *               saved-state information and initializes global variables.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		5/30/96	Initial Revision
 *			MGolden	7/14/97	Added a call to RestoreRevert()
 *			MGolden	8/11/97	Fixed the Missing app info block bug (code from Roger.)
 *			grant		4/7/99	Set the backup bit when creating the DB.
 *			jmp		10/18/99	Replaced inline code with call to MailGetDatabase().
 *			CS			11/14/00	Use PrefGetPreference instead of PrefGetPreferences.
 *
 ***********************************************************************/
static UInt16 StartApplication (void)
{
	Err err = errNone;
	UInt16 index;
	UInt32 uniqueID;
	Boolean synchronize = false;
	MailPackedDBRecordPtr recordP;
	UInt32   vgaVersion;

	// Get the date format from the system preferences.
	DateFormat = (DateFormatType)PrefGetPreference(prefDateFormat);
	TimeFormat = (TimeFormatType)PrefGetPreference(prefTimeFormat);

	// Find the application's data file.  If it doesn't exist create it.
	err = MailGetDatabase (&MailDB, dmModeReadWrite);
	if (err)
		return err;
	
	/* HandEra */
	if (_TRGVGAFeaturePresent(&vgaVersion))
	{
        VgaRotateModeType       rotate;

	    vgaExists = true;

        VgaGetScreenMode(NULL, &rotate);
        VgaSetScreenMode(screenMode1To1, rotate);
	}
	else
	    vgaExists = false;
	/* end HandEra */

	// Read the preferences / saved-state information.
	MailLoadPrefs();
	
	// Get the MemHandle of an unsent message for the last time the application
	// ran.
	NewRecord = noNewRecord;
	uniqueID = MailGetUnsentMessage (MailDB);
	
	//do the restore here...since the database may move around before it returns
	RestoreRevert(uniqueID);

	//now find the unique id record to edit.
	if (uniqueID)
		{
		DmFindRecordByID (MailDB, uniqueID, &NewRecord);
		if (DmQueryRecord (MailDB, NewRecord) == 0)
			NewRecord = noNewRecord;
		}

		
	// Get the count of the unread messages.
	index = 0;
	while (true)
		{
		DmSeekRecordInCategory (MailDB, &index, 0, dmSeekForward, inboxCategory);
		if (DmGetLastErr()) break;
		
		recordP = MemHandleLock (DmQueryRecord (MailDB, index));
		if (! recordP->flags.read)
			UnreadMessages++;
		MemPtrUnlock (recordP);
		index++;
		}	


	TopVisibleRecord = 0;
	CurrentRecord = noRecordSelected;
	
	//RestoreRevert(NewRecord);
	
	MessagesInCategory = DmNumRecordsInCategory (MailDB, CurrentCategory);
		
	return (err);
}


/***********************************************************************
 *
 * FUNCTION:    StopApplication
 *
 * DESCRIPTION: This routine closes the application's database
 *              and saves the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art		5/30/96		Initial Revision
 *			MGolden	7/14/97		Added a call to SaveRevert()
 *
 ***********************************************************************/
static void StopApplication (void)
{	
	// Write the preferences / saved-state information.
	MailSavePrefs();			// (BGT)

	// Send a frmSave event to all the open forms.
	FrmSaveAllForms ();
	
	// Close all the open forms.
	FrmCloseAllForms ();

	//save revert info
	SaveRevert();

	// Free all of the list selection stuff for default app
	if(appNamesArray)
		MemPtrFree(appNamesArray);
	if(appNames)
		MemPtrFree(appNames);
	if(crIDs)
		MemPtrFree(crIDs);

	// Bug fix since SyncUIStateH was never freed when exiting the application
	// in the EditView coming from the Sync Dialog
	// There should be a better way but it should work for now
	if (SyncUIStateH)
	{
		MemHandleFree(SyncUIStateH);
	}

	// Close the application's data file.
	DmCloseDatabase (MailDB);
}


/***********************************************************************
 *
 * FUNCTION:    SyncNotification
 *
 * DESCRIPTION: This routine is a entry point of the to do application.
 *              It is called when the to do application's database is 
 *              synchronized.  This routine will resort the database and
 *              reset the state file info if necessary.
 *
 * PARAMETERS:	 nothing
 *
 * RETURNED:	 nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *			jmp	10/18/99	Use MailGetDatabase() instead of DmOpenDatabaseByTypeCreator().
 *
 ***********************************************************************/
static void SyncNotification (void)
{
	Err err = errNone;
	DmOpenRef dbP;

	// Open the application's data file.
	err = MailGetDatabase (&dbP, dmModeReadWrite);
	if (err) return;
	
	// Resort the database.
	MailSort (dbP);
	
	DmCloseDatabase (dbP);
}



/***********************************************************************
 *
 * FUNCTION:     CreateDefaultDatabase
 *
 * DESCRIPTION:  This routine creates the default database from the
 *					  saved image in a resource in the application.
 *
 * PARAMETERS:   none
 *
 * RETURNED:     0 - if no error
 *					  otherwise appropriate error value
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vivek	02/01/01	Initial Revision
 *
 ***********************************************************************/
static Err CreateDefaultDatabase(void)
{
	MemHandle resH;
	DmOpenRef dbP;
	Err	error = errNone;

	// Attempt to get our default data image and create our
	// database.
	resH = DmGet1Resource(sysResTDefaultDB, sysResIDDefaultDB);
	if (resH)
	{
		error = DmCreateDatabaseFromImage(MemHandleLock(resH));

		if (!error)
		{
			MemHandleUnlock(resH);
			DmReleaseResource(resH);

			// Set the bakcup bit on the newly created DB.
			SetDBBackupBit(NULL);
		}
	}

	// If there is no default data, or we had a problem creating it,
	// then attempt to create an empty database.
	if (!resH || error)
	{
		error = MailGetDatabase (&dbP, dmModeReadWrite);

		if (!error)
			DmCloseDatabase(dbP);

	}
	return error;

}


/***********************************************************************
 *
 * FUNCTION:     RegisterLocaleChangedNotification

 *
 * DESCRIPTION:  Register with NotifyMgr for localeChanged notifications.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vivek	02/01/01	Initial Revision
 *
 ***********************************************************************/
static void RegisterLocaleChangedNotification(void)
{
	UInt16 cardNo;
	LocalID dbID;
	Err err;

	err = SysCurAppDatabase(&cardNo, &dbID);
	ErrNonFatalDisplayIf(err != errNone, "can't get app db info");
	if(err == errNone)
	{
		err = SysNotifyRegister(cardNo, dbID, sysNotifyLocaleChangedEvent,
								NULL, sysNotifyNormalPriority, NULL);

#if EMULATION_LEVEL == EMULATION_NONE
		ErrNonFatalDisplayIf((err != errNone) && (err != sysNotifyErrDuplicateEntry), "can't register");
#endif

	}

	return;
}





/***********************************************************************
 *
 * FUNCTION:    GoToItem
 *
 * DESCRIPTION: This routine is called when the "Go to" button 
 *              in the text search dialog is pressed.
 *
 * PARAMETERS:	 goToParams   - where to go to
 *              launchingApp - true is the application is being launched
 *
 * RETURNED:	 nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	06/27/96	Initial Revision
 *			kwk	12/03/98	Fixed param order in call to MemSet.
 *
 ***********************************************************************/
static void GoToItem (GoToParamsPtr goToParams, Boolean launchingApp)
{
	UInt16			attr;
	UInt16			category;
	UInt16			recordNum;
	UInt32			uniqueID;
	EventType	event;

	recordNum = goToParams->recordNum;


	// If the application is already running, close all the open forms.  If
	// the current record is blank, then it will be deleted, so we'll get 
	// the unique id of the record and use it to find the record index 
	// after all the forms are closed.
	if (! launchingApp)
		{
		DmRecordInfo (MailDB, recordNum, NULL, &uniqueID, NULL); 

		FrmSaveAllForms ();
		FrmCloseAllForms ();

		DmFindRecordByID (MailDB, uniqueID, &recordNum);
		}
		
	// Make the record visible in the List View.
	TopVisibleRecord = recordNum;

	
	// Change the current category.
	DmRecordInfo (MailDB, recordNum, &attr, NULL, NULL);
	category = (attr & dmRecAttrCategoryMask);
	if (CurrentCategory != category)
		{
		CurrentCategory = category;
		MessagesInCategory = DmNumRecordsInCategory (MailDB, CurrentCategory);
		}


	// Change the "show full header" setting such that the found string
	// will be displayable.
	switch (goToParams->matchFieldNum)
		{
		case msgFieldSentTo:
			ShowFullHeader = false;
			break;
			
		case msgFieldTo:
		case msgFieldFrom:
		case msgFieldSubject:
		case msgFieldCC:
			ShowFullHeader = true;
			break;
		}

	// Send an event to goto a form and select the matching text.
	MemSet (&event, sizeof(EventType), 0);

	event.eType = frmLoadEvent;
	event.data.frmLoad.formID = MessageView;
	EvtAddEventToQueue (&event);
 
	event.eType = frmGotoEvent;
	event.data.frmGoto.formID = MessageView;
	event.data.frmGoto.recordNum = recordNum;
	event.data.frmGoto.matchPos = goToParams->matchPos;
	event.data.frmGoto.matchLen = goToParams->matchCustom;
	event.data.frmGoto.matchFieldNum = goToParams->matchFieldNum;
	EvtAddEventToQueue (&event);
}


/***********************************************************************
 *
 * FUNCTION:    AddMessage
 *
 * DESCRIPTION: This routine add a mail new message to the mail datebase
 *              it is called via a launch command by other applications.
 *
 * PARAMETERS:	 msgParams - parameter block containing the new message.
 *               outBox    - put in outBox if true, otherwise drafts
 *
 * RETURNED:	 error code
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		1/21/97	Initial Revision
 *			MGolden	7/9/97	Fixed bug where database was not open.
 *			jmp		10/18/99	Replace inline code with call to MailGetDatabase().
 *			grant		11/30/99	Re-fixed bug where database was not open.
 *
 ***********************************************************************/
static Err AddMessage (MailAddRecordParamsPtr msgParams, Boolean outBox)
{
	Err 					err;
	UInt16				attr;
	UInt16		 		index;
	DmOpenRef			dbP;
	MailDBRecordType	record;

	if (! msgParams->to)
		return (0);

	MemSet (&record, sizeof (MailDBRecordType), 0);

	record.flags.signature = msgParams->signature;
	record.flags.confirmRead = msgParams->confirmRead;
	record.flags.confirmDelivery = msgParams->confirmDelivery;
	record.flags.priority = msgParams->priority;

	if (msgParams->subject)
		record.subject = msgParams->subject;

//removed this because the from field is ignored for outgoing e-mail
//	if (msgParams->from)
//		record.from = msgParams->from;

	if (msgParams->to)
		record.to = msgParams->to;

	if (msgParams->cc)
		record.cc = msgParams->cc;

	if (msgParams->bcc)
		record.bcc = msgParams->bcc;

	if (msgParams->replyTo)
		record.replyTo = msgParams->replyTo;

	if (msgParams->body)
		record.body = msgParams->body;

	// Find the application's data file; if it doesn't exist, create it.
	err = MailGetDatabase (&dbP, dmModeReadWrite);
	if (err) return err;
		
	// Insert the new record.
	err = MailNewRecord (dbP, &record, &index);
	if (err) goto exit;

	// Put the new record in the outbox category.
	DmRecordInfo (dbP, index, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	if(outBox)
		attr |= outboxCategory | dmRecAttrDirty;
	else
		attr |= draftCategory | dmRecAttrDirty;
	if (msgParams->secret)
		attr |= dmRecAttrSecret;
	DmSetRecordInfo (dbP, index, &attr, NULL);


exit:
	DmCloseDatabase (dbP);
	
	return (err);
}



/***********************************************************************
 *
 * FUNCTION:    SeekRecord
 *
 * DESCRIPTION: Given the index of a to do record, this routine scans 
 *              forewards or backwards for displayable 'to do' records.           
 *
 * PARAMETERS:  indexP  - pointer to the index of a record to start from;
 *                        the index of the record sought is returned in
 *                        this parameter.
 *
 *              offset  - number of records to skip:   
 *                        	0 - mean seek from the current record to the
 *                             next display record, if the current record is
 *                             a displayable record, its index is retuned.
 *                         1 - mean seek foreward, skipping one displayable 
 *                             record
 *                        -1 - menas seek backwards, skipping one 
 *                             displayable record
 *                             
 *
 * RETURNED:    false is return if a displayable record was not found.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static Boolean SeekRecord (UInt16 * indexP, Int16 offset, Int16 direction)
{
	DmSeekRecordInCategory (MailDB, indexP, offset, direction, CurrentCategory);
	if (DmGetLastErr()) return (false);
	
	return (true);
}


/***********************************************************************
 *
 * FUNCTION:    CreateRecord
 *
 * DESCRIPTION: This routine creates a new message record.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the record was sucessfully created.
 * 
 * NOTE:        This routine may change the index of the CurrentRecord,
 *              the global variable the holds this index is changed by
 *              this routine.
 *
 * REVISION HISTORY:
 *			Name	Date			Description
 *			----	----			-----------
 *			art	6/6/96		Initial Revision
 *
 ***********************************************************************/
static Boolean CreateRecord (void)
{
	Err					err;
	UInt16					attr;
	UInt32					uniqueID;
	MailDBRecordType	record;

	if (CurrentRecord != noRecordSelected)
		DmRecordInfo (MailDB, CurrentRecord, NULL, &uniqueID, NULL);
		

	MemSet (&record, sizeof (MailDBRecordType), 0);
	record.flags.priority = priorityNormal;
	record.flags.signature = AttachSignature;
	record.flags.confirmRead = ConfirmRead;
	record.flags.confirmDelivery = ConfirmDelivery;
	
	// Add a new record into the database.
	err = MailNewRecord (MailDB, &record, &NewRecord);

	// If the allocate failed, display a warning.
	if (err)
		{
		FrmAlert (DeviceFullAlert);
		NewRecord = noNewRecord;
		return (false);
		}

	// Put the new record in the draft category.
	DmRecordInfo (MailDB, NewRecord, &attr, NULL, NULL);
	attr &= ~(dmRecAttrCategoryMask | dmRecAttrDirty);
	attr |= draftCategory;
	DmSetRecordInfo (MailDB, NewRecord, &attr, NULL);
	
	if (CurrentCategory == draftCategory)
		MessagesInCategory++;		

	if (CurrentRecord != noRecordSelected)
		DmFindRecordByID (MailDB, uniqueID, &CurrentRecord);


	return (true);	
}


/***********************************************************************
 *
 * FUNCTION:    DirtyRecord
 *
 * DESCRIPTION: Mark a record dirty (modified).  Record marked dirty 
 *              will be synchronized.
 *
 * PARAMETERS:  record index
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *
 ***********************************************************************/
static void DirtyRecord (UInt16 index)
{
	UInt16		attr;

	DmRecordInfo (MailDB, index, &attr, NULL, NULL);
	attr |= dmRecAttrDirty;
	DmSetRecordInfo (MailDB, index, &attr, NULL);
}


/***********************************************************************
 *
 * FUNCTION:    PurgeDeletedRecord
 *
 * DESCRIPTION: This routine deletes all the mail messages in the 
 *              "deleted" category.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if record were purged
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
static Boolean PurgeDeletedRecord (void)
{
	UInt16 attr;
	UInt16 index;
	UInt16 numRecords;
	UInt32 uniqueID;
	UInt32 newUniqueID;


	// Check if there are any records to delete.
	index = 0;
	DmSeekRecordInCategory (MailDB, &index, 0, dmSeekForward, deletedCategory);
	if (DmGetLastErr())
		{
		FrmAlert(NothingToPurgeAlert);
		return (false);
		}

	if (FrmAlert(PurgeAlert) != PurgeYes)
		return (false);	


	if (CurrentRecord != noRecordSelected)
		{
		DmRecordInfo (MailDB, CurrentRecord, &attr, &uniqueID, NULL);
		if ((attr & dmRecAttrCategoryMask) == deletedCategory)
			CurrentRecord = noRecordSelected;
		}


	if (NewRecord != noRecordSelected)
		DmRecordInfo (MailDB, NewRecord, &attr, &newUniqueID, NULL);



	numRecords = DmNumRecords (MailDB);

	index = numRecords;
	while (true)
		{
		DmSeekRecordInCategory (MailDB, &index, 0, dmSeekBackward, 
			deletedCategory);
		if (DmGetLastErr()) break;
		
		DmDeleteRecord (MailDB, index);

		// Move deleted record to the end of the index so that the 
		// sorting routine will work.
		DmMoveRecord (MailDB, index, numRecords);
		}	


	if (CurrentRecord != noRecordSelected)
		DmFindRecordByID (MailDB, uniqueID, &CurrentRecord);


	if (NewRecord != noRecordSelected)
		DmFindRecordByID (MailDB, newUniqueID, &NewRecord);


	MessagesInCategory = DmNumRecordsInCategory (MailDB, CurrentCategory);

	// If we're in the deleted category, set the following global the controls
	// which view we return to, so that we will return to the List View.
	if (CurrentCategory == deletedCategory)
		ReturnToView = ListView;

	return (true);
}


/***********************************************************************
 *
 * FUNCTION:    SendMessage
 *
 * DESCRIPTION: This routine send a new mail message.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the record was sucessfully send.
 *
 * REVISION HISTORY:
 *			Name	Date			Description
 *			----	----			-----------
 *			art	6/6/96		Initial Revision
 *
 ***********************************************************************/
static Boolean SendMessage (void)
{
	Err					err;
	UInt16					attr;
	UInt32					uniqueID;
	Boolean				noAddress;
	MemHandle				recordH;	
	MailDBRecordType	record;


	// Varify that we have a "to" address.
	MailGetRecord (MailDB, NewRecord, &record, &recordH);
	noAddress = (*record.to == 0);
	MemHandleUnlock (recordH);

	if (noAddress)
		{
		FrmAlert (NoAddressAlert);
		return (false);
		}
		
	// Get the unique id of the current record, it may move.
	if (CurrentRecord != noRecordSelected)
		DmRecordInfo (MailDB, CurrentRecord, NULL, &uniqueID, NULL);


	// Move the record to its corrent sort position.
	err = MailSortRecord (MailDB, &NewRecord);
	if (err)
		{
		FrmAlert (DeviceFullAlert);
		return (false);
		}


	// Put the new record in the outbox category.
	DmRecordInfo (MailDB, NewRecord, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	attr |= outboxCategory | dmRecAttrDirty;
	DmSetRecordInfo (MailDB, NewRecord, &attr, NULL);
	

	// Clear the datebase value that keep track of the unsent message.
	NewRecord = noNewRecord;
	MailSetUnsentMessage (MailDB, 0);

	if (CurrentCategory == outboxCategory)
		MessagesInCategory++;
	else if (CurrentCategory == draftCategory)
		MessagesInCategory--;


	// Find the current record.
	if (CurrentRecord != noRecordSelected)
		DmFindRecordByID (MailDB, uniqueID, &CurrentRecord);


	return (true);	
}


/***********************************************************************
 *
 * FUNCTION:    SaveDraftMessage
 *
 * DESCRIPTION: This routine stores an new mail message in the draft 
 *              catagory
 *
 * PARAMETERS:  quite   - if false an alert is displayed to confirm
 *                        the operation
 *              deleted - returned: true if record was removed from the 
 *                        database
 *
 * RETURNED:    true if the record was sucessfully saved.
 *
 * REVISION HISTORY:
 *			Name	Date			Description
 *			----	----			-----------
 *			art		9/18/96		Initial Revision
 *			MGolden	7/14/97	revert the message if the user says no to the save
 *							in draft dialog
 *			MGolden	7/16/97	fixed bug where current record was not correct if the 
 *							User says no to the save in draft dialog from the draft folder.
 *			MGolden	7/17/97	Changed tohe code so if the user is clearing a message,
 *							It is not deleted if there is a mesage to revert to. So when 
 *							They are editing a message in the draft folder, and press cancel.
 *							It's not deleted.
 *			MGolden	7/28/97	Added the input Param MenuCmd. If true, the user did not select the 
 *							cancel button, but selected the menu command.
 ***********************************************************************/
static Boolean SaveDraftMessage (Boolean quiet, Boolean * deleted, Boolean menuCmd)
{
	Err					err;
	UInt16					attr;
	UInt16 					button;
	UInt32 				uniqueID;
	Boolean				empty;
	Boolean				save = false;
	MemHandle 			recordH;
	MailDBRecordType	record;
	Boolean 			uniqueIdValid = false;
	*deleted = false;

	if (NewRecord == noNewRecord)
		return (false);


	// Get the unique id of the current record, it may move.
	if (CurrentRecord != noRecordSelected)
		{
		DmRecordInfo (MailDB, CurrentRecord, NULL, &uniqueID, NULL);
		uniqueIdValid = true;
		}
		
	// If the record if empty delete it.
	MailGetRecord (MailDB, NewRecord, &record, &recordH);
	empty = (! *record.to) &&
			  (! *record.cc) &&
			  (! *record.bcc) &&
			  (! *record.subject) &&
			  (! *record.body);
	MemHandleUnlock (recordH);


	DmRecordInfo (MailDB, NewRecord, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	attr |= draftCategory;

	if (empty)
		{
		if (MessageOriginalRecordData && !menuCmd)
			{
			//we need to "Revert" this message to the old state
			MessageViewRevert(MailDB, &NewRecord);
							
			//return (true);
			}
		else 
			{
			if (menuCmd)
				{
				//show the user an alert! They can not save an empty message
				button = FrmAlert (SaveEmptyAlert);
				return false;
				}
				
			*deleted = true;
			if (attr & dmRecAttrDirty)
				{
				DmDeleteRecord (MailDB, NewRecord);
				DmMoveRecord (MailDB, NewRecord, DmNumRecords (MailDB));
				}
			else
				DmRemoveRecord (MailDB, NewRecord);
			}
		}

	else if (quiet)
		save = true;
		
	else
		{
		button = FrmAlert (SaveDraftAlert);
		
		if (button == SaveDraftYes)
			save = true;
		
		else if (button == SaveDraftCancel)
			return (false);
			
		else
			{
			//mgmg
			// Was this from the deleted category? If so, leave it there
			if (MessageOriginalRecordData)
				{				
				//we need to "Revert" this message to the old state
				MessageViewRevert(MailDB, &NewRecord);
								
				return (true);
				}
			else
				{				
				*deleted = true;
				if (attr & dmRecAttrDirty)
					{
					DmDeleteRecord (MailDB, NewRecord);
					DmMoveRecord (MailDB, NewRecord, DmNumRecords (MailDB));
					}
				else
					DmRemoveRecord (MailDB, NewRecord);
				
				//mgmg 7/16
				// clear current record, (we just deleted it)
				if (CurrentRecord == NewRecord)
					CurrentRecord = noRecordSelected;
				}	
			}
		}


	if (save)
		{
		// Move the record to its corrent sort position.
		err = MailSortRecord (MailDB, &NewRecord);
		if (err)
			{
			if (! quiet)
				FrmAlert (DeviceFullAlert);
			return (false);
			}

		DmSetRecordInfo (MailDB, NewRecord, &attr, NULL);
		
		//mgmg 7/16
		// assign this record as the current record, if we are in
		// the draft folder, it will be selected in the list.
		if (CurrentCategory == draftCategory)
			CurrentRecord = NewRecord;
		}


	// Clear the datebase value that keep track of the unsent message.
	NewRecord = noNewRecord;
	MailSetUnsentMessage (MailDB, 0);
	
	if (CurrentCategory == draftCategory && *deleted)
		MessagesInCategory--;

	// Find the current record.
	if (CurrentRecord != noRecordSelected && uniqueIdValid)
		if (DmFindRecordByID (MailDB, uniqueID, &CurrentRecord) != 0)
			CurrentRecord = noRecordSelected;
	
	return (true);
}


/***********************************************************************
 *
 * FUNCTION:    ChangeCategory
 *
 * DESCRIPTION: This routine updates the global varibles that keep track
 *              of category information.  
 *
 * PARAMETERS:  category  - new category (index)
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static void ChangeCategory (UInt16 category)
{
	MessagesInCategory = DmNumRecordsInCategory (MailDB, category);
	CurrentCategory = category;
	TopVisibleRecord = 0;
}


/***********************************************************************
 *
 * FUNCTION:    DrawString
 *
 * DESCRIPTION: This routine draws the string passed, truncating and
                adding an ellipsis to the end if necessary.
 *
 * PARAMETERS:	 str  - pointer to a string
 *              x     - draw position
 *              y     - draw position
 *              width - maximum width to draw.
 *
 * RETURNED:	 nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	05/30/96	Initial Revision
 *			kwk	05/28/99	Revised for Int'l support.
 *
 ***********************************************************************/
static void DrawString (Char * str, Int16 x, Int16 y, Int16 width)
{
	Char * eol = StrChr (str, linefeedChr);
	WinDrawTruncChars (str, (eol == NULL ? StrLen (str): (Int16) (eol - str)), x, y, width);
}


/***********************************************************************
 *
 * FUNCTION:    GetSignature
 *
 * DESCRIPTION: This routine returns a copy of the message signature 
 *              string.  This routine allocates a new MemHandle to hold
 *              signature,  the caller is responsible for freeing this 
 *              MemHandle.
 *              
 * PARAMETERS:  nothing
 *
 * RETURNED:    MemHandle of a signature or zero
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	1/24/96	Initial Revision
 *
 ***********************************************************************/
static MemHandle GetSignature (void)
{
	UInt16 prefsSize;
	Int16 version;
	Char * p;
	MemHandle h;
			
	// Get the length of the signature.
	prefsSize = 0;
	version = PrefGetAppPreferences (sysFileCMail, mailSignaturePrefID, NULL, &prefsSize, true);
	if (version == noPreferenceFound)
		return (0);

	// Copy the signature.
	h = MemHandleNew (prefsSize);
	p = MemHandleLock (h);
	PrefGetAppPreferences (sysFileCMail, mailSignaturePrefID, p, &prefsSize, true);
	MemPtrUnlock (p);

	return (h);
}


/***********************************************************************
 *
 * FUNCTION:    SetSignature
 *
 * DESCRIPTION: This routine save the passed signature to the 
 *              applicatiion information block.
 *
 * PARAMETERS:  dpP - database pointer
 *					 h   - MemHandle of the signature
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	1/24/96	Initial Revision
 *
 ***********************************************************************/
static void SetSignature (MemHandle h)
{
	UInt32				size = 0;
	Char *			p = 0;
			

	if (h)
		{
		p = MemHandleLock (h);
		size = StrLen (p) + 1;
		}

	PrefSetAppPreferences (sysFileCMail, mailSignaturePrefID, mailVersionNum, 
		p, size, true);

	if (h)
		MemPtrUnlock (p);
}


/***********************************************************************
 *
 * FUNCTION:    ClearEditState
 *
 * DESCRIPTION: This routine take the appication out of edit mode.
 *              The edit state of the current record is remember until
 *              this routine is called.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art		6/18/96		Initial Revision
 *
 ***********************************************************************/
static void ClearEditState (void)
{
	// Clear the global variables that keeps track of the edit start of the
	// current record.
	ItemSelected = false;
	EditPosition = 0;
	EditSelectionLength = 0;	
	NewViewCurrentRowID = editToField;
	
	
}


/***********************************************************************
 *
 * FUNCTION:    PrefApply
 *
 * DESCRIPTION: This routine applies the changes made in the Pref Dialog.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
static void PrefApply (void)
{
	MemHandle		h;
	FieldPtr		fld;
	ControlPtr 	ctl;
	UInt16 listItem;

	// Get the "confirm delete message" setting from the dialog box
	ctl = GetObjectPtr (PrefConfirmDeleteCheckbox);
	ConfirmDelete = (CtlGetValue (ctl) != 0);

	// Get the signature field.
	fld = GetObjectPtr (PrefSignatureField);
	if (FldDirty (fld))
		{
		h = FldGetTextHandle (fld);
		SetSignature (h);
		}

	// Set the default mail app
	if(appCnt && crIDs)
	{
		UInt32 crID;

		listItem = LstGetSelection(GetObjectPtr(PrefDefaultMailList));
		crID = crIDs[listItem];
		ExgSetDefaultApplication(crID, exgRegSchemeID, "mailto");
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvSetMailAppsList
 *
 * DESCRIPTION: This routine sets or initializes the list of mail apps
 *
 * PARAMETERS:  The list selection to make current, or -1 to get from the
 * 				exchange manager setting.
 *
 * RETURNED:    Nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			gcd     10/11/00	Initial Revision
 ***********************************************************************/
void PrvSetMailAppsList(Int32 listSelection)
{
	ControlPtr ctl;
	ListPtr lst;
	UInt32	defaultID;

	ctl = GetObjectPtr(PrefDefaultMailTrigger);
	lst = GetObjectPtr(PrefDefaultMailList);

	if(!crIDs)	// If we don't already have the id list
	{
		ExgGetRegisteredApplications(&crIDs, &appCnt, &appNames, NULL, exgRegSchemeID, "mailto");
		if(appCnt)
		{
			MemHandle tmpH = SysFormPointerArrayToStrings(appNames, appCnt);
			if(tmpH)
				appNamesArray = MemHandleLock(tmpH);
			else
				return;
		}
		else
			return;
	}

	if(appNamesArray)
		LstSetListChoices(lst, appNamesArray, appCnt);
	LstSetHeight(lst, appCnt < 6 ? appCnt : 6);

	if(listSelection == -1)
	{
		UInt16	i;
		ExgGetDefaultApplication(&defaultID, exgRegSchemeID, "mailto");

		for(i=0;i<appCnt;i++)
		{
			if(crIDs[i] == defaultID)
				LstSetSelection(lst, i);
		}
	}
	else
		LstSetSelection(lst, listSelection);

	CtlSetLabel(ctl, appNamesArray[LstGetSelection(lst)]);
}

/***********************************************************************
 *
 * FUNCTION:    PrefInit
 *
 * DESCRIPTION: This routine initializes the Pref Dialog.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
static void PrefInit (void)
{
	MemHandle h;
	FieldPtr fld;
	ControlPtr ctl;

	// Set the "confirm delete message" checkbox.
	ctl = GetObjectPtr (PrefConfirmDeleteCheckbox);
	CtlSetValue (ctl, (ConfirmDelete == true));
	
	// Set the signature field.
	h = GetSignature ();
	if (h)
		{
		fld = GetObjectPtr (PrefSignatureField);
		FldSetTextHandle (fld, h);
		}

	// Initialize the list of available mail apps
	PrvSetMailAppsList(-1);
}


/***********************************************************************
 *
 * FUNCTION:    PrefHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Prefereneces
 *              Dialog Box".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
static Boolean PrefHandleEvent (EventPtr event)
{
	FormPtr frm;
	Boolean handled = false;

	if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case PrefOkButton:
				PrefApply ();
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
				break;

			case PrefCancelButton:
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
				break;
			}
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
        if(vgaExists)
            VgaSaveScreenState(&gScreenState);
        ToolsVgaAdjustModalForm(frm, true);
/* end HandEra */
		PrefInit ();
		FrmDrawForm (frm);
		FrmSetFocus (frm, FrmGetObjectIndex (frm, PrefSignatureField));
		handled = true;
		}

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    OptionsApply
 *
 * DESCRIPTION: This routine applies the changes made in the Options Dialog
 *              (aka Preferences).
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static void OptionsApply (void)
{
	UInt8 sortOrder;
	UInt16 listItem;
	ControlPtr ctl;
	
	// Update the sort order.  Reset the To Do list to the top.
	listItem = LstGetSelection (GetObjectPtr (OptionsSortByList));
	switch (listItem)
		{
		case sortByDateItem:		sortOrder = sortByDate;	break;
		case sortByFromItem:		sortOrder = sortByFrom;	break;
		case sortBySubjectItem:		sortOrder = sortBySubject;	break;
		}


	if (MailGetSortOrder (MailDB) != sortOrder)
		{
		MailChangeSortOrder (MailDB, sortOrder);
		TopVisibleRecord = 0;
		}
		
	// Show or hide the due date column.
	ctl = GetObjectPtr (OptionsShowDates);
	ShowDates = CtlGetValue (ctl);
}


/***********************************************************************
 *
 * FUNCTION:    OptionsInit
 *
 * DESCRIPTION: This routine initializes the Options Dialog.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static void OptionsInit (void)
{
	UInt16 sortOrder;
	Char * label;
	ListPtr lst;
	ControlPtr ctl;
	
	// Set the trigger and popup list that indicates the sort order.
	sortOrder = MailGetSortOrder (MailDB);

	lst = GetObjectPtr (OptionsSortByList);
	label = LstGetSelectionText (lst, sortOrder);
	ctl = GetObjectPtr (OptionsSortByTrigger);
	CtlSetLabel (ctl, label);
	LstSetSelection (lst, sortOrder);


	// Initialize the checkboxes in the dialog box.
	ctl = GetObjectPtr (OptionsShowDates);
	CtlSetValue (ctl, ShowDates);
}


/***********************************************************************
 *
 * FUNCTION:    OptionsHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Options
 *              Dialog Box" of the Mail application.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static Boolean OptionsHandleEvent (EventPtr event)
{
	Boolean handled = false;
	FormPtr frm;

	if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case OptionsOkButton:
				OptionsApply ();
				FrmReturnToForm (ListView);
				FrmUpdateForm (ListView, updateDisplayOptsChanged);
				handled = true;
				break;

			case OptionsCancelButton:
				FrmReturnToForm (ListView);
				handled = true;
				break;
				
			}
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
        ToolsVgaAdjustModalForm(frm, false);
/* end HandEra */
		OptionsInit ();
		FrmDrawForm (frm);
		handled = true;
		}

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    ReplyFormatMessage
 *
 * DESCRIPTION: This routine format the message body of a reply message.
 *
 * PARAMETERS:  recordP - message to reply to
 *              forward - true in forwarding the message
 *
 * RETURNED:    true if the new message was sucessfully created.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/15/96	Initial Revision
 *
 ***********************************************************************/
static Err ReplyFormatMessage (MailDBRecordPtr record, Boolean forward)
{
	Err				err = 0;
	Char				chr;
	UInt16				len;
	UInt16				offset = 0;
	UInt16				bufferLen;
	Char *			buffer;
	Char *			str;
	Char *			name;
	MemHandle			resH;

	if ((!*record->body) || (! IncludeOriginalText))
		return (0);

	buffer = MemHandleLock (MemHandleNew (replayBufferLen));

	// Forwarded Message ?
	if (forward)
		{
		// Start the message with a linefeed.
		chr = linefeedChr;
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, &chr, 1, offset);
		if (err) goto exit;
		offset++;

		// Prepend "-- Forwarded Message --" to the beginning of the message
		// text.
		resH = DmGetResource (strRsc, forwardMessageStrId);
		str = MemHandleLock (resH);
		len = StrLen (str);
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, str, len, offset);

		MemHandleUnlock (resH);
		offset += len;
		if (err) goto exit;
		}


	else if (record->from && *record->from)
		{
		// Start the message with a linefeed.
		chr = linefeedChr;
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, &chr, 1, offset);
		if (err) goto exit;
		offset++;

		// Add the email name of the sender, formatted as:
		// <<Email Name(Friendly name if available)>> wrote on <<Message Date>>

		// Get the friendly name if there is one.
		name = MemPtrNew (maxFriendlyNameLen);
		MailParseAddress (record->from, true, name, maxFriendlyNameLen);
		if (*name)
			str = name;
		else
			str = record->from;
		len = StrLen (str);
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, str, len, offset);
		MemPtrFree (name);
		offset += len;
		if (err) goto exit;
		

		// Append the string " wrote on ".
		resH = DmGetResource (strRsc, wroteOnStrId);
		str = MemHandleLock (resH);
		len = StrLen (str);
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, str, len, offset);
		MemHandleUnlock (resH);
		offset += len;
		if (err) goto exit;


		// Add the message date, a colon and a couple of linefeeds.
		DateToAscii (record->date.month, record->date.day, 
			(record->date.year+firstYear) % 100, DateFormat, buffer);
		len = StrLen (buffer);
		buffer [len++] = spaceChr;

		TimeToAscii (record->time.hours, record->time.minutes, TimeFormat, &buffer[len]);

		len = StrLen (buffer);
		buffer [len++] = colonChr;
		buffer [len++] = linefeedChr;
		buffer [len++] = linefeedChr;
		
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, buffer, len, offset);
		offset += len;
		if (err) goto exit;
		}


	// Append the message body.
	if (! CommentOriginalText)
		{
		// Add the body of the message to the forwarded message.
		len = StrLen (record->body);
		err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, record->body, 
			len, offset);
		if (err) goto exit;
		}

	// Comment original text.
	else if (record->body)
		{
		bufferLen = 0;			
		
		str = record->body;
		while (true)
			{
			len = FntWordWrap (str, commentedMsgWidth);

			// Write the buffer if we at the end of the message or if there 
			// is not enought space in the buffer for the next line plus
			// a ">" and a linefeed. 
			if ((len + bufferLen > replayBufferLen - 2) || (len == 0))
				{
				err = MailInsertIntoField (MailDB, NewRecord, mailFieldBody, 
					buffer, bufferLen, offset);
				if (err) goto exit;
				offset += bufferLen;

				if (len == 0)
					break;
				else
					bufferLen = 0;
				}

			buffer[bufferLen++] = '>';
			MemMove (&buffer[bufferLen], str, len);
			str += len;
			bufferLen += len;

			if (buffer[bufferLen-1] != linefeedChr)
				buffer[bufferLen++] = linefeedChr;
			}
		}

exit:
	MemPtrFree (buffer);
	return (err);
}



/***********************************************************************
 *
 * FUNCTION:    ReplyApply
 *
 * DESCRIPTION: This routine applies the changes made in the Reply Dialog.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the new message was sucessfully created.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/15/96	Initial Revision
 *
 ***********************************************************************/
static Boolean ReplyApply (void)
{
	Err							err = 0;
	UInt16							len;
	UInt16							toID;
	UInt16							size;
	UInt32							uniqueID;
	Char *						MemPtr;
	Char *						prefix;
	Char *						to = 0;
	Char *						cc = 0;
	Char *						subject = 0;
	Char *						addresses;
	FormPtr						frm;
	MemHandle						resH;
	MemHandle						recordH;
	ControlPtr 					ctl;
	MailDBRecordType			record;
	

	// Save the include original text checkbox setting.
	ctl = GetObjectPtr (ReplayIncludeTextCheckbox);
	IncludeOriginalText = (CtlGetValue (ctl) != 0);
	
	// Save the comment original text checkbox setting.
	ctl = GetObjectPtr (ReplyCommentTextCheckbox);
	CommentOriginalText = (CtlGetValue (ctl) != 0);

	// Get the reply to setting (sender, all, or forward).
	frm = FrmGetActiveForm ();
	toID = FrmGetObjectId (frm, FrmGetControlGroupSelection (frm, ReplyToGroup));

	// Create the replay message.
	if (! CreateRecord ())
		return (false);

	// Get the curernt record.
	err = MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
	ErrFatalDisplayIf (err, "Error get record");

	// The current record may get move when we change the subject field.
	DmRecordInfo (MailDB, CurrentRecord, NULL, &uniqueID, NULL);

	// Format the subject of the new message.  Append the appropriate 
	//prefix (Re: or Fwd:) to the beginning of the subject.  If the 
	//subject already begins with the prefix don't add it.
	if (toID == ReplyForwardPushButton)
		resH = DmGetResource (strRsc, forwardPrefixStrId);
	else
		resH = DmGetResource (strRsc, replyPrefixStrId);
	prefix = MemHandleLock (resH);
	
	if (MemCmp (record.subject, prefix, StrLen(prefix)) == 0)
		err = MailChangeRecordField (MailDB, &NewRecord, mailFieldSubject, 
			record.subject);
	else
		{
		subject = MemPtrNew (StrLen(prefix) + StrLen (record.subject)  + 1);
		StrCopy (subject, prefix);
		StrCat (subject, record.subject);
		err = MailChangeRecordField (MailDB, &NewRecord, mailFieldSubject, subject);
		MemPtrFree (subject);
		}
	MemHandleUnlock (resH);
	if (err) goto errorExit;


	// Set the "To" field of the new record.  If we're replying to the sender
	// or to all, then set the "To" field to the "From" field of the 
	// original message.  If we're forwarding the message then the "To" field 
	// is blank.
	if (toID == ReplyToSenderPushButton)
		{
		size = StrLen (record.from)  + 1;
		if (size)
			{
			to = MemPtrNew (size);
			MailParseAddress (record.from, false, to, size);
			}
		}

	else if (toID == ReplyToAllPushButton)
		{
		size = StrLen (record.from) + StrLen (record.to) + 2;  // one for linfeedand one for terminator
		if (size)
			{
			to = MemPtrNew (size);
			MailParseAddress (record.from, false, to, size);
		
			// Add all the addresses in the "To" field of the original message.
			addresses = record.to;
			MemPtr = to;
			while (addresses && *addresses)
				{
				len = StrLen (MemPtr);
				if (len)
					MemPtr[len++] = linefeedChr;
				size -= len;
				MemPtr += len;
				MailParseAddress (addresses, false, MemPtr, size);
				addresses = StrChr (addresses, linefeedChr);
				if (addresses) addresses++;
				}
			}
		}

	if (to)
		{
		err = MailChangeRecordField (MailDB, &NewRecord, mailFieldTo, to);
		MemPtrFree (to);
		if (err) goto errorExit;
		}
	

	// Add all the addresses in the "CC" field of the original message.
	if (toID == ReplyToAllPushButton && *record.cc)
		{
		size = StrLen (record.cc) + 1;
		cc = MemPtrNew (size);
		*cc = 0;
		MemPtr = cc;
		addresses = record.cc;
		while (addresses && *addresses)
			{
			MailParseAddress (addresses, false, MemPtr, size);
			addresses = StrChr (addresses, linefeedChr);
			if (addresses)
				{
				addresses++;
				len = StrLen (MemPtr);
				MemPtr[len] = linefeedChr;
				size -= len + 1;
				MemPtr += len + 1;
				}
			}
		if (cc)
			{
			err = MailChangeRecordField (MailDB, &NewRecord, mailFieldCC, cc);
			MemPtrFree (cc);
			if (err) goto errorExit;
			}
		}


	// Format message body.
	err = ReplyFormatMessage (&record, (toID == ReplyForwardPushButton));
	if (err) goto errorExit;


	// Clean winUp
	MemHandleUnlock (recordH);	
	DmFindRecordByID (MailDB, uniqueID, &CurrentRecord);

	return (true);


errorExit:
	MemHandleUnlock (recordH);	
	DmFindRecordByID (MailDB, uniqueID, &CurrentRecord);

	// If the allocate failed, display a warning.
	FrmAlert (DeviceFullAlert);
	return (false);
}


/***********************************************************************
 *
 * FUNCTION:    ReplyInit
 *
 * DESCRIPTION: This routine initializes the Reply Dialog.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/15/96	Initial Revision
 *
 ***********************************************************************/
static void ReplyInit (void)
{
	ControlPtr ctl;

	// Turn on the reply to send push button.
	FrmSetControlGroupSelection (FrmGetActiveForm (), ReplyToGroup,
		ReplyToSenderPushButton);

	// Set the include original text checkbox.
	ctl = GetObjectPtr (ReplayIncludeTextCheckbox);
	CtlSetValue (ctl, (IncludeOriginalText == true));
	
	// Set the comment original text checkbox.
	ctl = GetObjectPtr (ReplyCommentTextCheckbox);
	CtlSetValue (ctl, (CommentOriginalText == true));
}


/***********************************************************************
 *
 * FUNCTION:    ReplyHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Reply
 *              Dialog Box".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/15/96	Initial Revision
 *
 ***********************************************************************/
static Boolean ReplyHandleEvent (EventPtr event)
{
	FormPtr frm;
	Boolean handled = false;

	if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case ReplyOkButton:
				if (ReplyApply ())
					{
					FrmCloseAllForms ();
					FrmGotoForm (NewView);
					}
				else
					FrmReturnToForm (0);
				handled = true;
				break;

			case ReplyCancelButton:
				FrmReturnToForm (0);
				handled = true;
				break;

			case ReplyForwardPushButton:
				CtlSetValue (GetObjectPtr (ReplayIncludeTextCheckbox), true);
//				CtlSetValue (GetObjectPtr (ReplyCommentTextCheckbox), true);
				break;
				
			case ReplyCommentTextCheckbox:
				if (event->data.ctlSelect.on)
					CtlSetValue (GetObjectPtr (ReplayIncludeTextCheckbox), true);
				break;
			}
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
        ToolsVgaAdjustModalForm(frm, false);
/* end HandEra */
		ReplyInit ();
		FrmDrawForm (frm);
		handled = true;
		}

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    DetailsApply
 *
 * DESCRIPTION: This routine applies the changes made in the Details Dialog.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    update code that is sent to the "New View".
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/11/96	Initial Revision
 *
 ***********************************************************************/
static UInt16 DetailsApply (void)
{
	UInt16							updateCode = 0;
	ListPtr 						lst;
	MemHandle						recordH;
	ControlPtr 					ctl;
	Boolean						on;
	MailFlagsType				flags;
	MailDBRecordType			record;


	MailGetRecord (MailDB, NewRecord, &record, &recordH);
	flags = record.flags;
	MemHandleUnlock (recordH);


	// Get the priority setting from the dialog box.
	lst = GetObjectPtr (DetailsPriorityList);
	flags.priority = LstGetSelection (lst);
	
	// Get the signature setting from the dialog box
	ctl = GetObjectPtr (DetailsSignatureCheckbox);
	on = (CtlGetValue (ctl) != 0);
	flags.signature = on;
	AttachSignature = on;

	// Get the "confirm read" setting from the dialog box.
	ctl = GetObjectPtr (DetailsReadCheckbox);
	on = (CtlGetValue (ctl) != 0);
	flags.confirmRead = on;
	ConfirmRead = on;

	// Get the "confirm delivery" setting from the dialog box.
	ctl = GetObjectPtr (DetailsDeliveryCheckbox);
	on = (CtlGetValue (ctl) != 0);
	flags.confirmDelivery = on;
	ConfirmDelivery = on;

	// Update the record if necessary.
	if (MemCmp (&flags, &record.flags, sizeof (MailFlagsType) != 0))
		MailChangeRecordField (MailDB, &NewRecord, mailFieldFlags, &flags);

	// Get the bcc setting
	ctl = GetObjectPtr (DetailsBCCCheckbox);
	on = (CtlGetValue (ctl) != 0);

	if (NewViewShowBCC != on)
		updateCode = updateReinitialize;
	else
		updateCode = updateRestoreFocus;
	NewViewShowBCC = on;

	return (updateCode);
}


/***********************************************************************
 *
 * FUNCTION:    DetailsInit
 *
 * DESCRIPTION: This routine initializes the Details Dialog.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/11/96	Initial Revision
 *
 ***********************************************************************/
static void DetailsInit (void)
{
	UInt16 priority;
	Char * label;
	ListPtr lst;
	MemHandle recordH;
	ControlPtr ctl;
	MailDBRecordType record;

	MailGetRecord (MailDB, NewRecord, &record, &recordH);

	// Set the priority
	priority = record.flags.priority;
	ctl = GetObjectPtr (DetailsPriorityTrigger);
	lst = GetObjectPtr (DetailsPriorityList);
	label = LstGetSelectionText (lst, priority);
	CtlSetLabel (ctl, label);
	LstSetSelection (lst, priority);

	// Set the BCC.
	ctl = GetObjectPtr (DetailsBCCCheckbox);
	CtlSetValue (ctl, (NewViewShowBCC == true));

	// Set the signature checkbox.
	ctl = GetObjectPtr (DetailsSignatureCheckbox);
	CtlSetValue (ctl, (record.flags.signature == true));
	
	// Set the "confirm read" checkbox.
	ctl = GetObjectPtr (DetailsReadCheckbox);
	CtlSetValue (ctl, (record.flags.confirmRead == true));
	
	// Set the "confirm delivery" checkbox.
	ctl = GetObjectPtr (DetailsDeliveryCheckbox);
	CtlSetValue (ctl, (record.flags.confirmDelivery == true));
	
	// Unlock the new message.
	MemHandleUnlock (recordH);
}


/***********************************************************************
 *
 * FUNCTION:    DetailsHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Details
 *              Dialog Box".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/11/96	Initial Revision
 *
 ***********************************************************************/
static Boolean DetailsHandleEvent (EventPtr event)
{
	UInt16 updateCode;
	FormPtr frm;
	Boolean handled = false;

	if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case DetailsOkButton:
				updateCode = DetailsApply ();
				FrmReturnToForm (NewView);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				FrmUpdateForm (NewView, updateCode);
				handled = true;
				break;

			case DetailsCancelButton:
				FrmReturnToForm (NewView);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				FrmUpdateForm (NewView, updateRestoreFocus);
				handled = true;
				break;
			}
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
        
/* HandEra */
        if(vgaExists)
            VgaSaveScreenState(&gScreenState);
        ToolsVgaAdjustModalForm(frm, true);
/* end HandEra */

		DetailsInit ();
		FrmDrawForm (frm);
		handled = true;
		}

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    EditViewUpdateScrollBar
 *
 * DESCRIPTION: This routine update the scroll bar.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/10/96	Initial Revision
 *
 ***********************************************************************/
static void EditViewUpdateScrollBar ()
{
	UInt16				scrollPos;
	UInt16				textHeight;
	UInt16				fieldHeight;
	Int16				maxValue;
	FormPtr			frm;
	FieldPtr			fld;
	ScrollBarPtr	bar;

	frm = FrmGetActiveForm ();
	fld = GetObjectPtr (EditField);
	bar = GetObjectPtr (EditScrollBar);
	
	FldGetScrollValues (fld, &scrollPos, &textHeight,  &fieldHeight);
	
	if (textHeight > fieldHeight)
		maxValue = textHeight - fieldHeight;
	else if (scrollPos)
		maxValue = scrollPos;
	else
		maxValue = 0;

	SclSetScrollBar (bar, scrollPos, 0, maxValue, fieldHeight-1);
}


/***********************************************************************
 *
 * FUNCTION:    EditViewScroll
 *
 * DESCRIPTION: This routine scrolls the message view by the number of
 *              lines specified.
 *
 * PARAMETERS:  direction - winUp or dowm
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/10/96	Initial Revision
 *
 ***********************************************************************/
static void EditViewScroll (Int16 linesToScroll)
{
	FieldPtr fld;

	fld = GetObjectPtr (EditField);

	if (linesToScroll < 0)
		FldScrollField (fld, -linesToScroll, winUp);

	else if (linesToScroll > 0)
		FldScrollField (fld, linesToScroll, winDown);
}


/***********************************************************************
 *
 * FUNCTION:    EditViewPageScroll
 *
 * DESCRIPTION: This routine scrolls the message a page winUp or winDown.
 *
 * PARAMETERS:   direction     winUp or winDown
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/10/96	Initial Revision
 *
 ***********************************************************************/
static void EditViewPageScroll (WinDirectionType direction)
{
	Int16 value;
	Int16 min;
	Int16 max;
	Int16 pageSize;
	UInt16 linesToScroll;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = GetObjectPtr (EditField);
	
	if (FldScrollable (fld, direction))
		{
		linesToScroll = FldGetVisibleLines (fld) - 1;
		FldScrollField (fld, linesToScroll, direction);


		// Update the scroll bar.
		bar = GetObjectPtr (EditScrollBar);
		SclGetScrollBar (bar, &value, &min, &max, &pageSize);

		if (direction == winUp)
			value -= linesToScroll;
		else
			value += linesToScroll;
		
		SclSetScrollBar (bar, value, min, max, pageSize);
		}
}

/***********************************************************************
 *
 * FUNCTION:    EditViewToggleFont
 *
 * DESCRIPTION: This routine changes the font used to display a note.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			MGolden	7/10/97		Change the font for the message view
 *
 ***********************************************************************/
static void EditViewToggleFont ()
	{
	FieldPtr fld;
	FontID originalFont = EditFont;
	
	fld = GetObjectPtr (EditField);
		
	EditFont =  FontSelect (originalFont);
	
	// FldSetFont will redraw the field if it is visible.
	
	if (EditFont != originalFont)
		{
		FldSetFont (fld, EditFont);	
		EditViewUpdateScrollBar ();
		}
	}//EditViewToggleFont

/***********************************************************************
 *
 * FUNCTION:    EditViewSetTitle
 *
 * DESCRIPTION: This routine sets the title of the note view.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	06/10/96	Initial Revision
 *			kwk	12/11/98	Modified for int'l support.
 *
 ***********************************************************************/
 static void EditViewSetTitle (void)
 {
	Int16 i;
	UInt16 len;
	UInt16 width;
 	Int16 maxWidth;
	Char * resP;
	Char * title;
	Char * subject;
	MemHandle resH;
	MemHandle recordH;
	MailDBRecordType record;
	
	if (EditTitleH)
		MemHandleFree (EditTitleH);

	EditTitleH = MemHandleNew (maxEditTitleLen + 2);
	if (! EditTitleH) return;
	title = MemHandleLock (EditTitleH);


	// If we are editing the to, cc, bcc, or subject field, get the title
	// from a resource.
	if (EditViewField < editBodyField)
		{
		resH = DmGetResource (strRsc, editViewTitlesStrId);
		resP = MemHandleLock (resH);
		for (i = 1; i <= EditViewField; i++)
			resP = StrChr (resP, commaChr) + 1;
		len = (UInt16) (StrChr (resP, commaChr) - resP);

		MemMove (title, resP, len);
		title[len] = 0;
		MemHandleUnlock (resH);
		}

	// If we're editing the body of the message then use the subject field
	// as the title.  Truncate the tile if it's to long.
	else
		{
		maxWidth = maxEditTitleLenWidth;
		width = 0;
		*title = 0;

		// Get a pointer to the start of the requested field.
		MailGetRecord (MailDB, NewRecord, &record, &recordH);

		subject = record.subject;
	
		if (*subject)
			{
			title[0] = 0;
			len = 0;
			while (true)
				{
				WChar ch;
				UInt16 charSize = TxtGetNextChar (subject, len, &ch);
				
				if ((ch == '\0')
				|| (ch == linefeedChr)
				|| (len + charSize > maxEditTitleLen)) break;
				
				TxtSetNextChar (title, len, ch);
				len += charSize;
				title[len] = 0;

				width += FntWCharWidth (ch);		
				if (width >= maxWidth) break;
				}

			if ((width >= maxWidth) && (subject[len] != linefeedChr))
				{
				maxWidth -= FntWCharWidth (chrEllipsis);
				while ((width > maxWidth) && (len > 0))
					{
					WChar ch;
					len -= TxtGetPreviousChar (title, len, &ch);
					width -= FntWCharWidth(ch);
					}
				
				title[len] = chrEllipsis;
				title[len+1] = 0;
				}
			}
	
		else
			{
			subject = MemHandleLock (DmGetResource (strRsc, noSubjectStrID));
			StrCopy (title, subject);
			MemPtrUnlock (subject);
			}

		MemHandleUnlock (recordH);
		}

	FrmSetTitle (FrmGetActiveForm (), title);
 }
 

/***********************************************************************
 *
 * FUNCTION:    EditViewDoCommand
 *
 * DESCRIPTION: This routine preforms the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    true if the menu command was handled
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	1/27/97		Initial Revision
 *
 ***********************************************************************/
static Boolean EditViewDoCommand (UInt16 command)
{
	FieldPtr fld;
	Boolean handled = false;
	
	switch (command)
		{
		case editLookupMenuCmd:
			if (EditViewField == editSubjectField ||
				 EditViewField == editBodyField)
				{
				MenuEraseStatus (0);
				SndPlaySystemSound (sndError);
				break;
				}
				
			fld = GetObjectPtr (EditField);
			AddressLookup (fld);
			handled = true;
			break;
		
		case editChangeFontCmd:
			EditViewToggleFont ();
			handled = true;
			break;
		
		}	

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    EditViewClose
 *
 * DESCRIPTION: This routine initializes the Edit View form.
 *
 * PARAMETERS:  frm - pointer to the Edit View form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/10/96	Initial Revision
 *
 ***********************************************************************/
static void EditViewClose (void)
{
	UInt16 selectStart;
	UInt16 selectEnd;
	FieldPtr fld;
	MemHandle textH;
	SyncUIStatePtr	stateP;

	// Release the block that holds the title.
	if (EditTitleH)
		{
		MemHandleFree (EditTitleH);
		EditTitleH = NULL;
		}

	// Save the insertion point position, and length of the selection.  
	// We'll ues these values to restore the edit state in the New View.
	fld = GetObjectPtr (EditField);
	if (ItemSelected)
		{
		EditPosition = FldGetInsPtPosition (fld);
		if (EditViewField == editBodyField)
			EditBodyPosition = EditPosition;
		
		FldGetSelection (fld, &selectStart, &selectEnd);
		EditSelectionLength = selectEnd - selectStart;
		if (EditSelectionLength)
			EditPosition = selectStart;
		}


	// If we're editing HotSync option, get the MemHandle of the field,  the 
	// field routines may have allocated a new MemHandle if the passed 
	// MemHandle was null.
	if (! EditViewEditRecord)
		{
		stateP = MemHandleLock (SyncUIStateH);

 		textH = FldGetTextHandle (fld);

		switch (EditViewField)
			{
			case editToField:			stateP->to = textH;			break;
			case editFromField:		stateP->from = textH;		break;
			case editSubjectField:	stateP->subject = textH;	break;
			}

		MemPtrUnlock (stateP);		
		}


	// Clear the MemHandle value in the field, otherwise the MemHandle
	// will be freed when the form is disposed of,  this call also unlocks
	// the MemHandle that contains the note string.
	FldSetTextHandle (fld, 0);
}

/* HandEra */
/***********************************************************************
 *
 * FUNCTION:    EditViewResize
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void EditViewResize(Boolean draw)
{
    Coord           x,y;
    FormPtr         frmP;
    Coord           xDiff, yDiff;
    Boolean         drawBtn;
    FieldPtr        fldP;
    RectangleType   fldR;
    RectangleType   r;
    
    WinGetDisplayExtent(&x, &y);

    // this will get the old extent
    // resize form
    frmP = FrmGetActiveForm();
    FrmGetFormBounds(frmP, &r);
    
    xDiff = x - (r.topLeft.x + r.extent.x);
    yDiff = y - (r.topLeft.y + r.extent.y);
    if ((xDiff == 0) && (yDiff == 0))
        return;

    // resize form
    r.extent.x += xDiff;
    r.extent.y += yDiff;
    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);

    // Need to erase the field first, otherwise it will erase moved controls
    // when shrinking it.
    fldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, EditField));
    if (draw)
        FldEraseField(fldP);

    FldGetBounds(fldP, &fldR);
    fldR.extent.x += xDiff;
    fldR.extent.y += yDiff;
    FldSetBounds(fldP, &fldR); 

    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, EditDoneButton), 0, yDiff, draw);
    drawBtn = (EditViewField <= editBCCField) ? draw : false;
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, EditLookupButton), 0, yDiff, draw && drawBtn);

    drawBtn = (EditViewField == editBodyField) ? draw : false;
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, EditSendButton), 0, yDiff, draw && drawBtn);

    ToolsMoveObject(frmP, ToolsFrmGetGSI(frmP), xDiff, yDiff, draw);

    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, EditScrollBar),   xDiff, 0, false);
    ToolsResizeObject(frmP, FrmGetObjectIndex(frmP, EditScrollBar), 0, yDiff, draw);
    
    if (draw)
        FrmDrawForm(frmP);
}
/* end HandEra */

/***********************************************************************
 *
 * FUNCTION:    EditViewInit
 *
 * DESCRIPTION: This routine initializes the Edit View form.
 *
 * PARAMETERS:  frm - pointer to the Edit View form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/10/96	Initial Revision
 *			MGolden	7/7/97	removed the font butotns. added menu
 *
 ***********************************************************************/
static void EditViewInit (FormPtr frm)
{
	UInt16							size;
	UInt16							offset;
	//UInt16							controlID;
	Char *						textP;
	MemHandle						textH;
	FieldPtr						fld;
	MemHandle						recordH;
	FieldAttrType				attr;
	MailDBRecordType			record;
	SyncUIStatePtr				stateP;
	

    EditViewResize(false);

	fld         = GetObjectPtr (EditField);
	FldSetFont (fld, EditFont);

	EditViewSetTitle ();

	// Are we editing an expense record or Hotsync options.
	if (EditViewEditRecord)
		{
		// Set the value of the edit field to the proper record field.
		MailGetRecord (MailDB, NewRecord, &record, &recordH);

		switch (EditViewField)
			{
			case editToField:			textP = record.to;			break;
			case editCCField:			textP = record.cc;			break;
			case editBCCField:		textP = record.bcc;			break;
			case editSubjectField:	textP = record.subject;		break;
			case editBodyField:		textP = record.body;			break;
			}
		
		// MailGetRecord returns a locked handle.
		// We need to get the Ptr, so we lock it again, and then unlock it twice.  Ick.
		
		offset = textP - ((Char *) MemHandleLock(recordH));
		size = StrLen (textP) + 1;  			// one for null terminator
		MemHandleUnlock (recordH);
		MemHandleUnlock (recordH);

		FldSetMaxChars (fld, maxNewFieldChars);
		FldSetText (fld, recordH, offset, size);
		}

	// We're editing the HotSync options
	else
		{
		stateP = MemHandleLock (SyncUIStateH);

		switch (EditViewField)
			{
			case editToField:			textH = stateP->to;			break;
			case editFromField:		textH = stateP->from;		break;
			case editSubjectField:	textH = stateP->subject;	break;
			}

		FldSetMaxChars (fld, maxFilterLength);
		FldSetTextHandle (fld, textH);
		MemPtrUnlock (stateP);		
		}
	
	
	// Use the edit state from the new view.
	if (ItemSelected)
		{
		FldSetInsPtPosition (fld, EditPosition);
		if (EditSelectionLength)
			FldSetSelection (fld, EditPosition, 
				EditPosition + EditSelectionLength);
		}


	// Have the field send event to maintain the scroll bar.
	FldGetAttributes (fld, &attr);
	attr.hasScrollBar = true;

	// Only the subject and message body are auto-shift fields.
	if (EditViewField == editSubjectField || EditViewField == editBodyField)
		attr.autoShift = true;

	FldSetAttributes (fld, &attr);


	// If we're editing the "To", "CC" or "BCC" field then show the 
	// lookup button.
	if (EditViewField <= editBCCField)
		FrmShowObject (frm, FrmGetObjectIndex (frm, EditLookupButton));

	// If we're editing the body of the message then shoe the send button.
	if (EditViewField == editBodyField)
		FrmShowObject (frm, FrmGetObjectIndex (frm, EditSendButton));


	// Highlight the corect font push button.
//mgmg removed the font buttons
/*
	if (MessageFont == stdFont)
		controlID = EditSmallFontButton;
	else
		controlID = EditLargeFontButton;
	FrmSetControlGroupSelection (frm, EditFontGroup, controlID);
*/

	EditViewUpdateScrollBar ();	
}


/***********************************************************************
 *
 * FUNCTION:    EditViewHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Edit View".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/10/96	Initial Revision
 *			MGolden	7/7/97	Removed the font buttons...added the menu
 *			MGolden	7/14/947 added MessageViewCleanUpRevert
 *			CSS	06/22/99	Standardized keyDownEvent handling
 *								(TxtCharIsHardKey, commandKeyMask, etc.)
 *
 ***********************************************************************/
static Boolean EditViewHandleEvent (EventPtr event)
{
	UInt32 uniqueID;
	FormPtr frm;
	FieldPtr fld;
	Boolean deleted;
	Boolean handled = false;

	if (event->eType == keyDownEvent)
		{
		if (TxtCharIsHardKey(event->data.keyDown.modifiers, event->data.keyDown.chr))
			{
			}
		else if (EvtKeydownIsVirtual(event))
			{
			if (event->data.keyDown.chr == vchrPageUp)
				{
				EditViewPageScroll (winUp);
				handled = true;
				}
	
			else if (event->data.keyDown.chr == vchrPageDown)
				{
				EditViewPageScroll (winDown);
				handled = true;
				}
			}
		// Don't allow linefeeds in the subject field or in any of the filter 
		// fields.
		else if (event->data.keyDown.chr == chrLineFeed)
			{
			handled = (EditViewField == editSubjectField || 
				       (! EditViewEditRecord));
			}
		}


	else if (event->eType == ctlSelectEvent)
		{		
		switch (event->data.ctlSelect.controlID)
			{
			case EditDoneButton:
				if (EditViewEditRecord)
					{
					RestoreFromEditView = true;
					FrmGotoForm (NewView);
					}
				else
					FrmGotoForm (SyncDialog);
				handled = true;
				break;

			case EditLookupButton:
				fld = GetObjectPtr (EditField);
				AddressLookup (fld);
				handled = true;
				break;

			case EditSendButton:
				if (SendMessage ())
					{
					RestoreFromEditView = false;
					EditViewClose ();
					MessageViewCleanUpRevert();
					ClearEditState ();
					FrmGotoForm (ReturnToView);
					}
				handled = true;
				break;
			}
		}

	else if (event->eType == menuEvent)
		{
		handled = EditViewDoCommand (event->data.menu.itemID);
		}

		
	else if (event->eType == fldChangedEvent)
		{
		EditViewUpdateScrollBar ();
		}

	else if (event->eType == sclRepeatEvent)
		{
		EditViewScroll (event->data.sclRepeat.newValue - 
			event->data.sclRepeat.value);
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
		/* HandEra */
		if (vgaExists)
		    VgaFormModify(frm, vgaFormModify160To240);
		/* end HandEra */
		EditViewInit (frm);
		FrmDrawForm (frm);
		FrmSetFocus (frm, FrmGetObjectIndex (frm, EditField));
		handled = true;
		}
		
	else if (event->eType == frmSaveEvent)
		{
		// If we're editing an expense record then save the unique id  
		// of the unsent message.
		if (EditViewEditRecord)
			{
			DmRecordInfo (MailDB, NewRecord, NULL, &uniqueID, NULL);
			ClearEditState ();
			EditViewClose ();
			SaveDraftMessage (true, &deleted, false);
			if (deleted)
				uniqueID = 0;
			MailSetUnsentMessage (MailDB, uniqueID);
			}
		}

	else if (event->eType == frmCloseEvent)
		{
		EditViewClose ();
		}
    /* HandEra */
    else if (event->eType == displayExtentChangedEvent)
        {
        EditViewResize(true);
        handled = true;
        }
    /* end HandEra */

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewRestoreEditState
 *
 * DESCRIPTION: This routine restores the edit state of the new message
 *              view, if the view was in edit mode. This routine is 
 *              called after returning from the details dialog
 *              or the edit view.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art		6/18/96		Initial Revision
 *
 ***********************************************************************/
static void NewViewRestoreEditState (void)
{
	Int16 row;
	FormPtr frm;
	TablePtr table;
	FieldPtr fld;
	FieldAttrType 				attr;


	// Determine the current edit view,  the lookup function need this 
	// value set.
	switch (NewViewCurrentRowID)
		{
		case newToRow:			EditViewField = editToField;			break;
		case newCCRow:			EditViewField = editCCField;			break;
		case newBCCRow:		EditViewField = editBCCField;			break;
		case newSubjectRow:	EditViewField = editSubjectField;	break;
		case newBodyRow:		EditViewField = editBodyField;		break;
		}

	// Find the row that the current record is in.  Its possible 
	// that the current record is nolong displayable (ex: it 
	// was scrolled out of view).
	table = GetObjectPtr (NewTable);
	if ( ! TblFindRowID (table, NewViewCurrentRowID, &row) )
		{
//		ClearEditState ();
		return;
		}

	frm = FrmGetActiveForm ();
	FrmSetFocus (frm, FrmGetObjectIndex (frm, NewTable));
	TblGrabFocus (table, row, newTextColumn);
	
	// Restore the insertion point position.
	fld = TblGetCurrentField (table);

	if (ItemSelected)
		{
		FldSetInsertionPoint (fld, EditPosition);
		if (EditSelectionLength)
			FldSetSelection (fld, EditPosition, 
				EditPosition + EditSelectionLength);
		}

	FldGrabFocus (fld);


	// Restore the dynamiclly resizeable attribute that 
	// was cleared in NewViewGetField to allow drag selection.
	FldGetAttributes (fld, &attr);
	attr.dynamicSize = true;
	FldSetAttributes (fld, &attr);

}


/***********************************************************************
 *
 * FUNCTION:    NewViewClearEditState
 *
 * DESCRIPTION: This routine clears the edit state of the to do list.
 *              It is caled whenever a table item is selected.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/18/96	Initial Revision
 *			jmp	9/29/99	Use FrmGetFormPtr() & FrmGetObjectIndex() instead of
 *								GetObjectPtr() because GetObjectPtr() calls FrmGetActiveForm(),
 *								and FrmGetActiveForm() may return a form that isn't the one we
 *								want when other forms are up when we are called.
 *								Fixes bug #22418.
 *
 ***********************************************************************/
static void NewViewClearEditState (void)
{
	TablePtr table;
	FormPtr frm;

	frm = FrmGetFormPtr (NewView);
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, NewTable));
	TblReleaseFocus (table);
	ClearEditState ();
}


/***********************************************************************
 *
 * FUNCTION:    NewViewSaveEditState
 *
 * DESCRIPTION: This routine is called by the table object, as a callback  
 *              routine. We want to capture the current edit state here.
 *
 * PARAMETERS:  table  - pointer to the memo list table (TablePtr)
 *              row    - row of the table to draw
 *              column - column of the table to draw 
 *
 * RETURNED:    true if the table needs to be redrawn
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/18/96	Initial Revision
 *
 ***********************************************************************/
static Boolean NewViewSaveEditState (void * table, Int16 row, Int16 UNUSED_PARAM(column))
{
	Int16 rowID;
	UInt16 selectStart;
	UInt16 selectEnd;
	FieldPtr fld;
	
	fld = TblGetCurrentField (table);
	
	// Save the insertion point position, and length of the selection.  
	// We'll need the insertion point position an selection length
	// if we put the table back into edit mode. 
	EditPosition = FldGetInsPtPosition (fld);
	
	FldGetSelection (fld, &selectStart, &selectEnd);
	EditSelectionLength = selectEnd - selectStart;
	if (EditSelectionLength)
		EditPosition = selectStart;

	if (TblGetRowID (table, row) == newBodyRow)
		EditBodyPosition = EditPosition;
		

	if (FldDirty (fld) && (NewRecord != noNewRecord))
		DirtyRecord (NewRecord);


	// Get the current scroll position.
	rowID = TblGetRowID (table, row);
	NewScrollPosition [rowID] = FldGetScrollPosition (fld);


	return (false);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewGetField
 *
 * DESCRIPTION: This routine returns a pointer to a field of a mail 
 *              record.  This routine is called by the table 
 *              object as a callback routine when it wants to display or
 *              edit a mail message field.
 *
 * PARAMETERS:  table  - pointer to the to do list table (TablePtr)
 *              row    - row of the table
 *              column - column of the table
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	06/06/96	Initial Revision
 *			kwk	04/23/99	Reload fld attributes after calling FldSetScrollPosition. 
 *
 ***********************************************************************/
static Err NewViewGetField (void * table, Int16 row, Int16 UNUSED_PARAM(column),
	Boolean editable, MemHandle *textHP, UInt16 * textOffset, UInt16 * textAllocSize,
	FieldPtr fld)
{
	UInt16						rowID;
	Char *						textP;
	MemHandle					recordH;
	FieldAttrType 				attr;
	MailDBRecordType			record;
	

	// Get a pointer to the start of the requested field.
	MailGetRecord (MailDB, NewRecord, &record, &recordH);

	rowID = TblGetRowID (table, row);
	switch (rowID)
		{
		case newToRow:			textP = record.to;			break;
		case newCCRow:			textP = record.cc;			break;
		case newBCCRow:		textP = record.bcc;			break;
		case newSubjectRow:	textP = record.subject;		break;
		case newBodyRow:		textP = record.body;			break;
		default:
			ErrDisplay ("Invalid row");
		}


	// Set the maximum number of characters that may be input.
	FldSetMaxChars (fld, maxNewFieldChars);

	// Compute the offset within the record of the field requested, and the 
	// length the the field.
	*textOffset = textP - ((Char *) MemHandleLock (recordH));
	*textAllocSize = StrLen (textP) + 1;  			// one for null terminator
	*textHP = recordH;
	
	MemHandleUnlock (recordH);
	MemHandleUnlock (recordH);

	// Normally the table object would initial the text of the current
	// field,  but since we want to also set the scroll position of the 
	// text will do it outself.  The table will not initialize the field
	// if it already has a text value
//	if (rowID == newBodyRow)
		{
		FldSetText (fld, *textHP, *textOffset, *textAllocSize);
		if (editable)
			{
			FldGetAttributes (fld, &attr);
			attr.visible = false;
			FldSetAttributes (fld, &attr);

			FldSetScrollPosition (fld, NewScrollPosition[rowID]);
			
			// Remember to reload attributes, in case they've been changed
			// by the call to FldSetScrollPosition.
			FldGetAttributes (fld, &attr);
			attr.visible = true;
			FldSetAttributes (fld, &attr);
			}
		else
			FldSetScrollPosition (fld, NewScrollPosition[rowID]);
		}

	// Set the field to support auto-shift. Only the subject and message 
	// body are auto-shift fields.
	if (editable  &&  (rowID == newSubjectRow || rowID == newBodyRow))
		{
		FldGetAttributes (fld, &attr);
		attr.autoShift = true;
		FldSetAttributes (fld, &attr);
		}


	// We need to clear the dynamiclly resizeable attribute
	// so that we can select text in the to, cc, bcc, and 
	// subject fields.  These fields are resizeable but we 
	// limit there maximum height to a value less than the 
	// table routines allow, this results in the field routines
	// attemping to make the field fully visible on a tblEnter
	// event and preventing drag selecting.  We restore the 
	// dynamiclly resizeable attribute after the tblEnter event
	// in precessed.
	if (editable  &&  rowID != newBodyRow)
		{
		FldGetAttributes (fld, &attr);
		attr.dynamicSize = false;
		FldSetAttributes (fld, &attr);
		}
	

	return (0);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewDrawLabel
 *
 * DESCRIPTION: This routine the draw the labels in the edit view. 
 *              This routine is called by the table routine 
 *              TblDrawTable each time a line of the table needs to
 *              be drawn.
 *
 * PARAMETERS:  table  - pointer to the memo list table (TablePtr)
 *              row    - row of the table to draw
 *              column - column of the table to draw 
 *              bounds - bound to the draw region
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/6/96		Initial Revision
 *
 ***********************************************************************/
static void NewViewDrawLabel (void * table, Int16 row, Int16 UNUSED_PARAM(column), 
	RectanglePtr bounds)
{
	Int16 i;
	Int16 x;
	UInt16 len;
	Int16 rowID;
	Int16 width;
	Char ch;
	FontID curFont;
	Char * label;
	MemHandle resH;
	
	rowID = TblGetRowID (table, row);

	resH = DmGetResource (strRsc, newViewLabelsStrId);
	label = MemHandleLock (resH);
	for (i = 1; i <= rowID; i++)
		label = StrChr (label, commaChr) + 1;
	len = (UInt16) (StrChr (label, commaChr) - label);

    /* HandEra */
    if ((vgaExists) && VgaIsVgaFont(EditFont))
    	curFont = FntSetFont (VgaBaseToVgaFont(stdFont));
    else
    	curFont = FntSetFont (stdFont);
    /* end HandEra */

	width = FntCharsWidth (label, len);
	x = bounds->topLeft.x + bounds->extent.x - width - FntCharWidth (colonChr);
	WinDrawChars (label, len, x, bounds->topLeft.y);
	x += width;

	ch = colonChr;
	WinDrawChars (&ch, 1, x, bounds->topLeft.y);

	FntSetFont (curFont);

	MemHandleUnlock (resH);
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewNextMessage
 *
 * DESCRIPTION: This routine displays the next mail message.  If the 
 *              last message in the category is currently displayed then 
 *					 the first message is shown.  
 *
 * PARAMETERS:  direction to find the next message
 *
 * RETURNED:    true if a message is found
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *			roger	6/17/97	Added scrolling backwards
 *
 ***********************************************************************/
static Boolean MessageViewNextMessage (Int16 direction)
{
	UInt16	recordNum;
	Boolean messageFound;

	// Find the next record
	recordNum = CurrentRecord;
	messageFound = SeekRecord (&recordNum, 1, direction);
	
	if (!messageFound && MessagesInCategory > 1)
		{
		if (direction == dmSeekBackward)
			{
			//go to the last record in the category
			//we are at the first record, seek MessagesInCategory - 1 records forward
			messageFound = SeekRecord (&recordNum, MessagesInCategory - 1, dmSeekForward);
			}
		else
			{
			//go to the first record in the category
			//we are at the last record, seek MessagesInCategory - 1 records backwards
			messageFound = SeekRecord (&recordNum, MessagesInCategory - 1, dmSeekBackward);
			}
		}
	
	if (messageFound)
		{
		CurrentRecord = recordNum;
//update the buttons
//mgmg mg add this code
		MessageViewEnableButtons(FrmGetActiveForm ());
//end code
		MessageViewDrawMessage ();
		}
		
	return messageFound;
}


/***********************************************************************
 *
 * FUNCTION:    NewViewUpdateScrollBar
 *
 * DESCRIPTION: This routine visually updates the new message view's scroll
 *              bar.
 *
 * PARAMETERS:  table  -  pointer to the new message tabel
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	12/12/96	Initial Revision
 *
 ***********************************************************************/
static void NewViewUpdateScrollBar (void)
{
	UInt16 width;
	UInt16 lines;
	Int16 rowID;
	UInt16 pos;
	Int16 rowLines;
	UInt16 maxLines;
	Int16 row, column;
	Int16 pageSize;
	Int16 maxValue;
	FontID currFont;
	Char * textP;
	TablePtr table;
	FieldPtr fld;
	MemHandle recordH;
	ScrollBarPtr bar;
	RectangleType r;
	MailDBRecordType	record;

	currFont = FntSetFont (EditFont);

	table = GetObjectPtr (NewTable);


	// Copute the page size, 
	TblGetBounds (table, &r);
	pageSize = (r.extent.y / FntLineHeight ()) - 1;


	// Compute the height of all the rows
	MailGetRecord (MailDB, NewRecord, &record, &recordH);
	width = TblGetColumnWidth (table, newTextColumn);


	// If we're editing the body field then get scroll position within the
	// body text.
	if (TblEditing(table))
		{
		TblGetSelection (table, &row, &column);
		if (TblGetRowID (table, row) == newBodyRow)
			{
			fld = TblGetCurrentField (table);				
			NewScrollPosition [newBodyRow] = FldGetScrollPosition (fld);
			}
	 	}


	// Get the scroll values of the body row.
	FntGetScrollValues (record.body, width, NewScrollPosition [newBodyRow],
	 	&lines, &pos);
	TblFindRowID (table, newBodyRow, &row);
	rowLines = TblGetRowHeight (table, row) / FntLineHeight ();


	/* CS2 (43540): The scroll logic is a little odd, but it makes sense once you think
	   about it.  Here goes.  The body field can be autoscrolled, if you drag across it.
	   When this happens, it does -not- resize in the message form.  Therefore, we don't
	   want to count the lines that are scrolled out of view when we're calculating the
	   scroll bar values.  -However-, if the body field is the topmost displayed row in
	   the table, then when you scroll up, you will scroll up through the body field
	   first, then the rows above it will be added to the table once you reach the top.
	   SO, if the body field is not the top row, we subtract off the number of lines
	   currently scrolled out of view from the total number of lines.  Whew.
	*/
	//if ( 0 != row )
	//	{
	//	lines -= pos;
	//	pos = 0;
	//	}


	lines = max (lines, rowLines);


	for (rowID = newToRow; rowID < newBodyRow; rowID++)
		{
		if (rowID == newBCCRow && (! NewViewShowBCC))
			continue;
		
		//is the row visible?
		if (TblFindRowID (table, rowID, &row))
			{
			rowLines = TblGetRowHeight (table, row) / FntLineHeight ();
			}
		else
			{
			switch (rowID)
				{
				case newToRow:
					textP = record.to;
					maxLines = maxNewAddressLines;
					break;
				case newCCRow:
					textP = record.cc;
					maxLines = maxNewAddressLines;
					break;
				case newBCCRow:
					textP = record.bcc;
					maxLines = maxNewAddressLines;
					break;
				case newSubjectRow:
					textP = record.subject;
					maxLines = maxNewSubjectLines;
					break;
				}				
			rowLines = min (FldCalcFieldHeight (textP, width), maxLines);
			pos += rowLines;
			}
			
		lines += rowLines;
		}

	
	if (lines > pageSize + 1)
		{
		maxValue = lines - pageSize - 1;
		}
	else
		{
		maxValue = 0;
		pos = 0;
		}


	bar = GetObjectPtr (NewScrollBar);
	SclSetScrollBar (bar, pos, 0, maxValue, pageSize);

	MemHandleUnlock (recordH);
	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewScroll
 *
 * DESCRIPTION: This routine scrolls the new message view.  It is called 
 *              when the scroll bar sends a scroll event or when the 
 *              winUp or doww button is pressed.
 *
 * PARAMETERS:  linesToScroll - line to scroll, if < 0 then scroll winUp,
 *                              if > 0  then scroll winDown.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/17/96	Initial Revision
 *
 ***********************************************************************/
static void NewViewScroll (Int16 linesToScroll)
{
	Int16							i;
	Int16							row;
	UInt16							lines;
	UInt16							rowID;
	Int16							width;
	Int16							length;
	Int16							height;
	Int16							columnWidth;
	Int16 						value;
	Int16 						minValue;
	Int16 						maxValue, maxDelta;
	Int16 						pageSize;
	FontID						currFont;
	Char *						chars;
	TablePtr						table;
	MemHandle						recordH;
	ScrollBarPtr 				bar;
	RectangleType				r;
	MailDBRecordType			record;
	
	currFont = FntSetFont (EditFont);

	table = GetObjectPtr (NewTable);	

	TblReleaseFocus (table);

	MailGetRecord (MailDB, NewRecord, &record, &recordH);
	
	rowID = TblGetRowID (table, 0);

	columnWidth = TblGetColumnWidth (table, newTextColumn);

	maxDelta = 0;

	// Scroll winUp.
	if (linesToScroll < 0)
		{
		if (rowID == newBodyRow && NewScrollPosition [newBodyRow])
			{
			lines = -linesToScroll;
			FntWordWrapReverseNLines (record.body, columnWidth, &lines, 
				&NewScrollPosition [newBodyRow]);
			linesToScroll += lines;
			}

		while (linesToScroll < 0 && rowID> newToRow)
			{
			TblInsertRow (table, 0);
			if (rowID-1 == newBCCRow && (! NewViewShowBCC))
				rowID -= 2;
			else
				rowID--;
			
			height = NewViewGetFieldHeight (NewRecord, rowID, columnWidth);
			NewViewInitRow (table, 0, rowID, height);

			linesToScroll += height / FntLineHeight();
			}
		}



	// Scroll winDown.
	else if (linesToScroll > 0)
		{
		while (linesToScroll > 0 && rowID < newBodyRow)
			{
			height = TblGetRowHeight (table, 0);
			TblRemoveRow (table, 0);
			linesToScroll -= height / FntLineHeight ();;
			rowID++;
			if (rowID == newBCCRow && (! NewViewShowBCC))
				rowID++;

			/* CS2 (43540): While scrolling down, if we cause the body row to hit the top,
			   we have to resynch the scroll bar, because when we start scrolling back up,
			   we will go through the body field first.  If the body field happens to be
			   scrolled, this will cause the scrollbar to jump and the thumb to resize.
			   Looks odd, but it keeps things in synch.
			*/
			if ( newBodyRow == rowID )
				{
					maxDelta = NewScrollPosition[newBodyRow];
				}
			}
		
		if (linesToScroll > 0)
			{
			width = TblGetColumnWidth (table, newTextColumn);
			while (linesToScroll)
				{
				chars = &record.body[NewScrollPosition [newBodyRow]];
				length = FntWordWrap (chars, width);
				if (! length) break;
	
				NewScrollPosition [newBodyRow] += length;
				linesToScroll--;
				}

			TblMarkRowInvalid (table, 0);
			}
		}

	MemHandleUnlock (recordH);


	// Reinitialize the message row so that it has the correct height.
	TblFindRowID (table, newBodyRow, &row);
	TblGetBounds (table, &r);
	height = r.extent.y;
	for (i = 0; i < row; i++)
		height -= TblGetRowHeight (table, i);
	NewViewInitRow (table, row, newBodyRow, height);

	if ( 0 != maxDelta )
		{
		/* CS2 (43540): Do the complete job of recalculating the scroll bar. */
		NewViewUpdateScrollBar();
		}
	// The "to", "cc", "bcc", and "subject" fields alway scroll there entire
	// height when moved on or off the display.  This causes the scroll bar 
	// to be out of sync with what is displayed.
	else if ( linesToScroll != 0 )
		{
		bar = GetObjectPtr (NewScrollBar);
		SclGetScrollBar (bar, &value, &minValue, &maxValue, &pageSize);
		value -= linesToScroll;
		SclSetScrollBar (bar, value, minValue, maxValue, pageSize);
		}
	

	TblMarkTableInvalid (table);	
	TblRedrawTable (table);

	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewPageScroll
 *
 * DESCRIPTION: This routine scrolls the message creation view winUp
 *              or winDown by a page.
 *
 * PARAMETERS:  direction     winUp or winDown
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	11/14/96	Initial Revision
 *
 ***********************************************************************/
static void NewViewPageScroll (WinDirectionType direction)
{
	UInt16 linesToScroll;
	Int16 value;
	Int16 minValue;
	Int16 maxValue;
	Int16 pageSize;
	FontID currFont;
	TablePtr table;
	ScrollBarPtr bar;
	RectangleType r;


	// Determine if we should scroll at all.
	bar = GetObjectPtr (NewScrollBar);
	SclGetScrollBar (bar, &value, &minValue, &maxValue, &pageSize);
	if (direction == winUp)
		{
		if (value == minValue)
			return;
		}
	else
		{
		if (value == maxValue)
			return;
		}


	// Copute the page size, 
	currFont = FntSetFont (EditFont);
	table = GetObjectPtr (NewTable);
	TblGetBounds (table, &r);
	pageSize = (r.extent.y / FntLineHeight ());
	FntSetFont (currFont);


	// Scroll the view.
	linesToScroll = pageSize - 1;
	if (direction == winUp)
		{
		if (linesToScroll > value - minValue)
			linesToScroll = value - minValue;
		linesToScroll = -linesToScroll;
		}
	else
		{
		if (linesToScroll > maxValue - value)
			linesToScroll = maxValue - value;
		}
	NewViewScroll (linesToScroll);

	NewViewRestoreEditState ();

	// Update the scroll bar.
	value += linesToScroll;
	SclSetScrollBar (bar, value, minValue, maxValue, pageSize);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewNextField
 *
 * DESCRIPTION: If a field is being edited, advance the focus to the
 *              next or previous field in the table.
 *
 * PARAMETERS:  direction - winDown (next) or winUp (previous)
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	7/27/95	Initial Revision
 *
 ***********************************************************************/
static void NewViewNextField (WinDirectionType direction)
{
	Int16 row;
	Int16 sRow;
	Int16 rowID;
	Int16 column;
	TablePtr table;
	FieldPtr fld;
	FieldAttrType attr;
	UInt16 zeroRowID;
	ScrollBarPtr bar;
	Int16 value, min, max, pageSize;
	Int16 columnWidth;
	

	// Find out which field is being edited.
	table = GetObjectPtr (NewTable);
	if (! TblEditing (table))
		return;
	
	TblGetSelection (table, &row, &column);
	rowID = TblGetRowID (table, row);
	sRow = row;
	if (direction == winDown)
		{
		if (rowID >= newBodyRow)
			{
			rowID = 0;
			
			//find the rowID...the table may need to be scrolled
			//get the rowID of the 0 row. then figure out how many to scroll
			zeroRowID = TblGetRowID (table, sRow);
			sRow = newToRow - zeroRowID;
			
			bar = GetObjectPtr (NewScrollBar);
			SclGetScrollBar (bar, &value, &min, &max , &pageSize);
			if (max > 0)
				{
				sRow = value * -1;
				}
			//add the number of lines of body that is above the screen
			//mgmgmg
			}
		else
			{
			row++;
			sRow = row;
			}
		}
	else //UP
		{
		//if the row id is 0 then we want to wrap around and select the body row...
		if (rowID == 0)
			{
			TblFindRowID (table, newBodyRow, &row);
			sRow = row;
			}
		else if (rowID >= newBodyRow)
			{
			//is the subject row visible?
			TblFindRowID (table, newSubjectRow, &row);
			row = TblGetRowID (table, row);
			if (row == newSubjectRow)
				{
				//yes
				sRow --;
				}
			else
				{
				//subject is not visible...how far winUp is it?
				bar = GetObjectPtr (NewScrollBar);
				SclGetScrollBar (bar, &value, &min, &max , &pageSize);
				columnWidth = TblGetColumnWidth (table, newTextColumn);
				
				value -= NewViewGetLineCount (NewRecord, newToRow, columnWidth);
				value -= NewViewGetLineCount (NewRecord, newCCRow, columnWidth);
				value -= NewViewGetLineCount (NewRecord, newSubjectRow, columnWidth);
				if (NewViewShowBCC)
					value -= NewViewGetLineCount (NewRecord, newBCCRow, columnWidth);
				
				sRow = (value + 1) * -1;
				}
			}		
		else	
			//select the previous row
			sRow--;
		}
	
	if (sRow < 0)
		{
		//scroll the table winUp, and select the first row
		NewViewScroll (sRow);
		NewViewUpdateScrollBar ();
		sRow = 0;
		}
	
	NewViewClearEditState ();
	
	NewViewCurrentRowID = TblGetRowID (table, sRow);

	NewViewRestoreEditState ();

	fld = TblGetCurrentField (table);
	if (fld)
		{
		FldGetAttributes (fld, &attr);
		if (! fld->attr.insPtVisible)
			{
			FldMakeFullyVisible (fld);
			}
		}

/*
	if (NewViewCurrentRowID == newBodyRow)
		{
		if (TblGetRowID (table, 0) != newBodyRow)
			{
			FieldPtr fld;
			FieldAttrType attr;

			}
		}
*/
	

	ItemSelected = true;
/*
	Int16 row;
	Int16 rowID;
	Int16 column;
	TablePtr table;
	
	
	// Find out which field is being edited.
	table = GetObjectPtr (NewTable);
	if (! TblEditing (table))
		return;
	
	TblGetSelection (table, &row, &column);
	rowID = TblGetRowID (table, row);
	if (direction == winDown)
		{
		if (rowID >= newBodyRow)
			row = 0;
		else
			row++;
		}
	else
		{
		if (row == 0)
			TblFindRowID (table, newBodyRow, &row);
		else
			row--;
		}

	NewViewClearEditState ();
	
	NewViewCurrentRowID = TblGetRowID (table, row);

	NewViewRestoreEditState ();

	ItemSelected = true;
*/
}


/***********************************************************************
 *
 * FUNCTION:    NewViewResizeBodyField
 *
 * DESCRIPTION: This routine sets the height of the body field to:
 *              the height of the table minus the height of any other
 *              visible lines.
 *              
 * PARAMETERS:  table - pointer to a table object.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art		12/13/96	Initial Revision
 *			mgolden	10/6/97		changed for the rocky fonts
 *
 ***********************************************************************/
static void NewViewResizeBodyField (TablePtr table)
{
	Int16 i;
	Int16 row, maxRows;
	UInt16 height;
	RectangleType r;
	FieldPtr fld;
	
	//new mg
	if (!TblFindRowID (table, newBodyRow, &row))
		{
		//show the body field
		maxRows = TblGetNumberOfRows(table);
		for ( row = 0; row < maxRows; row ++)
			{
			if (TblGetRowID (table, row) == newBodyRow)
				break;
			}
		if (row == maxRows)
			{
			//this is an error...what should we do here?
			return;
			}
			
		//set the row usable
		TblSetRowUsable  ( table, row, true);
		}
	
	TblGetBounds (table, &r);
	height = r.extent.y;
	for (i = 0; i < row; i++)
		height -= TblGetRowHeight (table, i);

	if (TblGetRowHeight (table, row) != height)
		TblMarkRowInvalid (table, row);				

	TblSetRowHeight (table, row, height);
	
	//do we need to resize the field? (only if we are editing the body!)
	if (NewViewCurrentRowID == newBodyRow)
		{
		fld = TblGetCurrentField (table);
		
		if (fld->rect.extent.y != height)
			{
			RctCopyRectangle (&(fld->rect), &r);
			r.extent.y = height;
			FldSetBounds (fld, &r);
			}
		}//if
			
	//do we need to resize the field?
	//
	//FldSetInsPtPosition (&table->currentField, 
	//	event->data.fldHeightChanged.currentPos);

	
}


/***********************************************************************
 *
 * FUNCTION:    NewViewResizeFieldHeight
 *
 * DESCRIPTION: This routine is called when the height of a line in
 *              the Edit View is changed as a result of user input.
 *              If the new height of the field is shorter,  more items
 *              may need to be added to the top of the list.
 *              
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/6/96	Initial Revision
 *
 ***********************************************************************/
static void NewViewResizeFieldHeight (EventPtr event)
{
	Int16				row, i;
	Int16				column;
	UInt16				newRowID;
	UInt16				rowID;
	Int16				height;
	Int16				freeSpace;
	Int16				newHeight;
	Int16				maxHeight;
	Int16				columnWidth;
	FontID			currFont;
	Boolean			insPtState;
	FieldPtr			fld;
	TablePtr 		table;
	RectangleType	fieldR;
	

	// Get the current height of the field;
	fld = event->data.fldHeightChanged.pField;
	FldGetBounds (fld, &fieldR);

	// Get the id of the current row.
	table = GetObjectPtr (NewTable);
	TblGetSelection (table, &row, &column);
	NewViewCurrentRowID = TblGetRowID (table, row);
	rowID = NewViewCurrentRowID;

	columnWidth = TblGetColumnWidth (table, newTextColumn);

	// Is the field's height contracting?
	if (event->data.fldHeightChanged.newHeight < fieldR.extent.y)
		{
		// if there are no items scrolled off the top of the table, 
		// scroll the text of the field winUp.
		if (TblGetRowID (table, 0) == 0)
			{
			if (rowID == newBodyRow)
				{
				goto doBodyRowThing;
				//return;
				}
			else
				{
				TblHandleEvent (table, event);

				// Add the freed space to the "body" field.
				NewViewResizeBodyField (table);				
				TblRedrawTable (table);
				}
			}

		
		// Add an item to the table to fill in the space made available by the 
		// shortening the field. This is because as the field shortens, we may 
		// now have space to show the fields above it...picture the bodfy field 
		// is expanded to fill the screen, as the user deletes lines, and once
		// it is small enough not to fill the screen, the field dire3ctly above 
		// the body (the subject) should now be visible. (ie scroll into view)
		else
			{
			freeSpace = fieldR.extent.y - event->data.fldHeightChanged.newHeight;
			newRowID = TblGetRowID (table, 0);
			
			while (newRowID > newToRow && freeSpace)
				{
				// Add a row to the top of the table an initialize it.
				if (newRowID-1 == newBCCRow && (! NewViewShowBCC))
					newRowID -= 2;
				else
					newRowID--;
				height = NewViewGetFieldHeight (NewRecord, newRowID, columnWidth);
				if (height > freeSpace)
					break;
	
				// If there room to scroll a field onto the screen then do it.
				TblInsertRow (table, 0);
				NewViewInitRow (table, 0, newRowID, height);
				
				freeSpace -= height;
				}

			if (freeSpace != fieldR.extent.y - event->data.fldHeightChanged.newHeight)
				{
				// Reinitialize the current row so that it has the correct size.
				if (rowID != newBodyRow)
					{
					height = NewViewGetFieldHeight (NewRecord, rowID, columnWidth);
					TblFindRowID (table, rowID, &row);
					NewViewInitRow (table, row, rowID, height);
					}

				// Add the freed space to the "body" field.
				NewViewResizeBodyField (table);

				insPtState = InsPtEnabled ();
				InsPtEnable (false);

				TblMarkTableInvalid (table);
				TblRedrawTable (table);
				
				InsPtEnable (insPtState);
				}
			}
		}		

	// The field's height is expanding, have the table object resize the field.
	// OR the field is the body field and we need to do the stuff below anyway.
	else 
		{
doBodyRowThing:

		currFont = FntSetFont (EditFont);
		newHeight = event->data.fldHeightChanged.newHeight;

		if (rowID == newSubjectRow)
			maxHeight = maxNewSubjectLines * FntLineHeight ();
		else
			maxHeight = maxNewAddressLines * FntLineHeight ();

		if (rowID != newBodyRow && 
			 newHeight > maxHeight &&
			 TblGetRowHeight (table, row) >= maxHeight)
			{
			FldSetInsPtPosition (fld, event->data.fldHeightChanged.currentPos);
			InsPtEnable (true);
			}
		else
			{
			if (rowID != newBodyRow && newHeight > maxHeight)
				event->data.fldHeightChanged.newHeight = maxHeight;
			else if (rowID == newBodyRow  && row == 0)
				{
				//set the max height based on the font 
				maxHeight = table->bounds.extent.y;
				 
				if (row == 0)
					{
					//get the max size of the body field
					// subtract the fields above it
					for (i = 0; i < row; i++)
						maxHeight -= TblGetRowHeight (table, i);
					}
									
				//is the new height larger than the size that the table want's to grow?
				if (newHeight > maxHeight)
					// set the new size to the max Size
					event->data.fldHeightChanged.newHeight = maxHeight;
				}			
			
			//in all cases...make sure that the .newHeight is not > than the table height!
			if (event->data.fldHeightChanged.newHeight > table->bounds.extent.y)
				event->data.fldHeightChanged.newHeight = table->bounds.extent.y;
				
			TblHandleEvent (table, event);

			NewViewResizeBodyField (table);
			
			fld = TblGetCurrentField (table);				
			NewScrollPosition [rowID] = FldGetScrollPosition (fld);
			
			if (rowID != newBodyRow)
				{
				TblMarkTableInvalid (table);
				TblRedrawTable (table);
				}
			}
		FntSetFont (currFont);
		}
	
	NewViewUpdateScrollBar ();
}


/***********************************************************************
 *
 * FUNCTION:    NewViewItemSelected
 *
 * DESCRIPTION: This routine is called when an item in the new table is
 *              selected.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static Boolean NewViewItemSelected (EventPtr event)
{
	Int16 rowID;
	TablePtr table;
	Boolean handled = false;

	table = event->data.tblSelect.pTable;
	rowID = TblGetRowID (table, event->data.tblSelect.row);


	// Keep track of which row we're in with the global variable.  We
	// need this information to draw the correct title on view such as
	// the Lookup View.
	switch (rowID)
		{
		case newToRow:			EditViewField = editToField;			break;
		case newCCRow:			EditViewField = editCCField;			break;
		case newBCCRow:		EditViewField = editBCCField;			break;
		case newSubjectRow:	EditViewField = editSubjectField;	break;
		case newBodyRow:		EditViewField = editBodyField;		break;
		}


	// If we've changed row then store the new row in a global variable,
	// we need this information so that we can later restore the edit
	// state of the tabe.
	// If the item selected is a label clear the edit state.
	if (NewViewCurrentRowID != rowID)
		{
		if (event->data.tblSelect.column == newLabelColumn)
			{
			ClearEditState ();
			NewViewCurrentRowID = rowID;
			if (rowID != newBodyRow)
				NewViewRestoreEditState ();
			else
				{
				EditPosition = EditBodyPosition;
				EditSelectionLength = 0;
				}
			}
		else 
			NewViewCurrentRowID = rowID;
		}


	if (event->data.tblSelect.column == newLabelColumn)
		{
		
		EditViewEditRecord = true;
		FrmGotoForm (EditView);
		handled = true;
		}

	ItemSelected = true;

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewDoCommand
 *
 * DESCRIPTION: This routine preforms the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    true if the menu command was handled
 *
 * HISTORY:
 *		06/26/96	art	Created by Art Lamb.
 *		08/28/00	kwk	Use new FrmGetActiveField trap.
 *
 ***********************************************************************/
static Boolean NewViewDoCommand (UInt16 command)
{
	Int16 row;
	Int16 column;
	Boolean deleted;
	FieldPtr fld;
	TablePtr table;
	Boolean handled = true;

	switch (command)
		{
		case newSaveDraftMenuCmd:
			NewViewClearEditState ();
			if (SaveDraftMessage (true, &deleted, true))
				//mgmg
				//FrmGotoForm (ListView);
				FrmGotoForm (ReturnToView);
			break;

		case newLookupMenuCmd:
			table = GetObjectPtr (NewTable);
			if (TblEditing (table))
				{
				TblGetSelection (table, &row, &column);
				switch (TblGetRowID (table, row))
					{
					case newToRow:
					case newCCRow:
					case newBCCRow:
						fld = FrmGetActiveField (NULL);
						AddressLookup (fld);
						break;
					
					default:
						SndPlaySystemSound (sndError);
					}
				}
			else
				SndPlaySystemSound (sndError);
			break;

		case newPreferencesMenuCmd:
			FrmPopupForm (PrefDialog);		
			break;
						
		case newHotSyncMenuCmd:
			FrmPopupForm (SyncDialog);
			break;
						
		case newPurgeMenuCmd:
			PurgeDeletedRecord ();
			break;
						
		case newAboutMenuCmd:
			AbtShowAbout (sysFileCMail);
			break;
		
		case newChangeFontMenuCmd:
			NewViewToggleFont();
			break;	
			
		default:
			handled = false;
		}	

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewGetLineCount
 *
 * DESCRIPTION: This routine returns the height, in # of rows, of a field 
 *              in a mail message.
 *
 * PARAMETERS:  recordNum - record index
 *              width     - width of description
 *
 * RETURNED:    height in pixels
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			MGolden	11/28/96	Initial Revision
 *
 ***********************************************************************/
static UInt16 NewViewGetLineCount (UInt16 recordNum, Int16 rowID, UInt16 width)
{
	UInt16 height;
	UInt16 maxLines;
	FontID curFont;
	Char * textP;
	MemHandle recordH;
	MailDBRecordType record;
	
	MailGetRecord (MailDB, recordNum, &record, &recordH);
	ErrFatalDisplayIf ((! recordH), "Record not found");
	
	// Compute the height of the to do item's description.
	curFont = FntSetFont (EditFont);

	switch (rowID)
		{
		case newToRow:				
			textP = record.to;
			maxLines = maxNewAddressLines;
			break;
			
		case newCCRow:
			textP = record.cc;
			maxLines = maxNewAddressLines;
			break;
			
		case newBCCRow:
			textP = record.bcc;
			maxLines = maxNewAddressLines;
			break;
			
		case newSubjectRow:
			textP = record.subject;
			maxLines = maxNewSubjectLines;
			break;
			
		case newBodyRow:
			textP = record.body;
			maxLines = maxFieldLines;
			break;
		}

	height = FldCalcFieldHeight (textP, width);
	height = min (height, maxLines);
	
	FntSetFont (curFont);

	MemHandleUnlock (recordH);

	return (height);
}

/***********************************************************************
 *
 * FUNCTION:    NewViewGetFieldHeight
 *
 * DESCRIPTION: This routine returns the height, in pixels, of a field 
 *              in a mail message.
 *
 * PARAMETERS:  recordNum - record index
 *              width     - width of description
 *
 * RETURNED:    height in pixels
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			MGolden	11/28/96	Initial Revision
 *
 ***********************************************************************/
static UInt16 NewViewGetFieldHeight (UInt16 recordNum, Int16 rowID, UInt16 width)
{
	UInt16 height;
	UInt16 maxLines;
	FontID curFont;
	Char * textP;
	MemHandle recordH;
	MailDBRecordType record;
	
	MailGetRecord (MailDB, recordNum, &record, &recordH);
	ErrFatalDisplayIf ((! recordH), "Record not found");
	
	// Compute the height of the to do item's description.
	curFont = FntSetFont (EditFont);

	switch (rowID)
		{
		case newToRow:				
			textP = record.to;
			maxLines = maxNewAddressLines;
			break;
			
		case newCCRow:
			textP = record.cc;
			maxLines = maxNewAddressLines;
			break;
			
		case newBCCRow:
			textP = record.bcc;
			maxLines = maxNewAddressLines;
			break;
			
		case newSubjectRow:
			textP = record.subject;
			maxLines = maxNewSubjectLines;
			break;
			
		case newBodyRow:
			textP = record.body;
			maxLines = maxFieldLines;
			break;
		}

	height = FldCalcFieldHeight (textP, width);
	height = min (height, maxLines);
	height *= FntLineHeight ();
	
	FntSetFont (curFont);

	MemHandleUnlock (recordH);

	return (height);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewHandleKey
 *
 * DESCRIPTION: This routine handles special processing of key events
 *              in the New View.  If a key event is received while the 
 *              body field has the focus and the insertion point in not 
 *              visible, then the table needs to be scrolled
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	12/15/96	Initial Revision
 *
 ***********************************************************************/
static Boolean NewViewHandleKey (EventPtr event)
{
	Boolean handled = false;
	FieldPtr fld;
	TablePtr table;
	FieldAttrType attr;

	if (TxtCharIsHardKey(event->data.keyDown.modifiers, event->data.keyDown.chr))
		return (false);

	if (NewViewCurrentRowID == newBodyRow)
		{
		table = GetObjectPtr (NewTable);
		if (TblGetRowID (table, 0) != newBodyRow)
			{
			fld = TblGetCurrentField (table);
			if (fld)
				{
				FldGetAttributes (fld, &attr);
				if ( ! fld->attr.insPtVisible)
					{
					if (FldMakeFullyVisible (fld))
						{
						handled = true;
						EvtAddEventToQueue (event);
						}
					}
				}
			}
		}
	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    NewViewInitRow
 *
 * DESCRIPTION: This routine initializes a row of the "Edit View" table.
 *
 * PARAMETERS:  table    - pointer to a table object
 *              row      - row to initialize
 *              rowID    - id of row
 *              height   - row height in pixels
 *
 * RETURNED:    tnothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art			6/6/96		Initial Revision
 *			MGolden		10/3/97		added rocky font code
 *
 ***********************************************************************/
static void NewViewInitRow (TablePtr table, Int16 row, Int16 rowID, UInt16 height)
{
	
	TblSetItemStyle (table, row, newLabelColumn, customTableItem);
	TblSetItemStyle (table, row, newTextColumn, textTableItem);
	
	
	TblSetRowUsable (table, row, true);
	
	TblSetRowID (table, row, rowID);

	TblSetRowHeight (table, row, height);
	
	// font code
	TblSetItemFont (table, row, newLabelColumn, stdFont);
	TblSetItemFont (table, row, newTextColumn, EditFont);
}


/* TRG */
/***********************************************************************
 *
 * FUNCTION:    NewViewResize
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void NewViewResize(Boolean draw)
{
    FormPtr         frmP;
    Coord           xDiff, yDiff;
    Coord           x,y;
    TablePtr        tableP;
    Boolean         editing;
    RectangleType   r;

    WinGetDisplayExtent(&x, &y);

    // this will get the old extent
    // resize form
    frmP = FrmGetActiveForm();
    FrmGetFormBounds(frmP, &r);
    
    xDiff = x - (r.topLeft.x + r.extent.x);
    yDiff = y - (r.topLeft.y + r.extent.y);
    if ((xDiff == 0) && (yDiff == 0))
        return;

    // resize form
    r.extent.x += xDiff;
    r.extent.y += yDiff;
    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);


    // Need to erase the table first, otherwise it will erase moved controls
    // when shrinking it.  Also, FrmHideObject doesn't do anything for tables.
    tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, NewTable));
    editing = TblEditing(tableP);
    if (draw)
    {
        TblReleaseFocus(tableP);
        TblEraseTable(tableP);
    }
    
	TblGetBounds(tableP, &r);
	r.extent.x += xDiff;
	r.extent.y += yDiff;
    gNewViewTableHeight = r.extent.y;
    TblSetBounds(tableP, &r);
    
//    x = TblGetColumnWidth(tableP, newTextColumn);
//    TblSetColumnWidth(tableP, newTextColumn, x + xDiff);
    TblSetColumnWidth(tableP, newTextColumn, r.extent.x - (TblGetColumnWidth(tableP, newLabelColumn) + spaceAfterLabel));

    if(draw && (yDiff < 0))
    {
	    r.topLeft.y = (r.topLeft.y + r.extent.y);
	    r.extent.y = -yDiff;
        WinEraseRectangle (&r, 0);
    }


    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, NewSendButton),    0, yDiff, draw);
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, NewCancelButton),  0, yDiff, draw);
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, NewDetailsButton), 0, yDiff, draw);

    ToolsMoveObject(frmP, ToolsFrmGetGSI(frmP), xDiff, yDiff, draw);

    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, NewScrollBar),   xDiff, 0, false);
    ToolsResizeObject(frmP, FrmGetObjectIndex(frmP, NewScrollBar), 0, yDiff, draw);
    
    if (draw)
    {
        NewViewLoadTable(frmP);
        FrmDrawForm(frmP);
        if (editing)
            NewViewRestoreEditState();
    }

}
/* end TRG */

/***********************************************************************
 *
 * FUNCTION:    NewViewInit
 *
 * DESCRIPTION: This routine initializes the "Edit View" of the 
 *              Mail application.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has MemHandle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		6/5/96	Initial Revision
 *			MGolden	7/7/97	add some space after the label column
 *			mgolden	10/6/97		changed for the rocky fonts
 *
 ***********************************************************************/
static void NewViewInit (FormPtr frm)
{
	MemHandle recordH;
	MailDBRecordType record;
	TablePtr table;
	
    NewViewResize(false);

	MailGetRecord (MailDB, NewRecord, &record, &recordH);
	
	if (RestoreFromEditView)
		{
		//check the stashed NewViewShowBCC 
		NewViewShowBCC = StashedNewViewShowBCC;
		RestoreFromEditView = false;
		}


	// If the record has a BCC field, make sure it is visible.
	if (!NewViewShowBCC)
		{
		NewViewShowBCC = (*record.bcc != 0);
		}

	// If we'er replying to or forwarding a message then set the insertion 
	// point's position in the body field to the start of the text, otherwise
	// place the insertion point at the end.
	if (CurrentCategory == inboxCategory || CurrentCategory == filedCategory)
		EditBodyPosition = 0;
	else
		EditBodyPosition = StrLen (record.body);

	MemHandleUnlock (recordH);

	//remember the table height...
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, NewTable));
	gNewViewTableHeight = table->bounds.extent.y;

	NewViewLoadTable(frm);
	}

/***********************************************************************
 *
 * FUNCTION:    NewViewLoadTable
 *
 * DESCRIPTION: Loads the rows of the table (broken out from NewViewInit)
 *				so the change font code can re-load the table
 *
 * PARAMETERS:  FormPtr
 *
 * RETURNED:    nothing.
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden		10/6/97		initial revision
 *			mgolden	10/6/97		changed for the rocky fonts
 *
 ***********************************************************************/
static void NewViewLoadTable(FormPtr frm)
	{
	Int16 i;
	Int16 row;
	Int16 rowID;
	UInt16 height;
	UInt16 bodyHeight;
	Int16 columnWidth;
	Int16 rowsInTable;
	TablePtr table;
	RectangleType tableR;
	FontID		currFont;
	
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, NewTable));
	
	/* HandEra */
	if (vgaExists)
	    VgaTableUseBaseFont(table, !VgaIsVgaFont(EditFont));
	/* end HandEra */

	//set the bounds of the table, to a direct multiple of the current fontHeight	
	currFont = FntSetFont (EditFont);
	table->bounds.extent.y = (gNewViewTableHeight / FntLineHeight ()) * FntLineHeight ();
	FntSetFont (currFont);
	
	columnWidth = TblGetColumnWidth (table, newTextColumn);
	TblGetBounds (table, &tableR);
	bodyHeight = tableR.extent.y;

	rowID = newToRow;
	
	//rowsInTable =  NewViewNumberOfRows (table);
	rowsInTable = TblGetNumberOfRows (table);
	for (row = 0; row < rowsInTable; row++)
		{
		if (rowID <= newBodyRow)
			{
			if (rowID < newBodyRow)
				{
				height = NewViewGetFieldHeight (NewRecord, rowID, columnWidth);
				bodyHeight -= height;
				}
			else
				height = bodyHeight;

			NewViewInitRow (table, row, rowID, height);

			if (rowID+1 == newBCCRow && (! NewViewShowBCC))
				rowID += 2;
			else
				rowID++;
			}
		else
			TblSetRowUsable (table, row, false);
		}

	TblSetColumnUsable (table, newLabelColumn, true);	
	TblSetColumnUsable (table, newTextColumn, true);	

	// add some space after the label column
	TblSetColumnSpacing (table, newLabelColumn, spaceAfterLabel);

	// Set the callback routine that will draw the labels.
	TblSetCustomDrawProcedure (table, newLabelColumn, NewViewDrawLabel);

	// Set the callback routines that will load and save fields 
	// of the mail message.
	TblSetLoadDataProcedure (table, newTextColumn, (TableLoadDataFuncPtr) NewViewGetField);
	TblSetSaveDataProcedure (table, newTextColumn, (TableSaveDataFuncPtr) NewViewSaveEditState);

	TblHasScrollBar (table, true);
	
	//init the scroll position
	for (i = newToRow; i <= newBodyRow; i++)
		NewScrollPosition [i] = 0;
		
	NewViewUpdateScrollBar ();
	}

/***********************************************************************
 *
 * FUNCTION:    NewViewHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Edit View"
 *              of the Mail application.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/5/96	Initial Revision
 *			MGolden	7/14/947 added MessageViewCleanUpRevert
 *			CSS	06/22/99	Standardized keyDownEvent handling
 *								(TxtCharIsHardKey, commandKeyMask, etc.)
 *
 ***********************************************************************/
static Boolean NewViewHandleEvent (EventPtr event)
{
	UInt32 uniqueID;
	FormPtr frm;
	FieldPtr fld;
	TablePtr table;
	Boolean deleted;
	Boolean handled = false;
	FieldAttrType attr;
	

	if (event->eType == keyDownEvent)
		{
		if (TxtCharIsHardKey(event->data.keyDown.modifiers, event->data.keyDown.chr))
			{
			}
		else if (EvtKeydownIsVirtual(event))
			{
			// Scroll winUp key presed?
			if (event->data.keyDown.chr == vchrPageUp)
				{
				NewViewPageScroll (winUp);
				handled = true;
				}
	
			// Scroll winDown key presed?
			else if (event->data.keyDown.chr == vchrPageDown)
				{
				NewViewPageScroll (winDown);
				handled = true;
				}
	
			// Move the focus to the next field ?
			else if (event->data.keyDown.chr == vchrNextField)
				{
				NewViewNextField (winDown);
				handled = true;
				}
	
			// Move the focus to the previous field ?
			else if (event->data.keyDown.chr == vchrPrevField)
				{
				NewViewNextField (winUp);
				handled = true;
				}

			// DOLATER CSS - Figure out if we still need to call this guy to
			// process other virtual characters.
			else 
				handled = NewViewHandleKey (event);
			}
			
		// Don't allow linefeeds in the subject field.
		else if (event->data.keyDown.chr == chrLineFeed)
			handled = (NewViewCurrentRowID == newSubjectRow);

		else 
			handled = NewViewHandleKey (event);
		}
		

	else if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case NewSendButton:
				if (SendMessage ())
					{
					MessageViewCleanUpRevert();
					ClearEditState ();
					FrmGotoForm (ReturnToView);
					}
				else
					{
					FrmUpdateForm (NewView, updateReinitialize);
					}
				handled = true;
				break;

			case NewCancelButton:
				if (SaveDraftMessage (false, &deleted, false))
					{
					MessageViewCleanUpRevert();
					ClearEditState ();
					FrmGotoForm (ReturnToView);
					}
				else
					NewViewRestoreEditState ();
				handled = true;
				break;

			case NewDetailsButton:
				FrmPopupForm (DetailsDialog);
				handled = true;
				break;
			}
		}


	else if (event->eType == ctlExitEvent)
		{
		switch (event->data.ctlExit.controlID)
			{
			case NewSendButton:
			case NewCancelButton:
			case NewDetailsButton:
				NewViewRestoreEditState ();
				break;
			}
		}


	else if (event->eType == fldHeightChangedEvent)
		{
		NewViewResizeFieldHeight (event);
		handled = true;
		}


	else if (event->eType == fldChangedEvent)
		{
		NewViewUpdateScrollBar ();
		handled = true;
		}


	else if (event->eType == tblEnterEvent)
		{
		if (event->data.tblEnter.column == newTextColumn)
			{
			table = event->data.tblEnter.pTable;
			if (TblEditing (table))
				{
				fld = TblGetCurrentField (table);
				FldGetAttributes (fld, &attr);
				attr.dynamicSize = false;
				FldSetAttributes (fld, &attr);
				}
			
			TblHandleEvent (table, event);
			handled = true;

			// Restore the dynamiclly resizeable attribute that 
			// was cleared in NewViewGetField to allow drag selection.
			fld = TblGetCurrentField (table);
			FldGetAttributes (fld, &attr);
			attr.dynamicSize = true;
			FldSetAttributes (fld, &attr);

			/* CS2 (43540): when the body field is auto-scrolled, the scrollbar can get
			   out of synch.  We can't let that happen!
			 */
			NewViewUpdateScrollBar();
			}
		}


	else if (event->eType == tblSelectEvent)
		{
		handled = NewViewItemSelected (event);
		}
		
		
	else if (event->eType == tblExitEvent)
		{
		NewViewRestoreEditState ();
		handled = true;
		}


	else if (event->eType == menuEvent)
		{
		handled = NewViewDoCommand (event->data.menu.itemID);
		}

		
	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
		if (vgaExists)
		    VgaFormModify(frm, vgaFormModify160To240);
/* end HandEra */
		NewViewInit (frm);
		FrmDrawForm (frm);

		NewViewRestoreEditState ();
		handled = true;
		}


	else if (event->eType == frmUpdateEvent)
		{
		if (event->data.frmUpdate.updateCode == updateRestoreFocus)
			{
			NewViewRestoreEditState ();
			handled = true;
			}
		else if (event->data.frmUpdate.updateCode == updateReinitialize)
			{
			frm = FrmGetActiveForm ();
			table = GetObjectPtr (NewTable);
			TblEraseTable (table);
			ClearEditState ();
			
			NewViewCurrentRowID = editToField;
			NewViewInit (frm);
			TblDrawTable (table);
			NewViewRestoreEditState ();
			handled = true;
			}
		}


	else if (event->eType == frmSaveEvent)
		{
		// Save the unique id  of the unsent message if there is one.
		DmRecordInfo (MailDB, NewRecord, NULL, &uniqueID, NULL);
		NewViewClearEditState ();
		SaveDraftMessage (true, &deleted, false);
		if (deleted)
			uniqueID = 0;
		MailSetUnsentMessage (MailDB, uniqueID);
		}


	else if (event->eType == frmCloseEvent)
		{
		StashedNewViewShowBCC = NewViewShowBCC;
		NewViewShowBCC = false;
		}


	else if (event->eType == sclRepeatEvent)
		{
		NewViewScroll (event->data.sclRepeat.newValue - 
			event->data.sclRepeat.value);
		}


	else if (event->eType == sclExitEvent)
		{
		//mgmg mg
		//why is this here?
		NewViewRestoreEditState ();
		}
/* HandEra */
    else if (event->eType == displayExtentChangedEvent)
        {
        NewViewResize(true);
        handled = true;
        }
/* end HandEra */
	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewMarkRead
 *
 * DESCRIPTION: This routine marks the current message as read.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/27/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewMarkRead (void)
{
	MemHandle						recordH;
	MailDBRecordType			record;
	MailChangedFieldsType	changedFields;
	

	if (CurrentCategory != inboxCategory)
		return;
		
	// Get the current record
	MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
	
	// If the current record hasn't been read yet, mark it read.
	if (! record.flags.read)
		{
		MemHandleUnlock (recordH);
		record.flags.read = true;
		MemSet (&changedFields, sizeof (changedFields), 0);
		changedFields.flags = true;
		MailChangeRecord (MailDB, &CurrentRecord, &record, changedFields);
		MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
		DirtyRecord (CurrentRecord);
		UnreadMessages--;
		}

	MemHandleUnlock (recordH);
}

/***********************************************************************
 *
 * FUNCTION:    MessageViewMarkUnRead
 *
 * DESCRIPTION: This routine marks the current message as unread.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden	7/10/97	Initial Revision
 *
 ***********************************************************************/
static void MessageViewMarkUnRead (void)
	{		
	MemHandle						recordH;
	MailDBRecordType			record;
	MailChangedFieldsType	changedFields;
	

	if (CurrentCategory != deletedCategory)
		return;
		
	// Get the current record
	MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
	
	// If the current record hasn't been read yet, mark it read.
	if (record.flags.read)
		{
		MemHandleUnlock (recordH);
		record.flags.read = false;
		MemSet (&changedFields, sizeof (changedFields), 0);
		changedFields.flags = true;
		MailChangeRecord (MailDB, &CurrentRecord, &record, changedFields);
		MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
		DirtyRecord (CurrentRecord);
		UnreadMessages++;
		}

	MemHandleUnlock (recordH);
	}//MessageViewMarkUnRead


/***********************************************************************
 *
 * FUNCTION:    MessageViewUpdateScrollBar
 *
 * DESCRIPTION: This routine update the scroll bar.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/4/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewUpdateScrollBar (void)
{
	UInt16 topLine;
	UInt16 textHeight;
	UInt16 pageHeight;
	Int16 maxValue;
	ScrollBarPtr bar;

	bar = GetObjectPtr (MessageScrollBar);
	MsgGetScrollValues (&textHeight,  &pageHeight, &topLine);

	if (textHeight > pageHeight)
		maxValue = textHeight - pageHeight;
	else
		maxValue = 0;

//the topLine & textHeight needs to be re-caclulated!



	SclSetScrollBar (bar, topLine, 0, maxValue, pageHeight-1);
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewScroll
 *
 * DESCRIPTION: This routine scrolls the message view by the number of
 *              lines specified.
 *
 * PARAMETERS:  direction - winUp or dowm
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/4/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewScroll (Int16 linesToScroll)
{
	if (linesToScroll < 0)
		MsgScroll (-linesToScroll, winUp);

	else if (linesToScroll > 0)
		MsgScroll (linesToScroll, winDown);
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewPageScroll
 *
 * DESCRIPTION: This routine scrolls the message a page winUp or winDown
 * or to the next message if scrolling past the end.
 *
 * PARAMETERS:   direction     winUp or winDown
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art		6/4/96	Initial Revision
 *			roger	6/17/97	Added scrolling to the next message
 *			MGolden	7/28/97	Reversed rogers scroll dirctions
 *
 ***********************************************************************/
static void MessageViewPageScroll (WinDirectionType direction)
{
	UInt16 objIndex;
	Int16 value;
	Int16 min;
	Int16 max;
	Int16 pageSize;
	UInt16 linesToScroll;
	FontID currFont;
	FormPtr frm;
	ScrollBarPtr bar;
	RectangleType bounds;
	Boolean movedToAnotherMessage;


	// Compute the page size.
	frm = FrmGetActiveForm ();
	objIndex = FrmGetObjectIndex (frm, MessageGadget);
	FrmGetObjectBounds (frm, objIndex, &bounds);
	currFont = FntSetFont (MessageFont);
	linesToScroll = (bounds.extent.y / FntLineHeight ()) - 1;
	FntSetFont (currFont);

	
	// Update the scroll bar.
	bar = GetObjectPtr (MessageScrollBar);
	SclGetScrollBar (bar, &value, &min, &max, &pageSize);

	if (direction == winUp)
		{
		value -= linesToScroll;
		if (value < min)
			{
			linesToScroll -= min - value;
			value = min;
			}
		}
	else
		{
		value += linesToScroll;
		 if (value > max)
		 	{
			linesToScroll -= value - max;
			value = max;
			}
		}

	if (linesToScroll)
		{
		MsgScroll (linesToScroll, direction);
		SclSetScrollBar (bar, value, min, max, pageSize);
		}
	else
		{
		// Scroll to the next message
		
		if (direction == winUp)
			movedToAnotherMessage = MessageViewNextMessage(dmSeekBackward);
		else
			movedToAnotherMessage = MessageViewNextMessage(dmSeekForward);
		
		if (movedToAnotherMessage)
			SndPlaySystemSound (sndInfo);
		}
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewChangeHeader
 *
 * DESCRIPTION: This routine toggles beteewn the abbreviated and 
 *					 full header display.
 *
 * PARAMETERS:  controlID - font button pressed (small or large).
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/4/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewChangeHeader (UInt16 controlID)
{
	if (controlID == MessageFullHeaderButton)
		{
		ShowFullHeader = true;
		MessageTopField = msgFieldTo;
		}
	else
		{
		ShowFullHeader = false;
		MessageTopField = msgFieldSentTo;
		}
		
	// Reset to the top of the message.
	MessageScrollPos = 0;

	MsgErase ();
	MsgDraw ();	

	MessageViewUpdateScrollBar ();

	MessageViewMarkRead ();
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewToggleFont
 *
 * DESCRIPTION: This routine changes the font used to display a note.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden	7/79/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewToggleFont ()
	{
	FontID originalFont = MessageFont;
	
	MessageFont =  FontSelect (originalFont);
		
	if (MessageFont == originalFont)
		return;
		
	MsgChangeFont ();

	MessageViewUpdateScrollBar();

	MessageViewMarkRead ();

	}//MessageViewToggleFont

/***********************************************************************
 *
 * FUNCTION:    MessageViewSetTitle
 *
 * DESCRIPTION: This routine sets the title of the note view.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		06/4/96	Initial Revision
 *			MGolden	07/14/97	Set the title to the current category name
 *									Set the message x of y text
 *			MGolden	08/11/97	Left Justified the message next to
 * 								the title.
 *			kwk		05/28/99	Use const Char* for form label text.
 *			kwk		06/28/99	Fixed bug w/passing obj ID vs. index to FrmGetObjectBounds
 *									and FrmSetObjectPosition.
 *								
 ***********************************************************************/
 static void MessageViewSetTitle (void)
	 {
	UInt16 len;
	Char * title;
	FontID currFont;
	MemHandle titleH;
	Char		countStr [6];
	Char *	titleTemplateP;
	Char *	tmp;
	Char *	MemPtr;
	FormPtr	frm;
	Int16 	total;
	UInt16 	labelIndex;
	UInt16	titleWidth = 0;
	RectangleType r;

	currFont = FntSetFont (boldFont);
	
	//set the title to the catgory name
	titleH = MemHandleNew (dmCategoryLength);
	if (titleH)
		{
		title = MemHandleLock (titleH);

		CategoryGetName ( MailDB, CurrentCategory, title);

		titleWidth = FntCharsWidth (title, StrLen(title));

		FrmSetTitle (FrmGetActiveForm (), title);
		}
	
	//free the old title MemHandle
	if (MessageTitleH)
		MemHandleFree (MessageTitleH);

	MessageTitleH = titleH;
	
	FntSetFont (currFont);
	
	//now set the label's "Message # of %"
	titleTemplateP = MemHandleLock (DmGetResource (strRsc, MessageTitleStrID));
	
	//get the position of this item in the category
	StrIToA (countStr, DmPositionInCategory ( MailDB, CurrentRecord, CurrentCategory) + 1);
	len = StrLen (countStr) + StrLen (titleTemplateP);

	tmp = MemPtrNew (len + 1);
	ErrNonFatalDisplayIf (!tmp, "Out of memory");

	MemPtr = StrChr (titleTemplateP, '#');
	len = MemPtr - titleTemplateP;
	if (len)
		StrNCopy (tmp, titleTemplateP, len);
	tmp[len] = 0;
	StrCat (tmp, countStr);	
	MemPtr++;
	StrCat (tmp, MemPtr);

	//now title has the message #...add the total count of the category
	total = DmNumRecordsInCategory (MailDB, CurrentCategory);
	StrIToA (countStr, total);
	len = StrLen (countStr) + StrLen (tmp);

	title = MemPtrNew (len + 1);
	ErrFatalDisplayIf (!title, "Out of memory");
	
	MemPtr = StrChr (tmp, '%');
	len = MemPtr - tmp;
	if (len)
		StrNCopy (title, tmp, len);
	title[len] = 0;
	StrCat (title, countStr);	
	MemPtr++;
	StrCat (title, MemPtr);
	
	//free the tmp MemPtr
	MemPtrFree(tmp);

	frm = FrmGetActiveForm ();
	labelIndex = FrmGetObjectIndex(frm, MessageTitleLabel);

	// erase the old label in case it gets shorter
	if (FrmVisible(frm))
		{
		FrmGetObjectBounds(frm, labelIndex, &r);
		WinEraseRectangle(&r, 0);
		}

	//move the label if needed
	if (titleWidth > 0)
		FrmSetObjectPosition(frm, labelIndex, titleWidth + kMessageTitleOffset, r.topLeft.y);
		
	//set the label text
	FrmCopyLabel ( frm, MessageTitleLabel, title);

	MemPtrUnlock(titleTemplateP);
	MemPtrFree(title);


/* OLDWAY
 	Int16 maxWidth;
	UInt16 len;
	UInt16 width;
	Char * title;
	Char * subject;
	FontID currFont;
	MemHandle titleH;
	MemHandle recordH;
	MailDBRecordType record;
	
	currFont = FntSetFont (boldFont);

	titleH = MemHandleNew (maxMessageTitleLen + 2);
	if (titleH)
		{
		title = MemHandleLock (titleH);
		
		maxWidth = maxMessageTitleLenWidth;
		width = 0;
		*title = 0;
	
		MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
		subject = record.subject;
	
		if (*subject)
			{
			for (len = 0; len < maxMessageTitleLen; len++)
				{
				if (subject[len] == linefeedChr) break;
				
				title[len] = subject[len];
				title[len+1] = 0;
		
				width += FntCharWidth (subject[len]);		
				if (width >= maxWidth) break;
				}
		
			if ( (width >= maxWidth) && (subject[len] != linefeedChr) )
				{
				maxWidth -= FntCharWidth (chrEllipsis);
				while (width > maxWidth)
					{
					width -= FntCharWidth (title[len]);
					len--;
					}
				title[len+1] = chrEllipsis;
				title[len+2] = 0;
				}
			}
			
		else
			{
			subject = MemHandleLock (DmGetResource (strRsc, noSubjectStrID));
			StrCopy (title, subject);
			MemPtrUnlock (subject);
			}
	
		MemHandleUnlock (recordH);
	
		FrmSetTitle (FrmGetActiveForm (), title);
		}

	if (MessageTitleH)
		MemHandleFree (MessageTitleH);

	MessageTitleH = titleH;

	FntSetFont (currFont);
*/
 }
 

/***********************************************************************
 *
 * FUNCTION:    MessageViewDrawMessage
 *
 * DESCRIPTION: This routine displays the next mail message.  If the 
 *              last message in the category is currently displayed then 
 *					 the first message is shown.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewDrawMessage (void)
{
	// Set and draw the title.
	MessageViewSetTitle ();

	// Reset to the top of the message.
	if (ShowFullHeader)
		MessageTopField = msgFieldTo;
	else
		MessageTopField = msgFieldSentTo;
	MessageScrollPos = 0;

	MessageSelectStartField = msgSelectNone;
	MessageSelectEndField = msgSelectNone;
	MessageSelectStartPos = 0;
	MessageSelectEndPos = 0;


	// Draw the message.
	MsgErase ();
	MsgDraw ();	

	MessageViewUpdateScrollBar ();	
	
	MessageViewMarkRead ();
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewDeleteMessage
 *
 * DESCRIPTION: This routine deletes a the current message from the 
 *              inbox.  It does not delete the record from the database.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the message view should be exited
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/4/96	Initial Revision
 *			MGolden	9/97	changed the UI so that the user always go 
 *							back to the list view after a delete.
 *
 ***********************************************************************/
static Boolean MessageViewDeleteMessage (void)
{
	UInt16	attr;
	UInt16	recordNum;
	
	if (CurrentCategory == deletedCategory)
		return (false);

	if (ConfirmDelete)
		{
		if (FrmAlert(DeleteMessageAlert) != DeleteMessageYes)
			return (false);
		}

	// Move the record to the "deleted" category and mark it dirty.
	DmRecordInfo (MailDB, CurrentRecord, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	attr |= deletedCategory | dmRecAttrDirty;
	DmSetRecordInfo (MailDB, CurrentRecord, &attr, NULL);

	MessagesInCategory--;
	ErrNonFatalDisplayIf (MessagesInCategory < 0, "invalid message count");


	// Find the next record
	recordNum = CurrentRecord;
	if (! SeekRecord (&recordNum, 0, dmSeekForward))
		{
		// Find the prior record.
		if (! SeekRecord (&recordNum, 0, dmSeekBackward))
			{
			//mgmg
			//clear the current record, we are going back to the 
			//list view and the current record is not in the category eny more
			CurrentRecord = noRecordSelected; 
			return (true);
			}
		}
	CurrentRecord = recordNum;
	return true;
	
	//MessageViewDrawMessage ();
	//return (false);
	}

/***********************************************************************
 *
 * FUNCTION:    MessageUndelete
 *
 * DESCRIPTION: Move the current message to the inbox, 
 * 						and mark it as unread.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden	7/10/97	Initial Revision
 *			MGolden	7/11/97	Removed the seek next & prev since we want to just
 *									return to the list view.
 *
 ***********************************************************************/
static Boolean MessageUndelete()
	{
	MemHandle				recordH;
	MailDBRecordType	record;
	UInt16					attr;
	Err					err = 0;
	//UInt16					recordNum;
	
	
	//get the record
	MailGetRecord (MailDB, CurrentRecord, &record, &recordH);

	// Change the records category to "inbox" and mark it dirty.
	DmRecordInfo (MailDB, CurrentRecord, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	attr |= inboxCategory | dmRecAttrDirty;
	DmSetRecordInfo (MailDB, CurrentRecord, &attr, NULL);
			
	MemHandleUnlock (recordH);

	//don't mark the message unread...it will cause sync problems.
	//MessageViewMarkUnRead ();
	MessageViewMarkRead ();
	
	MessagesInCategory--;
	ErrNonFatalDisplayIf (MessagesInCategory < 0, "invalid message count");

#ifdef NEEDED
	// If the record was deleted go to the next record.  If there is 
	// no next record go to the prior record.
	recordNum = CurrentRecord;
	if (! SeekRecord (&recordNum, 0, dmSeekForward))
		{
		// Find the prior record.
		if (! SeekRecord (&recordNum, 0, dmSeekBackward))
			return (true);
		}
	CurrentRecord = recordNum;

	MessageViewDrawMessage ();

	return (false);
#endif
	return (true);
	
	//I think we should alert the users to tell them it's in the inbox now...
	//Or change the category to the inbox and select this record
	//MGMG

	}//MessageUndelete
	
/***********************************************************************
 *
 * FUNCTION:    MessageViewFileMessage
 *
 * DESCRIPTION: This routine files a the current message, optionally 
 *              deleting it from the inbox.  It does not delete the 
 *              record from the database.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the message view should be exited
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/5/96	Initial Revision
 *			MGolden	9/9/97	added returnToLlist bool If
 *							the message is changing categories, then return to the
 *							list view...
 ***********************************************************************/
static Boolean MessageViewFileMessage (void)
{
	Err					err = 0;
	UInt16					attr;
	UInt16					button;
	UInt16					recordNum;
	Boolean				copyRecord = false;
	MemHandle				recordH;
	MailDBRecordType	record;
	Boolean				returnToList = true;

	if (CurrentCategory == inboxCategory)
		{
		button = FrmAlert(FileMessageAlert);
	
		if (button == FileMessageCancel)
			return (false);

		else if (button == FileMessageYes)
			copyRecord = true;
		}

	// If we're going to leave the message in the inbox, make a copy of the
	// record.
	if (copyRecord)
		{
		returnToList = false;
		MailGetRecord (MailDB, CurrentRecord, &record, &recordH);

		err = MailNewRecord (MailDB, &record, &recordNum);

		MemHandleUnlock (recordH);

		if (err)
			{
			FrmAlert (DeviceFullAlert);
			return (false);
			}
		}
	else
		recordNum = CurrentRecord;


	// Change the records category to "filed" and mark it dirty.
	DmRecordInfo (MailDB, recordNum, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	attr |= filedCategory | dmRecAttrDirty;
	DmSetRecordInfo (MailDB, recordNum, &attr, NULL);


	// If the record was deleted go to the next record.  If there is 
	// no next record go to the prior record.
	if (! copyRecord)
		{
		MessagesInCategory--;
		ErrNonFatalDisplayIf (MessagesInCategory < 0, "invalid message count");

		recordNum = CurrentRecord;
		if (! SeekRecord (&recordNum, 0, dmSeekForward))
			{
			// Find the prior record.
			if (! SeekRecord (&recordNum, 0, dmSeekBackward))
				return (true);
			}
		CurrentRecord = recordNum;
	
		//we a re going to the list view, we do not need to draw the record
		//MessageViewDrawMessage ();
		}

	return returnToList;
	}


/***********************************************************************
 *
 * FUNCTION:    MessageViewUnsendMessage
 *
 * DESCRIPTION: This routine removes the current message from the inbox
 *              and makes it an unsend message.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true if the note was deleted.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/5/96	Initial Revision
 *			MGolden	7/14/97	What do to if the message was in the deleted folder?
 ***********************************************************************/
static Boolean MessageViewUnsendMessage (void)
	{
	UInt16	attr;
	
	//mgmg
	// if the current category is "deleted", save a copy of the message.
	// Since this is called only from the edit button, it's safe to assume that if the category is filedCategory,
	// then it is an editable message, not a reply ot message.
	if (CurrentCategory == deletedCategory 
		|| CurrentCategory == outboxCategory 
		|| CurrentCategory == draftCategory
		|| CurrentCategory == filedCategory)
		{
		MessageViewInitializRevert();
		}
	
		
	// Change the records category to "filed" and mark it dirty.
	DmRecordInfo (MailDB, CurrentRecord, &attr, NULL, NULL);
	attr &= ~dmRecAttrCategoryMask;
	attr |= draftCategory | dmRecAttrDirty;
	DmSetRecordInfo (MailDB, CurrentRecord, &attr, NULL);

	if (CurrentCategory != draftCategory)
		MessagesInCategory--;
	ErrNonFatalDisplayIf (MessagesInCategory < 0, "invalid message count");

	NewRecord = CurrentRecord;

	return (true);
	}

/***********************************************************************
 *
 * FUNCTION:    MessageViewRevert
 *
 * DESCRIPTION: This routine reverts the index record to the 
 *				global copy of the record
 *
 * PARAMETERS:  database & current record
 *
 * RETURNED:    error code.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			MGolden	7/14/97		Initial Revision
 ***********************************************************************/
static Err MessageViewRevert(DmOpenRef dbP, UInt16 * index)
	{
	MailChangedFieldsType	changedFields;
	MailDBRecordType record;
	MailPackedDBRecordPtr src;
	Err err;
	UInt16	attr;
	
	if (!MessageOriginalRecordData)
		return -1;
	
	//set all of the change fields	
	changedFields.date = 1;
	changedFields.time = 1;
	changedFields.flags = 1;
	changedFields.subject = 1;
	changedFields.from = 1;
	changedFields.to = 1;
	changedFields.cc = 1;
	changedFields.bcc = 1;
	changedFields.replyTo = 1;
	changedFields.sentTo = 1;
	changedFields.body = 1;
	
	//unpack the record copy into a recordPtr
	src = (MailPackedDBRecordPtr) MemHandleLock (MessageOriginalRecordData);
	MailUnpackRecord (src, &record);
	
	err = MailChangeRecord (dbP, index, &record, changedFields);
	MemHandleUnlock (MessageOriginalRecordData);
	MemHandleFree(MessageOriginalRecordData);
	MessageOriginalRecordData = NULL;
	
	//set the record back to the original category
	DmRecordInfo (MailDB, *index, &attr, NULL, NULL);		
	attr &= ~(dmRecAttrCategoryMask | dmRecAttrDirty);
	attr |= MessageRevertCategory;
	DmSetRecordInfo (MailDB, *index, &attr, NULL);

	if (CurrentCategory != draftCategory)
		MessagesInCategory ++;
	
	NewRecord = noNewRecord;
	MailSetUnsentMessage (MailDB, 0);
	
	//we want to back to thelist view now...
	ReturnToView = ListView;
	
	return err;
	}//MessageViewRevert

/***********************************************************************
 *
 * FUNCTION:    MessageViewInitializRevert
 *
 * DESCRIPTION: This routine stores a copy of the current record, in case of a revert.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    error code.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			MGolden	7/14/97		Initial Revision
 ***********************************************************************/
static void MessageViewInitializRevert()
	{
	MemHandle				recordH;
	MailDBRecordType	record;
	Err err;
	MailPackedDBRecordPtr src, dest;
	
	//if there was one already, free it
	if (MessageOriginalRecordData)
		{
		MemHandleFree(MessageOriginalRecordData);
		MessageOriginalRecordData = NULL;
		}
			
	MailGetRecord (MailDB, CurrentRecord, &record, &recordH);

	//make a copy of the record
	MessageOriginalRecordData = MemHandleNew(MemHandleSize(recordH));
	MemHandleUnlock(recordH);
	
	//lock winDown both Handles
	src = (MailPackedDBRecordPtr) MemHandleLock (recordH);
	dest = (MailPackedDBRecordPtr) MemHandleLock (MessageOriginalRecordData);
	
	//copy the data into the new MemHandle
	err = MemMove( dest, src, MemHandleSize(recordH));
	
	//unlock the handles
	MemHandleUnlock(recordH);
	MemHandleUnlock(MessageOriginalRecordData);
	
	MessageRevertCategory = CurrentCategory;	
	}//MessageViewInitializRevert

/***********************************************************************
 *
 * FUNCTION:    RestoreRevert
 *
 * DESCRIPTION: Find if a record was stored in the database for revert...
 * 				It would be the only record in the ignoreCategory. If it exists, 
 *				get a MemHandle to it's data, then remove the record.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing.
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden		7/14/97		Initial Revision
 ***********************************************************************/
static void RestoreRevert(Boolean keepIt)
	{
	UInt16 theRecordID, numInCategory;
	MemHandle			recordH;
	MailDBRecordType	record;
	MailPackedDBRecordPtr src, dest;
	Err err;
	UInt16				attr;
	
	MessageOriginalRecordData = NULL;
	
	numInCategory = DmNumRecordsInCategory ( MailDB, ignoreCategory);

		
	if (numInCategory == 0)
		return;

/*
{
//debug
char tmp[32];
StrIToA(tmp, (long)numInCategory);
FrmCustomAlert ( 2009, "RestoreRevert", "numInCategory" , tmp);
}
*/
	theRecordID = 0;
	err = DmSeekRecordInCategory (MailDB, &theRecordID, 0, dmSeekForward, ignoreCategory);
	if (err)
		return;
	

	if (!keepIt)
		{
		DmRemoveRecord (MailDB, theRecordID);	
		return;
		}

	
	//remove any revert data & return
	//set the record back to the original category
	DmRecordInfo (MailDB, theRecordID, &attr, NULL, NULL);		
	attr &= ~(dmRecAttrCategoryMask | dmRecAttrDirty);
	attr |= MessageRevertCategory;
	DmSetRecordInfo (MailDB, theRecordID, &attr, NULL);

	//if found, read it and delete it
	MailGetRecord (MailDB, theRecordID, &record, &recordH);

	//make a copy of the record
	MessageOriginalRecordData = MemHandleNew(MemHandleSize(recordH));
	MemHandleUnlock(recordH);
	
	//lock winDown both Handles
	src = (MailPackedDBRecordPtr) MemHandleLock (recordH);
	dest = (MailPackedDBRecordPtr) MemHandleLock (MessageOriginalRecordData);
	
	//copy the data into the new MemHandle
	err = MemMove( dest, src, MemHandleSize(recordH));
	
	//unlock the handles
	MemHandleUnlock(recordH);
	MemHandleUnlock(MessageOriginalRecordData);
	
	//remove the record
	DmRemoveRecord (MailDB, theRecordID);	
	
	CurrentCategory = MessageRevertCategory;
	}//RestoreRevert
	
/***********************************************************************
 *
 * FUNCTION:    SaveRevert
 *
 * DESCRIPTION: Store the revert MemHandle in the databasein the ignoreCategory,
 *				Mark it deleted (Archived), move it to the end of the database.
 *
 * PARAMETERS:  none
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden	7/14/97	Initial Revision
 *			grant		7/7/99	Free MessageOriginalRecordData handle once the data is
 *									added to the DB.
 ***********************************************************************/
static void SaveRevert()
	{
	Err err;
	UInt16 attr;
	UInt16 theNewRecord, numRecords;
	MailPackedDBRecordPtr src;
	MemHandle h;
	MailDBRecordType	record;
	
	if (!MessageOriginalRecordData)
		return;
		
	h = MessageOriginalRecordData;
	src = (MailPackedDBRecordPtr) MemHandleLock (h);	
	MailUnpackRecord (src, &record);
	
	// Add a new record into the database.
	err = MailNewRecord (MailDB, &record, &theNewRecord);
	if (err)
		return;
	MemHandleUnlock(h);
	
	// MessageOriginalRecordData has been added to DB, so release the memory
	MemHandleFree(h);
	MessageOriginalRecordData = NULL;
	
	// Put the new record in the ignore category.
	DmRecordInfo (MailDB, theNewRecord, &attr, NULL, NULL);
	attr &= ~(dmRecAttrCategoryMask | dmRecAttrDirty);
	attr |= ignoreCategory;
	DmSetRecordInfo (MailDB, theNewRecord, &attr, NULL);

	numRecords = DmNumRecords (MailDB);
	//DmArchiveRecord (MailDB, theNewRecord);
	DmMoveRecord (MailDB, theNewRecord, numRecords);	
	}//SaveRevert

/***********************************************************************
 *
 * FUNCTION:    MessageViewCleanUpRevert
 *
 * DESCRIPTION: This routine removed the data stored by the revert code.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    error code.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			MGolden	7/14/97		Initial Revision
 ***********************************************************************/
static void MessageViewCleanUpRevert()
	{
	//if there was one already, free it
	if (MessageOriginalRecordData)
		{
		MemHandleFree(MessageOriginalRecordData);
		MessageOriginalRecordData = NULL;
		}
	}//MessageViewCleanUpRevert

/***********************************************************************
 *
 * FUNCTION:    MessageViewDoCommand
 *
 * DESCRIPTION: This routine preforms the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		6/4/96	Initial Revision
 *			MGolden	7/7/97	Added the change font menu
 *			MGolden	7/21/97	Set the ReturnToView when the user selects NEW
 *							So we will come back to the mesage that they were editing
 *							when they are done with the new message.
 *
 ***********************************************************************/
static void MessageViewDoCommand (UInt16 command)
{
	switch (command)
		{
		case messageNewMenuCmd:
			if (CreateRecord ())
				{
				//Set the ReturnToView when the user selects NEW
				ReturnToView = MessageView;
				FrmGotoForm (NewView);
				}
			break;
		
		case messageFileMenuCmd:
			if (CurrentCategory != filedCategory)
				{
				if (MessageViewFileMessage())
					FrmGotoForm (ListView);
				}
			else
				SndPlaySystemSound (sndError);
			break;
		
		case messageCopyCmd:
			MsgCopy ();
			break;
		
		case messageSelectAllCmd:
			MsgSelectAll ();
			break;
		
		//mgmg added change font menu item
		case messageChangeFontCmd:
			MessageViewToggleFont ();
			break;
			
		case messagePreferencesMenuCmd:
			FrmPopupForm (PrefDialog);		
			break;
		
		case messageHotSyncMenuCmd:
			FrmPopupForm (SyncDialog);
			break;
		
		case messagePurgeMenuCmd:
			if (PurgeDeletedRecord () && CurrentCategory == deletedCategory)
				{
				CurrentRecord = noRecordSelected;
				ChangeCategory (deletedCategory);
				FrmGotoForm (ListView);
				}
			break;
		
		case messageAboutMenuCmd:
			AbtShowAbout (sysFileCMail);
			break;
		
		}	
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewGoTo
 *
 * DESCRIPTION: This routine handles a frmGotoEvent
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/29/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewGoTo (EventPtr event)
{
	MsgSelectFieldType selFld;

	MessageTopField = (MsgFieldType) event->data.frmGoto.matchFieldNum;
	MessageScrollPos = event->data.frmGoto.matchPos;

	switch (MessageTopField)
		{
		case msgFieldSentTo:			selFld = msgSelectSentTo;			break;
		case msgFieldTo:				selFld = msgSelectTo;				break;
		case msgFieldFrom:			selFld = msgSelectFrom;				break;
		case msgFieldSubject:		selFld = msgSelectSubject;			break;
		case msgFieldCC:				selFld = msgSelectCC;				break;
		case msgFieldDate:			selFld = msgSelectDate;				break;
		case msgFieldBlankLine:		selFld = msgSelectBlankLine;		break;
		case msgFieldBody:			selFld = msgSelectBody;				break;
		}

	MessageSelectStartField = selFld;
	MessageSelectEndField = selFld;
	MessageSelectStartPos = event->data.frmGoto.matchPos;
	MessageSelectEndPos = event->data.frmGoto.matchPos + 
								 event->data.frmGoto.matchLen;
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewClose
 *
 * DESCRIPTION: This routine initializes the Message View form.
 *
 * PARAMETERS:  frm - pointer to the Message View form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/4/96	Initial Revision
 *
 ***********************************************************************/
static void MessageViewClose (void)
{
	// Release the block that holds the title.
	if (MessageTitleH)
		{
		MemHandleFree (MessageTitleH);
		MessageTitleH = NULL;
		}
}


/***********************************************************************
 *
 * FUNCTION:    MessageViewInit
 *
 * DESCRIPTION: This routine initializes the Message View form.
 *
 * PARAMETERS:  frm - pointer to the Message View form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art			6/4/96		Initial Revision
 *			MGolden		7/7/97		removed the font buttons
 *			MGolden 	7/10/97		Added the undelete button to the deleted view
 *									Changed the ReturnToView for the deleted category.
 * 									Otherwise, error occurs after an edit then send of 
 * 									Deleted (Created) mail.
 *			MGolden		7/21/97		Always set the return to view to the list view. It will be set 
 *									To the message view if the user selects new from the message view.
 *
 ***********************************************************************/
static void MessageViewInit (FormPtr frm)
{
	UInt16 controlID;
	//MailDBRecordType record;
	//MemHandle recordH;
	
	MessageViewSetTitle ();

	// Highlight the correct Header push button.
	if (ShowFullHeader)
		controlID = MessageFullHeaderButton;
	else
		controlID = MessageAbbrHeaderButton;
	FrmSetControlGroupSelection (frm, MessageHeaderGroup, controlID);

		
	// Highlight the corect font push button.
	//mgmg removed the font buttons

	// If the outbox is being shown then hide the reply button,  otherwise
	// hide the unsend button.
	MessageViewEnableButtons(frm);

	if (ShowFullHeader)
		MessageTopField = msgFieldTo;
	else
		MessageTopField = msgFieldSentTo;

	MessageScrollPos = 0;
	MessageSelectStartField = msgSelectNone;
	MessageSelectEndField = msgSelectNone;
	MessageSelectStartPos = 0;
	MessageSelectEndPos = 0;

#ifdef NEEDED
	// Set the following global so that we will return to the Message
	// View when a new message is sent.
	if (CurrentCategory == outboxCategory  ||  CurrentCategory == draftCategory || 
//mgmg new???
//ROGER?	
	CurrentCategory == deletedCategory || CurrentCategory == filedCategory )
		ReturnToView = ListView;
	else
		ReturnToView = MessageView;
#endif

	//always return to the list view!
	ReturnToView = ListView;
}

/***********************************************************************
 *
 * FUNCTION:    MessageViewEnableButtons
 *
 * DESCRIPTION: Show & hide the buttons in the message view, based on the current
 *						Category & message. The form may already be open, so we can not
 *						assume what buttons are already visible.
 *
 * PARAMETERS:  the formPtr
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden		7/10/97		Initial Revision
 *			Mgolden		9/29/97		added code to hide the next & prev buttons if
 *									there is only one message in the category.
 *	
 ***********************************************************************/
static void MessageViewEnableButtons(FormPtr frm)
	{
	MemHandle				recordH;
	MailDBRecordType	record;
	Boolean 				read = false;
	
	//how many messages are there...show or hide the next & prev buttons
	if (DmNumRecordsInCategory (MailDB, CurrentCategory) == 1)
		{
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessagePrevMessageButton));
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessageNextMessageButton));
		}
	else
		{
		FrmShowObject (frm, FrmGetObjectIndex (frm, MessagePrevMessageButton));
		FrmShowObject (frm, FrmGetObjectIndex (frm, MessageNextMessageButton));
		}
	
	if (CurrentCategory == inboxCategory)
		{
		//hide edit & undelete
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessageEditButton));
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessageUndeleteButton));
		
		//show reply & delete
		FrmShowObject (frm, FrmGetObjectIndex (frm, MessageReplyButton));
		FrmShowObject (frm, FrmGetObjectIndex (frm, MessageDeleteButton));
		return;
		}
	
	if (CurrentCategory == outboxCategory || CurrentCategory == draftCategory)
		{
		//hide reply & undelete
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessageReplyButton));
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessageUndeleteButton));

		//show edit & delete
		FrmShowObject (frm, FrmGetObjectIndex (frm, MessageEditButton));
		FrmShowObject (frm, FrmGetObjectIndex (frm, MessageDeleteButton));
		return;
		}
		
	//read the record and find out if it is a read message, or a created one
	MailGetRecord (MailDB, CurrentRecord, &record, &recordH);
	//if ((!record.flags.read) && (!record.from || (record.from[0] == '\0')))
	if ((!record.from || (record.from[0] == '\0')))
		read = false;
	else
		read = true;
	//unlock the record
	MemHandleUnlock (recordH);

	if (CurrentCategory == deletedCategory)
		{
		FrmHideObject (frm, FrmGetObjectIndex (frm, MessageDeleteButton));
		
		if (read)
			{
			//hide edit
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageEditButton));
			
			//show reply & undelete
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageReplyButton));
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageUndeleteButton));
			}
		else
			{
			//hide reply, & undelete
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageReplyButton));
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageUndeleteButton));
			
			//show edit
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageEditButton));
			}
		return;
		}

	if (CurrentCategory == filedCategory)
		{
		if (read)
			{
			//hide edit & undelete 
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageEditButton));
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageUndeleteButton));
			
			//show reply & delete
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageReplyButton));
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageDeleteButton));
			}
		else
			{
			//hide reply, & undelete
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageReplyButton));
			FrmHideObject (frm, FrmGetObjectIndex (frm, MessageUndeleteButton));
			
			//show edit & delete
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageEditButton));
			FrmShowObject (frm, FrmGetObjectIndex (frm, MessageDeleteButton));
			}
		return;
		}
			
	}//MessageViewEnableButtons


/***********************************************************************
 *
 * FUNCTION:    MessageViewHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Message View".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		6/4/96	Initial Revision
 *			MGolden	7/7/97	removed the font buttons
 *			MGolden	7/10/97	added code for the Undelete button.
 *			kwk		11/22/98	MemHandle command keys in separate code block so that
 *									TxtCharIsPrint doesn't get called w/virtual chars.
 *			jmp		10/29/99	Handle frmUpdateEvent ourselves now so we don't
 *									have partially drawn screens.
 *			jmp		10/31/99 Factored out commonality among frmOpenEvent, frmUpdateEvent, and
 *									frmGotoEvent since they are all substantially the same.
 *
 ***********************************************************************/
static Boolean MessageViewHandleEvent (EventPtr event)
{
	UInt16 objIndex;
	FormPtr frm;
	RectangleType r;
	Boolean handled = false;

	if (event->eType == keyDownEvent)
		{
		// Mail key pressed?  If so display the next mail message.
		if (TxtCharIsHardKey(event->data.keyDown.modifiers, event->data.keyDown.chr))
			{
			if (! (event->data.keyDown.modifiers & poweredOnKeyMask))
				{
				if (! MessageViewNextMessage (dmSeekForward))
					FrmGotoForm (ListView);
				handled = true;
				}
			}

		else if (EvtKeydownIsVirtual(event))
			{
			if (event->data.keyDown.chr == vchrPageUp)
				{
				MessageViewPageScroll (winUp);
				handled = true;
				}
	
			else if (event->data.keyDown.chr == vchrPageDown)
				{
				MessageViewPageScroll (winDown);
				handled = true;
				}
			}
		}

	else if (event->eType == ctlSelectEvent)
		{		
		switch (event->data.ctlSelect.controlID)
			{
			case MessageDoneButton:
				FrmGotoForm (ListView);
				handled = true;
				break;

			case MessageDeleteButton:
				if (MessageViewDeleteMessage())
					FrmGotoForm (ListView);
				handled = true;
				break;

			case MessageReplyButton:
				//mgmg return to the message after the reply!
				ReturnToView = MessageView;
				FrmPopupForm (ReplyDialog);
				handled = true;
				break;

			case MessageEditButton:
				if (MessageViewUnsendMessage ())
					FrmGotoForm (NewView);
				break;

			case MessageUndeleteButton:
				if (MessageUndelete())
					{
					//change the category to the inBox, we are going to follow this message there
					ChangeCategory (inboxCategory);
					FrmGotoForm (ListView);
					}
				break;
				
			case MessageAbbrHeaderButton:
			case MessageFullHeaderButton:
				MessageViewChangeHeader (event->data.ctlSelect.controlID);
				handled = true;
				break;

//mgmg new 9/9/97			
			case MessagePrevMessageButton:
				MessageViewNextMessage (dmSeekBackward);
				handled = true;
				break;

			case MessageNextMessageButton:
				MessageViewNextMessage (dmSeekForward);
				handled = true;
				break;
			}
		}

	else if (event->eType == penDownEvent)
		{
		frm = FrmGetActiveForm ();
		objIndex = FrmGetObjectIndex (frm, MessageGadget);
		FrmGetObjectBounds (frm, objIndex, &r);
		if (RctPtInRectangle (event->screenX, event->screenY, &r))
			MsgSelect (event->screenX, event->screenY);
		}

	else if (event->eType == menuEvent)
		{
		MessageViewDoCommand (event->data.menu.itemID);
		return (true);
		}
		
	else if (event->eType == menuCmdBarOpenEvent) 
		{
		MenuCmdBarAddButton(menuCmdBarOnRight, BarCopyBitmap, menuCmdBarResultMenuItem, messageCopyCmd, 0);

		// tell the field package to not add cut/copy/paste buttons automatically
		event->data.menuCmdBarOpen.preventFieldButtons = true;

		handled = false;  // mustn't say it is handled, otherwise the OS won't bring up the bar.
		}

	else if (event->eType == sclRepeatEvent)
		{
		MessageViewScroll (event->data.sclRepeat.newValue - 
			event->data.sclRepeat.value);
		}

	else if (event->eType == frmOpenEvent || event->eType == frmUpdateEvent || event->eType == frmGotoEvent)
		{
		frm = FrmGetActiveForm ();
		if (event->eType == frmGotoEvent)
			CurrentRecord = event->data.frmGoto.recordNum;
		if (event->eType == frmOpenEvent || event->eType == frmGotoEvent)
			MessageViewInit (frm);
		if (event->eType == frmGotoEvent)
			MessageViewGoTo (event);
		MessageViewUpdateScrollBar ();	
		FrmDrawForm (frm);
		MsgDraw ();
		MessageViewMarkRead ();
		handled = true;
		}

	else if (event->eType == frmCloseEvent)
		{
		MessageViewClose ();
		}

	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewSetTitle
 *
 * DESCRIPTION: This routine formats and sets the title of the List View.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	06/12/96	Initial Revision
 * 		MGolden	7/7/97	changed format to "x Unread, y Total". 
 *							Also changed it from the title to a label object next to the title
 *							# = UnreadMessages, % = MessagesInCategory
 * 		MGolden 7/9/97	Inbox works as above, all others use "# Total" instead
 *			kwk	05/28/99	Use const Char* for label text.
 *			kwk	06/28/99	Fixed calls to FrmHide/ShowObject using object id vs. index.
 *
 ***********************************************************************/
static void ListViewSetTitle (void)
{
	UInt16	len;
	Char	countStr [6];
	Char *	titleTemplateP;
	Char *	tmp;
	Char *	title;
	Char *	MemPtr;
	FormPtr	frm;
	Int16 	total;
	UInt16	objIndex;
	
	//check the count of records in the inBox
	total = DmNumRecordsInCategory (MailDB, inboxCategory);
				
	if (CurrentCategory == inboxCategory && total > 0)
		{
		// Format the title string.  A string resource is used as a template, the
		// '#' character in the template indicates the postions of the numeric
		// value.
		// DOLATER kwk - use new IntlGlue call to create text MemPtr from template
		// plus parameters.
		
		total = DmNumRecordsInCategory (MailDB, inboxCategory);

		if (total > 999)
			titleTemplateP = MemHandleLock (DmGetResource (strRsc, listTitleShortStrID));
		else if (total == 1)
			titleTemplateP = MemHandleLock (DmGetResource (strRsc, listTitleSingularStringId));
		else
			titleTemplateP = MemHandleLock (DmGetResource (strRsc, listTitleStringId));

		StrIToA (countStr, UnreadMessages);
		len = StrLen (countStr) + StrLen (titleTemplateP);
		
		tmp = MemPtrNew (len + 1);
		ErrFatalDisplayIf (!tmp, "Out of memory");

		MemPtr = StrChr (titleTemplateP, '#');
		len = MemPtr - titleTemplateP;
		if (len)
			StrNCopy (tmp, titleTemplateP, len);
		tmp[len] = 0;
		StrCat (tmp, countStr);	
		MemPtr++;
		StrCat (tmp, MemPtr);
		
		//now title has the unread count...add the total count of the inbox category
		//total = DmNumRecordsInCategory (MailDB, inboxCategory);
		StrIToA (countStr, total);
		len = StrLen (countStr) + StrLen (tmp);

		title = MemPtrNew (len + 1);
		ErrFatalDisplayIf (!title, "Out of memory");
		
		MemPtr = StrChr (tmp, '%');
		len = MemPtr - tmp;
		if (len)
			StrNCopy (title, tmp, len);
		title[len] = 0;
		StrCat (title, countStr);	
		MemPtr++;
		StrCat (title, MemPtr);
		
		//free the tmp MemPtr
		MemPtrFree(tmp);
		}
	else
		{
		// Format the title string.  A string resource is used as a template, the
		// '#' character in the template indicates the postions of the numeric
		// value.
		total = DmNumRecordsInCategory (MailDB, CurrentCategory);
		
		if (total == 1)
			titleTemplateP = MemHandleLock (DmGetResource (strRsc, listTitleNotInboxSingularStrId));
		else
			titleTemplateP = MemHandleLock (DmGetResource (strRsc, listTitleNotInboxStringId));

		StrIToA (countStr, total);
		len = StrLen (countStr) + StrLen (titleTemplateP);
		
		title = MemPtrNew (len + 1);
		ErrFatalDisplayIf (!title, "Out of memory");

		MemPtr = StrChr (titleTemplateP, '#');
		len = MemPtr - titleTemplateP;
		StrNCopy (title, titleTemplateP, len);
		title[len] = 0;
		StrCat (title, countStr);
		
		MemPtr++;
		StrCat (title, MemPtr);
		}
	
	//erase the title area then re-draw it
	frm = FrmGetFormPtr (ListView);

	objIndex = FrmGetObjectIndex (frm, ListTitleLabel);
	FrmHideObject (frm, objIndex);
	FrmCopyLabel (frm, ListTitleLabel, title);
	FrmShowObject (frm, objIndex);

	MemPtrUnlock(titleTemplateP);
	MemPtrFree(title);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewDrawDate
 *
 * DESCRIPTION: This routine draws a messages date.
 *
 * PARAMETERS:	 table  - pointer to a table object
 *              row    - row the item is in
 *              column - column the item is in
 *              bounds - region to draw in
 *
 * RETURNED:	 nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewDrawDate (DateType date, Int16 x, Int16 y)
{
	UInt16			dateStrLen;
	Char			dateBuffer [dateStringLength];
	Int16			drawX, drawY;
	Char *			dateStr;
	FontID			currFont;
	
	currFont = FntSetFont (ListFont);
	
	// If there is no date draw a dash to indicate such.
	if (DateToInt (date) == noDate)
		{
		drawX = x + ((listDateWidth - 5) >> 1);
		drawY = y + ((FntLineHeight () + 1) / 2);
		WinDrawLine (drawX, drawY, drawX+5, drawY);		
		}

	else
		{
		DateToAscii (date.month, date.day, date.year + firstYear, 
						DateFormat, dateBuffer);

		// Remove the year from the date string.
			dateStr = dateBuffer;
		if ((DateFormat == dfYMDWithSlashes) ||
			 (DateFormat == dfYMDWithDots) ||
			 (DateFormat == dfYMDWithDashes))
			dateStr += 3;
		else
			{
			dateStr[StrLen(dateStr) - 3] = 0;
			}


		// Draw the due date, right aligned.
		dateStrLen = StrLen (dateStr);
		WinDrawChars (dateStr, 
						  dateStrLen,
						  x + listDateWidth - FntCharsWidth (dateStr, dateStrLen),
						  y);
		}
			
	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewDrawRecord
 *
 * DESCRIPTION: This routine the draw the title memo record in the list 
 *              view.  This routine is called by the table routine, 
 *              TblDrawTable, each time a line of the table needs to
 *              be drawn.
 *
 * PARAMETERS:  table  - pointer to the memo list table (TablePtr)
 *              row    - row of the table to draw
 *              column - column of the table to draw 
 *              bounds - bound to the draw region
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/31/96		Initial Revision
 *			MGolden	7/9/97	Draw the missing subject text (noSubjectListViewStrID)
 *									if there is no subject to draw. Also draw the Missing
 *									To: text if there is no To:
 *			MGolden	7/9/97	Set drawing order back to: To, Subject, Date
 *			MGolden	7/9/97   Removed code to strip the to/from field at a space or @
 *
 ***********************************************************************/
static void ListViewDrawRecord (void * table, Int16 row, Int16 UNUSED_PARAM(column), 
	RectanglePtr bounds)
{
	//Char 					ch;
	Char 				name [maxListFromNameLen];
	Int16 				x, y, offset;
	Int16				width;
	UInt16 				recordNum;
	MemHandle			recordH;
	Boolean				showDateColumn;
	MailDBRecordType	record;
//	Char *				atSymbolP;
//	Char *				lastSpaceP;
	Int16				subjectWidth;
	Int16				addressWidth;
	Char *				subject;
	Char				title[32];
	FontID				curFont;
	UInt16 				iconID = 0;
	MemHandle 				h;
	BitmapPtr 			bitmapP;
	
	// Get the record number that corresponds to the table item to draw.
	// The record number is stored in the "intValue" field of the item.
	recordNum = TblGetRowID (table, row);
	MailGetRecord (MailDB, recordNum, &record, &recordH);

	x = bounds->topLeft.x;
	y = bounds->topLeft.y;
	width = bounds->extent.x;

	// Draw the check mark icon.
	if (CurrentCategory == inboxCategory)
		{
		iconID = 0;
		if (record.flags.read)
			{
			if (record.flags.priority == priorityHigh)
				iconID = readUrgentIconID;
			else
				iconID = readIconID;
			}
		else if (record.flags.priority == priorityHigh)
			iconID = urgentIconID;
			
		if (iconID > 0)
			{
			//draw the icon
			h = DmGetResource ('Tbmp', iconID);
			if (h)
				{
				bitmapP = (BitmapPtr)MemHandleLock(h);
				if (bitmapP)
					{
					//center the icon vertically in the space
					if ((*bitmapP).height < bounds->extent.y)
						offset = (bounds->extent.y - (*bitmapP).height)/2;
					else
						offset = 0;
						
					WinDrawBitmap (bitmapP , bounds->topLeft.x, bounds->topLeft.y + offset);
					MemPtrUnlock (bitmapP);
					}				
				DmReleaseResource (h);
				}
			}		
		x += listIconWidth;
		width -= listIconWidth;
		}	



#ifdef OLDWAY
	if (CurrentCategory == inboxCategory)
		{
//draw the check centered in the line height		
		
		FntSetFont (symbolFont);
		ch = symbolCheckMark;
		if (record.flags.read)
			WinDrawChars (&ch, 1, x, y);
		x += FntCharWidth (ch) + 2;
		width -= x;
		
		listIconWidth
		
		}
#endif
	
	showDateColumn = ShowDates && 
						  CurrentCategory != outboxCategory &&
						  CurrentCategory != draftCategory;

	if (showDateColumn)
		width -= listDateWidth + listColumnSpacing;
	
	width -= listColumnSpacing;

//	addressWidth = width / 3;
	addressWidth = width / 2;
	subjectWidth = width - addressWidth;

/*
	if (record.flags.priority == priorityHigh)
		curFont = FntSetFont (boldFont);
	else
		curFont = FntSetFont (stdFont);
*/
	curFont = FntSetFont (ListFont);

	// Draw the address. If the current category is the outbox then 
	// draw the "to" field, otherwise draw the "from" field.
	if (CurrentCategory != outboxCategory && CurrentCategory != draftCategory)
		{
		if (*record.from)
			{
			// Get the friendly name if there is one.
			MailParseAddress (record.from, true, name, sizeof (name));
			
/*
			// Because the name column is only a third of the width
			// show only the most important part of the sender's name
			
			// Strip off the domain name
			atSymbolP = StrChr(name, '@');
			if (atSymbolP)
				{
				*atSymbolP = '\0';
				}
			else
				{
				// Strip off the last name.  The last name is stripped instead of 
				// of the first name because users probably get mail from people with
				// the same last name (family members) than the same first names.
				lastSpaceP = StrChr(name, ' ');
				if (lastSpaceP)
					{
					*lastSpaceP = '\0';
					}
				}
*/			
			DrawString (name, x, y, addressWidth);
			}
		// check the to field ...
		else if (*record.to)
			{ 
			//is there a to?
			MailParseAddress (record.to, true, name, sizeof (name));
			DrawString (name, x, y, addressWidth);
			}
		else
			{
			//draw the missing string?
			subject = MemHandleLock (DmGetResource (strRsc, noToListViewStrID));
			StrCopy (title, subject);
			MemPtrUnlock (subject);
			DrawString (title, x, y, addressWidth);
			}
		}
	else
		{
		if (*record.to)
			{
			//mgmg changed 8/4/97
			//DrawString (record.to, x, y, addressWidth);
			MailParseAddress (record.to, true, name, sizeof (name));
			DrawString (name, x, y, addressWidth);			
			}
		else
			{
			//there is no To Field, draw the missing to field message
			subject = MemHandleLock (DmGetResource (strRsc, noToListViewStrID));
			StrCopy (title, subject);
			MemPtrUnlock (subject);
			DrawString (title, x, y, addressWidth);
			}
		}
	x += addressWidth;

	// Draw a line between the address and subject columns.
	WinDrawLine (x + 1, y, x + 1, y + bounds->extent.y - 1);
	x += listColumnSpacing;	
	
	// Draw the "subject" field.
	if (*record.subject)
		DrawString (record.subject, x, y, subjectWidth);
//mgmg : draw the noSubjectListViewStrID if there is no subject
	else
		{
		subject = MemHandleLock (DmGetResource (strRsc, noSubjectListViewStrID));
		StrCopy (title, subject);
		MemPtrUnlock (subject);
		DrawString (title, x, y, subjectWidth);
		}
	
	
	x += subjectWidth;

	if (showDateColumn)
		{
		// Draw a line between the subject and date columns.
		WinDrawLine (x + 1, y, x + 1, y + bounds->extent.y - 1);
		x += listColumnSpacing;	
		

		// Draw the "date" field.
		FntSetFont (stdFont);
		ListViewDrawDate (record.date, x, y);
		}

	MemHandleUnlock (recordH);

	FntSetFont (curFont);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewUpdateScrollers
 *
 * DESCRIPTION: This routine draws or erases the list view scroll arrow
 *              buttons.
 *
 * PARAMETERS:  frm        -  pointer to the to do list form
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewUpdateScrollers (TablePtr table)
{
	Int16 rows;
	UInt16 width;
	UInt16 minValue;
	UInt16 maxValue;
	RectangleType r;

	ErrNonFatalDisplayIf (DmNumRecordsInCategory (MailDB, CurrentCategory) !=
		MessagesInCategory, "Invalid record count");
	
	//rows = TblGetNumberOfRows (table);
	rows = ListViewNumberOfRows (GetObjectPtr(ListTable));

	// Leave room for the scroll bar if it will be visible.
	FrmGetFormBounds (FrmGetActiveForm (), &r);
	width = r.extent.x;
	if (MessagesInCategory > rows)
		width -= listScrollBarWidth;

	TblSetColumnWidth (table, 0, width);
	TblGetBounds (table, &r);
	r.extent.x = width;
	TblSetBounds (table, &r);
	
		
	if (MessagesInCategory)
		minValue = DmPositionInCategory (MailDB, TopVisibleRecord, CurrentCategory);
	else
		minValue = 0;

	if (MessagesInCategory > rows)
		maxValue = MessagesInCategory - rows;
	else
		maxValue = 0;

	SclSetScrollBar (GetObjectPtr (ListScrollBar), minValue, 0, maxValue, rows);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewLoadTable
 *
 * DESCRIPTION: This routine loads memo database records into
 *              the list view form.
 *
 * PARAMETERS:  recordNum index of the first record to display.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/31/96		Initial Revision
 *
 ***********************************************************************/
static void ListViewLoadTable (FormPtr frm, UInt16 recordNum)
{
	UInt16			row;
	UInt16			lastRecordNum;
	UInt16			numRows;
	UInt32			uniqueID;
	TablePtr 		table;
	MemHandle		recordH;
	FontID			currFont;
	RectangleType r;
	UInt16			dataHeight;
	UInt16			tableHeight;
	UInt16			lineHeight;

	
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, ListTable));

	/* HandEra */
	if (vgaExists)
	    VgaTableUseBaseFont(table, !VgaIsVgaFont(ListFont));
	/* end HandEra */

	TblGetBounds (table, &r);
	tableHeight = r.extent.y;

	dataHeight = 0;

	currFont = FntSetFont (ListFont);
	lineHeight = FntLineHeight ();
	FntSetFont (currFont);

	// For each row in the table, store the record number in the table item
	// that will dispaly the record.  
	numRows = TblGetNumberOfRows (table);
	for (row = 0; row < numRows; row++)
		{		
		// Get the next record in the currunt category.
		recordH = DmQueryNextInCategory (MailDB, &recordNum, 
			CurrentCategory);
		
		// If the record was found, store the record number in the table item,
		// otherwise set the table row unusable.
//		if (recordH)
		if (recordH && (tableHeight >= dataHeight + lineHeight))
			{
			TblSetRowID (table, row, recordNum);
			
//new font stuff
			TblSetItemStyle (table, row, 0, customTableItem);
			TblSetItemFont (table, row, 0, ListFont);
			TblSetRowHeight (table, row, lineHeight);

			DmRecordInfo (MailDB, recordNum, NULL, &uniqueID, NULL);
			if (TblGetRowData (table, row) != uniqueID || 
				(!TblRowUsable (table, row)))
				{
				TblSetRowUsable (table, row, true);
				
				// Store the unique id of the record in the row.
				TblSetRowData (table, row, uniqueID);

				// Mark the row invalid so that it will draw when we call the 
				// draw routine.
				TblMarkRowInvalid (table, row);
				}

			lastRecordNum = recordNum;
			if (row+1 < numRows) 
				recordNum++;
			
			dataHeight += lineHeight;
			}
		else
			{
			TblSetRowUsable (table, row, false);
			TblSetItemFont (table, row, 0, ListFont);
			TblSetRowHeight (table, row, lineHeight);
			}
		}

	// Update the scroll arrows.
	ListViewUpdateScrollers (table);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewLoadRecords
 *
 * DESCRIPTION: This routine loads memo database records into
 *              the list view form.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewLoadRecords (FormPtr frm)
{
	TablePtr 	table;
	UInt16		recordNum;
	UInt16		rowsInTable;
	
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, ListTable));
	//rowsInTable = TblGetNumberOfRows (table);
	rowsInTable = ListViewNumberOfRows (table);


	// Is the current record before the first visible record?
	if (CurrentRecord != noRecordSelected)
		{
		if (TopVisibleRecord > CurrentRecord)
			TopVisibleRecord = CurrentRecord;
		
		// Is the current record after the last visible record?
		else
			{
			recordNum = TopVisibleRecord;
			DmSeekRecordInCategory (MailDB, &recordNum, rowsInTable-1,
				dmSeekForward, CurrentCategory);
			if (recordNum < CurrentRecord)
				TopVisibleRecord = CurrentRecord;
			}
		}
		
	
	// Make sure we show a full display of records.
	recordNum = dmMaxRecordIndex;
	if (DmSeekRecordInCategory (MailDB, &recordNum, (rowsInTable-1),
		dmSeekBackward, CurrentCategory) == 0)
		TopVisibleRecord = min (TopVisibleRecord, recordNum);
	else
		TopVisibleRecord = 0;

	ListViewLoadTable (frm, TopVisibleRecord);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewChangeCategory
 *
 * DESCRIPTION: This routine display the category specified  
 *
 * PARAMETERS:  category - new category
 *
 * RETURNED:    The index of the new category.
 *
 *              The following global variables are modified:
 *							CurrentCategory
 *							ShowAllCategories
 *							CategoryName
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewChangeCategory (UInt16 category)
{
	FormPtr frm;
	TablePtr table;

	frm = FrmGetActiveForm ();
	table = GetObjectPtr (ListTable);

	ChangeCategory (category);

	// Display the new category.
	TblEraseTable (table);

	ListViewLoadTable (frm, TopVisibleRecord);

	TblDrawTable (table);
	
	ListViewSetTitle ();
	
	//unset the current record..it will not be in the new category!
	CurrentRecord = noRecordSelected; 
	
	//mgmg 7/16
	//ListViewSelectRecord(frm, CurrentRecord, true);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewUpdateDisplay
 *
 * DESCRIPTION: This routine updates the display of the List View
 *
 * PARAMETERS:  updateCode - a code that indicated what changes have been
 *                           made to the to do list.
 *                		
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		6/3/96	Initial Revision
 *			MGolden	10/3/97	Rocky font changes
 *			jmp		10/31/99	Handle frmRedrawUpdateCode as the generic form
 *									handling code no longer works for us in Palm
 *									OS 3.5 and later.
 *
 ***********************************************************************/
static Boolean ListViewUpdateDisplay (UInt16 updateCode)
{
	TablePtr table;
	
	table = GetObjectPtr (ListTable);

	// Was the UI unable to save an image of the ListView when it 
	// obscured part of the view with another dialog?  If so we'll
	// handle it here.
	if (updateCode & frmRedrawUpdateCode)
		{
		FormPtr frm = FrmGetActiveForm ();
		FrmDrawForm (frm);
		ListViewSelectRecord (frm, CurrentRecord, true);
		return (true);
		}

	// Were the display options modified (ToDoOption dialog)?
	if (updateCode & (updateDisplayOptsChanged | updateFontChanged))
		{
		//new for rocky font
		TopVisibleRecord = 0;

		ListViewLoadRecords (FrmGetActiveForm());
		table = GetObjectPtr (ListTable);
		TblEraseTable (table);
		TblDrawTable (table);		

		//TblEraseTable (table);
		//ListViewLoadTable (FrmGetActiveForm (), TopVisibleRecord);
		//TblDrawTable (table);
		//mgmg
		//ListViewSelectRecord(FrmGetActiveForm (), CurrentRecord, true);
		return (true);
		}

	// Were deleted records purged?
	if (updateCode & updatePurge)
		{
		SeekRecord (&TopVisibleRecord, 0, dmSeekForward);
		ListViewLoadTable (FrmGetActiveForm (), TopVisibleRecord);
		TblRedrawTable(table);
		return (true);
		}
	
	return (true);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewScroll
 *
 * DESCRIPTION: This routine scrolls the list of of message
 *              in the direction specified.
 *
 * PARAMETERS:  direction - winUp or dowm
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/3/96	Initial Revision
 *			MGolden	7/16/97	De-select the current selction before scrolling
 *							because of a bug in the table code that will trash the table struct
 *							if the selected item is not a valid row.
 *							
 *							TABLE does not MemHandle font's correctly!
 *
 ***********************************************************************/
static void ListViewScroll (Int16 linesToScroll)
{
	Int16 			i;
	UInt16			rows;
	UInt16			lastRow;
	UInt16 			scrollAmount;
	UInt16			lastRecord;
	UInt16 			newTopVisibleRecord;
	TablePtr 		table;
	RectangleType	scrollR;
	RectangleType	vacated;
	WinDirectionType	direction;

	//CurrentRecord = noRecordSelected;
	
	table = GetObjectPtr (ListTable);
	//rows = TblGetNumberOfRows (table);
	rows = ListViewNumberOfRows (table);

	//de-select the table item...
	TblUnhighlightSelection (table);

	// Find the new top visible record
	newTopVisibleRecord = TopVisibleRecord;

	// Scroll winDown.
	if (linesToScroll > 0)
		{
		if (! SeekRecord (&newTopVisibleRecord, linesToScroll, dmSeekForward))
			return;

		lastRecord = TopVisibleRecord;
		if (!SeekRecord (&lastRecord, rows, dmSeekForward))
			return;

		}

	// Scroll winUp.
	else if (linesToScroll < 0)
		{
		if (! SeekRecord (&newTopVisibleRecord, -linesToScroll, dmSeekBackward))
			return;
		}


	TopVisibleRecord = newTopVisibleRecord;

	// Move the bits that will remain visible.
	if (((linesToScroll > 0) && (linesToScroll < rows)) ||
		 ((linesToScroll < 0) && (-linesToScroll < rows)))
		{
		scrollAmount = 0;
	
		if (linesToScroll > 0)
			{
			lastRow = TblGetLastUsableRow (table) - 1;
			for (i = 0; i < linesToScroll; i++)
				{
				scrollAmount += TblGetRowHeight (table, lastRow);
				TblRemoveRow (table, 0);
				}
			direction = winUp;
			}
		else
			{
			for (i = 0; i < -linesToScroll; i++)
				{
				scrollAmount += TblGetRowHeight (table, 0);
				TblInsertRow (table, 0);
				}
			direction = winDown;
			}

		TblGetBounds (table, &scrollR);
		WinScrollRectangle (&scrollR, direction, scrollAmount, &vacated);		
		WinEraseRectangle (&vacated, 0);
		}
	
	
	ListViewLoadTable (FrmGetActiveForm (), TopVisibleRecord);
	
	TblRedrawTable(table);

	//mgmg 7/16
	//re-select the current record (only if it's visible now)
	ListViewSelectRecord (FrmGetActiveForm (), CurrentRecord, false);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewPageScroll
 *
 * DESCRIPTION: This routine scrolls the list of of expense items
 *              in the direction specified.
 *
 * PARAMETERS:  direction - winUp or dowm
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	1/2/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewPageScroll (WinDirectionType direction)
{
	TablePtr table;
	Int16 row;
	UInt16 recordNum;
	Int16 rowsInTable;
	UInt16 newTopVisibleRecord;

	table = GetObjectPtr (ListTable);
	//rowsInTable = TblGetNumberOfRows (table);
	rowsInTable = ListViewNumberOfRows (table);

	newTopVisibleRecord = TopVisibleRecord;
	
	//mgmg no reason for this that I can see...
	//CurrentRecord = noRecordSelected;

	//de-select the table item...
	TblUnhighlightSelection (table);

	// Scroll the table winDown a page (less one row).
	if (direction == winDown)
		{
		// Get the record index of the last visible record.  A row 
		// number of minus one indicates that there are no visible rows.
		row = TblGetLastUsableRow (table);
		if (row == -1) return;
		
		newTopVisibleRecord = TblGetRowID (table, row);
		recordNum = newTopVisibleRecord;

		// Try going forward one page
		if (!SeekRecord (&recordNum, rowsInTable - 1, dmSeekForward))
			{
			// Try going backwards one page from the last record
			newTopVisibleRecord = dmMaxRecordIndex;
			if (!SeekRecord (&newTopVisibleRecord, rowsInTable - 1, dmSeekBackward))
				{
				// Not enough records to fill one page.  Start with the first record
				newTopVisibleRecord = 0;
				SeekRecord (&newTopVisibleRecord, 0, dmSeekForward);
				}
			}
		}
		
	// Scroll winUp a page (less one row).
	else
		{
		if (!SeekRecord (&newTopVisibleRecord, rowsInTable - 1, dmSeekBackward))
			{
			// Not enough records to fill one page.  Start with the first record
			newTopVisibleRecord = 0;
			SeekRecord (&newTopVisibleRecord, 0, dmSeekForward);
			}
		}

	// Avoid redraw if no change
	if (TopVisibleRecord != newTopVisibleRecord)
		{
		TopVisibleRecord = newTopVisibleRecord;

//mgmg do later
//		ListViewLoadRecords (FrmGetActiveForm ());
//		TblRedrawTable(table);

		TblMarkTableInvalid (table);
		ListViewLoadTable (FrmGetActiveForm (), TopVisibleRecord);
		TblRedrawTable (table);

		}
	//mgmg 8/4
	//re-select the current record (only if it's visible now)
	ListViewSelectRecord (FrmGetActiveForm (), CurrentRecord, false);
}


/***********************************************************************
 *
 * FUNCTION:    ListViewDoCommand
 *
 * DESCRIPTION: This routine preforms the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/4/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewDoCommand (UInt16 command)
{
	switch (command)
		{
		case newMenuCmd:
			if (CreateRecord ())
				FrmGotoForm (NewView);			
			break;

//		case showMenuCmd:
//			FrmPopupForm (OptionsDialog);
//			break;

		case preferencesMenuCmd:
			FrmPopupForm (PrefDialog);		
			break;
						
		case hotSyncMenuCmd:
			FrmPopupForm (SyncDialog);
			break;
						
		case purgeMenuCmd:
			if (PurgeDeletedRecord ())
				{
				// If the "deleted" catagory is shown, erease the table.
				if (CurrentCategory == deletedCategory)
					ListViewChangeCategory (deletedCategory);
				else
					FrmUpdateForm (ListView, updatePurge);
				}
			break;
						
		case aboutMenuCmd:
			AbtShowAbout (sysFileCMail);
			break;
		
		case ListChangeFontMenuCmd:
			ListFont = SelectFont (ListFont);
			break;
		}	
}


/* HandEra */
/***********************************************************************
 *
 * FUNCTION:    ListViewResize
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void ListViewResize(Boolean draw)
{
    FormPtr         frmP;
    Coord           x, y;
    Coord           xDiff, yDiff;
    TablePtr        table;
    RectangleType   r;

    WinGetDisplayExtent(&x, &y);

    // this will get the old extent
    // resize form
    frmP = FrmGetActiveForm();
    FrmGetFormBounds(frmP, &r);
    
    xDiff = x - (r.topLeft.x + r.extent.x);
    yDiff = y - (r.topLeft.y + r.extent.y);
    if ((xDiff == 0) && (yDiff == 0))
        return;

    // resize form
    r.extent.x += xDiff;
    r.extent.y += yDiff;
    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);

    // resize table
    table = GetObjectPtr(ListTable);
	TblGetBounds(table, &r);
	r.extent.x += xDiff;
	r.extent.y += yDiff;
    TblSetBounds(table, &r);
    
    if(draw && (yDiff < 0))
    {
	    r.topLeft.y = (r.topLeft.y + r.extent.y);
	    r.extent.y = -yDiff;
        WinEraseRectangle (&r, 0);
    }
        
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, ListNewButton),  0, yDiff, draw);
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, ListShowButton), 0, yDiff, draw);
    
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, ListScrollBar),   xDiff, 0, false);
    ToolsResizeObject(frmP, FrmGetObjectIndex(frmP, ListScrollBar), 0, yDiff, draw);

    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, ListCategoryTrigger), xDiff, 0, draw);
    ToolsMoveObject(frmP, FrmGetObjectIndex(frmP, ListCategoryList),    xDiff, 0, draw);

    if (draw)
    {
        ListViewLoadTable(frmP, TopVisibleRecord);
        FrmDrawForm(frmP);
    }
}
/* end HandEra */

/***********************************************************************
 *
 * FUNCTION:    ListViewInit
 *
 * DESCRIPTION: This routine initializes the "List View" of the 
 *              Mail application.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has MemHandle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/31/96	Initial Revision
 *
 ***********************************************************************/
static void ListViewInit (FormPtr frm)
{
	Int16 row;
	Int16 rowsInTable;
	ListPtr lst;
	TablePtr table;
	ControlPtr ctl;
	
    ListViewResize(false);

	MessagesInCategory = DmNumRecordsInCategory (MailDB, CurrentCategory);

	ListViewSetTitle ();

	// Set the label of the category trigger.
	ctl = GetObjectPtr (ListCategoryTrigger);
	CategoryGetName (MailDB, CurrentCategory, CategoryName);
	CategorySetTriggerLabel (ctl, CategoryName);
	
	// Set the category list selection.
	lst = GetObjectPtr (ListCategoryList);
	LstSetSelection (lst, CurrentCategory);
			
	//now get the current table
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, ListTable));
	
	rowsInTable = TblGetNumberOfRows (table);
	for (row = 0; row < rowsInTable; row++)
		{		
		TblSetItemStyle (table, row, 0, customTableItem);
		TblSetRowUsable (table, row, false);
		}
		
	// Set the callback routine that will draw the records.
	TblSetCustomDrawProcedure (table, 0, ListViewDrawRecord);

	TblSetColumnUsable (table, 0, true);	

	ListViewLoadRecords (frm);

	// Set the following global so that we will return to the List
	// View when a new message is sent.
	ReturnToView = ListView;
}


/***********************************************************************
 *
 * FUNCTION:    ListViewHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "List View"
 *              of the Mail application.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	05/31/96	Initial Revision
 *			kwk	11/22/98	MemHandle command keys in separate code block so that
 *								TxtCharIsPrint doesn't get called w/virtual chars.
 *			jmp	10/31/99	Perform FrmDrawForm() in frmOpenForm BEFORE calling
 *								ListViewSelectRecord() to prevent selected record from
 *								getting drawn as unselected.  Fixes bug #23416.
 *
 ***********************************************************************/
static Boolean ListViewHandleEvent (EventPtr event)
{
	FormPtr frm;
	TablePtr table;
	Boolean handled = false;

	if (event->eType == keyDownEvent)
		{
		// Mail key pressed?  If so display the first mail message.
		if (TxtCharIsHardKey(event->data.keyDown.modifiers, event->data.keyDown.chr))
			{
			if (! (event->data.keyDown.modifiers & poweredOnKeyMask))
				{
				table = GetObjectPtr (ListTable);
				if (TblRowUsable (table, 0))
					{
					CurrentRecord = TblGetRowID (table, 0);
					FrmGotoForm (MessageView);
					}
				handled = true;
				}
			}
		
		else if (EvtKeydownIsVirtual(event))
			{
			// Scroll up key presed?
			if (event->data.keyDown.chr == vchrPageUp)
				{
				ListViewPageScroll (winUp);
				handled = true;
				}
	
			// Scroll down key presed?
			else if (event->data.keyDown.chr == vchrPageDown)
				{
				ListViewPageScroll (winDown);
				handled = true;
				}
			}
		}

	else if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case ListNewButton:
				if (CreateRecord ())
					FrmGotoForm (NewView);
				handled = true;
				break;

			case ListShowButton:
				FrmPopupForm (OptionsDialog);
				handled = true;
				break;
			}
		}

	else if (event->eType == tblSelectEvent)
		{
		// An item in the list of messages was selected, display it.
		CurrentRecord = TblGetRowID (event->data.tblSelect.pTable, 
			event->data.tblSelect.row);
		FrmGotoForm (MessageView);
		handled = true;
		}

	else if (event->eType == menuEvent)
		{
		ListViewDoCommand (event->data.menu.itemID);
		return (true);
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
		if (vgaExists)
		    VgaFormModify(frm, vgaFormModify160To240);
/* end HandEra */
		ListViewInit (frm);
		FrmDrawForm (frm);
		ListViewSelectRecord (frm, CurrentRecord, true);
		handled = true;
		}

	else if (event->eType == frmUpdateEvent)
		{
		handled = ListViewUpdateDisplay (event->data.frmUpdate.updateCode);
		}

	else if (event->eType == popSelectEvent)
		{
		ListViewChangeCategory (event->data.popSelect.selection);
		}

	else if (event->eType == sclRepeatEvent)
		{
		ListViewScroll (event->data.sclRepeat.newValue - 
			event->data.sclRepeat.value);
		}
/* HandEra */
    else if (event->eType == displayExtentChangedEvent)
        {
        ListViewResize(true);
        handled = true;
        }
/* end HandEra */
	return (handled);
}


/***********************************************************************
 *
 * FUNCTION:    ApplicationHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and sets the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *
 ***********************************************************************/
static Boolean ApplicationHandleEvent (EventPtr event)
{
	UInt16 formID;
	FormPtr frm;

	if (event->eType == frmLoadEvent)
		{
		// Load the form resource.
		formID = event->data.frmLoad.formID;
		frm = FrmInitForm (formID);
		FrmSetActiveForm (frm);		
		
		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmDispatchEvent each time is receives an
		// event.
		switch (formID)
			{
			case ListView:
				FrmSetEventHandler (frm, ListViewHandleEvent);
				break;

			case MessageView:
				FrmSetEventHandler (frm, MessageViewHandleEvent);
				break;

			case NewView:
				FrmSetEventHandler (frm, NewViewHandleEvent);
				break;

			case EditView:
				FrmSetEventHandler (frm, EditViewHandleEvent);
				break;

			case ReplyDialog:
				FrmSetEventHandler (frm, ReplyHandleEvent);
				break;
				
			case DetailsDialog:
				FrmSetEventHandler (frm, DetailsHandleEvent);
				break;
				
			case OptionsDialog:
				FrmSetEventHandler (frm, OptionsHandleEvent);
				break;

			case PrefDialog:
				FrmSetEventHandler (frm, PrefHandleEvent);
				break;

			case SyncDialog:
				FrmSetEventHandler (frm, SyncHandleEvent);
				break;

			case TruncateDialog:
				FrmSetEventHandler (frm, TruncateHandleEvent);
				break;

			}
		return (true);
		}
	return (false);
}

/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the Mail application
 *
 * PARAMETERS:  cmd			 - launch code
 *              cmdPBP      - paramenter block (launch code specific)
 *              launchFlags - SysAppLaunch flags (ses SystemMgr.h)
 *
 * RETURNED:    error code
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		5/30/96	Initial Revision
 *			MGolden	7/28/97	Added call to RomVersionCompatible
 *			jmp		10/18/99	If the default database image doesn't exist, then create
 *									an empty database.
 *
 ***********************************************************************/
UInt32	PilotMain (UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	UInt16 error;
	MailAddRecordParamsType	newMsg;


	// This app makes use of PalmOS 2.0 features.  It will crash if run on an
	// earlier version of PalmOS.  Detect and warn if this happens, then exit.
	// Actually, require 4.0 ROM since that's what it's tested to run on.
#ifdef DO_ROM_CHECK
	error = RomVersionCompatible (ourMinVersion, launchFlags);
	if (error)
		return error;
#endif
	// Normal Launch
	if (cmd == sysAppLaunchCmdNormalLaunch)
		{
		error = StartApplication ();
		if (error) return (error);

		#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
			ECMailDBValidate (MailDB);
		#endif

		if (NewRecord == noNewRecord)
			FrmGotoForm (ListView);
		else
			FrmGotoForm (NewView);
		EventLoop ();

		StopApplication ();
		}


	// This action code is sent to the app by the system when it is
	// doing a text search.
	else if (cmd == sysAppLaunchCmdFind)
		{
		Search ((FindParamsPtr)cmdPBP);
		}
	
	
	// This action code is sent to the app when the user hit the "Go To"
	// button in the Find Results dialog.
	else if (cmd == sysAppLaunchCmdGoTo)
		{
		Boolean launched;
		launched = launchFlags & sysAppLaunchFlagNewGlobals;
		
		if (launched) 
			{
			error = StartApplication ();
			if (error) return (error);

			GoToItem ((GoToParamsPtr) cmdPBP, launched);
			
			EventLoop ();
			StopApplication ();	
			}
		else
			GoToItem ((GoToParamsPtr) cmdPBP, launched);
		}
			

	else if (cmd == sysAppLaunchCmdSyncNotify)
		{
		ExgRegisterDatatype(sysFileCMail, exgRegSchemeID, "mailto",
						"Email URL", 0);
		SyncNotification ();
		}


	// This launch code is sent after the system is reset.  We use this time
	// to create our default database.  If there is no default database image,
	// then we create an empty database.
	else if (cmd == sysAppLaunchCmdSystemReset)
		{
		if (((SysAppLaunchCmdSystemResetType*)cmdPBP)->createDefaultDB)
			{
			error = CreateDefaultDatabase();
			}
		ExgRegisterDatatype(sysFileCMail, exgRegSchemeID, "mailto",
						"Email URL", 0);
		RegisterLocaleChangedNotification();
		}


	// This action code is sent by the DesktopLink server when it creates
	// a new database.  We will initializes the new database.
	else if (cmd == sysAppLaunchCmdInitDatabase)
		{
		MailAppInfoInit (((SysAppLaunchCmdInitDatabaseType*)cmdPBP)->dbP);
		
		// Set the backup bit.  This is to aid syncs with non-Palm software.
		SetDBBackupBit(((SysAppLaunchCmdInitDatabaseType*)cmdPBP)->dbP);
		}


	// This action code is sent by an other application when it want to 
	// add a record to the Mail outbox.
	else if (cmd == sysAppLaunchCmdAddRecord)
		{
		error = AddMessage ((MailAddRecordParamsPtr)cmdPBP, true);
		if (error) return (error);
		}

	// This action code is sent by exchange manager when we want to
	// open a mailto: url
	else if (cmd == sysAppLaunchCmdGoToURL)
	{
		Char *buf;
		UInt16 button;
		buf = MemPtrNew(StrLen(cmdPBP));
		if(!buf) return memErrNotEnoughSpace;
		error = ParseMailtoURL(&newMsg, cmdPBP, buf);
		if (error) {
			MemPtrFree(buf);
			return (error);
		}
		button = FrmAlert (SaveDraftAlert);
		if (button == SaveDraftYes)
			error = AddMessage(&newMsg, false);
		MemPtrFree(buf);
		if (error) return (error);
	}
	else if (cmd == sysAppLaunchCmdNotify)
	{
		if (((SysNotifyParamType*)cmdPBP)->notifyType == sysNotifyLocaleChangedEvent)
		{
			DmSearchStateType	searchState;
			LocalID	dbID;
			UInt16	cardNo;

			// Since the locale has changed, delete the existing database
			// and re-create it for the new locale
			error = DmGetNextDatabaseByTypeCreator (true, &searchState, mailDBType,
													sysFileCMail, true, &cardNo, &dbID);
			if (!error)
				DmDeleteDatabase(cardNo, dbID);

			error = CreateDefaultDatabase();

		}
	}

	return (errNone);
	}
/***********************************************************************
 *
 * FUNCTION:    EventLoop
 *
 * DESCRIPTION: This routine is the event loop for the Mail
 *              aplication.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *
 ***********************************************************************/
static void EventLoop (void)
{
	UInt16 error;
	EventType event;

	do
		{
		EvtGetEvent (&event, evtWaitForever);

		if (! SysHandleEvent (&event))
		
			if (! MenuHandleEvent (NULL, &event, &error))
			
				if (! ApplicationHandleEvent (&event))
	
					FrmDispatchEvent (&event); 
		}

	while (event.eType != appStopEvent);
}

/***********************************************************************
 *
 * FUNCTION:    PrvGetNameValuePair
 *
 * DESCRIPTION: Called from anyone that must parse name/value pairs.
 *
 * PARAMETERS:  
 *				strP		-> ptr to ptr to string holding HTML name/value pairs
 *				strLen	-> ptr to Int16 holding length of string
 *				nameP		-> ptr to ptr to location to store the resulting name string pointer
 *				nameLen	-> ptr to Int16 to hold resulting nameP length
 *				valueP	-> ptr to ptr to location to store the resulting value string pointer
 *				valueLen	-> ptr to Int16 to hold resulting valueP length
 *
 * RETURNED:   false if nothing found else true if found
 *
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			tgh	4/23/98	Initial Version
 *			gcd 7/25/00 Move into Msg.c instead of INetLib
 *
 ***********************************************************************/
static Boolean PrvGetNameValuePair(Char **strP, Int16 * strLen, Char **nameP, 
								   Int16 * nameLen, Char **valueP,
								   Int16 * valueLen)
{
	Boolean	result = false;
	Int16		i, j, index = 0;

	if (*strLen == 0)
		return false;

	*nameP = *strP;						// assume name to be first character
	
	for (i = 0; i < *strLen; i++)
	{
		if ((*strP)[i] == '=')
		{
			index++;
			break;
		}
	}

	*nameLen = i;							// set the length of the name
	*valueP = *strP + i + 1;			// skip past the '=' sign

	for (j = 0; j < *strLen - i - 1; j++)
	{
		if ((*valueP)[j] == '&')
		{
			index++;
			break;
		}
	}
	
	*valueLen = j;
	
	index += i + j;				// name + value + '=' and/or '&' (if any)
	if (index <= *strLen)
	{
		result = true;
		*strP = *strP + index;
		*strLen = *strLen - index;
	}
		
	return result;
}

/***********************************************************************
 *
 * FUNCTION:    ParseMailtoURL
 *
 * DESCRIPTION:	Parse a mailto: url that exchange manager has given us
 *
 * PARAMETERS:	newMsgP		- MailAddRecordParamsType to fill in
 * 				cmdPBP		- parameter block
 *
 * RETURNED:	error code
 *
 * REVISION HISTORY:
 * 			Name	Date		Description
 * 			----	----		-----------
 * 			gcd		7/24/00		Initial Revision
 ***********************************************************************/
static Err ParseMailtoURL(MailAddRecordParamsPtr newMsgP, MemPtr cmdPBP,
						  Char *buf)
{
	Char *c, *nameP, *valueP;
	Int16 strLen, nameLen, valueLen;
	UInt16 i = 0;

	MemSet(newMsgP, sizeof(MailAddRecordParamsType), 0);
	buf[0] = '\0';
	c = (Char *)StrChr((Char *)cmdPBP, ':');
	if(c)
	{
		c++;
		while(*c && *c != '?')
			buf[i++] = *c++;
		buf[i] ='\0';
		newMsgP->to = buf;
		if(*c == '?')
		{
			strLen = StrLen(c);
			while (PrvGetNameValuePair(&c, &strLen, &nameP, &nameLen, &valueP, &valueLen))
			{
				if(nameLen == 2 && (valueP) &&
				   (!StrNCaselessCompare(nameP, "to", nameLen)))
				{
					StrNCat(&buf[i], valueP, valueLen);
					newMsgP->to = &buf[i];
					i+=valueLen;
					buf[i]='\0';
				}
				if(nameLen == 7 && (valueP) &&
				   (!StrNCaselessCompare(nameP, "subject", nameLen)))
				{
					StrNCat(&buf[i], valueP, valueLen);
					newMsgP->subject = &buf[i];
					i+=valueLen;
					buf[i]='\0';
				}
				if(nameLen == 4 && (valueP) &&
				   (!StrNCaselessCompare(nameP, "body", nameLen)))
				{
					StrNCat(&buf[i], valueP, valueLen);
					newMsgP->body = &buf[i];
					i+=valueLen;
					buf[i]='\0';
				}
			}
		}
		return 0;
	}

	return exgErrBadParam;
}

/***********************************************************************
 *
 * FUNCTION:    TestAddRecord
 *
 * DESCRIPTION: Test the dd record function.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	1/23/97	Initial Revision
 *
 ***********************************************************************/
/*
static void TestAddRecord (void)
{
	MailMessageParamsType	params;


	params.secret = false;
	params.signature = true;
	params.confirmRead = true;
	params.confirmDelivery = true;
	params.priority = priorityHigh;
	params.subject = "Test two of add record";
	params.from = "Art Lamb";
	params.to = "To All";
	params.cc = "Copy";
	params.bcc = "Blind copy";
	params.replyTo = "Reply to";
	params.body = "This is a test";
	
	AddMessage (&params, true);
}
*/

/***********************************************************************
 *
 * FUNCTION:    ListViewSelectRecord
 *
 * DESCRIPTION: Selects (highlights) a record on the table, scrolling
 * 				 to the record if neccessary.
 *
 * PARAMETERS:  recordNum - record to select
 *                		
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden	7/11/97	select the current record in the list view if
 *									there is one.
 *			jmp		10/31/99	Allow scrollToSelect param to force a redraw
 *									of the currently selected record if necessary.
 *
 ***********************************************************************/
static void ListViewSelectRecord (FormPtr frm, UInt16 recordNum, Boolean scrollToSelect)
	{
	Int16 row, column;
	TablePtr tableP;
	UInt16 attr;
			
	if (recordNum == -1)
		return;
		
	ErrFatalDisplayIf (recordNum >= DmNumRecords(MailDB), "Record outside MailDB");
	
	// Make sure that recordNum is in CurrentCategory
	// Get the category and the secret attribute of the current record.
	DmRecordInfo (MailDB, recordNum, &attr, NULL, NULL);	
	if ((attr & dmRecAttrCategoryMask) != CurrentCategory)
		{
		//the current record is not in the current category. Clear the current record global
		CurrentRecord = noRecordSelected;
		return;
		}

	tableP = GetObjectPtr (ListTable);

	// Don't change anything if the same record is selected and we're not
	// trying to force a selection.
	if (TblGetSelection(tableP, &row, &column) &&
		recordNum == TblGetRowID (tableP, row)  &&
		!scrollToSelect)
		{
		return;
		}

	// See if the record is displayed by one of the rows in the table
	// A while is used because if TblFindRowID fails we need to
	// call it again to find the row in the reloaded table.
	while (!TblFindRowID(tableP, recordNum, &row))
		{
		if (!scrollToSelect)
			return;

/*	
		if (HideSecretRecords)
			{
			// If the record is hidden stop trying to show it.
			DmRecordInfo(AddrDB, recordNum, &attr, NULL, NULL);
			if (attr & dmRecAttrSecret)
				{
				return;
				}
			}
*/
				
		// Scroll the view winDown placing the item
		// on the top row
		TopVisibleRecord = recordNum;

		//ListLoadTable();
		ListViewLoadTable (frm, TopVisibleRecord);
		TblRedrawTable(tableP);
		}
	
	// Select the item
	TblSelectItem (tableP, row, 0);
	}//	


// еееееееееееееееееееееееее
//Branch Islands!

/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  formId - id of the form to display
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *
 ***********************************************************************/
void * GetObjectPtr (UInt16 objectID)
{
	FormPtr frm;
	
	frm = FrmGetActiveForm ();
	return (FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, objectID)));

}

/***********************************************************************
 *
 * FUNCTION:    SearchDraw
 *
 * DESCRIPTION: This routine draw the address and subject of a message 
 *              found by the text search routine.
 *
 * PARAMETERS:	 recordP  - a mail record
 *              category - category the record is in
 *              r        - draw region
 *
 * RETURNED:	 nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/27/96	Initial Revision
 *			CS2	10/17/00	Fix bug 42153: address is not displayed in results of [Find]
 *
 ***********************************************************************/
static void SearchDraw (MailDBRecordPtr recordP, UInt16 category, RectanglePtr r)
{
	Char 	name [maxFindFromNameLen];
 	Int16	x, y;
	Int16	width;

	x = r->topLeft.x + 1;
	y = r->topLeft.y;
	width = ((r->extent.x - 2) / 2) - findColumnSpacing;

/*	CS2 (42153): revised logic: If there is a from address, use it, otherwise use the to
	address.  This should superset the previous behavior as outbox messages or draft messages
	will lack Froms, inbox messages will have Froms, and deleted or filed messages will have
	either Froms or Tos based on whether they were received messages or created messages.
*/

	if ( *recordP->from )
	{
		// Get the friendly name if there is one.
		MailParseAddress( recordP->from, true, name, sizeof( name ) );
		DrawString( name, x, y, width );
	}
	else if ( *recordP->to )
	{
		DrawString( recordP->to, x, y, width );
	}
	else
	{
		MemHandle missingH = DmGetResource( strRsc, noToListViewStrID );
		
		if ( missingH )
		{
			MemPtr missingP = MemHandleLock( missingH );

			DrawString( missingP, x, y, width );

			MemPtrUnlock( missingP );
			DmReleaseResource( missingH );
		}
	}

/*	CS2 (42153): old logic
	if (category == outboxCategory || category == draftCategory)
		{
		DrawString (recordP->to, x, y, width);
		}
	else if (*recordP->from)
		{
		// Get the friendly name if there is one.
		MailParseAddress (recordP->from, true, name, sizeof (name));
		DrawString (name, x, y, width);
		}

*/

	x += width + findColumnSpacing;
	DrawString (recordP->subject, x, y, width);
}

/***********************************************************************
 *
 * FUNCTION:    Search
 *
 * DESCRIPTION: This routine searchs the to do database for records 
 *              contains the string passed. 
 *
 * PARAMETERS:	 findParams - text search parameter block
 *
 * RETURNED:	 nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		6/27/96	Initial Revision
 *			MGolden	7/28/97	Added code from 2.0 sdk example for canceling the search
 *       jmp   	10/21/99	Changed params to findParams to match other routines
 *                      	like this one.
 *
 ***********************************************************************/
static void Search (FindParamsPtr findParams)
{
	Err					err;
	UInt16				pos;
	UInt16				attr;
	UInt16				cardNo = 0;
	UInt16				fieldNum;
	UInt16				recordNum;
	Char *				header;
	Boolean				done;
	Boolean				match;
	MemHandle			recordH;
	MemHandle			headerH;
	LocalID				dbID;
	DmOpenRef			dbP;
	RectangleType		r;
	FindParamsPtr		params;
	MailDBRecordType	record;
	DmSearchStateType	searchState;
	UInt16				matchLength;

	params = (FindParamsPtr)findParams;
	
	// Find the application's data file.
	err = DmGetNextDatabaseByTypeCreator (true, &searchState, mailDBType,
					sysFileCMail, true, &cardNo, &dbID);
	if (err)
		{
		findParams->more = false;
		return;
		}

	// Open the to do database.
	dbP = DmOpenDatabase(cardNo, dbID, findParams->dbAccesMode);
	if (!dbP) 
		{
		findParams->more = false;
		return;
		}

	// Display the heading line.
	headerH = DmGetResource (strRsc, findHeaderStrId);
	header = MemHandleLock (headerH);
	done = FindDrawHeader (findParams, header);
	MemHandleUnlock(headerH);
	if (done)
		goto Exit;

	// Search the description and note fields for the "find" string.
	recordNum = findParams->recordNum;
	while (true)
		{
		// Because applications can take a long time to finish a find when
		// the result may be on the screen or for other reasons, users like
		// to be able to stop the find.  Stop the find if an event is pending.
		// This stops if the user does something with the device.  Because
		// this call slows winDown the search we perform it every so many 
		// records instead of every record.  The response time should still
		// be Int16 without introducing much extra work to the search.
		
		// Note that in the implementation below, if the next 16th record is  
		// secret the check doesn't happen.  Generally this shouldn't be a 
		// problem since if most of the records are secret then the search 
		// won't take long anyway!
		if ((recordNum & 0x000f) == 0 &&			// every 16th record
			EvtSysEventAvail(true))
			{
			// Stop the search process.
			findParams->more = true;
			break;
			}

		recordH = DmQueryNextInCategory (dbP, &recordNum, dmAllCategories);

		// Have we run out of records?
		if (! recordH)
			{
			findParams->more = false;			
			break;
			}

		MailGetRecord (dbP, recordNum, &record, &recordH);
		 
		// Search the description field,  if a match is not found search the
		// note field.
		match = MsgSearch (&record, findParams->strToFind, &fieldNum, &pos, &matchLength);
		if (match)
			{
			// Add the match to the find paramter block,  if there is no room to
			// display the match the following function will return true.
			done = FindSaveMatch (findParams, recordNum, pos, fieldNum, matchLength, cardNo, dbID);
			if (done)
				{
				MemHandleUnlock (recordH);
				break;
				}

			// Get the bounds of the region where we will draw the results.
			FindGetLineBounds (findParams, &r);
			
			// Display the match.
			DmRecordInfo (dbP, recordNum, &attr, NULL, NULL);
			SearchDraw (&record, (attr & dmRecAttrCategoryMask), &r);

			findParams->lineNumber++;
			}

		MemHandleUnlock (recordH);
		recordNum++;
		}

Exit:
	DmCloseDatabase (dbP);	
}

/***********************************************************************
 *
 * FUNCTION:    ListViewSelectFont
 *
 * DESCRIPTION: This routine handles selection of a font in the List 
 *              View. 
 *
 * PARAMETERS:  currFontID - id of current font
 *
 * RETURNED:    id of new font
 *
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/10/97	Initial Revision
 *
 ***********************************************************************/
static FontID SelectFont (FontID currFontID)
{
	UInt16 formID;
	FontID fontID;
	
	formID = (FrmGetFormId (FrmGetActiveForm ()));

	// Call the OS font selector to get the id of a font.
	fontID = FontSelect (currFontID);

	if (fontID != currFontID)
		FrmUpdateForm (formID, updateFontChanged);

	return (fontID);
}

/***********************************************************************
 *
 * FUNCTION:    ListViewNumberOfRows
 *
 * DESCRIPTION: This routine return the maximun number of visible rows,
 *              with the current list view font setting.
 *
 * PARAMETERS:  table - List View table
 *
 * RETURNED:    maximun number of displayable rows
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/28/97	Initial Revision
 *
 ***********************************************************************/
static UInt16 ListViewNumberOfRows (TablePtr table)
{
	UInt16				rows;
	UInt16				rowsInTable;
	UInt16				tableHeight;
	FontID			currFont;
	RectangleType	r;


	rowsInTable = TblGetNumberOfRows (table);

	TblGetBounds (table, &r);
	tableHeight = r.extent.y;

	currFont = FntSetFont (ListFont);
	rows = tableHeight / FntLineHeight ();
	FntSetFont (currFont);

	if (rows <= rowsInTable)
		return (rows);
	else
		return (rowsInTable);
}

/***********************************************************************
 *
 * FUNCTION:   NewViewNumberOfRows
 *
 * DESCRIPTION: This routine return the maximun number of visible rows,
 *              with the current list view font setting.
 *
 * PARAMETERS:  table - List View table
 *
 * RETURNED:    maximun number of displayable rows
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/28/97	Initial Revision
 *
 ***********************************************************************/
static UInt16 NewViewNumberOfRows (TablePtr table)
{
	UInt16				rows;
	UInt16				rowsInTable;
	UInt16				tableHeight;
	FontID			currFont;
	RectangleType	r;

	rowsInTable = TblGetNumberOfRows (table);

	TblGetBounds (table, &r);
	tableHeight = r.extent.y;

	currFont = FntSetFont (EditFont);
	rows = tableHeight / FntLineHeight ();
	FntSetFont (currFont);

	if (rows <= rowsInTable)
		return (rows);
	else
		return (rowsInTable);
	}//NewViewNumberOfRows

/***********************************************************************
 *
 * FUNCTION:    NewViewToggleFont
 *
 * DESCRIPTION: This routine changes the font used to display a note.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			MGolden		7/79/96		Initial Revision
 *			mgolden		10/6/97		changed for the rocky fonts
 *
 ***********************************************************************/
static void NewViewToggleFont ()
	{
	FontID originalFont = EditFont, newFont;
	TablePtr table;
	FormPtr frm;
	
	newFont =  FontSelect (originalFont);
		
	if (newFont == originalFont)
		return;
	
	frm = FrmGetActiveForm ();
	table = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, NewTable));
	
	TblReleaseFocus (table);
	
	// HACK! for some reason, the table does not erase the 
	// right edge of the underlined rows (of the edit fields!)
	table->bounds.extent.x += 3;
	TblEraseTable (table);
	table->bounds.extent.x -= 3;


	//change the table rect to be a direct multiple of the new line height?
	table->bounds.extent.y = (gNewViewTableHeight / FntLineHeight ()) * FntLineHeight ();

	// DOLATER (bug 44154) - At this point, if there's a BCC field, and
	// every field has two lines in it, and the font is getting changed
	// from small to big, then the body row is no longer visible. This
	// triggers a field error when the table tries to draw the body text,
	// since it winds up creating a field with a height of zero. On release
	// ROMs, the only problem is that the Body row label gets drawn over
	// the top of the bottom-left button...which seems like a table clipping
	// bug, but something that should be avoided in the app. So I think
	// there are three useful changes that should be made to really fix
	// this bug. (a) Modify the table drawing code to skip drawing a field
	// when the field height is zero. Maybe report this as a non-fatal alert.
	// (b) Modify the table code to not draw static text outside the bounds
	// of the table - this might be a more general fix that covers both
	// a & b. (c) Call a new routine here, which disabled/enables the body
	// row based on whether it's visible.
	
	EditFont = newFont;	
	NewViewLoadTable(frm);	
	TblDrawTable (table);
	NewViewRestoreEditState ();
	}//NewViewToggleFont

/***********************************************************************
 *
 * FUNCTION:    MailLoadPrefs
 *
 * DESCRIPTION: Load the preferences and MemHandle backward and forward
 *				compatibility
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			BGT			1/9/98		Initial Revision
 *
 ***********************************************************************/
static void MailLoadPrefs(void)
{
	UInt16 prefsSize;
	MailPreferenceType prefs;
	Int16 prefsVersion;
	UInt32 fontAttr;

	MessageFont = ListFont = EditFont = (FtrGet (sysFtrCreator, sysFtrDefaultFont, &fontAttr) == 0 ? (FontID) fontAttr : stdFont);

/* HandEra */
	if (vgaExists)
	    MessageFont = ListFont = EditFont = VgaBaseToVgaFont(EditFont);
/* end HandEra */

	prefsSize = sizeof (MailPreferenceType);
	prefsVersion = PrefGetAppPreferences (sysFileCMail, mailPrefID, &prefs, &prefsSize, true);
	if (prefsVersion > mailPrefsVersionNum) {
		prefsVersion = noPreferenceFound;
	}
	if (prefsVersion != noPreferenceFound)
		{
		MessageFont = prefs.messageFont;
		ListFont = prefs.listFont;
		EditFont = prefs.editFont;
		ShowDates = prefs.showDates;
		IncludeOriginalText = prefs.includeOriginalText;
		CommentOriginalText = prefs.commentOriginalText;
		ConfirmDelete = prefs.confirmDelete;
		ShowFullHeader = prefs.showFullHeader;
		AttachSignature = prefs.attachSignature;
		ConfirmRead = prefs.confirmRead;
		ConfirmDelivery = prefs.confirmDelivery;
		
		//mgmg
		MessageRevertCategory = prefs.revertCategory;
		}
}

/***********************************************************************
 *
 * FUNCTION:    MailSavePrefs
 *
 * DESCRIPTION: Save the preferences and prepare for backward compatibility
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			BGT			1/9/98		Initial Revision
 *
 ***********************************************************************/
void MailSavePrefs(void)
{
	MailPreferenceType prefs;

	prefs.messageFont = MessageFont;

	prefs.listFont = ListFont;
	prefs.editFont = EditFont;

	prefs.showDates = ShowDates;
	prefs.includeOriginalText = IncludeOriginalText;
	prefs.commentOriginalText = CommentOriginalText;
	prefs.confirmDelete = ConfirmDelete;
	prefs.showFullHeader = ShowFullHeader;
	prefs.attachSignature = AttachSignature;
	prefs.confirmRead	= ConfirmRead;
	prefs.confirmDelivery =	ConfirmDelivery;
	prefs.revertCategory = MessageRevertCategory;

	PrefSetAppPreferences (sysFileCMail, mailPrefID, mailPrefsVersionNum, 
		&prefs, sizeof (MailPreferenceType), true);
}

#pragma mark - 

/***********************************************************************
 *
 * FUNCTION:    ToolsMoveObject
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
void ToolsMoveObject(FormPtr frmP, UInt16 objIndex, Coord x_diff, Coord y_diff, Boolean draw)
{
    Coord   old_x, old_y;
    
    if(draw)
        FrmHideObject(frmP, objIndex);

    FrmGetObjectPosition(frmP, objIndex, &old_x, &old_y);
    FrmSetObjectPosition(frmP, objIndex, old_x + x_diff, old_y + y_diff);

    if(draw)
        FrmShowObject(frmP, objIndex);
}

/***********************************************************************
 *
 * FUNCTION:    ToolsResizeObject
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
void ToolsResizeObject(FormPtr frmP, UInt16 objIndex, Coord x_diff, Coord y_diff, Boolean draw)
{
    RectangleType r;
    
    if(draw)
        FrmHideObject(frmP, objIndex);

    FrmGetObjectBounds(frmP, objIndex, &r);
    r.extent.x += x_diff;
    r.extent.y += y_diff;
    FrmSetObjectBounds(frmP, objIndex, &r);

    if(draw)
        FrmShowObject(frmP, objIndex);
}


/***********************************************************************
 *
 * FUNCTION:    ToolsFrmGetGSI
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
UInt16 ToolsFrmGetGSI(FormPtr frmP)
{
    UInt16 retval = -1;
    UInt16 i, num;
    
    num = FrmGetNumberOfObjects(frmP);
    
    for (i=0;i<num;i++)
    {
        if (FrmGetObjectType(frmP, i)==frmGraffitiStateObj)
        {
            retval = i;
            break;
        }
    }
    
    return retval;
}

/***********************************************************************
 *
 * FUNCTION:    ToolsFrmGetGSI
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
FontID ToolsVgaFntSetFont(FontID select, FontID reference)
{
	if (VgaIsVgaFont(reference))
	    return FntSetFont (VgaBaseToVgaFont(select));
	else
	    return FntSetFont(select);
}

/***********************************************************************
 *
 * FUNCTION:    ToolsFrmGetGSI
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
Boolean ToolsVgaIsLandscapeMode(void)
{
    Boolean                 landscape = false;
    VgaRotateModeType       rotation;

	if (vgaExists)
    {
        VgaGetScreenMode(NULL, &rotation);
        landscape = (rotation == rotateMode90) || (rotation == rotateMode270);
    }

    return landscape;
}

/***********************************************************************
 *
 * FUNCTION:    ToolsFrmGetGSI
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *
 * RETURNED:
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
void ToolsVgaAdjustModalForm(FormPtr frmP, Boolean canChangeScreenMode)
{
    VgaScreenModeType   mode;
    VgaRotateModeType   rotation;
	RectangleType       r, frmBounds;
    WinPtr              saveWindow;
    Int16               maxY;
    Boolean             expand, landscape;

    if (vgaExists)
    {
        VgaGetScreenMode(&mode, &rotation);
        if(mode == screenMode1To1)
        {
            FrmGetFormBounds(frmP, &frmBounds);
            landscape = (rotation == rotateMode90) || (rotation == rotateMode270);

            if (landscape)
                maxY = 160;
            else
                maxY = 240;

            expand = (frmBounds.extent.y * 3 / 2) <= maxY;

            if(expand)
            {
                VgaFormModify(frmP, vgaFormModify160To240);

                if(landscape)
                {
                    FrmGetFormBounds(frmP, &frmBounds); //Get new expanded bounds
                    saveWindow = WinSetDrawWindow(FrmGetWindowHandle(frmP));
                    WinGetDrawWindowBounds(&r);
                    WinSetDrawWindow(saveWindow);
                    r.topLeft.y = maxY - ((frmBounds.topLeft.y + frmBounds.extent.y) - r.topLeft.y); 
                    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);
                }
            }
            else if (canChangeScreenMode)
                VgaSetScreenMode(screenModeOffset, rotation);
        }
    }
}
