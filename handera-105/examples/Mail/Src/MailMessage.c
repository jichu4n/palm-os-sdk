/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailMessage.c
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This is the Mail application's main module.
 *
 * History:
 *		June 19, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#include <PalmOS.h>

#include <PalmUtils.h>

#include "Mail.h"


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define tabChrWidth				20

#define maxCopyLength			1000


/***********************************************************************
 *
 *	Internal Structutes
 *
 ***********************************************************************/

typedef struct {
	UInt16					tabStop;
	UInt16					fieldTextPos;
	Char *					fieldText;
	MsgFieldType			fieldType;
	MsgSelectFieldType	selectStartField;
	MsgSelectFieldType	selectEndField;
	UInt8						reserved1;
	UInt16					selectStartPos;
	UInt16					selectEndPos;
	RectangleType			bounds;
	MailDBRecordType		record;
	MemHandle					recordH;
	Char						dateStr [timeStringLength + timeStringLength + 1];
	Char						toFriendlyName [maxFriendlyNameLen];
	Char						fromFriendlyName [maxFriendlyNameLen];
	UInt8						reserved2;
} MessegeInfoType;

typedef MessegeInfoType * MessegeInfoPtr;


/***********************************************************************
 *
 *	Internal Routines
 *
 ***********************************************************************/

static Char * MsgGetFieldPtr (MessegeInfoPtr msg);


/***********************************************************************
 *
 * FUNCTION:    MsgInit
 *
 * DESCRIPTION: This routine initialize the structure used to draw and
 *              scroll a mesage.
 *
 * PARAMETERS:  msg  - structure to initilize
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static void MsgInit (MessegeInfoPtr msg, MsgFieldType	fieldType, 
	UInt16 fieldTextPos)
{
	UInt16	len;
	UInt16	objIndex;
	FormPtr	frm;
	FontID	oldFont;
	UInt16	tabStop;
	int		i;
	static UInt16 labelIDs[] = { toStrId, fromStrId, subjectStrId, ccStrId, dateStrId };
	
	// Get the tab stop for the start of the header text.
	// DOLATER kwk - figure out if it's safe to use static labelIDs here,
	// since they rely on globals.
	oldFont = FntSetFont (MessageFont);
	tabStop = 0;
	
	for (i = 0; i < sizeof (labelIDs) / sizeof (labelIDs[0]); i += 1)
		{
		MemHandle resH;
		
		resH = DmGetResource (strRsc, labelIDs[i]);
		if (resH != 0)
			{
			Char * ptr;
			Int16 width;

			ptr = MemHandleLock (resH);
			width = FntCharsWidth (ptr, StrLen (ptr));
			if (tabStop < width)
			{
				tabStop = width;
			}
			MemHandleUnlock (resH);
			DmReleaseResource (resH);
			}
		}
	
	msg->tabStop = tabStop;
	FntSetFont (oldFont);
		
	// Get the bounds of the message.
	frm = FrmGetActiveForm ();
	objIndex = FrmGetObjectIndex (frm, MessageGadget);
	FrmGetObjectBounds (frm, objIndex, &msg->bounds);

	// Get the current record
	MailGetRecord (MailDB, CurrentRecord, &msg->record, &msg->recordH);

	// Get the friendly mame of the recipent and the sender of the message.
	*msg->toFriendlyName = 0;
	if ((msg->record.sentTo) && StrChr (msg->record.sentTo, '('))
		{
		MailParseAddress (msg->record.sentTo, true, msg->toFriendlyName, 
			maxFriendlyNameLen);
		}

	*msg->fromFriendlyName = 0;
	if ((msg->record.from) && StrChr (msg->record.from, '('))
		{
		MailParseAddress (msg->record.from, true, msg->fromFriendlyName, 
			maxFriendlyNameLen);
		}


	msg->fieldTextPos = fieldTextPos;
	msg->fieldType = fieldType;

	msg->fieldText = MsgGetFieldPtr (msg);

	msg->selectStartField = MessageSelectStartField;
	msg->selectEndField = MessageSelectEndField;
	msg->selectStartPos = MessageSelectStartPos;
	msg->selectEndPos = MessageSelectEndPos;	

	if (DateToInt (msg->record.date))
		{
		// Format the date and time.
		DateToAscii (msg->record.date.month, 
						 msg->record.date.day, 
						 (msg->record.date.year+firstYear) % 100,
						 DateFormat,
						 msg->dateStr);
	
		len = StrLen (msg->dateStr);
		msg->dateStr [len++] = spaceChr;
	
		TimeToAscii (msg->record.time.hours, 
						 msg->record.time.minutes,
						 TimeFormat,
						 &msg->dateStr[len]);
		}
	else
		{
		*msg->dateStr = 0;
		}
}



/***********************************************************************
 *
 * FUNCTION:    MsgSearch
 *
 * DESCRIPTION: This routine searchs the fields of the mail message 
 *              that are displayable in the Message View. 
 *
 * PARAMETERS:	 strToFind - string to find
 *              fieldP    - returned: field the match occurred in
 *              posP      - starting character position of the match             
 *
 * RETURNED:	 true if a match was found
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/27/96	Initial Revision
 *
 ***********************************************************************/
Boolean MsgSearch (MailDBRecordPtr recordP, Char * strToFind, 
	UInt16 * fieldNumP, UInt16 * posP, UInt16 * matchLengthP)
{
	UInt32	pos;
	UInt16	matchLength;
	UInt16	fieldType;
	Char *	ptr;
	Char		friendlyName [maxFriendlyNameLen];


	for (fieldType = msgFieldSentTo; fieldType <= msgFieldBody; fieldType++)
		{
		switch (fieldType)
			{
			case msgFieldSentTo:
				if ((recordP->sentTo) && StrChr (recordP->sentTo, '(' ))
					{
					MailParseAddress (recordP->sentTo, true, friendlyName, maxFriendlyNameLen);
					ptr = friendlyName;
					}
				else
					ptr = recordP->sentTo;
				break;
				
			case msgFieldTo:
				ptr = recordP->to;
				break;
				
			case msgFieldFrom:
				ptr = recordP->from;
				break;
				
			case msgFieldSubject:
				ptr = recordP->subject;
				break;
				
			case msgFieldCC:
				ptr = recordP->cc;
				break;
				
			case msgFieldBody:
				ptr = recordP->body;
				break;

			default:
				ptr = NULL;
			}

		if (ptr && *ptr)
			{
			if (TxtFindString (ptr, strToFind, &pos, &matchLength))
				{
				*fieldNumP = fieldType;
				*posP = pos;
				/* CS2 (24659, 25166, 44807): pass along selection range */
				*matchLengthP = matchLength;
				return (true);
				}
			}
		}
	return (false);
}


/***********************************************************************
 *
 * FUNCTION:    MsgIsHeaderField
 *
 * DESCRIPTION: Returns true if the mail field passed is a header field.
 *
 * PARAMETERS:  fieldType - type a a mail record field.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static Boolean MsgIsHeaderField (MsgFieldType fieldType) 
{
	switch (fieldType)
		{
		case msgFieldSentTo:
		case msgFieldTo:
		case msgFieldFrom:
		case msgFieldSubject:
		case msgFieldCC:
		case msgFieldDate:
			return (true);
			break;
		}
	return (false);
}


/***********************************************************************
 *
 * FUNCTION:    MsgGetHeaderLabel
 *
 * DESCRIPTION: Returns a pointer to a header line's label.
 *
 * PARAMETERS:  fieldType - type a a mail record field.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static MemHandle MsgGetHeaderLabel (MsgFieldType fieldType, MailAddressingType addressing)
{
	UInt16 id;
	
	switch (fieldType)
		{
		case msgFieldSentTo:
			switch (addressing)
				{
				case sentTo:
					id = toStrId;
					break;
				case sentCC:
					id = ccStrId;
					break;
				case sentBCC:
					id = bccStrId;
					break;
				}
			break;
				

		case msgFieldTo:
			id = toStrId;
			break;
			
		case msgFieldFrom:
			id = fromStrId;
			break;
			
		case msgFieldSubject:
			id = subjectStrId;
			break;
			
		case msgFieldCC:
			id = ccStrId;
			break;
			
		case msgFieldDate:
			id = dateStrId;
			break;
			
		default:
			return (0);
			
		}

	return (DmGetResource (strRsc, id));
}


/***********************************************************************
 *
 * FUNCTION:    MsgGetFieldPtr
 *
 * DESCRIPTION: This routine returns a pointer to the specified field
 *              of the record passed.
 *
 * PARAMETERS:  fieldType - type a a mail record field.
 *              repordP   - pointer to a mail record.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static Char * MsgGetFieldPtr (MessegeInfoPtr msg)
{
	switch (msg->fieldType)
		{
		case msgFieldSentTo:
			if (*msg->toFriendlyName)
				return (msg->toFriendlyName);
			else
				return (msg->record.sentTo);
			
		case msgFieldTo:
			return (msg->record.to);
			
		case msgFieldFrom:
			if (ShowFullHeader)
				return (msg->record.from);
			else if (*msg->fromFriendlyName)
				return (msg->fromFriendlyName);
			else
				return (msg->record.from);			
			
		case msgFieldSubject:
			return (msg->record.subject);
			
		case msgFieldCC:
			return (msg->record.cc);
			
		case msgFieldBody:
			return (msg->record.body);
			
		case msgFieldDate:
			return (msg->dateStr);
		}
	return (NULL);
}


/***********************************************************************
 *
 * FUNCTION:    MsgNextField
 *
 * DESCRIPTION: This routine the previous field of a message.
 *
 * PARAMETERS:  msg  -  used to draw and scroll a mesage.
 *
 * RETURNED:    false if there is no next field
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/21/96	Initial Revision
 *
 ***********************************************************************/
static Boolean MsgPreviousField (MessegeInfoPtr msg)
{
	if (ShowFullHeader)
		{
		switch (msg->fieldType)
			{
			case msgFieldTo:
			case msgFieldSentTo:
				msg->fieldText = NULL;
				return (false);

			case msgFieldFrom:
				msg->fieldType = msgFieldTo;
				msg->fieldText = msg->record.to;
				break;
				
			case msgFieldCC:
				msg->fieldType = msgFieldFrom;
				msg->fieldText = msg->record.from;
				break;

			case msgFieldSubject:
				msg->fieldType = msgFieldCC;
				msg->fieldText = msg->record.cc;
				break;
								
			case msgFieldDate:
				msg->fieldType = msgFieldSubject;
				msg->fieldText = msg->record.subject;
				break;
				
			case msgFieldBlankLine:
				msg->fieldType = msgFieldDate;
				msg->fieldText = msg->dateStr;
				break;
				
			case msgFieldBody:
				msg->fieldType = msgFieldBlankLine;
				msg->fieldText = NULL;
				break;
			}
		}
/*
		switch (msg->fieldType)
			{
			case msgFieldTo:
			case msgFieldSentTo:
				msg->fieldText = NULL;
				return (false);

			case msgFieldFrom:
				msg->fieldType = msgFieldTo;
				msg->fieldText = msg->record.to;
				break;

			case msgFieldSubject:
				msg->fieldType = msgFieldFrom;
				msg->fieldText = msg->record.from;
				break;
				
			case msgFieldCC:
				msg->fieldType = msgFieldSubject;
				msg->fieldText = msg->record.subject;
				break;
				
			case msgFieldDate:
				msg->fieldType = msgFieldCC;
				msg->fieldText = msg->record.cc;
				break;
				
			case msgFieldBlankLine:
				msg->fieldType = msgFieldDate;
				msg->fieldText = msg->dateStr;
				break;
				
			case msgFieldBody:
				msg->fieldType = msgFieldBlankLine;
				msg->fieldText = NULL;
				break;
			}
		}
*/

	else
		{
		switch (msg->fieldType)
			{
			case msgFieldTo:
			case msgFieldSentTo:
				return (false);

			case msgFieldFrom:
			case msgFieldSubject:
			case msgFieldCC:
			case msgFieldDate:
				msg->fieldType = msgFieldSentTo;
				if (*msg->toFriendlyName)
					msg->fieldText = msg->toFriendlyName;
				else
					msg->fieldText = msg->record.sentTo;
				break;
			
			case msgFieldBlankLine:
				msg->fieldType = msgFieldFrom;
				if (*msg->fromFriendlyName)
					msg->fieldText = msg->fromFriendlyName;
				else
					msg->fieldText = msg->record.from;
				break;
				
			case msgFieldBody:
				msg->fieldType = msgFieldBlankLine;
				msg->fieldText = NULL;
				break;
			}
		}

	if (msg->fieldText)
		msg->fieldTextPos = StrLen (msg->fieldText);
	else
		msg->fieldTextPos = 0;

	return (true);
}


/***********************************************************************
 *
 * FUNCTION:    MsgNextField
 *
 * DESCRIPTION: This routine the next field of a message.
 *
 * PARAMETERS:  msg  -  used to draw and scroll a mesage.
 *
 * RETURNED:    false if there is no next field
 *
 * REVISION HISTORY:
 *			Name		Date		Description
 *			----		----		-----------
 *			art		6/19/96	Initial Revision
 *			MGolden	7/11/97	Added the subject view to the Int16 header
 *
 ***********************************************************************/
static Boolean MsgNextField (MessegeInfoPtr msg)
{
	if (ShowFullHeader)
		{
		switch (msg->fieldType)
			{
			case msgFieldTo:
			case msgFieldSentTo:
				msg->fieldType = msgFieldFrom;
				msg->fieldText = msg->record.from;
				break;
				
			case msgFieldFrom:
				msg->fieldType = msgFieldCC;
				msg->fieldText = msg->record.cc;
				break;
				
			case msgFieldCC:
				msg->fieldType = msgFieldSubject;
				msg->fieldText = msg->record.subject;
				break;

			case msgFieldSubject:
				msg->fieldType = msgFieldDate;
				msg->fieldText = msg->dateStr;
				break;
				
			case msgFieldDate:
				msg->fieldType = msgFieldBlankLine;
				msg->fieldText = NULL;
				break;
			
			case msgFieldBlankLine:
				msg->fieldType = msgFieldBody;
				msg->fieldText = msg->record.body;
				break;
				
			case msgFieldBody:
				return (false);
			}
		}

	else
		{
		switch (msg->fieldType)
			{
			case msgFieldTo:
			case msgFieldSentTo:
				msg->fieldType = msgFieldFrom;
				if (*msg->fromFriendlyName)
					msg->fieldText = msg->fromFriendlyName;
				else
					msg->fieldText = msg->record.from;
				break;

//mgmg new add the subject field to the Int16 header...				
			case msgFieldFrom:
				msg->fieldType = msgFieldSubject;
				msg->fieldText = msg->record.subject;
				break;
			
			
			case msgFieldSubject:
			case msgFieldCC:
			case msgFieldDate:
				msg->fieldType = msgFieldBlankLine;
				msg->fieldText = NULL;
				break;
			
			case msgFieldBlankLine:
				msg->fieldType = msgFieldBody;
				msg->fieldText = msg->record.body;
				break;
				
			case msgFieldBody:
				return (false);
			}
		}
	
	msg->fieldTextPos = 0;

	return (true);
}


/***********************************************************************
 *
 * FUNCTION:    MsgSelectFieldVisible
 *
 * DESCRIPTION: This routine returns true if the selection field is 
 *              visible in the current message line.
 *
 * PARAMETERS:  msg         - used to draw and scroll a mesage.
 *              selectField - label of field of message
 *
 * RETURNED:    true if selection field is visible
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/28/96	Initial Revision
 *
 ***********************************************************************/
static Boolean MsgSelectFieldVisible (MessegeInfoPtr msg, 
	MsgSelectFieldType selectField)
{
	UInt16 pos;
	MsgFieldType fieldType;

	if (msg->selectStartField > selectField ||
		 msg->selectEndField < selectField)
		return (false);


	fieldType = msg->fieldType;
	pos = msg->fieldTextPos;

	switch (selectField)
		{
		case msgSelectSentToLabel:
			return (fieldType == msgFieldSentTo && pos == 0);

		case msgSelectSentTo:
			return (fieldType == msgFieldSentTo);

		case msgSelectToLabel:
			return (fieldType == msgFieldTo && pos == 0);

		case msgSelectTo:
			return (fieldType == msgFieldTo);

		case msgSelectFromLabel:
			return (fieldType == msgFieldFrom && pos == 0);

		case msgSelectFrom:
			return (fieldType == msgFieldFrom);

		case msgSelectSubjectLabel:
			return (fieldType == msgFieldSubject && pos == 0);

		case msgSelectSubject:
			return (fieldType == msgFieldSubject);

		case msgSelectCC:
			return (fieldType == msgFieldCC && ShowFullHeader);

		case msgSelectCCLabel:
			return (fieldType == msgFieldCC && pos == 0 && ShowFullHeader);

		case msgSelectDate:
			return (fieldType == msgFieldDate && ShowFullHeader);

		case msgSelectDateLabel:
			return (fieldType == msgFieldDate && pos == 0 && ShowFullHeader);

		case msgSelectBlankLine:
			return (fieldType == msgFieldBlankLine);

		case msgSelectBody:
			return (fieldType == msgFieldBody);
			
		}
	return (false);
}


/***********************************************************************
 *
 * FUNCTION:    MsgWordWrap
 *
 * DESCRIPTION: Given a string, determine the number of characters that
 *              can be displayed within the specified width.
 *
 * PARAMETERS:  msg  - structure contain info for word wrapping
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static UInt16 MsgWordWrap (MessegeInfoPtr msg)
{
	UInt16 width;
	UInt16 length = 0;
	Char * ptr;
	Boolean header;

		
	// Leave room for the field description (ex: "To:") if the field 
	// has a description.
	header = MsgIsHeaderField (msg->fieldType);
	if (header)
		{
		width = msg->bounds.extent.x - msg->tabStop;
		}
	else
		width = msg->bounds.extent.x;

	ptr = MsgGetFieldPtr (msg);

	if (ptr)
		{
		ptr += msg->fieldTextPos;
		length = FntWordWrap (ptr, width);
		}



/*
	// If the field is a header and we didn't wrap on a space of linefeed
	// character then wrap on the last address punctuation character.
	if (header && length)
		{
		count = 0;
		ptr += length - 1;
		if (*(ptr+1) != nullChr  &&  *ptr != spaceChr  &&  *ptr != linefeedChr)
			{
			while (count < length)
				{
				// Wrap after right paren.
				if (*ptr == ')')
					{ 
					length -= count;
					break;
					}
				// Wrap before left paren, @, and period.
				else if (*ptr == '(' || *ptr == '@' || *ptr == periodChr)
					{
					if (length > count + 1)
						length -= count + 1;
					break;
					}
				count++;
				ptr--;		
				}
			}
		}
*/
	return (length);
}


/***********************************************************************
 *
 * FUNCTION:    MsgIsLableField
 *
 * DESCRIPTION: Returns true if the field passed is a label field.
 *
 * PARAMETERS:  selectField - masseage label a field
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static Boolean MsgIsLableField (MsgSelectFieldType selectField) 
{
	switch (selectField)
		{
		case msgSelectSentToLabel:
		case msgSelectToLabel:
		case msgSelectFromLabel:
		case msgSelectSubjectLabel:
		case msgSelectCCLabel:
		case msgSelectDateLabel:
			return (true);
		}
	return (false);
}


/***********************************************************************
 *
 * FUNCTION:    InvertCharsRange
 *
 * DESCRIPTION: Invert specified characer range on the specified line.
 *              This rountine performs the actual inverting.
 *
 * PARAMETERS:	 fld      pointer to a FieldType structure.
 *              lineNum  line number
 *              startPos character position of start of range
 *              endPos   character position of end of range.
 *
 * RETURNED:	 nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/28/96	Initial Revision
 *
 ***********************************************************************/
static void MsgInvertCharsRange (MessegeInfoPtr msg, MsgSelectFieldType selectField, 
	UInt16 lineNumber, UInt16 startPos, UInt16 endPos)
{
	UInt16 				length;
	Int16				left;
	Int16				width;
	Char *			ptr;
	MemHandle			resH;
	Boolean 			label;
	RectangleType	r;
	

	label = MsgIsLableField (selectField);

	if (label)
		{
		resH = MsgGetHeaderLabel (msg->fieldType, 
			(MailAddressingType) msg->record.flags.addressing);
		ptr = MemHandleLock (resH);

		length = FntLineWidth (ptr, StrLen(ptr));
		left = msg->bounds.topLeft.x + 
		       (msg->tabStop - length) +
				 FntLineWidth (ptr, startPos);
		}

	else if (msg->fieldText)
		{
		ptr = msg->fieldText;

		left = msg->bounds.topLeft.x + 
			FntLineWidth (&ptr[msg->fieldTextPos], startPos - msg->fieldTextPos);

		if (MsgIsHeaderField (msg->fieldType))
			left += msg->tabStop;
		}
	else
		return;


	width = FntLineWidth (&ptr[startPos], endPos - startPos);

	
	r.topLeft.x = left;
	r.extent.x = width;

	r.extent.y = FntLineHeight ();
	r.topLeft.y = msg->bounds.topLeft.y + (lineNumber * FntLineHeight ());

	if (r.topLeft.x + r.extent.x > msg->bounds.topLeft.x + msg->bounds.extent.x)
		r.extent.x = msg->bounds.extent.x - (r.topLeft.x - msg->bounds.topLeft.x);

/*	CS2 (23430): use swap mode, not invert, for selection
	WinInvertRectangle (&r, 0);
*/

	WinPushDrawState();
	WinSetDrawMode( winSwap );
	WinPaintRectangle( &r, 0 );
	WinPopDrawState();

	if (label)
		{
		MemHandleUnlock (resH);
		DmReleaseResource (resH);
		}
}


/***********************************************************************
 *
 * FUNCTION:    MsgInvertSelection
 *
 * DESCRIPTION: Invert the current selection.
 *
 * PARAMETERS:	 nothing
 *
 * RETURNED:	 nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/28/96	Initial Revision
 *
 ***********************************************************************/
static void MsgInvertSelection (MessegeInfoPtr msg)
{
	UInt16 length;
	UInt16 lineNumber;
	UInt16 numLines;
	UInt16 startPos;
	UInt16 endPos;
	Char * ptr;
	MemHandle resH;
	MsgSelectFieldType selectField;

	if (msg->selectStartField == msgSelectNone)
		return;

	lineNumber = 0;
	numLines = msg->bounds.extent.y / FntLineHeight ();

	while (lineNumber < numLines)
		{
		length = MsgWordWrap (msg);
		if (length)
			{
			for (selectField = msg->selectStartField; 
				  selectField <= msg->selectEndField; selectField++)
				{
				if (MsgSelectFieldVisible (msg, selectField))
					{
					if (MsgIsLableField (selectField))
						{
						if (selectField == msg->selectStartField)
							startPos = msg->selectStartPos;
						else 
							startPos = 0;
							
						if (selectField == msg->selectEndField)
							endPos = msg->selectEndPos;
						else
							{
							resH = MsgGetHeaderLabel (msg->fieldType, 
								(MailAddressingType) msg->record.flags.addressing);
							ptr = MemHandleLock (resH);
							endPos = StrLen (ptr);
							MemHandleUnlock (resH);
							DmReleaseResource (resH);
							}
						}

					else
						{
						if (selectField == msg->selectStartField)
							{
							if (msg->selectStartPos < msg->fieldTextPos + length)
								startPos = max (msg->selectStartPos, msg->fieldTextPos);
							else
								continue;
							}
						else
							startPos = msg->fieldTextPos;
							

						if (selectField == msg->selectEndField)
							{
							if (msg->selectEndPos >= msg->fieldTextPos)
								endPos = min (msg->selectEndPos, msg->fieldTextPos + length);
							else
								continue;
							}
						else
							endPos = msg->fieldTextPos + length;
						}


					MsgInvertCharsRange (msg, selectField, lineNumber, startPos, endPos);
					}
				}

			msg->fieldTextPos += length;
			lineNumber++;
			}
		else
			{ 
			if (msg->fieldType == msgFieldBlankLine)
				lineNumber++;
			if (! MsgNextField (msg))
				break;
			}
		}
}



/***********************************************************************
 *
 * FUNCTION:    MsgDrawLine
 *
 * DESCRIPTION: This routine draws a line of the message.
 *
 * PARAMETERS:  msg         - structure contain info for drawing
 *              charsToDraw - the number of characters to draw
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
static void MsgDrawLine (MessegeInfoPtr msg, UInt16 charsToDraw, 
		UInt16 lineNumber)
{
	UInt16	len;
	UInt16	count;
	UInt16	charsDrawn;
	Int16 	x, y;
	Int16 	drawX;
	Char *	ptr;
	Boolean	header;

	// Calculate the draw position of the line.
	x = msg->bounds.topLeft.x;
	y = msg->bounds.topLeft.y + (lineNumber * FntLineHeight ());

	// If the line is the start of of the header draw the header label.  
	header = MsgIsHeaderField (msg->fieldType);
	if (header)
		{
		if (msg->fieldTextPos == 0)
			{
			ptr = MemHandleLock (MsgGetHeaderLabel (msg->fieldType, 
				(MailAddressingType) msg->record.flags.addressing));
			len = StrLen (ptr);
			drawX = x +( Int16)msg->tabStop - (Int16)FntCharsWidth (ptr, len);
			WinDrawChars (ptr, len, drawX, y);
			MemPtrUnlock (ptr);
			}
		x += msg->tabStop;
		}
	
	// Get a pointer to the start of the text to draw.
	ptr = MsgGetFieldPtr (msg);
	ptr += msg->fieldTextPos;
	
	// Don't draw the linefeedChr character.
	if ((charsToDraw) && ptr[charsToDraw-1] == linefeedChr)
		charsToDraw--;

	// Don't draw trailing spaceChrs.
	while (charsToDraw && ptr[charsToDraw-1] == spaceChr)
		charsToDraw--;

	// Draw the text.
	count = 0;
	charsDrawn = 0;
	while (true)
		{
		// Count until we reach a tab character.
		while ((count < charsToDraw) && (ptr[count] != tabChr))
			count++;
			
		// Draw the characters until/between the tab characters.
		WinDrawChars (&ptr[charsDrawn], count-charsDrawn, x, y);
		if (count >= charsToDraw) break;
		
		// Move the draw position past the tab.
		x += FntLineWidth (&ptr[charsDrawn], count-charsDrawn);
		x += tabChrWidth - ((x - msg->bounds.topLeft.x) % tabChrWidth);

		// Skip over the tab.
		count++;
		charsDrawn = count;
		}
}



/***********************************************************************
 *
 * FUNCTION:    MsgDraw
 *
 * DESCRIPTION: This routine draws an mail message.
 *
 * PARAMETERS:  msg         - structure contain info for drawing.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
void MsgDraw (void)
{
	UInt16				length;
	UInt16				lineNumber;
	UInt16				linesToDraw;
	FontID				currFont;
	MessegeInfoType	msg;
	Boolean isHeader;
	
	currFont = FntSetFont (MessageFont);

	MsgInit (&msg, MessageTopField, MessageScrollPos);

	lineNumber = 0;
	linesToDraw = msg.bounds.extent.y / FntLineHeight ();
	
	while (lineNumber < linesToDraw)
		{
		isHeader = false;
		//mgmg change this to draw the empty header if needed...
		length = MsgWordWrap (&msg);
		if (length)
			{
			MsgDrawLine (&msg, length, lineNumber);
			msg.fieldTextPos += length;
			lineNumber++;
			}
		else if (msg.fieldType == msgFieldBlankLine)
			{
			lineNumber++;
			if (! MsgNextField (&msg))
				break;
			}
		else if (!MsgNextField (&msg))
			break;
		}

	MemHandleUnlock (msg.recordH);


	MsgInit (&msg, MessageTopField, MessageScrollPos);
	MsgInvertSelection (&msg);
	MemHandleUnlock (msg.recordH);

	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    MsgErase
 *
 * DESCRIPTION: This routine erase the current mail message.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/19/96	Initial Revision
 *
 ***********************************************************************/
void MsgErase (void)
{
	UInt16 objIndex;
	FormPtr frm;
	RectangleType bounds;
	
	frm = FrmGetActiveForm ();
	objIndex = FrmGetObjectIndex (frm, MessageGadget);
	FrmGetObjectBounds (frm, objIndex, &bounds);
	WinEraseRectangle (&bounds, 0);
}


/***********************************************************************
 *
 * FUNCTION:    MsgGetFieldHeight
 *
 * DESCRIPTION: This routine returns the height of a format message 
 *              field.
 *
 * PARAMETERS:  fld - pointer to a FieldType structure.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/20/96	Initial Revision
 *
 ***********************************************************************/
static UInt16 MsgGetFieldHeight (MessegeInfoPtr msg)
{
	UInt16		length;
	UInt16		lines;
	
	if (msg->fieldType == msgFieldBlankLine)
		return (1);

	lines = 0;
	length = 0;
	msg->fieldTextPos = 0;

	do
		{
		length += MsgWordWrap (msg);
		msg->fieldTextPos = length;
		lines++;
		}
	while (msg->fieldText[length]);
	
	// If the text end with a linefeed add one to the height.
	if (length && msg->fieldText[length-1] == linefeedChr) 
		lines++;

	return (lines);
}


/***********************************************************************
 *
 * FUNCTION:    MsgGetScrollValues
 *
 * DESCRIPTION: This routine returns the values necessary to update a 
 *              scroll bar.
 *
 * PARAMETERS:  fld - pointer to a FieldType structure.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/20/96	Initial Revision
 *
 ***********************************************************************/
/*
void MsgGetScrollValues (UInt16 * textHeightP, UInt16 * pageHeightP)
{
	UInt16					lines = 0;
	FontID				currFont;
	MessegeInfoType	msg;

	currFont = FntSetFont (MessageFont);
	
	MsgInit (&msg, msgFieldTo, 0);

	while (true)
	 	{
		lines += MsgGetFieldHeight (&msg);
		
		if (! MsgNextField (&msg))
			break;
		}
		
	MemHandleUnlock (msg.recordH);
	FntSetFont (currFont);

	*textHeightP = lines;
	*pageHeightP = msg.bounds.extent.y / FntLineHeight ();;
}
*/


/***********************************************************************
 *
 * FUNCTION:    MsgGetScrollValues
 *
 * DESCRIPTION: This routine returns the values necessary to update a 
 *              scroll bar.
 *
 * PARAMETERS:  fld - pointer to a FieldType structure.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/20/96	Initial Revision
 *
 ***********************************************************************/
void MsgGetScrollValues (UInt16 * textHeightP, UInt16 * pageHeightP, UInt16 * topLineP)
{
	UInt16				length;
	UInt16				lines = 0;
	UInt16				topLine = 0;
	FontID				currFont;
	MsgFieldType		fieldType;
	MessegeInfoType	msg;

	currFont = FntSetFont (MessageFont);

	if (ShowFullHeader)
		fieldType = msgFieldTo;
	else
		fieldType = msgFieldSentTo;
	
	MsgInit (&msg, fieldType, 0);

	while (true)
	 	{
		if (msg.fieldType == MessageTopField)
			topLine = lines;

		if (msg.fieldType == msgFieldBlankLine)
			lines++;
		else
			{
			length = 0;
			msg.fieldTextPos = 0;
		
			do
				{
				length = MsgWordWrap (&msg);

				if (msg.fieldType == MessageTopField)
					{
					if (msg.fieldTextPos < MessageScrollPos && 
						 msg.fieldTextPos + length > MessageScrollPos)
						{
						MessageScrollPos = msg.fieldTextPos;
						topLine = lines;
						}
					else if (msg.fieldTextPos + length == MessageScrollPos)
						//mgmg
						//topLine = lines;
						topLine = lines;
					}

				if (length)
					{
					msg.fieldTextPos += length;
					lines++;
					}
				}
			while (msg.fieldText[msg.fieldTextPos]);
			
			// If the text end with a linefeed add one to the height.
			if (length && msg.fieldText[length-1] == linefeedChr) 
				lines++;
			}
		
		if (! MsgNextField (&msg))
			break;
		}
		
	MemHandleUnlock (msg.recordH);

	*topLineP = topLine;
	*textHeightP = lines;
	*pageHeightP = msg.bounds.extent.y / FntLineHeight ();;

	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    MsgScrollUpNLines
 *
 * DESCRIPTION: This routine scrolls a field of the message winUp by the
 *              number of lines specified.
 *
 * PARAMETERS:	 msg          structure contain message info
 *             linesToScroll number of lines to scroll.
 *
 * RETURNED:	nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	11/1/96	Initial Revision
 *
 ***********************************************************************/
static UInt16 MsgScrollUpNLines (MessegeInfoPtr msg, UInt16 linesToScroll)
{
	UInt16				width;
	UInt16				lineCount = 0;

	if (msg->fieldText && msg->fieldTextPos)
		{
		if (MsgIsHeaderField (msg->fieldType))
			width = msg->bounds.extent.x - msg->tabStop;
		else
			width = msg->bounds.extent.x;

		lineCount = linesToScroll;
		FntWordWrapReverseNLines (msg->fieldText, width, 
			&lineCount, &msg->fieldTextPos);
		}

	if (msg->fieldType == msgFieldBlankLine  && linesToScroll > 1)
		lineCount = 1;

	return (lineCount);
}


/***********************************************************************
 *
 * FUNCTION:    MsgScroll
 *
 * DESCRIPTION: This routine scrolls the message view by the number of
 *              lines specified.
 *
 * PARAMETERS:  direction - winUp or dowm
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/20/96	Initial Revision
 *
 ***********************************************************************/
void MsgScroll (Int16 linesToScroll, WinDirectionType direction)
{
	UInt16				length;
	UInt16				linesScrolled;
	FontID				currFont;
	MessegeInfoType	msg;

	currFont = FntSetFont (MessageFont);
	MsgInit (&msg, MessageTopField, MessageScrollPos);

	if (direction == winUp)
		{
		while (true)
			{
			linesScrolled = MsgScrollUpNLines (&msg, linesToScroll);
			if (linesScrolled == linesToScroll)
				break;
			linesToScroll -= linesScrolled;

			if (! MsgPreviousField (&msg))
				break;
			}
		}


	else if (direction == winDown)
		{
		while (linesToScroll)
			{
			length = MsgWordWrap (&msg);
			if (length)
				{
				msg.fieldTextPos += length;
				linesToScroll--;
				}
			else
				{
				if (msg.fieldType == msgFieldBlankLine)
					linesToScroll--;
				if (! MsgNextField (&msg))
					break;
				}
			}

		// If we at the end of the current field move to the next field so that
		// the field position and field top of the top line will be correct.
		if (length && msg.fieldText[msg.fieldTextPos] == 0)
			MsgNextField (&msg);
		}

	MemHandleUnlock (msg.recordH);
	FntSetFont (currFont);

	MessageScrollPos = msg.fieldTextPos;
	MessageTopField = msg.fieldType;

	// DOLATER ??? - Optimize redraw
	WinEraseRectangle (&msg.bounds, 0);
	MsgDraw ();
}


/***********************************************************************
 *
 * FUNCTION:    MsgUpdateHighlight
 *
 * DESCRIPTION: Adjust the current highlighted region to include or exclude
 *              the specified range of characters.
 *
 * PARAMETERS:	 msg       - structure contain message info
 *              startPos character position of start of range
 *              endPos   character position of end of range.           
 *
 * RETURNED:	 nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/29/96	Initial Revision
 *
 ***********************************************************************/
static void MsgUpdateHighlight (MessegeInfoPtr msg,
	 MsgSelectFieldType startField, UInt16 startPos, 
	 MsgSelectFieldType endField, UInt16 endPos)
{
	MsgSelectFieldType invertStartField;
	MsgSelectFieldType invertEndField;
	UInt16 invertStartPos;
	UInt16 invertEndPos;


	// No current selection?
	if (msg->selectStartField == msg->selectEndField &&
		 msg->selectStartPos == msg->selectEndPos)
		{
		invertStartField = startField;
		invertEndField = endField;
		invertStartPos = startPos;
		invertEndPos = endPos;
		}

	// Add to the end of the selection.
	else if (msg->selectStartField == startField &&
				msg->selectStartPos == startPos)
		{
		if (msg->selectEndField < endField)
			{
			invertStartField = msg->selectEndField;
			invertStartPos = msg->selectEndPos;
			invertEndField = endField;
			invertEndPos = endPos;
			}
		else if (msg->selectEndField > endField)
			{
			invertStartField = endField;
			invertStartPos = endPos;
			invertEndField = msg->selectEndField;
			invertEndPos = msg->selectEndPos;
			}
		else
			{
			invertStartField = msg->selectEndField;
			invertEndField = msg->selectEndField;
			if (msg->selectEndPos < endPos)
				{
				invertStartPos = msg->selectEndPos;
				invertEndPos = endPos;
				}
			else
				{
				invertStartPos = endPos;
				invertEndPos = msg->selectEndPos;
				}
			}
		}

	// Add to the start of the selection.
	else if (msg->selectEndField == endField &&
				msg->selectEndPos == endPos)
		{
		if (msg->selectStartField < startField)
			{
			invertStartField = msg->selectStartField;
			invertStartPos = msg->selectStartPos;
			invertEndField = startField;
			invertEndPos = startPos;
			}
		else if (msg->selectStartField > startField)
			{
			invertStartField = startField;
			invertStartPos = startPos;
			invertEndField = msg->selectStartField;
			invertEndPos = msg->selectStartPos;
			}
		else
			{
			invertStartField = msg->selectStartField;
			invertEndField = msg->selectStartField;
			if (msg->selectStartPos < startPos)
				{
				invertStartPos = msg->selectStartPos;
				invertEndPos = startPos;
				}
			else
				{
				invertStartPos = startPos;
				invertEndPos = msg->selectStartPos;
				}
			}
		}

	else
		{
		if (msg->selectStartField < startField)
			{
			invertStartField = msg->selectStartField;
			invertStartPos = msg->selectStartPos;
			}
		else if (msg->selectStartField > startField)
			{
			invertStartField = startField;
			invertStartPos = startPos;
			}
		else
			{
			invertStartField = msg->selectStartField;
			invertStartPos = min (msg->selectStartPos, startPos);
			}
			
		if (msg->selectEndField < endField)
			{
			invertEndField = endField;
			invertEndPos = endPos;
			}
		else if (msg->selectEndField > endField)
			{
			invertEndField = msg->selectEndField;
			invertEndPos = msg->selectEndPos;
			}
		else
			{
			invertEndField = msg->selectEndField;
			invertEndPos = max (msg->selectEndPos, endPos);
			}
		}

	msg->selectStartField = invertStartField;
	msg->selectStartPos = invertStartPos;
	msg->selectEndField = invertEndField;
	msg->selectEndPos = invertEndPos;

	MsgInvertSelection (msg);

	msg->selectStartField = startField;
	msg->selectStartPos = startPos;
	msg->selectEndField = endField;
	msg->selectEndPos = endPos;
}


/***********************************************************************
 *
 * FUNCTION:    MsgSetSelection
 *
 * DESCRIPTION: This 
 *
 * PARAMETERS:	 
 *
 * RETURNED:	 nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/28/96	Initial Revision
 *
 ***********************************************************************/
static void MsgSetSelection (MessegeInfoPtr msg, 
	MsgSelectFieldType startField, UInt16	startPos,
	MsgSelectFieldType endField, UInt16 endPos)
{
	UInt16 tempPos;
	MsgSelectFieldType tempField;

    if (startField == endField && startPos == endPos)
    	{
    	MsgInvertSelection (msg);
		msg->selectStartField = msgSelectNone;
		msg->selectStartPos = 0;
		msg->selectEndField = msgSelectNone;
		msg->selectEndPos = 0;
    	}

    else if (startField > endField)
    	{
    	tempField = startField;
    	startField = endField;
		endField = tempField;

    	tempPos = startPos;
    	startPos = endPos;
		endPos = tempPos;
    	}

    else if (startField == endField && startPos > endPos)
		{
    	tempPos = startPos;
    	startPos = endPos;
		endPos = tempPos;
		}

	MsgUpdateHighlight (msg, startField, startPos, endField, endPos);

	MessageSelectStartField = msg->selectStartField;
	MessageSelectEndField = msg->selectEndField;
	MessageSelectStartPos = msg->selectStartPos;
	MessageSelectEndPos = msg->selectEndPos;
}


/***********************************************************************
 *
 * FUNCTION:    MsgGetPositionOfPoint
 *
 * DESCRIPTION: Given the position in the pen this routine will 
 *              return the message field and character position 
 *              of the point.
 *
 * PARAMETERS:	 msg       - structure contain message info
 *              x, y      - pen position
 *              fieldP 	  - returned: field the pen is in
 *              posP 	  - returned: character position the pen is on
 *					 r         - returned: bounds of character position
 *
 * RETURNED:	 nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/28/96	Initial Revision
 *
 ***********************************************************************/
static void MsgGetPositionOfPoint (MessegeInfoPtr msg, Int16 x, Int16 y,
	MsgSelectFieldType * fldP,  UInt16 * posP, RectanglePtr r)
{
	UInt16			length;
	UInt16			lineNumber;
	UInt16			lines;
	UInt16			width;
	UInt16			lastWidth;
	Int16				xPos;
	Int16				yPos;
	Char *			ptr;
	Boolean			more;
	Boolean			heading;
	MemHandle			resH = 0;
	UInt16			chSize;


	// Find the line the point is on.
	y = max (y, msg->bounds.topLeft.y);
	y = min (y, msg->bounds.topLeft.y + msg->bounds.extent.y - 1);
	yPos = (y - msg->bounds.topLeft.y) / FntLineHeight ();

	lineNumber = 0;
	lines = msg->bounds.extent.y / FntLineHeight ();
	length = 0;
	
	while (lineNumber < lines)
		{
		if (lineNumber >= yPos)
			break;
			
		length = MsgWordWrap (msg);
		if (length)
			{
			msg->fieldTextPos += length;
			lineNumber++;
			}


		if ((! length) || (msg->fieldText [msg->fieldTextPos] == 0))
			{
			while (true)
				{
				if (msg->fieldType == msgFieldBlankLine)
					{
					if (lineNumber >= yPos)
						break;
					lineNumber++;
					}

				more = MsgNextField (msg);
				if (! more)
					break;
					
				if (msg->fieldText && *msg->fieldText)
					break;
				}
			if (! more) break;
			}
		}



	// Find the field the point in on.
	x = max (x, msg->bounds.topLeft.x);
	x = min (x, msg->bounds.topLeft.y + msg->bounds.extent.x - 1);

	heading = MsgIsHeaderField (msg->fieldType);
	if (heading && 
		 (msg->fieldTextPos == 0) &&
		 (x - msg->bounds.topLeft.x <= msg->tabStop))
		{
		switch (msg->fieldType)
			{
			case msgFieldSentTo:			*fldP = msgSelectSentToLabel;		break;
			case msgFieldTo:				*fldP = msgSelectToLabel;			break;
			case msgFieldFrom:			*fldP = msgSelectFromLabel;		break;
			case msgFieldSubject:		*fldP = msgSelectSubjectLabel;	break;
			case msgFieldCC:				*fldP = msgSelectCCLabel;			break;
			case msgFieldDate:			*fldP = msgSelectDateLabel;		break;
			}

		resH = MsgGetHeaderLabel (msg->fieldType, 
			(MailAddressingType) msg->record.flags.addressing);
		ptr = MemHandleLock (resH);
		length = FntCharsWidth (ptr, StrLen (ptr));

		x -= (msg->tabStop - length);
		if (x < 0) x = 0;
		}

	else 
		{
		switch (msg->fieldType)
			{
			case msgFieldSentTo:			*fldP = msgSelectSentTo;			break;
			case msgFieldTo:				*fldP = msgSelectTo;					break;
			case msgFieldFrom:			*fldP = msgSelectFrom;				break;
			case msgFieldSubject:		*fldP = msgSelectSubject;			break;
			case msgFieldCC:				*fldP = msgSelectCC;					break;
			case msgFieldDate:			*fldP = msgSelectDate;				break;
			case msgFieldBlankLine:		*fldP = msgSelectBlankLine;		break;
			case msgFieldBody:			*fldP = msgSelectBody;				break;
			}

		ptr = msg->fieldText + msg->fieldTextPos;
		length = MsgWordWrap (msg);
		if (length && ptr[length-1] == linefeedChr)
			length--;
			
		if (heading)
			x -= msg->tabStop;
		}
	

	// Determine the character position of the point.
//	if ((! ptr) || (! *ptr))
//		{
//		*posP = 0;
//		}

	if (! ptr)
		*posP = 0;
	
	else if  (! *ptr)
		*posP = msg->fieldTextPos;
	
	else
		{
		xPos = 0;
		width = 0;
		lastWidth = 0;
		while (xPos < length) 
			{
			chSize = TxtGetNextChar (ptr, xPos, NULL);
			width += (ptr[xPos] == tabChr
						 ? tabChrWidth - (width % tabChrWidth)
						 : FntCharsWidth (&ptr[xPos], chSize));
			
			if (width >= x)
				{
				if ((x - lastWidth) >= ((width - lastWidth) >> 1))
					xPos += chSize;
				break;
				}
			xPos += chSize;
			lastWidth = width;
			}
//		if (! resH)
//			xPos += msg->fieldTextPos;
//		*posP = xPos;
		*posP = xPos + msg->fieldTextPos;
		}


	// Determine the bounds of the character position.
	r->topLeft.x = msg->bounds.topLeft.x;
	r->extent.x = msg->bounds.extent.x;
	r->topLeft.y = msg->bounds.topLeft.y + (yPos * FntLineHeight()); 
	r->extent.y = FntLineHeight();

	if (length)
		{
		width = 0;

		// Calculate the size of half of the width of the character before
		// the insertion point.
		if (xPos > 0)
			{
			chSize = TxtGetPreviousChar (ptr, xPos, NULL);
			width = (ptr[xPos - chSize] != tabChr ? (FntCharsWidth (&ptr[xPos - chSize], chSize) + 1) / 2
						: xPos > 1 ? (FntLineWidth (ptr, xPos) - FntLineWidth (ptr, xPos - chSize) + 1) / 2
						: (tabChrWidth + 1) / 2);

			r->topLeft.x += FntLineWidth (ptr, xPos) - width;
			}

		// Add half of the width of the character following the insertion point.
		chSize = TxtGetNextChar (ptr, xPos, NULL);
		width += (ptr[xPos] != tabChr ? FntCharsWidth (&ptr[xPos], chSize) / 2
					 : length > 1 ? (FntLineWidth (ptr, xPos + chSize) - FntLineWidth (ptr, xPos)) / 2
					 : tabChrWidth / 2);

		r->extent.x = width;
		}

	switch (*fldP)
		{
		case msgSelectSentToLabel:
		case msgSelectToLabel:
		case msgSelectFromLabel:
		case msgSelectSubjectLabel:
		case msgSelectCCLabel:
		case msgSelectDateLabel:
			r->topLeft.x += (msg->tabStop - length);
			break;
	
		case msgSelectSentTo:
		case msgSelectTo:
		case msgSelectFrom:
		case msgSelectSubject:
		case msgSelectCC:
		case msgSelectDate:
			r->topLeft.x += msg->tabStop;
			break;
		}


//	WinInvertRectangle (r, 0);

	if (resH)
		MemHandleUnlock (resH);
}


/***********************************************************************
 *
 * FUNCTION:    MsgSelect
 *
 * DESCRIPTION: This is the massage selection routine.
 *
 * PARAMETERS:	 x, y - starting pen position
 *
 * RETURNED:	 nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	9/28/96	Initial Revision
 *
 ***********************************************************************/
void MsgSelect (Int16 x, Int16 y)
{
	FontID					currFont;
	UInt16					penPos;
	UInt16					anchorPos;
	Int16						value;
	Int16						min;
	Int16						max;
	Int16						pageSize;
	FormPtr					frm;
	Boolean					penDown;
	Boolean					update;
	ScrollBarPtr			bar;
	RectangleType			r;
	MessegeInfoType		msg;
	MsgSelectFieldType	penFld;
	MsgSelectFieldType	anchorFld;

	currFont = FntSetFont (MessageFont);

	MsgInit (&msg, MessageTopField, MessageScrollPos);
	
	// Cancel the existing selection
	MsgSetSelection (&msg, msgSelectNone, 0, msgSelectNone, 0);

	msg.fieldTextPos = MessageScrollPos;
	msg.fieldType = MessageTopField;
	msg.fieldText = MsgGetFieldPtr (&msg);
	MsgGetPositionOfPoint (&msg, x, y, &anchorFld, &anchorPos, &r);	

	msg.fieldTextPos = MessageScrollPos;
	msg.fieldType = MessageTopField;
	msg.fieldText = MsgGetFieldPtr (&msg);
	MsgSetSelection (&msg, anchorFld, anchorPos, anchorFld, anchorPos);


	frm = FrmGetActiveForm ();
	bar = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, MessageScrollBar));

	do
		{
		// Above the message bounds?
		if (y < msg.bounds.topLeft.y)
			{
			SclGetScrollBar (bar, &value, &min, &max, &pageSize);
			if (value)
				{
				MsgScroll (1, winUp);
				SclSetScrollBar (bar, value-1, min, max, pageSize);
				update = true;			
				}
			else
				update = false;
			}
				
			
		// Below the message bounds?
		else if (y >= msg.bounds.topLeft.y + msg.bounds.extent.y)
			{
			SclGetScrollBar (bar, &value, &min, &max, &pageSize);
			if (value < max)
				{
				MsgScroll (1, winDown);
				SclSetScrollBar (bar, value+1, min, max, pageSize);
				update = true;			
				}
			else
				update = false;
			}


		// Over a different character?
		else if (! RctPtInRectangle (x, y, &r))
			update =  true;
			
		else
			update = false;

		if (update)
			{
			msg.fieldTextPos = MessageScrollPos;
			msg.fieldType = MessageTopField;
			msg.fieldText = MsgGetFieldPtr (&msg);
			MsgGetPositionOfPoint (&msg, x, y, &penFld, &penPos, &r);	

			msg.fieldTextPos = MessageScrollPos;
			msg.fieldType = MessageTopField;
			msg.fieldText = MsgGetFieldPtr (&msg);
			MsgSetSelection (&msg, penFld, penPos, anchorFld, anchorPos);
			}

		PenGetPoint (&x, &y, &penDown);
		} while (penDown);

	MemHandleUnlock (msg.recordH);
	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    MsgCopySelectedField
 *
 * DESCRIPTION: This routine a selected message field to the passed handle.
 *
 * PARAMETERS:  msg            - structure containing message info
 *              selectField    - type of the field to copy
 *              destH          - handle to copy to
 *              srcP           - text of the message field to copy
 *              appendLinefeed - true 
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	10/2/96	Initial Revision
 *
 ***********************************************************************/
static Boolean MsgCopySelectedField (MessegeInfoPtr msg, MsgSelectFieldType selectField,
			MemHandle destH, Char * srcP, Boolean appendLinefeed)
{
	Err err;
	UInt16 size;
	UInt16 newSize;
	UInt16 length = 0;
	UInt16 endPos;
	UInt16 startPos;
	Char * destP;
	Boolean truncated = false;

	if (srcP)
		{
		if (selectField == msg->selectStartField)
			startPos = msg->selectStartPos;
		else 
			startPos = 0;
			
		if (selectField == msg->selectEndField)
			endPos = msg->selectEndPos;
		else
			endPos = StrLen (srcP);

		length = endPos - startPos;
		}


	size = MemHandleSize (destH);

	if (size + length > maxCopyLength)
		{
		length = maxCopyLength - size;
		truncated = true;
		}
	
	newSize = size + length;
	if (appendLinefeed)
		newSize++;
	 
	err = MemHandleResize (destH, newSize);
	if (err) return (true);
	
	destP = MemHandleLock (destH);
	destP += size-1;
	if (srcP)
		{
		MemMove (destP, srcP+startPos, length);
		destP += length;
		}
	
	// Add a linefeed to the end of the string.
	if (appendLinefeed)
		{
		*destP = linefeedChr;
		destP++;
		}

	// Null terminate the string.
	*destP = 0;
	MemHandleUnlock (destH);
	
	return (truncated);
}


/***********************************************************************
 *
 * FUNCTION:    MsgCopy
 *
 * DESCRIPTION: This routine copies the current selection to the clipboard.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	10/2/96	Initial Revision
 *
 ***********************************************************************/
void MsgCopy (void)
{
	Char * ptr;
	MemHandle resH = 0;
	MemHandle destH;
	Boolean appendLinefeed;
	Boolean truncated = false;
	MsgFieldType topField;
	MessegeInfoType msg;
	MsgSelectFieldType selectField;

	if (MessageSelectStartField == msgSelectNone)
		return;

	// Allocate a handle just large enough to hold a null-terminator.
	destH = MemHandleNew (1);
	if (! destH) return;
	
	ptr = MemHandleLock (destH);
	*ptr = 0;
	MemPtrUnlock (ptr);


	if (ShowFullHeader)
		topField = msgFieldTo;
	else
		topField = msgFieldSentTo;

	MsgInit (&msg, topField, 0);

	while (true)
		{
		if (msg.fieldText && *msg.fieldText)
			{
			for (selectField = msg.selectStartField; 
				  selectField <= msg.selectEndField; selectField++)
				{
				if (MsgSelectFieldVisible (&msg, selectField))
					{
					if (MsgIsLableField (selectField))
						{
						resH = MsgGetHeaderLabel (msg.fieldType, 
							(MailAddressingType) msg.record.flags.addressing);
						ptr = MemHandleLock (resH);
						appendLinefeed = false;
						}
					else
						{
						ptr = msg.fieldText + msg.fieldTextPos;
						switch (selectField)
							{
							case msgSelectSentTo:
							case msgSelectTo:
							case msgSelectFrom:
							case msgSelectSubject:
							case msgSelectCC:
							case msgSelectDate:
								appendLinefeed = (msg.selectEndField > selectField ||
									(msg.selectEndField == selectField &&
									msg.selectEndPos == StrLen (ptr)));
								break;
							default:
								appendLinefeed = false;
							}
						}
					
					if (MsgCopySelectedField (&msg, selectField, destH, ptr, appendLinefeed))
						truncated = true;

					if (resH)
						{
						MemHandleUnlock (resH);
						DmReleaseResource (resH);
						resH = 0;
						}
					}
				}
			}

		else if (msg.fieldType == msgFieldBlankLine && 
		         msg.selectStartField <= msgSelectBlankLine &&
		         msg.selectEndField >= msgSelectBlankLine)
			{
			if (MsgCopySelectedField (&msg, msgSelectBlankLine, destH, NULL, true))
				truncated = true;
			}

		if (! MsgNextField (&msg))
			break;
		}

	MemHandleUnlock (msg.recordH);


	ptr = MemHandleLock (destH);

	if (! truncated)
		ClipboardAddItem (clipboardText, ptr, StrLen (ptr));
	else
		FrmAlert (ClipboardLimitAlert);


	MemPtrFree (ptr);
}


/***********************************************************************
 *
 * FUNCTION:    MsgSelectAll
 *
 * DESCRIPTION: This routine selects an entire message.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	10/3/96	Initial Revision
 *
 ***********************************************************************/
void MsgSelectAll (void)
{
	FontID					currFont;
	UInt16						endPos;
	MessegeInfoType		msg;

	currFont = FntSetFont (MessageFont);

	MsgInit (&msg, MessageTopField, MessageScrollPos);
	
	// Cancel the existing selection
	MsgSetSelection (&msg, msgSelectNone, 0, msgSelectNone, 0);

	msg.fieldTextPos = MessageScrollPos;
	msg.fieldType = MessageTopField;
	msg.fieldText = MsgGetFieldPtr (&msg);

	endPos = StrLen (msg.record.body);
	MsgSetSelection (&msg, msgSelectSentToLabel, 0, msgSelectBody, endPos);

	MemHandleUnlock (msg.recordH);
	FntSetFont (currFont);
}


/***********************************************************************
 *
 * FUNCTION:    MsgChangeFont
 *
 * DESCRIPTION: This routine change the font used to display a mail message.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	12/15/96	Initial Revision
 *
 ***********************************************************************/
void MsgChangeFont (void)
{
	UInt16					pos;
	UInt16					length;
	UInt16					lineNumber;
	UInt16					linesToDraw;
	FontID				currFont;
	MessegeInfoType	msg;

	currFont = FntSetFont (MessageFont);

	MsgInit (&msg, MessageTopField, 0);

//set the view to the top of the current field
MsgPreviousField (&msg);
MsgNextField (&msg);
MessageScrollPos = 0;

	pos = 0;
	lineNumber = 0;
	linesToDraw = msg.bounds.extent.y / FntLineHeight ();
	
	while (lineNumber < linesToDraw)
		{
		length = MsgWordWrap (&msg);
		if (length)
			{
			msg.fieldTextPos += length;
			if (MessageTopField == msg.fieldType)
				{
				if (msg.fieldTextPos < MessageScrollPos)
					pos = msg.fieldTextPos;
				else if (msg.fieldTextPos == MessageScrollPos)
					{
					pos = msg.fieldTextPos;
					lineNumber++;
					}
				else
					lineNumber++;
				}
			else
				lineNumber++;
			}
		else if (msg.fieldType == msgFieldBlankLine)
			{
			lineNumber++;
			if (! MsgNextField (&msg))
				break;
			}
		else if (!MsgNextField (&msg))
			break;
		}
			
	MessageScrollPos = pos;

	MemHandleUnlock (msg.recordH);

	if (lineNumber < linesToDraw && pos != 0 &&
		 (MessageTopField != msgFieldTo || MessageTopField == msgFieldSentTo))
		{
		 MsgScroll (linesToDraw - lineNumber, winUp);
		}
	else
		{
		MsgErase ();
		MsgDraw ();
		}	

	FntSetFont (currFont);
}
