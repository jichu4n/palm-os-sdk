/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		HardwarePrv.h
 *
 * Description:
 *		Private Hardware Equates. This header file
 *  contains equates that are common across all platforms but that 
 *  change value for each hardware rev and platform.
 *
 * History:
 *   	4/27/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE	   // So app's don't mistakenly include this

#ifndef __HARDWAREPRV_H__
#define __HARDWAREPRV_H__


// This must be defined on the command line before using this header!!
#if HW_TARGET != HW_TARGET_NONE
	#ifndef HW_REV
		#error	THE COMPILER VARIABLE HW_REV MUST BE DEFINED!!
	#endif
#endif



/***********************************************************************
 *  Equates for Emulation Mode
 ***********************************************************************/
#if HW_TARGET == HW_TARGET_NONE

#define	hwrCardSize			0x10000000L				// Size of each card slot

// DOLATER.. take this out??? 
// Under native mode, this mask can be used to convert a pointer to a card offse 
//#define	hwrCardOffsetMask	0x0FFFFFFFL


#endif



/***********************************************************************
 *  First Rev of the Pilot Hardware
 ***********************************************************************/
#if HW_TARGET == HW_TARGET_TD1
	

//  Define location and setup for the 68328 Dragonball chip
#define	hwr68328Base		0xFFFFF000L					// Base address of 68328 



// Define the base of card 0 and the size of each card slot.
#if HW_REV <= 3
#define	hwrCardBase0		0x00800000L					// starts here
#define	hwrCardSize			0x00800000L					// Size of each card slot
#define	hwrFlashBase		0x00F00000L					// Base of Flash ROM
#define	hwrFlashSize		0x00080000L					// Size of Flash ROM
#define	hwrCardOffsetMask	0x007FFFFFL					// Used to convert a pointer to a card offset

#else // HW_REV >= 4
#define	hwrCardBase0		0x10000000L					// starts here
#define	hwrCardSize			0x10000000L					// Size of each card slot
#define	hwrFlashBase		0x10C00000L					// Base of Flash ROM
#define	hwrFlashSize		0x00100000L					// Size of Flash ROM
#define	hwrCardOffsetMask	0x0FFFFFFFL					// Used to convert a pointer to a card offset

#endif

// Define the Flash width
#define	hwrROMWidth			2

//  Define location and setup for the 68681 Duart
#define	hwrDuartBase		0x10E00000				// Base address of DUART

#define	hwrDuartClock		3686400					// in Hertz (3.6864 MHz)
 
// Setting of ACR register except for baud rate select 
// Timer mode - Crystal clock /1
#define	hwrDuartACRReg		0x60					


#endif // 	HW_TARGET == HW_TARGET_TD1




/***********************************************************************
 *  Motorola 68302 Development board
 ***********************************************************************/
#if HW_TARGET == HW_TARGET_302

// Define # of card slots and base address of each slot
#define	hwrNumCardSlots	1							// 1 card slot
#define	hwrCardBase0		0x00000000L				// starts at 0
#define	hwrCardSize			0x10000000L				// Size of each card slot

// Under native mode, this mask can be used to convert a pointer to a card offse 
#define	hwrCardOffsetMask	0x0FFFFFFFL


//  Define location and setup for the 68681 Duart
#define	hwrDuartBase		0x00400000				// Base address of DUART

#define	hwrDuartClock		3686400					// in Hertz (3.6864 MHz)
														
#define	hwrDuartIntVec		25							// Interrupt vector number for DUART

 
// Setting of ACR register except for baud rate select 
// Timer mode - Crystal clock /1
#define	hwrDuartACRReg		0x60					


// Define the Flash base address
#define	hwrFlashBase		0x00200000L
#define	hwrROMWidth			2


#endif // 	HW_TARGET == HW_TARGET_302




/***********************************************************************
 *  Motorola 68328 Emulator Development board
 ***********************************************************************/
#if HW_TARGET == HW_TARGET_328EMU


// Define # of card slots and base address of each slot
#define	hwrNumCardSlots	1								// 1 card slot
#define	hwrCardBase0		0x00000000L					// starts at 0
#define	hwrCardSize			0x10000000L				// Size of each card slot

// Under native mode, this mask can be used to convert a pointer to a card offse 
#define	hwrCardOffsetMask	0x0FFFFFFFL
 
 
// Define location and setup for the 68681 Duart
#define	hwrDuartBase		0x00F00000					// Base address of DUART

#define	hwrDuartClock		3686400						// in Hertz (3.6864 MHz)
														
#define	hwrDuartIntVec		25								// Interrupt vector number for DUART
 
 
 
// Setting of ACR register except for baud rate select
// Timer mode - Crystal clock /1
#define	hwrDuartACRReg		0x60					



// Define the Flash base address
#define	hwrFlashBase		0x00400000L
#define	hwrROMWidth			2



#endif // HW_TARGET == HW_TARGET_328EMU


#endif 	//__HARDWARE_H__
#endif 	// NON_PORTABLE
