/*
 * PmUtil.h
 *
 * header file for PmConnectorTest
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */

/** @ingroup PmConnectorTest
 *
**/

/** @file PmUtil.h
 * This file is the header for PmConnector utility functions
**/

/** @name
 *
**/
/*@{*/

void PmUtilTurnOnConnector(void);
void PmUtilTurnOffConnector(void);
void PmUtilDisableClassDetection(void);
Err  PmUtilGetLibVersion(void);
Err  PmUtilLoadAndOpenLib(void);
void PmUtilCloseLib(void);
void PmUtilRegisterForNotifications(void);
void PmUtilUnregisterForNotifications(void);
void PmUtilPeripheralAttached(SysNotifyParamType *notifyP);
void PmUtilGetConnectorType(SysNotifyParamType *notifyP);

/*@}*/

