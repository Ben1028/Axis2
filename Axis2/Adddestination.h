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

#if !defined(AFX_ADDDESTINATION_H__2ED3EDC3_D825_11D2_A614_004F4905E937__INCLUDED_)
#define AFX_ADDDESTINATION_H__2ED3EDC3_D825_11D2_A614_004F4905E937__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AddDestination.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddDestination dialog

class CAddDestination : public CDialog
{
// Construction
public:
	CPtrList * m_pCategoryList;
	CAddDestination(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddDestination)
	enum { IDD = IDD_ADD_DESTINATION };
	CComboBox	m_ccbSubsection;
	CEdit	m_ceDescription;
	CComboBox	m_ccbCategory;
	CString	m_csCategory;
	CString	m_csDescription;
	CString	m_csSubsection;
	CString	m_csXCoord;
	CString	m_csYCoord;
	CString	m_csZCoord;
	CString	m_csPlane;

	CStatic m_ctCategory;
	CStatic m_ctSusection;
	CStatic m_ctDescription;
	CStatic m_ctX;
	CStatic m_ctY;
	CStatic m_ctZ;
	CStatic m_ctMap;
	CStatic m_cbOk;
	CStatic m_cbCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddDestination)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddDestination)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCategory();
	afx_msg void OnEditchangeCategory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDDESTINATION_H__2ED3EDC3_D825_11D2_A614_004F4905E937__INCLUDED_)
