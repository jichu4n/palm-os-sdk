/******************************************************************************
 *
 * Copyright (c) 1996-2004 PalmSource, Inc. All rights reserved.
 *
 * File: PrivateRecords.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *	  This header file defines a generic private record maintainance dialogs, etc.
 *
 *****************************************************************************/

#ifndef	__PRIVATERECORDS_H__
#define	__PRIVATERECORDS_H__

#include <CoreTraps.h>

// Defines needed for hidden record visual determination.
typedef enum privateRecordViewEnum {
showPrivateRecords = 0x00,
maskPrivateRecords,
hidePrivateRecords
} privateRecordViewEnum;

//-----------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern privateRecordViewEnum SecSelectViewStatus (void)
		SYS_TRAP(sysTrapSecSelectViewStatus);

extern Boolean SecVerifyPW (privateRecordViewEnum newSecLevel)
		SYS_TRAP(sysTrapSecVerifyPW);

#ifdef __cplusplus 
}
#endif

#endif //__PRIVATERECORDS_H__
