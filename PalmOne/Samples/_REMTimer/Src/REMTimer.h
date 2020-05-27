/*
 * REMTimer.h
 *
 * header file for REM Timer
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
#ifndef REMTIMER_H_
#define REMTIMER_H_

/*********************************************************************
 * Internal Structures
 *********************************************************************/


/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'remT'
#define appName					"REM Timer"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01


Boolean SleepFormHandleEvent(EventType * eventP);
void RegisterForNotification(UInt32 notifyType);
void UnregisterForNotification(UInt32 notifyType);
void TurnDeviceOff(void);
void * GetObjectPtr(UInt16 objectID);
void MainFormInit(FormType *frmP);
Boolean MainFormDoCommand(UInt16 command);
Boolean MainFormHandleEvent(EventType * eventP);
Boolean SleepFormHandleEvent(EventType * eventP);
Boolean AppHandleEvent(EventType * eventP);
void AppEventLoop(void);
Err AppStart(void);
void AppStop(void);
Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags);
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags);

#endif /* REMTIMER_H_ */
