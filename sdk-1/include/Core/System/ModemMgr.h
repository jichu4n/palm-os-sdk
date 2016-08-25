/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
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

/************************************************************
 * Modem Manager data structures
 *************************************************************/
// Prototype for the "user cancel" check callback function
typedef Int (*MdmUserCanProcPtr)(DWord userRef);

typedef struct MdmInfoType {
	UInt		serRefNum;					// serial library reference number
	ULong		initialBaud;				// initial baud rate to use
	ULong		cmdTimeOut;					// number of Msec to wait after a cmd
	Int		dtWaitSec;					// dialtone wait (sec)
	Int		dcdWaitSec;					// dcd timeout wait (sec)
	Int		volume;						// speaker volume
	Boolean	pulse;						// pulse or tone dialing
	Boolean	hwHShake;					// enable cts/rts handshaking
	Boolean	autoBaud;					// enable/disable auto-baud to connected baud rate
	MdmUserCanProcPtr	canProcP;		// ptr to user-cancel function
	DWord		userRef;						// parameter for canProcP()
	Char		cmdBuf[mdmCmdBufSize];	// build all commands here
	Char		respBuf[mdmRespBufSize];// response buffer
	ULong		connectBaud;				// baud at which connection was established
												// (0 = unknown)
	Boolean	traceToLog;					// RESERVED FOR INTERNAL USE: when true,
												// modem commands and responses are traced
												// to the Sync log.
	} MdmInfoType;

typedef MdmInfoType*		MdmInfoPtr;

/************************************************************
 * Modem Manager result codes
 * (mdmErrorClass is defined in SystemMgr.h)
 *************************************************************/
#define	mdmErrNoTone			(mdmErrorClass | 1)		// no dial tone
#define	mdmErrNoDCD				(mdmErrorClass | 2)		// no carrier / timeout
#define	mdmErrBusy				(mdmErrorClass | 3)		// busy signal heard
#define	mdmErrUserCan			(mdmErrorClass | 4)		// cancelled by user
#define	mdmErrCmdError			(mdmErrorClass | 5)		// command error



/********************************************************************
 * Modem Manager Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/
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

