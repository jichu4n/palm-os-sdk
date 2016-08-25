/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		PadUtil.h
 *
 * Description:
 *		Packet Assembly/Disassembly utility definitions
 *
 * History:
 *   	8/11/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __PAD_UTIL_H__
#define __PAD_UTIL_H__

#if EMULATION_LEVEL == 1		// EMULATION_WINDOWS
	
	// Pilot common definitions
	#include "Common.h"

#else	// EMULATION_LEVEL != EMULATION_WINDOWS

	// Pilot common definitions
	#include <Common.h>

#endif	// EMULATION_LEVEL == EMULATION_WINDOWS

#if EMULATION_WINDOWS != 1
	LOGIC ERROR
#endif


// The PmSegmentType structure enables the caller to
// specify the block's data in multiple non-contiguous segments.  For example, when
// sending a record, DesktopLink will specify the function header, the argument wrapper,
// the fixed record info, and the record data as two or more segments(one or more segments
// for the fixed structures and one segment for the record data).  In such an instance,
// it is not practical(and sometimes not possible) to allocate a single data buffer to
// contain all that information contiguously.
//
typedef struct PmSegmentType {
	VoidPtr		dataP;				// ptr to segment data
	Word			dataSize;			// segment data size in bytes
	Boolean		wordAlign;			// if true, this segments data will be sent
											// at an even offset from the start of the
											// block, padding the previous segment's
											// data with a null byte, if necessary
	} PmSegmentType;

typedef PmSegmentType*		PmSegmentPtr;


#ifdef __cplusplus
extern "C" {
#endif

ULong PmAddSegmentSizes(Int segCount, PmSegmentType seg[]);

ULong	PmCopyFromSegments(VoidPtr bufP, Int segCount, PmSegmentType seg[],
			ULong blockOffset, ULong maxSize);


#ifdef __cplusplus 
}
#endif


#endif	// __PAD_UTIL_H__
