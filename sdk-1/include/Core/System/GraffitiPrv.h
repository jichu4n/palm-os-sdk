/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		GraffitiPRV.h
 *
 * Description:
 *		Private Header for the Graffiti interface
 *
 * History:
 *   	6/30  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef	NON_PORTABLE
#ifndef __GRAFFITIPRV_H__
#define __GRAFFITIPRV_H__



#define	DP3MachineMacintosh
#include <P3API.h>


/************************************************************
 * Graffiti Constants
 *************************************************************/
// Bytes that follow the grfSpecialSequence byte in Graffiti dictionary
//  or macros. These are used for special test code and debugging
#define	grfSpecialToggleSplitMode	0x01		// Toggle split mode
#define	grfSpecialNoAutoOff			0x02		// Disable auto-off
#define	grfSpecialShowUserInfo		0x03		//	Display user information
#define	grfSpecialDeleteUserInfo	0x04		//	Delete user name & id
#define	grfSpecialSystemBuildDate	0x05		//	Show System Build date
#define	grfSpecialBatteryKind		0x06		//	Change battery kind

// Max # of keys returned from dictionary or macros 
#define	grfMaxKeys				100				

// Resource type and ID of macros resource
#define	grfMacrosResT			'macr'
#define	grfMacrosResID			10000
#define	grfMacrosMinSize		10					// min size of macros resource

// Version of Graffiti Macros database
#define	grfMacrosDBVersion	0					// version word


// Constant used to translate internal Graffiti Warnings into Pilot error codes
#define	grfWarningOffset		128


/************************************************************
 * Graffiti Globals
 *************************************************************/
#define grfMaxMacroName 	32
typedef struct GrfGlobalsType {
	TP3Data				p3Data;
	TP3DictState 		state;
	Int					uncertain;
	Word					glyphFlags;
	Word					shiftState;				// temp shift state
	
	SWord					macroNameLen;
	Byte					macroName[grfMaxMacroName];
	Byte					lastChar;
	
	ULong					globalsSize;
	
	
	// Various flags  
	Word					splitMode : 1;
	Word					restoreCaps : 1;
	Word					inMacro : 1;
	Word					wasAutoShifted : 1;
	
	// Glyph ID assigned to the up-stroke
	Byte					upShiftGlyphID;
	
	// Handle to P3 private globals
	Handle				globalsH;
	
	// Handle to template file
	Handle				templateH;
	
	// Handle to dictionary
	Handle				dictH;
	
	// Handle to macros resource
	Handle				macrosH;
	// Database acccess pointer to macros database
	DmOpenRef			macrosDbP;
	
	} GrfGlobalsType;
	
typedef GrfGlobalsType*	GrfGlobalsPtr;


#endif //__GRAFFITIPRV_H__
#endif // NON_PORTABLE



