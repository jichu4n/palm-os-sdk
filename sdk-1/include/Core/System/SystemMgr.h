/*******************************************************************
 * 							 Pilot Software  
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:  
 *		SystemMgr.h
 *
 * Description:
 *		Pilot system equates
 *
 * History:
 *		10/27/94	RM	- Created by Ron Marianetti   
 *
 *******************************************************************/

 #ifndef __SYSTEMMGR_H__
 #define __SYSTEMMGR_H__

// Include elementary types
#include <Common.h>
#include <SysTraps.h>				// Trap Numbers.

// Other types
#include <SystemMgr.rh>				// Resource definitions.


// UI Headers 
#include <UICommon.h>
#include <Init.h>
#include <Rect.h>
#include <Font.h>
#include <Window.h>
#include <InsPoint.h>
#include <Event.h>
#include <DataMgr.h>					// for DmOpenRef



/************************************************************
 * System Constants
 *************************************************************/
// Define the number of ticks/second
#if EMULATION_LEVEL == EMULATION_MAC
	#define	sysTicksPerSecond				60					// 60/sec on Macintosh
#elif EMULATION_LEVEL == EMULATION_NONE
	#define	sysTicksPerSecond				100				// 100/sec on Pilot
#elif EMULATION_LEVEL == EMULATION_WINDOWS
	#define	sysTicksPerSecond				1000				// 1000/sec on Windows PC
#else
	#error
#endif

	
	

/************************************************************
 * Action Codes
 *************************************************************/
// SysAppLaunch Commands
#define	sysAppLaunchCmdNormalLaunch		0	// Normal Launch
#define	sysAppLaunchCmdFind					1	// Find string
#define	sysAppLaunchCmdGoTo					2	// Launch and go to a particular record
#define	sysAppLaunchCmdSyncNotify			3  // Sent to apps whose databases changed during
															// HotSync after the sync has been completed
#define	sysAppLaunchCmdTimeChange			4  // The system time has changed
#define	sysAppLaunchCmdSystemReset			5  // Sent after System hard resets
#define	sysAppLaunchCmdAlarmTriggered		6  // Schedule next alarm
#define	sysAppLaunchCmdDisplayAlarm		7  // Display given alarm dialog
#define	sysAppLaunchCmdCountryChange		8  // The country has changed
#define	sysAppLaunchCmdSyncRequest			9  // The "HotSync" button was pressed
#define	sysAppLaunchCmdSaveData			  10  // Sent to running app before sysAppLaunchCmdFind
															//  or other action codes that will cause
															//  data searches or manipulation.
#define	sysAppLaunchCmdInitDatabase	  11  // Initialize a database; sent by DesktopLink
															// server to the app whose creator ID matches
															// that of the database created in response to
															// the "create db" request.
#define	sysAppLaunchCmdSyncCallApplication	  12  // Used by DesktopLink Server command "call application"



// SysAppLaunch flags
#define	sysAppLaunchFlagNewThread		0x01	// create a new thread for application
															//  - implies sysAppLaunchFlagNewStack
#define	sysAppLaunchFlagNewStack		0x02	// create separate stack for application
#define	sysAppLaunchFlagNewGlobals		0x04	// create new globals world for application
															//  - implies new owner ID for Memory chunks
#define	sysAppLaunchFlagUIApp			0x08	// notifies launch routine that this is a UI app being
															//  launched.
#define	sysAppLaunchFlagSubCall			0x10	// notifies launch routine that the app is calling it's
															//  entry point as a subroutine call. This tells the launch
															//  code that it's OK to keep the A5 (globals) pointer valid 
															//  through the call.

//-------------------------------------------------------------------
// Parameter blocks for action codes
// NOTE: The parameter block for the  sysAppLaunchCmdFind  and sysAppLaunchCmdGoTo
//  action codes are defined in "Find.h";
//---------------------------------------------------------------------------

// For sysAppLaunchCmdSaveData
typedef struct {
	Boolean		uiComing;							// true if system dialog will be put up
															// before coming action code arrives.
	} SysAppLaunchCmdSaveDataType;
	
// For sysAppLaunchCmdSystemReset
typedef struct {
	Boolean		hardReset;							// true if system was hardReset.
	Boolean		createDefaultDB;					// true if app should create default database.
	} SysAppLaunchCmdSystemResetType;
	

// For sysAppLaunchCmdInitDatabase
typedef struct SysAppLaunchCmdInitDatabaseType {
	DmOpenRef	dbP;									// database reference
	ULong			creator;								// datatase creator
	ULong			type;									// database type
	UInt			version;								// database version
	} SysAppLaunchCmdInitDatabaseType;


// For sysAppLaunchCmdSyncCallApplication
typedef struct SysAppLaunchCmdSyncCallApplicationType {
	Word			action;								// call action id (app-specific)
	Word			paramSize;							// parameter size
	VoidPtr		paramP;								// ptr to parameter
	Byte			remoteSocket;						// remote socket id
	Byte			tid;									// command transaction id
	Boolean		handled;								// if handled, MUST be set true by the app
	} SysAppLaunchCmdSyncCallApplicationType;



/************************************************************
 * Function prototype for libraries
 *************************************************************/
typedef struct SysLibTblEntryType {
	Ptr*	dispatchTblP;								// pointer to library dispatch table
	void*	globalsP;									// Library globals
	} SysLibTblEntryType;
typedef SysLibTblEntryType*	SysLibTblEntryPtr;


typedef Err (*SysLibEntryProcPtr)(UInt refNum, SysLibTblEntryPtr entryP);

// This library refNum is reserved for the Debugger comm library
#define	sysDbgCommLibraryRefNum		0



/************************************************************
 * Function prototype for Kernel
 *************************************************************/
// Task termination procedure prototype for use with SysTaskSetTermProc
typedef void (*SysTermProcPtr)(ULong taskID, Long reason);

// Timer procedure for use with SysTimerCreate
typedef void (*SysTimerProcPtr)(Long timerID, Long param);




/************************************************************
 * Error Classes for each manager
 *************************************************************/
#define	memErrorClass				0x0100	// Memory Manager
#define	dmErrorClass				0x0200	// Data Manager
#define	serErrorClass				0x0300	// Serial Manager
#define	slkErrorClass				0x0400	// Serial Link Manager
#define	sysErrorClass				0x0500	// System Manager
#define	fplErrorClass				0x0600	// Floating Point Library
#define	evtErrorClass				0x0700  	// System Event Manager
#define	sndErrorClass				0x0800  	// Sound Manager
#define	almErrorClass				0x0900  	// Alarm Manager
#define	timErrorClass				0x0A00  	// Time Manager
#define	penErrorClass				0x0B00  	// Pen Manager
#define	ftrErrorClass				0x0C00  	// Feature Manager
#define	cmpErrorClass				0x0D00  	// Connection Manager
#define	dlkErrorClass				0x0E00	// Desktop Link Manager
#define	padErrorClass				0x0F00	// PAD Manager
#define	grfErrorClass				0x1000	// Graffiti Manager
#define	mdmErrorClass				0x1100	// Modem Manager


/************************************************************
 * System Errors
 *************************************************************/
#define	sysErrTimeout			(sysErrorClass | 1)
#define	sysErrParamErr			(sysErrorClass | 2)
#define	sysErrNoFreeResource	(sysErrorClass | 3)
#define	sysErrNoFreeRAM		(sysErrorClass | 4)
#define	sysErrNotAllowed		(sysErrorClass | 5)
#define	sysErrSemInUse			(sysErrorClass | 6)
#define	sysErrInvalidID		(sysErrorClass | 7)
#define	sysErrOutOfOwnerIDs	(sysErrorClass | 8)
#define	sysErrNoFreeLibSlots	(sysErrorClass | 9)
#define	sysErrLibNotFound		(sysErrorClass | 10)


/************************************************************
 * System Features
 *************************************************************/
#define	sysFtrCreator			sysFileCSystem		// Feature Creator
#define	sysFtrNumROMVersion	1						// ROM Version
			// 0xMMmfsbbb, where MM is major version, m is minor version
			// f is bug fix, s is stage: 3-release,2-beta,1-alpha,0-development,
			// bbb is build number for non-releases 
			// V1.12b3   would be: 0x01122003
			// V2.00a2   would be: 0x02001002
			// V1.01     would be: 0x01013000
#define	sysFtrNumProductID	2						// Product id
			// 0xMMMMRRRR, where MMMM is the model and RRRR
			// is the revision.




/************************************************************
 * System Types
 *************************************************************/
 
// Types of batteries installed.
typedef enum {
	sysBatteryKindAlkaline=0,
	sysBatteryKindNiCad,
	sysBatteryKindLithium
	} SysBatteryKind;



/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Prototype for Pilot applications entry point
DWord		PilotMain(Word cmd, Ptr cmdPBP, Word launchFlags);


void		SysUnimplemented(void)
							SYS_TRAP(sysTrapSysUnimplemented);

void		SysColdBoot(Ptr card0P, ULong card0Size, 
							Ptr card1P, ULong card1Size,
							DWord	sysCardHeaderOffset)
							SYS_TRAP(sysTrapSysColdBoot);
							
void 		SysInit(void)
							SYS_TRAP(sysTrapSysInit);
							
void 		SysReset(void)
							SYS_TRAP(sysTrapSysReset);
							
void 		SysPowerOn(Ptr card0P, ULong card0Size, 
							Ptr card1P, ULong card1Size,
							DWord sysCardHeaderOffset, Boolean reFormat);
							
							
void 		SysDoze(Boolean onlyNMI)
							SYS_TRAP(sysTrapSysDoze);
							
Err 		SysSetPerformance(ULongPtr sysClockP, UIntPtr cpuDutyP)
							SYS_TRAP(sysTrapSysSetPerformance);
							
void 		SysSleep(Boolean untilReset, Boolean emergency)
							SYS_TRAP(sysTrapSysSleep);
							
UInt		SysBatteryInfo(Boolean set, UIntPtr warnThresholdP, UIntPtr criticalThresholdP,
						UIntPtr maxTicksP, SysBatteryKind* kindP, Boolean* pluggedIn)
							SYS_TRAP(sysTrapSysBatteryInfo);
							
UInt 		SysSetAutoOffTime(UInt	seconds)
							SYS_TRAP(sysTrapSysSetAutoOffTime);

Err		SysLaunchConsole(void)
							SYS_TRAP(sysTrapSysLaunchConsole);

Boolean	SysHandleEvent(EventPtr eventP)
							SYS_TRAP(sysTrapSysHandleEvent);
							
void 		SysUILaunch(void)
							SYS_TRAP(sysTrapSysUILaunch);

Err 		SysUIAppSwitch(UInt cardNo, LocalID dbID, Word cmd, Ptr cmdPBP)
							SYS_TRAP(sysTrapSysUIAppSwitch);
							
Err 		SysCurAppDatabase(UIntPtr cardNoP, LocalID* dbIDP)
							SYS_TRAP(sysTrapSysCurAppDatabase);
							
Err		SysBroadcastActionCode(Word cmd, Ptr cmdPBP)
							SYS_TRAP(sysTrapSysBroadcastActionCode);
							
Err 		SysAppLaunch(UInt cardNo, LocalID dbID, UInt launchFlags,
							Word cmd, Ptr cmdPBP, DWord* resultP)
							SYS_TRAP(sysTrapSysAppLaunch);
							
UInt		SysNewOwnerID(void)
							SYS_TRAP(sysTrapSysNewOwnerID);
							
DWord		SysSetA5(DWord newValue)
							SYS_TRAP(sysTrapSysSetA5);


Ptr		SysCardImageInfo(UInt cardNo, ULongPtr sizeP);
							
void		SysCardImageDeleted(UInt cardNo);

// System Dialogs
void		SysBatteryDialog(void)
							SYS_TRAP(sysTrapSysBatteryDialog);

// Utilities
Err		SysSetTrapAddress(UInt trapNum, VoidPtr procP)
							SYS_TRAP(sysTrapSysSetTrapAddress);
							
VoidPtr	SysGetTrapAddress(UInt trapNum)
							SYS_TRAP(sysTrapSysGetTrapAddress);
							
Word		SysDisableInts(void)
							SYS_TRAP(sysTrapSysDisableInts);
							
void		SysRestoreStatus(Word status)
							SYS_TRAP(sysTrapSysRestoreStatus);
							

// Library Management
Err		SysLibInstall(SysLibEntryProcPtr libraryP, UIntPtr refNumP)
							SYS_TRAP(sysTrapSysLibInstall);
							
Err		SysLibRemove(UInt refNum)
							SYS_TRAP(sysTrapSysLibRemove);
							
Err		SysLibFind(CharPtr nameP, UIntPtr refNumP)
							SYS_TRAP(sysTrapSysLibFind);
							
SysLibTblEntryPtr	SysLibTblEntry(UInt refNum)
							SYS_TRAP(sysTrapSysLibTblEntry);
							
// Generic Library calls
Err		SysLibOpen(UInt refNum)
							SYS_TRAP(sysLibTrapOpen);
Err		SysLibClose(UInt refNum)
							SYS_TRAP(sysLibTrapClose);
Err		SysLibSleep(UInt refNum)
							SYS_TRAP(sysLibTrapSleep);
Err		SysLibWake(UInt refNum)
							SYS_TRAP(sysLibTrapWake);
							
							
//-----------------------------------------------------
// Kernel Prototypes
//-----------------------------------------------------
Err		SysTranslateKernelErr(Err err)
							SYS_TRAP(sysTrapSysTranslateKernelErr);
							
Err		SysTaskCreate(ULongPtr taskIDP, LongPtr creator, ProcPtr codeP,
							Ptr stackP, ULong stackSize, ULong attr, Long priority,
							ULong tSlice)
							SYS_TRAP(sysTrapSysTaskCreate);
							
Err		SysTaskDelete(ULong taskID, Long priority)
							SYS_TRAP(sysTrapSysTaskDelete);
							
Err		SysTaskTrigger(ULong taskID)
							SYS_TRAP(sysTrapSysTaskTrigger);

ULong		SysTaskID()
							SYS_TRAP(sysTrapSysTaskID);
							
Err		SysTaskDelay(Long delay)
							SYS_TRAP(sysTrapSysTaskDelay);
							
Err		SysTaskSetTermProc(ULong taskID, SysTermProcPtr termProcP)
							SYS_TRAP(sysTrapSysTaskSetTermProc);
							
Err		SysTaskSwitching(Boolean enable)
							SYS_TRAP(sysTrapSysTaskSwitching);
							
							
// Counting Semaphores
Err		SysSemaphoreCreate(LongPtr smIDP, LongPtr tagP, Long initValue)
							SYS_TRAP(sysTrapSysSemaphoreCreate);
							
Err		SysSemaphoreDelete(Long smID)
							SYS_TRAP(sysTrapSysSemaphoreDelete);
							
Err		SysSemaphoreWait(Long smID, Long priority, Long timeout)
							SYS_TRAP(sysTrapSysSemaphoreWait);
							
Err		SysSemaphoreSignal(Long smID)
							SYS_TRAP(sysTrapSysSemaphoreSignal);
							
Err		SysSemaphoreSet(Long smID)
							SYS_TRAP(sysTrapSysSemaphoreSet);
							
							
// Resource Semaphores				
Err		SysResSemaphoreCreate(LongPtr smIDP, LongPtr tagP)
							SYS_TRAP(sysTrapSysResSemaphoreCreate);
							
Err		SysResSemaphoreDelete(Long smID)
							SYS_TRAP(sysTrapSysResSemaphoreDelete);
							
Err		SysResSemaphoreReserve(Long smID, Long priority, Long timeout)
							SYS_TRAP(sysTrapSysResSemaphoreReserve);
							
Err		SysResSemaphoreRelease(Long smID)
							SYS_TRAP(sysTrapSysResSemaphoreRelease);
							
							
							
// Timers							
Err		SysTimerCreate(LongPtr timerIDP, LongPtr tagP, 
							SysTimerProcPtr timerProc, Long periodicDelay,
							Long	param)
							SYS_TRAP(sysTrapSysTimerCreate);
							
Err		SysTimerDelete(Long timerID)
							SYS_TRAP(sysTrapSysTimerDelete);
							
Err		SysTimerWrite(Long timerID, ULong value)
							SYS_TRAP(sysTrapSysTimerWrite);
							 
Err		SysTimerRead(Long timerID, ULong* valueP)
							SYS_TRAP(sysTrapSysTimerRead);
							 

// Information
Err		SysKernelInfo(VoidPtr paramP)
							SYS_TRAP(sysTrapSysKernelInfo);
							

#ifdef __cplusplus 
}
#endif



/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_SysSemaphoreSignal		\
				ASM_SYS_TRAP(sysTrapSysSemaphoreSignal)

#define	_SysSemaphoreSet		\
				ASM_SYS_TRAP(sysTrapSysSemaphoreSet)

#define	_SysDoze		\
				ASM_SYS_TRAP(sysTrapSysDoze)


#endif  //__SYSTEMMGR_H__
