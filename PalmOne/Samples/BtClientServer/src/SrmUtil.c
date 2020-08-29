 /****************************************************************************
 * Copyright (c) 2007 Palm, Inc. All rights reserved.
 *
 * File: SrmUtil.c
 *
 *****************************************************************************/
#include <SerialMgr.h>
#include "SrmUtil.h"
#include "LogUtil.h"
#include <BtLib.h>
#include <BtLibTypes.h>
#include <BtCommVdrv.h>
#include <PalmOS.h>
#include <VfsMgr.h>

UInt16    gbtPortId     = 0xffff;

#define MAXPAYLOADSIZE 0x800

#define PAYLOAD_IS_COMPLETE 0x1
#define MAXFILENAME 256
#define MAXSTRLEN   100
#define ERROR 1

typedef enum { BINARY, TEXT, ACK } payloadspecifier;
typedef struct _payload
{
    payloadspecifier       specifier;	
    UInt32                 payloadnum;
    UInt32                 payloadsize;  
    UInt32                 flags;
    UInt32                 filelen; // filename length
    char                   filename[MAXFILENAME];
    unsigned char          payload[MAXPAYLOADSIZE];       
} payload, *ppayload; 

#define PACKETSIZE sizeof(payload) 
#define PACKETHEADER (sizeof(payloadspecifier) + (3 * sizeof(uint32_t)))
#define PACKETHEADERFILELEN (sizeof(payloadspecifier) + (4 * sizeof(uint32_t)))

#define MAXSTRINGLEN 32
unsigned char *bufBaseP;
MemHandle bufH; 
Boolean portOpen = 0;

//Making global to avoid coverity errors
payload pBuf, data;

/***********************************************************************
 *
 * FUNCTION:    printError
 *
 * DESCRIPTION: Print error message 
 *
 * PARAMETERS:  err
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void printError(Err err)
{ 	
	  if(err == expErrEnumerationEmpty) {
	  	 LogPrintF("->Error - no storage volume found");  
	  } else if(err == sysErrParamErr) {
	  	 LogPrintF("->Error - volIteratorP invalid or vfsIteratorStop");
	  } else if(err == expErrCardReadOnly) {
	  	 LogPrintF("->Error - can't grant write access");
	  } else if(err == expErrNotOpen) {
	  	 LogPrintF("->Error - file system lib not installed or opened");
	  } else if(err == vfsErrBadName) {
	  	 LogPrintF("->Error - pathname invalid");
	  } else if(err == vfsErrFileNotFound) {
	  	 LogPrintF("->Error - file or dir not found");
	  } else if(err == vfsErrFilePermissionDenied) {
	  	 LogPrintF("->Error - can't open file in requested mode or file already opened exclusively");
	  } else if(err == vfsErrVolumeBadRef) {
	  	 LogPrintF("->Error - volume not mounted");
	  } else if(err == vfsErrFileBadRef) {
	  	 LogPrintF("->Error - file ref invalid");
	  } else if(err == vfsErrIsADirectory) {
	  	 LogPrintF("->Error - file ref is a dir");
	  } else if(err == vfsErrNoFileSystem) {
	  	 LogPrintF("->Error - no file system to handle request");
	  } else if(err == vfsErrVolumeFull) {
	  	 LogPrintF("->Error - volume is full");
	  }          
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilWriteToFile
 *
 * DESCRIPTION: Write payload to file
 *
 * PARAMETERS:  filename    - file to create
 *              payloadSize - size of payload
 *              payload     - payload data
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void SrmUtilWriteToFile(char *filename, UInt32 payloadSize, char *payload)
{ 
    Err err;
    FileRef fileRef;
    UInt32  bytesWritten; 
    UInt16 volRefNum; 
    UInt32 volIterator = vfsIteratorStart;
    	     
    LogPrintF("Writing payload to file");   
	while(volIterator != vfsIteratorStop) { 
	   err = VFSVolumeEnumerate(&volRefNum, &volIterator);  
	   printError(err);
       if(err == errNone) {       	
		  err = VFSFileOpen(volRefNum, filename, vfsModeReadWrite|vfsModeCreate, &fileRef);
		  printError(err);
		  if(err == errNone) {
	         err = VFSFileWrite(fileRef, payloadSize, payload, &bytesWritten);
	         printError(err);
	         if(err == errNone) {
	            err = VFSFileClose(fileRef);
	            printError(err);
	         } 
		  }
       } 
	}
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilSendData
 *
 * DESCRIPTION: Client sends data to server over Bluetooth connection  
 *
 * PARAMETERS:  filePath - file to send
 *              fileSize - size of file
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Err SrmUtilSendData(char *filePath, UInt32 fileSize)
{
	Err err;
	Char logStr[MAXSTRINGLEN];
	Int16 bytesSent;
	//payload data;
	
	// Set up payload struct
	data.specifier = BINARY;
	data.payloadnum = 1;
	data.flags = 1;
	data.filelen = sizeof(filePath);
	StrCopy(data.filename, filePath);
	MemMove (data.payload, bufBaseP, fileSize);
	data.payloadsize = fileSize;
	
	// Print log messages
    LogPrintF(data.filename);
    LogPrintF("Payload size:");
    StrIToA(logStr, data.payloadsize);
    LogPrintF(logStr);
        
    // Send data
	bytesSent = SrmSend( gbtPortId, &data, sizeof(data), &err);
	
    // Print log messages
    StrIToA(logStr, bytesSent);
    LogPrintF("Bytes sent:");
    LogPrintF(logStr);

    return err;
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilSetupClient
 *
 * DESCRIPTION: Set up as a client  
 *
 * PARAMETERS: 
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void SrmUtilSetupClient(void)
{
    SrmOpenConfigType   config;
    BtVdOpenParams      btParams;
    BtLibSdpUuidType    sppUuid;
    Err					err;

    // Set up a SerialPort service class UUID
    LogPrintF("Setting up client");
    MemSet(&sppUuid, sizeof(sppUuid), 0);
    
    sppUuid.size     = btLibUuidSize16;
    sppUuid.UUID[ 1] = 0x11; // Along with btParams.role set to btVdClient below, 
    sppUuid.UUID[ 0] = 0x01; // this determines what port is opened
 
    // Set up a bluetooth parameter. Remote device address is 0 
    // Use sdp to search for serial port profile service class uuid
    MemSet( &btParams, sizeof(btParams), 0 );     // This sets remote device address to null    

    btParams.role                    = btVdClient;         // Set up as a client
    btParams.u.client.method         = btVdUseUuidList;   // Use SDP to look for service class uuids
    btParams.u.client.u.uuidList.tab = &sppUuid;         // List of service uuids to search for
    btParams.u.client.u.uuidList.len = 1;               // Number of uuids in the list

    // Set up serial manager configuration
    MemSet(&config, sizeof(config), 0);

    config.function = 0;                       // Must be 0
    config.drvrDataP = (MemPtr)&btParams;      // Bluetooth-specific params  
    config.drvrDataSize = sizeof(btParams);    // Size of Bluetooth params

    // Open serial port
	err = SrmExtOpen( sysFileCVirtRfComm, // Type of port is bluetooth RfComm
                      &config,            // Port config params
                      sizeof(config),     // Size of port config params
                      &gbtPortId);        // Receives the id of this virtual port instance

	if (err == errNone) {
    	LogPrintF("Opened serial port for client"); 
    	portOpen = true;         
	} else if (err == serErrBadPort) {
        LogPrintF("Port open failed - invalid port"); 
    } else if (err == serErrBadParam) {
        LogPrintF("Port open failed - configP parameter is NULL");
    } else if(err == serErrAlreadyOpen) {
    	LogPrintF("Port open failed - port already opened by SM");
    } else if(err == memErrNotEnoughSpace) {
   	    LogPrintF("Port open failed - not enough memory");
    } else if(err == serErrNoMem) {
   	    LogPrintF("Port open failed - serErrNoMem"); 
    } else if(err == serErrBadConnID) {
   	    LogPrintF("Port open failed - serErrBadConnID"); 
    } else if(err == serErrAlreadyOpen) {
   	    LogPrintF("Port open failed - port already open"); 
    } else if(err == serErrNotSupported) {
   	    LogPrintF("Port open failed - port not supported");   	
    } else if(err == serErrConfigurationFailed) {
   	    LogPrintF("Port open failed - port config failed");   
   	} else if(err == serErrNoDevicesAvail) {
   	    LogPrintF("Port open failed - no serial devices found"); 	   	        
    } else {
    	LogPrintF("Port open failed - undefined error");
    }
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilGetFileContent
 *
 * DESCRIPTION: Read file content  
 *
 * PARAMETERS: 	filePath 
 *				fileSize
 * 
 * RETURNED:    Err
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Err SrmUtilGetFileContent(char *filePath, UInt32 *fileSize)
{
   Err err = errNone;
   FileRef fileRef;
   UInt16 volRefNum; 
   UInt32 volIterator = vfsIteratorStart; 
   UInt32  bytesRead;
   
   while (volIterator != vfsIteratorStop) { 
   	  err = VFSVolumeEnumerate(&volRefNum, &volIterator);
      if(err != errNone) {
      	 LogPrintF("VFS volume enumerate failed");
      	 return err;
      }
      
      err= VFSFileOpen (volRefNum, filePath, vfsModeRead, &fileRef); // Open file with read permissions
      if(err != errNone) {
      	 LogPrintF("VFS file open failed");
      	 return err;
      }
      
      err = VFSFileSize (fileRef, fileSize); // Get file size
      if(err != errNone || *fileSize > MAXPAYLOADSIZE) {
         LogPrintF("Error - file is larger than 2K limit");
         return ERROR;	
      }
      
	  bufH = MemHandleNew (*fileSize);      // Allocate memory for storing file content
	  if(!bufH) { 
	  	 LogPrintF("Error allocating memory");
	  	 return ERROR;
	  }
	  bufBaseP = MemHandleLock (bufH);
	  err = VFSFileRead (fileRef, *fileSize, bufBaseP, &bytesRead);  // Read file
	  if(err != errNone) {
	  	 LogPrintF("Error while reading file content");
	 	 return err;
	  }
		            
      VFSFileClose(fileRef); // Close file
	  if(err != errNone) {
		 LogPrintF("VFS file close failed");
		 return err;
	  }
   }
   return err;
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilClientSend
 *
 * DESCRIPTION: Client sends data   
 *
 * PARAMETERS:  filePath - file to send
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void SrmUtilClientSend(char *filePath)
{
    Err     err;
    UInt32  fileSize = 0;

    if(filePath == NULL) {
   	   LogPrintF("File path error");
   	   return;
    }

    err = SrmUtilGetFileContent(filePath, &fileSize); // Read file content
    if(err != errNone) {
    	if(bufH) {
           MemHandleFree(bufH);              
        }
    	return;
    }
    
    LogPrintF("Client sending request");
    err = SrmUtilSendData(filePath, fileSize); // Send the data
    
    if (err == errNone) {
    	LogPrintF("Data successfully sent");
    	err = SrmSendFlush( gbtPortId);         
	    if (err == errNone) {
	    	LogPrintF("Data flush successful");
	    } else if (err == serErrBadPort) {
	        LogPrintF("SrmSendFlush failed - port doesn't exist"); 
	    } else if (err == serErrNotOpen) {
	        LogPrintF("SrmSendFlush failed - port is not open");
	    } else if(err == serErrNotSupported) {
	    	LogPrintF("SrmSendFlush failed - port not foreground port");
	    } else if(err == serErrNoDevicesAvail) {
	   	    LogPrintF("SrmSendFlush failed - no serial devices found");
	    	return;
	    } else {
	    	LogPrintF("SrmSendFlush failed - undefined error");
	    }
    } else if (err == serErrBadPort) {
        LogPrintF("Data send failed - port doesn't exist"); 
    } else if (err == serErrNotOpen) {
        LogPrintF("Data send failed - port is not open");
    } else if(err == serErrTimeOut) {
    	LogPrintF("Data send failed - timeout");
    } else if(err == serErrNoDevicesAvail) {
   	    LogPrintF("Data send failed - no serial devices found");
    } else if(err == serErrConfigurationFailed) {
   	    LogPrintF("Data send failed - port config failed");
    } else if(err == serErrNotSupported) {
   	    LogPrintF("Data send failed - port not foreground port"); 	
    } else {
    	LogPrintF("Data send failed - undefined error");
    }

    if(bufH) {
       MemHandleFree(bufH);     
       bufH = NULL;         
    }
    
    SrmUtilUninit(); // Close port
} // SrmUtilClientSend


/***********************************************************************
 *
 * FUNCTION:    SrmUtilReceive
 *
 * DESCRIPTION: Server receives data 
 *
 * PARAMETERS:  portId
 *              timeout
 *              pErr
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static void SrmUtilReceive(Int32 timeout, Err *pErr)
{
	char intStr[MAXSTRLEN];
    UInt32 readLen = 0;
	Err err;
  //  payload pBuf;
    UInt32  rcvBufSize = sizeof(payload);
    UInt8 *pBuffer = (UInt8 *)&pBuf;
    payload *data;
    unsigned char *receiveBuf;
    MemHandle receiveBufH;
    char *intStrPtr = intStr;

    receiveBufH = MemHandleNew (rcvBufSize); 
    if(!receiveBufH) {
       LogPrintF("Error when allocating receive buffer(s)");  
       return; 	
    } else {
       receiveBuf = MemHandleLock (receiveBufH);
       MemSet(receiveBuf, rcvBufSize, 0);
    }
   
    err = SrmSetReceiveBuffer(gbtPortId, receiveBuf, rcvBufSize); // Set receive buffer
    if(err == errNone) {
   	  LogPrintF("Allocated receivebuffer of size:");
      StrIToA(intStrPtr, rcvBufSize);
      LogPrintF(intStrPtr);
      readLen = SrmReceive(gbtPortId, pBuffer, sizeof(pBuf), SysTicksPerSecond() * 10, pErr); // Receive data
        
      if (*pErr == errNone) {
    	LogPrintF("Data receive successful:");
    	data = (payload *)pBuffer;   	 	
    	LogPrintF(data->filename);
    	LogPrintF("Bytes received:");
    	StrIToA(intStrPtr, readLen);
    	LogPrintF(intStrPtr);
    	
    	SrmUtilWriteToFile((char *)data->filename, data->payloadsize, (char *)data->payload); // Write data to file

      } else if (*pErr == serErrBadPort) {
        LogPrintF("Data receive failed - port doesn't exist"); 
      } else if (*pErr == serErrNotOpen) {
        LogPrintF("Data receive failed - port is not open");
      } else if (*pErr == serErrTimeOut) {
        LogPrintF("Data receive failed - timeout");
      } else if (*pErr == serErrConfigurationFailed) {
        LogPrintF("Data receive failed - port config failed");       
      } else if(*pErr == serErrNotSupported) {
    	LogPrintF("Data receive failed - port not foreground port");
      } else if(*pErr == serErrNoDevicesAvail) {
   	    LogPrintF("Data receive failed - no serial devices found");
      } else if(*pErr == serErrLineErr) {
   	    LogPrintF("Data receive failed - line error");    	
      } else {
    	LogPrintF("Data receive failed - undefined error");
      }
      
   } else if(err == serErrBadPort) {
      LogPrintF("Error allocating receive buffer - port doesn't exist");   	
   } else if(err == serErrNotOpen) {
      LogPrintF("Error allocating receive buffer - port not open");   	
   } else if(err == memErrNotEnoughSpace) {
      LogPrintF("Error allocating receive buffer - not enough memory for default buffer");      	 
   } else if(err == serErrNoDevicesAvail) {
      LogPrintF("Error allocating receive buffer - no serial devices found");  	          	
   }

   SrmSetReceiveBuffer(gbtPortId, NULL, NULL); // Free receive buffer memory
   SrmUtilUninit(); // Close port
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilServerReceive
 *
 * DESCRIPTION: Server receives data 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void SrmUtilServerReceive(void)
{	
	Err err;
    LogPrintF("Server waiting for connection");
    SrmUtilReceive(SysTicksPerSecond() * 3600, &err);
    
} // SrmUtilServerReceive


/***********************************************************************
 *
 * FUNCTION:    SrmUtilSetupServer
 *
 * DESCRIPTION: Setup server 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void SrmUtilSetupServer( void )
{
    SrmOpenConfigType   config;
    BtVdOpenParams      btParams;
    Err					err;
    
    LogPrintF("Setting up server");
    MemSet( &btParams, sizeof( btParams), 0 );      // This leaves the remote dev addr 0    

    btParams.role = btVdServer;                     // Setting up server
    btParams.u.server.name          = "stgBTServer"; 

    btParams.u.server.uuid.size     = btLibUuidSize16; 
    btParams.u.server.uuid.UUID[ 1] = 0x11; // Along with btParams.role = btVdServer, 
    btParams.u.server.uuid.UUID[ 0] = 0x01; // this determines the port number
 
    MemSet( &config, sizeof( config), 0 );
    config.function = 0;                          // must be zero
    config.drvrDataP = (MemPtr)&btParams;       // Bluetooth specific parameters 
    config.drvrDataSize = sizeof(btParams);    

    // Open port
    err = SrmExtOpen(sysFileCVirtRfComm,  // Type of port is bluetooth RfComm
                      &config,            // Port configuration parameters
                      sizeof(config),     // Size of port config parameters
                      &gbtPortId);        // Receives id of this virtual port instance
               
	if (err == errNone) {
    	LogPrintF("Opened serial port for server");  
    	portOpen = true;            
	} else if (err == serErrBadPort) {
        LogPrintF("Port open failed - invalid port"); 
    } else if (err == serErrBadParam) {
        LogPrintF("Port open failed - configP parameter is NULL");
    } else if(err == serErrAlreadyOpen) {
    	LogPrintF("Port open failed - port already opened by SM");
    } else if(err == memErrNotEnoughSpace) {
   	    LogPrintF("Port open failed - not enough memory");
    } else {
    	LogPrintF("Port open failed - undefined error");
    }
}


/***********************************************************************
 *
 * FUNCTION:    SrmUtilUninit
 *
 * DESCRIPTION: Close serial port if open 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
Err SrmUtilUninit() {
	
    Err err = errNone;

    if(portOpen) {
       LogPrintF("Closing serial port.....");  
       err = SrmClose(gbtPortId);
       portOpen = false;
       
		if (err == errNone) {
	    	LogPrintF("Port closed");               
		} else if (err == serErrBadPort) {
	        LogPrintF("Port close failed - invalid port"); 
	    } else if (err == serErrNotOpen) {
	        LogPrintF("Port close failed - serial port not open");
	    } else if(err == serErrNoDevicesAvail) {
	    	LogPrintF("Port close failed - no serial devices found");
	    } else {
	    	LogPrintF("Port close failed - undefined error");
	    }
    }

    return err;
}

