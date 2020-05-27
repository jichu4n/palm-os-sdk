/***********************************************************************
 *
 * FILE:    TableUtilites.h
 * 
 * DESCRIPTION: Table handling
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/

#include "DbUtilities.h"
#include "MusicLibUtilities.h"

void TableInit(TablePtr pTable, UInt16 dRecords);
void TableSelectRow(TablePtr pTable, UInt16 dRow);
UInt16 TableGetPointRow(TableType *pTable, UInt16 y);
TablePtr TableGet();
Boolean TableIsFull();
void TableUpdate(TablePtr pTable);
