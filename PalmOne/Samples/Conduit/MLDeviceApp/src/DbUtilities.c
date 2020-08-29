/***********************************************************************
 *
 * FILE:    DbUtilities.c
 * 
 * DESCRIPTION: Database handling
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
#include "DbUtilities.h"
#include "MusicLibUtilities.h"

#define MAX_DB_SIZE 10

static DmOpenRef pDatabase;
LocalID  idApp;

/***********************************************************************
 *
 * FUNCTION:    DbOpenOrCreate
 * 
 * DESCRIPTION: Create or open existing database
 * 
 * PARAMETERS:  sDbName - name of database
 * 
 * RETURNED:    DmOpenRef - reference to open database
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
DmOpenRef DbOpenOrCreate(char *sDbName)
{    
     UInt16 dCard; 
     LocalID idApp, idDatabase;
	 Err error;
	 
     UInt32 dDbType    = 'DATA';
     UInt32 dDbCreator = 'IQJD';
     SysCurAppDatabase(&dCard, &idApp);
     idDatabase = DmFindDatabase(dCard, sDbName);
     if (!idDatabase) {
     	dCard = 0;
     	error = DmCreateDatabase(dCard, sDbName, dDbCreator, dDbType, false);
     }
     pDatabase = DmOpenDatabase(dCard, DmFindDatabase(dCard, sDbName), dmModeReadWrite);

     return pDatabase;
}

/***********************************************************************
 *
 * FUNCTION:    DbReleaseRecords
 * 
 * DESCRIPTION: Relase db records
 * 
 * PARAMETERS:  
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void DbReleaseRecords()
{
   int i;
   UInt16  dCardNo;
   LocalID IDLocal;
   UInt32  dRecordCount;
   
   SysCurAppDatabase(&dCardNo, &IDLocal);
   DmDatabaseSize(dCardNo, IDLocal, &dRecordCount, NULL, NULL);
   
   for(i = 0; i < MAX_DB_SIZE; i++) { 
       DmReleaseRecord(pDatabase, i, false /* not dirty */);
   }
}

/***********************************************************************
 *
 * FUNCTION:    DbAdd
 * 
 * DESCRIPTION: Add created record to database
 * 
 * PARAMETERS:  hRecord dRecordIndex pNewRecord dRecordSize
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
static void DbAdd(MemHandle hRecord, UInt16 dRecordIndex, void *pNewRecord, UInt16 dRecordSize)
{
    Boolean   dirty = true;
    MemPtr    pRecord;
    
    DmSetRecordInfo(pDatabase, dRecordIndex, NULL, NULL);
    pRecord = MemHandleLock(hRecord); 
    DmSet(pRecord, 0, dRecordSize, 0);
    DmWrite(pRecord, 0, pNewRecord, dRecordSize);
    MemPtrUnlock(pRecord);
    DmReleaseRecord(pDatabase, dRecordIndex, dirty);	
}

/***********************************************************************
 *
 * FUNCTION:    DbAddRecord
 * 
 * DESCRIPTION: Create record and add to database
 * 
 * PARAMETERS:  dRecordIndex pNewRecord dRecordSize
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void DbAddRecord(UInt16 dRecordIndex, void *pNewRecord, UInt16 dRecordSize)
{
   MemHandle hNewRecord;
   hNewRecord = DmNewRecord(pDatabase, &dRecordIndex, dRecordSize);

   if (hNewRecord) {
      DbAdd(hNewRecord, dRecordIndex, pNewRecord, dRecordSize); 
   }
   else {
      SysFatalAlert("Error: Failed to create new database record");
   }	
}

/***********************************************************************
 *
 * FUNCTION:    DbDeleteRecord
 * 
 * DESCRIPTION: Delete record from database
 * 
 * PARAMETERS:  dRecordIndex  
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void DbDeleteRecord(UInt16 dRecordIndex)
{
	DmDeleteRecord(pDatabase, dRecordIndex);
	DmMoveRecord(pDatabase, dRecordIndex, DmNumRecords(pDatabase));
}

/***********************************************************************
 *
 * FUNCTION:    DbGetRecordInfo
 * 
 * DESCRIPTION: Get db record info
 * 
 * PARAMETERS:  dRow  
 * 
 * RETURNED:    UInt32
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
UInt32 DbGetRecordInfo(UInt16 dRow)
{
	UInt32 dID;
    DmRecordInfo(pDatabase, dRow, NULL, &dID, NULL);
    return dID;
}

/***********************************************************************
 *
 * FUNCTION:    DbGetNumRecords
 * 
 * DESCRIPTION: Get number of records in database
 * 
 * PARAMETERS:    
 * 
 * RETURNED:    UInt16
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
UInt16 DbGetNumRecords()
{
   return DmNumRecords(pDatabase);	
}

/***********************************************************************
 *
 * FUNCTION:    DbGetNonDeletedRecCount
 * 
 * DESCRIPTION: Get non-deleted db records
 * 
 * PARAMETERS:    
 * 
 * RETURNED:    UInt16
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
UInt16 DbGetNonDeletedRecCount() 
{
   Err    error;
   UInt16 i;  
   UInt16 dRecordAttr;
   UInt16 dTotalRecCount = DmNumRecords(pDatabase);
   
   for(i = 0; i < dTotalRecCount; i++) { 
   	  error = DmRecordInfo(pDatabase, i, &dRecordAttr, NULL, NULL);
   	  if(dRecordAttr & dmRecAttrDelete) { // Record has been deleted
   	  	  break;
   	  }
   }
   return i;
}

/***********************************************************************
 *
 * FUNCTION:    DbClose
 * 
 * DESCRIPTION: Close database
 * 
 * PARAMETERS:    
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void DbClose()
{
     if(pDatabase) {
     	DmCloseDatabase(pDatabase);
     	pDatabase = 0;
     }	
}

/***********************************************************************
 *
 * FUNCTION:    DbGet
 * 
 * DESCRIPTION: Return database
 * 
 * PARAMETERS:    
 * 
 * RETURNED:    DmOpenRef
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
DmOpenRef DbGet()
{
     return pDatabase;	
}

/***********************************************************************
 *
 * FUNCTION:    DbGetRecordCount
 * 
 * DESCRIPTION: Get record count
 * 
 * PARAMETERS:    
 * 
 * RETURNED:    UInt16
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
UInt16 DbGetRecordCount()
{
   return DmNumRecords(pDatabase);	
}



