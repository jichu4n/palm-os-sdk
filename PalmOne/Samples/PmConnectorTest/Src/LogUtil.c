/*
 * LogUtil.c
 *
 * Log utility functions for PmConnectorTest
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */

/** @ingroup PmConnectorTest
 *
**/

/** @file LogUtil.h
 * This file contains log utility functions for PmConnectorTest
**/

/** @name
 *
**/
/*@{*/

#include <PalmOS.h>
#include <unix_stdarg.h>

#include "PmConnectorTest_Rsc.h"

#define kMaxSizeLog				4096

void PrintToField(const char *str);

/*
 * FUNCTION: PrintToField
 *
 * DESCRIPTION: Print text in MainOutField of MainForm
 *
 * PARAMETERS: str
 * 
 * CREDIT: From PalmSource sample code
 *
 */
void PrintToField(const char *str)
{
	static UInt16 gLen = 0;
	UInt16		  fldTxtLen;
	va_list		  args;
	Char		  fldTxt[128];
	
	FormPtr		  frmP = FrmGetFormPtr(MainForm);
	FieldPtr	  fldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainOutField));
	Char *		  fldTxtP = FldGetTextPtr(fldP);

	// Allocate field memory
	if (!fldTxtP) {
		fldTxtP = MemPtrNew(kMaxSizeLog);
		gLen = FldGetVisibleLines(fldP);
		MemSet(fldTxtP, gLen, '\n');
		fldTxtP[gLen] = '\0';
	}

	// Add string to field
	if (str) {
		// Format message
		va_start(args, str);
		StrVPrintF(fldTxt, str, args);
		va_end(args);
		
		// When out of space, keep last half of the log and move it to the top
		fldTxtLen = (UInt16) StrLen(fldTxt) + 1;
		if (gLen + fldTxtLen >= kMaxSizeLog) {
			gLen >>= 1;
			MemMove(fldTxtP, fldTxtP + gLen + (gLen & 1), gLen );
		}
	
		// Append carriage return and update length
		StrPrintF(fldTxtP + gLen, "\n%s", fldTxt);
		gLen += fldTxtLen;
		FldSetTextPtr(fldP, fldTxtP);
	}
	
	// Redraw field and scroll
	FldRecalculateField(fldP, false);
	FldScrollField(fldP, (UInt16) kMaxSizeLog, winDown);
	FldDrawField(fldP);
}

/*@}*/
