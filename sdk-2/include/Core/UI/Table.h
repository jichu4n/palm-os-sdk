/***********************************************************************
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
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

extern void TblDrawTable (TablePtr table)
							SYS_TRAP(sysTrapTblDrawTable);

extern void TblRedrawTable (TablePtr table)
							SYS_TRAP(sysTrapTblRedrawTable);

extern void TblEraseTable (TablePtr table)
							SYS_TRAP(sysTrapTblEraseTable);

extern Boolean TblHandleEvent (TablePtr table, EventPtr event)
							SYS_TRAP(sysTrapTblHandleEvent);

extern void TblGetItemBounds (TablePtr table, Word row, Word column,
	RectanglePtr r)
							SYS_TRAP(sysTrapTblGetItemBounds);

extern void TblSelectItem (TablePtr table, Word row, Word column)
							SYS_TRAP(sysTrapTblSelectItem);

extern Word TblGetItemInt (TablePtr table, Word row, Word column)
							SYS_TRAP(sysTrapTblGetItemInt);

extern void TblSetItemInt (TablePtr table, Word row, Word column, Word value)
							SYS_TRAP(sysTrapTblSetItemInt);

extern void TblSetItemPtr (TablePtr table, Word row, Word column, VoidPtr value)
							SYS_TRAP(sysTrapTblSetItemPtr);

extern void TblSetItemStyle (TablePtr table, Word row, Word column, 
	TableItemStyleType type)
							SYS_TRAP(sysTrapTblSetItemStyle);

extern void TblUnhighlightSelection (TablePtr table)
							SYS_TRAP(sysTrapTblUnhighlightSelection);

extern  Boolean TblRowUsable  (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblRowUsable);

extern void TblSetRowUsable (TablePtr table, Word row, Boolean usable)
							SYS_TRAP(sysTrapTblSetRowUsable);

extern  Word TblGetLastUsableRow (TablePtr table)
							SYS_TRAP(sysTrapTblGetLastUsableRow);


extern void TblSetColumnUsable (TablePtr table, Word row, Boolean usable)
							SYS_TRAP(sysTrapTblSetColumnUsable);

extern void TblSetRowSelectable (TablePtr table, Word row, Boolean selectable)
							SYS_TRAP(sysTrapTblSetRowSelectable);

extern Boolean TblRowSelectable (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblRowSelectable);

extern Word TblGetNumberOfRows (TablePtr table)
							SYS_TRAP(sysTrapTblGetNumberOfRows);

extern void TblSetCustomDrawProcedure 
	(TablePtr table, Word column, TableDrawItemFuncPtr drawCallback)
							SYS_TRAP(sysTrapTblSetCustomDrawProcedure);

extern void TblSetLoadDataProcedure 
	(TablePtr table, Word column, TableLoadDataFuncPtr loadDataCallback)
							SYS_TRAP(sysTrapTblSetLoadDataProcedure);

extern void TblSetSaveDataProcedure 
	(TablePtr table, Word column, TableSaveDataFuncPtr saveDataCallback)
							SYS_TRAP(sysTrapTblSetSaveDataProcedure);


extern void TblGetBounds (TablePtr table, RectanglePtr r)
							SYS_TRAP(sysTrapTblGetBounds);

extern void TblSetBounds (TablePtr table, RectanglePtr r)
							SYS_TRAP(sysTrapTblSetBounds);

extern Word TblGetRowHeight (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblGetRowHeight);

extern void TblSetRowHeight (TablePtr table, Word row, Word height)
							SYS_TRAP(sysTrapTblSetRowHeight);

extern Word TblGetColumnWidth (TablePtr table, Word column)
							SYS_TRAP(sysTrapTblGetColumnWidth);

extern void TblSetColumnWidth (TablePtr table, Word column, Word width)
							SYS_TRAP(sysTrapTblSetColumnWidth);

extern Word TblGetColumnSpacing (TablePtr table, Word column)
							SYS_TRAP(sysTrapTblGetColumnSpacing);

extern void TblSetColumnSpacing (TablePtr table, Word column, Word spacing)
							SYS_TRAP(sysTrapTblSetColumnSpacing);

extern Boolean TblFindRowID (TablePtr table, Word id, WordPtr rowP)
							SYS_TRAP(sysTrapTblFindRowID);

extern Boolean TblFindRowData (TablePtr table, DWord data, WordPtr rowP)
							SYS_TRAP(sysTrapTblFindRowData);

extern Word TblGetRowID (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblGetRowID);

extern void TblSetRowID (TablePtr table, Word row, Word id)
							SYS_TRAP(sysTrapTblSetRowID);

extern DWord TblGetRowData (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblGetRowData);

extern void TblSetRowData (TablePtr table, Word row, DWord data)
							SYS_TRAP(sysTrapTblSetRowData);

extern Boolean TblRowInvalid (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblRowInvalid);

extern void TblMarkRowInvalid (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblMarkRowInvalid);

extern void TblMarkTableInvalid (TablePtr table)
							SYS_TRAP(sysTrapTblMarkTableInvalid);

extern Boolean TblGetSelection (TablePtr table, WordPtr rowP, WordPtr columnP)
							SYS_TRAP(sysTrapTblGetSelection);

extern void TblInsertRow (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblInsertRow);
							
extern void TblRemoveRow (TablePtr table, Word row)
							SYS_TRAP(sysTrapTblRemoveRow);

extern void TblReleaseFocus (TablePtr table)
							SYS_TRAP(sysTrapTblReleaseFocus);
							
extern Boolean TblEditing (TablePtr table)
							SYS_TRAP(sysTrapTblEditing);

extern FieldPtr TblGetCurrentField (TablePtr table)
							SYS_TRAP(sysTrapTblGetCurrentField);
							
extern void TblGrabFocus (TablePtr table, Word row, Word column)
							SYS_TRAP(sysTrapTblGrabFocus);

extern  void TblSetColumnEditIndicator (TablePtr table, Word column, 
 				Boolean editIndicator)
							SYS_TRAP(sysTrapTblSetColumnEditIndicator);

extern void TblSetRowStaticHeight (TablePtr table, Word row, 
				Boolean staticHeight)
							SYS_TRAP(sysTrapTblSetRowStaticHeight);

extern void TblHasScrollBar (TablePtr table, Boolean hasScrollBar)
							SYS_TRAP(sysTrapTblHasScrollBar);

#endif //__TABLE_H__
