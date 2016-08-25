/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     field.h
 * AUTHOR:	 Art Lamb: September 1, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines table structures and routines.
 *
 ***********************************************************************/
#ifndef __TABLE_H__
#define __TABLE_H__

//-------------------------------------------------------------------
// Table structures
//-------------------------------------------------------------------

#define tableDefaultColumnSpacing 	1
#define tableNoteIndicatorWidth		7
#define tableNoteIndicatorHeight		11
#define tableMaxTextItemSize 			255	// does not incude terminating null

// Display style of a table item
//
enum tableItemStyles { checkboxTableItem, 
                       customTableItem, 
                       dateTableItem, 
                       labelTableItem,
                       numericTableItem,
                       popupTriggerTableItem,
                       textTableItem,
                       textWithNoteTableItem,
                       timeTableItem,
                       narrowTextTableItem
                       };
typedef enum tableItemStyles TableItemStyleType;


typedef struct {
	TableItemStyleType	itemType;		
   FontID					fontID;		// font for drawing text
	Word						intValue;
	CharPtr   				ptr;
} TableItemType;
typedef TableItemType * TableItemPtr;


// Draw item callback routine prototype, used only by customTableItem.
typedef void TableDrawItemFuncType  
		(VoidPtr table, Word row, Word column, RectanglePtr bounds);

typedef TableDrawItemFuncType * TableDrawItemFuncPtr;


// Load data callback routine prototype
typedef Err TableLoadDataFuncType 
		(VoidPtr table, Word row, Word column, Boolean editable, 
		VoidHand * dataH, WordPtr dataOffset, WordPtr dataSize, FieldPtr fld);

typedef TableLoadDataFuncType * TableLoadDataFuncPtr;


// Save data callback routine prototype
typedef	Boolean TableSaveDataFuncType
		(VoidPtr table, Word row, Word column);

typedef TableSaveDataFuncType * TableSaveDataFuncPtr;

typedef struct {
	Word							width;					// width in pixels
	Word							unused 			: 6;
	Word							editIndicator	: 1;
	Word							usable 			: 1;
	Word							spacing;					// space after column
	TableDrawItemFuncPtr		drawCallback;
	TableLoadDataFuncPtr		loadDataCallback;
	TableSaveDataFuncPtr		saveDataCallback;

} TableColumnAttrType;


typedef struct {
	Word							id;
	Word							height;					// row height in pixels
	DWord							data;

	Word							reserved1		: 7;
	Word							usable			: 1;
	Word							reserved2		: 5;
	Word							invalid			: 1;	// true if redraw needed
	Word 							staticHeight	: 1;  // Set if height does not expands as text is entered
	Word							selectable		: 1;

	Word							reserved3;

} TableRowAttrType;


typedef struct {
	Word 							visible:1;			// Set if drawn, used internally
   Word 							editable:1;			// Set if editable
	Word 							editing:1;			// Set if in edit mode
	Word 							selected:1;			// Set if the current item is selected
	Word							hasScrollBar:1;	// Set if the table has a scroll bar
} TableAttrType;


typedef struct {
	Word							id;
   RectangleType				bounds;
   TableAttrType				attr;
	Word							numColumns;
	Word							numRows;
	Word							currentRow;
	Word							currentColumn;
	Word							topRow;
	TableColumnAttrType *	columnAttrs;
   TableRowAttrType *      rowAttrs;
	TableItemPtr				items;
	FieldType					currentField;
} TableType;

typedef TableType * TablePtr;


//-------------------------------------------------------------------
// Table routines
//-------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

extern void TblDrawTable (const TablePtr table)
							SYS_TRAP(sysTrapTblDrawTable);

extern void TblRedrawTable (const TablePtr table)
							SYS_TRAP(sysTrapTblRedrawTable);

extern void TblEraseTable (const TablePtr table)
							SYS_TRAP(sysTrapTblEraseTable);

extern Boolean TblHandleEvent (const TablePtr table, const EventPtr event)
							SYS_TRAP(sysTrapTblHandleEvent);

extern void TblGetItemBounds (const TablePtr table, const Word row, 
	const Word column, const RectanglePtr r)
							SYS_TRAP(sysTrapTblGetItemBounds);

extern void TblSelectItem (const TablePtr table, const Word row, 
	const Word column)
							SYS_TRAP(sysTrapTblSelectItem);

extern Word TblGetItemInt (const TablePtr table, const Word row,
	const Word column)
							SYS_TRAP(sysTrapTblGetItemInt);

extern void TblSetItemInt (const TablePtr table, const Word row, 
	const Word column, const Word value)
							SYS_TRAP(sysTrapTblSetItemInt);

extern void TblSetItemPtr (const TablePtr table, const Word row, 
	const Word column, const VoidPtr value)
							SYS_TRAP(sysTrapTblSetItemPtr);

extern void TblSetItemStyle (const TablePtr table, const Word row,
	const Word column, const TableItemStyleType type)
							SYS_TRAP(sysTrapTblSetItemStyle);

extern void TblUnhighlightSelection (const TablePtr table)
							SYS_TRAP(sysTrapTblUnhighlightSelection);

extern  Boolean TblRowUsable  (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblRowUsable);

extern void TblSetRowUsable (const TablePtr table, const Word row, 
	const Boolean usable)
							SYS_TRAP(sysTrapTblSetRowUsable);

extern  Word TblGetLastUsableRow (const TablePtr table)
							SYS_TRAP(sysTrapTblGetLastUsableRow);


extern void TblSetColumnUsable (const TablePtr table, const Word row, 
	const Boolean usable)
							SYS_TRAP(sysTrapTblSetColumnUsable);

extern void TblSetRowSelectable (const TablePtr table, const Word row, 
	const Boolean selectable)
							SYS_TRAP(sysTrapTblSetRowSelectable);

extern Boolean TblRowSelectable (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblRowSelectable);

extern Word TblGetNumberOfRows (const TablePtr table)
							SYS_TRAP(sysTrapTblGetNumberOfRows);

extern void TblSetCustomDrawProcedure (const TablePtr table, const Word column,
	const TableDrawItemFuncPtr drawCallback)
							SYS_TRAP(sysTrapTblSetCustomDrawProcedure);

extern void TblSetLoadDataProcedure (const TablePtr table, const Word column, 
	const TableLoadDataFuncPtr loadDataCallback)
							SYS_TRAP(sysTrapTblSetLoadDataProcedure);

extern void TblSetSaveDataProcedure (const TablePtr table, const Word column, 
	const TableSaveDataFuncPtr saveDataCallback)
							SYS_TRAP(sysTrapTblSetSaveDataProcedure);


extern void TblGetBounds (const TablePtr table, const RectanglePtr r)
							SYS_TRAP(sysTrapTblGetBounds);

extern void TblSetBounds (const TablePtr table, const RectanglePtr r)
							SYS_TRAP(sysTrapTblSetBounds);

extern Word TblGetRowHeight (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblGetRowHeight);

extern void TblSetRowHeight (const TablePtr table, const Word row, 
	const Word height)
							SYS_TRAP(sysTrapTblSetRowHeight);

extern Word TblGetColumnWidth (const TablePtr table, const Word column)
							SYS_TRAP(sysTrapTblGetColumnWidth);

extern void TblSetColumnWidth (const TablePtr table, const Word column, 
	const Word width)
							SYS_TRAP(sysTrapTblSetColumnWidth);

extern Word TblGetColumnSpacing (const TablePtr table, const Word column)
							SYS_TRAP(sysTrapTblGetColumnSpacing);

extern void TblSetColumnSpacing (const TablePtr table, const Word column, 
	const Word spacing)
							SYS_TRAP(sysTrapTblSetColumnSpacing);

extern Boolean TblFindRowID (const TablePtr table, const Word id, 
	const WordPtr rowP)
							SYS_TRAP(sysTrapTblFindRowID);

extern Boolean TblFindRowData (const TablePtr table, const DWord data, 
	const WordPtr rowP)
							SYS_TRAP(sysTrapTblFindRowData);

extern Word TblGetRowID (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblGetRowID);

extern void TblSetRowID (const TablePtr table, const Word row, const Word id)
							SYS_TRAP(sysTrapTblSetRowID);

extern DWord TblGetRowData (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblGetRowData);

extern void TblSetRowData (const TablePtr table, const Word row, 
	const DWord data)
							SYS_TRAP(sysTrapTblSetRowData);

extern Boolean TblRowInvalid (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblRowInvalid);

extern void TblMarkRowInvalid (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblMarkRowInvalid);

extern void TblMarkTableInvalid (const TablePtr table)
							SYS_TRAP(sysTrapTblMarkTableInvalid);

extern Boolean TblGetSelection (const TablePtr table, const WordPtr rowP, 
	const WordPtr columnP)
							SYS_TRAP(sysTrapTblGetSelection);

extern void TblInsertRow (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblInsertRow);
							
extern void TblRemoveRow (const TablePtr table, const Word row)
							SYS_TRAP(sysTrapTblRemoveRow);

extern void TblReleaseFocus (const TablePtr table)
							SYS_TRAP(sysTrapTblReleaseFocus);
							
extern Boolean TblEditing (const TablePtr table)
							SYS_TRAP(sysTrapTblEditing);

extern FieldPtr TblGetCurrentField (const TablePtr table)
							SYS_TRAP(sysTrapTblGetCurrentField);
							
extern void TblGrabFocus (const TablePtr table, const Word row, 
	const Word column)
							SYS_TRAP(sysTrapTblGrabFocus);

extern  void TblSetColumnEditIndicator (const TablePtr table, const Word column, 
 				const Boolean editIndicator)
							SYS_TRAP(sysTrapTblSetColumnEditIndicator);

extern void TblSetRowStaticHeight (const TablePtr table, const Word row, 
				const Boolean staticHeight)
							SYS_TRAP(sysTrapTblSetRowStaticHeight);

extern void TblHasScrollBar (const TablePtr table, const Boolean hasScrollBar)
							SYS_TRAP(sysTrapTblHasScrollBar);

extern FontID TblGetItemFont (TablePtr table, Word row, Word column)
							SYS_TRAP(sysTrapTblGetItemFont);

extern  void TblSetItemFont (TablePtr table, Word row, Word column, FontID fontID)
							SYS_TRAP(sysTrapTblSetItemFont);

#ifdef __cplusplus 
}
#endif

#endif //__TABLE_H__
