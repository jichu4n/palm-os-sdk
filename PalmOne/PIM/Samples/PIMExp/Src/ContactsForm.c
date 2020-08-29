#include <PalmOS.h>
#include <PalmOSGlue.h>

#include "PIMExp.h"
#include "PIMExp_Rsc.h"

#include "AddressDB.h"
#include "AddrDefines.h"
#include "palmOneCreators.h"

#include "Hs.h"

static UInt16 gCurrentRecIndex = 0;
static DmOpenRef gContactsDBRef = 0;
static Boolean gEditMode = false;

extern UInt16 TonesLibRefNum;

Boolean ContactsFormHandleEvent(EventType * eventP);

Err PrvGetContactsDBRef(DmOpenRef *dbRef);
Err PrvFillContactsInfoInFields(AddrDBRecordPtr recP);
Err PrvDisplayFirstContact();
Err PrvDisplayPrevContact();
Err PrvDisplayNextContact();
Err PrvSaveCurrentContact();

extern void * GetObjectPtr(UInt16 objectID);
extern Err SetFieldTextFromStr(UInt16 fieldID, Char *stringPtr, Boolean redraw);
extern Err GetFieldTextFromStr(UInt16 fieldID, Char *stringPtr);
extern void PrvEnableEditableField(UInt16 fieldID);

Boolean ContactsFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType * frmP;
	DmOpenRef dbRef;

	switch (eventP->eType) 
	{
		case menuEvent:
			return false;

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);
			
			gEditMode = false;
            PrvDisplayFirstContact();

			handled = true;
			break;
            
        case frmUpdateEvent:
			break;
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == resButtonPrevContact)
			{
			    if (gEditMode) PrvSaveCurrentContact();
			    PrvDisplayPrevContact();
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == resButtonNextContact)
			{
			    if (gEditMode) PrvSaveCurrentContact();			
			    PrvDisplayNextContact();
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == resButtonContactEditMode)
			{
			    PrvEnableEditableField(resFieldLastName);
			    PrvEnableEditableField(resFieldFirstName);
			    PrvEnableEditableField(resFieldCompany);
			    PrvEnableEditableField(resFieldWork);
			    PrvEnableEditableField(resFieldHome);
			    PrvEnableEditableField(resFieldMobile);

                gEditMode = true;
                
			    handled = true;
			}

			break;
		}
		
		case frmCloseEvent:
    	    if (gEditMode) PrvSaveCurrentContact();		
		    PrvGetContactsDBRef(&dbRef);
		    DmCloseDatabase(dbRef);
		    break;
	}
    
	return handled;
}

Err PrvGetContactsDBRef(DmOpenRef *dbRef)
{
    if (!gContactsDBRef)
    {
        gContactsDBRef = DmOpenDatabaseByTypeCreator('DATA', kPalmOneCreatorIDContacts, dmModeReadWrite);
        if (!gContactsDBRef)
        {
            FrmCustomAlert(CustomAlert, "Can't open Contacts DB", 0, 0);
            return DmGetLastErr();
        }
    }
    
    *dbRef = gContactsDBRef;
    return errNone;
}

Err PrvDisplayFirstContact()
{
    DmOpenRef dbRef = 0;
    AddrDBRecordType addrRec;
    MemHandle recH = 0;
    
    PrvGetContactsDBRef(&dbRef);    // Can use AddrDBGetDatabase
                                    // but remember to modify source
                                    // code to use new creator ID 'PAdd'

    gCurrentRecIndex = 0;
    if (!DmNumRecords(dbRef)) return errNone;
        
    AddrDBGetRecord(dbRef, gCurrentRecIndex, &addrRec, &recH);
    
    PrvFillContactsInfoInFields(&addrRec);
    
    MemHandleUnlock(recH);
    
    return errNone;
}

Err PrvDisplayPrevContact()
{
    DmOpenRef dbRef = 0;
    AddrDBRecordType addrRec;
    MemHandle recH = 0;
    
    PrvGetContactsDBRef(&dbRef);    // Can use AddrDBGetDatabase
                                    // but remember to modify source
                                    // code to use new creator ID 'PAdd'

    if (gCurrentRecIndex)
    {
        gCurrentRecIndex--;
        
        AddrDBGetRecord(dbRef, gCurrentRecIndex, &addrRec, &recH);
        
        PrvFillContactsInfoInFields(&addrRec);
        
        MemHandleUnlock(recH);
    }
    
    return errNone;
}

Err PrvDisplayNextContact()
{
    DmOpenRef dbRef = 0;
    AddrDBRecordType addrRec;
    MemHandle recH = 0;
    
    PrvGetContactsDBRef(&dbRef);    // Can use AddrDBGetDatabase
                                    // but remember to modify source
                                    // code to use new creator ID 'PAdd'

    if (gCurrentRecIndex < ((Int32)(DmNumRecords(dbRef)) - 1))
    {
        gCurrentRecIndex++;
        
        AddrDBGetRecord(dbRef, gCurrentRecIndex, &addrRec, &recH);
        
        PrvFillContactsInfoInFields(&addrRec);
        
        MemHandleUnlock(recH);
    }
    
    return errNone;
}

Err PrvFillContactsInfoInFields(AddrDBRecordPtr recP)
{

    CharPtr ringToneName = NULL;
    UInt16 maxLength = 32;
    Err  error;
    
    error = TonesLibOpen  (TonesLibRefNum);   
    
    if (error !=errNone)
        return error;

    if (recP->fields[name]) SetFieldTextFromStr(resFieldLastName, recP->fields[name], true);
    else SetFieldTextFromStr(resFieldLastName, " ", true);
    
    if (recP->fields[firstName])SetFieldTextFromStr(resFieldFirstName, recP->fields[firstName], true);
    else SetFieldTextFromStr(resFieldFirstName, " ", true);
    
    if (recP->fields[company]) SetFieldTextFromStr(resFieldCompany, recP->fields[company], true);
    else SetFieldTextFromStr(resFieldCompany, " ", true);
    
    if (recP->fields[phone1]) SetFieldTextFromStr(resFieldWork, recP->fields[phone1], true);
    else SetFieldTextFromStr(resFieldWork, " ", true);
    
    if (recP->fields[phone2]) SetFieldTextFromStr(resFieldHome, recP->fields[phone2], true);
    else SetFieldTextFromStr(resFieldHome, " ", true);
    
    if (recP->fields[phone3]) SetFieldTextFromStr(resFieldMobile, recP->fields[phone3], true);
    else SetFieldTextFromStr(resFieldMobile, " ", true);    
    
    // changed 2.0
    if (recP->rel2blobInfo.ringtoneInfo.id) 
    {
        ringToneName = MemPtrNew(maxLength);
    
    	error = TonesLibGetToneName(TonesLibRefNum, recP->rel2blobInfo.ringtoneInfo, ringToneName, maxLength);
        if (error == errNone)    
    	    SetFieldTextFromStr(resFieldRingTone, ringToneName, true);
    	MemPtrFree(ringToneName);
    
    }
    else 
    	SetFieldTextFromStr(resFieldRingTone, " ", true);   
    //
    
    return errNone;
}

Err PrvSaveCurrentContact()
{
    DmOpenRef dbRef = 0;
    AddrDBRecordType addrRec;
    MemHandle recH = 0;
    UInt16 dirtyRecIndex = gCurrentRecIndex;
    AddrDBRecordFlags recFlags;
    Char phnStr1[12], phnStr2[12], phnStr3[12];
    
    MemSet(&recFlags, sizeof(AddrDBRecordFlags), 0);

    PrvGetContactsDBRef(&dbRef);
    AddrDBGetRecord(dbRef, dirtyRecIndex, &addrRec, &recH);
    MemHandleUnlock(recH);
       
    if ((recFlags.bits.name = FldDirty(GetObjectPtr(resFieldLastName))))
        GetFieldTextFromStr(resFieldLastName, addrRec.fields[name]);

    if ((recFlags.bits.firstName = FldDirty(GetObjectPtr(resFieldFirstName))))
        GetFieldTextFromStr(resFieldFirstName, addrRec.fields[firstName]);

    if ((recFlags.bits.company = FldDirty(GetObjectPtr(resFieldCompany))))        
        GetFieldTextFromStr(resFieldCompany, addrRec.fields[company]);
    
    if ((recFlags.bits.phone1 = FldDirty(GetObjectPtr(resFieldWork))))
    {
        GetFieldTextFromStr(resFieldWork, phnStr1);
        addrRec.fields[phone1] = phnStr1;
    }
    
    if ((recFlags.bits.phone2 = FldDirty(GetObjectPtr(resFieldHome))))
    {
        GetFieldTextFromStr(resFieldHome, phnStr2);
        addrRec.fields[phone2] = phnStr2;
    }
    
    if ((recFlags.bits.phone3 = FldDirty(GetObjectPtr(resFieldMobile))))
    {
        GetFieldTextFromStr(resFieldMobile, phnStr3);
        addrRec.fields[phone3] = phnStr3;
    }

    if (recFlags.bits.name | recFlags.bits.firstName | recFlags.bits.company |
        recFlags.bits.phone1 | recFlags.bits.phone2 | recFlags.bits.phone3)
        AddrDBChangeRecord(dbRef, &dirtyRecIndex, &addrRec, recFlags, false);

    return errNone;
}
