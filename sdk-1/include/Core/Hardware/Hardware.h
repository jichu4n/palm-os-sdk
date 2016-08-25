/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------   
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
 *   	4/27/95  RM - Created by Ron Marianetti
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



// Bits for the call HwrDockSignals
#define	hwrDockInSyncButton	0x0001					// Dock button
#define	hwrDockInGeneric1		0x0002					// General Purpose input #1

#define	hwrDockOutGeneric0	0x0001					// General Purpose output #0



// Define # of card slots
#if EMULATION_LEVEL == EMULATION_NONE
#define	hwrNumCardSlots	1								// 1 card slot
#else
#define	hwrNumCardSlots	2								// 2 card slots for emulation
#endif


// Screen Dimensions
#define	hwrDisplayWidth		160					// Width of display
#define	hwrDisplayHeight		160					// Height of display

// Screen refresh rate
#define	hwrDisplayPeriod		12						// frame period in ms.


// Battery scaling:
//    Voltage = (level + hwrVoltStepsOffset) / hwrStepsPerVolt
#define	hwrStepsPerVolt		78						// 78 = 1 volt.
#define	hwrVoltStepsOffset	0						// Add to level before scaling		

//----------------------------------------------------------------------------
// Digitizer Dimensions
//----------------------------------------------------------------------------
#define	hwrSystemTop			(hwrDisplayHeight+4)
#define	hwrSystemHeight		56

// Graffiti Area
#define	hwrGraffitiTop			hwrSystemTop			// Top coordinate of Graffiti area
#define	hwrGraffitiLeft		27							// Left coordinate of Graffiti area
#define	hwrGraffitiWidth		106						// Width of Graffiti area
#define	hwrGraffitiHeight		hwrSystemHeight		// Height of Graffiti area
#define	hwrGraffitiSplit		(hwrGraffitiLeft+62)	// Split mode dividing line

// Launch Button
#define	hwrLaunchBtnLeft		0							
#define	hwrLaunchBtnTop		hwrSystemTop						
#define	hwrLaunchBtnWidth		27
#define	hwrLaunchBtnHeight	(hwrSystemHeight/2)

// Menu Button
#define	hwrMenuBtnLeft			0							
#define	hwrMenuBtnTop			(hwrSystemTop+hwrMenuBtnHeight)  							
#define	hwrMenuBtnWidth		27
#define	hwrMenuBtnHeight		(hwrSystemHeight/2)

// Calculator Button
#define	hwrCalcBtnLeft			133							
#define	hwrCalcBtnTop			hwrSystemTop 						
#define	hwrCalcBtnWidth		27
#define	hwrCalcBtnHeight		(hwrSystemHeight/2)

// Find Button
#define	hwrFindBtnLeft			133							
#define	hwrFindBtnTop			(hwrSystemTop+hwrCalcBtnHeight)	 						
#define	hwrFindBtnWidth		27
#define	hwrFindBtnHeight		(hwrSystemHeight/2)



/**************************************************************************
 * Prototypes of functions used only when running on the real hardware
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


	// These routines are only accessed from the Debugger so they don't
	//  need traps.
	void		HwrPreRAMInit(void); 
	
	void		HwrPreDebugInit(void); 
	
	void 		HwrInit(void); 

	void		HwrPostDebugInit(void); 
	
	Err		HwrFlashWriteSub(Ptr bodyP);
							
	void		HwrResetNMI(void);
	
	DWord		HwrGetRAMSize(UInt cardNo, DWord limit);
	
	void		HwrDrawSplashScreen(void);

	void   	HwrDisableInts(void);
	
	void		HwrShutDownPLL(void);
	
	void		HwrLowBatteryHandler(void);


	// These routines are for "general" use.
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
							
	Boolean	HwrEnableDataWrites(void)
							SYS_TRAP(sysTrapHwrEnableDataWrites);
	
	void		HwrDisableDataWrites(void)
							SYS_TRAP(sysTrapHwrDisableDataWrites);
							
	Err		HwrDockSignals(WordPtr inputsP, Word outputSet, Word outputClr)
							SYS_TRAP(sysTrapHwrDockSignals);
	
							
#ifdef __cplusplus 
}
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_HwrWake		\
				ASM_SYS_TRAP(sysTrapHwrWake)


#endif 	//__HARDWARE_H__

#endif 	// NON_PORTABLE
