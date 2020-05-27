/*****************************************************************************
 *
 * File:        MLPcRecord.cpp
 * Description: PC record for Music Library Conduit
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
#include ".\mlpcrecord.h"

CMLPcRecord::CMLPcRecord(void)
{
	Init();
}

CMLPcRecord::~CMLPcRecord(void)
{
}

/*****************************************************************************
 *
 * Function:    Init
 * Description: Initialize record
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::Init(void)
{
	SetArtist("");
	SetTitle("");
    SetRecordID(0);
	SetStatus(0);
	SetIndex(0);
}

/*****************************************************************************
 *
 * Function:    SetArtist
 * Description: Set artist string
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetArtist(char *sArtist)
{
   strcpy(m_sArtist, sArtist);
}

/*****************************************************************************
 *
 * Function:    SetTitle
 * Description: Set title string
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetTitle(char *sTitle)
{
   strcpy(m_sTitle, sTitle);
}

/*****************************************************************************
 *
 * Function:    SetRecordID
 * Description: Set RecordID string
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetRecordID(DWORD dwID)
{
	m_dwRecID = dwID;
}

/*****************************************************************************
 *
 * Function:    SetStatus
 * Description: Set status string
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetStatus(DWORD dwStatus)
{
	m_dwStatus = dwStatus;
}

/*****************************************************************************
 *
 * Function:    SetIndex
 * Description: Set record index
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetIndex(DWORD dwIndex)
{
	m_dwIndex = dwIndex;
}

/*****************************************************************************
 *
 * Function:    SetCategory
 * Description: Set record category
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetCategory(DWORD dwCategory)
{
	m_dwCategory = dwCategory;
}

/*****************************************************************************
 *
 * Function:    SetAttributes
 * Description: Set record attributes
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
void CMLPcRecord::SetAttributes(WORD wAttrs)
{
    m_wAttrs = wAttrs;
}

/*****************************************************************************
 *
 * Function:    GetArtist
 * Description: Set artist string
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
char *CMLPcRecord::GetArtist(void)
{
   return m_sArtist;
}

/*****************************************************************************
 *
 * Function:    GetTitle
 * Description: Set title string
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
char *CMLPcRecord::GetTitle(void)
{
   return m_sTitle;
}

/*****************************************************************************
 *
 * Function:    GetRecordID
 * Description: Set record ID
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
DWORD CMLPcRecord::GetRecordID(void)
{
   return m_dwRecID;
}

/*****************************************************************************
 *
 * Function:    GetStatus
 * Description: Set record status
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
DWORD CMLPcRecord::GetStatus(void)
{
   return m_dwStatus;
}

/*****************************************************************************
 *
 * Function:    GetIndex
 * Description: Set record index
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
DWORD CMLPcRecord::GetIndex(void)
{
   return m_dwIndex;
}

/*****************************************************************************
 *
 * Function:    GetCategory
 * Description: Set record category
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
DWORD CMLPcRecord::GetCategory(void)
{
   return m_dwCategory;
}

/*****************************************************************************
 *
 * Function:    GetAttributes
 * Description: Set record attributes
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
WORD CMLPcRecord::GetAttributes(void)
{
   return m_wAttrs;
}
