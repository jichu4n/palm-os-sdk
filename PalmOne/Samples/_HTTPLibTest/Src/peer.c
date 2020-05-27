/*  peer.c */

#include <PalmOS.h>				
#include <68K\Hs.h>

#include "peer.h"
#include "HTTPLibTest.h"			// application resource defines


#define TCP_TIMEOUT 	 (SysTicksPerSecond()/2)  /* return immediately, we'll poll */
#define READ_CHUNK 65535

UInt16 AppNetLibNum;

static Boolean PPPConnectF;
static Boolean gIsNetLibOpen = false;
static Boolean gIsNetLibUp = false;

/*********************************************************************
* Logging 
*********************************************************************/
static void 
debug_text(Char *str, UInt16 len)
{
    FileHand file;
    UInt32 mode = fileModeAppend;
    Int32 nwritten = 0;
    Err err = errNone;
    
    if (str == NULL) return;
    
    file = FileOpen (0, "httplog", 0, 0, mode, &err);
    if (file != 0) {
        nwritten = FileWrite (file, str, len, 1, &err);
        FileClose(file);
    }
}
/*********************************************************************
* Peer Functions
*********************************************************************/


/* callbacks */
Int32 PrvTCPOpen(Int32 in_domain, HS_HTTPLibOpaque in_aux)
{
	NetSocketRef	sockRef;
	Err 					error;
	Int32 			 timeout = TCP_TIMEOUT;
	UInt16				 flag;
	UInt16					sizeFlag = sizeof(flag);
	Int16 				ret;
	NetSocketLingerType lin;
	
	sockRef = NetLibSocketOpen(AppNetLibNum, netSocketAddrINET,netSocketTypeStream,netSocketProtoIPTCP, timeout, &error);
	flag = 1;
	/* set to non-blocking socket */
	ret = NetLibSocketOptionSet(AppNetLibNum, sockRef, netSocketOptLevelSocket,netSocketOptSockNonBlocking, &flag, sizeFlag,timeout, &error);
	lin.onOff = true;
	lin.time = 0;
	/* set linger */
	ret = NetLibSocketOptionSet(AppNetLibNum, sockRef, netSocketOptLevelSocket,netSocketOptSockLinger, &lin, sizeof(lin),timeout, &error);

	/* netSocketAddrRaw : netSocketAddrINET */
	/* netSocketTypeDatagram : netSocketTypeRaw : netSocketTypeReliableMsg */
	/* netSocketIPTCP : netSocketIPUDP : netSocketIPRAW */
	if (error){
		NetLibSocketClose(AppNetLibNum, sockRef,timeout, &error);
		return httpErrorGeneric;
	}
	return sockRef;
}

void PrvTCPClose(Int32 in_desc, HS_HTTPLibOpaque in_aux)
{
	Int16  result;
	Int32 timeout = TCP_TIMEOUT;
	Err 	 error = 0;
	if(in_desc == -1) return;
	result = NetLibSocketClose(AppNetLibNum, (NetSocketRef)in_desc,timeout, &error);
}

Int32 PrvTCPIsConnected(Int32 in_desc, HS_HTTPLibOpaque in_aux)
{
	NetFDSetType	fds, wfds, dummy;
	Err 					error;
	long					timeout = TCP_TIMEOUT;
	Int16 				ret;
	UInt16					width;

	width = in_desc + 1;
	netFDZero(&fds);
	netFDSet((NetSocketRef)in_desc, &fds);
	netFDZero(&wfds);
	netFDSet((NetSocketRef)in_desc, &wfds);
	netFDZero(&dummy);
	ret = NetLibSelect(AppNetLibNum, width, &fds, &wfds, &dummy,timeout, &error);
	return ret;
}

Int32 
PrvTCPConnect(Int32 in_desc, HS_HTTPLibIPAddr *in_addr, Int32 in_port, HS_HTTPLibOpaque in_aux)
{
	Err 								 error;
	NetSocketAddrINType  sockAddr;
	Int32 							timeout = TCP_TIMEOUT;
	Int16 							 result;
	Int16 							 addrlen = sizeof(sockAddr);
	/* Address setup */
	MemSet(&sockAddr, addrlen, 0);
	
	sockAddr.family = netSocketAddrINET;
	sockAddr.port = (UInt16)NetHToNS((UInt16)in_port);
	{
			Char*  src;
			Int16 index = 0;
			/* memcpy */
			for (src = (Char*)in_addr->addr; index < 4; index++)
				*((Char*)&sockAddr.addr + index) = *(src + index);
	}
	/*	sockAddr.addr = PALM_NetLibAddrAToIN((Char*)in_addr); */
	/* connect */
	result = NetLibSocketConnect(AppNetLibNum, (Int16)in_desc,(NetSocketAddrType*)&sockAddr, addrlen,timeout, &error);
	if (result == -1) {
		/* error */
		switch (error) {
		case netErrSocketAlreadyConnected:
			result = httpErrorOK;
		break;
		case netErrSocketBusy:
		case netErrWouldBlock:
			result = httpErrorWouldBlock;
		break;
		default:
			result = httpErrorGeneric;
		break;
		}
	} else {
		result = httpErrorOK;
	}
	return result;
}

Int32 
PrvTCPRead(Int32 in_desc, Char *out_buf, Int32 in_len, HS_HTTPLibOpaque in_aux)
{
	Int16 	 result;
	Int32 	 timeout = TCP_TIMEOUT;
	UInt16		 flg = 0;
	Err 		 error;
	Int32 ret = 0x0000ffff;

	if (in_len > READ_CHUNK) {
		in_len = READ_CHUNK;
	}

	result = NetLibReceive(AppNetLibNum, (NetSocketRef)in_desc,out_buf, (UInt16)in_len,flg, NULL, NULL, timeout, &error);
	if (result == -1){
		switch (error) {
		case netErrSocketBusy:
		case netErrSocketNotConnected:
		case netErrWouldBlock:
			result = httpErrorWouldBlock;
		break;
		default:
			result = httpErrorGeneric;
		break;
		}
	} else {
		ret &= result;
		return ret;
	}
	return result;	
}

Int32 
PrvTCPWrite(Int32 in_desc, Char *in_buf, Int32 in_len, HS_HTTPLibOpaque in_aux)
{
	Int16  result;
	Int32  timeout = 5*SysTicksPerSecond();
	UInt16	 flg = 0;
	Err 	 error;

	if (in_len > READ_CHUNK) {
		in_len = READ_CHUNK;
	}
	result = NetLibSend(AppNetLibNum, (NetSocketRef)in_desc, in_buf, (UInt16)in_len,0, 0, flg, timeout, &error);
	if (result == -1) {
		switch (error) {
		case netErrSocketBusy:
		case netErrSocketNotConnected:
		case netErrWouldBlock:
			result = httpErrorWouldBlock;
		break;
		default:
			result = httpErrorGeneric;
		break;
		}
	} else {
    	debug_text(in_buf, in_len);
	}
	return result;
}

Int32 
PrvTCPCanReadWrite(Int32 in_desc, Int32 in_rw, HS_HTTPLibOpaque in_aux)
{
	NetFDSetType	fds, wfds, dummy;
	Err 					error;
	long					timeout = TCP_TIMEOUT;
	Int16 				selected;
	UInt16					width;
	Int32 ret;

	width = in_desc + 1;
	netFDZero(&fds);
	netFDSet((NetSocketRef)in_desc, &fds);
	switch (in_rw) {
	case httpStreamRead:
		selected = NetLibSelect(AppNetLibNum, width, &fds, NULL, &dummy,timeout, &error);
			if (selected != 1){
				ret = selected;
			}else{
				ret = httpStreamRead;
			}
	break;
	case httpStreamWrite:
		selected = NetLibSelect(AppNetLibNum, width, NULL, &fds, &dummy,timeout, &error);
		if (selected <=0){
			ret = selected;
		} else if (selected == 2){
			ret = httpStreamReadWrite;
		} else {
			ret = httpStreamWrite;
		}
	break;
	default:
		netFDZero(&wfds);
		netFDSet((NetSocketRef)in_desc, &wfds);
		selected = NetLibSelect(AppNetLibNum, width, &fds, &wfds, &dummy,timeout, &error);
		if (selected <=0){
			ret =  selected;
		} else if (selected == 2){
			ret = httpStreamReadWrite;
		} else if (netFDIsSet((NetSocketRef)in_desc, &fds)){
			ret = httpStreamRead;
		}else{
			ret = httpStreamWrite;
		}
	break;
	}
	return ret;
}

static UInt16 get_instance(void)
{
	Err err;
	UInt16 index,ifInstance;
	UInt32 ifCreator;
	UInt16 ret = 0xffff;
	for(index = 0;1;index++){
		err = NetLibIFGet(AppNetLibNum, index, &ifCreator, &ifInstance);
		if(err) return ret;
		if(ifCreator == netIFCreatorPPP){
			return ifInstance;
		}
	}
}

static Err ifup(void)
{
	UInt16 ifInstance;
	Err err;
	UInt16 ifErrs;
	
	if(gIsNetLibOpen == false){
   		err = NetLibOpen(AppNetLibNum, &ifErrs);
        if (err || ifErrs) {
            NetLibClose(AppNetLibNum, true);
            return -1;
        }
    	gIsNetLibOpen = true;
	}
	ifInstance = get_instance();
	if(ifInstance != 0xffff){
		err = NetLibIFUp(AppNetLibNum, netIFCreatorPPP,ifInstance);
		if (!err) gIsNetLibUp = true;	
        return err;
	}
	return -1;
}
static Err 
ifdown(void)
{
    /* Added NetLibClose and removed NetLibIFDown code. */

    /* Close NetLib if gIsNetLibOpen is true. */
	if(gIsNetLibOpen == true){
		NetLibClose(AppNetLibNum, false);
		gIsNetLibOpen = false;
	}
	return -1;
}

Err
PrvPeerInitialize(void)
{
	Err err = -1;
	
	gIsNetLibOpen = false;
	err = SysLibFind("Net.lib", &AppNetLibNum);
	
	if (AppNetLibNum){
		err = ifup();
	}else{
		err = -1;
	}
	if(!err){
		PPPConnectF = true;
		return err;
	}
	PPPConnectF = false;
	return err;
}

Err
PrvPeerFinalize(void)
{
	Err err;
	if (AppNetLibNum){
		err = ifdown();
	}
	PPPConnectF = false;
	return 0;
}