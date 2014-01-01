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

#if !defined(AFX_SETTINGSGENERAL_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
#define AFX_SETTINGSGENERAL_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Settings_General.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneral dialog

class CSettingsGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CSettingsGeneral)

// Construction
public:
	CSettingsGeneral();
	~CSettingsGeneral();

// Dialog Data
	//{{AFX_DATA(CSettingsGeneral)
	enum { IDD = IDD_SETTING_GENERAL };
	BOOL	m_bAllowMultiple;
	BOOL	m_bAlwaysOnTop;
	BOOL	m_bSysClose;
	BOOL	m_bLoadDefault;
	CComboBox	m_ccbStartTab;
	CButton m_cbResetSettings;
	CEdit	m_ceCommandPrefix;
	CComboBox	m_ccbLanguage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSettingsGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSettingsGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnAllowMultiple();
	afx_msg void OnAlwaysOnTop();
	afx_msg void OnSysClose();
	afx_msg void OnLoadDefault();
	afx_msg void OnSelchangeStartTab();
	afx_msg void OnResetSettings();
	afx_msg void OnResetTab();
	afx_msg void OnChangePrefix();
	afx_msg void OnSelchangeLanguage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSGENERAL_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
