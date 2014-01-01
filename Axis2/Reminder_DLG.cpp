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

// ReminderTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Reminder_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReminderDLG property page

IMPLEMENT_DYNCREATE(CReminderDLG, CDialog)

CReminderDLG::CReminderDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CReminderDLG::IDD)
{
	UNREFERENCED_PARAMETER(pParent);
}

CReminderDLG::~CReminderDLG()
{
}

void CReminderDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReminderDLG)
	DDX_Control(pDX, IDC_DELETE, m_cbDelete);
	DDX_Control(pDX, IDC_NEVER, m_cbNever);
	DDX_Control(pDX, IDC_TEXT, m_ceText);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReminderDLG, CDialog)
	//{{AFX_MSG_MAP(CReminderDLG)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_NEVER, OnNever)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReminderDLG message handlers

BOOL CReminderDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_ceText.SetWindowText(rObject->m_csText);
	this->SetWindowText(rObject->m_csTitle);
	if (rObject->bOnce)
		m_cbNever.EnableWindow(false);
	return TRUE;
}
void CReminderDLG::OnDelete()
{
	CString csKey;
	csKey.Format("%s\\%s",REGKEY_REMINDER, rObject->m_csTitle);
	Main->DeleteRegistryKey(csKey,hRegLocation);
	OnClose();
}

void CReminderDLG::OnNever()
{
	CString csKey;
	csKey.Format("%s\\%s",REGKEY_REMINDER, rObject->m_csTitle);
	Main->PutRegistryDword("Type",0,hRegLocation,csKey);
	OnClose();
}

void CReminderDLG::OnClose()
{
	if (rObject->bOnce)
	{
		CString csKey;
		csKey.Format("%s\\%s",REGKEY_REMINDER, rObject->m_csTitle);
		Main->PutRegistryDword("Type",0,hRegLocation,csKey);
	}
	OnCancel();
}

void CReminderDLG::OnCancel()
{
	HKEY hKey;
	CString csKey;
	csKey.Format("%s\\%s",REGKEY_REMINDER, rObject->m_csTitle);
	LONG lStatus = RegOpenKeyEx(hRegLocation, csKey, 0, KEY_ALL_ACCESS, &hKey);
	if ( lStatus == ERROR_SUCCESS )
	{
		CTime tNow = CTime::GetCurrentTime();
		CString csKey;
		csKey.Format("%s\\%s",REGKEY_REMINDER, rObject->m_csTitle);
		Main->PutRegistryDword("LastRead",tNow.GetDay(),hRegLocation,csKey);
	}
	delete rObject;
	CDialog::OnCancel();
}