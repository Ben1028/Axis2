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

#if !defined(AFX_SETTINGSDLG_H__6A6E37E0_EB0F_4888_AB33_38E7B560103B__INCLUDED_)
#define AFX_SETTINGSDLG_H__6A6E37E0_EB0F_4888_AB33_38E7B560103B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SettingsDlg.h : header file
//

BOOL CALLBACK HideButtonsProc(HWND hWnd, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CAxis2Dlg

class CSettingsDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CSettingsDlg)

// Construction
public:
	CSettingsDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSettingsDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	NOTIFYICONDATA m_nid;
	CSettingsDlg();
	HICON m_hIcon;
	virtual ~CSettingsDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSettingsDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__6A6E37E0_EB0F_4888_AB33_38E7B560103B__INCLUDED_)
