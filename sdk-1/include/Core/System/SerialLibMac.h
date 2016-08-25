/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SerialLibMac.h
 *
 * Description:
 *		Include file for Mac Serial Library
 *
 * History:
 *   	6/30/95 Created by Vitaly Kruglikov
 *
 *******************************************************************/

#ifndef __SERIAL_LIB_MAC_H
#define __SERIAL_LIB_MAC_H


// Pilot common definitions
#include "Common.h"
#include "SerialMgr.h"


/********************************************************************
 * Serial Globals
 ********************************************************************/
typedef struct SerMacGlobalsType {
	UInt					refNum;				// port reference number
	UInt					openCount;			// number of times we have been opened
	UInt					port;
	SerSettingsType	settings;
	Word					lineErrors;			// line errors bitfield
	
	short					inRefNum;			// input driver reference number
	short					outRefNum;			// output driver reference number
	Ptr					inBufP;				// custom input buffer ptr
	} SerMacGlobalsType;
typedef SerMacGlobalsType*	SerMacGlobalsPtr;	

#define	serDefaultRcvBufferSize	(30*1024)


#endif	//__SERIAL_LIB_MAC_H
