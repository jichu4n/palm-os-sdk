
/***********************************************************************
 *
 * Copyright (c) 2000 Handspring, Inc.
 * All rights reserved.
 *
 * PROJECT:  HelperSender
 * FILE:     HelperSender.c
 * AUTHOR:   Frank Voqui: Oct 20, 2003
 *
 * DECLARER: HelperSender
 *
 * DESCRIPTION:
 *	  
 *
 *    20-Oct-2003  FHV   Modified from SV's Helper API test applications.
 **********************************************************************/
#include <PalmOS.h>
#include <SysEvtMgr.h>
#include <Helper.h>
#include <HelperServiceClass.h>
#include <HsNav.h>

#include <68K\Hs.h>
#include "HelperSender.h"



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

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
enum button {sms, email, web};
enum button buttonHighlight;

/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			   'TRHS'


// Until the new Helper Service Classes are defined, we define them here
// so implementation for the Default Apps panel can continue.  Remove these
// service class ID definitions when HelperServiceClass.h has been updated.
#define kHelperServiceClassIDURL	'url_'


// Define the minimum OS version we support
#define OUR_MIN_VERSION	\
		sysMakeROMVersion(2,0,0,sysROMStageRelease,0)

#define VALID_LAUNCH_FLAGS \
		(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)

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
 ***********************************************************************/
static Err 
RomVersionCompatible (
		UInt32 							requiredVersion, 
		UInt16							launchFlags )
{
UInt32 romVersion;

// See if we're on in minimum required version of the ROM or later.
FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
if (romVersion < requiredVersion)
	{
	if ((launchFlags & VALID_LAUNCH_FLAGS) == VALID_LAUNCH_FLAGS)
		{
		FrmAlert (RomIncompatibleAlert);
	
		// Pilot 1.0 will continuously relaunch this app unless  
		// we switch to another safe one.
		if (romVersion < OUR_MIN_VERSION)
			AppLaunchWithCommand(sysFileCDefaultApp, 
					sysAppLaunchCmdNormalLaunch, NULL);
		}
	
	return (sysErrRomIncompatible);
	}

return 0;
}



/***********************************************************************
 *
 * FUNCTION:    PrvCreateFieldTextHandle
 *
 * DESCRIPTION: Allocate and/or initialize a text handle
 *				for a field object.
 *
 * PARAMETERS:  fldP - a referece to a field object.
 *				size - the size of the text handle.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void
PrvCreateFieldTextHandle (
			FieldPtr 						fldP, 
			UInt32 							size )
{
Err err = errNone;
MemHandle handH = FldGetTextHandle(fldP);
if (handH)
	{
	err = MemHandleResize(handH, size);
	}
else
	{
	handH = MemHandleNew(size);
	}
if (err == errNone)
{	
	MemSet(MemHandleLock(handH), sizeof(UInt8) * size, 0);
	MemHandleUnlock(handH);
	FldSetTextHandle(fldP, handH);
	FldSetTextAllocatedSize(fldP, size);
}
}

#if 0
/***********************************************************************
 *
 * FUNCTION:    PrvSetTextField
 *
 * DESCRIPTION: Initializes the text handle and sets the string.
 *
 * PARAMETERS:  fldP - a reference to the field object.
 *				textP - a pointer to a string array.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void
PrvSetTextField(
			FieldPtr					fldP,
			Char * 						textP )
{
MemHandle stringH;
UInt16 len;

len = StrLen(textP);
PrvCreateFieldTextHandle(fldP, len + 1);
stringH = FldGetTextHandle(fldP);
StrNCopy(MemHandleLock(stringH), textP, len);
MemHandleUnlock(stringH);
FldSetTextHandle(fldP, stringH);
}
#endif



/***********************************************************************
 *
 * FUNCTION:    PrvRegisterNotification
 *
 * DESCRIPTION:  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    Err
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err
PrvRegisterNotification (
		void )
{
UInt16 cardNo;
LocalID dbID;
Err err = 0;

// Get information about this application to send to Notification
// manager calls...
SysCurAppDatabase (&cardNo, &dbID);

err = SysNotifyRegister (cardNo, dbID, sysNotifyHelperEvent, 
                       NULL /*callbackP*/, 
                       sysNotifyNormalPriority, 
                       NULL /*userDataP*/);
if (err)
	{
	Char buffer[32];
	StrIToH(buffer, err);
	FrmCustomAlert(RscRegisterFailedAlert, buffer, NULL, NULL);
	}

return err;
}


/***********************************************************************
 *
 * FUNCTION:    PrvUnregisterNotification
 *
 * DESCRIPTION: Unregister this application from the notification
 *				manager.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    Err
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err
PrvUnregisterNotification (
		void )
{
UInt16 cardNo;
LocalID dbID;
Err err;

// Get information about this application to send to Notification
// manager calls...
SysCurAppDatabase (&cardNo, &dbID);

err = SysNotifyUnregister (cardNo, dbID, sysNotifyHelperEvent,
                         sysNotifyNormalPriority);
if (err)
	{
	Char buffer[32];
	StrIToH(buffer, err);
	FrmCustomAlert(RscUnregisterFailedAlert, buffer, NULL, NULL);
	}

return err;
}


/***********************************************************************
 *
 * FUNCTION:    PrvBroadcastHelperAction
 *
 * DESCRIPTION: Send the Helper Enumeration code.
 *
 * PARAMETERS:  details - structure containing the body of the
 *							broadcast.
 *
 * RETURNED:    Boolean
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean
PrvBroadcastHelperAction (
		void * 							details )
{
SysNotifyParamType note;
Err err = 0;

MemSet(&note, sizeof(note), 0);

note.notifyType = sysNotifyHelperEvent;
note.notifyDetailsP = details;
note.handled = false;

err = SysNotifyBroadcast(&note);
if (err)
	{
	Char buffer[32];
	StrIToH(buffer, err);
	FrmCustomAlert(RscNotificationFailedAlert, buffer, NULL, NULL);
	}

return note.handled;
}



/***********************************************************************
 *
 * FUNCTION:    PrvInvokeHelperService
 *
 * DESCRIPTION: Invoke the helper service.
 *
 * PARAMETERS:  serviceClassID - the helper service class ID 
 *				dataP - pointer to data for the dataP field in the
 *							HelperNotifyEventType structure.
 *				displayedNameP - a pointer to the name.
 *				detailsP - a pointer to 'details' data for the
 *							'detailsP' field in the HelperNotifyEventType
 *							structure.
 *
 * RETURNED:    Err
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err
PrvInvokeHelperService(
			UInt32						serviceClassID,
			Char *						dataP,
			Char *						displayedNameP,
			void *						detailsP )
{
Err err = 0;
HelperNotifyExecuteType execute;
HelperNotifyEventType event;
Boolean handled = false;

	{
	MemSet(&event, sizeof(event), 0);
	MemSet(&execute, sizeof(execute), 0);
	
	execute.dataP = dataP;
	execute.detailsP = detailsP;
	execute.displayedName = displayedNameP;
	execute.helperAppID = 0;					// All helpers.
	execute.serviceClassID = serviceClassID;
	execute.err = 0;

	event.version = kHelperNotifyCurrentVersion;
	event.actionCode = kHelperNotifyActionCodeExecute;
	event.data.executeP = &execute;
	
	handled = PrvBroadcastHelperAction(&event);
	if (!handled)
		{
		FrmCustomAlert(RscNotHandledAlert, "Execute", NULL, NULL);
		}
	else if (event.data.executeP->err)
		{
		Char buffer[32];
		StrIToH(buffer, err);
		FrmCustomAlert(RscNotificationFailedAlert, buffer, NULL, NULL);
		}
	}

return err;
}



/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:  frmP - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void 
MainFormInit (
		FormPtr 						frmP )
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
static Boolean 
MainFormDoCommand (
		UInt16 							command )
{
Boolean handled = false;

UInt16  cardNo;
LocalID dbID;
DmSearchStateType searchState;

DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);

switch (command)
	{
	case MainOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2003", "Handspring DTS Team");
		handled = true;
		break;

	default:
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
static Boolean 
MainFormHandleEvent (
		EventPtr 						eventP )
{
Boolean handled = false;
FormPtr frmP;

frmP = FrmGetActiveForm();

switch (eventP->eType) 
	{
	case menuEvent:
		handled = MainFormDoCommand(eventP->data.menu.itemID);
		break;

	case frmOpenEvent:
		MainFormInit( frmP);
		FrmDrawForm ( frmP);
		
		if(buttonHighlight == sms)
		  FrmNavObjectTakeFocus(frmP, MainHelperSMSSelectButton);
		else if(buttonHighlight == email)
		  FrmNavObjectTakeFocus(frmP, MainHelperMailSelectButton);
		else if(buttonHighlight == web) 
		  FrmNavObjectTakeFocus(frmP, MainHelperWebSelectButton);
		
		handled = true;
		break;
	
	case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
			{
			case MainHelperSMSSelectButton:
				// Bring up the appropriate GUI...
				FrmPopupForm(SMSForm);
				buttonHighlight = sms;
				handled = true;
				break;

			case MainHelperMailSelectButton:
				// Bring up the appropriate GUI...
				FrmPopupForm(MailForm);
				buttonHighlight = email;
				handled = true;
				break;

			case MainHelperWebSelectButton:
				// Bring up the appropriate GUI...
				FrmPopupForm(WebForm);
                buttonHighlight = web;
				handled = true;
				break;
				
			default:
				break;
			}
		break;
	

	default:
		break;
	
	}

return handled;
}


/***********************************************************************
 *
 * FUNCTION:    WebFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "WebForm" of this application.
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
static Boolean 
WebFormHandleEvent (
		EventPtr 						eventP )
{
Err err = 0;
Boolean handled = false;
FormPtr frmP;
FieldPtr fldP;
Char * text;

frmP = FrmGetActiveForm();
fldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, WebURLField));

switch (eventP->eType) 
	{
	case frmOpenEvent:
		PrvCreateFieldTextHandle(fldP, 80);
		FrmDrawForm(frmP);
		FrmNavObjectTakeFocus(frmP, WebURLField);
		handled = true;
		break;
	
	case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
			{
			case WebCancelButton:
				// Bring up the appropriate GUI...
				FrmGotoForm(MainForm);
				handled = true;
				break;
			
			case WebGoButton:
				{
				MemHandle textH = FldGetTextHandle(fldP);
				text = MemHandleLock(textH);
				if (StrLen(text))
					{
					err = PrvInvokeHelperService(kHelperServiceClassIDURL, 
							text, "Helper Sender", 
							NULL);
					if (err)
						{
						Char buffer[32];
						StrIToH(buffer, err);
						FrmCustomAlert(RscNotificationFailedAlert, buffer, NULL, NULL);
						}
					}
				MemHandleUnlock(textH);
				}
				handled = true;
				break;

			default:
				break;
			}
		break;
	
	default:
		break;
	
	}

return handled;
}


/***********************************************************************
 *
 * FUNCTION:    MailFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MailForm" of this application.
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
static Boolean 
MailFormHandleEvent (
		EventPtr 							eventP )
{
Boolean handled = false;
FormPtr frmP;
FieldPtr toFldP;
FieldPtr ccFldP;
FieldPtr subjectFldP;
FieldPtr messageFldP;
Err err = 0;

frmP = FrmGetActiveForm();

toFldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MailToField));
ccFldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MailCCField));
messageFldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MailMessageField));
subjectFldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MailSubjectField));


switch (eventP->eType) 
	{
	case frmOpenEvent:
		PrvCreateFieldTextHandle(toFldP, 2048);
		PrvCreateFieldTextHandle(ccFldP, 2048);
		PrvCreateFieldTextHandle(messageFldP, 16384);
		PrvCreateFieldTextHandle(subjectFldP, 2048);
		
		FrmDrawForm(frmP);
		FrmNavObjectTakeFocus(frmP, MailToField);
		handled = true;
		break;
	
	case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
			{
			case MailCancelButton:
				// Bring up the appropriate GUI...
				FrmGotoForm(MainForm);
				handled = true;
				break;
			
			case MailSendButton:
				{
				MemHandle toH;
				MemHandle ccH;
				MemHandle messageH;
				MemHandle subjectH;
				Char * text;
				
				HelperServiceEMailDetailsType mailDetails;

				toH = FldGetTextHandle(toFldP);
				ccH = FldGetTextHandle(ccFldP);
				messageH = FldGetTextHandle(messageFldP);
				subjectH = FldGetTextHandle(subjectFldP);
				
				MemSet(&mailDetails, sizeof(mailDetails), 0);
				
				mailDetails.version = 1;
				
				text = MemHandleLock(ccH);
				if (StrLen(text))
					{
					mailDetails.cc = text;
					}
				text = MemHandleLock(subjectH);
				if (StrLen(text))
					{
					mailDetails.subject = text;
					}
				text = MemHandleLock(messageH);
				if (StrLen(text))
					{
					mailDetails.message = text;
					}
				text = MemHandleLock(toH);
				if (StrLen(text))
					{
					err = PrvInvokeHelperService(kHelperServiceClassIDEMail, 
							text, "Helper Sender", 
							&mailDetails);
					}
					
				MemHandleUnlock(toH);
				MemHandleUnlock(ccH);
				MemHandleUnlock(messageH);
				MemHandleUnlock(subjectH);
				
				if (err)
					{
					Char buffer[32];
					StrIToH(buffer, err);
					FrmCustomAlert(RscNotificationFailedAlert, buffer, NULL, NULL);
					}
				}
				handled = true;
				break;

			default:
				break;
			}
		break;
	
	default:
		break;
	}

return handled;
}



/***********************************************************************
 *
 * FUNCTION:    SMSFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "SMSForm" of this application.
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
static Boolean 
SMSFormHandleEvent (
		EventPtr 						eventP )
{
Boolean handled = false;
FormPtr frmP;
FieldPtr toFldP;
FieldPtr messageFldP;

frmP = FrmGetActiveForm();
toFldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SMSToField));
messageFldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SMSMessageField));

switch (eventP->eType) 
	{
	case frmOpenEvent:
		PrvCreateFieldTextHandle(toFldP, 2048);
		PrvCreateFieldTextHandle(messageFldP, 640);
		FrmDrawForm(frmP);
		FrmNavObjectTakeFocus(frmP, SMSToField);
		handled = true;
		break;
	
	case ctlSelectEvent:
		switch (eventP->data.ctlSelect.controlID)
			{
			case SMSCancelButton:
				// Bring up the appropriate GUI...
				FrmGotoForm(MainForm);
				handled = true;
				break;
			
			case SMSSendButton:
				{
				Err err = 0;
				MemHandle toH;
				MemHandle messageH;
				Char * toP;
				Char * messageP;
				
				HelperServiceSMSDetailsType smsDetails;
				
				toH = FldGetTextHandle(toFldP);
				toP = MemHandleLock(toH);
				if (StrLen(toP))
					{
					MemSet(&smsDetails, sizeof(smsDetails), 0);

					smsDetails.version = 1;
	
					messageH = FldGetTextHandle(messageFldP);
					messageP = MemHandleLock(messageH);
					if (StrLen(messageP))
						{
						smsDetails.message = messageP;
						}
					else
						{
						smsDetails.message = NULL;
						}
					
					err = PrvInvokeHelperService(kHelperServiceClassIDSMS, 
							toP, "Helper Sender", 
							&smsDetails);

					MemHandleUnlock(messageH);
					
					if (err)
						{
						Char buffer[32];
						StrIToH(buffer, err);
						FrmCustomAlert(RscNotificationFailedAlert, buffer, NULL, NULL);
						}
					}
				MemHandleUnlock(toH);
				}
				handled = true;
				break;

			default:
				break;
			}
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
static Boolean 
AppHandleEvent (
		EventPtr 						eventP )
{
UInt16 formId;
FormPtr frmP;
Boolean handled;


handled = false;
if (eventP->eType == frmLoadEvent)
	{
	// Load the form resource.
	formId = eventP->data.frmLoad.formID;
	frmP = FrmInitForm(formId);
	FrmSetActiveForm(frmP);

	// Set the event handler for the form.  The handler of the 
	// currently active form is called by FrmHandleEvent each time
	// is receives an event.
	switch (formId)
		{
		case MainForm:
			FrmSetEventHandler(frmP, MainFormHandleEvent);
			break;
		
		case MailForm:
			FrmSetEventHandler(frmP, MailFormHandleEvent);
			break;
		
		case SMSForm:
			FrmSetEventHandler(frmP, SMSFormHandleEvent);
			break;

		case WebForm:
			FrmSetEventHandler(frmP, WebFormHandleEvent);
			break;
		
		default:
			ErrFatalDisplay("Invalid Form Load Event");
			break;

		}
	handled = true;
	}

return handled;
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
static void 
AppEventLoop (
		void )
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

	}
while (event.eType != appStopEvent);
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
static Err 
AppStart (
		void )
{
	Err err = 0;

    buttonHighlight = sms;
    
	// Register for notifications.
	err = PrvRegisterNotification();

	return err;
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
static void 
AppStop (
		void )
{

	FrmCloseAllForms();

	PrvUnregisterNotification();
}


/***********************************************************************
 *
 * FUNCTION:    StarterPilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with
 *					the launch code. 
 *              launchFlags -  word value providing extra information
 *					about the launch.
 *
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static UInt32 
StarterPilotMain (
		UInt16 							cmd, 
		MemPtr 							cmdPBP, 
		UInt16 							launchFlags )
{
Err error;

	error = RomVersionCompatible(OUR_MIN_VERSION, launchFlags);
	if (error) goto Exit;

	
	switch (cmd)
		{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error)
				{
				return error;
				}

			FrmGotoForm(MainForm);
			AppEventLoop();
			AppStop();
			break;

		case sysAppLaunchCmdNotify:
			{
			SysNotifyParamType* paramP = (SysNotifyParamType*) cmdPBP;

			// Look for notifications from the helper manager...
			if (!error && paramP->notifyType == sysNotifyHelperEvent)
				{
				if (((HelperNotifyEventType*)paramP->notifyDetailsP)->actionCode != kHelperNotifyActionCodeEnumerate)
					{
					/* Put diagnostics here if needed */ ;
					}
				}
			}
			break;

		default:
			break;
		}

	Exit:
	return error;
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
UInt32 
PilotMain ( 
		UInt16 							cmd, 
		MemPtr							cmdPBP, 
		UInt16							launchFlags )
{
return StarterPilotMain(cmd, cmdPBP, launchFlags);
}

