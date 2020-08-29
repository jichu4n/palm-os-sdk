 /****************************************************************************
 *
 * Copyright (c) 2007 Palm, Inc. All rights reserved.
 *
 * File:   LogUtil.c
 * Credit: LogPrintF function modelled after PalmSource sample code
 *  
 *****************************************************************************/
#include <PalmOS.h>
#include <unix_stdarg.h>
#include "AppResources.h"
#include "LogUtil.h"

#define kMaxSizeLog				8192

Char *logstr;

/***********************************************************************
 *
 * FUNCTION:    LogUtilUninit
 *
 * DESCRIPTION: Uninitialize log
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
void LogUtilUninit() 
{
   if(logstr) {
      MemPtrFree(logstr);
      logstr = NULL;
   }
}


/***********************************************************************
 *
 * FUNCTION:    LogPrintF
 *
 * DESCRIPTION: Add text to the log field, max 128 chars
 *
 * PARAMETERS:  str
 *
 * RETURNED:    
 * 
 * CREDIT:      Function modelled after PalmSource sample code
 * 
 ***********************************************************************/
void LogPrintF(const char *newstr, ...)
{ 
	static UInt16 	gLength = 0;
	UInt16			length;
	va_list			args;
	Char			tmp[128];
	FieldPtr		pField;
	FormPtr			pForm = FrmGetFormPtr(MainForm);
	
	if(!pForm) {
       return;
	}

	pField = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MainLogField));
    logstr = FldGetTextPtr(pField);
    
	if (!logstr) { // The first time, allocate field with max field size
		logstr = MemPtrNew(kMaxSizeLog);
		gLength = FldGetVisibleLines(pField);
		MemSet(logstr, gLength, '\n');
		logstr[gLength] = '\0';
	}

	if (newstr) { // Format string and add to log field
		va_start(args, newstr);
		StrVPrintF(tmp, newstr, args);
		va_end(args);
		
		// When not enough room, keep only last half of conversation in field
		length = (UInt16) StrLen(tmp) + 1;
		if (gLength + length >= kMaxSizeLog) {
			gLength >>= 1;
			MemMove(logstr, logstr + gLength + (gLength & 1), gLength );
		}
	
		// Add newline and update length
		StrPrintF(logstr + gLength, "\n%s", tmp);
		gLength += length;
		FldSetTextPtr(pField, logstr);
	}
	
	// Redraw field and scroll to the bottom
	FldRecalculateField(pField, false);
	FldScrollField(pField, (UInt16) kMaxSizeLog, winDown);
	FldDrawField(pField);
}
