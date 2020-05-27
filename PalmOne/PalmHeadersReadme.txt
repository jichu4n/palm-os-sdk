====================================
Palm Developer Technical Services
http://developer.palm.com
====================================

------------------------------------------
Palm SDK 5.1: Header Files
------------------------------------------

October 12, 2005

1. Added new device IDs definition for new devices in palmOneResources.h:

#define kPalmDeviceIDTX				'D050'	/**< Device ID for Palm T|X */
#define kPalmDeviceIDZ22			'D051'	/**< Device ID for Palm Z22 */

April 18, 2005

1. Added new device ID definition for LifeDrive device in palmOneResources.h:
#define kPalmOneDeviceIDTungstenX	'TunX'	/**< Device ID for Tungsten|X. */

2. Added  hsAttrAllKeysWakeFromSleep to hsAttrEnum in HsExtCommon.h

3. Added system notifications for various multi-connector related events in PmConnectorLibCommon.h
#define kPmConnectorNotifySysAttach         sysExternalConnectorAttachEvent
#define kPmConnectorNotifySysDetach         sysExternalConnectorDetachEvent

// Bits used in notifyDetailsP for kPmConnectorNotifySysAttach and kPmConnectorNotifySysDetach
#define kPmDockStatusCharging           0x08
#define kPmDockStatusUSBCradle          0x10
#define kPmDockStatusSerialPeripheral   0x40

4. Added FileBrowserLibShowMultiselectDialog() to File Browser Library in FileBrowserLib68K.h


April 13, 2005

Added new device ID definition for Tungsten E2 device in palmOneResources.h


April 4, 2005

diff -r (Sprint + Cingular)/incs/68k/libraries/carriercustomlib/CarrierCustomLibTypes.h
        (Sprint + Cingular + TIM + ROWdy)/incs/68k/libraries/carriercustomlib/CarrierCustomLibTypes.h
1. Added new enum constant for Email settings
kCCSMSettingsEmailSMTPServer,				/**< <char *> SMTP Server */

2. Added new enum constant for phone setting
kCCSMSettingsSPNSupported,				/**< <Boolean> flag if SPN is supported */

Only in (Sprint + Cingular + TIM + ROWdy)/incs/68k/libraries/pmconnectorlib: PmConnectorLib.h
1. Added a new library for the Multi-connector.

diff -r (Sprint + Cingular)/incs/68k/libraries/sndfilestream/palmOneSndFileStream.h
        (Sprint + Cingular + TIM + ROWdy)/incs/68k/libraries/sndfilestream/palmOneSndFileStream.h
1. Added a new Notification to be broadcast by SndFileStream when it has completed playing a stream.
This is an alternative to using callbacks in the case where SndFileStream is used
by an ARM app (eg. Blazer) or by a 68K app.

#define sndFileStreamNotifyStreamComplete 	'StrC'

diff -r (Sprint + Cingular)/incs/68k/libraries/telephony/HsPhoneGSM.h
        (Sprint + Cingular + TIM + ROWdy)/incs/68k/libraries/telephony/HsPhoneGSM.h

1. Sprint and Cingular use the following prototype:
extern Err PhnLibSendLongDTMF(UInt refNum, Char digit, Boolean on)
 			  PHN_LIB_TRAP(PhnLibTrapSendLongDTMF);


TIM and ROWdy use the following prototype:
extern Err PhnLibSendLongDTMF(UInt refNum, Char digit, PhnAudioSendMode sendMode,
     Boolean on)
 		  PHN_LIB_TRAP(PhnLibTrapSendLongDTMF);


You can define a new prototype for the Sprint and Cingular
extern Err PhnLibSendLongDTMFOld(UInt refNum, Char digit, Boolean on)
             PHN_LIB_TRAP(PhnLibTrapSendLongDTMF);

In the application source code:
if (device is Treo650 and carrier is Sprint/Cingular)
	err = PhnLibSendLongDTMFOld(phnLibRef, '8', true);
else if (device is Treo650 and carrier is TIM/ROWdy)
	err = PhnLibSendLongDTMF(phnLibRef, '8', phnAudioSilentMode, true);

2. Added a function to get the Service Provider Name from SIM
extern Err PhnLibGetSpn (UInt refNum, CharPtr* spn)
			  PHN_LIB_TRAP(PhnLibTrapGetSpn);

Only in (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries: PmConnectorLib

diff -r (Sprint + Cingular)/incs/common/libraries/pmsysgadgetlib/PmSysGadgetLibCommon.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/pmsysgadgetlib/PmSysGadgetLibCommon.h
1. Added a new enum constant related to the Bluetooth radio status
kIndicatorBtDUN			   // Connected to DUN

diff -r (Sprint + Cingular)/incs/common/libraries/smarttextengine/SmartTextEngineDef.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/smarttextengine/SmartTextEngineDef.h
1. Added a new initializer for the Smart Text Engine
#define steAllowPhoneNumberWords	0x00010000
/**<If true, then the phone number parser will allow numbers like 1-800-TESTING*/

diff -r (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneAudioTypes.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/telephony/HsPhoneAudioTypes.h
1. Added a new enum for Audio Send Modes
typedef enum
{
	phnAudioLocalMode,		/**<		*/
	phnAudioSilentMode,		/**<		*/
	phnAudioNetworkMode		/**<		*/
}_PhnAudioSendMode;

typedef UInt8 PhnAudioSendMode;		/**<		*/

diff -r (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneCDMATypes.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/telephony/HsPhoneCDMATypes.h

1 Added new Fields that are successfully updated with OTASP
#define phnOtaspMobdirCommitFlag    0x0001		/**<		*/
#define phnOtaspFTCCommitFlag       0x0002		/**<		*/
#define phnOtaspRoamListCommitFlag  0x0004		/**<		*/
#define phnOtaspSPCCommitFlag       0x0008		/**<		*/
#define phnOtaspIMSICommitFlag      0x0010		/**<		*/
#define phnOtaspNamLockCommitFlag   0x0020		/**<		*/
#define phnOtaspAkeyCommitFlag      0x0040		/**<		*/
#define phnOtasp3GPDCommitFlag      0x0080		/**<		*/
#define phnOtaspMIPStatCommitFlag   0x0100		/**<		*/
#define phnOtaspMIPRetryCommitFlag  0x0200		/**<		*/
#define phnOtaspSPCFail             0x0400		/**<		*/
#define phnOtaspCommitFlag          0x0800  	/**<		*/
#define phnOtaspInitProgReqFlag     0x1000		/**<		*/
#define phnOtaspProgInProgressFlag  0x2000		/**<		*/
#define phnOtaspSSDCommitFlag       0x4000		/**<		*/

#define phnOtaspSuccessful          0x02FF		/**<		*/
#define phnOtaspUnsuccessful        0x03FF		/**<		*/

2. Added a new enum for the Phone System Preference Settings
typedef enum
{
	phnSysPrefHomeOnly,		/**<		*/
	phnSysPrefAOnly,		/**<		*/
	phnSysPrefBOnly,		/**<		*/
	phnSysPrefStandard		/**<		*/
} _PhnSysPrefSetting;

typedef UInt8 PhnSysPrefSetting;		/**<		*/

diff -r (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneErrors.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/telephony/HsPhoneErrors.h
1. Added a new enum constant related to Phone Error Codes
	phnErrCauseCode16,		/**< 0x732D GSM04.08: Cause code 16 */
diff -r (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneEvent.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/telephony/HsPhoneEvent.h
1. Added two new unique identifiers for Indication notification.   These values will be
contained in PhnEventType notification events of phnEvtIndication type.

indicationInitComplete,		/**< sent when the GSM library is done with it initialization when the radio is powered on */indicationOtaspMsg 			/**< sent otasp msg from network */
2. Added the following structure to the PhnIndicationType union
struct
{
	UInt32 indicationCode;	/**<		*/
} otaspMsg;

diff -r (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneMiscTypes.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/telephony/HsPhoneMiscTypes.h
1. Added a new enum
typedef enum
{
	phnOprtModeNone =-1, /**< FOR INTERNAL USE OF CM ONLY! */
    phnOprtModePwroff,   /**< phone is powering off */
    phnOprtModeOffline,  /**< phone is offline Digital*/
    phnOprtModeOfflineA, /**< phone is offline analog */
    phnOprtModeOnline,   /**< phone is online */
    phnOprtModeLPM,      /**< phone is in LPM - Low Power Mode */
    phnOprtModeReset,    /**< phone is resetting - i.e. power-cycling */

    phnOprtModeMax

} _PhnOprtModeType;

typedef UInt8 PhnOprtModeType;

diff -r (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneTraps.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/libraries/telephony/HsPhoneTraps.h
1. Added a new trap
#define     PhnLibTrapGetSpn                       (156+sysLibTrapCustom)  /**< GSM Only */

diff -r (Sprint + Cingular)/incs/common/system/HsAppLaunchCmd.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/system/HsAppLaunchCmd.h
1. Added a new launch command to Launch the browser with a specific network service ID
#define   sysAppLaunchWebBrowserURLWithSvcID             (sysAppLaunchCmdCustomBase + 6)

2. Added a new structure that contains data used when launching web browser with a specific SvcID.
typedef struct {
	UInt32                   svcID;				/**< Network SvcID from NetPref */
	Char*                    launchUrl;			/**< URL to browse when launching */
} webBrowserURLWithSvcIDLaunchInfo;


diff -r (Sprint + Cingular)/incs/common/system/HsCreators.h
        (Sprint + Cingular + TIM + ROWdy)/incs/common/system/HsCreators.h
1. Added a new notification sent by Trace Viewer app (or another client that
changes trace settings) to let interested parties know
that trace settings *may* have changed.  No parameter
is presently defined, so notification detailsP will
be NULL for now, but may be non-NULL in the future.

#define pmNotifyTraceSettingsChangedEvent (hsFileCTraceViewer)

---------------------------------------------------------------------------------------------------

February 1, 2005

1. diff -r (Sprint)/incs/68k/libraries/carriercustomlib/CarrierCustomLibTypes.h
        (Sprint + Cingular)/incs/68k/libraries/carriercustomlib/CarrierCustomLibTypes.h
Updated CCSMVersionMinor from 5	to 6
Added new CCSM Settings constants for
1. MMS UA Prof (kCCSMSettingsMMSUAProf)
2. MMS UA Prof String (kCCSMSettingsMMSUAString)
3. MMS Maximum video size (kCCSMSettingsMMSMaxVideoSize)
4. MMS Version (kCCSMSettingsMMSVersionNumber)

2. diff -r (Sprint)/incs/68k/libraries/imaging/ImageLib.h
        (Sprint + Cingular)/incs/68k/libraries/imaging/ImageLib.h
Removed a duplicate prototype for ImageLibIterResize.
Err ImageLibIterResize (UInt16 libRef, ImageIteratorHandle iter, UInt16 index, Coord *widthP,
                Coord *heightP, Boolean maintainAspectRatio) SYS_TRAP(kImageLibTrapIterResize );

3. diff -r (Sprint)/incs/68k/libraries/network/PalmWiFiCommon.h
        (Sprint + Cingular)/incs/68k/libraries/network/PalmWiFiCommon.h
Added the following constants to the Supported Query requests for WiFi driver:
1. netQuery80211SecurityMode
2. netQuery80211StartConnection

Added the following enum for the Security Modes
enum SecurityModeType
{
	SecurityModeOpenSystem = 0,
	SecurityModeWEP,
	SecurityModeWPAPSK,
	SecurityModeLast
};

4. Only in (Sprint + Cingular)/incs/68k/libraries: pmtracelib
Added a PmTraceLib

5. diff -r (Sprint)/incs/68k/libraries/sndfilestream/palmOneSndFileStream.h
        (Sprint + Cingular)/incs/68k/libraries/sndfilestream/palmOneSndFileStream.h
Added the MP3 format (kSndFileFormatMP3) to the sound file streams enum

Added MP3 (kSndFileTypeMP3) to the file stream database type defines

6. diff -r (Sprint)/incs/68k/libraries/telephony/HsPhoneGSM.h
        (Sprint + Cingular)/incs/68k/libraries/telephony/HsPhoneGSM.h
Added PhnLibGetICCID function
  extern Err PhnLibGetICCID (UInt refNum, CharPtr* iccid)
  			  PHN_LIB_TRAP(PhnLibTrapGetICCID);

7. diff -r (Sprint)/incs/common/libraries/hssoundlib/HsSoundLibCommon.h
        (Sprint + Cingular)/incs/common/libraries/hssoundlib/HsSoundLibCommon.h
Added the following define:
#define hsSndSwitchID_RadioCall			HS_RDIO(0x00010000)

8. diff -r (Sprint)/incs/common/libraries/http/HS_HTTPLibConst.h
        (Sprint + Cingular)/incs/common/libraries/http/HS_HTTPLibConst.h
Changed the error values for httpErrorDNSInval (-101) ... httpErrorTLSIntolerant (-126)
to httpErrorDNSInval (-200) to httpErrorTLSIntolerant

9. Only in (Sprint)/incs/common/libraries: pmconnectorlib
Removed PmConnectorLib

10. diff -r (Sprint)/incs/common/libraries/telephony/HsPhoneEvent.h
        (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneEvent.h
Added the following fields to the KSATRefresh structure
1. UInt16  refreshType;
2. UInt32  EFListOffset;

11. diff -r (Sprint)/incs/common/libraries/telephony/HsPhoneGSMTypes.h
        (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneGSMTypes.h
Added the following constants to the SIM Application Toolkit's data types
1. kSATReqLaunchBrowser,
2. kSATReqPlayTone,
3. kSATReqRefresh,

12. diff -r (Sprint)/incs/common/libraries/telephony/HsPhoneMiscTypes.h
        (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneMiscTypes.h
Added the following constant to Phone Library Attributes
1. phnGsmRadioAudioSet

13. diff -r (Sprint)/incs/common/libraries/telephony/HsPhoneSMSTypes.h
        (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneSMSTypes.h
Added the following constant to the SMSMessageFlags
1. kWAPMsg = 1L << 18

14. diff -r (Sprint)/incs/common/libraries/telephony/HsPhoneTraps.h
        (Sprint + Cingular)/incs/common/libraries/telephony/HsPhoneTraps.h
Added the following trap to the function traps for the Phone Library
#define     PhnLibTrapGetICCID                          (155+sysLibTrapCustom)

15. diff -r (Sprint)/incs/common/libraries/toneslibrary/TonesLibTypes.h
        (Sprint + Cingular)/incs/common/libraries/toneslibrary/TonesLibTypes.h
Added MP3 (toneTypeMP3) to the Tone types

---------------------------------------------------------------------------------------------------

October 17, 2004
1. Added the following Phone Library Attribute to Common\Libraries\Telephony\HsPhoneMiscTypes.h
phnGsmRadioAudioSet,

October 1, 2004
1. Added a new SMS Message flag in Common/Libraries/Telephony/HsPhoneSMSTypes.h
kDeferredDelivery = 1L << 17
2. Added the following defines in Common/System/palmOneResources.h
2.1 #define kPalmOneDeviceIDTreo600Sim	'H201'	/**< Device ID for Treo600 Simulator. */
2.2 #define kPalmOneDeviceIDTreo650		'H102'	/**< Device ID for Treo650. */
2.3 #define kPalmOneDeviceIDTreo650Sim	'H202'	/**< Device ID for Treo650 Simulator. */

September 24, 2004
1. Added CPM to 68K/Libraries

2. Added TxnLog to 68K/Libraries

3. Added PmPalmOSNVFS.h to 68K/System

4. Added PmAthenaLib to Common/Libraries

5. Added the following define to Common/Libraries/SmartTextEngine/SmartTextEngineDef.h
#define steTextMustBeVisible 		0x00008000	/**<If true, then any routine that returns selected text must verify the text is visible first*/

6. Changed the phnLibCDMAName and removed phnLibOldCDMADbName from Common/Libraries/Telephony/HsPhoneTypes.h
#define phnLibCDMAName			  "Phone Library"       /**< CDMA Library name  */

7. Added the VMFontLib to Common/Libraries

8. Added the following to Common/System/HsExtCommon.h
8.1 Error Code
8.1.1 #define hsErrGPRSNotAttached			  (hsExtErrorClass | 0x19)  // Failed to attach GPRS, or timed out while waiting to register
8.1.2 #define hsErrTimeOut					  (hsExtErrorClass | 0x1A) // The requiested operation timed out, without completing its objective
8.1.3 #define hsErrUserInterrupted			  (hsExtErrorClass | 0x1B) // The requiested operation was interrupted or cancelled by user, before completing its objective


8.2 A new parameter to HsAttrGet/Set call
8.2.1 hsAttrPhoneRadioPowerState	/**< notifies HsExt that cell phone radio was just powered on
 										or off; non-zero if cell phone radio was powered on, zero if
 										powered down.  Power on does *not* imply completion of SIM
 										detection or registration */

8.3 bit flag defines for enabled circumstances

    application may call HsCurrentLightCircumstance() to get current

    enabled light circumstances test bit to see if light circumstance is enabled or disabled

8.3.1 #define hsLightCircumstanceAlertWokeDeviceFlagBit			0x0001
8.3.2 #define hsLightCircumstanceQuickTypingFlagBit				0x0002
8.3.3 #define hsLightCircumstanceUserNotLookingFlagBit			0x0004
8.3.4 #define hsLightCircumstanceUserNotLookingLongTimeFlagBit	0x0008
8.3.5 #define hsLightCircumstanceKeylightOffFlagBit				0x0010

September 10, 2004
1. Added the following to the enum for the phone settings flags in 68K/Libraries/CarrierCustomLib/CarrierCustomLibTypes.h
kCCSMPhoneSettingsUseEFSPN          = 0x00000020   /**< Use EF SPN if any */

2. Added the following API call to 68K/Libraries/Telephony/HsPhoneGSM.h
extern Err PhnLibSendLongDTMF(UInt refNum, Char digit, Boolean on)

3. Removed PmAthenaLib from Common/Libraries

4. Added the following defines in Common/Libraries/Telephony/HsPhoneTypes.h
4.1 #define phnCall1StatusAlerting    0x0800 /**< The first  call's status is phnConnectionIncoming */
4.2 #define phnCall2StatusAlerting    0x1000 /**< The second call's status is phnConnectionIncoming */

5. Added the following notification to Common/System/HsCreators.h
5.1 #define pmNotifyBrightModeKeyEvent		  'PmBM'  // Yes: Notification sent by HsExtensions when the key event it receives
													  // will kick the device out of dark mode and into bright mode.
													  // HsExtensions will eat-up this key to prevent further processing
													  // unless this notification is set as handled.

6. Added the following defines related to the key light state to Common/System/HsExtCommon.h
6.1 #define hsKeyLightStateOff	false
6.2 #define hsKeyLightStateOn	true

September 3, 2004

1. Added the following palmOne Specific Nav API to 68K/Libraries/PmUIUtilLib/PmUIUtilLib.h
1.1 void PmUIUtilNavGetFocusColor (UInt16 refNum, HsNavFocusColorEnum color, RGBColorType* rgbColorP)
1.2 void PmUIUtilNavSetFocusColor (UInt16 refNum, HsNavFocusColorEnum color, RGBColorType* rgbColorP, RGBColorType* oldRgbColorP)

2. Added the following API call to 68K/Libraries/SmartTextEngine/SmartTextEngine.h
   Err LoadSmartTextEngine(UInt16* refNumP, Boolean* libLoadedP)


3. Added the following API call to 68K/System/HsExt.h
3.1 void HsNavGetFocusColor (HsNavFocusColorEnum color, RGBColorType* rgbColorP)
3.2 void HsNavSetFocusColor (HsNavFocusColorEnum color, RGBColorType* rgbColorP, RGBColorType* oldRgbColorP)


4. Removed the following from 68K/System/HsNav.h
4.1 void HsNavGetFocusColor (HsNavFocusColorEnum color, RGBColorType* rgbColorP)
4.2 void HsNavSetFocusColor (HsNavFocusColorEnum color, RGBColorType* rgbColorP, RGBColorType* oldRgbColorP)

5. Added the follwoing Phone Events in Common/Libraries/Telephony/HsPhoneEvent.h
5.1 phnEvtAlertingPreConnected,   /** 0x0029  */
5.2 phnEvtAlerting,               /** 0x002A  */
5.3 phnEvtPreConnected,           /** 0x002B  */

6. Added Go LCD Common header file in Common/System

August 27, 2004
1. Removed CPM from 68K/Libraries
2. Removed VMFontLib from Common/Libraries

August 20, 2004
1. Added the following API call to 68K/Libraries/VMFont/palmOneVMFontLib.h
/* Clear the interal cache of the font library */
extern void	VMFontClearCache(UInt16 libRefNum)

2. Added a new library called PmTraceLib in Common/Libraries

3. Added the following to Common/Libraries/Telephony/HsPhoneCDMATypes.h
3.1 Enum
	typedef enum
	{
		phnPDPDefaultIndex,
		phnPDPTempIndex,
		phnPDPGenericIndex
	} _PhnPDPIndexType;

	typedef UInt8 PhnPDPIndexType;

3.2 Macro
	#define isValidPhnPDPIndexType(t) ((t >= phnPDPDefaultIndex) && (t <= phnPDPGenericIndex))

3.3 Enum
	typedef enum
	{
		phnPDPCopyOK,
		phnPDPCopyInvalidFromIndex,
		phnPDPCopyFromIndexNotActive,
		phnPDPCopyNotAllowedAtFromIndex,
		phnPDPCopyInvalidToIndex,
		phnPDPCopyToIndexInUse,
		phnPDPCopyNotAllowedAtToIndex,
		phnPDPCopyInvalidNAILen,
		phnPDPCopyInvalidNAIName,
		phnPDPCopyDuplicateNAI,
		phnPDPCopyInvalidAAALen,
		phnPDPCopyInvalidSvcStrLen,
		phnPDPCopyInternalModemErr
	} _PhnPDPCopyAck;

	typedef UInt8 PhnPDPCopyAck;

3.4 Macro
	#define isValidPhnPDPCopyAck(c) ((c >= phnPDPCopyOK) && (c <= phnPDPCopyInternalModemErr))

3.5 Enum
	typedef enum
	{
		phnPDPDeleteOK,
		phnPDPDeleteInvalidIndex,
		phnPDPDeleteIndexActive,
		phnPDPDeleteIndexNotUsed,
		phnPDPDeleteNotAllowedAtIndex,
		phnPDPDeleteInternalModemErr
	} _PhnPDPDeleteAck;

	typedef UInt8 PhnPDPDeleteAck;

3.6 Macro
	#define isValidPhnPDPDeleteAck(d) ((d >= phnPDPDeleteOK) && (d <= phnPDPDeleteInternalModemErr))

3.7 Enum
	typedef enum
	{
	phnPDPReplaceOK,
	phnPDPReplaceInvalidIndex,
	phnPDPReplaceInvalidNotUsed,
	phnPDPReplaceNotAllowedAtIndex,
	phnPDPReplaceInvalidLen,
	phnPDPReplaceInternalModemErr
	} _PhnPDPReplaceAck;

	typedef UInt8 PhnPDPReplaceAck;

3.8 Macro
	#define isValidPhnPDPReplaceAck(r) ((r >= phnPDPReplaceOK) && (r <= phnPDPReplaceInternalModemErr))


4. Added a new Telephone Service MMI code type in Common/Libraries/Telephony/HsPhoneNetworkTypes.h
	typedef enum {
		phnTeleserviceUnknownMmiCode        = 0,
		phnTeleserviceVoiceMmiCode          = 11,
		phnTeleserviceAllMmiCode            = 12,
		phnTeleserviceFaxMmiCode            = 13,
		phnTeleserviceSMSMmiCode            = 16,
		phnTeleserviceAllExceptSMSCode      = 19,
		phnTeleserviceDataCircSyncMmiCode   = 24,
		phnTeleserviceDataCircAsyncMmiCode  = 25,
		phnTeleserviceDedicatedPackMmiCode  = 26,
		phnTeleserviceDedicatedPADMmiCode   = 27
	} PhnTeleserviceMmiCodeType;


5. Added a new hsPhoneLocationTypes.h, that contains Phone Location data types, in Common/Libraries/Telephony

6. Removed the following from Common/Libraries/filebrowser/FileBrowserLibCommon.h

6.1 Launch code sent to application that register (using the FileBrowserLibRegister API) when the user taps on a file with an extension
#define kSysAppLaunchCmdOpenFile   (sysAppLaunchCmdCustomBase + 100)

6.2 The command parameter block type for a kSysAppLaunchCmdOpenFile
typedef struct   // for kSysAppLaunchCmdOpenFile
{
	UInt16 volRefNum;
	Char *path;     // pointer to path which follows
	// Char path[]; // path follows pointer
} SysAppLaunchCmdOpenFileType;


7. Added a new phone application launch code to display the Options Menu in Common/System/HsAppLaunchCmd.h
7.1 Launch Code
    #define phoneAppLaunchCmdGetALSLineNumber	  (sysAppLaunchCmdCustomBase+21)
7.2 The data structure that goes with phoneAppLaunchCmdGetALSLineNumber launch cmd
    typedef struct
    {
		UInt8		lineNumber;
	}
	PhoneAppLaunchCmdGetALSLineNumberType, *PhoneAppLaunchCmdGetALSLineNumberTypePtr;

August 12, 2004
8/12/04
1. Added the following API calls in 68K/Libraries/FavoritesDBLibrary/FavoritesDBLib.h
1.1 Err FavDBLibIsFavoriteWithId(UInt16 refNum, UInt32 keyId, BooleanPtr existsP)
1.2 Boolean FavDBLibLookupInSpeedDialDB (UInt16 refNum, CharPtr keyP, CharPtr *nameP)

2. Added the following event notification to the Sound Library in Common/Libraries/HsSoundLib/HsSoundLibCommon.h
2.1 #define hsSndNotifyRingerSwitchOn	'hROn' /* notifies that ringer switch is in the on (play sounds to back speaker) position */
2.2 #define hsSndNotifyRingerSwitchOff	'hROf' /* notifies that ringer switch is in the silent (no sounds to back speaker) position */

3. Increased the maximum album name lenght on device to 255 in Common/Libraries/Imaging/palmOnePhotoCommon.h
/** Maximum album name length on device. (Increased for v3.0.)  */
#define PALM_PHOTO_ALBUM_MAX_NAME			(255)

4. Added the follwoing HALIDs to Common/System/HsExtCommon.h
4.1 #define hsHALIDHandspringOs5Rev2	  'H5_2'	// Used by Handspring's 5.2 HALs
4.2 #define hsHALIDHandspringOs5Rev2Sim	  'H5s2'	// Used by Handspring's 5.2 Simulator HALs

August 5, 2004
1. Added refNum as a parameter to the following API call in 68K/Libraries/Telephony/HsPhoneMisc.h
extern Err      PhnLibGetDeviceID (UInt16 refNum, CharPtr * deviceId)


2. Added the following API call to 68K/Libraries/filebrowser/FileBrowserLib68K.h
Err FileBrowserLibParseFileURL(UInt16 refNum, const Char *urlP, UInt16 *volRefNumP, Char **filePathP)

3. Added the following API calls to 68K/System/HsExt.h
3.1 Err HsNavGetFocusRingInfo (const FormType* formP, UInt16* objectIDP, Int16* extraInfoP, RectangleType* boundsInsideRingP, HsNavFocusRingStyleEnum* ringStyleP)
3.2 Err HsNavDrawFocusRing (FormType* formP, UInt16 objectID, Int16 extraInfo, RectangleType* boundsInsideRingP, HsNavFocusRingStyleEnum ringStyle, Boolean forceRestore)
3.3 Err HsNavRemoveFocusRing (FormType* formP)
3.4 void HsNavObjectTakeFocus (const FormType* formP, UInt16 objID)

4. renamed HsNav to FrmNav in the following API calls in 68K/System/HsNav.h
4.1 FrmNavDrawFocusRing (FormType* formP, UInt16 objectID, Int16 extraInfo,
4.2 FrmNavRemoveFocusRing (FormType* formP)
4.3 FrmNavGetFocusRingInfo (const FormType* formP, UInt16* objectIDP,
4.4 FrmNavObjectTakeFocus (const FormType* formP, UInt16 objID)

5. In Common/Libraries/HsSoundLib/HsSoundLibCommon.h
5.1 Added the following Sound Library Error Codes
5.1.1 #define hsSndErr_PortMuteLocked		(HS_SND_ERR(12)) /* unable to change mute state when hsSndParamMode_Normal is default */
5.1.2 #define hsSndErr_PortVolumeLocked		(HS_SND_ERR(13)) /* unable to change volume parameter when hsSndParamMode_Normal is default */
5.1.3 #define hsSndErr_PortBoostLocked		(HS_SND_ERR(14)) /* unable to change boost state when hsSndParamMode_Normal is default */
5.1.4 #define hsSndErr_PortPanLocked		(HS_SND_ERR(15)) /* unable to change pan value when hsSndParamMode_Normal is default */

5.2 Added the following HsSndPortID
5.2.1#define hsSndPortID_InHostPhone			HS_AC97(0x00000012) /* input port to be used by host if phone or TTY is active */


5.3 Added the following
// Use macros below to add these to individual parameter values below for additional parameter control (eg: muteState = HsSndParamValue_SetWithLock(hsSndMute_Enable); )
#define hsSndParamMode_Mask				0x7000	/* we reserve the 3 upper bits after sign bit in parameter values for the parameter mode */
#define hsSndParamMode_Normal			0x0000	/* this is the default parameter mode, allowing parameter to be changed with any other hsSndParamMode */
#define hsSndParamMode_LockParam		0x4000	/* use this to limit parameter to be changed only with hsSndParamMode_LockParam or hsSndParamMode_UnlockParam */
#define hsSndParamMode_UnlockParam		0x2000	/* use to enable parameter to be changed with any other hsSndParamMode afterwards */

// Utility macros to add LockParam/UnlockParam to parameter value
#define HsSndParamValue_SetWithLock(v)	(((v) & ~hsSndParamMode_Mask) | hsSndParamMode_LockParam)
#define HsSndParamValue_SetWithUnlock(v)(((v) & ~hsSndParamMode_Mask) | hsSndParamMode_UnlockParam)
#define HsSndParamValue_RemoveMode(v)	(((v) & 0x8000)? (v) | hsSndParamMode_Mask : (v) & ~hsSndParamMode_Mask)

5.4 Added the following hsSndParam_Pan parameters
5.4.1 #define hsSndPan_RightNormal		((Int16) 0x07FF) /* pan all the way to the right, can use with any hsSndParamMode */
5.4.2 #define hsSndPan_LeftNormal		((Int16) 0x0800) /* pan all the way to the left,  can use with any hsSndParamMode */


5.5 Added the following to be used as "flags" variable in HsSndPortInfo structure
5.5.1  #define hsSndPortFlags_Mute_Locked		0x00001000	/* if locked, then can only be changed with lock/unlock mute parameter changes */
5.5.2  #define hsSndPortFlags_Mute_Unlocked		0x00000000	/* mute can be changed with any type of mute parameters */
5.5.3  #define hsSndPortFlags_Mute_LockMask		0x00001000
5.5.4  #define hsSndPortFlags_Boost_Locked		0x00002000	/* if locked, then can only be changed with lock/unlock boost parameter changes */
5.5.5  #define hsSndPortFlags_Boost_Unlocked	0x00000000	/* boost can be changed with any type of boost parameters */
5.5.6  #define hsSndPortFlags_Boost_LockMask	0x00002000
5.5.7  #define hsSndPortFlags_Pan_Locked		0x00004000	/* if locked, then can only be changed with lock/unlock pan parameter changes */
5.5.8  #define hsSndPortFlags_Pan_Unlocked		0x00000000	/* pan can be changed with any type of pan parameters */
5.5.9  #define hsSndPortFlags_Pan_LockMask		0x00004000
5.5.10 #define hsSndPortFlags_Vol_Locked		0x00008000	/* if locked, then can only be changed with lock/unlock volume parameter changes */
5.5.11 #define hsSndPortFlags_Vol_Unlocked		0x00000000	/* volume can be changed with any type of volume parameters */
5.5.12 #define hsSndPortFlags_Vol_LockMask		0x00008000



6 In Common/Libraries/Telephony/HsPhoneEvent.h
6.1 Added the following PhnIndicationKind enum value
indicationInitComplete		// sent when the GSM library is done with it initialization when the radio is powered on


7 Added a feature for BlockATCmd in Common/Libraries/Telephony/HsPhoneTypes.h
7.1 #define phnLibFtrNumBlockATCmd  12
7.2 #define phnLibFtrValBlockATCmd  'noAT'

8. Added a new error code in Common/Libraries/filebrowser/FileBrowserLibCommon.h
#define kFileBrowserLibErrCantParse		(kFileBrowserLibErrorClass | 0x07)

July 29, 2004
1. Added the following API calls in 68K/Libraries/FavoritesDBLibrary/FavoritesDBLib.h
1.1 Boolean FavDBLibSeekSpeedDialRecord(UInt16 refNum, UInt16 * indexP, Int16 offset, Int16 direction)
1.2 Boolean FavDBLibGetRecordLabel (UInt16 refNum, UInt16 index, CharPtr *labelPP)
1.3 Boolean FavDBLibGetRecordId (UInt16 refNum, UInt16 index, UInt32 *uniqueIdP)
1.4 Boolean FavDBLibGetSpeedDialInfo (UInt16 refNum, UInt16 index, CharPtr *numberPP, CharPtr *extraDigitsPP, BooleanPtr autoExtraDigitsP,BooleanPtr isVoicemailP)

2. Added a new library called PmConnectorLib in 68K/Libraries


3. Added the following define in Common/Libraries/HsSoundLib/HsSoundLibCommon.h
#define hsSndNotifyAnswerRelease	'hRel' /* after hsSndNotifyAnswerButton notification, we send this event when button released */

4. Added a new library called PmConnectorLib in Common/Libraries


5. Added the follwoing Creator IDs to Common/System/palmOneCreators.h
#define kPalmOneCreatorIDFavorites	'Fave'
#define kPalmOneCreatorIDFileBrowser 'PFil'

July 23, 2004

1. Added a new 68K library called DeviceRes to 68K/Libraries

2. Changed the following in the HTTP Library 68K/Libraries/HTTP/HS_HTTPLib68K.h
2.1. Changed the version number from 1.0 to 2.0

2.2 Added the following enum to select the useragent for each request.
	enum {
		httpUASelectTypeHost,
		httpUASelectTypePath
	};

2.3 Added a structure for the Useragent Selection Rule
	typedef struct {

		Int32 fType;

		Char *fMatchString; /* substring to be matched */

		Char *fUserAgent;

	} HttpUASelectionRule;


2.4 Changed the following calles to include a refnum:
2.4.1 void HS_HTTPLibStreamGetRequestSent(UInt16 refnum, HS_HTTPLibStream stream, Int32 *out_total, Int32 *out_sent)
2.4.2 Boolean HS_HTTPLibStreamGetHeaderWithOffset(UInt16 refnum, HS_HTTPLibStream stream, Int32 in_type, Int32 in_ofs, Int32 *out_off, Int32 *out_len)
2.4.3 Boolean HS_HTTPLibSetUserPassX(UInt16 refnum, HS_HTTPLibHandle libH, Int32 in_type, Int32 in_target, HS_HTTPLibSplit *in_challenge, HS_HTTPLibSplit *in_realm, Char *in_user, Int32 in_user_len, Char *in_pass, Int32 in_pass_len, Char *in_url, Int32 in_url_len, Char *in_host, Int32 in_host_len, Int32 in_port, Boolean in_keep_user, Boolean in_keep_pass)
2.4.4 void HS_HTTPLibFindAuthCandidate(UInt16 refnum, HS_HTTPLibHandle libH, Int32 in_target, Char *in_url, Int32 in_url_len, Char *in_host, Int32 in_host_len, Int32 in_port, Char *in_realm, Int32 in_realm_len, HS_HTTPLibString *out_user, HS_HTTPLibString *out_pass, Boolean *out_keep_user, Boolean *out_keep_pass)
2.4.5 void HS_HTTPLibSetWakeUpCallback(UInt16 refnum, HS_HTTPLibHandle libH, HS_HTTPLibWakeUpCallbackProc in_proc, HS_HTTPLibOpaque in_opaque)

2.5 Added a new API call:
Boolean HS_HTTPLibSetUseragentSelectionRule(UInt16 refnum, HS_HTTPLibHandle libH, HttpUASelectionRule *rules, Int32 len)

3. In 68K/Libraries/Imaging/palmOnePhoto.h
3.1 Added a new API call to display thumbnail for still image or video:
extern Err PalmPhotoDisplayThumbnailV3(UInt16 refNum, PalmPhotoHandle imageH, const PalmPhotoDisplayParam *displayParamP)

4. In 68K/Libraries/PmUIUtilLib/PmUIUtilLib.h
4.1 Err PmUIUtilLibOpen (UInt16 refNum)
4.2 Err PmUIUtilLibClose (UInt16 refNum)

5. Added a the Snd File Stream to 68K/Libraries/SndFileStream

6. In 68K/Libraries/Telephony/HsPhoneGSM.h
6.1 Added the following API call:
extern Err PhnLibGetSDNList(UInt16 refNum, PhnAddressList* aList, PhnPhoneBookInfoPtr info)

7. In 68K/Libraries/Telephony/HsPhoneMisc.h
7.1 Added the following API call:
extern Err      PhnLibGetDeviceID (CharPtr * deviceId)

8. In 68K/Libraries/TonesLibrary/TonesLib.h
8.1. Added the following API call:
Err TonesLibRecordTone (UInt16 refNum)


9. In Common/Libraries/HsSoundLib/HsSoundLibCommon.h
9.1. Added the following to the Sound library event notification
#define hsSndNotifyAnswerHold		'hHol' /* after hsSndNotifyAnswerButton notification, we may send this event if answer-button still held down for some time */

9.2. Added the following to Snd Port ID:
#define hsSndPortID_InHostBaseMic		HS_AC97(0x00000010) /* input port controlling source base port for host recording */
#define hsSndPortID_InHostHeadsetMic	HS_AC97(0x00000011) /* input port controlling source headset port for host recording */


9.3. Added the following value to the possible parameters of type HsSndPortClass passed to the HsSndSwitchGetPort or HsSndSwitchSetPort:
#define hsSndPortClass_Anchor			((UInt16) 3) /* valid only with HsSndSwitchGetPort. Use to obtain the fixed port in a switch that doesn't change (the one we patch to other ports) */


10. In Common/Libraries/Imaging/palmOnePhotoCommon.h
10.1. Changed typedef UInt8 PalmImageUID[21] to typedef UInt8 PalmPhotoUID[21];

10.2. Added the following:
10.2.1 /** Notes.infoP points to a PalmPhotoUID.		   New in v3.0. */
#define palmPhotoExtraInfoImageUID 			(0x0005)

10.2.2 /** Notes.infoP points to a PalmPhotoUID.		   New in v3.0. */
#define palmPhotoExtraInfoSoundUID			(0x0006)

10.2.3 /** Notes.infoP points to a PalmPhotoUID.		   New in v3.0. */
#define palmPhotoExtraInfoCompositeUID		(0x0007)

and removed the following:
/** Notes.infoP points to a PalmImageUID.		   New in v3.0. */
#define palmPhotoExtraInfoUID 				(0x0005)

11. Added the Multi-Connector Library called PmAthenaLib to Common/Libraries


12. In Common/System/HsChars.h
12.1 Added a define for the Dark Mode (Opt + right-shift)
// Opt + RightShift == "dark mode" on Treo 650
#define hsChrDarkMode                                     0x1620


May 27, 2004
palmOne's Latest Header Files
\palmOneHeadersPublic:

Added the following API calls 68K/Libraries/Telephony/HsPhoneMisc.h
1. extern Err      PhnLibAttrGet (UInt16 refNum, UInt16 attr, UInt32 flags, void* dataP)
2. extern Err      PhnLibAttrSet (UInt16 refNum, UInt16 attr, UInt32 flags, void* dataP)

Added the following Phone Library Attributes enum to Common/Libraries/Telephony/HsPhoneMiscTypes.h
	typedef enum
	{
		phnGsmAttrFirst = 0,
		phnGsmNoFWVersionCompatibilityCheck,

		phnGsmAttrLast
	}
	PhnLibAttrType, * PhnLibAttrPtr;

Added the following to the Preference Enum in Common/System/HsExtCommon.h
hsPrefSideHardCharAppCreator,		/**< UInt32 : Creator type of app to launch
										  instead of the normally mapped app when
										  the side key is pressed */
Added the following value to the Light Cirmustance Enum in Common/System/HsExtCommon.h
hsLightCircumstanceUserNotLookingLongTime = 3,
The user is not looking at the device and if the inactivity persists then we want to turn the
screen off

Added the following define to the Common/System/HsKeyCommon.h
#define vchrHardRockerCenter			0x013D	// Character tied directly to rocker-
												//	center hardware key.  vchrRockerCenter
												//	is associated with select action of
												//	rocker-center (which happens on key-up
												//	of rocker-center hardware key).

May 20, 2004
palmOne's Latest Header Files
\palmOneHeadersPublic:
palmOneHeadersPublic37/68K/Libraries/FavoritesDBLibrary/FavoritesDBLib.h
   Purpose       Looks in the favorites DB for a match of the address.  If found, it returns
                 the name (favorites label).
   Declared In   FavoritesDBLib.h
   Prototype     CharPtr FavDBLibResolveAddress (UInt16 refNum, CharPtr addressP, Word kind)
   Parameters    -> refNum        The library reference number.
                 -> addressP	  The address to match.
				 -> kind		  The kind of favorite to match
   Returns       A pointer to the name string, or NULL if not found.  The caller
                 is responsible for freeing up this memory block.

palmOneHeadersPublic37/68K/Libraries/Imaging/PalmPhoto.h
Media formats Supported
	AVI file with ADPCM audio + MPEG4 video		New in v2.0.
	AVI file with MP3 audio + MPEG4 video		New in v2.0.
	ASF file with ADPCM audio + MPEG4 video		New in v2.0.
	ASF file with MP3 audio + MPEG4 video		New in v2.0.
	3GP file with AMR audio + MPEG4 video 		New in v3.0.
	3GP file with AMR audio + H.263 video 		New in v3.0.
	3G2 file with QCELP audio + MPEG4 video 	New in v3.0.
	3G2 file with QCELP audio + H.263 video 	New in v3.0.
	Default still capture format.				New in v3.0.
	Default video capture format.				New in v3.0.

Capture resolution supported by device camera
	SUBQCIF (128x96)							New in v3.0.
	QQVGA (160x120)							 	New in v3.0.
	QCIF (176x144)							 	New in v3.0.
	QVGA (320x240)							 	New in v3.0.
	CIF (352x288)							 	New in v3.0.
	VGA (640x480)							 	New in v3.0.
	SVGA (800x600)							 	New in v3.0.
	XVGA (1024x768)							 	New in v3.0.
	SXGA (1280x960)							 	New in v3.0.

Capture a new still image or video clip using the built-in camera.

New functionality added in 3.0:
Can capture video as well as still images.
    (For video, height and width in captureParam.imageInfo
      must match a supported resolution as returned by
    PalmPhotoGetSupportedCaptureResolutionsV3. Location
      in captureParam.fileLocation must be a VFSFile.)
Can limit size (in bytes) of captured media.
     (Set captureParam.imageInfo.fileSize to the desired
   limit. If fileSize = 0, no constraint s placed on
   ithe file size.)

1.
   Purpose       Check for presence of an enabled camera.
   Declared In   PalmPhoto.h
   Prototype     extern Boolean PalmPhotoIsCameraAvailableV3(UInt16 refNum)
   Parameters    -> refNum        The library reference number.
   Returns       false if no camera or camera is disabled.

2.
   Purpose       Returns resolutions that are supported by the still or video camera.
                 Returns zero (palmPhotoCaptureResNone) if no camera, or camera disabled.
   Declared In   PalmPhoto.h
   Prototype     extern PalmPhotoCaptureResolution PalmPhotoGetSupportedCaptureResolutionsV3(UInt16 refNum, PalmPhotoMediaType mediaType)
   Parameters    -> refNum        The library reference number.
                 -> mediaType	  palmPhotoMediaTypePhoto or palmPhotoMediaTypeVideo.
   Returns       Flags (bit is set for each resolution that camera supports).

3.
   Purpose       Select images using the Photo Selection/Deletion dialog, with
                 optional camera support. The selected images are passed in the selection parameter.
                 To free the memory allocated, use the PalmPhotoFreeSelections() function.
   Declared In   PalmPhoto.h
   Prototype     extern Err PalmPhotoSelectDlgV3(UInt16 refNum, PalmPhotoSelectionParam *photoSelectionP, const PalmPhotoCaptureParamV2 *captureParamP,
                                        		 PalmPhotoDlgType dlgType, PalmPhotoMediaType mediaType, Boolean handleCard)
   Parameters     -> refNum             The library reference number.
                 <-> photoSelectionP	Palm Photo selection parameter. Contains array of selections on return.
				  -> captureParamP		Palm Photo capture parameter. Set to null to suppress the camera.
    							        Camera only supported for single select dialog type.
    							        Camera only supported when media type is photo OR video (not all)
				  -> dlgType			Selection dialog type (eg palmPhotoDlgSelection...)
				  -> mediaType			Dialog displays only photos, only videos, or all media types.
				  -> handleCard		    If true, the dialog with intercept card notifications.
   Returns       Error code.

4.
   Purpose       Display an image or play a video.
                 A still image referenced by imageH will be drawn in the window and
                 rectangle specified within *displayParamP.  If the window handle is NULL, the
                 current draw window is used.  A video referenced by imageH will be played in
                 the video player, and the window and rectangle will be ignored.

                 The image is displayed in "best fit" mode, scaled exactly so that the
                 rectangle is completely filled, then cropping anything outside the
                 rectangle. (See also PalmPhotoDisplayImage for shrink-to-fit scaling.)

                 If the callback function returns false, the Photo API will halt
                 the draw or play operation.

                 New in v3.0.

   Declared In   PalmPhoto.h
   Prototype     extern Err PalmPhotoDisplayImageToSizeV3(UInt16 refNum, PalmPhotoHandle imageH, const PalmPhotoDisplayParam *displayParamP)
   Parameters    -> refNum          The library reference number.
                 -> imageH		    Image handle.
				 -> displayParamP	Display parameter.
   Returns       Error code.


palmOneHeadersPublic37/Common/Libraries/CameraMgr/palmOneCameraCommon.h
	Added support to set or get stream preview (on or off)

palmOneHeadersPublic37/Common/Libraries/Telephony/HsPhoneEvent.h
	Added WAP related support.

palmOneHeadersPublic37/Common/Libraries/Telephony/HsPhoneTypes.h
	Added WAP to the Classes of service for which applications can register


May 14, 2004
palmOne's Latest Header Files
\palmOneHeadersPublic:
1. Common/Libraries/CameraMgr/palmOneCameraCommon.h
   Added the following defines:

   /** Capture format in 16-bit YUV420. */
   #define kCamLibCaptureDataFormatYUV420				0x0040
   /** Capture format in 16-bit YCbCr420. */
   #define kCamLibCaptureDataFormatYCbCr420			    0x0080
   /** Capture format in 16-bit YUV420 Planar. */
   #define kCamLibCaptureDataFormatYUV420Planar		    0x0100
   /** Capture format in 16-bit YCbCr420 Planar. */
   #define kCamLibCaptureDataFormatYCbCr420Planar		0x0200

2. Common/Libraries/Telephony/HsPhoneCDMATypes.h
   Added the following structure
   typedef struct
   {
     UInt32 timeInSecs;         /**< Current time in minutes */
     Boolean daylightSavings;   /**< Is this daylight savings  */
     UInt16 timeZone;           /**< Number of minutes east of GMT  */
   } PhnNetworkTime;

3. Common/System/HsExtCommon.h
   Added the following define:
   #define hsDispFtrValBulverdePxa27X			  0x00000005	// Intel Bulverde PXA 270, 271, etc.

4. Common/System/HsNavCommon.h
   Changed	hsNavFocusRingWidth from 3 to 6
   #define	hsNavFocusRingWidth			  6		  // Width in double-density coordinates

   Deprecated 0,1,2 and 3 (HsNav version 1 focus treatment) and
   added 5,6,7,8,9,10 (HsNav version 2 focus treatment
   enum HsNavFocusColorEnumTag
   {
     hsNavFocusColorInsideBottomRightRing	= 0,  // Deprecated (HsNav version 1 focus treatment)
     hsNavFocusColorInsideTopLeftRing = 1,		  // Deprecated (HsNav version 1 focus treatment)
     hsNavFocusColorMiddleRing = 2,			      // Deprecated (HsNav version 1 focus treatment)
     hsNavFocusColorOutsideRing = 3,			  // Deprecated (HsNav version 1 focus treatment)
     hsNavFocusColorSecondaryHighlight = 4,
     hsNavFocusColorRing1 = 5,					  // HsNav version 2 focus treatment.  Innermost ring.
     hsNavFocusColorRing2 = 6,					  // HsNav version 2 focus treatment.
     hsNavFocusColorRing3 = 7,					  // HsNav version 2 focus treatment.
     hsNavFocusColorRing4 = 8,					  // HsNav version 2 focus treatment.
     hsNavFocusColorRing5 = 9,					  // HsNav version 2 focus treatment.
     hsNavFocusColorRing6 = 10					  // HsNav version 2 focus treatment.  Outermost ring.
   };
   typedef UInt16 HsNavFocusColorEnum;

5. Moved PalmCreators.h and PalmFeatures.h from 68K/System to Common/System

6. 68K/Libraries and Common/Libraries
   Added a LCDOverlay API, which can be used to display YCbCr images directly on the
   screen.

May 05, 2004
palmOne's Latest Header Files
\palmOneHeadersPublic:
1. 68K/Libraries/SmartTextEngine/SmartTextEngine.h
   A new SmartTextEngine (STE) API call has been added to SmartTextEngine.h
   This call is STEHasMessageSelection:
   Purpose       This routine checks if there is a current message selected.
   Declared In   SmartTextEngine.h
   Prototype     extern Boolean STEHasMessageSelection(UInt16 refNum, UInt16 engineRefNum)
   Parameters    -> refNum        The library reference number
                 -> engineRefNum  The STE engine reference number
   Returns       True if there is a current message selected. False otherwise.

2. Common/Libraries/HsSoundLib/HsSoundLibCommon.h
   The HsSoundLib version has been changed to 0x01010000 from 0x01000000
   #define hsSndLibVersion				0x01010000	/* current HsSoundLib version */

3. Common/System/HsAppLaunchCmd.h
   Added a new launch code to the Phone application.
   // Launch the phone application and display the Options menu
   #define phoneAppLaunchOptionsMenu		  (sysAppLaunchCmdCustomBase+20)

April 30, 2004
palmOne's Latest Header Files

\palmOneHeadersPublic:
	The partially integrated Handspring and Palm SG headers.
	All the Handspring and Palm SG headers can be found here.
	They have not yet been fully integrated. This will gradually happen over a period of time.
	Please don't directly include the headers instead include palmOne_68K.h for general palmOne
	components or Hs.h for Handspring specific components. These two headers together include all
	the other headers. The filenames for other headers will change in the future.

	P.S. the 5-Way headers are the same as Treo 600 5-Way headers.

April 20th, 2004
palmOne's Latest Header Files

Please get the Palm OS Garnet Update (68K) R1 from www.palmos.com
to use these header files.

