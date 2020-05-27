/******************************************************************************
 *
 * Copyright (c) 2007 Palm, Inc.
 * All rights reserved.
 *
 * File: MemoryInfo.c
 *
 * Description:
 *
 *****************************************************************************/

#include <PalmOS.h>
#include <VFSMgr.h>
#include <PmPalmOSNVFS.h>
#include "MemoryInfoRsc.h"

#define DYNAMIC_HEAP_ID					0
#define STORAGE_HEAP_ID					1

/***********************************************************************
 *
 * FUNCTION:    		HandleMenu
 *
 * DESCRIPTION: 		Deals with menu commands issued
 *
 * PARAMETERS:  		itemID -> ID of the Menu which has been selected
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void HandleMenu(UInt16 itemID)
{
	FormType *frmAboutP = NULL;
    const RGBColorType black = { 0, 0, 0 ,0 };	
    const RGBColorType white = { 0, 255, 255 ,255 };	
    const RectangleType	currentColorRect = { {27, 142}, { 19, 17} };
	
	switch(itemID)
	{		
		case HelpAboutMemoryInfo:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
			
			WinSetForeColorRGB(&white, NULL);
			WinSetTextColorRGB(&black, NULL);
			WinDrawRectangle(&currentColorRect, 0);
			
			break;
			
	}
}


static Err GetNvfsVolRef( UInt16	*volRefNumP )
{
	UInt32 			volIterator = (vfsIteratorStart | 0x80000000);
	VolumeInfoType	volInfo;
	
	
	while ( VFSVolumeEnumerate( volRefNumP, &volIterator ) == errNone )
	{
		VFSVolumeInfo( *volRefNumP, &volInfo );
		if ( ( volInfo.attributes & vfsVolumeAttrHidden ) &&
		     ( volInfo.attributes & vfsVolumeAttrNonRemovable ) )
		{
			return ( errNone );
		}
	}
	
	return ( vfsErrNoFileSystem );
}


static void FormatSize( UInt32	bytes, Char  *sizeP )
{

	if ( sizeP == NULL )
	{
		return;
	}	

	if ( bytes > 1024 )
	{
		bytes /= 1024;
		StrIToA( sizeP, (Int32)bytes );
		StrCat( sizeP, "KB" );
	}
	else
	{
		StrIToA( sizeP, (Int32)bytes );
		StrCat( sizeP, "B" );
	}
	
	
	return;
}


static Boolean MainFormHandleEvent( EventType	*eventP )
{
	Boolean			 handled				= false;
	FormType		*formP 					= FrmGetActiveForm( );
	UInt32 			 totalSize				= 0;
	UInt32			 freeBytes				= 0;
	UInt32			 usedBytes				= 0;
	UInt32			 maxFreeChunkBytes		= 0;
	Char			 sizeTotalDheap			[ maxStrIToALen + 10 ];
	Char			 sizeFreeDheap			[ maxStrIToALen + 10 ];
	Char			 sizeLargestDheap		[ maxStrIToALen + 10 ];
	Char			 sizeTotalDbCache		[ maxStrIToALen + 10 ];
	Char			 sizeFreeDbCache		[ maxStrIToALen + 10 ];
	Char			 sizeLargestDbCache		[ maxStrIToALen + 10 ];
	Char			 sizeTotalFlashStorage	[ maxStrIToALen + 10 ];
	Char			 sizeFreeFlashStorage	[ maxStrIToALen + 10 ];
	UInt16			 volRefNum				= 0;

	
	if ( !eventP )
	{
		return ( handled );
	}

	switch ( eventP->eType )
	{
		case frmOpenEvent:
			// Calculate the Total Size of the Dynamic Heap
			totalSize = MemHeapSize( DYNAMIC_HEAP_ID );
			FormatSize( totalSize, sizeTotalDheap );
			FldSetTextPtr( FrmGetObjectPtr( formP,
											FrmGetObjectIndex( formP, MainFormTotalDheapField ) ),
						   sizeTotalDheap );

			// Calculate the free bytes in the Dynamic Heap and the Max Free Chunk
			MemHeapFreeBytes( DYNAMIC_HEAP_ID, &freeBytes, &maxFreeChunkBytes );
			FormatSize( freeBytes, sizeFreeDheap );
			FldSetTextPtr( FrmGetObjectPtr( formP,
											FrmGetObjectIndex( formP, MainFormFreeDheapField ) ),
						   sizeFreeDheap );
			
			FormatSize( maxFreeChunkBytes, sizeLargestDheap );
			FldSetTextPtr( FrmGetObjectPtr( formP,
											FrmGetObjectIndex( formP, MainFormLargestDheapField ) ),
						   sizeLargestDheap );

			// Calculate the Total DB Cache size
			totalSize = MemHeapSize( STORAGE_HEAP_ID | 0x8000 );
			FormatSize( totalSize, sizeTotalDbCache );
			FldSetTextPtr( FrmGetObjectPtr( formP,
											FrmGetObjectIndex( formP, MainFormTotalDbCacheField ) ),
						   sizeTotalDbCache );
			
			// Calculate the free bytes in the DB Cache and the max free chunk
			MemHeapFreeBytes( STORAGE_HEAP_ID | 0x8000, &freeBytes, &maxFreeChunkBytes );
			FormatSize( freeBytes, sizeFreeDbCache );
			FldSetTextPtr( FrmGetObjectPtr( formP,
											FrmGetObjectIndex( formP, MainFormFreeDbCacheField ) ),
						   sizeFreeDbCache );
			FormatSize( maxFreeChunkBytes, sizeLargestDbCache );
			FldSetTextPtr( FrmGetObjectPtr( formP,
											FrmGetObjectIndex( formP, MainFormLargestDbCacheField ) ),
						   sizeLargestDbCache );

			// Calculate the Total Size of the User Store on the Flash and free bytes in the User Store
			if ( GetNvfsVolRef( &volRefNum ) == errNone )
			{
				if ( VFSVolumeSize( volRefNum, &usedBytes, &totalSize ) == errNone )
				{
					FormatSize( totalSize, sizeTotalFlashStorage );
					FldSetTextPtr( FrmGetObjectPtr( formP,
													FrmGetObjectIndex( formP, MainFormTotalFlashStorageField ) ),
								   sizeTotalFlashStorage );
					FormatSize( totalSize - usedBytes, sizeFreeFlashStorage );
					FldSetTextPtr( FrmGetObjectPtr( formP,
													FrmGetObjectIndex( formP, MainFormFreeFlashStorageField ) ),
								   sizeFreeFlashStorage );
				}
			}
				
			FrmDrawForm( FrmGetActiveForm( ) );
			handled = true;
			break;

		case menuEvent:
			HandleMenu(eventP->data.menu.itemID);
			FrmDrawForm(formP);
			handled = true;
			break;


		default:
			break;
	}

	return ( handled );
}

static Boolean AppHandleEvent( EventType *eventP )
{
	Boolean		 handled	= false;
	FormType	*formP 		= (FormType *)NULL;

	
	if ( !eventP )
	{
		return ( handled );
	}

	switch ( eventP->eType )
	{
		case frmLoadEvent:
			formP 		= FrmInitForm( eventP->data.frmLoad.formID );
			
			
			if ( !formP )
			{
				return ( handled );
			}

			FrmSetActiveForm( formP );

			switch ( eventP->data.frmLoad.formID )
			{
				case MainForm:
					FrmSetEventHandler( formP, MainFormHandleEvent );
					break;

				default:
					break;
			}

			handled = true;
			break;

		default:
			break;
	}

	return ( handled );
}


static void AppEventLoop( void )
{
	EventType	event;
	Err 		err		= errNone;

	do
	{
		EvtGetEvent( &event, evtWaitForever );

		if ( !SysHandleEvent( &event ) )
		{
			if ( !MenuHandleEvent( 0, &event, &err ) )
			{
				if ( !AppHandleEvent( &event ) )
				{
					FrmDispatchEvent( &event );
				}
			}
		}
	}
	while ( event.eType != appStopEvent );
}

static Err AppStart( void )
{
	return ( errNone );
}

static void AppStop( void )
{
	FrmCloseAllForms( );
}


/************************************************************
 *
 *	FUNCTION:		PilotMain
 *
 *	DESCRIPTION:	Real entry point for MemoryInfo.
 *
 *************************************************************/

UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags )
{
	Err		err = errNone;

	
	cmdPBP = cmdPBP;
	launchFlags = launchFlags;
	
	switch ( cmd )
	{
		case sysAppLaunchCmdNormalLaunch:
			err = AppStart( );
			if ( err != errNone )
			{
				return err;
			}

			FrmGotoForm( MainForm );
			AppEventLoop( );

			AppStop( );
			break;

		default:
			break;
	}

	return ( err );
}
