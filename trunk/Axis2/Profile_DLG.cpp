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

// Profile_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Profile_DLG.h"
#include "Axis2Dlg.h"
#include <windowsx.h>  // required for GET_X_LPARAM, GET_Y_LPARAM)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileDLG

IMPLEMENT_DYNAMIC(CProfileDLG, CDialog)

CProfileDLG::CProfileDLG(CWnd* pParent /*=NULL*/) 
	: CDialog(CProfileDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfileDLG)
	m_bDefProfile = FALSE;
	m_bWebProfile = FALSE;
	m_bLoadResource = FALSE;
	//}}AFX_DATA_INIT
	m_hFolderIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FOLDER));
	m_hDocIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_DOC));

	m_ilIcons.Create(16, 16, ILC_MASK, 0, 1);
	m_iFolderIndex = m_ilIcons.Add(m_hFolderIcon);
	m_iDocIndex = m_ilIcons.Add(m_hDocIcon);

	m_iCurProfileSel = -1;
}


CProfileDLG::~CProfileDLG()
{
}


void CProfileDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileDLG)
	DDX_Control(pDX, IDC_PROFILES, m_clbProfiles);
	DDX_Control(pDX, IDC_AVAILABLESCRIPTS, m_ctcScripts);
	DDX_Control(pDX, IDC_DEFAULTCHECK, m_cbDefProfile);
	DDX_Control(pDX, IDC_WEBPROFILE, m_cbWebProfile);
	DDX_Control(pDX, IDC_NAME, m_ceName);
	DDX_Control(pDX, IDC_URL, m_ceURL);
	DDX_Control(pDX, IDC_BASEDIR, m_ceBaseDir);
	DDX_Control(pDX, IDC_ADD, m_cbAdd);
	DDX_Control(pDX, IDC_EDIT, m_cbEdit);
	DDX_Control(pDX, IDC_DELETE, m_cbDelete);
	DDX_Control(pDX, IDC_SAVE, m_cbSave);
	DDX_Control(pDX, IDC_CANCEL, m_cbCancel);
	DDX_Control(pDX, IDC_SAVESCRIPTS, m_cbSaveScripts);
	DDX_Control(pDX, IDC_DIRBROWSE, m_cbBrowse);
	DDX_Control(pDX, IDC_CURPROFILE, m_ceCurProfile);
	DDX_Control(pDX, IDC_LOAD, m_cbLoad);
	DDX_Control(pDX, IDC_STATICNAME, m_stName);
	DDX_Control(pDX, IDC_STATICDIR, m_stDir);
	DDX_Control(pDX, IDC_STATICURL, m_stURL);
	DDX_Check(pDX, IDC_DEFAULTCHECK, m_bDefProfile);
	DDX_Check(pDX, IDC_WEBPROFILE, m_bWebProfile);
	DDX_Control(pDX, IDC_LOADRESOURCE, m_cbLoadResource);
	DDX_Check(pDX, IDC_LOADRESOURCE, m_bLoadResource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileDLG, CDialog)
	//{{AFX_MSG_MAP(CProfileDLG)
	ON_LBN_SELCHANGE(IDC_PROFILES, OnSelchangeProfiles)
	ON_MESSAGE(WM_APP, OnTvCheckbox)
	ON_NOTIFY(NM_CLICK, IDC_AVAILABLESCRIPTS, OnClickAvailablescripts)
	ON_BN_CLICKED(IDC_DEFAULTCHECK, OnDefprofile)
	ON_BN_CLICKED(IDC_WEBPROFILE, OnWebprofile)
	ON_BN_CLICKED(IDC_LOADRESOURCE, OnLoadResource)
	ON_BN_CLICKED(IDC_ADD, OnNewprofile)
	ON_BN_CLICKED(IDC_EDIT, OnEditprofile)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteprofile)
	ON_BN_CLICKED(IDC_SAVE, OnSaveprofile)
	ON_BN_CLICKED(IDC_CANCEL, OnCancelprofile)
	ON_BN_CLICKED(IDC_SAVESCRIPTS, OnSaveScriptprofile)
	ON_BN_CLICKED(IDC_DIRBROWSE, OnDirbrowse)
	ON_BN_CLICKED(IDC_LOAD, OnLoadScripts)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDLG message handlers

BOOL CProfileDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HKEY hKey;
	LONG lStatus;
	int iIndex = 0;
	CString csCruProf, csDefault;
	csCruProf.Format("Current Profile : %s",Main->m_csCurentProfile);
	m_ceCurProfile.SetWindowText(csCruProf);
	csDefault = Main->GetRegistryString("Default Profile", "");
	m_clbProfiles.AddString("<Axis Profile>");
	m_clbProfiles.AddString("<None>");
	m_cbDefProfile.SetCheck(1);

	//Add Predefined Profiles
	if (csDefault == "<Axis Profile>")
	{
		m_clbProfiles.SetCurSel(0);
		m_ceName.SetWindowText("<Axis Profile>");
		m_ceBaseDir.SetWindowText(csProfilePath);
		m_iCurProfileSel = 0;
	}

	if (csDefault == "<None>")
	{
		m_clbProfiles.SetCurSel(1);
		m_ceName.SetWindowText("<None>");
		m_ceBaseDir.SetWindowText("None");
		m_iCurProfileSel = 1;
	}

	m_ctcScripts.SetImageList(&m_ilIcons, TVSIL_NORMAL);

	//Find all custom profiles
	lStatus = RegOpenKeyEx(hRegLocation, REGKEY_PROFILE, 0, KEY_ALL_ACCESS, &hKey);
	if ( lStatus == ERROR_SUCCESS )
	{
		while (lStatus == ERROR_SUCCESS)
		{
			char szBuffer[MAX_PATH];
			DWORD dwSize = sizeof(szBuffer);
			lStatus = RegEnumKeyEx(hKey, iIndex, &szBuffer[0], &dwSize, 0, NULL, NULL, NULL);
			CString csProfile;
			csProfile = szBuffer;
			if ((lStatus == ERROR_SUCCESS)&&(csProfile != "<Axis Profile>"))
			{	
				int iSel;
				iSel = m_clbProfiles.AddString(szBuffer);
				if (csDefault == csProfile)
				{
					//Highlight default Profile
					m_cbDelete.EnableWindow(true);
					
					//Find scripts for this profile
					CString csKey;
					csKey.Format("%s\\%s",REGKEY_PROFILE, csProfile);

					if(IsLocalProfile(csProfile))
					{
						m_ctcScripts.EnableWindow(true);
						CString csRoot = Main->GetRegistryString("Root Directory", "", hRegLocation, csKey);
						CStringArray csaSelectedScripts;
						Main->GetRegistryMultiSz("Selected Scripts", &csaSelectedScripts, hRegLocation, csKey);
						HTREEITEM hRoot = m_ctcScripts.InsertItem(csRoot);
						ParseDirectory(LPCTSTR(csRoot), hRoot, &csaSelectedScripts);
						m_ctcScripts.Expand(hRoot,TVE_EXPAND);
						m_ceBaseDir.SetWindowText(csRoot);

						DWORD dwResource = Main->GetRegistryDword("Load Resource", 0, hRegLocation, csKey);
						if(dwResource)
							m_cbLoadResource.SetCheck(1);
					}
					else
					{
						CString csURL = Main->GetRegistryString("URL", "", hRegLocation, csKey);
						m_ceURL.SetWindowText(csURL);
						m_cbWebProfile.SetCheck(1);
					}

					m_clbProfiles.SetCurSel(iSel);
					m_iCurProfileSel = iSel;
					m_ceName.SetWindowText(csProfile);
				}
			}
			iIndex++;
		}
		RegCloseKey(hKey);
	}
	UpdateData();

	CRect rectFrame, rectDlg;
	CWnd* pMainWnd = AfxGetMainWnd();
	if(pMainWnd != NULL)
	{
		pMainWnd->GetClientRect(rectFrame);
		pMainWnd->ClientToScreen(rectFrame);
		GetWindowRect(rectDlg);
		int nXPos = rectFrame.left + (rectFrame.Width() / 2) - (rectDlg.Width() / 2);
		int nYPos = rectFrame.top + (rectFrame.Height() / 2) - (rectDlg.Height() / 2);
		::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
	}

	return TRUE;
}


void CProfileDLG::OnSelchangeProfiles() 
{
	CString csProfile, csDefault;

	m_iCurProfileSel = m_clbProfiles.GetCurSel();

	m_cbAdd.EnableWindow(true);

	if (m_iCurProfileSel == -1)
		return;

	m_cbLoad.EnableWindow(true);

	m_stName.EnableWindow(false);
	m_stDir.EnableWindow(false);
	m_stURL.EnableWindow(false);
	m_ceBaseDir.EnableWindow(false);
	m_ceURL.EnableWindow(false);
	m_cbDefProfile.EnableWindow(false);
	m_cbWebProfile.EnableWindow(false);
	m_ceName.EnableWindow(false);
	m_cbSave.EnableWindow(false);
	m_cbSaveScripts.EnableWindow(false);
	m_cbBrowse.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbLoadResource.EnableWindow(false);
	m_cbLoadResource.SetCheck(0);

	m_ctcScripts.DeleteAllItems();	

	m_clbProfiles.GetText(m_iCurProfileSel, csProfile);
	m_ceName.SetWindowText(csProfile);

	//Set default field
	csDefault = Main->GetRegistryString("Default Profile", "");
	if (csDefault == csProfile)
		m_cbDefProfile.SetCheck(1);
	else
		m_cbDefProfile.SetCheck(0);
	UpdateData();

	if (csProfile == "<Axis Profile>")
	{
		m_ceBaseDir.SetWindowText(csProfilePath);
		m_ctcScripts.EnableWindow(false);
		m_cbDelete.EnableWindow(false);
		m_cbEdit.EnableWindow(true);
		return;
	}
	if (csProfile == "<None>")
	{
		m_ceBaseDir.SetWindowText("None");
		m_ctcScripts.EnableWindow(false);
		m_cbDelete.EnableWindow(false);
		m_cbEdit.EnableWindow(true);
		return;
	}


		m_cbDelete.EnableWindow(true);
		m_cbEdit.EnableWindow(true);

		CString csKey;
		csKey.Format("%s\\%s",REGKEY_PROFILE, csProfile);
		if(IsLocalProfile(csProfile))
		{
			m_ctcScripts.EnableWindow(true);
		
			//Find scripts for this profile
			CString csRoot = Main->GetRegistryString("Root Directory", "", hRegLocation, csKey);
			CStringArray csaSelectedScripts;
			Main->GetRegistryMultiSz("Selected Scripts", &csaSelectedScripts, hRegLocation, csKey);
			HTREEITEM hRoot = m_ctcScripts.InsertItem(csRoot);
			ParseDirectory(LPCTSTR(csRoot), hRoot, &csaSelectedScripts);
			m_ctcScripts.Expand(hRoot,TVE_EXPAND);
			m_ceBaseDir.SetWindowText(csRoot);
			m_ceURL.SetWindowText("");
			m_cbWebProfile.SetCheck(0);

			DWORD csResource = Main->GetRegistryDword("Load Resource", 0, hRegLocation, csKey);
			if(csResource)
				m_cbLoadResource.SetCheck(1);
		}
		else
		{
			CString csURL = Main->GetRegistryString("URL", "", hRegLocation, csKey);
			m_ceURL.SetWindowText(csURL);
			m_cbWebProfile.SetCheck(1);
			m_ceBaseDir.SetWindowText("");
			UpdateData();
		}
}


void CProfileDLG::OnNewprofile() 
{
	m_ceBaseDir.EnableWindow(true);
	m_cbDefProfile.EnableWindow(true);
	m_ceName.EnableWindow(true);
	m_cbSave.EnableWindow(true);
	m_cbBrowse.EnableWindow(true);
	m_cbCancel.EnableWindow(true);
	m_cbWebProfile.EnableWindow(true);
	m_cbLoadResource.EnableWindow(true);

	m_ctcScripts.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
	m_cbEdit.EnableWindow(false);
	m_cbAdd.EnableWindow(false);
	m_cbSaveScripts.EnableWindow(false);
	m_cbLoad.EnableWindow(false);
	m_ceURL.EnableWindow(false);

	m_stName.EnableWindow(true);
	m_stDir.EnableWindow(true);
	m_stURL.EnableWindow(false);
	
	m_ceName.SetWindowText("");
	m_ceBaseDir.SetWindowText("");
	m_ceURL.SetWindowText("");
	m_cbDefProfile.SetCheck(0);
	m_cbWebProfile.SetCheck(0);
	m_cbLoadResource.SetCheck(0);
	m_ctcScripts.DeleteAllItems();
	m_clbProfiles.SetCurSel(-1);
	m_iCurProfileSel = -1;
	UpdateData();
	this->SetFocus();
}


void CProfileDLG::OnEditprofile() 
{
	if ( m_iCurProfileSel == -1 )
		return;
	m_ctcScripts.EnableWindow(false);
	m_cbSaveScripts.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
	m_cbEdit.EnableWindow(false);
	m_cbAdd.EnableWindow(false);
	m_cbLoad.EnableWindow(false);

	m_cbDefProfile.EnableWindow(true);
	m_cbSave.EnableWindow(true);
	m_cbCancel.EnableWindow(true);

	CString csProfile;
	m_clbProfiles.GetText(m_iCurProfileSel, csProfile);
	if (csProfile == "<None>")
	{
		m_cbBrowse.EnableWindow(false);
		m_ceBaseDir.EnableWindow(false);
		return;
	}

	if((IsLocalProfile(csProfile))||(csProfile == "<Axis Profile>"))
	{
		m_ceBaseDir.EnableWindow(true);
		m_cbBrowse.EnableWindow(true);
		m_stDir.EnableWindow(true);
	}
	else
	{
		m_ceURL.EnableWindow(true);
		m_stURL.EnableWindow(true);
	}
	if(csProfile != "<Axis Profile>")
	{
		m_cbWebProfile.EnableWindow(true);
		if(IsLocalProfile(csProfile))
			m_cbLoadResource.EnableWindow(true);
	}

	this->SetFocus();
}


void CProfileDLG::OnDeleteprofile() 
{
	if ( m_iCurProfileSel == -1 )
		return;
	CString csProfile, csMessage, csKey, csDefault;
	m_clbProfiles.GetText(m_iCurProfileSel, csProfile);

	//Delete Confirmation
	csMessage.Format("Are you sure you want to delete the %s profile?", csProfile);
	if ( AfxMessageBox(csMessage, MB_YESNO | MB_ICONQUESTION) == IDNO )
		return;

	csKey.Format("%s\\%s",REGKEY_PROFILE, csProfile);
	Main->DeleteRegistryKey(csKey);

	//Reset Default Profile
	csDefault = Main->GetRegistryString("Default Profile", "");
	if (csDefault == csProfile)
		Main->PutRegistryString("Default Profile", "<None>");

	m_clbProfiles.DeleteString( m_iCurProfileSel );
	m_ceName.SetWindowText("");
	m_ceBaseDir.SetWindowText("");
	m_ceURL.SetWindowText("");
	m_cbDefProfile.SetCheck(0);
	m_cbWebProfile.SetCheck(0);
	m_cbLoadResource.SetCheck(0);
	m_ctcScripts.DeleteAllItems();
	m_iCurProfileSel = -1;

	m_ceName.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
	m_cbEdit.EnableWindow(false);
	m_ceBaseDir.EnableWindow(false);
	m_ceURL.EnableWindow(false);
	m_cbDefProfile.EnableWindow(false);
	m_cbWebProfile.EnableWindow(false);
	m_ctcScripts.EnableWindow(false);
	m_cbSave.EnableWindow(false);
	m_cbBrowse.EnableWindow(false);
	m_cbSaveScripts.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbLoad.EnableWindow(false);
	m_stName.EnableWindow(false);
	m_stURL.EnableWindow(false);
	m_stDir.EnableWindow(false);
	m_cbLoadResource.EnableWindow(false);
	this->SetFocus();
}


void CProfileDLG::OnDefprofile() 
{
	UpdateData();
}


void CProfileDLG::OnLoadResource() 
{
	UpdateData();
}

void CProfileDLG::OnWebprofile() 
{
	UpdateData();
	if (m_bWebProfile)
	{
		m_ceBaseDir.EnableWindow(false);
		m_cbBrowse.EnableWindow(false);
		m_stDir.EnableWindow(false);
		m_cbLoadResource.EnableWindow(false);

		m_ceURL.EnableWindow(true);
		m_stURL.EnableWindow(true);
	}
	else
	{
		m_ceBaseDir.EnableWindow(true);
		m_cbBrowse.EnableWindow(true);
		m_stDir.EnableWindow(true);
		m_cbLoadResource.EnableWindow(true);

		m_ceURL.EnableWindow(false);
		m_stURL.EnableWindow(false);
	}
}


void CProfileDLG::OnSaveprofile() 
{
	CString csKey, m_csName, m_csBaseDir, csError, csDefault, m_csURL;
	m_ceName.GetWindowText(m_csName);
	m_ceBaseDir.GetWindowText(m_csBaseDir);
	m_ceURL.GetWindowText(m_csURL);
	if(m_csName == "")
	{
		AfxMessageBox("Name field empty", MB_ICONSTOP);
		return;
	}
	if (m_bWebProfile)
	{
		if(m_csURL == "")
		{
			AfxMessageBox("URL field empty", MB_ICONSTOP);
			return;
		}
	}
	else
	{
		if(m_csBaseDir == "")
		{
			AfxMessageBox("Base Directory field empty", MB_ICONSTOP);
			return;
		}
	}
	csKey.Format("%s\\%s",REGKEY_PROFILE, m_csName);

	HKEY hKey;

	if (m_ceName.IsWindowEnabled())
	{
		if(( RegOpenKeyEx(hRegLocation, csKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS )||(m_csName == "<None>")||(m_csName == "<Axis Profile>"))
		{
		//This Profile already exists, stop here.
			csError.Format("Profile '%s' already exists" , m_csName);
			AfxMessageBox(csError, MB_ICONSTOP);
			RegCloseKey(hKey);
			return;
		}
		
		//Highlight new profile
		m_clbProfiles.AddString(m_csName);
		m_clbProfiles.SelectString(-1,m_csName);
		m_iCurProfileSel = m_clbProfiles.GetCurSel();
	}

	if ((m_csName != "<Axis Profile>")&&(m_csName != "<None>"))
	{
		//local or web profile?
		if (m_bWebProfile)
		{
			Main->PutRegistryString("Type", "Web", hRegLocation, csKey);
			Main->PutRegistryString("URL", m_csURL, hRegLocation, csKey);
			Main->DeleteRegistryValue("Load Resource",csKey);
			Main->DeleteRegistryValue("Root Directory",csKey);
			Main->DeleteRegistryValue("Selected Scripts",csKey);
		}
		else
		{
			Main->PutRegistryString("Type", "Local", hRegLocation, csKey);
			Main->PutRegistryDword("Load Resource", m_bLoadResource, hRegLocation, csKey);
			Main->PutRegistryString("Root Directory", m_csBaseDir, hRegLocation, csKey);
			Main->DeleteRegistryValue("URL",csKey);

			//Set script window
			m_ctcScripts.DeleteAllItems();
			CStringArray csaSelectedScripts;
			Main->GetRegistryMultiSz("Selected Scripts", &csaSelectedScripts, hRegLocation, csKey);
			HTREEITEM hRoot = m_ctcScripts.InsertItem(m_csBaseDir);
			ParseDirectory(LPCTSTR(m_csBaseDir), hRoot, &csaSelectedScripts);
		}
	}

	if(m_csName == "<Axis Profile>")
	{
		csProfilePath = m_csBaseDir;
		Main->PutRegistryString("Default ProfilePath", m_csBaseDir);
	}

	//Set default profile
	if (m_bDefProfile)
		Main->PutRegistryString("Default Profile", m_csName);

	csDefault = Main->GetRegistryString("Default Profile", "");
	if ((!m_bDefProfile)&&(csDefault == m_csName))
	{
		Main->PutRegistryString("Default Profile", "<None>");
		if (m_csName == "<None>")
			m_cbDefProfile.SetCheck(1);
	}
	Main->UpdateProfileMenu();

	//reset windows
	m_ceName.EnableWindow(false);
	m_ceBaseDir.EnableWindow(false);
	m_ceURL.EnableWindow(false);
	m_cbDefProfile.EnableWindow(false);
	m_cbWebProfile.EnableWindow(false);
	m_cbBrowse.EnableWindow(false);
	m_cbSave.EnableWindow(false);
	m_cbSaveScripts.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbDelete.EnableWindow(true);
	m_cbEdit.EnableWindow(true);
	m_cbAdd.EnableWindow(true);
	m_cbLoad.EnableWindow(true);
	m_ctcScripts.EnableWindow(true);
	m_stDir.EnableWindow(false);
	m_stURL.EnableWindow(false);
	m_cbLoadResource.EnableWindow(false);

	if (m_bWebProfile)
		m_ctcScripts.EnableWindow(false);

	if ((m_csName == "<Axis Profile>")||(m_csName == "<None>"))
	{
		m_cbDelete.EnableWindow(false);
		m_ctcScripts.EnableWindow(false);
	}
	this->SetFocus();
}


void CProfileDLG::OnSaveScriptprofile() 
{
	UpdateProfile();
	m_cbSaveScripts.EnableWindow(false);
	this->SetFocus();
}


void CProfileDLG::OnCancelprofile() 
{
	m_cbAdd.EnableWindow(true);

	m_ceName.EnableWindow(false);
	m_ceBaseDir.EnableWindow(false);
	m_cbDefProfile.EnableWindow(false);
	m_cbBrowse.EnableWindow(false);
	m_cbSave.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbWebProfile.EnableWindow(false);
	m_cbLoadResource.EnableWindow(false);
	m_ceURL.EnableWindow(false);

	m_stName.EnableWindow(false);
	m_stDir.EnableWindow(false);
	m_stURL.EnableWindow(false);

	if (m_iCurProfileSel != -1 )
	{
		m_cbEdit.EnableWindow(true);
		m_ctcScripts.EnableWindow(true);
		m_cbLoad.EnableWindow(true);

		CString csProfile;
		m_clbProfiles.GetText(m_iCurProfileSel, csProfile);
		if ((csProfile != "<Axis Profile>")&&(csProfile != "<None>"))
			m_cbDelete.EnableWindow(true);
		else
			m_ctcScripts.EnableWindow(false);
	}
	else
	{
		m_cbDefProfile.SetCheck(0);
		m_cbWebProfile.SetCheck(0);
	}
	this->SetFocus();

}


void CProfileDLG::OnDirbrowse() 
{

	CString pathSelected, m_csBaseDir;
	CFolderDialog dlg(&pathSelected,"Choose folder");
	m_ceBaseDir.GetWindowText(m_csBaseDir);
	dlg.m_ofn.lpstrInitialDir = m_csBaseDir;
	if (dlg.DoModal() == IDOK)
		m_ceBaseDir.SetWindowText(pathSelected);
	return;	
}

void CProfileDLG::OnLoadScripts()
{
	Main->m_pScripts->UnloadProfile();
	CString m_csName, csCruProf;
	Main->LoadIni(1);
	Main->LoadIni(2);
	m_ceName.GetWindowText(m_csName);
	csCruProf.Format("Current Profile : %s",m_csName);
	m_ceCurProfile.SetWindowText(csCruProf);
	Main->m_csCurentProfile = m_csName;
	CAxis2Dlg * hParent = (CAxis2Dlg *)this->GetParent();
	hParent->UpdateTip();
	AfxBeginThread(LoadProfileThread,(LPVOID)0);
}


void CProfileDLG::UpdateProfile()
{
	if ( m_iCurProfileSel == -1 )
		return;
	CString csProfile;
	m_clbProfiles.GetText(m_iCurProfileSel, csProfile);
	CStringArray aSelectedScripts;
	HTREEITEM hRoot = m_ctcScripts.GetRootItem();
	CString csRootDir = m_ctcScripts.GetItemText(hRoot);
	GetSelections(hRoot, csRootDir, &aSelectedScripts);
	CString csProfileKey;
	csProfileKey.Format("%s\\%s",REGKEY_PROFILE, csProfile);
	Main->PutRegistryMultiSz("Selected Scripts", &aSelectedScripts, hRegLocation, csProfileKey);		
}


void CProfileDLG::ParseDirectory(LPCTSTR pszPath, HTREEITEM hNode, CStringArray *pSelections)
{
		WIN32_FIND_DATA findData;
		memset(&findData, 0x00, sizeof(findData));
		CString csTestFile;
		csTestFile.Format("%s\\*", pszPath);
		HANDLE hSearch = FindFirstFile(csTestFile, &findData);
		if ( hSearch != INVALID_HANDLE_VALUE )
		{
			BOOL bStatus = TRUE;
			while (bStatus)
			{
				if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					// Recurse this
					if ( strcmp(findData.cFileName, "..") == 0 || strcmp(findData.cFileName, ".") == 0 )
					{
						// Ignore this one
					}
					else
					{
						HTREEITEM hBranch = m_ctcScripts.InsertItem(findData.cFileName, m_iFolderIndex, m_iFolderIndex, hNode);
						CString csNewPath;
						csNewPath.Format("%s\\%s", pszPath, findData.cFileName);
						ParseDirectory(csNewPath, hBranch, pSelections);
					}
				}
				else
				{
					CString csFileName = _T(findData.cFileName);
					if (( csFileName.Right(4).CompareNoCase(".scp") == 0)||(csFileName.CompareNoCase("sphere.ini")==0))
					{
						if (( csFileName.Mid(0,7).CompareNoCase("sphereb") == 0) && (IsNumber(csFileName.Mid(7,2))))
						{
							bStatus = FindNextFile(hSearch, &findData);
							continue;
						}
						CString csFullPath;
						csFullPath.Format("%s\\%s", pszPath, findData.cFileName);
						HTREEITEM hNewItem = m_ctcScripts.InsertItem(findData.cFileName, m_iDocIndex, m_iDocIndex, hNode);
						if ( IsSelected(pSelections, csFullPath ) )
						{
							m_ctcScripts.SetCheck(hNewItem);
							m_ctcScripts.SetCheck(m_ctcScripts.GetParentItem(hNewItem));
						}
					}
				}
				bStatus = FindNextFile(hSearch, &findData);
			}

			if (! m_ctcScripts.ItemHasChildren(hNode) )
				m_ctcScripts.DeleteItem(hNode);
			FindClose(hSearch);
		}
}


void CProfileDLG::GetSelections(HTREEITEM hNode, CString csRootDir, CStringArray *pSelections)
{
	HTREEITEM hChild = m_ctcScripts.GetNextItem(hNode, TVGN_CHILD);
	while (hChild != NULL )
	{
		CString csPath;
		if ( csRootDir != "" )
			csPath = csRootDir + _T("\\") + m_ctcScripts.GetItemText(hChild);
		else
			csPath = m_ctcScripts.GetItemText(hChild);
		if ( m_ctcScripts.ItemHasChildren(hChild) )
			GetSelections(hChild, csPath, pSelections);
		else
		{
			// This is a script file
			if ( m_ctcScripts.GetCheck(hChild) )
			{
				CString csFile = m_ctcScripts.GetItemText(hChild);
				csFile.MakeLower();
				if((csFile == "spheretables.scp")||(csFile == "sphere.ini"))
					pSelections->InsertAt(0,csPath);
				else
					pSelections->Add(csPath);
			}
		}
		hChild = m_ctcScripts.GetNextItem(hChild, TVGN_NEXT);
	}
}


bool CProfileDLG::IsSelected(CStringArray * pSelections, CString csFile)
{
	for ( int i = 0; i <= pSelections->GetUpperBound(); i++ )
	{
		if ( csFile == pSelections->GetAt(i) )
			return true;
	}
	return false;
}


void CProfileDLG::OnClickAvailablescripts(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_cbSaveScripts.EnableWindow(true);
	DWORD dw = GetMessagePos();                   // retrieve mouse cursor position when msg was sent
	CPoint p(GET_X_LPARAM(dw), GET_Y_LPARAM(dw)); // ..and put into point structure
	m_ctcScripts.ScreenToClient(&p);                    // make coords local to tree client area

	UINT htFlags = 0;
	HTREEITEM it = m_ctcScripts.HitTest(p, &htFlags);   // See where the click was on

	if (it != NULL && htFlags==TVHT_ONITEMSTATEICON)
	{   
		// the check box was hit.
		// we just post a message
		PostMessage(WM_APP, pNMHDR->idFrom, (LPARAM) it);
	}	
	*pResult = 0;
}


LRESULT CProfileDLG::OnTvCheckbox(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(wp);
	HTREEITEM hItem = (HTREEITEM) lp;
	int checked = (m_ctcScripts.GetItemState(hItem, TVIS_STATEIMAGEMASK) >> 12) - 1;

	CheckChildren(hItem, checked);
	
	return 0;
}


void CProfileDLG::CheckChildren(HTREEITEM hNode, int check)
{
	HTREEITEM hChild = m_ctcScripts.GetNextItem(hNode, TVGN_CHILD);
	while (hChild != NULL )
	{
		if ( m_ctcScripts.ItemHasChildren(hChild) )
			CheckChildren(hChild, check);
		m_ctcScripts.SetCheck(hChild, check);
		hChild = m_ctcScripts.GetNextItem(hChild, TVGN_NEXT);
	}
}

void CProfileDLG::OnExport()
{
	CString csLine, csFile;
	CStdioFile pFile;

	CFileDialog dlg(FALSE, "scp", Main->m_csCurentProfile, OFN_HIDEREADONLY, "Script Files (*.scp)|*.scp||" , NULL);
	if ( dlg.DoModal() == IDOK )
	{
		csFile = dlg.GetFileName();
		if ( csFile != "" )
		{

			if ( !pFile.Open(csFile, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate) )
			{
				Main->m_log.Add(1,"ERROR: Unable to create file Axiscats.scp");
				return;
			}

			//Items,Multis, and Templates
			for(int i = 0; i <= Main->m_pScripts->m_aItems.GetUpperBound(); i++)
			{
				CSObject * pobj = (CSObject *) Main->m_pScripts->m_aItems.GetAt(i);
				pobj->WriteBlock(pFile);
			}

			//NPCs and Spawns
			for(int i = 0; i <= Main->m_pScripts->m_aNPCs.GetUpperBound(); i++)
			{
				CSObject * pobj = (CSObject *) Main->m_pScripts->m_aNPCs.GetAt(i);
				pobj->WriteBlock(pFile);
			}

			//Areas
			for(int i = 0; i <= Main->m_pScripts->m_aAreas.GetUpperBound(); i++)
			{
				CSObject * pobj = (CSObject *) Main->m_pScripts->m_aAreas.GetAt(i);
				pobj->WriteBlock(pFile);
			}

			//Defname
			pFile.WriteString("[DEFNAME deflist] \n");
			for(int i = 0; i <= Main->m_pScripts->m_aDefList.GetUpperBound(); i++)
			{
				CSObject * pobj = (CSObject *) Main->m_pScripts->m_aDefList.GetAt(i);
				csLine.Format("%s %s \n",pobj->m_csValue,pobj->m_csID);
				pFile.WriteString(csLine);
			}

			//Brains
			pFile.WriteString("\n");
			pFile.WriteString("[DEFNAME brains] \n");
			for(int i = 0; i <= Main->m_pScripts->m_asaNPCBrains.GetUpperBound(); i++)
			{
				csLine = Main->m_pScripts->m_asaNPCBrains.GetAt(i);
				CString csBrain, csValue;
				csBrain = csLine.SpanExcluding("(");
				csValue = csLine.Mid(csLine.FindOneOf("(")+1);
				csValue = csValue.SpanExcluding(")");
				csLine.Format("%s %s \n",csBrain,csValue);
				pFile.WriteString(csLine);
			}

			//Types
			pFile.WriteString("\n");
			pFile.WriteString("[TYPEDEFS] \n");
			for(int i = 0; i <= Main->m_pScripts->m_asaITEMTypes.GetUpperBound(); i++)
			{
				csLine.Format("%s \n",Main->m_pScripts->m_asaITEMTypes.GetAt(i));
				pFile.WriteString(csLine);
			}

			//Events
			pFile.WriteString("\n");
			for(int i = 0; i <= Main->m_pScripts->m_asaEvents.GetUpperBound(); i++)
			{
				csLine.Format("[EVENTS %s] \n",Main->m_pScripts->m_asaEvents.GetAt(i));
				pFile.WriteString(csLine);
			}

			//Function
			pFile.WriteString("\n");
			for(int i = 0; i <= Main->m_pScripts->m_asaFunctions.GetUpperBound(); i++)
			{
				csLine.Format("[FUNCTION %s] \n",Main->m_pScripts->m_asaFunctions.GetAt(i));
				pFile.WriteString(csLine);
			}

			//Skills
			pFile.WriteString("\n");
			for(int i = 0; i <= Main->m_pScripts->m_asaNPCSkills.GetUpperBound(); i++)
			{
				csLine = Main->m_pScripts->m_asaNPCSkills.GetAt(i);
				CString csSkill, csValue;
				csSkill = csLine.SpanExcluding("(");
				csValue = csLine.Mid(csLine.FindOneOf("(")+1);
				csValue = csValue.SpanExcluding(")");
				csLine.Format("[SKILL %s] \n",csValue);
				pFile.WriteString(csLine);
				csLine.Format("Key=%s \n",csSkill);
				pFile.WriteString(csLine);
			}

			//Spells
			pFile.WriteString("\n");
			for(int i = 0; i <= Main->m_pScripts->m_aSpellList.GetUpperBound(); i++)
			{
				CSObject * pobj = (CSObject *) Main->m_pScripts->m_aSpellList.GetAt(i);
				pobj->WriteBlock(pFile);
			}

			//Char Spawns
			pFile.WriteString("\n");
			for(int i = 0; i <= Main->m_pScripts->m_asaSPAWNchar.GetUpperBound(); i++)
			{
				pFile.WriteString("[WI i_worldgem_bit] \n");
				csLine.Format("P=%s \n",Main->m_pScripts->m_asaSPAWNchar.GetAt(i));
				pFile.WriteString(csLine);
			}

			//Item Spawns
			pFile.WriteString("\n");
			for(int i = 0; i <= Main->m_pScripts->m_asaSPAWNitem.GetUpperBound(); i++)
			{
				pFile.WriteString("[WI i_worldgem_bit] \n");
				pFile.WriteString("TYPE=t_spawn_item \n");
				csLine.Format("P=%s \n",Main->m_pScripts->m_asaSPAWNitem.GetAt(i));
				pFile.WriteString(csLine);
			}

			pFile.Close();
		}
	}
}

void CProfileDLG::OnCancel() 
{
	Main->m_dlgProfile = NULL;
	CDialog::OnCancel();
}

HBRUSH CProfileDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	return Main->m_bkbrush;
}