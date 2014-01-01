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

#if !defined(AFX_SETTINGSPATHS_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
#define AFX_SETTINGSPATHS_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Settings_Paths.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsPaths dialog

class CSettingsPaths : public CPropertyPage
{
	DECLARE_DYNCREATE(CSettingsPaths)

// Construction
public:
	CSettingsPaths();
	~CSettingsPaths();

// Dialog Data
	//{{AFX_DATA(CSettingsPaths)
	enum { IDD = IDD_SETTING_PATHS };
	BOOL	m_bSameAsClient;
	CButton	m_cbClientBrowse;
	CButton m_cbMulBrowse;
	CStatic	m_csDefaultClient;
	CStatic	m_csDefaultMulPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSettingsPaths)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSettingsPaths)
	virtual BOOL OnInitDialog();
	afx_msg void OnSameAsClient();
	afx_msg void OnClientbrowse();
	afx_msg void OnMulBrowse();
	afx_msg void OnResetTab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSPATHS_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
