/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DebugPrv.h
 *
 * Description:
 *		Privat®Debugging functions
 *
 * History:
 *   	8/10/95 RM - Created by Ron Marianetti
 *
 *
 *******************************************************************/

#ifdef	NON_PORTABLE
#ifndef __DEBUGPRV_H__
#define __DEBUGPRV_H__


/***********************************************************************
 * Debugger Constants
 ***********************************************************************/
#define  dbgLoopsPerTick		0x0800
#define	dbgStackSize			0x0300			// DOLATER -- resize???

#define	dbgNormalBreakpoints	5
#define	dbgTempBPIndex			dbgNormalBreakpoints
#define	dbgTotalBreakpoints	(dbgTempBPIndex+1)

#define	dbgFlashCodeSpace		sysInitStack	// where we copy flash code
#define	dbgFlashCodeSize		0x200				// size of flash code

#define	dbgTotalTrapBreaks	5					// up to this many trap breaks at a time




/***********************************************************************
 * Structure of Debugger Breakpoints
 ***********************************************************************/
typedef struct BreakpointType {
	Ptr		addr;									// address of breakpoint
	Boolean	enabled;								// true if enabled
	Boolean	installed;							// for alignment
	} BreakpointType;





/***********************************************************************
 * Structure of Debugger Specific System Packet Type Bodies.
 ***********************************************************************/
 
#define			sysPktMaxNameLen		32		// Max length of a routine name 
 
//--------------------------------------------------------------
// sysPktStateCmd 
// - get processor state
//------------------------------------------------------------------
// number of remote code words to send in the 'state response' packet
#define sysPktStateRspInstWords	15


typedef struct SysPktStateCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktStateCmdCmdType;
typedef SysPktStateCmdCmdType*	SysPktStateCmdCmdPtr;


// Packet Body structure for the state command response packet
typedef struct SysPktStateRspType {
	_sysPktBodyCommon;								// Common Body header
	Boolean						resetted;			// true if target has just reset
	Word							exceptionId;		// exception which caused the
															// debugger to be entered.
	M68KRegsType				reg;					// current remote registers.
	Word							inst[sysPktStateRspInstWords];
															// instruction buffer for
															// code beginning at PC.
	BreakpointType				bp[dbgTotalBreakpoints];	// current breakpoints

	void*							startAddr;			// start address of routine
	void*							endAddr;				// end address of routine
	char							name[sysPktMaxNameLen];
															// routine name (0 or more chars,
															// no null), immediately
															// follows the address range.
	Byte							trapTableRev;		// rev of trap table. Used to determine
															//  when host's trap table cache is invalid
	} SysPktStateRspType;
typedef SysPktStateRspType*	SysPktStateRspPtr;

//--------------------------------------------------------------------
// sysPktGetRtnNameCmd
//  - get routine name
//--------------------------------------------------------------------
// The response packet body will contain start address(4 bytes),
// end address(4 bytes), followed by 0 or more bytes of the routine
// name (no null byte).
typedef struct SysPktRtnNameCmdType {
	_sysPktBodyCommon;								// Common Body header
	void*							address;			// -> address to query on
	} SysPktRtnNameCmdType;
typedef SysPktRtnNameCmdType*	SysPktRtnNameCmdPtr;

typedef struct SysPktRtnNameRspType {
	_sysPktBodyCommon;							// Common Body header
	void*							address;			// -> address to query on
	void*							startAddr;		// <- start address of routine
	void*							endAddr;			// <- end address of routine
	char							name[sysPktMaxNameLen];   		// <- routine name, if any immediately
														//    follows the address range.
														//    The balance need not be sent.
	} SysPktRtnNameRspType;
typedef SysPktRtnNameRspType*	SysPktRtnNameRspPtr;


//--------------------------------------------------------------------
// sysPktReadRegsCmd
// - read registers
//--------------------------------------------------------------------
// Packet Body structure for the read regs command response packet
typedef	struct SysPktReadRegsCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktReadRegsCmdType;
typedef SysPktReadRegsCmdType* 	SysPktReadRegsCmdPtr;

typedef	struct SysPktReadRegsRspType {
	_sysPktBodyCommon;								// Common Body header
	//M68KRegsType			reg;						// <- return registers
	} SysPktReadRegsRspType;
typedef SysPktReadRegsRspType* 	SysPktReadRegsRspPtr;



//--------------------------------------------------------------------
// sysPktWriteRegsCmd
// - write registers
//--------------------------------------------------------------------
// Packet Body structure for the write regs command packet
typedef	struct SysPktWriteRegsCmdType {
	_sysPktBodyCommon;								// Common Body header
	M68KRegsType		reg;							// -> registers to write
	} SysPktWriteRegsCmdType;
typedef SysPktWriteRegsCmdType* 	SysPktWriteRegsCmdPtr;


// Packet Body structure for the write regs command packet
typedef	struct SysPktWriteRegsRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktWriteRegsRspType;
typedef SysPktWriteRegsRspType* 	SysPktWriteRegsRspPtr;


//--------------------------------------------------------------------
// sysPktContinueCmd
// -  continue
//--------------------------------------------------------------------
// Packet Body structure for the continue command packet
typedef struct SysPktContinueCmdType {
	_sysPktBodyCommon;									// Common Body header
	M68KRegsType	 		regs;							// registers
	Boolean					stepSpy;						// set true to do step spy
	DWord						ssAddr;						// step spy address
	DWord						ssCount;						// # of bytes
	DWord						ssCheckSum;					// checksum
	} SysPktContinueCmdType;

typedef SysPktContinueCmdType* SysPktContinueCmdPtr;


//--------------------------------------------------------------------
// sysPktGetBreakPointsCmd
//
// - Get breakpoints
//--------------------------------------------------------------------
typedef struct SysPktGetBreakpointsCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktGetBreakpointsCmdType;
typedef SysPktGetBreakpointsCmdType*	SysPktGetBreakpointsCmdPtr;

typedef struct SysPktGetBreakpointsRspType {
	_sysPktBodyCommon;								// Common Body header
	//BreakpointType	bp[dbgTotalBreakpoints];
	} SysPktGetBreakpointsRspType;
typedef SysPktGetBreakpointsRspType*	SysPktGetBreakpointsRspPtr;


//--------------------------------------------------------------------
// sysPktSetBreakPointsCmd
//
// - Set breakpoints
//--------------------------------------------------------------------
typedef struct SysPktSetBreakpointsCmdType {
	_sysPktBodyCommon;								// Common Body header
	//BreakpointType	bp[dbgTotalBreakpoints];
	} SysPktSetBreakpointsCmdType;
typedef SysPktSetBreakpointsCmdType*	SysPktSetBreakpointsCmdPtr;

typedef struct SysPktSetBreakpointsRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktSetBreakpointsRspType;
typedef SysPktSetBreakpointsRspType*	SysPktSetBreakpointsRspPtr;




//--------------------------------------------------------------------
// sysPktDbgBreakToggleCmd
// - Enable/Disable DbgBreak's command
//--------------------------------------------------------------------
typedef struct SysPktDbgBreakToggleCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktDbgBreakToggleCmdType;
typedef SysPktDbgBreakToggleCmdType*	SysPktDbgBreakToggleCmdPtr;

typedef struct SysPktDbgBreakToggleRspType {
	_sysPktBodyCommon;								// Common Body header
	Boolean		newState;
	} SysPktDbgBreakToggleRspType;
typedef SysPktDbgBreakToggleRspType*	SysPktDbgBreakToggleRspPtr;


//--------------------------------------------------------------------
// sysPktFlashCmd
// Program Flash command - programs one sector of the FLASH ram
// If numBytes is 0, this routine returns info on the flash in:
//   manuf - manufacturer code
//   device - device code
//   width - width of FLASH
//   ramSpaceP - buffer in RAM for flash sector
//--------------------------------------------------------------------
typedef struct SysPktFlashWriteType {
	_sysPktBodyCommon;								// Common Body header
	Word			numBytes;							// -> # of bytes in sector
	DWord			flashBase;							// -> base address of FLASH, or 0 for default
	DWord			srcAddr;								// -> source address of new data
	DWord			dstAddr;								// -> dest address in flash memory
	Byte			manuf;								// <- manufacturer
	Byte			device;								// <- device
	Byte			width;								// <- Flash memory width 
	Byte			filler;
	DWord			ramSpaceP;							// <-  available space in RAM for sector
	} SysPktFlashWriteType;
typedef SysPktFlashWriteType* SysPktFlashWritePtr;


//--------------------------------------------------------------------
// sysPktGetTrapBreaksCmd
//
// - Get Trap Breaks
//--------------------------------------------------------------------
typedef struct SysPktGetTrapBreaksCmdType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktGetTrapBreaksCmdType;
typedef SysPktGetTrapBreaksCmdType*	SysPktGetTrapBreaksCmdPtr;

typedef struct SysPktGetTrapBreaksRspType {
	_sysPktBodyCommon;								// Common Body header
	//Word		trapBP[dbgTotalTrapBreaks];
	} SysPktGetTrapBreaksRspType;
typedef SysPktGetTrapBreaksRspType*	SysPktGetTrapBreaksRspPtr;


//--------------------------------------------------------------------
// sysPktSetTrapBreaksCmd
//
// - Set Trap Breaks
//--------------------------------------------------------------------
typedef struct SysPktSetTrapBreaksCmdType {
	_sysPktBodyCommon;								// Common Body header
	//Word		trapBP[dbgTotalTrapBreaks];
	} SysPktSetTrapBreaksCmdType;
typedef SysPktSetTrapBreaksCmdType*	SysPktSetTrapBreaksCmdPtr;

typedef struct SysPktSetTrapBreaksRspType {
	_sysPktBodyCommon;								// Common Body header
	} SysPktSetTrapBreaksRspType;
typedef SysPktSetTrapBreaksRspType*	SysPktSetTrapBreaksRspPtr;





// The following indicates how much space in debugger globals we've reserved for
// the SerialLibrary globals for the Debugger.  Reserve 8 extra bytes for
// patching.
#define	dbgSerRsvGlobalsSize			56




/***********************************************************************
 * Stucture of Debugger Globals
 ***********************************************************************/
struct DbgGlobalsType {
	// Saved registers
	M68KRegsType	reg;
	
	// Breakpoints and saved opcodes behind each one
	Boolean			romBP;
	Boolean			stepOverBP;							// true if single-stepping over a BP
	BreakpointType	bp[dbgTotalBreakpoints];
	Word				bpOpcode[dbgTotalBreakpoints];
	
	// Current trap breaks
	Word				trapBreak[dbgTotalTrapBreaks];
	DWord				oldDispatcher;						// saved trap dispatcher
	Boolean			breakingOnATrap;					// set by debugger trap dispatcher
	
	// Mode settings
	Boolean			ignoreDbgBreaks;					// if true, ignore DbgBreak's
	Boolean			commOpened;							// if true, comm library and socket have been
																//  opened.
	
	// Step spy support
	Boolean			stepSpy;								// true if step spying.
	DWord				ssAddr;								// address to step spy on 
	DWord				ssValue;								// saved value
	
	// Exception type
	Word				excType;								// why we entered debugger
	Byte				reEntered;							// true if we re-entered
	Byte				firstEntrance;						// true first time we enter debugger
	
	// Debugger Serial Library globals
	Byte				serGlobals[dbgSerRsvGlobalsSize];	// Debugger Serial globals
	
	// Debugger Stack
	Byte				stack[dbgStackSize];				// Debugger stack
	Word				stackEnd;							// End of stack
	
	};
	
typedef struct DbgGlobalsType DbgGlobalsType;
	
typedef DbgGlobalsType*		DbgGlobalsPtr;



#endif // __DEBUGMGR_H__
#endif // NON_PORTABLE

