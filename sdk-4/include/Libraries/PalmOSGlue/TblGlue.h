/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: TblGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *    Glue providing compatibility for applications that wish to make calls to
 *    some recent table functions, but which might actually be running on a
 *    system which does not support newer calls.
 *
 * History:
 *		2000-12-22	jwm	Created.
 *
 *****************************************************************************/

#ifndef __TBLGLUE_H__
#define __TBLGLUE_H__

#include <Table.h>

#ifdef __cplusplus
extern "C" {
#endif

Int16 TblGlueGetNumberOfColumns (const TableType *tableP);

Int16 TblGlueGetTopRow (const TableType *tableP);

void TblGlueSetSelection (TableType *tableP, Int16 row, Int16 column);

#ifdef __cplusplus
}
#endif

#endif
