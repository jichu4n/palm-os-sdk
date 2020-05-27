/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Starter.c
 *
 * Description:
 *              HTTP Library throughput test
 *
 * History:
 * 	 6/5/97	Gavin Peacock		Initial version
 *		4/22/2003	bda					mod for HTTP throughput
 *
 *****************************************************************************/

#include <PalmOS.h>				
#include <68K\Hs.h>
#include <68K\system\HsNav.h>

#include "AppStdIO.h"
#include "HTTPLibTest.h"
#include "httptest.h"


/***********************************************************************
 *
 *   Internal Structures
 *
 ***********************************************************************/ 
typedef struct 
	{
		Char url[128]; // last url used in GET
	} StarterPreferenceType;


/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			  		'axs2'  // in Makefile
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

#define version5_0		0x05000000	// PalmOS 5.0 version number (beaming support)

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/


/***********************************************************************
 *
 * Prototypes for internal functions
 *
 **********************************************************************/
static Err StartApplication(void);
static Boolean MainFormHandleEvent(EventType * event);
static void EventLoop(void);

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

/***********************************************************************
 *
 * FUNCTION:	GetFieldText
 *
 * DESCRIPTION:	Copies text from a field to a destination buffer.
 *
 * PARAMETERS:	destP		-- dest text buffer pointer
 *					bufSize	-- buffer size
 *					fldID		-- field object id
 *
 * RETURNED:    nothing.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk	10/3/95	Initial version
 *
 ***********************************************************************/
static void GetFieldText(Char* destP, UInt16 bufSize, FormPtr frmP, UInt16 fldID)
{
	MemHandle 		stringH;
	Char* 		stringP;
	
	destP[0] = '\0';
	stringH = (MemHandle)FldGetTextHandle( (FieldPtr)GetObjectPtr( fldID) );
	if ( !stringH )
		return;
	stringP = MemHandleLock( stringH );
	StrNCopy( destP, stringP, bufSize );
	destP[bufSize-1] = '\0';				// make sure it is null-terminated
	MemPtrUnlock( stringP );
}


/***********************************************************************
 *
 * FUNCTION:    PrvCommonInitGauge
 *
 * DESCRIPTION:	Call this function on form open to initialize the radio and
 *				 battery status gadgets.
 *			.
 *
 * PARAMETERS:  frm
 *
 * RETURNED:    nothing
 *
 * CALLED BY:	NetworkFormInit
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			vmk		1/30/02		Created -- based on David Matiskella's code
 *								 in Soho.
 *
 ***********************************************************************/
static void PrvCommonInitGauge( FormPtr frm )
{
  UInt32 hsStatusVersion;

  if (FtrGet (hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion) == 0)
	  {
		HsStatusSetGadgetType (frm, NetworkBatteryGadget, hsStatusGadgetBattery);
		HsStatusSetGadgetType (frm, NetworkSignalGadget, hsStatusGadgetSignal);
	  }
} // CommonInitGauge


/***************************************************************
 *	Function:	  PrvWriteMemoPad
 *
 *	Summary:	  This routine write data to a memo pad.
 *
 *	Parameters:
 *	  buf	  IN  data buffer.
 *	  bufsize IN  amout of data to be written.
 *
 *	Returns:
 *	  0 if no error.
 *	
 *	Called By: 
 *	  MainFrmEventHandler()
 *	
 *	Notes:
 *	
 *	History:
 *	  2003-02-04  SLV created 
 *
 ****************************************************************/
static Err
PrvWriteMemoPad (const Char *buf, UInt16 bufsize)
{

  Err err = 0;
  DmOpenRef dbP;
  MemHandle dstH;
  UInt16 memoIndex;
  
  // Open the Memo application database.
  dbP = DmOpenDatabaseByTypeCreator ('DATA', sysFileCMemo, dmModeReadWrite);
  if (!dbP) return DmGetLastErr ();

  memoIndex = DmNumRecords (dbP);

  dstH = DmNewHandle (dbP, bufsize + 1);
  if (!dstH)
	{
	  err = DmGetLastErr ();
	  goto Exit;
	}

  DmWrite (MemHandleLock (dstH), 0, buf, bufsize);
  MemHandleUnlock (dstH);

  err = DmAttachRecord (dbP, &memoIndex, dstH, 0);
  if (err)
	{
	  MemHandleFree (dstH);
	}
  else
	{
	  DmReleaseRecord (dbP, memoIndex, true);
	}
Exit:

  DmCloseDatabase (dbP);

  return err;
}



/***********************************************************************
 *
 * FUNCTION:     StartApplication
 *
 * DESCRIPTION:  This routine sets up the initial state of the application.
 *
 * PARAMETERS:   None.
 *
 * RETURNED:     Nothing.
 *
 ***********************************************************************/
static Err StartApplication(void)
{
	StarterPreferenceType prefs;
	UInt16 	prefSize = sizeof(prefs);
	Int16		res;
	Err		error = errNone;

	// Initialize and draw the main memo pad form.
	error = PrvHTTPLibInitialize();
	if (error)
	  return error;
		
	// remove existing data file
	FileDelete(0, "httpdata.pdb");

	// STDIO
	StdInit(MainForm, MainResultDataField, resMainResultDataScrollerID,	
			NULL/*void (*appProcessCmd)(CharPtr cmdP)*/ );
  
	// setup pref if never used
	res = PrefGetAppPreferences (appFileCreator, appPrefID,
						&prefs, &prefSize, true); 
	if (res == noPreferenceFound){
		StrCopy(prefs.url, "");
		PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
				&prefs, sizeof (prefs), true);
	}
		
						
	FrmGotoForm(MainForm);
	return errNone;
}

/***********************************************************************
 *
 * FUNCTION:    StopApplication
 *
 * DESCRIPTION: This routine closes the application's database
 *              and saves the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void StopApplication(void)
{
	StarterPreferenceType prefs;
	GetFieldText(prefs.url, 128, FrmGetActiveForm(), MainURLField);
	PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
		&prefs, sizeof (prefs), true);

	// STDIO
	//StdFree();
		
	FrmCloseAllForms();
	PrvHTTPLibFinalize();
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean 
MainFormDoCommand (
			UInt16 							command )
{
	Boolean handled = true;
	Char*		url=NULL;
//	Char		buf[8192];
	Char*		bufPtr=NULL;
	UInt32 		bufSize=8192;
    FormType *frmAboutP = NULL;

	bufPtr = (char *)MemPtrNew(bufSize);
	
	switch (command)
	{		
		case rscMainViewCptCmd:
			url = "http://www.palm.com";
			break;

		case rscMainViewGenericCmd:
			url = "http://google.com/";
			break;

		case rscMainViewClearCmd:
			StdClear();
			break;
						
		case rscMainViewSaveMemoCmd:
			GetFieldText(bufPtr, bufSize, FrmGetActiveForm(), MainResultDataField);			
			PrvWriteMemoPad (bufPtr, bufSize);
			break;

        case AboutForm:			
            frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);			
			break;


		default:
			handled = false;
			break;
	}
  
  if (url){
  	FormType *form = FrmGetFormPtr(MainForm);
  	FrmDrawForm(FrmGetActiveForm());
  	SetFieldText(form, MainURLField, url, false, true);
  }

	MemPtrFree(bufPtr);
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:		MainFormHandleEvent
 *
 * DESCRIPTION:	Handles processing of events for the ÒmainÓ form.
 *
 * PARAMETERS:		event	- the most recent event.
 *
 * RETURNED:		True if the event is handled, false otherwise.
 *
 ***********************************************************************/
static Boolean MainFormHandleEvent(EventPtr event)
{
	Boolean	handled = false;
	static UInt32		repeatCount=1;
	static Int32		dormancy = 0;
	UInt32 					ticksPerSec = SysTicksPerSecond();
		
	// See if StdIO can handle it
	if (StdHandleEvent(event)) return true;
	
	switch (event->eType) {
		case menuEvent:
			handled = MainFormDoCommand (event->data.menu.itemID);
			break;

	  // -------------------------------------------------------
	  // List
	  // -------------------------------------------------------
	  case popSelectEvent:
	  switch (event->data.popSelect.controlID)
	  {
	  	case rscMainAcceptEncodePopTrigger:
			switch (event->data.popSelect.selection)
			  {
				case 0:  //*/*
				  break;

				case 1: //gzip
				  break;

				default:
				  break;
			  }
	  	break;

	  	case rscMainDormancyListPopTrigger:
			switch (event->data.popSelect.selection)
			  {
				case 0: 
					dormancy = 0;
				  break;

				case 1: 
					dormancy = 1*ticksPerSec;
				  break;
				  
				case 2:
					dormancy = 5*ticksPerSec;
					break;

				case 3:
					dormancy = 10*ticksPerSec;
					break;
				
				case 4:
					dormancy = 60*ticksPerSec;
					break;

				default:
				  break;
			  }
	  	break;
	  	
	  	case rscMainRepeatListPopTrigger:
			switch (event->data.popSelect.selection)
			  {
				case 0: //1
					repeatCount = 1;
				  break;

				case 1: //3
					repeatCount = 3;
				  break;

				case 2: //5
					repeatCount = 5;
				  break;

				case 3: //10
					repeatCount = 10;
				  break;

				case 4: //10
					repeatCount = 0xFFFFFFFF;
				  break;

				default:
				  break;
			  }
	  	break;
	  }	  

  	case ctlSelectEvent:
  	
	 	if (event->data.ctlEnter.controlID == MainGetButton)	{
	 			UInt32	loop;
	 	    FormType *form = FrmGetFormPtr(MainForm);
	 	    UInt16 index = FrmGetObjectIndex(form, MainURLField);
	 	    Char *url = FldGetTextPtr(FrmGetObjectPtr(form, index));
	 	    if (url && StrLen(url) > 0) {
	 	    	for (loop = 0; loop<repeatCount; loop++){
						Int16 screenX, screenY;
						Boolean penDown;
						// Allows user to abort loop with pen down
						EvtGetPen ( &screenX, &screenY, &penDown );
						if (penDown)
						{
							printf("\nUser requested abort.\n");
							break;
						}

	 	    		if (dormancy) // using 0 causes system to hang!
	 	    			SysTaskDelay(dormancy); 
						printf("iteration: %ld of %ld\n", loop+1, repeatCount);
    	 	  	PrvHTTPLibTestGet(url);
    	 	  }
    	 	}
	 	} else if (event->data.ctlEnter.controlID == MainPostButton) {
	 	    FormType *form = FrmGetFormPtr(MainForm);
	 	    UInt16 index = FrmGetObjectIndex(form, MainURLField);
	 	    Char *url = FldGetTextPtr(FrmGetObjectPtr(form, index));
	 	    Char *postdata;
	 	    
	 	    index = FrmGetObjectIndex(form, MainPostDataField);;
	 	    postdata = FldGetTextPtr(FrmGetObjectPtr(form, index));
	 	    if (url && postdata && StrLen(url) > 0 && StrLen(postdata)) {
    	 	    PrvHTTPLibTestPost(url, postdata);

    	 	}
	 	}
		break;
			
    case frmUpdateEvent:
		  HsStatusUpdateGadgets ();
    	FrmDrawForm(FrmGetActiveForm());
    	handled = true;
       	break;
           	
    case frmOpenEvent:
    	{
				StarterPreferenceType prefs;
				UInt16 prefSize = sizeof(prefs);
	    		FormType *form = FrmGetFormPtr(MainForm);
				FrmSetNavState(form, kFrmNavStateFlagsObjectFocusMode);
	     		FrmDrawForm(FrmGetActiveForm());
	     		FrmNavObjectTakeFocus(form, MainURLField);
				
				PrefGetAppPreferences (appFileCreator, appPrefID,
						&prefs, &prefSize, true); 

	     	if  (prefs.url[0]!=NULL)
	     		SetFieldText(form, MainURLField, prefs.url, false, true);
	     		
	     	PrvCommonInitGauge(form);
			}
			break;
			       	
    default :
    	break;   
	}
	return(handled);
}

/***********************************************************************
 *
 * FUNCTION:    ApplicationHandleEvent
 *
 * DESCRIPTION: This routine loads a form resource and sets the event handler for the form.
 *
 * PARAMETERS:  event - a pointer to an EventType structure
 *
 * RETURNED:    True if the event has been handled and should not be
 *						passed to a higher level handler.
 *
 ***********************************************************************/
static Boolean ApplicationHandleEvent(EventPtr event)
{
	FormPtr	formP;
	UInt16		formId;
	Boolean	handled = false;

	if (event->eType == frmLoadEvent)
		{
		// Load the form resource specified in the event then activate the form.
		formId = event->data.frmLoad.formID;
		formP = FrmInitForm(formId);
		FrmSetActiveForm(formP);

		// Set the event handler for the form.  The handler of the currently 
		// active form is called by FrmDispatchEvent each time it receives an event.
		switch (formId)	{
		case MainForm:
			FrmSetEventHandler(formP, MainFormHandleEvent);
			break;		 	
		}
		handled = true;
	}
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:		EventLoop
 *
 * DESCRIPTION:	A simple loop that obtains events from the Event
 *						Manager and passes them on to various applications and
 *						system event handlers before passing them on to
 *						FrmHandleEvent for default processing.
 *
 * PARAMETERS:		None.
 *
 * RETURNED:		Nothing.
 *
 ***********************************************************************/
static void EventLoop(void)
{
	EventType	event;
	UInt16		error;
	
	do
		{
		// Get the next available event.
		EvtGetEvent(&event, evtWaitForever);
		
		// Give the system a chance to handle the event.
		if (! SysHandleEvent (&event))

			// Give the menu a chance to handle the event
			if (! MenuHandleEvent(0, &event, &error))

				// Give the application a chance to handle the event.
				if (! ApplicationHandleEvent(&event))

					// Let the form object provide default handling of the event.
					FrmDispatchEvent(&event);
		} 
	while (event.eType != appStopEvent);
}

/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: Check that the ROM version meets your
 *              minimum requirement.  Warn if the app was switched to.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags indicating how the application was
 *											 launched.  A warning is displayed only if
 *                                these flags indicate that the app is 
 *											 launched normally.
 *
 * RETURNED:    zero if rom is compatible else an error code
 *                             
 ***********************************************************************/
static Err RomVersionCompatible (UInt32 requiredVersion, UInt8 launchFlags)
{
	UInt32 romVersion;
	
	
	// See if we're on in minimum required version of the ROM or later.
	// The system records the version number in a feature.  A feature is a
	// piece of information which can be looked up by a creator and feature
	// number.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		// If the user launched the app from the launcher, explain
		// why the app shouldn't run.  If the app was contacted for something
		// else, like it was asked to find a string by the system find, then
		// don't bother the user with a warning dialog.  These flags tell how
		// the app was launched to decided if a warning should be displayed.
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			// Pilot 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.  The sysFileCDefaultApp is considered "safe".
			if (romVersion < 0x02000000)
				{				
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
				}
			}
		
		return (sysErrRomIncompatible);
		}

	return 0;
}


/***********************************************************************
 *
 * FUNCTION:    StarterPilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 *
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{	
	Err error;

	error = RomVersionCompatible (version5_0, launchFlags);
	if (error) 
	{
		return (error);
	}

	switch (cmd)
	{		
		case sysAppLaunchCmdNormalLaunch:
			error = StartApplication ();
			if (error)
			{ 
				return error;
			}
				
			EventLoop ();
			StopApplication ();
			break;
		
		default:
			break;
	}

	return(error);
}

