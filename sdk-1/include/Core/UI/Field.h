/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     field.h
 * AUTHOR:	 Art Lamb: August 29, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines field structures and routines.
 *
 ***********************************************************************/
#ifndef __FIELD_H__
#define __FIELD_H__

#define maxFieldTextLen	0x7fff

// Maximun number of line the a dynamicly sizing field will expand to.
#define  maxFieldLines 	11


// kind alignment values
#define leftAlign        	0
#define centerAlign      	1
#define rightAlign       	2


#define undoBufferSize 100

typedef enum { undoNone, undoTyping, undoBackspace, undoDelete,
					undoPaste, undoCut } UndoMode;

typedef struct {
	UndoMode		mode;
	Word			start;
	Word			end;
	Word			bufferLen;
	CharPtr		buffer;
	} FieldUndoType;


typedef struct {
	Word usable				:1;	// Set if part of ui 
	Word visible			:1;	// Set if drawn, used internally
	Word editable			:1;	// Set if editable
	Word singleLine		:1;	// Set if only a single line is displayed
	Word hasFocus       	:1;   // Set if the field has the focus
	Word dynamicSize		:1;   // Set if height expands as text is entered
	Word insPtVisible		:1;	// Set if the ins pt is scolled into view
	Word dirty				:1;	// Set if user modified
	Word underlined		:2;	// text underlined mode
	Word justification	:2;	// text alignment
} FieldAttrType;

typedef FieldAttrType * FieldAttrPtr;

typedef struct {
  Word	start;			// position in text string of first char.
  Word	length;			// number of character in the line
} LineInfoType;

typedef LineInfoType * LineInfoPtr;


typedef struct {
	Word					id;
	RectangleType		rect;
	FieldAttrType		attr;
	CharPtr				text;					// pointer to the start of text string 
	VoidHand				textHandle;			// block the contains the text string
	LineInfoPtr			lines;
	Word					textLen;
	Word					textBlockSize;
	Word					maxChars;
	Word					selFirstPos;
	Word					selLastPos;
	Word					insPtXPos;
	Word					insPtYPos;
	FontID				fontID;
} FieldType;


typedef FieldType * FieldPtr;

//---------------------------------------------------------------------
//	Field Functions
//---------------------------------------------------------------------

extern void FldCopy (FieldPtr fld)
							SYS_TRAP(sysTrapFldCopy);

extern void FldCut (FieldPtr fld)
							SYS_TRAP(sysTrapFldCut);

extern void FldDrawField (FieldPtr fld)
							SYS_TRAP(sysTrapFldDrawField);

extern void FldEraseField (FieldPtr fld)
							SYS_TRAP(sysTrapFldEraseField);
 
extern void FldFreeMemory (FieldPtr fld)
							SYS_TRAP(sysTrapFldFreeMemory);

extern void FldGetBounds (FieldPtr fld, RectanglePtr rect)
							SYS_TRAP(sysTrapFldGetBounds);

extern FontID FldGetFont (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetFont);

extern void FldGetSelection (FieldPtr fld, WordPtr startPosition, 
	WordPtr endPosition)
							SYS_TRAP(sysTrapFldGetSelection);

extern Handle FldGetTextHandle (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextHandle);

extern CharPtr FldGetTextPtr (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextPtr);

extern Boolean FldHandleEvent (FieldPtr fld, EventPtr pEvent)
							SYS_TRAP(sysTrapFldHandleEvent);

extern void FldPaste (FieldPtr fld)
							SYS_TRAP(sysTrapFldPaste);

extern void FldRecalculateField (FieldPtr fld, Boolean redraw)
							SYS_TRAP(sysTrapFldRecalculateField);

extern void FldSetBounds (FieldPtr fld, RectanglePtr rect)
							SYS_TRAP(sysTrapFldSetBounds);

extern void FldSetFont (FieldPtr fld, FontID fontID)
							SYS_TRAP(sysTrapFldSetFont);

extern void FldSetText (FieldPtr fld, VoidHand textHandle, Word offset, 
	Word size)
							SYS_TRAP(sysTrapFldSetText);

extern void FldSetTextHandle (FieldPtr fld, Handle textHandle)
							SYS_TRAP(sysTrapFldSetTextHandle);

extern void FldSetTextPtr (FieldPtr fld, CharPtr textP)
							SYS_TRAP(sysTrapFldSetTextPtr);

extern void FldSetUsable (FieldPtr fld, Boolean usable)
							SYS_TRAP(sysTrapFldSetUsable);

extern void FldSetSelection (FieldPtr fld, Word startPosition,
	Word endPosition)
							SYS_TRAP(sysTrapFldSetSelection);

extern void FldGrabFocus (FieldPtr fld)
							SYS_TRAP(sysTrapFldGrabFocus);

extern void FldReleaseFocus (FieldPtr fld)
							SYS_TRAP(sysTrapFldReleaseFocus);


extern Word FldGetInsPtPosition (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetInsPtPosition);

extern void FldSetInsPtPosition (FieldPtr fld, Word pos)
							SYS_TRAP(sysTrapFldSetInsPtPosition);

extern Word FldGetScrollPosition (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetScrollPosition);

extern void FldSetScrollPosition (FieldPtr fld, Word pos)
							SYS_TRAP(sysTrapFldSetScrollPosition);

extern Word FldGetTextLength (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextLength);

extern void FldScrollField (FieldPtr fld, Word linesToScroll, 
	DirectionType direction)
							SYS_TRAP(sysTrapFldScrollField);
							
extern Boolean FldScrollable (FieldPtr fld, DirectionType direction)
							SYS_TRAP(sysTrapFldScrollable);

extern Word FldGetVisibleLines (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetVisibleLines);

extern Word FldGetTextHeight (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextHeight);

extern Word FldCalcFieldHeight (CharPtr chars, Word maxWidth)
							SYS_TRAP(sysTrapFldCalcFieldHeight);

Word FldWordWrap (CharPtr chars, Word maxWidth)
							SYS_TRAP(sysTrapFldWordWrap);

extern void FldCompactText (FieldPtr fld)
							SYS_TRAP(sysTrapFldCompactText);

extern Boolean FldDirty (FieldPtr fld)
							SYS_TRAP(sysTrapFldDirty);

extern void FldSetDirty (FieldPtr fld, Boolean dirty)
							SYS_TRAP(sysTrapFldSetDirty);

extern Word FldGetMaxChars (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetMaxChars);

extern void FldSetMaxChars (FieldPtr fld, Word maxChars)
							SYS_TRAP(sysTrapFldSetMaxChars);

extern Boolean FldInsert (FieldPtr fld, CharPtr insertChars, Word insertLen)
							SYS_TRAP(sysTrapFldInsert);

extern void FldDelete (FieldPtr fld, Word start, Word end)
							SYS_TRAP(sysTrapFldDelete);

extern void FldUndo (FieldPtr fld)
							SYS_TRAP(sysTrapFldUndo);

extern Word FldGetTextAllocatedSize (FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextAllocatedSize);

extern void FldSetTextAllocatedSize (FieldPtr fld, Word allocatedSize)
							SYS_TRAP(sysTrapFldSetTextAllocatedSize);

extern void FldGetAttributes (FieldPtr fld, FieldAttrPtr attrP)
							SYS_TRAP(sysTrapFldGetAttributes);

extern void FldSetAttributes (FieldPtr fld, FieldAttrPtr attrP)
							SYS_TRAP(sysTrapFldSetAttributes);

extern void FldSendChangeNotification (FieldPtr fld)
							SYS_TRAP(sysTrapFldSendChangeNotification);

extern void FldSendHeightChangeNotification (FieldPtr fld, Word pos, 
	Short numLines)
							SYS_TRAP(sysTrapFldSendHeightChangeNotification);

extern Boolean FldMakeFullyVisible (FieldPtr fld)
							SYS_TRAP(sysTrapFldMakeFullyVisible);


#endif // __FIELD_H__



