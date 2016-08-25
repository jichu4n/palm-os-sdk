/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		Crc.h
 *
 * Description:
 *		This is the header file for the CRC calculation routines for Pilot.
 *
 * History:
 *		5/10/95		vmk		Initial version.
 *
 *******************************************************************/  


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

