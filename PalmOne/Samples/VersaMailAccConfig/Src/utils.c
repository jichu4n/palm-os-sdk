/******************************************************************************
 * Copyright (c) 2004-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup VersaMailAccConfig
 */
 
/**
 *  @brief 	Sample code for the VersaMail Setup Applications
 *          
 *
 *
 *  @version 1.0	- Initial Revision (08/21/03)
 *  @version 1.1	- Added "About" box and 5-way nav support (02/20/07),compiles under PODS and GCC
 *
 *  @file utils.c
 */


#include <PalmOS.h> 
#include "utils.h"
#include "VMAccConfig_Rsc.h"

extern Char *gKeys[NUM_KEYS];
extern Char **gValues;
extern Boolean gFoundDB;

Boolean gWriteSAM = false;

/***********************************************************************
 * FUNCTION:    	PrvUpdateList
 *
 * DESCRIPTION: 	Updates the list of values, given the pointer to the field
 *					from which to grab the value from
 *
 * PARAMETERS:  	fldP -> Pointer to the field
 *					key -> Denotes the key to which the value corresponds to
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/

static void PrvUpdateValueList(FieldType *fldP, UInt8 key)
{
	Char *fieldString = NULL;
	
	
	// If the field has been modified
	if(FldDirty(fldP))
	{
		// Get the pointer to the text in the field
		fieldString = FldGetTextPtr(fldP);
		
		// Only if the user has entered any info, will it be used
		if(StrLen(fieldString))
		{
			if(gValues[key])
			{
				MemPtrFree(gValues[key]);
				gValues[key] = NULL;
			}			

			gValues[key] = MemPtrNew(StrLen(fieldString) + 1);
			StrCopy(gValues[key], fieldString);
			
		}
		//if the field is empty
		else
		{
			if(gValues[key])
			{
				MemPtrFree(gValues[key]);
				gValues[key] = NULL;
			}
		}
	}
}			

/***********************************************************************
 * FUNCTION:    	PrvGenerateRecord
 *
 * DESCRIPTION: 	Generates a record according to the given syntax:
 *					which is "Set key accountSlot value"
 *
 * PARAMETERS:  	record -> String which will contain the record
 *					key -> String which will contain the key
 *					accSlotString -> String which contains the account slot
 *					value -> String which contains a value associated with the 
 *								key
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/

static void PrvGenerateRecord(Char *record, Char *key, Char *accSlotString, Char *value)
{
	StrCopy(record, "Set ");

	StrCat(record, key);
	StrCat(record, " ");

	StrCat(record, accSlotString);
	StrCat(record, " ");

	StrCat(record, value);
}

/***********************************************************************
 *
 * FUNCTION:    	SetFieldText
 *
 * DESCRIPTION: 	Sets a given text pointer as the text of a field	
 *
 * PARAMETERS:  	fieldP -> Pointer to the field to be drawn
 *					textP -> Pointer to the text that has to be drawn	
 *					blDraw -> Flag which will draw the text on the field		
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void	SetFieldText(FieldType *fieldP,Char *textP,Boolean blDraw)
{	
	MemHandle	FieldTextH = NULL;
	Char	*fieldText = NULL;
	Int16		textSize = 0;
	FieldTextH = FldGetTextHandle(fieldP);

	if(!FieldTextH)
	{
		FieldTextH=MemHandleNew(StrLen(textP));
		if(!FieldTextH)
			return;
	}
	
	FldSetTextHandle (fieldP, NULL);
	textSize=StrLen(textP) + 10;
	
	if(MemHandleResize(FieldTextH,textSize)==0) {
		fieldText=(Char *)MemHandleLock(FieldTextH);
	
		StrCopy(fieldText,textP);
		MemHandleUnlock(FieldTextH);
		FldSetTextHandle (fieldP, FieldTextH);
	}
	
	if(blDraw)
		FldDrawField(fieldP);
		
}

/***********************************************************************
 *
 * FUNCTION:    	FindMMDeviceDB
 *
 * DESCRIPTION: 	Finds the __MMDevice database 	
 *
 * PARAMETERS:  	localIDP -> Pointer to ID of the database
 *					errP -> Pointer to the error generated
 *
 * RETURNED:    	foundDB -> Whether the database has been found or not
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Boolean FindMMDeviceDB(LocalID *localIDP, Err *errP)
{
	Boolean 	foundDB = false;
	UInt16 		version = 0x1003;
	
	*localIDP = DmFindDatabase(0, "__MMDevice");
	
	if(*localIDP)
		foundDB = true;
	else
	{
		
		*errP = DmCreateDatabase(0,
							"__MMDevice",
							'asc3', 'asc3',
							false);
							
		*localIDP = DmFindDatabase(0, "__MMDevice");
		
		
		*errP = DmSetDatabaseInfo(0, *localIDP,
						NULL, NULL,
						&version, NULL,
						NULL, NULL,
						NULL, NULL,
						NULL, NULL,
						NULL);
	
		foundDB = true;
	}

	return foundDB;
}	

/***********************************************************************
 *
 * FUNCTION:    	ParseMMDeviceValues
 *
 * DESCRIPTION: 	Parses a given database to find a particular value,
 *					associated with a given key. If the deleteFlag is set, 
 *					it deletes corresponding records	
 *
 * PARAMETERS:  	localID -> ID of the database
 *					accSlot -> Account Slot for which the database must be 
 *								parsed
 *					deleteFlag -> If the delete flag is set, the corresponding
 *								record is deleted
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void ParseMMDeviceValues(LocalID localID, UInt16 accSlot, Boolean deleteFlag)
{	
	DmOpenRef	mmDeviceP = NULL;
	MemHandle 	recordH = NULL;
	
	UInt16 	count = 0;
	Int16	recCount = 0;
	UInt16	accSlotIndex = 0;
	UInt32	actualAccSlot = 1;
	UInt16 	numRecords = 0;
	
	Char 	*element = NULL;
	Char	*elementInLower = NULL;
	Char	*elementIndexP = NULL;
	Char 	*accSlotString = "";
	
	Err err = errNone;
	
	// Open the database
	mmDeviceP = DmOpenDatabase(0, localID, dmModeReadWrite);
	
	// Number of records in the DB
	// numRecords = DmNumRecords(mmDeviceP);
	numRecords = DmNumRecords(mmDeviceP);
	  
	// For each record, find out what the key is and store the value 
	// associated with the key
	for(recCount = numRecords - 1; recCount >= 0; recCount--)
	{
		accSlotIndex = 0;
		elementIndexP = NULL;
		
		// Get the record
		recordH = DmGetRecord(mmDeviceP, recCount);
		err = DmGetLastErr();
		if(recordH)
		{
			// Lock the handle, get the pointer
			element = (Char *)MemHandleLock(recordH);
			// Allocate memory on the dynamic heap for the lower-case version
			// of the record
			elementInLower = (Char *)MemPtrNew(StrLen(element) + 1);
			// Convert element to lower case
			elementInLower = (Char *)StrToLower(elementInLower, element);
		}
		
		// Find out if any of the given keys are present
		if(elementInLower)
			for(count = 0; count < NUM_KEYS; count++)
			{
				elementIndexP = (Char *)StrStr(elementInLower, gKeys[count]);				
				if(elementIndexP)
					break;
			}

		// If key is found
		if(elementIndexP)
		{
			// Move the point to after the key
			elementIndexP += StrLen(gKeys[count]);

			// Remove any white characters
			while(*elementIndexP == ' ')
				elementIndexP++;
			
			// To find out the account slot associated with this record
			accSlotString = elementIndexP;
			
			do
			{
				elementIndexP++;
				accSlotIndex++;
			
				if(*elementIndexP == '\0')
					break;
			} while(*elementIndexP != ' ');
					
			elementIndexP++;
			accSlotIndex++;
			
			elementIndexP = accSlotString;
			
			StrNCopy(accSlotString, elementIndexP, accSlotIndex - 1);
			
			// Account slot is found
			actualAccSlot = StrAToI(accSlotString);
			
			// Move the pointer to find the value associated with this key
			// and this account slot
			elementIndexP += accSlotIndex;
			
			// If the account slot is the same as the one requested,
			// store the value in the array gValues
			if(actualAccSlot == accSlot)
			{
				if(deleteFlag)
				{
					MemHandleUnlock(recordH);
					DmReleaseRecord(mmDeviceP, recCount, false);
					err = DmRemoveRecord(mmDeviceP, recCount);
				}
				else
				{
					UInt16 length = (UInt16)(elementIndexP - elementInLower); 
					
					if(length < StrLen(element))
					{
						element += length;
						gValues[count] = MemPtrNew(StrLen(element) + 1);
						MemSet(gValues[count], StrLen(element) + 1, 0);
						StrCopy(gValues[count], element);
					}
				}
			}
		}	
	
		// Free the memory for the lower-case record
		if(elementInLower)
		{
			MemPtrFree(elementInLower);
			elementInLower = NULL;
		}
		
		if(!deleteFlag)
		{
			// Unlock the handle
			MemHandleUnlock(recordH);
			// Release the record
			DmReleaseRecord(mmDeviceP, recCount, false);
		}
	}
	
	// Close the database
	DmCloseDatabase(mmDeviceP);		
}

/***********************************************************************
 *
 * FUNCTION:    	FillInFields
 *
 * DESCRIPTION: 	Draws fields with values corresponding to the different
 *					keys, 	
 *
 * PARAMETERS:  	displayFlag -> Denotes the form in which the fields must
 *									be redrawn
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void FillInFields(UInt8 displayFlag)
{
	// Fields in the main form
	FieldType *fldUserName = NULL;
	FieldType *fldIncomingServer = NULL;
	FieldType *fldPassword = NULL;
	FieldType *fldEmailAddress = NULL;	
	FieldType *fldOutgoingServer = NULL;
	FieldType *fldTitle = NULL;
	
	// Fields in the second form
	FieldType *fldSamId = NULL;
	FieldType *fldIncomingPort = NULL;
	FieldType *fldRootMailbox = NULL;
	FieldType *fldOutgoingPort = NULL;
	FieldType *fldReplyTo = NULL;
	
	// Fields in the fourth form
	FieldType *fldApn = NULL;
	FieldType *fldUseEncryptPassword = NULL;
	FieldType *fldUseSmtp = NULL;
	FieldType *fldUseIncomingSSL = NULL;
	FieldType *fldUseOutgoingSSL = NULL;
	
	FormType  *frmP = NULL;
	
	frmP = FrmGetActiveForm();

	switch(displayFlag)
	{
		case FIRSTPAGE:

			// Populates fields in the first form
			fldUserName = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainUsernameField));
			fldIncomingServer = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainIncomingServerField));
			fldPassword = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainPasswordField));
			fldEmailAddress = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainEmailAddressField));
			fldOutgoingServer = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainOutgoingServerField));
			fldTitle = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainTitleField));

			if(gValues[USERNAME])
				SetFieldText(fldUserName, gValues[USERNAME], true);
	
			if(gValues[INCOMINGSERVER])
				SetFieldText(fldIncomingServer, gValues[INCOMINGSERVER], true);

			if(gValues[PASSWORD])
				SetFieldText(fldPassword, gValues[PASSWORD], true);

			if(gValues[EMAILADDRESS])
				SetFieldText(fldEmailAddress, gValues[EMAILADDRESS], true);

			if(gValues[OUTGOINGSERVER])
				SetFieldText(fldOutgoingServer, gValues[OUTGOINGSERVER], true);

			if(gValues[TITLE])
				SetFieldText(fldTitle, gValues[TITLE], true);
			
			break;
	
		case SECONDPAGE:
			
			// Populates fields in the second form
			fldSamId = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondSamIDField));	
			fldIncomingPort = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondIncomingPortField));
			fldRootMailbox = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondRootMailboxField));
			fldOutgoingPort = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondOutgoingPortField));
			fldReplyTo = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondReplyToField));

            if(gValues[SAMCREATORID])
				SetFieldText(fldSamId, gValues[SAMCREATORID], true);

			if(gValues[INCOMINGPORT])
				SetFieldText(fldIncomingPort, gValues[INCOMINGPORT], true);

			if(gValues[ROOTMAILBOX])
				SetFieldText(fldRootMailbox, gValues[ROOTMAILBOX], true);

			if(gValues[OUTGOINGPORT])
				SetFieldText(fldOutgoingPort, gValues[OUTGOINGPORT], true);

			if(gValues[REPLYTO])
				SetFieldText(fldReplyTo, gValues[REPLYTO], true);

			break;
			
	
		case THIRDPAGE:
			
			// Populates fields in the third form
			fldApn = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdAPNField));		
			fldUseEncryptPassword = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseEncryptPassField));		
			fldUseSmtp = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseSmtpField));	
			fldUseIncomingSSL = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseIncomingSSLField));	
			fldUseOutgoingSSL = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseOutgoingSSLField));

			if(gValues[APN])
				SetFieldText(fldApn, gValues[APN], true);

			if(gValues[USEENCRYPTEDPASSWORD])
				SetFieldText(fldUseEncryptPassword, gValues[USEENCRYPTEDPASSWORD], true);

			if(gValues[USESMTP])
				SetFieldText(fldUseSmtp, gValues[USESMTP], true);
				
			if(gValues[USEINCOMINGSSL])
				SetFieldText(fldUseIncomingSSL, gValues[USEINCOMINGSSL], true);
				
			if(gValues[USEOUTGOINGSSL])
				SetFieldText(fldUseOutgoingSSL, gValues[USEOUTGOINGSSL], true);


			break;
	}
				
}

/***********************************************************************
 *
 * FUNCTION:    	UpdateValuesList
 *
 * DESCRIPTION: 	Updates fields with values corresponding to the different
 *					keys, 	
 *
 * PARAMETERS:  	displayFlag -> Denotes the form in which the fields must
 *									be redrawn
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
 
Boolean UpdateValuesList(UInt8 displayFlag)
{
	Boolean	errorFlag = false;
	
	// Fields in the main form
	FieldType *fldUserName = NULL;
	FieldType *fldIncomingServer = NULL;
	FieldType *fldPassword = NULL;
	FieldType *fldEmailAddress = NULL;	
	FieldType *fldOutgoingServer = NULL;
	FieldType *fldTitle = NULL;
	
	// Fields in the second form
	FieldType *fldSamId = NULL;
	FieldType *fldIncomingPort = NULL;
	FieldType *fldRootMailbox = NULL;
	FieldType *fldOutgoingPort = NULL;
	FieldType *fldReplyTo = NULL;
	
	// Fields in the fourth form
	FieldType *fldApn = NULL;
	FieldType *fldUseEncryptPassword = NULL;
	FieldType *fldUseSmtp = NULL;
	FieldType *fldUseIncomingSSL = NULL;
	FieldType *fldUseOutgoingSSL = NULL;
	
	FormType  *frmP = NULL;
	
	frmP = FrmGetActiveForm();

	switch(displayFlag)
	{
		case FIRSTPAGE:

			fldUserName = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainUsernameField));
			fldIncomingServer = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainIncomingServerField));
			fldPassword = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainPasswordField));
			fldEmailAddress = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainEmailAddressField));
			fldOutgoingServer = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainOutgoingServerField));
			fldTitle = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainTitleField));

			if(!FldGetTextPtr(fldTitle))
			{
				errorFlag = true;
				goto Done;
			}
			else if(!StrLen(FldGetTextPtr(fldTitle)))
			{
				errorFlag = true;
				goto Done;
			}
						
			PrvUpdateValueList(fldUserName, USERNAME);
			PrvUpdateValueList(fldIncomingServer, INCOMINGSERVER);
			PrvUpdateValueList(fldPassword, PASSWORD);
			PrvUpdateValueList(fldEmailAddress, EMAILADDRESS);
			PrvUpdateValueList(fldOutgoingServer, OUTGOINGSERVER);
			PrvUpdateValueList(fldTitle, TITLE);
		
			break;
	
		case SECONDPAGE:
			
			fldSamId = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondSamIDField));	
			fldIncomingPort = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondIncomingPortField));
			fldRootMailbox = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondRootMailboxField));
			fldOutgoingPort = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondOutgoingPortField));
			fldReplyTo = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, SecondReplyToField));

			PrvUpdateValueList(fldSamId, SAMCREATORID);
			PrvUpdateValueList(fldIncomingPort, INCOMINGPORT);
			
			
			PrvUpdateValueList(fldRootMailbox, ROOTMAILBOX);
			
			
			PrvUpdateValueList(fldOutgoingPort, OUTGOINGPORT);
			PrvUpdateValueList(fldReplyTo, REPLYTO);

			break;
			
	
		case THIRDPAGE:
			
			fldApn = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdAPNField));		
			fldUseEncryptPassword = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseEncryptPassField));		
			fldUseSmtp = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseSmtpField));	
			fldUseIncomingSSL = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseIncomingSSLField));
			fldUseOutgoingSSL = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, ThirdUseOutgoingSSLField));	

			PrvUpdateValueList(fldApn, APN);
			PrvUpdateValueList(fldUseEncryptPassword, USEENCRYPTEDPASSWORD);
			PrvUpdateValueList(fldUseSmtp, USESMTP);
			PrvUpdateValueList(fldUseIncomingSSL, USEINCOMINGSSL);
			PrvUpdateValueList(fldUseOutgoingSSL, USEOUTGOINGSSL);

			break;
	}

Done:
	return errorFlag;
}

/***********************************************************************
 *
 * FUNCTION:    	UpdateMMDeviceDB
 *
 * DESCRIPTION: 	Updates the __MMDevice database with the new values,
 *					if the records for the given account slot already exist, 
 *					the records are deleted 	
 *
 * PARAMETERS:  	currentAccSlot -> Account slot for the current slot
 *					localID -> ID of the database
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
 
void UpdateMMDeviceDB(UInt16 currentAccSlot, LocalID localID)
{

	DmOpenRef	mmDeviceP = NULL;
	MemHandle 	recordH = NULL;
	Char *record = NULL;
	Char accSlotString[2] = "";
	UInt16 count = 0;
	UInt16 index = dmMaxRecordIndex;
	UInt16 size = 0;
	UInt16 numSpaces = 3;
	Err err = errNone;
	
	mmDeviceP = DmOpenDatabase(0, localID, dmModeReadWrite);

	for(count = 0; count < NUM_KEYS; count++)
	{
		if(gValues[count] || (count == SAMTYPEID && gWriteSAM))
		{
		    if (count == SAMCREATORID) gWriteSAM = true;
		    
			StrPrintF(accSlotString, "%d", (UInt16)currentAccSlot);
			// Allocates memory for the record
			if (count == SAMTYPEID)
			    record = MemPtrNew(StrLen("Set") + StrLen(gKeys[count]) + StrLen("'appl'") + StrLen(accSlotString) + numSpaces + 1);
            else
    			record = MemPtrNew(StrLen("Set") + StrLen(gKeys[count]) + StrLen(gValues[count]) + StrLen(accSlotString) + numSpaces + 1);

			if(record)
			{
				// Generates the record
				if (count == SAMTYPEID)
    				PrvGenerateRecord(record, gKeys[count], accSlotString, "'appl'");
				else
    				PrvGenerateRecord(record, gKeys[count], accSlotString, gValues[count]);

				// Finds the size of the record, giving space for the NULL terminating character
				size = StrLen(record) + 1;
				// Creates a new record
				recordH = DmNewRecord(mmDeviceP, &index, size);	
				
				if(recordH)
				{
					MemPtr p = MemHandleLock(recordH);
					
					// Writes to the record
					if(p)
						err = DmWrite(p, 0, record, size);
					MemHandleUnlock(recordH);
					DmReleaseRecord(mmDeviceP, index, false);
				}

				MemPtrFree(record);
				record = NULL;
			}
		}
	}

	// Close the database
	DmCloseDatabase(mmDeviceP);	
}

/***********************************************************************
 * FUNCTION:    	ExitStop
 *
 * DESCRIPTION: 	Exits the app by generating an appStopEvent 	
 *
 * PARAMETERS:  	None
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 ***********************************************************************/

void ExitApp()
{
	EventType event;
	
	event.eType = appStopEvent;
	EvtAddEventToQueue(&event);
}
	
	


