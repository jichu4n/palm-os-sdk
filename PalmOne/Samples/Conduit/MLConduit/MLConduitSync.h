 /*****************************************************************************
 *
 * File:        MLConduitSync.h
 * Description: Generic Conduit CMLConduitSync Header File
 * History:     May 15 2006 - DTS
 *              Derived CMLConduitSync class from CSynchronizer to 
 *              synchronize ML conduit
 *
 ****************************************************************************/
#ifndef __MLConduit_SYNCHRONIZER_H_
#define __MLConduit_SYNCHRONIZER_H_

#include <GenSync.H>
#include "MLConduitPCMgr.h"
#include "MLConduitBackupMgr.h"



class CMLConduitSync : public CSynchronizer
{
public:
    CMLConduitSync(CSyncProperties& rProps, DWORD dwDatabaseFlags);
    virtual ~CMLConduitSync();

protected:
    virtual CPDbBaseMgr *CreateArchiveManager(TCHAR *pFilename);
	virtual long CreatePCManager(void);
    virtual long CreateBackupManager(void);
};

#endif


