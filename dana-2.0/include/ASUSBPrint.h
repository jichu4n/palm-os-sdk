/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            ASUSBPrint.h
 *
 * DESCRIPTION: AlphaSmart USB Printer Virtual Serial Driver header file.
 *
 ****************************************************************************/

#ifndef AS_USB_PRINT_H
#define AS_USB_PRINT_H


#include <PalmOS.h>
#include <SerialVdrv.h>

// the port ID associated with the AlphaSmart printer virtual serial driver, 
// i.e., the creator type for AlphaSmart printer interface.
#define	sysPortUSBPrint 'ASpr'

// error codes from the serial manager and their meaning with usb printing...
// from SrmOpen()
// serErrTimeOut	      - could not find any usb device.
// serErrNoDevicesAvail - could not find usb printer device.
// serErrNoMem				- not enough memory.
// serErrAlreadyOpen		- already opened.
// serErrBadPort			- usb hardware not found or functioning.
// from SrmClose()
// serErrStillOpen		- opened multiple times, and still open
// from SrmSend()
// serErrBadParam			- serial manager is confused.
// serErrLineErr			- printer disappeared from usb bus.
// from SrmControl
// serErrBadParam			- pLen not passed in.
// serErrLineErr			- usb error, could not complete request.

/*** SrmControl "*valueLenP" Parameter Definitions **************************/
// ------------------------------------------------------------------------------
// Control codes when using SerialMgr to access USB specific stuff
// ------------------------------------------------------------------------------
#define	usbOpCodeGetManufacturerString	srmCtlCustom+0
#define	usbOpCodeGetProductString			srmCtlCustom+1
#define	usbOpCodeGetSerialNumString		srmCtlCustom+2
// for usbOpCodeGetManufacturerString, usbOpCodeGetProductString, usbOpCodeGetSerialNumString
//  control call defines...
// Input:
//     valueP    = MemPtr to buffer for UNICODE string
//     valueLenP = MemPtr to buffer length in bytes
//
// Output: 
//     valueP    = MemPtr to filled in with UNICODE string
//     valueLenP = MemPtr to number of bytes copied (will always be even)

#define	usbOpCodePrintGetDeviceID					srmCtlCustom+3
// Input:
//     valueP    = MemPtr to buffer for data
//     valueLenP = MemPtr to buffer length in bytes - set to at least 512 bytes
//
// Output: 
//     valueP    = MemPtr to filled in with data
//     valueLenP = MemPtr to number of bytes read in.
// note that the data is proceeded with the length of the data in big endian format.

#define	usbOpCodePrintGetPortStatus				srmCtlCustom+4
// Input:
//     valueP    = MemPtr to byte to hold status bitfield...
//     valueLenP = should be sizeof(UInt8)
//
// Output: 
//     valueP    = MemPtr to byte, which now contains status - see following defines
//     valueLenP = sizeof(UInt8)
	#define usbOpCodePrintGetPortStatusBitEmptyPaper	0x20	// set if paper tray is empty
	#define usbOpCodePrintGetPortStatusBitSelect			0x10	// set if printer is selected
	#define usbOpCodePrintGetPortStatusBitNotError		0x08	// set if printer is NOT in error state
	
#define	usbOpCodePrintDoSoftReset					srmCtlCustom+5
// Input:	pass NULLs
// Output:  none.

#define	usbOpCodePrintGetInterfaceProtocol		srmCtlCustom+6
// Input:
//     valueP    = MemPtr to byte to hold interface protocol type
//     valueLenP = should be sizeof(UInt8)
//
// Output: 
//     valueP    = MemPtr to byte, which now contains interface protocol type - see following defines
//     valueLenP = sizeof(UInt8)
	#define usbOpCodePrintGetInterfaceProtocolUnidir		0x01	// set if paper tray is empty
	#define usbOpCodePrintGetInterfaceProtocolBidir			0x02	// set if printer is selected
	#define usbOpCodePrintGetInterfaceProtocolBidir1284	0x03	// set if printer is NOT in error state


// some sample code...with error handling omitted.
#if 0
#include <PalmOS.h>
#include "ASUSBPrint.h"

static const UInt8 PrinterMessage[] = "Some text to send the printer.\r\n\f";

void OutputToPrinter(void)
{
	UInt16 vuwPortId = 0;
	UInt16 vuwUniStringSize;
	UInt16 vuwUniString[64];
	UInt8	vubInterfaceProtocol;
	UInt8	vubPortStatus;
	UInt8	bigBuffer[512];
												// Open the virtual port to the printer.
												// At this time the printer is enumerated
												// and an error is returned if a printer is
												// not found.
	err = SrmOpen(sysPortUSBPrint, 57600, &vuwPortId);
												// Get the printer manufacturer length only - useful if
												// you will allocate memory to save it.
	vuwUniStringSize=0;
	err = SrmControl(vuwPortId, usbOpCodeGetManufacturerString, NULL, &vuwUniStringSize);

												// Actually get the usb manufacturer id string.
	err = SrmControl(vuwPortId, usbOpCodeGetManufacturerString, &vuwUniString, &vuwUniStringSize);

												// Get the product id string, assuming it will fit in 64 words.
	vuwUniStringSize=64;
	err = SrmControl(vuwPortId, usbOpCodeGetProductString, &vuwUniString, &vuwUniStringSize);
												// Get the Serial number string...
	vuwUniStringSize=64;
	err = SrmControl(vuwPortId, usbOpCodeGetSerialNumString, &vuwUniString, &vuwUniStringSize);

												// Get the device id string. This string can be parsed for
												// details about the printer
	vuwUniStringSize=sizeof(bigBuffer);
	err = SrmControl(vuwPortId, usbOpCodePrintGetDeviceID, bigBuffer, &vuwUniStringSize);

												// Get the port status....		
	vuwUniStringSize=sizeof(UInt8);
	err = SrmControl(vuwPortId, usbOpCodePrintGetPortStatus, &vubPortStatus, &vuwUniStringSize);

												// Reset the printer.. not needed here.
	err = SrmControl(vuwPortId, usbOpCodePrintDoSoftReset, NULL, NULL);
	
												// Get the interface protocol type.	
	vuwUniStringSize=sizeof(UInt8);
	err = SrmControl(vuwPortId, usbOpCodePrintGetInterfaceProtocol, &vubInterfaceProtocol, &vuwUniStringSize);

												// Send the print page here.
	SrmSend(vuwPortId, 
				PrinterMessage, 
				sizeof( PrinterMessage )-1, 
				&err);
	
	SrmClose(vuwPortId);					// close the port.
												// this is important, as the power consumption drops, when the
												// port is closed.
}
#endif

#endif 