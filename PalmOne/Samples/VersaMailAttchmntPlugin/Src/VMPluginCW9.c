/*
 * VMPluginCW9.c
 *
 * main file for VMPluginCW9
 *
 * This is a test program that shows how to write a sample plugin
 * for versamail. This Plugin helps you view text files. It handles
 * four launch codes, which are used by versamail to launch this 
 * app.
 *
 * Copyright (c) 1999-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */

#include <PalmOS.h>
#include <PalmOSGlue.h>
#include "VMPluginCW9_Res.h"
#include "PalmVMPlugin.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

/*********************************************************************
 * Global variables
 *********************************************************************/

 
/*********************************************************************
 * Internal Constants
 *********************************************************************/
 #define appError appErrorClass+1

/*********************************************************************
 * Function Prototypes
 *********************************************************************/

/* Plugin utility functions */
static Err PrvHandleGetInfo(void* cmdPBP);
static Err PrvHandleQuery(void* cmdPBP);
static Err PrvHandleSend(void* cmdPBP);
static Err PrvHandleReceive(void* cmdPBP);

/* UI related functions for text viewer */
static void ViewScroll(Word fieldID,Word scrollID,Short linesToScroll);
static void ViewUpdateScrollBar (Word fieldID,Word scrollID);
static void ViewPageScroll (Word fieldID,Word scrollID,	WinDirectionType direction);
static Boolean ViewFormHandleEvent(void);
static MemPtr GetObjectPtr (UInt16 objectID);

/*********************************************************************
 * FUNCTION: PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * 
 * PARAMETERS:
 *
 * cmd
 *     word value specifying the launch code. 
 *
 * cmdPB
 *     pointer to a structure that is associated with the launch code
 *
 * launchFlags
 *     word value providing extra information about the launch.
 *
 * RETURNED:
 *     Result of launch, errNone if all went OK
 */

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{

		
	Err err = sysErrParamErr; /* If it gets a unknown launch code */
	
	FrmAlert (VMTextPluginAlert);


	switch (cmd)
	{
	
		case MMPRO_PLUGIN_GET_INFO_LAUNCHCODE:
	
			// We need to return the info requested by VersaMail
			err = PrvHandleGetInfo(cmdPBP);			
			break;
			
		case MMPRO_PLUGIN_QUERY_LAUNCHCODE:
			
			// We need to return a list of memo Items
			err = PrvHandleQuery(cmdPBP);
			break;
			
		case MMPRO_PLUGIN_SEND_LAUNCHCODE :
		
			// We need to return the item selected
			err = PrvHandleSend(cmdPBP);
			break;
			
		case MMPRO_PLUGIN_RECEIVE_LAUNCHCODE:
		
			// We need to show the text attachment passed to us
			err = PrvHandleReceive(cmdPBP);
			break;
			
		case MMPRO_PLUGIN_EXTENDED_QUERY_LAUNCHCODE:
		
			// Not supported by this Plugin.
			break;
			
		default:
			break;
	}

	return err;
}

#if 0
#pragma mark -
#endif

/*********************************************************************
 * FUNCTION: PrvHandleGetInfo
 *
 * DESCRIPTION: Handles MMPRO_PLUGIN_GET_INFO_LAUNCHCODE
 * 
 * 
 * PARAMETERS:
 *
 * cmdPB
 *   pointer to a structure that is associated MMPRO_PLUGIN_GET_INFO_LAUNCHCODE
 *
 *
 * RETURNED: None
 *    
 */
static Err PrvHandleGetInfo(void* cmdPBP)
{
	
	pluginGetInfoParams 	*params; /* GetInfo parameters */
	Int16					size;
	MemHandle namesPH = NULL, nameslistH = NULL;
	MemHandle extPH = NULL, extlistH = NULL;
	MemHandle mimePH = NULL, mimelistH = NULL;
	MemHandle rcvExtPH = NULL, rcvExtlistH = NULL;	
	MemHandle rcvMimePH = NULL, rcvMimeListH = NULL;
	
	params = (pluginGetInfoParams *)cmdPBP;
	
	// Our plugin supports sending and viewing so set it to true 
	params->supportsReceiving = true;
	params->supportsSending	 = true;
	
	// Version number has to be same as Versamail version number else
	// Plugin will not work.
	//
	params->version	= 0x26000;
	
	// If sendInfo is requested from the plugin 
	if( params->getInfo == plugin_sendInfo || params->getInfo == plugin_allInfo)
	{
			
		params->numSendTypes = 1; // We support only .txt type //
		
		// Copy the type the plugin supports 
		namesPH = MemHandleNew( sizeof(char*) * 1) ;
		if(!namesPH)
			goto Done;
		
		params->listSendNames =(char **)MemHandleLock( namesPH );
		size = ATTYPE_MAX_NAME_LEN;
		
		nameslistH = MemHandleNew(size);
		if(!nameslistH)			
			goto Done; 
		
		params->listSendNames[0] =(char *)MemHandleLock(nameslistH);

		// This is what appears on the drop down list,to 
		MemSet(params->listSendNames[0], size, 0);
		StrNCopy(params->listSendNames[0], "Text(.txt)", ATTYPE_MAX_NAME_LEN);			


		// Copy the File extension the plugin supports 
		extPH = MemHandleNew( sizeof(char*) * 1) ;
		if(!extPH)
			goto Done;
			
		params->listSendFileExt =(char **)MemHandleLock(extPH);
		
		size = StrLen("txt") + 1;		
		extlistH = MemHandleNew(size);
		if(!extlistH)
			goto Done;
			
		params->listSendFileExt[0] =(char *)MemHandleLock(extlistH);
		MemSet(params->listSendFileExt[0], size, 0);
		StrCopy(params->listSendFileExt[0], "txt");

		// Copy the MIME type the plugin supports 
		mimePH = MemHandleNew( sizeof(char*) * 1) ;
		if(!mimePH)
			goto Done;
			
		params->listSendMIMEtypes =(char **)MemHandleLock(mimePH);
		size = StrLen("text/plain") + 1;
		
		mimelistH = MemHandleNew(size);
		if(!mimelistH)
			goto Done;
			
		params->listSendMIMEtypes[0] =(char *)MemHandleLock(mimelistH);
		MemSet(params->listSendMIMEtypes[0], size, 0);
		StrCopy(params->listSendMIMEtypes[0], "text/plain");
	}
	
	// If receiveInfo is requested from the plugin 
	if( params->getInfo == plugin_receiveInfo || params->getInfo == plugin_allInfo)
	{
		params->numReceiveTypes	= 1; // We support only .txt type 

		// Copy the File extension the plugin supports 
		rcvExtPH = MemHandleNew( sizeof(char*) * 1) ;
		if(!rcvExtPH)
			goto Done;			
		params->listReceiveFileExt =(char **)MemHandleLock(rcvExtPH);		
				
		size = StrLen("txt") + 1;		
		rcvExtlistH = MemHandleNew(size);
		if(!rcvExtlistH)
			goto Done;
			
		params->listReceiveFileExt[0] =(char *)MemHandleLock(rcvExtlistH);
		MemSet(params->listReceiveFileExt[0], size, 0);
		StrCopy(params->listReceiveFileExt[0], "txt");

		// Copy the MIME type the plugin supports 
		rcvMimePH = MemHandleNew( sizeof(char*) * 1) ;
		if(!rcvMimePH)
			goto Done;			
		params->listReceiveMIMEtypes =(char **)MemHandleLock(rcvMimePH);
		
		size = StrLen("text/plain") + 1;
		rcvMimeListH = MemHandleNew(size);
		if(!rcvMimeListH)
			goto Done;
			
		params->listReceiveMIMEtypes[0] =(char *)MemHandleLock(rcvMimeListH);
		MemSet(params->listReceiveMIMEtypes[0], size, 0);
		StrCopy(params->listReceiveMIMEtypes[0], "text/plain");
		
	}

	return errNone;

Done:

	/* If it comes here then there is an error */
	
	/* Make sure we've freed everything you have allocated, and return a non
	 * zero error code to the plugin manager.
	 */
 	if(mimePH)
	{
		MemHandleUnlock(mimePH);
		MemHandleFree(mimePH);
	}
	
	if(extlistH)
	{
		MemHandleUnlock(extlistH);
		MemHandleFree(extlistH);
	}
	
	if(extPH)
	{
		MemHandleUnlock(extPH);
		MemHandleFree(extPH);
	}
	
	if(nameslistH)
	{
		MemHandleUnlock(nameslistH);
		MemHandleFree(nameslistH);
	}
	
	if(namesPH)
	{
		MemHandleUnlock(namesPH);
		MemHandleFree(namesPH);
	}
	
	if(rcvExtPH)
	{
		MemHandleUnlock(rcvExtPH);
		MemHandleFree(rcvExtPH);
	}
	
	if(rcvExtlistH)
	{
		MemHandleUnlock(rcvExtlistH);
		MemHandleFree(rcvExtlistH);
	}

	if(rcvMimePH)
	{	
		MemHandleUnlock(rcvMimePH);
		MemHandleFree(rcvMimePH);
	}
		
		
	return appError; // returning a non zero value 
}


/*********************************************************************
 * FUNCTION: PrvHandleQuery
 *
 * DESCRIPTION: Handles MMPRO_PLUGIN_QUERY_LAUNCHCODE
 * We need to return a list of attachment here 
 *
 * PARAMETERS:
 *
 * cmdPB
 *   pointer to a structure that is associated MMPRO_PLUGIN_QUERY_LAUNCHCODE
 *
 *
 * RETURNED: None
 *    
 */
 
static Err PrvHandleQuery(void* cmdPBP)
{

	pluginQueryParams* param=(pluginQueryParams*)cmdPBP;
	
	DmOpenRef dbRef;
	UInt16 numRecords = 0;
	MemHandle listItemH = NULL, itemDesH = NULL, itemSizeH = NULL;
	FormPtr originalForm = NULL, frm = NULL;
	Boolean outOfMem = false;	
	int index;
	UInt32	freeP, freeMax;
	
		
	// Open MemoDB 	
	dbRef = DmOpenDatabaseByTypeCreator('DATA', sysFileCMemo, dmModeReadWrite);
	
	if(!dbRef)
		goto Done;
		
	// Get the total records in it
	numRecords = DmNumRecords( dbRef );

	// Allocate memory for listItems
	if (NULL == (listItemH = MemHandleNew(numRecords * sizeof(char*))))
		goto Done; 
	param->listItems =(char **)MemHandleLock(listItemH);

	// Allocate memory for ItemDescriptors
	if (NULL == (itemDesH = MemHandleNew(numRecords * sizeof(void*))))
		goto Exit;		
	param->itemDescriptors = (void **)MemHandleLock( itemDesH );

	// Allocate Memory for itemSizes
	if (NULL == (itemSizeH = MemHandleNew(numRecords * sizeof(UInt32))))
		goto Exit;		
	param->itemSizes =(UInt32 *)MemHandleLock(itemSizeH);

	// If too much data needs to be processed, display the a wait form
	if (numRecords > 100)
	{
		// hold the original form
		originalForm =  FrmGetActiveForm();		
		
		// initialize our form and field info
		frm = FrmInitForm (PleasewaitForm);
		FrmSetActiveForm (frm);
		FrmDrawForm (frm);
		
	}
	
	// For each record populate the params to pass back to VersaMail
	for (index = 0; index < numRecords && !outOfMem; index++)
	{
		MemHandle recHandle,tempHandle;
		Char* recText;
		int len;
		UInt32	uniqueID;
		
		// Get the Record
		recHandle = DmQueryRecord ( dbRef , index);
		
		if (!recHandle)
			continue;
			
		recText=(char *)MemHandleLock(recHandle);
		len = StrLen(recText);
		
		if (len>28)
			len=28;
			
		if (StrChr(recText,'\n'))
		{
			if (len > StrChr(recText,'\n')-recText )
				len = StrChr(recText,'\n')-recText;
		}
		
		// Check for Free Memory on dynamic heap
		MemHeapFreeBytes(0, &freeP, &freeMax);
		
		if( freeP < 1000 ) // less than a K
		{
	
			outOfMem = true;
		}
		else
		{
			//Load listems value
			tempHandle = MemHandleNew(len+1);
			
			param->listItems[param->numItems]=(char *)MemHandleLock(tempHandle);
			MemSet(param->listItems[param->numItems],len+1,0);
			StrNCopy(param->listItems[param->numItems],recText,len);
			*(param->listItems[param->numItems]+len) = '\0';		//--- Null terminate
			
			// Load itemDescriptor
			tempHandle = MemHandleNew(sizeof(UInt32));
			param->itemDescriptors[param->numItems] =(char *)MemHandleLock(tempHandle);
			DmRecordInfo(dbRef, index, 0L, &uniqueID, 0L);
			*((UInt32 *)(param->itemDescriptors[param->numItems])) = uniqueID;
			
			//Load itemSize
			param->itemSizes[param->numItems] = MemHandleSize(recHandle);
			
			param->numItems++;		
		}
		
		MemHandleUnlock(recHandle);
		DmReleaseRecord ( dbRef, index, false);
		
	}
	
	// Remove our Wait form and restore original form
	if (numRecords > 100 )
	{
		FrmSetFocus (frm, noFocus);
		FrmEraseForm (frm);
		FrmDeleteForm (frm);
		if (originalForm)
			FrmSetActiveForm (originalForm);
	
	}
	
	DmCloseDatabase( dbRef );
	return errNone;

// If we are here there has been an Error

Exit:

	if(listItemH)
	{	
		MemHandleUnlock(listItemH);
		MemHandleFree(listItemH);
	}
	
	if(itemDesH)
	{	
		MemHandleUnlock(itemDesH);
		MemHandleFree(itemDesH);
	}
	

Done:	
	DmCloseDatabase( dbRef );
	
	return appError;

}

/*********************************************************************
 * FUNCTION: PrvHandleSend
 *
 * DESCRIPTION: Handles MMPRO_PLUGIN_SEND_LAUNCHCODE
 * 
 * PARAMETERS:
 *
 * cmdPB
 *   pointer to a structure that is associated MMPRO_PLUGIN_SEND_LAUNCHCODE
 *
 *
 * RETURNED: None
 *    
 */

static Err PrvHandleSend(void* cmdPBP)
{

	pluginSendParams* param=(pluginSendParams*)cmdPBP;
	
	DmOpenRef dbRef;
	UInt16 numRecords = 0;
	UInt16 index;
	MemHandle recHandle = NULL, itemNameH = NULL, mimeTypeH = NULL, fileExtH = NULL, handle;
	Char* recText;
	FormPtr originalForm = NULL;
	FormPtr frm = NULL;
								
	// Open MemoDB 	
	dbRef = DmOpenDatabaseByTypeCreator('DATA', sysFileCMemo, dmModeReadWrite);
	
	if(!dbRef)
		goto Done;
		
	// Get the total records in it
	numRecords = DmNumRecords( dbRef );

	// If too much data needs to be processed, display the a wait form
	if (numRecords > 100)
	{
		// hold the original form
		originalForm =  FrmGetActiveForm();		
		
		// initialize our form and field info
		frm = FrmInitForm (PleasewaitForm);
		FrmSetActiveForm (frm);
		FrmDrawForm (frm);
		
	}
	
	// param->selectedDescriptor contains a pointer to a UInt32 containing the uniqueID of the rec.
	DmFindRecordByID(dbRef, *((UInt32 *)(param->selectedDescriptor)), &index);

	// Get the Record
	recHandle = DmQueryRecord ( dbRef , index);	
	recText=(char *)MemHandleLock(recHandle);	
	
	itemNameH = MemHandleNew(StrLen(param->item)+1 + StrLen(".txt") );
	if(!itemNameH)
		goto Done;	
	param->name =(char *)MemHandleLock(itemNameH);
	
	// this is what the attachment is called
	StrCopy(param->name,param->item);
	StrCat(param->name,".txt");
	
	// MIME and Extension 		    
	mimeTypeH = MemHandleNew(StrLen("text/plain")+1) ;
	if(!mimeTypeH)
		goto Exit;
	param->MIMEtype = (Char*)MemHandleLock(mimeTypeH);
	MemSet(param->MIMEtype, StrLen("text/plain")+1, 0); 
	    
	fileExtH = MemHandleNew(StrLen("txt")+1) ;
	if(!fileExtH)
		goto Exit;	
	param->fileExt = (Char*)MemHandleLock(fileExtH);
	MemSet(param->fileExt,StrLen("txt")+1, 0); 

	StrCopy(param->MIMEtype,"text/plain");
	StrCopy(param->fileExt, "txt");
	
	
	// allocate some memory, lock it down, and copy
	// the memo pad item into it
	// (this gets passed back to the caller)
	handle = DmNewHandle(param->db,StrLen(recText)+1);
	param->ptr =(char *)MemHandleLock(handle);
	
	DmStrCopy(param->ptr,0,recText);

	// return the size of the item we are passing back
	param->size = StrLen(recText)+1;	

	MemHandleUnlock(recHandle);
	DmReleaseRecord ( dbRef, index, false);						


	// Remove our Wait form and restore original form
	if (numRecords > 100)
	{
		FrmSetFocus (frm, noFocus);
		FrmEraseForm (frm);
		FrmDeleteForm (frm);
		if (originalForm)
			FrmSetActiveForm (originalForm);
	
	}
	
	DmCloseDatabase( dbRef );
	return errNone;
	

// If it comes here there has been an error
Exit:

	if(itemNameH)
	{	
		MemHandleUnlock(itemNameH);
		MemHandleFree(itemNameH);
	}
	
	if(mimeTypeH)
	{	
		MemHandleUnlock(mimeTypeH);
		MemHandleFree(mimeTypeH);
	}
	
Done:
	
	DmCloseDatabase( dbRef );
	return appError;
	

}

/*********************************************************************
 * FUNCTION: PrvHandleReceive
 *
 * DESCRIPTION: Handles MMPRO_PLUGIN_RECEIVE_LAUNCHCODE
 * Display the attachment
 * 
 * PARAMETERS:
 *
 * cmdPB
 *   pointer to a structure that is associated MMPRO_PLUGIN_RECEIVE_LAUNCHCODE
 *
 *
 * RETURNED: None
 *    
 */

static Err PrvHandleReceive(void* cmdPBP)
{

	pluginParams *params = (pluginParams*)cmdPBP;
	FormPtr originalForm;
	FormPtr frm;
	UInt16 strlen = 0;
	MemHandle bodyHandle;
	CharPtr bodyPtr=0;
	int fldIndex=0;
	
	if (!params)
	{
		return appError; // Return some non zero error
	}
		
	// hold the original form
	originalForm =  FrmGetActiveForm();
				
	// initialize our form and field info
	frm = FrmInitForm (ViewForm);
	FrmSetActiveForm (frm);
	fldIndex = FrmGetObjectIndex(frm, ViewBodyField);
		
	// The purpose of the following section is to remove \r characters
	// probably because we get \r\n and we only need \n
	// Not that we can't write beyond the end of the params->data size
	// otherwise DmWrite gets a fatal error. 
	if (params->data != NULL)		
	{
		char *pNextStart = params->data;
		char *pWriteHere = params->data;
		int i = 0, nWriteLen = 0;
		
		for (i = 0; i < params->size; i++)
		{
			if (!params->data[i])
				break; // Stop as soon as a null is encountered

			if (params->data[i] == '\r')
			{
				// Don't do this write if we're still at the beginning
				nWriteLen = params->data + i - pNextStart;
				if ((nWriteLen > 0) && (pNextStart != params->data))
					DmWrite(params->data, (pWriteHere - params->data), pNextStart, nWriteLen);
				pWriteHere += nWriteLen;
				pNextStart = params->data + i + 1;
			}
		}
		// Write the end part - 
		nWriteLen = params->data + params->size - pNextStart;
		if (pNextStart != params->data && (nWriteLen > 0))
			DmWrite(params->data, (pWriteHere - params->data), pNextStart, nWriteLen);
		pWriteHere += nWriteLen;
		// See if we can add a terminating null here
		if (pWriteHere - params->data < params->size)
			DmSet(params->data, (pWriteHere - params->data), 1, 0);
	}

	// this step was added so that we can determine whether the string was shortened
	// by the removal of \r
	if (params->data != NULL)	
		strlen = StrLen(params->data);
	if (strlen > params->size)
		strlen = params->size; // Limit this
		
	if( strlen >= 62000 ) // Limit to this, including terminating null
		strlen =  61999;
			
	bodyHandle = DmNewHandle(params->db, strlen + 1) ;
	
	if( !bodyHandle )
		goto Done;
		
	bodyPtr =(char *)MemHandleLock(bodyHandle);
	if (strlen > 0)
		DmWrite(bodyPtr, 0, params->data, strlen);
		
	DmSet(bodyPtr, strlen, 1, 0);
	MemHandleUnlock(bodyHandle);
		
	FldSetTextHandle((FieldType *)FrmGetObjectPtr(frm, fldIndex), bodyHandle); // field owns the handle
	
	ViewUpdateScrollBar(ViewBodyField,ViewScrollbarScrollBar);
	FrmDrawForm (frm);
	FldDrawField((FieldPtr)FrmGetObjectPtr(frm,fldIndex));	
	
	// handle user input
	ViewFormHandleEvent();
	
	// clean things up
	FrmSetFocus (frm, noFocus);
	FldSetTextHandle((FieldType *)FrmGetObjectPtr(frm, fldIndex), 0); // field owns the handle
	FrmEraseForm (frm);
	FrmDeleteForm (frm);
	MemHandleFree(bodyHandle);
	
	
	if (originalForm)
		FrmSetActiveForm (originalForm);

	return errNone;

Done:

	return appError;
}

#if 0
#pragma mark -
#endif

/*********************************************************************
 * FUNCTION: GetObjectPtr
 *
 * DESCRIPTION: Returns a pointer to an object
 *
 * PARAMETERS:
 *
 * objectID
 *   The ID of an object in a form
 *
 *
 * RETURNED: None
 *    
 */

static MemPtr GetObjectPtr (UInt16 objectID)
{

	FormType * form = FrmGetActiveForm();
	return (FrmGetObjectPtr(form, FrmGetObjectIndex(form,objectID)));
	
}

/*********************************************************************
 * FUNCTION: ViewUpdateScrollBar
 *
 * DESCRIPTION: Updates the scroll bar
 *
 * PARAMETERS:
 *
 * fieldID
 *   The ID of the field
 * scrollID
 *   The ID of the scroll bar
 *
 *
 * RETURNED: None
 *    
 */
static void ViewUpdateScrollBar (Word fieldID,Word scrollID)
{
	Word 		 scrollPos;
	Word 		 textHeight;
	Word 		 fieldHeight;
	Short 		 maxValue;
	
	FieldPtr 	 fld=(FieldPtr)GetObjectPtr(fieldID);
	ScrollBarPtr bar=GetObjectPtr(scrollID);

	// Get the values necessary to update the scroll bar.
	FldGetScrollValues (fld, &scrollPos, &textHeight,  &fieldHeight);

	if (textHeight > fieldHeight)
		maxValue = textHeight - fieldHeight;
	else if (scrollPos)
		maxValue = scrollPos;
	else
		maxValue = 0;

	SclSetScrollBar(bar, scrollPos, 0, maxValue, fieldHeight-1);
}


/*********************************************************************
 * FUNCTION: ViewScroll
 *
 * DESCRIPTION: Scrolls frm by a no. of lines
 *
 * PARAMETERS:
 *
 * fieldID
 *   The ID of the field
 * scrollID
 *   The ID of the scroll bar
 * linesToScroll
 *    Number of lines to scroll
 *
 *
 * RETURNED: None
 *    
 */
static void ViewScroll(Word fieldID,Word scrollID,Short linesToScroll)
{
	Word			blankLines;
	Short			min;
	Short			max;
	Short			value;
	Short			pageSize;
	FieldPtr		fld=(FieldPtr)GetObjectPtr(fieldID);
	ScrollBarPtr	bar;

	if (linesToScroll < 0) 
	{
		blankLines = FldGetNumberOfBlankLines (fld);
		FldScrollField (fld, -linesToScroll, winUp);

		// If there were blank lines visible at the end of the field
		// then we need to update the scroll bar.
		if (blankLines)	
		{
			// Update the scroll bar.
			bar = GetObjectPtr(scrollID);
			SclGetScrollBar (bar, &value, &min, &max, &pageSize);
			if (blankLines > -linesToScroll)
				max += linesToScroll;
			else
				max -= blankLines;
			SclSetScrollBar (bar, value, min, max, pageSize);
		}
	}

	else if (linesToScroll > 0)
		FldScrollField (fld, linesToScroll, winDown);
}

/*********************************************************************
 * FUNCTION: ViewPageScroll
 *
 * DESCRIPTION: Scrolls a page
 *
 * PARAMETERS:
 *
 * fieldID
 *   The ID of the field
 * scrollID
 *   The ID of the scroll bar
 * direction
 *    up or down
 *
 *
 * RETURNED: None
 *    
 */
static void ViewPageScroll (Word fieldID,Word scrollID,	WinDirectionType direction)
{
	Short value;
	Short min;
	Short max;
	Short pageSize;
	Word linesToScroll;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = (FieldPtr)GetObjectPtr(fieldID);

	if (FldScrollable (fld, direction))
	{
		linesToScroll = FldGetVisibleLines (fld) - 1;
		FldScrollField (fld, linesToScroll, direction);

		// Update the scroll bar.
		bar = GetObjectPtr(scrollID);
		SclGetScrollBar (bar, &value, &min, &max, &pageSize);

		if (direction == winUp)
			value -= linesToScroll;
		else
			value += linesToScroll;

		SclSetScrollBar (bar, value, min, max, pageSize);
	}
	FldGrabFocus (fld);
	
}

/*********************************************************************
 * FUNCTION: ViewFormHandleEvent
 *
 * DESCRIPTION: Scrolls a page
 *
 * PARAMETERS:
 *
 * fieldID
 *   The ID of the field
 * scrollID
 *   The ID of the scroll bar
 * direction
 *    up or down
 *
 *
 * RETURNED: None
 *    
 */
static Boolean ViewFormHandleEvent(void)
{
	EventType event;
	Boolean handled=false;
	FormPtr frm=0;


	while (true)
	{
		handled = false;
		EvtGetEvent (&event, evtWaitForever);
		
		if (SysHandleEvent (&event))
			continue;
		if (event.eType == ctlSelectEvent)
		{
			if ( event.data.ctlEnter.controlID==ViewDoneButton)
			{
				return true;
			}
			
		}
		else if (event.eType == fldChangedEvent)
		{
			ViewUpdateScrollBar (ViewBodyField,ViewScrollbarScrollBar); 
		}
		else if (event.eType == appStopEvent)
		{
			EvtAddEventToQueue(&event);
			return false;
		}
		else if (event.eType == keyDownEvent)
		{
   			if (event.data.keyDown.chr == pageUpChr)
   			{
				ViewPageScroll (ViewBodyField,ViewScrollbarScrollBar, winUp); 
				handled = true;
			}

			else if (event.data.keyDown.chr == pageDownChr) 
			{
				ViewPageScroll (ViewBodyField,ViewScrollbarScrollBar, winDown); 
				handled = true;
			}
			else 
			{
				// An ordinary ASCII character was entered.  Have the form
				// give the field the character.  Then check to see if the
				// scrolling changed.
				frm = FrmGetActiveForm ();
				FrmHandleEvent (frm, &event);
				ViewUpdateScrollBar (ViewBodyField,ViewScrollbarScrollBar);
				handled=true;
			}
		}
		else if (event.eType == sclRepeatEvent)
		{
			ViewScroll (ViewBodyField,ViewScrollbarScrollBar ,event.data.sclRepeat.newValue - event.data.sclRepeat.value);
		}


		// Check if the form can handle the event
		if (!handled)
			FrmHandleEvent (FrmGetActiveForm(), &event);
				
	}
}

