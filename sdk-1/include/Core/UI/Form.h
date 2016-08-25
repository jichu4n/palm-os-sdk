/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1994 -- All Rights Reserved
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
	frmGadgetObj };
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


//--------------------------------------------------------------------
//
// Form Function
//
//--------------------------------------------------------------------

extern FormPtr FrmInitForm (Word rscID)
							SYS_TRAP(sysTrapFrmInitForm);

extern void FrmDeleteForm (FormPtr frm)
							SYS_TRAP(sysTrapFrmDeleteForm);

extern void FrmDrawForm (FormPtr frm)
							SYS_TRAP(sysTrapFrmDrawForm);

extern void FrmEraseForm (FormPtr frm)
							SYS_TRAP(sysTrapFrmEraseForm);

extern FormPtr FrmGetActiveForm (void)
							SYS_TRAP(sysTrapFrmGetActiveForm);

extern void FrmSetActiveForm (FormPtr frm)
							SYS_TRAP(sysTrapFrmSetActiveForm);

extern Word FrmGetActiveFormID (void)
							SYS_TRAP(sysTrapFrmGetActiveFormID);

extern Boolean FrmGetUserModifiedState (FormPtr frm)
							SYS_TRAP(sysTrapFrmGetUserModifiedState);

extern void FrmSetNotUserModified (FormPtr frm)
							SYS_TRAP(sysTrapFrmSetNotUserModified);

extern Word FrmGetFocus (FormPtr frm)
							SYS_TRAP(sysTrapFrmGetFocus);
            
extern void FrmSetFocus (FormPtr frm, Word fieldIndex)
							SYS_TRAP(sysTrapFrmSetFocus);

extern Boolean FrmHandleEvent (FormPtr frm, EventPtr eventP)
							SYS_TRAP(sysTrapFrmHandleEvent);

extern void FrmGetFormBounds (FormPtr frm, RectanglePtr r)
							SYS_TRAP(sysTrapFrmGetFormBounds);

extern WinHandle FrmGetWindowHandle (FormPtr frm)
							SYS_TRAP(sysTrapFrmGetWindowHandle);

extern Word FrmGetFormId (FormPtr frm)
							SYS_TRAP(sysTrapFrmGetFormId);

extern FormPtr FrmGetFormPtr (Word formId)
							SYS_TRAP(sysTrapFrmGetFormPtr);

extern FormPtr FrmGetFirstForm (void)
							SYS_TRAP(sysTrapFrmGetFirstForm);

extern Word FrmGetNumberOfObjects (FormPtr frm)
							SYS_TRAP(sysTrapFrmGetNumberOfObjects);

extern Word FrmGetObjectIndex (FormPtr frm, Word objID)
							SYS_TRAP(sysTrapFrmGetObjectIndex);

extern Word FrmGetObjectId (FormPtr, Word objIndex)
							SYS_TRAP(sysTrapFrmGetObjectId);

extern FormObjectKind FrmGetObjectType (FormPtr frm, Word objIndex)
							SYS_TRAP(sysTrapFrmGetObjectType);

extern void * FrmGetObjectPtr (FormPtr frm, Word objIndex)
							SYS_TRAP(sysTrapFrmGetObjectPtr);

extern void FrmGetObjectBounds (FormPtr frm, Word pObjIndex, RectanglePtr r)
							SYS_TRAP(sysTrapFrmGetObjectBounds);

extern void FrmHideObject (FormPtr frm, Word objIndex)
							SYS_TRAP(sysTrapFrmHideObject);

extern void FrmShowObject (FormPtr frm, Word objIndex)
							SYS_TRAP(sysTrapFrmShowObject);

extern void FrmGetObjectPosition (FormPtr frm, Word objIndex, 
	SWordPtr x, SWordPtr y)
							SYS_TRAP(sysTrapFrmGetObjectPosition);

extern void FrmSetObjectPositon (FormPtr frm, Word objIndex, 
	SWord x, SWord y)
							SYS_TRAP(sysTrapFrmSetObjectPositon);




extern SWord FrmGetControlValue (FormPtr frm, Word controlID)
							SYS_TRAP(sysTrapFrmGetControlValue);

extern void FrmSetControlValue (FormPtr frm, Word controlID, SWord newValue)
							SYS_TRAP(sysTrapFrmSetControlValue);

extern Byte FrmGetControlGroupSelection (FormPtr frm, Byte groupNum)
							SYS_TRAP(sysTrapFrmGetControlGroupSelection);

extern void FrmSetControlGroupSelection (FormPtr frm, Byte groupNum, 
	Word controlID)
							SYS_TRAP(sysTrapFrmSetControlGroupSelection);

extern void FrmCopyLabel (FormPtr frm, Word lableID, CharPtr newLable)
							SYS_TRAP(sysTrapFrmCopyLabel);

extern CharPtr FrmGetLabel (FormPtr frm, Word lableID)
							SYS_TRAP(sysTrapFrmGetLabel);

extern void FrmSetCategoryLabel (FormPtr frm, Word objIndex, CharPtr newLabel)
							SYS_TRAP(sysTrapFrmSetCategoryLabel);

extern CharPtr FrmGetTitle (FormPtr frm)
							SYS_TRAP(sysTrapFrmGetTitle);

extern void FrmSetTitle (FormPtr frm, CharPtr newTitle)
							SYS_TRAP(sysTrapFrmSetTitle);

extern void FrmCopyTitle (FormPtr frm, CharPtr newTitle)
							SYS_TRAP(sysTrapFrmCopyTitle);

extern VoidPtr FrmGetGadgetData (FormPtr frm, Word objIndex)
							SYS_TRAP(sysTrapFrmGetGadgetData);

extern void FrmSetGadgetData (FormPtr frm, Word objIndex, VoidPtr data)
							SYS_TRAP(sysTrapFrmSetGadgetData);



extern Word FrmDoDialog (FormPtr frm)
							SYS_TRAP(sysTrapFrmDoDialog);

extern Word FrmAlert (Word  alertId)
							SYS_TRAP(sysTrapFrmAlert);
							
extern Word FrmCustomAlert (Word  alertId, CharPtr s1, CharPtr s2, CharPtr s3)
							SYS_TRAP(sysTrapFrmCustomAlert);

extern void FrmHelp (Word helpMsgId)
							SYS_TRAP(sysTrapFrmHelp);

extern void FrmUpdateScrollers (FormPtr frm, Word upIndex, Word downIndex,
	Boolean scrollableUp, Boolean scrollabledown)
							SYS_TRAP(sysTrapFrmUpdateScrollers);

extern Boolean FrmVisible (FormPtr frm)
							SYS_TRAP(sysTrapFrmVisible);

extern void FrmSetEventHandler (FormPtr frm, FormEventHandlerPtr)
							SYS_TRAP(sysTrapFrmSetEventHandler);

extern Boolean FrmDispatchEvent (EventPtr eventP)
							SYS_TRAP(sysTrapFrmDispatchEvent);




extern void FrmPopupForm (Word formId)
							SYS_TRAP(sysTrapFrmPopupForm);

extern void FrmGotoForm (Word formId)
							SYS_TRAP(sysTrapFrmGotoForm);

extern void FrmUpdateForm (Word formId, Word updateCode)
							SYS_TRAP(sysTrapFrmUpdateForm);
							
extern void FrmReturnToForm  (Word formId)
							SYS_TRAP(sysTrapFrmReturnToForm);
							
extern void FrmCloseAllForms (void)
							SYS_TRAP(sysTrapFrmCloseAllForms);

extern void FrmSaveAllForms (void)
							SYS_TRAP(sysTrapFrmSaveAllForms);



#endif __FORM_H__
