/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	id3.h
 *
 * Description:
 *	
 *
 *
 ***********************************************************************/

#ifndef ID3_H
#define ID3_H

#include <PalmOS.h>

/************************************************************
 *
 * Structures
 *
 *************************************************************/

typedef struct {
	Char title[31];
	Char artist[31];
	Char album[31];
	Char year[5];
	Char comment[31];
	Char genre;

} ID3v1Tag;

/************************************************************
 *
 * Function Prototypes
 *
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Boolean CheckID3v1(Char* stream, UInt16 streamLen, ID3v1Tag* tag);
Boolean CheckID3v2(Char* stream, UInt16 streamLen, UInt32* size);


#ifdef __cplusplus 
}
#endif

#endif