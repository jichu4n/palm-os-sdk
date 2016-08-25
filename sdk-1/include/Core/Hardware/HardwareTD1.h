/*******************************************************************
 * 							 Touchdown Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		HardwareTD1.h
 *
 * Description:
 *		Hardware Equates for the TD1 platform. This header file
 *  contains equates specific to Hardware platform TD1.
 *  It is included only in the modules "ROMHardwareTD1.c" and
 *  "AMXHardwareTD1.c".
 *
 * History:
 *   6/13/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE									// So app's don't mistakenly include this

#ifndef __HARDWARETD1_H__
#define __HARDWARETD1_H__



#if HW_REV == 1
/************************************************************************
 * Port F Bit settings
 ************************************************************************/
#define	hwrTD1PortFPanelYPOff		0x01			// (L) Vcc to Y Panel
#define	hwrTD1PortFPanelYMOn			0x02			// (H) Gnd to Y Panel
#define	hwrTD1PortFPanelXPOff		0x04			// (L) Vcc to X Panel
#define	hwrTD1PortFPanelXMOn			0x08			// (H) Gnd to X Panel
#define	hwrTD1PortFUnused4			0x10			
#define	hwrTD1PortFUnused5			0x20			
#define	hwrTD1PortFUnused6			0x40			
#define	hwrTD1PortFSerialOn			0x80			// (H) Serial driver Enable


// Configurations of the panel control bits
#define	hwrTD1PortFPanelMask			0x0F			// Panel control bits

// Configure for lowest power
#define	hwrTD1PortFPanelCfgOff			(hwrTD1PortFPanelYPOff | 		\
												 	 hwrTD1PortFPanelXPOff)

// Configure to generate pen interrupt
#define	hwrTD1PortFPanelCfgPenIRQ		(hwrTD1PortFPanelXMOn | 		\
								 				 	 hwrTD1PortFPanelYPOff | 		\
												 	 hwrTD1PortFPanelXPOff)

// Configure to Measure Y Position
#define	hwrTD1PortFPanelCfgYMeas		(hwrTD1PortFPanelXMOn | 		\
								 				 	 hwrTD1PortFPanelYPOff)

// Configure to Measure X Position
#define	hwrTD1PortFPanelCfgXMeas		(hwrTD1PortFPanelXPOff | 		\
								 				 	 hwrTD1PortFPanelYMOn)



/************************************************************************
 * Port J Bit Assignments
 ************************************************************************/
#define	hwrTD1PortJADCSOff			0x02			// (L) A/D Chip select
#define	hwrTD1PortJBattOff			0x10			// (L) Battery Voltage measure 		
#define	hwrTD1PortJLCDEnableOn		0x20			// (H) LCD Display On 	
#define	hwrTD1PortJLCDVccOff			0x40			// (L) LCD Vcc 
#define	hwrTD1PortJLCDVeeOn			0x80			// (H) LCD Vee 


/************************************************************************
 * Port K Bit Assignments
 ************************************************************************/
#define	hwrTD1PortKSelfRefOff		0x08			// (L) PSRAM Self Refresh mode
#define	hwrTD1PortKSelfRefOffBit	3


/************************************************************************
 * Port M Bit Assignments
 ************************************************************************/
#define	hwrTD1PortMVccFail			0x04			// (L) Input indicates 3.3v is too low
#define	hwrTD1PortMCardIRQ			0x08			// (L) IRQ3 input from card
#define	hwrTD1PortMUnused4			0x10			// Unused (IRQ2)
#define	hwrTD1PortMDockButton		0x20			// (L) Sync button from Serial Doc
#define	hwrTD1PortMPenIO				0x40			// Pen input/output
#define	hwrTD1PortMDockIn				0x80			// (L) Gen. Purpose input from Serial Doc



/************************************************************************
 * SPIM Base Configuration
 ************************************************************************/
#define	hwrTD1SPIMBaseControl		(hwr328SPIMControlRateDiv32 |	\
												 hwr328SPIMControlIntEnable |		\
												 hwr328SPIMControlInvPolarity |	\
												 hwr328SPIMControlOppPhase  |  	\
												 16-1)


/************************************************************************
 * Macros for turning write-protection on/off
 * These are used in-line for MemSemaphoreReserve and MemSemaphoreRelease
 *  for performance reasons.
 ************************************************************************/
#define	hwrWriteProtectOff()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
	/* Word			mask;		*/														\
																							\
	/* DOLATER.. take this out....	*/											\
	/* mask = baseP->csAGroupMask;		*/										\
																							\
	/* Clr read-only bit in chip select register for storage area */	\
	baseP->csASelect1 &= ~(0x0008);												\
	baseP->csASelect2 &= ~(0x0008);												\
																							\
	/* DOLATER.. take this out... */												\
	/* baseP->csAGroupMask = mask;	*/											\
	}

#ifndef	BUILD_OPTION_WP_OFF
#define	hwrWriteProtectOn()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
	/* Word			mask;		*/														\
																							\
	/* DOLATER.. take this out....	*/											\
	/* mask = baseP->csAGroupMask;		*/										\
																							\
	/* Set read-only bit in chip select register for storage area */	\
	baseP->csASelect1 |= (0x0008);												\
	baseP->csASelect2 |= (0x0008);												\
																							\
	/* DOLATER.. take this out... */												\
	/* baseP->csAGroupMask = mask;	*/											\
	}
#else
#define	hwrWriteProtectOn()
#endif


#elif HW_REV >= 2
/************************************************************************
 * Port D Bit settings
 ************************************************************************/
#define	hwrTD1PortDNoExtPower		0x80			// (L) External DC input

/************************************************************************
 * Port F Bit settings
 ************************************************************************/
#define	hwrTD1PortFPanelYPOff		0x01			// (L) Vcc to Y Panel
#define	hwrTD1PortFPanelYMOn			0x02			// (H) Gnd to Y Panel
#define	hwrTD1PortFPanelXPOff		0x04			// (L) Vcc to X Panel
#define	hwrTD1PortFPanelXMOn			0x08			// (H) Gnd to X Panel
#define	hwrTD1PortFLCDEnableOn		0x10			// (H) LCD Display on	
#define	hwrTD1PortFLCDVccOff			0x20			// (L) LCD Vcc		
#define	hwrTD1PortFLCDVeeOn			0x40			// (H) LCD Vee
#define	hwrTD1PortFADCSOff			0x80			// (L) A/D Chip Select


// Configurations of the panel control bits
#define	hwrTD1PortFPanelMask			0x0F			// Panel control bits

// Configure for lowest power
#define	hwrTD1PortFPanelCfgOff			(hwrTD1PortFPanelYPOff | 		\
												 	 hwrTD1PortFPanelXPOff)

// Configure to generate pen interrupt
#define	hwrTD1PortFPanelCfgPenIRQ		(hwrTD1PortFPanelXMOn | 		\
								 				 	 hwrTD1PortFPanelYPOff | 		\
												 	 hwrTD1PortFPanelXPOff)

// Configure to Measure Y Position
#define	hwrTD1PortFPanelCfgYMeas		(hwrTD1PortFPanelXMOn | 		\
								 				 	 hwrTD1PortFPanelYPOff)

// Configure to Measure X Position
#define	hwrTD1PortFPanelCfgXMeas		(hwrTD1PortFPanelXPOff | 		\
								 				 	 hwrTD1PortFPanelYMOn)


/************************************************************************
 * Port G Bit Assignments
 ************************************************************************/
#define	hwrTD1PortGSerialOn			0x08			// (H) Serial Driver Enable
#define	hwrTD1PortGBattOff			0x10			// (L) Battery Voltage Measure
#define	hwrTD1PortGRefreshOut		0x20			// (L) Timer 1 output to signal refresh
#define	hwrTD1PortGSelfRefOff		0x40			// (L) PSRAM Self Refresh mode
#define	hwrTD1PortGSelfRefOffBit	6				//  Bit # for above
#define	hwrTD1PortGDockOut1			0x80			// (H) Gen. Purpose output on Serial Doc


/************************************************************************
 * Port M Bit Assignments
 ************************************************************************/
#define	hwrTD1PortMVccFail			0x04			// (L) Input indicates 3.3v is too low
#define	hwrTD1PortMCardIRQ			0x08			// (L) IRQ3 input from card
#define	hwrTD1PortMUnused4			0x10			// Unused (IRQ2)
#define	hwrTD1PortMDockButton		0x20			// (L) Sync button from Serial Doc
#define	hwrTD1PortMPenIO				0x40			// Pen input/output
#define	hwrTD1PortMDockIn				0x80			// (L) Gen. Purpose input from Serial Doc



/************************************************************************
 * SPIM Base Configuration
 ************************************************************************/
#define	hwrTD1SPIMBaseControl		(hwr328SPIMControlRateDiv16 |		\
												 hwr328SPIMControlIntEnable |		\
												 hwr328SPIMControlInvPolarity |	\
												 hwr328SPIMControlOppPhase  |  	\
												 16-1)


/************************************************************************
 * Macros for turning write-protection on/off
 * These are used in-line for MemSemaphoreReserve and MemSemaphoreRelease
 *  for performance reasons.
 ************************************************************************/
#define	hwrWriteProtectOff()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
	/* Word			mask; */															\
																							\
	/* DOLATER.. take this out....	*/											\
	/* mask = baseP->csAGroupMask;	*/											\
																							\
	/* Clr read-only bit in chip select register for storage area */	\
	baseP->csASelect1 &= ~(0x0008);												\
	baseP->csASelect3 &= ~(0x0008);												\
																							\
	/* DOLATER.. take this out... */												\
	/*baseP->csAGroupMask = mask;	*/												\
	}

#ifndef	BUILD_OPTION_WP_OFF
#define	hwrWriteProtectOn()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
	/* Word			mask; */															\
																							\
	/* DOLATER.. take this out....	*/											\
	/* mask = baseP->csAGroupMask;	*/											\
																							\
	/* Set read-only bit in chip select register for storage area */	\
	baseP->csASelect1 |= (0x0008);												\
	baseP->csASelect3 |= (0x0008);												\
																							\
	/* DOLATER.. take this out... */												\
	/*baseP->csAGroupMask = mask;	*/												\
	}
#else
#define	hwrWriteProtectOn()	
#endif

#else // HW_REV==2
	#error "Compiler variable HW_REV not valid!!!"
#endif


/************************************************************************
 *Initial values for the PLL. We set the system clock to be VCO/2 so
 *  the system clock speed becomes:
 *
 *  32768 * ((hwrTD1FreqSelP + 1) * 14 + hwrTD1FreqSelQ + 1)
 *   --------------------------------------------------------
 *                            2
 ************************************************************************/
// NON-ASIC versions must run at 8 Mhz.
#if HW_REV < 4
#define	hwrTD1PLLControl			(hwr328PLLControlClkEn | 			\
											 hwr328PLLControlSysVCODiv2 |		\
											 hwr328PLLControlPixVCODiv2)
//#define	hwrTD1FreqSelQ				0x05
#define	hwrTD1FreqSelQ				0x07		// vmk 11/20/95 tunes clock freq for 38400 baud,
														// allowing standard baud rates 2400-57600 to function
//#define	hwrTD1FreqSelP				0x23
#define	hwrTD1FreqSelP				0x24
#define	hwrTD1Frequency			(32768*((hwrTD1FreqSelP+1)*14+hwrTD1FreqSelQ+1)/2)

// OLD ASIC versions must run between 10 and 12.5 Mhz.
#elif HW_REV == 4
#define	hwrTD1PLLControl			(hwr328PLLControlClkEn | 			\
											 hwr328PLLControlSysVCODiv1 | 	\
											 hwr328PLLControlPixVCODiv1)
#define	hwrTD1FreqSelQ				0x05
#define	hwrTD1FreqSelP				0x19
#define	hwrTD1Frequency			(32768*((hwrTD1FreqSelP+1)*14+hwrTD1FreqSelQ+1)/1)


// NEW ASIC versions run at 16.580608 Mhz
#else 
#define	hwrTD1PLLControl			(hwr328PLLControlClkEn | 			\
											 hwr328PLLControlSysVCODiv1 | 	\
											 hwr328PLLControlPixVCODiv1)
#define	hwrTD1FreqSelQ				0x01
#define	hwrTD1FreqSelP				0x23
#define	hwrTD1Frequency			(32768*((hwrTD1FreqSelP+1)*14+hwrTD1FreqSelQ+1)/1)
#endif


 
#endif 	//__HARDWARETD1_H__

#endif 	// NON_PORTABLE
