/******************************************************************************
 *
 *
 * File: Starter.c
 *
 *****************************************************************************/

#include <PalmOS.h>
#include "StarterRsc.h"


/***********************************************************************
 *
 *   Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *   Internal Structures
 *
 ***********************************************************************/
typedef struct 
	{
	UInt8 replaceme;
	} StarterPreferenceType;

typedef struct 
	{
	UInt8 replaceme;
	} StarterAppInfoType;

typedef StarterAppInfoType* StarterAppInfoPtr;


/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
//static Boolean HideSecretRecords;


/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/
#define appFileCreator				 'STRT'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

// Define the minimum OS version we support (2.0 for now).
#define ourMinVersion	sysMakeROMVersion(2,0,0,sysROMStageRelease,0)


/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/


/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			FrmAlert (RomIncompatibleAlert);
		
			// Palm OS 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.
			if (romVersion < ourMinVersion)
				{
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
				}
			}
		
		return sysErrRomIncompatible;
		}

	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  formId - id of the form to display
 *
 * RETURNED:    void *
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void * GetObjectPtr(UInt16 objectID)
{
	FormPtr frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}


/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:  frm - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainFormInit(FormPtr frmP)
{
	RectangleType	winRect;
	RectangleType	objectRect;
	RectangleType	originalFormRect;
	RectangleType	resizedFormRect;
	Coord			gapOnTextBottom;
	Coord			gapOnButtonBottom;
	Coord			gapOnCheckboxBottom;
	Coord			gapOnCheckboxRight;
	Coord			gapOnBitmapBottom;
	
	FrmGetFormBounds(frmP, &originalFormRect);

		
		// INCREASE THE SIZE OF THE FORM TO MATCH SCREEN SIZE
	winRect.topLeft.x = 0;
	winRect.topLeft.y = 0;
		// get the current screen size
	WinGetDisplayExtent(&winRect.extent.x, &winRect.extent.y);
	
		// change the size of the form to match the screen size
	WinSetWindowBounds(FrmGetWindowHandle(frmP), &winRect);
	
    FrmGetFormBounds(frmP, &resizedFormRect);

		// REPOSITION CONTROLS
		// text input field (grow both x and y direction)
	FrmGetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainTextInputField), &objectRect);
	gapOnTextBottom = originalFormRect.extent.y - 
		(objectRect.topLeft.y + objectRect.extent.y);
	objectRect.extent.x = resizedFormRect.extent.x;
	objectRect.extent.y = resizedFormRect.extent.y - gapOnTextBottom - objectRect.topLeft.y;
	FrmSetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainTextInputField), &objectRect);
	FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainTextInputField));
	
		// checkbox (right side of the screen)
	FrmGetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainSimpleCheckbox), &objectRect);
	gapOnCheckboxBottom = originalFormRect.extent.y - objectRect.topLeft.y;
	gapOnCheckboxRight = originalFormRect.extent.x - objectRect.topLeft.x;
	objectRect.topLeft.x = resizedFormRect.extent.x - gapOnCheckboxRight;
	objectRect.topLeft.y = resizedFormRect.extent.y - gapOnCheckboxBottom;
	FrmSetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainSimpleCheckbox), &objectRect);
	
		// bitmap (center in x direction) 
	FrmGetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainGlobeBitMap), &objectRect);
	gapOnBitmapBottom = originalFormRect.extent.y - objectRect.topLeft.y;
	objectRect.topLeft.x = (resizedFormRect.extent.x / 2) - (objectRect.extent.x / 2);
	objectRect.topLeft.y = resizedFormRect.extent.y - gapOnBitmapBottom;
	FrmSetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainGlobeBitMap), &objectRect);
	
		// button (left side of screen)
	FrmGetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainOKButton), &objectRect);
	gapOnButtonBottom = originalFormRect.extent.y - objectRect.topLeft.y;
	objectRect.topLeft.y = resizedFormRect.extent.y - gapOnButtonBottom;	
	FrmSetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainOKButton), &objectRect);
		
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
static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
   FormPtr frmP;

	switch (command)
		{
		case MainOptionsAboutStarterApp:
			MenuEraseStatus(0);					// Clear the menu status from the display.
			frmP = FrmInitForm (AboutForm);
			FrmDoDialog (frmP);					// Display the About Box.
			FrmDeleteForm (frmP);
			handled = true;
			break;

		}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  eventP  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandleEvent(EventPtr eventP)
{
   Boolean handled = false;
   FormPtr frmP;

	switch (eventP->eType) 
		{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			MainFormInit( frmP);
			FrmDrawForm ( frmP);
			handled = true;
			break;

		default:
			break;
		
		}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventPtr eventP)
{
	UInt16 formId;
	FormPtr frmP;

	if (eventP->eType == frmLoadEvent)
		{
		// Load the form resource.
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId)
			{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

			default:
//				ErrFatalDisplay("Invalid Form Load Event");
				break;

			}
		return true;
		}
	
	return false;
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value 0 if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStart(void)
{
    StarterPreferenceType prefs;
    UInt16 prefsSize;

	// Read the saved preferences / saved-state information.
	prefsSize = sizeof(StarterPreferenceType);
	if (PrefGetAppPreferences(appFileCreator, appPrefID, &prefs, &prefsSize, true) != 
		noPreferenceFound)
		{
		}
	
   return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{
   StarterPreferenceType prefs;

	// Write the saved preferences / saved-state information.  This data 
	// will be backed up during a HotSync.
	PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
		&prefs, sizeof (prefs), true);
		
	// Close all the open forms.
	FrmCloseAllForms ();
}


/***********************************************************************
 *
 * FUNCTION:    StarterPalmMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
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
static UInt32 StarterPalmMain(UInt16 cmd, MemPtr /*cmdPBP*/, UInt16 launchFlags)
{
	Err error;

	error = RomVersionCompatible (ourMinVersion, launchFlags);
	if (error) return (error);

	switch (cmd)
		{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error) 
				return error;
				
			FrmGotoForm(MainForm);
			AppEventLoop();
			AppStop();
			break;

		default:
			break;

		}
	
	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    return StarterPalmMain(cmd, cmdPBP, launchFlags);
}

