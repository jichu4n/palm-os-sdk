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
 *  @version 1.0	- Initial Revision (07/31/07)
 *
 *  @file Pushdemo.h
 */



#ifndef PUSHDEMO_H_
#define PUSHDEMO_H_

/*********************************************************************
 * Internal Structures
 *********************************************************************/


/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'STRu'
#define appName					"Pushdemo"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01


/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)


/* Function prototypes */

Err SocketOpen(FieldPtr fieldStatusP);
int TCPMain(void);
Int16 ReceiveDataPacket(FieldPtr fieldStatusP);
static Err GetAddr(UInt16 gNetLibRefNum, NetIPAddr *addrP, UInt16 *portP);
static Boolean GetValue(UInt16 fieldID, Int32 *valueP);
Err SocketConnect(NetSocketAddrINType *destAddr, FieldPtr fieldStatusP);


#endif /* PUSHDEMO_H_ */
