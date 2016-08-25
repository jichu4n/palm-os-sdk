/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SoundMgr.h
 *
 * Description:
 *		Include file for Sound Manager
 *
 * History:
 *   	4/11/95  VMK - Created by Vitaly Kruglikov
 *
 *******************************************************************/


#ifndef __SOUNDMGR_H__
#define __SOUNDMGR_H__


// Include elementary types
#include <Common.h>
#include <SysTraps.h>				// Trap Numbers.



/************************************************************
 * Sound Manager constants
 *
 *************************************************************/

// Sound Manager max and default volume levels
#define sndMaxAmp				64
//#define sndVolumeMask		0x0ff
#define sndDefaultAmp		sndMaxAmp


/************************************************************
 * Sound Manager data structures
 *
 *************************************************************/

//
// Command numbers for SndCommandType's cmd field
//
typedef enum SndCmdIDType {
									
	sndCmdFreqDurationAmp = 1		// play a sound
											// param1 = frequency in Hz
											// param2 = duration in milliseconds
											// param3 = amplitude (0 - sndMaxAmp)

	} SndCmdIDType;

//
// SndCommandType: used by SndDoCmd()
//

typedef struct SndCommandType {
SndCmdIDType	cmd;						// command id
Long				param1;					// first parameter
UInt				param2;					// second parameter
UInt				param3;					// third parameter
} SndCommandType;

typedef SndCommandType*		SndCommandPtr;


//
// Beep numbers used by SndSysBeep()
//

typedef enum SndSysBeepType {
	sndInfo = 1,
	sndWarning,
	sndError,
	sndStartUp,
	sndAlarm,
	sndConfirmation,
	sndClick
	} SndSysBeepType;



/************************************************************
 * Sound Manager result codes
 * (sndErrorClass is defined in SystemMgr.h)
 *************************************************************/
#define	sndErrBadParam			(sndErrorClass | 1)
#define	sndErrBadChannel		(sndErrorClass | 2)
#define	sndErrMemory			(sndErrorClass | 3)
#define	sndErrOpen				(sndErrorClass | 4)
#define	sndErrQFull				(sndErrorClass | 5)
#define	sndErrQEmpty			(sndErrorClass | 6)		// internal



/********************************************************************
 * Sound Manager Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------------------------------------
// Initialization
//-------------------------------------------------------------------

// Initializes the Sound Manager.  Should only be called by
// Pilot initialization code.
Err			SndInit(void)	SYS_TRAP(sysTrapSndInit);

// Frees the Sound Manager.
//void			SndFree(void)	SYS_TRAP(sysTrapSndFree);


//-------------------------------------------------------------------
// API
//-------------------------------------------------------------------

// Sets default sound volume levels
//
// Any parameter may be passed as NULL
void			SndSetDefaultVolume(UIntPtr alarmAmpP, UIntPtr sysAmpP, UIntPtr defAmpP)
							SYS_TRAP(sysTrapSndSetDefaultVolume);

// Gets default sound volume levels
//
// Any parameter may be passed as NULL
void			SndGetDefaultVolume(UIntPtr alarmAmpP, UIntPtr sysAmpP, UIntPtr defAmpP)
							SYS_TRAP(sysTrapSndGetDefaultVolume);

// Executes a sound command on the given sound channel (pass
// channelP = 0 to use the shared channel).
Err			SndDoCmd(VoidPtr /*SndChanPtr*/ channelP, SndCommandPtr cmdP, Boolean noWait)
							SYS_TRAP(sysTrapSndDoCmd);

// Plays one of several defined system beeps/sounds (see sndSysBeep...
// constants).
void			SndPlaySystemSound(SndSysBeepType beepID)
							SYS_TRAP(sysTrapSndPlaySystemSound);

#ifdef __cplusplus 
}
#endif



/************************************************************
 * Sound Manager Macros
 *
 *************************************************************/

#endif  // __SOUND_MGR_H__

