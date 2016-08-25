/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		MemoryMgrPrv.h
 *
 * Description:
 *		Private includes for Memory Manager
 *
 * History:
 *   	7/20/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE


// SystemPrv.h must be included before this header file. SystemPrv.h will
//  then in turn include this header.
#ifndef __SYSTEMPRV_H__
#include <SystemPrv.h>
#endif

#ifndef __MEMORYMGRPRV_H__
#define __MEMORYMGRPRV_H__



/********************************************************************
 * Structure of a Master Pointer chunk
 * The first master pointer chunk is always immediately following the
 *  heap header. As more movable chunks are created, new master 
 *  pointer chunks are allocated dynamically and linked together through
 *  the nextTblOffset field and prevTblOffset field.
 ********************************************************************/
typedef struct {
	Word			numEntries;				// number of master pointer entries
	Word			nextTblOffset;			// offset from start of heap to next table
	//Ptr			mstrP[numEntries];	// array of pointers to movable chunks
	} MstrPtrTableType;
typedef MstrPtrTableType*	MstrPtrTablePtr;


/********************************************************************
 * Structure of a Heap
 *
 * A heap starts with a HeapHeader
 * Followed by the offset table (numHandles)
 * Followed by movable chunks
 * Followed by non-movable chunks
 ********************************************************************/
typedef struct {
	Word					flags;			// heap flags;
	Word					size;				// size of heap - 0 means 64K;
	MstrPtrTableType	mstrPtrTbl;		// Master pointer table
	} HeapHeaderType;
typedef HeapHeaderType*  		HeapHeaderPtr;

// Flags Field
#define memHeapFlagReadOnly	0x01			// heap is read-only (ROM based)

#define memMstrPtrTableGrowBy			50		// # of entries to grow master pointer tables by
#define memMstrPtrTableInitSizeD		40		// # of entries to allocate initially for dynamic heap(s)
#define memMstrPtrTableInitSizeS		200	// # of entries to allocate initially for storage heap(s)



/********************************************************************
 * Structure of a Chunk
 *
 ********************************************************************/
typedef struct {
	Word			size;				// size of block in bytes, including header
	Byte			lockOwner;		// lock count in high nibble, owner in low
	Byte			flags;			// flags, size adj in low nibble
	SWord			hOffset;			// offset/2 to handle for movables
	} ChunkHeaderType;
typedef ChunkHeaderType* 		ChunkHeaderPtr; 

// Definition of ChunkHeaderType.flags --------------------------------
#define memChunkFlagFree		0x80		// free bit
#define memChunkFlagUnused1	0x40		// unused
#define memChunkFlagUnused2	0x20		// unused
#define memChunkFlagUnused3	0x10		// unused
#define memChunkFlagSizeAdj	0x0F		// mask for size adjustment


// A non-movable chunk has 0xF as the lock count
#define memChunkLockMask		0xF0
#define memChunkOwnerMask		0x0F





/************************************************************
 * Structure of a Card Header.
 * There is 1 card header for in every card that has ROM. The
 *  card header is stored at sysCardHeaderOffset into the card.
 *
 * RAM only cards will not have a card header
 *************************************************************/
#define	memMaxNameLen				32		// size of name and manuf fields including null
typedef struct {
	DWord			initStack;					// initial stack pointer
	DWord			resetVector;				// reset vector
	DWord			signature;					// must be sysCardSignature
	Word			hdrVersion;					// header version
	Word			flags;						// card flags;
	Byte			name[memMaxNameLen];		// card name
	Byte			manuf[memMaxNameLen];	// card manufacturer's name
	Word			version;						// card version
	DWord			creationDate;				// card creation date
	Word			numRAMBlocks;				// number of RAM blocks on card
	DWord			blockListOffset;			// offset to RAM block list
	Byte			reserved[164];				// to bring us to 0x100 alignment
	} CardHeaderType;
typedef CardHeaderType*		CardHeaderPtr;

#define	memCardHeaderFlagRAMOnly	0x0001	// RAM only card



/************************************************************
 * Structure of a Storage Header. 
 * There is 1 of these for every "store" on a memory card. A
 *  "store" can be all the RAM on a card or all the ROM on a card.
 *
 * The RAM storage header is stored at sysRAMHeader offset into the
 *	 card. and the ROM storage header is stored at sysROMHeader offset
 *  into the card.
 *************************************************************/
typedef struct {
	DWord			signature;					// must be sysStoreSignature
	Word			version;						// version of header
	Word			flags;						// flags
	Byte			name[memMaxNameLen];		// name of store
	DWord			creationDate;				// creation date
	DWord			backupDate;					// last backup date
	DWord			heapListOffset;			// offset to heap list for store
	DWord			initCodeOffset1;			// init code for store, if any
	DWord			initCodeOffset2;			// second init code for store, if any
	LocalID		databaseDirID;				// local ID of database dir.
	DWord			rsvSpace;					// where first heap starts.
	DWord			dynHeapSpace;				// how big the dynamic heap area is
													//  (always 0 for ROM stores)
	DWord			firstRAMBlockSize;		// Copy of firstRAMBlock size from cardinfo
													// Used to determine if we're rebooting
													// with a different amount of RAM.
													
	// The following fields are used to store non-volatile information that
	//  must be accessed by the system but that is not convenient to store
	//  in a database due to access time or frequency. It is only valid
	//  in the RAM STORE ON CARD #0!.
	SysNVParamsType	nvParams;
	
	// Filler bytes - reserved for future use. Size adjusted to
	// keep total size of storage header at 0x100 bytes.
	Byte			reserved[176-sizeof(SysNVParamsType)];		

	// CRC value
	DWord			crc;							// crc to check validity	
	} StorageHeaderType;
typedef StorageHeaderType*		StorageHeaderPtr;

#define	memStoreHeaderFlagRAMOnly	0x0001	// RAM store


// A Heap list for each store on a card (either RAM or ROM) gives a list of
// heaps' starting offsets for that store
typedef struct {
	Word				numHeaps;			// Number of heaps in store
	DWord				heapOffset[1];		// offset to heap
	} HeapListType;
	
	

/********************************************************************
 * CardInfo structure
 * The Pilot globals has a pointer to an array of CardInfo structures.
 *
 * This array is initialized by the Boot code before the Memory Manager
 *  starts up. Among other things, it tells the Memory Manager where 
 *  every card is and the size of each card.
 ********************************************************************/
typedef struct {

	// These fields are filled in by the boot code BEFORE
	//  MemInit() is called.
	BytePtr		baseP;						// base address of card
	DWord			size;							// address range of card
	DWord			firstRAMBlockSize;		// size of RAM block at offset 0
	DWord			targetROMShift;			// used only under Emulation mode, must be
													//  added to emulated ROM pointers when calculating
													//  a LocalID for use in the device ROM.

	
	DWord			cardHeaderOffset;			// offset to card header (usually in ROM)
													//  = 0 if RAM only card
												
	DWord			rsvSpace;					// reserve space in RAM before first heap
	DWord			dynHeapSpace;				// how much space to reserve for dynamic heap(s)
	StorageHeaderPtr		ramStoreP;		// pointer to RAM store header 
													// This location depends on how much space
													//  was reserved on the card for the dynamic
													//  heap.
												
	// These fields are copies of information in the card and storage headers
	//  they are cached here for quicker access by the Memory Manager. They
	//  are filled in by the Memory Manager during MemInit.
	Word			numRAMHeaps;				// Number of RAM heaps - for quick access
	DWordPtr		ramHeapOffsetsP;			// Table of RAM heap offsets - for quick access
	Word			numROMHeaps;				// Number of RAM heaps - for quick access
	DWordPtr		romHeapOffsetsP;			// Table of RAM heap offsets - for quick access

	} CardInfoType;
typedef CardInfoType*	CardInfoPtr;

// The constant hwrNumCardSlots, defined in Hardware.h defines the maximum number
//  of cards supported by the hardware.




/************************************************************
 * Private Memory Manager Constants
 *************************************************************/
// Default size of the dynamic heap
#define 	memDynamicHeapSize		(0x08000-sysLowMemSize)


// Private constant - # of dynamic heaps
#define	memDynamicHeaps		1




/************************************************************
 * Private Memory Manager Macros. These macros are not
 *  guaranteed to be compatible in the future and should
 *  only be used in system code, not applications.
 *
 * To use these, define NON_PORTABLE at the top of your
 *  source code module.
 *
 * WARNING: This macro must only be used on Locked chunks!!!
 *          When running with error checking, it will verify
 *				that the chunk is locked before it dereferences it.
 *************************************************************/

#if MEMORY_FORCE_LOCK == MEMORY_FORCE_LOCK_ON
	#define	memHandleProtect(h)											\
				((VoidHand)((DWord)h | 0x80000000))
	#define	memHandleUnProtect(h)										\
				((VoidHand)((DWord)h & 0x7FFFFFFF))
#else
	#define	memHandleProtect(h)		(h)
	#define	memHandleUnProtect(h)	(h)
#endif					
	

#if MEMORY_TYPE == MEMORY_LOCAL 
	#if ERROR_CHECK_LEVEL != ERROR_CHECK_NONE									 
		#define	MemDeref(h) 																\
			(MemHandleLockCount(h)==0 ? (*((VoidHand)0x80000000)) : (*memHandleUnProtect(h))) 
	#else																					 
		#define	MemDeref(h) 	 															\
			(*(memHandleUnProtect(h)))	
	#endif												
		
#else
	#if ERROR_CHECK_LEVEL != ERROR_CHECK_NONE									 
		#define	MemDeref(h) 																\
			(MemHandleLockCount(h)==0 ? (*((VoidHand)0x80000000)) : ((Ptr)ShlDWord(h))) 
	#else																					 
		#define	MemDeref(h) 	 															\
			((Ptr)ShlDWord(memHandleUnProtect(h)))	
	#endif												
#endif

#endif		// NON_PORTABLE



// Typecast access to the MemCardInfo Ptr
#define	memCardInfoP(cardNo)				\
				( ((CardInfoPtr) ((LowMemHdrType*)PilotGlobalsP)->globals.memCardInfoP) + cardNo)



/********************************************************************
 * Memory Manager Private Routines
 * These are used only by internal System code
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


HeapHeaderPtr	MemHeapPtr(UInt heapID)
							SYS_TRAP(sysTrapMemHeapPtr);
	
Err				MemStoreSearch(VoidPtr startP, DWord range, StorageHeaderPtr* storePP)
							SYS_TRAP(sysTrapMemStoreSearch);

Err				MemStoreInit(DWord firstHeapSize, UInt firstHeapMstrPtrs, 
						UInt cardNo,  Word numBlocks, DWordPtr blockInfoP, 
						StorageHeaderPtr storeP, Word numMstrPtrs)
							SYS_TRAP(sysTrapMemStoreInit);
	
Err				MemNVParams(Boolean set, SysNVParamsPtr paramsP)
							SYS_TRAP(sysTrapMemNVParams);

#ifdef __cplusplus 
}
#endif






#endif  // __MEMORYMGRPRV_H__

