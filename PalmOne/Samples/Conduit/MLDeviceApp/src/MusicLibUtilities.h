/***********************************************************************
 *
 * FILE:    MusicLibUtilities.h
 * 
 * DESCRIPTION: Music library functions
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
 
#include "DbUtilities.h"

#define IndexColumn  0
#define ArtistColumn 1
#define TitleColumn  2

void MusicLibPopulateDb(char *sDbName);
void MusicLibLoadRecord(TablePtr pTable, UInt16 dRecordIndex);
void MusicLibPopulateTable(TablePtr pTable, char *sDbName);
Err MusicLibTableLoadData(void *pTable, UInt16 dRow, UInt16 dCol, Boolean editable, MemHandle *dataH, Int16 *dataOffset, Int16 *dataSize, FieldType *fld);
Err MusicLibTableLoadData2(void *pTable, UInt16 dRow, UInt16 dCol, Boolean editable, MemHandle *dataH, Int16 *dataOffset, Int16 *dataSize, FieldType *fld);
void MusicLibAddRecord(UInt16 dIndex, char *sTitle, char *sArtist);
void MusicLibLoadLabel(TablePtr pTable, UInt16 dCol, MemHandle *hData, UInt16 *dSize, FieldType *fld);
void MusicLibLoadTblStr(TablePtr pTable, char *sTblEntry, UInt16 dRow, UInt16 dCol, MemHandle *hNew, UInt16 *dSize);
void MusicLibLoadFromDb(TablePtr pTable, UInt16 dRow, UInt16 dCol, MemHandle *hData, UInt16 *dSize, FieldType *fld, Boolean editable);
void MusicLibLoadIndex(TablePtr pTable, UInt16 dRow, MemHandle *hData, UInt16 *dSize, FieldType *fld);
