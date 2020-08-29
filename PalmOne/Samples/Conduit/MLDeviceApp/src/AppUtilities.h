/***********************************************************************
 *
 * FILE:    	Header file for AppUtilities.h
 *
 * HISTORY:		
 *		1.0   jbp  7/13/04	Initial version
 *
 * COPYRIGHT NOTICE:
 *
 *	Copyright © 2006 Palm, Inc. or its subsidiaries.  All rights reserved.
 *	
 *	You may incorporate this sample code (the "Code") into your applications
 *	for Palm OS(R) platform products and may use the Code to develop such 
 *	applications without restriction. The Code is provided to you on an "AS IS" 
 *	basis and the responsibility for its operation is 100% yours.
 *	PALMSOURCE, INC. AND ITS SUBSIDIARIES (COLLECTIVELY, "PALM") DISCLAIM ALL 
 *	WARRANTIES, TERMS AND CONDITIONS WITH RESPECT TO THE CODE, EXPRESS, IMPLIED, 
 *	STATUTORY OR OTHERWISE, INCLUDING WARRANTIES, TERMS OR CONDITIONS OF 
 *	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT AND 
 *	SATISFACTORY QUALITY. You are not permitted to redistribute the Code on a 
 *	stand-alone basis. TO THE FULL EXTENT ALLOWED BY LAW, PALMSOURCE ALSO 
 *	EXCLUDES ANY LIABILITY, WHETHER BASED IN CONTRACT OR TORT (INCLUDING 
 *	NEGLIGENCE), FOR INCIDENTAL, CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE 
 *	DAMAGES OF ANY  KIND, OR FOR LOSS OF REVENUE OR PROFITS, LOSS OF BUSINESS, 
 *	LOSS OF INFORMATION OR DATA, OR OTHER FINANCIAL LOSS ARISING OUT OF OR IN 
 *	CONNECTION WITH THE USE OR PERFORMANCE OF THE CODE.  The Code is subject to 
 *	Restricted Rights for U.S. government users and export regulations.
 *
 ******************************************************************************/
 
 #include <PalmOS.h>

// Assertion macro (#define NO_ASSERT for release builds)
#ifdef NO_ASSERT
	#define ASSERT(condition)	
#else
	#define ASSERT(condition) \
		do {	\
			if (!condition) \
				ErrDisplayFileLineMsg(__FILE__, (UInt16) __LINE__, #condition);	\
		} while (0)
#endif

// Switches for AppCopyTextToField
#define kAppDrawNow				true
#define kAppDrawLater			false

// Switches for AppSetFormFocus
#define	kAppSelectText			true
#define	kAppDontSelectText		false



#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * 
 *	GENERAL FORM AND FORM OBJECT ROUTINES
 *
 ***********************************************************************/
 
// Get a pointer to an object in the specified form.
extern void *AppGetFormObjectPtr(FormType *pForm, UInt16 u16ObjectID);

// Get the current value of a control given the form on which it resides and the
// control's ID.
extern Boolean AppGetControlValue(FormType *pForm, UInt16 u16ControlID);

// Set the current value of a control given the form on which it resides and the
// control's ID. Note that this function should not be used with controls such 
// as sliders that take a numeric value.
extern void AppSetControlValue(FormType *pForm, UInt16 u16ControlID, 
							Boolean fValue);

/***********************************************************************
 * 
 *	TEXT FIELD ROUTINES
 *
 ***********************************************************************/

// Get a pointer to the form object with the current focus.
extern FieldType *AppGetFocusObjectPtr(FormType *pForm);

// Set field object's text handle.	 Will reuse an existing text handle, if any.
extern void AppCopyTextToField(FormType *pForm, UInt16 uwFieldID, 
							char *pNewText, Boolean fDrawNow);

// Obtain a pointer to a field's text.
extern Char * AppGetFieldTextPtr(FormType *pForm, UInt16 u16FieldID);

// Scroll a text field with a scrollbar.
extern void AppUpdateScrollbar(FormType *pForm, UInt16 u16FieldID, 
								UInt16 u16ScrollID);
								
// Handle all events relating to the scrolling of multi-line text fields with
// scroll bars.
extern Boolean AppHandleFieldScrollingEvents(EventType *pEvent, 
							UInt16 u16FieldID, UInt16 u16ScrollbarID);

// Set the form's focus to a specified field, and optionally highlight the field
// contents.
extern void AppSetFormFocus(FormType *pForm, UInt16 u16FieldID, 
							Boolean fHiliteText);

// Determine whether a text field is empty of characters.
extern Boolean AppFieldIsEmpty(FormType *pForm, UInt16 u16FieldID);

// Create a list of index values, one for each of the editable text fields on a 
// specified form, sorted by position (top to bottom, left to right). This 
// function will optionally set the focus to the first (in sorted order) field 
// on the form. 
extern void AppInitTabList(FormType* pForm, UInt16 uwTabGadgetID,
							Boolean fSetFocus);

// Handle tab (and tab-like) keys, moving focus between a form's fields as 
// appropriate. Optionally select the contents of the field receiving the focus 
// (otherwise, the insertion point is placed at the end of the field's text).
extern Boolean AppHandleTabKeyEvents(EventType* pEvent, UInt16 u16TabGadgetID, 
														Boolean fSelectText);

// Set the focus to the first field in the tab list.
extern void AppSetFirstTabFocus(FormType* pForm, UInt16 u16TabGadgetID);

/***********************************************************************
 * 
 *	GADGET ROUTINES
 *
 ***********************************************************************/

//Get the value stored in the data field of a gadget object. 
extern MemPtr AppGetGadgetData(FormType *pForm, UInt16 u16GadgetID);

// Free a gadget's data block.
extern void AppFreeGadgetData(FormType *pForm, UInt16 u16GadgetID);

/***********************************************************************
 * 
 *	GET APPLICATION VERSION FROM RESOURCE
 *
 ***********************************************************************/
 
 // Get application's version string from its resource database.
extern Err AppGetAppVersionStr(char *pBuff, UInt16 u16BuffLen);

/***********************************************************************
 * 
 *	DATABASE ROUTINES
 *
 ***********************************************************************/

// Set a database's attribute bits. This function is particularly useful when
// setting the database's backup bit.
extern void AppSetDatabaseAttributes(DmOpenRef dbRef, UInt16 u16NewAttr);

/***********************************************************************
 * 
 *	STRING-HANDLING ROUTINES
 *
 ***********************************************************************/

// Convert a double to a string (poor man's sprintf()). The generated string is
// null-terminated. 
extern void AppDoubleToStr(char *strBuffer, double dblNumber, 
									UInt16 u16NumFractDigits);

// Copies a source string to a destination string, until the end of either
// string is reached or a specified maximum number of characters have been copied.
extern UInt16 AppStrLCopy(char *pDstStr, const char *pSrcStr, UInt16 u16Size);

// Appends a source string to a destination string, until the end of the source string is reached or the destination buffer becomes full.
extern UInt16 AppStrLCat(char *pDstStr, const char *pSrcStr, UInt16 u16Size);

/***********************************************************************
 * 
 *	DATE & TIME ROUTINES
 *
 ***********************************************************************/

// Get current data and time in a DateTimeType struct.
extern void AppGetCurrentDateTime(DateTimeType *pDt);

// Create a date string from the current date based on the user's prefs.
extern void AppGetCurrentDateStr(char *pStrBuffer, Boolean fUseLongFormat);

// Create a time string from the current time based on the user's prefs.
extern void AppGetCurrentTimeStr(char *pStrBuffer);

/***********************************************************************
 * 
 *	EXCHANGE MANAGER ROUTINES
 *
 ***********************************************************************/

// Given a database ID, send the database using the Exchange Manager.
extern Err AppSendDatabase(LocalID dbID, Char *pName, Char *pDescription);


#ifdef __cplusplus 
}
#endif
