 /*********************************************************************
 * Copyright (c) 2000-2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *********************************************************************/

 /**	
 @ingroup GSMStatus
 */
 
 
 /** 
 * @file MainInit.c
 * 
 * @brief To initialize the main form for GSMStatus
 *
 * 
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <HsNav.h>      // Include for HsExt.h
#include <HsExt.h>      // Include for HsGetPhoneLibrary()
#include <HsPhone.h>    // Include for Phone library API
#include <HsPhoneNetwork.h>
#include <HsNavCommon.h>
#include <palmOneResources.h>

#include "Common.h"
#include "GSMStatus.h"
#include "GSMStatus_Rsc.h"


/**
*	@name Global variables
*
*/
/*	@{ */

extern UInt16 gLibRef;

/*	@} */


#define MAX_STR 80

void * GetObjectPtr(UInt16 objectID);
void MainFormInit(FormType *frmP, Boolean redraw);

/**
 * @brief GetObjectPtr
 * 			This routine returns a pointer to an object in the current form.
 *
 *
 * @param	objectID id of the form to display
 * @retval	address of object as a void pointer
 *
 *
 * @version This function was added in version 1.0
 **/

void * GetObjectPtr(UInt16 objectID)
{
	FormType * frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
}


/**
 * @brief MainFormInit
 *	     This function initializes the main form 
 *
 *
 * @param frmP   IN:  	Pointer to the main form
 * @param redraw IN:   redraw the main form ?
 * @retval		void	None
 *
 *
 * @version This function was added in version 1.0
 *
 **/
void MainFormInit(FormType *frmP, Boolean redraw)
{
	Err err = errNone;
	
	Int16  bufSize       = 0;
	UInt16 signalQuality = 0;
	UInt32 lac           = 0;
	UInt32 cellId        = 0;
	UInt32 deviceID      = 0;
	
	Char    operatorID[MAX_STR];
	Char    tmp[MAX_STR];
	CharPtr tmpPtr = tmp;
	CharPtr operatorName = NULL;
	CharPtr voicemailNum = NULL;
    PhnPowerType power = phnPowerOff;
    PhnRadioStateType radioState;
  
	FieldType *ConnectionField;
	FieldType *OperatorField;
	FieldType *VoicemailField;
	FieldType *RadioField;
	FieldType *SigQualityField;
	FieldType *MCCField;
	FieldType *MNCField;
	FieldType *LACField;
	FieldType *CellIdField;		
	
	ConnectionField = (FieldType *) GetObjectPtr(MainConnectionField);
	OperatorField = (FieldType *) GetObjectPtr(MainOperatorField);
	VoicemailField = (FieldType *) GetObjectPtr(MainVoicemailField);
	RadioField = (FieldType *) GetObjectPtr(MainRadioField);
	SigQualityField = (FieldType *) GetObjectPtr(MainQualityField);
	MCCField = (FieldType *) GetObjectPtr(MainMCCField);	
	MNCField = (FieldType *) GetObjectPtr(MainMNCField);	
	CellIdField = (FieldType *) GetObjectPtr(MainCellIDField);
	LACField = (FieldType *) GetObjectPtr(MainLACField);
	
    power = PhnLibModulePowered(gLibRef);
    PhnLibGetRadioState(gLibRef, &radioState);
    
    if (power == phnPowerOff)
    {
        DbgMessage ("Radio Off \n");
        SetFieldTextFromStr(RadioField, "OFF", redraw);
        
    }
    else
    {
        DbgMessage ("Radio On \n");
        SetFieldTextFromStr(RadioField, "ON", redraw);
        
    }
    
    /* Checks if the phone found any network cellular service. */
	if (PhnLibRegistered(gLibRef))
	{
		DbgMessage ("connectionfiled OK \n");
		SetFieldTextFromStr(ConnectionField, "OK", redraw);
		
				
		err = PhnLibCurrentOperator(gLibRef, NULL, &operatorName, NULL);
		if (!err)
		{
			DbgMessage ("operatorName OK \n");
			SetFieldTextFromStr(OperatorField, operatorName, redraw);
			
		}
		else // Note: See HsPhoneErrors.h for complete list of telephony error codes
		{	
			DbgMessage ("operatorName Notavailable \n");
			SetFieldTextFromStr(OperatorField, "Retrieving...", redraw);
			
		}
		
		DbgMessage ("Get Voicemail Number OK \n");
		err = PhnLibGetBoxNumber(gLibRef, kBoxVoice, radioState.activeLineNumber, &voicemailNum);
		
		/* If the radio is ready and the voicemail # is available */
		/* There is a known bug in Treo 680 of the PhnLibGetBoxNumber API. You need to check */
		/* both the return value and the voicemail number to make sure you actually retrieve the voicemail#*/
		if ((!err)&&(voicemailNum != NULL))
		{
			DbgMessage ("voiceNum OK \n");
			SetFieldTextFromStr(VoicemailField, voicemailNum, redraw);
			
		}
		else // Note: See HsPhoneErrors.h for complete list of telephony error codes
		{	
			DbgMessage ("voiceNum Notavailable \n");
			SetFieldTextFromStr(VoicemailField, "Retrieving...", redraw);
			
		}	
			
		DbgMessage ("Get OperatorID Number OK \n");
		err = PhnLibCurrentOperatorID(gLibRef, operatorID, &bufSize);
		
		if (!err)
		{
			DbgMessage ("OperatorID OK \n");
			tmp[0] = operatorID[3];
			tmp[1] = operatorID[4];
			tmp[2] = operatorID[5];
			tmp[3] = chrNull;
			operatorID[3] = chrNull;
			SetFieldTextFromStr(MCCField, operatorID, redraw);
			SetFieldTextFromStr(MNCField, tmpPtr, redraw);			
		}
		else // Note: See HsPhoneErrors.h for complete list of telephony error codes
		{	
			DbgMessage ("operatorID Notavailable \n");
			SetFieldTextFromStr(MCCField, "Retrieving...", redraw);
			SetFieldTextFromStr(MNCField, "Retrieving...", redraw);
			
		}	
		
		DbgMessage ("Get SignalQuality OK \n");
		err = PhnLibSignalQuality(gLibRef, &signalQuality);
		if (!err)
		{
			DbgMessage ("SignalQuality OK \n");
			tmpPtr = StrIToA(tmpPtr, signalQuality);
			SetFieldTextFromStr(SigQualityField, tmpPtr, redraw);
			
		}
		else // Note: See HsPhoneErrors.h for complete list of telephony error codes
		{	
			DbgMessage ("SignalQuality Notavailable \n");
			SetFieldTextFromStr(SigQualityField, "Retrieving...", redraw);
			
		}	
		
		/// Cell ID information can only be gotten on Gnome devices
		FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
		if (deviceID == kPalmOneDeviceIDGnome) {
		
			DbgMessage ("Get CellInfo OK \n");
			err = PhnLibGetGSMCellInfo (gLibRef, &lac, &cellId);
			if (!err)
			{
				DbgMessage ("CellInfo OK \n");
				
				// Display cellID
				tmpPtr = StrIToA(tmpPtr, cellId);
				SetFieldTextFromStr(CellIdField, tmpPtr, redraw);
				
				// Display LAC
				tmpPtr = StrIToA(tmpPtr, lac);
				SetFieldTextFromStr(LACField, tmpPtr, redraw);
			}
			else // Note: See HsPhoneErrors.h for complete list of telephony error codes
			{	
				DbgMessage ("CellInfo Notavailable \n");
				SetFieldTextFromStr(CellIdField, "Retrieving...", redraw);
				SetFieldTextFromStr(LACField, "Retrieving...", redraw);
			}	
		} else { // Set cellID and LAC fields to "unsupported" on non-Gnome devices
		   
			SetFieldTextFromStr(CellIdField, "[Unsupported]", redraw);
			SetFieldTextFromStr(LACField, "[Unsupported]", redraw);			
		}	
	}
	else
	{
		DbgMessage ("Service not found \n");
		SetFieldTextFromStr(ConnectionField, "Service not found", redraw);
		DbgMessage ("operator - \n");
		SetFieldTextFromStr(OperatorField, "-", redraw);
		SetFieldTextFromStr(VoicemailField, "-", redraw);
		
	}
	
	if (operatorName) MemPtrFree(operatorName);
	if (voicemailNum) MemPtrFree(voicemailNum);
}

/* EOF *******************************************************************************************/