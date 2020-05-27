/*****************************************************************************
 *
 * CMLConduitBackupMgr Generic Conduit Backup Database Mgr Header File
 *
 ****************************************************************************/

#ifndef _MLConduit_BACKUP_DB_MGR_H_
#define _MLConduit_BACKUP_DB_MGR_H_

#include "MLConduitPCMgr.h"


class CMLConduitBackupMgr : public CMLConduitPCMgr
{
public:
    CMLConduitBackupMgr(CPLogging *pLogging, DWORD dwGenericFlags, char *szDbName, TCHAR *pFileName, TCHAR *pDirName = NULL, eSyncTypes syncType = eDoNothing);
    virtual ~CMLConduitBackupMgr();

protected:  // protected Variables
    TCHAR m_szRealDb[256];


public:     // Public Functions
    virtual long PreSync(void);
    virtual long PostSync(void);
    virtual long Close(BOOL bDontUpdate=FALSE);

protected:    // protected Functions
    virtual long OpenDB(void);

};

#endif

