/******************************************************************************
 *
 * Copyright (c) 1995-2002 PalmSource, Inc. All rights reserved.
 *
 * File: DateDB.c
 *
 * Release: Palm OS 5 SDK (68K) R3.
 *
 * Description:
 *		Appointment Manager routines
 *
 *****************************************************************************/

#include <PalmOS.h>

// Set this to get to private database defines
#define __APPTMGR_PRIVATE__
#include "DateDB.h"


/***********************************************************************
 *
 *	Internal Structutes
 *
 ***********************************************************************/

// The following structure doesn't really exist.  The first field
// varies depending on the data present.  However, it is convient
// (and less error prone) to use when accessing the other information.

typedef struct {
	ApptDateTimeType 	when;
	ApptDBRecordFlags	flags;	// A flag set for each  datum present
	char				firstField;
	UInt8				reserved;
} ApptPackedDBRecordType;

typedef ApptPackedDBRecordType * ApptPackedDBRecordPtr;

RepeatInfoPtr	ApptGetRepeatInfo(ApptPackedDBRecordPtr src);

/***********************************************************************
 *
 *	Internal Routines
 *
 ***********************************************************************/

static Int16 TimeCompare (TimeType t1, TimeType t2);

static Int16 DateCompare (DateType d1, DateType d2);


/************************************************************
 *
 *  FUNCTION: ECApptDBValidate
 *
 *  DESCRIPTION: This routine validates the integrity of a
 *					  datebook datebase.
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS: nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	11/15/95	Initial Revision
 *
 *************************************************************/
#if	EMULATION_LEVEL != EMULATION_NONE

#define maxDescLen	tableMaxTextItemSize
#define maxNoteLen	noteViewMaxLength
#define maxLocationLen	noteViewMaxLength		//DOLATER: peter - choose a max length for location field

void ECApptDBValidate (DmOpenRef dbP)
{
	UInt16 i;
	UInt16 size;
	UInt16 len;
	UInt16 blockSize;
	UInt16 numRecord;
	MemHandle recH;
	DateType date;
	ApptDBRecordType rec;
	
	numRecord = DmNumRecords (dbP);
	for (i = 0 ; i < numRecord; i++)
		{
		recH = DmQueryRecord (dbP, i);
		if (! recH) continue;

		ApptGetRecord (dbP, i, &rec, &recH);
		
		// Is the event an untimed event?
		if (TimeToInt(rec.when->startTime) == apptNoTime)
			{
			// There should not be and end time if there is no start time.
			if (TimeToInt(rec.when->endTime) != apptNoTime)
				ErrDisplay ("DB integrity error");				
			}

		// Validate the event date.
		if ((rec.when->date.month == 0) ||
			 (rec.when->date.month > 12) ||
			 (rec.when->date.day == 0) ||
			 (rec.when->date.day > DaysInMonth (rec.when->date.month, 
			 	rec.when->date.year + firstYear)))
			ErrDisplay ("DB integrity error");				
			 

		// The start time may not be greater than the end time.
		else if (TimeCompare (rec.when->startTime, rec.when->endTime) > 0)
			{
			ErrDisplay ("DB integrity error");				
			}

		// Validate the alarm info.
		if (rec.alarm)
			{
			if (rec.alarm->advance > 99)
				ErrDisplay ("DB integrity error");				

			if (rec.alarm->advanceUnit > P1clDays)
				ErrDisplay ("DB integrity error");				
			}


		// Validate the repeat info.
		if (rec.repeat)
			{
			// Validate the repeat type.
			if (rec.repeat->repeatType > P1clrepeatYearly)
				ErrDisplay ("DB integrity error");				

			// Validate the repeat end date.
			date = rec.repeat->repeatEndDate;
			if (DateToInt (date) != apptNoEndDate)
				{
				if (DateCompare (date, rec.when->date) < 0)
					ErrDisplay ("DB integrity error");				
				
				if ((date.month == 0) ||
					 (date.month > 12) ||
					 (date.day == 0) ||
					 (date.day > DaysInMonth (date.month, date.year + firstYear)))
					ErrDisplay ("DB integrity error");
				}				
			
			// Validate the repeat frequency.
			if (rec.repeat->repeatFrequency > 99)
				ErrDisplay ("DB integrity error");				

			// Validate the "repeatOn" info
			if (rec.repeat->repeatType == P1clrepeatWeekly)
				{
				if (rec.repeat->repeatOn == 0)
					ErrDisplay ("DB integrity error");				
				}
			else if (rec.repeat->repeatType == P1clrepeatMonthlyByDay)
				{
				if (rec.repeat->repeatOn > domLastSat)
					ErrDisplay ("DB integrity error");				
				}
			else
				{
				if (rec.repeat->repeatOn != 0)
					ErrDisplay ("DB integrity error");				
				}

			// Validate the "repeatStartOfWeek" info,
			if (rec.repeat->repeatType == P1clrepeatWeekly)
				{
				if (rec.repeat->repeatStartOfWeek > monday)
					ErrDisplay ("DB integrity error");
				}
			else if (rec.repeat->repeatStartOfWeek)
				ErrDisplay ("DB integrity error");

			}

		// Validate the record size.
		size = sizeof (ApptDateTimeType) + sizeof (ApptDBRecordFlags);
		if (rec.alarm)
			size += sizeof (AlarmInfoType);
		if (rec.repeat)
			size += sizeof (RepeatInfoType);
		if (rec.exceptions)
			size += sizeof (DateType) * rec.exceptions->numExceptions +
				sizeof (UInt16);
		if (rec.description)
			{
			len = StrLen (rec.description);
			ErrFatalDisplayIf (len > maxDescLen, "DB integrity error");
			size += len + 1;
			}
		if (rec.note)
			{
			len = StrLen (rec.note);
			ErrFatalDisplayIf (len > maxNoteLen, "DB integrity error");
			size += len + 1;
			}
		if (rec.location)
			{
			len = StrLen (rec.location);
			ErrFatalDisplayIf (len > maxLocationLen, "DB integrity error");
			size += len + 1;
			}

		blockSize = MemHandleSize (recH);
//		ErrFatalDisplayIf ( (blockSize != size), "DB integrity error");

		MemHandleUnlock (recH);
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
 *			art	6/12/95		Initial Revision
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
 *  FUNCTION: ApptGetRepeatInfo
 *
 *  DESCRIPTION: Fills in the ApptDBRecord structure
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the record unpacked
 *
 *  CREATED: 4/28/03
 *
 *  BY: Peter Epstein
 *
 *************************************************************/
 RepeatInfoPtr ApptGetRepeatInfo(ApptPackedDBRecordPtr src)
{
	ApptDBRecordFlags	flags;
	char *p;

	
	flags = src->flags;
	p = &src->firstField;

	if (flags.alarm) 
		p += sizeof (AlarmInfoType);

	if (flags.repeat)
		return (RepeatInfoType *) p;
	else
		return NULL;
}


/************************************************************
 *
 *  FUNCTION:    ApptComparePackedRecords
 *
 *  DESCRIPTION: Compare two packed records.
 *
 *  PARAMETERS:  r1    - database record 1
 *				     r2    - database record 2
 *               extra - extra data, not used in the function
 *
 *  RETURNS:    -1 if record one is less
 *		           1 if record two is less
 *
 *  CREATED: 1/14/95 
 *
 *  BY: Roger Flores
 *
 *	COMMENTS:	Compare the two records key by key until
 *	there is a difference.  Return -1 if r1 is less or 1 if r2
 *	is less.  A zero is never returned because if two records
 *	seem identical then their unique IDs are compared!
 *
 *************************************************************/ 
static Int16 ApptComparePackedRecords (ApptPackedDBRecordPtr r1, 
	ApptPackedDBRecordPtr r2, Int16 extra, SortRecordInfoPtr info1, 
	SortRecordInfoPtr info2, MemHandle appInfoH)
{
#pragma unused (extra, info1, info2, appInfoH)

	Int16 result;

	if ((r1->flags.repeat) || (r2->flags.repeat))
		{
		if ((r1->flags.repeat) && (r2->flags.repeat))
			{
			// In the past, two repeating events were considered equal. Now we sort them
			// by their end date in order to more efficiently iterate over the repeating
			// events on a given date or date range. First step is to find the repeat
			// info in each of the records so we can compare their end dates.
			// No end date is represented as -1, which will sort last, as desired.
			result = DateCompare (ApptGetRepeatInfo(r1)->repeatEndDate, ApptGetRepeatInfo(r2)->repeatEndDate);
			if (result == 0)
				// Two events than end on the same date are sorted by their start time.
				// We don't in fact rely on this, but might in the future.
				result = TimeCompare (r1->when.startTime, r2->when.startTime);
			}
		else if (r1->flags.repeat)
			result = -1;
		else
			result = 1;
		}

	else
		{
		result = DateCompare (r1->when.date, r2->when.date);
		if (result == 0)
			result = TimeCompare (r1->when.startTime, r2->when.startTime);
		}
	return result;
}

/************************************************************
 *
 *  FUNCTION: ApptPackedSize
 *
 *  DESCRIPTION: Return the packed size of an ApptDBRecordType 
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the size in bytes
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static UInt16 ApptPackedSize (ApptDBRecordPtr r)
{
	UInt16 size, index;
	
	size = sizeof (ApptDateTimeType) + sizeof (ApptDBRecordFlags);
	
	if (r->alarm != NULL)
		size += sizeof (AlarmInfoType);
	
	if (r->repeat != NULL)
		size += sizeof (RepeatInfoType);
	
	if (r->exceptions != NULL)
		size += sizeof (UInt16) + 
			(r->exceptions->numExceptions * sizeof (DateType));
	
	if (r->description != NULL)
		size += StrLen(r->description) + 1;
	
	if (r->note != NULL)
		size += StrLen(r->note) + 1;
	
	if (r->location != NULL)
		size += StrLen(r->location) + 1;
	
	if (StrLen(r->timeZone.name) != 0)
	{
		size += sizeof(UInt32);										// Room for blob id.
		size += sizeof(UInt16);										// Room for blob size.
		size += sizeof(r->timeZone.uTC);							// Room for actual blob contents.
		size += sizeof(r->timeZone.dSTStart);					// More actual blob contents.
		size += sizeof(r->timeZone.dSTEnd);						// More actual blob contents.
		size += sizeof(r->timeZone.dSTAdjustmentInMinutes);// More actual blob contents.
		size += sizeof(r->timeZone.country);					// More actual blob contents.
		size += sizeof(UInt8);										// More actual blob contents (custom flag).
		ErrNonFatalDisplayIf(StrLen(r->timeZone.name) > apptMaxTimeZoneNameLen - 1, "Time zone name too long");
		size += StrLen(r->timeZone.name) + 1;					// More actual blob contents.
	}

	if (r->meetingInfo.numAttendees != 0 || r->meetingInfo.apptStatus)
	{
		size += sizeof(UInt32);										// Room for blob id.
		size += sizeof(UInt16);										// Room for blob size.
		size += sizeof(r->meetingInfo.meetingStatus);		// Room for actual blob contents.
		size += sizeof(r->meetingInfo.apptStatus);			// More actual blob contents.
		size += sizeof(r->meetingInfo.numAttendees);			// More actual blob contents.
		for (index = 0; index < r->meetingInfo.numAttendees; index++)
		{
			size += sizeof(r->meetingInfo.attendees[index].role);			// More actual blob contents.
			size += StrLen(r->meetingInfo.attendees[index].name) + 1;	// More actual blob contents.
			size += StrLen(r->meetingInfo.attendees[index].email) + 1;	// More actual blob contents.
		}
	}

/*	if (r->serverID != NULL && StrLen(r->serverID) != 0)
	{
		size += sizeof(UInt32);										// Room for blob id.
		size += sizeof(UInt16);										// Room for blob size.
		size += StrLen(r->serverID) + 1;							// Room for actual blob contents.
	}*/
	
	for (index = 0; index < r->numBlobs; index++)
	{
		size += sizeof(r->blobs[index].creatorID);
		size += sizeof(r->blobs[index].size);
		size += r->blobs[index].size;
	}

	return size;
}


/************************************************************
 *
 *  FUNCTION: ApptPack
 *
 *  DESCRIPTION: Pack an ApptDBRecordType
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the ApptPackedDBRecord is packed
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static void ApptPack(ApptDBRecordPtr s, ApptPackedDBRecordPtr d)
{
	ApptDBRecordFlags	flags;
	UInt16 	size;
	ApptDateTimeType when;
	UInt16	len;
	UInt16	index;
	UInt32	offset = 0;
	UInt32	offsetForSize;
	UInt32	blobId;
	UInt16	blobSize;
	UInt16	blobCount = 0;
	*(UInt8 *)&flags = 0;			// clear the flags
	
	// copy the ApptDateTimeType
	//c = (char *) d;
	offset = 0;

	when = *s->when;
	if (when.endTime.hours == hoursPerDay)
		when.endTime.hours = 0;						// Never store hours = 24 in database.
	DmWrite(d, offset, &when, sizeof(ApptDateTimeType));
	offset += sizeof (ApptDateTimeType) + sizeof (ApptDBRecordFlags);
	
	if (s->alarm != NULL)
		{
		DmWrite(d, offset, s->alarm, sizeof(AlarmInfoType));
		offset += sizeof (AlarmInfoType);
		flags.alarm = 1;
		}
	
	if (s->repeat != NULL)
		{
		DmWrite(d, offset, s->repeat, sizeof(RepeatInfoType));
		offset += sizeof (RepeatInfoType);
		flags.repeat = 1;
		}
	
	if (s->exceptions != NULL)
		{
		size = sizeof (UInt16) + 
			(s->exceptions->numExceptions * sizeof (DateType));
		DmWrite(d, offset, s->exceptions, size);
		offset += size;
		flags.exceptions = 1;
		}
	
	if (s->description != NULL)
		{
		size = StrLen(s->description) + 1;
		DmWrite(d, offset, s->description, size);
		offset += size;
		flags.description = 1;
		}
	
	if (s->note != NULL)
		{
		size = StrLen(s->note) + 1;
		DmWrite(d, offset, s->note, size);
		offset += size;
		flags.note = 1;
		}
	
	if (s->location != NULL)
		{
		size = StrLen(s->location) + 1;
		DmWrite(d, offset, s->location, size);
		offset += size;
		flags.location = 1;
		}
	
	DmWrite(d, sizeof(ApptDateTimeType), &flags, sizeof(flags));
	
	// Include a time zone blob if needed.
	if (s->timeZone.name[0] != chrNull)
	{
		blobId = dateTimeZoneBlobId;
		
		// Write the 4 byte blob ID.
		len = sizeof(blobId);	
		DmWrite(d, offset, &blobId, len);
		offset += len;
		
		// Don't write the size yet, but remember where it goes.
		offsetForSize = offset;
		offset += sizeof(blobSize);
		
		// Write the blob content.
		DmWrite(d, offset, &s->timeZone.uTC, sizeof(s->timeZone.uTC));
		offset += sizeof(s->timeZone.uTC);
		DmWrite(d, offset, &s->timeZone.dSTStart, sizeof(s->timeZone.dSTStart));
		offset += sizeof(s->timeZone.dSTStart);
		DmWrite(d, offset, &s->timeZone.dSTEnd, sizeof(s->timeZone.dSTEnd));
		offset += sizeof(s->timeZone.dSTEnd);
		DmWrite(d, offset, &s->timeZone.dSTAdjustmentInMinutes, sizeof(s->timeZone.dSTAdjustmentInMinutes));
		offset += sizeof(s->timeZone.dSTAdjustmentInMinutes);
		DmWrite(d, offset, &s->timeZone.country, sizeof(s->timeZone.country));
		offset += sizeof(s->timeZone.country);
		DmWrite(d, offset, (UInt8 *)&s->timeZone.name - 1 /*custom flag*/, sizeof(UInt8));
		offset += sizeof(UInt8); /*custom flag*/
		ErrNonFatalDisplayIf(StrLen(s->timeZone.name) > apptMaxTimeZoneNameLen - 1, "Time zone name too long");
		DmWrite(d, offset, s->timeZone.name, StrLen(s->timeZone.name) + 1);
		offset += StrLen(s->timeZone.name) + 1;
		//HostTraceOutputTL (appErrorClass, "PacK wasSetByUser %d uTC %d ",s->timeZone.wasSetByUser, s->timeZone.uTC);	
		// Now go back and fill in the blob size.
		blobSize = offset - offsetForSize - sizeof(blobSize);
		DmWrite(d, offsetForSize, &blobSize, sizeof(blobSize));
		//HostTraceOutputTL (appErrorClass, "Appt Pack --> ");	
		blobCount++;
	}
	
	// Include a meeting blob if needed, have any attendees or set the appt status to something other than default
	if (s->meetingInfo.numAttendees != 0 || s->meetingInfo.apptStatus)
	{
		blobId = dateMeetingBlobId;
		
		// Write the 4 byte blob ID.
		len = sizeof(blobId);	
		DmWrite(d, offset, &blobId, len);
		offset += len;
		
		// Don't write the size yet, but remember where it goes.
		offsetForSize = offset;
		offset += sizeof(blobSize);
		
		// Write the blob content.
		DmWrite(d, offset, &s->meetingInfo.meetingStatus, sizeof(s->meetingInfo.meetingStatus));
		offset += sizeof(s->meetingInfo.meetingStatus);
		DmWrite(d, offset, &s->meetingInfo.apptStatus, sizeof(s->meetingInfo.apptStatus));
		offset += sizeof(s->meetingInfo.apptStatus);
		DmWrite(d, offset, &s->meetingInfo.numAttendees, sizeof(s->meetingInfo.numAttendees));
		offset += sizeof(s->meetingInfo.numAttendees);
		for (index = 0; index < s->meetingInfo.numAttendees; index++)
		{
			DmWrite(d, offset, &s->meetingInfo.attendees[index].role, sizeof(s->meetingInfo.attendees[index].role));
			offset += sizeof(s->meetingInfo.attendees[index].role);
			DmWrite(d, offset, s->meetingInfo.attendees[index].name, StrLen(s->meetingInfo.attendees[index].name) + 1);
			offset += StrLen(s->meetingInfo.attendees[index].name) + 1;
			DmWrite(d, offset, s->meetingInfo.attendees[index].email, StrLen(s->meetingInfo.attendees[index].email) + 1);
			offset += StrLen(s->meetingInfo.attendees[index].email) + 1;
		}
		
		// Now go back and fill in the blob size.
		blobSize = offset - offsetForSize - sizeof(blobSize);
		DmWrite(d, offsetForSize, &blobSize, sizeof(blobSize));
		
		blobCount++;
	}
	
/*	// Include a server ID blob if needed.
	if (s->serverID != NULL && StrLen(s->serverID) != 0)
	{
		blobId = dateServerIDBlobId;
		
		// Write the 4 byte blob ID.
		len = sizeof(blobId);	
		DmWrite(d, offset, &blobId, len);
		offset += len;
		
		// Write the 2 byte blob size.
		len = sizeof(blobSize);
		blobSize = StrLen(s->serverID) + 1;			// Blob size excludes space used by blob ID and size.
		DmWrite(d, offset, &blobSize, len);
		offset += len;
		
		// Write the blob content.
		DmWrite(d, offset, s->serverID, StrLen(s->serverID) + 1);
		offset += StrLen(s->serverID) + 1;
		
		blobCount++;
	}
*/	
	// Include any other blobs we don't understand.
	ErrNonFatalDisplayIf(blobCount + s->numBlobs > apptMaxBlobs, "Too many blobs");
	for (index = 0; index < s->numBlobs; index++)
	{
//		size = sizeof(s->blobs[index]->creatorID) + sizeof(s->blobs[index]->size) + s->blobs[index]->size;
		size = sizeof(s->blobs[index].creatorID);
		DmWrite(d, offset, &(s->blobs[index].creatorID), size);
		offset += size;
		size = sizeof(s->blobs[index].size);
		DmWrite(d, offset, &(s->blobs[index].size), size);
		offset += size;
		DmWrite(d, offset, s->blobs[index].content, s->blobs[index].size);
		offset += s->blobs[index].size;
	}

	ErrNonFatalDisplayIf(offset > MemHandleSize(MemPtrRecoverHandle(d)), "Not enough room for packed record");
	ErrNonFatalDisplayIf(offset < MemHandleSize(MemPtrRecoverHandle(d)), "Too much room for packed record");
}


/************************************************************
 *
 *  FUNCTION: ApptUnpack
 *
 *  DESCRIPTION: Fills in the ApptDBRecord structure
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the record unpacked
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static void ApptUnpack(ApptPackedDBRecordPtr src, ApptDBRecordPtr dest)
{
	ApptDBRecordFlags	flags;
	Char *p;
	Char *blobStart;
	Char *blobEnd;
	UInt16 recordSize;
	UInt32 blobId;
	UInt16 blobSize;
	UInt16 index;
	flags = src->flags;
	p = &src->firstField;

	recordSize = MemPtrSize(src);
	dest->when = (ApptDateTimeType *) src;
	ErrNonFatalDisplayIf(dest->when->endTime.hours == hoursPerDay, "Hours = 24 found in database");
	
	if (flags.alarm) 
		{
		dest->alarm = (AlarmInfoType *) p;
		p += sizeof (AlarmInfoType);
		}
	else
		dest->alarm = NULL;

	
	if (flags.repeat)
		{
		dest->repeat = (RepeatInfoType *) p;
		p += sizeof (RepeatInfoType);
		}
	else
		dest->repeat = NULL;
	
	
	if (flags.exceptions)
		{
		dest->exceptions = (ExceptionsListType *) p;
		p += sizeof (UInt16) + 
			(((ExceptionsListType *) p)->numExceptions * sizeof (DateType));
		}
	else
		dest->exceptions = NULL;
		
	
	if (flags.description)
		{
		dest->description = p;
		p += StrLen(p) + 1;
		}
	else
		dest->description = NULL;
		
	
	if (flags.note)
		{
		dest->note = p;
		p += StrLen(p) + 1;
		}
	else
		dest->note = NULL;
		
	
	if (flags.location)
		{
		dest->location = p;
		p += StrLen(p) + 1;
		}
	else
		dest->location = NULL;

	// There may also be blob data on the end of the record.
	// First set everything as if there were no blobs.
	dest->timeZone.name[0] = chrNull;			// This indicates that there is no time zone info for this meeting.
	dest->meetingInfo.meetingStatus = unansweredMeeting;
	dest->meetingInfo.apptStatus = showAsBusy;
	dest->meetingInfo.numAttendees = 0;			// This indicates that it is not a meeting.
//	dest->serverID = NULL;							// This indicates that there is no known server ID.
	dest->numBlobs = 0;								// Start by assuming no blobs we don't understand.
	
	
	// Then iterate through the blobs, ignoring any we don't understand.
	blobStart = p;			// First blob starts where last non-blob data ends.
	while (blobStart < (Char *)src + recordSize)
	{
		p = blobStart;
		// If a field is using edit in place to directly edit a database record at the time this routine
		// is called, or if the device was reset while an edit in place was in progress, the record can
		// be left with junk data on the end. FldCompactText would clean up this junk, but it either wasn't
		// called or simply hasn't yet been called. We will attempt to parse this junk data as blobs, but
		// more than likely these blobs will appear to be invalid. On release builds we want to recover
		// gracefully from this situation by ignoring the junk data.
		if ((Char *)src + recordSize - blobStart <= sizeof (blobId) + sizeof (blobSize))
		{
			ErrNonFatalDisplay("Blob goes beyond end of record - don't let fields edit records directly!");
			return;
		}
		MemMove(&blobId, p, sizeof (blobId));
		p += sizeof (blobId);
		MemMove(&blobSize, p, sizeof (blobSize));		
		p += sizeof (blobSize);
		blobEnd = p + blobSize;			// Blob size excludes space to store ID and size of blob.
		if (blobEnd > (Char *)src + recordSize)
		{
			ErrNonFatalDisplay("Blob goes beyond end of record - don't let fields edit records directly!");
			return;
		}
		switch (blobId)
		{
			case dateTimeZoneBlobId:
				ErrNonFatalDisplayIf(dest->timeZone.name[0] != chrNull, "Duplicate time zone blob");
				MemMove(&dest->timeZone.uTC, p, sizeof (dest->timeZone.uTC));
				p += sizeof (dest->timeZone.uTC);
				MemMove(&dest->timeZone.dSTStart, p, sizeof (dest->timeZone.dSTStart));
				p += sizeof (dest->timeZone.dSTStart);
				MemMove(&dest->timeZone.dSTEnd, p, sizeof (dest->timeZone.dSTEnd));
				p += sizeof (dest->timeZone.dSTEnd);
				MemMove(&dest->timeZone.dSTAdjustmentInMinutes, p, sizeof (dest->timeZone.dSTAdjustmentInMinutes));
				p += sizeof (dest->timeZone.dSTAdjustmentInMinutes);
				MemMove(&dest->timeZone.country, p, sizeof (dest->timeZone.country));
				p += sizeof (dest->timeZone.country);
				MemMove((Char *)&dest->timeZone.name - 1 /*custom flag*/, p, sizeof (UInt8));
				p += sizeof (UInt8);
				ErrNonFatalDisplayIf(StrLen(p) > apptMaxTimeZoneNameLen - 1, "Time zone name too long");
				StrCopy(dest->timeZone.name, p);
				p += StrLen(dest->timeZone.name) + 1;
				//HostTraceOutputTL (appErrorClass, "UnPack wasSetByUser %d uTC %d ",dest->timeZone.wasSetByUser, dest->timeZone.uTC);	
				break;

			case dateMeetingBlobId:
				ErrNonFatalDisplayIf(dest->meetingInfo.numAttendees != 0, "Duplicate meeting blob");
				MemMove(&dest->meetingInfo.meetingStatus, p, sizeof (dest->meetingInfo.meetingStatus));
				p += sizeof (dest->meetingInfo.meetingStatus);
				MemMove(&dest->meetingInfo.apptStatus, p, sizeof (dest->meetingInfo.apptStatus));
				p += sizeof (dest->meetingInfo.apptStatus);
				MemMove(&dest->meetingInfo.numAttendees, p, sizeof (dest->meetingInfo.numAttendees));
				p += sizeof (dest->meetingInfo.numAttendees);
				for (index = 0; index < dest->meetingInfo.numAttendees; index++)
				{
					MemMove(&dest->meetingInfo.attendees[index].role, p, sizeof (dest->meetingInfo.attendees[index].role));
					p += sizeof (dest->meetingInfo.attendees[index].role);
					dest->meetingInfo.attendees[index].name = p;
					p += StrLen(dest->meetingInfo.attendees[index].name) + 1;
					dest->meetingInfo.attendees[index].email = p;
					p += StrLen(dest->meetingInfo.attendees[index].email) + 1;
					dest->meetingInfo.attendees[index].AttendeePos = index;
				}
				break;
/*			
			case dateServerIDBlobId:
				dest->serverID = p;
				p += StrLen(dest->serverID);
				break;
*/			
			default:
			{
				ErrNonFatalDisplayIf (dest->numBlobs >= apptMaxBlobs, "Too many blobs");

				dest->blobs[dest->numBlobs].creatorID = blobId;
				dest->blobs[dest->numBlobs].size = blobSize;
				dest->blobs[dest->numBlobs].content = p;				
	
				dest->numBlobs++;
				p = blobEnd;
				//ErrNonFatalDisplayIf(dest->numBlobs >= apptMaxBlobs, "Too many blobs");
				//MemMove(&dest->blobs[dest->numBlobs], p, sizeof (dest->blobs[dest->numBlobs]));
				break;
			}	
		}
		ErrNonFatalDisplayIf(p != blobEnd, "Blob size does not agree with contents");
		blobStart = blobEnd;	// Next blob starts where last blob ends.
	}
	ErrNonFatalDisplayIf(blobStart != (Char *)src + recordSize,
		"Last blob not aligned with end of record - don't let fields edit records directly!");
	ErrNonFatalDisplayIf(ApptPackedSize(dest) != recordSize, "Blob size mismatch");	
}


/************************************************************
 *
 *  FUNCTION: ApptFindSortPosition
 *
 *  DESCRIPTION: Return where a record is or should be
 *		Useful to find or find where to insert a record.
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: position where a record should be
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static UInt16 ApptFindSortPosition(DmOpenRef dbP, ApptPackedDBRecordPtr newRecord)
{
	return (DmFindSortPosition (dbP, newRecord, NULL, (DmComparF *)ApptComparePackedRecords, 0));
}


/************************************************************
 *
 *  FUNCTION: ApptGetRecord
 *
 *  DESCRIPTION: Get a read-only record from the Appointment Database
 *
 *  PARAMETERS: database pointer
 *				    database index
 *				    database record
 *
 *  RETURNS: ##0 if successful, errorcode if not
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
Err ApptGetRecord (DmOpenRef dbP, UInt16 index, ApptDBRecordPtr r, 
	MemHandle * handleP)
{
	MemHandle					handle = NULL;
	ApptPackedDBRecordPtr 		src;
	Err							RetErrCode = errNone;

	handle = DmQueryRecord(dbP, index);
	ErrFatalDisplayIf(((RetErrCode = DmGetLastErr()) != errNone), "Error Querying record");
	if(handle != NULL && RetErrCode == errNone)
	{
		src = (ApptPackedDBRecordPtr) MemHandleLock (handle);

		if (DmGetLastErr())
		{
			*handleP = 0;
			return DmGetLastErr();
		}
		
		ApptUnpack(src, r);
		
		*handleP = handle;
	}	
	else
		*handleP = NULL;
	return RetErrCode;

}


/************************************************************
 *
 *  FUNCTION: ApptChangeRecord
 *
 *  DESCRIPTION: Change a record in the Appointment Database
 *
 *  PARAMETERS: database pointer
 *					 database index
 *					 database record
 *					 changed fields
 *
 *  RETURNS: ##0 if successful, errorcode if not
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
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
Err ApptChangeRecord(DmOpenRef dbP, UInt16 *index, ApptDBRecordPtr r, 
	ApptDBRecordChangeFlags changedFields)
{
	Err result;
	Int16 newIndex;
	UInt16 attributes;
	UInt16 attendeeIndex;
	Boolean dontMove;
	MemHandle oldH;
	MemHandle srcH;
	MemHandle dstH;
	ApptDBRecordType src;
	ApptPackedDBRecordPtr dst;
	ApptPackedDBRecordPtr cmp;
	UInt32					BlobSize = 0;  		//Size of the blob if present.
	MemHandle				hSrcRawRecord;		//Src Record Handle required to get the size of record.
	UInt32					SrcApptRecordSize = 0;//Size of the Src record without Blobs
	UInt32					NewSrcApptRecordSize = 0;//Size of the New record without Blobs
	UInt16					Counter;
	
	// We do not assume that r is completely valid so we get a valid
	// ApptDBRecordPtr...
	if ((result = ApptGetRecord(dbP, *index, &src, &srcH)) != 0)
		return result;

	ErrNonFatalDisplayIf(ApptPackedSize(&src) != MemHandleSize(srcH), "Size mismatch");
			
	//get handle to packed record, To check the existance of the Bolb
	hSrcRawRecord = DmQueryRecord(dbP, *index);
	BlobSize = MemHandleSize(hSrcRawRecord) - SrcApptRecordSize;	
	
	// and we apply the changes to it.
	if (changedFields.when)
		src.when = r->when;
	
	if (changedFields.alarm) 
		src.alarm = r->alarm;
	
	if (changedFields.repeat)
		src.repeat = r->repeat;
	
	if (changedFields.exceptions)
		src.exceptions = r->exceptions;
		
	if (changedFields.description)
		src.description = r->description;
		
	if (changedFields.note)
		src.note = r->note;

	if (changedFields.location)
		src.location = r->location;

	if (changedFields.timeZone)
	{
		MemMove(&src.timeZone, &r->timeZone, sizeof(r->timeZone));
	}
	
	if (changedFields.meeting)
	{
		src.meetingInfo.meetingStatus = r->meetingInfo.meetingStatus;
		src.meetingInfo.apptStatus = r->meetingInfo.apptStatus;
		src.meetingInfo.numAttendees = r->meetingInfo.numAttendees;
		for (attendeeIndex = 0; attendeeIndex < r->meetingInfo.numAttendees; attendeeIndex++)
		{
			src.meetingInfo.attendees[attendeeIndex].role = r->meetingInfo.attendees[attendeeIndex].role;
			src.meetingInfo.attendees[attendeeIndex].name = r->meetingInfo.attendees[attendeeIndex].name;
			src.meetingInfo.attendees[attendeeIndex].email = r->meetingInfo.attendees[attendeeIndex].email;
		}
	}
	
	if(changedFields.blobs)
	{
		for(Counter = 0;Counter< apptMaxBlobs ;Counter++)
		{
			src.blobs[Counter] = r->blobs[Counter]; 
		} 
		src.numBlobs = r->numBlobs;
	}
	// Allocate a new chunk with the correct size and pack the data from 
	// the unpacked record into it.
	dstH = DmNewHandle(dbP, (UInt32) ApptPackedSize(&src));
	if (dstH)
	{
		dst = MemHandleLock (dstH);
		ApptPack (&src, dst);
	}
	MemHandleUnlock (srcH);
	if (dstH == NULL)
		return dmErrMemError;

	// If the sort position is changed move to the new position.
	// In the past repeating events weren't maintained in sorted order, so
	// we could avoid comparing the record with it's neighbors in some cases.
	// Now we always check.
	//
	//		if ((!changedFields.when) && (! changedFields.repeat))
	//			goto attachRecord;				// repeating events aren't in sorted order

	// Make sure *index-1 < *index < *index+1, if so it's in sorted 
	// order.  Leave it there.	
	if (*index > 0)
		{
		// This record wasn't deleted and deleted records are at the end of the
		// database so the prior record may not be deleted!
		cmp = MemHandleLock (DmQueryRecord(dbP, *index-1));		
		dontMove = (ApptComparePackedRecords (cmp, dst, 0, NULL, NULL, 0) <= 0);
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
			dontMove = dontMove && ApptComparePackedRecords (dst, cmp, 0, NULL, NULL, 0) <= 0;
			MemPtrUnlock (cmp);
			}
		}

	if (dontMove)
		goto attachRecord;
	
	// The record isn't in the right position.  Move it.
	newIndex = ApptFindSortPosition (dbP, dst);
	DmMoveRecord (dbP, *index, newIndex);
	if (newIndex > *index) newIndex--;
	*index = newIndex;						// return new position

attachRecord:
	// Attach the new record to the old index,  the preserves the 
	// category and record id.
	result = DmAttachRecord (dbP, index, dstH, &oldH);
	
	MemPtrUnlock(dst);

	if (result) return result;

	MemHandleFree(oldH);

	#if	EMULATION_LEVEL != EMULATION_NONE
		ECApptDBValidate (dbP);
	#endif

	return 0;
}


/************************************************************
 *
 *  FUNCTION: ApptNewRecord
 *
 *  DESCRIPTION: Create a new packed record in sorted position
 *
 *  PARAMETERS: database pointer
 *				database record
 *
 *  RETURNS: ##0 if successful, error code if not
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
Err	ApptNewRecord (DmOpenRef dbP, ApptDBRecordPtr r, UInt16 category, Boolean secret, UInt16 *index)
{
	MemHandle recordH;
	ApptPackedDBRecordPtr recordP;
	UInt16 					newIndex;
	Err err;
	UInt16 attr;

	
	// Make a new chunk with the correct size.
	recordH = DmNewHandle (dbP, (UInt32) ApptPackedSize(r));
	if (recordH == NULL)
		return dmErrMemError;

	recordP = MemHandleLock (recordH);
	
	// Copy the data from the unpacked record to the packed one.
	ApptPack (r, recordP);

	newIndex = ApptFindSortPosition(dbP, recordP);

	MemPtrUnlock (recordP);

	// 4) attach in place
	err = DmAttachRecord(dbP, &newIndex, recordH, 0);
	if (err) 
		MemHandleFree(recordH);
	else
		{
		*index = newIndex;

		// Set the category and secret flag.
		DmRecordInfo (dbP, newIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;
		attr |= category;
		if (secret)
			attr |= dmRecAttrSecret;
		else
			attr &= ~dmRecAttrSecret;
		DmSetRecordInfo (dbP, newIndex, &attr, NULL);
		}
	
	return err;
}