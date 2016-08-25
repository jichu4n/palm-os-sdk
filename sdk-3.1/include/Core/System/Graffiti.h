/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		Graffiti.h
 *
 * Description:
 *		Header for the Graffiti interface
 *
 * History:
 *   	6/30  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __GRAFFITI_H__
#define __GRAFFITI_H__

#include <Common.h>

#define grfNoShortCut	0xffff		// Index which isn't a shortcut

/*------------------------------------------------------------------------------
 * Match info structure. Returned by GrfMatch and GrfMatchGlyph  
 *-----------------------------------------------------------------------------*/
typedef struct {
	Byte	glyphID;							/* glyph ID of this match */
	Byte	unCertainty;	            /* unCertainty of this match (0 most certain) */
	} GrfMatchType;	


#define grfMaxMatches	4
typedef struct {
	Word				numMatches;	      /* number of matches returned in this structure */
	GrfMatchType 	match[grfMaxMatches];	
	} GrfMatchInfoType;
typedef	GrfMatchInfoType*	GrfMatchInfoPtr;

#define grfNameLength	8				// eight letters possible (don't forget CR)
//#define grfTextLength	56				// Use <SysEvtPrv.h>


//----------------------------------------------------------------------------
// Escape codes preceding special sequences in the dictionary or macros
//----------------------------------------------------------------------------
// In dictionary or macros preceding virtual key event sequences. These are always 
// 13 byte sequences that have ASCII encoded values for the ascii code, keyCode,
//   and modifiers: 
//   grfVirtualSequence, ascii,   keyCode,  modifiers.
//         1 byte        4 bytes   4 bytes   4 bytes          
#define	grfVirtualSequence	0x01

// In dictionary to tell us about temp shift state changes.
#define	grfShiftSequence		0x02

// In dictionary/macros to hide special features
#define	grfSpecialSequence	0x03

// Char indicating a seqeunce of characters to expand.
#define	grfExpansionSequence	'@'

// Chars indicating what to expand into
#define	expandDateChar				'D'
#define	expandTimeChar				'T'
#define	expandStampChar			'S'	//	This follows 'D' or 'T' for the sake
													//	of the mnemonic name.

#define	shortcutBinaryDataFlag	0x01


// Determine if a string has a sequence
#define HasVirtualSequence(s)		(s[0] == grfVirtualSequence)
#define HasSpecialSequence(s)		(s[0] == grfSpecialSequence)
#define HasExpansionSequence(s)	(s[0] == grfExpansionSequence)



/*------------------------------------------------------------------------------
 * Temp shift states, returned by GrfGetState
 *-----------------------------------------------------------------------------*/
#define	grfTempShiftPunctuation			1
#define	grfTempShiftExtended				2
#define	grfTempShiftUpper					3
#define	grfTempShiftLower					4



/************************************************************
 * Graffiti result codes
 *************************************************************/
#define	grfErrBadParam						(grfErrorClass | 1)
#define	grfErrPointBufferFull			(grfErrorClass | 2)
#define	grfErrNoGlyphTable				(grfErrorClass | 3)
#define	grfErrNoDictionary				(grfErrorClass | 4)
#define	grfErrNoMapping					(grfErrorClass | 5)
#define	grfErrMacroNotFound				(grfErrorClass | 6)
#define	grfErrDepthTooDeep				(grfErrorClass | 7)
#define	grfErrMacroPtrTooSmall			(grfErrorClass | 8)
#define	grfErrNoMacros						(grfErrorClass | 9)

#define	grfErrMacroIncomplete			(grfErrorClass | 129)  // (grfWarningOffset+1)
#define	grfErrBranchNotFound				(grfErrorClass | 130)  // (grfWarningOffset+2)


/************************************************************
 * Graffiti interface procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {  
#endif


//-----------------------------------------------------------------
// High Level Calls
//------------------------------------------------------------------
Err		GrfInit (void)
				SYS_TRAP(sysTrapGrfInit);

Err		GrfFree (void)
				SYS_TRAP(sysTrapGrfFree);

Err		GrfProcessStroke(PointType* startPtP, PointType* endPtP,
					Boolean upShift)
				SYS_TRAP(sysTrapGrfProcessStroke);

Err		GrfFieldChange(Boolean resetState, UIntPtr characterToDelete)
				SYS_TRAP(sysTrapGrfFieldChange);

Err		GrfGetState(Boolean* capsLockP, Boolean* numLockP, 
					WordPtr tempShiftP, Boolean* autoShiftedP)
				SYS_TRAP(sysTrapGrfGetState);

Err		GrfSetState(Boolean capsLock, Boolean numLock, 
					Boolean upperShift)
				SYS_TRAP(sysTrapGrfSetState);


//-----------------------------------------------------------------
// Mid Level Calls
//------------------------------------------------------------------

Err 		GrfFlushPoints (void)
				SYS_TRAP(sysTrapGrfFlushPoints);

Err 		GrfAddPoint (PointType* pt)
				SYS_TRAP(sysTrapGrfAddPoint);

Err 		GrfInitState(void)
				SYS_TRAP(sysTrapGrfInitState);

Err 		GrfCleanState(void)
				SYS_TRAP(sysTrapGrfCleanState);

Err 		GrfMatch (WordPtr flagsP, void* dataPtrP, WordPtr dataLenP,
					WordPtr uncertainLenP, GrfMatchInfoPtr matchInfoP)
				SYS_TRAP(sysTrapGrfMatch);

Err 		GrfGetMacro(CharPtr nameP, BytePtr macroDataP,
					WordPtr	dataLenP)
				SYS_TRAP(sysTrapGrfGetMacro);

Err 		GrfGetAndExpandMacro(CharPtr nameP, BytePtr macroDataP,
					WordPtr	dataLenP)
				SYS_TRAP(sysTrapGrfGetAndExpandMacro);


//-----------------------------------------------------------------
// Low Level Calls
//------------------------------------------------------------------
Err 		GrfFilterPoints (void)
				SYS_TRAP(sysTrapGrfFilterPoints);

Err 		GrfGetNumPoints(WordPtr numPtsP)
				SYS_TRAP(sysTrapGrfGetNumPoints);

Err 		GrfGetPoint(Word index, PointType* pointP)
				SYS_TRAP(sysTrapGrfGetPoint);

Err 		GrfFindBranch(Word flags)
				SYS_TRAP(sysTrapGrfFindBranch);

Err 		GrfMatchGlyph (GrfMatchInfoPtr matchInfoP,
					Word maxUnCertainty, Word maxMatches)
				SYS_TRAP(sysTrapGrfMatchGlyph);

Err 		GrfGetGlyphMapping (Word glyphID, WordPtr flagsP,
					void* dataPtrP, WordPtr dataLenP, WordPtr uncertainLenP)
				SYS_TRAP(sysTrapGrfGetGlyphMapping);

Err 		GrfGetMacroName(Word index, CharPtr nameP)
				SYS_TRAP(sysTrapGrfGetMacroName);

Err 		GrfDeleteMacro(Word index)
				SYS_TRAP(sysTrapGrfDeleteMacro);

Err 		GrfAddMacro(CharPtr nameP, BytePtr macroDataP,
					Word dataLen)
				SYS_TRAP(sysTrapGrfAddMacro);




#ifdef __cplusplus
}
#endif


#endif //__SYSEVTMGR_H__



