/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1995 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     Crc.h
 * AUTHOR:	 Vitaly Kruglikov: May 10, 1995
 *
 *		
 * DESCRIPTION:
 *		This is the header file for the CRC calculation routines for Pilot.
 *
 *
 * REVISION HISTORY:
 *		5/10/95		vmk		Initial version.
 *
 **********************************************************************/


#ifndef __CRC_H__
#define __CRC_H__


// Include elementary types
#include <Common.h>
#include <SystemMgr.h>




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
Word			Crc16CalcBlock(VoidPtr bufP, UInt count, Word crc)
							SYS_TRAP(sysTrapCrc16CalcBlock);


#ifdef __cplusplus 
}
#endif



#endif  // __CRC_H__

