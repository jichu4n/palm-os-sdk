/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		MemoryMgr.h
 *
 * Description:
 *		Include file for Memory Manager
 *
 * History:
 *   	10/25/94  RM - Created by Ron Marianetti
 *
 *******************************************************************/


#ifndef __MEMORYMGR_H__
#define __MEMORYMGR_H__


// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.



/************************************************************
 * Memory Manager Types
 *************************************************************/
typedef enum {	memIDPtr, memIDHandle } LocalIDKind;



/************************************************************
 * Flags accepted as parameter for MemNewChunk.
 *************************************************************/
#define memNewChunkFlagPreLock				0x0100
#define memNewChunkFlagNonMovable			0x0200
#define memNewChunkFlagAtStart				0x0400	// force allocation at front of heap
#define memNewChunkFlagAtEnd					0x0800	// force allocation at end of heap



/************************************************************
 * Memory Manager Debug settings for the MemSetDebugMode function
 *************************************************************/
#define		memDebugModeCheckOnChange			0x0001
#define		memDebugModeCheckOnAll				0x0002
#define		memDebugModeScrambleOnChange		0x0004
#define		memDebugModeScrambleOnAll			0x0008
#define		memDebugModeFillFree					0x0010
#define		memDebugModeAllHeaps					0x0020
#define		memDebugModeRecordMinDynHeapFree	0x0040




/************************************************************
 * Memory Manager result codes
 *************************************************************/
#define	memErrChunkLocked			(memErrorClass | 1)
#define	memErrNotEnoughSpace		(memErrorClass | 2)
#define	memErrInvalidParam		(memErrorClass | 3)
#define	memErrChunkNotLocked		(memErrorClass | 4)
#define	memErrCardNotPresent		(memErrorClass | 5)
#define	memErrNoCardHeader		(memErrorClass | 6)
#define	memErrInvalidStoreHeader (memErrorClass | 7)
#define	memErrRAMOnlyCard			(memErrorClass | 8)
#define	memErrWriteProtect		(memErrorClass | 9)
#define	memErrNoRAMOnCard			(memErrorClass | 10)
#define	memErrNoStore				(memErrorClass | 10)



/********************************************************************
 * Memory Manager Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------------------------------------
// Initialization
//-------------------------------------------------------------------
Err			MemInit(void)
							SYS_TRAP(sysTrapMemInit);
							
Err			MemKernelInit(void)
							SYS_TRAP(sysTrapMemKernelInit);
	
Err			MemInitHeapTable(UInt cardNo)
							SYS_TRAP(sysTrapMemInitHeapTable);
	
//-------------------------------------------------------------------
// Card formatting and Info
//-------------------------------------------------------------------
UInt			MemNumCards(void)
							SYS_TRAP(sysTrapMemNumCards);
					
Err			MemCardFormat(UInt cardNo, CharPtr cardNameP, 
					CharPtr manufNameP, CharPtr ramStoreNameP)
							SYS_TRAP(sysTrapMemCardFormat);
					
Err			MemCardInfo(UInt cardNo, 
					CharPtr cardNameP, CharPtr manufNameP,
					UIntPtr versionP, ULongPtr crDateP,
					ULongPtr romSizeP, ULongPtr ramSizeP,
					ULongPtr freeBytesP)
							SYS_TRAP(sysTrapMemCardInfo);


//-------------------------------------------------------------------
// Store Info
//-------------------------------------------------------------------
Err			MemStoreInfo(UInt cardNo, UInt storeNumber,  
					UIntPtr versionP, UIntPtr flagsP, CharPtr nameP,
					ULongPtr	crDateP, ULongPtr bckUpDateP,
					ULongPtr	heapListOffsetP, ULongPtr initCodeOffset1P,
					ULongPtr initCodeOffset2P, LocalID*	databaseDirIDP)
							SYS_TRAP(sysTrapMemStoreInfo);

Err			MemStoreSetInfo(UInt cardNo, UInt storeNumber,
					UIntPtr versionP, UIntPtr flagsP,  CharPtr nameP, 
					ULongPtr crDateP, ULongPtr bckUpDateP, 
					ULongPtr heapListOffsetP, ULongPtr initCodeOffset1P,
					ULongPtr initCodeOffset2P, LocalID*	databaseDirIDP)
							SYS_TRAP(sysTrapMemStoreSetInfo);


//-------------------------------------------------------------------
// Heap Info & Utilities
//-------------------------------------------------------------------
UInt				MemNumHeaps(UInt cardNo)
							SYS_TRAP(sysTrapMemNumHeaps);
	
UInt				MemNumRAMHeaps(UInt cardNo)
							SYS_TRAP(sysTrapMemNumRAMHeaps);
	
UInt				MemHeapID(UInt cardNo, UInt heapIndex)
							SYS_TRAP(sysTrapMemHeapID);
	
Boolean			MemHeapDynamic(UInt heapID)
							SYS_TRAP(sysTrapMemHeapDynamic);
	
Err				MemHeapFreeBytes(UInt heapID, ULongPtr freeP, ULongPtr maxP)
							SYS_TRAP(sysTrapMemHeapFreeBytes);
							
ULong	  			MemHeapSize(UInt heapID)
							SYS_TRAP(sysTrapMemHeapSize);
							
UInt				MemHeapFlags(UInt heapID)
							SYS_TRAP(sysTrapMemHeapFlags);


// Heap utilities
Err				MemHeapCompact(UInt heapID)
							SYS_TRAP(sysTrapMemHeapCompact);
							
Err				MemHeapInit(UInt heapID, Int numHandles, Boolean initContents)
							SYS_TRAP(sysTrapMemHeapInit);
							
Err				MemHeapFreeByOwnerID(UInt heapID, UInt ownerID)
							SYS_TRAP(sysTrapMemHeapFreeByOwnerID);


//-------------------------------------------------------------------
// Low Level Allocation
//-------------------------------------------------------------------
VoidPtr			MemChunkNew(UInt heapID, ULong size, UInt attr)
							SYS_TRAP(sysTrapMemChunkNew);
							
Err				MemChunkFree(VoidPtr chunkDataP)
							SYS_TRAP(sysTrapMemChunkFree);



//-------------------------------------------------------------------
// Pointer (Non-Movable) based Chunk Routines
//-------------------------------------------------------------------
VoidPtr			MemPtrNew(ULong size) 
							SYS_TRAP(sysTrapMemPtrNew);
							
#define			MemPtrFree(	p) \
						MemChunkFree(p)

// Getting Attributes
VoidHand			MemPtrRecoverHandle(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrRecoverHandle);

UInt				MemPtrFlags(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrFlags);

ULong				MemPtrSize(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrSize);
							
UInt				MemPtrOwner(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrOwner);
							
UInt				MemPtrHeapID(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrHeapID);
							
Boolean			MemPtrDataStorage(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrDataStorage);
							
UInt				MemPtrCardNo(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrCardNo);
							
LocalID			MemPtrToLocalID(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrToLocalID);

// Setting Attributes
Err				MemPtrSetOwner(VoidPtr p, UInt owner)
							SYS_TRAP(sysTrapMemPtrSetOwner);
							
Err				MemPtrResize(VoidPtr p, ULong newSize)
							SYS_TRAP(sysTrapMemPtrResize);
							
Err				MemPtrResetLock(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrResetLock);

Err				MemPtrUnlock(VoidPtr p)
							SYS_TRAP(sysTrapMemPtrUnlock);


//-------------------------------------------------------------------
// Handle (Movable) based Chunk Routines
//-------------------------------------------------------------------
VoidHand			MemHandleNew(ULong size)
							SYS_TRAP(sysTrapMemHandleNew);
							
Err				MemHandleFree(VoidHand h)
							SYS_TRAP(sysTrapMemHandleFree);

// Getting Attributes
UInt				MemHandleFlags(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleFlags);				

ULong				MemHandleSize(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleSize);

UInt				MemHandleOwner(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleOwner);

UInt				MemHandleLockCount(VoidHand h)
							SYS_TRAP(sysTrapMemHandleLockCount);
							
UInt				MemHandleHeapID(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleHeapID);

Boolean			MemHandleDataStorage(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleDataStorage);

UInt				MemHandleCardNo(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleCardNo);

LocalID			MemHandleToLocalID(VoidHand h)
							SYS_TRAP(sysTrapMemHandleToLocalID);


// Setting Attributes
Err				MemHandleSetOwner( VoidHand h,  UInt owner) 
							SYS_TRAP(sysTrapMemHandleSetOwner);
						
Err				MemHandleResize(VoidHand h,  ULong newSize) 
							SYS_TRAP(sysTrapMemHandleResize);
						
VoidPtr			MemHandleLock(VoidHand h)
							SYS_TRAP(sysTrapMemHandleLock);
							
Err				MemHandleUnlock(VoidHand h)
							SYS_TRAP(sysTrapMemHandleUnlock);
							
Err				MemHandleResetLock(VoidHand h) 
							SYS_TRAP(sysTrapMemHandleResetLock);
							
							


//-------------------------------------------------------------------
// Local ID based routines
//-------------------------------------------------------------------
VoidPtr			MemLocalIDToGlobal(LocalID local, UInt cardNo)
							SYS_TRAP(sysTrapMemLocalIDToGlobal);
							
LocalIDKind		MemLocalIDKind(LocalID local)
							SYS_TRAP(sysTrapMemLocalIDKind);

VoidPtr			MemLocalIDToPtr(LocalID local, UInt cardNo)
							SYS_TRAP(sysTrapMemLocalIDToPtr);

VoidPtr			MemLocalIDToLockedPtr(LocalID local, UInt cardNo)
							SYS_TRAP(sysTrapMemLocalIDToLockedPtr);


//-------------------------------------------------------------------
// Utilities
//-------------------------------------------------------------------
Err				MemMove(VoidPtr dstP, VoidPtr srcP, ULong numBytes)
							SYS_TRAP(sysTrapMemMove);
							
Err				MemSet(VoidPtr dstP, ULong numBytes, Byte value)
							SYS_TRAP(sysTrapMemSet);

Int				MemCmp (VoidPtr s1, VoidPtr s2, ULong numBytes)
							SYS_TRAP(sysTrapMemCmp);

Err				MemSemaphoreReserve(Boolean writeAccess)
							SYS_TRAP(sysTrapMemSemaphoreReserve);
							
Err				MemSemaphoreRelease(Boolean writeAccess)
							SYS_TRAP(sysTrapMemSemaphoreRelease);

//-------------------------------------------------------------------
// Debugging Support
//-------------------------------------------------------------------
Word				MemDebugMode(void)
							SYS_TRAP(sysTrapMemDebugMode);

Err				MemSetDebugMode(Word flags)
							SYS_TRAP(sysTrapMemSetDebugMode);

Err				MemHeapScramble(UInt heapID)
							SYS_TRAP(sysTrapMemHeapScramble);
							
Err				MemHeapCheck(UInt heapID)
							SYS_TRAP(sysTrapMemHeapCheck);


#ifdef __cplusplus 
}
#endif





#endif  // __MEMORYMGR_H__

