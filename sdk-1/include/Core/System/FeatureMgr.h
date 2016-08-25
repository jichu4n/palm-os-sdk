/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		FeatureMgr.h
 *
 * Description:
 *		Header for the Feature Manager
 *
 * History:
 *   	11/14/94  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifndef __FEATUREMGR_H__
#define __FEATUREMGR_H__


#include <Common.h>



/************************************************************
 * Feature manager error codes
 * the constant ftrErrorClass is defined in SystemMgr.h
 *************************************************************/
#define	ftrErrInvalidParam				(ftrErrorClass | 1)
#define	ftrErrNoSuchFeature				(ftrErrorClass | 2)
#define	ftrErrAlreadyExists				(ftrErrorClass | 3)
#define	ftrErrROMBased						(ftrErrorClass | 4)
#define	ftrErrInternalErr					(ftrErrorClass | 5)


/************************************************************
 * Feature Manager procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


// Init the feature Manager
Err		FtrInit(void)
							SYS_TRAP(sysTrapFtrInit);


// Get a feature
Err		FtrGet(DWord creator, UInt featureNum, DWordPtr valueP)
							SYS_TRAP(sysTrapFtrGet);

// Set/Create a feature.
Err		FtrSet(DWord creator, UInt featureNum, DWord newValue)
							SYS_TRAP(sysTrapFtrSet);

// Unregister a feature
Err		FtrUnregister(DWord creator, UInt featureNum)
							SYS_TRAP(sysTrapFtrUnregister);

// Get a feature by index
Err		FtrGetByIndex(UInt index, Boolean romTable, 
					DWordPtr creatorP, UIntPtr numP, DWordPtr valueP)
							SYS_TRAP(sysTrapFtrGetByIndex);



#ifdef __cplusplus
}
#endif

#endif // __FEATUREMGR_H__

