/** 
 * \file   SimpleSMS.c
 *
 * SimpleSMS
 *
 * This is the main source file for the Simple SMS App. This
 * code demonstrates how to write your own SMS application. The app
 * will register with the Phone Library, send a custom message, and
 * display a dialog for incoming messages.
 *
 * \license
 * 
 * Copyright (c) 2002 Handspring Inc., All Rights Reserved
 *
 * $Id:$
 *
 ***************************************************************/
#include <PalmOS.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>

#include "SimpleSMS.h"

// -------------------------------------------------------------
// Constants
// -------------------------------------------------------------
#define appFileCreator			   'HSMS'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01
#define kMinVersionSupported		0x3523040
// -------------------------------------------------------------
// Structures
// -------------------------------------------------------------
#define serviceCenterSize		40
#define addressSize				40
#define messageSize				160

typedef struct 
{
	char address[addressSize + 1];
	char message[messageSize + 1];
} SimpleSMSPreferenceType;

typedef struct NBSNotificationEventType PhnNBSNotificationEventType;
// -------------------------------------------------------------
// Globals
// -------------------------------------------------------------
static SimpleSMSPreferenceType smsPrefs;
static Boolean smsLibLoaded;
static UInt16 smsLibRef;

// -------------------------------------------------------------
// Prototypes
// -------------------------------------------------------------
static Err		StartApplication (void);
static void		EventLoop (void);
static void		StopApplication (void);
static Boolean	SimpleSMSEvent (EventPtr event);

static UInt32	HandlePhoneLibraryEvent (MemPtr cmdPBP, UInt16 launchFlags);
static Err		LoadPhoneLibrary (UInt16* libRefP, Boolean* libLoadedP);
static Err		SMSRegister (Boolean bReg);
static void		SendTheMessage (void);
static void		DisplayMessage (UInt32 messageID);
static void		TurnOnOptionLock (void);
static void*	GetObjectPtr (UInt16 objectID);
static void		CopyFromGlobalToFields (void);
static UInt32		CopyFromFieldsToGlobal (void);

static void     HandleNotificationEvent ( MemPtr cmdPBP, UInt16 launchFlags);

// -------------------------------------------------------------
// Functions
// -------------------------------------------------------------

/****************************************************************/
/* PilotMain */
/**
 * Main entry point
 *
 * \param   cmd			  IN    Value specifying the launch code. 
 * \param   cmdPBP		  IN	Pointer to a structure that is associated with the launch code. 
 * \param	launchFlags	  IN	Value providing extra information about the launch.
 *
 * \retval  UInt32
 *
 ****************************************************************/
UInt32
PilotMain (UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  UInt32 error;

  switch (cmd)
	{
	  case sysAppLaunchCmdNormalLaunch:
		// normal application launch
		error = StartApplication ();
		if (error) 
		  return error;

		EventLoop ();

		StopApplication ();
		break;

	  case phnLibLaunchCmdRegister:   
	  	// Received when device is reset (or the library is loaded ?)
	  	// DbgBreak();
		// SMSRegister (true);
		break;

	  case 0x7000:	// For Treo 300 workaround. Currently the launch code is different on Treo 300 than 180/270
	  case phnLibLaunchCmdEvent:
		// we have an event from the Phone library, so handle it
		error = HandlePhoneLibraryEvent (cmdPBP, launchFlags);
		if (error) 
		  return error;
		break;

	  case sysAppLaunchCmdNotify:
		HandleNotificationEvent (cmdPBP, launchFlags);
		break; 
	  

	  default:
		break;
	}

  return errNone;
}

/****************************************************************/
/* StartApplication */
/**
 * Application start 
 *
 * \retval  Err
 *
 ****************************************************************/
static Err
StartApplication (void)
{
	Err		error = 0;
	char 	errMsg[10];

	smsLibLoaded = false;
	smsLibRef = 0;


#if 0  // This block is not really neccessary since we use the Handspring/Treo's phone libraries
	   // whick guarantees the device is (should be) a Treo.

	UInt16	prefsSize;
	void*	procP;
	UInt32	value;

	// Make sure that we are running on the Treo
	// Make sure that we are at the appropriate Palm OS version
	if (!FtrGet (hsFtrCreator, hsFtrIDVersion, &value))
	{
		if (value < kMinVersionSupported)
		{
			FrmCustomAlert (resAlertPhoneLibrary, "PalmOS version too low", errMsg, NULL);
			return 1;
		}
	}
	else
	{
		FrmCustomAlert (resAlertPhoneLibrary, "No hsFtrIDVersion", errMsg, NULL);
		return 1;
	}

  // Make sure that we support the HsIndicator function call.
  // We use this test to make sure that we are on a Treo device.
  procP = HsGetTrapAddress (hsSelIndicator);
  if (procP && procP != HsGetTrapAddress(hsSelUnimplemented))
	{
	  // Read the saved preferences / saved-state information.
	  prefsSize = sizeof(SimpleSMSPreferenceType);
	  if (PrefGetAppPreferences (appFileCreator, appPrefID, &smsPrefs, &prefsSize, true) != noPreferenceFound)
	  	;
	}
  else
	{
	  // Display the error dialog and return with error
	FrmCustomAlert (resAlertPhoneLibrary, "No hsSelIndicator proc", errMsg, NULL);
	  return 1;
	}

#endif

  
  // load the Phone Library
	error = LoadPhoneLibrary (&smsLibRef, &smsLibLoaded);
	if (error)
	{
		StrIToA(errMsg, error);
		FrmCustomAlert (resAlertPhoneLibrary, "Can't Load the Phone Library", errMsg, NULL);
		return 1;
	}
	else
	{	// We were able to load the library
		// Now, let's register with the SMS library
		error = SMSRegister (true);
		if (error)
			FrmAlert (resAlertCantRegisterWithSMSLibrary);
	}

  // verify the Phone is powered and SIM status is ready
  if ( PhnLibModulePowered (smsLibRef) == false)
  	{
	  FrmAlert (resAlertPhoneNotReady);
	  if (smsLibLoaded)
	  	(void)PhnLibClose (smsLibRef);
	  return 1;
	}

  // Check what device we are on: GSM or CDMA
  
  FrmGotoForm(resFormIDSimpleSMS);
  
  /*
  (void)HsAttrGet (hsAttrPhoneType, 0, &phoneType);
  if (phoneType == hsAttrPhoneTypeGSM)
	{
	  FrmGotoForm (resFormIDSimpleSMS);
	}
  else if (phoneType == hsAttrPhoneTypeCDMA)
	{
	  FrmGotoForm (resFormIDSimpleSMSNoSend);
	}
  else
	{
	  // Default to the GSM (send/receive message) form. Pre-Treo 300 devices do not support this attribute.
	  FrmGotoForm (resFormIDSimpleSMS);
	  phoneType = hsAttrPhoneTypeGSM;
	}
  */
  
  return error;
}

/****************************************************************/
/* EventLoop */
/**
 * Main event loop
 *
 * \retval  none
 *
 ****************************************************************/
static void EventLoop(void)
{
  UInt16	err;
  UInt16	formID;
  FormPtr	formP;
  EventType event;

  do
	{
	  EvtGetEvent (&event, sysTicksPerSecond/2);

	  if (SysHandleEvent (&event))
		continue;
	  if (MenuHandleEvent ((void *)0, &event, &err))
		continue;
	  if (event.eType == frmLoadEvent)
		{
		  formID = event.data.frmLoad.formID;
		  formP = FrmInitForm (formID);
		  FrmSetActiveForm (formP);

		  switch (formID) 
			{
			case resFormIDSimpleSMS:
			  FrmSetEventHandler (formP, (FormEventHandlerPtr)SimpleSMSEvent);
			  break;
			case resFormIDSimpleSMSNoSend:
			  FrmSetEventHandler (formP, (FormEventHandlerPtr)SimpleSMSEvent);
			  break;
			}
		}

	  FrmDispatchEvent(&event);

	 } while (event.eType != appStopEvent);
}

/****************************************************************/
/* StopApplication */
/**
 * Stop the application
 *
 * \retval  none
 *
 ****************************************************************/
static void
StopApplication (void)
{
  //if (phoneType == hsAttrPhoneTypeGSM)
	//{
	//  CopyFromFieldsToGlobal();
	//}

  // Write the saved preferences / saved-state information.  This data 
  // will be backed up during a HotSync.
  //PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, &smsPrefs, sizeof (smsPrefs), true);

  // Close all the open forms.
  FrmCloseAllForms ();

  // Release the SMS library
  if (smsLibLoaded)
	  PhnLibClose(smsLibRef);
}

/****************************************************************/
/* SimpleSMSEvent */
/**
 * Event handler for the main form
 *
 * \param	event		  IN	Event to process
 *
 * \retval  Boolean
 *
 ****************************************************************/
static Boolean
SimpleSMSEvent (EventPtr eventP)
{
  UInt16 err;
  UInt16  cardNo;
  FormPtr frmP;
  LocalID dbID;
  FieldPtr fieldP;
  DmSearchStateType searchState;
  Boolean handled = false;

  DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
							   appFileCreator, true, &cardNo, &dbID);

  switch (eventP->eType) 
	{
	case menuEvent:
	  switch (eventP->data.menu.itemID)
		{
		case resMenuItemAbout:
		  MenuEraseStatus (0);
		  HsAboutHandspringApp (cardNo, dbID, "2007", "palmOne DTS Team");
		  handled = true;
		  break;

		case resMenuItemUnRegister:
		  MenuEraseStatus (0);
		  err = SMSRegister (false);
		  if (err)
			FrmAlert (resAlertCantRegisterWithSMSLibrary);
		  handled = true;
		  break;
		}
	  break;

	case frmOpenEvent:
	  frmP = FrmGetActiveForm();

		  // Set the focus to the edit field
	  FrmSetFocus(frmP, noFocus);
	  FrmSetFocus(frmP, FrmGetObjectIndex(frmP, resButtonSend));
	  FrmDrawForm (frmP);

	  if (FrmGetActiveFormID () == resFormIDSimpleSMS)
		{
		  CopyFromGlobalToFields();



		  TurnOnOptionLock ();
		}
	  else if (FrmGetActiveFormID () == resFormIDSimpleSMSNoSend)
		{
		}

	  // Setup the battery and signal gadgets
	  {
		UInt32 hsStatusVersion;

		if (FtrGet (hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion) == 0)
		  {
			HsStatusSetGadgetType (frmP, resGadgetMainBatteryLevel, hsStatusGadgetBattery);
			HsStatusSetGadgetType (frmP, resGadgetSignalLevel, hsStatusGadgetSignal);
		  }
	  }

	  handled = true;
	  break;
		
	case ctlSelectEvent:
	  switch (eventP->data.ctlSelect.controlID)
		{
		case resButtonSend:
		  SendTheMessage();
		  break;
			
		default:
		  break;
		}
	  break;

	case fldEnterEvent:
	  fieldP = (FieldType*)GetObjectPtr (eventP->data.fldEnter.fieldID);
	  switch (eventP->data.fldEnter.fieldID)
		{
		case resFieldAddress:
		case resFieldServiceCenter:
          FldHandleEvent (fieldP, eventP);
		  TurnOnOptionLock ();
          handled = true;
		  break;

		case resFieldMessage:
          FldHandleEvent (fieldP, eventP);
          if (FldGetInsPtPosition(fieldP) == 0)
            HsGrfSetStateExt(false, false, false, true, false, true);
          handled = true;
		  break;
		}
	  break;

	default:
	  break;
	}
	
	return handled;
}




/****************************************************************/
/* HandleNotificationEvent */
/**
 * This function handles all Phone specific events.
 *
 * \param   cmdPBP		  IN	Pointer to a structure that is associated with the launch code. 
 * \param	launchFlags	  IN	Value providing extra information about the launch.
 * 
 * \retval	UInt32 - Result of launch
 *
 ****************************************************************/
static void 
HandleNotificationEvent ( MemPtr cmdPBP, UInt16 launchFlags) 
{
  SysNotifyParamType* notifyParam = (SysNotifyParamType*)cmdPBP ;

  launchFlags = launchFlags;		// Prevent compiler error
  
  if (notifyParam->notifyType == phnNBSEvent)
    {
      PhnNBSNotificationEventType* NBSParamP = (PhnNBSNotificationEventType*) notifyParam->notifyDetailsP;


      // Display NBS info 

      if (NBSParamP->binary)	// NBS binary
	  {
	  	;
	  }
	  else // NBS text
	  {

		char dstPort[20], srcPort[20];

//		  NBSParamP->refnum;
//		  NBSParamP->maxnum;
//		  NBSParamP->seqnum;

//		  NBSParamP->srcPort;
//		  NBSParamP->dstPort;
	
//		  NBSParamP->senderP;
//		  NBSParamP->datetime;

		StrIToA(dstPort, (long) NBSParamP->dstPort);
		StrIToA(srcPort, (long) NBSParamP->srcPort);
		StrCat(dstPort, "/");
		StrCat(dstPort, srcPort);
		FrmCustomAlert(resAlertIncomingNBSMessage, NBSParamP->senderP, dstPort, NBSParamP->dataP);


		//Display like regular SMS.
		//DisplayMessage(NBSParamP->msgID);

	  }
      
   
	  notifyParam->handled = true;	

    }
}


/****************************************************************/
/* HandlePhoneLibraryEvent */
/**
 * This function handles all Phone specific events.
 *
 * \param   cmdPBP		  IN	Pointer to a structure that is associated with the launch code. 
 * \param	launchFlags	  IN	Value providing extra information about the launch.
 * 
 * \retval	UInt32 - Result of launch
 *
 ****************************************************************/
static UInt32
HandlePhoneLibraryEvent(MemPtr cmdPBP, UInt16 launchFlags)
{
  PhnEventPtr eventP = (PhnEventPtr)cmdPBP;
  UInt32 flags;
  Err error = 0;
  UInt32 locked = 0;
  
  launchFlags = launchFlags;		// Prevent compiler warning of unused var

  switch (eventP->eventType) 
	{
	case phnEvtSegmentInd:
	case phnEvtMessageInd:
	  // a message has been received
	  DisplayMessage (eventP->data.params.id);
	  // Tell the Phone Library that we handled the event and no
	  // other app needs to know about this.
	  eventP->acknowledge = true;
	  
	  HsAttrGet(hsAttrKeyboardLocked, 0, &locked);
	  
	  if (locked)
	  	HsAttrSet(hsAttrKeyboardLocked, 0, 0);
	  
	  break;
		
	case phnEvtMessageStat:
	  // a message status has changed
	  switch (eventP->data.params.newStatus)
		{
		case kSending:
		  // this is an expected status, do nothing
		  break;
		case kSent:
		  // ok, did we successfully send the message?
		  error = PhnLibGetFlags (smsLibRef, eventP->data.params.id, &flags);
		  if (error)
			  return error;
		  if ((flags & kFailed) == 0)
			  FrmAlert (resAlertSuccessfulTransfer);
		  else
			  FrmAlert (resAlertUnsuccessfulTransfer);
		  break;
		default:
		  break;
		}
	  eventP->acknowledge = true;
	  break;
		
	default:
	  break;
	}
  return error;
}

/****************************************************************/
/* LoadPhoneLibrary */
/**
 * Get a libRef to the Phone library
 * 
 * \param	libRefP		  OUT	Pointer to library reference ID to return. If NULL,
 *								library isn't loaded and an error is returned.
 * \param	libLoadedP	  INOUT	Pointer to boolean indicating whether the library
 *								had to be loaded or not. If the value is true on    
 *								entry, it will be left as true, even if the library 
 *								was found already loaded - this allows reuse of the 
 *								same boolean indicating load status throughout the  
 *								application. This routine NEVER sets this parameter 
 *								to false.                                           
 *
 * \retval	none
 *
 ****************************************************************/
static Err
LoadPhoneLibrary(UInt16* libRefP, Boolean* libLoadedP)
{
  Err error = 0;
  
  error = HsGetPhoneLibrary(libRefP);
  if (error) return error;
  
  error = PhnLibOpen(*libRefP);
  if (error) return error;
  
  *libLoadedP = true;
  
  return error;
}

/****************************************************************/
/* SMSRegister */
/**
 * This routine registers this application with the SMS library.
 * 
 * \param	bReg		  IN	true to register; false to unregister
 *
 * \retval	none
 *
 ****************************************************************/
static Err
SMSRegister (Boolean bReg)
{
  Boolean libLoaded = smsLibLoaded;
  Err error = 0;
  UInt16 libRef = smsLibRef;

  if (!libLoaded)
	  error = LoadPhoneLibrary (&libRef, &libLoaded);

  if (!error)
  {
	// Register this application with the Phone library so it will receive the appropriate events
	error = PhnLibRegister (libRef, appFileCreator, bReg ? phnServiceSMS : 0);
	if (libLoaded)
	  (void)PhnLibClose (libRef);
  }
  
  
	// Register NBS Notification
	  {
	  UInt16  cardNo;
	  LocalID dbID;
	  DmSearchStateType searchState;

	  DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								   appFileCreator, true, &cardNo, &dbID);

	  if ( bReg)
	  {
	  	  error = SysNotifyRegister (cardNo, dbID, phnNBSEvent, NULL, sysNotifyNormalPriority, NULL);
	  	  if (error == sysNotifyErrDuplicateEntry)  // Already registered
	  	  	error = 0;  // This is OK.  Clear this error
	  }
	  else
		  error = SysNotifyUnregister (cardNo, dbID, phnNBSEvent,sysNotifyNormalPriority);
	  }

  return error;
}

/****************************************************************/
/* SendTheMessage */
/**
 * This routine will send the message
 * 
 * \retval	none
 *
 ****************************************************************/
static void
SendTheMessage(void)
{
  UInt32 msgID = 0;
  PhnAddressList addList;
  PhnAddressHandle addressH;
  Err error = 0;
  DmOpenRef smsRefNum = 0;      // CDMA workaround

  // verify the Phone is powered
  if (!PhnLibModulePowered (smsLibRef))
	{
	  FrmAlert (resAlertPhoneNotReady);
	  return;
	}

  // verify the Phone is on the network
  if (!PhnLibRegistered (smsLibRef))
	{
	  FrmAlert (resAlertNotRegistered);
	  return;
	}

  smsRefNum = PhnLibGetDBRef(smsLibRef);    // CDMA workaround

  // get the values from the fields into the globals
  // check if there is no address input. if not, do not send SMS and return. Modified by TaoC
  if (CopyFromFieldsToGlobal() == 1)
  {

  	FrmAlert (resAlertNoAddressInput);
  	return ;
  }

  // now, create the new message
  msgID = PhnLibNewMessage (smsLibRef, kMTOutgoing);
  if (!msgID)
	goto SendMessage_CloseAndRelease;

  // the the owner of this new message to this application	
  PhnLibSetOwner (smsLibRef, msgID, appFileCreator);

  // fill in the text of this message
  PhnLibSetText (smsLibRef, msgID, smsPrefs.message, (short) StrLen(smsPrefs.message));

  // fill in the address
  addList = PhnLibNewAddressList (smsLibRef);
  if (!addList)
	goto SendMessage_CloseAndRelease;

  addressH = PhnLibNewAddress (smsLibRef, smsPrefs.address, phnLibUnknownID);
  if (!addressH)
	goto SendMessage_CloseAndRelease;

  PhnLibAddAddress (smsLibRef, addList, addressH);
  MemHandleFree (addressH);

  PhnLibSetAddresses (smsLibRef, msgID, addList);
//  PhnLibDisposeAddressList (smsLibRef, addList);

  // and fire the message off!
  error = PhnLibSendMessage (smsLibRef, msgID, true);

SendMessage_CloseAndRelease:
  PhnLibReleaseDBRef(smsLibRef, smsRefNum);    // CDMA workaround

}

/****************************************************************/
/* DisplayMessage */
/**
 * This routine displays information about the given message
 *
 * \param	messageID	  IN	The message to display
 * 
 * \retval	none
 *
 ****************************************************************/
static void
DisplayMessage(UInt32 messageID)
{
	PhnAddressList		addList;
	PhnAddressHandle	addressH;
	MemHandle			messageH;
	char*				addressP;
	char*				messageP;
	Err					error = 0;
	DmOpenRef			smsRefNum = 0;      // CDMA workaround
	// we cannot use the global smsLibRef,
	// because this can be called when the application is not running
	UInt16				localSmsLibRef;
	Boolean				localSmsLibLoaded;
	char 				errMsg[10];
	UInt16              msgIndex;
	
	//Initialize some values
	localSmsLibLoaded = false;
	localSmsLibRef = 0;


	error = LoadPhoneLibrary (&localSmsLibRef, &localSmsLibLoaded);
	if (error)
	{
		StrIToA(errMsg, error);
		FrmCustomAlert (resAlertPhoneLibrary, "Can't Load the Phone Library", errMsg, NULL);
		return;
	}

	smsRefNum = PhnLibGetDBRef(localSmsLibRef);    // CDMA workaround

  // get the address list
  error = PhnLibGetAddresses (localSmsLibRef, messageID, &addList);
  if (error)
	goto DisplayMessage_CloseAndRelease;

  // now get the first entry from the list
  error = PhnLibGetNth (localSmsLibRef, addList, 1, &addressH);
  if (error)
	goto DisplayMessage_CloseAndRelease;

  // now get the actual address
  addressP = PhnLibGetField (localSmsLibRef, addressH, phnAddrFldPhone);

  // and get the message text
  error = PhnLibGetText (localSmsLibRef, messageID, &messageH);
  if (error)
	goto DisplayMessage_CloseAndRelease;

  messageP = MemHandleLock (messageH);
	
  // display the incoming message alert
  FrmCustomAlert (resAlertIncomingMessage, addressP, messageP, NULL);
  
  // and clean up
  if (addressP)
	MemPtrFree (addressP);
  if (messageH)
	{
	  MemHandleUnlock (messageH);
	  MemHandleFree (messageH);
	}
	
  if (!DmFindRecordByID(smsRefNum, messageID, &msgIndex))
	{
	  DmRemoveRecord(smsRefNum, msgIndex); // CDMA workaround
	}
  else
    {
      FrmCustomAlert(resAlertIncomingMessage, "000000", "Record not found!", NULL);
    }
	
DisplayMessage_CloseAndRelease:
  PhnLibReleaseDBRef(localSmsLibRef, smsRefNum);    // CDMA workaround

  // unload if necessary
  if (localSmsLibLoaded)
	(void)PhnLibClose (localSmsLibRef);
}

/****************************************************************/
/* TurnOnOptionLock */
/**
 * Turn on the option lock on keyboard enabled devices.
 *
 * \param	none
 *
 * \retval	none
 *
 ****************************************************************/
static void
TurnOnOptionLock (void)
{
  // See if we are on a keyboard device and enable the option-lock
  UInt16 err;
  UInt32 keyboardType;

  // See if we have a keyboard
  err = FtrGet (hsFtrCreator, hsFtrIDTypeOfKeyboard, &keyboardType);
  if (!err && (keyboardType & hsFtrValKeyboardQwerty))
	HsGrfSetStateExt (false,	// capsLock
					  false,	// numLock 
					  true,		// optLock
					  false,	// upperShift
					  false,	// optShift
					  false);   // autoShift
}
/****************************************************************/
/* GetObjectPtr */
/**
 * This routine returns a pointer to an object in the current form.
 * 
 * \param	formId		  IN	ID of the form to display
 *
 * \retval	void* - Pointer to the object
 *
 ****************************************************************/
static void*
GetObjectPtr (UInt16 objectID)
{
  FormPtr frmP;
  frmP = FrmGetActiveForm ();
  return FrmGetObjectPtr (frmP, FrmGetObjectIndex(frmP, objectID));
}

/****************************************************************/
/* CopyFromGlobalsToFields */
/**
 * This routine will copy the text strings from the global prefs into
 * the text fields on the main form
 *
 * \param	none
 * 
 * \retval	none
 *
 ****************************************************************/
static void
CopyFromGlobalToFields (void)
{
  Err error = 0;
  
  FieldPtr fld;
  MemHandle h, oldH;
  PhnAddressHandle addressH;
  CharPtr addressP;
  UInt32 phnType;
  
  HsAttrGet(hsAttrPhoneType, 0, &phnType);

  // Fill in the fields from the prefs
  fld = GetObjectPtr (resFieldAddress);
  h = MemHandleNew (StrLen(smsPrefs.address) + 1);
  if (!h)
    return;

  StrCopy (MemHandleLock (h), smsPrefs.address);
  MemHandleUnlock (h);
  oldH = FldGetTextHandle (fld);
  FldSetTextHandle (fld, h);
  FldDrawField (fld);
  if (oldH)
    MemHandleFree (oldH);

  fld = GetObjectPtr (resFieldMessage);
  h = MemHandleNew (StrLen (smsPrefs.message) + 1);
  if (!h)
    return;

  StrCopy (MemHandleLock (h), smsPrefs.message);
  MemHandleUnlock (h);
  oldH = FldGetTextHandle (fld);
  FldSetTextHandle (fld, h);
  FldDrawField (fld);
  if (oldH)
    MemHandleFree (oldH);

  if (phnType == hsAttrPhoneTypeGSM)
  {
	  // fill in the service center from the Phone library
	  fld = GetObjectPtr (resFieldServiceCenter);
	  error = PhnLibGetServiceCentreAddress (smsLibRef, &addressH);
	  if (!error && addressH)
	  {
		  addressP = PhnLibGetField (smsLibRef, addressH, phnAddrFldPhone);

	      h = MemHandleNew (serviceCenterSize);
	      if (!h)
		  {
		    MemHandleFree (addressH);
		    MemPtrFree (addressP);
		    return;
		  }
		
		  StrCopy (MemHandleLock (h), addressP);
		  MemHandleUnlock (h);
		  oldH = FldGetTextHandle (fld);
		  FldSetTextHandle (fld, h);
		  FldDrawField (fld);
		  if (oldH)
		    MemHandleFree (oldH);

		  // clean up
		  MemHandleFree (addressH);
		  MemPtrFree (addressP);
	  }
  }
}


/****************************************************************/
/* CopyFromFieldsToGlobal */
/**
 * This routine will copy the text strings from the text
 * fields on the main form into the global prefs.
 *
 * \param	none
 *
 * \retval	none - rev.1
 *			0: success - rev.2
 *			1: no address input added by TaoC
 *
 ****************************************************************/
static UInt32
CopyFromFieldsToGlobal (void)
{
  FieldPtr fld;
  char* s;
  UInt16 len = 0;
  
  // Get the pref values from the fields
  fld = GetObjectPtr(resFieldAddress);
  s = FldGetTextPtr(fld);
  if (s)
	{
	  // check if user has enter an address. if not, then do not send SMS. Added by TaoC
	  len = StrLen(s);
	  if ( len > 0)	
	  	// copy contents of address field into prefs
	  	StrCopy(smsPrefs.address, s);
	  else
	  	return 1 ;
	}
  fld = GetObjectPtr(resFieldMessage);
  s = FldGetTextPtr(fld);
  if (s)
	{
	  // copy contents of message field into prefs
	  StrCopy(smsPrefs.message, s);
	}
  
  return 0;
}



