/***********************************************************************
 *
 * Copyright (c) 2001 PalmOne Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	FileBrowserForm.h
 *
 * Description:
 *	CoolPlayer File Browser Form
 *
 *
 ***********************************************************************/

#ifndef FILEBROWSERFORM_H_
#define FILEBROWSERFORM_H_

/***********************************************************************
 *	Defines
 ***********************************************************************/

#define FILE_COLUMN			0
#define MAX_PATH_LENGTH		512
#define MAX_FILENAME_LENGTH	256

//#define UNWANTED_FILES		( vfsFileAttrLink | vfsFileAttrVolumeLabel )
#define UNWANTED_FILES		( vfsFileAttrVolumeLabel )

#define fbIteratorStart              0L
#define fbIteratorStop               0xffffffffL

#define	fbDBName			"FileBrowserDB"
#define fbCreatorID			'cPLA'
#define fbCardNumber		0

/************************************************************
 * Error codes
 *************************************************************/

#define fbErrNoSelection			(appErrorClass | 1)	// No files were selected
#define fbErrNoMoreSelection		(appErrorClass | 2) // No more files in enumeration

/***********************************************************************
 *	Typedef
 ***********************************************************************/

typedef enum
{
	DIRECTORY	= 1 << 0,
	FILE		= 1 << 1
} FileBrowserView;

typedef enum
{
	SINGLE,
	MULTIPLE
} FileBrowserSelection;

typedef void (*FileBrowserCallback)(UInt16 volume, const Char* path, const Char* file);

typedef UInt32 fbIterator;

/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

Boolean	FileBrowserFormHandleEvent(EventType *eventP);
void 	FileBrowserSetCallback(FileBrowserCallback callback, UInt32 formID);
Boolean FileBrowserIsCallbackSet();
Boolean FileBrowserIsReturnFormSet();

void	FileBrowserRefresh();
Err		FileBrowserDeleteDB();
Err		FileBrowserEnumerateSelection(UInt16 *volume, Char* path, Char* file, fbIterator *iterator);

#ifdef __cplusplus 
}
#endif

#endif /* FILEBROWSERFORM_H_ */
