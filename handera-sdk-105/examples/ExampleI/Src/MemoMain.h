/******************************************************************************
 *
 * Copyright (c) 1995-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MemoMain.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Include file the Memo application
 *
 * History:
 *   	9/27/95	Created by Christopher Raff
 *		10/02/99	Externed the SetDBBackupBit() routine.
 *
 *****************************************************************************/

#ifndef 	__MEMOMAIN_H__
#define	__MEMOMAIN_H__

#include <IMCUtils.h>
#include <ExgMgr.h>

#define memoDBName						"MemoDB"
#define memoDBType						'DATA'
#define memoMaxLength					4096		// note: must be same as tFLD 1109 max length!!!
															// dje - Was 8192. Changed to fix bug #24574.
#define memoExtension					"txt"
#define memoMIMEType						"text/plain"


/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


typedef UInt32 ReadFunctionF (const void * stream, Char * bufferP, UInt32 length);
typedef UInt32 WriteFunctionF (void * stream, const Char * const bufferP, Int32 length);


// From MemoTransfer.c
extern void MemoSendRecord (DmOpenRef dbP, Int16 recordNum, const Char * const prefix);

extern void MemoSendCategory (DmOpenRef dbP, UInt16 categoryNum, const Char * const prefix, UInt16 noDataAlertID);

extern Err MemoReceiveData(DmOpenRef dbP, ExgSocketPtr exgSocketP, UInt16 *numRecordsReceived);
extern void MemoTransferPreview(ExgPreviewInfoType *infoP);

extern Boolean MemoImportMime(DmOpenRef dbR, void * inputStream, ReadFunctionF inputFunc, 
	Boolean obeyUniqueIDs, Boolean beginAlreadyRead, UInt16 *numRecordsReceivedP, Char* descriptionP, UInt16 descriptionSize);

extern void MemoExportMime(DmOpenRef dbP, Int16 index, MemoDBRecordType *recordP, 
	void * outputStream, WriteFunctionF outputFunc, 
	Boolean writeUniqueIDs, Boolean outputMimeInfo);
	
extern void SetDBBackupBit(DmOpenRef dbP);

#ifdef __cplusplus 
}
#endif

#endif	//	__MEMOMAIN_H__
