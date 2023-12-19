/******************************************************************************
 *
 *
 * File: Starter.c
 *
 *****************************************************************************/

#include <PalmOS.h>
#include "StarterRsc.h"

#include "WritePad.h"  

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

Boolean writePadPresent;

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
#define appFileCreator            'STRT'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

#define NUM_NEW_BUTTONS            11

#define KEYPAD_TOPX    	98
#define KEYPAD_TOPY     11
#define BUTTON_WIDTH    9
#define BUTTON_HEIGHT   9

// Define the minimum OS version we support (2.0 for now).
#define ourMinVersion   sysMakeROMVersion(2,0,0,sysROMStageRelease,0)

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
 * FUNCTION:    SetNewButtons
 *
 * DESCRIPTION: Sets up the numeric keypad structure.
 *
 * PARAMETERS:  
 *
 * RETURNED:    void *
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void SetNewButtons(PenBtnListType *buttonList)
{
    UInt16 index, i;

    index = buttonList->numButtons;

/*------------------------------------------------------------------------
 * Look for the numeric keyboard button.  Simply set the width to 0 for this
 * example.  A complete app would competely remove it.
 *----------------------------------------------------------------------*/
    for (i = 0;i < index; i++)
    {
        if (buttonList->buttons[i].asciiCode == vchrKeyboardNumeric)
        {
            buttonList->buttons[i].boundsR.extent.x = 1;
            buttonList->buttons[i].boundsR.extent.y = 1;
        }   
    }   

/*------------------------------------------------------------------------
 * '1'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+0].boundsR.topLeft.x = KEYPAD_TOPX ;
    buttonList->buttons[index+0].boundsR.topLeft.y = KEYPAD_TOPY;
    buttonList->buttons[index+0].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+0].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+0].asciiCode         = '1';
    buttonList->buttons[index+0].keyCode           = 0;
    buttonList->buttons[index+0].modifiers         = 0;

/*------------------------------------------------------------------------
 * '2'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+1].boundsR.topLeft.x = KEYPAD_TOPX + BUTTON_WIDTH;
    buttonList->buttons[index+1].boundsR.topLeft.y = KEYPAD_TOPY;
    buttonList->buttons[index+1].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+1].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+1].asciiCode         = '2';
    buttonList->buttons[index+1].keyCode           = 0;
    buttonList->buttons[index+1].modifiers         = 0;

/*------------------------------------------------------------------------
 * '3'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+2].boundsR.topLeft.x = KEYPAD_TOPX + (BUTTON_WIDTH*2);
    buttonList->buttons[index+2].boundsR.topLeft.y = KEYPAD_TOPY;
    buttonList->buttons[index+2].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+2].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+2].asciiCode         = '3';
    buttonList->buttons[index+2].keyCode           = 0;
    buttonList->buttons[index+2].modifiers         = 0;

/*------------------------------------------------------------------------
 * '4'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+3].boundsR.topLeft.x = KEYPAD_TOPX;
    buttonList->buttons[index+3].boundsR.topLeft.y = KEYPAD_TOPY + BUTTON_HEIGHT;
    buttonList->buttons[index+3].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+3].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+3].asciiCode         = '4';
    buttonList->buttons[index+3].keyCode           = 0;
    buttonList->buttons[index+3].modifiers         = 0;

/*------------------------------------------------------------------------
 * '5'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+4].boundsR.topLeft.x = KEYPAD_TOPX + BUTTON_WIDTH;
    buttonList->buttons[index+4].boundsR.topLeft.y = KEYPAD_TOPY + BUTTON_HEIGHT;
    buttonList->buttons[index+4].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+4].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+4].asciiCode         = '5';
    buttonList->buttons[index+4].keyCode           = 0;
    buttonList->buttons[index+4].modifiers         = 0;

/*------------------------------------------------------------------------
 * '6'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+5].boundsR.topLeft.x = KEYPAD_TOPX + (BUTTON_WIDTH*2);
    buttonList->buttons[index+5].boundsR.topLeft.y = KEYPAD_TOPY + BUTTON_HEIGHT;
    buttonList->buttons[index+5].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+5].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+5].asciiCode         = '6';
    buttonList->buttons[index+5].keyCode           = 0;
    buttonList->buttons[index+5].modifiers         = 0;

/*------------------------------------------------------------------------
 * '7'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+6].boundsR.topLeft.x = KEYPAD_TOPX;
    buttonList->buttons[index+6].boundsR.topLeft.y = KEYPAD_TOPY + (BUTTON_HEIGHT*2);
    buttonList->buttons[index+6].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+6].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+6].asciiCode         = '7';
    buttonList->buttons[index+6].keyCode           = 0;
    buttonList->buttons[index+6].modifiers         = 0;

/*------------------------------------------------------------------------
 * '8'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+7].boundsR.topLeft.x = KEYPAD_TOPX + BUTTON_WIDTH;
    buttonList->buttons[index+7].boundsR.topLeft.y = KEYPAD_TOPY + (BUTTON_HEIGHT*2);
    buttonList->buttons[index+7].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+7].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+7].asciiCode         = '8';
    buttonList->buttons[index+7].keyCode           = 0;
    buttonList->buttons[index+7].modifiers         = 0;

/*------------------------------------------------------------------------
 * '9'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+8].boundsR.topLeft.x = KEYPAD_TOPX + (BUTTON_WIDTH*2);
    buttonList->buttons[index+8].boundsR.topLeft.y = KEYPAD_TOPY + (BUTTON_HEIGHT*2);
    buttonList->buttons[index+8].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+8].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+8].asciiCode         = '9';
    buttonList->buttons[index+8].keyCode           = 0;
    buttonList->buttons[index+8].modifiers         = 0;

/*------------------------------------------------------------------------
 * '0'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+9].boundsR.topLeft.x = KEYPAD_TOPX;
    buttonList->buttons[index+9].boundsR.topLeft.y = KEYPAD_TOPY + (BUTTON_HEIGHT*3);
    buttonList->buttons[index+9].boundsR.extent.x  = (BUTTON_WIDTH * 2) - 1;
    buttonList->buttons[index+9].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+9].asciiCode         = '0';
    buttonList->buttons[index+9].keyCode           = 0;
    buttonList->buttons[index+9].modifiers         = 0;

/*------------------------------------------------------------------------
 * '.'
 *----------------------------------------------------------------------*/
    buttonList->buttons[index+10].boundsR.topLeft.x = KEYPAD_TOPX + (BUTTON_WIDTH*2);
    buttonList->buttons[index+10].boundsR.topLeft.y = KEYPAD_TOPY + (BUTTON_HEIGHT*3);
    buttonList->buttons[index+10].boundsR.extent.x  = BUTTON_WIDTH - 1;
    buttonList->buttons[index+10].boundsR.extent.y  = BUTTON_HEIGHT - 1;
    buttonList->buttons[index+10].asciiCode         = '.';
    buttonList->buttons[index+10].keyCode           = 0;
    buttonList->buttons[index+10].modifiers         = 0;
 
    buttonList->numButtons += NUM_NEW_BUTTONS;

}

/***********************************************************************
 *
 * FUNCTION:    SetTemplate
 *
 * DESCRIPTION: This routine changes the template to our template
 *
 * PARAMETERS:  
 *
 * RETURNED:    void *
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void SetTemplate(void)
{
    MemHandle       normalH, invertedH;
    BitmapPtr       normal, inverted, originalNormal, originalInverted;
    RectangleType   alpha, numeric;
    UInt16          numNewButtons;
    PenBtnListType *currentPtr, *newPtr;
    UInt16          currentSize, newSize;
    WinHandle		winH, oldWinH;
    Err				err;

/*------------------------------------------------------------------------
 * First draw the new writepad template
 *----------------------------------------------------------------------*/
 		// get the normal keypad
    normalH = DmGetResource(bitmapRsc, NormalWritePadBitmapFamily);
    ErrFatalDisplayIf(!normalH, "Missing normal bitmap");
    normal = MemHandleLock(normalH);

		// draw the normal keypad on the normal writepad bitmap
 	WrtpGetTemplateBitmaps(&originalNormal, &originalInverted, NULL, NULL);
	winH = WinCreateBitmapWindow(originalNormal, &err);
	oldWinH = WinSetDrawWindow(winH);
	WinDrawBitmap(normal, KEYPAD_TOPX - 1, KEYPAD_TOPY - 1);
    WinSetDrawWindow(oldWinH);
	WinDeleteWindow(winH, false);

 		// get the inverted keypad
    invertedH = DmGetResource(bitmapRsc, InvertedWritePadBitmapFamily);
    ErrFatalDisplayIf(!invertedH, "Missing inverted bitmap");
    inverted = MemHandleLock(invertedH);

		// draw the invertedkeypad on the inverted writepad bitmap
	winH = WinCreateBitmapWindow(originalInverted, &err);
	oldWinH = WinSetDrawWindow(winH);
	WinDrawBitmap(inverted, KEYPAD_TOPX - 1, KEYPAD_TOPY - 1);
    WinSetDrawWindow(oldWinH);
	WinDeleteWindow(winH, false);

    WrtpSetTemplateBitmaps(originalNormal, originalInverted, NULL, NULL);
    WrtpMaximizeWindow();	// force of redraw of the writepad

    MemPtrUnlock(normal);
    DmReleaseResource(normalH);
    MemPtrUnlock(inverted);
    DmReleaseResource(invertedH);

/*------------------------------------------------------------------------
 * The numeric graffiti area can not be removed, so make its width 1, which 
 * will effectively remove it.
 *----------------------------------------------------------------------*/
	WrtpGetAreas(&alpha, &numeric);
	numeric.extent.x = 1;
	WrtpSetAreas(&alpha, &numeric);

/*------------------------------------------------------------------------
 * First get the number of writepad buttons so we know how much to allocate,
 * with our additional buttons.
 *----------------------------------------------------------------------*/
    currentSize = WrtpGetButtonListSize(true);

    if ((currentPtr = MemPtrNew(currentSize)) == NULL)
    {
        ErrFatalDisplay("Allcoate Errror");
        return;
    }    

    WrtpGetButtonList(currentPtr, true);

/*------------------------------------------------------------------------
 * Allocate space for new button list.  
 *----------------------------------------------------------------------*/
    numNewButtons = currentPtr->numButtons + NUM_NEW_BUTTONS;
    newSize = sizeof(PenBtnListType) + (sizeof(PenBtnInfoType) * (numNewButtons - 1));

    if ((newPtr = MemPtrNew(newSize)) == NULL)
    {
        MemPtrFree(currentPtr);
        ErrFatalDisplay("Allcoate Errror");
        return;
    }    

    MemMove(newPtr, currentPtr, currentSize);

    SetNewButtons(newPtr);
    WrtpSetButtonList(newPtr, true);

/*------------------------------------------------------------------------
 * Since the button list is copied by the system, we can free our copy.
 *----------------------------------------------------------------------*/
    MemPtrFree(currentPtr);
    MemPtrFree(newPtr);

}



/***********************************************************************
 *
 * FUNCTION:    ResetTemplate
 *
 * DESCRIPTION: This routine restores the defaul template, areas and 
 *              writepad buttons.
 *
 * PARAMETERS:  
 *
 * RETURNED:    void *
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void ResetTemplate(void)
{
    WrtpRestoreDefaultTemplates();
    WrtpMaximizeWindow();
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
        case MainOptionsAboutStarterApp :
            MenuEraseStatus(0);              // Clear the menu status from the display.
            frmP = FrmInitForm (AboutForm);
            FrmDoDialog (frmP);              // Display the About Box.
            FrmDeleteForm (frmP);
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
 
        case ctlSelectEvent :
            switch(eventP->data.ctlSelect.controlID)
            {
                case MainSetTemplateButton :
                    if (!writePadPresent)
                        FrmAlert(WritePadExtNotFoundAlert);
                    else    
                        SetTemplate();
                    break;

                case MainResetTemplateButton :
                    if (!writePadPresent)
                        FrmAlert(WritePadExtNotFoundAlert);
                    else    
                        ResetTemplate();
                    break;
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

    if (_WritePadFeaturePresent(&version))          
        writePadPresent = true;
    else
        writePadPresent = false;

   
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
static UInt32 StarterPalmMain(UInt16 cmd, MemPtr /*cmdPBP*/, UInt16 launchFlags)
{
    Err error;
 
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

