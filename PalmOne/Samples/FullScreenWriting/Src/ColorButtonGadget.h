/***********************************************************************
 *
 * Copyright (c) 2001 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	ColorButtonGadget.h
 *
 * Description:
 *	Color button with push effect and shadow.
 *
 * Version 1.0 	- Initial Revision (03/12/03)
 *
 ***********************************************************************/

#ifndef COLORBUTTONGADGET_H_
#define COLORBUTTONGADGET_H_

#include <PalmOS.h>

/************************************************************
 *
 * Function Prototypes
 *
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Boolean ColorButtonGadgetHandler(FormGadgetTypeInCallback* pGadget, UInt16 cmd, void* pParam);

#ifdef __cplusplus 
}
#endif

#endif // COLORBUTTONGADGET_H_