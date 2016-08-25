/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: PrivateRecords.h
 *
 * Description:
 *	  This header file defines a generic private record maintainance dialogs, etc.
 *
 * History:
 *		6/23/99.	Created by Craig Skinner
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

