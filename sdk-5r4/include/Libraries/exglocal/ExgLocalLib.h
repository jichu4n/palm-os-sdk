/******************************************************************************
 *
 * Copyright (c) 2000-2004 PalmSource, Inc. All rights reserved.
 *
 * File: ExgLocalLib.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		Public include file for ExgLocal Library
 *
 *****************************************************************************/

#ifndef __EXG_LOCAL_LIB_H
#define __EXG_LOCAL_LIB_H


#include <PalmTypes.h>
#include <ExgMgr.h>
#include <FileStream.h>


// Name of Exg Local library. This name goes in the last slot of the dispatch table.
// It must match the name of the database, so that the Exchange Manager can map my
// creator ID to a reference number. It must have a creator ID suffix to conform to
// Palm OS naming conventions (to ensure uniqueness) and so that the Exchange Manaager
// can map my creator ID to a reference number on the simulator.
#define exgLocalLibName				"ExgLocal Library-locl"

// Specific scheme for Local exg lib
#define exgLocalScheme				"_local"

// Enum for operations in progress
#define exgLocalOpNone				0
#define exgLocalOpPut				1
#define exgLocalOpAccept			2
#define exgLocalOpGet				3
#define exgLocalOpGetSender		4
typedef UInt8 ExgLocalOpType;

// Structure pointed to by ExgSocketType.socketRef
typedef struct {
	Boolean freeOnDisconnect;	// whether to free this struct when the operation completes
	Boolean noAsk;					// whether to skip exgAskUser launch and ExgDoDialog call
	ExgPreviewInfoType *previewInfoP;	// if specified, object will be previewed only
	FileHand tempFileH;			// temp buffer
	Err err;							// error code to return to outer caller of ExgDisconnect
	ExgLocalOpType op;			// operation in progress, if any
} ExgLocalSocketInfoType;	


#endif  // EXG_LOCAL_LIB_H
