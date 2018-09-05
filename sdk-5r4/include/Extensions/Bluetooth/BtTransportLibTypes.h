/******************************************************************************
 *
 * Copyright (c) 1998-2004 PalmSource, Inc. All rights reserved.
 *
 * File: BtTransportLibTypes.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *          	Public Bluetooth HCI transport definitions.
 *
 *****************************************************************************/

#ifndef _BTTRANSPORTLIBTYPES_H
#define _BTTRANSPORTLIBTYPES_H

//============================================================================
// Transport driver definitions
//============================================================================
// Transport driver libraries are standard libraries, except that type uses
// the sysFileCBtTransLib ('bttx') creator ID, instead of the standard 'libr'.
// This facilitates building a dynamic list of available transport drivers.
// Specific transport drivers are identified by a combination of creator ID
// and library name. If more than one library is available, a preference must
// be established.

#ifndef sysFileCBtTransLib
#define sysFileCBtTransLib							'bttx'
#endif

#ifndef sysFileTBtTransLib
#define sysFileTBtTransLib							sysFileCBtTransLib
#endif



//============================================================================
// Assigned numbers (as of 22-February-2001)
//============================================================================

#define anLMPCompIDEricsson						0
#define anLMPCompIDNokia							1
#define anLMPCompIDIntel							2
#define anLMPCompIDIBM								3
#define anLMPCompIDToshiba							4
#define anLMPCompID3Com								5
#define anLMPCompIDMicrosoft						6
#define anLMPCompIDLucent							7
#define anLMPCompIDMotorola						8
#define anLMPCompIDInfineon						9
#define anLMPCompIDCambridge						10
#define anLMPCompIDSiliconWave					11
#define anLMPCompIDDigianswer						12
#define anLMPCompIDLast								12

#define anLMPVersion_1_0							0
#define anLMPVersion_1_1							1

// The PnP manufacturer is a 16 bit number returned as a 4 character hex string!
// 'SW' (0x5357) is returned as "5357\0"
// Media type will be autoRunMediaPnps ('pnps')
#define anPnPManSiliconWave						"5357"	// 'SW' assigned
#define anPnPManCambridge							"4341"	// 'CA' TBD

// SDIO identification info.
// Media type will be autoRunMediaSDIO ('sdio')
#define anSDIOOEMManToshibaV0X					0x00004321	// Toshiba SD v0.7c and v0.96a (temporary)
#define anSDIOOEMManToshibaV10					0x00000296	// Toshiba SD v1.0 (Palm)
#define anSDIOOEMIdToshibaV10						0x00000001	// Toshiba SD v1.0 (Palm)
#define anSDIOOEMFuncToshiba						1				// Toshiba SD on function number 1


//============================================================================
// Interface definitions
//============================================================================

#define btTransportPacketTypeCommand			1
#define btTransportPacketTypeACLData			2
#define btTransportPacketTypeSCOData			3
#define btTransportPacketTypeEvent				4

#define btTransportPutNoData						1
#define btTransportPutGoodData					2
#define btTransportPutBadData						3

#define btTransportEventAttach					1
#define btTransportEventFakeAttach				2
#define btTransportEventDetach					3
#define btTransportEventFakeDetach				4
#define btTransportEventAttention				5
#define btTransportEventOpen						6
#define btTransportEventClose						7
#define btTransportEventSleep						8
#define btTransportEventWake						9
#define btTransportEventError						10
#define btTransportEventDetachClose				11
#define btTransportEventSleepSchedule 	12
#define btTransportEventLast						12

#define btTransportStatusOSAttached				0x00000001	// Attach registered by OS
#define btTransportStatusFakeAttached			0x00000002	// Attach implied
#define btTransportStatusSleeping				0x00000004	// Transport is sleeping
#define btTransportStatusYielded					0x00000008	// Transport is yielded
#define btTransportStatusOpened					0x00000010	// Transport is opened
#define btTransportStatusInfo						0x00000020	// Transport has acquired radio info
#define btTransportStatusHCIReversed			0x00000040	// HCI host and host controller direction reversed 
#define btTransportStatusExpectAttention		0x00000080	// Expecting attention from radio
#define btTransportStatusPresent					0x00000100	// Hardware has been detected
#define btTransportStatusDisabled				0x00000200	// Transport has been disabled
#define btTransportStatusSwitchablePower		0x08000000	// Radio power can be switched on and off
#define btTransportStatusInboundOnly			0x10000000	// Can only be used in response to an attention
#define btTransportStatusFakeAttachable		0x20000000	// The radio can be passively detected
#define btTransportStatusAttachable				0x40000000	// The radio is attachable (sled or slot)
#define btTransportStatusEmbedded				0x80000000	// The radio is embedded (always attached)
#define btTransportStatusAttached				(btTransportStatusOSAttached|btTransportStatusFakeAttached)

#define btTransportPowerModeOnOkay				1
#define btTransportPowerModeOn					2
#define btTransportPowerModeOff					3

#define btTransportControlGetTransportInfo	1		// paramP -> BtTransportInfoType
#define btTransportControlGetAttachInfo		2		// paramP -> BtTransportAttachInfoType
#define btTransportControlGetHardwareInfo		3		// paramP -> BtTransportHardwareInfoType
#define btTransportControlSetPowerMode			4		// paramP -> BtTransportPowerType
#define btTransportControlSendBaseband			5		// paramP -> BtTransportBasebandType
#define btTransportControlSleep					6		// paramP -> none
#define btTransportControlWake					7		// paramP -> none
#define btTransportControlUnload					8		// paramP -> none
#define btTransportControlSetExpectAttention	9		// paramP -> UInt16 (0/1)
#define btTransportControlSetDebugMask			10		// paramP -> UInt16 (debug mask)
#define btTransportControlGetHistogram			11		// paramP -> BtTransportHistogramType
#define btTransportControlCustom					100	// paramP -> ???

// Retrieve transport info
typedef struct {
	UInt32			creatorID;						// CreatorID of this transport
	UInt32			status;							// status of this transport (dynamic)
	Err				lastError;						// last transport error (not automatically cleared)
	Char				*transportIDStringP;			// pointer to buffer to receive name (pass null if not wanted)
} BtTransportInfoType;

// Retrieve attach info
typedef struct {
	UInt16			slotRefNum;
	UInt16			slotLibRefNum;
	UInt32			mediaID;
} BtTransportAttachInfoType;

// Retrieve hardware info
typedef struct {
	UInt16			version;
	UInt16			revision;
	UInt16			lmpVersion;
	UInt16			manufacturer;
	UInt16			lmpSubversion;
	UInt8				lmpFeatures[8];
	UInt16			aclPacketLength;
	UInt16			scoPacketLength;
	UInt16			aclTotalPackets;
	UInt16			scoTotalPackets;
	UInt16			country;
	UInt8				bdAddr[6];
} BtTransportHardwareInfoType;

// Set power mode
typedef struct {
	UInt8				powerMode;
} BtTransportPowerType;

// Send HCI frame via transport
typedef struct {
	UInt8				*txBufferP;
	UInt16			txBufferLen;
	UInt8				*rxBufferP;
	UInt16			*rxBufferLenP;
	UInt16			waitMS;
} BtTransportBasebandType;

// Retrieve histogram info
typedef struct {
	UInt8				*rxSizeP;
	UInt32			rxCount;
	UInt8				*txSizeP;
	UInt32			txCount;
	UInt16			size;
} BtTransportHistogramType;

// Event structure passed in transport event callback
typedef struct BtTransportEventType {
														//	source		comment
														//	------		-------
	UInt16			libRefNum;					// transport	refNum of library generating the event
	UInt16			event;						// transport	event type (btTransportEventXXX)
	UInt8				handled;						// transport	true if event has been handled
	UInt8				transportIndex;			// transport	feature index of this transport
	union {
		struct {
			UInt16	slot;							// transport	slotRefNum
		} attach;
		struct {
			UInt16	slot;							// transport	slotRefNum
		} detach;
		struct {
			UInt16	slot;							// transport	slotRefNum
		} attention;
		struct {
			Err		err;							// transport	actual system error
		} error;
	} data;
} BtTransportEventType;

typedef void (*btTransportGetProcP)(UInt32 refCon, void **bufHandlePP, void **dataHandlePP, UInt8 type, UInt16 length);
typedef void (*btTransportPutProcP)(UInt32 refCon, void *bufHandle, UInt16 length, UInt16 putType);
typedef void (*btTransportSentProcP)(UInt32 refCon, void *bufHandle);
typedef void (*btTransportErrorProcP)(UInt32 refCon, Err err);

typedef struct {
	UInt32						refCon;
	btTransportGetProcP		getProcP;
	btTransportPutProcP		putProcP;
	btTransportSentProcP		sentProcP;
	btTransportErrorProcP	errProcP;
} BtTransportProcSpecType;


//============================================================================
// Library error codes
//============================================================================

#ifndef btTransportErrorClass					// may be defined in <ErrorBase.h>
#define btTransportErrorClass					blthErrorClass
#endif

// Generic no-error and error
#define btTransportErrNoError					(0)
#define btTransportErrError					(btTransportErrorClass | 0xFF)
// Transport errors are errorBase + 0x00E0..0x00FE
#define btTransportErrAlreadyOpen			(btTransportErrorClass | (0xE0 + 0x01))	// 0xE1
#define btTransportErrFeatureMissing		(btTransportErrorClass | (0xE0 + 0x02))	// 0xE2
#define btTransportErrNotOpen					(btTransportErrorClass | (0xE0 + 0x03))	// 0xE3
#define btTransportErrOutOfMemory			(btTransportErrorClass | (0xE0 + 0x04))	// 0xE4
#define btTransportErrBadParameter			(btTransportErrorClass | (0xE0 + 0x05))	// 0xE5
#define btTransportErrUnknownControl		(btTransportErrorClass | (0xE0 + 0x06))	// 0xE6
#define btTransportErrNotAttached			(btTransportErrorClass | (0xE0 + 0x07))	// 0xE7
#define btTransportErrTimeout					(btTransportErrorClass | (0xE0 + 0x08))	// 0xE8
#define btTransportErrMaxTxRetries			(btTransportErrorClass | (0xE0 + 0x09))	// 0xE9
#define btTransportErrLineError				(btTransportErrorClass | (0xE0 + 0x0A))	// 0xEA
#define btTransportErrBadData					(btTransportErrorClass | (0xE0 + 0x0B))	// 0xEB
#define btTransportErrNoBuffer				(btTransportErrorClass | (0xE0 + 0x0C))	// 0xEC
#define btTransportErrNoData					(btTransportErrorClass | (0xE0 + 0x0D))	// 0xED
#define btTransportErrWrongHardware			(btTransportErrorClass | (0xE0 + 0x0E))	// 0xEE
#define btTransportErrBadResponse			(btTransportErrorClass | (0xE0 + 0x0F))	// 0xEF
#define btTransportErrRadioReset				(btTransportErrorClass | (0xE0 + 0x10))	// 0xF0
#define btTransportErrDisabled				(btTransportErrorClass | (0xE0 + 0x11))	// 0xF1
#define btTransportErrInboundOnly			(btTransportErrorClass | (0xE0 + 0x12))	// 0xF2


//============================================================================
// Library trap values
//============================================================================

#define btTransportLibTrapReserved			(sysLibTrapCustom+0)
#define btTransportLibTrapSend				(sysLibTrapCustom+1)
#define btTransportLibTrapControl			(sysLibTrapCustom+2)


#endif	// _BTTRANSPORTLIBTYPES_H
