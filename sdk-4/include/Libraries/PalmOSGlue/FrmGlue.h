/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: FrmGlue.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *		Glue providing compatibility for applications that wish
 *		to make calls to the Form Manager, but which might be running
 *		on a system which does not support newer calls.
 *
 * History:
 *		08/29/00	kwk	Created by Ken Krugler.
 *		01/08/01	tlw	Added FrmGlueGetObjectUsable.
 *
 *****************************************************************************/

#ifndef __FRMGLUE_H__
#define __FRMGLUE_H__

#include <Field.h>
#include <Form.h>

#ifdef __cplusplus
	extern "C" {
#endif

FieldType* FrmGlueGetActiveField(const FormType* formP);
Boolean FrmGlueGetObjectUsable(const FormType *formP, UInt16 objIndex);

#ifdef __cplusplus
	}
#endif

#endif	// __FRMGLUE_H__
