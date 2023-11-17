/******************************************************************************
 *
 * File: Starter.c
 *
 * Project : Example L
 *
 *****************************************************************************/

#include <PalmOS.h>
#include "StarterRsc.h"

#include "Vga.h"          
#include "TrgRsc.h"

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

typedef struct
{
    LocalID  dbID;
    UInt32   creator;
} db_type;


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
#define appFileCreator            'Ex_L'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

#define appLrgIconID               1000
#define appSmallIconID             1001

#define maxDbs                       50

// Define the minimum OS version we support (2.0 for now).
#define ourMinVersion   sysMakeROMVersion(2,0,0,sysROMStageRelease,0)

/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/

/***********************************************************************
 *
 *   Local variables
 *
 ***********************************************************************/
static Boolean vgaPresent    = false;
static db_type db[maxDbs];
static Int16   numDbs;


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
 * FUNCTION:    LoadDbs
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void LoadDbs(void)
{
    DmSearchStateType search;
    LocalID           dbID;
    UInt32            creator;
    Err               err;

    err = DmGetNextDatabaseByTypeCreator(true, &search, 'appl', 0, true, 0, &dbID);

    for (numDbs = 0; (err == errNone) && (numDbs < maxDbs); numDbs++)
    {

        DmDatabaseInfo(0, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &creator);

        db[numDbs].dbID = dbID;
        db[numDbs].creator = creator;

        err = DmGetNextDatabaseByTypeCreator(false, &search, 'appl', 0, true, 0, &dbID);
    }
}


/***********************************************************************
 *
 * FUNCTION:    DrawLargeVgaIcon
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void DrawLargeVgaIcon(LocalID dbID, UInt32 creator, Coord x, Coord y)
{
    DmOpenRef appDB = NULL;
    BitmapPtr iconP = NULL;
    MemHandle iconH = NULL;

/*-------------------------------------------------------------------------
 * First look for icons in VGA Resource Database, if not found look in the
 * app itself.
 *------------------------------------------------------------------------*/
    if ((appDB = DmOpenDatabaseByTypeCreator(vgaResourceDbType, creator, dmModeReadOnly)) == NULL)
        appDB = DmOpenDatabase(0, dbID, dmModeReadOnly);

    if(appDB != NULL)
    {
/*-------------------------------------------------------------------------
 * If 32x32 exists, use it, otherwise expand the 22x22 icon
 *------------------------------------------------------------------------*/
        if ((iconH = DmGet1Resource(iconType,  vgaAppIconID)) != NULL)
        {
            iconP = MemHandleLock(iconH);
            WinDrawBitmap(iconP, x, y);
            MemHandleUnlock(iconH);
            DmReleaseResource(iconH);
        }
        else if ((iconH = DmGet1Resource(iconType,  appLrgIconID)) != NULL)
        {
            iconP = MemHandleLock(iconH);
            VgaWinDrawBitmapExpanded(iconP, x, y);
            MemHandleUnlock(iconH);
            DmReleaseResource(iconH);
        }
        DmCloseDatabase(appDB);
    }

/*------------------------------------------------------------------------
 * Draw Default Icon  - Left as an exercise for the reader
 *----------------------------------------------------------------------*/
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
    Coord         x,y;
    RectangleType r;

    WinGetDisplayExtent(&x, &y);
    RctSetRectangle(&r, 0, 0, x, y);

    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);
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
    if (vgaPresent)
    {
        VgaFormModify(frmP, vgaFormModify160To240);
        MainFormResize(frmP, false);
    }    

    LoadDbs();
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDrawIcons
 *
 * DESCRIPTION: Draw the first n icons on the screen.  Do not worry about
 *              cosmetic appearance.
 *
 * PARAMETERS: 
 *
 * RETURNED:    Nohting
 *
 ***********************************************************************/
static void MainFormDrawIcons(void)
{
    Int16 index = 0;
    Coord x,y;

    for (y = 30; y < 210; y+=50)
    {
        for (x = 10; x < 210; x += 50)
        {
            DrawLargeVgaIcon(db[index].dbID, db[index].creator, x, y);
            index++;

            if (index == numDbs)
                return;
        }
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
 ***********************************************************************/
static Boolean MainFormDoCommand(UInt16 command)
{
    Boolean             handled = false;
    UInt16              formID;
    FormPtr             frmP;
    VgaScreenModeType   screenMode;
    VgaRotateModeType   rotateMode;

    formID = (FrmGetFormId (FrmGetActiveForm ()));

    switch (command)
    {
        case MainOptionsAboutExampleL:
            MenuEraseStatus(0);   
            if (vgaPresent)
            {
                VgaGetScreenMode(&screenMode, &rotateMode);
                VgaSetScreenMode(screenModeScaleToFit, rotateMode);
            }    
            frmP = FrmInitForm (AboutForm);
            FrmDoDialog (frmP);              // Display the About Box.
            FrmDeleteForm (frmP);
            if (vgaPresent)
                VgaSetScreenMode(screenMode, rotateMode);
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
            MainFormDrawIcons();
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
    {
        FrmAlert(VgaNotFoundAlert);
        vgaPresent = false;
        return(-1);
    }

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
 ***********************************************************************/
static UInt32 StarterPalmMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err error;
 
    if ((error = RomVersionCompatible (ourMinVersion, launchFlags)) != 0)
        return (error);
 
    switch (cmd)
    {
        case sysAppLaunchCmdNormalLaunch:
            if ((error = AppStart()) != errNone)
                return(error);

            if (vgaPresent)
                VgaSetScreenMode(screenMode1To1, rotateModeNone);

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

