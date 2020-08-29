/******************************************************************************
 * Copyright (c) 2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup Pushdemo
 */
 
/** 
 *  @brief The Pushdemo sample app shows how to set up socket connections between a 
 *		   Treo or Centro device and a laptop (or PC) and use this connection to push
 *		   a data packet from the laptop to the mobile device.
 *		   
 *
 *  @version 1.0	- Initial Revision (07/31/07)
 *
 *  @file Pushdemo.c
 */


 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <HsExt.h>
//#include <HsPhone.h>


#include "Pushdemo.h"
#include "Pushdemo_Rsc.h"
#include "common.h"


#define ServerTCPPort  9930

// size of buffers
#define MAXX 1024

/*********************************************************************
 * Entry Points
 *********************************************************************/


/*********************************************************************
 * Internal Structures
 *********************************************************************/

typedef struct 
{
   NetIPAddr addr;
   UInt16 port;
} PushDemoPreferenceType;




/*********************************************************************
 * Global variables
 *********************************************************************/
UInt16             gNetLibRefNum         = sysInvalidRefNum;
NetSocketRef       gSocketRef            = (NetSocketRef) -1;

PushDemoPreferenceType gPrefs = {0, 0};




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
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	
	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);


	switch (command)
	{
		case MainOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2007", "Palm DTS Team");
		handled = true;
		break;
	
		default:
			break;

	}

	return handled;
}




/*
 * FUNCTION: SocketOpen
 *
 * DESCRIPTION: This routine opens the socket
 *
 * PARAMETERS:
 *
 * fieldStatusP
 *     to update status
 * 
 *	   
 */  

Err SocketOpen(FieldPtr fieldStatusP)
{
	Err err = errNone;
	
	NetSocketLingerType linger;
    
	linger.onOff = true;
	linger.time = 0;

	
	gSocketRef = NetLibSocketOpen(gNetLibRefNum, netSocketAddrINET,
                                        netSocketTypeStream, 0, -1, &err);
                                        
    if (err != errNone || gSocketRef == (NetSocketRef) -1)
    {
        SetFieldTextFromStr( fieldStatusP, "Could not open socket", true);
     	return err;
    }
    
    SetFieldTextFromStr( fieldStatusP, "socket opened", true);
   
    return err;
} 



/*
 * FUNCTION: SocketConnect
 *
 * DESCRIPTION: This routine connects the socket to the server
 *
 * PARAMETERS:
 *
 * fieldStatusP
 *     to update status
 * 
 *	   
 */  

Err SocketConnect(NetSocketAddrINType *destAddr, FieldPtr fieldStatusP)
{
	Err err = errNone;
	Int16 rc    = -1;

	 SetFieldTextFromStr( fieldStatusP, "Connecting", true);
	 FrmCustomAlert(ConnectionAlert, "Proceeding to connect", "", ""); 
       
       rc = NetLibSocketConnect(gNetLibRefNum,                  // Network library
                                gSocketRef,                        // Socket reference
                                (NetSocketAddrType*)destAddr, // Destination address
                                sizeof(destAddr),              // Length of destAddr
                                1000,                            // Timeout
                                &err                         // Error result
                                );
                                
                                
       if (-1 == rc || errNone != err)
       {
           Err cerr = errNone;
           NetLibSocketClose(gNetLibRefNum, gSocketRef, -1, &cerr);
           gSocketRef = (NetSocketRef) -1;
         
           switch (err)
           {
           		case netErrTimeout:
           			FrmCustomAlert(ConnectionAlert, "Connection timed out", "", "");
           			break;
           			
           		case netErrSocketAlreadyConnected :
					FrmCustomAlert(ConnectionAlert, "Socket already connected", "", "");
					err = errNone;
					break;
				
				case netErrSocketClosedByRemote :
					FrmCustomAlert(ConnectionAlert, "Server socket not open", "", "");
					break;
			
				default:
					FrmCustomAlert(ConnectionAlert, "Socket connect error", "", "");
					break;
					
			}
		 return err;
       }
      
      SetFieldTextFromStr( fieldStatusP, "socket connected", true); 
         
      return err;
		
}


/*
 * FUNCTION: ReceivePacket
 *
 * DESCRIPTION: This routine receives TCP data packet pushed from a laptop or PC
 *
 * PARAMETERS:
 *
 * fieldStatusP
 *     fieldptr
 *
 */


Int16 ReceiveDataPacket(FieldPtr fieldStatusP)
{

	Err err = errNone;
	Int16 rc    = 0;
	
	char inpbuf[MAXX] = "";
	
	SetFieldTextFromStr( fieldStatusP, "Waiting to receive ...", true);
	
	  
     rc = NetLibReceive(gNetLibRefNum, gSocketRef,(void *) inpbuf, MAXX,
                            0, NULL, 0, -1, &err);
                           
                           
     if (rc == -1) 
     { 
          FrmCustomAlert(ConnectionAlert, "Receive timed out", "", "");
           				 
          return err;
      }
           	
      else
      {
            
          FrmCustomAlert(ConnectionAlert, "Data packet received.", "", "");
   		  SetFieldTextFromStr( fieldStatusP, inpbuf, true);
   	  }   
   	
     return err;
     
}     
       
      	 




/*
 *
 * FUNCTION:    SetValue
 *
 * DESCRIPTION: This function sets the numeric value of the specified
 *              text field.
 *
 * PARAMETERS:  fieldID  - ID of the field on the main form
 *              value  - the value to insert
 *
 * RETURNED:    True if successful, false otherwise.
 *
 * NOTES:       Determines the maximum length to use based upon the ID
 *
 */
 
static Boolean SetValue(UInt16 fieldID, Int32 value)
{
   FormType *formPtr;
   UInt16 fieldIndex;
   FieldType *fieldPtr;
   MemHandle newHandle, oldHandle;
   Char *text;

   formPtr = FrmGetFormPtr(MainForm);
   if (!formPtr) return false;

   fieldIndex = FrmGetObjectIndex(formPtr, fieldID);
   fieldPtr = (FieldType*)FrmGetObjectPtr(formPtr, fieldIndex);
   if (!fieldPtr) return false;

   if (MainPortField == fieldID) {
      newHandle = MemHandleNew(6);
   } else {
      newHandle = MemHandleNew(4);
   }
   if (!newHandle) return false;

   text = MemHandleLock(newHandle);
   if (MainPortField == fieldID) {
      StrPrintF(text, "%u", (UInt16)value);
   } else {
      StrPrintF(text, "%u", (UInt8)value);
   }
   MemHandleUnlock(newHandle);
   oldHandle = FldGetTextHandle(fieldPtr);
   FldSetTextHandle(fieldPtr, newHandle);
   if (oldHandle) MemHandleFree(oldHandle);

   return true;
}



/*
 *
 * FUNCTION:    GetValue
 *
 * DESCRIPTION: This function gets the numeric value of the specified
 *              text field.
 *
 * PARAMETERS:  fieldID  - ID of the field on the main form
 *              valueP - pointer to memory where value is returned
 *
 * RETURNED:    True if successful, false otherwise.
 *
 */
 
static Boolean GetValue(UInt16 fieldID, Int32 *valueP)
{
   FormType *formPtr;
   UInt16 fieldIndex;
   FieldType *fieldPtr;
   Char *text;

   formPtr = FrmGetFormPtr(MainForm );
   if (!formPtr) return false;

   fieldIndex = FrmGetObjectIndex(formPtr, fieldID);
   fieldPtr = (FieldType*)FrmGetObjectPtr(formPtr, fieldIndex);
   if (!fieldPtr) return false;

   text = FldGetTextPtr(fieldPtr);
   if (!text) return false;

   *valueP = StrAToI(text);
   return true;
}


/***********************************************************************
 *
 * FUNCTION:    GetAddr
 *
 * DESCRIPTION: This function gets the IP address and port that the user
 *              has entered.
 *
 * PARAMETERS:  gNetLibRefnum - Reference number of NetLib
 *              addrP        - pointer to memory where address is
 *                             returned
 *              portP        - pointer to memory where port is
 *                             returned
 *
 * RETURNED:    True if the IP address and port are valid, false
 *              otherwise.
 *
 * NOTES:       The only check peformed is to see whether the values
 *              are in the appropriate numeric ranges. This doesn't
 *              check whether the specified host exists or is listening
 *              on the specified port.
 *
 ***********************************************************************/
 
static Err GetAddr(UInt16 gNetLibRefNum, NetIPAddr *addrP, UInt16 *portP)
{
   UInt8 i;
   Int32 ip[4];
   Int32 port;
   Char textIP[16];
   Err err = errNone;

   // Get the values
   for (i = 0; i < 4; i++) {
      if (!GetValue(MainIP1Field + i, &(ip[i])) ||
          ip[i] > 255 ||
          ip[i] < 0) {
         return (err = 1);
      }
   }

   if (!GetValue(MainPortField, &port) ||
       port > 65535 ||
       port < 1) {
      return (err = 1);
   }

   StrPrintF(textIP, "%u.%u.%u.%u", (UInt8)ip[0], (UInt8)ip[1], (UInt8)ip[2], (UInt8)ip[3]);
   *addrP = NetLibAddrAToIN(gNetLibRefNum, textIP);
   *portP = (UInt16)port;
   
   
   if (*addrP == 0xFFFFFFFF)
	{
		FrmCustomAlert(ConnectionAlert, "IP address is null", "", "");
	}
   
   return err;
}







/*
 * FUNCTION: TCPMain
 *
 * DESCRIPTION: This routine handles the netlib socket calls
 *
 * PARAMETERS:
 *
 * none
 *     
 *
 */


int TCPMain()
{
		
	Err err    = errNone;
	Err ifErrs = errNone;
     
	NetSocketAddrINType destAddr;
    FieldPtr	fieldStatusP = (FieldPtr)NULL;
    FormType *frmP = (FormType *)FrmGetActiveForm( );
 
 
    fieldStatusP = (FieldPtr)FrmGetPtr( frmP, MainStatusField );
    SetFieldTextFromStr( fieldStatusP, "", true);
  	   
    
    // Get and validate the destination
    if ((err = GetAddr(gNetLibRefNum, &gPrefs.addr, &gPrefs.port)) != errNone)
     {
       FrmCustomAlert(ConnectionAlert, "Invalid server address", "", "");
       return err;
   	 }
    
    
     	 
	 /* Open the Network library */
	 err = NetLibOpen(gNetLibRefNum, &ifErrs);
     if (err || ifErrs)
     {
     	SetFieldTextFromStr( fieldStatusP, "Could not open netlib", true);
     	return err;
     }
     
         
     
      /* Open the socket */	
      if (gSocketRef == (NetSocketRef) -1)
      {
      	 err = SocketOpen(fieldStatusP);
      	 
      }
      
         
   // Connect the socket to its destination
   
   MemSet(&destAddr, sizeof(destAddr), 0);
   destAddr.family = netSocketAddrINET; // This should match the second argument to NetLibSocketOpen
   destAddr.port = gPrefs.port;
   destAddr.addr = gPrefs.addr;
   err = 0;
   
   err = SocketConnect(&destAddr, fieldStatusP);
   
   if (err == errNone)
   {
   
   		err = ReceiveDataPacket(fieldStatusP);
   		
   }
      
      
   if (gSocketRef != (NetSocketRef) -1)
    {
      Err cerr = errNone;
      NetLibSocketClose(gNetLibRefNum, gSocketRef, -1, &cerr);
      gSocketRef = (NetSocketRef) -1;
    }
    
      
 	NetLibClose(gNetLibRefNum, false);
 	      
    return err;
  
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
	int rc = -1;
		
	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			MainFormInit(frmP);
			
			// Set focus on MainStartButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainStartButton));
			
			// Load initial values from prefs
      		SetValue(MainIP1Field, (gPrefs.addr & 0xFF000000) >> 24);
     		SetValue(MainIP2Field, (gPrefs.addr & 0x00FF0000) >> 16);
      		SetValue(MainIP3Field, (gPrefs.addr & 0x0000FF00) >> 8);
      		SetValue(MainIP4Field, gPrefs.addr & 0x000000FF);
      		SetValue(MainPortField, ServerTCPPort);
						
			FrmDrawForm(frmP);
			
			FrmCustomAlert(ConnectionAlert, "Enter IP address of server. Then press Start.", "", "");
			
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
			switch (eventP->data.ctlSelect.controlID)
			{
			case MainStartButton:
			{
				rc = TCPMain();	
				handled = true;
				
				if (rc != 0)
				{
					eventP->eType = appStopEvent;
					EvtAddEventToQueue (eventP);
				}
			
				break;
			}
			case MainDoneButton:
			{
				eventP->eType = appStopEvent;
				EvtAddEventToQueue (eventP);
				handled = true;
				break;
			}	
			
			default:
				break;
			}
		}
		
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

	Err err = errNone;
	UInt16 prefsSize;
    
  if (gNetLibRefNum == sysInvalidRefNum)
  {
    err = SysLibFind("Net.lib", &gNetLibRefNum);
  }

  // Read the saved preferences / saved-state information.
   prefsSize = sizeof(gPrefs);
   PrefGetAppPreferences(appFileCreator, appPrefID, &gPrefs, &prefsSize, true);
    

  return err;
}



/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

static Err AppStop(void)
{
       
  // Write the saved preferences / saved-state information.  This data 
   // will be saved during a HotSync backup.
   PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
                          &gPrefs, sizeof (gPrefs), true);
         
  if (gNetLibRefNum != sysInvalidRefNum)
  {
    if (gSocketRef != (NetSocketRef) -1)
    {
      Err cerr = errNone;
      NetLibSocketClose(gNetLibRefNum, gSocketRef, -1, &cerr);
      gSocketRef = (NetSocketRef) -1;
    }
    NetLibClose(gNetLibRefNum, false);
    gNetLibRefNum = sysInvalidRefNum;
   
  }
  FrmCloseAllForms( );
  return errNone;

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
