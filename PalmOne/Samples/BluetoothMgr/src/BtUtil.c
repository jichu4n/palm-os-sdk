/****************************************************************************
 * Copyright (c) 2007 Palm, Inc. All rights reserved.
 *
 * File: BtUtil.c
 *
*****************************************************************************/
#include <PalmOS.h>
#include <HsExt.h>

#include "BtUtil.h"
#include "LogUtil.h"
#include "AppResources.h"

/****************************/

#define kInvalidSdpRecord 0
#define kInvalidSocket    0xFFFF
#define appName           "BtMgr"

Boolean BtUtilDisconnect();
Boolean BtUtilIsConnected();

/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/
static Err SendData();
static Err BtUtilAdvanceCredit();
static BtLibSocketListenInfoType BtUtilGetRfCommSocketListenInfo();
static void BtUtilManagementEventCallback(BtLibManagementEventType *mEventP, UInt32 refCon);
static void BtLibSocketCallbackProc(BtLibSocketEventType *mEventP, UInt32 refCon);
static void BtUtilConnectToRfCommSocket(BtLibRfCommServerIdType serverChannel);

 /***********************************************************************
 *
 *	Global variables
 *
 ***********************************************************************/
UInt16  gBtLibRefNum = 0;
MemHandle gDataHandle = NULL;
Boolean gBtLibOpened  = false;
BtLibFriendlyNameTypePtr deviceNameP;
BtLibDeviceAddressType gBtLibDeviceConnectedTo;
BtLibFriendlyNameTypePtr deviceNameP;
BtLibSocketRef socketRefP = kInvalidSocket;
BtLibSocketRef sdpSocket  = kInvalidSocket;
BtLibSdpRecordHandle gSdpRecordH = kInvalidSdpRecord;

// {8A97B361-6221-4459-9241-227576CC6AB8}
static const BtLibSdpUuidType kRfCommUUID = {btLibUuidSize128,
{ 0x8a, 0x97, 0xb3, 0x61, 0x62, 0x21, 0x44, 0x59, 0x92, 0x41, 
0x22, 0x75, 0x76, 0xcc, 0x6a, 0xb8 } };	

static Boolean bBtUtilACLEstablished;

/***********************************************************************
 *
 * FUNCTION:    BtUtilDeviceHasBt
 *
 * DESCRIPTION: Determine if device supports Bluetooth
 *
 * PARAMETERS:  launchFlags
 *
 * RETURNED:    error 
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Err BtUtilDeviceHasBt(UInt16 launchFlags)
{   
   Err    error = errNone;
   UInt32 btVersion; 
 
   // Make sure Bluetooth components are installed 
   // This check also ensures Palm OS 4.0 or greater 
   if (FtrGet(btLibFeatureCreator, btLibFeatureVersion, &btVersion) != errNone) { 
      // Alert the user if it's the active application 
      if ((launchFlags & sysAppLaunchFlagNewGlobals) && (launchFlags & sysAppLaunchFlagUIApp)) 
         FrmAlert (MissingBtComponentsAlert); 
      error = sysErrRomIncompatible; 
   }
   return error; 
} 


/***********************************************************************
 *
 * FUNCTION:    BtUtilLibIsOpen
 *
 * DESCRIPTION: Determine if Bluetooth Library is open
 *
 * PARAMETERS:  
 *
 * RETURNED:    gBtLibOpened
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
 Boolean BtUtilLibIsOpen()
 {
 	return(gBtLibOpened);
 }


/***********************************************************************
 *
 * FUNCTION:    BtUtilLibOpen
 *
 * DESCRIPTION: Open Bluetooth library and register for Bluetooth
 *              manager notifications
 *
 * PARAMETERS:  
 *
 * RETURNED:    gBtLibOpened
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
 Boolean BtUtilLibOpen()
{
   Err error = errNone;
   
   error = BtLibOpen(gBtLibRefNum,false);
   if(error) {
      LogPrintF("-> BtLib open failed: 0x%hx", error);
   } else {
   	  LogPrintF("-> BtLib opened");
   	  gBtLibOpened = true;
   	  
	  error = BtLibRegisterManagementNotification(gBtLibRefNum, BtUtilManagementEventCallback, 0);
	  if(error == btLibErrNoError) {
	  	  LogPrintF("-> Registered for BtMgr events");
	  } else if(error == btLibErrAlreadyRegistered) {
	  	  LogPrintF("-> Already registered for BtMgr events");
	  } else if(error == btLibErrNotOpen) {
	  	  LogPrintF("-> Error registering for BtMgr events - bt lib not open");
	  } else if(error == btLibErrParamError) {
	  	  LogPrintF("-> Error registering for BtMgr events - parameters invalid");
	  } else if(error == btLibErrStackNotOpen) {
	  	  LogPrintF("-> Error registering for BtMgr events - bt stack not initialized");
	  } else if(error == btLibErrTooMany) {
	  	  LogPrintF("-> Error registering for BtMgr events - no space for callback");
	  } else {
	  	  LogPrintF("-> Error registering for BtMgr events");
	  }
   }  
   return gBtLibOpened;
}

/***********************************************************************
 *
 * FUNCTION:    BtUtilLibInit
 *
 * DESCRIPTION: Initialize and open Bluetooth library
 *
 * PARAMETERS:  
 *
 * RETURNED:    opened
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Boolean BtUtilLibInit()
{
	Err error;
    Boolean opened = false;
    
    gBtLibDeviceConnectedTo.address[0] = 0;
    gBtLibDeviceConnectedTo.address[1] = 0;   
    gBtLibDeviceConnectedTo.address[2] = 0;
    gBtLibDeviceConnectedTo.address[3] = 0;
    gBtLibDeviceConnectedTo.address[4] = 0;
    gBtLibDeviceConnectedTo.address[5] = 0;

	// Find the Library
	if( SysLibFind( btLibName, &gBtLibRefNum) ) {
		// Load the library if it can't be found	
		error = SysLibLoad( sysFileTLibrary , sysFileCBtLib, &gBtLibRefNum) ;
		if(error)
			LogPrintF("-> Error - could not find BtLib");
		else {
			LogPrintF("-> BtLib Loaded");
			opened = BtUtilLibOpen(); // Open Bluetooth Library
		}
	} else {
		LogPrintF("-> BtLib found");
		opened = BtUtilLibOpen();
	}	
    return opened;
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilLibUninit
 *
 * DESCRIPTION: Clean up, unregister for Bluetooth Manager notifications
 *              and if connected, disconnect from Bluetooth device
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void BtUtilLibUninit()
{
	Err error;
	
	if(gDataHandle != NULL) {
		MemHandleUnlock(gDataHandle);
	    MemHandleFree(gDataHandle);
	    gDataHandle = NULL;
	}
    
    error = BtLibUnregisterManagementNotification(gBtLibRefNum, BtUtilManagementEventCallback);
    LogPrintF("-> Unregistering for BtMgr notifications...");
    if(error == btLibErrNoError) {                                     
        LogPrintF("-> Unregistered for BtMgr notifications");
	} else if(error == btLibErrError) {
	    LogPrintF("-> Error unregistering for BtMgr notifications - no callback function registered");
	} else if(error == btLibErrNotOpen) {
		LogPrintF("-> Error unregistering for BtMgr notifications - bluetooth library not open");
	} else if(error == btLibErrParamError) {
	    LogPrintF("-> Error unregistering for BtMgr notifications - params invalid");
	} else if(error == btLibErrStackNotOpen) {
	    LogPrintF("-> Error unregistering for BtMgr notifications - Bt stack uninitialized");
	} else {
	  	LogPrintF("-> Error while unregistering for BtMgr notifications");
	}
    
	BtUtilDisconnect(); 
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilDiscoverandConnectToDevice
 *
 * DESCRIPTION: Discover Bt enabled devices and connect to chosen one
 *
 * PARAMETERS:  
 *
 * RETURNED:    result - boolean telling if connected or not
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Boolean BtUtilDiscoverandConnectToDevice()
{
	Err     error;
	Boolean result = false;
	BtLibDeviceAddressType selectedDevice;
	
	error = BtLibDiscoverSingleDevice	(gBtLibRefNum, 
	                                     /* instruction text */ NULL, 
	                                     /* class of devices */ NULL, 
	                                     /* class of devices count */ 0, 
	                                     &selectedDevice, 
	                                     /* display address as name */ false, 
	                                     /* show last list */ false);
	                                
	if(error == btLibErrNoError) {                                     
        result = BtUtilConnect(&selectedDevice);
	} else if(error == btLibErrCanceled) {
	    LogPrintF("-> User canceled device discovery");
	} else if(error == btLibErrNotOpen) {
		LogPrintF("-> Error during device discovery - bluetooth library not open");
	} else if(error == btLibErrStackNotOpen) {
	    LogPrintF("-> Error during device discovery - bluetooth stack failed to initialize");
	} else {
	  	LogPrintF("-> Error while discovering device");
	}
	return result;
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilGetDeviceName
 *
 * DESCRIPTION: Get Bt name from connected device
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void BtUtilGetDeviceName()
{   
	Err error;
	BtLibFriendlyNameType deviceName;
	deviceName.name = MemPtrNew(64);
	deviceName.nameLength = 64;
	deviceNameP = &deviceName;
	
	if(!BtUtilLibIsOpen()) {
	   LogPrintF("-> Error - Bluetooth library is not open");
	   return;
	}
	
	if(BtUtilIsConnected()) {
			
    	error = BtLibGetRemoteDeviceName(gBtLibRefNum, &gBtLibDeviceConnectedTo, deviceNameP, btLibRemoteOnly);
		if(error == btLibErrNoError) {
			LogPrintF("-> Getting remote device name...");
		} else if(error == btLibErrBusy) {
			LogPrintF("-> Error getting remote device name - name already pending");
		} else if(error == btLibErrNotOpen) {
			LogPrintF("-> Error getting remote device name - BtLib not open");
		} else if(error == btLibErrPending) {
			LogPrintF("-> Getting remote device name...");
		} else if(error == btLibErrStackNotOpen) {
			LogPrintF("-> Error getting remote device name - Bt stack failed to initialize");
		} else {
	  	  LogPrintF("-> Error while getting remote device name");
		}	              
	}
}

/***********************************************************************
 *
 * FUNCTION:    BtUtilSocketClose
 *
 * DESCRIPTION: Close socket
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void BtUtilSocketClose()
{
   Err error;
   
   if(socketRefP != kInvalidSocket) {
      error = BtLibSocketClose(gBtLibRefNum, socketRefP);
      if(error == btLibErrNoError) {
      	 LogPrintF("-> Socket closed");
      	 socketRefP = kInvalidSocket;
      } else if(error == btLibErrNotOpen) {
      	 LogPrintF("-> Error closing socket - BtLib not open");
      } else if(error == btLibErrSocket) {
      	 LogPrintF("-> Error closing socket - socket invalid");
      } else if(error == btLibErrStackNotOpen) {
      	 LogPrintF("-> Error closing socket - Bt stack failed to initialize");
      } else {
	  	 LogPrintF("-> Error during socket close");
      }
   }
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilManagementEventCallback
 *
 * DESCRIPTION: Handle radio events
 *
 * PARAMETERS:  mEventP - 
 * 				refCon  - 
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/
static void BtUtilManagementEventCallback(BtLibManagementEventType *mEventP, UInt32 refCon)
{	
	switch(mEventP->event)
	{
		case btLibManagementEventRadioState:
            LogPrintF("-> Bluetooth radio changed state");
			break;
			
		case btLibManagementEventInquiryResult:
			LogPrintF("-> Inquiry result");
			break;
			
		case btLibManagementEventInquiryComplete:
			LogPrintF("-> Inquiry complete");
			break;
			
		case btLibManagementEventInquiryCanceled:
			LogPrintF("-> Inquiry canceled");
			break;
		    
		case btLibManagementEventACLConnectInbound:
	        LogPrintF("-> Inbound connection established");
	        bBtUtilACLEstablished = true;
			break;
			
		case btLibManagementEventACLConnectOutbound:
		
		    if(mEventP->status == btLibErrNoError) {
			    LogPrintF("-> Outbound connection made");
			    bBtUtilACLEstablished = true;

		    } else { 
		    	LogPrintF("-> Outbound connection failed");
		    }
		    break;
		    
		case btLibManagementEventAuthenticationComplete:
            LogPrintF("-> Authentication complete");
			break;
			
		case btLibManagementEventEncryptionChange:
	        LogPrintF("-> Encryption change");
			break;
			
		case btLibManagementEventACLDisconnect:
			LogPrintF("-> Disconnected ACL link");
			bBtUtilACLEstablished = false;
			break;
				
		case btLibManagementEventNameResult:
		    if(mEventP->status == btLibErrNoError) {
			    LogPrintF("-> Device name request complete");
			    LogPrintF("-> Device name = %s", mEventP->eventData.nameResult.name);
		    } else {
		    	LogPrintF("-> Device name request failed");
		    }
		    
		    if(deviceNameP) {
			   MemPtrFree(deviceNameP->name);
			   deviceNameP->name = NULL;
			}
			
			break;
			
		case btLibManagementEventRoleChange:
		    LogPrintF("-> Event role change");
			break;
			
		case btLibManagementEventAccessibilityChange:
			switch( mEventP->eventData.accessible)
			{
				case btLibNotAccessible:
					LogPrintF("-> Not Accessible");
					break;
				case btLibConnectableOnly:
					LogPrintF("-> Connectable");
					break;
				case btLibDiscoverableAndConnectable:
					LogPrintF("-> Discoverable and Connectable");
					break;
			}
			break;
		default:
		    LogPrintF("-> BtLibManager default case");
		    break;
	}
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilConnect
 *
 * DESCRIPTION: Connect to specified device
 *
 * PARAMETERS:  device - device to connect to
 *
 * RETURNED:    result - whether or not connected
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Boolean BtUtilConnect(BtLibDeviceAddressType *device)
{
	Boolean result = false;
	Err error = BtLibLinkConnect(gBtLibRefNum, device);
	
	if(error == btLibErrPending) {
	    LogPrintF("-> Connecting...");   	
	    gBtLibDeviceConnectedTo = *device;
	    result = true;
	} else if(error == 	btLibErrAlreadyConnected) {
        LogPrintF("-> Error connecting to device - already connected");   	
	} else if(error == btLibErrBluetoothOff) {
 	    LogPrintF("-> Error connecting to device - bluetooth radio is off");   	
	} else if(error == btLibErrBusy) {
		LogPrintF("-> Error connecting to device - piconet being created or distroyed");
	} else if(error == btLibErrNotOpen) {
        LogPrintF("-> Error connecting to device - bluetooth library not open");
    } else if(error == btLibErrStackNotOpen) {
    	LogPrintF("-> Error connecting to device - bluetooth stack failed to initialize");
    } else if(error == btLibErrTooMany) {
		LogPrintF("-> Error connecting to device - can't create another ACL link");
	} else {
	  	LogPrintF("-> Error connecting to device");
	}
	return result;
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilDisconnect
 *
 * DESCRIPTION: Disconnect from device
 *
 * PARAMETERS:  
 *
 * RETURNED:    isDisconnected
 *
 ***********************************************************************/
Boolean BtUtilDisconnect()
{
    Boolean isDisconnected = false;
	Err error;
	
	// Disconnect if connected
	if(BtUtilIsConnected()) {
		error = BtLibLinkDisconnect(gBtLibRefNum, &gBtLibDeviceConnectedTo);
		if(error == btLibErrNoError) {

		    gBtLibDeviceConnectedTo.address[0] = 0;
		    gBtLibDeviceConnectedTo.address[1] = 0;   
		    gBtLibDeviceConnectedTo.address[2] = 0;
		    gBtLibDeviceConnectedTo.address[3] = 0;
		    gBtLibDeviceConnectedTo.address[4] = 0;
		    gBtLibDeviceConnectedTo.address[5] = 0;	    
		   
		    isDisconnected = true;
		    
		} else if(error == btLibErrPending) {
	        LogPrintF("-> Disconnecting...");   	
		} else if(error == btLibErrBusy) {
	 	    LogPrintF("-> Error disconnecting - piconet is being distroyed");   	
		} else if(error == btLibErrNoConnection) {
			LogPrintF("-> Error disconnecting - not connected to specified device");
		} else if(error == btLibErrNotOpen) {
	        LogPrintF("-> Error disconnecting from device - bluetooth library not open");
	    } else if(error == btLibErrStackNotOpen) {
	    	LogPrintF("-> Error disconnecting from device - bluetooth stack failed to initialize");
	    } else {
		  	LogPrintF("-> Error while disconnecting from device");
	    }
	} else {
		isDisconnected = true;
	}
	
    return isDisconnected;
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilIsConnected
 *
 * DESCRIPTION: Determine if device is connected
 *
 * PARAMETERS:  
 *
 * RETURNED:    isConnected
 *
 ***********************************************************************/
Boolean BtUtilIsConnected()
{
	Boolean linkAuthenticated;
	Boolean isConnected = false;
	
    Err error = BtLibLinkGetState(gBtLibRefNum, &gBtLibDeviceConnectedTo, 
                                  btLibLinkPref_Authenticated, &linkAuthenticated, sizeof(Boolean));
	
	if(error == btLibErrNoError) {
		isConnected = true;  	
	} else if(error == btLibErrNoAclLink) {
 	    LogPrintF("-> Error getting link state - device is not connected");   	
	} else if(error == btLibErrNotOpen) {
		LogPrintF("-> Error getting link state - Bt Lib not open");
	} else if(error == btLibErrParamError) {
        LogPrintF("-> Error with param when getting link state");
    } else if(error == btLibErrStackNotOpen) {
    	LogPrintF("-> Error getting link state - bluetooth stack failed to initialize");
    } else {
	  	LogPrintF("-> Error while getting link state");
	}

    return isConnected;
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilCreateRfCommSocket
 *
 * DESCRIPTION: Create RFComm socket
 *
 * PARAMETERS:  
 *
 * RETURNED:    error
 *
 ***********************************************************************/
static Err BtUtilCreateRfCommSocket()
{ 	
	Err error = BtLibSocketCreate(gBtLibRefNum, &socketRefP, BtLibSocketCallbackProc, 0, btLibRfCommProtocol);
    
	if(error == btLibErrNoError) {                                     
	    LogPrintF("-> Created RFCOMM socket");
	} else if(error == btLibErrNotOpen) {
	    LogPrintF("-> Error RFCOMM create - BtLib not open");
	} else if(error == btLibErrParamError) {
		LogPrintF("-> Error RFCOMM create - socketRef or callback is NULL");
	} else if(error == btLibErrStackNotOpen) {
	    LogPrintF("-> Error RFCOMM create - bluetooth stack failed to initialize");
	} else if(error == btLibErrTooMany) {
	    LogPrintF("-> Error RFCOMM create - maxed out on sockets");
	} else {
	  	LogPrintF("-> Error while creating RFCOMM socket");
	}
	
	return error;
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilSocketListen
 *
 * DESCRIPTION: Set up device to receive Bluetooth data
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
void BtUtilSocketListen()
{
	Err error;
	BtLibSocketListenInfoType listenInfo;
	   
    if(!BtUtilLibIsOpen()) {
	   LogPrintF("-> Error - Bluetooth library is not open");
	   return;
	}   
	   
	// 1. Create RFCOMM socket
	BtUtilCreateRfCommSocket();
	
	// 2. Set up RFCOMM socket as listener
	listenInfo = BtUtilGetRfCommSocketListenInfo();
	
	// listenInfo.data.RfComm.serviceID will be filled in by call to BtLibSocketListen
	error = BtLibSocketListen(gBtLibRefNum, socketRefP, &listenInfo);
	if(error == btLibErrNoError) {
	   LogPrintF("-> Socket is listening");
	} else if(error == btLibErrNotOpen) {
	   LogPrintF("-> Listening error - BtLib not open");
	} else if(error == btLibErrParamError) {
	   LogPrintF("-> Listening error - listenInfo is NULL");
	} else if(error == btLibErrSocket) {
	   LogPrintF("-> Listening error - socket invalid");
	} else if(error == btLibErrSocketProtocol) {
	   LogPrintF("-> Listening error - protocol not supported");
	} else if(error == btLibErrSocketRole) {
	   LogPrintF("-> Listening error - socket already listening/connected");
	} else if(error == btLibErrStackNotOpen) {
	   LogPrintF("-> Listening error - Bt stack uninitialized");
	} else if(error == btLibErrTooMany) {
	   LogPrintF("-> Listening error - no resources for socket");
	} else {
	   LogPrintF("-> Error setting up socket as listener");
	}
	
	// 3.  Allocate memory chunk that represents an SDP service record
	error = BtLibSdpServiceRecordCreate(gBtLibRefNum, &gSdpRecordH);
	if(error == btLibErrNoError) {
	   LogPrintF("-> SDP service record created");
	} else if(error == btLibErrNotOpen) {
	   LogPrintF("-> SDP error - BtLib not open");
	} else if(error == btLibErrOutOfMemory) {
	   LogPrintF("-> SDP error - out of memory");
	} else if(error == btLibErrParamError) {
	   LogPrintF("-> SDP error - handle is NULL");
	} else if(error == btLibErrStackNotOpen) {
	   LogPrintF("-> SDP error - Bt stack uninitialized");
	} else {
	   LogPrintF("-> Error creating sdp service record");
	} 
	
	// 4. Init SDP memory record to represent the newly-created RFCOMM listener socket as service
	error = BtLibSdpServiceRecordSetAttributesForSocket(gBtLibRefNum, socketRefP, (BtLibSdpUuidType*)&kRfCommUUID, 1, appName, StrLen(appName), gSdpRecordH);
    if(error == btLibErrNoError) {
       LogPrintF("-> Initialized SDP record");
       
       // 5. Make SDP memory record visible to remote devices
       error = BtLibSdpServiceRecordStartAdvertising(gBtLibRefNum, gSdpRecordH);
       if(error == btLibErrNoError) {
       	  LogPrintF("-> Advertised SDP record");
       } else if(error == btLibErrNotOpen) {
       	  LogPrintF("-> Error advertising SDP record - bt lib not open");       	
       } else if(error == btLibErrParamError) {
          LogPrintF("-> Error advertising SDP record - recordH invalid handle");       	
       } else if(error == btLibErrSdpAdvertised) {
          LogPrintF("-> Error advertising SDP record - record already accessible");
       } else if(error == btLibErrStackNotOpen) {
          LogPrintF("-> Error advertising SDP record - bt stack failed to init");       	
       } else {
	  	  LogPrintF("-> Error advertising service record");
       }
    } else if(error == btLibErrNotOpen) {
       LogPrintF("-> Error initing SDP record - bt lib not open");       	
    } else if(error == btLibErrOutOfMemory) {
       LogPrintF("-> Error initing SDP record - out of memory");       	
    } else if(error == btLibErrParamError) {
       LogPrintF("-> Error initing SDP record - recordH invalid handle");       	
    } else if(error == btLibErrSocket) {
       LogPrintF("-> Error initing SDP record - socket invalid or not in use");       	
    } else if(error == btLibErrSocketRole) {
       LogPrintF("-> Error initing SDP record - socket not listener socket");       	
    } else if(error == btLibErrStackNotOpen) {
       LogPrintF("-> Error initing SDP record - bt stack failed to init");       	
    } else {
	   LogPrintF("-> Error setting service record attrs");
    } 
}


/***********************************************************************
 *
 * FUNCTION:    BtLibSocketCallbackProc
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  mEventP - socket event
 * 				refCon  - caller defined data
 *
 * RETURNED:    
 *
 ***********************************************************************/
static void BtLibSocketCallbackProc(BtLibSocketEventType *mEventP, UInt32 refCon)
{
	Err error;
	
	switch(mEventP->event) {
		case btLibSocketEventConnectRequest: 
		    LogPrintF("-> Connect request");
		    error = BtLibSocketRespondToConnection(gBtLibRefNum, socketRefP, true);    
		    if(error == btLibErrNoError) {                                     
			    LogPrintF("-> Accepted connection"); 
			} else if(error == btLibErrFailed) {
			    LogPrintF("-> Error - parameters invalid");
			} else if(error == btLibErrPending) {
				LogPrintF("-> Incoming connection...");
			} else if(error == btLibErrNotOpen) {
			    LogPrintF("-> Error - BtLib not open");
			} else if(error == btLibErrSocket) {
			    LogPrintF("-> Error - socket invalid or not in use");
			} else if(error == btLibErrSocketProtocol) {
			    LogPrintF("-> Error - socket protocol not supported");
			} else if(error == btLibErrSocketRole) {
			    LogPrintF("-> Error - socket not listener socket");
			} else if(error == btLibErrStackNotOpen) {
			    LogPrintF("-> Error - Bt stack not initialized");
			} else {
	  	        LogPrintF("-> Error while responding to connection request");
		    }
		    break;
		    
		case btLibSocketEventData:
		{   Char *tmp;
			tmp = (Char *)mEventP->eventData.data.data;
		    LogPrintF("-> Data received on the socket");
		    LogPrintF(tmp);
		    // Close sockets
		    BtUtilSocketClose();
		    break;
	    }
		case btLibSocketEventSendComplete:
		    LogPrintF("-> Data send completed");
		    // Close sockets
		    BtUtilSocketClose();		     
		    break;
			
	    case  btLibSocketEventConnectedInbound:
	        LogPrintF("-> RFComm inbound connection made");
	        // Get socket from event data field, BtLibSocketRef newSocket;
	        // use BtLibSocketSend to send data
	        break;
	        
	    case btLibSocketEventConnectedOutbound:
	        if(mEventP->status == btLibErrNoError) {
	           LogPrintF("-> RFComm outbound connection");
	           error = SendData();
	        } else {
	           LogPrintF("-> RFComm outbound connection failed");
	        }
	        break;
	        
	    case  btLibSocketEventDisconnected:
	        LogPrintF("-> RFComm connection disconnected");
	        socketRefP = kInvalidSocket;
	        break;
	        
	    case btLibSocketEventSdpServiceRecordHandle:
	        if(mEventP->status == btLibErrNoError) {
	        	LogPrintF("-> Request for service record completed");
	        } else {
	        	LogPrintF("-> Service record request failed");
	        }
	        break;
	      
	    case btLibSocketEventSdpGetAttribute:
	    	if(mEventP->status == btLibErrNoError) {
	        	LogPrintF("-> Request for attribute completed");
	        } else {
	        	LogPrintF("-> Attribute request failed");
	        }
	        break;  
	        
	    case btLibSocketEventSdpGetStringLen:
	        LogPrintF("-> btLibSocketEventSdpGetStringLen");
	        break;
	        
	    case btLibSocketEventSdpGetNumListEntries:
	    	LogPrintF("-> btLibSocketEventSdpGetNumListEntries");
	        break;
	        
	    case btLibSocketEventSdpGetNumLists:
	    	LogPrintF("-> btLibSocketEventSdpGetNumLists");
	        break;
	        	    
	    case btLibSocketEventSdpGetRawAttribute:
	    	LogPrintF("-> btLibSocketEventSdpGetRawAttribute");
	        break;
	        	
	    case btLibSocketEventSdpGetRawAttributeSize:
	    	LogPrintF("-> btLibSocketEventSdpGetRawAttributeSize");
	        break;
	        	        
	    case btLibSocketEventSdpGetServerChannelByUuid:
	        if(mEventP->status == btLibErrNoError) {
	    	   LogPrintF("-> Server channel request done");
	    	   // Create and connect to RFComm socket
	    	   BtUtilCreateRfCommSocket();
	    	   BtUtilConnectToRfCommSocket(mEventP->eventData.sdpByUuid.param.channel);
	        } else {
	           LogPrintF("-> Server channel request failed");
	        }
	        break;

	    case btLibSocketEventSdpGetPsmByUuid:
	    	LogPrintF("-> btLibSocketEventSdpGetPsmByUuid");
	        break;
	        	        	        	    	        	        	        	        	        	       
		default:
		    LogPrintF("-> Default case for BtLibSocketCallbackProc");
		    break;
	}
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilSendData
 *
 * DESCRIPTION: Send data over Bluetooth
 *
 * PARAMETERS:  
 *
 * RETURNED:    error
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void BtUtilSendData()
{	
	Err error;
	 
	if(!BtUtilLibIsOpen()) {
	   LogPrintF("-> Error - Bluetooth library is not open");
	   return;
	}

	if(!BtUtilIsConnected()) {
	   LogPrintF("-> Error - not connected to device");
	   return;
	}
	
	// Create sdp socket
	error = BtLibSocketCreate(gBtLibRefNum, &sdpSocket, BtLibSocketCallbackProc, 0, btLibSdpProtocol);
	if(error == btLibErrNoError) {
		LogPrintF("-> Created sdp socket");
	} else {
		LogPrintF("-> Error creating sdp socket");
		return;
	}
	
	// Call generates a btLibSocketEventSdpGetServerChannelByUuid event. When received, create and connect rfcomm port
	error = BtLibSdpGetServerChannelByUuid(gBtLibRefNum, sdpSocket, &gBtLibDeviceConnectedTo, (BtLibSdpUuidType *)&kRfCommUUID, 1);
	if(error == btLibErrNoError) {
		LogPrintF("-> Got server channel by uuid");
	} else if(error == btLibErrPending) {
		LogPrintF("-> Getting server channel..");
	} else if(error == btLibErrNotOpen) {
		LogPrintF("-> Error getting server channel - bt lib not open");
	} else if(error == btLibErrOutOfMemory) {
		LogPrintF("-> Error getting server channel - out of memory");
	} else if(error == btLibErrParamError) {
		LogPrintF("-> Error getting server channel - params invalid");
	} else if(error == btLibErrSocket) {
		LogPrintF("-> Error getting server channel - invalid socket");
	} else if(error == btLibErrSocketRole) {
		LogPrintF("-> Error getting server channel - socket not connected");
	} else if(error == btLibErrStackNotOpen) {
		LogPrintF("-> Error getting server channel - bt stack not initialized");
	} else { 
		LogPrintF("-> Error getting server channel");
	}
} 


/***********************************************************************
 *
 * FUNCTION:    BtUtilConnectToRfCommSocket
 *
 * DESCRIPTION: Connect to RFComm socket
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void BtUtilConnectToRfCommSocket(BtLibRfCommServerIdType serverChannel) 
{
	Err error;
	BtLibSocketConnectInfoType connectInfo;
	
	connectInfo.remoteDeviceP = &gBtLibDeviceConnectedTo;
	connectInfo.data.RfComm.remoteService = serverChannel;
	connectInfo.data.RfComm.maxFrameSize = BT_RF_MAXFRAMESIZE;
	connectInfo.data.RfComm.advancedCredit = (UInt8)BT_RF_MAXFRAMESIZE;
	    
	error = BtLibSocketConnect(gBtLibRefNum, socketRefP, &connectInfo); 
	if(error == btLibErrPending) {
		   LogPrintF("-> Socket connecting...");
		} else if(error == btLibErrNoAclLink) {
		   LogPrintF("-> Error connecting to socket - no ACL link");
		} else if(error == btLibErrNotOpen) {
		   LogPrintF("-> Error connecting to socket - bt lib not open");
		} else if(error == btLibErrSocket) {
		   LogPrintF("-> Error connecting to socket - socket invalid");
		} else if(error == btLibErrSocketProtocol) {
		   LogPrintF("-> Error connecting to socket - socket protocol unsupported");	
		} else if(error == btLibErrSocketRole) {
		   LogPrintF("-> Error connecting to socket - socket already connected/listening");
		} else if(error == btLibErrStackNotOpen) {
		   LogPrintF("-> Error connecting to socket - bt stack failed to initialize");	
		} else if(error == btLibErrParamError) {
		   LogPrintF("-> Error connecting to socket - parameter incorrect");
		} else {
	  	  LogPrintF("-> Error while connecting to socket");
		}
}


/***********************************************************************
 *
 * FUNCTION:    SendData
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err SendData()
{
	Char *dataPtr;
	Char sendStr[] = "   -> This is the sent string <--";
	Err error = 0;
	
	if(gDataHandle == NULL)
		gDataHandle = MemHandleNew(1024);
	
	if (socketRefP == kInvalidSocket) {
		LogPrintF("-> Error sending data - no socket");
		error = btLibErrSocketChannelUnavailable;
		return error;
	}
	if (gDataHandle == NULL) {
	    LogPrintF("-> Error sending data - data handle is null");
		error = btLibErrOutOfMemory;
		return error;
	}
	// Get a pointer to the buffer.  Important - this is kept locked until 
	// the btLibSocketEventSendComplete callback is received!
	dataPtr = MemHandleLock(gDataHandle);
	// Send the data 
	MemMove(dataPtr, sendStr, sizeof(sendStr));
	
	error = BtUtilAdvanceCredit();
	if(error == btLibErrNoError) {
        error = BtLibSocketSend(gBtLibRefNum, socketRefP, (UInt8*) dataPtr, sizeof(sendStr));
       	if(error == btLibErrPending) {
		    LogPrintF("-> Sending data...");	
		} else if(error == btLibErrBusy) {
			LogPrintF("-> Error sending data - send already in progress");
		} else if(error == btLibErrNoAclLink) {
			LogPrintF("-> Error sending data - no ACL link");
       	} else if(error == btLibErrSocket) {
			LogPrintF("-> Error sending data - socket invalid");		
		} else if(error == btLibErrSocketProtocol) { 
			LogPrintF("-> Error sending data - socket protocol not supported");
		} else if(error == btLibErrSocketRole) {
			LogPrintF("-> Error sending data - socket not connected");		
		} else if(error == btLibErrStackNotOpen) {
			LogPrintF("-> Error sending data - bt stack uninitialized");		
		} else {
			LogPrintF("-> Error sending data");
       	}
	} 
	return (error);
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilAdvanceCredit
 *
 * DESCRIPTION: Advance credit to RFCOMM connection socket
 *
 * PARAMETERS:  
 *
 * RETURNED:    error
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err BtUtilAdvanceCredit() 
{
	Err error = BtLibSocketAdvanceCredit(gBtLibRefNum, socketRefP, 1);
	if(error == btLibErrNoError) {
		LogPrintF("-> Advanced credit");
	} else if(error == btLibErrFailed) {
		LogPrintF("-> Error advancing credit - too many credits advanced");
	} else if(error == btLibErrSocket) {
		LogPrintF("-> Error advancing credit - socket invalid");
	} else if(error == btLibErrSocketProtocol) { 
		LogPrintF("-> Error advancing credit - socket not RFCOMM");
	} else if(error == btLibErrSocketRole) {
		LogPrintF("-> Error advancing credit - socket not connected");		
	} else {
	  	LogPrintF("-> Error advancing credit");
	}
    return(error);
}


/***********************************************************************
 *
 * FUNCTION:    BtUtilGetRfCommSocketListenInfo
 *
 * DESCRIPTION: Set socket listen info
 *
 * PARAMETERS:  
 *
 * RETURNED:    listenInfo
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static BtLibSocketListenInfoType BtUtilGetRfCommSocketListenInfo()
{
	BtLibSocketListenInfoType listenInfo;
	listenInfo.data.RfComm.maxFrameSize = BT_RF_MAXFRAMESIZE;
	listenInfo.data.RfComm.advancedCredit = (UInt8)BT_RF_MAXFRAMESIZE;
	
	return(listenInfo);
}



