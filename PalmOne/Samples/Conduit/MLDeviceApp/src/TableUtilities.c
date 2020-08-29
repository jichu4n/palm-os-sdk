/***********************************************************************
 *
 * FILE:    TableUtilites.c
 * 
 * DESCRIPTION: Table handling
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
#include "TableUtilities.h"

#define MAX_TABLE_SIZE 10
TablePtr gpTable;

/***********************************************************************
 *
 * FUNCTION:    TableUpdate
 * 
 * DESCRIPTION: Update table
 * 
 * PARAMETERS:  pTable
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void TableUpdate(TablePtr pTable) 
{  
   UInt16 row;
   UInt16 dTblRowCount = TblGetNumberOfRows(pTable);   
   UInt16 dRecords = DbGetNonDeletedRecCount();
   
   TblSetRowUsable(pTable, 0, true);
   TblSetRowID(pTable, 0, 0);
   TblSetRowHeight(pTable, 0, 11);
    
   for (row = 1; row < dTblRowCount; row++) { 
  
       if(row <= dRecords) {
           UInt32 id = DbGetRecordInfo(row - 1);
           TblSetRowUsable(pTable, row, true);
           TblSetRowID(pTable, row, id);
           TblSetRowHeight(pTable, row, 11);
           TblSetItemInt(pTable, row, IndexColumn, row);
       } else { 
       	   TblSetRowUsable(pTable, row, false);
       }
   }
}

/***********************************************************************
 *
 * FUNCTION:    TableSelectRow
 * 
 * DESCRIPTION: Select row
 * 
 * PARAMETERS:  pTable dRow
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void TableSelectRow(TablePtr pTable, UInt16 dRow)
{
   TblSelectItem(pTable, dRow, IndexColumn);
}

/***********************************************************************
 *
 * FUNCTION:    TableInit
 * 
 * DESCRIPTION: Initialize table
 * 
 * PARAMETERS:  pTable dRecords
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: Initial revision
 *
 ***********************************************************************/
void TableInit(TablePtr pTable, UInt16 dRecords)
{
	UInt16 row;
	char indexInitStr[] = "Init";
	char libInitStr[]   = "Initialization str";
    		
   	for (row = 0; row < (dRecords + 1); row++) {
 
   		TblSetRowUsable(pTable, row, true);
   		TblSetItemStyle(pTable, row, IndexColumn, numericTableItem); 	
        TblSetItemStyle(pTable, row, ArtistColumn, textTableItem); 
        TblSetItemStyle(pTable, row, TitleColumn, textTableItem); 
   		TblSetRowHeight(pTable, row, FntLineHeight()); /* make sure all rows are single line */
        TblSetItemPtr(pTable, row, ArtistColumn, NULL);
        TblSetItemPtr(pTable, row, TitleColumn,  NULL);       
    } 
    
    TblSetColumnUsable(pTable, IndexColumn,  true);
    TblSetColumnUsable(pTable, ArtistColumn, true); 
    TblSetColumnUsable(pTable, TitleColumn,  true);
    
    TblSetColumnWidth(pTable, IndexColumn,  FntCharsWidth(indexInitStr, StrLen(indexInitStr))); 
    TblSetColumnWidth(pTable, ArtistColumn, FntCharsWidth(libInitStr, StrLen(libInitStr))); 
    TblSetColumnWidth(pTable, TitleColumn,  FntCharsWidth(libInitStr, StrLen(libInitStr))); 
        
    TblSetLoadDataProcedure(pTable, IndexColumn,  (TableLoadDataFuncPtr)MusicLibTableLoadData);     
    TblSetLoadDataProcedure(pTable, ArtistColumn, (TableLoadDataFuncPtr)MusicLibTableLoadData);    
    TblSetLoadDataProcedure(pTable, TitleColumn,  (TableLoadDataFuncPtr)MusicLibTableLoadData);   
    
    TblSetItemFont(pTable, 0, IndexColumn,  boldFont);
    TblSetItemFont(pTable, 0, ArtistColumn, boldFont);
    TblSetItemFont(pTable, 0, TitleColumn,  boldFont);

    TableUpdate(pTable);   
    gpTable = pTable;
}


/***********************************************************************
 *
 * FUNCTION:    TableGetPointRow
 * 
 * DESCRIPTION: Find what row is selected based on coordinates 
 * 
 * PARAMETERS:  pTable dRecords
 * 
 * RETURNED:    
 * 
 * REVISION HISTORY: 
 *
 ***********************************************************************/
UInt16 TableGetPointRow(TableType *pTable, UInt16 yCoord)
{
    RectangleType tableBounds;
    Int16  currRow, lastRow;
    UInt16 rowY, rowHeight;

    TblGetBounds(pTable, &tableBounds);
    rowY = tableBounds.topLeft.y;

    for(currRow = 0, lastRow = TblGetLastUsableRow(pTable); currRow <= lastRow; currRow++)
    {
        rowHeight = TblGetRowHeight(pTable, currRow);
        if(rowY <= yCoord && yCoord < rowY + rowHeight)
            break;
        rowY += rowHeight;
    }
    if(currRow > lastRow) {
    	return -1;
    } else { 
    	return currRow;
    }
}

/***********************************************************************
 *
 * FUNCTION:    TableGet
 * 
 * DESCRIPTION: Get global table
 * 
 * PARAMETERS:   
 * 
 * RETURNED:    TablePtr
 * 
 * REVISION HISTORY: 
 *
 ***********************************************************************/
TablePtr TableGet()
{
    return gpTable;	
}

/***********************************************************************
 *
 * FUNCTION:    TableIsFull
 * 
 * DESCRIPTION: Check if table is full
 * 
 * PARAMETERS:   
 * 
 * RETURNED:    Boolean
 * 
 * REVISION HISTORY: 
 *
 ***********************************************************************/
Boolean TableIsFull()
{
   if(DbGetNonDeletedRecCount() == MAX_TABLE_SIZE) { 
   	  return true;
   } else { 
   	  return false;
   }	  
}


