/*

 **********************************************************************
 *
 * Original Axis by:
 * Copyright (C) Philip A. Esterle 1998-2002 + (C) parts Adron 2002
 *
 * 55r,56(x) Mods, and Axis2 re-build by:
 * Copyright (C) Benoit Croussette 2004-2006
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 **********************************************************************

*/

#if !defined(AFX_COMMON_H__59DF2C0D_D0BD_45ED_9116_B34861746A45__INCLUDED_)
#define AFX_COMMON_H__59DF2C0D_D0BD_45ED_9116_B34861746A45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Common.h : header file

#define GETNONWHITESPACE( pStr ) while ( pStr[0] == ' ' || pStr[0] == '\t' ) pStr ++;

static int WM_RCONSOLEMESSAGE = WM_USER + 100;
static int WM_RCONSOLERECONNECT = WM_USER + 101;

#define TYPE_NONE 0
#define TYPE_ITEM 1
#define TYPE_CHAR 2
#define TYPE_DEF 3
#define TYPE_AREA 4
#define TYPE_SPAWN 5
#define TYPE_TEMPLATE 6
#define TYPE_SPELL 7
#define TYPE_MULTI 8

//Script Objects

class CTObject
{
public:
	CTObject();
	virtual ~CTObject();
	CString m_csValue;
	CString m_csFilename;
	BYTE m_bType;
	bool m_bCustom;
};

class CSObject : public CTObject
{
public:
	CSObject();
	~CSObject();
	CString ReadBlock(CStdioFile &csfInput);
	void WriteBlock(CStdioFile &csfInput);
	CString m_csDescription;
	CString m_csSubsection;
	CString m_csCategory;
	CString m_csDupeItem;
	CString m_csColor;
	CString m_csDisplay;
	CString m_csID;
};

//Script Array

class CScriptArray : public CPtrArray, public CTObject
{
public:
	CScriptArray();
	~CScriptArray();
	int Insert(CTObject * pScript);
	int Find(CString csName);
	int Find(CString csName, BYTE type);
	CString GetDef(CString csName);
};

//String Array
class CAdvStringArray : public CStringArray
{
public:
	int Insert(CString csName);
	int Find(CString csName);
	CString GetValue(CString csKey);
};

//Categories

class CCategory : public CTObject
{
public:
	CCategory();
	CString m_csName;
	CPtrList m_SubsectionList;
};

class CSubsection : public CTObject
{
public:
	CSubsection();
	CString m_csName;
	CPtrList m_ItemList;
};

class CReminder
{
public:
	CString m_csTitle;
	CString m_csText;
	DWORD m_dwTime;
	bool bOnce;
};

class CSoundIDX
{
public:
	DWORD dwStart;
	DWORD dwLenght;
	WORD wIndex;
	CString csName;
};

class CMusicList
{
public:
	CString csName;
	CString csPath;
	WORD wIndex;
};

//String Table Class

class CMsg : public CString  
{
public :
  CMsg(UINT nID);
  CMsg(LPCTSTR lpszString);
};

class CFMsg : public CString 
{                            
public:
  CFMsg(LPCTSTR pszFormatstr,...);
  CFMsg(UINT nFormatID,...);
};

// Utility functions
CString GetMulFileName(int iIndex);
int FindTable( const char * pFind, const char * const * ppTable, int iCount, int iElemSize = sizeof(const char *));
DWORD ahextoi( const char * pszStr );
DWORD alltoi( const char * pszStr, const char * pszStrComp = "", int iSkip = 0);
DWORD getrand( DWORD dwMin, DWORD dwMax );
bool IsNumber( const char * pszStr );
bool ReplaceFiles( CString csNewFile, CString csOldFile );
CString Encrypt(CString csValue);
CString EncryptUO(CString csValue);
CCategory * FindCategory(CPtrList * pPtrList, CString csName, bool bCreate = true);
CSubsection * FindSubsection(CCategory * pCategory, CString csName, bool bCreate = true);
UINT LoadProfileThread(LPVOID pParam);
UINT TrackingThread(LPVOID pParam);
int CalibrateEntry(byte * mask, byte * vals, int size);
bool Calibrate();
int CalibrateCoord(byte * ptrX, int MaskLocation);
bool AdjustPrivileges();
CString GetInternetCodeString(DWORD dwRet);
void ReminderLaunch(int iStartup = 0);
UINT ReminderThread(LPVOID pParam);
UINT Reminder(LPVOID pParam);
__int64 HashFileName(CString csFile);


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMON_H__59DF2C0D_D0BD_45ED_9116_B34861746A45__INCLUDED_)