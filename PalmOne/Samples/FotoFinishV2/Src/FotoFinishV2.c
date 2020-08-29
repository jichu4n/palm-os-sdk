/***********************************************************************
 *
 * Copyright (c) 2003-2004 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	FotoFinish.c
 *
 * Description:
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision          (18-aug-2003)
 *          1.1     - library update            (23-sep-2003)
 *          2.0     - require Photo Lib v2.0    (20-jan-2004)
 *
 ***********************************************************************/

#include <PalmOS.h>

#include "FotoFinishV2.h"
#include "FotoFinishRsc.h"
#include "utils.h"
#include "MainForm.h"
#include "PalmPhoto.h"
#include "DetailsForm.h"
#include "SavePhotoForm.h"
#include "AddTextForm.h"
//#include "Global.h"

#define		kPhotoLibRequiredVersion		kPalmPhotoLibVersion2

// Globals
UInt16		gLibRefNum = 0;
ListType 	*gLstP = NULL;



// Number of albums on the device
UInt16		gAlbumCount = 0;

// Contains the array of album-names
Char 		**gAlbumList = NULL;

extern UInt32 verPresent;

/***********************************************************************
 *
 * FUNCTION:    	AppHandleEvent
 *
 * DESCRIPTION: 	Handles events as they happen - Registers event
 *					handlers for all the respective forms	
 *
 * PARAMETERS:  	EventType *eventP -> Type of event	
 *
 * RETURNED:    	Boolean handled -> Denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean AppHandleEvent(EventType * eventP)	
{
	UInt16 formId = 0;
	FormType *frmP = NULL;
	Boolean	handled = false;
	
	if (eventP->eType == frmLoadEvent)
	{
		/* Load the form resource. */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				handled = true;
				break;
		}
		
		goto Done;
	}

Done:
	return handled;	
}	

/***********************************************************************
 *
 * FUNCTION:    	AppStart
 *
 * DESCRIPTION: 	Initializes some parameters -> Loads the Photo library,
 *					Sets the screen depth to 16 bit	
 *
 * PARAMETERS:  	originalDepth -> Pointer to the original depth	
 *
 * RETURNED:    	Err value	
 *
 * REVISION HISTORY:
 *   18 aug 2003    v1.0    Original version
 *   20 jan 2004    v2.0    require Photo Lib v2.0.
 *
 *
 ***********************************************************************/

static Err AppStart(UInt32	*originalDepthP)		/* Initialization before the app */
{
	Err err = errNone;
	UInt32 newDepth = 16;
	//UInt16 count = 0;
	
	
	err = SysLibFind(PalmPhotoLibName, &gLibRefNum);

	if( err != errNone )
		err = SysLibLoad(PalmPhotoLibTypeID, PalmPhotoLibCreatorID, &gLibRefNum);

	if( errNone == err )
		err = PalmPhotoLibOpen(gLibRefNum);

	if( err != errNone ) goto Done;

	err = PalmPhotoLibGetVersion( gLibRefNum, kPhotoLibRequiredVersion, &verPresent);
	if( err != errNone ) goto Done;

	
	WinScreenMode(winScreenModeGet, NULL, NULL, originalDepthP, NULL);
	WinScreenMode(winScreenModeSet, NULL, NULL, &newDepth, NULL);

Done:
	if( err != errNone ) FrmAlert( PhotoLibErrorAlert );
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    	AppEventLoop
 *
 * DESCRIPTION: 	The engine of FotoFinish (If you didn't get that,
 *					read the Palm OS Bible)
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void AppEventLoop(void)	/* Motor of the application */
{
	EventType event;
	UInt16 error = 0;

	do 
	{
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event)) {
			if (! MenuHandleEvent(0, &event, &error)) {
				if (! AppHandleEvent(&event)) {
					FrmDispatchEvent(&event);
				}
			}
		}
	} while (event.eType != appStopEvent);
}

/***********************************************************************
 *
 * FUNCTION:    	AppStop
 *
 * DESCRIPTION: 	Restores all parameters -> Sets the screen-depth back
 *					to the original value	
 *
 * PARAMETERS:  	depth -> The original screen-depth	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void AppStop(UInt32 depth)		/* Cleanup after the app */
{
    PalmPhotoLibClose(gLibRefNum);		
	SysLibRemove(gLibRefNum);

	// Close all the open forms.
	FrmCloseAllForms ();

	WinScreenMode(winScreenModeSet, NULL, NULL, &depth, NULL);
}



#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	InitAlbumList
 *
 * DESCRIPTION: 	Initializes gAlbumList with the names of all the 
 *					albums on the device	
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Err value returned by PalmPhotoGetAlbumName	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Err InitAlbumList()
{
	Err err = errNone;
	UInt16	count = 0;
	UInt16	id = 0;
	UInt16	i = 0;
	UInt16 	volumeRef = 0;
		
	if (verPresent >=  kPalmPhotoLibVersion1 && verPresent < kPalmPhotoLibVersion2)
	{
		gAlbumCount = PalmPhotoAlbumCount(gLibRefNum);
	}
	else if (verPresent >=  kPalmPhotoLibVersion2)
	{
		gAlbumCount = PalmPhotoAlbumCountV2(gLibRefNum, volumeRef);
	}
	
   	if(gAlbumCount && !gAlbumList)
		gAlbumList = MemPtrNew(gAlbumCount * sizeof(Char *));
	else
		goto NoAlbums;
		
	for(count = 0; count < gAlbumCount; count++)
	{
		gAlbumList[count] = (Char *)MemPtrNew(PALM_PHOTO_MAX_PATH+1);
		
		
		if (verPresent >=  kPalmPhotoLibVersion1 && verPresent < kPalmPhotoLibVersion2)
		{
			id = PalmPhotoGetAlbumID(gLibRefNum, count);
		}
		else if (verPresent >=  kPalmPhotoLibVersion2)
		{
			id = PalmPhotoGetAlbumIDV2(gLibRefNum, 0, count);
		}
		
		err = PalmPhotoGetAlbumName(gLibRefNum, id, gAlbumList[count], PALM_PHOTO_MAX_PATH+1);		
		
		if(err)
			goto Err;
	}
	
	if(!err)
		goto Done;
	
NoAlbums:
    gAlbumList = MemPtrNew(sizeof(Char *));
	gAlbumList[0] = (Char *)MemPtrNew(sizeof(Char *));
	StrCopy(gAlbumList[0], "ALL");
	gAlbumCount = 1;
	goto Done;

Err:
	for(i = 0; i < count; i++)
		MemPtrFree(gAlbumList[i]);
	MemPtrFree(gAlbumList);

Done:
	return err;

}

/***********************************************************************
 *
 * FUNCTION:    	DeInitAlbumList
 *
 * DESCRIPTION: 	De-allocates memory for the album list	
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void DeInitAlbumList()
{
	UInt16 count = 0;
	if(gAlbumList)
	{
		for(count = 0; count < gAlbumCount; count++)
		{
			if(gAlbumList[count])
			{
				MemPtrFree(gAlbumList[count]);
				gAlbumList[count] = NULL;
			}
		}
		MemPtrFree(gAlbumList);
		gAlbumList = NULL;
	}
	else
		goto Done;
Done:
	return;
}

/***********************************************************************
 *
 * FUNCTION:    	InitAlbumListUI
 *
 * DESCRIPTION: 	Initializes the pop-up list - populates list with 
 *					the array of album names
 *
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void InitAlbumListUI(ListType *lstP)
{
	LstSetHeight(lstP, gAlbumCount);
	LstSetListChoices(lstP, gAlbumList, gAlbumCount);
	LstSetSelection(lstP, 0);
}	

/***********************************************************************
 *
 * FUNCTION:    	UpdateAlbumListUI
 *
 * DESCRIPTION: 	Sets the caption of the popup list to the album name	
 *
 * PARAMETERS:  	popupID -> ID of the popup list on the active form	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void UpdateAlbumListUI(UInt16 popupID)
{
	FormType *frmP = FrmGetActiveForm();
	// Sets the caption of the popup list to the album name
	CtlSetLabel( (ControlType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, popupID)),
												LstGetSelectionText(gLstP,LstGetSelection(gLstP)) );
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	PilotMain
 *
 * DESCRIPTION: 	The Pilot which drives the engine ( If you didn't get 
 *					that too, read the Palm OS Bible)	
 *
 * PARAMETERS:  	cmd -> Command launch flag
 *					cmdPBP -> Pointer to some app-launch parameters	
 *					launchFlags -> Special launch flags
 *
 * RETURNED:    	Err value	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags) /*Entry to app*/
{
	Err error = errNone;
	UInt32	originalDepth = 0;
	
	switch (cmd)
	{
		case sysAppLaunchCmdNormalLaunch:
			
			
			error = AppStart(&originalDepth);
			if (error) 
				return error;
			FrmGotoForm(MainForm);
			AppEventLoop();

			AppStop(originalDepth);
			break;
	}

	return errNone;
}