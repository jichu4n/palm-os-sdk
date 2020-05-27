/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	MainForm.h
 *
 * Description:
 *	Contains all event-handling code associated with MainForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *          1.1     - library update   (09/23/03)
 *
 ***********************************************************************/

#ifndef MAINFORM_H
#define MAINFORM_H

Boolean MainFormHandleEvent(EventType *eventP);
void CommitToBackBuffer();
void DeInitOpQueue();

#define	NO_SELECTION			0x00
#define	CURVY_LINE_SELECTION	0x01
#define	STRAIGHT_LINE_SELECTION	0x02
#define	TEXT_SELECTION			0x03
#define	TIME_SELECTION			0x04
#define	PAN_SELECTION			0x05	

#endif