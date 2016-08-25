/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
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
enum justifications {leftAlign, centerAlign, rightAlign};
typedef enum justifications JustificationType;


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
	Word autoShift			:1;	// Set if auto case shift
	Word hasScrollBar		:1;	// Set if the field has a scroll bar
	Word numeric			:1;	// Set if numeric, digits and secimal separator only
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

#ifdef __cplusplus
extern "C" {
#endif

extern void FldCopy (const FieldPtr fld)
							SYS_TRAP(sysTrapFldCopy);

extern void FldCut (const FieldPtr fld)
							SYS_TRAP(sysTrapFldCut);

extern void FldDrawField (const FieldPtr fld)
							SYS_TRAP(sysTrapFldDrawField);

extern void FldEraseField (const FieldPtr fld)
							SYS_TRAP(sysTrapFldEraseField);
 
extern void FldFreeMemory (const FieldPtr fld)
							SYS_TRAP(sysTrapFldFreeMemory);

extern void FldGetBounds (const FieldPtr fld, const RectanglePtr rect)
							SYS_TRAP(sysTrapFldGetBounds);

extern FontID FldGetFont (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetFont);

extern void FldGetSelection (const FieldPtr fld, const WordPtr startPosition, 
	const WordPtr endPosition)
							SYS_TRAP(sysTrapFldGetSelection);

extern Handle FldGetTextHandle (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextHandle);

extern CharPtr FldGetTextPtr (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextPtr);

extern Boolean FldHandleEvent (const FieldPtr fld, const EventPtr pEvent)
							SYS_TRAP(sysTrapFldHandleEvent);

extern void FldPaste (const FieldPtr fld)
							SYS_TRAP(sysTrapFldPaste);

extern void FldRecalculateField (const FieldPtr fld, const Boolean redraw)
							SYS_TRAP(sysTrapFldRecalculateField);

extern void FldSetBounds (const FieldPtr fld, const RectanglePtr rect)
							SYS_TRAP(sysTrapFldSetBounds);

extern void FldSetFont (const FieldPtr fld, const FontID fontID)
							SYS_TRAP(sysTrapFldSetFont);

extern void FldSetText (const FieldPtr fld, const VoidHand textHandle, 
	const Word offset, const Word size)
							SYS_TRAP(sysTrapFldSetText);

extern void FldSetTextHandle (const FieldPtr fld, const Handle textHandle)
							SYS_TRAP(sysTrapFldSetTextHandle);

extern void FldSetTextPtr (const FieldPtr fld, const CharPtr textP)
							SYS_TRAP(sysTrapFldSetTextPtr);

extern void FldSetUsable (const FieldPtr fld, const Boolean usable)
							SYS_TRAP(sysTrapFldSetUsable);

extern void FldSetSelection (const FieldPtr fld, const Word startPosition,
	const Word endPosition)
							SYS_TRAP(sysTrapFldSetSelection);

extern void FldGrabFocus (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGrabFocus);

extern void FldReleaseFocus (const FieldPtr fld)
							SYS_TRAP(sysTrapFldReleaseFocus);


extern Word FldGetInsPtPosition (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetInsPtPosition);

extern void FldSetInsPtPosition (const FieldPtr fld, const Word pos)
							SYS_TRAP(sysTrapFldSetInsPtPosition);

extern void FldSetInsertionPoint (const FieldPtr fld, const Word pos)
							SYS_TRAP(sysTrapFldSetInsertionPoint);

extern Word FldGetScrollPosition (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetScrollPosition);

extern void FldSetScrollPosition (const FieldPtr fld, Word pos)
							SYS_TRAP(sysTrapFldSetScrollPosition);
							
extern void FldGetScrollValues (const FieldPtr fld, const WordPtr scrollPosP, 
	const WordPtr textHeightP, const WordPtr fieldHeightP)
							SYS_TRAP(sysTrapFldGetScrollValues);


extern Word FldGetTextLength (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextLength);

extern void FldScrollField (const FieldPtr fld, Word linesToScroll, 
	const DirectionType direction)
							SYS_TRAP(sysTrapFldScrollField);
							
extern Boolean FldScrollable (const FieldPtr fld, 
	const DirectionType direction)
							SYS_TRAP(sysTrapFldScrollable);

extern Word FldGetVisibleLines (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetVisibleLines);

extern Word FldGetTextHeight (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextHeight);

extern Word FldCalcFieldHeight (const CharPtr chars, const Word maxWidth)
							SYS_TRAP(sysTrapFldCalcFieldHeight);

Word FldWordWrap (const CharPtr chars, const Word maxWidth)
							SYS_TRAP(sysTrapFldWordWrap);

extern void FldCompactText (const FieldPtr fld)
							SYS_TRAP(sysTrapFldCompactText);

extern Boolean FldDirty (const FieldPtr fld)
							SYS_TRAP(sysTrapFldDirty);

extern void FldSetDirty (const FieldPtr fld, const Boolean dirty)
							SYS_TRAP(sysTrapFldSetDirty);

extern Word FldGetMaxChars (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetMaxChars);

extern void FldSetMaxChars (const FieldPtr fld, const Word maxChars)
							SYS_TRAP(sysTrapFldSetMaxChars);

extern Boolean FldInsert (const FieldPtr fld, const CharPtr insertChars, 
	const Word insertLen)
							SYS_TRAP(sysTrapFldInsert);

extern void FldDelete (const FieldPtr fld, const Word start, const Word end)
							SYS_TRAP(sysTrapFldDelete);

extern void FldUndo (const FieldPtr fld)
							SYS_TRAP(sysTrapFldUndo);

extern Word FldGetTextAllocatedSize (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetTextAllocatedSize);

extern void FldSetTextAllocatedSize (const FieldPtr fld, 
	const Word allocatedSize)
							SYS_TRAP(sysTrapFldSetTextAllocatedSize);

extern void FldGetAttributes (const FieldPtr fld, const FieldAttrPtr attrP)
							SYS_TRAP(sysTrapFldGetAttributes);

extern void FldSetAttributes (const FieldPtr fld, const FieldAttrPtr attrP)
							SYS_TRAP(sysTrapFldSetAttributes);

extern void FldSendChangeNotification (const FieldPtr fld)
							SYS_TRAP(sysTrapFldSendChangeNotification);

extern void FldSendHeightChangeNotification (const FieldPtr fld, 
	const Word pos, const Short numLines)
							SYS_TRAP(sysTrapFldSendHeightChangeNotification);

extern Boolean FldMakeFullyVisible (const FieldPtr fld)
							SYS_TRAP(sysTrapFldMakeFullyVisible);

extern Word FldGetNumberOfBlankLines (const FieldPtr fld)
							SYS_TRAP(sysTrapFldGetNumberOfBlankLines);

extern FieldPtr FldNewField (VoidPtr *formPP, const Word id, 
	const Word x, const Word y, const Word width, const Word height, 
	const FontID font, DWord maxChars, Boolean editable, Boolean underlined, 
	Boolean singleLine, Boolean dynamicSize, JustificationType justification, 
	Boolean autoShift, Boolean hasScrollBar, Boolean numeric)
							SYS_TRAP(sysTrapFldNewField);


#ifdef __cplusplus 
}
#endif

#endif // __FIELD_H__



