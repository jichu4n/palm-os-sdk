/*
 * PmUtil.c
 *
 * PmConnector utility functions for PmConnectorTest
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
 * This file contains utility functions for PmConnectorTest
**/

/** @name
 *
**/
/*@{*/

#include <PalmOS.h>

#include <PmConnectorLib.h>

#include "PmUtil.h"
#include "LogUtil.h"
#include "PmConnectorTest_Rsc.h"

#define DockStatusAttached 0x0002 
#define DockStatusExternalPower 0x0004
#define DockStatusCharging 0x0008
#define DockStatusUSBCradle 0x0010
#define DockStatusSerialPeripheral 0x0040
#define DockStatusPower 0x000c


/*********************************************************************
 * Global variables
 *********************************************************************/

static UInt16 pmConnLib = sysInvalidRefNum;

/*********************************************************************/

/*
 * FUNCTION: PmUtilTurnOnConnector
 *
 * DESCRIPTION: Turn connector pins on
 *
 * PARAMETERS:
 */
void PmUtilTurnOnConnector()
{  
    UInt32 dummy = 0;
    Err error = PmConnectorLibControl(pmConnLib, (UInt16)kPmConnectorLibCtrlPowerOn, &dummy);
    if (error)
        FrmCustomAlert(CustomAlert, "Error powering on connector!", 0, 0);
    else { 
        PrintToField("Turned on connector pins");
    }
}

/*
 * FUNCTION: PmUtilTurnOffConnector
 *
 * DESCRIPTION: Turn connector pins off
 *
 * PARAMETERS:
 *
 */
void PmUtilTurnOffConnector(void)
{
    UInt32 dummy = 0;
    Err error = PmConnectorLibControl(pmConnLib, kPmConnectorLibCtrlPowerOff, &dummy);
    if (error)
       FrmCustomAlert(CustomAlert, "Error powering off connector!", 0, 0);
    else { 
       PrintToField("Turned off connector pins");
    }
}

/*
 * FUNCTION: PmUtilDisableDetection
 *
 * DESCRIPTION: Disable class detection
 *
 * PARAMETERS:
 *
 */
void PmUtilDisableClassDetection(void)
{
    UInt16 gPortID = 0;
    Err err = errNone;
    
    err = SrmOpen(serPortCradleRS232Port, 57600, &gPortID);
	if (err != errNone)
	{
		switch (err)
		{
			case serErrAlreadyOpen:
				FrmCustomAlert(CustomAlert, "Port already open!", 0, 0);
				break;

			case serErrBadPort:
				FrmCustomAlert(CustomAlert, "Bad port", 0, 0);
				break;
				
			case memErrNotEnoughSpace:
				FrmCustomAlert(CustomAlert, "Not enough space", 0, 0);
				break;
				
			default:
				FrmCustomAlert(CustomAlert, "Error opening port", 0, 0);
				break;
		}
	}
	else
	{
        err = SrmControl(gPortID, (0x8000 + 256), (void *)NULL, (UInt16 *)NULL);
        if (err != errNone)
        {
            Char msg[10];
            StrPrintF(msg, "%x", err);
            FrmCustomAlert(CustomAlert, "Error setting serial control: ", msg, 0);
        }
        else {
            PrintToField("Class detection disabled");
        }
        SrmClose(gPortID);
    }

}

/*
 * FUNCTION: PmUtilLoadAndOpenLib
 *
 * DESCRIPTION: Load and open the multi-connector library
 *
 * PARAMETERS:
 *
 */
Err PmUtilLoadAndOpenLib(void)
{
    Err error = SysLibFind(kPmConnectorLibName, &pmConnLib);
    
    if (error == sysErrLibNotFound) {
        error = SysLibLoad(kPmConnectorLibType, kPmConnectorLibCreator, &pmConnLib);
    } 
    
    if (!error) {
        error = PmConnectorLibOpen(pmConnLib);
    } 
    
    if (error) { 
        FrmCustomAlert(CustomAlert, "Can't find PmConnector library", 0, 0);
    }
    return error;
}


/*
 * FUNCTION: PmUtilCloseLib
 *
 * DESCRIPTION: Close the multi-connector library
 *
 * PARAMETERS:
 *
 */
void PmUtilCloseLib(void)
{
   Err err = errNone;
   
   if (pmConnLib) {
      	err = PmConnectorLibClose(pmConnLib);
	if ( err != errNone )
	{
		pmConnLib = sysInvalidRefNum;
		return;
	}
	pmConnLib = sysInvalidRefNum;
   }
}


/*
 * FUNCTION: PmUtilGetLibVersion
 *
 * DESCRIPTION: Get PmConnector library version
 *
 * PARAMETERS:
 *
 */
Err PmUtilGetLibVersion(void)
{
    UInt32 libVersion = 0;
    Err error = PmConnectorLibGetVersion(pmConnLib, kPmConnectorLibVersion, &libVersion);
    if (error)
    {
        Char msg[10];
        StrPrintF(msg, "%x", error);
        FrmCustomAlert(CustomAlert, "Error: ", msg, 0);
    }
    else
    {
        UInt16 major = 0;
        UInt16 minor = 0;
        Char version[20];
        
        major = sysGetROMVerMajor(libVersion);
        minor = sysGetROMVerMinor(libVersion);
        
        StrPrintF(version, "%d.%d", major, minor);
        FrmCustomAlert(CustomAlert, "Welcome! PmConnector Library v.", version, 0);
    }
    return error;
}


/*
 * FUNCTION: PmUtilRegisterForNotifications
 *
 * DESCRIPTION: Register for attach/detach notification
 *
 * PARAMETERS:
 *
 */
void PmUtilRegisterForNotifications(void)
{
    UInt16 cardNo = 0;
    LocalID dbID = 0;

    SysCurAppDatabase(&cardNo, &dbID); 
    SysNotifyRegister(cardNo, dbID, sysExternalConnectorAttachEvent, NULL, sysNotifyNormalPriority, NULL);
    SysNotifyRegister(cardNo, dbID, sysExternalConnectorDetachEvent, NULL, sysNotifyNormalPriority, NULL);
}


/*
 * FUNCTION: PmUtilUnegisterForNotifications
 *
 * DESCRIPTION: Unregister for attach/detach notification
 *
 * PARAMETERS:
 *
 */
void PmUtilUnregisterForNotifications(void)
{
    UInt16 cardNo = 0;
    LocalID dbID = 0;

    SysCurAppDatabase(&cardNo, &dbID); 
    SysNotifyUnregister(cardNo, dbID, sysExternalConnectorAttachEvent, sysNotifyNormalPriority);
    SysNotifyUnregister(cardNo, dbID, sysExternalConnectorDetachEvent, sysNotifyNormalPriority);
}


/*
 * FUNCTION: PmUtilGetConnectorType
 *
 * DESCRIPTION: Get connector type
 *
 * PARAMETERS: notifyP
 *
 */
void PmUtilGetConnectorType(SysNotifyParamType *notifyP)
{
   UInt16 connectorType = (UInt16)(notifyP->notifyDetailsP); 
    
   switch (connectorType)
   {
      case DockStatusAttached: 
      	PrintToField("Some peripheral is attached");
      	break;

      case DockStatusExternalPower:
      case DockStatusCharging:
      case DockStatusPower:
      	PrintToField("Power source attached");  
        break;
        
      case DockStatusUSBCradle:
        PrintToField("USB cable or cradle is attached");
        break;
        
      case DockStatusSerialPeripheral:
      	PrintToField("Multi-connector serial peripheral is attached");
      	break;
      	
      default: 
        break;
    }
}


/*
 * FUNCTION: PmUtilPeripheralAttached
 *
 * DESCRIPTION: Handle attached peripheral
 *
 * PARAMETERS: notifyP
 *
 */
void PmUtilPeripheralAttached(SysNotifyParamType *notifyP)
{
    PmUtilGetConnectorType(notifyP); 
    //PrintToField("External connector attached");
}


/*@}*/



