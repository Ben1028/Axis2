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

#include "stdafx.h"
#include "Axis2.h"
#include "RemoteConsoleLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleLoginDlg dialog


CRemoteConsoleLoginDlg::CRemoteConsoleLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteConsoleLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteConsoleLoginDlg)
	//}}AFX_DATA_INIT
	m_iSel = 0;
}


void CRemoteConsoleLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteConsoleLoginDlg)
	DDX_Control(pDX, IDC_ACCTNAME, m_ccbName);
	DDX_Control(pDX, IDC_ACCT, m_ceAccount);
	DDX_Control(pDX, IDC_ADDRESS, m_ceAddress);
	DDX_Control(pDX, IDC_PORT, m_cePort);
	DDX_Control(pDX, IDC_PWD, m_cePassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteConsoleLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteConsoleLoginDlg)
	ON_CBN_SELCHANGE(IDC_ACCTNAME, OnSelchangeAcctname)
	ON_CBN_EDITCHANGE(IDC_ACCTNAME, OnEditchangeAcctname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleLoginDlg message handlers

BOOL CRemoteConsoleLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HKEY hKey;
	LONG lStatus;
	lStatus = RegOpenKeyEx(hRegLocation, REGKEY_CONSOLE, 0, KEY_ALL_ACCESS, &hKey);
	int iIndex = 0;
	if ( lStatus == ERROR_SUCCESS )
	{
		while (lStatus == ERROR_SUCCESS)
		{
			char szBuffer[MAX_PATH];
			DWORD dwSize = sizeof(szBuffer);
			lStatus = RegEnumKeyEx(hKey, iIndex, &szBuffer[0], &dwSize, 0, NULL, NULL, NULL);
			if (lStatus == ERROR_SUCCESS)
			{
				CString csProfile;
				csProfile.Format("%s", szBuffer);
				m_ccbName.AddString(csProfile);
			}
			iIndex++;
		}
		RegCloseKey(hKey);
	}

	// Now select the last selected login

	CString csLast = Main->GetRegistryString("LastRemoteLoginAccount");
	if ( csLast.IsEmpty() )
		m_ccbName.SetCurSel(-1);
	else
		m_ccbName.SelectString(0, csLast);
	OnSelchangeAcctname();

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteConsoleLoginDlg::OnOK() 
{
	// Save the current stuff
	UpdateData(TRUE);
	m_ceAddress.GetWindowText(m_csAddress);
	m_cePort.GetWindowText(m_csPort);
	m_ceAccount.GetWindowText(m_csAccount);
	m_cePassword.GetWindowText(m_csPassword);
	m_ccbName.GetWindowText(m_csAccountName);
	CString csKey;
	csKey.Format("%s\\%s", REGKEY_CONSOLE, m_csAccountName);
	Main->PutRegistryString("Address", m_csAddress, hRegLocation, csKey);
	Main->PutRegistryString("Port", m_csPort, hRegLocation, csKey);
	Main->PutRegistryString("Account", m_csAccount, hRegLocation, csKey);
	Main->PutRegistryString("Password", Encrypt(m_csPassword), hRegLocation, csKey);
	Main->PutRegistryString("LastRemoteLoginAccount", m_csAccountName);

	CDialog::OnOK();
}

void CRemoteConsoleLoginDlg::OnSelchangeAcctname() 
{
	UpdateData(TRUE);
	// Save the previous stuff?
	CString csKey;
	// Populate the fields with the values of the new selection
	m_iSel = m_ccbName.GetCurSel();
	if ( m_iSel == -1 )
	{
		m_ceAccount.SetWindowText("");
		m_cePassword.SetWindowText("");
		m_ceAddress.SetWindowText("");
		m_cePort.SetWindowText("");
		UpdateData(FALSE);
		return;
	}
	m_ccbName.GetLBText(m_iSel, m_csAccountName);
	csKey.Format("%s\\%s", REGKEY_CONSOLE, m_csAccountName);
	m_ceAddress.SetWindowText(Main->GetRegistryString("Address", "", hRegLocation, csKey));
	m_cePort.SetWindowText(Main->GetRegistryString("Port", "", hRegLocation, csKey));
	m_ceAccount.SetWindowText(Main->GetRegistryString("Account", "", hRegLocation, csKey));
	m_cePassword.SetWindowText(Encrypt(Main->GetRegistryString("Password", "", hRegLocation, csKey)));

	// Update the last account setting
	Main->PutRegistryString("LastRemoteLoginAccount", m_csAccountName);
	UpdateData(FALSE);
}

void CRemoteConsoleLoginDlg::OnEditchangeAcctname() 
{
	m_ceAccount.SetWindowText("");
	m_cePassword.SetWindowText("");
	m_ceAddress.SetWindowText("");
	m_cePort.SetWindowText("");
	UpdateData(FALSE);
	m_iSel = -1;
}
