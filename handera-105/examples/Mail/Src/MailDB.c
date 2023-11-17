/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailDB.c
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file contains the database functions of the Mail application.
 *
 * History:
 *		May 30, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#include <PalmOS.h>
#include <PalmUtils.h>
#include "Mail.h"

/************************************************************
 * DB initialization date
 *************************************************************/

// DOLATER kwk - decide if this data should be put into a resource
// for localization into other languages.

MailAppInfoType DefaultMailApplicationInfo = {
 	// Renamed categories
 	0x000f,
 	
 	// Category labels
	"Inbox",
	"Outbox",
	"Deleted",
	"Filed",
	"Draft",	"",	"",	"",
	"",	"",	"",	"",
	"",	"",	"",	"",
	
	// Category Uniq IDs
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
	
	// Last Uniq ID
	15,

	// Reserved
	0, 0,

 	// Dirty app info
 	0xFFFF,
	
	// sort by priority, due date
	sortByDate,

	// unsend message MemHandle
	0,
	
	// signature offset
	sizeof (MailAppInfoType),
	};


/************************************************************
 *
 *  FUNCTION: ECMailDBValidate
 *
 *  DESCRIPTION: This routine validates the integrity of the mail
 *		datebase.
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS: nothing
 *
 *  CREATED: 11/6/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL

void ECMailDBValidate (DmOpenRef dbP)
{
	UInt16 				i;
	UInt16 				numRecord;
	MemHandle			recordH;
	MailDBRecordType	record;
	
	numRecord = DmNumRecords (dbP);
	for (i = 0 ; i < numRecord; i++)
		{
		recordH = DmQueryRecord (dbP, i);
		if (! recordH) continue;

		MailGetRecord (dbP, i, &record, &recordH);

		ErrFatalDisplayIf ((record.flags.priority > priorityLow),
			"Invalid priority");
		
		ErrFatalDisplayIf ((record.flags.addressing > sentBCC),
			"Invalid addressing flag");
		
		MemHandleUnlock (recordH);
		}
}
#endif


/***********************************************************************
 *
 * FUNCTION:    DateCompare
 *
 * DESCRIPTION: This routine compares two dates.
 *
 * PARAMETERS:  d1 - a date 
 *              d2 - a date 
 *
 * RETURNED:    if d1 > d2  returns a positive int
 *              if d1 < d2  returns a negative int
 *              if d1 = d2  returns zero
 *
 * NOTE: This routine treats the DateType structure like an unsigned int,
 *       it depends on the fact the the members of the structure are ordered
 *       year, month, day form high bit to low low bit.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/95		Initial Revision
 *
 ***********************************************************************/
static Int16 DateCompare (DateType d1, DateType d2)
{
	UInt16 int1, int2;
	
	int1 = DateToInt(d1);
	int2 = DateToInt(d2);
	
	if (int1 > int2)
		return (1);
	else if (int1 < int2)
		return (-1);
	return 0;
}

/***********************************************************************
 *
 * FUNCTION:    TimeCompare
 *
 * DESCRIPTION: This routine compares two times.  "No time" is represented
 *              by minus one, and is considered less than all times.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    if t1 > t2  returns a positive int
 *              if t1 < t2  returns a negative int
 *              if t1 = t2  returns zero
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/95	Initial Revision
 *
 ***********************************************************************/
static Int16 TimeCompare (TimeType t1, TimeType t2)
{
	Int16 int1, int2;
	
	int1 = TimeToInt(t1);
	int2 = TimeToInt(t2);
	
	if (int1 > int2)
		return (1);
	else if (int1 < int2)
		return (-1);
	return 0;

}


/************************************************************
 *
 *  FUNCTION: CompareRecords
 *
 *  DESCRIPTION: Compare two records.
 *
 *  PARAMETERS: database record 1
 *				    database record 2
 *
 *  RETURNS: -n if record one is less (n != 0)
 *			     n if record two is less
 *
 *  CREATED: 1/23/95 
 *
 *  BY: Roger Flores
 *
 *	COMMENTS:	Compare the two records key by key until
 *	there is a difference.  Return -n if r1 is less or n if r2
 *	is less.  A zero is never returned because if two records
 *	seem identical then their unique IDs are compared!
 *
 * This function accepts record data chunk pointers to avoid
 * requiring that the record be within the database.  This is
 * important when adding records to a database.  This prevents
 * determining if a record is a deleted record (which are kept
 * at the end of the database and should be considered "greater").
 * The caller should test for deleted records before calling this
 * function!
 *
 * REVISION HISTORY:
 *			Name				Date		Description
 *			----				----		-----------
 *			Roger Flores	1/23/95	Initial Revision
 *			MGolden			7/14/97	added sort by read at the end if the result was 0.
 *
 *************************************************************/
static Int16 CompareRecords (MailPackedDBRecordPtr r1, MailPackedDBRecordPtr r2,
		 		Int16 sortOrder, SortRecordInfoPtr UNUSED_PARAM(info1), 
				SortRecordInfoPtr UNUSED_PARAM(info2), MemHandle UNUSED_PARAM(appInfoH))
{
	Int16		i;
	Int16 	result;
	Char 		name1 [maxFriendlyNameLen];
	Char 		name2 [maxFriendlyNameLen];
	Char *	p1;
	Char *	p2;

	// Sort high priority message to the top.
	if (r1->flags.priority == priorityHigh || 
		 r2->flags.priority == priorityHigh)
		result = (Int16)r1->flags.priority - (Int16)r2->flags.priority;
	else
		result = 0;


	if (result == 0)
		{
		// Sort by date
		if (sortOrder == sortByDate)
			{
			result = DateCompare (r2->date, r1->date);
			if (result == 0)
				{
				result = TimeCompare (r2->time, r1->time);
				}
			}
	
		// Sort by subject.
		else if (sortOrder == sortBySubject)
			result = StrCaselessCompare (&r1->firstField, &r2->firstField);
		
		// Sort by "from" field.
		else
			{
			p1 = &r1->firstField;
			p2 = &r2->firstField;
			for (i = mailFieldSubject; i < mailFieldFrom; i++)
				{
				p1 += StrLen(p1) + 1;
				p2 += StrLen(p2) + 1;
				}
			MailParseAddress (p1, true, name1, maxFriendlyNameLen);
			MailParseAddress (p2, true, name2, maxFriendlyNameLen);

			result = StrCaselessCompare (name1, name2);
			}
		}

//mgmg
	if (result == 0)
		{
		//sort by the read flag
		if (r2->flags.read != r1->flags.read)
			{
			if (r1->flags.read)
				result = 1;
			else
				result = -1;
			}
		}	

	return result;
}


/************************************************************
 *
 *  FUNCTION: FindSortPosition
 *
 *  DESCRIPTION: Return where a record is or should be
 *		           Useful to find or find where to insert a record.
 *
 *  PARAMETERS: database record (not deleted!)
 *
 *  RETURNS: the size in bytes
 *
 *  CREATED: 5/30/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
static UInt16 FindSortPosition (DmOpenRef dbP, MailPackedDBRecordPtr r)
{
	int sortOrder;
	
	sortOrder = MailGetSortOrder (dbP);
	
	return (DmFindSortPosition (dbP, r, NULL, (DmComparF *) CompareRecords, sortOrder));
}


/************************************************************
 *
 *  FUNCTION: PackedRecordSize
 *
 *  DESCRIPTION: Return the packed size of an MailDBRecordType 
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the size in bytes
 *
 *  CREATED: 5/30/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
static UInt16 PackedRecordSize (MailDBRecordPtr r)
{
	UInt16 size;
	
	size = sizeof (MailPackedDBRecordType) + 
		(mailFieldBody - mailFieldSubject);
	
	if (r->subject != NULL)
		size += StrLen (r->subject);
	
	if (r->from != NULL)
		size += StrLen (r->from);
	
	if (r->to != NULL)
		size += StrLen (r->to);
	
	if (r->cc != NULL)
		size += StrLen (r->cc);
	
	if (r->bcc != NULL)
		size += StrLen (r->bcc);
	
	if (r->replyTo != NULL)
		size += StrLen (r->replyTo);
	
	if (r->sentTo != NULL)
		size += StrLen (r->sentTo);
	
	if (r->body != NULL)
		size += StrLen (r->body);
	
	return size;
}


/************************************************************
 *
 *  FUNCTION: PackRecord
 *
 *  DESCRIPTION: Pack an MailDBRecordType
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the MailPackedDBRecord is packed
 *
 *  CREATED: 5/30/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
static void PackRecord (MailDBRecordPtr s, MailPackedDBRecordPtr d)
{
	UInt16 	size;
	Char	zero = 0;
	UInt32	offset = 0;
	
	
	// Write date field.
	DmWrite (d, offset, &s->date, sizeof(DateType));
	offset += sizeof (DateType);
	
	// Write time field.
	DmWrite (d, offset, &s->time, sizeof(TimeType));
	offset += sizeof (TimeType);
	
	// Write flags field.
	DmWrite (d, offset, &s->flags, sizeof(MailFlagsType));
	offset += sizeof (MailFlagsType);

	// Write subject field.
	if (s->subject != NULL)
		{
		size = StrLen (s->subject) + 1;
		DmWrite (d, offset, s->subject, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}
	
	// Write "from" field.
	if (s->from != NULL)
		{
		size = StrLen (s->from) + 1;
		DmWrite (d, offset, s->from, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}
	
	// Write "to" field.
	if (s->to != NULL)
		{
		size = StrLen (s->to) + 1;
		DmWrite (d, offset, s->to, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}
	
	// Write "cc" field.
	if (s->cc != NULL)
		{
		size = StrLen (s->cc) + 1;
		DmWrite (d, offset, s->cc, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}
	
	// Write "bcc" field.
	if (s->bcc != NULL)
		{
		size = StrLen (s->bcc) + 1;
		DmWrite (d, offset, s->bcc, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}
	
	// Write "reply to" field.
	if (s->replyTo != NULL)
		{
		size = StrLen (s->replyTo) + 1;
		DmWrite (d, offset, s->replyTo, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}
	
	// Write "sent to" field.
	if (s->sentTo != NULL)
		{
		size = StrLen (s->sentTo) + 1;
		DmWrite (d, offset, s->sentTo, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}

	// Write "body" field.
	if (s->body != NULL)
		{
		size = StrLen (s->body) + 1;
		DmWrite (d, offset, s->body, size);
		offset += size;
		}
	else
		{
		DmWrite (d, offset, &zero, 1);
		offset++;
		}	
}


/************************************************************
 *
 *  FUNCTION: MailUnpackRecord
 *
 *  DESCRIPTION: Fills in the MailDBRecord structure
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the record unpacked
 *
 *  CREATED: 5/30/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
void MailUnpackRecord (MailPackedDBRecordPtr src, MailDBRecordPtr dest)
{
	char *p;

	
	dest->date = src->date;
	dest->time = src->time;
	dest->flags = src->flags;

	p = &src->firstField;

	// Get the "subject" field.
	if (*p)
		{
		dest->subject = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->subject = p;
		p++;
		}
	
	// Get the "from" field.	
	if (*p)
		{
		dest->from = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->from = p;
		p++;
		}
	
	// Get the "to" field.	
	if (*p)
		{
		dest->to = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->to = p;
		p++;
		}
	
	// Get the "cc" field.	
	if (*p)
		{
		dest->cc = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->cc = p;
		p++;
		}
	
	// Get the "bcc" field.	
	if (*p)
		{
		dest->bcc = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->bcc = p;
		p++;
		}
	
	// Get the "reply to" field.	
	if (*p)
		{
		dest->replyTo = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->replyTo = p;
		p++;
		}
	
	// Get the "sent to" field.	
	if (*p)
		{
		dest->sentTo = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->sentTo = p;
		p++;
		}
	
	// Get the "body" field.	
	if (*p)
		{
		dest->body = p;
		p += StrLen(p) + 1;
		}
	else
		{
		dest->body = p;
		p++;
		}
}


/************************************************************
 *
 *  FUNCTION: MailLocalizeAppInfo
 *
 *  DESCRIPTION: Look for localize app info strings and copy
 *  them into the app info block.
 *
 *  PARAMETERS: application info ptr
 *
 *  RETURNS: nothing
 *
 *  CREATED: 12/13/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static void MailLocalizeAppInfo (MailAppInfoPtr appInfoP)
{
	MemHandle 		localizedAppInfoH;
	Char *	 		localizedAppInfoP;
	MailAppInfoPtr	nilP = 0;
	MemHandle 		stringsH;
	Char *			*stringsP;
	int 				i;


	localizedAppInfoH = DmGetResource (appInfoStringsRsc, mailLocalizedAppInfoStr);
	if (localizedAppInfoH)
		{
		localizedAppInfoP = MemHandleLock (localizedAppInfoH);
		stringsH = SysFormPointerArrayToStrings (localizedAppInfoP, 
			dmRecNumCategories);
		stringsP = MemHandleLock (stringsH);
		
		// Copy each category
		for (i = 0; i < dmRecNumCategories; i++)
			{
			if (stringsP[i][0] != '\0')
				DmStrCopy (appInfoP, (UInt32) nilP->categoryLabels[i], stringsP[i]);
			}
		
		MemPtrFree (stringsP);
		MemPtrFree (localizedAppInfoP);
		}
}


/************************************************************
 *
 *  FUNCTION: MailAppInfoInit
 *
 *  DESCRIPTION: Create an app info chunk if missing.  Set
 *		the strings to a default.
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS: 0 if successful, errorcode if not
 *
 *  CREATED: 1/20/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
Err	MailAppInfoInit (DmOpenRef dbP)
{
	UInt16 size;
	UInt16 cardNo;
	UInt16 wordValue;
	MemHandle h;
	LocalID dbID;
	LocalID appInfoID;
	MailAppInfoPtr	nilP = 0;
	MailAppInfoPtr appInfoP;
	
	if (DmOpenDatabaseInfo (dbP, &dbID, NULL, NULL, &cardNo, NULL))
		return dmErrInvalidParam;

	if (DmDatabaseInfo (cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, 
		 NULL, &appInfoID, NULL, NULL, NULL))
		return dmErrInvalidParam;
	
	if (appInfoID == 0)
		{
		// Add one for signature null-terminator.
		size = sizeof (MailAppInfoType) + 1;
		h = DmNewHandle (dbP, size);
		if (! h) return dmErrMemError;

		appInfoID = MemHandleToLocalID (h);
		DmSetDatabaseInfo(cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, 
			NULL, &appInfoID, NULL, NULL, NULL);
		}
	
	appInfoP = MemLocalIDToLockedPtr (appInfoID, cardNo);

	// Clear the app info block.
	DmSet (appInfoP, 0, size, 0);

	// Initialize the categories.
	CategoryInitialize ((AppInfoPtr) appInfoP, mailLocalizedAppInfoStr);

	// I don't know this field is used for.
	wordValue = 0xFFFF;
	DmWrite (appInfoP, (UInt32)&nilP->dirtyAppInfo, &wordValue,
		sizeof(appInfoP->dirtyAppInfo));

	// Initialize the sort order.
	DmSet (appInfoP, (UInt32)&nilP->sortOrder, sizeof(appInfoP->sortOrder), 
		sortByDate);

	// Initialize the field the holds the index of the last unsent message.
//	wordValue= noUnsendMessage;
//	DmWrite (appInfoP, (UInt32)&nilP->unsentMessage, &wordValue,
//		sizeof(appInfoP->unsentMessage));

	// Initialize the offset of message signature.
	wordValue = sizeof (MailAppInfoType);
	DmWrite (appInfoP, (UInt32)&nilP->signatureOffset, &wordValue,
		sizeof(appInfoP->signatureOffset));

	// Initialize the message signature.
	DmSet (appInfoP, appInfoP->signatureOffset, 1, 0);

	MemPtrUnlock (appInfoP);

	return 0;
}


/************************************************************
 *
 *  FUNCTION: MailGetAppInfo
 *
 *  DESCRIPTION: Get the app info chunk 
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS: MemHandle to the to do application info block (MailAppInfoType)
 *
 *  CREATED: 5/12/95 
 *
 *  BY: Art Lamb
 *
 *************************************************************/
MemHandle MailGetAppInfo (DmOpenRef dbP)
{
	Err error;
	UInt16 cardNo;
	LocalID dbID;
	LocalID appInfoID;
	
	error = DmOpenDatabaseInfo (dbP, &dbID, NULL, NULL, &cardNo, NULL);
	ErrFatalDisplayIf (error,  "Get getting app info");

	error = DmDatabaseInfo (cardNo, dbID, NULL, NULL, NULL, NULL, NULL, 
			NULL, NULL, &appInfoID, NULL, NULL, NULL);
	ErrFatalDisplayIf (error,  "Get getting app info");

	return (MemLocalIDToGlobal (appInfoID, cardNo));
}


/************************************************************
 *
 *  FUNCTION: MailGetSortOrder
 *
 *  DESCRIPTION: This routine get the sort order value from the 
 *               mail application info block.
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS:    sort order:
 *                  0 - by date
 *						  1 - by subject
 *						  2 - by form
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/30/96	Initial Revision
 *
 *************************************************************/
UInt8 MailGetSortOrder (DmOpenRef dbP)
{
	UInt8 sortOrder;
	MailAppInfoPtr appInfoP;
			
	appInfoP = MemHandleLock (MailGetAppInfo (dbP));
	sortOrder = appInfoP->sortOrder;
	MemPtrUnlock (appInfoP);	

	return (sortOrder);
}


/************************************************************
 *
 *  FUNCTION: MailChangeSortOrder
 *
 *  DESCRIPTION: Change the Mail Database's sort order
 *
 *  PARAMETERS: database pointer
 *				TRUE if sort by company
 *
 *  RETURNS: nothing
 *
 *  CREATED: 1/17/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
Err MailChangeSortOrder(DmOpenRef dbP, Boolean sortOrder)
{
	MailAppInfoPtr appInfoP;
	MailAppInfoPtr	nilP = 0;
	UInt16				dirtyAppInfo;


	appInfoP = MemHandleLock (MailGetAppInfo (dbP));

	if (appInfoP->sortOrder != sortOrder)
		{
		dirtyAppInfo = appInfoP->dirtyAppInfo | mailSortByDirty;
		DmWrite(appInfoP, (UInt32)&nilP->dirtyAppInfo, &dirtyAppInfo, sizeof(appInfoP->dirtyAppInfo)); 
		DmWrite(appInfoP, (UInt32)&nilP->sortOrder, &sortOrder, sizeof(appInfoP->sortOrder));
		
		DmInsertionSort(dbP, (DmComparF *) &CompareRecords, (Int16) sortOrder);
		}
		
	MemPtrUnlock (appInfoP);	

	return 0;
}


/************************************************************
 *
 *  FUNCTION: MailSort
 *
 *  DESCRIPTION: Sort the appointment database.
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: nothing
 *
 *  CREATED: 10/17/95 
 *
 *  BY: Art Lamb
 *
 *************************************************************/
void MailSort (DmOpenRef dbP)
{
	int sortOrder;
	
	sortOrder = MailGetSortOrder (dbP);
	DmInsertionSort(dbP, (DmComparF *) &CompareRecords, (Int16) sortOrder);
}


/***********************************************************************
 *
 * FUNCTION:    MailGetUnsentMessage
 *
 * DESCRIPTION: This routine returns the ihndex of the unsent message,
 *              if there is one.  If the application is exited while 
 *              a new message is being edited, the MemHandle of the unsent 
 *              message is save in the application info block.  The 
 *              record is not yet attached to the database index.
 *
 * PARAMETERS:  dpP - database pointer
 *
 * RETURNED:    unique id of a packed record or -1
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
UInt32 MailGetUnsentMessage (DmOpenRef dbP)
{
	UInt32 uniqueID;
	MailAppInfoPtr appInfoP;
			
	appInfoP = MemHandleLock (MailGetAppInfo (dbP));
	uniqueID = appInfoP->unsentMessage;
	MemPtrUnlock (appInfoP);	

	return (uniqueID);
}


/***********************************************************************
 *
 * FUNCTION:    MailSetUnsentMessage
 *
 * DESCRIPTION: This routine sets the index of the unsent message.
 *              If the application is exited while a new message 
 *              is being edited, the index of the unsent message 
 *              is save in the application info block.
 *
 * PARAMETERS:  dpP      - database pointer
 *					 uniqueID - unique id of a packed record or -1
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/12/96	Initial Revision
 *
 ***********************************************************************/
void MailSetUnsentMessage (DmOpenRef dbP, UInt32 uniqueID)
{
	MailAppInfoPtr	nilP = 0;
	MailAppInfoPtr appInfoP;
			
	appInfoP = MemHandleLock (MailGetAppInfo (dbP));
	DmWrite(appInfoP, (UInt32)&nilP->unsentMessage, &uniqueID, 
		sizeof(appInfoP->unsentMessage));

	MemPtrUnlock (appInfoP);	
}


/***********************************************************************
 *
 * FUNCTION:    MailGetSignature
 *
 * DESCRIPTION: This routine returns a copy of the message signature 
 *              string.  This routine allocates a new MemHandle to hold
 *              signature,  the caller is responsible for freeing this 
 *              MemHandle.
 *              
 * PARAMETERS:  dpP - database pointer
 *
 * RETURNED:    MemHandle of a signature or zero
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/25/96	Initial Revision
 *
 ***********************************************************************/
MemHandle MailGetSignature (DmOpenRef dbP)
{
	UInt16 len;
	Char * p;
	Char * signatureP;
	MemHandle h = 0;
	MailAppInfoPtr appInfoP;
			
	appInfoP = MemHandleLock (MailGetAppInfo (dbP));

	signatureP = ((Char *)appInfoP) + appInfoP->signatureOffset;
	len = StrLen (signatureP);
	
	if (len)
		{
		h = MemHandleNew (len + 1);
		p = MemHandleLock (h);
		StrCopy (p, signatureP);
		MemPtrUnlock (p);
		}

	MemPtrUnlock (appInfoP);	

	return (h);
}


/***********************************************************************
 *
 * FUNCTION:    MailParseAddress
 *
 * DESCRIPTION: This routine parses a mail address
 *
 * PARAMETERS:  address  - address string, ex: "art@palm (Art Lamb)"
 *              realName - if true return the name part, else return the 
 *                         address part
 *              buffer   - buffer to hold return value
 *              len      - length of buffer
 *              
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	06/12/96	Initial Revision
 *			kwk	12/02/98	Fixed bug w/calling TxtCharIsCntrl with bytes.
 *
 ***********************************************************************/
void MailParseAddress (Char * address, Boolean realName, Char * buffer,
			UInt16 bufferLen)
{
	UInt16 len;
	Char* p;
	Char* endP;
	Char* startP;

	*buffer = 0;

	if (realName)
		{
		// Get the name portion of the string passed.
		endP = StrChr (address, linefeedChr);
		if (! endP)
			endP = StrChr (address, 0);

		startP = StrChr (address, '(');

		if (startP && startP < endP)
			{
			startP++;
			len = endP - startP;
			while (endP-- > startP)
				{
				if (*endP == ')')
					{
					len = endP - startP;
					break;
					}
				}
			if (len > bufferLen - 1)
				len =  bufferLen - 1;

			p = buffer;
			while (len--)
				{
				*p++ = *startP++;
				}
			*p = 0;
			}
		}

	if (*buffer == 0)
		{
		len = 0;
		while (true)
			{
			WChar theChar;
			address += TxtGetNextChar(address, 0, &theChar);
			if (TxtCharIsCntrl(theChar) || (theChar == chrLeftParenthesis))
				break;
			else if (len + TxtCharSize(theChar) > bufferLen - 1)
				break;
			else
				len += TxtSetNextChar(buffer, len, theChar);
			}
		
		buffer[len] = 0;
		
		// Remove trailing spaces.
		while (len)
			{
			WChar theChar;
			len -= TxtGetPreviousChar(buffer, len, &theChar);
			if (TxtCharIsSpace(theChar))
				buffer[len] = 0;
			else
				break;
			}
		}		
}


/************************************************************
 *
 *  FUNCTION: MailNewRecord
 *
 *  DESCRIPTION: Create a new packed record.  The new record is 
 *
 *  PARAMETERS: database pointer
 *				    database record
 *
 *  RETURNS:    record MemHandle or 0 if error
 *
 *  CREATED: 6/6/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
Err MailNewRecord (DmOpenRef dbP, MailDBRecordPtr r, UInt16 *indexP)
{
	Err err;
	UInt16 index;
	MemHandle recordH;
	MailPackedDBRecordPtr recordP;
	
	// Make a new chunk with the correct size.
	recordH = DmNewHandle (dbP, (UInt32) PackedRecordSize (r));
	if (recordH == NULL)
		return dmErrMemError;

	// Copy the data from the unpacked record to the packed one.
	recordP = MemHandleLock (recordH);
	PackRecord (r, recordP);

	// Determine the sort position of the new record.
	index = FindSortPosition (dbP, recordP);

	MemPtrUnlock (recordP);

	err = DmAttachRecord (dbP, &index, recordH, 0);
	if (err) 
		MemHandleFree (recordH);
	else
		*indexP = index;

	return (err);
}


/************************************************************
 *
 *  FUNCTION: MailAttachRecord
 *
 *  DESCRIPTION: Attach a packed record in sorted position.
 *
 *  PARAMETERS: database pointer
 *              database record MemHandle
 *
 *  RETURNS: 0 if successful, error code if not
 *
 *  CREATED: 6/6/96
 *
 *  BY: Art Lamb
 *
 *************************************************************/
Err MailAttachRecord (DmOpenRef dbP, MemHandle recordH, UInt16 * indexP)
{
	Err err;
	UInt16 index;
	MailPackedDBRecordPtr recordP;

	recordP = MemHandleLock (recordH);

	index = FindSortPosition (dbP, recordP);

	MemPtrUnlock (recordP);

	err = DmAttachRecord (dbP, &index, recordH, 0);
	if (err) 
		MemHandleFree (recordH);
	else
		*indexP = index;
	
	return err;
}


/************************************************************
 *
 *  FUNCTION: MailGetRecord
 *
 *  DESCRIPTION: Get a record from a Mail database
 *
 *  PARAMETERS: database pointer
 *				    database index
 *				    database record
 *
 *  RETURNS: 0 if successful, errorcode if not
 *
 *  CREATED: 5/12/95 
 *
 *  BY: Art Lamb
 *
 *************************************************************/
Err MailGetRecord (DmOpenRef dbP, UInt16 index, MailDBRecordPtr r, 
	MemHandle * handleP)
{
	MemHandle handle;
	MailPackedDBRecordPtr src;
	

	handle = DmQueryRecord(dbP, index);
	ErrFatalDisplayIf(DmGetLastErr(), "Error Querying record");
	
	src = (MailPackedDBRecordPtr) MemHandleLock (handle);

	if (DmGetLastErr())
		{
		*handleP = 0;
		return DmGetLastErr();
		}
	
	MailUnpackRecord (src, r);
	
	*handleP = handle;
	return 0;
}


/************************************************************
 *
 *  FUNCTION: MailChangeRecord
 *
 *  DESCRIPTION: Change a record in the Appointment Database
 *
 *  PARAMETERS: database pointer
 *					 database index
 *					 database record
 *					 changed fields
 *
 *  RETURNS: 0 if successful, errorcode if not
 *
 *  RETURNS: 0 if successful, errorcode if not
 *
 *  CREATED: 5/12/95 
 *
 *	COMMENTS:	Records are not stored with extra padding - they
 *	are always resized to their exact storage space.  This avoids
 *	a database compression issue.  The code works as follows:
 *	
 *	1)	get the size of the new record
 *	2)	make the new record
 *	3)	pack the packed record plus the changes into the new record
 *	4)	if the sort position is changes move to the new position
 *	5)	attach in position
 *
 *************************************************************/
Err MailChangeRecord (DmOpenRef dbP, UInt16 * index, MailDBRecordPtr r, 
	MailChangedFieldsType changedFields)
{
	Err 							result;
	Int16 							newIndex;
	UInt16 							attributes;
	Boolean 						dontMove;
	MemHandle 					oldH;
	MemHandle 					srcH;
	MemHandle 					dstH;
	MailDBRecordType 			src;
	MailPackedDBRecordPtr 	dst;
	MailPackedDBRecordPtr 	cmp;
	
	// We do not assume that r is completely valid so we get a valid
	// MailDBRecordPtr...
	if ((result = MailGetRecord(dbP, *index, &src, &srcH)) != 0)
		return result;
	
	// and we apply the changes to it.
	if (changedFields.date) 
		src.date = r->date;
	
	if (changedFields.time) 
		src.time = r->time;

	if (changedFields.flags) 
		src.flags = r->flags;
	
	if (changedFields.subject)
		src.subject = r->subject;
	
	if (changedFields.from)
		src.from = r->from;
		
	if (changedFields.to)
		src.to = r->to;
		
	if (changedFields.cc)
		src.cc = r->cc;
		
	if (changedFields.bcc)
		src.bcc = r->bcc;
		
	if (changedFields.replyTo)
		src.replyTo = r->replyTo;
		
	if (changedFields.sentTo)
		src.sentTo = r->sentTo;
		
	if (changedFields.body)
		src.body = r->body;
		

	// Allocate a new chunk with the correct size and pack the data from 
	// the unpacked record into it.
	dstH = DmNewHandle(dbP, (UInt32) PackedRecordSize (&src));
	if (dstH)
		{
		dst = MemHandleLock (dstH);
		PackRecord (&src, dst);
		}

	MemHandleUnlock (srcH);
	if (dstH == NULL)
		return dmErrMemError;


	// If the sort position is changed move to the new position.
	// Check if any of the key fields have changed. 
	if ((! changedFields.date) && 
	    (! changedFields.time) && 
	    (! changedFields.subject) && 
	    (! changedFields.from))
		goto attachRecord;
	
		
	// Make sure *index-1 < *index < *index+1, if so it's in sorted 
	// order.  Leave it there.	
	if (*index > 0)
		{
		// This record wasn't deleted and deleted records are at the end of the
		// database so the prior record may not be deleted!
		cmp = MemHandleLock (DmQueryRecord(dbP, *index-1));		
		dontMove = (CompareRecords (cmp, dst, 0, NULL, NULL, 0) == -1);
		MemPtrUnlock (cmp);
		}
	else 
		dontMove = true;


	if (dontMove && (*index+1 < DmNumRecords (dbP)))
		{
		DmRecordInfo(dbP, *index+1, &attributes, NULL, NULL);
		if ( ! (attributes & dmRecAttrDelete) )
			{
			cmp = MemHandleLock (DmQueryRecord(dbP, *index+1));
			dontMove &= (CompareRecords (dst, cmp, 0, NULL, NULL, 0) == -1);
			MemPtrUnlock (cmp);
			}
		}
		
	if (dontMove)
		goto attachRecord;
	
	
	// The record isn't in the right position.  Move it.
	newIndex = FindSortPosition (dbP, dst);
	DmMoveRecord (dbP, *index, newIndex);
	if (newIndex > *index) newIndex--;
	*index = newIndex;						// return new position


attachRecord:
	// Attach the new record to the old index,  the preserves the 
	// category and record id.
	result = DmAttachRecord (dbP, index, dstH, &oldH);
	
	MemPtrUnlock (dst);

	if (result) return result;

	MemHandleFree (oldH);

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:    GetRecordField
 *
 * DESCRIPTION: This routine changes a field of the passed record.
 *
 * PARAMETERS:  dpP         - database pointer
 *					 recordField - fields to chance
 *              data        - new field value
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/17/96	Initial Revision
 *
 ***********************************************************************/
static void * GetRecordField (MailDBRecordPtr recordP, 
		MailRecordFieldType recordField)
{
	void * ptr;

	switch (recordField)
		{
		case mailFieldSubject:
			ptr = recordP->subject;
			break;
			
		case mailFieldFrom:
			ptr = recordP->from;
			break;
			
		case mailFieldTo:
			ptr = recordP->to;
			break;
			
		case mailFieldCC:			
			ptr = recordP->cc;
			break;
			
		case mailFieldBCC:		
			ptr = recordP->bcc;
			break;
			
		case mailFieldReplyTo:		
			ptr = recordP->replyTo;
			break;
			
		case mailFieldSentTo:		
			ptr = recordP->sentTo;
			break;
			
		case mailFieldBody:				
			ptr = recordP->body;
			break;

		default:
			ErrDisplay ("Invalid record field");
		}

	return (ptr);
}


/***********************************************************************
 *
 * FUNCTION:    MailChangeRecordField
 *
 * DESCRIPTION: This routine changes a field of the passed record.
 *
 * PARAMETERS:  dpP         - database pointer
 *					 hP          - pointer to the MemHandle of a packed database
 *                            record. A new MemHandle may be returned as
 *                            the result of resizing the record.
 *					 recordField - fields to chance
 *
 * RETURNED:    nothing
 *
 *	COMMENTS:   This routine does not move the modified record to to 
 *             correct sort position if one of the sort key field is
 *             modified.  This function is intented for use on 
 *             records that have not been attached to the database.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/11/96	Initial Revision
 *
 ***********************************************************************/
/*
Err MailChangeRecordField (DmOpenRef dbP, UInt16 index, 
	MailRecordFieldType recordField, void * data)
{
	Err							err = 0;
	UInt16							newSize;
	UInt16							oldSize;
	MemHandle						recordH;
	MailDBRecordType 			record;
	MailPackedDBRecordPtr	r;

	
	recordH = DmQueryRecord (dbP, index);;
	r = MemHandleLock (recordH);
	MailUnpackRecord (r, &record);
	oldSize = PackedRecordSize (&record);

	ChangeRecordField (&record, recordField, data);

	newSize = PackedRecordSize (&record);

	// If the record the same size?
	if (oldSize == newSize)
		{
		PackRecord (&record, r);
		MemPtrUnlock (r);
		}
		
	// Is the record smaller?
	else if (oldSize > newSize)
		{
		PackRecord (&record, r);
		MemPtrUnlock (r);
		err = MemHandleResize (recordH, newSize);
		}

	// The record is larger.
	else
		{
		MemPtrUnlock (r);
		
		recordH = DmResizeRecord (dbP, index, newSize);
		if (! recordH) return (err);

		r = MemHandleLock (recordH);
		MailUnpackRecord (r, &record);
		ChangeRecordField (&record, recordField, data);
		PackRecord (&record, r);
		MemPtrUnlock (r);
		}

	return (err);
}
*/

/***********************************************************************
 *
 * FUNCTION:    MailInsertIntoField
 *
 * DESCRIPTION: This routine inserts the passed string into the 
 *              specified a field of the passed record.
 *
 * PARAMETERS:  dpP          - database pointer
 *					 index        - index of a packed database record
 *					 recordField  - fields to insert into
 *              insertData	  - string to insert
 *              insertLen    - length of insert string
 *              insertOffset - offset from start of field to insert at
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/17/96	Initial Revision
 *
 ***********************************************************************/
Err MailInsertIntoField (DmOpenRef dbP, UInt16 index, 
	MailRecordFieldType recordField, Char * insertData, UInt16 insertLen,
	UInt16 insertOffset)
{
	Err							err = 0;
	UInt16						newSize;
	UInt16						oldSize;
	UInt32						offset;
	UInt32						bytes;
	Char * 						ptr;
	UInt16 *						srcP=0, dstP=0;
	MemHandle						recordH;
	MailDBRecordType 			record;
	MailPackedDBRecordPtr	r;


	if (! insertLen) return (0);
	
	// Resize the record.
	recordH = DmQueryRecord (dbP, index);;
	oldSize = MemHandleSize (recordH);
	newSize = oldSize + insertLen;
	recordH = DmResizeRecord (dbP, index, newSize);
	if (recordH == NULL)
		return dmErrMemError;

	
	r = MemHandleLock (recordH);
	MailUnpackRecord (r, &record);

	switch (recordField)
		{
		case mailFieldSubject:
			ptr = record.subject;
			break;
			
		case mailFieldFrom:
			ptr = record.from;
			break;
			
		case mailFieldTo:
			ptr = record.to;
			break;
			
		case mailFieldCC:			
			ptr = record.cc;
			break;
			
		case mailFieldBCC:		
			ptr = record.bcc;
			break;
			
		case mailFieldReplyTo:		
			ptr = record.replyTo;
			break;
			
		case mailFieldSentTo:		
			ptr = record.sentTo;
			break;
			
		case mailFieldBody:				
			ptr = record.body;
			break;

		default:
			ErrDisplay ("Invalid record field");
		}


	// Move the rest of the record.
	ptr += insertOffset;
	offset = (ptr - (Char *)r) + insertLen;
	bytes = oldSize - (UInt32)(ptr - (Char *)r);
	if (bytes)
		DmWrite (r, offset, ptr, bytes);
			
	// Write the new field.
	offset = (ptr - (Char *)r);
	DmWrite (r, offset, insertData, insertLen);

	MemHandleUnlock (recordH);


	return (err);
}

/************************************************************
 *
 * FUNCTION:    MailSortRecord
 *
 * DESCRIPTION: Move the passed record to its correct sort
 *              position.
 *
 * PARAMETERS:  database pointer
 *				    record index
 *
 * RETURNS:     zero if successful, errorcode if not
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/18/96	Initial Revision
 *
 *************************************************************/
Err MailSortRecord (DmOpenRef dbP, UInt16 * indexP)
{
	Err err;
	Int16 sortOrder;
	UInt16 index;
	UInt16 attributes;
	UInt32 uniqueID;
	MemHandle recordH;
	Boolean dontMove;
	MailPackedDBRecordPtr cmp;
	MailPackedDBRecordPtr recordP;
	
	sortOrder = MailGetSortOrder (dbP);

	// Check if the record is already in the correct position.
	recordP = MemHandleLock (DmQueryRecord (dbP, *indexP));		
	if (*indexP > 0)
		{
		// This record wasn't deleted and deleted records are at the end of the
		// database so the prior record may not be deleted!
		cmp = MemHandleLock (DmQueryRecord (dbP, *indexP-1));		
		dontMove = (CompareRecords (cmp, recordP, sortOrder, NULL, NULL, 0) <= 0);
		MemPtrUnlock (cmp);
		}
	else 
		dontMove = true;

	if (dontMove && (*indexP+1 < DmNumRecords (dbP)))
		{
		DmRecordInfo(dbP, *indexP+1, &attributes, NULL, NULL);
		if ( ! (attributes & dmRecAttrDelete) )
			{
			cmp = MemHandleLock (DmQueryRecord (dbP, *indexP+1));
			dontMove &= (CompareRecords (recordP, cmp, sortOrder, NULL, NULL, 0) <= 0);
			MemPtrUnlock (cmp);
			}
		}
	MemPtrUnlock (recordP);

	if (dontMove) return (0);


	// Since the routine that determines the records sort position uses a 
	// binary search algorythm we need to remove the record from the database 
	// before we can determine its new position.  We will also save and restore the 
	// record's attributes and unique ID.
	DmRecordInfo (dbP, *indexP, &attributes, &uniqueID, NULL);

	err = DmDetachRecord (dbP, *indexP, &recordH);
	if (err) return (err);
	
	recordP = MemHandleLock (recordH);		
	index = DmFindSortPosition (dbP, recordP, NULL, (DmComparF *) CompareRecords, sortOrder);
	MemPtrUnlock (recordP);
	
	err = DmAttachRecord (dbP, &index, recordH, 0);
	if (err) return (err);

	DmSetRecordInfo (dbP, index, &attributes, &uniqueID);	

	*indexP = index;
	
	return (err);
}


/***********************************************************************
 *
 * FUNCTION:    MailChangeRecordField
 *
 * DESCRIPTION: This routine changes a field of the passed record.
 *
 * PARAMETERS:  dpP         - database pointer
 *					 hP          - pointer to the MemHandle of a packed database
 *                            record. A new MemHandle may be returned as
 *                            the result of resizing the record.
 *					 recordField - fields to chance
 *
 * RETURNED:    nothing
 *
 *	COMMENTS:   This routine does not move the modified record to to 
 *             correct sort position if one of the sort key field is
 *             modified.  This function is intented for use on 
 *             records that have not been attached to the database.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/17/96	Initial Revision
 *
 ***********************************************************************/
Err MailChangeRecordField (DmOpenRef dbP, UInt16 * indexP, 
	MailRecordFieldType recordField, void * data)
{
	Err							err = 0;
	UInt16						newSize;
	UInt16						curSize;
	UInt32						offset;
	UInt32						bytes;
	Char * 						ptr;
	Char *						fieldP;
	MemHandle					recordH;
	MailDBRecordType 			record;
	MailPackedDBRecordPtr	r;
	MailPackedDBRecordPtr 	nilP = 0;
	
	// Get the record which we are going to change
	recordH = DmQueryRecord (dbP, *indexP);;
	r = MemHandleLock (recordH);
	

	// We MemHandle the fixed size fields here.
	switch (recordField)
		{
		case mailFieldDate:
			DmWrite (r, (UInt32)&nilP->date, data, sizeof(r->date));
			goto sort;
			
		case mailFieldTime:
			DmWrite (r, (UInt32)&nilP->time, data, sizeof(r->time));
			goto sort;
			
		case mailFieldFlags:
			DmWrite (r, (UInt32)&nilP->flags, data, sizeof(r->flags));
			goto sort;
		}



	// Calculate the size of the changed record
	MailUnpackRecord (r, &record);
	fieldP = GetRecordField (&record, recordField);
	curSize = MemPtrSize (r);
	newSize = curSize + StrLen (data) - StrLen (fieldP);


	// If the new record is longer, expand the record.
	if (newSize > curSize)
		{
		MemPtrUnlock (r);

		recordH = DmResizeRecord (dbP, *indexP, newSize);
		if (! recordH) return (dmErrMemError);

		r = MemHandleLock (recordH);
		MailUnpackRecord (r, &record);
		}


	// Change the record field. First Move the rest of the record.
	fieldP = GetRecordField (&record, recordField);
	offset = (fieldP - (Char *)r) + StrLen (data) + 1;
	ptr = fieldP + StrLen (fieldP) + 1;
	bytes = curSize - (UInt32)(ptr - (Char *)r);
	if (bytes)
		DmWrite (r, offset, ptr, bytes);
			
	// Write the new field.
	offset = (fieldP - (Char *)r);
	DmStrCopy (r, offset, data);
	

	// If the new record is shorter, shrink the record.
	if (newSize < curSize)
		MemPtrResize (r, newSize);


sort:
	MemPtrUnlock (r);


	// If the sort position is changed move to the new position.
	// Check if any of the key fields have changed. 
	switch (recordField)
		{
		case mailFieldDate:
		case mailFieldTime:
		case mailFieldSubject:
		case mailFieldFrom:
			err = MailSortRecord (dbP, indexP);
		}


	return err;
}


/***********************************************************************
 *
 * FUNCTION:     MailGetDatabase
 *
 * DESCRIPTION:  Get the application's database.  Open the database if it
 * exists, create it if neccessary.
 *
 * PARAMETERS:   *dbPP - pointer to a database ref (DmOpenRef) to be set
 *					  mode - how to open the database (dmModeReadWrite)
 *
 * RETURNED:     Err - zero if no error, else the error
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			jmp		10/18/99	Initial Revision
 *
 ***********************************************************************/
Err MailGetDatabase (DmOpenRef *dbPP, UInt16 mode)
{
	Err error = errNone;
	DmOpenRef dbP;
	UInt16 cardNo;
	LocalID dbID;

	*dbPP = NULL;
  
  // Find the application's data file.  If it doesn't exist create it.
	dbP = DmOpenDatabaseByTypeCreator (mailDBType, sysFileCMail, mode);
	if (!dbP)
		{
		error = DmCreateDatabase (0, mailDBName, sysFileCMail, mailDBType, false);
		if (error)
			return error;
		
		dbP = DmOpenDatabaseByTypeCreator(mailDBType, sysFileCMail, mode);
		if (!dbP)
			return (1);

		// Set the backup bit.  This is to aid syncs with non-Palm software.
		SetDBBackupBit(dbP);
		
		error = MailAppInfoInit (dbP);
      if (error) 
      	{
			DmOpenDatabaseInfo(dbP, &dbID, NULL, NULL, &cardNo, NULL);
      	DmCloseDatabase(dbP);
      	DmDeleteDatabase(cardNo, dbID);
         return error;
         }
		}
	
	*dbPP = dbP;
	return 0;
}
