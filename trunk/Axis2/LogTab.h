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

#if !defined(AFX_LOGTAB_H__E4FC6116_99A5_11D6_95AB_00805FD91B8C__INCLUDED_)
#define AFX_LOGTAB_H__E4FC6116_99A5_11D6_95AB_00805FD91B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTab.h : header file
//

#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CLogTab dialog

class CLogTab : public CDockingPage
{
	DECLARE_DYNCREATE(CLogTab)
	
// Construction
public:
	void AddText(CString csText, COLORREF color);
	void SetVisible(bool bVisible) { m_bVisible = bVisible; }
	CLogTab();   // standard constructor
	virtual ~CLogTab();
	bool m_bVisible;

// Dialog Data
	//{{AFX_DATA(CLogTab)
	enum { IDD = IDD_LOG_TAB };
	CRichEditCtrl m_ceLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTab)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTab)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTAB_H__E4FC6116_99A5_11D6_95AB_00805FD91B8C__INCLUDED_)
