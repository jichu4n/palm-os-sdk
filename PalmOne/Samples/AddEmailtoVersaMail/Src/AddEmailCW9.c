/*
 * AddEmailCW9.c
 *
 * main file for AddEmailCW9
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <Helper.h>
#include <HelperServiceClass.h>
#include <PalmVMLaunch.h>
#include <HsNav.h>
#include <palmOneResources.h>
#include "AddEmailCW9.h"
#include "AddEmailCW9_res.h"


/*********************************************************************
 * Entry Points
 *********************************************************************/

/*********************************************************************
 * Global variables
 *********************************************************************/



/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/


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
    FormType *frmAboutP = NULL;
    
	switch (command)
	{
		case HelpAboutAddEmail:
			frmAboutP = FrmInitForm(HelpAboutAddEmail);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
						 
			handled = true;
			break;
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

	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);
			FrmNavObjectTakeFocus(frmP, MainExchangeButton);
			MainFormInit(frmP);
			handled = true;
			break;
            
        case frmUpdateEvent:
			/* 
			 * To do any custom drawing here, first call
			 * FrmDrawForm(), then do your drawing, and
			 * then set handled to true. 
			 */
			break;
			
		case ctlSelectEvent:
		{
		

			if (eventP->data.ctlSelect.controlID == MainExchangeButton)
			{
				/* Use exchange manager to throw up a New mail message */
				/* Lets you add an attachment */
				
				ExgSocketType exgSocket;
			    Err err = errNone;
			   
	    		Char 	*textBuf = "test";
	    		UInt32 size = StrLen(textBuf) + 1;

			    // it's important to initialize structure to zeros
	    		MemSet(&exgSocket,sizeof(exgSocket),0);
	    		exgSocket.description = "Testing";	 
	    			    		
	    		/* A box pops up with an option to choose SMS, VersaMail etc 
	    		 * You may not see VersaMail in the list, if VersaMail is not
	    		 * installed in your device.
	    		 */   
			    exgSocket.name = "?_send:Sample.txt";/* send is important here */
			    exgSocket.type = ".txt";
			    err = ExgPut(&exgSocket);
	    		
	    		 if (err == 0) {
          			ExgSend(&exgSocket,textBuf,size,&err);
          			ExgDisconnect(&exgSocket,err);
     			}

				handled = true;
				break;
			}
			else if (eventP->data.ctlSelect.controlID == MainHelperButton)
			{		
			
				/* You may want to check if VersaMail is there to handle
				 * this. So check it with kHelperNotifyActionCodeValidate.
				 * I have assumed VersaMail is present here.
				 */
			
				SysNotifyParamType param;
				HelperNotifyEventType details;
				HelperNotifyExecuteType execute;			
				HelperServiceEMailDetailsType emailDetails;
				
				Char subj[3] = "Hi";
				Char msg[30] = "This is a test Email";
				Char mail[15] = "john@doe.com";
				Char name[10] = "John Doe";
			
				UInt16				cardNo; 
				LocalID				dbID; 
				DmSearchStateType	searchState; 

				DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication, 
							'asc3', true,	&cardNo, &dbID); 
				
				if (!dbID) { 

					FrmCustomAlert(GeneralAlert, "Versamail not present","","");
					handled = true;
					break;
				}													
				
				param.notifyType = sysNotifyHelperEvent;
				param.broadcaster = appFileCreator;
				param.notifyDetailsP = &details;
				param.handled = false;
				
				details.version = kHelperNotifyCurrentVersion;
				details.actionCode = kHelperNotifyActionCodeExecute;
				details.data.executeP = &execute;
				execute.serviceClassID = kHelperServiceClassIDEMail;
				execute.helperAppID = 0;
				execute.dataP = mail;
				execute.displayedName = name;
				execute.err = errNone;
				
				
				/* This part is not working because of a bug in VersaMail
				 * but should work after it is fixed in future versions 
				 */
				emailDetails.version = 1;
				emailDetails.cc = mail;
				emailDetails.subject = subj;
				emailDetails.message = msg;
								
				execute.detailsP = &emailDetails;
								
				SysNotifyBroadcast(&param);			
				handled = true;				
				
				break;
			}
				/* SubLaunch silently adds email to versamail */
			else if (eventP->data.ctlSelect.controlID == MainLaunchButton)
			{
				MailAddRecordParamsTypePlus mailPlus;
				MailAddRecordAttachment attchmnt;			
				UInt32 deviceID = 0;

			    FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);

			    if (deviceID == kPalmOneDeviceIDTreo650  ||
			        deviceID == kPalmOneDeviceIDVentura  ||
			        deviceID == kPalmOneDeviceIDLowrider ||
			        deviceID == kPalmOneDeviceIDTorino   ||
			        deviceID == kPalmOneDeviceIDGandalf  ||
			        deviceID == kPalmOneDeviceIDGnome)
			    {
			        FrmAlert(UnsupportedAlert);
			        handled  = true;	
				    break;
			    }
	
				attchmnt.attachType = "text/plain";
				attchmnt.attachDescription = "Text Attachment";
				
				/* Extension is important the plugin viewer is selected 
				 * using this.
				 */
				attchmnt.attachFileName = "TextFile.txt"; 
				attchmnt.data.attachData = "Hmmm this is a test attachment";
				attchmnt.attachSize = StrLen( attchmnt.data.attachData ) + 1;

				
				mailPlus.category = 2; /* Add the msg to Drafts */
				mailPlus.numAttachments = 1;
				mailPlus.attachments[0] = attchmnt;
								
				
				mailPlus.mainParams.secret = false;
				mailPlus.mainParams.signature = true;
				mailPlus.mainParams.confirmRead = false;
				mailPlus.mainParams.confirmDelivery = false;
				mailPlus.mainParams.priority = mailPriorityNormal;
				mailPlus.mainParams.subject = "Hi";				
				mailPlus.mainParams.from = NULL;/* Note from is automatically taken from the account settings */
				mailPlus.mainParams.to = "joe@doe.com";
				mailPlus.mainParams.cc = NULL;
				mailPlus.mainParams.bcc = NULL;
				mailPlus.mainParams.replyTo = "jane@doe.com";
				mailPlus.mainParams.body = "Test message";				
				
				
				{
					UInt16				cardNo; 
					LocalID				dbID; 
					DmSearchStateType	searchState; 
					UInt32 				result; 
					Err 				err; 

					DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication, 
								'asc3', true,	&cardNo, &dbID); 
					
					if (dbID) 
					{ 
						err = SysAppLaunch(cardNo, dbID, 0, MMPRO_ADD_MESSAGE_WITH_ATTACHMENT, (MemPtr) &mailPlus, &result); 
						
						if(err)
							FrmCustomAlert(GeneralAlert, "Error adding mail to Versamail","","");					
						else					
							FrmCustomAlert(GeneralAlert, "Mail has been added to Versamail","","");
					
					} 
					else
						FrmCustomAlert(GeneralAlert, "Versamail not present","","");
						
					
				}								
				
				
				handled  = true;	
				break;
			}
			else if (eventP->data.ctlSelect.controlID == MainUILaunchButton)
			{
			
				/* Launch with UI */
				MMProLaunchStruct* maildetailsP = (MMProLaunchStruct*) MemPtrNew(sizeof(MMProLaunchStruct));
				MemHandle toH = NULL, bodyH = NULL, subjH = NULL;
				
				toH = MemHandleNew(15);
				bodyH = MemHandleNew(10);
				subjH = MemHandleNew(10);
				
				maildetailsP->to = MemHandleLock(toH);
				maildetailsP->cc = NULL;
				maildetailsP->bcc = NULL;
				maildetailsP->body = MemHandleLock(bodyH);
				maildetailsP->subject = MemHandleLock(subjH);
				maildetailsP->numAttach = 0;
				maildetailsP->attachments = NULL;
				maildetailsP->interactive = false;
				
				StrCopy(maildetailsP->to,"jon@doe.com");
				StrCopy(maildetailsP->body,"Test");
				StrCopy(maildetailsP->subject,"Hi");
				
				MemPtrSetOwner(maildetailsP, 0);
				MemHandleSetOwner(toH,0);
				MemHandleSetOwner(bodyH,0);
				MemHandleSetOwner(subjH,0);
						
						
										
				{
				
					UInt16				cardNo; 
					LocalID				dbID; 
					DmSearchStateType	searchState; 
					Err 					err; 
					DmGetNextDatabaseByTypeCreator(true, &searchState, 'appl', 
						'asc3', true,	&cardNo, &dbID); 
					 
					if (dbID) 
					{ 
						err = SysUIAppSwitch(cardNo, dbID, MMPRO_LAUNCH_CODE, maildetailsP); 
						if(err)
							FrmCustomAlert(GeneralAlert, "Error adding mail to Versamail","","");											
					}
					else
						FrmCustomAlert(GeneralAlert, "Versamail not present","","");
				}
				
				handled = true;
				break;
			}
			
		}//case
				
				
		default:	
			break;
	}
	
    
	return handled;
}

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
		/* change timeout if you need periodic nilEvents */
		EvtGetEvent(&event, evtWaitForever);

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
	
	return errNone;
}

/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

static void AppStop(void)
{
        
	/* Close all the open forms. */
	FrmCloseAllForms();

}

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
	}

	return errNone;
}
