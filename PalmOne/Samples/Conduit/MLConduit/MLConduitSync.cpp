/*****************************************************************************
 *
 * File:        MLConduitSync.cpp
 * Description: Generic Conduit - CMLConduitSync Src File
 * History:     May 15 2006 - DTS 
 *              Derived CSynchronizer class for ML conduit
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
#include "MLConduitSync.h"
#include "MLConduitArchive.h"
extern HANDLE hLangInstance;

CMLConduitSync::CMLConduitSync(CSyncProperties& rProps, DWORD dwDatabaseFlags)
: CSynchronizer(rProps, dwDatabaseFlags)
{
}

CMLConduitSync::~CMLConduitSync()
{
}


/******************************************************************************
 *
 * Class:   CMLConduitSync
 *
 * Method:  CreatePCManager()
 *
 * Description: This method creates the PC database manager. If it successfully creates
 *          the manager, it then opens the database.
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
 *  05/16/06    DTS         derived for music library conduit
 *
 *****************************************************************************/
long CMLConduitSync::CreatePCManager(void)
{
    DeletePCManager();

    m_dbPC = new CMLConduitPCMgr(m_pLog, 
                        m_dwDatabaseFlags,
                        m_remoteDB->m_Name, 
                        m_rSyncProperties.m_LocalName, 
                        m_rSyncProperties.m_PathName,
                        m_rSyncProperties.m_SyncType);
    if (!m_dbPC)
        return GEN_ERR_LOW_MEMORY;
    return m_dbPC->Open();
}

/******************************************************************************
 *
 * Class:   CMLConduitSync
 *
 * Method:  CreateBackupManager()
 *
 * Description: This method creates the backup database manager. If it successfully creates
 *          the manager, it then opens the database.
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
 *  05/16/06    DTS         derived for music library conduit
 *
 *****************************************************************************/
long CMLConduitSync::CreateBackupManager(void)
{
    DeleteBackupManager();

    m_dbBackup = new CMLConduitBackupMgr(m_pLog, 
                                m_dwDatabaseFlags,
                                m_remoteDB->m_Name, 
                                m_rSyncProperties.m_LocalName, 
                                m_rSyncProperties.m_PathName,
                                m_rSyncProperties.m_SyncType);
    if (!m_dbBackup)
        return GEN_ERR_LOW_MEMORY;
    return m_dbBackup->Open();
}

/******************************************************************************
 *
 * Class:   CMLConduitSync
 *
 * Method:  CreateArchiveManager()
 *
 * Description: 
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
 *  05/16/06    DTS         derived for music library conduit
 *
 *****************************************************************************/
CPDbBaseMgr *CMLConduitSync::CreateArchiveManager(TCHAR *pFilename) 
{
    return (CPDbBaseMgr *)new CMLConduitArchive(m_pLog,
                                m_dwDatabaseFlags,
                                m_remoteDB->m_Name, 
                                pFilename, 
                                m_rSyncProperties.m_PathName);
}



