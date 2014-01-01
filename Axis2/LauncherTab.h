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

#if !defined(AFX_LAUNCHERTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_LAUNCHERTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LauncherTab.h : header file
//

#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CItemTab dialog

class CLauncherTab : public CDockingPage
{
	DECLARE_DYNCREATE(CLauncherTab)

// Construction
public:
	CLauncherTab();
	virtual ~CLauncherTab();

// Dialog Data
	//{{AFX_DATA(CLauncherTab)
	enum { IDD = IDD_LAUNCHER_TAB };
	CListBox	m_clbAccountList;
	CEdit	m_cePort;
	CEdit	m_cePassword;
	CEdit	m_ceName;
	CEdit	m_ceIPAddress;
	CEdit	m_ceAccount;
	CEdit	m_ceAltClient;
	CButton	m_cbBrowse;
	CButton	m_cbDone;
	CButton	m_cbCancel;
	CButton	m_cbEdit;
	CButton	m_cbNew;
	CButton	m_cbDelete;
	CButton	m_cbLaunch2D;
	CButton	m_cbLaunch3D;
	CButton	m_cbLaunchAlt;
	CButton	m_cbLaunchAuto;
	CStatic		m_stName;
	CStatic		m_stAcct;
	CStatic		m_stPass;
	CStatic		m_stIP;
	CStatic		m_stPort;

	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLauncherTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iCurAccountSel;
	// Generated message map functions
	//{{AFX_MSG(CLauncherTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAccountList();
	afx_msg void OnSave();
	afx_msg void OnCancelAccount();
	afx_msg void OnClientBrowse();
	afx_msg void OnEdit();
	afx_msg void OnNew();
	afx_msg void OnDelete();
	afx_msg void OnLaunch2D();
	afx_msg void OnLaunch3D();
	afx_msg void OnLaunchAlt();
	afx_msg void OnLaunchAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LauncherTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
