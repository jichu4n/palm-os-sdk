 /*****************************************************************************
 *
 * File:        MLConduitPcMgr.h
 * Description: Generic Conduit CMLConduitPCMgr Src File
 * History:     May 15 2006 - DTS
 *              Derived CMLConduitPCMgr from CPcMgr class
 *
 ****************************************************************************/
#define ASSERT(f)          ((void)0)
#define TRACE0(sz)
#define TRACE(sz)
#include <windows.h>
#include <string.h>
#include <stdio.h>
#ifdef METROWERKS_WIN
#include <wmem.h>
#else
#include <memory.h>
#endif
#include <sys/stat.h>
#include <TCHAR.H>
#include <syncmgr.h>

#include "MLConduitPCMgr.h"
#include "resource.h"

#ifndef NO_HSLOG
extern HANDLE hLangInstance;
#endif

/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  Constructor
 *
 * Description: This method initializes the class object.
 *
 * Parameter(s): szDbName - a pointer to a character buffer specifying the 
 *                          name of the database being synced.
 *               pFileName - a pointer to a character buffer specifying the 
 *                          filename of the database.
 *               pDirName - a pointer to a character buffer specifying the 
 *                          directory where the database is stored.
 *               syncType - an enum value specifying the sync type to be performed.
 *
 * Return Value(s): None.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *
 *****************************************************************************/
CMLConduitPCMgr::CMLConduitPCMgr(CPLogging *pLogging, DWORD dwGenericFlags, char *szDbName, TCHAR *pFileName, TCHAR *pDirName, eSyncTypes syncType)  
: CPcMgr(pLogging, dwGenericFlags, szDbName, pFileName, pDirName, syncType)
{
}


CMLConduitPCMgr::~CMLConduitPCMgr() 
{
    TRACE0("CMLConduitPCMgr Destructor\n");

    if (m_hFile != INVALID_HANDLE_VALUE) 
    {
        fclose((FILE *)m_hFile); 
        m_hFile = INVALID_HANDLE_VALUE;
    }

    if (m_pDBInfo)
    {
        free( m_pDBInfo);
        m_pDBInfo = NULL;
    }
    if (m_pSortInfo) {
        free( m_pSortInfo);
        m_pSortInfo = NULL;
    }

    if (m_pAppInfo) 
    {
        free( m_pAppInfo);
        m_pAppInfo = NULL;
    }

}

/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  ConvertPCtoGeneric()
 *
 * Description: This method converts records from the PC storage type to the 
 *              CPalmRecord record type.
 *
 * Parameter(s): pRec - a pointer to a StorageRecordType record to be converted.
 *               palmRec - a CPalmRecord object to receive the converted record.
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *  05/16/06    DTS         derived CPcMgr class for music library
 *
 *****************************************************************************/
long CMLConduitPCMgr::ConvertPCtoGeneric(StorageRecordType *pRec, CPalmRecord &palmRec)
{
    long retval = 0;
    DWORD dwAttrs;
	MusicLibEntry mlEntry;
    MusicLibEntry *pMlEntry;

	ASSERT(pRec);

	//Create PalmRecord from music library structure
    CMLPcRecord *pcRec = (CMLPcRecord *)pRec;
    char *sTitle  = pcRec->GetTitle();
    char *sArtist = pcRec->GetArtist();

    strcpy(mlEntry.sTitle,  sTitle);
    strcpy(mlEntry.sArtist, sArtist);
    pMlEntry = &mlEntry;
    palmRec.SetRawData(sizeof(MusicLibEntry), (BYTE *)pMlEntry);
    palmRec.SetCategory(pcRec->GetCategory());
    //palmRec.SetIndex(pcRec->GetIndex());
    palmRec.SetID(pcRec->GetRecordID());
   
    palmRec.SetIndex((DWORD)-1);
    palmRec.ResetAttribs();
    dwAttrs = pcRec->GetAttributes();

	//Set record attributes used during sync
    if (dwAttrs & ATTRIB_UPDATE)
        palmRec.SetUpdate();

    if (dwAttrs & ATTRIB_DELETE)
        palmRec.SetDeleted();

    if (dwAttrs & ATTRIB_ARCHIVE)
        palmRec.SetArchived();

    if (dwAttrs & ATTRIB_NEW)
        palmRec.SetNew();

    // bug in not retrieving the private attribute from file
    palmRec.SetPrivate((BOOL)(dwAttrs & ATTRIB_PRIVATE));

    return retval;
}

/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  ReadNextRecEntry()
 *
 * Description: Parse and extract entry from buffer string. The buffer contains 
                the following:
 *              <artist> <title> <recordID> <record index> <record attributes>
 *
 * Parameter(s): sRecord   - buffer to parse
 *               sRecEntry - the extracted entry
                 dwIndex   - the start index in the buffer
 *
 * Return Value(s): 0 - not done
 *                  <0 - done
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  05/16/06    DTS         initial version
 *
 *****************************************************************************/
BOOL CMLConduitPCMgr::ReadNextRecEntry(char *sRecord, char *sRecEntry, DWORD &dwIndex)
{
	BOOL  bDone      = FALSE;
	DWORD dwNewIndex = 0;
    char sTmpEntry[MAX_STR_LENGTH];

	for(; *(sRecord + dwIndex) != '\n' && *(sRecord + dwIndex) != '\t'; dwIndex++) {
		sTmpEntry[dwNewIndex] = *(sRecord + dwIndex);
		dwNewIndex++;
	}

	sTmpEntry[dwNewIndex] = '\0';
    strcpy(sRecEntry, sTmpEntry);
	if(*(sRecord + dwIndex) == '\n') {
		bDone = TRUE;
	}
	dwIndex++;

	return bDone;
}

/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  CreatePcRecord()
 *
 * Description:  Create pc record from buffer string
 *
 * Parameter(s): sRecord - buffer to parse
 *               pcRec   - pc record to fill  
 *
 * Return Value(s): 0 - not done
 *                  <0 - done
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  05/16/06    DTS         initial version
 *
 *****************************************************************************/
long CMLConduitPCMgr::CreatePcRecord(char *sRecord, CMLPcRecord *pcRec)
{  
	BOOL bDone;
	DWORD dwIndex = 0;
	char sRecEntry[MAX_STR_LENGTH];
	
	bDone = ReadNextRecEntry(sRecord, sRecEntry, dwIndex);
    pcRec->SetArtist(sRecEntry);	
	strcpy(sRecEntry, "");

	if(!bDone) {
	    bDone = ReadNextRecEntry(sRecord, sRecEntry, dwIndex);
		pcRec->SetTitle(sRecEntry);
	    strcpy(sRecEntry, "");
	}
	if(!bDone) {
   	    bDone = ReadNextRecEntry(sRecord, sRecEntry, dwIndex);
	    DWORD dwRecID = atoi(sRecEntry);
	    pcRec->SetRecordID(dwRecID);
        strcpy(sRecEntry, "");
	}
	if(!bDone) {
	    bDone = ReadNextRecEntry(sRecord, sRecEntry, dwIndex);
	    DWORD dwIndex = atoi(sRecEntry);
	    pcRec->SetIndex(dwIndex);
        strcpy(sRecEntry, "");
	}
	if(!bDone) {
	    bDone = ReadNextRecEntry(sRecord, sRecEntry, dwIndex);
	    WORD wAttrs = atoi(sRecEntry);
	    pcRec->SetAttributes(wAttrs);
	}
    return 0;
}

/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  ReadOneRecFromFile()
 *
 * Description:  Read one music library record from file
 *
 * Parameter(s): palmRec - 
 *               bDone   - 
 *
 * Return Value(s): 0 - not done
 *                  <0 - done
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  05/16/06    DTS         initial version
 *
 *****************************************************************************/
long CMLConduitPCMgr::ReadOneRecFromFile(CPalmRecord *palmRec, BOOL &bDone)
{
   long retval = 0;
   char sRecord[MAX_BUFFER_LENGTH]; 
   CMLPcRecord *pcRec = new CMLPcRecord;

   if(fgets(sRecord, MAX_BUFFER_LENGTH, (FILE *)m_hFile) == NULL || strlen(sRecord) == 0) {
	   bDone = TRUE;
   } else if(!sRecord) {
	   retval = GEN_ERR_READING_DATA;
   } else { 
	   Sleep(100);
	   CreatePcRecord(sRecord, pcRec); 
	   ConvertPCtoGeneric((StorageRecordType *)pcRec, *palmRec);
   }
   delete pcRec;
   return retval;
}

/***************************************************************************
 *
 * Class:   CPcMgr
 *
 * Method:  RetrieveDB()
 *
 * Description: This method loads the database from file.
 *
 * Parameter(s): None.
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *  05/16/06    DTS         adapted to music library conduit
 *****************************************************************************/
long CMLConduitPCMgr::RetrieveDB(void)
{
    m_bNeedToSave = FALSE;
	if(!_tcslen((const char *)m_szDataFile))
		return(GEN_ERR_INVALID_DB_NAME);

    if (m_hFile == INVALID_HANDLE_VALUE)
        return GEN_ERR_INVALID_DB;

    BOOL bDone = FALSE;
    long retval = 0;
    CPalmRecord *palmRec = new CPalmRecord;

    for(int i = 0; !retval && !bDone; i++){
	   retval = ReadOneRecFromFile(palmRec, bDone);  
	   if(bDone || retval)
		   break;
	   AddRec(*palmRec);
	   palmRec->Initialize();  
    }

	delete palmRec;
	return retval;
}


/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  ReadInData()
 *
 * Description: This method is used to load data in from files.
 *
 * Parameter(s): pBuffer - a pointer to a buffer to receive the read in data.
 *               dwCharsToRead - a DWORD specifying how much data to 
 *                      attempt to read.
 *
 * Return Value(s): 0>= - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *  05/15/06    DTS         adapt to music library conduit
 *****************************************************************************/
long CMLConduitPCMgr::ReadInData(LPVOID pBuffer, DWORD dwCharsToRead)
{
	if(!fgets((char *)pBuffer, dwCharsToRead, (FILE *)m_hFile))
        return GEN_ERR_READING_DATA;

    return 0;
}


/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  StoreDB()
 *
 * Description: This method is called to store changes back into the 
 *              storage medium.
 *
 * Parameter(s): None.
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *  05/15/06    DTS         adapted for music library conduit
 *****************************************************************************/
long CMLConduitPCMgr::StoreDB(void)
{
    if ( !m_bNeedToSave) { // if no changes, don't waste time saving
        if ((!m_pCatMgr) || (!m_pCatMgr->IsChanged()))
            return 0; 
    }

    long retval = OpenDB();
    if (retval)
        return GEN_ERR_UNABLE_TO_SAVE;

    if (m_pAppInfo) {

        if (retval != 0){
            CloseDB(true);
            return GEN_ERR_UNABLE_TO_SAVE;
        }
    }

	BOOL bClearAttrs = false;
	CMLPcRecord *mlPcRec = new CMLPcRecord();

    for (DWORD dwIndex = 0; (dwIndex < m_dwMaxRecordCount) && (!retval); dwIndex++){
        if (!m_pRecordList[dwIndex]) // if there is no record, skip ahead
            continue;

		retval = ConvertGenericToPc(m_pRecordList[dwIndex], (BYTE *)mlPcRec, bClearAttrs);
        if (retval) {
            break;
        }
 
		mlPcRec->SetIndex(dwIndex);
        retval = WriteOutData(mlPcRec, MAX_RECORD_SIZE);

        if (retval){
            CloseDB(true);
			delete mlPcRec;
            return GEN_ERR_UNABLE_TO_SAVE;
        }
		mlPcRec->Init();
    }

    delete mlPcRec;
    CloseDB(true);
    m_bNeedToSave = FALSE;
    return 0;
}


/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  WriteOutDBInfo()
 *
 * Description: 
 *
 * Parameter(s): 
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  
 *
 *****************************************************************************/
/*
long CMLConduitPCMgr::WriteOutDBInfo(DWORD dwRecCount)
{
    long retval = 0;
    BOOL bSuccess;
	DWORD dwItemsToWrite = 1;
    DWORD dwItemsWritten;

	dwItemsWritten = fwrite(&dwRecCount, sizeof(DWORD), sizeof(dwRecCount), (FILE *)m_hFile);
	Sleep(100);
    return retval;
}
*/

/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  ConvertGenericToPc()
 *
 * Description: This method converts records from the PC storage type to the 
 *          CPalmRecord record type.
 *
 * Parameter(s): palmRec - a CPalmRecord object to be converted.
 *               pMLPcRec - a pointer to a StorageRecordType record to receive the converted data.
 *               bClearAttributes - a boolean specifying whether the attributes should be kept(FALSE)
 *                      or cleared (TRUE). The default is TRUE.
 *               
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  04/07/98    KRM         initial revision
 *  05/15/06    DTS         adapted to music library conduit
 *****************************************************************************/
long CMLConduitPCMgr::ConvertGenericToPc(CPalmRecord *pPalmRec, BYTE *pMLPcRec, BOOL bClearAttributes)
{
    DWORD dwRecSize;
    CMLPcRecord *pRecord;
	BYTE *pData = NULL;
    long retval = 0;
    MusicLibEntry *pMLHHRec;

    pRecord = (CMLPcRecord *)pMLPcRec;

	pData = (BYTE *)malloc(MAX_RECORD_SIZE);
	memset(pData, 0, MAX_RECORD_SIZE);
    retval = pPalmRec->GetRawData(pData, &dwRecSize);

	pMLHHRec = (MusicLibEntry *)pData;

    pRecord->SetArtist(pMLHHRec->sArtist);
    pRecord->SetTitle(pMLHHRec->sTitle);
    pRecord->SetAttributes(0);

    WORD wAttrs = pRecord->GetAttributes();
    if (!bClearAttributes) {
		
        if (pPalmRec->IsUpdate())
            wAttrs |= ATTRIB_UPDATE;
        if (pPalmRec->IsDeleted())
			wAttrs |= ATTRIB_DELETE;
        if (pPalmRec->IsArchived())
            wAttrs |= ATTRIB_ARCHIVE; 
        if (pPalmRec->IsNew())
            wAttrs |= ATTRIB_NEW;
        if (pPalmRec->IsPrivate()) {
            wAttrs |= ATTRIB_PRIVATE;
        }
    }
    pRecord->SetAttributes(wAttrs);
    pRecord->SetRecordID(pPalmRec->GetID());    
    pRecord->SetCategory(pPalmRec->GetCategory());    

    return retval;
}

 /******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  WriteOutData()
 *
 * Description: Store the database
 *
 * Parameter(s): pBuffer - buffer to print to file
 *               dwNumberOfBytesToWrite - not used
 *               
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  04/07/98    KRM         initial revision
 *  05/15/06    DTS         adapted to music library conduit
 *****************************************************************************/
long CMLConduitPCMgr::WriteOutData(LPVOID pBuffer, DWORD dwNumberOfBytesToWrite)
{
    long retval = 0;
	char buffer[MAX_BUFFER_LENGTH];

    CMLPcRecord *rec = (CMLPcRecord *)pBuffer;
	WORD wAttrs   = rec->GetAttributes();
	char *sTitle  = rec->GetTitle();
	char *sArtist = rec->GetArtist();
	DWORD dwRecID = rec->GetRecordID();
	DWORD dwIndex = rec->GetIndex();

	sprintf(buffer, "%s\t%s\t%d\t%d\t%d\n", sArtist, sTitle, dwRecID, dwIndex, wAttrs);
	fprintf((FILE *)m_hFile, "%s", buffer);
	Sleep(100);

    return retval;
}


/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  CloseDB()
 *
 * Description: This method closes the Database file handle if it is open
 *
 * Parameter(s): bDontUpdate - an ignored boolean.
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *  22/05/06    DTS         adapted to music library conduit
 *
 *****************************************************************************/
long CMLConduitPCMgr::CloseDB(BOOL bDontUpdate)
{
    long retval = 0;
	if (m_hFile != INVALID_HANDLE_VALUE) {
        retval = fclose((FILE *)m_hFile);
		Sleep(100);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    return retval;
}


/******************************************************************************
 *
 * Class:   CMLConduitPCMgr
 *
 * Method:  Open()
 *
 * Description: This method opens the actual database file.
 *
 * Parameter(s): mode - an enum value specifying how to open the database file.
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *  22/05/06    DTS         adapted to music library conduit
 *
 *****************************************************************************/
long CMLConduitPCMgr::Open(eOpenDbType mode)
{
    long retval = 0;

    Close();
    switch (mode) {
        case eRead:
			m_hFile = fopen(m_szDataFile, "r");
            break;
        case eWrite:
			m_hFile = fopen(m_szDataFile, "r+");
            break;
        case eReadWrite:
			m_hFile = fopen(m_szDataFile, "r+");
			if (m_hFile != NULL)
                break;
        case eCreate:
			m_hFile = fopen(m_szDataFile, "w+");
            break;
        default:
            break;
    }
    Sleep(100);
	if (m_hFile == NULL) {
		m_hFile = INVALID_HANDLE_VALUE;
        retval = GEN_ERR_UNABLE_TO_OPEN_FILE;
    }
    
    return retval;
}
