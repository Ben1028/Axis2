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

// Settings_Paths.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Settings_OverridePaths.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsOverridePaths property page

IMPLEMENT_DYNCREATE(CSettingsOverridePaths, CPropertyPage)

CSettingsOverridePaths::CSettingsOverridePaths() : CPropertyPage(CSettingsOverridePaths::IDD)
{
	//{{AFX_DATA_INIT(CSettingsOverridePaths)
	//}}AFX_DATA_INIT
}


CSettingsOverridePaths::~CSettingsOverridePaths()
{
}


void CSettingsOverridePaths::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsOverridePaths)
	DDX_Control(pDX, IDC_PATHBROWSE, m_cbPathBrowse);
	DDX_Control(pDX, IDC_RESETPATH, m_cbResetPath);
	DDX_Control(pDX, IDC_PATHLIST, m_clcPathList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsOverridePaths, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsOverridePaths)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATHLIST, OnPathchangedItem)
	ON_BN_CLICKED(IDC_PATHBROWSE, OnPathBrowse)
	ON_BN_CLICKED(IDC_RESETPATH, OnResetPath)
	ON_BN_CLICKED(IDC_RESET_OVERPATHS, OnResetTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsOverridePaths message handlers

BOOL CSettingsOverridePaths::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_clcPathList.InsertColumn(0, "File", LVCFMT_LEFT, 80, -1);
	m_clcPathList.InsertColumn(1, "Path", LVCFMT_LEFT, 400, -1);
	OnFillPaths();

	return TRUE;
}

void CSettingsOverridePaths::OnPathBrowse() 
{
	CString csFile = m_clcPathList.GetItemText(iSelIndex, 0);
	CString csExt = csFile.Mid(csFile.Find("."));
	CString csFilter;
	csFilter.Format(_T("Ultima Online files (*%s)|*%s|All files (*.*)|*.*||"), csExt, csExt);
	CFileDialog dlg(TRUE, csExt, csFile, 0, csFilter, NULL);
	if ( dlg.DoModal() == IDOK )
	{
		CString csPath;
		csPath = dlg.GetPathName();
		m_clcPathList.SetItemText(iSelIndex, 1, csPath);
		Main->SetMulPath(iSelIndex, csPath);
		m_cbResetPath.EnableWindow(true);
		Main->PutRegistryString(GetMulFileName(iSelIndex), csPath, hRegLocation, REGKEY_OVERRIDEPATH);
	}
}

void CSettingsOverridePaths::OnResetPath()
{
	CString csPath;
	csPath.Format(_T("%s%s"), csMulPath, GetMulFileName(iSelIndex));
	m_clcPathList.SetItemText(iSelIndex, 1, csPath);
	Main->SetMulPath(iSelIndex, csPath);
	m_cbResetPath.EnableWindow(false);
	Main->DeleteRegistryValue(GetMulFileName(iSelIndex), REGKEY_OVERRIDEPATH, hRegLocation);
}

void CSettingsOverridePaths::OnResetTab()
{
	if ( AfxMessageBox( "Are you sure you want to reset all Paths settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	for(int i = 0; i < VERFILE_QTY; i++)
	{
		CString csPath;
		csPath.Format(_T("%s%s"), csMulPath, GetMulFileName(i));
		m_clcPathList.SetItemText(i, 1, csPath);
		Main->SetMulPath(i, csPath);
		Main->DeleteRegistryValue(GetMulFileName(i), REGKEY_OVERRIDEPATH, hRegLocation);
	}
	m_cbResetPath.EnableWindow(false);
	UpdateData(false);
}

void CSettingsOverridePaths::OnFillPaths()
{
	for(int i = 0; i < VERFILE_QTY; i++)
	{
		m_clcPathList.InsertItem(i, GetMulFileName(i) );
		m_clcPathList.SetItemText(i, 1, Main->GetMulPath(i));
	}
}

void CSettingsOverridePaths::OnPathchangedItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		// this is the one that has been selected
		iSelIndex = m_clcPathList.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex == -1)
		{
			m_cbPathBrowse.EnableWindow(false);
			m_cbResetPath.EnableWindow(false);
			return;
		}
		m_clcPathList.SetHotItem(iSelIndex);
		m_cbPathBrowse.EnableWindow(true);
		CString csOrigPath;
		csOrigPath.Format(_T("%s%s"), csMulPath, GetMulFileName(iSelIndex));
		CString csPath = m_clcPathList.GetItemText(iSelIndex, 1);
		if (csOrigPath != csPath)
			m_cbResetPath.EnableWindow(true);
		else
			m_cbResetPath.EnableWindow(false);
		return;
	}
	m_cbPathBrowse.EnableWindow(false);
	m_cbResetPath.EnableWindow(false);
	*pResult = 0;
}