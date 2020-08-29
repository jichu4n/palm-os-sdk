/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	id3.c
 *
 * Description:
 *	
 *
 ***********************************************************************/
 
#include "id3.h"

/***********************************************************************
 *
 * FUNCTION:    ConvertSynchsafeToIntger
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

static UInt32 ConvertSynchsafeToIntger(UInt32 value)
{
	UInt32 result = 0;
	
	result = value & 0x7F;
	result |= ( value & 0x7F00 ) >> 1;
	result |= ( value & 0x7F0000 ) >> 1;
	result |= ( value & 0x7F000000 ) >> 1;
		
	return result;
}

/***********************************************************************
 *
 * FUNCTION:    TagIsID3v1
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Boolean CheckID3v1(Char* stream, UInt16 streamLen, ID3v1Tag* tag)
{
	if( streamLen < 128 )
		return false;
		
	if( stream[0] == 'T' && stream[1] == 'A' && stream[2] == 'G' )
	{
		MemSet((Char*)tag->title, 31, 0);
		StrNCopy(tag->title,  stream + 3, 30);
		MemSet((Char*)tag->artist, 31, 0);
		StrNCopy(tag->artist, stream + 33, 30);
		MemSet((Char*)tag->album, 31, 0);
		StrNCopy(tag->album,  stream + 63, 30);
		MemSet((Char*)tag->year, 31, 0);
		StrNCopy(tag->year,  stream + 93, 4);
		MemSet((Char*)tag->comment, 31, 0);
		StrNCopy(tag->comment, stream + 97, 30);
		tag->genre = stream[127];
	}
	else
		return false;
		
	return true;
}

/***********************************************************************
 *
 * FUNCTION:    TagIsID3v2
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Boolean CheckID3v2(Char* stream, UInt16 streamLen, UInt32* size)
{
	Char flags;
	UInt32 headerSize;
	Boolean footer;
	
	*size = 0;
	
	if( streamLen < 10 )
		return false;
		
	// Check the first 3 bytes
	if( stream[0] == 'I' && stream[1] == 'D' && stream[2] == '3' )
	{
		// Read the flags
		flags = stream[5];
		footer = (flags & 0x10)?true:false;
		
		headerSize = ((UInt32)(stream[6]) << 24)| ((UInt32)(stream[7]) << 16) | ((UInt32)(stream[8]) << 8) |((UInt32)(stream[9]));
		headerSize = ConvertSynchsafeToIntger(headerSize);
		if( footer ) headerSize += 10;
		
		*size = 10 + headerSize;
	}
	else
		return false;
		
	return true;	
}