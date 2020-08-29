#include <PalmOS.h>
#include <HsCreators.h>
#include <HsNav.h>
#include <HsExt.h>
#include <NetMasterLibrary.h>
#include <NetPrefLibrary.h>
#include "DataSessionLogin.h"
#include <HsNav.h>

#define appFileCreator	'DtSs'

#define MIN_VERSION                 sysMakeROMVersion(5,1,0,sysROMStageRelease,0)

#define LAUNCH_FLAGS                (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)
#define netLibTestLaunchCmdBGLogin  (sysAppLaunchCmdCustomBase + 0)


typedef struct NetProfileType_ {
    Char serviceName[32];
    Char userName[32];
    Char password[32];
    Char apn[32];
    NetPrefSvcMediumEnum medium;
} NetProfileType;

UInt16	gNetMasterLibRefNum = 0;
UInt16	gNetPrefLibRefNum = 0;
Boolean	gNetPrefLibLoaded = false;

struct  NetPrefContextTypeTag* cxtP;
struct  NetPrefRecordTypeTag* recP;

UInt16  gTotalProfiles = 0;
UInt16  gCurrentRecID = 0;

void RunTest(Boolean isUITask);
static Int32 TestSessionControl(NetMasterNetLibOpenOptionsType openOption, UInt32 recID, Boolean isUITask);
void LaunchBackgroundLogin(void);
static Boolean CreateNewProfile(NetProfileType *profile);

static void * GetObjectPtr(UInt16 objectID)
{
    FormType * frmP;

    frmP = FrmGetActiveForm();
    return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}

static void SetFieldText(FormPtr frmP, UInt16 fldID, Char* srcP, Boolean append,
		Boolean redraw)
{
	MemHandle 	stringH;
	UInt16		oldLength = 0;
	UInt16		newSize;
	Char* 		stringP;
	FieldPtr	fldP;

	newSize = StrLen(srcP) + 1;	// initialize new size
	
	fldP = (FieldPtr)GetObjectPtr(fldID);
	
	stringH = (MemHandle)FldGetTextHandle( fldP ); // get the current text handle
	FldSetTextHandle( fldP, 0 ); // release this handle from field

	// Resize the existing handle, if any
	if ( stringH )
	{
		if ( append )
		{
			stringP = MemHandleLock( stringH );
			oldLength = StrLen(stringP);
			newSize += oldLength;
			MemPtrUnlock( stringP );
		}
			
		if ( MemHandleResize(stringH, newSize) )
			goto Exit;
	} // Resize the existing handle, if any
	
	// Otherwise, allocate a new handle
	else
	{
		stringH = MemHandleNew( newSize ); // allocate a new chunk
		if ( !stringH )	return;
	}

	// Append the new text
	stringP = MemHandleLock( stringH );
	StrCopy( stringP + oldLength, srcP ); // copy the new text
	MemPtrUnlock( stringP );
	
Exit:
	FldSetTextHandle( fldP, (MemHandle)stringH ); // set the new text handle
	if ( redraw ) // redraw the field
	{
		FldEraseField( fldP );
		FldDrawField( fldP );
	}
}

static void GetFieldText(Char* destP, Int16 bufSize, FormPtr frmP, UInt16 fldID)
{
	MemHandle stringH;
	Char* stringP;
	
	destP[0] = '\0';
	stringH = (MemHandle)FldGetTextHandle( (FieldPtr)GetObjectPtr( fldID) );
	if ( !stringH )
		return;
	stringP = MemHandleLock( stringH );
	StrNCopy( destP, stringP, bufSize );
	destP[bufSize-1] = '\0'; // make sure it is null-terminated
	MemPtrUnlock( stringP );
}

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet (sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion >= requiredVersion)
	{
		return 0;
	}

	if ((launchFlags & LAUNCH_FLAGS) == LAUNCH_FLAGS)
	{
		FrmAlert (rscRomIncompatibleAlert);
	
		// Pilot 1.0 will continuously relaunch this app unless we switch to 
		// another safe one.
		if (romVersion < MIN_VERSION)
			AppLaunchWithCommand (sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
	}

	return sysErrRomIncompatible;
}

void RunTest(Boolean isUITask)
{	
	UInt32	recID;
	Err	err;
	

	// Use the current Internet target
	err = NetPrefDefaultTargetGet (gNetPrefLibRefNum, cxtP, 
					netPrefSvcTargetInternet, &recID);
	if (err) 
		return;

	TestSessionControl(netMasterNetLibOpenOptAutoDismiss, recID, isUITask);
	
}

static Int32 TestSessionControl(NetMasterNetLibOpenOptionsType openOption, UInt32 recID, Boolean isUITask)  
{
	NetMasterDataContextIDType dataCxtID;
	Err err = errNone;
	Boolean	isNetIFError = false;

	err = NetMasterSCNetAttach (gNetMasterLibRefNum, recID,
					openOption,
					netMasterLoginPriorityDefault,
					0 /*anchorTimeoutSec*/,
					&dataCxtID,
					&isNetIFError);
	if (err) goto Exit;

    if (isUITask)
    {
    	HsStatusUpdateGadgets ();
    	FrmDrawForm(FrmGetActiveForm());
    }
    
	SndPlaySystemSound(sndStartUp);

	// Give some delay and display Signal Gadget status before detaching.
    // Delay also useful to give UI task (the app) a chance to update the Signal Gadget.
	SysTaskDelay(3*SysTicksPerSecond());

    if (isUITask)
    {
    	HsStatusUpdateGadgets ();
    	FrmDrawForm(FrmGetActiveForm());
    }

	SndPlaySystemSound(sndInfo);
	
	if (err == errNone) {
		err = NetMasterSCNetDetach (gNetMasterLibRefNum, dataCxtID);
	}

	// Give Signal Gadget status of detaching.
	
	if (isUITask)
	{
    	HsStatusUpdateGadgets ();
    	FrmDrawForm(FrmGetActiveForm());
    }
    
Exit:
	return err;
} 

void  LaunchBackgroundLogin(void)  
{
	Err err;
	UInt16  launchFlags;
	UInt16	cardNo = 0;
	LocalID	dbID = 0;
	DmSearchStateType searchState;

	UInt32	dummyResult;


    launchFlags =		sysAppLaunchFlagNewThread
					  |	sysAppLaunchFlagNewStack
					  | sysAppLaunchFlagNewGlobals;

	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);

    err = SysAppLaunch (cardNo, dbID, launchFlags,
						  netLibTestLaunchCmdBGLogin, NULL,
						  &dummyResult);
	 return;
}

static Boolean CreateNewProfile(NetProfileType *profile)
{
    struct NetPrefRecordTypeTag* recP = NULL;
    Err error = errNone;
    
    NetPrefRecNew(gNetPrefLibRefNum, cxtP, 0, &recP);
    if (!recP) return false;
    
    error |= NetPrefRecFieldSetDefineStd(gNetPrefLibRefNum, cxtP, recP, profile->medium, NULL, 0);

    NetPrefRecReadOnlyOverrideStart(gNetPrefLibRefNum, cxtP, recP);
    
    error |= NetPrefRecFieldSet(gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldSvcMediumAlias, &profile->medium, sizeof(profile->medium));

    error |= NetPrefRecFieldSet(gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldServiceName, profile->serviceName, StrLen(profile->serviceName) + 1);
        
    if (profile->medium == netPrefSvcMediumGPRS)
    {
        error |= NetPrefRecFieldSet(gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldUsername, profile->userName, StrLen(profile->userName) + 1);
                
        error |= NetPrefRecFieldSet(gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldPassword, profile->password, StrLen(profile->password) + 1);
    
        error |= NetPrefRecFieldSet(gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldAPN, profile->apn, StrLen(profile->apn) + 1);
    }
    
    NetPrefRecReadOnlyOverrideEnd(gNetPrefLibRefNum, cxtP, recP);
    
    error |= NetPrefRecSave(gNetPrefLibRefNum, cxtP, recP);
    
    error |= NetPrefRecRelease(gNetPrefLibRefNum, cxtP, &recP);
    
    return true;
}


static UInt32 PrvInitNetPrefLib(void)
{
	Err	err;
	
	// Find the NetPref library and open it.
	err = SysLibFind (netPrefLibName, &gNetPrefLibRefNum );

	if (err)
	{
		err = SysLibLoad (netPrefLibTypeID, netPrefLibCreatorID,
						&gNetPrefLibRefNum);
		if (err)
		{
			ErrNonFatalDisplay ("failed to load NetPrefLib");
			return err;
		
			gNetPrefLibLoaded = true;
		}
	}

	err = NetPrefLibOpen (gNetPrefLibRefNum, &cxtP);
	if (err)
	{
	    ErrNonFatalDisplay ("failed to open NetPrefLib");
		goto Exit;
	}
		
	err = NetPrefRecCountGet (gNetPrefLibRefNum, cxtP, &gTotalProfiles);
	if (err)
	{
	    ErrNonFatalDisplay ("failed to get record count");
	    goto Exit;
	}
	
Exit:
	if (err)
	{
		if (gNetPrefLibRefNum != 0)
			SysLibRemove (gNetPrefLibRefNum);

		gNetPrefLibRefNum = 0;
	}

	return (err);
}

static void PrvUninitNetPrefLib(void)
{
	if (gNetPrefLibRefNum)
	{
		if (cxtP)
			NetPrefLibClose (gNetPrefLibRefNum, cxtP);
	}

	if (gNetPrefLibLoaded)
		SysLibRemove (gNetPrefLibRefNum);

	gNetPrefLibRefNum = 0;
	gNetPrefLibLoaded = false;
	cxtP = NULL;

	return;
}

static UInt32 PrvInitNetMasterLib(void)
{
	Err	err;

	// Find the NetPref library and open it.
	err = SysLibFind (netMasterLibName, &gNetMasterLibRefNum );

	if (err)
		ErrNonFatalDisplay ("failed to find NetMasterLib");

	// If NetMaster is not loaded by the system, there is a good
	// reason for this.  So don't load it.

	return (err);
}

static void MainFormInit(FormPtr frmP)
{

}

static Boolean MainFormDoCommand(UInt16	command)
{
	Boolean handled = false;
	UInt32 recUID = 0;
	Err err = errNone;

	switch (command)
	{
		case rscMainForegroundLogin:
			RunTest(true);
			handled = true;
			break;
			
		case rscMainBackgroundLogin:
			LaunchBackgroundLogin();
			handled = true;
			break;
			
		case rscMainNewProfile:
		    FrmGotoForm(rscProfileForm);
		    handled = true;
		    break;
		    
		case rscMainDefaultProfile:
		    err = NetPrefRecIDGetByIndex(gNetPrefLibRefNum, cxtP, gCurrentRecID, &recUID);
		    if (err == errNone) {
		    err = NetPrefDefaultTargetSet(gNetPrefLibRefNum, cxtP, netPrefSvcTargetInternet, recUID);
		    }
		    if (err == errNone)
		    	handled = true;
		    break;
		    
		case rscMainHelpAbout:
			MenuEraseStatus (0);
			AbtShowAbout (appFileCreator);
			handled = true;
			break;
			
		default:
			break;
	}

	return handled;
}

static Boolean MainFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP = FrmGetActiveForm();

	UInt32 hsStatusVersion;
	NetPrefRecFieldViewType viewFlags = netPrefRecFieldViewRead;
	Char *serviceName;
	UInt32 serviceNameLen;
	Err err = errNone;
	UInt32 recUID;

	switch (eventP->eType) 
	{
		case menuEvent:
			handled = MainFormDoCommand (eventP->data.menu.itemID);
			break;

		case frmOpenEvent:
    		MainFormInit (frmP);
            FrmNavObjectTakeFocus(frmP, rscMainPrevButton);
            
			if (FtrGet (hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion) == 0)
			{
				HsStatusSetGadgetType (frmP, rscGadgetMainBatteryLevel, hsStatusGadgetBattery);
				HsStatusSetGadgetType (frmP, rscGadgetSignalLevel, hsStatusGadgetSignal);
			}

            gCurrentRecID = 0;
			FrmDrawForm (frmP);
			handled = true;
			break;

		case frmUpdateEvent:
			HsStatusUpdateGadgets ();
    		FrmDrawForm(FrmGetActiveForm());
    		handled = true;
       		break;

        case ctlSelectEvent:
            switch (eventP->data.ctlSelect.controlID)
    		{
    		    case rscMainPrevButton:
        			if (gCurrentRecID > 0 ) 
        			  gCurrentRecID -= 1;
        			else
        			  gCurrentRecID = gTotalProfiles - 1;

        			err = NetPrefRecIDGetByIndex (gNetPrefLibRefNum, cxtP, gCurrentRecID, &recUID);
                    if (err) break;
						
        			err = NetPrefRecLoad (gNetPrefLibRefNum, cxtP, recUID, &recP);
        			if (err) break;
        			
                    err = NetPrefRecFieldGet (gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldServiceName, (void **)&serviceName, &serviceNameLen, &viewFlags);
                    if (err) break;
                    
          			SetFieldText(frmP, rscMainNameField, (Char *)serviceName, false, true);
          			
          			NetPrefRecRelease (gNetPrefLibRefNum, cxtP, &recP);

        			handled = true;
        			break;

    		    case rscMainNextButton:
        			if (gCurrentRecID < (gTotalProfiles - 1) ) 
        			  gCurrentRecID += 1;
        			else
        			  gCurrentRecID = 0;
        			  
        			err = NetPrefRecIDGetByIndex (gNetPrefLibRefNum, cxtP, gCurrentRecID, &recUID);
                    if (err) break;

        			err = NetPrefRecLoad (gNetPrefLibRefNum, cxtP, recUID, &recP);
                    if (err) break;
                    
                    err = NetPrefRecFieldGet (gNetPrefLibRefNum, cxtP, recP, netPrefRecFieldServiceName, (void **) &serviceName, &serviceNameLen, &viewFlags);
                    if (err) break;
                    
          			SetFieldText(frmP, rscMainNameField, (Char *)serviceName, false, true);
          			
          			NetPrefRecRelease (gNetPrefLibRefNum, cxtP, &recP);

        			handled = true;
        			break;

    		    case rscMainConnectButton:
        			err = NetPrefRecIDGetByIndex (gNetPrefLibRefNum, cxtP, gCurrentRecID, &recUID);
                    if (err) break;
    		    
       				TestSessionControl(netMasterNetLibOpenOptAutoDismiss, recUID, true);
        			handled = true;
        			break;
        			
        		case rscMainDisconnectButton:
                    NetMasterNetInterfacesShutDown(gNetMasterLibRefNum, 0, netMasterNetIFShutDownOptDismissPrgDialog |
                        netMasterNetIFShutDownOptResetNetGuard);
        		    handled = true;
        		    break;

    		    default:
    			    break;
    		}

		default:
			break;
	}
	
	return handled;
}

static void ProfileFormInit()
{
    FormPtr frmP = FrmGetActiveForm();
    UInt32 phnType = hsAttrPhoneTypeGSM;
    
    HsAttrGet(hsAttrPhoneType, 0, &phnType);
    
    if (phnType == hsAttrPhoneTypeGSM)
        SetFieldText(frmP, rscProfConnectionField, "GPRS", false, false);
    else
    {
        SetFieldText(frmP, rscProfConnectionField, "1xRTT", false, false);
        FrmHideObject(frmP, FrmGetObjectIndex(frmP, rscProfAPNLabel));
        FrmHideObject(frmP, FrmGetObjectIndex(frmP, rscProfAPNField));
        FrmHideObject(frmP, FrmGetObjectIndex(frmP, rscProfUserLabel));
        FrmHideObject(frmP, FrmGetObjectIndex(frmP, rscProfUserField));
        FrmHideObject(frmP, FrmGetObjectIndex(frmP, rscProfPasswordLabel));
        FrmHideObject(frmP, FrmGetObjectIndex(frmP, rscProfPasswordField));
    }
}

static Boolean ProfileFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP = FrmGetActiveForm();
    Err error = errNone;
    
    switch (eventP->eType)
    {
        case frmOpenEvent:
            ProfileFormInit();
            FrmDrawForm(frmP);
            FrmNavObjectTakeFocus(frmP, rscProfNameField);
            handled = true;
            break;
            
        case ctlSelectEvent:
        {
            NetProfileType profile;
            UInt32 phnType = hsAttrPhoneTypeGSM;
            
            HsAttrGet(hsAttrPhoneType, 0, &phnType);

            GetFieldText(profile.serviceName, 32, frmP, rscProfNameField);

            if (phnType == hsAttrPhoneTypeGSM)
            {
                profile.medium = netPrefSvcMediumGPRS;
                GetFieldText(profile.userName, 32, frmP, rscProfUserField);
                GetFieldText(profile.password, 32, frmP, rscProfPasswordField);
                GetFieldText(profile.apn, 32, frmP, rscProfAPNField);
            }
            else
            {
                profile.medium = netPrefSvcMedium1xRTT;
                StrCopy(profile.userName, "None");
                StrCopy(profile.password, "None");
                StrCopy(profile.apn, "None");
            }

            error = (Err)!( StrLen(profile.serviceName) &&
                            StrLen(profile.userName) &&
                            StrLen(profile.password) &&
                            StrLen(profile.apn) );
                       
            if (error != errNone)
            {
                FrmCustomAlert(rscCustomAlert, "Information incomplete. Profile not created", 0, 0);
            }
            else
            {
                if (CreateNewProfile(&profile))
                    FrmCustomAlert(rscCustomAlert, "Success!", 0, 0);
                else
                    FrmCustomAlert(rscCustomAlert, "Fail to create new profile. Internal Error", 0, 0);
            }
            
            FrmGotoForm(rscMainForm);
            handled = true;
            break;
        }
            
        default:
            break;
    }
    
    return handled;
}

static Boolean AppHandleEvent(EventPtr eventP)
{
	UInt16 formId;
	FormPtr frmP;

	if (eventP->eType == frmLoadEvent)
	{
		// Load the form resource.
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm (formId);
		FrmSetActiveForm (frmP);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId)
		{
			case rscMainForm:
				FrmSetEventHandler (frmP, MainFormHandleEvent);
				break;
				
			case rscProfileForm:
			    FrmSetEventHandler (frmP, ProfileFormHandleEvent);
			    break;

			default:
				ErrFatalDisplay ("Invalid Form Load Event");
				break;
		}
			
		return true;
	}
	
	return false;
}

static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do 
	{
		EvtGetEvent (&event, evtWaitForever);
		
		if (!SysHandleEvent (&event))
			if (!MenuHandleEvent (0, &event, &error))
				if (!AppHandleEvent (&event))
					FrmDispatchEvent (&event);

	} while (event.eType != appStopEvent);
}

static Err AppStart(void)
{
	PrvInitNetPrefLib();
	PrvInitNetMasterLib();
	
	return errNone;
}

static void AppStop(void)
{
	FrmCloseAllForms ();
	PrvUninitNetPrefLib();

	// We don't unload the NetMaster library since we don't load it.
}

static UInt32 StarterPilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;

	error = RomVersionCompatible (MIN_VERSION, launchFlags);
	if (error) 
	{
		return (0);
	}

	switch (cmd)
	{
		case sysAppLaunchCmdNormalLaunch:
		{
			error = AppStart ();
			if (error)
				return error;
				
			FrmGotoForm (rscMainForm);
			AppEventLoop ();
			AppStop ();
			break;
		}
					
		// Launch code sent to initiate background login
  		case netLibTestLaunchCmdBGLogin:
  		{
			PrvInitNetPrefLib();
			PrvInitNetMasterLib();
			RunTest(false);
			PrvUninitNetPrefLib();

			break;
		}
		
		default:
			break;
	}
	
	return 0;
}

UInt32 PilotMain(UInt16	cmd, MemPtr	cmdPBP,	UInt16 launchFlags)
{
    return StarterPilotMain (cmd, cmdPBP, launchFlags);
}
