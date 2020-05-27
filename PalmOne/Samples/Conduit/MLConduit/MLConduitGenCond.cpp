/*****************************************************************************
 *
 * File:        MLConduitSync.cpp
 * Description: CMLConduitGenCond Generic Conduit Entry Points Source File
 * History:     May 15 2006 - DTS
 *              Adapted generic conduit entry points for ML conduit
 *
 ****************************************************************************/
#define STRICT 1
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
#include <COMMCTRL.H>

#include <syncmgr.h>
#include "MLConduitGenCond.h"

#include <logstrng.h>
#include "MLConduitSync.h"

#include <PALM_CMN.H>
#include "resource.h"

HANDLE hLangInstance;
HANDLE hAppInstance;
extern HANDLE hLangInstance;
extern HANDLE hAppInstance;

long CALLBACK ConfigureDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void LoadCfgDlgBitmaps(HWND hDlg);

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
//     Function:        DllMain()
//
//     Description:    main entry point to the MLConduit component
//
//     Parameters:    hInstance - instance handle of the DLL
//                    dwReason  - why the entry point was called
//                    lpReserved - reserved
//
//     Returns:        1 if okay
//
/////////////////////////////////////////////////////////////////////////////
//     REVISION HISTORY:
//      03/25/98 KRM inital 
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _68K_

static int iTerminationCount = 0;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("CMLConduitGenCond Initializing!\n");
        
        if (!iTerminationCount ) {
            hAppInstance = hInstance;
			// use PalmLoadLanguage here to load different languages
            hLangInstance = hInstance;
		        }
        ++iTerminationCount;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("CMLConduitGenCond Terminating!\n");

        --iTerminationCount;
        if (!iTerminationCount ) {
			// use PalmFreeLanguage here to unload different languages
        }
    }
    return 1;   // ok
}
#endif


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
//     Function:        OpenConduit()
//
//     Description:  Extern "C" entry point into this conduit which starts the
//                 process of synchronizing the local database table with
//                 a remote conterpart residing on the remote view device. 
//
//     Parameters:   Pointer to a callback function used to report progress.
//                    
//                
//
//     Returns:        
//
/////////////////////////////////////////////////////////////////////////////
//     REVISION HISTORY:
//      03/25/98 KRM inital 
//      05/15/06 DTS created CMLConduitSync object
/////////////////////////////////////////////////////////////////////////////
ExportFunc long OpenConduit(PROGRESSFN pFn, CSyncProperties& rProps)
{
    long retval = -1;
    if (pFn)
    {
		CMLConduitSync* pGeneric;
		rProps.m_nRemoteCount = 1;

		pGeneric = new CMLConduitSync(rProps, GENERIC_FLAG_CATEGORY_SUPPORTED );
			if (pGeneric){
				retval = pGeneric->Perform();

				delete pGeneric;
			}
    }
    return(retval);
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
//       Function:              GetConduitName()
//
//       Description:  Extern "C" entry point into this conduit which returns
//                                 the name to be used when display messages regarding
//                                 this conduit.
//
//       Parameters:   pszName - buffer in which to place the name
//                                 nLen - maximum number of bytes of buffer     
//                                      
//                              
//
//       Returns:          -1 indicates erros
//
/////////////////////////////////////////////////////////////////////////////
//       REVISION HISTORY:
//      03/25/98 KRM inital 
//
/////////////////////////////////////////////////////////////////////////////
ExportFunc long GetConduitName(char* pszName,WORD nLen)
{
    long retval = -1;

    if (::LoadString((HINSTANCE)hLangInstance, IDS_CONDUIT_NAME, pszName, nLen))
        retval = 0;

    return retval;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
//       Function:     GetConduitVersion()
//
//       Description:  Extern "C" entry point into this conduit which returns
//                     the conduits version
//
//       Parameters:   none
//
//       Returns:      DWORD indicating major and minor version number
//                        HIWORD - reserved
//                        HIBYTE(LOWORD) - major number
//                        LOBYTE(LOWORD) - minor number
//
/////////////////////////////////////////////////////////////////////////////
//       REVISION HISTORY:
//      03/25/98 KRM inital 
/////////////////////////////////////////////////////////////////////////////
ExportFunc DWORD GetConduitVersion()
{
    return GENERIC_CONDUIT_VERSION;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
//       Function:     ConfigureConduit
//
//       Description:  Extern "C" entry point into this conduit which returns
//                     should display the UI necessary to configure this 
//                       conduit.
//
//       Parameters:   none
//
//       Returns:      0 - success, !0 - failure
//
/////////////////////////////////////////////////////////////////////////////
//       REVISION HISTORY:
//      03/25/98 KRM inital 
/////////////////////////////////////////////////////////////////////////////
ExportFunc long ConfigureConduit(CSyncPreference& pref)
{

    long nRtn = -1;
    CfgConduitInfoType cfg;
    cfg.dwVersion = CFGCONDUITINFO_VERSION_1;
    cfg.dwSize  = sizeof(CfgConduitInfoType);
    cfg.dwCreatorId = 0;
    cfg.dwUserId = 0;
    memset(cfg.szUser , 0, sizeof(cfg.szUser));
    memset(cfg.m_PathName, 0, sizeof(cfg.m_PathName)); 
    cfg.syncPermanent = pref.m_SyncType;
    cfg.syncTemporary = pref.m_SyncType;
    cfg.syncNew = pref.m_SyncType;
    cfg.syncPref = eTemporaryPreference; 

    int iResult;
    iResult = DialogBoxParam((HINSTANCE)hLangInstance, 
              MAKEINTRESOURCE(IDD_CONDUIT_ACTION), 
              GetForegroundWindow(), 
              (DLGPROC)ConfigureDlgProc,
              (LPARAM)&cfg);
    if (iResult == 0) {
        pref.m_SyncType = cfg.syncNew;
        pref.m_SyncPref = cfg.syncPref;
        nRtn = 0;
    }
    return nRtn;
}

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////
//
//	 Method:		GetConduitInfo
//
//	 Description:	This function provides a way for a Conduit to provide info
//                  to the caller. 
//                  In this version of the call, MFC Version, Conduit Name, and
//                  Default sync action are the types of information this call
//                  will return.
//
//	 Parameters:	ConduitInfoEnum infoType - enum specifying what info is being
//                          requested.
//                  void *pInArgs - This parameter may be null, except for the Conduit
//                          name enum, this value will be a ConduitRequestInfoType structure.
//                  This following to parameters vary depending upon the info being requested. 
//                  For enum eConduitName
//                  void *pOut - will be a pointer to a character buffer
//                  DWORD *pdwOutSize - will be a pointer to a DWORD specifying the size of the character buffer.
//
//                  For enum eMfcVersion
//                  void *pOut - will be a pointer to a DWORD
//                  DWORD *pdwOutSize - will be a pointer to a DWORD specifying the size of pOut.
//
//                  For enum eDefaultAction
//                  void *pOut - will be a pointer to a eSyncType variable
//                  DWORD *pdwOutSize - will be a pointer to a DWORD specifying the size of pOut.
//
//	 Returns:		0		- Success.
//					!0      - error code.
//
/////////////////////////////////////////////////////////////////////////////
//	 REVISION HISTORY:
//		01/23/98    KRM Created
//
/////////////////////////////////////////////////////////////////////////////
ExportFunc long GetConduitInfo(ConduitInfoEnum infoType, void *pInArgs, void *pOut, DWORD *pdwOutSize)
{
    if (!pOut)
        return CONDERR_INVALID_PTR;
    if (!pdwOutSize)
        return CONDERR_INVALID_OUTSIZE_PTR;

    switch (infoType) {
        case eConduitName:

            // This code is for example. This conduit does not use this code
            
            if (!pInArgs)
                return CONDERR_INVALID_INARGS_PTR;
            ConduitRequestInfoType *pInfo;
            pInfo = (ConduitRequestInfoType *)pInArgs;
            if ((pInfo->dwVersion != CONDUITREQUESTINFO_VERSION_1) ||
                (pInfo->dwSize != SZ_CONDUITREQUESTINFO))
                return CONDERR_INVALID_INARGS_STRUCT;
            
	        if (!::LoadString((HINSTANCE)hLangInstance, IDS_CONDUIT_NAME, (TCHAR*)pOut, *pdwOutSize))
                return CONDERR_CONDUIT_RESOURCE_FAILURE;
            break;
        case eDefaultAction:
            if (*pdwOutSize != sizeof(eSyncTypes))
                return CONDERR_INVALID_BUFFER_SIZE;
            (*(eSyncTypes*)pOut) = eFast;
            break;
        case eMfcVersion:
            if (*pdwOutSize != sizeof(DWORD))
                return CONDERR_INVALID_BUFFER_SIZE;
            (*(DWORD*)pOut) = MFC_NOT_USED;
            break;
        default:
            return CONDERR_UNSUPPORTED_CONDUITINFO_ENUM;
    }
    return 0;
}


////////////////////////////////////===============================///////////

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////
//
//	 Method:		CfgConduit
//
//	 Description:	This is the second instance of ConfigureConduit. This function
//                  is provided with more info from the caller.
//                  This version of the function only supports the eConfig1 enum.
//
//	 Parameters:	ConduitCfgEnum cfgType - enum specifying the version of the 
//                      CfgConduit Call.
//                  void *pArgs - In the eConfig1 call, this is a pointer to aCfgConduitInfoType 
//                      structure. This structure provides extra info to the conduit.
//                  DWORD *pdwArgsSize - This is a pointer to a DWORD specifying the size of pArgs.
//
//	 Returns:		0		- Success.
//					!0      - error code.
//
/////////////////////////////////////////////////////////////////////////////
//	 REVISION HISTORY:
//		01/23/98    KRM Created
//
/////////////////////////////////////////////////////////////////////////////

ExportFunc long CfgConduit( ConduitCfgEnum cfgType, void *pArgs, DWORD *pdwArgsSize)
{
	long nRtn = -1;
	TCHAR szName[256];
    DWORD dwNamesize;
    ConduitRequestInfoType infoStruct;
    CfgConduitInfoType *pCfgInfo;


    dwNamesize = sizeof(szName);


    if (!pArgs)
        return CONDERR_INVALID_INARGS_PTR;
    if (!pdwArgsSize)
        return CONDERR_INVALID_ARGSSIZE_PTR;
    if (*pdwArgsSize != SZ_CFGCONDUITINFO) 
        return CONDERR_INVALID_ARGSSIZE;

    if (cfgType != eConfig1)
            return CONDERR_UNSUPPORTED_CFGCONDUIT_ENUM;

    pCfgInfo = (CfgConduitInfoType *)pArgs;
    if (pCfgInfo->dwVersion != CFGCONDUITINFO_VERSION_1)
        return CONDERR_UNSUPPORTED_STRUCT_VERSION;



    infoStruct.dwVersion = CONDUITREQUESTINFO_VERSION_1;
    infoStruct.dwSize = SZ_CONDUITREQUESTINFO;
    infoStruct.dwCreatorId = pCfgInfo->dwCreatorId;
    infoStruct.dwUserId = pCfgInfo->dwUserId;
    strcpy(infoStruct.szUser, pCfgInfo->szUser);
    nRtn = GetConduitInfo(eConduitName, (void *)&infoStruct, (void *)szName, &dwNamesize);
    if (nRtn)
        return nRtn;

    int iResult;
    iResult = DialogBoxParam((HINSTANCE)hLangInstance, 
              MAKEINTRESOURCE(IDD_CONDUIT_CFG_DETAILED), 
              GetForegroundWindow(), 
              (DLGPROC)ConfigureDlgProc,
              (LPARAM)pCfgInfo);

    return 0;
}



long CALLBACK ConfigureDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    static CfgConduitInfoType *pCfgInfo;
    TCHAR szPath[256];

    switch (Message) {
        case WM_INITDIALOG:

            if (lParam != 0) {
                TCHAR szBuffer[256];
                TCHAR szBuf2[256];
                LoadString((HINSTANCE)hLangInstance, IDS_SYNC_ACTION_TEXT, szBuffer, sizeof(szBuffer));
                SetDlgItemText(hWnd, IDC_ACTIONGROUPBOXTEXT, szBuffer);

                LoadString((HINSTANCE)hLangInstance, IDS_CONDUIT_NAME, szBuffer, sizeof(szBuffer));
                SetWindowText(hWnd, szBuffer);
     
				// Load the bitmaps properly
				LoadCfgDlgBitmaps(hWnd);

                pCfgInfo = (CfgConduitInfoType *)lParam;
                switch (pCfgInfo->syncTemporary){
                    case eFast:
                    case eSlow:
                        CheckRadioButton( hWnd, IDC_RADIO_SYNC, IDC_RADIO_DONOTHING, IDC_RADIO_SYNC);
                        LoadString((HINSTANCE)hLangInstance, IDS_SYNC_FILES, szBuffer, sizeof(szBuffer));
                        break;
                    case ePCtoHH:
                        CheckRadioButton( hWnd, IDC_RADIO_SYNC, IDC_RADIO_DONOTHING, IDC_RADIO_PCTOHH);
                        LoadString((HINSTANCE)hLangInstance, IDS_PCTOHH, szBuffer, sizeof(szBuffer));
                        break;
                    case eHHtoPC:
                        CheckRadioButton( hWnd, IDC_RADIO_SYNC, IDC_RADIO_DONOTHING, IDC_RADIO_HHTOPC);
                        LoadString((HINSTANCE)hLangInstance, IDS_HHTOPC, szBuffer, sizeof(szBuffer));
                        break;
                    case eDoNothing:
                    default:
                        CheckRadioButton( hWnd, IDC_RADIO_SYNC, IDC_RADIO_DONOTHING, IDC_RADIO_DONOTHING);
                        LoadString((HINSTANCE)hLangInstance, IDS_DO_NOTHING, szBuffer, sizeof(szBuffer));
                        break;
                }
                // did we get called from the old config call or the new cfg call?
                if (pCfgInfo->dwCreatorId != 0) {

                    SetDlgItemText(hWnd, IDC_STATIC_TEMPORARY, szBuffer);

                    switch (pCfgInfo->syncPermanent){
                        case eFast:
                        case eSlow:
                            LoadString((HINSTANCE)hLangInstance, IDS_SYNC_FILES, szBuffer, sizeof(szBuffer));
                            break;
                        case ePCtoHH:
                            LoadString((HINSTANCE)hLangInstance, IDS_PCTOHH, szBuffer, sizeof(szBuffer));
                            break;
                        case eHHtoPC:
                            LoadString((HINSTANCE)hLangInstance, IDS_HHTOPC, szBuffer, sizeof(szBuffer));
                            break;
                        case eDoNothing:
                        default:
                            LoadString((HINSTANCE)hLangInstance, IDS_DO_NOTHING, szBuffer, sizeof(szBuffer));
                            break;
                    }
                    SetDlgItemText(hWnd, IDC_STATIC_PERMANENT, szBuffer);

                    LoadString((HINSTANCE)hLangInstance, IDS_CURRENT_SETTINGS_GROUP, szBuffer, sizeof(szBuffer));
                    wsprintf(szBuf2, szBuffer, pCfgInfo->szUser);
                    SetDlgItemText(hWnd, IDC_CURRENT_SETTINGS_GROUP, szBuf2);
                }
            }
            break;
        case WM_COMMAND:
            switch (wParam) {
                case IDC_RADIO_SYNC:
                case IDC_RADIO_PCTOHH:
                case IDC_RADIO_HHTOPC:
                case IDC_RADIO_DONOTHING:
                    CheckRadioButton( hWnd, IDC_RADIO_SYNC, IDC_RADIO_DONOTHING, wParam);
                    break;
                case IDCANCEL:
                    EndDialog(hWnd, 1);
                    return TRUE;
                case IDOK:
                    if (IsDlgButtonChecked(hWnd, IDC_RADIO_SYNC)) {
                        pCfgInfo->syncNew = eFast;
                    } else if (IsDlgButtonChecked(hWnd, IDC_RADIO_PCTOHH)) {
                        pCfgInfo->syncNew = ePCtoHH;
                    } else if (IsDlgButtonChecked(hWnd, IDC_RADIO_HHTOPC)) {
                        pCfgInfo->syncNew = eHHtoPC;
                    } else {
                        pCfgInfo->syncNew = eDoNothing;
                    }
                    if (IsDlgButtonChecked(hWnd, IDC_MAKEDEFAULT)){
                        pCfgInfo->syncPref = ePermanentPreference;
                    } else {
                        pCfgInfo->syncPref = eTemporaryPreference;
                    }
                    EndDialog(hWnd, 0);
                    return TRUE;
				case IDHELP:
#ifdef _DEBUG
#define HELP_FILE "hotsyncd.hlp"
#else
#define HELP_FILE "hotsync.hlp"
#endif
					if (GetModuleFileName((HINSTANCE)hAppInstance, szPath, sizeof(szPath)))
					{
						// Look for the end of the path
						int		fileOffset;
						fileOffset = strlen(szPath);
						for (; fileOffset > 0; fileOffset--)
						{
							if (szPath[fileOffset] == '\\')
								break;
						}

						if (fileOffset > 0)
						{
							szPath[fileOffset + 1] = 0;
							strcat(szPath, HELP_FILE);
							WinHelp(hWnd, szPath, HELP_CONTEXT, IDD_CONDUIT_ACTION + 0x20000 /*base resource*/);
						}
					}
					return TRUE;
					break;
                default:
                    break;
            }
            break;

		case WM_HELP:
			if (GetModuleFileName((HINSTANCE)hAppInstance, szPath, sizeof(szPath)))
			{
				// Look for the end of the path
				int		fileOffset;
				fileOffset = strlen(szPath);
				for (; fileOffset > 0; fileOffset--)
				{
					if (szPath[fileOffset] == '\\')
						break;
				}

				if (fileOffset > 0)
				{
					szPath[fileOffset + 1] = 0;
					strcat(szPath, HELP_FILE);
					WinHelp(hWnd, szPath, HELP_CONTEXT, IDD_CONDUIT_ACTION + 0x20000 /*base resource*/);
				}
			}
			return TRUE;
		case WM_SYSCOLORCHANGE:
			LoadCfgDlgBitmaps(hWnd);
			break;
        default:
            break;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////
//
//	 Method:		LoadCfgDlgBitmaps
//
//	 Description:	This routines translates the bitmap gray background to the
//					button face color set in the control panel/window appearances
//
//	 Parameters:	HWND	- hDlg, handle of the dialog
//
//	 Returns:		none
//
/////////////////////////////////////////////////////////////////////////////
//	 REVISION HISTORY:
//		01/15/99    Carl Created
//
/////////////////////////////////////////////////////////////////////////////
void LoadCfgDlgBitmaps(HWND hDlg)
{
	COLORMAP		colorMap;
	HWND			hwndButton;
	HBITMAP			hBitmap, hOldBitmap;

	//
	// setup the bitmaps
	//
	colorMap.to = GetSysColor(COLOR_BTNFACE);
	colorMap.from = RGB(192,192,192);

	// Sync 
	hBitmap = CreateMappedBitmap((HINSTANCE)hLangInstance, IDB_SYNC, 0, &colorMap, 1);
	// associate the bitmap with the button.
	if ((hwndButton = GetDlgItem(hDlg, IDC_SYNC)) != NULL)
	{
		hOldBitmap = (HBITMAP)SendMessage(hwndButton, STM_SETIMAGE,(WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)hBitmap);
		if (hOldBitmap != NULL)
			DeleteObject((HGDIOBJ)hOldBitmap);
	}

	// PC to HH 
	hBitmap = CreateMappedBitmap((HINSTANCE)hLangInstance, IDB_PCTOHH, 0, &colorMap, 1);
	// associate the bitmap with the button.
	if ((hwndButton = GetDlgItem(hDlg, IDC_PCTOHH)) != NULL)
	{
		hOldBitmap = (HBITMAP)SendMessage(hwndButton, STM_SETIMAGE,(WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)hBitmap);
		if (hOldBitmap != NULL)
			DeleteObject((HGDIOBJ)hOldBitmap);
	}


	// HH to PC 
	hBitmap = CreateMappedBitmap((HINSTANCE)hLangInstance, IDB_HHTOPC, 0, &colorMap, 1);
	// associate the bitmap with the button.
	if ((hwndButton = GetDlgItem(hDlg, IDC_HHTOPC)) != NULL)
	{
		hOldBitmap = (HBITMAP)SendMessage(hwndButton, STM_SETIMAGE,(WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)hBitmap);
		if (hOldBitmap != NULL)
			DeleteObject((HGDIOBJ)hOldBitmap);
	}

	// Do Nothing 
	hBitmap = CreateMappedBitmap((HINSTANCE)hLangInstance, IDB_DONOTHING, 0, &colorMap, 1);
	// associate the bitmap with the button.
	if ((hwndButton = GetDlgItem(hDlg, IDC_DONOTHING)) != NULL)
	{
		hOldBitmap = (HBITMAP)SendMessage(hwndButton, STM_SETIMAGE,(WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)hBitmap);
		if (hOldBitmap != NULL)
			DeleteObject((HGDIOBJ)hOldBitmap);
	}
}
