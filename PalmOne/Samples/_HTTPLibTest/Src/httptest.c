#include <PalmOS.h>				
#include <68K\Hs.h>

#include <StringMgr.h>

#include "peer.h"
#include "httptest.h"
#include "HTTPLibTest.h"			// application resource defines
#include "AppStdIO.h"


#define DOWNLOAD_BUFFER_SIZE (8192)

#define __SKIP_WRITE 0

/*********************************************************************
* Local variables
*********************************************************************/
#if 0
static Char *ProxyHost = "10.0.0.112";
static Int32 ProxyPort = 8001;
static Char *NoProxyHost = "";
#endif 

static UInt16 gRefNum = sysInvalidRefNum;
static HS_HTTPLibHandle gLibHandle = NULL;
static HS_HTTPLibAppInfo gAppInfo;
static HS_HTTPLibNetLibInfo gNetLibInfo;
static HS_HTTPLibPeer gPeer;

static Char title[50];

#define FrmSetTitle(...) if(0){}

UInt32	gstartTicks;
UInt32	gstopTicks;
UInt32	glatencyTicks;
Boolean	gErrorFlag = false;


/*********************************************************************
* Function declaration
*********************************************************************/
Int32 PrvTunnelingCallback(HS_HTTPLibStream in_stream, Char* buf, Int32 len, void *aux);

/*********************************************************************
* Data structure
*********************************************************************/
enum
{
	kDownloadState_None,
	kDownloadState_Request,
	kDownloadState_RequestWithPost,
	kDownloadState_RequestWithChunkEncoding,
	kDownloadState_ReceiveHeader,
	kDownloadState_ReceiveContent,
	kDownloadState_Close,
	kDownloadState_Done,
	kDownloadState_Error,
	kDownloadState_Cancel,
	kDownloadState_Abort,
	kDownloadState_Max
};

typedef struct 
{
	HS_HTTPLibStream fHTTPStream;
	Int32 fState;
	Int32 fCurrentLength;
	Int32 fContentLength;
	Char *fBuffer;
	Int32 fBufferLen;
	Char *fPostData;
	Int32 fPostDataLen;
	Int32 fPostedLen;
} DownloadInfo;

/*********************************************************************
* Functions
*********************************************************************/
Err PrvHTTPLibInitialize(void)
{
    Err err = errNone;
    
    /* load the library */
	if( gRefNum == sysInvalidRefNum ) {
		err = SysLibFind(HS_HTTPLibName, &gRefNum);
	    if (err) {
	        err = SysLibLoad(HS_HTTPLibDBType, HS_HTTPLibID, &gRefNum);
		}
	    if (err) {
			return err;
	    }
	}
	err = HS_HTTPLibOpen(gRefNum);

	/* gPeer */
	MemSet(&gPeer, sizeof(HS_HTTPLibPeer), 0);
	gPeer.HS_HTTPLibPeerTCPOpen = &PrvTCPOpen;
	gPeer.HS_HTTPLibPeerTCPClose = &PrvTCPClose;
	gPeer.HS_HTTPLibPeerTCPIsConnected = &PrvTCPIsConnected;
	gPeer.HS_HTTPLibPeerTCPConnect = &PrvTCPConnect;
	gPeer.HS_HTTPLibPeerTCPRead = &PrvTCPRead;
	gPeer.HS_HTTPLibPeerTCPWrite = &PrvTCPWrite;
	gPeer.HS_HTTPLibPeerTCPCanReadWrite = &PrvTCPCanReadWrite;
	
	gAppInfo.maxSockets = 3;
	gAppInfo.isForeground = true;
	gAppInfo.cookieMaxJarSize = (UInt16)300 * (UInt16)1024;

	PrvPeerInitialize();
	gLibHandle = HS_HTTPLibInitialize(gRefNum, &gAppInfo, &gNetLibInfo, &gPeer);
	
	/* set callbacks */
	//HS_HTTPLibSetSSLServerCertConfirmProc(gRefNum, gLibHandle, &test_confirm_cb, (HS_HTTPLibOpaque)gLibHandle);
	//HS_HTTPLibSetTunnelingCallback(gRefNum, gLibHandle, &PrvTunnelingCallback, NULL);
	
	/* set timeout time */
	HS_HTTPLibSetConnectTimeOut(gRefNum, gLibHandle, -1);
	HS_HTTPLibSetReqTimeOut(gRefNum, gLibHandle, -1);
	HS_HTTPLibSetRspTimeOut(gRefNum, gLibHandle, 10 * 1000);
	
	/* set proxy info */
	//HS_HTTPLibSetProxy(gRefNum, gLibHandle, ProxyHost, StrLen(ProxyHost), ProxyPort, ProxyPort, NoProxyHost, 0);
	//HS_HTTPLibSetUseProxy(gRefNum, gLibHandle, true);
	
	return err;
}

Err PrvHTTPLibFinalize(void)
{
	Err err;
	UInt16 count;
	
	HS_HTTPLibFinalize(gRefNum, gLibHandle);
	PrvPeerFinalize();
	
	err = HS_HTTPLibClose(gRefNum, &count);
	
	/* unload the library */
	if( gRefNum != sysInvalidRefNum ){
		SysLibRemove(gRefNum);
		gRefNum = sysInvalidRefNum;
	}
	return err;
}

Int32 
PrvTunnelingCallback(HS_HTTPLibStream in_stream, Char* buf, Int32 len, void *aux)
{
    return 0;
}

#if __SKIP_WRITE
/*********************************************************************
* Store Functions
*********************************************************************/
static void 
set_text(Char *str)
{
    FileHand file;
    UInt32 mode = fileModeAppend;
    Int32 nwritten = 0;
    Err err = errNone;
    
    if (str == NULL) return;
    
    file = FileOpen (0, "httpdata.pdb", 0, 0, mode, &err);
    if (file != 0) {
        nwritten = FileWrite (file, str, StrLen(str), 1, &err);
        FileClose(file);
    }
}
#endif

/*********************************************************************
* Download Functions
*********************************************************************/
static Int32
download_open(DownloadInfo *self)
{
	return 0;//dummy
}
static Int32
download_write(DownloadInfo *self, Int32 in_len)
{
#if __SKIP_WRITE // this code works, but don't want overhead for throughput test
    Char *buffer;
    
    if (in_len == 0) return 0;
    
    buffer = MemPtrNew(in_len);
    if (buffer) {
        MemMove(buffer, self->fBuffer, in_len);
    	set_text(buffer);
    	buffer[in_len] = 0;
    	MemPtrFree(buffer);
    }
#endif
	return 0;//dummy
}
static void
download_close(DownloadInfo *self)
{
	if(self->fHTTPStream){
		HS_HTTPLibStreamClose(gRefNum, self->fHTTPStream );	/* Close HTTP */
		HS_HTTPLibStreamDelete(gRefNum, self->fHTTPStream );
		self->fHTTPStream = NULL;
	}
}
static Int32
download_do_request(DownloadInfo *self)
{
	Int32 result;
	result = HS_HTTPLibStreamSendRequest(gRefNum, self->fHTTPStream, NULL);
	if(result == httpErrorOK){
		self->fState = kDownloadState_ReceiveHeader;
	}else if(result == httpErrorWouldBlock){
		/* do nothing */
	}else{
		self->fState = kDownloadState_Error;
	}
	result = httpErrorWouldBlock;
	return result;
}

static Int32
download_do_request_withpost(DownloadInfo *self)
{
	Int32 result;
	Int32 writtenLen;
	
	result = HS_HTTPLibStreamSendRequestWithPostData(gRefNum, self->fHTTPStream, self->fPostData, self->fPostDataLen - self->fPostedLen, &writtenLen, NULL);
	if(writtenLen > 0){
		self->fPostData += writtenLen;
		self->fPostedLen += writtenLen;
	}
	if(result == httpErrorOK){
		self->fState = kDownloadState_ReceiveHeader;
	}else if(result == httpErrorWouldBlock){
		/* do nothing */
	}else{
	    Char error[30];
	    StrIToA(error, result);
	    FrmCustomAlert(ErrorAlert, "request withpost", error, "");	    
		self->fState = kDownloadState_Error;
	}
	result = httpErrorWouldBlock;
	return result;
}

static Int32
download_receive_header(DownloadInfo *self)
{
	Int32 result;
	Int32 len=0;
	//HS_HTTPLibVHandle hdrH=NULL;
	
	result = HS_HTTPLibStreamReceiveResponse(gRefNum, self->fHTTPStream, NULL, 0, &len, NULL);
	//hdrH = HS_HTTPLibStreamGetResponseHeader(gRefNum, self->fHTTPStream);

	switch(result){
	case httpErrorOK:
		//self->fState = kDownloadState_ReceiveContent;
	case httpErrorWouldBlock:
		if(HS_HTTPLibStreamIsHeaderReceived(gRefNum, self->fHTTPStream)){
			self->fState = kDownloadState_ReceiveContent;
			self->fContentLength = HS_HTTPLibStreamGetContentLength(gRefNum, self->fHTTPStream);
		}
		else
		{
			Int32	hLen;
			hLen = HS_HTTPLibStreamGetResponseHeaderLength(gRefNum, self->fHTTPStream);

			// when hLen > 0, start content timer
			// everything up to here is latency!
			glatencyTicks = TimGetTicks();
		}
	break;
	default:
		self->fState = kDownloadState_Error;
	break;
	}
	result = httpErrorWouldBlock;
	return result;
}

static Int32
download_receive_content(DownloadInfo *self)
{
	Int32 result;
	Int32 len;
	result = HS_HTTPLibStreamReceiveResponse(gRefNum, self->fHTTPStream, 
										self->fBuffer, self->fBufferLen, &len, NULL);
	switch(result){
	case httpErrorOK:
	case httpErrorWouldBlock:
	case httpErrorIOSleep:
		if(len > 0){
			download_write(self, len);
			self->fCurrentLength += len;
		} else
		{
			if(len == 0){
				download_write(self, len);/* for ExchangeManager GUI */
			}
		}
		
		if(self->fContentLength > 0){
			if(self->fContentLength <= self->fCurrentLength){
				self->fState = kDownloadState_Close;
			}
		}
		if(result == httpErrorOK){
			self->fContentLength += self->fCurrentLength;
			self->fState = kDownloadState_Close;
		}
	break;
	default:
		self->fState = kDownloadState_Error;
	break;
	}
	result = httpErrorWouldBlock;
	return result;
}
static Int32
download_receive_close(DownloadInfo *self)
{
	download_close(self);
	self->fState = kDownloadState_Done;
	return httpErrorWouldBlock;
}
static Int32
download_receive_done(DownloadInfo *self)
{
	return httpErrorOK;
}
static Int32
download_receive_error(DownloadInfo *self)
{
	download_close(self);
	self->fState = kDownloadState_Abort;
	gErrorFlag = true;
	return httpErrorWouldBlock;
}
static Int32
download_receive_cancel(DownloadInfo *self)
{
	download_close(self);
	self->fState = kDownloadState_Abort;
	return httpErrorWouldBlock;
}
static Int32
download_receive_abort(DownloadInfo *self)
{
	return httpErrorOK;
}

static DownloadInfo *
DonwloadInfoNew(Char *urlP)
{
	DownloadInfo *infoP = NULL;
	
	infoP = (DownloadInfo *)MemPtrNew(sizeof(DownloadInfo));
	if(!infoP) return NULL;
	
	MemSet(infoP, sizeof(DownloadInfo), 0);
	infoP->fState = kDownloadState_Request;
	infoP->fCurrentLength = 0;
	infoP->fContentLength = 0;
	infoP->fBufferLen = DOWNLOAD_BUFFER_SIZE;
	infoP->fPostDataLen = 0;
	infoP->fPostData = NULL;
	infoP->fPostedLen = 0;
	
	infoP->fBuffer = MemPtrNew(DOWNLOAD_BUFFER_SIZE);
	
	infoP->fHTTPStream = HS_HTTPLibStreamNew(gRefNum, gLibHandle);
	if(!infoP->fHTTPStream) goto error;
	
	download_open(infoP);
	
	return infoP;

error:
	if(infoP->fBuffer){
		MemPtrFree(infoP->fBuffer);
	}
	if(infoP->fHTTPStream){
		HS_HTTPLibStreamClose(gRefNum, infoP->fHTTPStream);	/* Close HTTP */
		HS_HTTPLibStreamDelete(gRefNum, infoP->fHTTPStream);
	}
	MemPtrFree(infoP);
	return NULL;
}
static void
DonwloadInfoFree(DownloadInfo *self)
{
	if(!self) return;
	if(self->fBuffer){
		MemPtrFree(self->fBuffer);
	}
	
	if(self->fHTTPStream){
		HS_HTTPLibStreamClose(gRefNum, self->fHTTPStream);	/* Close HTTP */
		HS_HTTPLibStreamDelete(gRefNum, self->fHTTPStream);
	}
	MemPtrFree(self);
	
}
static Int32
DownloadInfoProgress(DownloadInfo *self)
{
	Int32 result = httpErrorOK;
	
	switch(self->fState){
	case kDownloadState_None:
	break;
	case kDownloadState_Request:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Requesting");
		result = download_do_request(self);
	break;
	case kDownloadState_RequestWithPost:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Requesting w/ post");
		result = download_do_request_withpost(self);
	break;
	case kDownloadState_ReceiveHeader:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Receive Header");
		result = download_receive_header(self);
		//HsExtTraceText (gTraceContext, kHsExtTraceLevelDevInfo,
	  //		"result: %ld", result);
	break;
	case kDownloadState_ReceiveContent:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Receive Content");
		result = download_receive_content(self);
	break;
	case kDownloadState_Close:
		result = download_receive_close(self);
	break;
	case kDownloadState_Done:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Done");
		result = download_receive_done(self);
	break;
	case kDownloadState_Error:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Error");
		result = download_receive_error(self);
	break;
	case kDownloadState_Cancel:
	  FrmSetTitle(FrmGetFormPtr(MainForm), NULL);
	  StrCopy(title, "Canceling");
		result = download_receive_cancel(self);
	break;
	case kDownloadState_Abort:
		result = download_receive_abort(self);
	break;
	default:
		result = httpErrorOK;
	break;
	}
	FrmSetTitle(FrmGetFormPtr(MainForm), title);
	return result;
}

void 
PrvHTTPLibTestGet(Char *urlP)
{
	DownloadInfo *self;
	Int32 result;
	UInt32 	secs;
	UInt32	msecs;
	UInt32	ticks;
	UInt32	ticksPerSecond=SysTicksPerSecond();
	UInt32	thruput;

	self = DonwloadInfoNew(urlP);
	
	result = HS_HTTPLibStreamCreateRequest(gRefNum, self->fHTTPStream, httpMethodGET, 
			urlP, StrLen(urlP), "", 0, "", 0, 
			httpVersion_1_1, false, false, httpSSLFlagConnV2V3TLS);
	// override the "gzip, deflate" default
	HS_HTTPLibStreamAddHeader(gRefNum, self->fHTTPStream, httpHeaderIDAcceptEncoding, "*/*", 3);	

	result = -1;
	
	// start timer
	gstartTicks = TimGetTicks ();
	gErrorFlag = false;
	
	while(result != httpErrorOK){
		result = DownloadInfoProgress(self);
	}
	
	// stop timer
	gstopTicks = TimGetTicks();
	
	if (!gErrorFlag)
	{
		secs = (gstopTicks-gstartTicks) / ticksPerSecond;
		ticks = (gstopTicks-gstartTicks) % ticksPerSecond;
		msecs = (ticks * 1000) / ticksPerSecond;
		printf("Total Time  : %ld s %ld ms\n", secs, msecs);
		secs = (glatencyTicks-gstartTicks) / ticksPerSecond;
		ticks = (glatencyTicks-gstartTicks) % ticksPerSecond;
		msecs = (ticks * 1000) / ticksPerSecond;
		printf("Resp Latency: %ld s %ld ms\n", secs, msecs);
		printf("Total Bytes : %ld bytes\n", self->fContentLength);
		if (gstopTicks-glatencyTicks){ // Simulator sometimes showed '0' time elapsed
			thruput = (self->fContentLength*ticksPerSecond)/(gstopTicks - glatencyTicks);
			printf("Throughput  : %ld bytes/sec\n", thruput);
		}
		else
			;
	}
	else
	{
		;
	}

	DonwloadInfoFree(self);
}

void 
PrvHTTPLibTestPost(Char *urlP, Char *postDataP)
{
	DownloadInfo *self;
	Int32 result;

	self = DonwloadInfoNew(urlP);
	
	result = HS_HTTPLibStreamCreateRequest(gRefNum, self->fHTTPStream, httpMethodPOST, 
		urlP, StrLen(urlP), "", 0, "", 0, 
		httpVersion_1_1, false, false, httpSSLFlagConnV2V3TLS);
	
	HS_HTTPLibStreamSetPostData(gRefNum, self->fHTTPStream, httpPostURLENCODED, NULL, StrLen(postDataP));
	self->fPostData = postDataP;
	self->fPostDataLen = StrLen(postDataP);
	
	self->fState = kDownloadState_RequestWithPost;
	result = -1;
	while(result != httpErrorOK){
		result = DownloadInfoProgress(self);
	}
	DonwloadInfoFree(self);
}
