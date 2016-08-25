/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
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


extern Int DlkUtilFillArgWrapper(DlpGenericArgWrapperPtr wrapperP,
		Word wID, ULong argSize);
						
extern Boolean DlkUtilFindArg(VoidPtr bufP, ULong bufSize,
		Word wID, VoidPtr* argDataPP, ULongPtr argDataSizeP);


#ifdef __cplusplus 
}
#endif


/********************************************************************
 * Utility Macros
 ********************************************************************/



#endif	// __DL_UTIL_H__
