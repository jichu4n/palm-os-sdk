/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:  
 *		SystemPrv.h
 *
 * Description:
 *		Private Pilot system equates
 *
 * History:
 *		8/9/95	RM	 - Created by Ron Marianetti   
 *		1/28/98	SCL - Added HotSync port "hint" for SerialHWMgr
 *		2/04/98 srj - 	Added HW_TARGET_EZ in appropriate places.
 *						HW_TARGET_TD1 in this file means "any
 *						handheld", not non-EZ platforms.
 *
 *******************************************************************/

#ifdef	NON_PORTABLE

#ifndef __SYSTEMPRV_H__
#define __SYSTEMPRV_H__

// Include fixed global type
#include <Globals.h>
#include <SerialMgr.h>		// for serPortLocalHotSync define


/************************************************************
 * Common Equates between Native and Emulation mode
 *************************************************************/
#define	sysCardSignature				0xFEEDBEEFL			// card signature long word
#define  sysStoreSignature				0xFEEDFACEL			// store signature long word


// This should be set to sizeof(LowMemType) for the final version, but
//  until then we'll make it bigger than LowMemType so that we can
//  add more globals without having to re-format memory cards.
#define	sysLowMemSize					0x1510						// size of Low Memory Area
#define	sysInitStack					(sysLowMemSize+0x1000)	// stack ptr during boot



// The location of the Card Header (start of ROM) is plugged in at run
//  time to the global variable GSysCardHeaderOffset. This MUST be done
//  by the startup code. We make this a run-time variable so that the same
//  executable ROM image can be run from ROM or RAM.
// #define sysCardHeaderOffset		someConstant
#define	sysRAMOnlyCardHeaderOffset 0x0000000L			// RAM only card header created here 
#define	sysCardHeaderSize				0x0100				// size of card header


// Offset to ROM storage header from card header
#define	sysROMStoreRelOffset			sysCardHeaderSize
#define	sysStoreHeaderSize			0x0100				// size of storage header


// This equate tells us how large of a heap info table to reserve in the
//  globals for each card
#define	sysMaxHeapsPerCard			128					// max # of heaps per card


// This equtes tells us the initial size of the Library table as held in
//  the Low Memory area
#define	sysDefaultLibraries			DEFAULT_LIB_ENTRIES	// default size of library table


// The default port number and baud rate for the serial port
//  (send "HotSync" port hint for SerialHWMgr)
#if (HW_TARGET == HW_TARGET_TD1) || (HW_TARGET == HW_TARGET_EZ)
#define	sysDefaultSerPort				serPortLocalHotSync	// Use port #0
#define	sysDefaultSerBaud				57600						// baud rate 
#else
#define	sysDefaultSerPort				1							// Use port #1
#define	sysDefaultSerBaud				57600						// baud rate 
#endif


// Define initial setting for software 

#define	sysInitialContrastSetting	0x80

#define HwrLCDContrastMin	0x64
#define HwrLCDContrastScale	0x02


/************************************************************
 * Structure of the PREF resource in applications. Note, this
 *  structure must mirror the structure of the PREF resource
 *	 as defined in the PilotRez include file.
 *************************************************************/
typedef struct SysAppPrefs {
	Word		priority;					// task priority
	DWord		stackSize;					// required stack space
	DWord		minHeapSpace;				// minimum heap space required
	} SysAppPrefsType;
typedef SysAppPrefsType* SysAppPrefsPtr;



/************************************************************
 * Structure of Application info for an application. Applications
 *  do not necessarily have to be on their own thread - there
 *  can be more than 1 app on the same AMX task. Each application
 *  has an assocated SysAppInfoType structure which holds the
 *  application specific information like the database handle of the
 *  app, the code handle, the stack chunk pointer, the owner ID, etc.
 *
 * As of PalmOS 3.X, one of these structures is created for each 
 *  app running as an action code.
 *
 * ***
 * ***IMPORTANT: ADD NEW FIELDS AT THE END OF THE STRUCTURE FOR
 * ***           BACKWARD COMPATIBILITY 
 * ***
 *************************************************************/
typedef struct SysAppInfoType {
	Int				cmd;							// command code for app
	Ptr				cmdPBP;						// cmd ParamBlock  
	UInt				launchFlags;				// launch flags  
	
	Long				taskID;						// AMX task ID of task that app runs in
	Handle			codeH;						// code handle of the main code segment
	DmOpenRef		dbP;							// Application database access ptr of App
	Ptr				stackP;						// stack chunk for the App
	Ptr				globalsChunkP;				// globals chunk for the App

	UInt				memOwnerID;					// owner ID for Memory Manager chunks 
	Ptr				dmAccessP;					// pointer to linked list of opened DB's
	SWord				dmLastErr;					// Last error from Data Manager
	Ptr				errExceptionP;				// ErrTry,Catch exception list

	// PalmOS v3.0 fields begin here	
	Ptr				a5Ptr;						// A5 ptr for this app
	Ptr				stackEndP;					// stack chunk end for the App (last byte)
	Ptr				globalEndP;					// global chunk end for the App (last byte)
	struct SysAppInfoType *rootP;				// Points to the SysAppInfoType first
														// allocated for this thread.
	Ptr				extraP;						// unused ptr for the App.
	} SysAppInfoType;
typedef SysAppInfoType* SysAppInfoPtr;


/************************************************************
 * Structure of the user info in the TCB of a task. This structure
 *	is stored in the xtcbuser fields of the AMX TCB for a task and 
 * must not exceed 4 DWords in length (the size reserved by AMX for
 * user info).
 *************************************************************/
typedef struct SysTCBUserInfoType {
	SysAppInfoPtr	tmpAppInfoP;				// used to pass appInfo to nested launches
	SysAppInfoPtr	rootAppInfoP;				// App info for the root application on this task
	DWord				initialA5;					// Inital value for A5 globals ptr
	} SysTCBUserInfoType;
typedef SysTCBUserInfoType* SysTCBUserInfoPtr;


//*****************************************************************
// Structure of ParamBlock for the SysKernelInfo call
//*****************************************************************

// Selector codes
typedef enum	 {
	sysKernelInfoSelCurTaskInfo,
	sysKernelInfoSelTaskInfo,
	sysKernelInfoSelSemaphoreInfo,
	sysKernelInfoSelTimerInfo
	} SysKernelInfoSelector;

typedef struct SysKernelInfoType {
	SysKernelInfoSelector	selector;		// Which info to get
	DWord							id;				// which object to get info on
	
	union {
		struct {
			DWord		id;							// ID of task
			DWord		nextID;						// ID of next task
			
			DWord		tag;							// task tag
			DWord		status;						// task status
			DWord		timer;						// # ticks left if task's timeout
			DWord		timeSlice;					// task's time slice
			SWord		priority;					// task priority
			Word		attributes;					// task attributes
			SWord		pendingCalls;				// # of pending calls
			DWord		senderTaskID;				// task ID of message sender
			DWord		msgExchangeID;				// task's message exchange ID, if any
			
			DWord		tcbP;							// pointer to TCB
			DWord		stackP;						// stack pointer of task
			DWord		stackStart;					// top of task stack (where it started).
			DWord		stackSize;					// size of stack
			} task;

		struct {
			DWord		id;
			DWord		nextID;						// ID of next semaphore

			DWord		tag;							// semaphore tag
			SWord		initValue;					// semaphore initial value 
														//  >= 0 for counting, -1 for resource
			SWord		curValue;					// semaphore current value
														//  >0 available
														//   0 not available
														//  -n not available, n tasks waiting
			SWord		nestLevel;					// 0 for counting, >= 0 for resource
			DWord		ownerID;						// owner ID
														//   0 for counting or resource free
			} semaphore;
			
		struct {
			DWord		id;
			DWord		nextID;						// ID of next timer
			
			DWord		tag;							// timer tag
			DWord		ticksLeft;					// ticks left till timer runs
			DWord		period;						// timer period, if periodic
			DWord		proc;							// timer procedure
			} timer;
		} param;
		
	} SysKernelInfoType;
	
	
typedef SysKernelInfoType*	SysKernelInfoPtr;




//-----------------------------------------------------------------
// Bits for status field in task info parameter block
//------------------------------------------------------------------
#define	sysTaskStatusTriggerWait		0x01		// (CJ_MATSWTR) Task idle (waiting for trigger)
#define	sysTaskStatusSuspended			0x02		// (CJ_MATSWSUS) Task suspended (waiting for resume)
#define	sysTaskStatusWaiting				0x04		// (CJ_MATSWAIT) Task waiting (see other bits)
#define	sysTaskStatusHalted				0x80		// (CJ_MATSWHLT) Task halted

// If sysTaskStatusWaiting set, these bits tell why....
#define	sysTaskStatusWaitSemaphore		0x00100	// (CJ_MATSWSM) waiting on semaphore
#define	sysTaskStatusWaitEvent			0x00200	// (CJ_MATSWEV) waiting on event group
#define	sysTaskStatusWaitMailbox		0x00400	// (CJ_MATSWMB) waiting on mailbox
#define	sysTaskStatusWaitMsgExchange	0x00800	// (CJ_MATSWMX) waiting on message exchange
#define	sysTaskStatusWaitBuffer			0x01000	// (CJ_MATSWBUF) waiting on buffer
#define	sysTaskStatusWaitTimer			0x08000	// (CJ_MATSWTM) waiting on timer

#define	sysTaskStatusWaitMailboxAck	0x00010000L	// (CJ_MATSWMBAK) mailbox ack
#define	sysTaskStatusWaitMsgXAck		0x00020000L	// (CJ_MATSWMXAK) msg exchange ack
#define	sysTaskStatusWaitTaskSig		0x00040000L	// (CJ_MATSWSIG) task signal wait

//-----------------------------------------------------------------
// Bits for attributes field in task info parameter block
//------------------------------------------------------------------
#define	sysTaskAttrNoHalt					0x02		// (CJ_MATAHLT) Task cannot be halted
#define	sysTaskAttrMsgStack				0x08		// (CJ_MATAMSG) receives msgs on stack
#define	sysTaskAttrMsgByRef				0x10		// (CJ_MATAPBR) messages passed by reference
#define	sysTaskAttrMsgOnStack			0x4000	// (CJ_MATAMRDY) message ready on stack
#define	sysTaskAttrMsgSenderWaiting	0x8000	// (CJ_MATAMSW) msg sender waiting for ack



/************************************************************
 * This structure defines a section within the RAM storage header
 *  on Card#0 used to hold non-volatile System information. We store
 *  System information that can not be conveniently stored or accessed
 *  from a Database in this area because:
 *	1.) it can be accessed earlier during the boot-up process and 
 * 2.) It can be accessed from an interrupt routine.
 *************************************************************/ 
typedef struct SysNVParamsType {
	DWord		rtcHours;									// Real-Time clock hours - add to value
																// in DragonBall RTC register to get
																// actual date & time.
	DWord		rtcHourMinSecCopy;						// Copy of latest value in rtcHourMinSec reg of 
																// DBall. Used as default RTC value on Reset.
	Byte		swrLCDContrastValue;						// Contrast Value for LCD on EZ-based products
																// that use the software contrast PWM (such as Sumo)
																// this is the SOFTWARE value, which is scaled to 
																// get the hardware value from 0-255.
	} SysNVParamsType;
typedef SysNVParamsType*	SysNVParamsPtr;



/************************************************************
 * Structure of the System Globals. This is where we place
 *  variable size globals that the System Manager uses. This
 *  is currently not used but a pointer for it is reserved
 *  in the low memory globals
 *************************************************************/
/*
typedef struct SysGlobalsType {
	} SysGlobalsType;
typedef SysGlobalsType* SysGlobalsPtr;
*/



/************************************************************
 * Assignment of bits to the event group used for implementing the
 *   NetLibSelect() call. This event group is also used by the System
 *   Event Manager (SysEvtMgr.c) and includes a bit for "stdin" 
 *   (pen & keyboard) besides the other bits used for Net Library socket refNums. 
 *************************************************************/
#define	sysFileDescNet	(sysFileDescStdIn+1)		// first available refNum for use by
																//  Net library.


// Macro to generate a bitmask for the event group corresponding to a particular
//  refNum id and boolean indicating whether it's for input or output. The outputs
// are in the upper 16 bits of the event group and the inputs are in the
// lower 16 bits.
#define	sysEvtGroupMask(fd,output)	\
		(output ?  (0x00010000 << (fd)) : (0x00000001 << (fd)) )


/************************************************************
 * This structure defines the entire the Low Memory Area
 *  and includes the low memory header defined in Globals.h,
 *  followed by some variable size globals that may change 
 *  from rev to rev of the System Software.
 *
 * When emulating under Windows, this structure ends up at some random
 * place in memory and the global variable GlobalsP points to it.
 *
 * When running in the native environment, this structure starts at
 * address sysGlobalsOffset. 
 *
 * In order to simplify the source code, macros are defined for
 *  accessing commonly used globals in the optimim manner for
 *  each environment.
 *************************************************************/ 
#include <SerialLinkPrv.h>
#include <MemoryPrv.h>
#include <SystemPkt.h>
#include <DebugPrv.h>
#include <Hardware.h>
typedef struct LowMemType {


	// Fixed size globals, defined in Globals.h
	LowMemHdrType			fixed; 
	

	//---------------------------------------------------------------------------
	// These structures are always placed at the end of the LowMem area so
	// that other globals don't generally have to move when their sizes change
	//---------------------------------------------------------------------------
	CardInfoType			memCardInfo[hwrNumCardSlots];		// Memory Card Info Array
	
	SlkGlobalsType			slkGlobals;								// Serial Link Mgr globals
	
	DbgGlobalsType			dbgGlobals;								// Debugger globals
	
	SysAppInfoType			sysAppInfo;								// SysAppInfoType for system
	
	Ptr						sysDispatchTable[sysNumTraps];	// trap table
	
	} LowMemType;
	

// Access to variable size globals
#define	GMemCardInfo				(((LowMemType*)PilotGlobalsP)->memCardInfo)
#define	GSlkGlobals					(((LowMemType*)PilotGlobalsP)->slkGlobals)
#define	GDbgGlobals					(((LowMemType*)PilotGlobalsP)->dbgGlobals)
#define	GSysAppInfo					(((LowMemType*)PilotGlobalsP)->sysAppInfo)
#define	GSysLibTable			  	(((LowMemType*)PilotGlobalsP)->sysLibTable)
#define	GSysDispatchTable		  	(((LowMemType*)PilotGlobalsP)->sysDispatchTable)


// Flags for the GSysResetFlags global
#define	sysResetFlagHardReset			0x01					// HardReset
#define	sysResetFlagCreateDefaultDBs	0x02					// create default databases
#define	sysResetFlagNoExtensions		0x04					// don't execute extensions


// Flags for the GSysPrefFlags global
#define	sysPrefFlagTaxiDisable			0x0001				// Not used in >1.0 ROM.
#define	sysPrefFlagEnableEasterEggs	0x0002				// Enable easter eggs
#define	sysPrefFlagTaxiDisIdle			0x8000				// Disable Idle Taxi temporarily, 
																			//  (not saved in preferences)
#define	sysPrefFlagTaxiDisIdleTime		0x4000				// Copy of the 16th bit of the system
																			//  time when sysPrefFlagTaxiDisIdle bit
																			//	 was set (not saved in preferences).


// Flags for the GSysMiscFlags global
#define	sysMiscFlagInFatalAlert			0x0001				// Currently displaying fatal alert
#define	sysMiscFlagAlwaysSwitchApp		0x0002				// Inform UI App Shell to switch
																			//  apps even if already running
#define	sysMiscFlagProfile				0x0004				// true when profiling code. 
#define	sysMiscFlagGrfDisable			0x0008				// true when Graffiti is disabled
#define	sysMiscFlagInDemoAlert			0x0010				// true when Demo unit alert is displayed
#define 	sysMiscFlagBacklightDisable	0x0020				// true if backlight is disabled
#define 	sysMiscFlagUIInitialized		0x0040				// true after UI has been inited
#define 	sysMiscFlagExgEvent				0x0080				// true when a library (Ir) has data


// Flags for the GDbgWasEntered global

// dbgEnteredFlagPseudoNMI: manually setting this flag in the GDbgWasEntered low mem global
// causes the HotSync cradle button IRQ handler to enter the debugger.  Have to
// set each time because entering the debugger resets the flag. (this is a hack)
#define dbgEnteredFlagPseudoNMI		0x80



/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

							
SysTCBUserInfoPtr	SysTaskUserInfoPtr(ULong taskID)
							SYS_TRAP(sysTrapSysTaskUserInfoPtr);
							
Err		SysAppStartup(SysAppInfoPtr* appInfoPP, Ptr* prevGlobalsP, 
							Ptr* globalsPtrP)
							SYS_TRAP(sysTrapSysAppStartup);
							
Err		SysAppExit(SysAppInfoPtr appInfoP, Ptr prevGlobalsP, Ptr globalsP)
							SYS_TRAP(sysTrapSysAppExit);
							
SysAppInfoPtr 	SysCurAppInfoPV20(void)
							SYS_TRAP(sysTrapSysCurAppInfoPV20);
							
SysAppInfoPtr 	SysGetAppInfo(SysAppInfoPtr *uiAppPP, SysAppInfoPtr *actionCodeAppPP)
							SYS_TRAP(sysTrapSysGetAppInfo);
							

#ifdef __cplusplus 
}
#endif

/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_SysFatalAlert		\
				ASM_SYS_TRAP(sysTrapSysFatalAlert)

#define	_SysReset		\
				ASM_SYS_TRAP(sysTrapSysReset)



#endif  //__SYSTEMPRV_H__

#endif // NON_PORTABLE

