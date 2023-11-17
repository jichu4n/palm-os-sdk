/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailTruncate.c
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  The file contains routines the manage the Mail application's 
 *   HotSync truncation options.
 *
 * History:
 *		Aug 1, 1996	Created by Art Lamb
 *
 *****************************************************************************/

#include <PalmOS.h>

#include "Mail.h"

/* HandEra */
#include "vga.h"
/* end HandEra */


/***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/

extern UInt16				TruncateLen;		// Maximun length of retrieved message

/* HandEra */
static  VgaScreenStateType      gScreenState;
/* end HandEra */

/***********************************************************************
 *
 * Internal Constants
 *
 ***********************************************************************/
#define truncateLen0		250
#define truncateLen1		500
#define truncateLen2		1000
#define truncateLen3		2000
#define truncateLen4		4000
#define truncateLen5		6000
#define truncateLen6		8000


/***********************************************************************
 *
 * FUNCTION:    TruncateApply
 *
 * DESCRIPTION: This routine applies the changes made in the Truncate
 *              Option Dialog.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/1/96	Initial Revision
 *			MGolden	7/14/97	Removed the switch due to 16 bit link errors...
 *
 ***********************************************************************/
static void TruncateApply (void)
{
	ListPtr lst;
	UInt16 selection;
	
	lst = GetObjectPtr (TruncateList);

	selection = LstGetSelection (lst);

	if (selection == 0)
		TruncateLen = truncateLen0;
	else if (selection == 1)
		TruncateLen = truncateLen1;
	else if (selection == 2)
		TruncateLen = truncateLen2;
	else if (selection == 3)
		TruncateLen = truncateLen3;
	else if (selection == 4)
		TruncateLen = truncateLen4;
	else if (selection == 5)
		TruncateLen = truncateLen5;
	else if (selection == 6)
		TruncateLen = truncateLen6;
	else
		TruncateLen = syncDefaultMaxMsgLen;

/*
	switch (LstGetSelection (lst))
		{
		case 0:	TruncateLen = truncateLen0;	break;
		case 1:	TruncateLen = truncateLen1;	break;
		case 2:	TruncateLen = truncateLen2;	break;
		case 3:	TruncateLen = truncateLen3;	break;
		case 4:	TruncateLen = truncateLen4;	break;
		case 5:	TruncateLen = truncateLen5;	break;
		case 6:	TruncateLen = truncateLen6;	break;
		default:	TruncateLen = syncDefaultMaxMsgLen;
		}
*/
}


/***********************************************************************
 *
 * FUNCTION:    TruncateInit
 *
 * DESCRIPTION: This routine initializes the Truncate Dialog.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/1/96	Initial Revision
 *			MGolden	7/14/97	Removed the switch due to 16 bit link errors...
 *
 ***********************************************************************/
static void TruncateInit (void)
{
	UInt16 item;
	ListPtr lst;

	lst = GetObjectPtr (TruncateList);
	
	if (TruncateLen == truncateLen0)
		item = 0;
	else if (TruncateLen == truncateLen1)
		item = 1;
	else if (TruncateLen == truncateLen2)
		item = 2;
	else if (TruncateLen == truncateLen3)
		item = 3;
	else if (TruncateLen == truncateLen4)
		item = 4;
	else if (TruncateLen == truncateLen5)
		item = 5;
	else if (TruncateLen == truncateLen6)
		item = 6;
	else
		{
		item = 4;
		TruncateLen = syncDefaultMaxMsgLen;
		}
		
/*
	switch (TruncateLen)
		{
		case truncateLen0:	item = 0;	break;
		case truncateLen1:	item = 1;	break;
		case truncateLen2:	item = 2;	break;
		case truncateLen3:	item = 3;	break;
		case truncateLen4:	item = 4;	break;
		case truncateLen5:	item = 5;	break;
		case truncateLen6:	item = 6;	break;
		default:
			item = 4;
			TruncateLen = syncDefaultMaxMsgLen;
		}
*/

	LstSetSelection (lst, item);
}


/***********************************************************************
 *
 * FUNCTION:    TruncateHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Truncate
 *              Options Dialog Box".
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event was handled and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	8/1/96	Initial Revision
 *
 ***********************************************************************/
Boolean TruncateHandleEvent (EventPtr event)
{
	FormPtr frm;
	Boolean handled = false;

	if (event->eType == ctlSelectEvent)
		{
		switch (event->data.ctlSelect.controlID)
			{
			case TruncateOkButton:
				TruncateApply ();
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
				break;

			case TruncateCancelButton:
				FrmReturnToForm (0);
/* HandEra */
                if(vgaExists)
                    VgaRestoreScreenState(&gScreenState);
/* end HandEra */
				handled = true;
				break;
			}
		}

	else if (event->eType == frmOpenEvent)
		{
		frm = FrmGetActiveForm ();
/* HandEra */
        if(vgaExists)
            VgaSaveScreenState(&gScreenState);
        ToolsVgaAdjustModalForm(frm, true);
/* end HandEra */
		TruncateInit ();
		FrmDrawForm (frm);
		handled = true;
		}

	return (handled);
}
