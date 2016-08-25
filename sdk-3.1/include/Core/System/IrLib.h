/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1997, Palm Computing Inc., All Rights Reserved  
 *
 *-------------------------------------------------------------------
 * FileName:
 *		IrLib.h
 *
 * Description:
 *		Include file for IrDA Library
 *
 * History:
 *   	5/23/97 Created by Gavin Peacock
 *
 *******************************************************************/

#ifndef __IR_LIB_H
#define __IR_LIB_H

// The Ir library is used mainly as an Obx library. ObxLib.h defines all the
// primary entrypoints into the library. The rest of this include file defines the
// direct stack API for apps not using the OBX interface. The Stack API comes after 
// the Obx API in the library.
#include <ExgLib.h>

// include counterpoint library headers - fix this later so that we only include
// what is needed for our API...
#include <irias.h>

// name of Ir library
#define irLibName "IrDA Library"

// Feature Creators and numbers, for use with the FtrGet() call. This
//  feature can be obtained to get the current version of the Ir Library
#define		irFtrCreator			sysFileCIrLib
#define		irFtrNumVersion		0				// get version of Net Library
			// 0xMMmfsbbb, where MM is major version, m is minor version
			// f is bug fix, s is stage: 3-release,2-beta,1-alpha,0-development,
			// bbb is build number for non-releases 
			// V1.12b3   would be: 0x01122003
			// V2.00a2   would be: 0x02001002
			// V1.01     would be: 0x01013000


// Options values for IrOpen
#define irOpenOptBackground		0x80000000
#define irOpenOptSpeed115200	IRLAP_SPEED115200
#define irOpenOptSpeed57600		IRLAP_SPEED57600
#define irOpenOptSpeed9600		IRLAP_SPEED9600

// Option codes for ExgLibControl
// These options are all unique to the Ir transport
#define irGetScanningMode	exgLibCtlSpecificOp | 1  // returns scanning enabled
#define irSetScanningMode	exgLibCtlSpecificOp | 2	 // en/disables ir scanning mode
#define irGetStatistics		exgLibCtlSpecificOp | 3	 // returns performance stats
#define irSetSerialMode		exgLibCtlSpecificOp | 4	 // sets driver to use direct serial
#define irSetBaudMask		exgLibCtlSpecificOp | 5	 // set possible baud rates (irOpenOpts)
#define irSetSupported		exgLibCtlSpecificOp | 6	 // disables the ir not supported dialog

// structure returned by irGetStatistics
typedef struct {
	Word recLineErrors;		// # of serial errors since library opend
	Word crcErrors;			// # of crc errors ...
} IrStatsType;

//-----------------------------------------------------------------------------
// Ir library call ID's. Each library call gets the trap number:
//   irLibTrapXXXX which serves as an index into the library's dispatch table.
//   The constant sysLibTrapCustom is the first available trap number after
//   the system predefined library traps Open,Close,Sleep & Wake.
//
// WARNING!!! This order of these traps MUST match the order of the dispatch
//  table in IrLibDispatch.c!!!
//-----------------------------------------------------------------------------
#pragma mark Traps
typedef enum {
	irLibTrapBind = exgLibTrapLast,  // these start after the ObxLib interface...
	irLibTrapUnBind,
	irLibTrapDiscoverReq,
	irLibTrapConnectIrLap,
	irLibTrapDisconnectIrLap,
	irLibTrapConnectReq,
	irLibTrapConnectRsp,
	irLibTrapDataReq,
	irLibTrapLocalBusy,
	irLibTrapMaxTxSize,
	irLibTrapMaxRxSize,
	irLibTrapSetDeviceInfo,
	irLibTrapIsNoProgress,
	irLibTrapIsRemoteBusy,
	irLibTrapIsMediaBusy,
	irLibTrapIsIrLapConnected,
	irLibTrapTestReq,
	irLibTrapIAS_Add,
	irLibTrapIAS_Query,
	irLibTrapIAS_SetDeviceName,
	irLibTrapIAS_Next,
	irLibTrapIrOpen,
	irLibTrapHandleEvent,
	irLibTrapWaitForEvent,
	
	irLibTrapLast
	} IrLibTrapNumberEnum;


/************************************************************
 * Net Library procedures.
 *************************************************************/ 
#pragma mark Functions
#ifdef __cplusplus
extern "C" {
#endif

#if EMULATION_LEVEL != EMULATION_NONE
// Private entrypoint used by emulator to install library
Err	IrdPrvInstallDispatcher(UInt refNum, SysLibTblEntryPtr entryP);
#endif
					

IrStatus IrBind(UInt refNum,IrConnect* con, IrCallBack callBack)
			SYS_TRAP(irLibTrapBind);

IrStatus IrUnbind(UInt refNum,IrConnect* con)
			SYS_TRAP(irLibTrapUnBind);

IrStatus IrDiscoverReq(UInt refNum,IrConnect* con)
			SYS_TRAP(irLibTrapDiscoverReq);

IrStatus IrConnectIrLap(UInt refNum,IrDeviceAddr deviceAddr)
			SYS_TRAP(irLibTrapConnectIrLap);

IrStatus IrDisconnectIrLap(UInt refNum)
			SYS_TRAP(irLibTrapDisconnectIrLap);

IrStatus IrConnectReq(UInt refNum,IrConnect* con, IrPacket* packet, U8 credit) 
			SYS_TRAP(irLibTrapConnectReq);

IrStatus IrConnectRsp(UInt refNum,IrConnect* con,IrPacket* packet, U8 credit) 
			SYS_TRAP(irLibTrapConnectRsp);

IrStatus IrDataReq(UInt refNum,IrConnect* con, IrPacket* packet)
			SYS_TRAP(irLibTrapDataReq);

void IrLocalBusy(UInt refNum,BOOL flag)
			SYS_TRAP(irLibTrapLocalBusy);

U16 IrMaxTxSize(UInt refNum,IrConnect* con)
			SYS_TRAP(irLibTrapMaxTxSize);

U16 IrMaxRxSize(UInt refNum,IrConnect* con)
			SYS_TRAP(irLibTrapMaxRxSize);

IrStatus IrSetDeviceInfo(UInt refNum,U8* info, U8 len)
			SYS_TRAP(irLibTrapSetDeviceInfo);

BOOL IrIsNoProgress(UInt refNum)
			SYS_TRAP(irLibTrapIsNoProgress);

BOOL IrIsRemoteBusy(UInt refNum)
			SYS_TRAP(irLibTrapIsRemoteBusy);

BOOL IrIsMediaBusy(UInt refNum)
			SYS_TRAP(irLibTrapIsMediaBusy);

BOOL IrIsIrLapConnected(UInt refNum)
			SYS_TRAP(irLibTrapIsIrLapConnected);
			
IrStatus IrTestReq(UInt refNum,IrDeviceAddr devAddr, IrConnect* con, IrPacket* packet) 
			SYS_TRAP(irLibTrapTestReq);


IrStatus IrIAS_Add(UInt refNum,IrIasObject* obj)
			SYS_TRAP(irLibTrapIAS_Add);

IrStatus IrIAS_Query(UInt refNum,IrIasQuery* token)
			SYS_TRAP(irLibTrapIAS_Query);

IrStatus IrIAS_SetDeviceName(UInt refNum,U8* name, U8 len)
			SYS_TRAP(irLibTrapIAS_SetDeviceName);

U8* IrIAS_Next(UInt refNum,IrIasQuery* token)
			SYS_TRAP(irLibTrapIAS_Next);

Err IrOpen(UInt refNum,ULong options)
			SYS_TRAP(irLibTrapIrOpen);

Err	IrClose (Word libRefnum)
			SYS_TRAP(sysLibTrapClose);
					
Boolean IrHandleEvent(Word libRefnum)
			SYS_TRAP(irLibTrapHandleEvent);

Err IrWaitForEvent(Word libRefnum,Long timeout)
			SYS_TRAP(irLibTrapWaitForEvent);


#endif  // IR_LIB_H


