/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailLookup.c
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This is the Mail application's main module.
 *
 * History:
 *		June 26, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#include <PalmOS.h>

#include "Mail.h"

/***********************************************************************
 *
 * FUNCTION:    AddressLookup
 *
 * DESCRIPTION: This routine initializes a row of the "Edit View" table.
 *
 * PARAMETERS:  fld - 
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	06/26/96	Initial Revision
 *			meg	10/09/98 free the resultH to avoid a memory leak
 *			kwk	12/04/98	Modified to work for Japanese, plus fixed logic
 *								bug w/deciding when to search right for end of word.
 *			LFe	09/18/00	Use the new PhoneLookup Api
 *			CS2 12/01/00	Pick format string based on whether it's the first address or not
 *
 ***********************************************************************/

void AddressLookup (FieldPtr fld)
{
	MemHandle				formatH;
	MemHandle				buttonH;
	MemHandle				titleH;
	Char					*textP;
	UInt32					wordStart, wordEnd, leftPos, textLen;
	AddrLookupParamsType	params;
	UInt16					titleStrID, insPtPos;
	Boolean					firstAddress;

	MemSet(&params, sizeof(params), 0);

	switch (EditViewField)
	{
		case editToField:		titleStrID = mailToLookupTitleStrID;		break;
		case editFromField:	titleStrID = mailFromLookupTitleStrID;		break;
		case editCCField:		titleStrID = mailCCLookupTitleStrID;		break;
		case editBCCField:	titleStrID = mailBCCLookupTitleStrID;		break;
	}

	/* CS2 (43813): for the first address, we don't want a comma but for additional
	   addresses, we do want to prepend a comma.  Choose the appropriate format string.
	   
	   If the field has no text, we're the first address.
	   If we're within or adjacent to the first word in the field, we're the first address.
	   Otherwise, we're not the first address. 
	 */

	firstAddress = false;
	textP = FldGetTextPtr( fld );
	if ( NULL != textP )
		{
		/* This code is taken from PhoneLookup itself, we want to use the same word-finding
		   algorithm.  This duplicates some work but is the only way.
		 */
		insPtPos = FldGetInsPtPosition( fld );
		leftPos = insPtPos;

		/* If we're not at the start of the field, check word boundaries. */
		if ( 0 < leftPos )
			{
			leftPos -= TxtPreviousCharSize( textP, insPtPos );
			
			textLen = FldGetTextLength( fld );

			/* Look to the left of the insertion point first, then to the right,
			   being careful of the end of the text
			 */
			if ( TxtWordBounds( textP, textLen, leftPos, &wordStart, &wordEnd ) ||
				 ((leftPos != insPtPos) && (insPtPos < textLen) &&
				  TxtWordBounds( textP, textLen, insPtPos, &wordStart, &wordEnd )) )
				{
				if ( 0 == wordStart )
					firstAddress = true;
				}
			}
		else
			{
			firstAddress = true;
			}
		}
	else
		{
		firstAddress = true;
		}

	if ( firstAddress )
		{
		formatH = DmGetResource( strRsc, mailLookupFormatStrID );
		}
	else
		{
		formatH = DmGetResource( strRsc, mailAddlLookupFormatStrID );
		}
	buttonH = DmGetResource (strRsc, phoneLookupAddStrID);
	titleH = DmGetResource (strRsc, titleStrID);

	ErrNonFatalDisplayIf(!formatH, "null resource");
	ErrNonFatalDisplayIf(!buttonH, "null resource");
	ErrNonFatalDisplayIf(!titleH, "null resource");

	params.title = MemHandleLock (titleH);
	params.pasteButtonText = MemHandleLock(buttonH);
	params.formatStringP = MemHandleLock(formatH);
	params.field1 = addrLookupSortField;
	params.field2 = addrLookupEmail;
	params.field2Optional = false;
	params.userShouldInteract = true;

	PhoneNumberLookupCustom (fld, &params, true);
	
	MemHandleUnlock(titleH);
	MemHandleUnlock(buttonH);
	MemHandleUnlock(formatH);
	DmReleaseResource(titleH);
	DmReleaseResource(buttonH);
	DmReleaseResource(formatH);
}
