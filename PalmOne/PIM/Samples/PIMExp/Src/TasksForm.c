#include <PalmOS.h>
#include <PalmOSGlue.h>

#include "PIMExp.h"
#include "PIMExp_Rsc.h"

#include "ToDoDB.h"
#include "palmOneCreators.h"

static UInt16 gCurrentRecIndex = 0;
static DmOpenRef gTasksDBRef = 0;
static Boolean gEditMode = false;

Boolean TasksFormHandleEvent(EventType * eventP);

Err PrvGetTasksDBRef(DmOpenRef *dbRef);
Err PrvFillTaskInfoInFields(ToDoDBRecordPtr recP);
Err PrvDisplayFirstTask();
Err PrvDisplayPrevTask();
Err PrvDisplayNextTask();
Err PrvSaveCurrentTask();

extern void * GetObjectPtr(UInt16 objectID);
extern Err SetFieldTextFromStr(UInt16 fieldID, Char *stringPtr, Boolean redraw);
extern Err GetFieldTextFromStr(UInt16 fieldID, Char *stringPtr);
extern void PrvEnableEditableField(UInt16 fieldID);

Boolean TasksFormHandleEvent(EventType * eventP)
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
            PrvDisplayFirstTask();

			handled = true;
			break;
            
        case frmUpdateEvent:
			break;
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == resButtonPrevTask)
			{
			    if (gEditMode) PrvSaveCurrentTask();
			    PrvDisplayPrevTask();
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == resButtonNextTask)
			{
			    if (gEditMode) PrvSaveCurrentTask();			
			    PrvDisplayNextTask();
			    handled = true;
			}
			else if (eventP->data.ctlSelect.controlID == resButtonTaskEditMode)
			{
			    //PrvEnableEditableField(resFieldDueDate);
			    //PrvEnableEditableField(resFieldCompleteDate);
			    //PrvEnableEditableField(resFieldAlarm);
			    //PrvEnableEditableField(resFieldTaskRepeat);
			    PrvEnableEditableField(resFieldTaskDescription);
			
			    gEditMode = true;
			    
			    handled = true;
			}

			break;
		}
		
		case frmCloseEvent:
		    if (gEditMode) PrvSaveCurrentTask();		
		    PrvGetTasksDBRef(&dbRef);
		    DmCloseDatabase(dbRef);
		    break;
	}
    
	return handled;
}

Err PrvGetTasksDBRef(DmOpenRef *dbRef)
{
    if (!gTasksDBRef)
    {
        gTasksDBRef = DmOpenDatabaseByTypeCreator('DATA', kPalmOneCreatorIDTasks, dmModeReadWrite);
        if (!gTasksDBRef)
        {
            FrmCustomAlert(CustomAlert, "Can't open Tasks DB", 0, 0);
            return DmGetLastErr();
        }
    }
    
    *dbRef = gTasksDBRef;
    return errNone;
}

Err PrvDisplayFirstTask()
{
    DmOpenRef dbRef = 0;
    ToDoDBRecordPtr taskRec;
    MemHandle recH = 0;
    
    PrvGetTasksDBRef(&dbRef);

    gCurrentRecIndex = 0;
    if (!DmNumRecords(dbRef)) return errNone;
       
    //TaskGetRecord(dbRef, gCurrentRecIndex, &taskRec, &recH);
    recH = DmQueryRecord(dbRef, gCurrentRecIndex);
    taskRec = (ToDoDBRecordPtr) MemHandleLock(recH);
    
    PrvFillTaskInfoInFields(taskRec);
    
    MemHandleUnlock(recH);
    
    return errNone;
}

Err PrvDisplayPrevTask()
{
    DmOpenRef dbRef = 0;
    ToDoDBRecordPtr taskRec;
    MemHandle recH = 0;
    
    PrvGetTasksDBRef(&dbRef);

    if (gCurrentRecIndex)
    {
        gCurrentRecIndex--;
        
        //TaskGetRecord(dbRef, gCurrentRecIndex, &taskRec, &recH);
        recH = DmQueryRecord(dbRef, gCurrentRecIndex);
        taskRec = (ToDoDBRecordPtr) MemHandleLock(recH);
        
        PrvFillTaskInfoInFields(taskRec);
        
        MemHandleUnlock(recH);
    }
    
    return errNone;
}

Err PrvDisplayNextTask()
{
    DmOpenRef dbRef = 0;
    ToDoDBRecordPtr taskRec;
    MemHandle recH = 0;
    
    PrvGetTasksDBRef(&dbRef);       // Can use ToDoDBGetDatabase
                                    // but remember to modify source
                                    // code to use new creator ID 'PAdd'

    if (gCurrentRecIndex < ((Int32)(DmNumRecords(dbRef)) - 1))
    {
        gCurrentRecIndex++;
        
        //TaskGetRecord(dbRef, gCurrentRecIndex, &taskRec, &recH);
        recH = DmQueryRecord(dbRef, gCurrentRecIndex);
        taskRec = (ToDoDBRecordPtr) MemHandleLock(recH);
        
        PrvFillTaskInfoInFields(taskRec);
        
        MemHandleUnlock(recH);
    }
    
    return errNone;
}

Err PrvFillTaskInfoInFields(ToDoDBRecordPtr recP)
{
    Char msg[50];
    DateType dueDate;
    DateType completeDate;
    ToDoAlarmInfoType alarmInfo;
    ToDoRepeatInfoType repeatInfo;
    
    if (recP->dataFlags.dueDate)
    {
        MemMove(&dueDate, recP->optionalData, sizeof(DateType));
        StrPrintF(msg, "%d/%d/%d", dueDate.month, dueDate.day, firstYear + dueDate.year);
        SetFieldTextFromStr(resFieldDueDate, msg, true);
    }
    else
        SetFieldTextFromStr(resFieldDueDate, "Not set", true);        
    
    if (recP->dataFlags.completionDate)
    {
        MemMove(&completeDate,
                recP->optionalData + (sizeof(DateType) * recP->dataFlags.dueDate),
                sizeof(DateType));
        StrPrintF(msg, "%d/%d/%d", completeDate.month, completeDate.day, firstYear + completeDate.year);
        SetFieldTextFromStr(resFieldCompleteDate, msg, true);        
    }
    else
        SetFieldTextFromStr(resFieldCompleteDate, "Not set", true);        
    
    if (recP->dataFlags.alarm)
    {
        MemMove(&alarmInfo,
                recP->optionalData + (sizeof(DateType) * recP->dataFlags.dueDate) +
                (sizeof(DateType) * recP->dataFlags.completionDate),
                sizeof(ToDoAlarmInfoType));
        StrPrintF(msg, "%02u:%02u", alarmInfo.alarmTime.hours, alarmInfo.alarmTime.minutes);
        SetFieldTextFromStr(resFieldAlarm, msg, true);
    }
    else
        SetFieldTextFromStr(resFieldAlarm, "Not set", true);        
    
    if (recP->dataFlags.repeat)
    {
        MemMove(&repeatInfo,
                recP->optionalData + (sizeof(DateType) * recP->dataFlags.dueDate) +
                (sizeof(DateType) * recP->dataFlags.completionDate) +
                (sizeof(ToDoAlarmInfoType) * recP->dataFlags.alarm),
                sizeof(ToDoRepeatInfoType));

        switch (repeatInfo.repeatInfo.repeatType)
        {
            case repeatNone:
                SetFieldTextFromStr(resFieldTaskRepeat, "None", true);
                break;
                
            case repeatDaily:
                SetFieldTextFromStr(resFieldTaskRepeat, "Daily", true);
                break;
                
            case repeatWeekly:
                SetFieldTextFromStr(resFieldTaskRepeat, "Weekly", true);        
                break;
                
            case repeatMonthlyByDay:
                SetFieldTextFromStr(resFieldTaskRepeat, "Monthly by day", true);        
                break;
                
            case repeatMonthlyByDate:
                SetFieldTextFromStr(resFieldTaskRepeat, "Monthly by date", true);        
                break;
                
            case repeatYearly:
                SetFieldTextFromStr(resFieldTaskRepeat, "Yearly", true);        
                break;
                
            default:
                SetFieldTextFromStr(resFieldTaskRepeat, "Error", true);        
        }
    }
    else
        SetFieldTextFromStr(resFieldTaskRepeat, "Not set", true);        
    
    if (recP->dataFlags.description)
    {
        StrCopy(msg,
                recP->optionalData + (sizeof(DateType) * recP->dataFlags.dueDate) +
                (sizeof(DateType) * recP->dataFlags.completionDate) +
                (sizeof(ToDoAlarmInfoType) * recP->dataFlags.alarm) +
                (sizeof(ToDoRepeatInfoType) * recP->dataFlags.repeat));
        SetFieldTextFromStr(resFieldTaskDescription, msg, true);
    }
    else
        SetFieldTextFromStr(resFieldTaskDescription, "Not set", true);        
    
    return errNone;
}

Err PrvSaveCurrentTask()
{
    DmOpenRef dbRef = 0;
    UInt16 dirtyRecIndex = gCurrentRecIndex;
    Char descStr[80];
        
    PrvGetTasksDBRef(&dbRef);
    
    if (FldDirty(GetObjectPtr(resFieldTaskDescription)))
    {
        GetFieldTextFromStr(resFieldTaskDescription, descStr);
        ToDoChangeRecord(dbRef, &dirtyRecIndex, TODO_FILTER_ALL,
                        TODO_SUBFILTER_DATE_LAST_SEVEN,
                        toDoRecordFieldDescription, descStr);
    }
    
    return errNone;
}