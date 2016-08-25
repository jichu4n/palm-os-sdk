/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		SoundPrv.h
 *
 * Description:
 *		Private Include file for Sound Manager
 *
 * History:
 *   	4/11/95  VMK - Created by Vitaly Kruglikov
 *
 *******************************************************************/

#ifdef	NON_PORTABLE

#ifndef __SOUNDPRV_H__
#define __SOUNDPRV_H__



/************************************************************
 * Sound Manager constants
 *************************************************************/

#define	sndSamplingRate					8000	// 8 kHz


// DOLATER: determine appropriate values experimentally (TBD)
#define	sndBeepDurationMSec				70				// msec
#define	sndConfirmationDurationMSec	70				// msec
#define	sndClickDurationMSec				9				// msec

// These frequencies are more audible on the current hardware.
#define	sndInfoFreq							500			// Hz
#define	sndWarningFreq						500			// Hz
#define	sndErrorFreq						500			// Hz
#define	sndConfirmationFreq				500			// Hz
#define	sndStartUpFreq						1000			// Hz
#define	sndClickFreq						200			// Hz


/*******************************************************************
 * Sound Manager Globals
 *
 *******************************************************************/
typedef struct SndGlobalsType {
	Long				smID;					// semaphore id for data access integrity
	Byte				sysAmp;				// system sound amplitude(0-sndMaxVolume)
	Byte				alarmAmp;			// alarm sound amplitude(0-sndMaxVolume)
	Byte				defAmp;				// default amplitude for other sounds(0-sndMaxVolume)
	} SndGlobalsType;

typedef SndGlobalsType*		SndGlobalsPtr;


#endif  // __SOUNDPRV_H__
#endif  // NON_PORTABLE
