/*****************************************************************************
 *
 * Generic Conduit CMLConduitArchive Src File
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
#include "MLConduitArchive.h"


/* NOTE: This file is the Same as GenCn\Archive.CPP except for
 * class inheritance. This Class is subclassed from 
 * CAddressPcMgr instead of CPcMgr. The class name has been changed
 * from CArchiveDatabase to CMLConduitArchive.
 */

/******************************************************************************
 *
 * Class:   CMLConduitArchive
 *
 * Method:  Constructor
 *
 * Description: This method initial class data. This class is used to
 *              archive records from categories.
 *
 * Parameter(s): szDbName   - a pointer to a character buffer specifying the name
 *                          of the database associated with this archive.
 *               pFileName  - a pointer to a character buffer specifying the name
 *                          of the database to save to.
 *               pDirName   - a pointer to a character buffer specifying the name
 *                          of the directory where the database is stored
 *
 * Return Value(s): None
 *
 * Revision History:
 *
 * Date         Name        Description
 * ----------------------------------------------------------
 *  03/25/98    KRM         initial revision
 *
 *****************************************************************************/
CMLConduitArchive::CMLConduitArchive(CPLogging *pLogging, DWORD dwGenericFlags, char *szDbName, TCHAR *pFileName, TCHAR *pDirName)
: CMLConduitPCMgr(pLogging, dwGenericFlags, szDbName, pFileName, pDirName, eFast)
{
    TRACE0("CMLConduitArchive Constructor\n");

    // change the name of the file to the name of the Archive file.
    // 
	TCHAR *szExtension = ".ARC";
    char *pDot = _tcsrchr(m_szDataFile, '.');
    if (!pDot){
        _tcscat(m_szDataFile, szExtension);
    } else {
        char *pSlash = _tcsrchr(m_szDataFile, '\\');
        if ((pSlash) && (pSlash > pDot)) // just tack on the extension
            _tcscat(m_szDataFile, szExtension);
        else { // set the dot to null and cat on the extension.
            *pDot = '\0';
            _tcscat(m_szDataFile, szExtension);
        }
    }
}



/******************************************************************************
 *
 * Class:   CMLConduitArchive
 *
 * Method:  Destructor
 *
 * Description: This method frees any allocated objects
 *
 * Parameter(s): None
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
CMLConduitArchive::~CMLConduitArchive()
{
    TRACE0("CMLConduitArchive Destructor\n");
}

/******************************************************************************
 *
 * Class:   CMLConduitArchive
 *
 * Method:  PostSync()
 *
 * Description: This method does closing type functions. It stores the database 
 *              and closes it.
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
long CMLConduitArchive::PostSync(void)
{
    long retval;
    m_bStartSync = FALSE;
    retval = StoreDB();
    if (retval)
        return retval;
    retval = CloseDB(true);
    return retval;
}


/******************************************************************************
 *
 * Class:   CMLConduitArchive
 *
 * Method:  OpenDB()
 *
 * Description: This protected method opens the database and loads it if available.
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
long CMLConduitArchive::OpenDB(void)
{
    long retval = 0;
	// always open the archive in ReadWrite Mode
    if (m_bStartSync){
        retval = Open(eRead);
        
        if ((retval) && (retval == GEN_ERR_UNABLE_TO_OPEN_FILE)){
            // this is not a problem if the file does not exist.
            return 0;
        }
        if (!retval){
            retval = PreSync();
        }
    } else {
        retval = Open(eCreate);
    }
    return retval;
}

/******************************************************************************
 *
 * Class:   CMLConduitArchive
 *
 * Method:  PreSync()
 *
 * Description: This method does initialization actions with the archive DB. If a database
 *              is access in memory, this method will load the database into memory.
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
long CMLConduitArchive::PreSync(void)
{
    long retval = 0;
    if (m_bAlreadyLoaded)
        return 0;
    m_bAlreadyLoaded = TRUE;
    retval = RetrieveDB();
    return retval;
}



