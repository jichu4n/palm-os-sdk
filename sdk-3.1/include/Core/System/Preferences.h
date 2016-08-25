/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
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

#define noPreferenceFound	-1

// Preference version constants
#define preferenceDataVer2			2
#define preferenceDataVer3			3
#define preferenceDataVer4			4
#define preferenceDataVer5			5

// Be sure to update "preferenceDataVerLatest" when adding a new prefs version...
#define preferenceDataVerLatest	preferenceDataVer5


#define defaultAutoOffDuration	2			//	minutes


// Obsolete after V20
#if EMULATION_LEVEL == EMULATION_NONE
	#define defaultSysSoundLevel		slOn
	#define defaultGameSoundLevel		slOn
	#define defaultAlarmSoundLevel	slOn
#else	// EMULATION_LEVEL != EMULATION_NONE
	#define defaultSysSoundLevel		slOff
	#define defaultGameSoundLevel		slOff
	#define defaultAlarmSoundLevel	slOn
#endif

	
#if EMULATION_LEVEL == EMULATION_NONE
	#define defaultSysSoundVolume		sndMaxAmp
	#define defaultGameSoundVolume	sndMaxAmp
	#define defaultAlarmSoundVolume	sndMaxAmp
#else	// EMULATION_LEVEL != EMULATION_NONE
	#define defaultSysSoundVolume		0
	#define defaultGameSoundVolume	0
	#define defaultAlarmSoundVolume	sndMaxAmp
#endif

	
typedef enum
	{
	cAustralia = 0,		// 0
	cAustria,				// 1
	cBelgium,				// 2
	cBrazil,					// 3
	cCanada,					// 4
	cDenmark,				// 5
	cFinland,				// 6
	cFrance,					// 7
	cGermany,				// 8
	cHongKong,				// 9
	cIceland,				// 10
	cIreland,				// 11
	cItaly,					// 12
	cJapan,					// 13
	cLuxembourg,			// 14
	cMexico,					// 15
	cNetherlands,			// 16
	cNewZealand,			// 17
	cNorway,					// 18
	cSpain,					// 19
	cSweden,					// 20
	cSwitzerland,			// 21
	cUnitedKingdom,		// 22
	cUnitedStates			// 23
	
	// Always add new countries at the end!
	
	} CountryType;
	
#define countryFirst		cAustralia
#define countryLast		cUnitedStates
#define countryCount		(countryLast - countryFirst + 1)

		
typedef enum 
	{
	tfColon,
	tfColonAMPM,			// 1:00 pm
	tfColon24h,				// 13:00
	tfDot,
	tfDotAMPM,				// 1.00 pm
	tfDot24h,				// 13.00
	tfHoursAMPM,			// 1 pm
	tfHours24h,				// 13
	tfComma24h				// 13,00
	} TimeFormatType;


typedef enum {
	dsNone,					// Daylight Savings Time not observed
	dsUSA,					// United States Daylight Savings Time
	dsAustralia,			// Australian Daylight Savings Time
	dsWesternEuropean,	// Western European Daylight Savings Time
	dsMiddleEuropean,		// Middle European Daylight Savings Time
	dsEasternEuropean,	// Eastern European Daylight Savings Time
	dsGreatBritain,		// Great Britain and Eire Daylight Savings Time
	dsRumania,				// Rumanian Daylight Savings Time
	dsTurkey,				// Turkish Daylight Savings Time
	dsAustraliaShifted	// Australian Daylight Savings Time with shift in 1986
	} DaylightSavingsTypes;


// pass a TimeFormatType	
#define Use24HourFormat(t) ((t) == tfColon24h || (t) == tfDot24h || (t) == tfHours24h || (t) == tfComma24h)
#define TimeSeparator(t) ( t <= tfColon24h ? ':' : (t <= tfDot24h ? '.' : ','))


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
	dfYMDLongWithSpace,	//	1995 Dec 31

	dfMYMed,					//	Dec '95
	dfMYMedNoPost			//	Dec 95		(added for French 2.0 ROM)
	} DateFormatType;


// The number format (thousands separator and decimal point).  This defines
// how numbers are formatted and not neccessarily currency numbers (i.e. Switzerland).
typedef enum {
	nfCommaPeriod,
	nfPeriodComma,
	nfSpaceComma,
	nfApostrophePeriod,
	nfApostropheComma
	} NumberFormatType;

	
//	These sound levels must corrospond to positions in the popup lists
//	used by the preferences app.  These are made obsolete after V20.  The
// loudness of the sound is now represented as a number from 0 to sndMaxAmp.
typedef enum {
	slOn = 0,
	slOff = 1
	} SoundLevelTypeV20;
	

#define countryNameLength		20
#define currencyNameLength		20
#define currencySymbolLength	6

// An array of these structures (one per country) is kept in the system
// resource.
typedef struct 
	{
	CountryType country;					// Country the structure represents
	Char countryName[countryNameLength];
	DateFormatType dateFormat;			// Format to display date in
	DateFormatType longDateFormat;	// Format to display date in
	Byte weekStartDay;					// Sunday or Monday
	TimeFormatType timeFormat;			// Format to display time in
	NumberFormatType numberFormat;	// Format to display numbers in
	Char currencyName[currencyNameLength];					// Dollars
	Char currencySymbol[currencySymbolLength];			// $
	Char uniqueCurrencySymbol[currencySymbolLength];	// US$
	Byte currencyDecimalPlaces;		// 2 for 1.00
	DaylightSavingsTypes	daylightSavings;	// Type of daylight savings correction
	DWord minutesWestOfGMT;				// minutes west of Greenwich
	} CountryPreferencesType;

// The number format (thousands separator and decimal point).  This defines
// how numbers are formatted and not neccessarily currency numbers (i.e. Switzerland).
typedef enum {
	alOff,									// Never show an animation
	alEventsOnly,							// Show an animation for an event
	alEventsAndRandom,					// Also show random animation
	alEventsAndMoreRandom				// Show random animations more frequently
	} AnimationLevelType;


typedef enum 
	{
	prefVersion,
	prefCountry,
	prefDateFormat,
	prefLongDateFormat,
	prefWeekStartDay,
	prefTimeFormat,
	prefNumberFormat,
	prefAutoOffDuration,
	prefSysSoundLevelV20,				// slOn or slOff - error beeps and other non-alarm/game sounds
	prefGameSoundLevelV20,				// slOn or slOff - game sound effects 
	prefAlarmSoundLevelV20,				// slOn or slOff - alarm sound effects 
	prefHidePrivateRecords,
	prefDeviceLocked,
	prefLocalSyncRequiresPassword,
	prefRemoteSyncRequiresPassword,
	prefSysBatteryKind,
	prefAllowEasterEggs,
	prefMinutesWestOfGMT,
	prefDaylightSavings,
	prefRonamaticChar,
	prefHard1CharAppCreator,			// App creator for hard key #1
	prefHard2CharAppCreator,			// App creator for hard key #2
	prefHard3CharAppCreator,			// App creator for hard key #3
	prefHard4CharAppCreator,			// App creator for hard key #4
	prefCalcCharAppCreator,				// App creator for calculator soft key
	prefHardCradleCharAppCreator,		// App creator for hard cradle key
	prefLauncherAppCreator,				// App creator for launcher soft key
	prefSysPrefFlags,		
	prefHardCradle2CharAppCreator,	// App creator for 2nd hard cradle key
	prefAnimationLevel,

	// Additions for PalmOS 3.0:
	prefSysSoundVolume,					// actual amplitude - error beeps and other non-alarm/game sounds
	prefGameSoundVolume,					// actual amplitude - game sound effects
	prefAlarmSoundVolume,				// actual amplitude - alarm sound effects
	prefBeamReceive,						// False turns off IR sniffing, sends still work.
	prefCalibrateDigitizerAtReset,	// True makes the user calibrate at soft reset time
	prefSystemKeyboardID,				// ID of the preferred keyboard resource
	prefDefSerialPlugIn,					// creator ID of the default serial plug-in

	// Additions for PalmOS 3.1:
	prefStayOnWhenPluggedIn,			// don't sleep after timeout when using line current
	prefStayLitWhenPluggedIn			// keep backlight on when not sleeping on line current
	} SystemPreferencesChoice;
	
	
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
	SoundLevelTypeV20 sysSoundLevel;		//	slOn or slOff - error beeps and other non-alarm sounds
	SoundLevelTypeV20 alarmSoundLevel;	//	slOn or slOff - alarm only
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
	
	} SystemPreferencesTypeV10;



// Any entries added to this structure must be initialized in 
// Prefereces.c:GetPreferenceResource

typedef struct 
	{
	Word version;							// Version of preference info
	
	// International preferences
	CountryType country;					// Country the device is in
	DateFormatType dateFormat;			// Format to display date in
	DateFormatType longDateFormat;	// Format to display date in
	Byte weekStartDay;					// Sunday or Monday
	TimeFormatType timeFormat;			// Format to display time in
	NumberFormatType numberFormat;	// Format to display numbers in
	
	// system preferences
	Byte autoOffDuration;				// Time period in minutes before shutting off
	SoundLevelTypeV20 sysSoundLevelV20;		//	slOn or slOff - error beeps and other non-alarm/game sounds
	SoundLevelTypeV20 gameSoundLevelV20;	//	slOn or slOff - game sound effects 
	SoundLevelTypeV20 alarmSoundLevelV20;	//	slOn or slOff - alarm sound effects
	Boolean hideSecretRecords;			// True to not display records with
												// their secret bit attribute set
	Boolean deviceLocked;				// Device locked until the system
												// password is entered
	Boolean localSyncRequiresPassword;	// User must enter password on Pilot
	Boolean remoteSyncRequiresPassword;	// User must enter password on Pilot
	Word sysPrefFlags;					// Miscellaneous system pref flags
												//  copied into the global GSysPrefFlags
												//  at boot time. Constants are
												//  sysPrefFlagXXX defined in SystemPrv.h
	SysBatteryKind	sysBatteryKind;	// The type of batteries installed. This
												// is copied into the globals GSysbatteryKind
												//  at boot time.
	DWord minutesWestOfGMT;				// minutes west of Greenwich
	DaylightSavingsTypes	daylightSavings;	// Type of daylight savings correction
	UInt ronamaticChar;					// character to generate from ronamatic stroke.
												//  Typically it popups the onscreen keyboard.
	DWord	hard1CharAppCreator;			// creator of application to launch in response
												//  to the hard button #1. Used by SysHandleEvent.
	DWord	hard2CharAppCreator;			// creator of application to launch in response
												//  to the hard button #2. Used by SysHandleEvent.
	DWord	hard3CharAppCreator;			// creator of application to launch in response
												//  to the hard button #3. Used by SysHandleEvent.
	DWord	hard4CharAppCreator;			// creator of application to launch in response
												//  to the hard button #4. Used by SysHandleEvent.
	DWord	calcCharAppCreator;			// creator of application to launch in response
												//  to the Calculator icon. Used by SysHandleEvent.
	DWord	hardCradleCharAppCreator;	// creator of application to launch in response
												//  to the Cradle button. Used by SysHandleEvent.
	DWord	launcherCharAppCreator;		// creator of application to launch in response
												//  to the launcher button. Used by SysHandleEvent.
	DWord	hardCradle2CharAppCreator;	// creator of application to launch in response
												//  to the 2nd Cradle button. Used by SysHandleEvent.
	AnimationLevelType animationLevel;	// amount of animation to display
	
	// Additions for PalmOS 3.0:
	Word sysSoundVolume;					//	system amplitude (0 - sndMaxAmp) - taps, beeps
	Word gameSoundVolume;				//	game amplitude (0 - sndMaxAmp) - explosions
	Word alarmSoundVolume;				//	alarm amplitude (0 - sndMaxAmp)
	Boolean beamReceive;						// False turns off IR sniffing, sends still work.
	Boolean calibrateDigitizerAtReset;	// True makes the user calibrate at soft reset time
	Word systemKeyboardID;					// ID of the preferred keyboard resource
	DWord defSerialPlugIn;					// creator ID of the default serial plug-in

	// Additions for PalmOS 3.1:
	Boolean stayOnWhenPluggedIn;			// don't sleep after timeout when using line current
	Boolean stayLitWhenPluggedIn;			// keep backlight on when not sleeping on line current

	} SystemPreferencesType;


	
typedef SystemPreferencesType * SystemPreferencesPtr;


//-------------------------------------------------------------------
// Preferences routines
//-------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif


extern DmOpenRef PrefOpenPreferenceDBV10 (void)
		SYS_TRAP(sysTrapPrefOpenPreferenceDBV10);

extern DmOpenRef PrefOpenPreferenceDB (Boolean saved)
		SYS_TRAP(sysTrapPrefOpenPreferenceDB);

extern void PrefGetPreferences(SystemPreferencesPtr p)
		SYS_TRAP(sysTrapPrefGetPreferences);

extern void PrefSetPreferences(SystemPreferencesPtr p)
		SYS_TRAP(sysTrapPrefSetPreferences);

extern DWord PrefGetPreference(SystemPreferencesChoice choice)
		SYS_TRAP(sysTrapPrefGetPreference);

extern void PrefSetPreference(SystemPreferencesChoice choice, DWord value)
		SYS_TRAP(sysTrapPrefSetPreference);

extern SWord PrefGetAppPreferences (DWord creator, Word id, VoidPtr prefs, 
	Word *prefsSize, Boolean saved)
		SYS_TRAP(sysTrapPrefGetAppPreferences);

extern Boolean PrefGetAppPreferencesV10 (DWord type, SWord version, VoidPtr prefs,
	Word prefsSize)
		SYS_TRAP(sysTrapPrefGetAppPreferencesV10);

extern void PrefSetAppPreferences (DWord creator, Word id, SWord version, 
	VoidPtr prefs, Word prefsSize, Boolean saved)
		SYS_TRAP(sysTrapPrefSetAppPreferences);

extern void PrefSetAppPreferencesV10 (DWord creator, SWord version, VoidPtr prefs,
	Word prefsSize)
		SYS_TRAP(sysTrapPrefSetAppPreferencesV10);


#ifdef __cplusplus 
}
#endif


#endif	// __PREFERENCES_H__
