/***********************************************************************
 *
 * FILE:        DbUtilities.h
 * 
 * DESCRIPTION: Database handling
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
#include <PalmOS.h>

DmOpenRef DbOpenOrCreate(char *sDbName);
void DbAddRecord(UInt16 dRecordIndex, void *newRecord, UInt16 dRecordSize);
DmOpenRef DbGet();
void      DbClose();
void DbDeleteRecord(UInt16 dRecordIndex);
MemPtr DbGetLockedRecord(UInt16 dRecordIndex);
UInt16 DbGetNumRecords();
void DbResizeRecord(UInt16 dRecordIndex, void *sNewEntry, UInt16 dResizeTo);
UInt16 DbGetRecordCount();
UInt32 DbGetRecordInfo(UInt16 dRow);
void DbReleaseRecords();
UInt16 DbGetNonDeletedRecCount();