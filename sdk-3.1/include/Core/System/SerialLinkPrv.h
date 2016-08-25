/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		SerialLinkPrv.c
 *
 * Description:
 *		Private Serial Link includes
 *
 * History:
 *   	2/6/95 replaces DSerial.h from Debugger 
 *
 *******************************************************************/

#ifdef	NON_PORTABLE

#ifndef __SERIALLINKPRV_H
#define __SERIALLINKPRV_H

#include <SerialLinkMgr.h>


//*************************************************************************
//  Misc Constants
//*************************************************************************
// Default timeout  value once into receiving a packet in ticks
#define		slkDefaultInPktTimeout		(sysTicksPerSecond*2)




/*******************************************************************
 * Serial Link Globals
 *
 *******************************************************************/
#define	slkMaxSockets		5						// max # of sockets
#define	slkMaxRefNums		slkMaxSockets		// max # of refNums in use at any one time
#define	slkAnySocket		0xFF		

#define	slkEmptyRefNum		0xFF					// used to track empty refInfos

// We keep an array of these (slkMaxSockets in size) to track which
//  librarie's we've received packet headers for. Since we can have up
//  to slkMaxSockets unique refNums in use at any one time, we must make
//  the array at least that size.
typedef struct SlkRefInfoType {
	Byte		refNum;									// refNum of library, or 0xFF if empty
	SByte		destID;									// dest socketID, or -1 if none
	} SlkRefInfoType;
typedef SlkRefInfoType* SlkRefInfoPtr;	

typedef struct SlkGlobalsType {
	UInt						openCount;
	
	Byte						socketRefNum[slkMaxSockets];
	Boolean					socketOpen[slkMaxSockets];
	SlkSocketListenPtr	socketListenP[slkMaxSockets];
	Word						socketTimeout[slkMaxSockets];		// inPacket timeout
	
	
	// We can keep track of packets coming in for up to slkMaxRefNums libraries.
	//  This array holds the destination socket ID for packets received from
	//  each library.
	SlkRefInfoType			refInfo[slkMaxRefNums];
	
	Byte						transId;					// next transaction ID
	DWord						xmitSemaphoreID;		// Semaphore ID for transmitting
	Word						numReceived;			// # of packets received since serial link
															// was opened.
	} SlkGlobalsType;

typedef SlkGlobalsType*	SlkGlobalsPtr;


	
#endif	//__SERIALLINKPRV_H

#endif 	//NON_PORTABLE
