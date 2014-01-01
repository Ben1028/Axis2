/*
 $Id: SearchCritDlg.h,v 1.3 2002/05/01 03:33:10 pesterle Exp $

 **********************************************************************
 * Copyright (C) Philip A. Esterle 1998-2002
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 * 
 **********************************************************************

*/

#if !defined(AFX_SEARCHCRITDLG_H__50CF63A3_D8CE_11D3_A38A_00805FD91B8C__INCLUDED_)
#define AFX_SEARCHCRITDLG_H__50CF63A3_D8CE_11D3_A38A_00805FD91B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchCritDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchCritDlg dialog

class CSearchCritDlg : public CDialog
{
// Construction
public:
	CString m_csValue;
	CSearchCritDlg(CWnd* pParent = NULL);   // standard constructor
	bool bArea;

// Dialog Data
	//{{AFX_DATA(CSearchCritDlg)
	enum { IDD = IDD_SEARCHCRITDLG };
	int		m_crPref;
	CEdit	m_ceValue;
	CButton m_cbID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchCritDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSearchCritDlg)
	afx_msg void OnSearchId();
	afx_msg void OnSearchDescription();
	afx_msg void OnSearchDefname();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHCRITDLG_H__50CF63A3_D8CE_11D3_A38A_00805FD91B8C__INCLUDED_)
