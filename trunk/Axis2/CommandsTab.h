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

#include "afxwin.h"
#if !defined(AFX_COMMANDSTAB_H__E166830D_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)
#define AFX_COMMANDSTAB_H__E166830D_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommandsTab.h : header file
//

#include "TabCtrl.h"
#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCommandsTab dialog

class CCommandsTab : public CDockingPage
{
	DECLARE_DYNAMIC(CCommandsTab)

// Construction
public:
	CCommandsTab();
	virtual ~CCommandsTab();
	static int iEditBtn;
	static int iRemoveBtn;

// Dialog Data
	//{{AFX_DATA(CCommandsTab)
	enum { IDD = IDD_COMMANDS_TAB };
	CMyTabCtrl m_ctTab;
	CEdit m_ceCaption;
	CEdit m_ceCommand;
	CEdit m_ceTabText;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCommandsTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int iNextID;
	// Generated message map functions
	//{{AFX_MSG(CCommandsTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddtab();
	afx_msg void OnDeltab();
	afx_msg void OnCleartab();
	afx_msg void OnAddbutton();
	afx_msg void OnEditbutton();
	afx_msg void OnRemovebutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	LRESULT ButtonPressed(WPARAM w, LPARAM l);
	CStringArray csaTabs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDSTAB_H__E166830D_C5AF_4EBE_B76F_8EC4D7DFA15F__INCLUDED_)