/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		NetMgr.h
 *
 * Description:
 *	  This module contains the interface definition for the TCP/IP
 *  library on Pilot.
 *
 * History:
 *   	2/14/96  RM - Created by Ron Marianetti
 *
 *******************************************************************/  

#ifndef 	__NETMGR_H__
#define	__NETMGR_H__

#include <Common.h>
#include <SysTraps.h>


/********************************************************************
 * Type and creator of Net Library database
 ********************************************************************/
 
// Creator. Used for both the database that contains the Net Library and
//  it's preferences database.
#define		netCreator				'netl'		// Our Net Library creator

// Feature Creators and numbers, for use with the FtrGet() call. This
//  feature can be obtained to get the current version of the Net Library
#define		netFtrCreator			netCreator
#define		netFtrNumVersion		0				// get version of Net Library
			// 0xMMmfsbbb, where MM is major version, m is minor version
			// f is bug fix, s is stage: 3-release,2-beta,1-alpha,0-development,
			// bbb is build number for non-releases 
			// V1.12b3   would be: 0x01122003
			// V2.00a2   would be: 0x02001002
			// V1.01     would be: 0x01013000


// Types. Used to identify the Net Library from it's prefs.
#define		netLibType				'libr'		// Our Net Code Resources Database type
#define		netPrefsType			'rsrc'		// Our Net Preferences Database type


// All Network interface's have the following type:
#define		netIFFileType			'neti'		// The filetype of all Network Interfaces

// Each Network interface has a unique creator:
#define		netIFCreatorLoop		'loop'		// Loopback network interface creator.
#define		netIFCreatorSLIP		'slip'		// SLIP network interface creator.
#define		netIFCreatorPPP		'ppp_'		// PPP network interface creator.



//-----------------------------------------------------------------------------
// Misc. constants
//-----------------------------------------------------------------------------
#define	netDrvrTypeNameLen		8				// Maximum driver type length
#define	netDrvrHWNameLen			16				// Maximum driver hardware name length
#define	netIFNameLen				10				// Maximum interface name (driver type + instance num)
#define	netIFMaxHWAddrLen			14				// Maximum size of a hardware address



//-----------------------------------------------------------------------------
// Option constants that can be passed to NetSocketOptionSet and NetSocketOptionGet
// When an option is set or retrieved, both the level of the option and the
// option number must be specified. The level refers to which layer the option
// refers to, like the uppermost socket layer, for example.
//-----------------------------------------------------------------------------

// Socket level options
typedef enum {
	// IP Level options
	netSocketOptIPOptions = 1,						// options in IP header (IP_OPTIONS)
	
	// TCP Level options
	netSocketOptTCPNoDelay = 1,					// don't delay send to coalesce packets
	netSocketOptTCPMaxSeg = 2,						// TCP maximum segment size (TCP_MAXSEG)

	// Socket level options
	netSocketOptSockDebug = 0x0001,				// turn on debugging info recording
	netSocketOptSockAcceptConn = 0x0002,		// socket has had listen
	netSocketOptSockReuseAddr = 0x0004,			// allow local address reuse
	netSocketOptSockKeepAlive = 0x0008,			// keep connections alive
	netSocketOptSockDontRoute = 0x0010,			// just use interface addresses
	netSocketOptSockBroadcast = 0x0020,			// permit sending of broadcast msgs
	netSocketOptSockUseLoopback = 0x0040,		// bypass hardware when possible
	netSocketOptSockLinger = 0x0080,				// linger on close if data present
	netSocketOptSockOOBInLine = 0x0100,			// leave received OutOfBand data in line
	
	netSocketOptSockSndBufSize = 0x1001,		// send buffer size
	netSocketOptSockRcvBufSize = 0x1002,		// receive buffer size
	netSocketOptSockSndLowWater = 0x1003,		// send low-water mark
	netSocketOptSockRcvLowWater = 0x1004,		// receive low-water mark
	netSocketOptSockSndTimeout = 0x1005,		// send timeout
	netSocketOptSockRcvTimeout = 0x1006,		// receive timeout
	netSocketOptSockErrorStatus= 0x1007,		// get error status and clear
	netSocketOptSockSocketType = 0x1008,		// get socket type
	
	// The following are Pilot specific options
	netSocketOptSockNonBlocking = 0x2000		// set non-blocking mode on or off
	} NetSocketOptEnum;




// Option levels for SocketOptionSet and SocketOptionGet
typedef enum {
	netSocketOptLevelIP = 0,					// IP level options (IPPROTO_IP)
	netSocketOptLevelTCP = 6,					// TCP level options (IPPROTO_TCP)
	netSocketOptLevelSocket = 0xFFFF			// Socket level options (SOL_SOCKET)
	} NetSocketOptLevelEnum;


// Structure used for manipulating the linger option
typedef struct {
	SWord		onOff;								// option on/off
	SWord		time;									// linger time in seconds
	} NetSocketLingerType;

//-----------------------------------------------------------------------------
// Enumeration of Socket domains and types passed to NetSocketOpen
//-----------------------------------------------------------------------------
typedef enum {
	netSocketAddrINET=2									// (AF_INET)
	} NetSocketAddrEnum;

typedef enum {
	netSocketTypeStream=1,								// (SOCK_STREAM)
	netSocketTypeDatagram=2,							// (SOCK_DGRAM)
	netSocketTypeRaw=3									// (SOCK_RAW)
	} NetSocketTypeEnum;


//-----------------------------------------------------------------------------
// Enumeration of Socket direction, passed to NetSocketShutdown
//-----------------------------------------------------------------------------
typedef enum {
	netSocketDirInput=0,
	netSocketDirOutput=1,
	netSocketDirBoth=2
	} NetSocketDirEnum;


//-----------------------------------------------------------------------------
// Basic Types
//-----------------------------------------------------------------------------
// Socket refnum
typedef	SWord			NetSocketRef;		

// Type used to hold internet addresses
typedef	DWord			NetIPAddr;		// a 32-bit IP address.



//-----------------------------------------------------------------------------
// Structure used to hold an internet socket address. This includes the internet 
//  address and the port number. This structure directly maps to the BSD unix 
//  struct sockaddr_in.
//-----------------------------------------------------------------------------
typedef struct NetSocketAddrINType {
	SWord			family;					// Address family in HBO (Host Byte Order)
	Word			port;						// the UDP port in NBO (Network Byte Order)
	NetIPAddr	addr;						// IP address in NBO (Network Byte Order)
	} NetSocketAddrINType;
	
// Constant that means "use the local machine's IP address"
#define	netIPAddrLocal		0			// Can be used in NetSockAddrINType.addr


// Structure used to hold a generic socket address. This is a generic struct 
// designed to hold any type of address including internet addresses. This 
// structure directly maps to the BSD unix struct sockaddr.
typedef struct  NetSocketAddrType {
	SWord			family;					// Address family
	Byte			data[14];				// 14 bytes of address 
	} NetSocketAddrType;
	
	
	
//-----------------------------------------------------------------------------
// Structure used to hold information about data to be sent. This structure
//  is passed to NetLibSendMsg and contains the optional address to send to,
//  a scatter-write array of data to be sent, and optional access rights
//-----------------------------------------------------------------------------

// Scatter/Gather array type. A pointer to an array of these structs is
//  passed to the NetLibSendPB and NetLibRecvPB calls. It specifies where
//  data should go to or come from as a list of buffer addresses and sizes.
typedef struct NetIOVecType {
	BytePtr			bufP;							// buffer address
	Word				bufLen;						// buffer length
	} NetIOVecType, *NetIOVecPtr;
	
#define	netIOVecMaxLen			16				// max# of NetIOVecTypes in an array

// Read/Write ParamBlock type. Passed directly to the SendPB and RecvPB calls.
typedef struct {
	BytePtr			addrP;						// address - or 0 for default
	Word				addrLen;						// length of address
	NetIOVecPtr		iov;							// scatter/gather array
	Word				iovLen;						// length of above array
	BytePtr			accessRights;				// access rights
	Word				accessRightsLen;			// length of accessrights
	} NetIOParamType, *NetIOParamPtr;
	
// Flags values for the NetLibSend, NetLibReceive calls
#define	netIOFlagOutOfBand		0x01		// process out-of-band data
#define	netIOFlagPeek				0x02		// peek at incoming message
#define	netIOFlagDontRoute		0x04		// send without using routing


						
//-----------------------------------------------------------------------------
// Structures used for looking up a host by name or address (NetLibGetHostByName)
//-----------------------------------------------------------------------------

// Equates for DNS names, from RFC-1035
#define	netDNSMaxDomainName		255
#define	netDNSMaxDomainLabel		63

#define	netDNSMaxAliases			1				// max # of aliases for a host
#define	netDNSMaxAddresses		4				// max # of addresses for a host


// The actual results of NetLibGetHostByName() are returned in this structure.
// This structure is designed to match the "struct hostent" structure in Unix.
typedef struct  {
	CharPtr		nameP;								// official name of host
	CharPtr*		nameAliasesP;						// array of alias's for the name
	Word			addrType;							// address type of return addresses
	Word			addrLen;								// the length, in bytes, of the addresses
	BytePtr*		addrListP;							// array of ptrs to addresses in HBO
	} NetHostInfoType, *NetHostInfoPtr;
	

// "Buffer" passed to call as a place to store the results
typedef struct {
	NetHostInfoType	hostInfo;					// high level results of call are here

	// The following fields contain the variable length data that 
	//  hostInfo points to
	Char			name[netDNSMaxDomainName+1];			// hostInfo->name

	CharPtr		aliasList[netDNSMaxAliases+1];		// +1 for 0 termination.
	Char			aliases[netDNSMaxAliases][netDNSMaxDomainName+1];

	NetIPAddr*	addressList[netDNSMaxAddresses];
	NetIPAddr	address[netDNSMaxAddresses];

	} NetHostInfoBufType, *NetHostInfoBufPtr;
	
	
//-----------------------------------------------------------------------------
// Structures used for looking up a service (NetLibGetServByName)
//-----------------------------------------------------------------------------

// Equates for service names
#define	netServMaxName				15				// max # characters in service name
#define	netProtoMaxName			15				// max # characters in protocol name
#define	netServMaxAliases			1				// max # of aliases for a service


// The actual results of NetLibGetServByName() are returned in this structure.
// This structure is designed to match the "struct servent" structure in Unix.
typedef struct {
	CharPtr		nameP;								// official name of service
	CharPtr*		nameAliasesP;						// array of alias's for the name
	Word			port;									// port number for this service
	CharPtr		protoP;								// name of protocol to use
	} NetServInfoType, *NetServInfoPtr;
	
// "Buffer" passed to call as a place to store the results
typedef struct {
	NetServInfoType	servInfo;					// high level results of call are here

	// The following fields contain the variable length data that 
	//  servInfo points to
	Char			name[netServMaxName+1];					// hostInfo->name

	CharPtr		aliasList[netServMaxAliases+1];		// +1 for 0 termination.
	Char			aliases[netServMaxAliases][netServMaxName];
	Char			protoName[netProtoMaxName+1];

	} NetServInfoBufType, *NetServInfoBufPtr;
	



/********************************************************************
 * Tracing Flags. These flags are ORed together and passed as a DWord
 *  in the netSettingTraceFlags setting and netIFSettingTraceFlags to
 *  enable/disable various trace options.
 ********************************************************************/
#define		netTracingErrors		0x00000001			// record errors
#define		netTracingMsgs			0x00000002			// record messages
#define		netTracingPkts			0x00000004			// record packets I/O
#define		netTracingFuncs		0x00000008			// record function flow
#define		netTracingAppMsgs		0x00000010			// record application messages
																	// (NetLibTracePrintF, NetLibTracePutS)



/********************************************************************
 * Command numbers and parameter blocks for the NetLibMaster() call.
 * This call is used to put the Net library into certain debugging modes
 *		or for obtaining statistics from the Net Library.
 * 
 ********************************************************************/
#pragma mark Master
typedef enum {
	// These calls return info
	netMasterInterfaceInfo,
	netMasterInterfaceStats,
	netMasterIPStats,
	netMasterICMPStats,
	netMasterUDPStats,
	netMasterTCPStats,
	
	// This call used to read the trace buffer.
	netMasterTraceEventGet					// get trace event by index

	} NetMasterEnum;
	
	
typedef struct NetMasterPBType {

	// These fields are specific to each command
	union {
	
		//.............................................................
		// InterfaceInfo command
		//.............................................................
		struct  {
			Word			index;					// -> index of interface
			DWord			creator;					// <- creator
			Word			instance;				// <- instance
			VoidPtr		netIFP;					// <- net_if pointer
			
			// driver level info
			Char			drvrName[netDrvrTypeNameLen];		// <- type of driver (SLIP,PPP, etc)
			Char			hwName[netDrvrHWNameLen];			// <- hardware name (Serial Library, etc)
			Byte			localNetHdrLen;		// <- local net header length
			Byte			localNetTrailerLen;	// <- local net trailer length
			Word			localNetMaxFrame;		// <- local net maximum frame size
			
			// media layer info
			Char			ifName[netIFNameLen];// <- interface name w/instance
			Boolean		driverUp;				// <- true if interface driver up
			Boolean		ifUp;						// <- true if interface is up
			Word			hwAddrLen;				// <- length of hardware address
			Byte			hwAddr[netIFMaxHWAddrLen];		// <- hardware address
			Word			mtu;						// <- maximum transfer unit of interface
			DWord			speed;					// <- speed in bits/sec.
			DWord			lastStateChange;		// <- time in milliseconds of last state change
			
			// Address info
			NetIPAddr	ipAddr;					// Address of this interface
			NetIPAddr	subnetMask;				// subnet mask of local network
			NetIPAddr	broadcast;				// broadcast address of local network
			} interfaceInfo;
			
		//.............................................................
		// InterfaceStats command
		//.............................................................
		struct  {
			Word			index;					// -> index of interface
			DWord			inOctets;				// <- ....
			DWord			inUcastPkts;
			DWord			inNUcastPkts;
			DWord			inDiscards;
			DWord			inErrors;
			DWord			inUnknownProtos;
			DWord			outOctets;
			DWord			outUcastPkts;
			DWord			outNUcastPkts;
			DWord			outDiscards;
			DWord			outErrors;
			} interfaceStats;
						
		//.............................................................
		// IPStats command
		//.............................................................
		struct  {
			DWord			ipInReceives;	
			DWord			ipInHdrErrors;
			DWord			ipInAddrErrors;
			DWord			ipForwDatagrams;
			DWord			ipInUnknownProtos;
			DWord			ipInDiscards;
			DWord			ipInDelivers;
			DWord			ipOutRequests;
			DWord			ipOutDiscards;
			DWord 		ipOutNoRoutes;
			DWord 		ipReasmReqds;
			DWord 		ipReasmOKs;
			DWord 		ipReasmFails;
			DWord		 	ipFragOKs;
			DWord 		ipFragFails;
			DWord 		ipFragCreates;
			DWord 		ipRoutingDiscards;
			DWord 		ipDefaultTTL;        
			DWord 		ipReasmTimeout;      
			} ipStats;
						
		//.............................................................
		// ICMPStats command
		//.............................................................
		struct  {
			DWord			icmpInMsgs;
			DWord			icmpInErrors;
			DWord			icmpInDestUnreachs;
			DWord			icmpInTimeExcds;
			DWord			icmpInParmProbs;
			DWord			icmpInSrcQuenchs;
			DWord			icmpInRedirects;
			DWord			icmpInEchos;
			DWord			icmpInEchoReps;
			DWord			icmpInTimestamps;
			DWord			icmpInTimestampReps;
			DWord			icmpInAddrMasks;
			DWord			icmpInAddrMaskReps;
			DWord			icmpOutMsgs;
			DWord			icmpOutErrors;
			DWord			icmpOutDestUnreachs;
			DWord			icmpOutTimeExcds;
			DWord			icmpOutParmProbs;
			DWord			icmpOutSrcQuenchs;
			DWord			icmpOutRedirects;
			DWord			icmpOutEchos;
			DWord			icmpOutEchoReps;
			DWord			icmpOutTimestamps;
			DWord			icmpOutTimestampReps;
			DWord			icmpOutAddrMasks;
			DWord			icmpOutAddrMaskReps;
			} icmpStats;
						
		//.............................................................
		// UDPStats command
		//.............................................................
		struct  {
			DWord			udpInDatagrams;
			DWord			udpNoPorts;
			DWord			udpInErrors;
			DWord			udpOutDatagrams;
			} udpStats;
						
		//.............................................................
		// TCPStats command
		//.............................................................
		struct  {
			DWord			tcpRtoAlgorithm;
			DWord			tcpRtoMin;
			DWord			tcpRtoMax;
			DWord			tcpMaxConn;
			DWord			tcpActiveOpens;
			DWord			tcpPassiveOpens;
			DWord			tcpAttemptFails;
			DWord			tcpEstabResets;
			DWord			tcpCurrEstab;
			DWord			tcpInSegs;
			DWord			tcpOutSegs;
			DWord			tcpRetransSegs;
			DWord			tcpInErrs;
			DWord			tcpOutRsts;
			} tcpStats;
						
		//.............................................................
		// TraceEventGet command
		//.............................................................
		struct  {
			Word			index;				// which event
			CharPtr		textP;				// ptr to text string to return it in
			} traceEventGet;
			
		} param;

	} NetMasterPBType, *NetMasterPBPtr;
	
	

	
	
//-----------------------------------------------------------------------------
// Enumeration of Net settings as passed to NetLibSettingGet/Set. 
//-----------------------------------------------------------------------------
#pragma mark Settings
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Global environment settings common to all attached network interfaces,
//   passed to NetLibSettingGet/Set
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef enum {	
	netSettingResetAll,						// void, NetLibSettingSet only, resets all settings
													//  to their defaults.

	netSettingPrimaryDNS,					// DWord, IP address of Primary DN Server
	netSettingSecondaryDNS,					// DWord, IP address of Secondary DN Server
	netSettingDefaultRouter,				// DWord, IP address of Default router
	netSettingDefaultIFCreator,			// DWord, Creator type of default interface
	netSettingDefaultIFInstance,			// Word, Instance# of default interface
	netSettingHostName,						// Char[64], name of host (not including domain)
	netSettingDomainName,					// Char[256], domain name of hosts's domain
	netSettingHostTbl,						// Char[], host table 
	netSettingCloseWaitTime,				// DWord, time in milliseconds to stay in close-wait state
	
	
	// The following settings are not used for configuration, but rather put the
	//  stack into various modes for debugging, etc. 
	netSettingTraceBits = 0x1000,			// DWord, enable/disable various trace flags (netTraceBitXXXX)
	netSettingTraceSize,						// DWord, max trace buffer size in bytes. Default 0x800.
													//  Setting this will also clear the trace buffer.
	netSettingTraceStart,					// DWord, for internal use ONLY!!
	netSettingTraceRoll,						// Byte, if true, trace buffer will rollover after it fills.
													//  Default is true.
													
	netSettingRTPrimaryDNS,					// used internally by Network interfaces
													//  that dynamically obtain the DNS address
	netSettingRTSecondaryDNS				// used internally by Network interfaces
													//  that dynamically obtain the DNS address

	} NetSettingEnum;


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Settings for each Network Interface, passed to NetLibIFSettingGet/Set
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef enum {
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// Reset all settings to defaults
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingResetAll,					// void, NetLibIFSettingSet only, resets all settings
													//  to their defaults.
													
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// Status - read only
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingUp,							// Byte, true if interface is UP.
	netIFSettingName,							// Char[32], name of interface

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// Addressing
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingReqIPAddr,					// DWord, requested IP address of this interface
	netIFSettingSubnetMask,					// DWord, subnet mask of this interface
	netIFSettingBroadcast,					// DWord, broadcast address for this interface

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// User Info
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingUsername,					// Char[], login script user name
													//				 If 0 length, then user will be prompted for it
	netIFSettingPassword,					// Char[], login script user password
													//				 If 0 length, then user will be prompted for it
	netIFSettingDialbackUsername,			// Char[], login script dialback user name.  
													//				 If 0 length, then netIFSettingUsername is used
	netIFSettingDialbackPassword,			// Char[], login script dialback user password. 
													//				 If 0 length, then user will be prompted for it
	netIFSettingAuthUsername,				// Char[], PAP/CHAP name. 
													//				 If 0 length, then netIFSettingUsername is used
	netIFSettingAuthPassword,				// Char[], PAP/CHAP password. 
													//				 If "$", then user will be prompted for it
													//				 else If 0 length, then netIFSettingPassword or result
													//					of it's prompt (if it was empty) will be used
													//				 else it is used as-is.
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// Connect Settings
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingServiceName,				// Char[], name of service
	netIFSettingLoginScript,				// Char[], login script
	netIFSettingConnectLog,					// Char[], connect log 
	netIFSettingInactivityTimeout,		// Word, # of seconds of inactivity allowed before
													//  interface is brought down. If 0 then
													//  no inactivity timeout enforced.
	netIFSettingEstablishmentTimeout,	// Word, max delay in seconds between connection 
													//  establishment stages
	
	// Serial based protocol options
	netIFSettingDynamicIP,					// Byte, if true, get IP address from server
													//  N/A for SLIP
	netIFSettingVJCompEnable,				// Byte, if true enable VJ Header compression
													//  Default is on for PPP, off for SLIP
	netIFSettingVJCompSlots,				// Byte, # of slots to use for VJ compression.
													//  Default is 4 for PPP, 16 for SLIP
													//  (each slot uses 256 bytes of RAM).
	netIFSettingMTU,							// Word, maximum transmission unit in bytes
													//  ignored in current PPP and SLIP interfaces
	netIFSettingAsyncCtlMap,				// DWord, bitmask of characters to escape
													//  ignored in current PPP interfaces

	// Serial settings, used by serial based network interfaces
	netIFSettingPortNum,						// Word, port number to use
	netIFSettingBaudRate,					// DWord, baud rate in bits/sec.
	netIFSettingFlowControl,				// Byte, flow control setting bits. Set to 0x01 for
													//   hardware flow control, else set to 0x00.
	netIFSettingStopBits,					// Byte, # of stop bits
	netIFSettingParityOn,					// Byte, true if parity on
	netIFSettingParityEven,					// Byte, true if parity even

	// Modem settings, optionally used by serial based network interfaces
	netIFSettingUseModem,					// Byte, if true dial-up through modem
	netIFSettingPulseDial,					// Byte, if true use pulse dial, else tone
	netIFSettingModemInit,					// Char[], modem initialization string
	netIFSettingModemPhone,					// Char[], modem phone number string
	netIFSettingRedialCount,				// Word, # of times to redial
	


	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// The following settings are not used for configuration, but rather put the
	//  stack into various modes for debugging, etc. 
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingTraceBits = 0x1000,		// DWord, enable/disable various trace flags (netTraceBitXXXX)
	netIFSettingGlobalsPtr,					// DWord, (Read-Only) sinterface's globals ptr
	netIFSettingActualIPAddr,				// DWord, (Read-Only) the actual IP address that the interface
													//   ends up using. The login script executor stores
													//   the result of the "g" script command here as does
													//   the PPP negotiations. 
	netIFSettingServerIPAddr,				// DWord, (Read-Only) the IP address of the PPP server
													//  we're connected to 
	
	
	// The following setting should be true if this network interface should be
	// brought down when the Pilot is turned off.
	netIFSettingBringDownOnPowerDown,	// Byte, if true interface will be brought down when
													//  Pilot is turned off.

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// 3rd party settings start here...
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	netIFSettingCustom = 0x8000
	
	} NetIFSettingEnum;





/************************************************************
 * Net Library Macros
 *************************************************************/
// Return current time in milliseconds.
#define NetNow()     (TimGetTicks() * 1000L/sysTicksPerSecond) 


// File Descriptor macros used for the NetLibSelect() call
typedef DWord					NetFDSetType;
#define	netFDSetSize		32

#define	netFDSet(n,p)		((*p) |= (1L << n))
#define	netFDClr(n,p)		((*p) &= ~(1L << n))
#define	netFDIsSet(n,p)	((*p) & (1L << n))
#define	netFDZero(p)		((*p) = 0)



//-----------------------------------------------------------------------------
// Net error codes
//-----------------------------------------------------------------------------
#pragma mark ErrorCodes
#define	netErrAlreadyOpen					(netErrorClass | 1)
#define	netErrNotOpen						(netErrorClass | 2)
#define	netErrStillOpen					(netErrorClass | 3)
#define	netErrParamErr						(netErrorClass | 4)
#define	netErrNoMoreSockets				(netErrorClass | 5)
#define	netErrOutOfResources				(netErrorClass | 6)
#define	netErrOutOfMemory					(netErrorClass | 7)
#define	netErrSocketNotOpen				(netErrorClass | 8)
#define	netErrSocketBusy					(netErrorClass | 9)		//EINPROGRESS
#define	netErrMessageTooBig				(netErrorClass | 10)
#define	netErrSocketNotConnected 		(netErrorClass | 11)
#define	netErrNoInterfaces				(netErrorClass | 12)		//ENETUNREACH
#define	netErrBufTooSmall					(netErrorClass | 13)
#define	netErrUnimplemented				(netErrorClass | 14)
#define	netErrPortInUse					(netErrorClass | 15)		//EADDRINUSE
#define	netErrQuietTimeNotElapsed		(netErrorClass | 16)		//EADDRINUSE
#define	netErrInternal						(netErrorClass | 17)
#define	netErrTimeout						(netErrorClass | 18)		//ETIMEDOUT
#define	netErrSocketAlreadyConnected 	(netErrorClass | 19)		//EISCONN
#define	netErrSocketClosedByRemote 	(netErrorClass | 20)
#define	netErrOutOfCmdBlocks			 	(netErrorClass | 21)
#define	netErrWrongSocketType			(netErrorClass | 22)
#define	netErrSocketNotListening		(netErrorClass | 23)
#define	netErrUnknownSetting				(netErrorClass | 24)
#define	netErrInvalidSettingSize		(netErrorClass | 25)
#define	netErrPrefNotFound				(netErrorClass | 26)
#define	netErrInvalidInterface			(netErrorClass | 27)
#define	netErrInterfaceNotFound			(netErrorClass | 28)
#define	netErrTooManyInterfaces			(netErrorClass | 29)
#define	netErrBufWrongSize				(netErrorClass | 30)
#define	netErrUserCancel					(netErrorClass | 31)
#define	netErrBadScript					(netErrorClass | 32)
#define	netErrNoSocket						(netErrorClass | 33)
#define	netErrSocketRcvBufFull			(netErrorClass | 34)
#define	netErrNoPendingConnect			(netErrorClass | 35)
#define	netErrUnexpectedCmd				(netErrorClass | 36)
#define	netErrNoTCB							(netErrorClass | 37)
#define	netErrNilRemoteWindowSize		(netErrorClass | 38)
#define	netErrNoTimerProc					(netErrorClass | 39)
#define	netErrSocketInputShutdown		(netErrorClass | 40)		// EOF to sockets API
#define	netErrCmdBlockNotCheckedOut	(netErrorClass | 41)		 
#define	netErrCmdNotDone					(netErrorClass | 42)		 
#define	netErrUnknownProtocol			(netErrorClass | 43)		 
#define	netErrUnknownService				(netErrorClass | 44)		 
#define	netErrUnreachableDest			(netErrorClass | 45)	
#define	netErrReadOnlySetting			(netErrorClass | 46)	 
#define	netErrWouldBlock					(netErrorClass | 47)		//EWOULDBLOCK
#define	netErrAlreadyInProgress			(netErrorClass | 48)		//EALREADY
#define	netErrPPPTimeout					(netErrorClass | 49)
#define	netErrPPPBroughtDown				(netErrorClass | 50)
#define	netErrAuthFailure					(netErrorClass | 51)
#define	netErrPPPAddressRefused			(netErrorClass | 52)
// The following map into the Epilogue DNS errors declared in DNS.ep.h:
//  and MUST be kept in this order!!
#define	netErrDNSNameTooLong				(netErrorClass | 53)
#define	netErrDNSBadName					(netErrorClass | 54)
#define	netErrDNSBadArgs					(netErrorClass | 55)
#define	netErrDNSLabelTooLong			(netErrorClass | 56)
#define	netErrDNSAllocationFailure		(netErrorClass | 57)
#define	netErrDNSTimeout					(netErrorClass | 58)
#define	netErrDNSUnreachable				(netErrorClass | 59)
#define	netErrDNSFormat					(netErrorClass | 60)
#define	netErrDNSServerFailure			(netErrorClass | 61)
#define	netErrDNSNonexistantName		(netErrorClass | 62)
#define	netErrDNSNIY						(netErrorClass | 63)
#define	netErrDNSRefused					(netErrorClass | 64)
#define	netErrDNSImpossible				(netErrorClass | 65)
#define	netErrDNSNoRRS						(netErrorClass | 66)
#define	netErrDNSAborted					(netErrorClass | 67)
#define	netErrDNSBadProtocol				(netErrorClass | 68)
#define	netErrDNSTruncated				(netErrorClass | 69)
#define	netErrDNSNoRecursion				(netErrorClass | 70)
#define	netErrDNSIrrelevant				(netErrorClass | 71)
#define	netErrDNSNotInLocalCache		(netErrorClass | 72)
#define	netErrDNSNoPort					(netErrorClass | 73)
// The following map into the Epilogue IP errors declared in IP.ep.h:
//  and MUST be kept in this order!!
#define	netErrIPCantFragment				(netErrorClass | 74)
#define	netErrIPNoRoute					(netErrorClass | 75)
#define	netErrIPNoSrc						(netErrorClass | 76)
#define	netErrIPNoDst						(netErrorClass | 77)
#define	netErrIPktOverflow				(netErrorClass | 78)
// End of Epilogue IP errors
#define	netErrTooManyTCPConnections	(netErrorClass | 79)
#define  netErrNoDNSServers				(netErrorClass | 80)


//-----------------------------------------------------------------------------
// Net library call ID's. Each library call gets the trap number:
//   netTrapXXXX which serves as an index into the library's dispatch table.
//   The constant sysLibTrapCustom is the first available trap number after
//   the system predefined library traps Open,Close,Sleep & Wake.
//
// WARNING!!! This order of these traps MUST match the order of the dispatch
//  table in NetDispatch.c!!!
//-----------------------------------------------------------------------------
#pragma mark Traps
typedef enum {
	netLibTrapAddrINToA = sysLibTrapCustom,
	netLibTrapAddrAToIN,
	
	netLibTrapSocketOpen,
	netLibTrapSocketClose,
	netLibTrapSocketOptionSet,
	netLibTrapSocketOptionGet,
	netLibTrapSocketBind,
	netLibTrapSocketConnect,
	netLibTrapSocketListen,
	netLibTrapSocketAccept,
	netLibTrapSocketShutdown,
	
	netLibTrapSendPB,
	netLibTrapSend,
	netLibTrapReceivePB,
	netLibTrapReceive,
	netLibTrapDmReceive,
	netLibTrapSelect,

	netLibTrapPrefsGet,
	netLibTrapPrefsSet,

	// The following traps are for internal and Network interface
	//  use only.
	netLibTrapDrvrWake,
	netLibTrapInterfacePtr,
	netLibTrapMaster,
	
	// New Traps, re-order these later..
	netLibTrapGetHostByName,
	netLibTrapSettingGet,
	netLibTrapSettingSet,
	netLibTrapIFAttach,
	netLibTrapIFDetach,
	netLibTrapIFGet,
	netLibTrapIFSettingGet,
	netLibTrapIFSettingSet,
	netLibTrapIFUp,
	netLibTrapIFDown,
	netLibTrapIFMediaUp,
	netLibTrapScriptExecute,
	netLibTrapGetHostByAddr,
	netLibTrapGetServByName,
	netLibTrapSocketAddr,
	netLibTrapFinishCloseWait,
	netLibTrapGetMailExchangeByName,
	netLibTrapPrefsAppend,
	netLibTrapIFMediaDown,
	netLibTrapOpenCount,
	
	netLibTrapTracePrintF,
	netLibTrapTracePutS,
	
	netLibTrapOpenIfCloseWait,
	netLibTrapHandlePowerOff,
	
	netLibTrapConnectionRefresh,
	
	netLibTrapLast
	} NetLibTrapNumberEnum;



/************************************************************
 * Net Library procedures.
 *************************************************************/ 
#pragma mark Functions
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------
// Library initialization, shutdown, sleep and wake
//--------------------------------------------------
Err				NetLibOpen (Word libRefnum, WordPtr netIFErrsP)
						SYS_TRAP(sysLibTrapOpen);
					
Err				NetLibClose (Word libRefnum, Word immediate)
						SYS_TRAP(sysLibTrapClose);
					
Err				NetLibSleep (Word libRefnum)
						SYS_TRAP(sysLibTrapSleep);
					
Err				NetLibWake (Word libRefnum)
						SYS_TRAP(sysLibTrapWake);
					
					
// This call forces the library to complete a close if it's
//  currently in the close-wait state. Returns 0 if library is closed,
//  Returns netErrFullyOpen if library is still open by some other task.
Err				NetLibFinishCloseWait(Word libRefnum)
						SYS_TRAP(netLibTrapFinishCloseWait);
						
// This call is for use by the Network preference panel only. It
// causes the NetLib to fully open if it's currently in the close-wait 
//  state. If it's not in the close wait state, it returns an error code
Err				NetLibOpenIfCloseWait(Word libRefnum)
						SYS_TRAP(netLibTrapOpenIfCloseWait);
						
// Get the open Count of the NetLib
Err				NetLibOpenCount (Word refNum, WordPtr countP)
						SYS_TRAP(netLibTrapOpenCount);
					
// Give NetLib a chance to close the connection down in response
// to a power off event. Returns non-zero if power should not be
//  turned off. EventP points to the event that initiated the power off
//  which is either a keyDownEvent of the hardPowerChr or the autoOffChr.
Err				NetLibHandlePowerOff (Word refNum, EventPtr eventP)
						SYS_TRAP(netLibTrapHandlePowerOff);
						
	
// Check status or try and reconnect any interfaces which have come down.
// This call can be made by applications when they suspect that an interface
// has come down (like PPP or SLIP). NOTE: This call can display UI 
// (if 'refresh' is true) so it MUST be called from the UI task. 
Err				NetLibConnectionRefresh(Word refNum, Boolean refresh, 
							BytePtr allInterfacesUpP, WordPtr netIFErrP)
						SYS_TRAP(netLibTrapConnectionRefresh); 
										
					
//--------------------------------------------------
// Net address translation and conversion routines.
//--------------------------------------------------

// convert host short to network short
#define			NetHToNS(x) 	(x)	
					
// convert host long to network long
#define			NetHToNL(x) 	(x)	
					
// convert network short to host short
#define			NetNToHS(x) 	(x)	
					
// convert network long to host long
#define			NetNToHL(x) 	(x)						

// Convert 32-bit IP address to ascii dotted decimal form. The Sockets glue
//  macro inet_ntoa will pass the address of an application global string in
//  spaceP.
CharPtr			NetLibAddrINToA(Word libRefnum, NetIPAddr	inet, CharPtr spaceP)
						SYS_TRAP(netLibTrapAddrINToA);
					
// Convert a dotted decimal ascii string format of an IP address into
//  a 32-bit value.
NetIPAddr		NetLibAddrAToIN(Word libRefnum, CharPtr a)
						SYS_TRAP(netLibTrapAddrAToIN);
						


//--------------------------------------------------
// Socket creation and option setting
//--------------------------------------------------

// Create a socket and return a refnum to it. Protocol is normally 0. 
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
NetSocketRef	NetLibSocketOpen(Word libRefnum, NetSocketAddrEnum domain, 
							NetSocketTypeEnum type, SWord protocol, SDWord timeout, 
							Err* errP)
						SYS_TRAP(netLibTrapSocketOpen);

// Close a socket. 
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketClose(Word libRefnum, NetSocketRef socket, SDWord timeout, 
							Err* errP)
						SYS_TRAP(netLibTrapSocketClose);
						
// Set a socket option.	Level is usually netSocketOptLevelSocket. Option is one of
//  netSocketOptXXXXX. OptValueP is a pointer to the new value and optValueLen is
//  the length of the option value.				
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketOptionSet(Word libRefnum, NetSocketRef socket,
							Word /*NetSocketOptLevelEnum*/ level, Word /*NetSocketOptEnum*/ option, 
							VoidPtr optValueP, Word optValueLen,
							SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSocketOptionSet);
						
// Get a socket option.					
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketOptionGet(Word libRefnum, NetSocketRef socket,
							Word /*NetSocketOptLevelEnum*/ level, Word /*NetSocketOptEnum*/ option,
							VoidPtr optValueP, WordPtr optValueLenP,
							SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSocketOptionGet);
						

//--------------------------------------------------
// Socket Control
//--------------------------------------------------

// Bind a source address and port number to a socket. This makes the
//  socket accept incoming packets destined for the given socket address.
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketBind(Word libRefnum, NetSocketRef socket,
							NetSocketAddrType* sockAddrP, SWord addrLen, SDWord timeout, 
							Err* errP)
						SYS_TRAP(netLibTrapSocketBind);
						
						
// Connect to a remote socket. For a stream based socket (i.e. TCP), this initiates
//  a 3-way handshake with the remote machine to establish a connection. For
//  non-stream based socket, this merely specifies a destination address and port
//  number for future outgoing packets from this socket.
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketConnect(Word libRefnum, NetSocketRef socket,
							NetSocketAddrType* sockAddrP, SWord addrLen, SDWord timeout, 
							Err* errP)
						SYS_TRAP(netLibTrapSocketConnect);
						

// Makes a socket ready to accept incoming connection requests. The queueLen 
//  specifies the max number of pending connection requests that will be enqueued
//  while the server is busy handling other requests.
//  Only applies to stream based (i.e. TCP) sockets.
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketListen(Word libRefnum, NetSocketRef socket,
							Word	queueLen, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSocketListen);
						

// Blocks the current process waiting for an incoming connection request. The socket
//  must have previously be put into listen mode through the NetLibSocketListen call.
//  On return, *sockAddrP will have the remote machines address and port number.
//  Only applies to stream based (i.e. TCP) sockets.
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketAccept(Word libRefnum, NetSocketRef socket,
							NetSocketAddrType* sockAddrP, SWord* addrLenP, SDWord timeout,
							Err* errP)
						SYS_TRAP(netLibTrapSocketAccept);


// Shutdown a connection in one or both directions.  
//  Only applies to stream based (i.e. TCP) sockets.
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketShutdown(Word libRefnum, NetSocketRef socket, 
							SWord /*NetSocketDirEnum*/ direction, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSocketShutdown);
						


// Gets the local and remote addresses of a socket. Useful for TCP sockets that 
//  get dynamically bound at connect time. 
// Returns 0 on success, -1 on error. If error, *errP gets filled in with error code.
SWord				NetLibSocketAddr(Word libRefnum, NetSocketRef socketRef,
							NetSocketAddrType* locAddrP, SWord* locAddrLenP, 
							NetSocketAddrType* remAddrP, SWord* remAddrLenP, 
							SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSocketAddr);
						
						

//--------------------------------------------------
// Sending and Receiving
//--------------------------------------------------
// Send data through a socket. The data is specified through the NetIOParamType
//  structure.
// Flags is one or more of netMsgFlagXXX.
// Returns # of bytes sent on success, or -1 on error. If error, *errP gets filled 
//  in with error code.
SWord				NetLibSendPB(Word libRefNum, NetSocketRef socket,
							NetIOParamType* pbP, Word flags, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSendPB);

// Send data through a socket. The data to send is passed in a single buffer,
//  unlike NetLibSendPB. If toAddrP is not nil, the data will be sent to 
//  address *toAddrP.
// Flags is one or more of netMsgFlagXXX.
// Returns # of bytes sent on success, or -1 on error. If error, *errP gets filled 
//  in with error code.
SWord				NetLibSend(Word libRefNum, NetSocketRef socket,
							const VoidPtr bufP, Word bufLen, Word flags,
							VoidPtr toAddrP, Word toLen, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapSend);

// Receive data from a socket. The data is gatthered into buffers specified in the 
//  NetIOParamType structure.
// Flags is one or more of netMsgFlagXXX.
// Timeout is max # of ticks to wait, or -1 for infinite, or 0 for none.
// Returns # of bytes received, or -1 on error. If error, *errP gets filled in 
//  with error code.
SWord				NetLibReceivePB(Word libRefNum, NetSocketRef socket,
							NetIOParamType* pbP, Word flags, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapReceivePB);

// Receive data from a socket. The data is read into a single buffer, unlike
//  NetLibReceivePB. If fromAddrP is not nil, *fromLenP must be initialized to
//  the size of the buffer that fromAddrP points to and on exit *fromAddrP will
//  have the address of the sender in it.
// Flags is one or more of netMsgFlagXXX.
// Timeout is max # of ticks to wait, or -1 for infinite, or 0 for none.
// Returns # of bytes received, or -1 on error. If error, *errP gets filled in 
//  with error code.
SWord				NetLibReceive(Word libRefNum, NetSocketRef socket,
							VoidPtr bufP, Word bufLen, Word flags, 
							VoidPtr fromAddrP, WordPtr fromLenP, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapReceive);


// Receive data from a socket directly into a (write-protected) Data Manager 
//  record. 
// If fromAddrP is not nil, *fromLenP must be initialized to
//  the size of the buffer that fromAddrP points to and on exit *fromAddrP will
//  have the address of the sender in it.
// Flags is one or more of netMsgFlagXXX.
// Timeout is max # of ticks to wait, or -1 for infinite, or 0 for none.
// Returns # of bytes received, or -1 on error. If error, *errP gets filled in 
//  with error code.
SWord				NetLibDmReceive(Word libRefNum, NetSocketRef socket,
							VoidPtr recordP, DWord recordOffset, Word rcvLen, Word flags, 
							VoidPtr fromAddrP, WordPtr fromLenP, SDWord timeout, Err* errP)
						SYS_TRAP(netLibTrapDmReceive);


//--------------------------------------------------
// Name Lookups
//--------------------------------------------------
NetHostInfoPtr	NetLibGetHostByName(Word libRefNum, CharPtr nameP, 
							NetHostInfoBufPtr bufP, Long	timeout, Err* errP)
						SYS_TRAP(netLibTrapGetHostByName);


NetHostInfoPtr	NetLibGetHostByAddr(Word libRefNum, BytePtr addrP, Word len, Word type,
							NetHostInfoBufPtr bufP, Long	timeout, Err* errP)
						SYS_TRAP(netLibTrapGetHostByAddr);


NetServInfoPtr	NetLibGetServByName(Word libRefNum, CharPtr servNameP, 
							CharPtr protoNameP,  NetServInfoBufPtr bufP, 
							Long	timeout, Err* errP)
						SYS_TRAP(netLibTrapGetServByName);

// Looks up a mail exchange name and returns a list of hostnames for it. Caller
//  must pass space for list of return names (hostNames), space for 
//  list of priorities for those hosts (priorities) and max # of names to 
//  return (maxEntries).
// Returns # of entries found, or -1 on error. If error, *errP gets filled in
//  with error code.
SWord				NetLibGetMailExchangeByName(Word libRefNum, CharPtr mailNameP, 
							Word maxEntries, 
							Char hostNames[][netDNSMaxDomainName+1], Word priorities[], 
							Long	timeout, Err* errP)
						SYS_TRAP(netLibTrapGetMailExchangeByName);


//--------------------------------------------------
// Interface setup
//--------------------------------------------------
Err				NetLibIFGet(Word libRefNum, Word index, DWordPtr ifCreatorP, 
								WordPtr ifInstanceP)
						SYS_TRAP(netLibTrapIFGet);

Err				NetLibIFAttach(Word libRefNum, DWord ifCreator, Word ifInstance,
							SDWord timeout)
						SYS_TRAP(netLibTrapIFAttach);

Err				NetLibIFDetach(Word libRefNum, DWord ifCreator, Word ifInstance,
							SDWord timeout)
						SYS_TRAP(netLibTrapIFDetach);

Err				NetLibIFUp(Word libRefNum, DWord ifCreator, Word ifInstance)
						SYS_TRAP(netLibTrapIFUp);

Err				NetLibIFDown(Word libRefNum, DWord ifCreator, Word ifInstance,
							SDWord timeout)
						SYS_TRAP(netLibTrapIFDown);




//--------------------------------------------------
// Settings
//--------------------------------------------------
// General settings
Err				NetLibSettingGet(Word libRefNum,
							Word /*NetSettingEnum*/ setting, VoidPtr valueP, WordPtr valueLenP)
						SYS_TRAP(netLibTrapSettingGet);

Err				NetLibSettingSet(Word libRefNum, 
							Word /*NetSettingEnum*/ setting, VoidPtr valueP, Word valueLen)
						SYS_TRAP(netLibTrapSettingSet);
						
// Network interface specific settings.
Err				NetLibIFSettingGet(Word libRefNum, DWord ifCreator, Word ifInstance,
							Word /*NetIFSettingEnum*/ setting, VoidPtr valueP, WordPtr valueLenP)
						SYS_TRAP(netLibTrapIFSettingGet);

Err				NetLibIFSettingSet(Word libRefNum, DWord ifCreator, Word ifInstance,
							Word /*NetIFSettingEnum*/ setting, VoidPtr valueP, Word valueLen)
						SYS_TRAP(netLibTrapIFSettingSet);



//--------------------------------------------------
// System level
//--------------------------------------------------
SWord				NetLibSelect(Word libRefNum, Word width, NetFDSetType* readFDs, 
							NetFDSetType* writeFDs, NetFDSetType* exceptFDs,
							Long	timeout, Err* errP)
						SYS_TRAP(netLibTrapSelect);



//--------------------------------------------------
// Debugging support
//--------------------------------------------------
Err				NetLibMaster(Word libRefNum, Word cmd, NetMasterPBPtr pbP,
						Long timeout)
						SYS_TRAP(netLibTrapMaster);

Err				NetLibTracePrintF(Word libRefNum, CharPtr formatStr, ...)
						SYS_TRAP(netLibTrapTracePrintF);
						
Err				NetLibTracePutS(Word libRefNum, CharPtr strP)
						SYS_TRAP(netLibTrapTracePutS);
						
						
#ifdef __cplusplus
}
#endif


#endif // __NETMGR_H__






