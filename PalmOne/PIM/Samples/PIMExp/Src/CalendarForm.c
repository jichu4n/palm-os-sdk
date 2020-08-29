#include <PalmOS.h>
#include <PalmOSGlue.h>

#include "PIMExp.h"
#include "PIMExp_Rsc.h"

#include "DateDB.h"
#include "palmOneCreators.h"

static UInt16 gCurrentRecIndex = 0;
static DmOpenRef gCalendarDBRef = 0;
static Boolean gEditMode = false;

Boolean CalendarFormHandleEvent(EventType * eventP);

Err PrvGetCalendarDBRef(DmOpenRef *dbRef);
Err PrvFillApptInfoInFields(ApptDBRecordPtr recP);
Err PrvDisplayFirstAppt();
Err PrvDisplayPrevAppt();
Err PrvDisplayNextAppt();
Err PrvSaveCurrentAppt();

extern void * GetObjectPtr(UInt16 objectID);
extern Err SetFieldTextFromStr(UInt16 fieldID, Char *stringPtr, Boolean redraw);
extern Err GetFieldTextFromStr(UInt16 fieldID, Char *stringPtr);
extern void PrvEnableEditableField(UInt16 fieldID);

Boolean CalendarFormHandleEvent(EventType * eventP)
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
            PrvDisplayFirstAppt();

			handled = true;
			break;
            
        case frmUpdateEvent:
			break;
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == resButtonPrevAppt)
			{
			    if (gEditMode) PrvSaveCurrentAppt();
			    PrvDisplayPrevAppt();
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == resButtonNextAppt)
			{
			    if (gEditMode) PrvSaveCurrentAppt();
			    PrvDisplayNextAppt();
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == resButtonApptEditMode)
			{
			    PrvEnableEditableField(resFieldDescription);
			    PrvEnableEditableField(resFieldLocation);
			    
			    gEditMode = true;
			    
			    handled = true;
			}

			break;
		}
		
		case frmCloseEvent:
		    if (gEditMode) PrvSaveCurrentAppt();
		    PrvGetCalendarDBRef(&dbRef);
		    DmCloseDatabase(dbRef);
		    break;
	}
    
	return handled;
}

Err PrvGetCalendarDBRef(DmOpenRef *dbRef)
{
    if (!gCalendarDBRef)
    {
        gCalendarDBRef = DmOpenDatabaseByTypeCreator('DATA', kPalmOneCreatorIDCalendar, dmModeReadWrite);
        if (!gCalendarDBRef)
        {
            FrmCustomAlert(CustomAlert, "Can't open Calendar DB", 0, 0);
            return DmGetLastErr();
        }
    }
    
    *dbRef = gCalendarDBRef;
    return errNone;
}

Err PrvDisplayFirstAppt()
{
    DmOpenRef dbRef = 0;
    ApptDBRecordType apptRec;
    MemHandle recH = 0;
    
    PrvGetCalendarDBRef(&dbRef);

    gCurrentRecIndex = 0;
    if (!DmNumRecords(dbRef)) return errNone;
       
    ApptGetRecord(dbRef, gCurrentRecIndex, &apptRec, &recH);
    
    PrvFillApptInfoInFields(&apptRec);
    
    MemHandleUnlock(recH);
    
    return errNone;
}

Err PrvDisplayPrevAppt()
{
    DmOpenRef dbRef = 0;
    ApptDBRecordType apptRec;
    MemHandle recH = 0;
    
    PrvGetCalendarDBRef(&dbRef);

    if (gCurrentRecIndex)
    {
        gCurrentRecIndex--;
        
        ApptGetRecord(dbRef, gCurrentRecIndex, &apptRec, &recH);
        
        PrvFillApptInfoInFields(&apptRec);
        
        MemHandleUnlock(recH);
    }
    
    return errNone;
}

Err PrvDisplayNextAppt()
{
    DmOpenRef dbRef = 0;
    ApptDBRecordType apptRec;
    MemHandle recH = 0;
    
    PrvGetCalendarDBRef(&dbRef);    // Can use ApptDBGetDatabase
                                    // but remember to modify source
                                    // code to use new creator ID 'PAdd'

    if (gCurrentRecIndex < ((Int32)(DmNumRecords(dbRef)) - 1))
    {
        gCurrentRecIndex++;
        
        ApptGetRecord(dbRef, gCurrentRecIndex, &apptRec, &recH);
        
        PrvFillApptInfoInFields(&apptRec);
        
        MemHandleUnlock(recH);
    }
    
    return errNone;
}

Err PrvFillApptInfoInFields(ApptDBRecordPtr recP)
{
    Char msg[50];
    
    if (recP->description) SetFieldTextFromStr(resFieldDescription, recP->description, true);
    else SetFieldTextFromStr(resFieldDescription, " ", true);
    
    if (recP->when)
    {
        StrPrintF(msg, "%02u:%02u", recP->when->startTime.hours,
                                recP->when->startTime.minutes);
        SetFieldTextFromStr(resFieldStartTime, msg, true);
            
        StrPrintF(msg, "%02u:%02u", recP->when->endTime.hours,
                                recP->when->endTime.minutes);
        SetFieldTextFromStr(resFieldEndTime, msg, true);
    }
    else
    {
        SetFieldTextFromStr(resFieldStartTime, "Not set", true);
        SetFieldTextFromStr(resFieldEndTime, "Not set", true);
    }
    
    if (recP->location) SetFieldTextFromStr(resFieldLocation, recP->location, true);
    else SetFieldTextFromStr(resFieldLocation, "Not set ", true);

    if (recP->repeat)
    {
        switch (recP->repeat->repeatType)
        {
            case repeatNone:
                SetFieldTextFromStr(resFieldRepeat, "None", true);
                break;
                
            case repeatDaily:
                SetFieldTextFromStr(resFieldRepeat, "Daily", true);
                break;
                
            case repeatWeekly:
                SetFieldTextFromStr(resFieldRepeat, "Weekly", true);        
                break;
                
            case repeatMonthlyByDay:
                SetFieldTextFromStr(resFieldRepeat, "Monthly by day", true);        
                break;
                
            case repeatMonthlyByDate:
                SetFieldTextFromStr(resFieldRepeat, "Monthly by date", true);        
                break;
                
            case repeatYearly:
                SetFieldTextFromStr(resFieldRepeat, "Yearly", true);        
                break;
                
            default:
                SetFieldTextFromStr(resFieldRepeat, "Error", true);        
        }
    }
    else SetFieldTextFromStr(resFieldRepeat, "Not set", true);
        
    if (StrLen(recP->timeZone.name)) SetFieldTextFromStr(resFieldTimezone, recP->timeZone.name, true);
    else SetFieldTextFromStr(resFieldTimezone, "Not set", true);
    
    switch (recP->meetingInfo.meetingStatus)
    {
        case unansweredMeeting:
            SetFieldTextFromStr(resFieldMeeting, "Unanswered", true);
            break;
        
        case tentativeMeeting:
            SetFieldTextFromStr(resFieldMeeting, "Tentative", true);
            break;
            
        case declinedMeeting:
            SetFieldTextFromStr(resFieldMeeting, "Declined", true);
            break;
            
        case acceptedMeeting:
            SetFieldTextFromStr(resFieldMeeting, "Accepted", true);
            break;
        
        case cancelledMeeting:
            SetFieldTextFromStr(resFieldMeeting, "Cancelled", true);
            break;
            
        default:
            SetFieldTextFromStr(resFieldMeeting, "Error", true);
    }
    
    return errNone;
}

Err PrvSaveCurrentAppt()
{
    DmOpenRef dbRef = 0;
    ApptDBRecordType apptRec;
    MemHandle recH = 0;
    UInt16 dirtyRecIndex = gCurrentRecIndex;
    ApptDBRecordChangeFlags recFlags;
    Char descStr[80], locStr[apptMaxLocationLength];
    
    MemSet(&recFlags, sizeof(ApptDBRecordFlags), 0);

    PrvGetCalendarDBRef(&dbRef);
    ApptGetRecord(dbRef, dirtyRecIndex, &apptRec, &recH);
    MemHandleUnlock(recH);
    
    if ((recFlags.description = FldDirty(GetObjectPtr(resFieldDescription))))
    {
        GetFieldTextFromStr(resFieldDescription, descStr);
        apptRec.description = descStr;
    }

    if ((recFlags.location = FldDirty(GetObjectPtr(resFieldLocation))))
    {
        GetFieldTextFromStr(resFieldLocation, locStr);
        apptRec.location = locStr;
    }

    if (recFlags.description | recFlags.location)
        ApptChangeRecord(dbRef, &dirtyRecIndex, &apptRec, recFlags);

    return errNone;
}
