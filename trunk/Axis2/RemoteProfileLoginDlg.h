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

#if !defined(AFX_REMOTEPROFILELOGINDLG_H__E1187437_4922_4615_9160_0B40CA3351BB__INCLUDED_)
#define AFX_REMOTEPROFILELOGINDLG_H__E1187437_4922_4615_9160_0B40CA3351BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteProfileLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemoteProfileLoginDlg dialog

class CRemoteProfileLoginDlg : public CDialog
{
// Construction
public:
	CRemoteProfileLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteProfileLoginDlg)
	enum { IDD = IDD_PROFILE_LOGIN_DLG };
	CComboBox	m_ccbName;
	CEdit	m_cePassword;
	CString	m_csPassword;
	CString m_csAccountName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteProfileLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iSel;

	// Generated message map functions
	//{{AFX_MSG(CRemoteProfileLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeAcctname();
	afx_msg void OnEditchangeAcctname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTECONSOLELOGINDLG_H__E1187437_4922_4615_9160_0B40CA3351BB__INCLUDED_)
