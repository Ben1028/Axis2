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

#if !defined(AFX_AXIS2DLG_H__6A6E37E0_EB0F_4888_AB33_38E7B560103B__INCLUDED_)
#define AFX_AXIS2DLG_H__6A6E37E0_EB0F_4888_AB33_38E7B560103B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Axis2Dlg.h : header file
//

BOOL CALLBACK HideButtonsProc(HWND hWnd, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CAxis2Dlg

class CAxis2Dlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CAxis2Dlg)

// Construction
public:
	CAxis2Dlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAxis2Dlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxis2Dlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateTip();
	void ReloadActiveTabPage();

	NOTIFYICONDATA m_nid;
	 CAxis2Dlg();
	HICON m_hIcon;
	virtual ~CAxis2Dlg();

	// Generated message map functions
protected:
	bool m_bInit;
	//{{AFX_MSG(CAxis2Dlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSettingsGeneral();
	afx_msg void OnSettingsPaths();
	afx_msg void OnSettingsItem();
	afx_msg void OnSettingsTravel();
	afx_msg void OnSettingsSpawn();
	afx_msg void OnSettingsOverridePaths();
	afx_msg void OnOpenProfileOption();
	afx_msg void OnUnloadProfile();
	afx_msg void OnLoadDefProfile();
	afx_msg void OnLoadLastProfile();
	afx_msg void OnHelp();
	afx_msg void OnAboutDlg();
	afx_msg void OnClose();
	//afx_msg void OnReadAnim();

public:
	afx_msg void OnOpenSettingsPage(int iPage);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXIS2DLG_H__6A6E37E0_EB0F_4888_AB33_38E7B560103B__INCLUDED_)
