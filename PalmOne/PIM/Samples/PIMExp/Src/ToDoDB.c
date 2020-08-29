#define __TODOMGR_PRIVATE__

#include <PalmOS.h>
#include "ToDoDB.h"

#define TODO_SORT_OTHER_MAKE( sort, filter, subfilter )	\
	((((sort) & 0x0F) << 8) |	\
	 (((filter) & 0x0F) << 4) |	\
	 (((subfilter) & 0x0F))		\
	)
#define TODO_SORT_OTHER_GET_SORT_ORDER( other )	(((other) >> 8) & 0x0F)
#define TODO_SORT_OTHER_GET_FILTER( other )		(((other) >> 4) & 0x0F)
#define TODO_SORT_OTHER_GET_SUBFILTER( other )	(((other) & 0x0F))

void ECToDoDBValidate(DmOpenRef dbP);
static UInt16 ToDoSize( ToDoDBRecordPtr r );
	
static Char *ToDoDBRecordGetFieldPointer( ToDoDBRecordPtr recordP,
		ToDoRecordFieldType field );
static Err ToDoDBRecordSetOptionalField( DmOpenRef dbP, UInt16 index,
		ToDoRecordFieldType field, void *fieldDataP, UInt32 fieldDataSize );
static Err ToDoDBRecordClearOptionalField( DmOpenRef dbP, UInt16 index,
		ToDoRecordFieldType field, UInt32 fieldDataSize );

#define maxDescLen	256
#define maxNoteLen	maxFieldTextLen             //was 4096, now is 32,767

void ECToDoDBValidate (DmOpenRef dbP)
{
	UInt16 i;
	UInt16 size;
//	UInt16 cLen;
	UInt16 recSize;
	UInt16 descLen;
	UInt16 noteLen;
	UInt16 numRecord;
	UInt16 priority;
//	Char * c;
//	Char * note;
	MemHandle recH;
	ToDoDBRecordPtr rec;
	
	numRecord = DmNumRecords (dbP);
	for (i = 0 ; i < numRecord; i++)
		{
		recH = DmQueryRecord (dbP, i);
		if (! recH) continue;

		rec = MemHandleLock (recH);
		
//		priority = rec->priority & priorityOnly;
		priority = rec->priority;
		ErrFatalDisplayIf (priority > toDoMaxPriority,  "DB integrity error");
		
		descLen = StrLen (GetToDoDescriptionPtr(rec));
		ErrFatalDisplayIf (descLen > maxDescLen, "DB integrity error");

//		note = &rec->description + descLen + 1;
		noteLen = StrLen (GetToDoNotePtr(rec));
		ErrFatalDisplayIf (noteLen > maxNoteLen, "DB integrity error");
		
		// Validate the record size.
//		size = sizeof (DateType) + 1;
//		c = &rec->description;
//		cLen = StrLen(c) + 1;
//		size += cLen;
//		c += cLen;
//		size += StrLen(c) + 1;
		size = ToDoSize(rec);
		recSize = MemPtrSize (rec);
	//	ErrFatalDisplayIf ( (recSize != size), "DB integrity error");

		MemPtrUnlock (rec);
		}
}
		
/************************************************************
 *
 *  FUNCTION: DateTypeCmp
 *
 *  DESCRIPTION: Compare two dates
 *
 *  PARAMETERS: 
 *
 *  RETURNS: 
 *
 *  CREATED: 1/20/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static Int16 DateTypeCmp(DateType d1, DateType d2)
{
	Int16 result;
	
	result = d1.year - d2.year;
	if (result != 0)
		{
		if (DateToInt(d1) == 0xffff)
			return 1;
		if (DateToInt(d2) == 0xffff)
			return -1;
		return result;
		}
	
	result = d1.month - d2.month;
	if (result != 0)
		return result;
	
	result = d1.day - d2.day;

	return result;

}


/************************************************************
 *
 *  FUNCTION: CategoryCompare
 *
 *  DESCRIPTION: Compare two categories
 *
 *  PARAMETERS:  attr1, attr2 - record attributes, which contain 
 *						  the category. 
 *					  appInfoH - MemHandle of the applications category info
 *
 *  RETURNS: 0 if they match, non-zero if not
 *				 + if s1 > s2
 *				 - if s1 < s2
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/5/96	Initial Revision
 *
 *************************************************************/
static Int16 CategoryCompare (UInt8 attr1, UInt8 attr2, MemHandle appInfoH)
{
	Int16 result;
	UInt8 category1;
	UInt8 category2;
	ToDoAppInfoPtr appInfoP;
	
	
	category1 = attr1 & dmRecAttrCategoryMask;
	category2 = attr2 & dmRecAttrCategoryMask;

	result = category1 - category2;
	if (result != 0)
		{
		if (category1 == dmUnfiledCategory)
			return (1);
		else if (category2 == dmUnfiledCategory)
			return (-1);
		
		appInfoP = MemHandleLock (appInfoH);
		result = StrCompare (appInfoP->categoryLabels[category1],
									appInfoP->categoryLabels[category2]);

		MemPtrUnlock (appInfoP);
		return result;
		}
	
	return result;
}


/************************************************************
 *
 *  FUNCTION: ToDoCompareRecords
 *
 *  DESCRIPTION: Compare two records.
 *
 *  PARAMETERS: database record 1
 *				database record 2
 *
 *  RETURNS: -n if record one is less (n != 0)
 *			  n if record two is less
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
 *************************************************************/
 
static Int16 ToDoCompareRecords( ToDoDBRecordPtr r1,  ToDoDBRecordPtr r2,
	Int16 sortOther, SortRecordInfoPtr info1, SortRecordInfoPtr info2,
	MemHandle appInfoH )
{
	DateType	*dateP;
	DateType	r1DueDate, r2DueDate;
	Int16		sortOrder, filter, subFilter;
	Int16		result;

	sortOrder	= TODO_SORT_OTHER_GET_SORT_ORDER( sortOther );
	filter		= TODO_SORT_OTHER_GET_FILTER( sortOther );
	subFilter	= TODO_SORT_OTHER_GET_SUBFILTER( sortOther );

#if 1
	dateP = ( DatePtr ) ToDoDBRecordGetFieldPointer( r1, toDoRecordFieldDueDate );
	if ( dateP )
		{
		r1DueDate = *dateP;
		}
	else
		{
		dateP = ( DatePtr ) ToDoDBRecordGetFieldPointer( r1,
			toDoRecordFieldCompletionDate );
		if ( dateP )
			{
			r1DueDate = *dateP;
			}
		else
			{
			DateToInt( r1DueDate ) = toDoNoDueDate;
			}
		}

	dateP = ( DatePtr ) ToDoDBRecordGetFieldPointer( r2, toDoRecordFieldDueDate );
	if ( dateP )
		{
		r2DueDate = *dateP;
		}
	else
		{
		dateP = ( DatePtr ) ToDoDBRecordGetFieldPointer( r2,
			toDoRecordFieldCompletionDate );
		if ( dateP )
			{
			r2DueDate = *dateP;
			}
		else
			{
			DateToInt( r2DueDate ) = toDoNoDueDate;
			}
		}
#else
	DateSecondsToDate( TimGetSeconds(), &today );

	if ( errNone != ToDoDBRecordGetDueDate( r1, filter, subFilter,
			today, &r1DueDate ) )
		{
		DateToInt( r1DueDate ) = toDoNoDueDate;
		}

	if ( errNone != ToDoDBRecordGetDisplayedDueDate( r2, filter, subFilter,
			today, &r2DueDate ) )
		{
		DateToInt( r2DueDate ) = toDoNoDueDate;
		}
#endif

	// Sort by priority, due date, and category.
	if (sortOrder == soPriorityDueDate)
		{
		result = (r1->priority) - (r2->priority);
		if (result == 0)
			{
			result = DateTypeCmp( r1DueDate, r2DueDate );
			if (result == 0)
				{
				result = CategoryCompare (info1->attributes, info2->attributes, appInfoH);
				}
			}
		}

	// Sort by due date, priority, and category.
	else if (sortOrder == soDueDatePriority)
		{
		result = DateTypeCmp( r1DueDate, r2DueDate );
		if (result == 0)
			{
			result = (r1->priority) - (r2->priority);
			if (result == 0)
				{
				result = CategoryCompare (info1->attributes, info2->attributes, appInfoH);
				}
			}
		}
	
	// Sort by category, priority, due date
	else if (sortOrder == soCategoryPriority)
		{
		result = CategoryCompare (info1->attributes, info2->attributes, appInfoH);
		if (result == 0)
			{
			result = (r1->priority) - (r2->priority);
			if (result == 0)
				{
				result = DateTypeCmp( r1DueDate, r2DueDate );
				}
			}
		}

	// Sort by category, due date, priority
	else if (sortOrder == soCategoryDueDate)
		{
		result = CategoryCompare (info1->attributes, info2->attributes, appInfoH);
		if (result == 0)
			{
			result = DateTypeCmp( r1DueDate, r2DueDate );
			if (result == 0)
				{
				result = (r1->priority) - (r2->priority);
				}
			}
		}
	
	return result;
}

/************************************************************
 *
 *  FUNCTION: ToDoGetSortOrder
 *
 *  DESCRIPTION: This routine gets the sort order value from the 
 *               'to do' application info block.
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS:    true - if the 'to do' record are sorted by priority, 
 *              false - if the records are sorted by due date.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	5/12/95	Initial Revision
 *       art	3/22/96	Rename routine and added more sort orders
 *
 *************************************************************/
UInt8 ToDoGetSortOrder (DmOpenRef dbP)
{
	UInt8 sortOrder;
	ToDoAppInfoPtr appInfoP;
			
	appInfoP = MemHandleLock (ToDoGetAppInfo (dbP));
	sortOrder = appInfoP->sortOrder;
	MemPtrUnlock (appInfoP);	

	return (sortOrder);
}

/************************************************************
 *
 *  FUNCTION: ToDoFindSortPosition
 *
 *  DESCRIPTION: Return where a record is or should be.
 *		Useful to find a record or find where to insert a record.
 *
 *  PARAMETERS: database record (not deleted!)
 *
 *  RETURNS: the size in bytes
 *
 *  CREATED: 1/11/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static UInt16 ToDoFindSortPosition(DmOpenRef dbP, ToDoDBRecord *newRecord, 
		SortRecordInfoPtr newRecordInfo, UInt16 filter, UInt16 subFilter )
{
	Int16	sortOther;
	UInt8	sortOrder;
	
	sortOrder = ToDoGetSortOrder (dbP);
	
	sortOther = TODO_SORT_OTHER_MAKE( sortOrder, filter, subFilter );
		
	return (DmFindSortPosition (dbP, newRecord, newRecordInfo, 
		(DmComparF *)ToDoCompareRecords, sortOther));
}

/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordSetDueDate
 *
 * DESCRIPTION:	Set or add due date on the specified record
 *
 * PARAMETERS:	dbP -			database pointer
 *				index -			record index
 *				dueDate -		due date
 *
 * RETURNED:	errNone if successful, error code otherwise
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		03/13/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordSetDueDate( DmOpenRef dbP, UInt16 index, DateType *dueDateP )
{
	return ToDoDBRecordSetOptionalField( dbP, index, toDoRecordFieldDueDate,
		dueDateP, sizeof( DateType ) );
}

/************************************************************
 *
 *  FUNCTION: ToDoGetAppInfo
 *
 *  DESCRIPTION: Get the app info chunk 
 *
 *  PARAMETERS: database pointer
 *
 *  RETURNS: MemHandle to the to do application info block (ToDoAppInfoType)
 *
 *  CREATED: 5/12/95 
 *
 *  BY: Art Lamb
 *
 *************************************************************/
MemHandle ToDoGetAppInfo (DmOpenRef dbP)
{
	Err error;
	UInt16 cardNo;
	LocalID dbID;
	LocalID appInfoID;
	
	error = DmOpenDatabaseInfo (dbP, &dbID, NULL, NULL, &cardNo, NULL);
	ErrFatalDisplayIf (error,  "Get getting to do app info block");

	error = DmDatabaseInfo (cardNo, dbID, NULL, NULL, NULL, NULL, NULL, 
			NULL, NULL, &appInfoID, NULL, NULL, NULL);
	ErrFatalDisplayIf (error,  "Get getting to do app info block");

	return ((MemHandle) MemLocalIDToGlobal (appInfoID, cardNo));
}

/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordClearDueDate
 *
 * DESCRIPTION:	Clear due date on the specified record
 *
 * PARAMETERS:	dbP -			database pointer
 *				index -			record index
 *				dueDate -		due date
 *
 * RETURNED:	errNone if successful, error code otherwise
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		03/13/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordClearDueDate( DmOpenRef dbP, UInt16 index )
{
	return ToDoDBRecordClearOptionalField( dbP, index, toDoRecordFieldDueDate,
		sizeof( DateType ) );
}


/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordGetCompletionDate
 *
 * DESCRIPTION:	Retrieve the completion date (if any) on the specified record
 *
 * PARAMETERS:	dbP -					database pointer
 *				index -					record index
 *				hasCompletionDateP -	whether record has a completion date
 *				completionDateP -		completion date, if any
 *
 * RETURNED:	errNone if successful, error code otherwise
 *				any of the result pointers may be NULL
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		03/13/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordGetCompletionDate( DmOpenRef dbP, UInt16 index,
		Boolean *hasCompletionDateP, DateType *completionDateP )
{
	MemHandle	recordH;
	Err			ret;

	recordH = DmQueryRecord( dbP, index );
	if ( recordH )
	{
		ToDoDBRecordPtr	recordP;
		Char			*compDateP;
		
		recordP = ( ToDoDBRecordPtr ) MemHandleLock( recordH );
		
		compDateP = ToDoDBRecordGetFieldPointer( recordP,
			toDoRecordFieldCompletionDate );
		
		if ( compDateP )
		{
			if ( hasCompletionDateP )
			{
				*hasCompletionDateP = true;
			}
			
			if ( completionDateP )
			{
				*completionDateP = *( DateType * ) compDateP;
			}
		}
		else
		{
			if ( hasCompletionDateP )
			{
				*hasCompletionDateP = false;
			}
		}
		
		MemHandleUnlock( recordH );
		
		ret = errNone;
	}
	else
	{
		ret = dmErrNotValidRecord;
	}

	return ret;
}


/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordSetCompletionDate
 *
 * DESCRIPTION:	Set or add completion date on the specified record
 *
 * PARAMETERS:	dbP -				database pointer
 *				index -				record index
 *				completionDate -	completion date
 *
 * RETURNED:	errNone if successful, error code otherwise
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		03/13/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordSetCompletionDate( DmOpenRef dbP, UInt16 index,
		DateType *completionDateP )
{
	return ToDoDBRecordSetOptionalField( dbP, index,
		toDoRecordFieldCompletionDate, completionDateP, sizeof( DateType ) );
}


/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordClearCompletionDate
 *
 * DESCRIPTION:	Clear completion date on the specified record
 *
 * PARAMETERS:	dbP -				database pointer
 *				index -				record index
 *				completionDate -	due date
 *
 * RETURNED:	errNone if successful, error code otherwise
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		03/13/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordClearCompletionDate( DmOpenRef dbP, UInt16 index )
{
	return ToDoDBRecordClearOptionalField( dbP, index,
		toDoRecordFieldCompletionDate, sizeof( DateType ) );
}

static Err ToDoDBRecordSetOptionalField( DmOpenRef dbP, UInt16 index,
		ToDoRecordFieldType field, void *fieldDataP, UInt32 fieldDataSize )
{
	MemHandle	recordH;
	Err			ret;

	recordH = DmQueryRecord( dbP, index );
	if ( recordH )
	{
		ToDoDBRecordPtr	recordP;
		Char			*src, *dest;
		UInt32			offset, numBytes;
		
		recordP = ( ToDoDBRecordPtr ) MemHandleLock( recordH );
		
		// if the record doesn't already have the field, make room first
		if ( ((toDoRecordFieldDueDate == field) &&
			  !recordP->dataFlags.dueDate) ||
			 ((toDoRecordFieldCompletionDate == field) &&
			  !recordP->dataFlags.completionDate) ||
			 ((toDoRecordFieldAlarm == field) &&
			  !recordP->dataFlags.alarm) ||
			 ((toDoRecordFieldRepeat == field) &&
			  !recordP->dataFlags.repeat) )
		{
			MemHandleUnlock( recordH );
			
			numBytes = MemHandleSize( recordH ) + fieldDataSize;

			ret = MemHandleResize( recordH, numBytes );
			
			if ( errNone == ret )
			{
				ToDoDBDataFlags	newFlags;

				recordP = ( ToDoDBRecordPtr ) MemHandleLock( recordH );
				
				// set new field's flag
				newFlags = recordP->dataFlags;

				if ( toDoRecordFieldDueDate == field )
				{
					newFlags.dueDate = 1;
				}
				else if ( toDoRecordFieldCompletionDate == field )
				{
					newFlags.completionDate = 1;
				}
				else if ( toDoRecordFieldAlarm == field )
				{
					newFlags.alarm = 1;
				}
				else if ( toDoRecordFieldRepeat == field )
				{
					newFlags.repeat = 1;
				}

				DmWrite( recordP, OffsetOf( ToDoDBRecord, dataFlags ),
					&newFlags, sizeof( ToDoDBDataFlags ) );
				
				// move the rest of the data down
				
				// it will move FROM where the field will now be
				src = ToDoDBRecordGetFieldPointer( recordP, field );

				// it will move TO just after the new field
				dest = src + fieldDataSize;

				// how many bytes to move?
				// numBytes is new size of record.
				// to calculate the number of bytes, we must get back to
				// the old size of the record
				numBytes -= fieldDataSize;
				// and then we must subtract off the part that won't move
				numBytes -= (src - (( Char * ) recordP ));

				offset = dest - (( Char * ) recordP );
				DmWrite( recordP, offset, src, numBytes );
			}
		}
		else
		{
			ret = errNone;
		}
		
		if ( errNone == ret )
		{
			dest = ToDoDBRecordGetFieldPointer( recordP, field );
			
			// write new field data into record
			offset = dest - (( Char * ) recordP);
			DmWrite( recordP, offset, fieldDataP, fieldDataSize );
		}
		
		MemHandleUnlock( recordH );
	}
	else
	{
		ret = dmErrNotValidRecord;
	}

#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
	ECToDoDBValidate( dbP );
#endif

	return ret;
}


static Err ToDoDBRecordClearOptionalField( DmOpenRef dbP, UInt16 index,
		ToDoRecordFieldType field, UInt32 fieldDataSize )
{
	MemHandle	recordH;
	Err			ret;

	recordH = DmQueryRecord( dbP, index );
	if ( recordH )
	{
		ToDoDBRecordPtr	recordP;
		
		recordP = ( ToDoDBRecordPtr ) MemHandleLock( recordH );

		// if the record has the field, clear the data
		if ( ((toDoRecordFieldDueDate == field) &&
			  recordP->dataFlags.dueDate) ||
			 ((toDoRecordFieldCompletionDate == field) &&
			  recordP->dataFlags.completionDate) ||
			 ((toDoRecordFieldAlarm == field) &&
			  recordP->dataFlags.alarm) ||
			 ((toDoRecordFieldRepeat == field) &&
			  recordP->dataFlags.repeat) )
		{
			ToDoDBDataFlags	newFlags;
			Char			*src, *dest;
			UInt32			offset, numBytes;
			
			dest = ToDoDBRecordGetFieldPointer( recordP, field );
			
			src = dest;
			src += fieldDataSize;
			
			offset = dest - (( Char * ) recordP);
			numBytes = MemHandleSize( recordH ) - offset -
				fieldDataSize;

			DmWrite( recordP, offset, src, numBytes );
			
			numBytes += offset;

			// clear flags after data manipulation, because
			// GetFieldPointer relies on field flag to be set
			newFlags = recordP->dataFlags;

			if ( toDoRecordFieldDueDate == field )
			{
				newFlags.dueDate = 0;
			}
			else if ( toDoRecordFieldCompletionDate == field )
			{
				newFlags.completionDate = 0;
			}
			else if ( toDoRecordFieldAlarm == field )
			{
				newFlags.alarm = 0;
			}
			else if ( toDoRecordFieldRepeat == field )
			{
				newFlags.repeat = 0;
			}

			DmWrite( recordP, OffsetOf( ToDoDBRecord, dataFlags ),
				&newFlags, sizeof( ToDoDBDataFlags ) );

			MemHandleUnlock( recordH );
			
			ret = MemHandleResize( recordH, numBytes );
			ErrFatalDisplayIf( (errNone != ret), "Error resizing record" );
		}
		else
		{
			MemHandleUnlock( recordH );
		}
		
		ret = errNone;
	}
	else
	{
		ret = dmErrNotValidRecord;
	}

#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
	ECToDoDBValidate( dbP );
#endif

	return ret;
}

/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordGetAlarmInfo
 *
 * DESCRIPTION:	Retrieve the alarm info (if any) on the specified record
 *
 * PARAMETERS:	dbP -			database pointer
 *				index -			record index
 *				hasAlarmP -		whether record has an alarm
 *				dueDateP -		record's due date
 *				alarmTimeP -	time of record's alarm
 *				advanceDaysP -	number of days in advance record's alarm goes
 *
 * RETURNED:	errNone if successful, error code otherwise
 *				any of the result pointers may be NULL
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		02/06/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordGetAlarmInfo( DmOpenRef dbP, UInt16 index, Boolean *hasAlarmP,
		DateType *dueDateP, ToDoAlarmInfoType *alarmInfoP )
{
	MemHandle		recordH;
	Err				ret;

	recordH = DmQueryRecord( dbP, index );
	if ( recordH )
	{
		ToDoDBRecordPtr	recordP;
		Char			*dataP;

		recordP = ( ToDoDBRecordPtr ) MemHandleLock( recordH );

		dataP = ToDoDBRecordGetFieldPointer( recordP,
			toDoRecordFieldDueDate );
		
		if ( dataP )
		{
			if ( dueDateP )
			{
				*dueDateP = *( DateType * ) dataP;
			}
		}
		
		dataP = ToDoDBRecordGetFieldPointer( recordP,
			toDoRecordFieldAlarm );
		
		if ( dataP )
		{
			if ( hasAlarmP )
			{
				*hasAlarmP = true;
			}
			
			if ( alarmInfoP )
			{
				alarmInfoP->alarmTime =
					(( ToDoAlarmInfoPtr ) dataP)->alarmTime;
				alarmInfoP->alarmAdvanceDays =
					(( ToDoAlarmInfoPtr ) dataP)->alarmAdvanceDays;
			}
		}
		else
		{
			if ( hasAlarmP )
			{
				*hasAlarmP = false;
			}
		}

		MemHandleUnlock( recordH );

		ret = errNone;
	}
	else
	{
		ret = dmErrNotValidRecord;
	}

	return ret;
}


/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordSetAlarmInfo
 *
 * DESCRIPTION:	Set or add alarm info on the specified record
 *
 * PARAMETERS:	dbP -			database pointer
 *				index -			record index
 *				alarmTimeP -	time of new alarm
 *				advanceDaysP -	offset days of new alarm
 *
 * RETURNED:	errNone if successful, error code otherwise
 *				neither of the data pointers may be NULL
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		02/06/03	Initial Revision
 *
 *****************************************************************************/
Err ToDoDBRecordSetAlarmInfo( DmOpenRef dbP, UInt16 index,
		ToDoAlarmInfoType *alarmInfoP )
{
	return ToDoDBRecordSetOptionalField( dbP, index, toDoRecordFieldAlarm,
		alarmInfoP, sizeof( ToDoAlarmInfoType ) );
}

/***********************************************************************
 *
 * FUNCTION:    GetToDoDescriptionPtr
 *
 * DESCRIPTION: This routine returns a pointer to the description field in a to 
 *              do record.
 *
 * PARAMETERS:  recordP - pointer to a ToDo record
 *
 * RETURNED:    pointer to a null-terminated description
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		01/15/03	Initial Revision
 *
 ***********************************************************************/
Char* GetToDoDescriptionPtr (ToDoDBRecordPtr recordP)
{
	return ToDoDBRecordGetFieldPointer( recordP, toDoRecordFieldDescription );
}


/***********************************************************************
 *
 * FUNCTION:    GetToDoNotePtr
 *
 * DESCRIPTION: This routine returns a pointer to the note field in a to 
 *              do record.
 *
 * PARAMETERS:  recordP - pointer to a ToDo record
 *
 * RETURNED:    pointer to a null-terminated note
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	art		4/15/95		Initial Revision
 *	CS2		01/23/03	Revised for new structs
 *
 ***********************************************************************/
Char* GetToDoNotePtr (ToDoDBRecordPtr recordP)
{
	return ToDoDBRecordGetFieldPointer( recordP, toDoRecordFieldNote );
}


/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordGetFieldPointer
 *
 * DESCRIPTION:	Get a pointer to the data for a specified field
 *
 * PARAMETERS:	recordP -		record pointer
 *				field -			which field
 *
 * RETURNED:	pointer to data if present, NULL otherwise
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		02/06/03	Initial Revision
 *
 *****************************************************************************/
static Char *ToDoDBRecordGetFieldPointer( ToDoDBRecordPtr recordP,
		ToDoRecordFieldType field )
{
	Char	*ret;

	if ( toDoRecordFieldDataFlags == field )
	{
		return ( Char * ) &recordP->dataFlags;
	}
	
	if ( toDoRecordFieldRecordFlags == field )
	{
		return ( Char * ) &recordP->recordFlags;
	}

	if ( toDoRecordFieldPriority == field )
	{
		return ( Char * ) &recordP->priority;
	}
	
	ret = ( Char * ) &recordP->optionalData;
	
	if ( toDoRecordFieldDueDate == field )
	{
		if ( recordP->dataFlags.dueDate )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}
	
	if ( recordP->dataFlags.dueDate )
	{
		ret += sizeof( DateType );
	}
	
	if ( toDoRecordFieldCompletionDate == field )
	{
		if ( recordP->dataFlags.completionDate )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	if ( recordP->dataFlags.completionDate )
	{
		ret += sizeof( DateType );
	}

	if ( toDoRecordFieldAlarm == field )
	{
		if ( recordP->dataFlags.alarm )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	if ( recordP->dataFlags.alarm )
	{
		ret += sizeof( ToDoAlarmInfoType );
	}

	if ( toDoRecordFieldRepeat == field )
	{
		if ( recordP->dataFlags.repeat )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	if ( recordP->dataFlags.repeat )
	{
		ret += sizeof( ToDoRepeatInfoType );
	}

	if ( toDoRecordFieldDescription == field )
	{
		return ret;
	}

	ret += (StrLen( ret ) + 1);
	
	if ( toDoRecordFieldNote == field )
	{
		return ret;
	}
	
	ret += (StrLen( ret ) + 1);
	
	if ( toDoRecordFieldEndOfRecord == field )
	{
		return ret;
	}

	return NULL;
}

/******************************************************************************
 *
 * FUNCTION:    ToDoSize
 *
 * DESCRIPTION: Return the size of the data referred to by a ToDoDBRecordType
 *
 * PARAMETERS:  r - pointer to database record
 *                		
 * RETURNED:    the size in bytes
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	Roger	01/10/95	Initial Revision
 *	CS2		01/15/03	Revised for Mullet database structure
 *
 *****************************************************************************/
static UInt16 ToDoSize( ToDoDBRecordPtr r )
{
	UInt16 size, dataSize;
	char *c;
	int cLen;

	size = sizeof( ToDoDBDataFlags ) +	// dataFlags
		sizeof( UInt16 ) +					// recordFlags
		sizeof( UInt16 );					// priority

	c = ( char * ) &r->optionalData;

	if ( r->dataFlags.dueDate )
	{
		dataSize = sizeof( DateType );
		size += dataSize;
		c += dataSize;
	}
	
	if ( r->dataFlags.completionDate )
	{
		dataSize = sizeof( DateType );
		size += dataSize;
		c += dataSize;
	}

	if ( r->dataFlags.alarm )
	{
		dataSize = sizeof( ToDoAlarmInfoType );
		size += dataSize;
		c += dataSize;
	}
	
	if ( r->dataFlags.repeat )
	{
		dataSize = sizeof( ToDoRepeatInfoType );
		size += dataSize;
		c += dataSize;
	}
	
	cLen = StrLen( c ) + 1;
	size += cLen;
	c += cLen;

	cLen = StrLen( c ) + 1;
	size += cLen;

	return size;
}

/******************************************************************************
 *
 * FUNCTION:    ToDoNewRecord
 *
 * DESCRIPTION: Create a new record in sorted position
 *
 * PARAMETERS:  dbP - database pointer
 *				item - item pointer
 *				category - new record's category
 *				index - on return, new record's index
 *                		
 * RETURNED:    errNone if successful, error code if not
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	Roger	01/10/95	Initial Revision
 *	CS2		01/15/03	Revised for Mullet database structure
 *
 *****************************************************************************/
Err ToDoNewRecord( DmOpenRef dbP, ToDoItemPtr item, UInt16 category,
		UInt16 filter, UInt16 subFilter, UInt16 *index )
{
	Err 				err;
	UInt16				size, dataSize;
	UInt16				zero = 0;
	UInt16				attr;
	UInt16 				newIndex;
	UInt32				offset;
	ToDoDBRecordPtr		nilP = 0;
	ToDoDBRecordPtr		recordP;
	MemHandle 			recordH;
	SortRecordInfoType	sortInfo;

	// Compute the size of the new to do record.
	size = sizeDBRecord;

	if ( item->dataFlags.dueDate )
	{
		size += sizeof( DateType );
	}
	
	if ( item->dataFlags.completionDate )
	{
		size += sizeof( DateType );
	}

	if ( item->dataFlags.alarm )
	{
		size += sizeof( ToDoAlarmInfoType );
	}
	
	if ( item->dataFlags.repeat )
	{
		size += sizeof( ToDoRepeatInfoType );
	}

	if ( item->dataFlags.description )
	{
		size += StrLen( item->descriptionP );
	}

	if ( item->dataFlags.note )
	{
		size += StrLen( item->noteP );
	}

	//  Allocate a chunk in the database for the new record.
	recordH = ( MemHandle ) DmNewHandle( dbP, size );
	if ( recordH == NULL )
	{
		return dmErrMemError;
	}

	// Pack the the data into the new record.
	recordP = MemHandleLock( recordH );
	
	DmWrite( recordP, OffsetOf( ToDoDBRecord, dataFlags ),
		&item->dataFlags, sizeof( ToDoDBDataFlags ) );

	DmWrite( recordP, OffsetOf( ToDoDBRecord, recordFlags ),
		&item->recordFlags, sizeof( UInt16 ) );

	DmWrite( recordP, OffsetOf( ToDoDBRecord, priority ),
		&item->priority, sizeof( UInt16 ) );
	
	offset = OffsetOf( ToDoDBRecord, optionalData );

	if ( item->dataFlags.dueDate )
	{
		dataSize = sizeof( DateType );
		DmWrite( recordP, offset, &item->dueDate, dataSize );
		offset += dataSize;
	}
	
	if ( item->dataFlags.completionDate )
	{
		dataSize = sizeof( DateType );
		DmWrite( recordP, offset, &item->completionDate, dataSize );
		offset += dataSize;
	}

	if ( item->dataFlags.alarm )
	{
		dataSize = sizeof( ToDoAlarmInfoType );
		DmWrite( recordP, offset, &item->alarmInfo, dataSize );
		offset += dataSize;
	}

	if ( item->dataFlags.repeat )
	{
		dataSize = sizeof( ToDoRepeatInfoType );
		DmWrite( recordP, offset, &item->repeatInfo, dataSize );
		offset += dataSize;
	}

	if ( item->dataFlags.description )
	{
		DmStrCopy( recordP, offset, item->descriptionP );
		offset += StrLen( item->descriptionP ) + 1;
	}
	else
	{
		DmWrite( recordP, offset, &zero, 1 );
		offset += 1;
	}

	if ( item->dataFlags.note )
	{
		DmStrCopy( recordP, offset, item->noteP );
	}
	else
	{
		DmWrite( recordP, offset, &zero, 1 );
	}

	sortInfo.attributes = category;
	sortInfo.uniqueID[0] = 0;
	sortInfo.uniqueID[1] = 0;
	sortInfo.uniqueID[2] = 0;
	
	// Determine the sort position of the new record.
	newIndex = ToDoFindSortPosition (dbP, recordP, &sortInfo, filter,
		subFilter);

	MemPtrUnlock (recordP);

	// Insert the record.
	err = DmAttachRecord(dbP, &newIndex, recordH, 0);
	if (err) 
		MemHandleFree(recordH);
	else
		{
		*index = newIndex;

		// Set the category.
		DmRecordInfo (dbP, newIndex, &attr, NULL, NULL);
		attr &= ~dmRecAttrCategoryMask;
		attr |= category;
		DmSetRecordInfo (dbP, newIndex, &attr, NULL);
		}
		
	return err;
}


/******************************************************************************
 *
 * FUNCTION:    ToDoChangeRecord
 *
 * DESCRIPTION: Change a record in the ToDo database.
 *
 * PARAMETERS:  dbP - database pointer
 *				index - database index
 *				changedField - field to change
 *				data - pointer to new field data
 *                		
 * RETURNED:    errNone if successful, error code if not
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	Roger	01/14/95	Initial Revision
 *	CS2		01/15/03	Revised for Mullet database structure
 *
 *	COMMENTS:	Records are not stored with extra padding - they
 *	are always resized to their exact storage space.  This avoids
 *	a database compression issue.  The code works as follows:
 *****************************************************************************/
Err ToDoChangeRecord(DmOpenRef dbP, UInt16 *index, UInt16 filter,
	UInt16 subFilter, ToDoRecordFieldType changedField, const void * data)
{
	Char * 				c;
	MemHandle 			recordH = 0;
	ToDoDBRecordPtr 	src;
	UInt32				offset;
	ToDoDBDataFlags		newFlags;
	Err					err;
	Int16 				cLen;
	UInt16				attr;
	UInt16 				curSize;
	UInt16 				newSize;
	UInt16				descriptionOffset;
	UInt16 				newIndex;
	UInt16				priority;
	UInt16				recordFlags;

	// Get the record which we are going to change
	recordH = DmQueryRecord(dbP, *index);
	src = MemHandleLock (recordH);

	newFlags = src->dataFlags;
	
	// If the record is being changed such that its sort position will 
	// change, move the record to its new sort position.
	if ( (changedField == toDoRecordFieldCategory) ||
		 (changedField == toDoRecordFieldPriority) || 
		 (changedField == toDoRecordFieldDueDate)  ||
		 (changedField == toDoRecordFieldCompletionDate) )
	{
		SortRecordInfoType	sortInfo;
		MemHandle			tempRecH = 0;
		ToDoDBRecordPtr		tempRecP;
		UInt32				tempRecSize;
		DateType			srcDueDate, srcCompletionDate;
		Boolean				newHasDueDate, newHasCompletionDate;

		MemSet( &sortInfo, sizeof( sortInfo ), 0 );
		DmRecordInfo( dbP, *index, &attr, NULL, NULL );
		sortInfo.attributes = attr;

		// we don't bother adding alarm info to the temporary new record,
		// since it doesn't matter to sorting.
		if ( toDoRecordFieldDueDate == changedField )
		{
			newHasDueDate = (toDoNoDueDate != *( UInt16 * ) data);
		}
		else
		{
			newHasDueDate = src->dataFlags.dueDate;
		}

		if ( toDoRecordFieldCompletionDate == changedField )
		{
			newHasCompletionDate = (toDoNoCompletionDate != *( UInt16 * ) data);
		}
		else
		{
			newHasCompletionDate = src->dataFlags.completionDate;
		}

		tempRecSize = sizeDBRecord;
		if ( newHasDueDate )
		{
			tempRecSize += sizeof( DateType );
		}
		if ( newHasCompletionDate )
		{
			tempRecSize += sizeof( DateType );
		}
		if ( src->dataFlags.repeat )
		{
			tempRecSize += sizeof( ToDoRepeatInfoType );
		}
		
		tempRecH = MemHandleNew( tempRecSize );
		if ( !tempRecH )
		{
			goto exit;
		}
		
		tempRecP = ( ToDoDBRecordPtr ) MemHandleLock( tempRecH );
		MemSet( tempRecP, tempRecSize, 0 );

		// set data flags before the direct data manipulation below
		tempRecP->dataFlags.dueDate			= newHasDueDate;
		tempRecP->dataFlags.completionDate	= newHasCompletionDate;
		tempRecP->dataFlags.repeat			= src->dataFlags.repeat;

		if ( tempRecP->dataFlags.repeat && src->dataFlags.repeat )
		{
			MemMove( ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldRepeat ),
				ToDoDBRecordGetFieldPointer( src,
					toDoRecordFieldRepeat ),
				sizeof( ToDoRepeatInfoType ) );
		}

		if ( src->dataFlags.dueDate )
		{
			srcDueDate = *( DateType * ) ToDoDBRecordGetFieldPointer( src,
				toDoRecordFieldDueDate );
		}
		else
		{
			DateToInt( srcDueDate ) = toDoNoDueDate;
		}

		if ( src->dataFlags.completionDate )
		{
			srcCompletionDate = *( DateType * ) ToDoDBRecordGetFieldPointer(
				src, toDoRecordFieldCompletionDate );
		}
		else
		{
			DateToInt( srcCompletionDate ) = toDoNoCompletionDate;
		}

		if ( changedField == toDoRecordFieldCategory )
		{
			tempRecP->priority	= src->priority;
			if ( newHasDueDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldDueDate ) = srcDueDate;
			}
			if ( newHasCompletionDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldCompletionDate ) = srcCompletionDate;
			}
			sortInfo.attributes = *( UInt16 * ) data;			
		}
		else if ( changedField == toDoRecordFieldPriority )
		{
			tempRecP->priority	= *( UInt16 * ) data;
			if ( newHasDueDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldDueDate ) = srcDueDate;
			}
			if ( newHasCompletionDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldCompletionDate ) = srcCompletionDate;
			}
			sortInfo.attributes = attr;
		}
		else if ( changedField == toDoRecordFieldDueDate ) 
		{
			tempRecP->priority	= src->priority;
			if ( newHasDueDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldDueDate ) = *(( DatePtr ) data);
			}
			if ( newHasCompletionDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldCompletionDate ) = srcCompletionDate;
			}
			sortInfo.attributes = attr;
		}
		else if ( changedField == toDoRecordFieldCompletionDate )
		{
			tempRecP->priority	= src->priority;
			if ( newHasDueDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldDueDate ) = srcDueDate;
			}
			if ( newHasCompletionDate )
			{
				*( DateType * ) ToDoDBRecordGetFieldPointer( tempRecP,
					toDoRecordFieldCompletionDate ) = *(( DatePtr ) data);
			}
			sortInfo.attributes = attr;
		}

		newIndex = ToDoFindSortPosition( dbP, tempRecP, &sortInfo, filter,
			subFilter );
		DmMoveRecord( dbP, *index, newIndex );

		if ( newIndex > *index )
		{
			newIndex--;
		}

		*index = newIndex;
		
		MemHandleUnlock( tempRecH );
		MemHandleFree( tempRecH );
	}

	if ( changedField == toDoRecordFieldCategory )
	{
		attr = (attr & ~dmRecAttrCategoryMask) | *( UInt16 * ) data;

		DmSetRecordInfo( dbP, newIndex, &attr, NULL );

		goto exit;
	}

	if ( changedField == toDoRecordFieldPriority )
	{
		priority = *( UInt16 * ) data;

		DmWrite( src, OffsetOf( ToDoDBRecord, priority ),
			&priority, sizeof( UInt16 ) );

		goto exit;
	}

	if ( changedField == toDoRecordFieldComplete )
	{
		recordFlags = src->recordFlags;

		if ( *( UInt16 * ) data )
		{
			recordFlags |= TODO_RECORD_FLAG_COMPLETE;
		}
		else
		{
			recordFlags &= ~TODO_RECORD_FLAG_COMPLETE;
		}

		DmWrite( src, OffsetOf( ToDoDBRecord, recordFlags ),
			&recordFlags, sizeof( UInt16 ) );

		goto exit;
	}

	if ( toDoRecordFieldDueDate == changedField )
	{
		MemPtrUnlock( src );

		if ( toDoNoDueDate == *( UInt16 * ) data )
		{
			ToDoDBRecordClearDueDate( dbP, *index );
		}
		else
		{
			ToDoDBRecordSetDueDate( dbP, *index,
				( DateType * ) data );
		}

		goto exitNoUnlock;
	}
	
	if ( toDoRecordFieldCompletionDate == changedField )
	{
		MemPtrUnlock( src );

		if ( toDoNoCompletionDate == *( UInt16 * ) data )
		{
			ToDoDBRecordClearCompletionDate( dbP, *index );
		}
		else
		{
			ToDoDBRecordSetCompletionDate( dbP, *index,
				( DateType * ) data );
		}
		
		goto exitNoUnlock;
	}

	if ( toDoRecordFieldAlarm == changedField )
	{
		MemPtrUnlock( src );
		
		ToDoDBRecordSetAlarmInfo( dbP, *index,
			( ToDoAlarmInfoType * ) data );
		
		goto exitNoUnlock;
	}

	if ( toDoRecordFieldRepeat == changedField )
	{
//		MemPtrUnlock( src );
//		
//		ToDoDBRecordSetRepeatInfo( dbP, *index,
//			( ToDoRepeatInfoType * ) data );
//		
//		goto exitNoUnlock;

		goto exit;
	}

	// Calculate the size of the changed record. First,
	// find the size of the data used from the old record.
	newSize = sizeof( ToDoDBDataFlags ) +	// dataFlags
		sizeof( UInt16 ) +					// recordFlags
		sizeof( UInt16 );					// priority

	offset = OffsetOf( ToDoDBRecord, optionalData );
	c = ( char * ) &src->optionalData;

	if ( src->dataFlags.dueDate )
	{
		newSize += sizeof( DateType );
	}
	
	if ( src->dataFlags.completionDate )
	{
		newSize += sizeof( DateType );
	}

	if ( src->dataFlags.alarm )
	{
		newSize += sizeof( ToDoAlarmInfoType );
	}

	if ( src->dataFlags.repeat )
	{
		newSize += sizeof( ToDoRepeatInfoType );
	}

	descriptionOffset = newSize;

	// Now, add in the size of the new data
	newSize += StrLen( ( Char * ) data ) + 1;

	// Now, add in the size of whichever of the description and note will
	// remain unchanged.
	c = ( Char * ) src + descriptionOffset;
	cLen = StrLen( c ) + 1;

	if ( changedField != toDoRecordFieldDescription )
	{
		newSize += cLen;
	}

	if ( changedField != toDoRecordFieldNote )
	{
		c += cLen;
		newSize += StrLen( c ) + 1;
	}

	// Change the description field.
	if ( changedField == toDoRecordFieldDescription )
	{
		newFlags.description = (0 != StrLen( ( Char * ) data ));

		if ( newFlags.description != src->dataFlags.description )
		{
			DmWrite( src, OffsetOf( ToDoDBRecord, dataFlags ),
				&newFlags, sizeof( ToDoDBDataFlags ) );
		}

		// If the new description is longer, expand the record.
		curSize = MemPtrSize( src );
		if ( newSize > curSize )
		{
			MemPtrUnlock( src );
			err = MemHandleResize( recordH, newSize );
			if ( err )
			{
				return err;
			}

			src = MemHandleLock( recordH );
		}
		
		// Move the note field.
		
		// Calculate new offset of note field
		offset = descriptionOffset + StrLen( ( Char * ) data ) + 1;

		// Point c at current note field
		c = ( Char * ) src + descriptionOffset;
		c += StrLen( c ) + 1;

		DmWrite( src, offset, c, StrLen( c ) + 1 );

		// Write the new description field.
		offset = descriptionOffset;
		DmStrCopy( src, offset, ( Char * ) data );

		// If the new description is shorter, shrink the record.
		if ( newSize < curSize )
		{
			MemHandleResize( recordH, newSize );
		}

		goto exit;
	}

	// Change the note field
	if ( changedField == toDoRecordFieldNote )
	{
		newFlags.note = (0 != StrLen( ( Char * ) data ));
		
		if ( newFlags.note != src->dataFlags.note )
		{
			DmWrite( src, OffsetOf( ToDoDBRecord, dataFlags ),
				&newFlags, sizeof( ToDoDBDataFlags ) );
		}

		c = ( Char * ) src + descriptionOffset;
		offset = descriptionOffset + StrLen( c ) + 1;

		MemPtrUnlock( src );
		err = MemHandleResize( recordH, newSize );
		if ( err )
		{
			return err;
		}

		src = MemHandleLock( recordH );

		DmStrCopy( src, offset, data );

		goto exit;
	}

exit:
	MemPtrUnlock( src );

exitNoUnlock:

#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
	ECToDoDBValidate( dbP );
#endif

	return 0;
}
