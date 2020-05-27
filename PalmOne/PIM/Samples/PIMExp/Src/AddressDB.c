/******************************************************************************
 *
 * Copyright (c) 1995-2002 PalmSource, Inc. All rights reserved.
 *
 * File: AddressDB.c
 *
 * Release: Palm OS 5 SDK (68K) R3.
 *
 * Description:
 *      Address Manager routines
 *
 *****************************************************************************/

#include "AddressDB.h"
#include "AddrDefines.h"

#include <UIResources.h>
#include <SysUtils.h>
#include <ErrorMgr.h>
#include <StringMgr.h>
#include <TextMgr.h>
#include <PalmUtils.h>

#include <FeatureMgr.h>
#include <AddressSortLib.h>

#define sortKeyFieldBits   (BitAtPosition(name) | BitAtPosition(firstName) | BitAtPosition(company))

// The following structure doesn't really exist.  The first field
// varies depending on the data present.  However, it is convient
// (and less error prone) to use when accessing the other information.

typedef struct {
	AddrOptionsType		options;            // Display by company or by name
	AddrDBRecordFlags	flags;
	UInt8				companyFieldOffset; // Offset from firstField
	char				firstField;
} PrvAddrPackedDBRecord;


/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/

static void		PrvAddrDBFindKey(PrvAddrPackedDBRecord *r, char **key, UInt16 *whichKey, Int16 sortByCompany);
static Int16	PrvAddrDBComparePackedRecords(PrvAddrPackedDBRecord *r1, PrvAddrPackedDBRecord *r2, Int16 sortByCompany, SortRecordInfoPtr /*info1*/, SortRecordInfoPtr /*info2*/, MemHandle /*appInfoH*/);
static Int16	PrvAddrDBUnpackedSize(AddrDBRecordPtr r);
static void		PrvAddrDBPack(AddrDBRecordPtr s, void * recordP);
static void		PrvAddrDBUnpack(PrvAddrPackedDBRecord *src, AddrDBRecordPtr dest);
static UInt16	PrvAddrDBFindSortPosition(DmOpenRef dbP, PrvAddrPackedDBRecord *newRecord);

AddrAppInfoPtr   AddrDBAppInfoGetPtr(DmOpenRef dbP)
{
	UInt16     cardNo;
	LocalID    dbID;
	LocalID    appInfoID;

	if (DmOpenDatabaseInfo(dbP, &dbID, NULL, NULL, &cardNo, NULL))
		return NULL;
	if (DmDatabaseInfo(cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &appInfoID, NULL, NULL, NULL))
		return NULL;

	if (appInfoID == 0)
		return NULL;
	else
		return ((AddrAppInfoType*) MemLocalIDToLockedPtr(appInfoID, cardNo));

}

Err AddrDBNewRecord(DmOpenRef dbP, AddrDBRecordPtr r, UInt16 *index)
{
	MemHandle               recordH;
	Err                   	err;
	PrvAddrPackedDBRecord*  recordP;
	UInt16                  newIndex;

	// 1) and 2) (make a new chunk with the correct size)
	recordH = DmNewHandle(dbP, (Int32) PrvAddrDBUnpackedSize(r));
	if (recordH == NULL)
		return dmErrMemError;

	// 3) Copy the data from the unpacked record to the packed one.
	recordP = (PrvAddrPackedDBRecord*) MemHandleLock(recordH);
	PrvAddrDBPack(r, recordP);

	// Get the index
	newIndex = PrvAddrDBFindSortPosition(dbP, recordP);
	MemPtrUnlock(recordP);

	// 4) attach in place
	err = DmAttachRecord(dbP, &newIndex, recordH, 0);
	if (err)
		MemHandleFree(recordH);
	else
		*index = newIndex;

	return err;
}

Err AddrDBChangeRecord(DmOpenRef dbP, UInt16 *index, AddrDBRecordPtr r, AddrDBRecordFlags changedFields, UInt16 blobDirty)
{
	AddrDBRecordType   		src;
	MemHandle          		srcH;
	Err            			result;
	MemHandle          		recordH=0;
	MemHandle          		oldH;
	Int16              		i;
	AddrDBRecordFlags		changes;
	Int16              		sortByCompany;
	AddrAppInfoPtr   		appInfoPtr;
	Boolean          		dontMove;
	UInt16             		attributes;      // to contain the deleted flag

	PrvAddrPackedDBRecord* 	cmpP;
	PrvAddrPackedDBRecord* 	recordP;
	UInt16				    Counter = 0;
	
	MemHandle packedH;
	
	MemMove(&changes, &changedFields, sizeof(changedFields));

	// We do not assume that r is completely valid so we get a valid
	// AddrDBRecordPtr...
	if ((result = AddrDBGetRecord(dbP, *index, &src, &srcH)) != 0)
		return result;
	
	// Get handle to packed record, we need to figure out if there is blob data
	packedH = DmQueryRecord(dbP, *index);
	
	// And we apply the changes to it.
	MemMove(&(src.options), (&r->options), sizeof(AddrOptionsType));
	
	for (i = firstAddressField; i < addrNumStringFields; i++)
	{
		// If the flag is set, point to the string else NULL
		if (GetBitMacro(changes, i) != 0)
		{
			src.fields[i] = r->fields[i];
			RemoveBitMacro(changes, i);
		}
		
		if ((changes.allBits == 0) && (changes.allBits2 == 0))
			break;  // no more changes
	}
	
	// Fill in birthday info only if the birth date is present.
	if(GetBitMacro(changes, birthdayDate))
	{
		src.birthdayInfo.birthdayDate = r->birthdayInfo.birthdayDate;
		RemoveBitMacro(changes, birthdayDate);
	}
	
	if(GetBitMacro(changes, birthdayMask))
	{
		src.birthdayInfo.birthdayMask = r->birthdayInfo.birthdayMask;
		RemoveBitMacro(changes, birthdayMask);
	}
	
	if(GetBitMacro(changes, birthdayPreset))
	{
		src.birthdayInfo.birthdayPreset = r->birthdayInfo.birthdayPreset;
		RemoveBitMacro(changes, birthdayPreset);
	}
	
	
	// change 2.0
		
	if(r->rel2blobInfo.dirty)
	{
		MemMove(&(src.rel2blobInfo.anniversaryInfo), &(r->rel2blobInfo.anniversaryInfo), sizeof(BirthdayInfo) + sizeof(ToneIdentifier));
		r->rel2blobInfo.dirty = false;
	}
	
	//
	
	
	if(r->pictureInfo.pictureDirty)
	{
		src.pictureInfo.pictureDirty = 1;
		src.pictureInfo.pictureSize = r->pictureInfo.pictureSize;
		src.pictureInfo.pictureData = r->pictureInfo.pictureData;		
	}

	if(blobDirty)
	{
		for(Counter = 0;Counter< r->numBlobs  ;Counter++)
		{
			src.blobs[Counter] = r->blobs[Counter]; 
		} 
		src.numBlobs = r->numBlobs;
	}

	// 1) and 2) (make a new chunk with the correct size)
	recordH = DmNewHandle(dbP, PrvAddrDBUnpackedSize(&src));
	if (recordH == NULL)
	{
		MemHandleUnlock(srcH);  // undo lock from AddrGetRecord above
		return dmErrMemError;
	}
	recordP = (PrvAddrPackedDBRecord*) MemHandleLock(recordH);

	// 3) Copy the data from the unpacked record to the packed one.
	PrvAddrDBPack(&src, recordP);

	// The original record is copied and no longer needed.
	MemHandleUnlock(srcH);

	// 4) If the sort position changes...
	// Check if any of the key fields have changed
	if ((changedFields.allBits & sortKeyFieldBits) == 0)
		goto attachRecord;

	// Make sure *index-1 < *index < *index+1, if so it's in sorted
	// order.  Leave it there.
	appInfoPtr = (AddrAppInfoPtr) AddrDBAppInfoGetPtr(dbP);
	sortByCompany = appInfoPtr->misc.sortByCompany;
	MemPtrUnlock(appInfoPtr);

	if (*index > 0)
	{
		// This record wasn't deleted and deleted records are at the end of the
		// database so the prior record may not be deleted!
		cmpP = (PrvAddrPackedDBRecord*) MemHandleLock(DmQueryRecord(dbP, *index-1));
		dontMove = (PrvAddrDBComparePackedRecords (cmpP,  recordP, sortByCompany,
							NULL, NULL, 0) == -1);
		MemPtrUnlock(cmpP);
	}
	else
		dontMove = true;


	if (*index+1 < DmNumRecords (dbP))
	{
		DmRecordInfo(dbP, *index+1, &attributes, NULL, NULL);
		if (attributes & dmRecAttrDelete)
			;      // don't move it after the deleted record!
		else {
			cmpP = (PrvAddrPackedDBRecord*) MemHandleLock(DmQueryRecord(dbP, *index+1));
			dontMove = dontMove && (PrvAddrDBComparePackedRecords (recordP, cmpP, sortByCompany,
									NULL, NULL, 0) == -1);
			MemPtrUnlock(cmpP);
		}
	}

	if (dontMove)
		goto attachRecord;

	// The record isn't in the right position.  Move it.
	i = PrvAddrDBFindSortPosition(dbP, recordP);
	DmMoveRecord(dbP, *index, i);
	if (i > *index) i--;
	*index = i; // return new position

	// Attach the new record to the old index, the preserves the
	// category and record id.
attachRecord:

	result = DmAttachRecord(dbP, index, recordH, &oldH);
	MemPtrUnlock(recordP);
	if (result) return result;

	MemHandleFree(oldH);
	return 0;
}

Err AddrDBGetRecord(DmOpenRef dbP, UInt16 index, AddrDBRecordPtr recordP,
				  MemHandle *recordH)
{
	PrvAddrPackedDBRecord *src;

	*recordH = DmQueryRecord(dbP, index);
	
	if (*recordH == NULL)
		return dmErrIndexOutOfRange;
		
	src = (PrvAddrPackedDBRecord *) MemHandleLock(*recordH);
	if (src == NULL)
		return dmErrIndexOutOfRange;

	PrvAddrDBUnpack(src, recordP);

	return 0;
}

Int16 PrvAddrDBUnpackedSize(AddrDBRecordPtr r)
{
	Int16 size;
	Int16   index;

	size = sizeof (PrvAddrPackedDBRecord) - sizeof (char);   // correct
	for (index = firstAddressField; index < addrNumStringFields; index++)
	{
		 if (r->fields[index] != NULL)
			size += StrLen(r->fields[index]) + 1;
	}
	
	//calculate length for birthday info
	if(r->birthdayInfo.birthdayDate.month && r->birthdayInfo.birthdayDate.day)
	{
		size += sizeof(DateType);
		size += sizeof(AddressDBBirthdayFlags);
		if(r->birthdayInfo.birthdayMask.alarm)
		{
			size += sizeof(UInt8); //preset			
		}
	}
	if(r->pictureInfo.pictureSize && r->pictureInfo.pictureData)
	{
		size += sizeof(UInt32); //blob id
		size+=sizeof(r->pictureInfo.pictureSize); //blob size, same as sizeof(picture size)
		size+= sizeof(r->pictureInfo.pictureDirty);  		
		size+= r->pictureInfo.pictureSize;
	}
	
	
// change 2.0

	// SDS - if either an anniversary OR a custom ringtone, add the blob.
	if(r->rel2blobInfo.anniversaryInfo.birthdayDate.month && r->rel2blobInfo.anniversaryInfo.birthdayDate.day || r->rel2blobInfo.ringtoneInfo.id)
	{
		size += sizeof(UInt32);	//blob id
		size +=sizeof(UInt16);	//blob size

		size += sizeof(ToneIdentifier) + sizeof(BirthdayInfo);
	}

//	
	
	
	

	for (index = 0; index < r->numBlobs; index++)
 	{
 		size += sizeof(r->blobs[index].creatorID);
 		size += sizeof(r->blobs[index].size);
 		size += r->blobs[index].size;
 	}
	return size;
}




void PrvAddrDBPack(AddrDBRecordPtr s, void * recordP)
{
	Int32                offset;
	AddrDBRecordFlags    flags;
	Int16                index;
	PrvAddrPackedDBRecord*  d=0;
	UInt16                len;
	void *               srcP;
	UInt8                companyFieldOffset;
	UInt16				blobCount = 0;
 	UInt16				size = 0;

	flags.allBits = 0;
	flags.allBits2 = 0;

	DmWrite(recordP, (Int32)&d->options, &s->options, sizeof(s->options));
	offset = (Int32)&d->firstField;

	for (index = firstAddressField; index < addrNumStringFields; index++) {
		if (s->fields[index] != NULL)
			/*         if (s->fields[index][0] == '\0')
			 {
			 // so set the companyFieldOffset or clear it code doesn't fail
			 s->fields[index] = NULL;
			 }
			 else
			 */
		{
			//ErrFatalDisplayIf(s->fields[index][0] == '\0' && index != P1ctnote,
			//				  "Empty field being added");
			
			// Skip fields with empty strings
			if ((s->fields[index][0] != '\0') || (index == note))
			{
				srcP = s->fields[index];
				len = StrLen((Char*)srcP) + 1;
				DmWrite(recordP, offset, srcP, len);
				offset += len;
				SetBitMacro(flags, index);
			}
		}
	}
	
	//write birthday info
	len = sizeof(DateType); 						
	
	//fill in birthday info only if the birth date is present
	if(s->birthdayInfo.birthdayDate.month && s->birthdayInfo.birthdayDate.day)
	{
		DmWrite(recordP, offset, &(s->birthdayInfo.birthdayDate), len);
		offset += len;
		SetBitMacro(flags, birthdayDate);
		
		//write other birthday info only if birthdate is valid				
		//write birthday mask to determine if year is set.
		len = sizeof(AddressDBBirthdayFlags);				
		DmWrite(recordP, offset, &(s->birthdayInfo.birthdayMask), len);
		offset += len;
		SetBitMacro(flags, birthdayMask);
			
		//set preset and text only if Date Book reminder option is set
		if(s->birthdayInfo.birthdayMask.alarm)
		{
			len = sizeof(UInt8);
			DmWrite(recordP, offset, &(s->birthdayInfo.birthdayPreset), len);
			offset += len;
			SetBitMacro(flags, birthdayPreset);
		}		
	}
	
	//set picture blob information
	if(s->pictureInfo.pictureSize && s->pictureInfo.pictureData)
	{
		UInt32 blobId = addrPictureBlobId;
		UInt16 blobSize = 0;
		
		//set 4 byte id
		len = sizeof(blobId);	
		DmWrite(recordP, offset, &blobId, len);
		offset += len;
		
		//set 2 bytes len: blob size = 2 bytes picture dirty flag + picture size
		len = sizeof(blobSize);
		blobSize = s->pictureInfo.pictureSize + sizeof(s->pictureInfo.pictureDirty);
		DmWrite(recordP, offset, &blobSize, len);
		offset += len;		
		
		//set dirty flag and picture
		len = sizeof(s->pictureInfo.pictureDirty);	
		DmWrite(recordP, offset, &(s->pictureInfo.pictureDirty), len);
		offset += len;
		
		DmWrite(recordP, offset, (s->pictureInfo.pictureData), s->pictureInfo.pictureSize);
		offset += s->pictureInfo.pictureSize;		
		
		blobCount++;
 	}
 	
	// Write the Release 2 blob information
	if( (s->rel2blobInfo.anniversaryInfo.birthdayDate.month && s->rel2blobInfo.anniversaryInfo.birthdayDate.day) || s->rel2blobInfo.ringtoneInfo.id )
	{
		UInt32 blobId = addrRelease2BlobId;
		UInt16 blobSize = 0;
		
		//set 4 byte id
		len = sizeof(blobId);	
		DmWrite(recordP, offset, &blobId, len);
		offset += len;
		
		//set 2 bytes len: blob size = 2 bytes
		len = sizeof(blobSize);
		blobSize = sizeof(ToneIdentifier) + sizeof(BirthdayInfo);
		DmWrite(recordP, offset, &blobSize, len);
		offset += len;		
		
		// write out anniversary info and ringtone info
		DmWrite(recordP, offset, &(s->rel2blobInfo.anniversaryInfo), blobSize);
		offset += len;
						
		blobCount++;
	}
 	
 	// Include any other blobs we don't understand.
 	ErrNonFatalDisplayIf(blobCount + s->numBlobs > apptMaxBlobs, "Too many blobs");
 	// Include any other blobs we don't understand.
	for (index = 0; index < s->numBlobs; index++)
	{
		size = sizeof(s->blobs[index].creatorID);
		DmWrite(recordP, offset, &(s->blobs[index].creatorID), size);
		offset += size;
		size = sizeof(s->blobs[index].size);
		DmWrite(recordP, offset, &(s->blobs[index].size), size);
		offset += size;
		DmWrite(recordP, offset, s->blobs[index].content, s->blobs[index].size);
		offset += s->blobs[index].size;
	}
 
 	ErrNonFatalDisplayIf(offset > MemHandleSize(MemPtrRecoverHandle(recordP)), "Not enough room for packed record");

	// Set the flags indicating which fields are used
	DmWrite(recordP, (Int32)&d->flags.allBits, &flags.allBits, sizeof(flags.allBits));
	DmWrite(recordP, (Int32)&d->flags.allBits2, &flags.allBits2, sizeof(flags.allBits2));

	// Set the companyFieldOffset or clear it
	if (s->fields[company] == NULL)
		companyFieldOffset = 0;
	else {
		index = 1;
		if (s->fields[name] != NULL)
			index += StrLen(s->fields[name]) + 1;
		if (s->fields[firstName] != NULL)
			index += StrLen(s->fields[firstName]) + 1;
		companyFieldOffset = (UInt8) index;
	}
	DmWrite(recordP, (Int32)(&d->companyFieldOffset), &companyFieldOffset, sizeof(companyFieldOffset));
}




void PrvAddrDBUnpack(PrvAddrPackedDBRecord *src, AddrDBRecordPtr dest)
{
	Int16   				index;
	AddrDBRecordFlags 		flags;
	char 					*p;
 	UInt16					recordSize = 0;
 	Char 					*blobStart;
 	Char 					*blobEnd;
 	UInt32					blobId;
 	UInt16 					blobSize;

	MemMove(&(dest->options), &(src->options), sizeof(AddrOptionsType));
	MemMove(&flags, &(src->flags), sizeof(AddrDBRecordFlags));
	p = &src->firstField;


	for (index = firstAddressField; index < addrNumStringFields; index++)
	{
		// If the flag is set point to the string else NULL
		if (GetBitMacro(flags, index) != 0)
		{
			dest->fields[index] = p;
			p += StrLen(p) + 1;
		}
		else
			dest->fields[index] = NULL;
	}
	
	//unpack birthday info
	MemSet(&(dest->birthdayInfo), sizeof(BirthdayInfo), 0 );
	
	if(GetBitMacro(flags, birthdayDate))
	{
		MemMove(&(dest->birthdayInfo.birthdayDate), p, sizeof(DateType));
		p+= sizeof(DateType);
	}
	if(GetBitMacro(flags, birthdayMask)	)
	{
		MemMove(&(dest->birthdayInfo.birthdayMask), p, sizeof(AddressDBBirthdayFlags));
		//dest->birthdayInfo.birthdayMask = *((AddressDBBirthdayFlags*)p);		
		p+= sizeof(AddressDBBirthdayFlags);
	}
	if(GetBitMacro(flags, birthdayPreset)	)
	{
		dest->birthdayInfo.birthdayPreset = *((UInt8*)p);		
		p+= sizeof(UInt8);
	}
	//get picture info	
	dest->pictureInfo.pictureDirty = 0;
	dest->pictureInfo.pictureSize = 0;
	dest->pictureInfo.pictureData = NULL;
	
	// SDS - init all blob2 fields to zero.
	MemSet(&(dest->rel2blobInfo), sizeof(Release2BlobType), 0 );
	
	dest->numBlobs = 0;	

 	// Then iterate through the blobs, ignoring any we don't understand.
 	blobStart = p;			// First blob starts where last non-blob data ends.
 	recordSize = MemPtrSize(src);
 	while (blobStart < (Char *)src + recordSize)
   	{	
   		p = blobStart;
		ErrNonFatalDisplayIf((Char *)src + recordSize - blobStart <= sizeof (blobId) + sizeof (blobSize),"Invalid blob encountered");
 		MemMove(&blobId, p, sizeof (blobId));
 		p += sizeof (blobId);
 		MemMove(&blobSize, p, sizeof (blobSize));		
 		p += sizeof (blobSize);
 		blobEnd = p + blobSize;			// Blob size excludes space to store ID and size of blob.
 		switch (blobId)
   		{
	   		case addrPictureBlobId:
			{
				UInt16  pictureDirtySize;
				pictureDirtySize = sizeof(dest->pictureInfo.pictureDirty);
				dest->pictureInfo.pictureSize = blobSize - pictureDirtySize;
				MemMove(&(dest->pictureInfo.pictureDirty), p, pictureDirtySize);
				p+= pictureDirtySize;
					
				if(dest->pictureInfo.pictureSize)
					dest->pictureInfo.pictureData = p;
				p+= dest->pictureInfo.pictureSize;
				break;				
			}
			case addrRelease2BlobId:
			{
				MemMove(&dest->rel2blobInfo.anniversaryInfo, p, sizeof(BirthdayInfo));
				p += sizeof(BirthdayInfo);
				MemMove(&dest->rel2blobInfo.ringtoneInfo, p, sizeof(ToneIdentifier));
				p += sizeof(ToneIdentifier);
				break;
			}
			default:
			{
				ErrNonFatalDisplayIf (dest->numBlobs >= apptMaxBlobs, "Too many blobs");
				dest->blobs[dest->numBlobs].creatorID = blobId;
				dest->blobs[dest->numBlobs].size = blobSize;
				dest->blobs[dest->numBlobs].content = p;				
	
				dest->numBlobs++;
				p = blobEnd;
				break;
	 		}		

   		}
		ErrNonFatalDisplayIf(p != blobEnd, "Blob size does not agree with contents");
		blobStart = blobEnd;	// Next blob starts where last blob ends.
	}	
	ErrNonFatalDisplayIf(blobStart != (Char *)src + recordSize,
		"Last blob not aligned with end of record - don't let fields edit records directly!");
}



Int16 PrvAddrDBComparePackedRecords(PrvAddrPackedDBRecord *r1,
												PrvAddrPackedDBRecord *r2,
												Int16 sortByCompany,
												SortRecordInfoPtr UNUSED_PARAM(info1),
												SortRecordInfoPtr UNUSED_PARAM(info2),
												MemHandle UNUSED_PARAM(appInfoH))
{
	UInt16 whichKey1, whichKey2;
	char *key1, *key2;
	Int16 result;

	whichKey1 = 1;
	whichKey2 = 1;

	do {
		PrvAddrDBFindKey(r1, &key1, &whichKey1, sortByCompany);
		PrvAddrDBFindKey(r2, &key2, &whichKey2, sortByCompany);

		// A key with NULL loses the StrCompare.
		if (key1 == NULL)
		{
			// If both are NULL then return them as equal
			if (key2 == NULL)
			{
				result = 0;
				return result;
			}
			else
				result = -1;
		}
		else
			if (key2 == NULL)
				result = 1;
			else
			{
				// With Palm OS 4.0, StrCompare will try a caseless
				// comparison first, then a case-sensitive, so we
				// only need to call StrCompare. Also, we can call
				// TxtCompare to avoid one extra trap dispatch.
				
				// result = StrCaselessCompare(key1, key2);
				// if (result == 0)
				//		result = StrCompare(key1, key2);
				
				result = TxtCompare(	key1,		// const Char *s1,
											0xFFFF,	// UInt16 s1Len,
											NULL,		// UInt16 *s1MatchLen,
											key2,		// const Char *s2,
											0xFFFF,	// UInt16 s2Len,
											NULL);	// UInt16 *s2MatchLen
			}

	} while (!result);


	return result;
}

UInt16 PrvAddrDBFindSortPosition(DmOpenRef dbP, PrvAddrPackedDBRecord *newRecord)
{
	Int16 sortByCompany;
	AddrAppInfoPtr appInfoPtr;


	appInfoPtr = (AddrAppInfoPtr) AddrDBAppInfoGetPtr(dbP);
	sortByCompany = appInfoPtr->misc.sortByCompany;
	MemPtrUnlock(appInfoPtr);

	return DmFindSortPosition(dbP, (void *) newRecord, NULL, (DmComparF *)
							  PrvAddrDBComparePackedRecords, (Int16) sortByCompany);
}

void PrvAddrDBFindKey(PrvAddrPackedDBRecord *r, char **key, UInt16 *whichKey, Int16 sortByCompany)
{
	AddrDBRecordFlags fieldFlags;

	fieldFlags.allBits = r->flags.allBits;

	ErrFatalDisplayIf(*whichKey == 0 || *whichKey == 5, "Bad addr key");

	if (sortByCompany)
	{
		if (*whichKey == 1 && fieldFlags.bits.company)
		{
			*whichKey = 2;
			goto returnCompanyKey;
		}

		if (*whichKey <= 2 && fieldFlags.bits.name)
		{
			*whichKey = 3;
			goto returnNameKey;
		}

		if (*whichKey <= 3 && fieldFlags.bits.firstName)
		{
			*whichKey = 4;
			goto returnFirstNameKey;
		}
	}
	else
	{
		if (*whichKey == 1 && fieldFlags.bits.name)
		{
			*whichKey = 2;
			goto returnNameKey;
		}

		if (*whichKey <= 2 && fieldFlags.bits.firstName)
		{
			*whichKey = 3;
			goto returnFirstNameKey;
		}

		// For now don't consider company name when sorting by person name
		// unless there isn't a name or firstName
		if (*whichKey <= 3 && fieldFlags.bits.company &&
			!(fieldFlags.bits.name || fieldFlags.bits.firstName))
		{
			*whichKey = 4;
			goto returnCompanyKey;
		}

	}

	// All possible fields have been tried so return NULL so that
	// the uniq ID is compared.
	*whichKey = 5;
	*key = NULL;
	return;



returnCompanyKey:
	*key = (char *) &r->companyFieldOffset + r->companyFieldOffset;
	return;


returnNameKey:
	*key = &r->firstField;
	return;


returnFirstNameKey:
	*key = &r->firstField;
	if (r->flags.bits.name)
	{
		*key += StrLen(*key) + 1;
	}
	return;

}