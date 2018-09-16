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

// Axis2.h : main header file for the AXIS2 application
//

#if !defined(AFX_AXIS2_H__A4D4BBF1_8874_4913_BDA8_293B666FB0A3__INCLUDED_)
#define AFX_AXIS2_H__A4D4BBF1_8874_4913_BDA8_293B666FB0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ScriptObjects.h"

#include "LogTab.h"
#include "ItemTab.h"
#include "SpawnTab.h"
#include "TravelTab.h"
#include "CommandsTab.h"
#include "Common.h"
#include "TabCtrl.h"
#include "PlayerTweakTab.h"
#include "ItemTweakTab.h"
#include "GeneralTab.h"
#include "LauncherTab.h"
#include "AccountTab.h"
#include "ReminderTab.h"
#include "UOart.h"
#include "ClientInfo.h"
#include "RemoteConsole.h"
#include "Axis2Dlg.h"
#include "WorldMap.h"
#include "DockingPage.h"
#include "DoorWizard.h"
#include "LightWizard.h"
#include "SettingsDlg.h"
#include "Settings_General.h"
#include "Settings_Paths.h"
#include "Settings_Item.h"
#include "Settings_Travel.h"
#include "Settings_Spawn.h"
#include "Settings_OverridePaths.h"
#include "Reminder_DLG.h"
#include "Profile_DLG.h"
#include "MiscTab.h"
#include "ScriptEditor.h"
#include "ScriptEditorFrame.h"
#include "ScriptInfo.h"
#include "MultiView.h"
#include "ColorSelectionDlg.h"
#include "ToolBar.h"


/////////////////////////////////////////////////////////////////////////////
// CAxis2App:
// See Axis2.cpp for the implementation of this class
//

class CAxis2App : public CWinApp
{
public:
	CString GetVersionTitle();
	CString GetBuildTimestamp();
	CString GetRegistryString(CString csValue, CString csDefault = "", HKEY hMainKey = hRegLocation, CString csSubKey = REGKEY_AXIS);
	CString m_csPosition;
	CString m_csCurrentProfile;
	CString m_csRootDirectory;
	CAdvStringArray pLng;
	bool LoadLang();

	void DeleteRegistryKey(CString csKeyName, HKEY hRootKey = hRegLocation);
	void DeleteRegistryValue(CString csValueName, CString csSubKey = REGKEY_AXIS , HKEY hRootKey = hRegLocation);
	void PutRegistryMultiSz(CString csValue, CStringArray * pStrings, HKEY hMainKey = hRegLocation, CString csSubKey = REGKEY_AXIS);
	void GetRegistryMultiSz(CString csValue, CStringArray * pStrings, HKEY hMainKey = hRegLocation, CString csSubKey = REGKEY_AXIS);
	void PutRegistryDword(CString csValue, DWORD dwValue, HKEY hMainKey = hRegLocation, CString csSubKey = REGKEY_AXIS);
	void PutRegistryString(CString csValue, CString csString, HKEY hMainKey = hRegLocation, CString csSubKey = REGKEY_AXIS);

	void InitializeMulPaths();
	void LoadCustomPaths();
	BOOL SetMulPath(short sFileIndex, LPCTSTR pszFilePath);
	CString GetMulPath(short sFileIndex);
	bool FindInstance();
	bool LoadIni(int iLoadPart = -1, CString csLoadString = "");
	CBrush m_bkbrush;
	COLORREF crAxisBkg;

	void LoadBodyDef();
	void LoadBodyConvert();
	void LoadHues();
	void LoadSounds();
	void LoadMusic();
	void UnLoadBodyDef();
	void UnLoadBodyConvert();
	void UnLoadHues();
	void UnLoadLog();
	void UnLoadSounds();
	void UnLoadMusic();
	CDefArray m_daBodydef;
	CDefArray m_daBodyConv;

	DWORD GetRegistryDword(CString csValue, DWORD dwDefault = 0, HKEY hMainKey = hRegLocation, CString csSubKey = REGKEY_AXIS);
	DWORD m_dwSysClose;
	DWORD m_dwAllowMultiple;
	DWORD m_dwAlwaysOnTop;
	DWORD m_dwDisableToolbar;
	DWORD m_dwLoadDefault;
	DWORD m_dwStartTab;
	DWORD m_dwRoomView;
	DWORD m_dwSameAsClient;
	DWORD m_dwDrawStatics;
	DWORD m_dwDrawDifs;
	DWORD m_dwInitiallize;
	DWORD m_dwShowSpawnpoints;
	DWORD m_dwNPCSpawnColor;
	DWORD m_dwItemSpawnColor;
	DWORD m_dwItemBGColor;
	DWORD m_dwSpawnBGColor;
	DWORD m_dwShowItems;
	DWORD m_dwShowNPCs;
	DWORD m_dwShowMap;
	CString m_csCommandPrefix;
	CString m_csUOTitle;
	bool bMemoryAccess;
	bool bLoadedMul;


	CPtrArray m_aHueGroups;
	CPtrArray m_aSounds;
	CPtrArray m_aMusic;

	CAxis2App();
	virtual ~CAxis2App();

	CAxis2Dlg* m_pAxisMainWnd;
	CAxisLog m_log;

	CScriptObjects * m_pScripts;
	CClientInfo * m_pClient;
	CRemoteConsole * m_pRConsole;
	int m_iReceiveTimeout;

	CPtrList m_olLog;

	CMenu pDefMenu;
	void UpdateProfileMenu();

	void UpdateProfileSettings();

	CLogTab * m_pcppAxisLogTab;
	CItemTab * m_pcppItemTab;
	CSpawnTab * m_pcppSpawnTab;
	CTravelTab * m_pcppTravelTab;
	CCommandsTab * m_pcppCommandsTab;
	CPlayerTweakTab * m_pcppPlayerTweakTab;
	CItemTweakTab * m_pcppItemTweakTab;
	CAccountTab * m_pcppAccountTab;
	CGeneralTab * m_pcppGeneralTab;
	CLauncherTab * m_pcppLauncherTab;
	CReminderTab * m_pcppReminderTab;
	CMiscTab * m_pcppMiscTab;

	CWorldMap * m_dlgWorldMap;
	CDoorWizard * m_dlgDoorWiz;
	CLightWizard * m_dlgLightWiz;
	CReminderDLG * m_dlgReminder;
	CProfileDLG * m_dlgProfile;
	CMultiView * m_dlgMultiView;
	CColorSelectionDlg * m_dlgColorPal;
	CAxis2LBar * m_dlgToolBar;

	//Settings Pages
	CSettingsGeneral * m_pcppSetGeneral;
	CSettingsPaths * m_pcppSetPaths;
	CSettingsItem * m_pcppSetItem;
	CSettingsTravel * m_pcppSetTravel;
	CSettingsSpawn * m_pcppSetSpawn;
	CSettingsOverridePaths * m_pcppSetOverridePaths;


	int LocX, sizeX;
	int LocY, sizeY;
	int LocZ, sizeZ;
	int LocM, sizeM;
	bool calibrated;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxis2App)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAxis2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CAxis2App * Main;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXIS2_H__A4D4BBF1_8874_4913_BDA8_293B666FB0A3__INCLUDED_)
