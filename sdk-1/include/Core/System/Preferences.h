/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		Preferences.h
 *
 * Description:
 *		Header for the system preferences
 *
 * History:
 *   	2/31/95  rsf - Created by Roger Flores
 *
 *******************************************************************/

#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__


/***********************************************************************
 *	Constants
 ***********************************************************************/

#define preferenceDataVersion	1

#define defaultAutoOffDuration	2			//	minutes

#define prefFileTData			'PREF'		//	Database file type

#if EMULATION_LEVEL == EMULATION_NONE
	#define defaultSysSoundLevel		slOn
	#define defaultAlarmSoundLevel	slOn
#else	// EMULATION_LEVEL != EMULATION_NONE
	#define defaultSysSoundLevel		slOff
	#define defaultAlarmSoundLevel	slOn
#endif

//	This is Marketing's official list of countries.  I didn't make this
//	up - kcr.
typedef enum {
	Australia,
	Austria,
	BelgiumDutch,
	BelgiumFrench,
	Brazil,
	CanadaEnglish,
	CanadaFrench,
	Denmark,
	Finland,
	France,
	Germany,
	Iceland,
	Ireland,
	Italy,
	Japan,
	Mexico,
	Netherlands,
	NewZealand,
	Norway,
	Portugal,
	SouthKorea,
	Spain,
	Sweden,
	SwitzerlandFrench,
	SwitzerlandGerman,
	SwitzerlandItalian,
	Taiwan,
	UnitedStates,
	UnitedKingdom
	} CountryType;
	
typedef enum {
	tfColon,
	tfColonAMPM,
	tfColon24h,
	tfDot,
	tfDotAMPM,
	tfDot24h,
	tfHoursAMPM,
	tfHours24h
	} TimeFormatType;
	
//	These sound levels must corrospond to positions in the popup lists
//	used by the preferences app.
typedef enum {
	slOn = 0,
	slOff = 1
	} SoundLevelType;
	

// pass a TimeFormatType	
#define Use24HourFormat(t) ((t) == tfColon24h || (t) == tfDot24h || (t) == tfHours24h)
#define TimeSeparator(t) ( t >= tfDot ? '.' : ':')


typedef enum {
	dfMDYWithSlashes,		// 12/31/95
	dfDMYWithSlashes,		// 31/12/95
	dfDMYWithDots,			// 31.12.95
	dfDMYWithDashes,		// 31-12-95
	dfYMDWithSlashes,		// 95/12/31
	dfYMDWithDots,			// 95.12.31
	dfYMDWithDashes,		// 95-12-31

	dfMDYLongWithComma,	// Dec 31, 1995
	dfDMYLong,				// 31 Dec 1995
	dfDMYLongWithDot,		// 31. Dec 1995
	dfDMYLongNoDay,		// Dec 1995
	dfDMYLongWithComma,	//	31 Dec, 1995
	dfYMDLongWithDot,		//	1995.12.31

	dfMYMed					//	Dec '95
	} DateFormatType;


// The number format (thousands separator and decimal point).
typedef enum {
	nfCommaPeriod,
	nfPeriodComma,
	nfSpaceComma,
	nfApostrophePeriod
	} NumberFormatType;

	

	
typedef struct {
	Word version;							// Version of preference info
	
	// International preferences
	CountryType country;					// Country the device is in
	DateFormatType dateFormat;			// Format to display date in
	DateFormatType longDateFormat;	// Format to display date in
	Byte weekStartDay;					// Sunday or Monday
	TimeFormatType timeFormat;			// Format to display time in
	NumberFormatType numberFormat;	// Format to display numbers in
	
	// system preferences
	Byte autoOffDuration;				// Time period before shutting off
	SoundLevelType sysSoundLevel;		//	High, Med, Low, Off - error beeps
	SoundLevelType alarmSoundLevel;	//	High, Med, Low, Off - alarm only
	Boolean hideSecretRecords;			// True to not display records with
												// their secret bit attribute set
	Boolean deviceLocked;				// Device locked until the system
												// password is entered
	Word		sysPrefFlags;				// Miscellaneous system pref flags
												//  copied into the global GSysPrefFlags
												//  at boot time.
	SysBatteryKind	sysBatteryKind;	// The type of batteries installed. This
												// is copied into the globals GSysbatteryKind
												//  at boot time.
	
	} SystemPreferencesType;


	
typedef SystemPreferencesType * SystemPreferencesPtr;


extern DmOpenRef PrefOpenPreferenceDB (void)
		SYS_TRAP(sysTrapPrefOpenPreferenceDB);

extern void PrefGetPreferences(SystemPreferencesPtr p)
		SYS_TRAP(sysTrapPrefGetPreferences);

extern void PrefSetPreferences(SystemPreferencesPtr p)
		SYS_TRAP(sysTrapPrefSetPreferences);

extern Boolean PrefGetAppPreferences (ULong type, Int version, VoidPtr prefs,
	Word prefsSize)
		SYS_TRAP(sysTrapPrefGetAppPreferences);

void PrefSetAppPreferences (ULong type, Int version, VoidPtr prefs,
	Word prefsSize)
		SYS_TRAP(sysTrapPrefSetAppPreferences);

#endif	// __PREFERENCES_H__
