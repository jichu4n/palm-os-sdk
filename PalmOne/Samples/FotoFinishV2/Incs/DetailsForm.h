/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	DetailsForm.h
 *
 * Description:
 *	Contains all event-handling code associated with DetailsForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/

#ifndef DETAILSFORM_H
#define DETAILSFORM_H

#include "PalmPhoto.h"

Boolean DetailsFormHandleEvent(EventType *eventP);

void ToggleLabelsDisplay(FormType *frmP, Boolean showFlag);

Err	DoSelection(UInt16 albID);
UInt16 GetAlbumID();
void DisplayImageDetails(PalmPhotoHandle selectedPhotoH);
void UpdateArrows();

#endif