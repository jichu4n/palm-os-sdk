/******************************************************************************
 *
 * File: MainForm.c
 *
 * Project : Example H
 *
 *****************************************************************************/

#include <PalmOS.h>
#include <vfsmgr.h>
#include "StarterRsc.h"

#include "Vga.h"          
#include "Audio.h"
#include "MainForm.h"

/***********************************************************************
 *
 *   Local Types
 *
 ***********************************************************************/

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
extern Boolean vgaPresent;

/***********************************************************************
 *
 *   Local Constants
 *
 ***********************************************************************/
#define maxTableItems           13
#define tblNameCol              0x00

/***********************************************************************
 *
 *   Local Functions
 *
 ***********************************************************************/
static void *GetObjectPtr      (UInt16 objectID);
static void PrvMoveObject      (FormPtr frmP, UInt16 objIndex, Coord y_diff, Boolean draw);
static void PrvResizeObject    (FormPtr frmP, UInt16 objIndex, Coord y_diff, Boolean draw);
static void TableDrawItem      (void * tableP, Int16 row, Int16 column,  RectanglePtr bounds);
static void MainFormScrollLines(Int16 lines, Boolean force_redraw);


/***********************************************************************
 *
 *   Local Variables
 *
 ***********************************************************************/
static FontID currentFont;
static Int16  numTableItems;
static Int16  currentSelection;

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
 * FUNCTION:    PrvFrmGetGSI
 *
 * DESCRIPTION: FrmGetObjectIndex() does not work for the graffiti shift
 *              indicator.  Therefore, we must do it ourselves.
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
static UInt16 PrvFrmGetGSI(FormPtr frmP)
{
    UInt16 i, numObjects;
    
    numObjects = FrmGetNumberOfObjects(frmP);
    
    for (i=0; i<numObjects; i++)
    {
        if (FrmGetObjectType(frmP, i) == frmGraffitiStateObj)
            return(i);
    }
    return(-1);
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
 * FUNCTION:    PrvResizeObject
 *
 * DESCRIPTION: This routine moves an object vertically within a form.
 *
 * PARAMETERS:  
 *
 * RETURNED:    nothing
 *
 ***********************************************************************/
static void PrvResizeObject(FormPtr frmP, UInt16 objIndex, Coord y_diff, Boolean draw)
{
    RectangleType r;
    
    FrmGetObjectBounds(frmP, objIndex, &r);
    if (draw)
        WinEraseRectangle(&r, 0);
    r.extent.y += y_diff;
    FrmSetObjectBounds(frmP, objIndex, &r);
}

/***********************************************************************
 * FUNCTION:     HandleSelectItem
 * DESCRIPTION:
 * PARAMETERS:
 * RETURNED:
 ***********************************************************************/
static void HandleSelectItem(Int16 row)
{
    Char         buf[30];
    ScrollBarPtr barP;
    Int16        valueP, minP, maxP, pageSizeP;

    barP = GetObjectPtr(MainAppScrollBar);
    SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);
    currentSelection = row;
    StrPrintF(buf, "Line %d selected", currentSelection + valueP + 1);
    FrmCustomAlert(LineNumberAlert, buf, NULL, NULL);
    TblUnhighlightSelection(GetObjectPtr(MainAppTable));
    TblSelectItem(GetObjectPtr(MainAppTable), currentSelection, 0);
}

/***********************************************************************
 * FUNCTION:     HandlePrevKey
 * DESCRIPTION:
 * PARAMETERS:
 * RETURNED:
 ***********************************************************************/
static void HandlePrevKey(void)
{
    ScrollBarPtr barP;
    Int16        valueP, minP, maxP, pageSizeP;

    barP = GetObjectPtr(MainAppScrollBar);
    SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);

    if (currentSelection == -1)
        currentSelection = 0;
    else
    {
        if (currentSelection > 0)
            currentSelection --;
        else
        {
            if (valueP > minP)
                MainFormScrollLines(-1, true);
        }
    }        
                         
    TblSelectItem(GetObjectPtr(MainAppTable), currentSelection, 0);
}    

/***********************************************************************
 * FUNCTION:     HandleNextKey
 * DESCRIPTION:
 * PARAMETERS:
 * RETURNED:
 ***********************************************************************/
static void HandleNextKey(void)
{
    ScrollBarPtr barP;
    Int16        valueP, minP, maxP, pageSizeP;

    barP = GetObjectPtr(MainAppScrollBar);
    SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);

    if (currentSelection == -1)
        currentSelection = 0;
    else
    {
        if (currentSelection < pageSizeP - 1)
            currentSelection ++;
        else
        {
            if (valueP <  maxP)
                MainFormScrollLines(1, true);
        }
    }        
                         
    TblSelectItem(GetObjectPtr(MainAppTable), currentSelection, 0);
}    


/***********************************************************************
 * FUNCTION:     TableDrawItem
 * DESCRIPTION:
 * PARAMETERS:
 * RETURNED:
 ***********************************************************************/
static void TableDrawItem(void * tableP, Int16 row, Int16 column, RectanglePtr bounds)
{
    FontID         saveID;
    Int16          valueP, minP, maxP, pageSizeP;
    ScrollBarPtr   barP;
    Char           buf[20];

    WinEraseRectangle(bounds, 0);

    barP = GetObjectPtr(MainAppScrollBar);
    SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);
    row += valueP;

    if(row < numTableItems)
    {
        StrPrintF(buf, "Line %d", row + 1);

        saveID = FntSetFont(currentFont);
        WinDrawTruncChars(buf, StrLen(buf), bounds->topLeft.x, bounds->topLeft.y, TblGetColumnWidth((TablePtr) tableP, column));
        FntSetFont(saveID);
    }
}



/***********************************************************************
 * FUNCTION:     TableLoad
 * DESCRIPTION:
 * PARAMETERS:
 * RETURNED:
 ***********************************************************************/
static void TableLoad(Int16 listOffset)
{
    ScrollBarPtr    barP;
    Int16           listItems = 0;
    RectangleType   rect;
    Int16           running_total, row_height;
    Int16           tblRows, row, visibleRows;
    TablePtr        tableP;

    tableP  = GetObjectPtr(MainAppTable);
    tblRows = TblGetNumberOfRows(tableP);
    
    TblSetColumnUsable (tableP, tblNameCol, true);

    FntSetFont(currentFont);
    row_height = FntLineHeight();
    TblGetBounds(tableP, &rect);
    running_total = 0;
    visibleRows = 0;

    for(row=0; row< tblRows; row++)
    {
        TblSetRowHeight(tableP, row, row_height);

        running_total += row_height;

        if(row < numTableItems)
            TblSetRowUsable(tableP, row, running_total < rect.extent.y);
        else
            TblSetRowUsable(tableP, row, false);

        if (running_total < rect.extent.y)
            visibleRows++;

        TblMarkRowInvalid(tableP, row);
    }

    barP = GetObjectPtr(MainAppScrollBar);
    if(numTableItems > visibleRows)
        SclSetScrollBar(barP, listOffset, 0, numTableItems - visibleRows, visibleRows);
    else
        SclSetScrollBar(barP, 0, 0, 0, numTableItems);
    
}

/***********************************************************************
 * FUNCTION:     TableInit
 * DESCRIPTION:
 * PARAMETERS:
 * RETURNED:
 ***********************************************************************/
static void TableInit(void)
{
    ScrollBarPtr barP;
    Int16        tblRows, row, row_height;
    TablePtr     tableP;

    currentSelection = -1;

    tableP = GetObjectPtr(MainAppTable);
    tblRows = TblGetNumberOfRows(tableP);

    FntSetFont(currentFont);
        
    row_height    = FntLineHeight();

    for(row=0; row< tblRows; row++)
    {
        TblSetItemStyle(tableP, row, tblNameCol, customTableItem);
        TblSetRowHeight(tableP, row, row_height);
    }    

/*------------------------------------------------------------------------
 * Set the callback routine that draws the Name column.
 *----------------------------------------------------------------------*/
    TblSetCustomDrawProcedure (tableP, tblNameCol, TableDrawItem);

    barP = GetObjectPtr(MainAppScrollBar);
    SclSetScrollBar(barP, 0, 0, 0, 0);

}

/***********************************************************************
 * FUNCTION:    
 * DESCRIPTION:
 * PARAMETERS: 
 * RETURNED:   
 * REVISION HISTORY:
 ***********************************************************************/
static void MainFormScroll(Int16 newValue, Int16 oldValue, Boolean force_redraw)
{
    TablePtr    tableP;

    tableP = GetObjectPtr(MainAppTable);

    TblUnhighlightSelection(tableP);

    if(oldValue != newValue)
    {
        TableLoad(newValue);

        if(force_redraw)
            TblDrawTable(tableP);
    }

}

/***********************************************************************
 * FUNCTION:    
 * DESCRIPTION:
 * PARAMETERS: 
 * RETURNED:   
 * REVISION HISTORY:
 ***********************************************************************/
static void MainFormScrollLines(Int16 lines, Boolean force_redraw)
{
    ScrollBarPtr    barP;
    Int16           valueP, minP, maxP, pageSizeP;
    Int16           newValue;

    barP = GetObjectPtr(MainAppScrollBar);
    SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);

    TblUnhighlightSelection(GetObjectPtr(MainAppTable));

    //scroll up
    if(lines < 0)
    {
        //we are at the start
        if(valueP == minP)
            return;

        newValue = valueP + lines;
        if(newValue < minP)
            newValue = minP;
    }
    else
    {
        if(valueP == maxP)
            return;

        newValue = valueP + lines;
        if(newValue  > maxP)
            newValue = maxP;
    }

    SclSetScrollBar(barP, newValue, minP, maxP, pageSizeP);
    MainFormScroll(newValue, valueP, force_redraw);

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
    Int16         valueP, minP, maxP, pageSizeP;
    ScrollBarPtr  barP;
    UInt16        objIdx;
    Coord         x, y;
    Coord         y_diff;
    RectangleType r, erase_rect;
    
/*------------------------------------------------------------------------
 * Get the new extent
 *----------------------------------------------------------------------*/
    WinGetDisplayExtent(&x, &y);
    
/*------------------------------------------------------------------------
 * Get the old extent
 *----------------------------------------------------------------------*/
    FrmGetFormBounds(frmP, &r);
    
/*------------------------------------------------------------------------
 * Calculate change
 *----------------------------------------------------------------------*/
    y_diff = y - (r.topLeft.y + r.extent.y);

/*------------------------------------------------------------------------
 * If the silkscreen was maximized, erase the area under maxmized silkscreen
 *----------------------------------------------------------------------*/
    if (draw  &&  (y_diff < 0))
    {
        erase_rect           = r;
        erase_rect.topLeft.y = r.extent.y + y_diff;
        erase_rect.extent.y  = -y_diff;
        WinEraseRectangle(&erase_rect, 0);
    }    
        
/*------------------------------------------------------------------------
 * Resize the form
 *----------------------------------------------------------------------*/
    r.extent.y += y_diff;
    WinSetWindowBounds(FrmGetWindowHandle(frmP), &r);

/*------------------------------------------------------------------------
 * Move Bottom Controls Button
 *----------------------------------------------------------------------*/
    PrvMoveObject(frmP, FrmGetObjectIndex(frmP, MainHelpButton), y_diff, draw);
    PrvMoveObject(frmP, FrmGetObjectIndex(frmP, MainTextField), y_diff, draw);

/*------------------------------------------------------------------------
 * Move the GSI (graffiti shift indicator)
 * Need to set the GSI's location as well as it's location on the form
 *----------------------------------------------------------------------*/
    objIdx = PrvFrmGetGSI(frmP);
    PrvMoveObject       (frmP, objIdx, y_diff, draw);
    FrmGetObjectPosition(frmP, objIdx, &x, &y);
    GsiSetLocation      (x, y+y_diff);

/*------------------------------------------------------------------------
 * Resize Scrollbar
 *----------------------------------------------------------------------*/
    PrvResizeObject(frmP, FrmGetObjectIndex(frmP, MainAppScrollBar), y_diff, draw);

/*------------------------------------------------------------------------
 * Resize the table
 *----------------------------------------------------------------------*/
    PrvResizeObject(frmP, FrmGetObjectIndex(frmP, MainAppTable), y_diff, draw);

    if (draw)
    {
        barP = GetObjectPtr(MainAppScrollBar);
        SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);

        TableLoad(valueP);
        FrmDrawForm(frmP);

        currentSelection = -1;
    }    
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
    TableInit();

    numTableItems = maxTableItems;
    currentFont   = stdFont;

    if (vgaPresent)
    {
        currentFont = VgaBaseToVgaFont(currentFont);
        VgaTableUseBaseFont((TablePtr)GetObjectPtr(MainAppTable), false);
        VgaFormModify(frmP, vgaFormModify160To240);
        MainFormResize(frmP, false);
    }    

    TableLoad(0);
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
    FontID              newFont;
    FormPtr             frmP;
    VgaScreenModeType   screenMode;
    VgaRotateModeType   rotateMode;

    formID = (FrmGetFormId (FrmGetActiveForm ()));

    switch (command)
    {
        case MainOptionsAboutExampleH:
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

        case MainOptionsFont :
            TblUnhighlightSelection(GetObjectPtr(MainAppTable));
            currentSelection = -1;
            newFont          = FontSelect(currentFont);
/*------------------------------------------------------------------------
 * Note: There is a bug in the OS which causes large Table Fonts to not 
 *       highlight properly when selected.  The work around is to implement 
 *       the selection yourself.  For simplicity sake, this example 
 *       does not do this.
 *----------------------------------------------------------------------*/
            currentFont      = newFont;
            if (vgaPresent)
            {
                if (VgaIsVgaFont(currentFont))
                    VgaTableUseBaseFont((TablePtr)GetObjectPtr(MainAppTable), false);
                else    
                    VgaTableUseBaseFont((TablePtr)GetObjectPtr(MainAppTable), true);
            }        
            MainFormResize(FrmGetActiveForm(), true);
            handled     = true;
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
    ScrollBarPtr barP;
    Int16        valueP, minP, maxP, pageSizeP;

 
    switch (eventP->eType) 
    {
        case menuEvent:
            return MainFormDoCommand(eventP->data.menu.itemID);

        case ctlSelectEvent :
            switch(eventP->data.ctlSelect.controlID)
            {
                case MainHelpButton :
                    FrmHelp(HelpString);
                    handled = true;
                    break;
            }        
            break;
        case frmOpenEvent:
            frmP = FrmGetActiveForm();
            MainFormInit(frmP);
            FrmDrawForm (frmP);
            handled = true;
            break;

        case displayExtentChangedEvent :
            MainFormResize(FrmGetActiveForm(), true);
            break;

        case frmCloseEvent :
            handled = true;
            break;

        case sclRepeatEvent:
            currentSelection = -1;
            MainFormScroll(eventP->data.sclRepeat.newValue, eventP->data.sclRepeat.value, true);
            break;

        case tblSelectEvent :
            HandleSelectItem(eventP->data.tblSelect.row);
            break;

        case keyDownEvent:
            barP = GetObjectPtr(MainAppScrollBar);
            SclGetScrollBar(barP, &valueP, &minP, &maxP, &pageSizeP);

            switch (eventP->data.keyDown.chr)
            {
                case vchrPageUp :
                    currentSelection = -1;
                    MainFormScrollLines(-pageSizeP, true);
                    handled = true;
                    break;
                case vchrPageDown :
                    currentSelection = -1;
                    MainFormScrollLines(pageSizeP, true);
                    handled = true;
                    break;
                case vchrPrevField :
                    HandlePrevKey();
                    handled = true;
                    break;
                case vchrNextField :
                    HandleNextKey();
                    handled = true;
                    break;
                case chrCarriageReturn :
                    if (currentSelection != -1)
                        HandleSelectItem(currentSelection);
                    handled = true;
                    break;
            }   
            break;

        default:
            break;
    }
    return(handled);
}


