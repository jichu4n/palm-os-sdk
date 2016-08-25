/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
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
#define	mdmDefaultResponseWait		300000L		// in microseconds

/************************************************************
 * Modem Manager data structures
 *************************************************************/
typedef enum  {
	mdmCmdCSI,
	mdmCmdEOC,
	mdmCmdQuiet,
	mdmCmdDialTone,
	mdmCmdDialPulse,
	mdmCmdEchoOff,
	mdmCmdVerbose,
	mdmCmdDTWait,
	mdmCmdDCDWait,
	mdmCmdBasicXCmd,
	mdmCmdAdvXCmd,
	mdmCmdSpkrOnTillConnect,
	mdmCmdSpkrOff,
	mdmCmdVolume,
	mdmCmdHUP,
	mdmCmdFirmDTRDCD,
	mdmCmdEscape,
	mdmCmdReset,
	mdmCmdFactory
	} MdmCmdEnum;

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


// Configuration constants:

// Hardware handshake settings
enum {
	mdmHandshakeStd		= 0,			// standard AT handshaking (on at > 2400 baud)
	mdmHandshakeOff		= 1,			// force handshaking off
	mdmHandshakeOn			= 2			// force handshaking on
	};


//
// Modem Manager preference structure (system pref rsc id = sysResIDModemMgrPref):
//
#define mdmPrefVersionNum		2					// modem preference version

typedef struct MdmPrefType {
	Word			version;
	ULong			baud;										// baud rate
	Byte			volume;									// speaker volume level
	Boolean		pulse;									// set to true for pulse dial mode
	Byte			hShk;										// handshake option
	Boolean		autoBaud;								// set to true for auto-baud
	Char			resetString[(mdmCmdSize*2)+1];	// string to be embedded in the reset command (vmk added 12/17/96)
	Char			initString[mdmCmdBufSize];			// modem init string
	} MdmPrefType;
typedef MdmPrefType*		MdmPrefPtr;

#define mdmDefaultDialMode		false


/************************************************************
 * Modem Manager default settings. Used by PrvGetModemPrefs() 
 *  in ModemMgr.c when invalid or no prefs found.
 *
 * WARNING: In order to avoid confusion for the user, these
 *  defaults should match what the Modem Panel uses as it's defaults.
 *  The modem panels defaults are set by editing it's resources so
 *  they must be manually updated when these equates are changed.
 *************************************************************/
#define	mdmDefaultBaud				19200
#define	mdmDefaultVolume			mdmVolumeLow
#define	mdmDefaultPulse			false
#define 	mdmDefaultHShk				mdmHandshakeStd
#define 	mdmDefaultAutoBaud		false
#define	mdmDefaultResetString	""
#define 	mdmDefaultInitString		"&F&B1X4"


/************************************************************
 * Modem Manager Macros
 *************************************************************/

#endif  // __MODEM_PRV_H__

