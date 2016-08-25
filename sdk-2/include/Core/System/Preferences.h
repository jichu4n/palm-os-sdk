/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1995-1997, Palm Computing Inc., All Rights Reserved
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

#define noPreferenceFound	-1

#define preferenceDataVersion	2

#define defaultAutoOffDuration	2			//	minutes


#if EMULATION_LEVEL == EMULATION_NONE
	#define defaultSysSoundLevel		slOn
	#define defaultGameSoundLevel		slOn
	#define defaultAlarmSoundLevel	slOn
#else	// EMULATION_LEVEL != EMULATION_NONE
	#define defaultSysSoundLevel		slOff
	#define defaultGameSoundLevel		slOff
	#define defaultAlarmSoundLevel	slOn
#endif

	
typedef enum
	{
	cAustralia,
	cAustria,
	cBelgium,
	cBrazil,
	cCanada,
	cDenmark,
	cFinland,
	cFrance,
	cGermany,
	cHongKong,
	cIceland,
	cIreland,
	cItaly,
	cJapan,
	cLuxembourg,
	cMexico,
	cNetherlands,
	cNewZealand,
	cNorway,
	cSpain,
	cSweden,
	cSwitzerland,
	cUnitedKingdom,
	cUnitedStates
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

	dfMYMed					//	Dec '95
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
//	used by the preferences app.
typedef enum {
	slOn = 0,
	slOff = 1
	} SoundLevelType;
	

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
	prefSysSoundLevel,
	prefGameSoundLevel,
	prefAlarmSoundLevel,
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
	prefAnimationLevel

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
	SoundLevelType sysSoundLevel;		//	High, Med, Low, Off - error beeps
	SoundLevelType gameSoundLevel;	//	game sound effects 
	SoundLevelType alarmSoundLevel;	//	High, Med, Low, Off - alarm only
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
	} SystemPreferencesType;


	
typedef SystemPreferencesType * SystemPreferencesPtr;


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

#endif	// __PREFERENCES_H__
