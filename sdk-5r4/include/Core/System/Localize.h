/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: Localize.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		Functions to localize data.
 *
 *****************************************************************************/

#ifndef __LOCALIZE_H__
#define __LOCALIZE_H__


// Include elementary types
#include <PalmTypes.h>					// Basic types
#include <CoreTraps.h>				// Trap Numbers.

// The number format (thousands separator and decimal point).  This defines
// how numbers are formatted and not neccessarily currency numbers (i.e. Switzerland).
typedef enum {
	nfCommaPeriod,
	nfPeriodComma,
	nfSpaceComma,
	nfApostrophePeriod,
	nfApostropheComma
	} NumberFormatType;

	

#ifdef __cplusplus
extern "C" {
#endif


void 		LocGetNumberSeparators(NumberFormatType numberFormat, 
				Char *thousandSeparator, Char *decimalSeparator)
							SYS_TRAP(sysTrapLocGetNumberSeparators);



#ifdef __cplusplus 
}
#endif


#endif	// __LOCALIZE_H__
