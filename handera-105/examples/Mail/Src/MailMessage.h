/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailMessage.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines the massage viewing structures and functions of 
 *   the msg application.
 *
 * History:
 *		June 20, 1996	Created by Art Lamb
 *
 *****************************************************************************/

// Used by message selection routions.
typedef enum { 
	msgSelectSentToLabel,
	msgSelectSentTo,
	msgSelectToLabel,
	msgSelectTo,
	msgSelectFromLabel,
	msgSelectFrom,
	msgSelectCCLabel,
	msgSelectCC,
	msgSelectSubjectLabel,
	msgSelectSubject,
	msgSelectDateLabel,
	msgSelectDate,
	msgSelectBlankLine,
	msgSelectBody,
	msgSelectNone } MsgSelectFieldType;


typedef enum { 
	msgFieldSentTo,
	msgFieldTo,
	msgFieldFrom,
	msgFieldSubject,
	msgFieldCC,
	msgFieldDate,
	msgFieldBlankLine,
	msgFieldBody } MsgFieldType;


extern void MsgDraw (void);

extern void MsgErase (void);

extern void MsgGetScrollValues (UInt16 * textHeightP, UInt16 * pageHeightP, UInt16 * topLineP);

extern void MsgScroll (Int16 linesToScroll, WinDirectionType direction);

extern Boolean MsgSearch (MailDBRecordPtr recordP, Char * strToFind, UInt16 * fieldNumP, UInt16 * posP, UInt16 *matchLenP);

extern void MsgSelect (Int16 x, Int16 y);

extern void MsgCopy (void);

extern void MsgSelectAll (void);

extern void MsgChangeFont (void);
