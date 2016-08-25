/*******************************************************************
 * 							 Pilot Software  
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:  
 *		SystemMgr.h
 *
 * Description:
 *		Pilot system equates
 *
 * History:
 *		10/27/94	RM		- Created by Ron Marianetti 
 *		10/ 7/96 SCL	- Added sysAppLaunchFlagDataRelocated flag
 *		11/13/96	vmk	- Added sysErrDelayWakened error code
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
 * 
 * IMPORTANT ACTION CODE CONSIDERATIONS:
 *
 * Many action codes are "sent" to apps via a direct function call into the app's
 * PilotMain() function without launching the app.  For these action codes, the
 * application's global and static variables are *not* available, unless the
 * application is already running. Some action codes are synchronized with the
 * currently running UI applcation via the event manager (alarm action codes,
 * for example), while others, such as HotSync action codes, are sent from a
 * background thread. To find out if your app is running (is the current UI
 * app) when an action code is received, test the sysAppLaunchFlagSubCall flag
 * (defined in SystemMgr.h) which is passed to your PilotMain in the
 * launchFlags parameter (the third PilotMain parameter). If it is non-zero,
 * you may assume that your app is currently running and the global variables
 * are accessible. This information is useful if your app maintains an open
 * data database (or another similar resource) when it is running. If the app
 * receives an action code and the sysAppLaunchFlagSubCall is set in
 * launchFlags, the handler may access global variables and use the open
 * database handle while handling the call. On the other hand, if the
 * sysAppLaunchFlagSubCall flag is not set (ie., zero), the handler will need
 * to open and close the database itself and is not allowed to access global
 * or static variables.
 *
 *************************************************************/

// System SysAppLaunch Commands
#define	sysAppLaunchCmdNormalLaunch		0	// Normal Launch

#define	sysAppLaunchCmdFind					1	// Find string

#define	sysAppLaunchCmdGoTo					2	// Launch and go to a particular record

#define	sysAppLaunchCmdSyncNotify			3  // Sent to apps whose databases changed during
															// HotSync after the sync has been completed,
															// including when the app itself has been installed
															// by HotSync. The data database(s) must have the
															// same creator ID as the application for this
															// mechanism to function correctly. This is a
															// good opportunity to update/initialize/validate
															// the app's data, such as resorting records,
															// setting alarms, etc.
															//
															// Parameter block: None.
															// Restrictions: No accessing of global or
															//		static variables; no User Interface calls.
															// Notes: This action code is sent via a
															//		direct function call into the app's
															//		PilotMain function from the background
															//		thread of the HotSync application.


#define	sysAppLaunchCmdTimeChange			4  // Sent to all applications and preference
															// panels when the system time is changed.
															// This notification is the right place to
															// update alarms and other time-related
															// activities and resources.
															//
															// Parameter block: None.
															// Restrictions: No accessing of global or
															//		static variables; no User Interface calls.
															// Notes: This action code is sent via a direct
															//		function call into the app's PilotMain
															//		function without "launching" the app.

#define	sysAppLaunchCmdSystemReset			5  // Sent to all applications and preference
															// panels when the system is either soft-reset
															// or hard-reset.  This notification is the
															// right place to initialize and/or validate
															// your application's preferences/features/
															// database(s) as well as to update alarms and
															// other time-related activities and resources.
															//
															// Parameter block: SysAppLaunchCmdSystemResetType
															// Restrictions: No accessing of global or
															//		static variables; no User Interface calls.
															// Notes: This action code is sent via a direct
															//		function call into the app's PilotMain
															//		function without "launching" the app.

#define	sysAppLaunchCmdAlarmTriggered		6  // Sent to an application at the time its
															// alarm time expires (even when another app
															// is already displaying its alarm dialog box).
															// This call is intended to allow the app to
															// perform some very quick activity, such as
															// scheduling the next alarm or performing a
															// quick maintenance task.  The handler for
															// sysAppLaunchCmdAlarmTriggered must take as
															// little time as possible and is *not* allowed
															// to block (this would delay notification for
															// alarms set by other applications).  
															//
															// Parameter block: SysAlarmTriggeredParamType
															//		(defined in AlarmMgr.h)
															// Restrictions: No accessing of global or
															//		static variables unless sysAppLaunchFlagSubCall
															//		flag is set, as discussed above.
															// Notes: This action code is sent via a direct
															//		function call into the app's PilotMain
															//		function without "launching" the app.

#define	sysAppLaunchCmdDisplayAlarm		7  // Sent to an application when it is time
															// to display the alarm UI. The application
															// is responsible for making any alarm sounds
															// and for displaying the alarm UI.
															// sysAppLaunchCmdDisplayAlarm calls are ordered
															// chronoligically and are not overlapped.
															// This means that your app will receive
															// sysAppLaunchCmdDisplayAlarm only after
															// all earlier alarms have been displayed.
															//
															// Parameter block: SysDisplayAlarmParamType
															//		(defined in AlarmMgr.h)
															// Restrictions: No accessing of global or
															//		static variables unless sysAppLaunchFlagSubCall
															//		flag is set, as discussed above.  UI calls are
															//		allowed to display the app's alarm dialog.
															// Notes: This action code is sent via a direct
															//		function call into the app's PilotMain
															//		function without "launching" the app.

#define	sysAppLaunchCmdCountryChange		8  // The country has changed

#define	sysAppLaunchCmdSyncRequestLocal	9  // Sent to the HotSync application to request a
															// local HotSync.  ("HotSync" button was pressed.)

#define	sysAppLaunchCmdSyncRequest			sysAppLaunchCmdSyncRequestLocal	// for backward compatibility

#define	sysAppLaunchCmdSaveData			  	10 // Sent to running app before sysAppLaunchCmdFind
															// or other action codes that will cause data
															// searches or manipulation.

#define	sysAppLaunchCmdInitDatabase	  	11	// Sent to an application when a database with
															// a matching Creator ID is created during
															// HotSync (in response to a "create db"
															// request). This allows the application to
															// initialize a newly-created database during
															// HotSync.  This might include creating some
															// default records, setting up the database's
															// application and sort info blocks, etc.
															//
															// Parameter block: SysAppLaunchCmdInitDatabaseType
															// Restrictions: No accessing of global or
															//		static variables; no User Interface calls.
															// Notes: This action code is sent via a
															//		direct function call into the app's
															//		PilotMain function from the background
															//		thread of the HotSync application.

#define	sysAppLaunchCmdSyncCallApplicationV10	12 // Used by DesktopLink Server command "call application";
																	// Pilot v1.0 only!!!
																		
//------------------------------------------------------------------------
// New launch codes defined for PalmOS 2.0
//------------------------------------------------------------------------

#define	sysAppLaunchCmdPanelCalledFromApp	13 // The panel should display a done
																// button instead of the pick list.
																// The Done button will return the user
																// to the last app.

#define	sysAppLaunchCmdReturnFromPanel		14	// A panel returned to this app

#define	sysAppLaunchCmdLookup				  	15	// Lookup info managed by an app

#define	sysAppLaunchCmdSystemLock			  	16	// Lock the system until a password is entered.

#define	sysAppLaunchCmdSyncRequestRemote		17	// Sent to the HotSync application to request
																// a remote HotSync.  ("Remote HotSync" button
																// was pressed.)

#define	sysAppLaunchCmdHandleSyncCallApp		18 // Pilot v2.0 and greater.  Sent by DesktopLink Server to an application to handle
																// the "call application" command; use DlkControl with
																// control code dlkCtlSendCallAppReply to send the reply(see DLServer.h).
																// This action code replaces the v1.0 code sysAppLaunchCmdSyncCallApplication.
																// vmk 11/26/96

#define sysAppLaunchCmdAddRecord					19	// Add a record to an applications's database. 

#define sysAppLaunchCmdFailedAppNotify			24	// An app just switched to failed. 


//------------------------------------------------------------------------
// Standard Service Panel launch codes (used by network panel, dialer panel, etc)
// (DOLATER... reserve a range of codes and document parameter block structures)
//------------------------------------------------------------------------
#define	sysSvcLaunchCmdSetServiceID			20
#define	sysSvcLaunchCmdGetServiceID			21
#define	sysSvcLaunchCmdGetServiceList			22
#define	sysSvcLaunchCmdGetServiceInfo			23


//------------------------------------------------------------------------
// Standard Dialer Service launch codes
// (DOLATER... reserve a range of codes and document parameter block structures)
//------------------------------------------------------------------------

#define sysDialLaunchCmdDial						30	// Dial the modem (optionally display dial progress UI)
																// given service ID and serial library refnum.

#define sysDialLaunchCmdHangUp					31	// Hang up the modem (optionally display disconnect
																// progress UI), given service ID and serial library
																// refnum.


//------------------------------------------------------------------------
// Custom action code base (custom action codes begin at this value)
//------------------------------------------------------------------------
#define	sysAppLaunchCmdCustomBase	0x8000

// Your custom launch codes can be defined like this:
//
//	typedef enum {
//		myAppCmdDoSomething = sysAppLaunchCmdCustomBase, 
//		myAppCmdDoSomethingElse,
//		myAppCmdEtcetera
//
//		} MyAppCustomActionCodes;



//------------------------------------------------------------------------
// SysAppLaunch flags (passed to PilotMain)
//------------------------------------------------------------------------

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
#define sysAppLaunchFlagDataRelocated	0x80	// global data (static ptrs) have been "relocated"
															//  by either SysAppStartup or StartupCode.c

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
	Boolean		hardReset;							// true if system was hardReset, false if soft-reset.
	Boolean		createDefaultDB;					// true if app should create default database.
	} SysAppLaunchCmdSystemResetType;
	

// For sysAppLaunchCmdInitDatabase
typedef struct SysAppLaunchCmdInitDatabaseType {
	DmOpenRef	dbP;									// Handle of the newly-created database,
															//		already open for read/write access.
															//		IMPORTANT: The handler *MUST* leave
															//		this database handle open on return.
	ULong			creator;								//	Creator ID of the newly-created database
	ULong			type;									// Type ID of the newly-created database
	UInt			version;								// Version number of the newly-created database
	} SysAppLaunchCmdInitDatabaseType;


// For sysAppLaunchCmdSyncCallApplicationV10
// This structure used on Pilot v1.0 only.  See sysAppLaunchCmdHandleSyncCallApp
// for later platforms.
typedef struct SysAppLaunchCmdSyncCallApplicationTypeV10 {
	Word			action;					// call action id (app-specific)
	Word			paramSize;				// parameter size
	VoidPtr		paramP;					// ptr to parameter
	Byte			remoteSocket;			// remote socket id
	Byte			tid;						// command transaction id
	Boolean		handled;					// if handled, MUST be set true by the app
	} SysAppLaunchCmdSyncCallApplicationTypeV10;


// For sysAppLaunchCmdHandleSyncCallApp (Pilot v2.0 and greater).
// This structure replaces SysAppLaunchCmdSyncCallApplicationType
// which was used in Pilot v1.0
typedef struct SysAppLaunchCmdHandleSyncCallAppType {
	Word			pbSize;					// this parameter block size (set to sizeof SysAppLaunchCmdHandleSyncCallAppType)
	Word			action;					// call action id (app-specific)
	VoidPtr		paramP;					// ptr to parameter
	DWord			dwParamSize;			// parameter size
	VoidPtr		dlRefP;					// DesktopLink reference pointer for passing
												// to DlkControl()'s dlkCtlSendCallAppReply code
												
	Boolean		handled;					// initialized to FALSE by DLServer; if
												// handled, MUST be set TRUE by the app(the
												// handler MUST call DlkControl with
												// control code dlkCtlSendCallAppReply);
												// if the handler is not going to send a reply,
												// it should leave this field set to FALSE, in which
												// case DesktopLink Server will send the default
												// "unknown request" reply.
												
	Err			replyErr;				// error from dlkCtlSendCallAppReply
	
	// RESERVED FOR FUTURE EXTENSIONS				
	DWord			dwReserved1;			// RESERVED -- set to null!!!
	DWord			dwReserved2;			// RESERVED -- set to null!!!

	// Target executable creator and type for testing the mechanism
	// in EMULATION MODE ONLY!!!
	#if EMULATION_LEVEL != EMULATION_NONE
		DWord			creator;
		DWord			type;
	#endif

	} SysAppLaunchCmdHandleSyncCallAppType;

// For sysAppLaunchCmdFailedAppNotify
typedef struct 
	{
	DWord			creator;
	DWord			type;
	Err			result;
	} SysAppLaunchCmdFailedAppNotifyType;
	

/************************************************************
 * Function prototype for libraries
 *************************************************************/

// ***IMPORTANT***
// ***IMPORTANT***
// ***IMPORTANT***
//
// The assembly level TrapDispatcher() function uses a hard-coded value for
// the size of the structure SysLibTblEntryType to obtain a pointer to a
// library entry in the library table.  Therefore, any changes to this structure,
// require corresponding changes in TrapDispatcher() in ROMBoot.c.  Furthermore,
// it is advantageous to keep the size of the structure a power of 2 as this
// improves performance by allowing the entry offset to be calculated by shifting
// left instead of using the multiply instruction.  vmk 8/27/96 (yes, I fell into
// this trap myself)
typedef struct SysLibTblEntryType {
	Ptr*			dispatchTblP;					// pointer to library dispatch table
	void*			globalsP;						// Library globals
	
	// New INTERNAL fields for v2.0 (vmk 8/27/96):
	LocalID		dbID;								// database id of the library
	VoidPtr		codeRscH;						// library code resource handle for RAM-based libraries
	} SysLibTblEntryType;
typedef SysLibTblEntryType*	SysLibTblEntryPtr;

// Library entry point procedure
typedef Err (*SysLibEntryProcPtr)(UInt refNum, SysLibTblEntryPtr entryP);

// This library refNum is reserved for the Debugger comm library
#define	sysDbgCommLibraryRefNum		0

// This refNum signals an invalid refNum
#define	sysInvalidRefNum				0xFFFF


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
#define	flpErrorClass				0x0680	// New Floating Point Library
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
#define	netErrorClass				0x1200	// Net Library
#define	htalErrorClass				0x1300	// HTAL Library

#define	appErrorClass				0x8000	// Application-defined errors

/************************************************************
 * System Errors
 *************************************************************/
#define	sysErrTimeout				(sysErrorClass | 1)
#define	sysErrParamErr				(sysErrorClass | 2)
#define	sysErrNoFreeResource		(sysErrorClass | 3)
#define	sysErrNoFreeRAM			(sysErrorClass | 4)
#define	sysErrNotAllowed			(sysErrorClass | 5)
#define	sysErrSemInUse				(sysErrorClass | 6)
#define	sysErrInvalidID			(sysErrorClass | 7)
#define	sysErrOutOfOwnerIDs		(sysErrorClass | 8)
#define	sysErrNoFreeLibSlots		(sysErrorClass | 9)
#define	sysErrLibNotFound			(sysErrorClass | 10)
#define	sysErrDelayWakened		(sysErrorClass | 11)		// SysTaskDelay wakened by SysTaskWake before delay completed.
#define	sysErrRomIncompatible	(sysErrorClass | 12)

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
#define	sysFtrNumBacklight	3						// Backlight
			// bit 0:	1 if present. 0 if Feature does not exist or backlight is not present
#define	sysFtrNumEncryption	4						// Which encryption schemes are present
#define	sysFtrNumEncryptionMaskDES		0x00000001 // bit 0:	1 if DES is present
			
			
/************************************************************
 * Macros for extracting and combining ROM/OS version components
 *************************************************************/

// ROM/OS stage numbers
#define sysROMStageDevelopment	(0)
#define sysROMStageAlpha			(1)
#define sysROMStageBeta				(2)
#define sysROMStageRelease			(3)


// MACRO: sysMakeROMVersion
//
// Builds a ROM version value from the major, minor, fix, stage, and build numbers
//
#define sysMakeROMVersion(major, minor, fix, stage, buildNum)			\
		(																					\
		(((DWord)((Byte)(major) & 0x0FF)) << 24) |							\
		(((DWord)((Byte)(minor) & 0x00F)) << 20) |							\
		(((DWord)((Byte)(fix)   & 0x00F)) << 16) |							\
		(((DWord)((Byte)(stage) & 0x00F)) << 12) |							\
		(((DWord)((Word)(buildNum) & 0x0FFF)))									\
		)


// Macros for parsing the ROM version number
// (the system OS version is obtained by calling
// FtrGet(sysFtrCreator, sysFtrNumROMVersion, dwOSVerP), where dwOSVerP is
// a pointer to to a DWord variable that is to receive the OS version number)
#define sysGetROMVerMajor(dwROMVer)		(((Word)((dwROMVer) >> 24)) & 0x00FF)
#define sysGetROMVerMinor(dwROMVer)		(((Word)((dwROMVer) >> 20)) & 0x000F)
#define sysGetROMVerFix(dwROMVer)		(((Word)((dwROMVer) >> 16)) & 0x000F)
#define sysGetROMVerStage(dwROMVer)		(((Word)((dwROMVer) >> 12)) & 0x000F)
#define sysGetROMVerBuild(dwROMVer)		(((Word)(dwROMVer))         & 0x0FFF)



/************************************************************
 * System Types
 *************************************************************/
 
// Types of batteries installed.
typedef enum {
	sysBatteryKindAlkaline=0,
	sysBatteryKindNiCad,
	sysBatteryKindLithium
	} SysBatteryKind;


// SysCreateDataBaseList can generate a list of database.
typedef struct 
	{
	Char			name[dmDBNameLength];
	ULong			creator;
	ULong			type;
	UInt			version;
	LocalID		dbID;
	UInt 			cardNo;
	BitmapPtr	iconP;
	} SysDBListItemType;
	

// Structure of a generic message that can be send to a mailbox
// through the SysMailboxSend call. Note, this structure MUST 
// be  CJ_MAXMSZ bytes large, where CJ_MAXMSZ is defined in
// the AMX includes.
typedef struct {
	DWord		data[3];
	} SysMailboxMsgType;
	

// Constants used by the SysEvGroupSignal call
#define	sysEvGroupSignalConstant			0
#define	sysEvGroupSignalPulse				1

// Constants used by the SysEvGroupWait call
#define	sysEvGroupWaitOR						0
#define	sysEvGroupWaitAND						1



/************************************************************
 * System Pre-defined "file descriptors"
 * These are used by applications that use the  Net Library's 
 *   NetLibSelect() call 
 *************************************************************/
#define	sysFileDescStdIn						0


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

Word 		SysTicksPerSecond(void)
							SYS_TRAP(sysTrapSysTicksPerSecond);

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

Err		SysLibLoad(DWord libType, DWord libCreator, UIntPtr refNumP)
							SYS_TRAP(sysTrapSysLibLoad);

							
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
// Task Creation and deleation
Err		SysTranslateKernelErr(Err err)
							SYS_TRAP(sysTrapSysTranslateKernelErr);
							
Err		SysTaskCreate(DWordPtr taskIDP, DWordPtr creator, ProcPtr codeP,
							Ptr stackP, DWord stackSize, DWord attr, DWord priority,
							DWord tSlice)
							SYS_TRAP(sysTrapSysTaskCreate);
							
Err		SysTaskDelete(DWord taskID, DWord priority)
							SYS_TRAP(sysTrapSysTaskDelete);
							
Err		SysTaskTrigger(DWord taskID)
							SYS_TRAP(sysTrapSysTaskTrigger);

ULong		SysTaskID()
							SYS_TRAP(sysTrapSysTaskID);
							
Err		SysTaskDelay(SDWord delay)
							SYS_TRAP(sysTrapSysTaskDelay);
							
Err		SysTaskSetTermProc(DWord taskID, SysTermProcPtr termProcP)
							SYS_TRAP(sysTrapSysTaskSetTermProc);
							
Err		SysTaskSwitching(Boolean enable)
							SYS_TRAP(sysTrapSysTaskSwitching);
							
Err		SysTaskWait(SDWord timeout)
							SYS_TRAP(sysTrapSysTaskWait);
							
Err		SysTaskWake(DWord taskID)
							SYS_TRAP(sysTrapSysTaskWake);		
							
Err		SysTaskWaitClr(void)
							SYS_TRAP(sysTrapSysTaskWaitClr);	
						
Err		SysTaskSuspend(DWord taskID)
							SYS_TRAP(sysTrapSysTaskSuspend);
					
Err		SysTaskResume(DWord taskID)
							SYS_TRAP(sysTrapSysTaskResume);				
							
							
// Counting Semaphores
Err		SysSemaphoreCreate(DWordPtr smIDP, DWordPtr tagP, SDWord initValue)
							SYS_TRAP(sysTrapSysSemaphoreCreate);
							
Err		SysSemaphoreDelete(DWord smID)
							SYS_TRAP(sysTrapSysSemaphoreDelete);
							
Err		SysSemaphoreWait(DWord smID, DWord priority, SDWord timeout)
							SYS_TRAP(sysTrapSysSemaphoreWait);
							
Err		SysSemaphoreSignal(DWord smID)
							SYS_TRAP(sysTrapSysSemaphoreSignal);
							
Err		SysSemaphoreSet(DWord smID)
							SYS_TRAP(sysTrapSysSemaphoreSet);
							
							
// Resource Semaphores				
Err		SysResSemaphoreCreate(DWordPtr smIDP, DWordPtr tagP)
							SYS_TRAP(sysTrapSysResSemaphoreCreate);
							
Err		SysResSemaphoreDelete(DWord smID)
							SYS_TRAP(sysTrapSysResSemaphoreDelete);
							
Err		SysResSemaphoreReserve(DWord smID, DWord priority, SDWord timeout)
							SYS_TRAP(sysTrapSysResSemaphoreReserve);
							
Err		SysResSemaphoreRelease(DWord smID)
							SYS_TRAP(sysTrapSysResSemaphoreRelease);
							
							
							
// Timers							
Err		SysTimerCreate(DWordPtr timerIDP, DWordPtr tagP, 
							SysTimerProcPtr timerProc, DWord periodicDelay,
							DWord	param)
							SYS_TRAP(sysTrapSysTimerCreate);
							
Err		SysTimerDelete(DWord timerID)
							SYS_TRAP(sysTrapSysTimerDelete);
							
Err		SysTimerWrite(DWord timerID, DWord value)
							SYS_TRAP(sysTrapSysTimerWrite);
							 
Err		SysTimerRead(DWord timerID, DWordPtr valueP)
							SYS_TRAP(sysTrapSysTimerRead);
							 

// Information
Err		SysKernelInfo(VoidPtr paramP)
							SYS_TRAP(sysTrapSysKernelInfo);

Boolean SysCreateDataBaseList(ULong type, ULong creator, WordPtr dbCount, 
						Handle *dbIDs, Boolean lookupName)
							SYS_TRAP(sysTrapSysCreateDataBaseList);

Boolean	SysCreatePanelList(WordPtr panelCount, Handle *panelIDs)
							SYS_TRAP(sysTrapSysCreatePanelList);
					

// Mailboxes
Err		SysMailboxCreate(DWordPtr mbIDP, DWordPtr tagP, DWord depth)
							SYS_TRAP(sysTrapSysMailboxCreate);

Err		SysMailboxDelete(DWord mbID)
							SYS_TRAP(sysTrapSysMailboxDelete);
							
Err		SysMailboxFlush(DWord mbID)
							SYS_TRAP(sysTrapSysMailboxFlush);

Err		SysMailboxSend(DWord mbID, VoidPtr msgP, DWord wAck)
							SYS_TRAP(sysTrapSysMailboxSend);

Err		SysMailboxWait(DWord mbID, VoidPtr msgP, DWord priority,
								SDWord timeout)
							SYS_TRAP(sysTrapSysMailboxWait);		
							
// Event Groups
Err		SysEvGroupCreate(DWordPtr evIDP, DWordPtr tagP, DWord init)
							SYS_TRAP(sysTrapSysEvGroupCreate);
							
//Err		SysEvGroupDelete(DWord evID)									// save trap table space - don't need
							//SYS_TRAP(sysTrapSysEvGroupDelete);
							
Err		SysEvGroupSignal(DWord evID, DWord mask, DWord value, SDWord type)
							SYS_TRAP(sysTrapSysEvGroupSignal);
							
Err		SysEvGroupRead(DWord evID, DWordPtr valueP)
							SYS_TRAP(sysTrapSysEvGroupRead);
							
Err		SysEvGroupWait(DWord evID, DWord mask, DWord value, SDWord matchType,
									SDWord timeout)
							SYS_TRAP(sysTrapSysEvGroupWait);

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
