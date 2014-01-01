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

// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "SettingsDlg.h"
#include "stdio.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg

IMPLEMENT_DYNAMIC(CSettingsDlg, CPropertySheet)

CSettingsDlg::CSettingsDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSettingsDlg::CSettingsDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSettingsDlg::~CSettingsDlg()
{
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return bResult;
}

CSettingsDlg::CSettingsDlg()
{
	CString csTitle;
	csTitle.Format( "%s - Settings", Main->GetVersionTitle() );
	CSettingsDlg(LPCTSTR(csTitle));
}

int CSettingsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_nid.cbSize = sizeof(m_nid);
	m_nid.hWnd = this->GetSafeHwnd();
	m_nid.uID = 1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE;
	m_nid.uCallbackMessage = WM_USER;
	m_nid.hIcon = m_hIcon;
	m_bModeless = 1;
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle(0, WS_MINIMIZEBOX);
	return 0;
}