/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		Hardware.h
 *
 * Description:
 *		General Hardware Equates. This header file
 *  contains general hardware configuration equates that are not dependent on
 *  the version of the hardware we're running on. Equates specific to
 *  a certain component like the 68328, etc. is contained in separate
 *  header files.
 *
 * History:
 *   	04/27/95	RM		Created by Ron Marianetti
 *    02/04/98	srj	Added LCD contrast trap routine prototype
 *    05/05/98	art	Added button layout for Instant Karma
 *    08/17/98	SCL	Synchronized HwrMiscFlags between Sumo/Eleven
 *		09/07/98	kwk	Updated TSM button sizes.
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE	   // So app's don't mistakenly include this

#ifndef __HARDWARE_H__
#define __HARDWARE_H__



/**************************************************************************
 * General Equates
 ***************************************************************************/
 
// Bits in the low memory global GHwrWakeUp
#define	hwrWakeUpGeneral			0x0001
#define	hwrWakeUpPWM				0x0002
#define	hwrWakeUpFromKey			0x0004				// set if we woke from a key press
#define	hwrWakeUpReturnToSleep	0x0008				// set if we woke from 24 hr. interrupt
																	//  in which case we want to go right back to sleep


// Bits in the low memory global GHwrMiscFlags
#define	hwrMiscFlagHasBacklight		0x0001			// set if backlight is present
#define	hwrMiscFlagHasMbdIrDA		0x0002			// set if IrDA is present (on main board)
#define	hwrMiscFlagHasCardIrDA		0x0004			// set if IrDA is present (on memory card)
#define	hwrMiscFlagHasBurrBrown		0x0008			// set if BurrBrown A/D is present
#define	hwrMiscFlagHasJerryHW		0x0010			// set if Jerry Hardware is present
#define	hwrMiscFlagReserved1			0x0020			// reserved for Eleven
#define	hwrMiscFlagReserved2			0x0040			// reserved for Eleven
#define	hwrMiscFlagReserved3			0x0080			// reserved for Eleven
#define	hwrMiscFlagHasCradleDetect	0x0100			// set if we can detect the cradle
#define	hwrMiscFlagHasSWContrast	0x0200			// set if UI should support software contrast
#define	hwrMiscFlagInvertLCDForBL	0x0400			// set if we need to invert LCD w/Backlight
#define	hwrMiscFlagHasMiscFlagExt	0x0800			// set if we have new hwrMiscFlagsExt

			// The following bit flags are set by HwrIdentifyFeatures.
			// They allow software to read the hardware ID without poking at hardware.
			// They also provide some isolation from different ID detection schemes
			// such as if the ID detection mechanism should change with EZ...
#define	hwrMiscFlagID1				0x1000				// set if ID bit keyBitHard1 was set
#define	hwrMiscFlagID2				0x2000				// set if ID bit keyBitHard2 was set
#define	hwrMiscFlagID3				0x4000				// set if ID bit keyBitHard3 was set
#define	hwrMiscFlagID4				0x8000				// set if ID bit keyBitHard4 was set
#define	hwrMiscFlagIDMask			0xF000
#define	hwrMiscFlagIDOffset		12						// Bits to shift to get a numeric ID

#define	hwrMiscFlagIDSumo			(hwrMiscFlagID1 | hwrMiscFlagID2 | hwrMiscFlagID4)
#define	hwrMiscFlagIDBrad			(hwrMiscFlagID1 | hwrMiscFlagID3 | hwrMiscFlagID4)


// Bits in the low memory global GHwrMiscFlagsExt
#define	hwrMiscFlagExtSubID1		0x0001				// subtype ID (for feature select in dev.)
#define	hwrMiscFlagExtSubID2		0x0002				// subtype ID (for feature select in dev.)
#define	hwrMiscFlagExtSubID3		0x0004				// subtype ID (for feature select in dev.)
#define	hwrMiscFlagExtSubIDMask	0x0007				// sybtype ID Mask

#define	hwrMiscFlagExtHasLiIon	0x0010				// set if we have Lithium Ion battery
#define	hwrMiscFlagExtHasRailIO	0x0020				// set if we have Rail I/O hardware


// Bits for the call HwrDockSignals
#define	hwrDockInSyncButton	0x0001					// Dock button
#define	hwrDockInGeneric1		0x0002					// General Purpose input #1

#define	hwrDockOutGeneric0	0x0001					// General Purpose output #0

// Enum returned by hwrDockStatus() call
typedef enum {
		DockStatusNotDocked = 0,
		DockStatusInModem,
		DockStatusInCharger,
		DockStatusUnknown = 0xFF
		} hwrDockStatusState, *hwrDockStatusStateP;
		
// Define # of card slots
#if EMULATION_LEVEL == EMULATION_NONE
#define	hwrNumCardSlots	1								// 1 card slot
#else
#define	hwrNumCardSlots	2								// 2 card slots for emulation
#endif


// Screen Dimensions
#define	hwrDisplayWidth		160					// Width of display
#define	hwrDisplayHeight		160					// Height of display

// Screen depth info
#define	hwrDisplayBootDepth	1						// Boot time bits per pixel (system default)
#define	hwrDisplayMaxDepth	2						// Maximum bits per pixel hardware supports
#define	hwrDisplayAllDepths	0x0003				// All display depths (in bit map format)

// Screen refresh rate
#define	hwrDisplayPeriod		12						// frame period in ms.

//----------------------------------------------------------------------------
// Digitizer Dimensions
//----------------------------------------------------------------------------
#define	hwrSystemTop			(hwrDisplayHeight+4)
#define	hwrSystemHeight		56


// If the device supports a Text Services Managers then the are 
// addition buttons in the silk screen area, between the left-most app buttons
// and the left edge of the Graffiti alpha area.
#if TEXT_SERVICES == TEXT_SERVICES_ON

	#define	hwrBtnCount					8
	#define	hwrBtnWidth					20
	#define	hwrGraffitiAlphaWidth	50
	#define	hwrTSMBtnWidth				22
	#define	hwrTSMBtnHeight			16				// Std. height...last button is 1/2 this.
	#define	hwrCalcChr					keyboardChr

#else

	#define	hwrBtnCount					4
	#define	hwrBtnWidth					27
	#define	hwrGraffitiAlphaWidth	62
	#define	hwrTSMBtnWidth				0
	#define	hwrTSMBtnHeight			0
	#define	hwrCalcChr					calcChr

#endif

// Graffiti Area
#define	hwrGraffitiTop						hwrSystemTop															// Top coordinate of Graffiti area
#define	hwrGraffitiLeft					(hwrBtnWidth + hwrTSMBtnWidth)									// Left coordinate of Graffiti area
#define	hwrGraffitiWidth					(hwrDisplayWidth - (hwrBtnWidth *2) - hwrTSMBtnWidth)		// Width of Graffiti area
#define	hwrGraffitiHeight					hwrSystemHeight														// Height of Graffiti area
#define	hwrGraffitiSplit					(hwrGraffitiLeft + hwrGraffitiAlphaWidth)												// Split mode dividing line

// Launch Button
#define	hwrLaunchBtnLeft					0							
#define	hwrLaunchBtnTop					hwrSystemTop						
#define	hwrLaunchBtnWidth					hwrBtnWidth
#define	hwrLaunchBtnHeight				(hwrSystemHeight/2)

// Menu Button
#define	hwrMenuBtnLeft						0							
#define	hwrMenuBtnTop						(hwrSystemTop+hwrMenuBtnHeight)  							
#define	hwrMenuBtnWidth					hwrBtnWidth
#define	hwrMenuBtnHeight					(hwrSystemHeight/2)

// Calculator Button
#define	hwrCalcBtnLeft						(hwrDisplayWidth - hwrBtnWidth)
#define	hwrCalcBtnTop						hwrSystemTop 						
#define	hwrCalcBtnWidth					hwrBtnWidth
#define	hwrCalcBtnHeight					(hwrSystemHeight/2)

// Find Button
#define	hwrFindBtnLeft						(hwrDisplayWidth - hwrBtnWidth)
#define	hwrFindBtnTop						(hwrSystemTop+hwrCalcBtnHeight)	 						
#define	hwrFindBtnWidth					hwrBtnWidth
#define	hwrFindBtnHeight					(hwrSystemHeight/2)

// Keyboard Alpha Button
#define	hwrKeyboardAlphaBtnLeft			(hwrBtnWidth + hwrTSMBtnWidth)						
#define	hwrKeyboardAlphaBtnTop			(hwrSystemTop+hwrSystemHeight-hwrSystemHeight/4)	 						
#define	hwrKeyboardAlphaBtnWidth		18
#define	hwrKeyboardAlphaBtnHeight		(hwrSystemHeight/4)

// Keyboard Numeric Button
#define	hwrKeyboardNumericBtnLeft		(hwrFindBtnLeft-hwrKeyboardNumericBtnWidth)						
#define	hwrKeyboardNumericBtnTop		hwrKeyboardAlphaBtnTop	 						
#define	hwrKeyboardNumericBtnWidth		18
#define	hwrKeyboardNumericBtnHeight	hwrKeyboardAlphaBtnHeight

// TSM Buttons 1
#define	hwrTSMBtn1Left						hwrBtnWidth							
#define	hwrTSMBtn1Top						(hwrSystemTop + (hwrTSMBtnHeight * 0))
#define	hwrTSMBtn1Width					hwrTSMBtnWidth
#define	hwrTSMBtn1Height					hwrTSMBtnHeight

// TSM Buttons 2
#define	hwrTSMBtn2Left						hwrBtnWidth							
#define	hwrTSMBtn2Top						(hwrSystemTop + (hwrTSMBtnHeight * 1))
#define	hwrTSMBtn2Width					hwrTSMBtnWidth
#define	hwrTSMBtn2Height					hwrTSMBtnHeight

// TSM Buttons 3
#define	hwrTSMBtn3Left						hwrBtnWidth							
#define	hwrTSMBtn3Top						(hwrSystemTop + (hwrTSMBtnHeight * 2))
#define	hwrTSMBtn3Width					hwrTSMBtnWidth
#define	hwrTSMBtn3Height					hwrTSMBtnHeight

// TSM Buttons 4
#define	hwrTSMBtn4Left						hwrBtnWidth							
#define	hwrTSMBtn4Top						(hwrSystemTop + (hwrTSMBtnHeight * 3))
#define	hwrTSMBtn4Width					hwrTSMBtnWidth
#define	hwrTSMBtn4Height					hwrTSMBtnHeight/2


// HwrLCDGetDepth requests
#define	hwrLCDGetCurrentDepth			0				// Get depth display is currently set to

//--------------------------------------------------------------------------
// The battery data in the ROM is stored in a structure like this.
// 
// This is an array of structures in the ROM.
//
// WARNING: DO NOT CHANGE THE STRUCTURE OF THIS RECORD WITHOUT MODIFYING THE
//          ASSOCIATED ROUTINES IN SystemMgr.c !!!
//----------------------------------------------------------------------------
typedef struct {
	Word	sysBattDataStructVersion;	// rev level of the structure (this one is 1)
	Word	sysBattMildWrngVoltage;		// in centivolts-- this is the voltage where the warning dialog appears
	Word	sysBattSevereWrngVoltage;	// in cVolts-- this is the voltage where the critical warning dialog appears
	Word	sysBattShutdownEnVoltage;	// in cV-- This voltage causes us to do a software shutdown
	Word	sysBattShutdownDisVoltage;	// in cV-- This voltage allows us to wakeup from shutdown
	Word	sysBattMinIRVoltage;			// prevents IR from working if below this threshold
	DWord	sysBattMildWrngTimeout;		// in ticks-- this is the number of ticks to wait between displaying warning dialogs
	DWord	sysBattSevereWrngTimeout;	// in ticks-- this is the number of ticks to wait between displaying critical warning dialogs
	Word	sysBattCritWarnOffDays;		// in days-- number of days between mild and severe warnings
	DWord	sysBattCritWarnOnTicks;		// in ticks-- number of ticks between mild and severe warnings
	Word	sysBattStepsPerVolt;			// for scaling A/D to Voltages
	SWord	sysBattVoltageStepOffset;	// for scaling A/D to Voltages
	DWord	sysBattReserved1;				// future use
	DWord	sysBattReserved2;				// future use
	Word	sysBattVoltageCurve[11];	// in cV; this is the voltage at the specified percentage/10; i.e. if the voltage
												// with 20% remaining is 2.5 volts, battVoltageCurve[2] = 250.
	} SysBatteryDataStruct, *SysBatteryDataStructP;

//--------------------------------------------------------------------
// Layout of the A to D convertor calibration ROM token.
// This data is calculated for each unit during production, and burnt
// into the ROM.  It consists of 2 precise voltages which are 
// applied to the A to D convertor, as well as their respective outputs
// from the A/D convertor.  Because of manufacturing tolerances in the 
// convertor, its important that the conversion of the output (0-255) 
// into centivolts is tailored to each device during manufacturing.
//--------------------------------------------------------------------
typedef struct {
	Word	lowCentivolts;
	Word	lowAToDOutput;
	Word	highCentivolts;
	Word	highAToDOutput;
} SysAToDCalibrationTokenType;



//----------------------------------------------------------------------------
// The ROM token list is an array of zero or more HwrROMTokenRec records
// terminated by a record having token 0xFFFFFFFF. Each record is variable
// length (6 bytes minimum) and must begin on an even byte boundry.
// 
// WARNING: DO NOT CHANGE THE STRUCTURE OF THIS RECORD WITHOUT MODIFYING THE
//          ASSOCIATED ASSEMBLY ROUTINES IN ROMHardwareTD1.c !!!
//          PrvGetROMToken, HwrGetROMToken, PrvExecuteROMToken1
//----------------------------------------------------------------------------
typedef struct {
	ULong	token;
	UInt	len;
	UChar	data[0];		// variable length data goes here (0 or more chars)
} HwrROMTokenRec, *HwrROMTokenRecPtr;


#define	hwrROMTokenSnum		'snum'	// MUST be the first token, should be 12 bytes long
													//   Snum == Memory Card Flash ID (serial number)
#define	hwrROMTokenIrda		'irda'	// present ONLY on memory card w/IrDA (Gordon)
#define	hwrROMTokenFlex		'flex'	// present ONLY on memory card w/Flex pager (Cessna)
#define	hwrROMTokenBoot1		'cd01'	// boot time code plug-in #1 (See RomBoot.c)
#define	hwrROMTokenBoot2		'cd02'	// boot time code plug-in #2 (See RomBoot.c)
#define	hwrROMTokenBoot3		'cd03'	// boot time code plug-in #3 (See RomBoot.c)
#define	hwrROMTokenAToD		'adcc'	// Data for the A to D Convertor calibration

// The following include has to appear here, because MemoryPrvNew.h
// needs hwrNumCardSlots, which is defined above...

#include <MemoryPrvNew.h>	// for CardHeaderPtr below


/**************************************************************************
 * Prototypes of functions used only when running on the real hardware
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


	// These routines are only accessed from the Debugger or boot code,
	//  so they don't need traps.
	void		HwrPreRAMInit(void); 
	
	void		HwrPreDebugInit(void); 
	
	Err		HwrChecksumBigROM (CardHeaderPtr cardP);

	void 		HwrInit(void); 

	void		HwrPostDebugInit(void); 
	
	void		HwrIdentifyFeatures(void); 
	
	void		HwrResetNMI(void);
	
	DWord		HwrGetRAMSize(UInt cardNo, DWord limit);
	
	void		HwrDrawSplashScreen(void);

	void   	HwrDisableInts(void);
	
	void		HwrShutDownPLL(void);
	
	void		HwrLowBatteryHandler(void);
	
	void		HwrModelSpecificInit(void);

#ifndef __GNUC__
	asm void	PrvGetROMTokenListByCardNo(void);
	
	asm void	PrvGetROMToken(void);
	
	asm void	PrvExecuteROMToken1(void);
#endif

	// Interrupt handlers should NOT patch the low-memory interrupt vectors.
	// Instead, they should be installed using the following traps, which the
	// kernel will call whenever the given interrupt triggers.
	// Additionally, since the handlers are limited in what they can do (no calls
	// to Memory Manager or other routines that use it, including FeatureMgr),
	// a pointer to globals should be stored in the corresponding LowMem global.
	// See irq1GlobalsP, irq2GlobalsP, irq3GlobalsP, & irq6GlobalsP in <Globals.h>
	void		HwrIRQ1Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ1Handler);
	void		HwrIRQ2Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ2Handler);
	void		HwrIRQ3Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ3Handler);
	void		HwrIRQ4Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ4Handler);
	void		HwrIRQ5Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ5Handler);
	void		HwrIRQ6Handler(DWord param)
							SYS_TRAP(sysTrapHwrIRQ6Handler);
							
	
	// These routines are for "general" use.
	
	void 		HwrGetRAMMapping(Ptr /*CardInfoPtr*/ cardInfoP, UInt* numBlocksP, 
							DWordPtr physBlockInfoP, DWordPtr logBlockInfoP)
							SYS_TRAP(sysTrapHwrGetRAMMapping);
				
	DWord		HwrMemWritable(VoidPtr addr)
							SYS_TRAP(sysTrapHwrMemWritable);
	
	DWord		HwrMemReadable(VoidPtr addr)
							SYS_TRAP(sysTrapHwrMemReadable);
							
	Err		HwrCursor(Boolean set, Int* modeP, PointType* whereP, PointType* sizeP, 
						UInt* blinkP)
							SYS_TRAP(sysTrapHwrCursor);
	
	void		HwrDelay(ULong	microseconds)
							SYS_TRAP(sysTrapHwrDelay);
	
	void		HwrDoze(Boolean onlyNMI)
							SYS_TRAP(sysTrapHwrDoze);
	
	void		HwrSleep(Boolean untilReset, Boolean emergency)
							SYS_TRAP(sysTrapHwrSleep);
	
	Err		HwrWake(void)
							SYS_TRAP(sysTrapHwrWake);
							
	Err		HwrSetSystemClock(ULong*	freqP)
							SYS_TRAP(sysTrapHwrSetSystemClock);
							
	Err		HwrSetCPUDutyCycle(UInt*	dutyP)
							SYS_TRAP(sysTrapHwrSetCPUDutyCycle);
							
	Err		HwrLCDInit(UInt framePeriod, VoidPtr baseP, UInt width,
							UInt height, UInt depth)
							SYS_TRAP(sysTrapHwrLCDInit);
							
	Err		HwrLCDBaseAddr(Ptr* newAddrP)
							SYS_TRAP(sysTrapHwrLCDBaseAddr);
							
	Err		HwrLCDDrawBitmap(UInt x, UInt y, Ptr bitmapP, Boolean clearFirst)
							SYS_TRAP(sysTrapHwrLCDDrawBitmap);
							
	Err		HwrLCDSleep(Boolean untilReset, Boolean emergency)
							SYS_TRAP(sysTrapHwrLCDSleep);
							
	Err		HwrLCDWake(void)
							SYS_TRAP(sysTrapHwrLCDWake);
							
	Word		HwrLCDGetDepth(Word request)
							SYS_TRAP(sysTrapHwrLCDGetDepth);
							
	Err		HwrTimerInit(UInt timer)
							SYS_TRAP(sysTrapHwrTimerInit);
							
	Err		HwrTimerSleep(UInt timer)
							SYS_TRAP(sysTrapHwrTimerSleep);
							
	Err		HwrTimerWake(UInt timer)
							SYS_TRAP(sysTrapHwrTimerWake);
							
	UInt		HwrBatteryLevel(void)
							SYS_TRAP(sysTrapHwrBatteryLevel);
							
	Boolean	HwrPluggedIn(void)
							SYS_TRAP(sysTrapHwrPluggedIn);
							
	Boolean	HwrBacklight(Boolean set, Boolean newState)
							SYS_TRAP(sysTrapHwrBacklight);
							
	Boolean	HwrEnableDataWrites(void)
							SYS_TRAP(sysTrapHwrEnableDataWrites);
	
	void		HwrDisableDataWrites(void)
							SYS_TRAP(sysTrapHwrDisableDataWrites);
							
	Err		HwrDockSignals(WordPtr inputsP, Word outputSet, Word outputClr)
							SYS_TRAP(sysTrapHwrDockSignals);
	
	Err		HwrGetROMToken (Word cardNo, DWord token, BytePtr *dataP, WordPtr sizeP )
							SYS_TRAP(sysTrapHwrGetROMToken);
							
	Byte		HwrLCDContrast(Boolean set, Byte NewContrastLevel)
							SYS_TRAP(sysTrapHwrLCDContrast);

	hwrDockStatusState	HwrDockStatus(void)
							SYS_TRAP(sysTrapHwrDockStatus);

							
#ifdef __cplusplus 
}
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_HwrWake		\
				ASM_SYS_TRAP(sysTrapHwrWake)

#define	_HwrDelay	\
				ASM_SYS_TRAP(sysTrapHwrDelay)


#endif 	//__HARDWARE_H__

#endif 	// NON_PORTABLE
