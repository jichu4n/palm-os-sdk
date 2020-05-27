/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	FotoFinish.h
 *
 * Description:
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/
#include "Global.h"
 
#ifndef FOTOFINISH_H_
#define FOTOFINISH_H_

#define appFileCreator			'FFin'
#define appName					"FotoFinishV2"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

void	DeInitAlbumList();
Err		InitAlbumList();
void	InitAlbumListUI(ListType *lstP);
void	UpdateAlbumListUI(UInt16 popupID);



#endif 
