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

#if !defined(AFX_SETTINGSTRAVEL_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
#define AFX_SETTINGSTRAVEL_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Settings_Travel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsTravel dialog

class CSettingsTravel : public CPropertyPage
{
	DECLARE_DYNCREATE(CSettingsTravel)

// Construction
public:
	CSettingsTravel();
	~CSettingsTravel();
	DWORD m_dwNPCSpawnColor;
	DWORD m_dwItemSpawnColor;

// Dialog Data
	//{{AFX_DATA(CSettingsTravel)
	enum { IDD = IDD_SETTING_TRAVEL };
	BOOL	m_bDrawStatics;
	BOOL	m_bDrawDifs;
	BOOL	m_bShowSpawnpoints;
	BOOL	m_bShowMap;
	CButton	m_cbDrawStatics;
	CButton	m_cbDrawDifs;
	CButton	m_cbShowSpawnpoints;
	CStatic	m_csItemColor;
	CStatic	m_csNpcColor;
	CStatic	m_stNPCCol;
	CStatic	m_stItemCol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSettingsTravel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSettingsTravel)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawStatics();
	afx_msg void OnDrawDifs();
	afx_msg void OnShowSpawnpoints();
	afx_msg void OnShowMap();
	afx_msg void OnItemColor();
	afx_msg void OnNpcColor();
	afx_msg void OnPaint();
	afx_msg void OnResetTab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSTRAVEL_H__E166830F_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
