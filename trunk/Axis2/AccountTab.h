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

#if !defined(AFX_ACCOUNTTAB_H__37A80977_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_ACCOUNTTAB_H__37A80977_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountTab.h : header file
//

#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CAccountTab dialog

class CAccountTab : public CDockingPage
{
	DECLARE_DYNCREATE(CAccountTab)

// Construction
public:
	CAccountTab();
	virtual ~CAccountTab();

// Dialog Data
	//{{AFX_DATA(CAccountTab)
	enum { IDD = IDD_ACCOUNT_TAB };
	BOOL	m_bPriv02;
	BOOL	m_bPriv08;
	BOOL	m_bPriv010;
	BOOL	m_bPriv020;
	BOOL	m_bPriv040;
	BOOL	m_bPriv080;
	BOOL	m_bPriv0200;
	BOOL	m_bPriv0400;
	BOOL	m_bPriv0800;
	BOOL	m_bPriv02000;
	BOOL	m_bPriv04000;
	CStatic	m_csPrivValue;

	CStatic m_cbSetResdisp;
	CStatic m_cbSetPriv;
	CStatic m_cbSetPrivs;
	CStatic m_cbResetPrivs;
	CStatic m_ccPriv02;
	CStatic m_ccPriv08;
	CStatic m_ccPriv010;
	CStatic m_ccPriv020;
	CStatic m_ccPriv040;
	CStatic m_ccPriv080;
	CStatic m_ccPriv0200;
	CStatic m_ccPriv0400;
	CStatic m_ccPriv0800;
	CStatic m_ccPriv02000;
	CStatic m_ccPriv04000;

	CStatic m_crResdisp0;
	CStatic m_crResdisp1;
	CStatic m_crResdisp2;
	CStatic m_crResdisp3;
	CStatic m_crResdisp4;
	CStatic m_crResdisp5;
	CStatic m_crResdisp6;
	CStatic m_crResdisp7;

	CStatic m_crPlvl0;
	CStatic m_crPlvl1;
	CStatic m_crPlvl2;
	CStatic m_crPlvl3;
	CStatic m_crPlvl4;
	CStatic m_crPlvl5;
	CStatic m_crPlvl6;
	CStatic m_crPlvl7;

	CStatic m_cgPlvl;
	CStatic m_cgPriv;
	CStatic m_cgResdisp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAccountTab)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int iPrivLevel;
	int iResdisp;
	// Generated message map functions
	//{{AFX_MSG(CAccountTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnOwnerprivs();
	afx_msg void OnAdministratorprivs();
	afx_msg void OnDeveloperprivs();
	afx_msg void OnGmprivs();
	afx_msg void OnSeerprivs();
	afx_msg void OnCounselorprivs();
	afx_msg void OnPlayerprivs();
	afx_msg void OnGuestprivs();
	afx_msg void OnSetpriv();
	afx_msg void OnResdisp0();
	afx_msg void OnResdisp1();
	afx_msg void OnResdisp2();
	afx_msg void OnResdisp3();
	afx_msg void OnResdisp4();
	afx_msg void OnResdisp5();
	afx_msg void OnResdisp6();
	afx_msg void OnResdisp7();
	afx_msg void OnSetresdisp();
	afx_msg void OnPriv02();
	afx_msg void OnPriv08();
	afx_msg void OnPriv010();
	afx_msg void OnPriv020();
	afx_msg void OnPriv040();
	afx_msg void OnPriv080();
	afx_msg void OnPriv0200();
	afx_msg void OnPriv0400();
	afx_msg void OnPriv0800();
	afx_msg void OnPriv02000();
	afx_msg void OnPriv04000();
	afx_msg void OnSetprivs();
	afx_msg void OnResetPrivs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTTAB_H__37A80977_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
