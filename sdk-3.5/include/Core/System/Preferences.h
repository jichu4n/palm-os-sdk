/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Preferences.h
 *
 * Description:
 *		Header for the system preferences
 *
 * History:
 *   	02/31/95 rsf	Created by Roger Flores
 *		06/26/99	kwk	Added LanguageType.
 *		06/30/99	CS		Added MeasurementSystemType, then added it to both
 *							CountryPreferencesType and SystemPreferencesType,
 *							and bumped the version to 7.
 *					CS		Added prefMeasurementSystem to select this
 *							preference.
 *					CS		Added filler fields to CountryPreferencesType
 *							structure, since this guy gets saved as a
 *							resource.
 *		09/20/99	gap	added additional cXXXX country values.
 *		09/20/99	gap	cPRC -> cRepChina.
 *		10/4/99	jmp	Add support for auto-off duration times in seconds
 *							instead of minutes (the new seconds-based auto-off
 *							duration time is preferred; the minutes-based auto-ff
 *							duration times are maintained for compatibility).
 *		10/5/99	jmp	Make the seconds auto-off duration field a UInt16
 *							instead of a UInt8; also define constants for the
 *							"pegged" auto-off duration values (when the value
 *							is pegged, we no longer automatically shut off).
 *		12/23/99	jmp	Fix <> vs. "" problem.
 *
 *****************************************************************************/

#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <DateTime.h>
#include <Localize.h>
#include <SystemMgr.h>

/***********************************************************************
 *	Constants
 ***********************************************************************/

#define noPreferenceFound			-1

// Preference version constants
#define preferenceDataVer2			2 // Palm OS 2.0
#define preferenceDataVer3			3 // Palm OS 3.0
#define preferenceDataVer4			4 // Palm OS 3.1
#define preferenceDataVer5			5 // Palm OS 3.2a
#define preferenceDataVer6			6 // Palm OS 3.2b/3.3
#define preferenceDataVer8			8 // Palm OS 3.5

// Be SURE to update "preferenceDataVerLatest" when adding a new prefs version...
#define preferenceDataVerLatest	preferenceDataVer8


#define defaultAutoOffDuration		2								//	minutes
#define defaultAutoOffDurationSecs	(2 * minutesInSeconds)	// seconds

#define peggedAutoOffDuration			0xFF							// minutes (UInt8)
#define peggedAutoOffDurationSecs	0xFFFF						// seconds (UInt16)


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
	cUnitedStates,			// 23
	cIndia,					// 24
	cIndonesia,				// 25
	cKorea,					// 26
	cMalaysia,				// 27
	cRepChina,				// 28
	cPhilippines,			// 29
	cSingapore,				// 30
	cThailand,				// 31
	cTaiwan					// 32
	
	// Always add new countries at the end!
	// Always update OverlayMgr.cp!
	
	} CountryType;
	
#define countryFirst		cAustralia
#define countryLast		cTaiwan
#define countryCount		(countryLast - countryFirst + 1)


typedef enum
	{
	languageFirst = 0,
	lEnglish = languageFirst,	// 0
	lFrench,							// 1
	lGerman,							// 2
	lItalian,						// 3
	lSpanish,						// 4
										// 5 (English WorkPad - unused)
	lJapanese = 6,					// 6
	lDutch,							// 7
	
	
	// Always add new languages at the end!
	// Always update OverlayMgr.cp!
	
	languageLast = lDutch
	} LanguageType;
	
#define languageCount		(languageLast - languageFirst + 1)

typedef enum
	{
	unitsEnglish = 0,		// Feet, yards, miles, gallons, pounds, slugs, etc.
	unitsMetric				//	Meters, liters, grams, newtons, etc.
	} MeasurementSystemType;


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
	UInt8 filler1;							// (Word alignment)
	Char countryName[countryNameLength];
	DateFormatType dateFormat;			// Format to display date in
	DateFormatType longDateFormat;	// Format to display date in
	Int8 weekStartDay;					// Sunday or Monday
	TimeFormatType timeFormat;			// Format to display time in
	NumberFormatType numberFormat;	// Format to display numbers in
	UInt8 filler2;							// (Word alignment)
	Char currencyName[currencyNameLength];					// Dollars
	Char currencySymbol[currencySymbolLength];			// $
	Char uniqueCurrencySymbol[currencySymbolLength];	// US$
	UInt8 currencyDecimalPlaces;		// 2 for 1.00
	DaylightSavingsTypes	daylightSavings;	// Type of daylight savings correction
	UInt32 minutesWestOfGMT;				// minutes west of Greenwich
	MeasurementSystemType measurementSystem; // metric, english, etc.
	UInt8 filler3;							// (Word alignment)
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
	prefAutoOffDuration,					// prefAutoOffDurationSecs is now preferred (prefAutoOffDuration is in minutes)
	prefSysSoundLevelV20,				// slOn or slOff - error beeps and other non-alarm/game sounds
	prefGameSoundLevelV20,				// slOn or slOff - game sound effects 
	prefAlarmSoundLevelV20,				// slOn or slOff - alarm sound effects 
	prefHidePrivateRecordsV33,
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
	prefStayLitWhenPluggedIn,			// keep backlight on when not sleeping on line current

	// Additions for PalmOS 3.2:
	prefAntennaCharAppCreator,			// App creator for antenna key

	// Additions for PalmOS 3.3:
	prefMeasurementSystem,				// English, Metric, etc.
	
	// Additions for PalmOS 3.5:
	prefShowPrivateRecords,				// returns privateRecordViewEnum
	prefAutoOffDurationSecs				// auto-off duration in seconds

	} SystemPreferencesChoice;
	
	
typedef struct {
	UInt16 version;						// Version of preference info
	
	// International preferences
	CountryType country;					// Country the device is in
	DateFormatType dateFormat;			// Format to display date in
	DateFormatType longDateFormat;	// Format to display date in
	UInt8 weekStartDay;					// Sunday or Monday
	TimeFormatType timeFormat;			// Format to display time in
	NumberFormatType numberFormat;	// Format to display numbers in
	
	// system preferences
	UInt8 autoOffDuration;				// Time period before shutting off (in minutes)
	SoundLevelTypeV20 sysSoundLevel;		//	slOn or slOff - error beeps and other non-alarm sounds
	SoundLevelTypeV20 alarmSoundLevel;	//	slOn or slOff - alarm only
	Boolean hideSecretRecords;			// True to not display records with
												// their secret bit attribute set
	Boolean deviceLocked;				// Device locked until the system
												// password is entered
	UInt8 reserved1;
	UInt16 sysPrefFlags;					// Miscellaneous system pref flags
												//  copied into the global GSysPrefFlags
												//  at boot time.
	SysBatteryKind	sysBatteryKind;	// The type of batteries installed. This
												// is copied into the globals GSysbatteryKind
												//  at boot time.
	UInt8 reserved2;
	
	} SystemPreferencesTypeV10;


// Any entries added to this structure must be initialized in 
// Prefereces.c:GetPreferenceResource

typedef struct 
	{
	UInt16 version;						// Version of preference info
	
	// International preferences
	CountryType country;					// Country the device is in
	DateFormatType dateFormat;			// Format to display date in
	DateFormatType longDateFormat;	// Format to display date in
	Int8 weekStartDay;					// Sunday or Monday
	TimeFormatType timeFormat;			// Format to display time in
	NumberFormatType numberFormat;	// Format to display numbers in
	
	// system preferences
	UInt8 autoOffDuration;				// Time period in minutes before shutting off (use autoOffDurationSecs instead).
	SoundLevelTypeV20 sysSoundLevelV20;		//	slOn or slOff - error beeps and other non-alarm/game sounds
	SoundLevelTypeV20 gameSoundLevelV20;	//	slOn or slOff - game sound effects 
	SoundLevelTypeV20 alarmSoundLevelV20;	//	slOn or slOff - alarm sound effects
	Boolean hideSecretRecords;			// True to not display records with
												// their secret bit attribute set
	Boolean deviceLocked;				// Device locked until the system
												// password is entered
	Boolean localSyncRequiresPassword;	// User must enter password on Pilot
	Boolean remoteSyncRequiresPassword;	// User must enter password on Pilot
	UInt16 sysPrefFlags;					// Miscellaneous system pref flags
												//  copied into the global GSysPrefFlags
												//  at boot time. Constants are
												//  sysPrefFlagXXX defined in SystemPrv.h
	SysBatteryKind	sysBatteryKind;	// The type of batteries installed. This
												// is copied into the globals GSysbatteryKind
												//  at boot time.
	UInt8 reserved1;
	UInt32 minutesWestOfGMT;			// minutes west of Greenwich
	DaylightSavingsTypes	daylightSavings;	// Type of daylight savings correction
	UInt8 reserved2;
	UInt16 ronamaticChar;				// character to generate from ronamatic stroke.
												//  Typically it popups the onscreen keyboard.
	UInt32	hard1CharAppCreator;		// creator of application to launch in response
												//  to the hard button #1. Used by SysHandleEvent.
	UInt32	hard2CharAppCreator;		// creator of application to launch in response
												//  to the hard button #2. Used by SysHandleEvent.
	UInt32	hard3CharAppCreator;		// creator of application to launch in response
												//  to the hard button #3. Used by SysHandleEvent.
	UInt32	hard4CharAppCreator;		// creator of application to launch in response
												//  to the hard button #4. Used by SysHandleEvent.
	UInt32	calcCharAppCreator;		// creator of application to launch in response
												//  to the Calculator icon. Used by SysHandleEvent.
	UInt32	hardCradleCharAppCreator;	// creator of application to launch in response
												//  to the Cradle button. Used by SysHandleEvent.
	UInt32	launcherCharAppCreator;		// creator of application to launch in response
												//  to the launcher button. Used by SysHandleEvent.
	UInt32	hardCradle2CharAppCreator;	// creator of application to launch in response
												//  to the 2nd Cradle button. Used by SysHandleEvent.
	AnimationLevelType animationLevel;	// amount of animation to display
	
	Boolean	maskPrivateRecords;			// Only meaningful if hideSecretRecords is true.
													//true to show a grey placeholder box for secret records.
													//was reserved3 - added for 3.5
													
	
	// Additions for PalmOS 3.0:
	UInt16 sysSoundVolume;					//	system amplitude (0 - sndMaxAmp) - taps, beeps
	UInt16 gameSoundVolume;					//	game amplitude (0 - sndMaxAmp) - explosions
	UInt16 alarmSoundVolume;				//	alarm amplitude (0 - sndMaxAmp)
	Boolean beamReceive;						// False turns off IR sniffing, sends still work.
	Boolean calibrateDigitizerAtReset;	// True makes the user calibrate at soft reset time
	UInt16 systemKeyboardID;				// ID of the preferred keyboard resource
	UInt32 defSerialPlugIn;					// creator ID of the default serial plug-in

	// Additions for PalmOS 3.1:
	Boolean stayOnWhenPluggedIn;			// don't sleep after timeout when using line current
	Boolean stayLitWhenPluggedIn;			// keep backlight on when not sleeping on line current

	// Additions for PalmOS 3.2:
	UInt32	antennaCharAppCreator;		// creator of application to launch in response
													//  to the antenna key. Used by SysHandleEvent.

	// Additions for PalmOS 3.5:
	MeasurementSystemType measurementSystem;	// metric, english, etc.
	UInt8						reserved3;				
	UInt16 autoOffDurationSecs;					// Time period in seconds before shutting off.
	
	} SystemPreferencesType;


	
typedef SystemPreferencesType *SystemPreferencesPtr;


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

extern UInt32 PrefGetPreference(SystemPreferencesChoice choice)
		SYS_TRAP(sysTrapPrefGetPreference);

extern void PrefSetPreference(SystemPreferencesChoice choice, UInt32 value)
		SYS_TRAP(sysTrapPrefSetPreference);

extern Int16 PrefGetAppPreferences (UInt32 creator, UInt16 id, void *prefs, 
	UInt16 *prefsSize, Boolean saved)
		SYS_TRAP(sysTrapPrefGetAppPreferences);

extern Boolean PrefGetAppPreferencesV10 (UInt32 type, Int16 version, void *prefs,
	UInt16 prefsSize)
		SYS_TRAP(sysTrapPrefGetAppPreferencesV10);

extern void PrefSetAppPreferences (UInt32 creator, UInt16 id, Int16 version, 
	void *prefs, UInt16 prefsSize, Boolean saved)
		SYS_TRAP(sysTrapPrefSetAppPreferences);

extern void PrefSetAppPreferencesV10 (UInt32 creator, Int16 version, void *prefs,
	UInt16 prefsSize)
		SYS_TRAP(sysTrapPrefSetAppPreferencesV10);


#ifdef __cplusplus 
}
#endif


#endif	// __PREFERENCES_H__

