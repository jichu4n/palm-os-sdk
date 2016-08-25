#ifndef __UICOMMON_H__
#define __UICOMMON_H__

enum justifications { leftAlign, centerAlign, rightAlign };
typedef enum justifications JustificationType;

//------------------------------------------------------------
// System functions 
//------------------------------------------------------------
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))



//------------------------------------------------------------
// Temporary resource routines 
//------------------------------------------------------------

#define strRsc 								'tSTR'
#define ainRsc 								'tAIN'
#define iconType								'tAIB'
#define bitmapRsc 							'Tbmp'
#define alertRscType   						'Talt'
#define kbdRscType							'tkbd'
#define MenuRscType							'MBAR'
#define fontRscType							'NFNT'
#define verRsc									'tver'
#define appInfoStringsRsc					'tAIS'


// Application version string ID
#define appVersionID							1
#define ainID									1000

//	System version string ID - this is hidden in
//	the SystemVersion.rsrc resource, because the 'system' resources
//	don't have ResEdit formats.
#define systemVersionID						10000

// System bitmaps
#define palmLogoBitmap						10000
#define keyboardBackspaceBitmap			10001
#define keyboardTabBitmap					10002
#define keyboardReturnBitmap				10003
#define InformationAlertBitmap 			10004
#define ConfirmationAlertBitmap			10005
#define WarningAlertBitmap					10006
#define ErrorAlertBitmap					10007
#define keyboardShiftBitmap				10008
#define keyboardCapBitmap					10009


// System string resources
#define daysOfWeekStrID						10000
#define dayFullNamesStrID					10001
#define monthNamesStrID						10002
#define monthFullNamesStrID				10003
#define categoryAllStrID					10004
#define categoryEditStrID  				10005
#define menuCommandStrID					10006
#define launcherBatteryStrID				10007
#define systemNameStrID						10008


// System Alerts
#define RenameCategoryAlert				10000
#define RenameCategoryYes					0
#define RenameCategoryNo					1

#define RemoveCategoryAlert				10001
#define RemoveCategoryYes					0
#define RemoveCategoryNo					1

#define LowBatteryAlert						10002
#define VeryLowBatteryAlert				10003
#define UndoAlert								10004
#define UndoCancelButton					1

#define MergeCategoryAlert					10005
#define MergeCategoryYes					0
#define MergeCategoryNo						1

#define privateRecordInfoAlert			10006

#define ClipboardLimitAlert				10007

#define CategoryExistsAlert				10012

#define DeviceFullAlert						10013


// System Menu Bar and Menus
#define sysEditMenuID						10000
#define sysEditMenuUndoCmd					10000
#define sysEditMenuCutCmd					10001
#define sysEditMenuCopyCmd					10002
#define sysEditMenuPasteCmd				10003
#define sysEditMenuSelectAllCmd			10004
#define sysEditMenuSeparator				10005
#define sysEditMenuKeyboardCmd			10006


// Keyboard Menu Bar and Menus
#define sysKeyboardMenuID					10100
#define sysKeyboardEditUndoCmd			100
#define sysKeyboardEditCutCmd				101
#define sysKeyboardEditCopyCmd			102
#define sysKeyboardEditPasteCmd			103
#define sysKeyboardEditSelectAllCmd		104


// Note View Menu Bar and Menus
#define noteMenuID							10200
#define noteUndoCmd							10000
#define noteCutCmd							10001
#define noteCopyCmd							10002
#define notePasteCmd							10003
#define noteSelectAllCmd					10004
#define noteSeparator						10005
#define noteKeyboardCmd						10006
#define noteTopOfPageCmd					10200
#define noteBottomOfPageCmd				10201


// System Keyboard Dialog
#define SystemKeyboardID					10000


// Categories Edit Dialog
#define CategoriesEditForm					10000
#define CategoriesEditDoneButton			10002
#define CategoriesEditFirstField 		10004


// Date Selector Dialog
#define DateSelectorForm					10100
#define DateSelectorYearLabel				10102
#define DateSelectorPriorYearButton		10103
#define DateSelectorNextYearButton		10104
#define DateSelectorTodayButton			10118
#define DateSelectorCancelButton			10119
#define DateSelectorDayGadget				10120


// Time Selector Dialog
#define TimeSelectorForm					10200
#define TimeSelectorStartTimeButton		10204
#define TimeSelectorEndTimeButton		10205
#define TimeSelectorHourList				10206
#define TimeSelectorMinuteList			10207
#define TimeSelectorOKButton				10208
#define TimeSelectorCancelButton			10209
#define TimeSelectorNoTimeButton			10210


// Onscreen Keyboard
#define KeyboardForm							10300
#define KeyboardGadget						10310

// Help Dialog
#define HelpForm								10400
#define HelpField								10402
#define HelpDoneButton						10403
#define HelpUpButton							10404
#define HelpDownButton						10405


// Find Dialog
#define FindDialog							10500
#define FindStrField							10503
#define FindOKButton							10504


// Find Results Dialog
#define FindResultsDialog					10600
#define FindResultsMsgLabel				10602
#define FindResultsTable					10603
#define FindResultsGoToButton				10604
#define FindResultsCancelButton			10605
#define FindResultsMoreButton				10606
#define FindResultsSearchingStr			10607
#define FindResultsMatchesStr				10608
#define FindResultsNoMatchesStr			10609
#define FindResultsContinueStr			10610


// Note View (used by Datebook, To Do, and Address apps)
#define NoteView								10900
#define NoteField								10901
#define NoteDoneButton						10902
#define NoteSmallFontButton				10903
#define NoteLargeFontButton				10904
#define NoteDeleteButton					10905
#define NoteUpButton							10906
#define NoteDownButton						10907
#define NoteFontGroup						1

//	About Box - used by Datebook, Memo, Address, To Do, & others
#define aboutDialog							11000
#define aboutNameLabel						11001
#define aboutVersionLabel					11002
#define aboutErrorStr						11003

//------------------------------------------------------------
// Temporary resource routines 
//------------------------------------------------------------

void*		ResLoadForm (Word rscID)
						SYS_TRAP(sysTrapResLoadForm);

void* 	ResLoadMenu (Word rscID)
						SYS_TRAP(sysTrapResLoadMenu);


CharPtr	ResLoadString (Word rscID);



//------------------------------------------------------------
// Temporary debugging routines 
//------------------------------------------------------------
extern Boolean HeapDump (void);


#endif //__UICOMMON_H__
