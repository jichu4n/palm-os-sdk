/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DataMgr.h
 *
 * Description:
 *		Header for the Data Manager
 *
 * History:
 *   	11/14/94  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __DATAMGR_H__
#define __DATAMGR_H__


// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.

// Other headers we depend on
#include <MemoryMgr.h>				 


/************************************************************
 * Category equates
 *************************************************************/
#define	dmRecAttrCategoryMask	0x0F	// mask for category #
#define	dmRecNumCategories		16		// number of categories
#define	dmCategoryLength			16		// 15 chars + 1 null terminator

#define  dmAllCategories			0xff
#define  dmUnfiledCategory  		0

#define	dmMaxRecordIndex			0xffff



// Record Attributes 
#define	dmRecAttrDelete			0x80	// delete this record next sync
#define	dmRecAttrDirty				0x40	// archive this record next sync
#define	dmRecAttrBusy				0x20	// record currently in use
#define	dmRecAttrSecret			0x10	// "secret" record - password protected




/************************************************************
 * Database Header equates
 *************************************************************/
#define	dmDBNameLength				32			// 31 chars + 1 null terminator

// Attributes of a Database
#define	dmHdrAttrResDB					0x0001	// Resource database
#define 	dmHdrAttrReadOnly				0x0002	// Read Only database
#define	dmHdrAttrAppInfoDirty		0x0004	// Set if Application Info block is dirty
															// Optionally supported by an App's conduit
#define	dmHdrAttrBackup				0x0008	//	Set if database should be backed up to PC if
															//	no app-specific synchronization conduit has
															//	been supplied.
#define	dmHdrAttrOKToInstallNewer 	0x0010	// This tells the backup conduit that it's OK
															//  for it to install a newer version of this database
															//  with a different name if the current database is
															//  open. This mechanism is used to update the 
															//  Graffiti Shortcuts database, for example. 
#define	dmHdrAttrResetAfterInstall	0x0020 	// Device requires a reset after this database is 
															// installed.
#define	dmHdrAttrOpen					0x8000	// Database not closed properly


/************************************************************
 * Unique ID equates
 *************************************************************/
#define	dmRecordIDReservedRange		1			// The range of upper bits in the database's
															// uniqueIDSeed from 0 to this number are
															// reserved and not randomly picked when a
															// database is created.
#define	dmDefaultRecordsID			0			// Records in a default database are copied
															// with their uniqueIDSeeds set in this range.


/************************************************************
 * Mode flags passed to DmOpenDatabase
 *************************************************************/
#define	dmModeReadOnly				0x0001		// read  access
#define	dmModeWrite					0x0002		// write access
#define	dmModeReadWrite			0x0003		// read & write access
#define	dmModeLeaveOpen			0x0004		// leave open when app quits
#define	dmModeExclusive			0x0008		// don't let anyone else open it
#define	dmModeShowSecret			0x0010		// force show of secret records

// Generic type used to represent an open Database
typedef	VoidPtr						DmOpenRef;


/************************************************************
 * Structure passed to DmGetNextDatabaseByTypeCreator and used
 *  to cache search information between multiple searches.
 *************************************************************/
typedef struct {
	DWord		info[8];
	} DmSearchStateType;
typedef DmSearchStateType*	DmSearchStatePtr;	



/************************************************************
 * Structures used by the sorting routines
 *************************************************************/
typedef struct {
	Byte			attributes;					// record attributes;
	Byte			uniqueID[3];				// unique ID of record
	} SortRecordInfoType;

typedef SortRecordInfoType * SortRecordInfoPtr;

typedef Int DmComparF (void *, void *, Int other, SortRecordInfoPtr, 
								SortRecordInfoPtr, VoidHand appInfoH);



/************************************************************
 * Database manager error codes
 * the constant dmErrorClass is defined in SystemMgr.h
 *************************************************************/
#define	dmErrMemError					(dmErrorClass | 1)
#define	dmErrIndexOutOfRange			(dmErrorClass | 2)
#define	dmErrInvalidParam				(dmErrorClass | 3)
#define	dmErrReadOnly					(dmErrorClass | 4)
#define	dmErrDatabaseOpen				(dmErrorClass | 5)
#define	dmErrCantOpen					(dmErrorClass | 6)
#define	dmErrCantFind					(dmErrorClass | 7)
#define	dmErrRecordInWrongCard		(dmErrorClass | 8)
#define	dmErrCorruptDatabase			(dmErrorClass | 9)
#define	dmErrRecordDeleted			(dmErrorClass | 10)
#define	dmErrRecordArchived			(dmErrorClass | 11)
#define	dmErrNotRecordDB				(dmErrorClass | 12)
#define	dmErrNotResourceDB			(dmErrorClass | 13)
#define	dmErrROMBased					(dmErrorClass | 14)
#define	dmErrRecordBusy				(dmErrorClass | 15)
#define	dmErrResourceNotFound		(dmErrorClass | 16)
#define	dmErrNoOpenDatabase			(dmErrorClass | 17)
#define	dmErrInvalidCategory			(dmErrorClass | 18)
#define	dmErrNotValidRecord			(dmErrorClass | 19)
#define	dmErrWriteOutOfBounds		(dmErrorClass | 20)
#define	dmErrSeekFailed				(dmErrorClass | 21)
#define	dmErrAlreadyOpenForWrites	(dmErrorClass | 22)
#define	dmErrOpenedByAnotherTask	(dmErrorClass | 23)
#define  dmErrUniqueIDNotFound		(dmErrorClass | 24)
#define  dmErrAlreadyExists			(dmErrorClass | 25)
#define	dmErrInvalidDatabaseName	(dmErrorClass | 26)
#define	dmErrDatabaseProtected		(dmErrorClass | 27)


/************************************************************
 * Values for the direction parameter of DmSeekRecordInCategory
 *************************************************************/
#define dmSeekForward				 1
#define dmSeekBackward				-1


/************************************************************
 * Data Manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


// Initialization
Err		DmInit(void)
							SYS_TRAP(sysTrapDmInit);


// Directory Lists
Err		DmCreateDatabase(UInt cardNo, const CharPtr nameP, 
					ULong creator, ULong type, Boolean resDB)
							SYS_TRAP(sysTrapDmCreateDatabase);

Err		DmCreateDatabaseFromImage(Ptr bufferP)
							SYS_TRAP(sysTrapDmCreateDatabaseFromImage);


Err		DmDeleteDatabase(UInt cardNo, LocalID dbID)
							SYS_TRAP(sysTrapDmDeleteDatabase);

UInt		DmNumDatabases(UInt cardNo)
							SYS_TRAP(sysTrapDmNumDatabases);
							
LocalID	DmGetDatabase(UInt cardNo, UInt index)
							SYS_TRAP(sysTrapDmGetDatabase);

LocalID	DmFindDatabase(UInt cardNo, const CharPtr nameP)
							SYS_TRAP(sysTrapDmFindDatabase);

Err	DmGetNextDatabaseByTypeCreator(Boolean newSearch, DmSearchStatePtr stateInfoP,
			 		ULong	type, ULong creator, Boolean onlyLatestVers, 
			 		UIntPtr cardNoP, LocalID* dbIDP)
							SYS_TRAP(sysTrapDmGetNextDatabaseByTypeCreator);


// Database info
Err		DmDatabaseInfo(UInt cardNo, LocalID	dbID, const CharPtr nameP,
					UIntPtr attributesP, UIntPtr versionP, ULongPtr crDateP,
					ULongPtr	modDateP, ULongPtr bckUpDateP,
					ULongPtr	modNumP, LocalID* appInfoIDP,
					LocalID* sortInfoIDP, ULongPtr typeP,
					ULongPtr creatorP)
							SYS_TRAP(sysTrapDmDatabaseInfo);

Err		DmSetDatabaseInfo(UInt cardNo, LocalID	dbID, const CharPtr nameP,
					UIntPtr attributesP, UIntPtr versionP, ULongPtr crDateP,
					ULongPtr	modDateP, ULongPtr bckUpDateP,
					ULongPtr	modNumP, LocalID* appInfoIDP,
					LocalID* sortInfoIDP, ULongPtr typeP,
					ULongPtr creatorP)
							SYS_TRAP(sysTrapDmSetDatabaseInfo);

Err		DmDatabaseSize(UInt cardNo, LocalID dbID, ULongPtr numRecordsP,
					ULongPtr	totalBytesP, ULongPtr dataBytesP)
							SYS_TRAP(sysTrapDmDatabaseSize);
							
							
// This routine can be used to prevent a database from being deleted (by passing
//  true for 'protect'). It will increment the protect count if 'protect' is true
//  and decrement it if 'protect' is false. This is used by code that wants to
//  keep a particular record or resource in a database locked down but doesn't
//  want to keep the database open. This information is keep in the dynamic heap so
//  all databases are "unprotected" at system reset. 
Err		DmDatabaseProtect(UInt cardNo, LocalID dbID, Boolean protect)
							SYS_TRAP(sysTrapDmDatabaseProtect);


// Open Databases info
DmOpenRef	DmOpenDatabase(UInt cardNo, LocalID dbID, UInt mode)
							SYS_TRAP(sysTrapDmOpenDatabase);
							
DmOpenRef	DmOpenDatabaseByTypeCreator(ULong type, ULong creator, UInt mode)
							SYS_TRAP(sysTrapDmOpenDatabaseByTypeCreator);
							
Err			DmCloseDatabase(DmOpenRef dbP)
							SYS_TRAP(sysTrapDmCloseDatabase);
							

DmOpenRef	DmNextOpenDatabase(DmOpenRef currentP)
							SYS_TRAP(sysTrapDmNextOpenDatabase);
							
Err			DmOpenDatabaseInfo(DmOpenRef dbP, LocalID* dbIDP, 
					UIntPtr openCountP, UIntPtr modeP, UIntPtr cardNoP,
					BooleanPtr resDBP)
							SYS_TRAP(sysTrapDmOpenDatabaseInfo);
							
LocalID		DmGetAppInfoID (DmOpenRef dbP)
							SYS_TRAP(sysTrapDmGetAppInfoID);


// Utility to unlock all records and clear busy bits
Err			DmResetRecordStates(DmOpenRef dbP)
							SYS_TRAP(sysTrapDmResetRecordStates);


// Error Query
Err			DmGetLastErr(void)
							SYS_TRAP(sysTrapDmGetLastErr);


//------------------------------------------------------------
// Record based access routines
//------------------------------------------------------------

// Record Info
UInt		DmNumRecords(DmOpenRef dbP)
							SYS_TRAP(sysTrapDmNumRecords);

UInt		DmNumRecordsInCategory(DmOpenRef dbP, UInt category)
							SYS_TRAP(sysTrapDmNumRecordsInCategory);

Err		DmRecordInfo(DmOpenRef dbP, UInt index,
					UIntPtr attrP, ULongPtr uniqueIDP, LocalID* chunkIDP)
							SYS_TRAP(sysTrapDmRecordInfo);

Err		DmSetRecordInfo(DmOpenRef dbP, UInt index,
					UIntPtr attrP, ULongPtr uniqueIDP)
							SYS_TRAP(sysTrapDmSetRecordInfo);
							


// Record attaching and detaching
Err		DmAttachRecord(DmOpenRef dbP, UIntPtr atP,
					Handle	newH, Handle* oldHP)
							SYS_TRAP(sysTrapDmAttachRecord);

Err		DmDetachRecord(DmOpenRef dbP, UInt index,
					Handle* oldHP)
							SYS_TRAP(sysTrapDmDetachRecord);
					
Err		DmMoveRecord(DmOpenRef dbP, UInt from, UInt to)
							SYS_TRAP(sysTrapDmMoveRecord);



// Record creation and deletion
VoidHand	DmNewRecord(DmOpenRef dbP, UIntPtr atP,
					ULong size)
							SYS_TRAP(sysTrapDmNewRecord);

Err		DmRemoveRecord(DmOpenRef dbP, UInt index)
							SYS_TRAP(sysTrapDmRemoveRecord);

Err		DmDeleteRecord(DmOpenRef dbP, UInt index)
							SYS_TRAP(sysTrapDmDeleteRecord);

Err		DmArchiveRecord(DmOpenRef dbP, UInt index)
							SYS_TRAP(sysTrapDmArchiveRecord);

VoidHand	DmNewHandle(DmOpenRef dbP, ULong size)
							SYS_TRAP(sysTrapDmNewHandle);

Err		DmRemoveSecretRecords(DmOpenRef dbP)
							SYS_TRAP(sysTrapDmRemoveSecretRecords);


// Record viewing manipulation
Err		DmFindRecordByID(DmOpenRef dbP, ULong uniqueID, UIntPtr indexP)
							SYS_TRAP(sysTrapDmFindRecordByID);

VoidHand	DmQueryRecord(DmOpenRef dbP, UInt index)
							SYS_TRAP(sysTrapDmQueryRecord);

VoidHand	DmGetRecord(DmOpenRef dbP, UInt index)
							SYS_TRAP(sysTrapDmGetRecord);
							
VoidHand	DmQueryNextInCategory(DmOpenRef dbP, UIntPtr indexP, UInt category)
							SYS_TRAP(sysTrapDmQueryNextInCategory);
							
UInt		DmPositionInCategory (DmOpenRef dbP, UInt index, UInt category)
							SYS_TRAP(sysTrapDmPositionInCategory);
							
Err		DmSeekRecordInCategory (DmOpenRef dbP, UIntPtr indexP, Int offset,
					Int direction, UInt category)
							SYS_TRAP(sysTrapDmSeekRecordInCategory);


VoidHand	DmResizeRecord(DmOpenRef dbP, UInt index, ULong newSize)
							SYS_TRAP(sysTrapDmResizeRecord);

Err		DmReleaseRecord(DmOpenRef dbP, UInt index, Boolean dirty)
							SYS_TRAP(sysTrapDmReleaseRecord);

Int		DmSearchRecord(VoidHand recH, DmOpenRef* dbPP)
							SYS_TRAP(sysTrapDmSearchRecord);


// Category manipulation
Err		DmMoveCategory (DmOpenRef dbP, UInt toCategory, UInt fromCategory, Boolean dirty)
							SYS_TRAP(sysTrapDmMoveCategory);

Err		DmDeleteCategory (DmOpenRef dbR, UInt categoryNum)
							SYS_TRAP(sysTrapDmDeleteCategory);
							
							
// Validation for writing
Err		DmWriteCheck(VoidPtr recordP, ULong offset, ULong bytes)
							SYS_TRAP(sysTrapDmWriteCheck);
							
// Writing
Err		DmWrite(VoidPtr recordP, ULong offset, VoidPtr srcP, ULong bytes)
							SYS_TRAP(sysTrapDmWrite);
							
Err		DmStrCopy(VoidPtr recordP, ULong offset, CharPtr srcP)
							SYS_TRAP(sysTrapDmStrCopy);

Err		DmSet(VoidPtr recordP, ULong offset, ULong bytes, Byte value)
							SYS_TRAP(sysTrapDmSet);
							

							

//------------------------------------------------------------
// Resource based access routines
//------------------------------------------------------------

// High level access routines
VoidHand		DmGetResource(ULong type, Int id)
							SYS_TRAP(sysTrapDmGetResource);

VoidHand		DmGet1Resource(ULong type, Int id)
							SYS_TRAP(sysTrapDmGet1Resource);

Err			DmReleaseResource(VoidHand resourceH)
							SYS_TRAP(sysTrapDmReleaseResource);

VoidHand		DmResizeResource(VoidHand resourceH, ULong newSize)
							SYS_TRAP(sysTrapDmResizeResource);


// Searching resource databases  
DmOpenRef	DmNextOpenResDatabase(DmOpenRef dbP)
							SYS_TRAP(sysTrapDmNextOpenResDatabase);

Int			DmFindResourceType(DmOpenRef dbP, ULong resType, Int typeIndex)
							SYS_TRAP(sysTrapDmFindResourceType);

Int			DmFindResource(DmOpenRef dbP, ULong resType, Int resID, 
					VoidHand resH)
							SYS_TRAP(sysTrapDmFindResource);

Int			DmSearchResource(ULong resType, Int resID,
					VoidHand resH, DmOpenRef* dbPP)
							SYS_TRAP(sysTrapDmSearchResource);


// Resource Info
UInt			DmNumResources(DmOpenRef dbP)
							SYS_TRAP(sysTrapDmNumResources);

Err			DmResourceInfo(DmOpenRef dbP, Int index,
					ULongPtr resTypeP, IntPtr resIDP,  
					LocalID* chunkLocalIDP)
							SYS_TRAP(sysTrapDmResourceInfo);

Err			DmSetResourceInfo(DmOpenRef dbP, Int index,
					ULongPtr resTypeP, IntPtr resIDP)
							SYS_TRAP(sysTrapDmSetResourceInfo);



// Resource attaching and detaching
Err			DmAttachResource(DmOpenRef dbP, VoidHand	newH, 
					ULong resType, Int resID)
							SYS_TRAP(sysTrapDmAttachResource);

Err			DmDetachResource(DmOpenRef dbP, Int index,
					VoidHand* oldHP)
							SYS_TRAP(sysTrapDmDetachResource);



// Resource creation and deletion
VoidHand		DmNewResource(DmOpenRef dbP, ULong resType, Int resID,
					ULong size)
							SYS_TRAP(sysTrapDmNewResource);

Err			DmRemoveResource(DmOpenRef dbP, Int index)
							SYS_TRAP(sysTrapDmRemoveResource);



// Resource manipulation
VoidHand		DmGetResourceIndex(DmOpenRef dbP, Int index)
							SYS_TRAP(sysTrapDmGetResourceIndex);



// Record sorting
Err 			DmQuickSort(const DmOpenRef dbP, DmComparF *compar, Int other)
							SYS_TRAP(sysTrapDmQuickSort);

Err			DmInsertionSort (const DmOpenRef dbR, DmComparF *compar, Int other)
							SYS_TRAP(sysTrapDmInsertionSort);

UInt			DmFindSortPosition(DmOpenRef dbP, VoidPtr newRecord,
					SortRecordInfoPtr newRecordInfo, DmComparF *compar, Int other)
							SYS_TRAP(sysTrapDmFindSortPosition);

UInt			DmFindSortPositionV10(DmOpenRef dbP, VoidPtr newRecord,
					DmComparF *compar, Int other)
							SYS_TRAP(sysTrapDmFindSortPositionV10);

#ifdef __cplusplus
}
#endif

#endif // __DATAMGR_H__

