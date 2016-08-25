/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		AlarmPrv.h
 *
 * Description:
 *		Private Include file for Alarm Manager
 *
 * History:
 *   	4/11/95  VMK - Created by Vitaly Kruglikov
 *
 *******************************************************************/


#ifdef	NON_PORTABLE
#ifndef __ALARM_PRV_H__
#define __ALARM_PRV_H__



/************************************************************
 * Alarm Manager Constants
 *
 *************************************************************/
#define almMinTableLength	2		// minimum # of entries in alarm table;
											// MUST NOT BE ZERO.
											
#define almMaxTableLength		20	// maximum # of enries in alarm table


/*******************************************************************
 * Alarm Manager alarm table structures
 *
 *******************************************************************/

// An entry in the alarm table
typedef struct AlmEntryType {
	DWord				ref;				// alarm reference value passed by caller;

	ULong				alarmSeconds;	// alarm date/time in seconds since 1/1/1904;
											// a value of 0 indicates unused entry;
	
	LocalID			dbID;				// creator app database ID;
	Word				cardNo : 1;		// creator app card number;
	
	Word				quiet : 1;		// 1 indicates that the caller wants
											// to be quietly notified when the alarm
											// goes off without all the alarm bells
											// and whistles;

	Word				triggered : 1;	// 1 indicates the alarm has been triggered;
											
	Word				notified : 1;	// 1 indicates we notified the caller that
											// the alarm went off;
	} AlmEntryType;

typedef AlmEntryType*		AlmEntryPtr;


// The alarm table
typedef struct AlmTableType {
	Short				numEntries;		// Number of entries in alarm Table;
	
											// List of alarm entries;
											// This is a variable size field which
											// ***MUST BE LAST***
	AlmEntryType	list[almMinTableLength];
	} AlmTableType;

typedef AlmTableType*		AlmTablePtr;
typedef AlmTablePtr*			AlmTableHandle;


/*******************************************************************
 * Alarm Manager Globals
 *
 *******************************************************************/
typedef struct AlmGlobalsType {
	// We're not making the alarm table a part of the
	// Alarm Globals structure because it is allowed
	// to resize, thus posing a problem for
	// AlmAlarmCallback() which needs to access the
	// Alarm Globals at interrupt time.
	AlmTableHandle	tableH;			// alarm table handle;

	ULong				lastSoundSeconds;
											// alarm seconds of the last alarm for
											// which an alarm was sounded;

	Word				displaying : 1;// 1 indicates we're blocked waiting for
											// some app to finish displaying an alarm
											// dialog box; set and cleared by
											// AlmDisplayNextAlarm();
											
	Word				triggered : 1;	// 1 indicates an alarm was triggered; this
											// field is set by the AlmAlarmCallback() and
											// is checked and cleared by AlmDisplayAlarm();

	Byte				disableCount;	// if this value is greater than zero, Alarm
											// notifications are disabled (see AlmEnableNotification);
	} AlmGlobalsType;

typedef AlmGlobalsType*		AlmGlobalsPtr;


#endif  // __ALARM_MGR_H__
#endif  // NON_PORTABLE
