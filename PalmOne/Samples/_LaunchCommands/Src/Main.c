/******************************************************************************
 * Copyright (c) 2004-2006 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup _LaunchCommands
 */
 
/**
 *  @brief 	This sample code shows how to use various Phone app launch commands to dial or 
 *              show keypad. It also shows how to launch the Web browser, including in 
 *		minimal mode.
 *
 *
 *  @version 1.0	- Initial Revision 
 *  @version 1.1	- Fixed errors in MainDialPadEmptyButton and MainDialButton cases
 *			- Separated common functions from specific _LaunchCommands 
 *			  related ones. (11/28/06) - Usha Shekar
 *
 *  @file Main.c
 */

 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <palmOneCreators.h>
#include <HsCreators.h>
#include <HelperServiceClass.h>
#include <ByteOrderUtils.h>
#include <palmOnePhotoCommon.h>
#include <HsAppLaunchCmd.h>
#include <HsExt.h>

#include "LaunchCommands.h"
#include "LaunchCommands_Rsc.h"

// Don't define the following if HsAppLaunchCmd.h is included
// Launch the phone application and go to a view
#define phoneAppLaunchCmdGoToView (sysAppLaunchCmdCustomBase+24)

typedef struct
{
    UInt32  viewType;           /**< the view type to displayed */
    UInt16  updateCode;         /**< The View Manager will pass this update code
                                     along with pUpdateParams if updateCode does
                                     not equal kPhoneAppUpdateCodeNone */
    void*   pUpdateParams;      /**< The View Manager will pass this data block to the view
                                     once the view has been opened, if updateCode does
                                     not equal kPhoneAppUpdateCodeNone*/
}
PhoneAppLaunchCmdGoToViewType, *PhoneAppLaunchCmdGoToViewPtr;


#ifndef __PHONEAPPCOMMONPROT_H__ // Don't define the following if PhoneAppCommonProt.h is included

#define kPhoneViewNone                  0
#define	kPhoneViewMainType              'Main'
#define	kPhoneViewDialPadType           'DlPd'
#define	kPhoneViewContactsType          'Cont'
#define	kPhoneViewFavoritesType         'Favs'
#define	kPhoneViewCallLogType           'ClLg'
#define kPhoneViewActiveCallType        'ActC'
#define kPhoneViewIncomingCallType      'IncC'

#define kPhoneAppUpdateCodeNone         0x0000

#endif

#define MaximumDoneButtonLabelSize 32


/*********************************************************************
 * Entry Points
 *********************************************************************/
 
Boolean MainFormHandleEvent(EventType * eventP);

/*********************************************************************
 * Global variables
 *********************************************************************/
 webBrowserMinimalModeLaunchInfo* minimalModeInfo;

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/

/**
 * @brief GetObjectPtr
 *	     This routine returns a pointer to an object in the current form. 
 *
 *
 * @param   objectID:   IN:  	id of the object to display
 * @retval		void:	address of object as a void pointer
 *
 *
 * @version This function was added in version 1.0
 *
 **/

static void * GetObjectPtr(UInt16 objectID)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}


/**
 * @brief MainFormInit
 *	     This function initializes the main form 
 *
 *
 * @param frmP:   IN:  	Pointer to the main form
 * @retval		void:	None
 *
 *
 * @version This function was added in version 1.0
 *
 **/

static void MainFormInit(FormType *frmP)
{

}


/**
 * @brief MainFormDoCommand
 *	    This routine performs the menu command specified.
 *
 *
 * @param commnad:   IN:  	Menu item id.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.1
 *
 **/ 

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	
	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);

	/* The About button was hit -- display contents of About box */
	switch (command)
	{
	case MainOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
		handled = true;
		break;
	
	default:
		break;
	
	}

	return handled;
}



/**
 * @brief MainFormHandleEvent
 *	     Main form event handler.
 *
 *
 * @param *eventP :   IN:  	Pointer to an event.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/


Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP;
	
	UInt16 cardNo;
	LocalID dbID;
	DmSearchStateType stateInfo;
	SysNotifyParamType *notifyParamP;

	PhoneAppLaunchCmdDialPtr paramsP = NULL;
	
	UInt16 size = sizeof(PhoneAppLaunchCmdDialType);
	
	FieldType *field = (FieldType*)GetObjectPtr(MainDescriptionField);
	Char urlP[50];
	Char* numberP;
	Char* url;

    Char* tempStr;

	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			MainFormInit(frmP);
			
			// Set focus on MainContactButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainContactButton));
						
			FrmDrawForm(frmP);
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
			switch(eventP->data.ctlSelect.controlID)
			{
				case MainContactButton:
					notifyParamP = (SysNotifyParamType *) MemPtrNew(sizeof(SysNotifyParamType));
					MemSet(notifyParamP, sizeof(SysNotifyParamType), 0);

					notifyParamP->notifyType = addrAppNotificationCreateNewRecord;
					notifyParamP->broadcaster = 'LCTt';
					notifyParamP->notifyDetailsP = NULL;
					notifyParamP->handled = false;

					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, kPalmOneCreatorIDContacts, true,	&cardNo, &dbID);
					SysUIAppSwitch(cardNo, dbID, sysAppLaunchCmdNotify, notifyParamP);
				
					break;
					
				case MainFavoritesButton:
				{
					PhoneAppLaunchCmdGoToViewPtr cmdPBP = (PhoneAppLaunchCmdGoToViewPtr) MemPtrNew(sizeof(PhoneAppLaunchCmdGoToViewType));
    				MemPtrSetOwner(cmdPBP, 0);
    				
    				cmdPBP->viewType = EndianSwap32(kPhoneViewFavoritesType);
    				cmdPBP->updateCode = EndianSwap32(kPhoneAppUpdateCodeNone);
    				
					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication,  hsFileCPhone, true, &cardNo, &dbID);
					SysUIAppSwitch (cardNo, dbID, phoneAppLaunchCmdViewSpeed, cmdPBP);
				}
					break;

				case MainCallLogButton:
				
				{
					PhoneAppLaunchCmdGoToViewPtr cmdPBP = (PhoneAppLaunchCmdGoToViewPtr) MemPtrNew(sizeof(PhoneAppLaunchCmdGoToViewType));
    				MemPtrSetOwner(cmdPBP, 0);
    				
    				DbgMessage("in call log\n");
    				    				
    				cmdPBP->viewType = EndianSwap32(kPhoneViewCallLogType);
    				cmdPBP->updateCode = EndianSwap32(kPhoneAppUpdateCodeNone);
    				DbgMessage("done swapping\n");
    				
    				
					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, hsFileCPhone, true, &cardNo, &dbID);
					DbgMessage("ready to switch app\n");
					SysUIAppSwitch (cardNo, dbID, phoneAppLaunchCmdViewHistory, cmdPBP);
				}
					break;

				case MainMinBlazerButton:
					if (FldGetTextPtr(field))
						StrCopy(urlP, FldGetTextPtr(field));
					else {
					    FrmAlert(MissingUrlAlert);
						break;
						
				    }

				    minimalModeInfo = MemPtrNew( sizeof(webBrowserMinimalModeLaunchInfo) );

				    url = MemPtrNew(StrLen(urlP) + 1);
    				if (!url) break;
    				
    				StrCopy(url, urlP);

					// Since we're calling into ARM, byte-swap the pointers
					minimalModeInfo->launchUrl = (Char*)EndianSwap32(url);

				    tempStr = MemPtrNew(MaximumDoneButtonLabelSize);
				    StrCopy(tempStr, "Done");

				    // Since we're calling into ARM, byte-swap the pointers
				    minimalModeInfo->doneButtonLabel = (Char*)EndianSwap32(tempStr);

					MemPtrSetOwner(minimalModeInfo, 0); // No need to free this chunck of memory - the OS will do it

					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, hsFileCBlazer3, true, &cardNo, &dbID);
					SysUIAppSwitch(cardNo, dbID, sysAppLaunchWebBrowserMinimalMode, minimalModeInfo);
			
					break;

				case MainMaxBlazerButton:
					if (FldGetTextPtr(field))
						StrCopy(urlP, FldGetTextPtr(field));
					else {
					    FrmAlert(MissingUrlAlert);
						break;
						
				    }
					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, hsFileCBlazer3, true, &cardNo, &dbID);
				    
				    url = MemPtrNew(StrLen(urlP) + 1);
    				if (!url) break;
    				
    				StrCopy(url, urlP);

					MemPtrSetOwner(url, 0); 
					SysUIAppSwitch(cardNo, dbID, sysAppLaunchCmdGoToURL, url);
				
					break;
			
				case MainDialPadEmptyButton:
				{	
					// Setup a parameter block but without a phone number
					paramsP = MemPtrNew (size);
					
					MemSet (paramsP, size, 0);
					paramsP->version = 1;
					paramsP->failLaunchCreator = 'LCTt';
	 				MemPtrSetOwner(paramsP, 0);
	 				 				
					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, hsFileCPhone, true, &cardNo, &dbID);
					SysUIAppSwitch(cardNo, dbID, phoneAppLaunchCmdViewKeypad, paramsP);
					
					break;
				}
				
			
			
				/* The MainDialPadFilledButton launch code does not prefill the number in the 
				   dial pad on the Treo 680 */
				   
				case MainDialPadFilledButton:
				case MainDialButton:
						
					numberP = FldGetTextPtr(field);
			
					// Setup a parameter block so the Phone application automatically dials a phone number
					if (numberP)
						size += StrLen(numberP) + 1;
					else break;
	   				
	   				paramsP = MemPtrNew (size);
					
					MemSet (paramsP, size, 0);
					
					paramsP->version =1;
					paramsP->failLaunchCreator = 'LCTt';
						 				
	 				if (numberP)
	 				{
						paramsP->number = MemPtrNew(StrLen(numberP) + 1);
						StrCopy(paramsP->number, numberP);
						MemPtrSetOwner (paramsP->number, 0);
					}

					MemPtrSetOwner(paramsP, 0);
					DmGetNextDatabaseByTypeCreator(true, &stateInfo, sysFileTApplication, hsFileCPhone, true, &cardNo, &dbID);
										
										
					if (eventP->data.ctlSelect.controlID == MainDialButton)
						SysUIAppSwitch(cardNo, dbID, phoneAppLaunchCmdDial, paramsP);
					else if (eventP->data.ctlSelect.controlID == MainDialPadFilledButton)
						SysUIAppSwitch(cardNo, dbID, phoneAppLaunchCmdViewKeypad, paramsP);
					
					break;

				default: break;
			}

			break;
			
			default:
			    break;
		}
	}
    
	return handled;
}



