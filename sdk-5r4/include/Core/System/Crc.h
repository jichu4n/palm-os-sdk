/******************************************************************************
 *
 * Copyright (c) 1995-2004 PalmSource, Inc. All rights reserved.
 *
 * File: Crc.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *		This is the header file for the CRC calculation routines for Pilot.
 *
 *****************************************************************************/

#ifndef __CRC_H__
#define __CRC_H__


// Include elementary types
#include <PalmTypes.h>
#include <CoreTraps.h>




/********************************************************************
 * CRC Calculation Routines
 * These are define as external calls only under emulation mode or
 *  under native mode from the module that actually installs the trap
 *  vectors
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


	

//-------------------------------------------------------------------
// API
//-------------------------------------------------------------------


// Crc16CalcBlock()
//
// Calculate the 16-bit CRC of a data block using the table lookup method.
// 
UInt16		Crc16CalcBlock(const void *bufP, UInt16 count, UInt16 crc)
							SYS_TRAP(sysTrapCrc16CalcBlock);

//Crc16CalcBigBlock is not a general purpose API thus should be called for Device build. 
UInt16		Crc16CalcBigBlock(void *bufP, UInt32 count, UInt16 crc);


#ifdef __cplusplus 
}
#endif



#endif  // __CRC_H__
