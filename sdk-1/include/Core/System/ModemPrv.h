/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		ModemPrv.h
 *
 * Description:
 *		Private Include file for Modem Manager
 *
 * History:
 *   	9/25/95  VMK - Created by Vitaly Kruglikov
 *
 *******************************************************************/


#ifndef __MODEM_PRV_H__
#define __MODEM_PRV_H__

// Include elementary types
#include <Common.h>



/************************************************************
 * Modem Manager constants
 *************************************************************/


/************************************************************
 * Modem Manager data structures
 *************************************************************/

typedef struct MdmCommandsType {
	Char		csi[mdmCmdSize];			// command sequence introducer
	Char		eoc[mdmCmdSize];			// end-of-command character(s)
	Char		quietCmd[mdmCmdSize];	// Q cmd: send result codes
	Char		dialTone[mdmCmdSize];	// tone dial command
	Char		dialPulse[mdmCmdSize];	// pulse dial command
	Char		cmdEchoOff[mdmCmdSize];	// E cmd: echoes in command mode
	Char		verboseCmd[mdmCmdSize];	// V cmd: word or digit result codes
	Char		dtWait[mdmCmdSize];		// (S6) dialtone wait (sec)
	Char		dcdWait[mdmCmdSize];		// (S7) dcd timeout wait (sec)
	Char		basicxcmd[mdmCmdSize];	// sets result code level for "basic" smartmodem
	Char		advxcmd[mdmCmdSize];		// sets result code level for advanced clones
	Char		spkrMode[mdmCmdSize];	// speaker mode
	Char		volume[mdmCmdSize];		// speaker volume prefix
	Char		hupCmd[mdmCmdSize];		// hang up command
	Char		firmDTRDCD[mdmCmdSize];	// DTR on, DCD on
	Char		escape[mdmCmdSize];		// Escape sequence: +++
	Char		reset[mdmCmdSize];		// Z cmd: reset the modem
	Char		factory[mdmCmdSize];		// recall factory config
	} MdmCommandsType;


typedef enum MdmRespIDType {
	mdmRespUnknown = 0,
	mdmRespOK,
	mdmRespConnect,
	mdmRespNoCarrier,
	mdmRespError,
	mdmRespNoDialTone,
	mdmRespBusy,
	mdmRespRing,
	
	mdmRespReserved = -1					// reserve -1 for internal use
	} MdmRespIDType;

typedef struct MdmRespEntryType {
	Char				subStr[20];			// response substring
	MdmRespIDType	id;					// id associated with substring
	} MdmRespEntryType;


/************************************************************
 * Modem Manager result codes
 * (mdmErrorClass is defined in SystemMgr.h)
 *************************************************************/
//#define	mdmErrBadParam			(mdmErrorClass | 1)



/************************************************************
 * Modem Manager Macros
 *************************************************************/

#endif  // __MODEM_PRV_H__

