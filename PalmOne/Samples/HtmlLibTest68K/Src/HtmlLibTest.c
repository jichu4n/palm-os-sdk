/******************************************************************************
 * HtmlLibTest.c
 * Copyright (c) 2004-2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

#define ERROR_CHECK_LEVEL ERROR_CHECK_FULL

#include <PalmOS.h>
#include <HtmlLibTestRsc.h>
#include <HtmlLib68K.h>
#include <VFSMgr.h>
#include <HsKeyCommon.h>


/* Local variables */
static MemHandle gHtmlLibH = NULL;
static MemHandle gContentObjH = NULL;
static HtmlDisplayMode gDisplayMode = htmlHandheldDisplayMode;
static HtmlLibFormWidgetProc *widgetProc = NULL;

static UInt16 gRefNum = sysInvalidRefNum;
static Boolean gWideDisplay;

/* Extern */
extern Int32 gEventWait;

/* Internal functions */
static Char* AllocFileName(Char *in_fname);
FileRef OpenFile(Char *in_fname, UInt16 mode);


/***********************************************************************
 *
 * FUNCTION:    Render_RequestData
 *
 * DESCRIPTION: Render user request data
 *
 * PARAMETERS:  url: the url where the data are rendered
 *             
 * RETURNED:    None
 *
 ***********************************************************************/
static void Render_RequestData(Char *url)
{
	Char data[100] = "<html><body>test</body></html>";
	
	/* Destroy old content before rendering new one */
	HtmlLibAbortRenderData (gRefNum, gHtmlLibH);
	if (gContentObjH) {
		HtmlLibDestroyContentObject(gRefNum, gContentObjH);
		gContentObjH = NULL;
	}
	
	/* Create new contnet */
	gContentObjH = HtmlLibCreateContentObject(gRefNum, gHtmlLibH);
	if (gContentObjH) {
		HtmlLibAddTextData(gRefNum, gContentObjH, url, "text/html", "us-ascii", data, StrLen(data));
		HtmlLibRenderData(gRefNum, gContentObjH);
	}
}


/***********************************************************************
 *
 * FUNCTION:	LinkSelectionCallback 
 *
 * DESCRIPTION: Callback function after users click on a link on test1 page
 *
 * PARAMETERS:  htmlLibH:	opened HTMLLib handle
 *				url:		url of the link
 *				cbData:		reserved
 *             
 * RETURNED:    None
 *
 ***********************************************************************/
static void
LinkSelectionCallback(MemHandle htmlLibH, Char *url, void *cbData)
{
	FrmCustomAlert(LinkSelectAlert, url, NULL, NULL);
	Render_RequestData(url);
}


/***********************************************************************
 *
 * FUNCTION:    SubmitFormCallback
 *
 * DESCRIPTION: Callback function after users submit a URL on test2 page
 *
 * PARAMETERS:  htmlLibH:	opened HTMLLib handle
 *				URL: 		html page address
 *				method: 	post or get
 *				query: 		submit query
 *				cbData: 	reserved
 *             
 * RETURNED:    None 
 *
 ***********************************************************************/
static void
SubmitFormCallback(MemHandle htmlLibH, Char *url, HtmlSubmitFormMethod method, Char *query, void *cbData) 
{
	FrmCustomAlert(SubmitFormAlert, url, 
		(method == htmlHttpMethodPost ? "Post" : "Get"), query);
}


/***********************************************************************
 *
 * FUNCTION:    ScanPhoneNumberCallback
 *
 * DESCRIPTION: Callback function to extract the phone number on hyperlink
 *
 * PARAMETERS:  htmlLibH: 	opened HTMLLib handle
 *				buffer: 	buffer saved the phone number
 *				length: 	length of the phone number
 *             
 * RETURNED:    True: 		success
 *				False: 		fail
 *
 ***********************************************************************/
static Boolean
ScanPhoneNumberCallback(MemHandle htmlLibH, Char *buffer, Int32 length, Int32 *patternStart, Int32 *patternEnd)
{
	*patternStart = 0;
	*patternEnd = length-1;
	return true;
	
	/* If you don't want to display a phone number as a hyperlink, */
	/* comments out the code above and only leave the following code */	
	/* return false ;*/
}


/***********************************************************************
 *
 * FUNCTION:	InclusionCallback   
 *
 * DESCRIPTION: Callback function to render image data
 *
 * PARAMETERS:  htmlLibH: 	opened HTMLLib handle
 *				url: 		image name / url
 *				cbData: 	reserved
 *             
 * RETURNED:    None
 *
 ***********************************************************************/
static void
InclusionCallback(MemHandle htmlLibH, Char *url, void *cbData)
{
	FileRef file;
	Err err;

	/* Open image file */
	file = OpenFile(url, vfsModeRead);

	if (file != (FileRef)-1) 
	{
		void *buf;
		UInt32 size = 0;
		
		if (file)
		{
			/* Obtain the size of an open file */
			if(VFSFileSize (file, &size)!=errNone) 
				return;
		}
		if (size == 0)
		{
			FrmAlert(ImageSizeErrAlert);	
			return ;
		}
		
		/* Allocate memory for the file data */
		buf = MemPtrNew(size);
		if (buf) 
		{
			/* Read image file */
			err = VFSFileRead(file, size, buf, &size);
			if (err != errNone)
			{
				FrmAlert(FileReadErr);
			}
			
			/* Add image data to HtmlLib */
			err = HtmlLibAddImageData(gRefNum, gContentObjH, url, "image/jpeg", buf, size);
			if (err)
			{
				FrmAlert(ImageRenderErr);
			}
			MemPtrFree(buf);
		}
		else
		{
			FrmAlert(MemoryAllocateErr);
			return ;
		}
		
		VFSFileClose(file);
	}

	return;

}


/***********************************************************************
 *
 * FUNCTION:    RendererNewControlProc
 *
 * DESCRIPTION: Create a new control oject in render form
 *
 * PARAMETERS:  Parameters needed to call CtlNewControl
 *             
 * RETURNED:    True: 	success
 *				False: 	fail
 *
 ***********************************************************************/
static Boolean
RendererNewControlProc(UInt16 ID, 
 	UInt16 style, const Char *textP, 
	Coord x, Coord y, Coord width, Coord height, 
	UInt16 font, UInt8 group, Boolean leftAnchor)

{
	ControlType *ctlP;
	FormType *formP = FrmGetActiveForm();
	ctlP = CtlNewControl((void**)&formP, ID, style, textP, x, y, width, height, font, group, leftAnchor);
	if (ctlP) {
		return true;
	}
	return false;
}


/***********************************************************************
 *
 * FUNCTION:    RendererNewFieldProc
 *
 * DESCRIPTION: Draw render field on screen
 *
 * PARAMETERS:  Parameters needed to call FldNewField
 *             
 * RETURNED:    True: 	success
 *				False: 	fail
 *
 ***********************************************************************/
static Boolean
RendererNewFieldProc(UInt16 id, 
	Coord x, Coord y, Coord width, Coord height, 
	UInt16 font, UInt32 maxChars, UInt16 editable, UInt16 underlined, 
	UInt16 singleLine, UInt16 dynamicSize, UInt16 justification, 
	UInt16 autoShift, UInt16 hasScrollBar, UInt16 numeric)

{
	FieldType *fieldP;
	FormType *formP = FrmGetActiveForm();
	fieldP = FldNewField((void**)&formP, id, x, y, width, height, (FontID)font,
					maxChars, (Boolean)editable, (Boolean)underlined, (Boolean)singleLine,
					(Boolean)dynamicSize, (JustificationType)justification, (Boolean)autoShift, 
					(Boolean)hasScrollBar, (Boolean)numeric);
	if (fieldP) {
		return true;
	}
	return false;
}


/***********************************************************************
 *
 * FUNCTION:	RendererNewListProc
 *
 * DESCRIPTION: Render a new list on the active form
 *
 * PARAMETERS:  Parameters needed to call LstNewList
 *             
 * RETURNED:    err message
 *
 ***********************************************************************/
static Err 
RendererNewListProc(UInt16 id, 
	Coord x, Coord y, Coord width, Coord height, 
	UInt16 font, Int16 visibleItems, Int16 triggerId)
{
	FormType *formP = FrmGetActiveForm();
	return LstNewList((void**)&formP, id, x, y, width, height, font,
						visibleItems, triggerId);
}


/***********************************************************************
 *
 * FUNCTION:	RendererDeleteObjectProc 
 *
 * DESCRIPTION: Delete render content object
 *
 * PARAMETERS:  index: content object ID
 *             
 * RETURNED:    None
 *
 ***********************************************************************/
static void
RendererDeleteObjectProc(UInt16 index)
{
	FormType *formP = FrmGetActiveForm();
	FrmRemoveObject(&formP, index);
}


/***********************************************************************
 *
 * FUNCTION:	RendererInitialize 
 *
 * DESCRIPTION: Open and Initialize HTMLLib
 *
 * PARAMETERS:  None
 *             
 * RETURNED:    True: 	success
 *				False: 	fail
 *
 ***********************************************************************/
Boolean RendererInitialize(void)
{
	RectangleType bounds;
	Err err = errNone;
	UInt16 scrollbars[6] = {
		RenderScroll0ScrollBar, RenderScroll1ScrollBar, RenderScroll2ScrollBar, 
		RenderScroll3ScrollBar, RenderScroll4ScrollBar, RenderScroll5ScrollBar
	};

	bounds.topLeft.x = 2;
	bounds.topLeft.y = 20;
	bounds.extent.x = 140;
	bounds.extent.y = 100;
	
	gWideDisplay = false; 
	
	if (gRefNum == sysInvalidRefNum) {
		err = SysLibFind(HtmlLibName, &gRefNum);
		if (err) {
			err = SysLibLoad(HtmlLibDBType, HtmlLibID, &gRefNum);			
		}
		if (err) {
			return false;
		}
	}
	
	err = HtmlLibOpen(gRefNum);
	if (err) {
		return false;
	}
	widgetProc = MemPtrNew(sizeof(HtmlLibFormWidgetProc));
	if (widgetProc) {
		widgetProc->newControlProc = RendererNewControlProc;
		widgetProc->newFieldProc = RendererNewFieldProc;
		widgetProc->newListProc = RendererNewListProc;
		widgetProc->deleteObjectProc = RendererDeleteObjectProc;
	}
	
	gHtmlLibH = HtmlLibInitialize(gRefNum, bounds, &err);
	if (err == errNone && gHtmlLibH) {
		HtmlLibSetDisplayMode(gRefNum, gHtmlLibH, gDisplayMode);
		HtmlLibSetLinkSelectionCallback(gRefNum, gHtmlLibH, LinkSelectionCallback, NULL);
		HtmlLibSetScanPhoneNumberCallback(gRefNum, gHtmlLibH, ScanPhoneNumberCallback);
		HtmlLibSetInclusionCallback(gRefNum, gHtmlLibH, InclusionCallback, NULL);
		HtmlLibSetSubmitFormCallback(gRefNum, gHtmlLibH, SubmitFormCallback, NULL);
		HtmlLibSetScrollbars(gRefNum, gHtmlLibH, scrollbars, 6);
		HtmlLibSetFormWidgetProc(gRefNum, gHtmlLibH, widgetProc);
		return true;
	}
	return false;
}


/***********************************************************************
 *
 * FUNCTION:	RendererFinalize 
 *
 * DESCRIPTION: Finalize HTMLLib
 *
 * PARAMETERS:  None
 *             
 * RETURNED:    None
 *
 ***********************************************************************/
void RendererFinalize(void)
{
	if (gHtmlLibH) {
		HtmlLibFinalize(gRefNum, gHtmlLibH);
	}
	HtmlLibClose(gRefNum);
	if (widgetProc) {
		MemPtrFree(widgetProc);
		widgetProc = NULL;
	}
	if (gRefNum != sysInvalidRefNum) {
		SysLibRemove(gRefNum);
		gRefNum = sysInvalidRefNum;
	}
}


/***********************************************************************
 *
 * FUNCTION:	RenderStart 
 *
 * DESCRIPTION: Initialize HTMLLib and render form 
 *
 * PARAMETERS:  None
 *             
 * RETURNED:    True: 	success
 * 				False: 	fail
 *
 ***********************************************************************/
Boolean RenderStart(void)
{
	void *data = NULL;
	UInt32 dataLen = 0;
	FormPtr mainFormPtr;

	/* Check one more time for gHtmlLibH */
	if (!gHtmlLibH) {
		if (!RendererInitialize()) {
			/* To do: error dialog */
			return false;
		}
	}

	/* Set screen size, if needed */
	
	/* Create content object */
	HtmlLibAbortRenderData (gRefNum, gHtmlLibH);
	if (gContentObjH) {
		HtmlLibDestroyContentObject(gRefNum, gContentObjH);
		gContentObjH = NULL;
	}
	gContentObjH = HtmlLibCreateContentObject(gRefNum, gHtmlLibH);
	if (!gContentObjH) {
		return false;
	}
	
	mainFormPtr = FrmGetFormPtr(MainForm);
	if (mainFormPtr) {
		UInt16 index = FrmGetObjectIndex(mainFormPtr, MainContentField);
		if (index != frmInvalidObjectId) {
			FieldPtr fldP = FrmGetObjectPtr(mainFormPtr, index);
			if (fldP) {
				data = FldGetTextPtr(fldP);
				if (data) {
					dataLen = StrLen(data);
					/* Add text data */
					if (HtmlLibAddTextData(gRefNum, gContentObjH, "test.html", "text/html", "us-ascii", data, dataLen) != errNone) {
						return false;
					}
					
					/* You could add image data here instead of adding it in the callback function */
					/* Refer Palm Developer Guide HTML chapter for more information */
					
					/* Render data */			
					HtmlLibRenderData(gRefNum, gContentObjH);
					gEventWait = 0;
				}
			}
		}
	}

	return true;
}


/***********************************************************************
 *
 * FUNCTION:	RenderEnd 
 *
 * DESCRIPTION: Clear render screen and destroy old content object
 *
 * PARAMETERS:  None
 *             
 * RETURNED:    None
 *
 ***********************************************************************/
void RenderEnd(void)
{
	/* clear screen */
	HtmlLibClearScreen(gRefNum, gHtmlLibH);

	/* destroy content object */
	if (gContentObjH) {
		HtmlLibDestroyContentObject(gRefNum, gContentObjH);
		gContentObjH = NULL;
	}
}


/***********************************************************************
 *
 * FUNCTION:    RenderFormHandleEvent
 *
 * DESCRIPTION: Main function to handle render form events
 *
 * PARAMETERS:  eventP: event parameter
 *             
 * RETURNED:    True:	event handled
 *				False: 	event not handled
 *
 ***********************************************************************/
Boolean RenderFormHandleEvent(EventPtr eventP)
{
	UInt16 status;
	FormPtr frmP = NULL;
	Boolean handled = false;

	switch (eventP->eType) {
	case frmOpenEvent:
		frmP = FrmGetActiveForm();
		FrmDrawForm(frmP);
		RenderStart();
		handled = true;
		break;

	case frmUpdateEvent:
		FrmDrawForm(FrmGetFormPtr(eventP->data.frmUpdate.formID));
		HtmlLibRedrawScreen(gRefNum, gHtmlLibH);
		handled = true;
		break;

	case ctlSelectEvent:
		if (eventP->data.ctlSelect.controlID == RenderDoneButton) {
			RenderEnd();
			FrmReturnToForm(MainForm);
			handled = true;
		} else {
			handled = HtmlLibNotifyUIEvent(gRefNum, gHtmlLibH, eventP);
		}
		break;

	case htmlLibProgressEvent:
		if (gHtmlLibH) {
			/* Let browser run for 30 seconds */
			status = HtmlLibProgress(gRefNum, gHtmlLibH, 3000);
			switch (status) {
			case htmlProgressBusy:
				/* Don't let OS idle too long */
				gEventWait = 0;
				break;
			case htmlProgressOutOfMemory:
				/* To do: Out of memory Dialog */
				break;
			case htmlProgressDone:
				gEventWait = 6000; /* 1 minute or something sensible */
				break;
			}
			handled = true;
		}
		break;

	case menuEvent:
		if (eventP->data.menu.itemID == PageFindTextOnPage) {
			FormPtr findFormPtr = FrmInitForm(FindForm);
			Char *searchT;
			if (findFormPtr) {
				if (FrmDoDialog(findFormPtr) == FindFindButton) {
					searchT = FldGetTextPtr(FrmGetObjectPtr(findFormPtr, FrmGetObjectIndex(findFormPtr, FindFindField)));
					if (searchT && *searchT) {
						if (HtmlLibFindText(gRefNum, gHtmlLibH, searchT, true) == htmlErrNotFound) {
							FrmAlert(TextNotFoundAlert);
						}
					}
				}
				FrmDeleteForm(findFormPtr);
			}
			handled = true;
		} else if (eventP->data.menu.itemID == PageGetTextSelection) {
			Char *selectedText = NULL;
			if (HtmlLibGetTextSelection(gRefNum, gHtmlLibH, &selectedText)) {
				FrmCustomAlert(SelectedTextAlert, selectedText, NULL, NULL);
				if (selectedText) {
					MemPtrFree(selectedText);
				}
			} else {
				FrmAlert(NoSelectedTextAlert);
			}
			handled = true;
		} else if (eventP->data.menu.itemID == PageWideDisplay) {
			HtmlLibSetDisplayMode(gRefNum, gHtmlLibH, htmlFaithfulDisplayMode);
			HtmlLibRedrawScreen(gRefNum, gHtmlLibH);
			gDisplayMode = htmlFaithfulDisplayMode;
			/* force menu update */
			FrmSetMenu(FrmGetActiveForm(), RenderMenuBar);
			handled = true;
		} else if (eventP->data.menu.itemID == PageNarrowDisplay) {
			HtmlLibSetDisplayMode(gRefNum, gHtmlLibH, htmlHandheldDisplayMode);
			HtmlLibRedrawScreen(gRefNum, gHtmlLibH);
			gDisplayMode = htmlHandheldDisplayMode;
			/* force menu update */
			FrmSetMenu(FrmGetActiveForm(), RenderMenuBar);
			handled = true;
		} else if (eventP->data.menu.itemID == PageResizeScreen) {
			RectangleType bounds;

			if(gWideDisplay) { // Set to narrow display
				bounds.topLeft.x = 2;
				bounds.topLeft.y = 20;
				bounds.extent.x = 140;
				bounds.extent.y = 100;
				gWideDisplay = false;		
			} else { // Set to wide display
				bounds.topLeft.x = 0;
				bounds.topLeft.y = 20;
				bounds.extent.x = 160;
				bounds.extent.y = 120;
				gWideDisplay = true;
			}
		
			HtmlLibResizeScreen(gRefNum, gHtmlLibH, bounds);

			handled = true;
		}
		break;

	case menuOpenEvent:
		if (gDisplayMode == htmlHandheldDisplayMode) {
			MenuShowItem(PageWideDisplay);
			MenuHideItem(PageNarrowDisplay);
		} else {
			MenuShowItem(PageNarrowDisplay);
			MenuHideItem(PageWideDisplay);
		}		
		break;

    case keyDownEvent:
        if(eventP->data.keyDown.chr == vchrRockerCenter && eventP->data.keyDown.chr == vchrHardRockerCenter)
           handled = false;
        break;
        
	default:
		handled = HtmlLibNotifyUIEvent(gRefNum, gHtmlLibH, eventP);
		break;
	}
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    AllocFileName
 *
 * DESCRIPTION: Get appropriate file name
 *
 * PARAMETERS:  in_fname: the file name in html page
 *             
 * RETURNED:    Appropriate file name for VFS to open
 *
 ***********************************************************************/
static Char*
AllocFileName(Char *in_fname)
{
	Char *fname, *ptr, *str;
	
	fname = MemPtrNew( StrLen(in_fname) + 1 + 1);
	if (fname == NULL)
		return NULL;
	
	ptr = fname;
	if (in_fname[0] != '/')
	{
		*ptr = '/';
		ptr++;
	}
	
	for (str = ptr; *in_fname ; in_fname++, str++)
	{
		*str = *in_fname;
	}
	
	*str = '\0';
	return fname;
}


/***********************************************************************
 *
 * FUNCTION:	OpenFile
 *
 * DESCRIPTION: Open a specified file
 *
 * PARAMETERS:  in_fname:	file name
 *				mode:		the mode to open the file with
 *             
 * RETURNED:    The file reference number
 *
 ***********************************************************************/
FileRef
OpenFile(Char *in_fname, UInt16 mode)
{
	Char *fname;
	FileRef file;
	Int32 ret;
	UInt32 volIterator = vfsIteratorStart;
	UInt16 volRefNum = 0xffff;
	
	/* Look for volume reference number */
	while (volIterator != vfsIteratorStop)
	{
		if (VFSVolumeEnumerate (&volRefNum, &volIterator)) 
		{
			break;
		}
	}

	/* Get approprite file name */
	fname = AllocFileName(in_fname);
	if (fname == NULL)
		return (FileRef)-1;

	/* Open the file */
	ret = VFSFileOpen(volRefNum ,fname, mode , &file);
	if(ret != errNone)
	{
		FrmAlert(ImageNotFoundAlert);
	}
		
	MemPtrFree(fname);
	
	if (ret) return (FileRef)-1;
	return file;
}

