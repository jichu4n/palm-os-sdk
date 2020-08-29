/*****************************************************************************
 *
 * MLConduitArchive Generic Conduit Archive Database Mgr Header File
 *
 ****************************************************************************/
#ifndef _MLConduit_ARCHIVE_DB_MGR_H_
#define _MLConduit_ARCHIVE_DB_MGR_H_

#include "MLConduitPCMgr.h"


class CMLConduitArchive : public CMLConduitPCMgr
{
public:
    CMLConduitArchive(CPLogging *pLogging, DWORD dwGenericFlags, char *szDbName, TCHAR *pFileName, TCHAR *pDirName = NULL);
    virtual ~CMLConduitArchive();
public:   // Public Variables

protected:  // protected Variables

public:     // Public Functions
    virtual long PostSync(void);
    virtual long PreSync(void);

protected:    // protected Functions
    virtual long OpenDB(void);
};

#endif

