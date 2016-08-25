/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		DLUtil.h
 *
 * Description:
 *		Desktop Link Protocol(DLP) utility definitions.
 *
 * History:
 *   	8/12/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __DL_UTIL_H__
#define __DL_UTIL_H__

// Pilot common definitions
#include <Common.h>

// Desktop Link common definitions
#include	<DLCommon.h>


/********************************************************************
 * Utility Routines
 ********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


Int DlkUtilFillArgWrapper(DlpGenericArgWrapperPtr wrapperP,
		Byte argID, Word argSize);
						
Boolean DlkUtilGetArg(VoidPtr bodyBufP, Word bodySize, Byte argID,
		VoidPtr* dataPP, WordPtr dataSizeP);


#ifdef __cplusplus 
}
#endif


/********************************************************************
 * Utility Macros
 ********************************************************************/



#endif	// __DL_UTIL_H__
