/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		ModemMgr.h
 *
 * Description:
 *		Include file for Modem Manager
 *
 * History:
 *   	9/20/95  VMK - Created by Vitaly Kruglikov
 *
 *******************************************************************/


#ifndef __MODEM_MGR_H__
#define __MODEM_MGR_H__


// Include elementary types
#include <Common.h>
#include <SysTraps.h>				// Trap Numbers.



/************************************************************
 * Modem Manager constants
 *************************************************************/

#define mdmMaxStringSize	40

#define mdmCmdBufSize		81		// command buffer capacity (including null)
#define mdmRespBufSize		81		// reply buffer capacity (including null)
#define mdmCmdSize			8		// max storage needed for smartmodem command

#define mdmDefCmdTimeOut			500000L		// in micro-seconds

#define mdmDefDTWaitSec				4
#define mdmDefDCDWaitSec			70
#define mdmDefSpeakerVolume		1


// Speaker volume settings
enum {
	mdmVolumeOff = 0,
	mdmVolumeLow = 1,
	mdmVolumeMed = 2,
	mdmVolumeHigh = 3
	};


// Modem connection stages (NEW for Pilot 2.0)
typedef enum {
	mdmStageInvalid = 0,					// invalid state
	mdmStageReserved = 1,				// reserved for 1.0 compatibility
	mdmStageFindingModem,				// checking if modem is present
	mdmStageInitializing,				// initializing the modem
	mdmStageDialing,						// dialing the modem
	mdmStageWaitingForCarrier,			// waiting for carrier detect
	mdmStageHangingUp						// hanging up the modem
	} MdmStageEnum;

/************************************************************
 * Modem Manager data structures
 *************************************************************/
// Prototype for the "user cancel" check callback function
typedef Int (*MdmUserCanProcPtr)(DWord userRef);

typedef struct MdmInfoType {
	UInt		serRefNum;					// serial library reference number
	ULong		initialBaud;				// initial baud rate to use
	ULong		cmdTimeOut;					// number of micro-sec to wait after a cmd
	Int		dtWaitSec;					// dialtone wait (sec) (-1 for modem's default)
	Int		dcdWaitSec;					// dcd timeout wait (sec) (-1 for modem's default)
	Int		volume;						// speaker volume(see mdmVolume... constants)
	Boolean	pulse;						// pulse or tone dialing
	Boolean	hwHShake;					// enable cts/rts handshaking
	Boolean	autoBaud;					// enable/disable auto-baud to connected baud rate
	MdmUserCanProcPtr	canProcP;		// ptr to user-cancel function
	DWord		userRef;						// parameter for canProcP()
	Char		cmdBuf[mdmCmdBufSize];	// build all commands here
	Char		respBuf[mdmRespBufSize];// response buffer
	ULong		connectBaud;				// baud at which connection was established
												// (0 = unknown)
	Byte		curStage;					// set by ModemMgr to report current MdmStageEnum
	} MdmInfoType;

typedef MdmInfoType*		MdmInfoPtr;

/************************************************************
 * Modem Manager result codes
 * (mdmErrorClass is defined in SystemMgr.h)
 *************************************************************/
#pragma mark -Error Codes-

#define	mdmErrNoTone			(mdmErrorClass | 1)		// no dial tone
#define	mdmErrNoDCD				(mdmErrorClass | 2)		// no carrier / timeout
#define	mdmErrBusy				(mdmErrorClass | 3)		// busy signal heard
#define	mdmErrUserCan			(mdmErrorClass | 4)		// cancelled by user
#define	mdmErrCmdError			(mdmErrorClass | 5)		// command error
#define	mdmErrNoModem			(mdmErrorClass | 6)		// no modem detected
#define	mdmErrMemory			(mdmErrorClass | 7)		// not enough memory
#define	mdmErrPrefs				(mdmErrorClass | 8)		// modem preferences have not been
																		// setup - (app should take user to modem prefs panel)
#define	mdmErrDial				(mdmErrorClass | 9)		// dial command error - most likely the dial
																		// string is too long for the modem's buffer or
																		// contains invalid characters



/********************************************************************
 * Modem Manager Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/
#pragma mark -API Routines-

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------
// API
//-------------------------------------------------------------------

Err MdmDial(MdmInfoPtr modemP, CharPtr okDialP, CharPtr userInitP, CharPtr phoneNumP)
							SYS_TRAP(sysTrapMdmDial);

Err MdmHangUp(MdmInfoPtr modemP)
							SYS_TRAP(sysTrapMdmHangUp);


#ifdef __cplusplus 
}
#endif



/************************************************************
 * Modem Manager Macros
 *************************************************************/

#endif  // __MODEM_MGR_H__

