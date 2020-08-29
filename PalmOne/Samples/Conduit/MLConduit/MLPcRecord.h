/*****************************************************************************
 *
 * File:        MLPcRecord.h
 * Description: PC record for Music Library Conduit
 * History:     May 15 2006 - DTS
 *              Initial revision
 *
 ****************************************************************************/
#pragma once
#ifndef _MLConduit_PC_REC_H_
#define _MLConduit_PC_REC_H_
#include <stdio.h>
#include <windows.h>

#define MAX_STR_LENGTH 100

class CMLPcRecord
{
public:
	CMLPcRecord(void);
	~CMLPcRecord(void);

    void Init(void);

public:
	char m_sArtist[MAX_STR_LENGTH];
	char m_sTitle[MAX_STR_LENGTH];
	DWORD m_dwRecID;   
	DWORD m_dwStatus;  
	DWORD m_dwIndex;
	DWORD m_dwCategory;
	WORD  m_wAttrs;

public:
	void SetArtist(char *sArtist);
	void SetTitle(char *sTitle);
	void SetRecordID(DWORD dwID);
	void SetStatus(DWORD dwStatus);
	void SetIndex(DWORD dwIndex);
    void SetCategory(DWORD dwCategory);
    void SetAttributes(WORD wAttrs);

	char *GetArtist(void);
	char *GetTitle(void);
    DWORD GetRecordID(void);
    DWORD GetStatus(void);
    DWORD GetIndex(void);
	DWORD GetCategory(void);
    WORD GetAttributes(void);
};

#endif
