/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		PadCommn.h
 *
 * Description:
 *		Common Packet Assembly / Disassembly Protocol(PADP) definitions.
 *
 * History:
 *   	8/8/95	vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/


#ifndef __PAD_COMMON_H__
#define __PAD_COMMON_H__

#include <Common.h>


/********************************************************************
 * PADP Packet Format
 ********************************************************************/

// The PADP packets are used for transmitting data for the Pilot PADP protocol layer.
// The structure of the PADP packet is built into the data part of the SLP packet and is,
// therefore, constrained by the SLP packet data size limitations.


// Packet type values used in the PADP packet header's "type" field:
//
typedef enum PadType {
	// 1.0 types
	padData		= 0x01,					// PADP data packet
	padAck		= 0x02,					// PADP ACK packet
	padUnused	= 0x03,					// used to be PADP NACK packet
	padTickle	= 0x04,					// PADP TICKLE packet(keep-alive)
	// 1.1 stuff added here:
	padAbort		= 0x08					// PAD abort connection packet
	} PadType;


// PADP "short" packet header (Pilot v1.0-compatible):
//
typedef struct PadShortHeaderType {
	Byte	type;							// PADP packet type
	Byte	flags;						// flags to signal special conditions
	Word	wSizeOrOffset;				// (high, low) --
											// in the 1st padData packet of the block, this field holds the
											// total block data size; in the subsequent padData packets,
											// this field holds the offset of data being sent; in padAck
											// packets, this field is set to the value of the padData packet
											// being ack'ed.
	} PadShortHeaderType;			// TOTAL: 4 bytes

typedef PadShortHeaderType*		PadShortHeaderPtr;


#if 0		// FUTURE
			// PADP "long" packet header (Future extension):
			//
			typedef struct PadLongHeaderType {
				Byte	type;							// PADP packet type
				Byte	flags;						// flags to signal special conditions
				DWord	dwSizeOrOffset;			// (high, low) --
														// in the 1st padData packet of the block, this field holds the
														// total block data size; in the subsequent padData packets,
														// this field holds the offset of data being sent; in padAck
														// packets, this field is set to the value of the padData packet
														// being ack'ed.
				} PadLongHeaderType;			// TOTAL: 6 bytes

			typedef PadLongHeaderType*		PadLongHeaderPtr;


			// Union of all header types
			typedef union PadGenericHeaderType {
				struct {
					Byte	type;
					Byte	flags;
					}						typeAndFlags;
				PadShortHeaderType	shortHdr;
				PadLongHeaderType		longHdr;
				} PadGenericHeaderType;
				
			// This is the maximum amount of data which may be sent in one PADP block using
			// long packet headers (Future extension)
			//
			#define padMaxLongBlockSize		0xFFFFFFFFL

			#define padHdrFlagLongHeader	0x10	// signals long header (Future extension)

#endif	// FUTURE


// This is the maximum amount of data which may be sent in one PADP block using
// short packet headers
//
#define padMaxShortBlockSize		0x0000FFFFL				// 64K - 1




// Flag values used in the PADP packet header's "flags" field.  padHdrFlagFirst and
// padHdrFlagLast must both be set when the entire data block is being transmitted
// in a single PADP packet.
//
#define padHdrFlagFirst			0x80	// identifies the 1st padData packet
												// of the block
														
#define padHdrFlagLast			0x40	// identifies the last padData packet
												// of the block
														
#define padHdrFlagErrMemory	0x20	// signals remote memory error in
												// padAck packets
														
														


/********************************************************************
 * PADP Protocol Parameters
 ********************************************************************/


// This is the maximum amount of data(in bytes) which may be sent in a single
// PADP packet(not including the PADP packet header)
//
#define padMaxPktDataSize			(1*1024)


// This is the maximum number of retries for resending a given data packet
//
#define	padMaxRetries				10


// This is the maximum number of seconds to wait for an acknowledgement after sending
// a data packet
//
#define padAckWaitSec				2


// This is the maximum number of seconds to wait for the next data packet to begin arriving
//
#define padInterPktTimeOutSec		30		// large value for debugging


// default max sec. to wait for command/reply
//
#define padCmdWaitSec				30
#define padReplyWaitSec				30


// Pilot-to-host tickle(keep alive) interval in seconds
//
#define padPilotToHostTickleIntervalSec	3

/********************************************************************
 * Macros
 ********************************************************************/



#endif	// __PAD_COMMON_H__
