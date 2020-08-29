/****************************************************************************
 * Copyright (c) 2007 Palm, Inc. All rights reserved.
 *
 * File: BtUtil.h
 *
*****************************************************************************/
#include <BtLib.h>

#define BT_RF_MAXFRAMESIZE 400

Err BtUtilDeviceHasBt(UInt16 launchFlags);
Boolean BtUtilLibInit( void );
Boolean BtUtilDiscoverandConnectToDevice();
Boolean BtUtilConnect(BtLibDeviceAddressType *device);
Boolean BtUtilDisconnect();
void BtUtilRegisterService( void );
void BtUtilLibUninit( void );
void BtUtilGetDeviceName();
void BtUtilSocketEstablishRFCOMMConnection();
void BtUtilSocketListen();
void BtUtilSendData();
void BtUtilSocketClose();
Boolean BtUtilLibOpen();
Boolean BtUtilLibIsOpen();
