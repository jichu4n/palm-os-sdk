/******************************************************************************
 *
 * File: Starter.c
 *
 * Project : Example D
 *
 *****************************************************************************/

#include <PalmOS.h>
#include "StarterRsc.h"

#include "vga.h"          

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

FontID  currentFont   = stdFont;
Boolean vgaPresent    = false;
Char    helloStr[]    = "Hello World";

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/


/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/
#define appFileCreator            'Ex_D'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

// Define the minimum OS version we support (2.0 for now).
#define ourMinVersion   sysMakeROMVersion(2,0,0,sysROMStageRelease,0)

#define OptionsRotate               1500

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
 ***********************************************************************/
static void * GetObjectPtr(UInt16 objectID)
{
    FormPtr frmP;
 
    frmP = FrmGetActiveForm();
    return(FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID)));
}

/***********************************************************************
 *
 * FUNCTION:    PrvMoveObject
 *
 * DESCRIPTION: This routine moves an object vertically within a form.
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void PrvMoveObject(FormPtr frmP, UInt16 objIndex, Coord y_diff, Boolean draw)
{
    RectangleType r;
    
    FrmGetObjectBounds(frmP, objIndex, &r);
    if (draw)
    {
        RctInsetRectangle(&r, -2);   //need to erase the frame as well
        WinEraseRectangle(&r, 0);
        RctInsetRectangle(&r, 2);
    }    
    r.topLeft.y += y_diff;
    FrmSetObjectBounds(frmP, objIndex, &r);
}

/***********************************************************************
 *
 * FUNCTION:    PrvCenterObject
 *
 * DESCRIPTION: This routine centers an object within a form.
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void PrvCenterObject(FormPtr frmP, UInt16 objIndex, Boolean draw)
{
    RectangleType r, fr;
    
/*------------------------------------------------------------------------
 * Get the frame and bitmap size
 *----------------------------------------------------------------------*/
    FrmGetFormBounds(frmP, &fr);
    FrmGetObjectBounds(frmP, objIndex, &r);

    if (draw)
    {
        RctInsetRectangle(&r, -2);   //need to erase the frame as well
        WinEraseRectangle(&r, 0);
        RctInsetRectangle(&r, 2);
    }    

    r.topLeft.x = (fr.extent.x/2) - (r.extent.x/2);
    r.topLeft.y = (fr.extent.y/2) - (r.extent.y/2);

    FrmSetObjectBounds(frmP, objIndex, &r);
}

/***********************************************************************
 *
 * FUNCTION:    PrvSetHelloFont
 *
 * DESCRIPTION: Sets the font and centers the field horizontally
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void PrvCenterTextField(FormPtr frmP, FieldPtr fldP, Boolean draw)
{
    RectangleType r, fr;
    Int16         stringWidth;

/*------------------------------------------------------------------------
 * First get the width of the field string in pixels.
 *----------------------------------------------------------------------*/
    FntSetFont(FldGetFont(fldP));
    stringWidth = FntCharsWidth(helloStr, StrLen(helloStr));

/*------------------------------------------------------------------------
 * Now center the field based on width of string in pixels.
 *----------------------------------------------------------------------*/
    FrmGetFormBounds(frmP, &fr);
    FrmGetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainHelloField), &r);

    if (draw)
        FldEraseField(fldP);

    r.topLeft.x = (fr.extent.x/2) - (stringWidth/2);
    FrmSetObjectBounds(frmP, FrmGetObjectIndex(frmP, MainHelloField), &r);

    if (draw)
        FldDrawField(fldP);
}

/***********************************************************************
 *
 * FUNCTION:    MainFormResize
 *
 * DESCRIPTION: 
 *
 * PARAMETERS: 
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void MainFormResize(FormPtr frmP, Boolean draw)
{
    Coord         x, y, y_diff;
    RectangleType r;

/*------------------------------------------------------------------------
 * Get the new extent
 *----------------------------------------------------------------------*/
    WinGetDisplayExtent(&x, &y);
    
/*------------------------------------------------------------------------
 * Get the old extent
 *----------------------------------------------------------------------*/
    FrmGetFormBounds(frmP, &r);
    
/*------------------------------------------------------------------------
 * Calculate change in form size.
 *----------------------------------------------------------------------*/
    y_diff = y - (r.topLeft.y + r.extent.y);

/*------------------------------------------------------------------------
 * Resize the form
 *----------------------------------------------------------------------*/
    r.extent.y = y;
    r.extent.x = x;
    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);
   
    PrvCenterObject   (frmP, FrmGetObjectIndex(frmP, MainGlobeBitMap), draw);
    PrvCenterTextField(frmP, (FieldPtr)GetObjectPtr(MainHelloField), false);;
    PrvMoveObject     (frmP, FrmGetObjectIndex(frmP, MainHelloField), y_diff, draw);

    if (draw)
        FrmDrawForm(frmP);
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
 ***********************************************************************/
static void MainFormInit(FormPtr frmP)
{
    FieldPtr  fldP;
    Char     *tmp;

    currentFont = stdFont;

/*------------------------------------------------------------------------
 * Initialize the field to "Hello World"  and set its font
 *----------------------------------------------------------------------*/
    fldP = (FieldPtr)GetObjectPtr(MainHelloField);
    if ((tmp = FldGetTextPtr(fldP)) != NULL)
        MemPtrFree(tmp);
    tmp = (Char *)MemPtrNew(StrLen(helloStr) + 1);
    StrCopy(tmp, helloStr);
    FldSetTextPtr(fldP, tmp);
    FldSetFont((FieldPtr)GetObjectPtr(MainHelloField), currentFont);

    if (vgaPresent)
        VgaFormModify(frmP, vgaFormModify160To240);

    MainFormResize(frmP, false);
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
 ***********************************************************************/
static Boolean MainFormDoCommand(UInt16 command)
{
    Boolean             handled = false;
    UInt16              formID;
    FormPtr             frmP, aboutFrmP;
    VgaScreenModeType   screenMode;
    VgaRotateModeType   rotateMode;

    frmP   = FrmGetActiveForm();
    formID = FrmGetFormId (frmP);

    switch (command)
    {
        case OptionsAboutExampleD:
            MenuEraseStatus(0);   
            if (vgaPresent)
            {
                VgaGetScreenMode(&screenMode, &rotateMode);
                VgaSetScreenMode(screenModeScaleToFit, rotateMode);
            }    
            aboutFrmP = FrmInitForm (AboutForm);
            FrmDoDialog (aboutFrmP);              // Display the About Box.
            FrmDeleteForm (aboutFrmP);
            if (vgaPresent)
                VgaSetScreenMode(screenMode, rotateMode);
            handled = true;
            break;
           
        case OptionsFont :
            currentFont = FontSelect(currentFont);
            FldSetFont((FieldPtr)GetObjectPtr(MainHelloField), currentFont);
            PrvCenterTextField(frmP, (FieldPtr)GetObjectPtr(MainHelloField), true);
            handled = true;
            break;

        case OptionsRotate :   
            VgaGetScreenMode(&screenMode, &rotateMode);
            VgaSetScreenMode(screenMode1To1, VgaRotateSelect(rotateMode));
            MainFormResize(FrmGetActiveForm(), true);
            handled = true;
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
 
        case frmOpenEvent:
            frmP = FrmGetActiveForm();
            MainFormInit(frmP);
            FrmDrawForm (frmP);
            handled = true;
            break;

        case menuOpenEvent :
            if (vgaPresent)
                MenuAddItem(OptionsFont, OptionsRotate, 0, "Rotate...");
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
 ***********************************************************************/
static Err AppStart(void)
{
    StarterPreferenceType prefs;
    UInt16                prefsSize;
    UInt32                version;

/*------------------------------------------------------------------------
 * Read the saved preferences / saved-state information.
 *----------------------------------------------------------------------*/
    prefsSize = sizeof(StarterPreferenceType);
    if (PrefGetAppPreferences(appFileCreator, appPrefID, &prefs, &prefsSize, true) != 
            noPreferenceFound)
    {
    }

/*------------------------------------------------------------------------
 * Check for VGA Extension
 *----------------------------------------------------------------------*/
    if (_TRGVGAFeaturePresent(&version))          
        vgaPresent  = true;
    else
        vgaPresent = false;

    return(errNone);
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
 ***********************************************************************/
static void AppStop(void)
{
    StarterPreferenceType prefs;

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
    Err               error;
    VgaScreenModeType screenMode;
    VgaRotateModeType rotateMode;
 
    if ((error = RomVersionCompatible (ourMinVersion, launchFlags)) != 0)
        return (error);
 
    switch (cmd)
    {
        case sysAppLaunchCmdNormalLaunch:
            if ((error = AppStart()) != errNone)
                return(error);

/*------------------------------------------------------------------------
 * You must set the screen mode before the form is loaded.
 *----------------------------------------------------------------------*/
            if (vgaPresent)
            {
/*------------------------------------------------------------------------
 * Call VgaGetScreenMode() to get the current rotation.  We force the mode
 * to 1to1 but want to keep the rotation.
 * 
 * Note: If your app does not support screen rotation, simply call 
 *
 *       VgaSetScreenMode(screenMode1To1, rotateModeNone);
 *
 *----------------------------------------------------------------------*/
                VgaGetScreenMode(&screenMode, &rotateMode);
                VgaSetScreenMode(screenMode1To1, rotateMode);
            }

            FrmGotoForm(MainForm);
            AppEventLoop();
            AppStop();
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
