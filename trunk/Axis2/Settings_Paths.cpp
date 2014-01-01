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
#include "Settings_Paths.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsPaths property page

IMPLEMENT_DYNCREATE(CSettingsPaths, CPropertyPage)

CSettingsPaths::CSettingsPaths() : CPropertyPage(CSettingsPaths::IDD)
{
	//{{AFX_DATA_INIT(CSettingsPaths)
	m_bSameAsClient = FALSE;
	//}}AFX_DATA_INIT
}


CSettingsPaths::~CSettingsPaths()
{
}


void CSettingsPaths::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsPaths)
	DDX_Check(pDX, IDC_AS_USESAMEPATHASCLIENT, m_bSameAsClient);
	DDX_Control(pDX, IDC_DEFAULTCLIENT, m_csDefaultClient);
	DDX_Control(pDX, IDC_DEFUALTMULPATH, m_csDefaultMulPath);
	DDX_Control(pDX, IDC_CLIENTBROWSE, m_cbClientBrowse);
	DDX_Control(pDX, IDC_AS_MULBROWSE, m_cbMulBrowse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsPaths, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsPaths)
	ON_BN_CLICKED(IDC_CLIENTBROWSE, OnClientbrowse)
	ON_BN_CLICKED(IDC_AS_MULBROWSE, OnMulBrowse)
	ON_BN_CLICKED(IDC_AS_USESAMEPATHASCLIENT, OnSameAsClient)
	ON_BN_CLICKED(IDC_RESET_PATHS, OnResetTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsPaths message handlers

BOOL CSettingsPaths::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bSameAsClient = (BOOL) Main->m_dwSameAsClient;

	m_csDefaultClient.SetWindowText(csUOPath);
	m_csDefaultMulPath.SetWindowText(csMulPath);

	m_cbMulBrowse.EnableWindow(m_bSameAsClient ? false : true);
	m_csDefaultMulPath.EnableWindow(m_bSameAsClient ? false : true);

	UpdateData(false);

	return TRUE;
}

void CSettingsPaths::OnClientbrowse() 
{
	CFileDialog dlg(TRUE, "exe", "client.exe", 0, "Application files (*.exe)|*.exe|All files (*.*)|*.*||", NULL);
	if ( dlg.DoModal() == IDOK )
	{
		csUOPath = dlg.GetPathName();
		Main->PutRegistryString("Default client", csUOPath);
		m_csDefaultClient.SetWindowText(csUOPath);
		if ( m_bSameAsClient )
		{
			CString csPath = csUOPath;
			csPath.SetAt(csUOPath.ReverseFind('\\')+1, 0x00);
			csMulPath = csPath;
			Main->PutRegistryString("Default MulPath", csPath);
			m_csDefaultMulPath.SetWindowText(csPath);
			Main->InitializeMulPaths();
		}
	}
}

void CSettingsPaths::OnMulBrowse()
{
	CFileDialog dlg(TRUE, "mul", "map0.mul", 0, "Application files (*.mul)|*.mul|All files (*.*)|*.*||", NULL);
	if ( dlg.DoModal() == IDOK )
	{
		CString csPath = dlg.GetPathName();
		csPath.SetAt(dlg.GetPathName().ReverseFind('\\')+1, 0x00);
		csMulPath = csPath;
		Main->PutRegistryString("Default MulPath", csPath);
		m_csDefaultMulPath.SetWindowText(csPath);
		Main->InitializeMulPaths();
	}
}

void CSettingsPaths::OnSameAsClient()
{
	UpdateData();
	Main->PutRegistryDword("SameAsClient", m_bSameAsClient);
	Main->m_dwSameAsClient = m_bSameAsClient;

	CString csPath = csUOPath;
	csPath.SetAt(csUOPath.ReverseFind('\\')+1, 0x00);
	csMulPath = csPath;
	Main->PutRegistryString("Default MulPath", csPath);
	m_csDefaultMulPath.SetWindowText(csPath);

	m_cbMulBrowse.EnableWindow(m_bSameAsClient ? false : true);
	m_csDefaultMulPath.EnableWindow(m_bSameAsClient ? false : true);
	Main->InitializeMulPaths();
}

void CSettingsPaths::OnResetTab()
{
	if ( AfxMessageBox( "Are you sure you want to reset the Paths settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	Main->LoadIni(0,"SameAsClient");
	m_bSameAsClient = (BOOL) Main->m_dwSameAsClient;
	Main->PutRegistryDword("SameAsClient", Main->m_dwSameAsClient);

	CString csDefUOPath = Main->GetRegistryString("ExePath", "", HKEY_LOCAL_MACHINE, "SOFTWARE\\Origin Worlds Online\\Ultima Online\\1.0");
	if ( csDefUOPath != "" )
	{
		csUOPath = csDefUOPath;
		Main->PutRegistryString("Default Client", csUOPath);
		m_csDefaultClient.SetWindowText(csUOPath);

		if (Main->m_dwSameAsClient)
		{
			csMulPath = csUOPath;
			csMulPath.SetAt(csUOPath.ReverseFind('\\')+1, 0x00);
			Main->PutRegistryString("Default MulPath", csMulPath);
			m_csDefaultMulPath.SetWindowText(csMulPath);
			Main->InitializeMulPaths();
		}
	}
	m_cbMulBrowse.EnableWindow(Main->m_dwSameAsClient ? false : true);
	m_csDefaultMulPath.EnableWindow(Main->m_dwSameAsClient ? false : true);

	UpdateData(false);
}