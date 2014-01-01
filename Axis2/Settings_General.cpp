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

// Settings_General.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Settings_General.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneral property page

IMPLEMENT_DYNCREATE(CSettingsGeneral, CPropertyPage)

CSettingsGeneral::CSettingsGeneral() : CPropertyPage(CSettingsGeneral::IDD)
{
	//{{AFX_DATA_INIT(CSettingsGeneral)
	m_bAllowMultiple = FALSE;
	m_bAlwaysOnTop = FALSE;
	m_bSysClose = FALSE;
	m_bLoadDefault = FALSE;
	//}}AFX_DATA_INIT
}


CSettingsGeneral::~CSettingsGeneral()
{
}


void CSettingsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsGeneral)
	DDX_Check(pDX, IDC_ALWAYS_ONTOP, m_bAlwaysOnTop);
	DDX_Check(pDX, IDC_SYSCLOSE, m_bSysClose);
	DDX_Check(pDX, IDC_LOADDEFAULT, m_bLoadDefault);
	DDX_Check(pDX, IDC_ALLOWMULTIPLE, m_bAllowMultiple);
	DDX_Control(pDX, IDC_STARTTAB, m_ccbStartTab);
	DDX_Control(pDX, IDC_RESETSTING, m_cbResetSettings);
	DDX_Control(pDX, IDC_PREFIX, m_ceCommandPrefix);
	DDX_Control(pDX, IDC_LANGUAGE, m_ccbLanguage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsGeneral)
	ON_BN_CLICKED(IDC_ALLOWMULTIPLE, OnAllowMultiple)
	ON_BN_CLICKED(IDC_ALWAYS_ONTOP, OnAlwaysOnTop)
	ON_BN_CLICKED(IDC_SYSCLOSE, OnSysClose)
	ON_BN_CLICKED(IDC_LOADDEFAULT, OnLoadDefault)
	ON_CBN_SELCHANGE(IDC_STARTTAB, OnSelchangeStartTab)
	ON_BN_CLICKED(IDC_RESETSTING, OnResetSettings)
	ON_BN_CLICKED(IDC_RESET_GENERAL, OnResetTab)
	ON_EN_CHANGE(IDC_PREFIX, OnChangePrefix)
	ON_CBN_SELCHANGE(IDC_LANGUAGE, OnSelchangeLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneral message handlers

BOOL CSettingsGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bAlwaysOnTop = (BOOL) Main->m_dwAlwaysOnTop;
	m_bSysClose = (BOOL) Main->m_dwSysClose;
	m_bAllowMultiple = (BOOL) Main->m_dwAllowMultiple;
	m_bLoadDefault = (BOOL) Main->m_dwLoadDefault;
	m_ccbStartTab.SetCurSel(Main->m_dwStartTab);
	m_ceCommandPrefix.SetWindowText(Main->m_csCommandPrefix);

	UpdateData(false);

	return TRUE;
}

void CSettingsGeneral::OnAllowMultiple()
{
	UpdateData();
	Main->PutRegistryDword("AllowMultipleInstances", m_bAllowMultiple);
	Main->m_dwAllowMultiple = m_bAllowMultiple;
}

void CSettingsGeneral::OnAlwaysOnTop()
{
	UpdateData();
	Main->PutRegistryDword("AlwaysOnTop", m_bAlwaysOnTop);
	Main->m_dwAlwaysOnTop = m_bAlwaysOnTop;

	Main->m_pMainWnd->SetWindowPos(Main->m_dwAlwaysOnTop ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CSettingsGeneral::OnSysClose()
{
	UpdateData();
	Main->PutRegistryDword("SysClose", m_bSysClose);
	Main->m_dwSysClose = m_bSysClose;
}

void CSettingsGeneral::OnLoadDefault()
{
	UpdateData();
	Main->PutRegistryDword("LoadDefault", m_bLoadDefault);
	Main->m_dwLoadDefault = m_bLoadDefault;
}

void CSettingsGeneral::OnSelchangeStartTab() 
{
	UpdateData();
	int iSel = m_ccbStartTab.GetCurSel();
	Main->PutRegistryDword("StartTab", iSel);
	Main->m_dwStartTab = iSel;
}

void CSettingsGeneral::OnSelchangeLanguage()
{
	UpdateData();
	//int iSel = m_ccbLanguage.GetCurSel();
	//Main->PutRegistryString(("Language", iSel);
}

void CSettingsGeneral::OnChangePrefix()
{
	CString csPrefix;
	m_ceCommandPrefix.GetWindowText(csPrefix);
	Main->PutRegistryString("CommandPrefix", csPrefix);
	Main->m_csCommandPrefix = csPrefix;
}

void CSettingsGeneral::OnResetTab()
{
	if ( AfxMessageBox( "Are you sure you want to reset the General settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	Main->LoadIni(0,"AlwaysOnTop");
	m_bAlwaysOnTop = (BOOL) Main->m_dwAlwaysOnTop;
	Main->PutRegistryDword("AlwaysOnTop", m_bAlwaysOnTop);
	Main->LoadIni(0,"SysClose");
	m_bSysClose = (BOOL) Main->m_dwSysClose;
	Main->PutRegistryDword("SysClose", m_bSysClose);
	Main->LoadIni(0,"AllowMultiple");
	m_bAllowMultiple = (BOOL) Main->m_dwAllowMultiple;
	Main->PutRegistryDword("AllowMultipleInstances", m_bAllowMultiple);
	Main->LoadIni(0,"LoadDefault");
	m_bLoadDefault = (BOOL) Main->m_dwLoadDefault;
	Main->PutRegistryDword("LoadDefault", m_bLoadDefault);
	Main->LoadIni(0,"StartTab");
	m_ccbStartTab.SetCurSel(Main->m_dwStartTab);
	Main->PutRegistryDword("StartTab", Main->m_dwStartTab);
	Main->LoadIni(0,"CommandPrefix");
	m_ceCommandPrefix.SetWindowText(Main->m_csCommandPrefix);
	Main->PutRegistryString("CommandPrefix", Main->m_csCommandPrefix);

	UpdateData(false);
}

void CSettingsGeneral::OnResetSettings()
{
	if ( AfxMessageBox( "Are you sure you want to reset all settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	//Only load default settings from Axis.ini
	Main->LoadIni(0);

	//General Tab
	m_bAlwaysOnTop = (BOOL) Main->m_dwAlwaysOnTop;
	Main->PutRegistryDword("AlwaysOnTop", m_bAlwaysOnTop);
	m_bSysClose = (BOOL) Main->m_dwSysClose;
	Main->PutRegistryDword("SysClose", m_bSysClose);
	m_bAllowMultiple = (BOOL) Main->m_dwAllowMultiple;
	Main->PutRegistryDword("AllowMultipleInstances", m_bAllowMultiple);
	m_bLoadDefault = (BOOL) Main->m_dwLoadDefault;
	Main->PutRegistryDword("LoadDefault", m_bLoadDefault);
	m_ccbStartTab.SetCurSel(Main->m_dwStartTab);
	Main->PutRegistryDword("StartTab", Main->m_dwStartTab);
	m_ceCommandPrefix.SetWindowText(Main->m_csCommandPrefix);
	Main->PutRegistryString("CommandPrefix", Main->m_csCommandPrefix);

	//Item Tab
	Main->m_pcppSetItem->m_bRoomView = (BOOL) Main->m_dwRoomView;
	Main->PutRegistryDword("RoomView", Main->m_dwRoomView);
	Main->m_pcppSetItem->m_bShowItems = (BOOL) Main->m_dwShowItems;
	Main->PutRegistryDword("ShowItems", Main->m_dwShowItems);
	Main->m_pcppSetItem->m_cbRoomView.EnableWindow(Main->m_dwShowItems? true : false);
	Main->m_pcppSetItem->m_dwItemBGColor = Main->m_dwItemBGColor;
	Main->PutRegistryDword("ItemBGColor", Main->m_dwItemBGColor);

	//Travel Tab
	Main->m_pcppSetTravel->m_bDrawStatics = (BOOL) Main->m_dwDrawStatics;
	Main->PutRegistryDword("DrawStatics", Main->m_dwDrawStatics);
	Main->m_pcppSetTravel->m_bDrawDifs = (BOOL) Main->m_dwDrawDifs;
	Main->PutRegistryDword("DrawDifs", Main->m_dwDrawDifs);
	Main->m_pcppSetTravel->m_bShowSpawnpoints = (BOOL) Main->m_dwShowSpawnpoints;
	Main->PutRegistryDword("ShowSpawnpoints", Main->m_dwShowSpawnpoints);
	Main->m_pcppSetTravel->m_bShowMap = (BOOL) Main->m_dwShowMap;
	Main->PutRegistryDword("ShowMap", Main->m_dwShowMap);
	Main->m_pcppSetTravel->m_dwNPCSpawnColor = Main->m_dwNPCSpawnColor;
	Main->PutRegistryDword("NPCSpawnColor", Main->m_dwNPCSpawnColor);
	Main->m_pcppSetTravel->m_dwItemSpawnColor = Main->m_dwItemSpawnColor;
	Main->PutRegistryDword("ItemSpawnColor", Main->m_dwItemSpawnColor);
	
	Main->m_pcppSetTravel->m_csItemColor.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	Main->m_pcppSetTravel->m_csNpcColor.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	Main->m_pcppSetTravel->m_stNPCCol.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	Main->m_pcppSetTravel->m_stItemCol.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	Main->m_pcppSetTravel->m_cbDrawStatics.EnableWindow(Main->m_dwShowMap? true : false);
	Main->m_pcppSetTravel->m_cbDrawDifs.EnableWindow(Main->m_dwShowMap? true : false);
	Main->m_pcppSetTravel->m_cbShowSpawnpoints.EnableWindow(Main->m_dwShowMap? true : false);

	//Spawn Tab
	Main->m_pcppSetSpawn->m_bShowNPCs = (BOOL) Main->m_dwShowNPCs;
	Main->PutRegistryDword("ShowNPCs", Main->m_dwShowNPCs);
	Main->m_pcppSetSpawn->m_dwSpawnBGColor = Main->m_dwSpawnBGColor;
	Main->PutRegistryDword("SpawnBGColor", Main->m_dwSpawnBGColor);

	//Override Paths Tab
	for(int i = 0; i < VERFILE_QTY; i++)
	{
		CString csPath;
		csPath.Format(_T("%s%s"), csMulPath, GetMulFileName(i));
		if(Main->m_pcppSetOverridePaths->m_clcPathList)
			Main->m_pcppSetOverridePaths->m_clcPathList.SetItemText(i, 1, csPath);
		Main->SetMulPath(i, csPath);
		Main->DeleteRegistryValue(GetMulFileName(i), REGKEY_OVERRIDEPATH, hRegLocation);
	}
	Main->m_pcppSetOverridePaths->m_cbResetPath.EnableWindow(false);

	//Paths Tab
	Main->m_pcppSetPaths->m_bSameAsClient = (BOOL) Main->m_dwSameAsClient;
	Main->PutRegistryDword("SameAsClient", Main->m_dwSameAsClient);
	CString csDefUOPath = Main->GetRegistryString("ExePath", "", HKEY_LOCAL_MACHINE, "SOFTWARE\\Origin Worlds Online\\Ultima Online\\1.0");
	if ( csDefUOPath != "" )
	{
		csUOPath = csDefUOPath;
		Main->PutRegistryString("Default Client", csUOPath);
		if (Main->m_pcppSetPaths->m_csDefaultClient)
			Main->m_pcppSetPaths->m_csDefaultClient.SetWindowText(csUOPath);

		if (Main->m_dwSameAsClient)
		{
			csMulPath = csUOPath;
			csMulPath.SetAt(csUOPath.ReverseFind('\\')+1, 0x00);
			Main->PutRegistryString("Default MulPath", csMulPath);
			if (Main->m_pcppSetPaths->m_csDefaultMulPath)
				Main->m_pcppSetPaths->m_csDefaultMulPath.SetWindowText(csMulPath);
			Main->InitializeMulPaths();
		}
	}
	Main->m_pcppSetPaths->m_cbMulBrowse.EnableWindow(Main->m_dwSameAsClient ? false : true);
	Main->m_pcppSetPaths->m_csDefaultMulPath.EnableWindow(Main->m_dwSameAsClient ? false : true);

	UpdateData(false);
}