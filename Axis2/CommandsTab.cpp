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

// CommandsTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "CommandsTab.h"
#include "Common.h"
#include "TabCtrl.h"


// CCommandsTab dialog

IMPLEMENT_DYNAMIC(CCommandsTab, CDockingPage)

int CCommandsTab::iEditBtn = 0;
int CCommandsTab::iRemoveBtn = 0;

CCommandsTab::CCommandsTab() : CDockingPage(CCommandsTab::IDD,CMsg("IDS_COMMANDS"))
{
	//{{AFX_DATA_INIT(CCommandsTab)
	//}}AFX_DATA_INIT
}

CCommandsTab::~CCommandsTab()
{
}

void CCommandsTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommandsTab)
	DDX_Control(pDX, IDC_TAB, m_ctTab);
	DDX_Control(pDX, IDC_CAPTION, m_ceCaption);
	DDX_Control(pDX, IDC_COMMAND, m_ceCommand);
	DDX_Control(pDX, IDC_TABTEXT, m_ceTabText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommandsTab, CDockingPage)
	//{{AFX_DATA_MAP(CCommandsTab)
	ON_BN_CLICKED(IDC_ADDTAB, OnAddtab)
	ON_BN_CLICKED(IDC_DELTAB, OnDeltab)
	ON_BN_CLICKED(IDC_CLEARTAB, OnCleartab)
	ON_BN_CLICKED(IDC_ADDBTN, OnAddbutton)
	ON_BN_CLICKED(IDC_EDITBTN, OnEditbutton)
	ON_BN_CLICKED(IDC_REMOVEBTN, OnRemovebutton)
	ON_MESSAGE(WM_BUTTONPRESSED,ButtonPressed)
	//}}AFX_DATA_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpawnTab message handlers

BOOL CCommandsTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppCommandsTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CCommandsTab;
	Main->m_pcppCommandsTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	iNextID = 10000;
	LONG lStatus;
	HKEY hKey;
	m_ctTab.Init();
	Main->GetRegistryMultiSz("Command Tabs", &csaTabs, hRegLocation, REGKEY_COMMANDS);
		if ( csaTabs.GetSize() > 0 )
		{
			for ( int i = 0; i <= csaTabs.GetUpperBound(); i++ )
			{
				int iIndex = 0;
				m_ctTab.InsertItem(i, csaTabs.GetAt(i));
				CString csKey;
				csKey.Format("%s\\%s",REGKEY_COMMANDS ,csaTabs.GetAt(i));
				lStatus = RegOpenKeyEx(hRegLocation, csKey, 0, KEY_ALL_ACCESS, &hKey);
				if ( lStatus == ERROR_SUCCESS )
				{
					while (lStatus == ERROR_SUCCESS)
					{
						char szBuffer[MAX_PATH];
						DWORD dwSize = sizeof(szBuffer);
						DWORD dwType;
						unsigned char szData[MAX_PATH];
						DWORD dwDataSize = sizeof(szData);
						lStatus = RegEnumValue( hKey, iIndex, &szBuffer[0], &dwSize, 0, &dwType, &szData[0], &dwDataSize );
						if (lStatus == ERROR_SUCCESS)
						{
							m_ctTab.CreateButton(szBuffer,iNextID,i,0,0,0,72);
							iNextID++;
						}
						iIndex++;
					}
					RegCloseKey(hKey);
				}
				m_ctTab.SortButtons(i);
			}
		}
		else
		{
			m_ctTab.InsertItem(0, "Misc");
			csaTabs.InsertAt(0, "Misc");
			Main->PutRegistryMultiSz("Command Tabs", &csaTabs, hRegLocation, REGKEY_COMMANDS);
		}
	return TRUE;
}

void CCommandsTab::OnAddtab()
{
	iEditBtn = 0;
	iRemoveBtn = 0;
	int i = m_ctTab.GetCurFocus()+1;
	CString csLabel, csError;
	m_ceTabText.GetWindowText(csLabel);
	if (csLabel == "")
	{
		AfxMessageBox("You must enter a Label", MB_ICONSTOP);
		return;
	}

	if ( csaTabs.GetSize() > 0 )
	{
		for ( int i = 0; i <= csaTabs.GetUpperBound(); i++ )
		{
			if(csaTabs.GetAt(i) == csLabel)
			{
				csError.Format("Tab '%s' already exists" , csLabel);
				AfxMessageBox(csError, MB_ICONSTOP);
				return;
			}
		}
	}

	m_ctTab.InsertItem(i, csLabel);
	m_ctTab.SpreadTabs(i);
	m_ctTab.SetCurSel(i);
	m_ctTab.ShowTab(i);
	csaTabs.InsertAt(i, csLabel);
	Main->PutRegistryMultiSz("Command Tabs", &csaTabs, hRegLocation, REGKEY_COMMANDS);
}

void CCommandsTab::OnDeltab()
{
	iEditBtn = 0;
	iRemoveBtn = 0;
	if ( csaTabs.GetSize() == 1 )
	{
		AfxMessageBox("You can't delete the last tab", MB_ICONSTOP);
		return;
	}

	int i = m_ctTab.GetCurFocus();
	if (m_ctTab.GetCurFocus() == -1)
	{
		AfxMessageBox("No tab selected", MB_ICONSTOP);
		return;
	}
	CString csMsg;
	csMsg.Format("Are you sure you want to delete the \"%s\" Tab?", csaTabs.GetAt(i));
	if ( AfxMessageBox( csMsg, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;
	CString csKey;
	csKey.Format("%s\\%s",REGKEY_COMMANDS ,csaTabs.GetAt(i));
	Main->DeleteRegistryKey(csKey);
	m_ctTab.ClearTab(i);
	m_ctTab.ResetTab(i);
	m_ctTab.DeleteItem(i);
	m_ctTab.SetCurSel(i);

	if (m_ctTab.GetCurFocus() == -1)
		m_ctTab.SetCurSel(i-1);

	m_ctTab.FocusTab(m_ctTab.GetCurFocus());
	csaTabs.RemoveAt(i);
	Main->PutRegistryMultiSz("Command Tabs", &csaTabs, hRegLocation, REGKEY_COMMANDS);
}

void CCommandsTab::OnCleartab()
{
	iEditBtn = 0;
	iRemoveBtn = 0;
	int i = m_ctTab.GetCurFocus();
	if (m_ctTab.GetCurFocus() == -1)
	{
		AfxMessageBox("No tab selected", MB_ICONSTOP);
		return;
	}
	CString csMsg;
	csMsg.Format("Are you sure you want to clear the \"%s\" Tab?", csaTabs.GetAt(i));
	if ( AfxMessageBox( csMsg, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;
	CString csKey;
	csKey.Format("%s\\%s",REGKEY_COMMANDS, csaTabs.GetAt(i));
	Main->DeleteRegistryKey(csKey);
	m_ctTab.ClearTab(i);
}

void CCommandsTab::OnAddbutton()
{
	iEditBtn = 0;
	iRemoveBtn = 0;
	if (m_ctTab.GetCurFocus() == -1)
	{
		AfxMessageBox("No tab selected", MB_ICONSTOP);
		return;
	}
	if (m_ctTab.GetButtonCount(m_ctTab.GetCurFocus()) >= 36)
	{
		AfxMessageBox("Tab is Full", MB_ICONSTOP);
		return;
	}
	CString csCaption, csCommand, csKey, csTab;
	m_ceCaption.GetWindowText(csCaption);
	m_ceCommand.GetWindowText(csCommand);
	if (csCaption == "")
	{
		AfxMessageBox("You must enter a caption", MB_ICONSTOP);
		return;
	}
	if (csCommand == "")
	{
		AfxMessageBox("You must enter a command", MB_ICONSTOP);
		return;
	}

	csTab = csaTabs.GetAt(m_ctTab.GetCurFocus());
	csKey.Format("%s\\%s", REGKEY_COMMANDS, csTab);

	LONG lStatus;
	HKEY hKey;
	int iIndex = 0;
	lStatus = RegOpenKeyEx(hRegLocation, csKey, 0, KEY_ALL_ACCESS, &hKey);
	if ( lStatus == ERROR_SUCCESS )
	{
		while (lStatus == ERROR_SUCCESS)
		{
			char szBuffer[MAX_PATH];
			DWORD dwSize = sizeof(szBuffer);
			DWORD dwType;
			unsigned char szData[MAX_PATH];
			DWORD dwDataSize = sizeof(szData);
			lStatus = RegEnumValue( hKey, iIndex, &szBuffer[0], &dwSize, 0, &dwType, &szData[0], &dwDataSize );
			if (lStatus == ERROR_SUCCESS)
			{
				if (csCaption == szBuffer)
				{
					AfxMessageBox("This button already exist on this tab", MB_ICONSTOP);
					return;
				}
			}
			iIndex++;
		}
		RegCloseKey(hKey);
	}

	m_ctTab.CreateButton(csCaption,iNextID,m_ctTab.GetCurFocus(),0,0,0,72);
	m_ctTab.SortButtons(m_ctTab.GetCurFocus());

	Main->PutRegistryString(csCaption, csCommand, hRegLocation, csKey);
	iNextID++;
}

void CCommandsTab::OnEditbutton()
{
	iRemoveBtn = 0;
	if (m_ctTab.GetButtonCount(m_ctTab.GetCurFocus()) == 0)
	{
		AfxMessageBox("No button to edit", MB_ICONSTOP);
		return;
	}
	if ( AfxMessageBox( "Select a button to edit", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
	{
		iEditBtn = 0;
		return;
	}
	iEditBtn = 1;
}

void CCommandsTab::OnRemovebutton()
{
	iEditBtn = 0;
	if (m_ctTab.GetButtonCount(m_ctTab.GetCurFocus()) == 0)
	{
		AfxMessageBox("No button to remove", MB_ICONSTOP);
		return;
	}
	if ( AfxMessageBox( "Select a button to remove", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
	{
		iRemoveBtn = 0;
		return;
	}
	iRemoveBtn = 1;
}

LRESULT CCommandsTab::ButtonPressed(WPARAM w, LPARAM l)
{
	UNREFERENCED_PARAMETER(l);
	CString csCaption, csCmd, csTab, csKey;
	m_ctTab.GetDlgItem((int)w)->GetWindowText(csCaption);
	csTab = csaTabs.GetAt(m_ctTab.GetCurFocus());
	csKey.Format("%s\\%s", REGKEY_COMMANDS, csTab);

	if (iEditBtn == 1)
	{	
		//Delete old button
		Main->DeleteRegistryValue(csCaption, csKey, hRegLocation);

		//Add new button
		m_ceCaption.GetWindowText(csCaption);
		m_ceCommand.GetWindowText(csCmd);
		Main->PutRegistryString(csCaption, csCmd, hRegLocation, csKey);
		m_ctTab.GetDlgItem((int)w)->SetWindowText(csCaption);
		iEditBtn = 0;
		return 0;
	}

	if (iRemoveBtn == 1)
	{	
		CString csMsg;
		csMsg.Format("Are you sure you want to remove the \"%s\" Button?", csCaption);
		if ( AfxMessageBox( csMsg, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		{
			iRemoveBtn = 0;
			return 0;
		}
		//Delete button
		Main->DeleteRegistryValue(csCaption, csKey, hRegLocation);
		m_ctTab.RemoveButton((int) w);
		m_ctTab.SortButtons(m_ctTab.GetCurFocus());
		iRemoveBtn = 0;
		return 0;
	}

	csCmd = Main->GetRegistryString(csCaption, "", hRegLocation, csKey);
	m_ceCaption.SetWindowText(csCaption);
	m_ceCommand.SetWindowText(csCmd);
	SendToUO(csCmd);
	
	return 0;
}