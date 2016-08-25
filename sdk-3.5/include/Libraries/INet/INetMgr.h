/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: INetMgr.h
 *
 * Description:
 *	  This header file contains equates for the Internet Library. 
 *
 * History:
 *		6/2/97	Created by Ron Marianetti
 *		12/23/99	Fix <> vs. "" problem. (jmp)
 *
 *****************************************************************************/

#ifndef 	__INETLIB_H__
#define	__INETLIB_H__

#include <PalmTypes.h>
#include <LibTraps.h>
#include <DataMgr.h>
#include <SystemResources.h>

#pragma mark Constants

// Creator. Used for both the database that contains the INet Library and
//  it's features for the feature manager. 
#define	inetCreator				sysFileCINetLib		// The Net Library creator

// INet Library features have this creator 
#define	inetLibFtrCreator		sysFileCINetLib		// creatorID of INet Lib features. 

// Name of the InetLib
#define	inetLibName				"INet.lib"				// pass in to SysLibFind()


// Feature inetCreator, #0 is index of the the version number feature.
// The Feature creator is inetLibFtrCreator.
// Encoding is: 0xMMmfsbbb, where MM is major version, m is minor version
// f is bug fix, s is stage: 3-release,2-beta,1-alpha,0-development,
// bbb is build number for non-releases 
// V1.12b3   would be: 0x01122003
// V2.00a2   would be: 0x02001002
// V1.01     would be: 0x01013000
#define  inetFtrNumVersion		0

#define	inetLibType				sysFileTLibrary		// Our Net Code Resources Database type


// ID for proxy IP address in flash
#define	inetFlashProxyID				'IP'
#define	inetDefaultFlashProxyID		'DP'
//Also uses mobitexNetworkIdUS and mobitexNetworkIdCanada (0xb433 and 0xc4d7) to store
//current proxies for US and Canada. The responsibility for writing these and keeping
//them in sync lies with the Wireless panel, not with netlib.

//-----------------------------------------------------------------------------
// IP addresses of Elaine servers - used for default wireless proxies
//-----------------------------------------------------------------------------
#define netProxyIPManhattanHGA			0x0A0186A5			// Manhattan HGA = 10.1.134.165 or MAN 100005
#define	netProxyIPTuscanyHGA				0x0A0186A3 			// Tuscany HGA = 10.1.134.163 or MAN 100003
#define	netProxyIPRonsHGA					0x0A0186A4 			// Ron's HGA = 10.1.134.164 or MAN 100004
#define netProxyIPDefaultHGA				netProxyIPManhattanHGA
#define netProxyIPDefaultHGAStr			"10.1.134.165"		//Should correspond to above value
#define netProxyIPDefaultHGACanada				netProxyIPManhattanHGA
#define netProxyIPDefaultHGAStrCanada			"10.1.134.165"		//Should correspond to above value


#pragma mark ErrorCodes
/********************************************************************
 * Error codes
 ********************************************************************/
#define 	inetErrNone							0
#define	inetErrTooManyClients			(inetErrorClass | 1)		// Too many clients already
#define	inetErrHandleInvalid				(inetErrorClass | 2)		// Invalid inetH or sockH
#define	inetErrParamsInvalid				(inetErrorClass | 3)		//  
#define	inetErrURLVersionInvalid		(inetErrorClass | 4)		// 
#define	inetErrURLBufTooSmall			(inetErrorClass | 5)		// 
#define	inetErrURLInvalid					(inetErrorClass | 6)		// 
#define	inetErrTooManySockets			(inetErrorClass | 7)		// 
#define	inetErrNoRequestCreated			(inetErrorClass | 8)
#define	inetErrNotConnected				(inetErrorClass | 9)
#define	inetErrInvalidRequest			(inetErrorClass | 10)
#define	inetErrNeedTime					(inetErrorClass | 11)
#define	inetErrHostnameInvalid			(inetErrorClass | 12)
#define	inetErrInvalidPort				(inetErrorClass | 13)
#define	inetErrInvalidHostAddr			(inetErrorClass | 14)
#define	inetErrNilBuffer					(inetErrorClass | 15)
#define	inetErrConnectTimeout			(inetErrorClass | 16)
#define	inetErrResolveTimeout			(inetErrorClass | 17)
#define	inetErrSendReqTimeout			(inetErrorClass | 18)
#define	inetErrReadTimeout				(inetErrorClass | 19)
#define	inetErrBufTooSmall				(inetErrorClass | 20)
#define	inetErrSchemeNotSupported		(inetErrorClass | 21)
#define	inetErrInvalidResponse			(inetErrorClass | 22)
#define	inetErrSettingTooLarge			(inetErrorClass | 25)
#define	inetErrSettingSizeInvalid		(inetErrorClass | 26)
#define	inetErrRequestTooLong			(inetErrorClass | 27)
#define	inetErrSettingNotImplemented	(inetErrorClass | 28)

// Configuration errors
#define	inetErrConfigNotFound			(inetErrorClass | 29)
#define	inetErrConfigCantDelete			(inetErrorClass | 30)
#define	inetErrConfigTooMany				(inetErrorClass | 31)
#define	inetErrConfigBadName				(inetErrorClass | 32)
#define	inetErrConfigNotAlias			(inetErrorClass | 33)
#define	inetErrConfigCantPointToAlias	(inetErrorClass | 34)
#define	inetErrConfigEmpty				(inetErrorClass | 35)
#define	inetErrConfigAliasErr			(inetErrorClass | 37)

#define	inetErrNoWirelessInterface		(inetErrorClass | 38)

// Encryption related errors
#define	inetErrEncryptionNotAvail		(inetErrorClass | 39)
	// Need to re-send transaction because server told us to reset our
	//  encryption sequence number
#define	inetErrNeedRetryEncSeqNum		(inetErrorClass | 40)
	// Need to re-send transaction because server sent us a new
	//  public key to use. 
#define	inetErrNeedRetryEncPublicKey	(inetErrorClass | 41)

#define	inetErrResponseTooShort			(inetErrorClass | 42)

// errors specific to handling Mobitex ILLEGAL responses
#define	inetErrMobitexIllegalOKHost	(inetErrorClass | 43)
#define	inetErrMobitexIllegalBadHost	(inetErrorClass | 44)
// see error 92 also

// HTTP errors
#define	inetErrHTTPBadRequest			(inetErrorClass | 45)
#define	inetErrHTTPUnauthorized			(inetErrorClass | 46)
#define	inetErrHTTPForbidden				(inetErrorClass | 47)
#define	inetErrHTTPNotFound				(inetErrorClass | 48)
#define	inetErrHTTPMethodNotAllowed	(inetErrorClass | 49)
#define	inetErrHTTPNotAcceptable		(inetErrorClass | 50)
#define	inetErrHTTPProxyAuthRequired	(inetErrorClass | 51)
#define	inetErrHTTPRequestTimeout		(inetErrorClass | 52)
#define	inetErrHTTPConflict				(inetErrorClass | 53)
#define	inetErrHTTPGone					(inetErrorClass | 54)
#define	inetErrHTTPLengthRequired		(inetErrorClass | 55)
#define	inetErrHTTPPreconditionFailed	(inetErrorClass | 56)
#define	inetErrHTTPRequestTooLarge		(inetErrorClass | 57)
#define	inetErrHTTPRequestURITooLong	(inetErrorClass | 58)
#define	inetErrHTTPUnsupportedType		(inetErrorClass | 59)
#define	inetErrHTTPServerError			(inetErrorClass | 60)

// CTP errors
#define	inetErrCTPServerError			(inetErrorClass | 61)

// Cache errors
#define	inetErrTypeNotCached				(inetErrorClass | 62)
#define 	inetErrCacheInvalid				(inetErrorClass | 63)

// Palm: and PalmCall: scheme errors
#define	inetErrURLDispatched				(inetErrorClass | 64)
#define 	inetErrDatabaseNotFound			(inetErrorClass | 65)

#define 	inetErrCTPMalformedRequest		(inetErrorClass | 66)
#define 	inetErrCTPUnknownCommand		(inetErrorClass | 67)
#define 	inetErrCTPTruncated				(inetErrorClass | 68)
#define 	inetErrCTPUnknownError			(inetErrorClass | 69)
#define 	inetErrCTPProxyError				(inetErrorClass | 70)
#define 	inetErrCTPSocketErr				(inetErrorClass | 71)
    
#define 	inetErrCTPInvalidURL				(inetErrorClass | 72)
#define 	inetErrCTPReferringPageOutOfDate (inetErrorClass | 73)
#define 	inetErrCTPBadRequest				(inetErrorClass | 74)
#define 	inetErrUNUSED					 	(inetErrorClass | 75)
#define 	inetErrCTPMailServerDown		(inetErrorClass | 76)
#define 	inetErrCTPHostNotFound			(inetErrorClass | 77)


// Content Conversion Errors
#define 	inetErrCTPContentInvalidTag       (inetErrorClass | 78)
#define 	inetErrCTPContentInternal         (inetErrorClass | 79)
#define 	inetErrCTPContentDataEnd          (inetErrorClass | 80)
#define 	inetErrCTPContentResourceTooBig   (inetErrorClass | 81)
#define 	inetErrCTPContentNoNoFrames       (inetErrorClass | 82)
#define 	inetErrCTPContentUnsupportedContent 	(inetErrorClass | 83)
#define 	inetErrCTPContentUnsupportedEncoding 	(inetErrorClass | 84)
#define 	inetErrCTPContentBadForm          		(inetErrorClass | 85)
#define 	inetErrCTPContentBadFormMissingAction 	(inetErrorClass | 86)
#define 	inetErrCTPContentBadFormMissingMethod 	(inetErrorClass | 87)
#define 	inetErrCTPContentNoSourceData     (inetErrorClass | 88)
#define 	inetErrCTPContentBadImage         (inetErrorClass | 89)
#define 	inetErrCTPContentImageTooLarge    (inetErrorClass | 90)

// Mobitex illegal handled error code.  This error is sent after
//INetLib handles inetErrMobitexIllegalOKHost or inetErrMobitexIllegalBadHost
//errors.  The application needs to know that something went wrong and it needs
//to change state.  This error does not need to be displayed to the user.
#define	inetErrMobitexErrorHandled		(inetErrorClass | 91)

// Proxy down, non-default host, show dialog asking to revert to default
#define	inetErrProxyDownBadHost			(inetErrorClass | 92)

// A second type of readtime.  This should occur only when some data is received
// and the connection is lost.
#define	inetErrHostConnectionLost		(inetErrorClass | 93)

// Unable to locate link record within a PQA file
#define 	inetErrLinkNotFound				(inetErrorClass | 94)
//

#pragma mark Types

/********************************************************************
 * Input flags
 ********************************************************************/
//-----------------------------------------------------------------------------
// flag word definitions for INetLibURLOpen
//-----------------------------------------------------------------------------
#define	inetOpenURLFlagLookInCache	0x0001
#define  inetOpenURLFlagKeepInCache	0x0002
#define	inetOpenURLFlagForceEncOn  0x0008 // use encryption even if 
														 //  scheme does not desire it
#define	inetOpenURLFlagForceEncOff	0x0010 // no encryption even if 
														 //  scheme desires it

//-----------------------------------------------------------------------------
// flag word definitions for INetURLInfo. These flags bits are set in the
//   flags field of the INetURLINfoType structure by INetLibURLGetInfo()
//-----------------------------------------------------------------------------
#define	inetURLInfoFlagIsSecure		0x0001
#define  inetURLInfoFlagIsRemote		0x0002
#define  inetURLInfoFlagIsInCache	0x0004


/********************************************************************
 * Configuration Support
 ********************************************************************/
//-----------------------------------------------------------------------------
// Names of built-in configuration aliases available through the
//  INetLibConfigXXX calls
//-----------------------------------------------------------------------------
#define	inetCfgNameDefault		".Default"		// The default configuration
#define	inetCfgNameDefWireline	".DefWireline"	// The default wireline configuration
#define	inetCfgNameDefWireless	".DefWireless"	// The default wireless configuration
#define	inetCfgNameCTPDefault	".CTPDefault"	// Points to either .CTPWireline or .CTPWireless
#define	inetCfgNameCTPWireline	".CTPWireline"	// Wireline through the Jerry Proxy
#define	inetCfgNameCTPWireless	".CTPWireless"	// Wireless through the Jerry Proxy


//--------------------------------------------------------------------
// Structure of a configuration name. Used by INetLibConfigXXX calls
//---------------------------------------------------------------------
#define	inetConfigNameSize		32
typedef struct {
	Char			name[inetConfigNameSize];			  // name of configuration
	} INetConfigNameType, *INetConfigNamePtr;




/********************************************************************
 * Scheme Types
 ********************************************************************/
typedef enum {
	inetSchemeUnknown = -1,
	inetSchemeDefault = 0,
	
	inetSchemeHTTP,								// http:
	inetSchemeHTTPS,								// https:
	inetSchemeFTP,									// ftp:
	inetSchemeGopher,								// gopher:
	inetSchemeFile,								// file:
	inetSchemeNews,								// news:
	inetSchemeMailTo,								// mailto:
   inetSchemePalm,								// palm:
   inetSchemePalmCall,							// palmcall:
   
	inetSchemeMail,								// not applicable to URLS, but used
														//  for the INetLibSockOpen call when
														//  creating a socket for mail IO
	inetSchemeMac,									// mac: - Mac file system HTML													
	
	inetSchemeFirst = inetSchemeHTTP,		// first one
	inetSchemeLast	= inetSchemeMail			// last one
	} INetSchemeEnum;


/********************************************************************
 * Scheme Ports
 ********************************************************************/
#define	inetPortFTP		21
#define	inetPortHTTP	80
#define	inetPortGopher	70
#define	inetPortNews	119
#define	inetPortHTTPS	44



/********************************************************************
 * Structure of a cracked URL. 
 ********************************************************************/
typedef struct {
	UInt16	version;								// should be 0, for future compatibility
	
	UInt8 *	schemeP;								// ptr to scheme portion
	UInt16	schemeLen;							// size of scheme portion
	UInt16	schemeEnum;							// INetSchemEnum
	
	UInt8 *	usernameP;							// ptr to username portion
	UInt16	usernameLen;						// size of username
	
	UInt8 *	passwordP;							// ptr to password portion
	UInt16	passwordLen;						// size of password
	
	UInt8 *	hostnameP;							// ptr to host name portion
	UInt16	hostnameLen;						// size of host name
	
	UInt16	port;									// port number
	
	UInt8 *	pathP;								// ptr to path portion
	UInt16	pathLen;								// size of path
	
	UInt8 *	paramP;								// param (;param)
	UInt16	paramLen;							// size of param
	
	UInt8 *	queryP;								// query (?query)
	UInt16	queryLen;							// size of query
	
	UInt8 *	fragP;								// fragment (#frag)
	UInt16	fragLen;								// size of fragment
	} INetURLType;
	


/********************************************************************
 * Structure for INetURLInfo. This structure is filled in with info
 *  about a URL. 
 ********************************************************************/
typedef struct {
	UInt16	version;								// should be 0, for future compatibility
	
	UInt16	flags;								// flags word, one or ore of 
														//   inetURLInfoFlagXXX flags
	UInt32	undefined;							// reserved for future use
	} INetURLInfoType;



/********************************************************************
 * Content and Compression Type Enums(from proxy server or PQA Builder)
 ********************************************************************/
typedef enum {
	inetContentTypeTextPlain = 0,
	inetContentTypeTextHTML,
	inetContentTypeImageGIF,
	inetContentTypeImageJPEG,
	inetContentTypeApplicationCML,
	inetContentTypeImagePalmOS,
	inetContentTypeOther
	} INetContentTypeEnum;
	
typedef enum {
	inetCompressionTypeNone = 0,
	inetCompressionTypeBitPacked,
	inetCompressionTypeLZ77
	} INetCompressionTypeEnum;

/********************************************************************
 * Proxy Types
 ********************************************************************/
typedef enum {
	inetProxyNone = 0,							// no proxy
	inetProxyCTP = 1								// CTP (Jerry) proxy
	} INetProxyEnum;



/********************************************************************
 * Settings for the INetLibSettingSet/Get call. 
 ********************************************************************/
typedef enum {
	inetSettingProxyType,						// (RW) UInt32, INetProxyEnum
	
	inetSettingProxyName,						// (RW) Char[], name of proxy
	inetSettingProxyPort,						// (RW) UInt32,  TCP port # of proxy
	
	inetSettingProxySocketType,				// (RW) UInt32, which type of socket to use
														//  netSocketTypeXXX
	
	inetSettingCacheSize,						// (RW) UInt32, max size of cache
	inetSettingCacheRef,							// (R) DmOpenRef, ref of cache DB
	
	inetSettingNetLibConfig,					// (RW) UInt32, Which NetLib config to use.
		
	inetSettingRadioID,							// (R)  UInt32[2], the 64-bit radio ID
	inetSettingBaseStationID,					// (R)  UInt32, the radio base station ID
	
	inetSettingMaxRspSize,						// (W) UInt32 (in bytes)
	inetSettingConvAlgorithm,					// (W) UInt32 (CTPConvEnum)
	inetSettingContentWidth,					// (W) UInt32 (in pixels)
	inetSettingContentVersion,					// (W) UInt32 Content version (encoder version)
	
	inetSettingNoPersonalInfo,					// (RW) UInt32 send no deviceID/zipcode
	
	inetSettingUserName,

	inetSettingLast
	} INetSettingEnum;


/********************************************************************
 * Settings for the INetLibSockSettingSet/Get call. 
 ********************************************************************/
typedef enum {
	inetSockSettingScheme,						// (R) UInt32, INetSchemeEnum
	inetSockSettingSockContext,				// (RW) UInt32,

	inetSockSettingCompressionType,			// (R) Char[]
	inetSockSettingCompressionTypeID,		// (R) UInt32 (INetCompressionTypeEnum)
	inetSockSettingContentType,				// (R) Char[]
	inetSockSettingContentTypeID,				// (R) UInt32 (INetContentTypeEnum)
	inetSockSettingData,							// (R) UInt32, pointer to data
	inetSockSettingDataHandle,					// (R) UInt32, MemHandle to data
	inetSockSettingDataOffset,					// (R) UInt32, offset to data from MemHandle

	inetSockSettingTitle,						// (RW) Char[]
	inetSockSettingURL,							// (R) Char[]
	inetSockSettingIndexURL,					// (RW) Char[]
	
	inetSockSettingFlags,						// (W) UInt16, one or more of
														//   inetOpenURLFlagXXX flags
														
	inetSockSettingReadTimeout,				// (RW) UInt32. Read timeout in ticks
	
	inetSockSettingContentVersion,			// (R) UInt32, version number for content

	inetSockSettingLast
	} INetSockSettingEnum;




/********************************************************************
 * Possible socket status values that can be returned from INetLibSockStatus
 ********************************************************************/
typedef enum {
	inetStatusNew,									// just opened
	inetStatusResolvingName,					// looking up host address
	inetStatusNameResolved,						// found host address
	inetStatusConnecting,						// connecting to host
	inetStatusConnected,							// connected to host
	inetStatusSendingRequest,					// sending request
	inetStatusWaitingForResponse,				// waiting for response
	inetStatusReceivingResponse,				// receiving response
	inetStatusResponseReceived,				// response received
	inetStatusClosingConnection,				// closing connection
	inetStatusClosed,								// closed
	inetStatusAcquiringNetwork,				// network temporarily
													// unreachable; socket on hold
	inetStatusPrvInvalid = 30					// internal value, not
													// returned by INetMgr. Should
													// be last.
	} INetStatusEnum;



/********************************************************************
 * HTTP Attributes which can be set/get using the
 *  INetLibHTTPAttrSet/Get calls.
 *
 * Generally, attributes are only set BEFORE calling 
 *		INetLibSockHTTPReqSend
 * and attributes are only gotten AFTER the complete response
 *		 has been received.
 *
 * Attributes marked with the following flags:
 *		(R)	- read only
 *		(W)	- write only
 *		(RW)	- read/write
 *		(-) 	- not implemented yet
 ********************************************************************/
typedef enum {

	// local error trying to communicate with server, if any
	inetHTTPAttrCommErr,							// (R) UInt32, read-only
	
	// object attributes, defined at creation
	inetHTTPAttrEntityURL,						// (-) Char[], which resource was requested
	
	
	//-----------------------------------------------------------
	// Request only attributes
	//-----------------------------------------------------------
	inetHTTPAttrReqAuthorization,				// (-) Char[]		
	inetHTTPAttrReqFrom,							// (-) Char[]
	inetHTTPAttrReqIfModifiedSince,			// (-) UInt32
	inetHTTPAttrReqReferer,						// (-) Char[]

	// The following are ignored unless going through a CTP proxy
	inetHTTPAttrWhichPart,						// (W) UInt32 (0 -> N)
	inetHTTPAttrIncHTTP,							// (W) UInt32 (Boolean) only applicable
														//   when inetHTTPAttrConvAlgorithm set to
														//   ctpConvNone
	inetHTTPAttrCheckMailHi,					// (W) UInt32
	inetHTTPAttrCheckMailLo,					// (W) UInt32
	inetHTTPAttrReqContentVersion,			// (W) UInt32 Desired content version. Represented
														//  as 2 low bytes. Lowest byte is minor version,
														//  next higher byte is major version. 
	
	

	//--------------------------------------------------------------
	// Response only attributes
	//--------------------------------------------------------------
	// Server response info
	inetHTTPAttrRspAll,							// (-) Char[] - entire HTTP response including
														//   data
	inetHTTPAttrRspSize,							// (R) UInt32 - entire HTTP Response size including
														//   header and data
	inetHTTPAttrRspVersion,						// (-) Char[]
	inetHTTPAttrResult,							// (R) UInt32 (ctpErrXXX when using CTP Proxy)
	inetHTTPAttrErrDetail,						// (R) UInt32 (server/proxy err code when 
														//      using CTP Proxy)
	inetHTTPAttrReason,							// (R) Char[]
	inetHTTPAttrDate,								// (-) UInt32
	inetHTTPAttrNoCache,							// (-) UInt32
	inetHTTPAttrPragma,							// (-) Char[]
	inetHTTPAttrServer,							// (-) Char[]
	inetHTTPAttrWWWAuthentication,			// (-) Char[]
	
	
	// Returned entity attributes
	inetHTTPAttrContentAllow,					// (-) Char[]
	inetHTTPAttrContentLength,					// (R) UInt32
	inetHTTPAttrContentLengthUncompressed,	// (R) UInt32 (in bytes)
	inetHTTPAttrContentPtr,						// (-) Char *
	inetHTTPAttrContentExpires,				// (-) UInt32
	inetHTTPAttrContentLastModified,			// (-) UInt32
	inetHTTPAttrContentLocation,				// (-) Char[]
	inetHTTPAttrContentLengthUntruncated,	// (R) UInt32
	inetHTTPAttrContentVersion,					// (R) UInt32, actual content version. Represented
														//  as 2 low bytes. Lowest byte is minor version,
														//  next higher byte is major version. 
	inetHTTPAttrContentCacheID,					// (R) UInt32, cacheID for this item
	inetHTTPAttrReqSize							// (R) UInt32 size of request sent
	
	
	} INetHTTPAttrEnum;




/********************************************************************
 * Structure of our Internet events. This structure is a superset of
 *  the regular event type. Note that we use the first 2 user events
 *  for the Internet Library so any app that uses this library must be
 *  to use user event IDs greater than inetLastEvent.
 *
 *  library refNum in it....
 ********************************************************************/
#define		inetSockReadyEvent				firstINetLibEvent
#define		inetSockStatusChangeEvent		(firstINetLibEvent+1)
#define		inetLastEvent						(firstINetLibEvent+1)

typedef struct {
	UInt16			eType;
	Boolean        penDown;
	UInt8				reserved;
	Int16          screenX;
	Int16          screenY;
	union {
		struct  {
			UInt16			datum[8];
			} generic;

		struct  {
			MemHandle	sockH;								// socket MemHandle
			UInt32	context;								// application defined
			Boolean	inputReady;							// true if ready for reads
			Boolean	outputReady;						// true if ready for writes
			} inetSockReady;

		struct  {
			MemHandle	sockH;								// socket MemHandle
			UInt32	context;								// application defined
			UInt16	status;								// new status
			Err		sockErr;								// socket err, if any
			}inetSockStatusChange;
		} data;
	} INetEventType;

/********************************************************************
 * Commands for InetLibWiCmd
 ********************************************************************/
typedef enum {
 	wiCmdInit =0,
 	wiCmdClear,
 	wiCmdSetEnabled,
 	wiCmdDraw,
	wiCmdEnabled,
	wiCmdSetLocation,
	wiCmdErase
} WiCmdEnum;

/********************************************************************
 * INet Library functions. 
 ********************************************************************/

#ifdef __GNUC__

#define inetLibTrapSettingGet					(sysLibTrapCustom)
#define inetLibTrapSettingSet					(sysLibTrapCustom+1)

#define inetLibTrapGetEvent					(sysLibTrapCustom+2)

#define inetLibTrapURLOpen						(sysLibTrapCustom+3)

#define inetLibTrapSockRead					(sysLibTrapCustom+4)
#define inetLibTrapSockWrite					(sysLibTrapCustom+5)

#define inetLibTrapSockOpen					(sysLibTrapCustom+6)
#define inetLibTrapSockClose					(sysLibTrapCustom+7)
#define inetLibTrapSockStatus					(sysLibTrapCustom+8)
#define inetLibTrapSockSettingGet			(sysLibTrapCustom+9)
#define inetLibTrapSockSettingSet			(sysLibTrapCustom+10)
#define inetLibTrapSockConnect				(sysLibTrapCustom+11)

	// Utilities
#define inetLibTrapURLCrack					(sysLibTrapCustom+12)
#define inetLibTrapURLsAdd						(sysLibTrapCustom+13)
#define inetLibTrapURLsCompare				(sysLibTrapCustom+14)
#define inetLibTrapURLGetInfo					(sysLibTrapCustom+15)

	// HTTP calls
#define inetLibTrapSockHTTPReqCreate		(sysLibTrapCustom+16)
#define inetLibTrapSockHTTPAttrSet			(sysLibTrapCustom+17)
#define inetLibTrapSockHTTPReqSend			(sysLibTrapCustom+18)
#define inetLibTrapSockHTTPAttrGet			(sysLibTrapCustom+19)

	// Mail traps
#define inetLibTrapSockMailReqCreate		(sysLibTrapCustom+20)
#define inetLibTrapSockMailAttrSet			(sysLibTrapCustom+21)
#define inetLibTrapSockMailReqAdd			(sysLibTrapCustom+22)
#define inetLibTrapSockMailReqSend			(sysLibTrapCustom+23)
#define inetLibTrapSockMailAttrGet			(sysLibTrapCustom+24)
#define inetLibTrapSockMailQueryProgress	(sysLibTrapCustom+25)

	// Cache calls
#define inetLibTrapCacheList					(sysLibTrapCustom+26)
#define inetLibTrapCacheGetObject			(sysLibTrapCustom+27)

	// Config calls
#define inetLibConfigMakeActive				(sysLibTrapCustom+28)
#define inetLibConfigList						(sysLibTrapCustom+29)
#define inetLibConfigIndexFromName			(sysLibTrapCustom+30)
#define inetLibConfigDelete					(sysLibTrapCustom+31)
#define inetLibConfigSaveAs					(sysLibTrapCustom+32)
#define inetLibConfigRename					(sysLibTrapCustom+33)
#define inetLibConfigAliasSet					(sysLibTrapCustom+34)
#define inetLibConfigAliasGet					(sysLibTrapCustom+35)

	//wireless Indicator
#define inetLibTrapWiCmd						(sysLibTrapCustom+36)

	// File Calls
#define inetLibTrapSockFileGetByIndex		(sysLibTrapCustom+37)

#define inetLibTrapCheckAntennaState		(sysLibTrapCustom+38)

#define inetLibTrapLast							(sysLibTrapCustom+39)

#else

#pragma mark Traps
typedef enum {
	inetLibTrapSettingGet = sysLibTrapCustom,
	inetLibTrapSettingSet,
	
	inetLibTrapGetEvent,
	
	inetLibTrapURLOpen,
	
	inetLibTrapSockRead,
	inetLibTrapSockWrite,
	
	inetLibTrapSockOpen,
	inetLibTrapSockClose,
	inetLibTrapSockStatus,
	inetLibTrapSockSettingGet,
	inetLibTrapSockSettingSet,
	inetLibTrapSockConnect,
	
	// Utilities
	inetLibTrapURLCrack,
	inetLibTrapURLsAdd,
	inetLibTrapURLsCompare,
	inetLibTrapURLGetInfo,
	
	// HTTP calls
	inetLibTrapSockHTTPReqCreate,
	inetLibTrapSockHTTPAttrSet,
	inetLibTrapSockHTTPReqSend,
	inetLibTrapSockHTTPAttrGet,
	
	// Mail traps
	inetLibTrapSockMailReqCreate,
	inetLibTrapSockMailAttrSet,
	inetLibTrapSockMailReqAdd,
	inetLibTrapSockMailReqSend,
	inetLibTrapSockMailAttrGet,
	inetLibTrapSockMailQueryProgress,
	
	// Cache calls
	inetLibTrapCacheList,
	inetLibTrapCacheGetObject,
	
	// Config calls
	inetLibConfigMakeActive,
	inetLibConfigList,
	inetLibConfigIndexFromName,
	inetLibConfigDelete,
	inetLibConfigSaveAs,
	inetLibConfigRename,
	inetLibConfigAliasSet,
	inetLibConfigAliasGet,
	
	//wireless Indicator
	inetLibTrapWiCmd,
	
	// File Calls
	inetLibTrapSockFileGetByIndex,
	
	inetLibTrapCheckAntennaState,

	inetLibTrapLast
	} INetLibTrapNumberEnum;

#endif


/********************************************************************
 * Structure of cache entry
 *	Used as a parameter to INetLibCacheList. If urlP or titleP are NULL,
 *	the corresponding length fields will be updated with the desired lengths
 ********************************************************************/
typedef struct {
	UInt8 *	urlP;
	UInt16	urlLen;
	
	UInt8 *	titleP;
	UInt16	titleLen;
	
	UInt32	lastViewed;							// seconds since 1/1/1904 
	UInt32	firstViewed;						// seconds since 1/1/1904 
} INetCacheEntryType, *INetCacheEntryP;

/********************************************************************
 * Structure for INetLibCacheGetObject. This structure is filled in with info
 *  about a cache entry. 
 ********************************************************************/
typedef struct {
	MemHandle						recordH;
	INetContentTypeEnum		contentType;
	INetCompressionTypeEnum	encodingType;
	UInt32						uncompressedDataSize;
	UInt8							flags;
	UInt8							reserved;
	UInt16						dataOffset;					// offset to content
	UInt16						dataLength;					// size of content
	UInt16						urlOffset;					// offset to URL
	UInt32						viewTime;					// time last viewed
	UInt32						createTime;					// time entry was created
	UInt16						murlOffset;					// offset to master URL
	} INetCacheInfoType, *INetCacheInfoPtr;



#pragma mark Functions

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------
// Library initialization, shutdown, sleep and wake
//--------------------------------------------------
Err				INetLibOpen (UInt16 libRefnum, UInt16 config, UInt32 flags,
							DmOpenRef cacheRef, UInt32 cacheSize, 
							MemHandle* inetHP)
						SYS_TRAP(sysLibTrapOpen);
					
Err				INetLibClose (UInt16 libRefnum, MemHandle inetH)
						SYS_TRAP(sysLibTrapClose);
					
Err				INetLibSleep (UInt16 libRefnum)
						SYS_TRAP(sysLibTrapSleep);
					
Err				INetLibWake (UInt16 libRefnum)
						SYS_TRAP(sysLibTrapWake);
					
//--------------------------------------------------
// Settings
//--------------------------------------------------
Err				INetLibSettingGet(UInt16 libRefnum, MemHandle inetH, 
							UInt16 /*INetSettingEnum */ setting,
							void * bufP, UInt16 * bufLenP)
						SYS_TRAP(inetLibTrapSettingGet);

Err				INetLibSettingSet(UInt16 libRefnum, MemHandle inetH, 
							UInt16 /*INetSettingEnum*/ setting,
							void * bufP, UInt16 bufLen)
						SYS_TRAP(inetLibTrapSettingSet);


//--------------------------------------------------
// Event Management
//--------------------------------------------------

void				INetLibGetEvent(UInt16 libRefnum, MemHandle inetH, 
									INetEventType* eventP, Int32 timeout)
						SYS_TRAP(inetLibTrapGetEvent);

						

//--------------------------------------------------
// High level calls
//--------------------------------------------------

Err				INetLibURLOpen(UInt16 libRefnum, MemHandle inetH, UInt8 * urlP, 
							UInt8 * cacheIndexURLP, MemHandle* sockHP, Int32 timeout, UInt16 flags)
						SYS_TRAP(inetLibTrapURLOpen);
						

Err				INetLibSockClose(UInt16 libRefnum, MemHandle socketH)
						SYS_TRAP(inetLibTrapSockClose);
						

//--------------------------------------------------
// Read/Write
//--------------------------------------------------

Err				INetLibSockRead(UInt16 libRefnum, MemHandle sockH, void * bufP,
							UInt32 reqBytes, UInt32 * actBytesP, Int32 timeout)
						SYS_TRAP(inetLibTrapSockRead);
						
Err				INetLibSockWrite(UInt16 libRefnum, MemHandle sockH, void * bufP,
							UInt32 reqBytes, UInt32 * actBytesP, Int32 timeout)
						SYS_TRAP(inetLibTrapSockWrite);
						

//--------------------------------------------------
// Low level Socket calls
//--------------------------------------------------

Err				INetLibSockOpen(UInt16 libRefnum, MemHandle inetH,
							UInt16 /*INetSchemEnum*/ scheme,  MemHandle* sockHP)
						SYS_TRAP(inetLibTrapSockOpen);
						
Err				INetLibSockStatus(UInt16 libRefnum, MemHandle socketH, UInt16 * statusP,
							Err* sockErrP, Boolean* inputReadyP, Boolean* outputReadyP)
						SYS_TRAP(inetLibTrapSockStatus);
						

Err				INetLibSockSettingGet(UInt16 libRefnum, MemHandle socketH, 
							UInt16 /*INetSockSettingEnum*/ setting,
							void * bufP, UInt16 * bufLenP)
						SYS_TRAP(inetLibTrapSockSettingGet);

Err				INetLibSockSettingSet(UInt16 libRefnum, MemHandle socketH, 
							UInt16 /*INetSockSettingEnum*/ setting,
							void * bufP, UInt16 bufLen)
						SYS_TRAP(inetLibTrapSockSettingSet);


Err				INetLibSockConnect(UInt16 libRefnum, MemHandle sockH, UInt8 * hostnameP,
							UInt16 port, Int32 timeou)
						SYS_TRAP(inetLibTrapSockConnect);

//--------------------------------------------------
// HTTP specific calls
//--------------------------------------------------

Err				INetLibSockHTTPReqCreate(UInt16 libRefnum, MemHandle sockH,
							UInt8 * verbP, UInt8 * resNameP, UInt8 * refererP)
						SYS_TRAP(inetLibTrapSockHTTPReqCreate);
						
Err				INetLibSockHTTPAttrSet(UInt16 libRefnum, MemHandle sockH, 
							UInt16 /*inetHTTPAttrEnum*/ attr, UInt16 attrIndex, 
							UInt8 * bufP,  UInt16 bufLen, UInt16 flags)
						SYS_TRAP(inetLibTrapSockHTTPAttrSet);
						
Err				INetLibSockHTTPReqSend(UInt16 libRefnum, MemHandle sockH, 
							void * writeP, UInt32 writeLen, 
							Int32 timeout)
						SYS_TRAP(inetLibTrapSockHTTPReqSend);
						
Err				INetLibSockHTTPAttrGet(UInt16 libRefnum, MemHandle sockH, 
							UInt16 /*inetHTTPAttrEnum*/ attr, UInt16 attrIndex,
							void * bufP, UInt32 * bufLenP)
						SYS_TRAP(inetLibTrapSockHTTPAttrGet);
						


//--------------------------------------------------
// Utilities
//--------------------------------------------------

Err				INetLibURLCrack(UInt16 libRefnum, UInt8 * urlTextP, INetURLType* urlP)
						SYS_TRAP(inetLibTrapURLCrack);

Err 				INetLibURLsAdd(UInt16 libRefnum, Char * baseURLStr, Char * embeddedURLStr, 
						Char * resultURLStr, UInt16 * resultLenP)
						SYS_TRAP(inetLibTrapURLsAdd);

Int16 				INetLibURLsCompare(UInt16 libRefnum, Char * URLStr1, Char * URLStr2)
						SYS_TRAP(inetLibTrapURLsCompare);

Err 				INetLibURLGetInfo(UInt16 libRefnum, MemHandle inetH,
						UInt8 * urlTextP, INetURLInfoType* urlInfoP)
						SYS_TRAP(inetLibTrapURLGetInfo);

Boolean 			INetLibWiCmd (UInt16 refNum, UInt16 /*WiCmdEnum*/ cmd, int enableOrX, int y)
						SYS_TRAP(inetLibTrapWiCmd);
						
Err				INetLibCheckAntennaState(UInt16 refNum)
						SYS_TRAP(inetLibTrapCheckAntennaState);				

//--------------------------------------------------
// Cache interface
//--------------------------------------------------

Err				INetLibCacheList(UInt16 libRefnum, MemHandle inetH,
							UInt8 * cacheIndexURLP, UInt16 * indexP, UInt32 * uidP,
							INetCacheEntryP cacheP)
						SYS_TRAP(inetLibTrapCacheList);

Err 				INetLibCacheGetObject(UInt16 libRefnum, MemHandle clientParamH, UInt8 * urlTextP,
							UInt32 uniqueID, INetCacheInfoPtr cacheInfoP )
						SYS_TRAP(inetLibTrapCacheGetObject);

//--------------------------------------------------
// Configuration Calls
//--------------------------------------------------
Err				INetLibConfigMakeActive( UInt16 refNum, MemHandle inetH, 
							UInt16 configIndex)
						SYS_TRAP(inetLibConfigMakeActive);
						
Err				INetLibConfigList( UInt16 refNum, INetConfigNameType nameArray[],
							UInt16 * arrayEntriesP)
						SYS_TRAP(inetLibConfigList);
						
Err				INetLibConfigIndexFromName( UInt16 refNum, INetConfigNamePtr nameP,
							UInt16 * indexP)
						SYS_TRAP(inetLibConfigIndexFromName);
						
Err				INetLibConfigDelete( UInt16 refNum, UInt16 index)
						SYS_TRAP(inetLibConfigDelete);
						
Err				INetLibConfigSaveAs( UInt16 refNum, MemHandle inetH, 
							INetConfigNamePtr nameP)
						SYS_TRAP(inetLibConfigSaveAs);
						
Err				INetLibConfigRename( UInt16 refNum, UInt16 index,
							INetConfigNamePtr newNameP)
						SYS_TRAP(inetLibConfigRename);

Err				INetLibConfigAliasSet( UInt16 refNum, UInt16 configIndex,
							UInt16 aliasToIndex)
						SYS_TRAP(inetLibConfigAliasSet);

Err				INetLibConfigAliasGet( UInt16 refNum, UInt16 aliasIndex,
							UInt16 * indexP, Boolean * isAnotherAliasP)
						SYS_TRAP(inetLibConfigAliasGet);

//--------------------------------------------------
// File specific calls
//--------------------------------------------------

Err	INetLibSockFileGetByIndex(UInt16 libRefnum, MemHandle sockH, 
				UInt32 index, MemHandle *handleP, UInt32 * offsetP, UInt32 * lengthP)
						SYS_TRAP(inetLibTrapSockFileGetByIndex);

#ifdef __cplusplus 
}
#endif

#endif 	//__INETLIB_H__

