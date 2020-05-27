/*****************************************************************************
 *
 * Generic Conduit CMLConduitBackupMgr Src File
 *
 ****************************************************************************/
#define ASSERT(f)          ((void)0)
#define TRACE0(sz)
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
#include "MLConduitBackupMgr.h"
#include <Pgenerr.h>

/******************************************************************************
 *
 * Class:   CMLConduitBackupMgr
 *
 * Method:  Constructor
 *
 * Description: This method initial class data. This class is used to
 *              backup the main database. This backup copy is used during
 *              slow syncs to determine which records have been modified 
 *              on the Palm organizer.
 *
 * Parameter(s): szDbName   - a pointer to a character buffer specifying the name
 *                          of the database associated with this archive.
 *               pFileName  - a pointer to a character buffer specifying the name
 *                          of the database to save to.
 *               pDirName   - a pointer to a character buffer specifying the name
 *                          of the directory where the database is stored.
 *               syncType   - a enum value specifying what type of synchronization
 *                          is to be performed.
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
CMLConduitBackupMgr::CMLConduitBackupMgr(CPLogging *pLogging, DWORD dwGenericFlags, char *szDbName, TCHAR *pFileName, TCHAR *pDirName, eSyncTypes syncType)
: CMLConduitPCMgr(pLogging, dwGenericFlags, szDbName, pFileName, pDirName, syncType)
{
    TRACE0("CMLConduitBackupMgr Constructor\n");

    // change the name of the file to the name of the backup file.
    // 
    _tcscpy(m_szRealDb, m_szDataFile);
    char *pDot = _tcsrchr(m_szDataFile, '.');
    if (!pDot){
        _tcscat(m_szDataFile, ".BAK");
    } else {
        char *pSlash = _tcsrchr(m_szDataFile, '\\');
        if ((pSlash) && (pSlash > pDot)) // just tack on the extension
            _tcscat(m_szDataFile, ".BAK");
        else { // set the dot to null and cat on the extension.
            *pDot = '\0';
            _tcscat(m_szDataFile, ".BAK");
        }
    }
}

/******************************************************************************
 *
 * Class:   CMLConduitBackupMgr
 *
 * Method:  Destructor
 *
 * Description: This method frees any allocated objects
 *
 * Parameter(s): None.
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
CMLConduitBackupMgr::~CMLConduitBackupMgr()
{
    TRACE0("CMLConduitBackupMgr Destructor\n");
}

/******************************************************************************
 *
 * Class:   CMLConduitBackupMgr
 *
 * Method:  Close()
 *
 * Description: This method closes the backup database. Since the PostSync()
 *              method just copies the normal database, there is no reason
 *              to update the backup database here.
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
 *
 *****************************************************************************/
long CMLConduitBackupMgr::Close(BOOL bDontUpdate)
{
    long retval = CloseDB(TRUE);
    return retval;
}

/******************************************************************************
 *
 * Class:   CMLConduitBackupMgr
 *
 * Method:  PostSync()
 *
 * Description: This method does closing type functions on the backup 
 *              Database. In this case, it closes the current open database
 *              and then File copies the real database on top of the 
 *              old backup file.
 *
 * Parameter(s):    None.
 *
 * Return Value(s): 0 - no error
 *                  <0 - error code.
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *
 *****************************************************************************/
long CMLConduitBackupMgr::PostSync(void)
{
    long retval = CloseDB(TRUE);
    if (retval)
        return retval;

    // copy the PC DB to the bak file.
    BOOL bSuccess = CopyFile(m_szRealDb, m_szDataFile, FALSE);
    if (bSuccess)
        return 0;
    return GEN_ERR_UNABLE_TO_SAVE_BACKUP;
}


/******************************************************************************
 *
 * Class:   CMLConduitBackupMgr
 *
 * Method:  OpenDB()
 *
 * Description: This protected method is used to retrieve the database from
 *              its storage medium. For backup database, this retrieval is
 *              only performed on the start of a slow sync.
 *              In all over cases, the database is not retrieved.
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
 *
 *****************************************************************************/
long CMLConduitBackupMgr::OpenDB(void)
{
    long retval = 0;
    // only open and read the backup file in a slow sync.
    if ((m_SyncType == eSlow) && (m_bStartSync)) {
        retval = Open(eRead);
        if ((retval) && (retval == GEN_ERR_UNABLE_TO_OPEN_FILE)) {
            // if there is no backup file, don't worry about it
            return 0;
        }
        if (!retval){ // Fixed 3/25/99 KRM
            retval = PreSync();
        }
    }
    return retval;
}

/******************************************************************************
 *
 * Class:   CMLConduitBackupMgr
 *
 * Method:  PreSync()
 *
 * Description: This method does initialization actions with the Backup DB. If a database
 *              is access in memory, this method will load the database into memory. For backup 
 *              database, this retrieval is only performed on the 
 *              start of a slow sync.
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
 *
 *****************************************************************************/
long CMLConduitBackupMgr::PreSync(void)
{
    long retval = 0;
    if (m_bAlreadyLoaded)
        return 0;
    m_bAlreadyLoaded = TRUE;
    // only open and read the backup file in a slow sync.
    if (m_SyncType == eSlow) {
        retval = RetrieveDB();
        if ((retval ) && (retval == GEN_ERR_INVALID_DB)){
        // if there is no backup file, don't worry about it
            retval = 0;
        }
    }
    return retval;
}



