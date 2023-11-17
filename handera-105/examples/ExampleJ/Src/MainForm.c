/******************************************************************************
 *
 * File: MainForm.c
 *
 * Project : Example 7
 *
 *****************************************************************************/

#include <PalmOS.h>
#include <vfsmgr.h>
#include "StarterRsc.h"

#include "Vga.h"          
#include "Audio.h"
#include "TrgChars.h"
#include "Wave.h"
#include "starter.h"
#include "MainForm.h"

/***********************************************************************
 *
 *   Local Types
 *
 ***********************************************************************/
typedef enum {
    audioStateNoVolume, 
    audioStateNoFile, 
    audioStateOpen, 
    audioStatePlaying, 
    audioStateRecording
} AudioStateType;

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
extern Boolean vgaPresent;
extern Boolean auxButtonPresent;
extern Boolean cardChangedOccurred;

/***********************************************************************
 *
 *   Local Constants
 *
 ***********************************************************************/
#define WAVE_FILE       "ExampleJ.wav"

#define recordStartEvent      ((eventsEnum)((Int16)firstUserEvent))

/***********************************************************************
 *
 *   Local Functions
 *
 ***********************************************************************/
static void *GetObjectPtr      (UInt16 objectID);
static void SetControls        (void);
static void SetSliderPercent   (FormPtr frmP, UInt16 percent);
static void HideObject         (UInt16 id);
static void ShowObject         (UInt16 id);

static void HandlePlayButton   (void);
static void HandleStopButton   (void);
static void HandleRecordButton (void);
static void HandleProgress     (EventPtr eventP);
static void HandlePauseButton  (void);
static void HandleSliderControl(EventPtr eventP);


/***********************************************************************
 *
 *   Local Variables
 *
 ***********************************************************************/
static AudioStateType audioState;
static Boolean        canRecord;
static Boolean        launchedFromButton;


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
 * FUNCTION:    SetSliderPercent
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void SetSliderPercent(FormPtr frmP, UInt16 percent)
{
    ControlPtr sliderP;

    sliderP = (ControlPtr)GetObjectPtr(MainProgressSliderControl);
    CtlSetSliderValues(sliderP, NULL, NULL, NULL, &percent);
}

/***********************************************************************
 *
 * FUNCTION:    HideObject
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HideObject(UInt16 id)
{
    UInt16 obj_index;
    
    obj_index = FrmGetObjectIndex(FrmGetActiveForm(), id);
    FrmHideObject(FrmGetActiveForm(), obj_index);
}

/***********************************************************************
 *
 * FUNCTION:    ShowObject
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void ShowObject(UInt16 id)
{
    UInt16 obj_index;
    
    obj_index = FrmGetObjectIndex(FrmGetActiveForm(), id);
    FrmShowObject(FrmGetActiveForm(), obj_index);
}


/***********************************************************************
 *
 * FUNCTION:    SetControls
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void SetControls(void)
{
    FieldPtr  fldP;
    Char     *tmp;
    FormPtr  frmP;

    switch (audioState)
    {
        case audioStateNoVolume :
            HideObject(MainPlayButton);
            HideObject(MainPauseButton);
            HideObject(MainRecordButton);
            HideObject(MainStopButton);
            HideObject(MainProgressSliderControl);
            ShowObject(MainCardNotFoundLabel);
            HideObject(MainProgressField);
            break;
        case audioStateNoFile :
            HideObject(MainPlayButton);
            HideObject(MainPauseButton);
            canRecord ? ShowObject(MainRecordButton) : HideObject(MainRecordButton);
            HideObject(MainStopButton);
            HideObject(MainProgressSliderControl);
            HideObject(MainCardNotFoundLabel);
            HideObject(MainProgressField);
            break;
        case audioStateOpen :
            ShowObject(MainPlayButton);
            ShowObject(MainPauseButton);
            canRecord ? ShowObject(MainRecordButton) : HideObject(MainRecordButton);
            ShowObject(MainStopButton);
            ShowObject(MainProgressSliderControl);
            HideObject(MainCardNotFoundLabel);
            HideObject(MainProgressField);
            break;
        case audioStatePlaying :
            HideObject(MainPlayButton);
            ShowObject(MainPauseButton);
            HideObject(MainRecordButton);
            ShowObject(MainStopButton);
            ShowObject(MainProgressSliderControl);
            HideObject(MainCardNotFoundLabel);
            HideObject(MainProgressField);
            break;
        case audioStateRecording :
            HideObject(MainPlayButton);
            HideObject(MainPauseButton);
            HideObject(MainRecordButton);
            ShowObject(MainStopButton);
            HideObject(MainProgressSliderControl);
            HideObject(MainCardNotFoundLabel);
            frmP = FrmGetActiveForm();
            ShowObject(MainProgressField);
            fldP = (FieldPtr)GetObjectPtr(MainProgressField);
            if ((tmp = FldGetTextPtr(fldP)) != NULL)
                MemPtrFree(tmp);
            tmp = (Char *)MemPtrNew(10);
            StrCopy(tmp, "00:00");
            FldSetTextPtr(fldP, tmp);
            FldDrawField(fldP);
            break;
    }
}
    
/***********************************************************************
 *
 * FUNCTION:    HandlePlayButton
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HandlePlayButton(void)
{
    Err retval;
    
    if (audioState != audioStateOpen)
        return;

    audioState = audioStatePlaying;
    SetControls();
    if ((retval = WavePlay()) == errNone)
        audioState = audioStateOpen;
    SetControls();

    if (retval != errNone)
        ErrAlert(retval);
}

/***********************************************************************
 *
 * FUNCTION:    HandlePauseButton
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HandlePauseButton(void)
{
    if (audioState != audioStatePlaying)
        return;

/*------------------------------------------------------------------------
 * Note : There is no pause state.  WavePlay() will return after call to 
 *        WavePause(), which will put state into audioStateStop.  
 *----------------------------------------------------------------------*/
    WavePause();
}

/***********************************************************************
 *
 * FUNCTION:    HandleStopButton
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HandleStopButton(void)
{
    WaveStop();
    SetSliderPercent(FrmGetActiveForm(), 0);
}

/***********************************************************************
 *
 * FUNCTION:    HandleRecordButton
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HandleRecordButton(void)
{
    Boolean savedLaunchedFlag = false;
    Err retval;
    
    if (audioState == audioStatePlaying)
        return;

    WaveClose();

    if ((retval = WaveCreate(WAVE_FILE)) == errNone)
    {
        audioState = audioStateRecording;
        SetControls();

        if (launchedFromButton)
        {
            savedLaunchedFlag = true;
            SndPlaySystemSound(sndInfo);
            SysTaskDelay(8); 
        }    

        retval = WaveRecord();

        if (savedLaunchedFlag)
        {
            SndPlaySystemSound(sndInfo);
            SysTaskDelay(10);
            SndPlaySystemSound(sndInfo);
        }    

        WaveClose();
        
        if (retval == errNone)
        {
            if (WaveOpen(WAVE_FILE) == errNone)
            {
                audioState = audioStateOpen;
                SetSliderPercent(FrmGetActiveForm(), 0);
            }    
            else
                audioState = audioStateNoFile;
        }    
    }
    else
        audioState = audioStateNoFile;

    if (retval != errNone)
        ErrAlert(retval);

    SetSliderPercent(FrmGetActiveForm(), 0);
    SetControls();    
}

/***********************************************************************
 *
 * FUNCTION:    HandleProgress
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HandleProgress(EventPtr eventP)
{
    FormPtr  frmP;
    FieldPtr fldP;
    UInt8    percent;
    UInt16   minutes, seconds;

    percent = eventP->tapCount;
    minutes = eventP->screenX;
    seconds = eventP->screenY;

    frmP = FrmGetActiveForm();

    if (launchedFromButton)
    {
/*------------------------------------------------------------------------
 * If user let go of the Aux button
 *----------------------------------------------------------------------*/
        if (auxButtonPresent && ((KeyCurrentState() & keyBitAux) == 0))
        {
            launchedFromButton = false;
            WaveStop();
            SetSliderPercent(FrmGetActiveForm(), 0);
            return;
        }
    }

    if (audioState == audioStatePlaying)
        SetSliderPercent(frmP, percent);
    else if (audioState == audioStateRecording)
    {
        Char *buf, *tmp;
        
        buf = (Char *)MemPtrNew(20);
        StrPrintF(buf, "%02d:%02d", minutes, seconds);
        fldP = (FieldPtr)GetObjectPtr(MainProgressField);
        if ((tmp = FldGetTextPtr(fldP)) != NULL)
            MemPtrFree(tmp);
        FldSetTextPtr(fldP, buf);
        FldDrawField(fldP);
    }    

    EvtResetAutoOffTimer();
}

/***********************************************************************
 *
 * FUNCTION:    HandleSliderControl
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void HandleSliderControl(EventPtr eventP)
{
    static Err retval;
    
    retval = WaveSeek(eventP->data.ctlSelect.value);        // tapCount is percent

}

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleVolumeMounted
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
void MainFormHandleVolumeMounted (Boolean draw)
{
    if (WaveOpen(WAVE_FILE) == errNone)
        audioState = audioStateOpen;
    else
        audioState = audioStateNoFile;

    if (draw)
    {
        SetSliderPercent(FrmGetActiveForm(), 0);
        SetControls();
        FrmDrawForm(FrmGetActiveForm());
    }    
}

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleVolumeUnmounted
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
void MainFormHandleVolumeUnmounted(Boolean draw)
{
    audioState = audioStateNoVolume;

    if (draw)
    {
        SetControls();
        FrmDrawForm(FrmGetActiveForm());
    }    
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
    Coord x, y;
    Coord y_diff;
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
 * Calculate change, if none, do nothing 
 *----------------------------------------------------------------------*/
    y_diff = y - (r.topLeft.y + r.extent.y);
    if (y_diff == 0)
        return;
        
/*------------------------------------------------------------------------
 * Resize the form
 *----------------------------------------------------------------------*/
    r.extent.y += y_diff;
    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);

/*------------------------------------------------------------------------
 * Move the buttons.
 *----------------------------------------------------------------------*/
    PrvMoveObject(frmP, FrmGetObjectIndex(frmP, MainPlayButton), y_diff, draw);
    PrvMoveObject(frmP, FrmGetObjectIndex(frmP, MainPauseButton), y_diff, draw);
    PrvMoveObject(frmP, FrmGetObjectIndex(frmP, MainRecordButton), y_diff, draw);
    PrvMoveObject(frmP, FrmGetObjectIndex(frmP, MainStopButton), y_diff, draw);

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
    UInt32         features;
    VolumeInfoType info;

/*------------------------------------------------------------------------
 * Check to see if we were started using the Aux Button
 *----------------------------------------------------------------------*/
    if (auxButtonPresent && ((KeyCurrentState() & keyBitAux) != 0))
    {
        EventType       new_event;

        new_event.eType   = recordStartEvent;
        EvtAddEventToQueue(&new_event);
        launchedFromButton = true;
    }    
    else
        launchedFromButton = false;

    if (vgaPresent)
    {
        VgaFormModify(frmP, vgaFormModify160To240);
        MainFormResize(frmP, false);
    }    

/*------------------------------------------------------------------------
 * Check to see if unit has record capability
 *----------------------------------------------------------------------*/
    AudioGetSupportedFeatures(&features);

    if (features & audioFtrRecordWave)
        canRecord = true;
    else    
        canRecord = false;

    WaveInit();

    if (VFSVolumeInfo(VOLUME_REF_NUM, &info) == errNone)
    {
        if (WaveOpen(WAVE_FILE) == errNone)
        {
            audioState = audioStateOpen;
            SetSliderPercent(FrmGetActiveForm(), 0);
        }    
        else
            audioState = audioStateNoFile;
    }
    else
        audioState = audioStateNoVolume;

    SetControls();
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
    Boolean handled = false;
    UInt16  formID;
    FormPtr frmP;
    VgaScreenModeType   screenMode;
    VgaRotateModeType   rotateMode;

    formID = (FrmGetFormId (FrmGetActiveForm ()));

    switch (command)
    {
        case MainOptionsAboutExampleJ:
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
Boolean MainFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;

    frmP = FrmGetActiveForm();
 
    switch (eventP->eType) 
    {
        case menuEvent:
            return MainFormDoCommand(eventP->data.menu.itemID);

        case ctlSelectEvent :
            switch(eventP->data.ctlSelect.controlID)
            {
                case MainPlayButton :
                    HandlePlayButton();
                    handled = true;
                    break;
                case MainPauseButton :
                    HandlePauseButton();
                    handled = true;
                    break;
                case MainStopButton :
                    HandleStopButton();
                    handled = true;
                    break;
                case MainRecordButton :
                    HandleRecordButton();
                    handled = true;
                    break;
                case MainProgressSliderControl :
                    HandleSliderControl(eventP);
                    handled = true;
                    break;

            }        
            break;
        case frmOpenEvent:
            MainFormInit(frmP);
            FrmDrawForm (frmP);
            handled = true;
            break;

        case displayExtentChangedEvent :
            MainFormResize(frmP, true);
            break;

/*------------------------------------------------------------------------
 * Check to see if a card was removed or inserted while we were not the 
 * active form.
 *----------------------------------------------------------------------*/
        case winEnterEvent :
            if (cardChangedOccurred)
            {
                SetControls();
                MainFormResize(frmP, true);
                cardChangedOccurred = false;
            }
            break;    

        case frmCloseEvent :
            WaveClose();
            handled = true;
            break;

        case recordStartEvent :
            HandleRecordButton();
            break;

        case audioProgressEvent :
            HandleProgress(eventP);
            handled = true;
            break;

        default:
            break;
    }
    return(handled);
}


