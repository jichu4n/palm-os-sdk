/******************************************************************************
 *
 * File: Starter.c
 *
 * Project : ExampleF
 *
 *****************************************************************************/

#include <PalmOS.h>
#include <PalmOSGlue.h>
#include "StarterRsc.h"

#include "WritePad.h"
#include "Screen.h"

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

Boolean writePadExtensionPresent;

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
static Boolean notifyRegistered = false;
static DmResID	currentGraphicID = Graphic1Bitmap;

/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/
#define appFileCreator            'STRT'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

// Define the minimum OS version we support (2.0 for now).
#define ourMinVersion   sysMakeROMVersion(2,0,0,sysROMStageRelease,0)

/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/
static void UpdateWritePadInfo(writePadEventType *writePadEventP);
static void BuildWritePadEvent(void);


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
 
/*------------------------------------------------------------------------
 * See if we're on in minimum required version of the ROM or later.
 *----------------------------------------------------------------------*/
    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    if (romVersion < requiredVersion)
    {
        if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
           (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
        {
            FrmAlert (RomIncompatibleAlert);
        
/*------------------------------------------------------------------------
 * Palm OS 1.0 will continuously relaunch this app unless we switch to 
 * another safe one.
 *----------------------------------------------------------------------*/
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
    return(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID)));
}

/***********************************************************************
 *
 * FUNCTION:    DrawGraphic
 *
 * DESCRIPTION: This routine puts a graphic on the WritePad alpha area.
 *
 * PARAMETERS:  none
 *
 * RETURNED:    err
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err DrawGraphic(void)
{
    WinHandle       writePadWindow, savedWindow;
    MemHandle       resH;
    BitmapPtr       resP;
    Coord           x, y, winWidth, winHeight;
	Coord			resWidth, resHeight;
	UInt16			rowBytes;
    Boolean			screenActive;

/*------------------------------------------------------------------------
 * Get the WinHandle to the WritePad window
 *----------------------------------------------------------------------*/
    if ((writePadWindow  = WrtpGetWindow()) == NULL)
    {
        return(-1);
	}
        

		// only draw the graphic when the writepad is visible
    if (WrtpWindowMaximized())
    {
		savedWindow = WinSetDrawWindow(writePadWindow);

			// must disable screen extension so that we can draw to the
			// WritePad
		screenActive = ScrnIsEnabled();
		if (screenActive)
		{
			ScrnDisable();
		}
		
			// check for rollover
		if (currentGraphicID > Graphic4Bitmap)
		{
			currentGraphicID = Graphic1Bitmap;
		}
		
        resH = DmGetResource(bitmapRsc, currentGraphicID);
        ErrFatalDisplayIf(!resH, "Missing bitmap");
        resP = MemHandleLock(resH);
        
		++currentGraphicID;
/*------------------------------------------------------------------------
 * Determine the size of the WritePad area so we can center the graphic.
 *----------------------------------------------------------------------*/
        WinGetWindowExtent(&winWidth, &winHeight);

/*------------------------------------------------------------------------
 * Draw the graphic
 *----------------------------------------------------------------------*/
		BmpGlueGetDimensions(resP, &resWidth, &resHeight, &rowBytes);
        x = (winWidth/2) - (resWidth/2);
        x -= 20;   //OK, fudge a little so we don't erase the down arrow.
        y = (winHeight/2) - (resHeight/2);
        WinPaintBitmap(resP, x, y);
    
        MemPtrUnlock(resP);
        DmReleaseResource(resH);
    
    		// turn the screen extension back on if necessary
	 	if (screenActive)
		{
			ScrnEnable();
		}

    	WinSetDrawWindow(savedWindow);
    }
    else
    {
    }    

    
 	
    return(errNone);
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
static void MainFormInit(FormPtr /*frmP*/)
{
	BuildWritePadEvent();

}

/***********************************************************************
 *
 * FUNCTION:    BuildWritePadEvent
 *
 * DESCRIPTION: This routine builds a writepad event structure so
 *					that the information can be updated
 *
 * PARAMETERS:  frm - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void BuildWritePadEvent(void)
{
    writePadEventType	writePadEvent;

	writePadEvent.writePad_minimized = !WrtpWindowMaximized();
	
	writePadEvent.writePad_onTheRight = WrtpWindowRight();
	
	UpdateWritePadInfo(&writePadEvent);

}

/***********************************************************************
 *
 * FUNCTION:    UpdateWritePadInfo
 *
 * DESCRIPTION: This routine updates the writepad information on the
 *					Main form.
 *
 * PARAMETERS:  writePadEventP - pointer to writepad info
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void UpdateWritePadInfo(writePadEventType *writePadEventP)
{

    FormPtr frmP;
    UInt16	formID;

	formID = FrmGetActiveFormID();
	if (formID != MainForm)
	{
		return;
	}

	frmP = FrmGetActiveForm();


	if (writePadEventP->writePad_minimized)
	{
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainMinLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainMaxLabel));
	}
	else
	{
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainMinLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainMaxLabel));
	}

	if (writePadEventP->writePad_onTheRight)
	{
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainRightLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainLeftLabel));
	}
	else
	{
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainRightLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainLeftLabel));
	}

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
        case AboutForm:
            MenuEraseStatus(0);              // Clear the menu status from the display.
            frmP = FrmInitForm (AboutForm);
            FrmDoDialog (frmP);              // Display the About Box.
            FrmDeleteForm (frmP);
            handled = true;
            BuildWritePadEvent();
            break;
           
    }
    return(handled);
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
 
        case ctlSelectEvent :
            switch(eventP->data.ctlSelect.controlID)
            {


            }
            break;    

        case frmOpenEvent:
            frmP = FrmGetActiveForm();
            MainFormInit( frmP);
            FrmDrawForm ( frmP);
            handled = true;
            break;

        default:
            break;
    }
    return(handled);
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
    UInt16  formId;
    FormPtr frmP;

    if (eventP->eType == frmLoadEvent)
    {
/*------------------------------------------------------------------------
 * Load the form resource.
  *----------------------------------------------------------------------*/
        formId = eventP->data.frmLoad.formID;
        frmP   = FrmInitForm(formId);
        FrmSetActiveForm(frmP);
/*------------------------------------------------------------------------
 * Set the event handler for the form.  The handler of the currently
 * active form is called by FrmHandleEvent each time is receives an event.
 *----------------------------------------------------------------------*/
        switch (formId)
        {
            case MainForm:
                FrmSetEventHandler(frmP, MainFormHandleEvent);
                break;
            default:
                ErrFatalDisplay("Invalid Form Load Event");
                break;
 
        }
        return(true);
    }
    
    return(false);
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

    do 
    {
        EvtGetEvent(&event, 15);
        
        DrawGraphic();
 
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
    StarterPreferenceType 	prefs;
    UInt16                	prefsSize;
    UInt32                	version;
    Err						error;
	UInt16					cardNo;
	LocalID					dbID;
    
/*------------------------------------------------------------------------
 * Read the saved preferences / saved-state information.
 *----------------------------------------------------------------------*/
    prefsSize = sizeof(StarterPreferenceType);
    if (PrefGetAppPreferences(appFileCreator, appPrefID, &prefs, &prefsSize, true) != 
            noPreferenceFound)
    {
    }

    if (_WritePadFeaturePresent(&version))         
        writePadExtensionPresent = true;
    else
        writePadExtensionPresent = false;

	if (notifyRegistered == false)
    {
		error = SysCurAppDatabase(&cardNo, &dbID);
		error = SysNotifyRegister(cardNo, dbID, notifyWritePadEvent,
			 NULL, sysNotifyNormalPriority , NULL);
    
    	notifyRegistered = true;
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
    Err					  	error;
	UInt16					cardNo;
	LocalID					dbID;
    StarterPreferenceType 	prefs;

/*------------------------------------------------------------------------
 * Write the saved preferences / saved-state information.  This data 
 * will be backed up during a HotSync.
 *----------------------------------------------------------------------*/
    PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
                           &prefs, sizeof (prefs), true);
      
/*------------------------------------------------------------------------
 * Close all the open forms.
 *----------------------------------------------------------------------*/
    FrmCloseAllForms ();

    if (notifyRegistered == true)
    {
	    error = SysCurAppDatabase(&cardNo, &dbID);
		error = SysNotifyUnregister(cardNo, dbID, notifyWritePadEvent, 
			sysNotifyNormalPriority);
    }

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
static UInt32 StarterPalmMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err error;
    writePadEventType	*writePadEventP;
 
    if ((error = RomVersionCompatible (ourMinVersion, launchFlags)) != 0)
        return (error);
 
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
 
 
 		case sysAppLaunchCmdNotify:
	 		if (((SysNotifyParamType *)cmdPBP)->notifyType == notifyWritePadEvent) 
	 		{
	 			writePadEventP = ((SysNotifyParamType *)cmdPBP)->notifyDetailsP;
				UpdateWritePadInfo(writePadEventP);
	 		
	 		}
 			break;
 			
 			 
        default:
            break;
  
    }
    return(errNone);
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

