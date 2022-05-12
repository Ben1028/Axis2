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

// Axis2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Axis2Dlg.h"
#include "stdio.h"
#include "AboutDlg.h"
#include "SettingsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxis2Dlg

IMPLEMENT_DYNAMIC(CAxis2Dlg, CPropertySheet)

CAxis2Dlg::CAxis2Dlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAxis2Dlg::CAxis2Dlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInit = false;
}

CAxis2Dlg::~CAxis2Dlg()
{
	m_nid.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}


BEGIN_MESSAGE_MAP(CAxis2Dlg, CPropertySheet)
	//{{AFX_MSG_MAP(CAxis2Dlg)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_MOVE()
	ON_COMMAND(ID_SETTINGS_GENERAL, OnSettingsGeneral)
	ON_COMMAND(ID_SETTINGS_FILEPATHS, OnSettingsPaths)
	ON_COMMAND(ID_SETTINGS_ITEMTAB, OnSettingsItem)
	ON_COMMAND(ID_SETTINGS_TRAVELTAB, OnSettingsTravel)
	ON_COMMAND(ID_SETTINGS_SPAWNTAB, OnSettingsSpawn)
	ON_COMMAND(ID_SETTINGS_OVERRIDEPATHS, OnSettingsOverridePaths)
	ON_COMMAND(ID_PROFILES_OPTION, OnOpenProfileOption)
	ON_COMMAND(ID_PROFILES_UNLOAD, OnUnloadProfile)
	ON_COMMAND(ID_PROFILES_LOADDEFAULT, OnLoadDefProfile)
	ON_COMMAND(ID_PROFILES_LOADLASTPROFILE, OnLoadLastProfile)
	ON_COMMAND(ID_HELP_DOCUMENTATION, OnHelp)
	ON_COMMAND(ID_HELP_ABOUTAXIS2, OnAboutDlg)
	ON_COMMAND(ID_EXIT_CLOSEAXIS2, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxis2Dlg message handlers

BOOL CAxis2Dlg::OnInitDialog() 
{

	BOOL bResult = CPropertySheet::OnInitDialog();

	Main->pDefMenu.LoadMenu(IDR_MENU1);
	SetMenu(&Main->pDefMenu);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		int iCount = pSysMenu->GetMenuItemCount();
		for (int i = 0; i < iCount; i++)
		{
			CString csItem;
			pSysMenu->GetMenuString(i, csItem, MF_BYPOSITION);
			if (csItem.Find("Ma&ximize") != -1)
				pSysMenu->EnableMenuItem(i, MF_BYPOSITION | MF_GRAYED);
			if (csItem.Find("&Size") != -1)
				pSysMenu->EnableMenuItem(i, MF_BYPOSITION | MF_GRAYED);
		}
	}

	Main->UpdateProfileMenu();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	int x, y;
	x = -1;
	y = -1;
	CString csX, csY;
	csX = Main->m_csPosition.SpanExcluding(",");
	if ( csX != "" )
		x = atoi(csX);
	if ( Main->m_csPosition.Find(",") != -1 )
		csY = Main->m_csPosition.Mid(Main->m_csPosition.Find(",") + 1 );
	if ( csY != "" )
		y = atoi(csY);
	int X, Y;
	X = GetSystemMetrics(SM_CXFULLSCREEN)-20;
	Y = GetSystemMetrics(SM_CYFULLSCREEN)-20;
	CRect rectDlg;
	GetWindowRect(rectDlg);
	if ( x >= 0 && y >= 0 && x <= X && y <= Y )
		this->SetWindowPos( NULL, x, y, rectDlg.Width(), rectDlg.Height()+23, SWP_NOZORDER );

	m_bInit = true;
	return bResult;
}

CAxis2Dlg::CAxis2Dlg()
{
	CAxis2Dlg(CFMsg(CMsg("IDS_AXISTITLE"), Main->GetVersionTitle()));
}

int CAxis2Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_nid.cbSize = sizeof(m_nid);
	m_nid.hWnd = this->GetSafeHwnd();
	m_nid.uID = 1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_USER;
	m_nid.hIcon = m_hIcon;
	CString csTip;
	csTip.Format("%s (%s)", Main->GetVersionTitle(), Main->m_csCurrentProfile);
	strcpy_s(m_nid.szTip,sizeof(m_nid.szTip), csTip);
	Shell_NotifyIcon(NIM_ADD, &m_nid);
	m_bModeless = 1;
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle(0, WS_MINIMIZEBOX);

	Main->m_dlgToolBar = new CAxis2LBar;
	Main->m_dlgToolBar->Create(IDD_TOOLBAR);
	Main->m_dlgToolBar->ShowWindow(SW_HIDE);
	
	return 0;
}


LRESULT CAxis2Dlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_USER)
	{
		if (lParam == WM_LBUTTONDOWN)
		{
			this->ShowWindow(SW_SHOWNORMAL);
			Main->m_dlgToolBar->ShowWindow(SW_HIDE);
			return TRUE;
		}
	}
	if (message == WM_SYSCOMMAND)
	{
		if (wParam == SC_MINIMIZE)
		{
			this->ShowWindow(SW_HIDE);
			if (!Main->m_dwDisableToolbar)
				Main->m_dlgToolBar->ShowWindow(SW_SHOWNORMAL);
			return TRUE;
		}
	}
	if (message == WM_DESTROY)
	{
		this->PressButton(PSBTN_OK);
		return TRUE;
	}
	return CPropertySheet::DefWindowProc(message, wParam, lParam);
}

void CAxis2Dlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CLOSE)
	{
		if (Main->m_dwSysClose)
			this->PressButton(PSBTN_OK);
		else
			ShowWindow(SW_HIDE);
		return;
	}
	CPropertySheet::OnSysCommand(nID, lParam);
}

void CAxis2Dlg::OnMove(int x, int y) 
{
	CPropertySheet::OnMove(x, y);

	if ( !m_bInit )
		return;

	WINDOWPLACEMENT place;
	this->GetWindowPlacement(&place);
	HKEY hKey;
	DWORD dwDisp;
	LONG lStatus = RegCreateKeyEx(hRegLocation, REGKEY_AXIS, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
	if (lStatus == ERROR_SUCCESS)
	{
		Main->m_csPosition.Format("%ld,%ld", place.rcNormalPosition.left, place.rcNormalPosition.top);
		lStatus = RegSetValueEx( hKey, "Position", 0, REG_SZ, ((BYTE *) Main->m_csPosition.GetBuffer(Main->m_csPosition.GetLength())), Main->m_csPosition.GetLength() );
	}
	RegCloseKey( hKey );
}

void CAxis2Dlg::UpdateTip()
{
	m_nid.cbSize = sizeof(m_nid);
	m_nid.hWnd = this->GetSafeHwnd();
	m_nid.uID = 1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_USER;
	m_nid.hIcon = m_hIcon;
	CString csTip;
	csTip.Format("%s (%s)", Main->GetVersionTitle(), Main->m_csCurrentProfile);
	strcpy_s(m_nid.szTip,sizeof(m_nid.szTip), csTip);
	Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

void CAxis2Dlg::ReloadActiveTabPage()
{
	int active = this->GetActiveIndex();
	this->SetActivePage(-1);
	this->SetActivePage(active);
}

//Settings Menu

void CAxis2Dlg::OnOpenSettingsPage(int iPage)
{
	CSettingsDlg dlg(CMsg("IDS_SETTINGS"));
	dlg.m_psh.dwFlags = PSH_NOAPPLYNOW | PSH_PROPSHEETPAGE;
	dlg.EnableStackedTabs( false );
	dlg.AddPage(Main->m_pcppSetGeneral);
	dlg.AddPage(Main->m_pcppSetPaths);
	dlg.AddPage(Main->m_pcppSetItem);
	dlg.AddPage(Main->m_pcppSetTravel);
	dlg.AddPage(Main->m_pcppSetSpawn);
	dlg.AddPage(Main->m_pcppSetOverridePaths);

	dlg.SetActivePage(iPage);
	dlg.DoModal();
}

void CAxis2Dlg::OnSettingsGeneral()
{
	OnOpenSettingsPage(0);
}

void CAxis2Dlg::OnSettingsPaths()
{
	OnOpenSettingsPage(1);
}

void CAxis2Dlg::OnSettingsItem()
{
	OnOpenSettingsPage(2);
}

void CAxis2Dlg::OnSettingsTravel()
{
	OnOpenSettingsPage(3);
}

void CAxis2Dlg::OnSettingsSpawn()
{
	OnOpenSettingsPage(4);
}

void CAxis2Dlg::OnSettingsOverridePaths()
{
	OnOpenSettingsPage(5);
}


//Profile Menu
void CAxis2Dlg::OnOpenProfileOption()
{
	if (Main->m_dlgProfile)
		delete Main->m_dlgProfile;
	Main->m_dlgProfile = new CProfileDLG;
	Main->m_dlgProfile->Create(IDD_PROFILE_DLG);
}

void CAxis2Dlg::OnUnloadProfile()
{
	Main->m_pScripts->UnloadProfile();
	Main->LoadIni(1);
	Main->LoadIni(2);
	Main->m_csCurrentProfile = CMsg(IDS_NONE);
	UpdateTip();
	AfxBeginThread(LoadProfileThread,(LPVOID)0);
}

void CAxis2Dlg::OnLoadDefProfile()
{
	Main->m_pScripts->UnloadProfile();
	Main->LoadIni(1);
	Main->LoadIni(2);
	Main->m_csCurrentProfile = Main->GetRegistryString("Default Profile");
	UpdateTip();
	AfxBeginThread(LoadProfileThread,(LPVOID)1);
}

void CAxis2Dlg::OnLoadLastProfile()
{
	Main->m_pScripts->UnloadProfile();
	Main->LoadIni(1);
	Main->LoadIni(2);
	Main->m_csCurrentProfile = Main->GetRegistryString("Last Profile Loaded");
	UpdateTip();
	AfxBeginThread(LoadProfileThread,(LPVOID)0);
}


//Help Menu

void CAxis2Dlg::OnHelp()
{
	HtmlHelp((DWORD_PTR)"Axis2.chm::/Welcome.htm", HH_DISPLAY_TOPIC);
}

void CAxis2Dlg::OnAboutDlg() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CAxis2Dlg::OnClose() 
{
	PressButton(PSBTN_OK);
}