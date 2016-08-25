/******************************************************************************
 *
 * Copyright (c) 2001 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: LstGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *    Glue providing compatibility for applications that wish to make calls to
 *    some recent list functions, but which might actually be running on a
 *    system which does not support these newer calls.
 *
 * History:
 *		2001-02-26	jwm	Created.
 *
 *****************************************************************************/

#ifndef __LSTGLUE_H__
#define __LSTGLUE_H__

#include <List.h>

#ifdef __cplusplus
extern "C" {
#endif

Int16 LstGlueGetTopItem (const ListType *listP);

#ifdef __cplusplus
}
#endif

#endif
