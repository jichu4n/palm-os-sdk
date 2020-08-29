/***********************************************************************
 *
 * FILE:        MusicLibUtilities.c
 * 
 * DESCRIPTION: Music library functions
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
#include "MusicLibUtilities.h"

#define MAX_LIB_LENGTH  10
#define MAX_STR_LENGTH 100

typedef struct {
	char   sTitle[MAX_STR_LENGTH];
	char   sArtist[MAX_STR_LENGTH];
} MusicLibEntry;


/***********************************************************************
 *
 * FUNCTION:    MusicLibLoadLabel
 * 
 * DESCRIPTION: Load label
 * 
 * PARAMETERS:  pTable dCol hData hData fld
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void MusicLibLoadLabel(TablePtr pTable, UInt16 dCol, MemHandle *hData, UInt16 *dSize, FieldType *fld)
{   
	UInt16 fldSize;
    char sLabel[MAX_STR_LENGTH];
    MemHandle hNew;
    char *sNew;

    if(dCol == ArtistColumn) {
    	StrCopy(sLabel, "Artist");
    } else if(dCol == TitleColumn) {
    	StrCopy(sLabel, "Title");
    } else if(dCol == IndexColumn) { 
        return;	
    }
    
    hNew = MemHandleNew(sizeof(sLabel) + 1);   
    sNew = (char *)MemHandleLock(hNew);      
    StrCopy(sNew, sLabel);
    *hData = hNew;
    *dSize = MemHandleSize(hNew);
    MemPtrUnlock(sNew);
    fldSize = FldGetMaxChars(fld);
    FldSetMaxChars(fld, 20);
    FldSetTextAllocatedSize(fld, 100);
}

/***********************************************************************
 *
 * FUNCTION:    MusicLibLoadTblStr
 * 
 * DESCRIPTION: Load table string
 * 
 * PARAMETERS:  pTable sTblEntry dRow dCol hNew dSize
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void MusicLibLoadTblStr(TablePtr pTable, char *sTblEntry, UInt16 dRow, UInt16 dCol, MemHandle *hNew, UInt16 *dSize) 
{
	char *sNew;
	UInt16 dSizeL = sizeof(sTblEntry) + 1;
	MemHandle hNew1;
	
	hNew1 = MemHandleNew(dSizeL);	
	*hNew = hNew1;
	*dSize = MemHandleSize(hNew1);
	sNew = (char *)MemHandleLock(hNew1);
	StrCopy(sNew, sTblEntry);   
	MemPtrUnlock(sNew);
}

/***********************************************************************
 *
 * FUNCTION:    MusicLibLoadFromDb
 * 
 * DESCRIPTION: Load table string
 * 
 * PARAMETERS:  pTable dRow dCol hData dSize fld editable
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void MusicLibLoadFromDb(TablePtr pTable, UInt16 dRow, UInt16 dCol, MemHandle *hData, UInt16 *dSize, FieldType *fld, Boolean editable)
{
    MemHandle hRec; 
    DmOpenRef pDatabase;
    MusicLibEntry *pLibEntry;
    MemHandle hNew;
    char *sNew;
 
    pDatabase = DbGet();
    hRec = DmQueryRecord(pDatabase, dRow-1);
    if(hRec) {
    	pLibEntry = MemHandleLock(hRec); 
    	if(dCol == ArtistColumn) {
            *dSize = StrLen(pLibEntry->sArtist) + 1;
    	} else if(dCol == TitleColumn) {
            *dSize = StrLen(pLibEntry->sTitle) + 1;
    	}
    	hNew = MemHandleNew(*dSize);
    	sNew = (char *)MemHandleLock(hNew);
    	
    	if(dCol == ArtistColumn) {
    	    StrCopy(sNew, pLibEntry->sArtist);
    	} else if(dCol == TitleColumn) {
    	    StrCopy(sNew, pLibEntry->sTitle);
    	}

    	MemPtrUnlock(pLibEntry);
    	MemPtrUnlock(sNew);
    	*hData = hNew;
    
        if (fld) {
            FldSetMaxChars(fld, 20);
        }
    }
}

/***********************************************************************
 *
 * FUNCTION:    MusicLibLoadIndex
 * 
 * DESCRIPTION: Load index
 * 
 * PARAMETERS:  pTable dRow hData dSize fld 
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void MusicLibLoadIndex(TablePtr pTable, UInt16 dRow, MemHandle *hData, UInt16 *dSize, FieldType *fld)
{
	UInt16 *dIndex;
	MemHandle hNew = MemHandleNew(sizeof(UInt16)); 

    dIndex = (UInt16 *)MemHandleLock(hNew);  
    *dIndex = dRow;
    *hData = hNew;   
    *dSize = MemHandleSize(hNew);
    MemPtrUnlock(dIndex);
}

/***********************************************************************
 *
 * FUNCTION:    MusicLibTableLoadData
 * 
 * DESCRIPTION: Load label, index and music library record
 * 
 * PARAMETERS:  pTable dRow dCol editable hData dOffset dSize fld
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
Err MusicLibTableLoadData(void *pTable, UInt16 dRow, UInt16 dCol, Boolean editable, MemHandle *hData, Int16 *dOffset, Int16 *dSize, FieldType *fld)
{
    if(dRow == 0) {
    	MusicLibLoadLabel(pTable, dCol, hData, (unsigned short *)dSize, fld);
    } else if(dCol == IndexColumn) { 
        MusicLibLoadIndex(pTable, dRow, hData, (unsigned short *)dSize, fld);	
    } else {
    	MusicLibLoadFromDb(pTable, dRow, dCol, hData, (unsigned short *)dSize, fld, editable);
    }
    *dOffset = 0;
    return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    MusicLibAddRecord
 * 
 * DESCRIPTION: Add record to database
 * 
 * PARAMETERS:  dIndex sTitle sArtist 
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void MusicLibAddRecord(UInt16 dIndex, char *sTitle, char *sArtist)
{
	MusicLibEntry libEntry;
        
    StrCopy(libEntry.sTitle, sTitle);
    StrCopy(libEntry.sArtist, sArtist);

	DbAddRecord(dIndex, &libEntry, sizeof(MusicLibEntry));
}


