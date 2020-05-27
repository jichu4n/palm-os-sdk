/******************************************************************************
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/
/**
 * @file  TicksTock.c
 * @brief Source code for the TickStock sample application.
 *
 * TickStock is a sample application that demonstrates:
 *
 * - mainly, how to detect and handle incoming call interruption
 * - how to detect if device is using EvDO connection
 * - downloading data while keeping UI responsive
 * - implementing [background] HTTP request as a state machine
 *
 * The application connects to Yahoo! Finance website and download
 * the HTTP content until it finds the latest price of the queried
 * stock.
 *
 * Based on BGStockTicker sample code.
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>

#include "TicksTock.h"
#include "TicksTock_Rsc.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

#define kAppGlobalFtrID         0x00            // store globals as feature memory
#define kUpdateSuspendedFtrID   0x02            // store suspended state as feature

#define kDisplayLatestPriceUpdateCode   0xFF    // custom form redraw code

/*********************************************************************
 * Global variables
 *********************************************************************/

// Different types of data services
const Char services[6][6] = {"None", "CSD", "1xRTT", "EVDO", "GPRS", "EDGE"};


/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/

static void DisplayProgressRect(Char *progressStr, UInt16 count)
{
    RectangleType rect = {{42, 120}, {76, 20}};
    Char str[256] = "";
    
    WinEraseRectangleFrame(roundFrame, &rect);
    WinEraseRectangle(&rect, 0);

    // StrPrintF(str, "%s %d", progressStr, count); // Print with retry count
    StrPrintF(str, "%s", progressStr);
    
    WinPaintRectangleFrame(roundFrame, &rect);
    WinPaintChars(str, StrLen(str), 52, 124);
    
    return;
}

static void DisplayNetworkStatus()
{
    UInt16 phnRefNum = sysInvalidRefNum;
    PhnDataSessionStateType state;
    RectangleType rect = {{133, 1}, {27, 12}};
    Err error = errNone;
    UInt32 deviceID = 0;
    
    // Only implemented starting from Treo 700p
    FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);

    if (deviceID != 'D052' /* Check device id in palmOneResources.h */)
    {
        // device is not running on a Treo 700p
        return;
    }
       
    HsGetPhoneLibrary(&phnRefNum);
    PhnLibOpen(phnRefNum);
    
    // Must fill out the version
    MemSet(&state, sizeof(PhnDataSessionStateType), 0);
    state.version = PhnDataSessionStateTypeVersion;
    error = PhnLibGetDataSessionInfo(phnRefNum, &state);
    
    PhnLibClose(phnRefNum);

    if (error)
    {
        WinEraseRectangle(&rect, 0);
        WinDrawChars("Unknown", StrLen("Unknown"), 133, 1);
    }
    else if (state.sessionStatus != phnPacketDataSessionNone)
    {
        WinEraseRectangle(&rect, 0);
        WinDrawChars(services[state.sessionType], StrLen(services[state.sessionType]), 133, 1);
    }
    
    return;
}

// Look for "Last Trade: " string and parse out the bold HTML tag.
// The price should follow after.
static Boolean ParseHTTPContent(MyAppGlobalType *globals)
{
    Char *pStart = NULL;
    Char *pEnd = NULL;
    Char *ptr = NULL;
    
	ptr = StrStr(globals->recvBuf.ptr, "Last Trade:");
	if(!ptr) return false;
	
	ptr = StrStr(ptr, "<b>");				
	ptr += 3;                   // move pointer to the end of <b>
	pStart = ptr;               // point to start of price string
	
	pEnd = StrStr(ptr, "</b>");
	if(!pEnd) return false;
	*pEnd = 0;                  // assign \0 to the end of price string
		
	StrCopy(globals->data[globals->company].price, pStart);

	return true;
}

#pragma mark -

static Err UpdateGlobals(MyAppGlobalType *globals)
{
    MemPtr pData = NULL;
    Err error = errNone;
    
    error = FtrGet(appFileCreator, kAppGlobalFtrID, (UInt32 *)&pData);
    if (error || !pData)
    {
        error = FtrPtrNew(appFileCreator, kAppGlobalFtrID, sizeof(MyAppGlobalType), &pData);
    }    
    
    if (pData)
        DmWrite(pData, 0, globals, sizeof(MyAppGlobalType));

    return error;
}

static Err GetGlobals(MyAppGlobalType *globals)
{
    Err error = errNone;
    MemPtr pData = NULL;
    MyAppGlobalType newGlobals;
    
   MemSet(&newGlobals, sizeof(MyAppGlobalType), 0);

    error = FtrGet(appFileCreator, kAppGlobalFtrID, (UInt32 *)&pData);
    if (error || !pData)
    {
        error = FtrPtrNew(appFileCreator, kAppGlobalFtrID, sizeof(MyAppGlobalType), &pData);
        if (pData)
        {
            DmWrite(pData, 0, &newGlobals, sizeof(MyAppGlobalType));
            *globals = newGlobals;
        }
    }
    else
    {
        if (!globals) ErrFatalDisplay("No container for global data");
        if (!pData) ErrFatalDisplay("pData = NULL");    
        MemMove(globals, pData, sizeof(MyAppGlobalType));
    }       
    return error;
}

static Err SetFieldTextFromStr(FieldPtr field, Char *s, Boolean redraw)
{
	MemHandle h;
	
	h = FldGetTextHandle(field);
	if(h)
	{
		Err err;
		FldSetTextHandle(field, NULL);
		err = MemHandleResize(h, StrLen(s)+1);
		if(err!=errNone)
		{
			FldSetTextHandle(field, h);
			return err;
		}
	}
	else
	{
		h = MemHandleNew(StrLen(s)+1);
		if(!h)
			return memErrNotEnoughSpace;
	}
	
	StrCopy((Char *)MemHandleLock(h), s);
	MemHandleUnlock(h);
	
	FldSetTextHandle(field, h);
	if(redraw)
		FldDrawField(field);
	
	return errNone;
}

/*
 * FUNCTION: GetObjectPtr
 *
 * DESCRIPTION:
 *
 * This routine returns a pointer to an object in the current form.
 *
 * PARAMETERS:
 *
 * formId
 *     id of the form to display
 *
 * RETURNED:
 *     address of object as a void pointer
 */

static void * GetObjectPtr(UInt16 objectID)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}

#pragma mark -

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


/*
 * FUNCTION: MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void MainFormInit(FormType *frmP)
{
    ControlType *pCheckbox = NULL;
    MyAppGlobalType globals;
    
    GetGlobals(&globals);
    
    pCheckbox = GetObjectPtr(MainAutoUpdateCheckbox);
    CtlSetValue(pCheckbox, globals.prefs.auto_update);
    return;
}

/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id
 */

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
									 appFileCreator, true, &cardNo, &dbID);
	switch (command)
	{
		case OptionsAboutTicksTock:
		{
			MenuEraseStatus(0);
			HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
			handled = true;
			break;
		}
	}

	return handled;
}

/*
 * FUNCTION: MainFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "MainForm" of this 
 * application.
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP;
    MyAppGlobalType globals;
    
    GetGlobals(&globals);
    
	switch (eventP->eType) 
	{
	    // Process HTTP request and response during nilEvent
	    case nilEvent:
	        return HTTPHandleEvent();
	        
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainRefreshButton));
			FrmDrawForm(frmP);
			MainFormInit(frmP);
            
            // Initialize the HTTP state to:
            // - No state
            // - Retry count is 0
            // - First company stock to query is 'Palm'
            
			globals.HTTPState = HTTPStateNone;
			globals.stateCount = 0;
			globals.company = Palm;

			PrvCommonInitGauge(frmP);
			handled = true;
			break;
            
        case frmUpdateEvent:
            if (eventP->data.frmUpdate.formID == MainForm &&
                eventP->data.frmUpdate.updateCode == kDisplayLatestPriceUpdateCode)
            {
                FieldType *pPalmFld = GetObjectPtr(MainPalmField);
                FieldType *pYhooFld = GetObjectPtr(MainYhooField);
                FieldType *pGoogFld = GetObjectPtr(MainGoogField);
                FieldType *pMsftFld = GetObjectPtr(MainMsftField);
                FieldType *pIntcFld = GetObjectPtr(MainIntcField);
                
                frmP = FrmGetActiveForm();
                FrmDrawForm(frmP);

                DisplayNetworkStatus();

                SetFieldTextFromStr(pPalmFld, globals.data[Palm].price, true);
                SetFieldTextFromStr(pYhooFld, globals.data[Yahoo].price, true);
                SetFieldTextFromStr(pGoogFld, globals.data[Google].price, true);
                SetFieldTextFromStr(pMsftFld, globals.data[Microsoft].price, true);
                SetFieldTextFromStr(pIntcFld, globals.data[Intel].price, true);

                handled = true;
            }
			break;
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == MainRefreshButton)
			{
                if (globals.HTTPState == HTTPStateNone)
                {
                    // Change HTTP state to Init to start initialization,
                    // after which the HTTP request is made
                    
        			globals.HTTPState = HTTPStateInit;
        			globals.stateCount = 0;
        			globals.company = Palm;

    			    handled = true;
    			}
			}
			else if (eventP->data.ctlSelect.controlID == MainAutoUpdateCheckbox)
			{
			    globals.prefs.auto_update =
			        (CtlGetValue(eventP->data.ctlSelect.pControl))?true:false;
			        
			    handled = true;
			}
			break;
		}
		
		case appStopEvent:
		    // If application is quitting, and state machine is still running,
		    // close down and clean up as necessary
		    if (globals.HTTPState != HTTPStateNone)
		    {
                if (globals.netLib.socket) CloseSocket(globals.netLib.refNum, globals.netLib.socket);
                if (globals.netLib.refNum) CloseNetLib(globals.netLib.refNum);
		    }
		    break;
		    
		default:
		    break;
	}
	
	if (handled)
	    UpdateGlobals(&globals);
    
	return handled;
}

#pragma mark -

Boolean HTTPHandleEvent()
{
    Boolean handled = true;
    Err error = errNone;
    MyAppGlobalType globals;
    Boolean done = false;
    UInt32 suspended = 0;
    HTTPStateType state;

    GetGlobals(&globals);

    state = globals.HTTPState;

    // Check whether the state is currently suspended.
    FtrGet(appFileCreator, kUpdateSuspendedFtrID, &suspended);
    if (suspended)
    {
        state = HTTPStateSuspended;
    }
    else
        globals.stateCount++;
    
    switch (state)
    {
        case HTTPStateInit:
            // Load NetLib and create socket. If success, clear retry count and
            // proceed to connect
            globals.netLib.loaded = InitNetLib(&globals.netLib.refNum, &error);
            if (!error)
            {
                DisplayNetworkStatus();
                
                error = OpenSocket(globals.netLib.refNum, &globals.netLib.socket, globals.prefs.blocking);
                if (!error)
                {
                    globals.HTTPState = HTTPStateConnect;
                    globals.stateCount = 0;
                }
            }
            break;
            
        case HTTPStateConnect:
            // Connect to Yahoo! Finance website. If connected, clear retry count and
            // proceed to send request
            DisplayProgressRect("Connecting", globals.stateCount);
            error = ConnectSocket(globals.netLib.refNum, globals.netLib.socket, QUOTE_SERVER);
            if (!error)
            {
                globals.HTTPState = HTTPStateSendRequest;
                globals.stateCount = 0;
            }
            break;
            
        case HTTPStateSendRequest:
        {
            // Send request string to webserver. When done, clear retry count and
            // proceed to receive response
            const Char symbols[5][5] = {"PALM", "YHOO", "GOOG", "MSFT", "INTC"};
            Char data[64] = "";
            UInt16 size = 0;
            
            DisplayProgressRect("Sending", globals.stateCount);
            StrPrintF(data, QUOTE_CMD, symbols[globals.company]);
            size = StrLen(data);
            
            done = SendData(globals.netLib.refNum, globals.netLib.socket, data, &size, &error);
            if (done)
            {
                globals.HTTPState = HTTPStateRecvHeader;
                globals.stateCount = 0;
            }
            break;
        }
        
        case HTTPStateRecvHeader:
            // Don't care about this for now; initiate download by de-initializing
            // the receive buffer so that it can be renewed later

            globals.recvBuf.ptr = NULL;
            globals.recvBuf.offset = 0;
            globals.recvBuf.size = 0;
            
            globals.HTTPState = HTTPStateRecvContent;
            globals.stateCount = 0;
            break;
            
        case HTTPStateRecvContent:
        {
            // Download and wait for data until one of the following is true:
            // - If buffer full, reset buffer and ask for more data
            // - If connection is closed remotely, parse data and move to close state
            // - If received data contains what we need, parse data and move to close state

            Boolean complete = false;
            
            DisplayProgressRect("Receiving", globals.stateCount);
            done = RecvData(globals.netLib.refNum, globals.netLib.socket,
                (void **)&globals.recvBuf.ptr, &globals.recvBuf.offset, &globals.recvBuf.size, &error);

            // Peek into buffer and check if it already contains the price data
            complete = ParseHTTPContent(&globals);
            if (complete || done)
            {
                globals.HTTPState = HTTPStateClose;
                globals.stateCount = 0;
            }
            
            // Clean up if buffer is full or if request is complete
            if (complete || done || globals.recvBuf.size == 0)
            {
                MemPtrFree(globals.recvBuf.ptr);
                globals.recvBuf.ptr = NULL;
            }
            break;
        }
        
        case HTTPStateClose:
            // Close socket and check if this is the last company stock
            // that we want to query. If not, open socket and connect again.
            // Otherwise, clean up and close NetLib and display the latest prices
            DisplayProgressRect("Closing", globals.stateCount);
            CloseSocket(globals.netLib.refNum, globals.netLib.socket);
            
            if ((globals.company + 1) == CompanyCount)
            {
                CloseNetLib(globals.netLib.refNum);
                globals.HTTPState = HTTPStateDone;
                globals.stateCount = 0;
            }
            else
            {
                // Get latest stock price for the next company
                globals.company++;
                error = OpenSocket(globals.netLib.refNum, &globals.netLib.socket, globals.prefs.blocking);
                if (!error)
                {
                    globals.HTTPState = HTTPStateConnect;
                    globals.stateCount = 0;
                }
                else
                {
                    globals.HTTPState = HTTPStateError;
                    globals.stateCount = 0;
                    globals.error = error;
                }
            }
            break;
                   
        case HTTPStateDone:
            // At this point, the latest prices of all companies have
            // been received
            FrmUpdateForm(MainForm, kDisplayLatestPriceUpdateCode);
            globals.HTTPState = HTTPStateNone;
            globals.stateCount = 0;
            break;
        
        case HTTPStateError:
        {
            Char errStr[8] = "";
            StrPrintF(errStr, "0x%x", globals.error);
            FrmCustomAlert(ErrorAlert, "Error code: ", errStr, 0);
            globals.HTTPState = HTTPStateNone;
            globals.stateCount = 0;
            break;
        }
        
        case HTTPStateSuspended:
            // If connection is suspended, clean up and close socket and NetLib.
            // It is up to applications how this should be handled. If the server is
            // capable of maintaining state of data transfer, then instead of cleaning up
            // application should save the state somewhere here.
            //
            // Please note that no data can be sent or received once the phone has switched to
            // data call.
            DisplayProgressRect("Cleaning up", globals.stateCount);
            if (globals.netLib.socket) CloseSocket(globals.netLib.refNum, globals.netLib.socket);
            if (globals.netLib.refNum) CloseNetLib(globals.netLib.refNum);
            globals.HTTPState = HTTPStateNone;
            globals.stateCount = 0;
            break;
        
        default:
            handled = false;
            break;
    }
    
    if (handled)
    {
        UpdateGlobals(&globals);
    }
    
    return handled;
}

void PhoneHandleEvent(PhnEventType *pEvent)
{
    UInt32 suspended = 0;
    
    switch(pEvent->eventType)
    {
        case phnEvtStartIncomingCall:
            // Set HTTP state to be suspended because there is an incoming call
            FtrSet(appFileCreator, kUpdateSuspendedFtrID, 0xFFFF);
            DisplayProgressRect("Incoming call!", 0);

            // One last call to HTTPHandleEvent() for clean-up
            // Next event in the event queue will be the appStopEvent
            // to switch to the phone app
            HTTPHandleEvent();
			break;
			
		case phnEvtDisconnectConf:
		case phnEvtDisconnectInd:
		    // If call is disconnected, and the app is in suspended mode, re-launch
		    // and retry/continue download
		    FtrGet(appFileCreator, kUpdateSuspendedFtrID, &suspended);
		    if (suspended)
		    {
		        UInt16 cardNo = 0;
		        LocalID dbID = 0;
		        
                FtrSet(appFileCreator, kUpdateSuspendedFtrID, (UInt32) 0);
		        SysCurAppDatabase(&cardNo, &dbID);
		        SysUIAppSwitch(cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL);
		    }
		    
		    break;
    }
    
    return;
}

#pragma mark -

/*
 * FUNCTION: AppHandleEvent
 *
 * DESCRIPTION: 
 *
 * This routine loads form resources and set the event handler for
 * the form loaded.
 *
 * PARAMETERS:
 *
 * event
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed
 *     to a higher level handler.
 */

static Boolean AppHandleEvent(EventType * eventP)
{
	UInt16 formId;
	FormType * frmP;

	if (eventP->eType == frmLoadEvent)
	{
		/* Load the form resource. */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		/* 
		 * Set the event handler for the form.  The handler of the
		 * currently active form is called by FrmHandleEvent each
		 * time is receives an event. 
		 */
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

		}
		return true;
	}

	return false;
}

/*
 * FUNCTION: AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.
 */

static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do 
	{
        //Slow down! So that data transfer is more interruptible
        //by phone call. This is used for DEMO purposes only!
        //Process network event every 3 secs
	
		EvtGetEvent(&event, SysTicksPerSecond() * 3);

		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				if (! AppHandleEvent(&event))
				{
    			    FrmDispatchEvent(&event);
				}
			}
		}
	} while (event.eType != appStopEvent);
}

/*
 * FUNCTION: AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * RETURNED:
 *     errNone - if nothing went wrong
 */

static Err AppStart(void)
{
    MyAppGlobalType globals;
	UInt16 prefsSize;
	UInt16 phnRefNum = 0;
	Err err = errNone;
	
	GetGlobals(&globals);
	
	/* Read the saved preferences / saved-state information. */
	prefsSize = sizeof(globals.prefs);
	if (PrefGetAppPreferences(
		appFileCreator, appPrefID, &globals.prefs, &prefsSize, true) == 
		noPreferenceFound)
	{
		/* no prefs; initialize pref struct with default values */
		globals.prefs.blocking = false;
		globals.prefs.timeout = -1;
		globals.prefs.auto_update = false;
	}

    // Register for phone events:
    err=HsGetPhoneLibrary(&phnRefNum);
    PhnLibOpen(phnRefNum);
    PhnLibRegister(phnRefNum, appFileCreator, phnServiceAll);
    PhnLibClose(phnRefNum);
    
    UpdateGlobals(&globals);
    
	return err;
}

/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

static void AppStop(void)
{
    MyAppGlobalType globals;
    
    GetGlobals(&globals);
    
	/* 
	 * Write the saved preferences / saved-state information.  This
	 * data will be saved during a HotSync backup. 
	 */
	PrefSetAppPreferences(
		appFileCreator, appPrefID, appPrefVersionNum, 
		&globals.prefs, sizeof(globals.prefs), true);
        
	/* Close all the open forms. */
	FrmCloseAllForms();
	
	UpdateGlobals(&globals);
}

#pragma mark -

/*
 * FUNCTION: RomVersionCompatible
 *
 * DESCRIPTION: 
 *
 * This routine checks that a ROM version is meet your minimum 
 * requirement.
 *
 * PARAMETERS:
 *
 * requiredVersion
 *     minimum rom version required
 *     (see sysFtrNumROMVersion in SystemMgr.h for format)
 *
 * launchFlags
 *     flags that indicate if the application UI is initialized
 *     These flags are one of the parameters to your app's PilotMain
 *
 * RETURNED:
 *     error code or zero if ROM version is compatible
 */

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	/* See if we're on in minimum required version of the ROM or later. */
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
	{
		if ((launchFlags & 
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
		{
			FrmAlert (RomIncompatibleAlert);

			/* Palm OS versions before 2.0 will continuously relaunch this
			 * app unless we switch to another safe one. */
			if (romVersion < kPalmOS20Version)
			{
				AppLaunchWithCommand(
					sysFileCDefaultApp, 
					sysAppLaunchCmdNormalLaunch, NULL);
			}
		}

		return sysErrRomIncompatible;
	}

	return errNone;
}

/*
 * FUNCTION: PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * 
 * PARAMETERS:
 *
 * cmd
 *     word value specifying the launch code. 
 *
 * cmdPB
 *     pointer to a structure that is associated with the launch code
 *
 * launchFlags
 *     word value providing extra information about the launch.
 *
 * RETURNED:
 *     Result of launch, errNone if all went OK
 */

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
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

			/* 
			 * start application by opening the main form
			 * and then entering the main event loop 
			 */
			FrmGotoForm(MainForm);
			AppEventLoop();

			AppStop();
			break;
			
		case phnLibLaunchCmdEvent:
		    PhoneHandleEvent((PhnEventType *)cmdPBP);
		    break;
	}

	return errNone;
}
