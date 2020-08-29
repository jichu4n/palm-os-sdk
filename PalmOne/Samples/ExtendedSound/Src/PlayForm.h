/***********************************************************************
 *
 * Copyright (c) 2002 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	PlayForm.h
 *
 * Description:
 *	Player form header.
 *
 *	Version 1.0 	- Initial Revision (03/04/03)
 *
 ***********************************************************************/

#ifndef PLAYFORM_H_
#define PLAYFORM_H_

#include <PalmOS.h>

/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Err 	PlayStreamEventNotify(SysNotifyParamType *notifyParamP);
Boolean PlayFormHandleEvent(EventType *eventP);

#ifdef __cplusplus 
}
#endif

#endif // PLAYFORM_H_