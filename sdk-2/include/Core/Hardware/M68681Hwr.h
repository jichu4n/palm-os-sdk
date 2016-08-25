/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		M68681Hwr.h
 *
 * Description:
 *		Hardware Equates for the Motorola 68681 Duart 
 *
 * History:
 *   	1/18/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/  


#include <Common.h>							// Touchdown includes


/***********************************************************************
 * Motorola 68681 DUART registers
 ***********************************************************************/
 
#if (HW_TARGET == HW_TARGET_302 || HW_TARGET == HW_TARGET_328EMU)
typedef union HwrDuartType {

	// Read Registers
	struct {
		Byte	u0;
		Byte		mra;					// Mode registers 1A and 2A
		Byte	u2;
		Byte		sra;					// Status register A
		Byte	u4;
		Byte		doNotAccess1;		// Do Not Access
		Byte	u6;
		Byte		rba;					// Receive Buffer A
		Byte	u8;
		Byte		ipcr;					// Inport Port Change Register
		Byte	uA;
		Byte		isr;					// Input Status Register
		Byte	uC;
		Byte		cur;					// Current MSB of counter
		Byte	uE;
		Byte		clr;					// Current LSB of counter
		Byte	u10;
		Byte		mrb;					// Mode registers 1B and 2B
		Byte	u12;
		Byte		srb;					// Status register B
		Byte	u14;
		Byte		doNotAccess2;		// Do Not Access
		Byte	u16;
		Byte		rbb;					// Receive Buffer B
		Byte	u18;				
		Byte		ivr;					// Interrupt vector register
		Byte	u1A;	
		Byte		ipr;					// Input Port register
		Byte	u1C;
		Byte		startCtr;			// Start Counter command register
		Byte	u1E;
		Byte		stopCtr;				// Stop Counter command register
		} read;
		
	// Write Registers
	struct {
		Byte	u0;
		Byte		mra;					// Mode registers 1A and 2A
		Byte	u2;
		Byte		csra;					// Clock Select Register A
		Byte	u4;
		Byte		cra;					// Command Register A
		Byte	u6;
		Byte		tba;					// Transmit Buffer A
		Byte	u8;
		Byte		acr;					// Auxiliary Control Register 
		Byte	uA;
		Byte		imr;					// Interrupt Mask Register
		Byte	uC;
		Byte		ctur;					// Counter/Timer Upper Preload Register
		Byte	uE;
		Byte		ctlr;					// Couunter/Timer Lower Preload Register
		Byte	u10;
		Byte		mrb;					// Mode registers 1B and 2B
		Byte	u12;
		Byte		csrb;					// Clock Select Register B
		Byte	u14;
		Byte		crb;					// Command Register B
		Byte	u16;				
		Byte		tbb;					// Transmitter Buffer B
		Byte	u18;	
		Byte		ivr;					// Interrupt Vector Register
		Byte	u1A;
		Byte		opcr;					// Output Port Config Register
		Byte	u1C;
		Byte		opSet;				// Output Port Bit Set
		Byte	u1E;
		Byte		opReset;				// Output Port Bit Reset
		} write;
	} HwrDuartType;
#else	// ! (HW_TARGET == HW_TARGET_302 || HW_TARGET == HW_TARGET_328EMU)


// For Pilot debug board zzz make conditional later
typedef union HwrDuartType {

	// Read Registers
	struct {
		Byte		mra;					// Mode registers 1A and 2A
		Byte	u0;
		Byte		sra;					// Status register A
		Byte	u2;
		Byte		doNotAccess1;		// Do Not Access
		Byte	u4;
		Byte		rba;					// Receive Buffer A
		Byte	u6;
		Byte		ipcr;					// Inport Port Change Register
		Byte	u8;
		Byte		isr;					// Input Status Register
		Byte	uA;
		Byte		cur;					// Current MSB of counter
		Byte	uC;
		Byte		clr;					// Current LSB of counter
		Byte	uE;
		Byte		mrb;					// Mode registers 1B and 2B
		Byte	u10;
		Byte		srb;					// Status register B
		Byte	u12;
		Byte		doNotAccess2;		// Do Not Access
		Byte	u14;
		Byte		rbb;					// Receive Buffer B
		Byte	u16;
		Byte		ivr;					// Interrupt vector register
		Byte	u18;				
		Byte		ipr;					// Input Port register
		Byte	u1A;	
		Byte		startCtr;			// Start Counter command register
		Byte	u1C;
		Byte		stopCtr;				// Stop Counter command register
		Byte	u1E;
		} read;
		
	// Write Registers
	struct {
		Byte		mra;					// Mode registers 1A and 2A
		Byte	u0;
		Byte		csra;					// Clock Select Register A
		Byte	u2;
		Byte		cra;					// Command Register A
		Byte	u4;
		Byte		tba;					// Transmit Buffer A
		Byte	u6;
		Byte		acr;					// Auxiliary Control Register 
		Byte	u8;
		Byte		imr;					// Interrupt Mask Register
		Byte	uA;
		Byte		ctur;					// Counter/Timer Upper Preload Register
		Byte	uC;
		Byte		ctlr;					// Couunter/Timer Lower Preload Register
		Byte	uE;
		Byte		mrb;					// Mode registers 1B and 2B
		Byte	u10;
		Byte		csrb;					// Clock Select Register B
		Byte	u12;
		Byte		crb;					// Command Register B
		Byte	u14;
		Byte		tbb;					// Transmitter Buffer B
		Byte	u16;				
		Byte		ivr;					// Interrupt Vector Register
		Byte	u18;	
		Byte		opcr;					// Output Port Config Register
		Byte	u1A;
		Byte		opSet;				// Output Port Bit Set
		Byte	u1C;
		Byte		opReset;				// Output Port Bit Reset
		Byte	u1E;
		} write;
	} HwrDuartType;

#endif	// #if (HW_TARGET == HW_TARGET_302 || HW_TARGET == HW_TARGET_328EMU)


typedef	volatile   HwrDuartType*	HwrDuartPtr;





/***********************************************************************
 * For Final code on the Touchdown Hardware, define all register bits here 
 *	 for now, we'll just define a few and use numbers in the code for the rest
 ***********************************************************************/
 
// Status Register (SR)
#define	hwrDuartSRRegRB			0x80		// Receive Break
#define	hwrDuartSRRegFE			0x40		// Framing Err
#define	hwrDuartSRRegPE			0x20		// Parity Err
#define	hwrDuartSRRegOE			0x10		// Overrun Err
#define	hwrDuartSRRegTxEMP		0x08		// Transmitter Empty
#define	hwrDuartSRRegTxRDY		0x04		// Transmitter Ready
#define	hwrDuartSRRegFFULL		0x02		// FIFO Full
#define	hwrDuartSRRegRxRDY		0x01		// Receiver Ready
