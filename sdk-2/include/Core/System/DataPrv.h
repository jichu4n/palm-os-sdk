/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DataPrv.h
 *
 * Description:
 *		Private Header for the Data Manager
 *
 * History:
 *   	8/10/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE
#ifndef __DATAPRV_H__
#define __DATAPRV_H__



/************************************************************
 * Structure of a Record entry
 *************************************************************/
typedef struct {
	LocalID		localChunkID;				// local chunkID of a record
	Byte			attributes;					// record attributes;
	Byte			uniqueID[3];				// unique ID of record; should
													//	not be 0 for a legal record.
	} RecordEntryType;
typedef RecordEntryType*	RecordEntryPtr;



/************************************************************
 * Structure of a Resource entry
 *************************************************************/
typedef struct {
	DWord			type;							// resource type
	Word			id;							// resource id
	LocalID		localChunkID;				// resource data chunk ID
	} RsrcEntryType;
typedef RsrcEntryType*	RsrcEntryPtr;

// Attributes field
#define	dmRsrcAttrUnused		0x0000	// to be defined...



/************************************************************
 * Structure of a record list extension. This is used if all
 *  the database record/resource entries of a database can't fit into
 *  the database header.
 *************************************************************/
typedef struct {
	LocalID				nextRecordListID;			// local chunkID of next list
	Word					numRecords;					// number of records in this list
	Word					firstEntry;					// array of Record/Rsrc entries 
															// starts here
	} RecordListType;
typedef RecordListType*	RecordListPtr;




/************************************************************
 * Structure of a Database Header
 *************************************************************/
typedef struct {
	Byte			name[dmDBNameLength];	// name of database
	Word			attributes;					// database attributes
	Word			version;						// version of database

	DWord			creationDate;				// creation date of database
	DWord			modificationDate;			// latest modification date
	DWord			lastBackupDate;			// latest backup date
	DWord			modificationNumber;		// modification number of database

	LocalID		appInfoID;					// application specific info
	LocalID		sortInfoID;					// app specific sorting info

	DWord			type;							// database type
	DWord			creator;						// database creator 
	
	DWord			uniqueIDSeed;				// used to generate unique IDs.
													//	Note that only the low order
													//	3 bytes of this is used (in
													//	RecordEntryType.uniqueID).
													//	We are keeping 4 bytes for 
													//	alignment purposes.

	RecordListType	recordList;				// first record list
	} DatabaseHdrType;		

typedef DatabaseHdrType*	DatabaseHdrPtr;
typedef DatabaseHdrPtr*		DatabaseHdrHand;





/************************************************************
 * Directory of all available databases - maintained by the
 *  Database Manager. This directory also includes resource
 *  databases. There is one of these directories for each store
 *  (either RAM or ROM store) in a memory card.
 *************************************************************/
typedef struct {
	LocalID		nextDatabaseListID;		// continuation list, if any
	Word			numDatabases;				// # of database ID's in this list
	LocalID		databaseID[1];				// ChunkID of each Database
	} DatabaseDirType;

typedef DatabaseDirType*	DatabaseDirPtr;




/************************************************************
 * This structure is used to hold information on an open database
 * It gets allocated as a non-movable chunk in the dynamic heap
 *  when a database is opened and caches database information that
 *  helps speed up performance.
 *************************************************************/
typedef struct  {
	VoidPtr				next;							// ptr to next access structure
	UInt					openCount;					// incremented every time it's opened
	ULong					ownerTaskID;				// taskID of owner
	UInt					exclusive 	: 1;			// true if opened with exclusive access
	UInt					writeAccess : 1;			// true if opened with write access
	UInt					resDB		 	: 1;			// true if resource database
	LocalID				hdrID;						// local ID of database header
	DatabaseHdrHand	hdrH;							// Database header handle
	DatabaseHdrPtr		hdrP;							// Database header ptr for faster access
	UInt					cardNo;						// card number of database
	UInt					numRecords; 				// cached here for better performance
	Ptr					hdrMasterP;					// master ptr to header if ROM based
	Ptr*					handleTableP;				// pointer to block of ROM handles
	} DmOpenInfoType;

typedef DmOpenInfoType*		DmOpenInfoPtr;



/************************************************************
 * This structure is used to hold client specific information for
 *  an open database. There is one of these allocated every time 
 *  a database is opened, even if it's already open. This structure
 *  holds the open mode of the client and a pointer to the DmOpenInfoType
 *  of the database which contains the common database info for all
 *  clients. The SysAppInfoPtr, referenced off of the current A5, 
 *  has a pointer to the first DmAccessType in the linked list.
 *************************************************************/
typedef struct DmAccessType {
	struct DmAccessType*	next;							// ptr to next access structure
	UInt						mode;							// mode it was opened in
	DmOpenInfoPtr			openP;						// ptr to DmOpenInfoType for database
	} DmAccessType;

typedef DmAccessType*		DmAccessPtr;



/************************************************************
 * This structure is used to hold the results of a search
 *  in DmGetNextDatabaseByTypeCreator. It is the "real" structure
 *  of the generic type DmSearchStateType used by applications.
 *************************************************************/
typedef struct {
		Int				index;
		UInt				relIndex;
		SByte				cardNo;
		SByte				storeNo;
		UInt				storeEntries;
		UInt				storeStartIndex;
		LocalID			dirID;
		} DmPrvSearchStateType;
typedef DmPrvSearchStateType*	DmPrvSearchStatePtr;	
	

/************************************************************
 * This structure is used to keep track of protected databases. These
 *  are databases which have been protected through the DmDatabaseProtect()
 *  call. A handle to an array of these structures is stored in the
 *  low memory global GDmProtectListH. The number of entries in the array
 *  must be calculated by taking the size of the handle and dividing
 *  by sizeof (DmProtectEntryType)
 *************************************************************/
typedef struct {
		Byte				protectCount;
		Byte				cardNo;
		LocalID			dbID;
		} DmProtectEntryType, *DmProtectEntryPtr;
	


/************************************************************
 * The owner ID of chunks allocated by the Database Manager
 *************************************************************/
#define	dmDynOwnerID					0x00		// Dynamic heap chunks
#define	dmMgrOwnerID					0x01		// Management chunks
#define	dmRecOwnerID					0x02		// Record chunks
#define	dmOrphanOwnerID				0x03		// Orphaned record chunks


/************************************************************
 * Private Data Manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err			DmMoveOpenDBContext(DmAccessPtr* listHeadP, DmAccessPtr dbP)
							SYS_TRAP(sysTrapDmMoveOpenDBContext);



#ifdef __cplusplus
}
#endif


#endif // __DATAPRV_H__
#endif //NON_PORTABLE

