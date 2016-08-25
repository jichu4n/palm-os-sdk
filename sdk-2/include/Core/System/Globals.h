/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		globals.h
 *
 * Description:
 *		Structure definition of Pilot globals
 *
 * History:
 *   	10/26/94  RM - Created by Ron Marianetti
 *		4/13/95		VMK		Added Sound Manager globals
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
	SWord						sysNextBatteryAlertTimer;			// # of ticks till next battery warning
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
	Ptr						curUIAppInfoP;							// SysAppInfoPtr of current UI App
	DWord						nextUIAppDBID;							// Database LocalID of next UI App to launch 
	Word						nextUIAppCmd;							// Cmd for next UI App
	Ptr						nextUIAppCmdPBP;						// Cmd ParamBlock Ptr for next UI App
	Byte						nextUIAppCardNo;						// Card number of next UI App to launch
	
	
	// Hardware Manager Globals
	SByte						hwrDataWELevel;						// if > 0, Data area is write enabled
	Word						hwrWakeUp;								// Bit mask of which peripherals need to be 
																			//  enabled when system wakes
	Byte						hwrCPUDutyCycle;						// Desired Duty cycle of CPU in 31ths.
	Byte						hwrPenDown;								// last state of pen-down
	DWord						hwrCurTicks;							// Current hardware tickCount - 
																			//  maintained separately from AMX ticks for performance reasons.
	DWord						hwrTotalRAMSize;						// total RAM Size, used by HwrMemWritable
	DWord						hwrDozeSubTicks;						// # of subTicks we've dozed for since reset
																			// a sub-tick is 1/4096 seconds.
	
	// Pad server
	Ptr						padGlobalsP;							// PAD Server globals

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
	// New Globals added after 1.0 release...
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
	// Pilot:Libraries:NewFloatMgr:mc68ksfpe.s
	DWord						flpSoftFPSCR;

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
#define	GPADGlobalsP				(((LowMemHdrType*)PilotGlobalsP)->globals.padGlobalsP)

#define	GUIGlobalsP					(((LowMemHdrType*)PilotGlobalsP)->globals.uiGlobals)

#define	GNextUIAppDBID				(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppDBID)
#define	GNextUIAppCardNo			(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCardNo)
#define	GNextUIAppCmd				(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCmd)
#define	GNextUIAppCmdPBP			(((LowMemHdrType*)PilotGlobalsP)->globals.nextUIAppCmdPBP)
#define	GCurUIAppInfoP				(((LowMemHdrType*)PilotGlobalsP)->globals.curUIAppInfoP)

#define	GHwrDataWELevel			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrDataWELevel)
#define  GHwrWakeUp					(((LowMemHdrType*)PilotGlobalsP)->globals.hwrWakeUp)
#define  GHwrCPUDutyCycle			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrCPUDutyCycle)
#define  GHwrPenDown					(((LowMemHdrType*)PilotGlobalsP)->globals.hwrPenDown)
#define  GHwrCurTicks				(((LowMemHdrType*)PilotGlobalsP)->globals.hwrCurTicks)
#define  GHwrTotalRAMSize			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrTotalRAMSize)
#define  GHwrDozeSubTicks			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrDozeSubTicks)
#define  GHwrHardwareRev			(((LowMemHdrType*)PilotGlobalsP)->globals.hwrHardwareRev)
#define  GHwrMiscFlags				(((LowMemHdrType*)PilotGlobalsP)->globals.hwrMiscFlags)

#define  GSysLibTableP				(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibTableP)
#define  GSysLibTableEntries		(((LowMemHdrType*)PilotGlobalsP)->globals.sysLibTableEntries)
#define	GDbgSerGlobalsP			(((LowMemHdrType*)PilotGlobalsP)->globals.dbgSerGlobalsP)


#endif 	//__GLOBALS_H__

#endif 	// NON_PORTABLE




