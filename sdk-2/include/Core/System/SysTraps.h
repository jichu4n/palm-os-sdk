/*******************************************************************
 * 							 Pilot Software  
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:  
 *		SysTraps.h
 *
 * Description:
 *		Pilot Traps
 *
 * History:
 *		6/13/95	RM	- Created by Ron Marianetti   
 *
 *******************************************************************/

 #ifndef __SYSTRAPS_H_
 #define __SYSTRAPS_H_

// Include elementary types
#include <Common.h>
#include <M68KHwr.h>

// Regular traps start here and go up by 1's
#define	sysTrapBase			0xA000
typedef enum {
	sysTrapMemInit = sysTrapBase,
	sysTrapMemInitHeapTable,
	sysTrapMemStoreInit,
	sysTrapMemCardFormat,
	sysTrapMemCardInfo,
	sysTrapMemStoreInfo,
	sysTrapMemStoreSetInfo,
	sysTrapMemNumHeaps,
	sysTrapMemNumRAMHeaps,
	sysTrapMemHeapID,
	sysTrapMemHeapPtr,
	sysTrapMemHeapFreeBytes,
	sysTrapMemHeapSize,
	sysTrapMemHeapFlags,
	sysTrapMemHeapCompact,
	sysTrapMemHeapInit,
	sysTrapMemHeapFreeByOwnerID,
	sysTrapMemChunkNew,
	sysTrapMemChunkFree,
	sysTrapMemPtrNew,
	sysTrapMemPtrRecoverHandle,
	sysTrapMemPtrFlags,
	sysTrapMemPtrSize,
	sysTrapMemPtrOwner,
	sysTrapMemPtrHeapID,
	sysTrapMemPtrCardNo,
	sysTrapMemPtrToLocalID,
	sysTrapMemPtrSetOwner,
	sysTrapMemPtrResize,
	sysTrapMemPtrResetLock,
	sysTrapMemHandleNew,
	sysTrapMemHandleLockCount,
	sysTrapMemHandleToLocalID,
	sysTrapMemHandleLock,
	sysTrapMemHandleUnlock,
	sysTrapMemLocalIDToGlobal,
	sysTrapMemLocalIDKind,
	sysTrapMemLocalIDToPtr,
	sysTrapMemMove,
	sysTrapMemSet,
	sysTrapMemStoreSearch,
	sysTrapMemPtrDataStorage,
	sysTrapMemKernelInit,
	sysTrapMemHandleFree,
	sysTrapMemHandleFlags,
	sysTrapMemHandleSize,
	sysTrapMemHandleOwner,
	sysTrapMemHandleHeapID,
	sysTrapMemHandleDataStorage,
	sysTrapMemHandleCardNo,
	sysTrapMemHandleSetOwner,
	sysTrapMemHandleResize,
	sysTrapMemHandleResetLock,
	sysTrapMemPtrUnlock,
	sysTrapMemLocalIDToLockedPtr,
	sysTrapMemSetDebugMode,
	sysTrapMemHeapScramble,
	sysTrapMemHeapCheck,
	sysTrapMemNumCards,
	sysTrapMemDebugMode,
	sysTrapMemSemaphoreReserve,
	sysTrapMemSemaphoreRelease,
	sysTrapMemHeapDynamic,
	sysTrapMemNVParams,
	
	
	sysTrapDmInit,
	sysTrapDmCreateDatabase,
	sysTrapDmDeleteDatabase,
	sysTrapDmNumDatabases,
	sysTrapDmGetDatabase,
	sysTrapDmFindDatabase,
	sysTrapDmDatabaseInfo,
	sysTrapDmSetDatabaseInfo,
	sysTrapDmDatabaseSize,
	sysTrapDmOpenDatabase,
	sysTrapDmCloseDatabase,
	sysTrapDmNextOpenDatabase,
	sysTrapDmOpenDatabaseInfo,
	sysTrapDmResetRecordStates,
	sysTrapDmGetLastErr,
	sysTrapDmNumRecords,
	sysTrapDmRecordInfo,
	sysTrapDmSetRecordInfo,
	sysTrapDmAttachRecord,
	sysTrapDmDetachRecord,
	sysTrapDmMoveRecord,
	sysTrapDmNewRecord,
	sysTrapDmRemoveRecord,
	sysTrapDmDeleteRecord,
	sysTrapDmArchiveRecord,
	sysTrapDmNewHandle,
	sysTrapDmRemoveSecretRecords,
	sysTrapDmQueryRecord,
	sysTrapDmGetRecord,
	sysTrapDmResizeRecord,
	sysTrapDmReleaseRecord,
	sysTrapDmGetResource,
	sysTrapDmGet1Resource,
	sysTrapDmReleaseResource,
	sysTrapDmResizeResource,
	sysTrapDmNextOpenResDatabase,
	sysTrapDmFindResourceType,
	sysTrapDmFindResource,
	sysTrapDmSearchResource,
	sysTrapDmNumResources,
	sysTrapDmResourceInfo,
	sysTrapDmSetResourceInfo,
	sysTrapDmAttachResource,
	sysTrapDmDetachResource,
	sysTrapDmNewResource,
	sysTrapDmRemoveResource,
	sysTrapDmGetResourceIndex,
	sysTrapDmQuickSort,
	sysTrapDmQueryNextInCategory,
	sysTrapDmNumRecordsInCategory,
	sysTrapDmPositionInCategory,
	sysTrapDmSeekRecordInCategory,
	sysTrapDmMoveCategory,
	sysTrapDmOpenDatabaseByTypeCreator,
	sysTrapDmWrite,
	sysTrapDmStrCopy,
	sysTrapDmGetNextDatabaseByTypeCreator,
	sysTrapDmWriteCheck,
	sysTrapDmMoveOpenDBContext,
	sysTrapDmFindRecordByID,
	sysTrapDmGetAppInfoID,
	sysTrapDmFindSortPositionV10,
	sysTrapDmSet,
	sysTrapDmCreateDatabaseFromImage,

	
	sysTrapDbgSrcMessage,
	sysTrapDbgMessage,
	sysTrapDbgGetMessage,
	sysTrapDbgCommSettings,
	
	sysTrapErrDisplayFileLineMsg,
	sysTrapErrSetJump,
	sysTrapErrLongJump,
	sysTrapErrThrow,
	sysTrapErrExceptionList,
	
	sysTrapSysBroadcastActionCode,
	sysTrapSysUnimplemented,
	sysTrapSysColdBoot,
	sysTrapSysReset,
	sysTrapSysDoze,
	sysTrapSysAppLaunch,
	sysTrapSysAppStartup,
	sysTrapSysAppExit,
	sysTrapSysSetA5,
	sysTrapSysSetTrapAddress,
	sysTrapSysGetTrapAddress,
	sysTrapSysTranslateKernelErr,
	sysTrapSysSemaphoreCreate,
	sysTrapSysSemaphoreDelete,
	sysTrapSysSemaphoreWait,
	sysTrapSysSemaphoreSignal,
	sysTrapSysTimerCreate,
	sysTrapSysTimerWrite,
	sysTrapSysTaskCreate,
	sysTrapSysTaskDelete,
	sysTrapSysTaskTrigger,
	sysTrapSysTaskID,
	sysTrapSysTaskUserInfoPtr,
	sysTrapSysTaskDelay,
	sysTrapSysTaskSetTermProc,
	sysTrapSysUILaunch,
	sysTrapSysNewOwnerID,
	sysTrapSysSemaphoreSet,
	sysTrapSysDisableInts,
	sysTrapSysRestoreStatus,
	sysTrapSysUIAppSwitch,
	sysTrapSysCurAppInfoP,
	sysTrapSysHandleEvent,
	sysTrapSysInit,
	sysTrapSysQSort,
	sysTrapSysCurAppDatabase,
	sysTrapSysFatalAlert,
	sysTrapSysResSemaphoreCreate,
	sysTrapSysResSemaphoreDelete,
	sysTrapSysResSemaphoreReserve,
	sysTrapSysResSemaphoreRelease,
	sysTrapSysSleep,
	sysTrapSysKeyboardDialogV10,
	sysTrapSysAppLauncherDialog,
	sysTrapSysSetPerformance,
	sysTrapSysBatteryInfo,
	sysTrapSysLibInstall,
	sysTrapSysLibRemove,
	sysTrapSysLibTblEntry,
	sysTrapSysLibFind,
	sysTrapSysBatteryDialog,
	sysTrapSysCopyStringResource,
	sysTrapSysKernelInfo,
	sysTrapSysLaunchConsole,
	sysTrapSysTimerDelete,
	sysTrapSysSetAutoOffTime,
	sysTrapSysFormPointerArrayToStrings,
	sysTrapSysRandom,
	sysTrapSysTaskSwitching,
	sysTrapSysTimerRead,


	sysTrapStrCopy,
	sysTrapStrCat,
	sysTrapStrLen,
	sysTrapStrCompare,
	sysTrapStrIToA,
	sysTrapStrCaselessCompare,
	sysTrapStrIToH,
	sysTrapStrChr,
	sysTrapStrStr,
	sysTrapStrAToI,
	sysTrapStrToLower,

	sysTrapSerReceiveISP,
	
	sysTrapSlkOpen,
	sysTrapSlkClose,
	sysTrapSlkOpenSocket,
	sysTrapSlkCloseSocket,
	sysTrapSlkSocketRefNum,
	sysTrapSlkSocketSetTimeout,
	sysTrapSlkFlushSocket,
	sysTrapSlkSetSocketListener,
	sysTrapSlkSendPacket,
	sysTrapSlkReceivePacket,
	sysTrapSlkSysPktDefaultResponse,
	sysTrapSlkProcessRPC,

	
	sysTrapConPutS,
	sysTrapConGetS,
	
	sysTrapFplInit,
	sysTrapFplFree,
	sysTrapFplFToA,
	sysTrapFplAToF,
	sysTrapFplBase10Info,
	sysTrapFplLongToFloat,
	sysTrapFplFloatToLong,
	sysTrapFplFloatToULong,
	sysTrapFplMul,
	sysTrapFplAdd,
	sysTrapFplSub,
	sysTrapFplDiv,
	
	sysTrapScrInit,
	sysTrapScrCopyRectangle,
	sysTrapScrDrawChars,
	sysTrapScrLineRoutine,
	sysTrapScrRectangleRoutine,
	sysTrapScrScreenInfo,
	sysTrapScrDrawNotify,
	sysTrapScrSendUpdateArea,
	sysTrapScrCompressScanLine,
	sysTrapScrDeCompressScanLine,
	
	
	sysTrapTimGetSeconds,
	sysTrapTimSetSeconds,
	sysTrapTimGetTicks,
	sysTrapTimInit,
	sysTrapTimSetAlarm,
	sysTrapTimGetAlarm,
	sysTrapTimHandleInterrupt,
	sysTrapTimSecondsToDateTime,
	sysTrapTimDateTimeToSeconds,
	sysTrapTimAdjust,
	sysTrapTimSleep,
	sysTrapTimWake,
	
	sysTrapCategoryCreateListV10,
	sysTrapCategoryFreeListV10,
	sysTrapCategoryFind,
	sysTrapCategoryGetName,
	sysTrapCategoryEditV10,
	sysTrapCategorySelectV10,
	sysTrapCategoryGetNext,
	sysTrapCategorySetTriggerLabel,
	sysTrapCategoryTruncateName,
	
	sysTrapClipboardAddItem,
	sysTrapClipboardCheckIfItemExist,
	sysTrapClipboardGetItem,
	
	sysTrapCtlDrawControl,
	sysTrapCtlEraseControl,
	sysTrapCtlHideControl,
	sysTrapCtlShowControl,
	sysTrapCtlGetValue,
	sysTrapCtlSetValue,
	sysTrapCtlGetLabel,
	sysTrapCtlSetLabel,
	sysTrapCtlHandleEvent,
	sysTrapCtlHitControl,
	sysTrapCtlSetEnabled,
	sysTrapCtlSetUsable,
	sysTrapCtlEnabled,

	
	sysTrapEvtInitialize,
	sysTrapEvtAddEventToQueue,
	sysTrapEvtCopyEvent,
	sysTrapEvtGetEvent,
	sysTrapEvtGetPen,
	sysTrapEvtSysInit,
	sysTrapEvtGetSysEvent,
	sysTrapEvtProcessSoftKeyStroke,
	sysTrapEvtGetPenBtnList,
	sysTrapEvtSetPenQueuePtr,
	sysTrapEvtPenQueueSize,
	sysTrapEvtFlushPenQueue,
	sysTrapEvtEnqueuePenPoint,
	sysTrapEvtDequeuePenStrokeInfo,
	sysTrapEvtDequeuePenPoint,
	sysTrapEvtFlushNextPenStroke,
	sysTrapEvtSetKeyQueuePtr,
	sysTrapEvtKeyQueueSize,
	sysTrapEvtFlushKeyQueue,
	sysTrapEvtEnqueueKey,
	sysTrapEvtDequeueKeyEvent,
	sysTrapEvtWakeup,
	sysTrapEvtResetAutoOffTimer,
	sysTrapEvtKeyQueueEmpty,
	sysTrapEvtEnableGraffiti,

	
	sysTrapFldCopy,
	sysTrapFldCut,
	sysTrapFldDrawField,
	sysTrapFldEraseField,
	sysTrapFldFreeMemory,
	sysTrapFldGetBounds,
	sysTrapFldGetTextPtr,
	sysTrapFldGetSelection,
	sysTrapFldHandleEvent,
	sysTrapFldPaste,
	sysTrapFldRecalculateField,
	sysTrapFldSetBounds,
	sysTrapFldSetText,
	sysTrapFldGetFont,
	sysTrapFldSetFont,
	sysTrapFldSetSelection,
	sysTrapFldGrabFocus,
	sysTrapFldReleaseFocus,
	sysTrapFldGetInsPtPosition,
	sysTrapFldSetInsPtPosition,
	sysTrapFldSetScrollPosition,
	sysTrapFldGetScrollPosition,
	sysTrapFldGetTextHeight,
	sysTrapFldGetTextAllocatedSize,
	sysTrapFldGetTextLength,
	sysTrapFldScrollField,
	sysTrapFldScrollable,
	sysTrapFldGetVisibleLines,
	sysTrapFldGetAttributes,
	sysTrapFldSetAttributes,
	sysTrapFldSendChangeNotification,
	sysTrapFldCalcFieldHeight,
	sysTrapFldGetTextHandle,
	sysTrapFldCompactText,
	sysTrapFldDirty,
	sysTrapFldWordWrap,
	sysTrapFldSetTextAllocatedSize,
	sysTrapFldSetTextHandle,
	sysTrapFldSetTextPtr,
	sysTrapFldGetMaxChars,
	sysTrapFldSetMaxChars,
	sysTrapFldSetUsable,
	sysTrapFldInsert,
	sysTrapFldDelete,
	sysTrapFldUndo,
	sysTrapFldSetDirty,
	sysTrapFldSendHeightChangeNotification,
	sysTrapFldMakeFullyVisible,
	
	
	sysTrapFntGetFont,
	sysTrapFntSetFont,
	sysTrapFntGetFontPtr,
	sysTrapFntBaseLine,
	sysTrapFntCharHeight,
	sysTrapFntLineHeight,
	sysTrapFntAverageCharWidth,
	sysTrapFntCharWidth,
	sysTrapFntCharsWidth,
	sysTrapFntDescenderHeight,
	sysTrapFntCharsInWidth,
	sysTrapFntLineWidth,


	
	sysTrapFrmInitForm,
	sysTrapFrmDeleteForm,
	sysTrapFrmDrawForm,
	sysTrapFrmEraseForm,
	sysTrapFrmGetActiveForm,
	sysTrapFrmSetActiveForm,
	sysTrapFrmGetActiveFormID,
	sysTrapFrmGetUserModifiedState,
	sysTrapFrmSetNotUserModified,
	sysTrapFrmGetFocus,
	sysTrapFrmSetFocus,
	sysTrapFrmHandleEvent,
	sysTrapFrmGetFormBounds,
	sysTrapFrmGetWindowHandle,
	sysTrapFrmGetFormId,
	sysTrapFrmGetFormPtr,
	sysTrapFrmGetNumberOfObjects,
	sysTrapFrmGetObjectIndex,
	sysTrapFrmGetObjectId,
	sysTrapFrmGetObjectType,
	sysTrapFrmGetObjectPtr,
	sysTrapFrmHideObject,
	sysTrapFrmShowObject,
	sysTrapFrmGetObjectPosition,
	sysTrapFrmSetObjectPosition,
	sysTrapFrmGetControlValue,
	sysTrapFrmSetControlValue,
	sysTrapFrmGetControlGroupSelection,
	sysTrapFrmSetControlGroupSelection,
	sysTrapFrmCopyLabel,
	sysTrapFrmSetLabel,
	sysTrapFrmGetLabel,
	sysTrapFrmSetCategoryLabel,
	sysTrapFrmGetTitle,
	sysTrapFrmSetTitle,
	sysTrapFrmAlert,
	sysTrapFrmDoDialog,
	sysTrapFrmCustomAlert,
	sysTrapFrmHelp,
	sysTrapFrmUpdateScrollers,
	sysTrapFrmGetFirstForm,
	sysTrapFrmVisible,
	sysTrapFrmGetObjectBounds,
	sysTrapFrmCopyTitle,
	sysTrapFrmGotoForm,
	sysTrapFrmPopupForm,
	sysTrapFrmUpdateForm,
	sysTrapFrmReturnToForm,
	sysTrapFrmSetEventHandler,
	sysTrapFrmDispatchEvent,
	sysTrapFrmCloseAllForms,
	sysTrapFrmSaveAllForms,
	sysTrapFrmGetGadgetData,
	sysTrapFrmSetGadgetData,
	sysTrapFrmSetCategoryTrigger, 

	
	sysTrapUIInitialize,
	sysTrapUIReset,

	sysTrapInsPtInitialize,
	sysTrapInsPtSetLocation,
	sysTrapInsPtGetLocation,
	sysTrapInsPtEnable,
	sysTrapInsPtEnabled,
	sysTrapInsPtSetHeight,
	sysTrapInsPtGetHeight,
	sysTrapInsPtCheckBlink,
	
	sysTrapLstSetDrawFunction,
	sysTrapLstDrawList,
	sysTrapLstEraseList,
	sysTrapLstGetSelection,
	sysTrapLstGetSelectionText,
	sysTrapLstHandleEvent,
	sysTrapLstSetHeight,
	sysTrapLstSetSelection,
	sysTrapLstSetListChoices,
	sysTrapLstMakeItemVisible,
	sysTrapLstGetNumberOfItems,
	sysTrapLstPopupList,
	sysTrapLstSetPosition,
	
	sysTrapMenuInit,
	sysTrapMenuDispose,
	sysTrapMenuHandleEvent,
	sysTrapMenuDrawMenu,
	sysTrapMenuEraseStatus,
	sysTrapMenuGetActiveMenu,
	sysTrapMenuSetActiveMenu,

	
	sysTrapRctSetRectangle,
	sysTrapRctCopyRectangle,
	sysTrapRctInsetRectangle,
	sysTrapRctOffsetRectangle,
	sysTrapRctPtInRectangle,
	sysTrapRctGetIntersection,

	
	sysTrapTblDrawTable,
	sysTrapTblEraseTable,
	sysTrapTblHandleEvent,
	sysTrapTblGetItemBounds,
	sysTrapTblSelectItem,
	sysTrapTblGetItemInt,
	sysTrapTblSetItemInt,
	sysTrapTblSetItemStyle,
	sysTrapTblUnhighlightSelection,
	sysTrapTblSetRowUsable,
	sysTrapTblGetNumberOfRows,
	sysTrapTblSetCustomDrawProcedure,
	sysTrapTblSetRowSelectable,
	sysTrapTblRowSelectable,
	sysTrapTblSetLoadDataProcedure,
	sysTrapTblSetSaveDataProcedure,
	sysTrapTblGetBounds,
	sysTrapTblSetRowHeight,
	sysTrapTblGetColumnWidth,
	sysTrapTblGetRowID,
	sysTrapTblSetRowID,
	sysTrapTblMarkRowInvalid,
	sysTrapTblMarkTableInvalid,
	sysTrapTblGetSelection,
	sysTrapTblInsertRow,
	sysTrapTblRemoveRow,
	sysTrapTblRowInvalid,
	sysTrapTblRedrawTable,
	sysTrapTblRowUsable,
	sysTrapTblReleaseFocus,
	sysTrapTblEditing,
	sysTrapTblGetCurrentField,
	sysTrapTblSetColumnUsable,
	sysTrapTblGetRowHeight,
	sysTrapTblSetColumnWidth,
	sysTrapTblGrabFocus,
	sysTrapTblSetItemPtr,
	sysTrapTblFindRowID,
	sysTrapTblGetLastUsableRow,
	sysTrapTblGetColumnSpacing,
	sysTrapTblFindRowData,
	sysTrapTblGetRowData,
	sysTrapTblSetRowData,
	sysTrapTblSetColumnSpacing,


	
	sysTrapWinCreateWindow,
	sysTrapWinCreateOffscreenWindow,
	sysTrapWinDeleteWindow,
	sysTrapWinInitializeWindow,
	sysTrapWinAddWindow,
	sysTrapWinRemoveWindow,
	sysTrapWinSetActiveWindow,
	sysTrapWinSetDrawWindow,
	sysTrapWinGetDrawWindow,
	sysTrapWinGetActiveWindow,
	sysTrapWinGetDisplayWindow,
	sysTrapWinGetFirstWindow,
	sysTrapWinEnableWindow,
	sysTrapWinDisableWindow,
	sysTrapWinGetWindowFrameRect,
	sysTrapWinDrawWindowFrame,
	sysTrapWinEraseWindow,
	sysTrapWinSaveBits,
	sysTrapWinRestoreBits,
	sysTrapWinCopyRectangle,
	sysTrapWinScrollRectangle,
	sysTrapWinGetDisplayExtent,
	sysTrapWinGetWindowExtent,
	sysTrapWinDisplayToWindowPt,
	sysTrapWinWindowToDisplayPt,
	sysTrapWinGetClip,
	sysTrapWinSetClip,
	sysTrapWinResetClip,
	sysTrapWinClipRectangle,
	sysTrapWinDrawLine,
	sysTrapWinDrawGrayLine,
	sysTrapWinEraseLine,
	sysTrapWinInvertLine,
	sysTrapWinFillLine,
	sysTrapWinDrawRectangle,
	sysTrapWinEraseRectangle,
	sysTrapWinInvertRectangle,
	sysTrapWinDrawRectangleFrame,
	sysTrapWinDrawGrayRectangleFrame,
	sysTrapWinEraseRectangleFrame,
	sysTrapWinInvertRectangleFrame,
	sysTrapWinGetFramesRectangle,
	sysTrapWinDrawChars,
	sysTrapWinEraseChars,
	sysTrapWinInvertChars,
	sysTrapWinGetPattern,
	sysTrapWinSetPattern,
	sysTrapWinSetUnderlineMode,
	sysTrapWinDrawBitmap,
	sysTrapWinModal,
	sysTrapWinGetWindowBounds,
	sysTrapWinFillRectangle,
	sysTrapWinDrawInvertedChars,
	
	
	
	sysTrapPrefOpenPreferenceDBV10,
	sysTrapPrefGetPreferences,
	sysTrapPrefSetPreferences,
	sysTrapPrefGetAppPreferencesV10,
	sysTrapPrefSetAppPreferencesV10,

	
	sysTrapSndInit,
	sysTrapSndSetDefaultVolume,
	sysTrapSndGetDefaultVolume,
	sysTrapSndDoCmd,
	sysTrapSndPlaySystemSound,
	
	
	sysTrapAlmInit,
	sysTrapAlmCancelAll,
	sysTrapAlmAlarmCallback,
	sysTrapAlmSetAlarm,
	sysTrapAlmGetAlarm,
	sysTrapAlmDisplayAlarm,
	sysTrapAlmEnableNotification,
	
	
	sysTrapHwrGetRAMMapping,
	sysTrapHwrMemWritable,
	sysTrapHwrMemReadable,
	sysTrapHwrDoze,
	sysTrapHwrSleep,
	sysTrapHwrWake,
	sysTrapHwrSetSystemClock,
	sysTrapHwrSetCPUDutyCycle,
	sysTrapHwrLCDInit,
	sysTrapHwrLCDSleep,
	sysTrapHwrTimerInit,
	sysTrapHwrCursor,
	sysTrapHwrBatteryLevel,
	sysTrapHwrDelay,
	sysTrapHwrEnableDataWrites,
	sysTrapHwrDisableDataWrites,
	sysTrapHwrLCDBaseAddr,
	sysTrapHwrLCDDrawBitmap,
	sysTrapHwrTimerSleep,
	sysTrapHwrTimerWake,
	sysTrapHwrLCDWake,
	sysTrapHwrIRQ1Handler,
	sysTrapHwrIRQ2Handler,
	sysTrapHwrIRQ3Handler,
	sysTrapHwrIRQ4Handler,
	sysTrapHwrIRQ5Handler,
	sysTrapHwrIRQ6Handler,
	sysTrapHwrDockSignals,
	sysTrapHwrPluggedIn,

	
	sysTrapCrc16CalcBlock,
	
	
	sysTrapSelectDayV10,
	sysTrapSelectTime,
	
	sysTrapDayDrawDaySelector,
	sysTrapDayHandleEvent,
	sysTrapDayDrawDays,
	sysTrapDayOfWeek,
	sysTrapDaysInMonth,
	sysTrapDayOfMonth,
	
	sysTrapDateDaysToDate,
	sysTrapDateToDays,
	sysTrapDateAdjust,
	sysTrapDateSecondsToDate,
	sysTrapDateToAscii,
	sysTrapDateToDOWDMFormat,
	sysTrapTimeToAscii,
		
	
	sysTrapFind,
	sysTrapFindStrInStr,
	sysTrapFindSaveMatch,
	sysTrapFindGetLineBounds,
	sysTrapFindDrawHeader,
	
	sysTrapPenOpen,
	sysTrapPenClose,
	sysTrapPenGetRawPen,
	sysTrapPenCalibrate,
	sysTrapPenRawToScreen,
	sysTrapPenScreenToRaw,
	sysTrapPenResetCalibration,
	sysTrapPenSleep,
	sysTrapPenWake,
	
	
	sysTrapResLoadForm,
	sysTrapResLoadMenu,
	
	sysTrapFtrInit,
	sysTrapFtrUnregister,
	sysTrapFtrGet,
	sysTrapFtrSet,
	sysTrapFtrGetByIndex,
	
	
	
	sysTrapGrfInit,
	sysTrapGrfFree,
	sysTrapGrfGetState,
	sysTrapGrfSetState,
	sysTrapGrfFlushPoints,
	sysTrapGrfAddPoint,
	sysTrapGrfInitState,
	sysTrapGrfCleanState,
	sysTrapGrfMatch,
	sysTrapGrfGetMacro,
	sysTrapGrfFilterPoints,
	sysTrapGrfGetNumPoints,
	sysTrapGrfGetPoint,
	sysTrapGrfFindBranch,
	sysTrapGrfMatchGlyph,
	sysTrapGrfGetGlyphMapping,
	sysTrapGrfGetMacroName,
	sysTrapGrfDeleteMacro,
	sysTrapGrfAddMacro,
	sysTrapGrfGetAndExpandMacro,
	sysTrapGrfProcessStroke,
	sysTrapGrfFieldChange,
	
	
	sysTrapGetCharSortValue,
	sysTrapGetCharAttr,
	sysTrapGetCharCaselessValue,
	
	
	sysTrapPwdExists,
	sysTrapPwdVerify,
	sysTrapPwdSet,
	sysTrapPwdRemove,
	
	sysTrapGsiInitialize,
	sysTrapGsiSetLocation,
	sysTrapGsiEnable,
	sysTrapGsiEnabled,
	sysTrapGsiSetShiftState,
	
	sysTrapKeyInit,
	sysTrapKeyHandleInterrupt,
	sysTrapKeyCurrentState,
	sysTrapKeyResetDoubleTap,
	sysTrapKeyRates,
	sysTrapKeySleep,
	sysTrapKeyWake,
	
	
	sysTrapDlkControl,				// was sysTrapCmBroadcast
	
	sysTrapDlkStartServer,
	sysTrapDlkGetSyncInfo,
	sysTrapDlkSetLogEntry,

	sysTrapUnused2,				// *DO NOT RE-USE UNTIL 3.0* was sysTrapPsrInit (vmk 7/16/96)
	sysTrapSysLibLoad,			// was sysTrapPsrClose,
	sysTrapUnused4,				// was sysTrapPsrGetCommand,
	sysTrapUnused5,				// was sysTrapPsrSendReply,
	
	sysTrapAbtShowAbout,
	
	sysTrapMdmDial,
	sysTrapMdmHangUp,
	
	sysTrapDmSearchRecord,

	sysTrapSysInsertionSort,
	sysTrapDmInsertionSort,
	
	sysTrapLstSetTopItem,

	sysTrapSclSetScrollBar,
	sysTrapSclDrawScrollBar,
	sysTrapSclHandleEvent,
	
	sysTrapSysMailboxCreate,
	sysTrapSysMailboxDelete,
	sysTrapSysMailboxFlush,
	sysTrapSysMailboxSend,
	sysTrapSysMailboxWait,
	
	sysTrapSysTaskWait,
	sysTrapSysTaskWake,
	sysTrapSysTaskWaitClr,
	sysTrapSysTaskSuspend,
	sysTrapSysTaskResume,
	
	sysTrapCategoryCreateList,
	sysTrapCategoryFreeList,
	sysTrapCategoryEdit,
	sysTrapCategorySelect,
	
	sysTrapDmDeleteCategory,
	
	sysTrapSysEvGroupCreate,
	sysTrapSysEvGroupSignal,
	sysTrapSysEvGroupRead,
	sysTrapSysEvGroupWait,
	
	sysTrapEvtEventAvail,
	sysTrapEvtSysEventAvail,
	sysTrapStrNCopy,
	
	sysTrapKeySetMask,
	
	sysTrapSelectDay,
	
	sysTrapPrefGetPreference,
	sysTrapPrefSetPreference,
	sysTrapPrefGetAppPreferences,
	sysTrapPrefSetAppPreferences,
	
	sysTrapFrmPointInTitle,
	
	sysTrapStrNCat,
	
	sysTrapMemCmp,
	
	sysTrapTblSetColumnEditIndicator,

	sysTrapFntWordWrap,
	
	sysTrapFldGetScrollValues,
	
	sysTrapSysCreateDataBaseList,
	sysTrapSysCreatePanelList,
	
	sysTrapDlkDispatchRequest,
	
	sysTrapStrPrintF,
	sysTrapStrVPrintF,
	
	sysTrapPrefOpenPreferenceDB,

	sysTrapSysGraffitiReferenceDialog,
	
	sysTrapSysKeyboardDialog,
	
	sysTrapFntWordWrapReverseNLines,
	sysTrapFntGetScrollValues,
	
	sysTrapTblSetRowStaticHeight,
	sysTrapTblHasScrollBar,
	
	sysTrapSclGetScrollBar,
	
	sysTrapFldGetNumberOfBlankLines,
	
	sysTrapSysTicksPerSecond,
	sysTrapHwrBacklight,
	sysTrapDmDatabaseProtect,
	
	sysTrapTblSetBounds,
	
	sysTrapStrNCompare,
	sysTrapStrNCaselessCompare,	
	
	sysTrapPhoneNumberLookup,
	
	sysTrapFrmSetMenu,
	
	sysTrapEncDigestMD5,
	
	sysTrapDmFindSortPosition,
	
	sysTrapSysBinarySearch,
	sysTrapSysErrString,
	sysTrapSysStringByIndex,
	
	sysTrapEvtAddUniqueEventToQueue,
	
	sysTrapStrLocalizeNumber,
	sysTrapStrDelocalizeNumber,
	sysTrapLocGetNumberSeparators,
	
	sysTrapMenuSetActiveMenuRscID,
	
	sysTrapLstScrollList,
	
	sysTrapCategoryInitialize,
	
	sysTrapEncDigestMD4,
	sysTrapEncDES,
	
	sysTrapLstGetVisibleItems,
	
	sysTrapWinSetWindowBounds,
	
	sysTrapCategorySetName,
	
	sysTrapFldSetInsertionPoint,
	
	sysTrapFrmSetObjectBounds,
	
	sysTrapWinSetColors,
	
	sysTrapFlpDispatch,
	sysTrapFlpEmDispatch,
	
	// WARNING!! LEAVE THIS ONE AT THE END AND ALWAYS ADD NEW TRAPS TO
	//  THE END OF THE TRAP TABLE BUT RIGHT BEFORE THIS TRAP!!!!!!!!!
	sysTrapLastTrapNumber
	} SysTrapNumber;
	
#define	sysNumTraps	 (sysTrapLastTrapNumber - sysTrapBase)



//--------------------------------------------------------------------
// Define Library Trap Numbers
//--------------------------------------------------------------------
// Library traps start here and go up by 1's
#define	sysLibTrapBase		0xA800
typedef enum {
	sysLibTrapName = sysLibTrapBase,
	sysLibTrapOpen,
	sysLibTrapClose,
	sysLibTrapSleep,
	sysLibTrapWake,
	sysLibTrapCustom
	} SysLibTrapNumber;



	
/************************************************************
 * Pilot specific TRAP instruction numbers
 *************************************************************/
#define sysDbgBreakpointTrapNum		0		// For soft breakpoints		
#define sysDbgTrapNum					8		// For compiled breakpoints			
#define sysDispatchTrapNum				15		// Trap dispatcher

#ifdef __GNUC__

	#define SYS_TRAP(trapNum) \
	        __attribute__ ((systrap (trapNum)))

#else

	#define SYS_TRAP(trapNum) \
		TWOWORD_INLINE(m68kTrapInstr+sysDispatchTrapNum,trapNum)

#endif	

	
#define ASM_SYS_TRAP(trapNum)	\
			DC.W	m68kTrapInstr+sysDispatchTrapNum; \
			DC.W	trapNum
	

#endif  //__SYSTRAPS_H_


