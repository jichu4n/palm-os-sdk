 /*****************************************************************************
 *
 * File:        MLConduitPcMgr.h
 * Description: CMLConduitPCMgr Generic Conduit PC Database Mgr Header File
 * History:     May 15 2006 - DTS
 *              Derived CMLConduitPCMgr class from CPcMgr
 *
 ****************************************************************************/
#ifndef _MLConduit_PC_DB_MGR_H_
#define _MLConduit_PC_DB_MGR_H_
#include <CPString.h>
#include <PcMgr.h>
#include <time.h>
#include "MLPcRecord.h"

#define MAX_STR_LENGTH 100
#define MAX_BUFFER_LENGTH 300

// Music Library structure as on handheld
typedef struct {
	char   sTitle[MAX_STR_LENGTH];
	char   sArtist[MAX_STR_LENGTH];
} MusicLibEntry;

typedef struct {

    DWORD dwRecCount;

} MLPcRec;


class CMLConduitPCMgr : public CPcMgr
{
public: 
    CMLConduitPCMgr(CPLogging *pLogging, DWORD dwGenericFlags, char *szDbName, TCHAR *pFileName = NULL, TCHAR *pDirName = NULL, eSyncTypes syncType = eDoNothing);
    virtual ~CMLConduitPCMgr();
    virtual long RetrieveDB(void);
    virtual long StoreDB(void);
	virtual long ConvertGenericToPc(CPalmRecord *pPalmRec, BYTE *pBinary, BOOL bClearAttributes);
	virtual long ConvertPCtoGeneric(StorageRecordType *pRec, CPalmRecord &palmRec);
	virtual long CloseDB(BOOL bDontUpdate);
	virtual long Open(eOpenDbType mode);
	virtual long ReadInData(LPVOID pBuffer, DWORD dwNumberOfBytesToRead);

	//long WriteOutDBInfo(DWORD dwRecCount);

protected:
	virtual long WriteOutData(LPVOID pBuffer, DWORD dwNumberOfBytesToWrite);
	long ReadOneRecFromFile(CPalmRecord *rec, BOOL &bDone);
	BOOL ReadNextRecEntry(char *sRecord, char *sRecEntry, DWORD &dwIndex);
	long CreatePcRecord(char *sRecord, CMLPcRecord *pcRec);
};



#endif



