/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		Globals.h
 *
 * Description:
 *		Structure definition of Pilot globals
 *
 * History:
 *   	10/26/94 RM		Created by Ron Marianetti
 *		 4/13/95	VMK	Added Sound Manager globals
 *		 8/15/97	vmk	Reused padGlobalsP for sndOffTicks
 *		 1/18/98	SCL	Removed lastActionCodeAppInfoP, added irq globals.
 *  	 3/13/98	srj	Added new battery subsystem
 *   	 6/22/98	srj	Added new global for software shutdown
 *     8/17/98	SCL	Synchronized globals between Sumo/Eleven
 *		 8/26/98	kwk	Deleted unused FEP globals pointer.
 *     8/28/98	SCL	Synchronized globals between Sumo/Eleven
 *    10/28/98	SCL	Synchronized globals between Sumo/Eleven
 *
 *******************************************************************/

#ifdef	NON_PORTABLE									// So app's don't mistakenly include this

#ifndef 	__GLOBALS_H__
#define	__GLOBALS_H__

#include <M68KHwr.h>

// The following indicates how much space in low memory we've reserved for
//  the UI globals. The UI initialization code makes sure that we don't
//  overrun this space by comparing the size of the structure UIGlobalsType
//  with this constant.

// Changing this will cause globals after uiGlobals to move including flpSoftFPSCR.
// See note below regarding flpSoftFPSCR and __SoftFPSCR__ in mc68ksfpe.s.
#define	sysUIRsvGlobalsSize			0xB0



/************************************************************
 * Structure of Fixed System Globals
 *************************************************************/
typedef struct FixedGlobalsType {

	// Memory Manager Globals
	Byte						memCardSlots;							// number of memory card slots
	Byte						dbgWasEntered;							// set true the first time debugger is entered
	Ptr						memCardInfoP;							// pointer to array of CardInfoTypes stored
	DWord						memSemaphoreID;						// Memory Manager semaphore
	Word						memDebugMode;							// Current debug mode of Memory Manager

	// Data Manager Globals
	Ptr						dmOpenList;								// Linked list of open databases (DmOpenInfoPtr)
	
	// Debugger/System Shared globals
	Byte						dbgInDebugger;							// true if in debugger
	Byte						dbgTracing;								// tracing in debugger
	Ptr						dbgGlobalsP;							// pointer to dbgGlobals
	Ptr						dbgSerGlobalsP;						// pointer to Debugger Serial globals
	
	
	// System Manager Globals
	Ptr						sysAppInfoP;							// pointer to SysAppInfoType for system
	Ptr						sysKernelDataP;						// pointer to Kernel data
	Ptr*						sysDispatchTableP;					// pointer to dispatch table
	DWord						sysOwnerIDsInUse;						// 1 bit for each owner ID in use
	Ptr						sysAMXAppInfoP;						// SysAppInfoType for AMX app.
	DWord						sysClockFreq;							// processor clock frequency
	DWordPtr					sysHardKeyCreators;					// ptr to array of App Creator types

	SWord						sysBatteryCheckTimer;				// # of ticks left before we check battery again
	Byte						sysBatteryMinThreshold;				// System will shut down if battery falls below this
	Byte						hwrBatteryLevel;						// current battery level
	SWord						sysNextBatteryAlertTimer;			// # of ticks till next battery warning (OBSOLETE, See Below)
	Byte						sysBatteryWarnThreshold;			// If battery below this level, post low battery keyboard event

	Byte						sysDispatchTableRev;					// incremented every time a trap is patched. Used
																			//  by host debugger to invalidate it's cache.
	Word						sysDispatchTableSize;				// # of entries in dispatch table
	Ptr						sysLibTableP;							// (SysLibTblEntryPtr) pointer to Library entries
	Word						sysLibTableEntries;					// size of library table
	Ptr						sysConsoleStackChunkP;				// pointer to console task stack chunk
	Ptr						sysUIShellAppInfoP;					// AppInfoPtr for UIAppShell
	
	DWord						sysTimerID;								// TimerID of timer used for periodic system stuff
	ULong						sysAutoOffEvtTicks;					// Ticks of last event - used to support auto-off
	Word						sysAutoOffSeconds;					// Auto-sleep timeout in seconds.
	
	Long						sysRandomSeed;							// Random # seed 
	
	
	
	// Other Manager's globals
	Ptr						slkGlobalsP;							// pointer to serial link globals
	Ptr						serGlobalsP;							// pointer to serial manager globals
	Ptr						scrGlobalsP;							// pointer to screen driver globals
	Ptr						fplGlobalsP;							// pointer to Floating Point Library globals
	Ptr						penGlobalsP;							// Pen manager globals ptr
	Ptr						sysEvtMgrGlobalsP;					// System Event Manager Globals
	Ptr						sndGlobalsP;							// Sound Manager globals
	Ptr						timGlobalsP;							// Time Manager globals
	Ptr						almGlobalsP;							// Alarm Manager globals
	Ptr						ftrGlobalsP;							// pointer to feature manager globals																		
	Ptr						grfGlobalsP;							// pointer to Graffiti globals
	Ptr						keyGlobalsP;							// Key manager globals
	
	// UI Globals
	Byte						uiGlobals[sysUIRsvGlobalsSize];	// Basic UI Globals	
	Ptr						uiExtensionsP;							// for future use...																	 


	// Globals used by UIAppShell in order to manage the launching of UI Apps. 
#if EMULATION_LEVEL != EMULATION_NONE
	Ptr						curUIAppInfoP;							// SysAppInfoPtr of current UI App or
																			// last action code
#else
	Ptr						unusedPtr;
#endif

	DWord						nextUIAppDBID;							// Database LocalID of next UI App to launch 
	Word						nextUIAppCmd;							// Cmd for next UI App
	Ptr						nextUIAppCmdPBP;						// Cmd ParamBlock Ptr for next UI App
	Byte						nextUIAppCardNo;						// Card number of next UI App to launch
	
	
	// Hardware Manager Globals
	SByte						hwrDataWELevel;						// if > 0, Data area is write enabled
	Word						hwrWakeUp;								// Bit mask of which peripherals need to be 
																			//  enabled when system wakes
	Byte						hwrCPUDutyCycle;						// Desired Duty cycle of CPU in 31ths.
	Byte						hwrPenDown;								// last state of pen-down (zero = up
																			//   non-zero = down, number of ticks 'till next sample)
	DWord						hwrCurTicks;							// Current hardware tickCount - 
																			//  maintained separately from AMX ticks for performance reasons.
	DWord						hwrTotalRAMSize;						// total RAM Size, used by HwrMemWritable
	DWord						hwrDozeSubTicks;						// # of subTicks we've dozed for since reset
																			// a sub-tick is 1/4096 seconds.
	
	// Sound Manager "sound off" ticks
	// WAS: Pad server globals pointer
	// WAS: Ptr						padGlobalsP;							// PAD Server globals
	DWord						sndOffTicks;							// # of ticks til disabling the PWM (0 = do nothing)

	// New System globals
	Byte						sysResetFlags;							// Reset flags sysResetFlagXXXX
	Byte						sysBatteryKind;						// SysBatteryKind enumerated type

	// New Memory Manager global for recording minimum heap free - used for debugging only
	DWord						memMinDynHeapFree;					// Minimum free space in dynamic heap
	
	
	// System Preferences flags
	Word						sysPrefFlags;							// System Pref flags: sysPrefFlagXXXX
	
	
	// This is a pointer to the System Manager variable size globals
	Ptr						sysGlobalsP;							// pointer to System Globals 
	
	// Miscellaneous System flags
	Word						sysMiscFlags;							// System Misc flags: sysMiscFlagXXX
	
	
	//-----------------------------------------------------------------------
	// New Globals added for the 2.0 release...
	//-----------------------------------------------------------------------
	Word						sysLibNet;								// refnum of TCP/IP library
	VoidPtr					netPktFreeQ;							// global needed by Net Library
	

	// EventGroupID used by the System Event Manager for blocking on user input
	// and by the Net Library for implementing NetLibSelect()
	DWord						sysEvGroupID;							// AMX EventGroup ID.
	
	
	Ptr						irq3GlobalsP;							// IRQ3 handler globals
	
	// Tick count last time we displayed a low battery warning. This is used by
	// the system timer procedure in AMXHardwareTD1.c to insure that it doesn't
	// display low battery warnings more than once/minute. 
	DWord						sysLastBatteryWarning;				// tickcount of last warning. 
	
	// Checksum of exception vector area of low memory. Used when ROM built for full
	//  error checking to make sure no apps overwrite low memory by using null pointers.
	DWord						sysLowMemChecksum;					// checksum of low memory.

	// Hardware rev. This is determined during bootup time. The first shipping rev
	//  is 1.
	Word						hwrHardwareRev;						// hardware rev.
	
	
	// Handle to list of protected databases 
	Handle					dmProtectListH;						// handle to array of protected 
																			// databases (DmProtectEntryType)
	// Miscellaneous hardware flags
	Word						hwrMiscFlags;							// Hardware Misc Flags: hwrMiscFlagXXX
	
	// Support for profiling
	DWord						sysProfileRefcon;						// Refcon value for profiler function
	Ptr						sysProfileProcP;						// Profiler procedure, called if non-zero
																			//  during tick interrupt (100/sec).

	// NewFloatMgr's MWSFPE (MetroWerks Software Floating Point Emulator)
	// needs a global to keep track of its status and control.
	// This global (__SoftFPSCR__) is referenced by the 68K code located at
	// :Libraries:NewFloatMgr:mc68ksfpe.s
	DWord						flpSoftFPSCR;


	//-----------------------------------------------------------------------
	// New Globals added for the 3.0 release...
	//-----------------------------------------------------------------------
	Ptr						irLibGlobalsP;							// Ir Library globals pointer
	Word						exgActiveLib;							// active Exchange library

	Word						uiBusyCount;							// UI busy counter
	
	Ptr						irq1GlobalsP;							// IRQ1 handler globals
	Ptr						irq2GlobalsP;							// IRQ2 handler globals
	Ptr						irq6GlobalsP;							// IRQ6 handler globals


	//-----------------------------------------------------------------------
	// New Globals added for the 3.1 release...
	//-----------------------------------------------------------------------

	// Globals for Language support.
	Ptr						intlMgrGlobalsP;						// Int'l Manager globals

	// Globals for Field inline Text Services
	Word						inlineStart;							// Starting position of the inline text
	Word						inlineEnd;								// Ending position of the inline text
	Boolean					inlineActive;							// True if inline text support is active.
	Byte						inlineReservedFillerByte1;			// (for alignment)
	VoidPtr					inlineStatus;							// Ptr to FEP status record.
	Word						inlineLibRef;							// Text Services Library Reference number	
	Word						inlineHighlightLen;					// Length of highlighted inline text

	// <chg 3/13/98 srj> Added for the battery subsystem
	Ptr						sysBatteryDataP;						// pointer to current battery data
	Ptr						sysBatteryValidKindsP;				// pointer to list of valid batteries

	// <chg 6/22/98 srj> Added for the battery subsystem
	Boolean					sysInShutdown;							// True: We are in lockout (Sumo)
	Byte						sysMinWakeupThreshold;				// System can wake up once we're over this voltage

	// <chg 7/8/98 srj> More miscellaneous hardware flags
	DWord						hwrMiscFlagsExt;						// Hardware Misc Flags: hwrMiscFlagExtXXX
	
	// <chg 8/15/98 srj> Added to provide critical warning timer
	Boolean					sysUseCritWarnTimer;
	Boolean					sysCritWarnTimerRunning;
	SWord						sysCritWarnRTCDayStarted;
	SDWord					sysCritWarnTickTimer;
	DWord						sysCritWarnLastTick;
	Boolean					sysCharging;
	Boolean					sysUpdateVoltageLevel;

	// Notification Manager globals:
	Handle					notifyGlobalsH;			// handle to NotifyMgr globals
	
	// Some globals reserved for future use (only available in 3.1 or later):
	DWord						sysReserved31DWord1;
	DWord						sysReserved31DWord2;
	DWord						sysReserved31DWord3;
	DWord						sysReserved31DWord4;

	//-----------------------------------------------------------------------
	// Put New globals here...
	//-----------------------------------------------------------------------
	// type 					newGlobal;

	} FixedGlobalsType;
	
		
/************************************************************ 
 * This structure defines the start of the Low Memory Area.
 * The complete structure of LowMemory is defined in SystemPrv.h
 *  and includes this low memory header followed by some variable
 *  size globals that may change from rev to rev of the System Software.
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
typedef struct LowMemHdrType {
	//---------------------------------------------------------------------------
	// Processor exception table
	//---------------------------------------------------------------------------
	M68KExcTableType		vectors;						// Exception vectors, 
																// or Card header for RAM only cards
	
	//---------------------------------------------------------------------------
	// System Globals
	//---------------------------------------------------------------------------
	FixedGlobalsType		globals;						// System Globals
	
	} LowMemHdrType;



#if EMULATION_LEVEL == EMULATION_NONE
	#define	PilotGlobalsP	0
#else
	extern	Ptr	PilotGlobalsP;							// pointer to start of Memory card 0
#endif    





/************************************************************
 * Macros for accessing globals. 
 * When emulating, the globals are allocated in a memory chunk and our
 *  global variable Card0P points to this chunk.
 * In native mode, Card0P is defined to be 0 at all times.
 *************************************************************/
#define	GMemCardSlots				(((LowMemHdrType*)PilotGlobalsP)->globals.memCardSlots)
#define	GMemCardInfoP				(((LowMemHdrType*)PilotGlobalsP)->globals.memCardInfoP)
#define	GMemSemaphoreID			(((LowMemHdrType*)PilotGlobalsP)->globals.memSemaphoreID)
#define  GMemDebugMode				(((LowMemHdrType*)PilotGlobalsP)->globals.memDebugMode)
#define  GMemMinDynHeapFree		(((LowMemHdrType*)PilotGlobalsP)->globals.memMinDynHeapFree)

#define	GDmOpenList		  			(((LowMemHdrType*)PilotGlobalsP)->globals.dmOpenList)
#define	GDmProtectListH 			(((LowMemHdrType*)PilotGlobalsP)->globals.dmProtectListH)

#define	GDbgWasEntered	  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgWasEntered)
#define	GDbgInDebugger	  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgInDebugger)
#define	GDbgTracing		  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgTracing)
#define	GDbgGlobalsP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgGlobalsP)

#define	GSysAppInfoP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.sysAppInfoP)
#define	GSysKernelDataP	  		(((LowMemHdrType*)PilotGlobalsP)->globals.sysKernelDataP)
#define	GSysDispatchTableP	  	(((LowMemHdrType*)PilotGlobalsP)->globals.sysDispatchTableP)
#define	GSysDispatchTableSize	(((LowMemHdrType*)PilotGlobalsP)->globals.sysDispatchTableSize)
#define	GSysDispatchTableRev		(((LowMemHdrType*)PilotGlobalsP)->globals.sysDispatchTableRev)
#define	GSysOwnerIDsInUse		  	(((LowMemHdrType*)PilotGlobalsP)->globals.sysOwnerIDsInUse)
#define	GSysAMXAppInfoP			(((LowMemHdrType*)PilotGlobalsP)->globals.sysAMXAppInfoP)
#define	GSysClockFreq				(((LowMemHdrType*)PilotGlobalsP)->globals.sysClockFreq)
#define  GSysHardKeyCreators		(((LowMemHdrType*)PilotGlobalsP)->globals.sysHardKeyCreators)
#define  GSysNextBatteryAlertTimer		(((LowMemHdrType*)PilotGlobalsP)->globals.sysNextBatteryAlertTimer)
#define  GSysBatteryCheckTimer	(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryCheckTimer)
#define  GSysBatteryWarnThreshold		(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryWarnThreshold)
#define  GHwrBatteryLevel			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrBatteryLevel)
#define  GSysBatteryMinThreshold		(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryMinThreshold)
#define  GSysBatteryKind			(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryKind)
#define  GSysConsoleStackChunkP	(((LowMemHdrType*)PilotGlobalsP)->globals.sysConsoleStackChunkP)
#define  GSysUIShellAppInfoP		(((LowMemHdrType*)PilotGlobalsP)->globals.sysUIShellAppInfoP)
#define  GSysTimerID					(((LowMemHdrType*)PilotGlobalsP)->globals.sysTimerID)
#define  GSysAutoOffEvtTicks		(((LowMemHdrType*)PilotGlobalsP)->globals.sysAutoOffEvtTicks)
#define  GSysAutoOffSeconds		(((LowMemHdrType*)PilotGlobalsP)->globals.sysAutoOffSeconds)
#define  GSysRandomSeed				(((LowMemHdrType*)PilotGlobalsP)->globals.sysRandomSeed)
#define  GSysResetFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.sysResetFlags)
#define  GSysPrefFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.sysPrefFlags)
#define  GSysMiscFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.sysMiscFlags)
#define  GSysGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.sysGlobalsP)
#define  GSysLibNet					(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibNet)
#define  GSysEvGroupID				(((LowMemHdrType*)PilotGlobalsP)->globals.sysEvGroupID)
#define  GSysLastBatteryWarning	(((LowMemHdrType*)PilotGlobalsP)->globals.sysLastBatteryWarning)
#define  GSysLowMemChecksum		(((LowMemHdrType*)PilotGlobalsP)->globals.sysLowMemChecksum)
#define  GSysProfileRefcon			(((LowMemHdrType*)PilotGlobalsP)->globals.sysProfileRefcon)
#define  GSysProfileProcP			(((LowMemHdrType*)PilotGlobalsP)->globals.sysProfileProcP)

#define  GNetPktFreeQ				(((LowMemHdrType*)PilotGlobalsP)->globals.netPktFreeQ) 


#define	GSlkGlobalsP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.slkGlobalsP)
#define	GSerGlobalsP	  			(((LowMemHdrType*)PilotGlobalsP)->globals.serGlobalsP)
#define	GSer681GlobalsP	  		(((LowMemHdrType*)PilotGlobalsP)->globals.irq3GlobalsP)
#define	GScrGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.scrGlobalsP)
#define	GFplGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.fplGlobalsP)
#define	GPenGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.penGlobalsP)
#define	GSysEvtMgrGlobalsP		(((LowMemHdrType*)PilotGlobalsP)->globals.sysEvtMgrGlobalsP)
#define	GSndGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.sndGlobalsP)
#define	GTimGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.timGlobalsP)
#define	GAlmGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.almGlobalsP)
#define	GFtrGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.ftrGlobalsP)
#define	GGrfGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.grfGlobalsP)
#define	GKeyGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.keyGlobalsP)
//#define	GPADGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.padGlobalsP) // not used vmk 8/15/97

#define	GSndOffTicks				(((LowMemHdrType*)PilotGlobalsP)->globals.sndOffTicks)

#define	GUIGlobalsP					(((LowMemHdrType*)PilotGlobalsP)->globals.uiGlobals)

#define	GNextUIAppDBID				(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppDBID)
#define	GNextUIAppCardNo			(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCardNo)
#define	GNextUIAppCmd				(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCmd)
#define	GNextUIAppCmdPBP			(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCmdPBP)
#if EMULATION_LEVEL != EMULATION_NONE
#define	GCurUIAppInfoP				(((LowMemHdrType*)PilotGlobalsP)->globals.curUIAppInfoP)
#endif

#define	GHwrDataWELevel			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrDataWELevel)
#define  GHwrWakeUp					(((LowMemHdrType*)PilotGlobalsP)->globals.hwrWakeUp)
#define  GHwrCPUDutyCycle			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrCPUDutyCycle)
#define  GHwrPenDown					(((LowMemHdrType*)PilotGlobalsP)->globals.hwrPenDown)
#define  GHwrCurTicks				(((LowMemHdrType*)PilotGlobalsP)->globals.hwrCurTicks)
#define  GHwrTotalRAMSize			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrTotalRAMSize)
#define  GHwrDozeSubTicks			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrDozeSubTicks)
#define  GHwrHardwareRev			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrHardwareRev)
#define  GHwrMiscFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.hwrMiscFlags)
#define	GHwrMiscFlagsExt			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrMiscFlagsExt)

#define  GSysLibTableP				(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibTableP)
#define  GSysLibTableEntries		(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibTableEntries)
#define	GDbgSerGlobalsP			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgSerGlobalsP)

#define  GIrLibGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irLibGlobalsP)
#define	GExgActiveLib				(((LowMemHdrType*)PilotGlobalsP)->globals.exgActiveLib)
#define	GSysUIBusyCount			(((LowMemHdrType*)PilotGlobalsP)->globals.uiBusyCount)

#define	GIrq1GlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irq1GlobalsP)
#define	GIrq2GlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irq2GlobalsP)
#define	GIrq3GlobalsP		  		(((LowMemHdrType*)PilotGlobalsP)->globals.irq3GlobalsP)
#define	GIrq6GlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.irq6GlobalsP)

#define	GIntlMgrGlobalsP			(((LowMemHdrType*)PilotGlobalsP)->globals.intlMgrGlobalsP)

#define	GInlineStart				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineStart)
#define	GInlineEnd					(((LowMemHdrType*)PilotGlobalsP)->globals.inlineEnd)
#define	GInlineActive				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineActive)
#define	GInlineStatus				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineStatus)
#define	GInlineLibRef				(((LowMemHdrType*)PilotGlobalsP)->globals.inlineLibRef)
#define	GInlineHighlightLen		(((LowMemHdrType*)PilotGlobalsP)->globals.inlineHighlightLen)

#define	GSysBatteryDataP			(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryDataP)
#define	GSysBatteryValidKindsP	(((LowMemHdrType*)PilotGlobalsP)->globals.sysBatteryValidKindsP)
#define	GSysInShutdown				(((LowMemHdrType*)PilotGlobalsP)->globals.sysInShutdown)
#define	GSysMinWakeupThreshold	(((LowMemHdrType*)PilotGlobalsP)->globals.sysMinWakeupThreshold)

#define	GSysUseCritWarnTimer			(((LowMemHdrType*)PilotGlobalsP)->globals.sysUseCritWarnTimer)
#define	GSysCritWarnTimerRunning	(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnTimerRunning)
#define	GSysCritWarnRTCDayStarted	(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnRTCDayStarted)
#define	GSysCritWarnTickTimer		(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnTickTimer)
#define	GSysCritWarnLastTick    	(((LowMemHdrType*)PilotGlobalsP)->globals.sysCritWarnLastTick)
#define	GSysCharging				(((LowMemHdrType*)PilotGlobalsP)->globals.sysCharging)
#define	GSysUpdateVoltageLevel	(((LowMemHdrType*)PilotGlobalsP)->globals.sysUpdateVoltageLevel)

#define	GSysNotifyGlobalsH		(((LowMemHdrType*)PilotGlobalsP)->globals.notifyGlobalsH)


#endif 	//__GLOBALS_H__

#endif 	// NON_PORTABLE
