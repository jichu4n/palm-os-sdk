/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Form.h
 * AUTHOR:	 Art Lamb: September 6, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines dialog box structures and routines.
 *
 ***********************************************************************/

#ifndef __FORM_H__
#define __FORM_H__

#define noFocus 0xffff

// Update code send as part of a frmUpdate event.
#define frmRedrawUpdateCode			0x8000


// Alert constants and structures
enum alertTypes {
	informationAlert,
	confirmationAlert,
	warningAlert,
	errorAlert };
typedef enum alertTypes AlertType;

typedef struct {
	Word		alertType;
	Word		helpRscID;
	Word		numButtons;
	Word		defaultButton;
} AlertTemplateType;


// Types of object in a dialog box
enum formObjects {
	frmFieldObj,
	frmControlObj,
	frmListObj,
	frmTableObj,
	frmBitmapObj,
	frmLineObj,
	frmFrameObj,
	frmRectangleObj,
	frmLabelObj,
	frmTitleObj,
	frmPopupObj,
	frmGraffitiStateObj,
	frmGadgetObj,
	frmScrollBarObj };
typedef enum formObjects FormObjectKind;
	

typedef struct {
	Word usable			:1;	// Set if part of ui 
} FormObjAttrType;


typedef struct {
	FormObjAttrType			attr;
	PointType					pos;
	Word		     				rscID;
} FormBitmapType;


typedef struct {
	FormObjAttrType			attr;
	PointType	      		point1;	   
	PointType	      		point2;	   
} FormLineType;


typedef struct {
	Word							id;
	FormObjAttrType			attr;
	RectangleType				rect;
	Word     					frameType;
} FormFrameType;


typedef struct {
	FormObjAttrType			attr;
	RectangleType				rect;
} FormRectangleType;


typedef struct {
	Word							id;
	PointType					pos;
	FormObjAttrType			attr;
	FontID						fontID;
	Char *						text;
} FormLabelType;


typedef struct {
	RectangleType				rect;
	Char *						text;
} FormTitleType;


typedef struct {
	Word							controlID;
	Word							listID;
} FormPopupType;


typedef struct {
	PointType					pos;
} FrmGraffitiStateType;


typedef struct {
	Word				id;
	FormObjAttrType			attr;
	RectangleType				rect;
	VoidPtr			     		data;
} FormGadgetType;


typedef union {
	void *                  ptr;
	FieldType *					field;
	ControlType *				control;
	ListType	*					list;
	TableType *					table;
	FormBitmapType *			bitmap;
//	FormLineType *				line;
//	FormFrameType *			frame;
//	FormRectangleType *		rectangle;
	FormLabelType *			label;
	FormTitleType *			title;	
	FormPopupType *			popup;
	FrmGraffitiStateType *  grfState;
	FormGadgetType *			gadget;
	ScrollBarType *			scrollBar;
} FormObjectType;

// typedef FormObjectType * FormObjectPtr;


typedef struct {
	FormObjectKind				objectType;
	FormObjectType				object;
} FormObjListType;


typedef struct {
	Word usable				:1;	// Set if part of ui 
	Word enabled			:1;	// Set if interactable (not grayed out)
	Word visible			:1;	// Set if drawn, used internally
	Word dirty				:1;	// Set if dialog has been modified
	Word saveBehind		:1;	// Set if bits behind form are save when form ids drawn
	Word graffitiShift	:1;   // Set if graffiti shift indicator is supported
	Word reserved			:11;
} FormAttrType;


typedef Boolean FormEventHandlerType (EventPtr eventP);

typedef FormEventHandlerType * FormEventHandlerPtr;

typedef struct {
	WindowType					window;
	Word							formId;
   FormAttrType				attr;
	WinHandle	       		bitsBehindForm;
	FormEventHandlerPtr		handler;
	Word							focus;
	Word							defaultButton;
	Word							helpRscId;
	Word							menuRscId;
	Word							numObjects;
	FormObjListType *			objects;
} FormType;


typedef FormType * FormPtr;


// FormActiveStateType: this structure is passed to FrmActiveState for
// saving and restoring active form/window state; this structure's
// contents are abstracted because the contents will differ significantly
// as PalmOS evolves
// Added for PalmOS 3.0
typedef struct FormActiveStateType {
	Word	data[11];
	} FormActiveStateType;


//-----------------------------------------------
//  Macros
//-----------------------------------------------

#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL
#define ECFrmValidatePtr(frm) FrmValidatePtr(frm)
#else
#define ECFrmValidatePtr(frm) 
#endif

//--------------------------------------------------------------------
//
// Form Function
//
//--------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern FormPtr FrmInitForm (const Word rscID)
							SYS_TRAP(sysTrapFrmInitForm);

extern void FrmDeleteForm (const FormPtr frm)
							SYS_TRAP(sysTrapFrmDeleteForm);

extern void FrmDrawForm (const FormPtr frm)
							SYS_TRAP(sysTrapFrmDrawForm);

extern void FrmEraseForm (const FormPtr frm)
							SYS_TRAP(sysTrapFrmEraseForm);

extern FormPtr FrmGetActiveForm (void)
							SYS_TRAP(sysTrapFrmGetActiveForm);

extern void FrmSetActiveForm (const FormPtr frm)
							SYS_TRAP(sysTrapFrmSetActiveForm);

extern Word FrmGetActiveFormID (void)
							SYS_TRAP(sysTrapFrmGetActiveFormID);

extern Boolean FrmGetUserModifiedState (const FormPtr frm)
							SYS_TRAP(sysTrapFrmGetUserModifiedState);

extern void FrmSetNotUserModified (const FormPtr frm)
							SYS_TRAP(sysTrapFrmSetNotUserModified);

extern Word FrmGetFocus (const FormPtr frm)
							SYS_TRAP(sysTrapFrmGetFocus);
            
extern void FrmSetFocus (const FormPtr frm, const Word fieldIndex)
							SYS_TRAP(sysTrapFrmSetFocus);

extern Boolean FrmHandleEvent (const FormPtr frm, const EventPtr eventP)
							SYS_TRAP(sysTrapFrmHandleEvent);

extern void FrmGetFormBounds (const FormPtr frm, const RectanglePtr r)
							SYS_TRAP(sysTrapFrmGetFormBounds);

extern WinHandle FrmGetWindowHandle (const FormPtr frm)
							SYS_TRAP(sysTrapFrmGetWindowHandle);

extern Word FrmGetFormId (const FormPtr frm)
							SYS_TRAP(sysTrapFrmGetFormId);

extern FormPtr FrmGetFormPtr (const Word formId)
							SYS_TRAP(sysTrapFrmGetFormPtr);

extern FormPtr FrmGetFirstForm (void)
							SYS_TRAP(sysTrapFrmGetFirstForm);

extern Word FrmGetNumberOfObjects (const FormPtr frm)
							SYS_TRAP(sysTrapFrmGetNumberOfObjects);

extern Word FrmGetObjectIndex (const FormPtr frm, const Word objID)
							SYS_TRAP(sysTrapFrmGetObjectIndex);

extern Word FrmGetObjectId (const FormPtr, const Word objIndex)
							SYS_TRAP(sysTrapFrmGetObjectId);

extern FormObjectKind FrmGetObjectType (const FormPtr frm, const Word objIndex)
							SYS_TRAP(sysTrapFrmGetObjectType);

extern VoidPtr FrmGetObjectPtr (const FormPtr frm, const Word objIndex)
							SYS_TRAP(sysTrapFrmGetObjectPtr);

extern void FrmGetObjectBounds (const FormPtr frm, const Word pObjIndex, 
	const RectanglePtr r)
							SYS_TRAP(sysTrapFrmGetObjectBounds);

extern void FrmHideObject (const FormPtr frm, const Word objIndex)
							SYS_TRAP(sysTrapFrmHideObject);

extern void FrmShowObject (const FormPtr frm, const Word objIndex)
							SYS_TRAP(sysTrapFrmShowObject);

extern void FrmGetObjectPosition (const FormPtr frm, const Word objIndex, 
	const SWordPtr x, const SWordPtr y)
							SYS_TRAP(sysTrapFrmGetObjectPosition);

extern void FrmSetObjectPosition (const FormPtr frm, const Word objIndex, 
	const SWord x, const SWord y)
							SYS_TRAP(sysTrapFrmSetObjectPosition);

extern void FrmSetObjectBounds (const FormPtr frm, const Word objIndex, 
	const RectanglePtr bounds)
							SYS_TRAP(sysTrapFrmSetObjectBounds);



extern SWord FrmGetControlValue (const FormPtr frm, const Word controlID)
							SYS_TRAP(sysTrapFrmGetControlValue);

extern void FrmSetControlValue (const FormPtr frm, const Word controlID, 
	SWord newValue)
							SYS_TRAP(sysTrapFrmSetControlValue);

extern Byte FrmGetControlGroupSelection (const FormPtr frm, 
	const Byte groupNum)
							SYS_TRAP(sysTrapFrmGetControlGroupSelection);

extern void FrmSetControlGroupSelection (const FormPtr frm, 
	const Byte groupNum, const Word controlID)
							SYS_TRAP(sysTrapFrmSetControlGroupSelection);

extern void FrmCopyLabel (const FormPtr frm, const Word lableID, 
	const CharPtr newLable)
							SYS_TRAP(sysTrapFrmCopyLabel);

extern CharPtr FrmGetLabel (const FormPtr frm, const Word lableID)
							SYS_TRAP(sysTrapFrmGetLabel);

extern void FrmSetCategoryLabel (const FormPtr frm, const Word objIndex, 
	const CharPtr newLabel)
							SYS_TRAP(sysTrapFrmSetCategoryLabel);

extern CharPtr FrmGetTitle (const FormPtr frm)
							SYS_TRAP(sysTrapFrmGetTitle);

extern void FrmSetTitle (const FormPtr frm, const CharPtr newTitle)
							SYS_TRAP(sysTrapFrmSetTitle);

extern void FrmCopyTitle (const FormPtr frm, const CharPtr newTitle)
							SYS_TRAP(sysTrapFrmCopyTitle);

extern VoidPtr FrmGetGadgetData (const FormPtr frm, const Word objIndex)
							SYS_TRAP(sysTrapFrmGetGadgetData);

extern void FrmSetGadgetData (const FormPtr frm, const Word objIndex, 
	const VoidPtr data)
							SYS_TRAP(sysTrapFrmSetGadgetData);



extern Word FrmDoDialog (const FormPtr frm)
							SYS_TRAP(sysTrapFrmDoDialog);

extern Word FrmAlert (const Word  alertId)
							SYS_TRAP(sysTrapFrmAlert);
							
extern Word FrmCustomAlert (const Word  alertId, const CharPtr s1, 
	const CharPtr s2, const CharPtr s3)
							SYS_TRAP(sysTrapFrmCustomAlert);

extern void FrmHelp (const Word helpMsgId)
							SYS_TRAP(sysTrapFrmHelp);

extern void FrmUpdateScrollers (const FormPtr frm, const Word upIndex, 
	const Word downIndex, const Boolean scrollableUp,
	const Boolean scrollabledown)
							SYS_TRAP(sysTrapFrmUpdateScrollers);

extern Boolean FrmVisible (const FormPtr frm)
							SYS_TRAP(sysTrapFrmVisible);

extern void FrmSetEventHandler (const FormPtr frm, const FormEventHandlerPtr)
							SYS_TRAP(sysTrapFrmSetEventHandler);

extern Boolean FrmDispatchEvent (const EventPtr eventP)
							SYS_TRAP(sysTrapFrmDispatchEvent);




extern void FrmPopupForm (const Word formId)
							SYS_TRAP(sysTrapFrmPopupForm);

extern void FrmGotoForm (const Word formId)
							SYS_TRAP(sysTrapFrmGotoForm);

extern void FrmUpdateForm (const Word formId, const Word updateCode)
							SYS_TRAP(sysTrapFrmUpdateForm);
							
extern void FrmReturnToForm  (const Word formId)
							SYS_TRAP(sysTrapFrmReturnToForm);
							
extern void FrmCloseAllForms (void)
							SYS_TRAP(sysTrapFrmCloseAllForms);

extern void FrmSaveAllForms (void)
							SYS_TRAP(sysTrapFrmSaveAllForms);



extern Boolean FrmPointInTitle (const FormPtr frm, const Short x, 
	const Short y)
							SYS_TRAP(sysTrapFrmPointInTitle);

extern void FrmSetMenu (const FormPtr frm, const Word menuRscID)
							SYS_TRAP(sysTrapFrmSetMenu);

extern Boolean FrmValidatePtr (FormPtr frm)
							SYS_TRAP(sysTrapFrmValidatePtr);

extern Err FrmAddSpaceForObject (FormPtr *formPP, Ptr *objectPP, 
	FormObjectKind objectKind, Word objectSize)
							SYS_TRAP(sysTrapFrmAddSpaceForObject);

extern Err FrmRemoveObject (FormPtr *formPP, Word objIndex)
							SYS_TRAP(sysTrapFrmRemoveObject);

extern FormPtr FrmNewForm (const Word formID, const Char * const titleStrP, 
	Word x, Word y, Word width, Word height, const Boolean modal, 
	const Word defaultButton, const Word helpRscID, const Word menuRscID)
							SYS_TRAP(sysTrapFrmNewForm);

extern FormLabelType * FrmNewLabel (VoidPtr *formPP, const Word ID, const Char * const textP, 
	const Word x, const Word y, const FontID font)
							SYS_TRAP(sysTrapFrmNewLabel);

extern FormBitmapType * FrmNewBitmap (VoidPtr *formPP, const Word ID, 
	const Word rscID, const Word x, const Word y)
							SYS_TRAP(sysTrapFrmNewBitmap);

extern FormGadgetType * FrmNewGadget (VoidPtr *formPP, const Word id, 
	const Word x, const Word y, const Word width, const Word height)
							SYS_TRAP(sysTrapFrmNewGadget);

extern Err FrmActiveState (FormActiveStateType* stateP, Boolean save)
							SYS_TRAP(sysTrapFrmActiveState);

#define FrmSaveActiveState(stateP)			FrmActiveState(stateP, true)
#define FrmRestoreActiveState(stateP)		FrmActiveState(stateP, false)



#ifdef __cplusplus 
}
#endif

#endif __FORM_H__
