/*******************************************************************
 *
 * Project:
 *		Gumby
 *
 * Copyright info:
 *		Copyright(c) 2000, Handspring Inc., All Rights Reserved
 *
 * FileName:
 *	  Ringer.c
 *
 * Description:
 *		Sample application to show ring tone playing.
 *
 * History:
 *   	09-30-03  FHV  Created 
 *
 *******************************************************************/

#include <PalmOS.h>
#include <StdIOPalm.h>
#include <68K/Hs.h>
#include <HsNav.h>

#include <TonesLibTypes.h>
#include <TonesLib.h>

#include "RingTone.h"
#include <SysEvtMgr.h>

//**********************************************************************
//  Types
//**********************************************************************


//**********************************************************************
//  Constants
//**********************************************************************


//**********************************************************************
//  Global variables
//**********************************************************************


static ToneItemPtr	   ToneList = NULL;
static UInt16		   TotalTones = 0;
//static CharPtr *       RingNames = NULL;

static UInt16		   ToneListSequence = 0;


UInt16  TonesLibRefNum = 0;
static Boolean isPDA();

							  

//**********************************************************************
//  Macros
//**********************************************************************


//**********************************************************************
//  Internal functions
//**********************************************************************


static void * GetObjectPtr(Int16 objectID)
{
    FormType * frmP;

    frmP = FrmGetActiveForm();
    return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}


/***********************************************************************
 *
 * FUNCTION:     SetFieldText
 *
 * DESCRIPTION:  This routine sets up the initial state of the application.
 *
 * PARAMETERS:   None.
 *
 * RETURNED:     Nothing.
 *
 ***********************************************************************/
static void SetFieldText(FormPtr frmP, Int16 fldID, Char* srcP, Boolean append,
		Boolean redraw)
{
	MemHandle 		stringH;
	Int16			oldLength = 0;
	Int16			newSize;
	Char* 		stringP;
	FieldPtr		fldP;

	newSize = StrLen(srcP) + 1;					// initialize new size
	
	fldP = (FieldPtr)GetObjectPtr(fldID);
	
	stringH = (MemHandle)FldGetTextHandle( fldP );			// get the current text handle
	FldSetTextHandle( fldP, 0 );					// release this handle from field

	// Resize the existing handle, if any
	if ( stringH )
		{
		if ( append )
			{
			stringP = MemHandleLock( stringH );
			oldLength = StrLen(stringP);
			newSize += oldLength;
			MemPtrUnlock( stringP );
			}
		if ( MemHandleResize(stringH, newSize) )
			goto Exit;
		} // Resize the existing handle, if any
	
	// Otherwise, allocate a new handle
	else
		{
		stringH = MemHandleNew( newSize );		// allocate a new chunk
		if ( !stringH )	return;
		}

	// Append the new text
	stringP = MemHandleLock( stringH );
	StrCopy( stringP + oldLength, srcP );		// copy the new text
	MemPtrUnlock( stringP );
	
Exit:
	FldSetTextHandle( fldP, (MemHandle)stringH );			// set the new text handle
	if ( redraw )										// redraw the field
		{
		FldEraseField( fldP );
		FldDrawField( fldP );
		}

}

/*****************************************************************************/
/* Function:
 *	  GetToneLists 
 *
 * Summary:
 *    Get the current set of tones. It 
 *    queries the tone library to get the tones (they could have changed from
 *    the ones we have in our globals). 
 *
 * Parameters:
 *		
 *
 * Returns: 
 *		void
 *
 * History: 
 *		09/27/2003 FHV	 Created 
 *
 *****************************************************************************/
static void GetToneLists (void)
{
  Word				  numRings;
//  Word				  i;
//  CharPtr*			  namePtr;
//  ToneItemPtr		  toneInfo = NULL;
  Err                 err;

  
 // Build a list containing the name of all the ring sounds.
  err = TonesLibGetToneList (TonesLibRefNum, &ToneList, &numRings);
  if (err) 
	{
	  ErrNonFatalDisplay ("Error getting ring tone list");
	  return; 
	}
  
  TotalTones = numRings;

/*	  
  RingNames = MemPtrNew (numRings * (sizeof (CharPtr) + sizeof (ToneItemType)));
  if (RingNames)
	{
	  namePtr = RingNames;
	  toneInfo = (ToneItemPtr) (((CharPtr)RingNames) + (numRings * (sizeof (CharPtr))));

	  for (i = 0; i < numRings; i++)
		{
		  CopyToneIdentifier ((&toneInfo[i].identifier),
			  ToneList[i].identifier);
		  StrCopy (toneInfo[i].name, ToneList[i].name);
		  *namePtr = toneInfo[i].name;
		  namePtr++;
		}

	}
*/  


}

/***********************************************************************
 *
 * FUNCTION:    PlayRingTone
 *
 * DESCRIPTION: This routine play a ring tone.
 *
 * PARAMETERS:  
 *				
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *		Name	Date		Description
 *		----	----		-----------
 *		FHV		09/30/03	Initial Revision
 *
 ***********************************************************************/
static void PlayRingTone (UInt16 Sequence)
{
  Word				volume = toneVolume3;
  ToneIdentifier	toneId = {0,0};

  // Get Tone Id from Tone List
  CopyToneIdentifier ((&toneId), ToneList[Sequence].identifier);

  // Play the tone 
  
  TonesLibPlayTone (TonesLibRefNum, toneId, 1 /*playCount*/, volume,
		  toneVibrateNone, true /*blocking*/);

  return;
}



/***********************************************************************
 *
 * FUNCTION:    StopRingTone
 *
 * DESCRIPTION: This routine stop a ring tone.
 *
 * PARAMETERS:  
 *				
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *		Name	Date		Description
 *		----	----		-----------
 *		FHV		09/30/03	Initial Revision
 *
 ***********************************************************************/
static void StopRingTone ()
{

  // Stop the tone 
  
  TonesLibStopTone (TonesLibRefNum);

  return;
}





/***********************************************************************
 *
 * FUNCTION:    RingToneHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the "Ring Tone 
 *              Sample App" dialog box.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *		Name	Date		Description
 *		----	----		-----------
 *		art		09/30/03	Initial Revision
 *
 ***********************************************************************/
static Boolean RingToneHandleEvent (EventPtr event)
{
  FormPtr 		frm = FrmGetActiveForm();

  Boolean 		handled = false;
//  EventType newEvent;
	  
  if (event->eType == ctlSelectEvent)
	{
	  switch (event->data.ctlSelect.controlID)
		{
		  /*
		  case DoneButton:
			MemSet (&newEvent, sizeof (newEvent), 0);
			newEvent.eType = appStopEvent;
			EvtAddEventToQueue (&newEvent);
			handled = true;
			break;
		  */
		  case PrevToneButton:
			if (ToneListSequence > 0 ) 
			  ToneListSequence -= 1;
			else
			  ToneListSequence = TotalTones - 1;		// wrap around to last tone

  			SetFieldText(frm, ToneNameField, ToneList[ToneListSequence].name, false, true);

			handled = true;
			break;

		  case NextToneButton:
			if (ToneListSequence < (TotalTones - 1) ) 
			  ToneListSequence += 1;
			else
			  ToneListSequence = 0;						// wrap around to first tone
			
  			SetFieldText(frm, ToneNameField, ToneList[ToneListSequence].name, false, true);

			handled = true;
			break;

		  case PlayToneButton:
			PlayRingTone (ToneListSequence);
			handled = true;
			break;

		  case StopToneButton:
			StopRingTone();
			handled = true;
			break;

		  default:
			break;
		}
	}


  else if (event->eType == frmOpenEvent)
	{
	  ToneListSequence = 0;
  	  SetFieldText(frm, ToneNameField, ToneList[ToneListSequence].name, false, true);

	  FrmDrawForm (frm);
	  FrmNavObjectTakeFocus(frm, PrevToneButton); // Set navigation focus
	  handled = true;
	}
  else if (event->eType == frmCloseEvent)
	{
		StopRingTone();

	}
  else if (event->eType == menuEvent)
  {
 
	  if (event->data.menu.itemID == resMenuItemAbout)
		{
		  UInt16  cardNo;
		  LocalID dbID;
		  DmSearchStateType searchState;

		  DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 'TRRT', true, &cardNo, &dbID);

		  MenuEraseStatus (0);
		  HsAboutHandspringApp(cardNo, dbID, "2003", "Handspring DTS Team");
		  handled = true;
		}
  }


  return (handled);
}





/***************************************************************
 *  Function:    StartApplication
 * 
 *  Summary:
 *    Initialization for starting the application...
 * 
 *  Parameters:
 *    cmd IN  Action code for the app passed in from PilotMain().
 *     
 *  Returns: 
 *    nothing
 *  
 *  Called By:  
 *    PilotMain
 *
 *  History: 
 *    09-30-00  FHV  Created...
 *
 ****************************************************************/
static UInt16
StartApplication (void)
{
  Err err;

  err = SysLibFind (tonesLibName, &TonesLibRefNum);

  if (err)
	  err = SysLibLoad (tonesLibType, tonesLibCreator, &TonesLibRefNum);

  if (err)
  {
     ErrNonFatalDisplay ("Failed to find/load tones library");
	 return err;
  }

  GetToneLists();

  return 0;
}


/***************************************************************
 *  Function:    StopApplication
 * 
 *  Summary:
 *    Cleanup for stopping the application 
 * 
 *  Parameters:
 *    none
 *     
 *  Returns: 
 *    nothing
 *  
 *  Called By:  
 *    PilotMain
 *
 *  History: 
 *    18-May-00  Art  Created...
 *
 ****************************************************************/
static void
StopApplication (void)
{
  FrmCloseAllForms ();


  if (ToneList)
     MemPtrFree (ToneList);
/*
  if ( RingNames )
	MemPtrFree (RingNames);
*/
}


 /***************************************************************
 *  Function:    ApplicationHandleEvent
 * 
 *  Summary:
 *    This routine loads form resources and sets the 
 *    event handler for the form loaded.
 * 
 *  Parameters:
 *    eventP    IN  Pointer to the EventType structure.
 *     
 *  Returns: 
 *    true if the event was handled and should not be passed
 *    to a higher level handler.
 *  
 *  Called By:  
 *    EventLoop 
 *
 *  History: 
 *    01-mar-00  dia  Created...
 *
 ****************************************************************/

static Boolean
ApplicationHandleEvent (EventType* eventP)
{
  Boolean handled = false;

  if (eventP->eType == frmLoadEvent)
	{
      UInt16 frmId = eventP->data.frmLoad.formID;
      FormType* frmP = FrmInitForm (frmId);

      // Set the form to be active...
	  FrmSetActiveForm (frmP);

	  // Set the event handler for the form.  The handler of the currently
	  // active form is called by FrmDispatchEvent each time it receives an
	  // event.
	  switch (frmId)
		{
		case RingToneForm:
		  FrmSetEventHandler (frmP, RingToneHandleEvent);
          handled = true;
		  break;

		}
	}

  return handled;
}


/***********************************************************************
 *
 * FUNCTION:    RingToneFormDoDailog
 *
 * DESCRIPTION: This routine displays the dialog.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *		Name	Date		Description
 *		----	----		-----------
 *		art		05/24/00	Initial Revision
 *
 ***********************************************************************/
static void RingToneFormDoDialog (void)
{
	Word				error;
	EventType 			event;

	UInt16 formID;

	formID = RingToneForm;

	FrmGotoForm (formID);

	do
	  {
		EvtGetEvent (&event, 100);
		
		if (! SysHandleEvent (&event))
		
			if (! MenuHandleEvent (0, &event, &error))
				if ( ! ApplicationHandleEvent( &event ) )
				  FrmDispatchEvent (&event); 
	  }
	while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:    isPDA
 *
 * DESCRIPTION: This routine checks if the device is a PDA.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    true, if PDA; false, if not PDA
 *
 * REVISION HISTORY:
 *		Name	Date		Description
 *		----	----		-----------
 *		
 *
 ***********************************************************************/

static Boolean isPDA()
{
	// This checks whether the device is a PDA.
	
	//UInt32 company = 0;
	UInt32 dev = 0;
	
	FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &dev);
	if( dev == 'TunX' || // LifeDrive
		 dev == 'Zir4' || // Tungsten E2
		 dev == 'TnT5' || // Tungsten T5
		 dev == 'D050')	   // T|X
		return true;
	return false;
}



/***************************************************************
 *  Function:    PilotMain
 * 
 *  Summary:
 *    Main entry point for the Panel.
 * 
 *  Parameters:
 *    cmd           IN  Action code for the app. This is one of the
 *						action codes sysAppLaunchCmdXXX defined in
 *						<SystemMgr.h>
 *    cmdPBP        I/O Parameter block pointer for action code. 
 *    launchFlags   IN  Launch flags, one or more of 
 *                      sysAppLaunchFlagXXX defined in <SystemMgr.h>
 *     
 *  Returns: 
 *    An error code (or 0 if no error).
 *  
 *  Called By:  
 *    Startup code  
 *
 *  History: 
 *    18-May-00  Art  Created...
 *
 ****************************************************************/
UInt32
PilotMain (UInt16 cmd, Ptr cmdPBP, UInt16 launchFlags)
{
	Err err = 0;
	Boolean check_pda = false;
	switch (cmd)
		{
		  case sysAppLaunchCmdNormalLaunch:
			  {
			  	check_pda = isPDA();
				if (check_pda)
				{
					FrmAlert(PDAAlert);
					return err;
				}
				err = StartApplication ();
				if (err) return (err);

				RingToneFormDoDialog ();

				StopApplication ();
				break;
			  }   

		default:
			return sysErrParamErr;

		}

	return 0;
}
