/*
 $Id: SearchCritDlg.cpp,v 1.4 2002/05/01 03:33:10 pesterle Exp $

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

// SearchCritDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "SearchCritDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchCritDlg dialog


CSearchCritDlg::CSearchCritDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchCritDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchCritDlg)
	bArea = 0;
	//}}AFX_DATA_INIT
	m_crPref = 0;
}


void CSearchCritDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchCritDlg)
	DDX_Radio(pDX, IDC_ID_SEARCH, m_crPref);
	DDX_Control(pDX, IDC_SEARCH_VALUE, m_ceValue);
	DDX_Control(pDX, IDC_ID_SEARCH, m_cbID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchCritDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchCritDlg)
	ON_BN_CLICKED(IDC_ID_SEARCH, OnSearchId)
	ON_BN_CLICKED(IDC_DESC_SEARCH, OnSearchDescription)
	ON_BN_CLICKED(IDC_DEF_SEARCH, OnSearchDefname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchCritDlg message handlers

void CSearchCritDlg::OnSearchId() 
{
	m_crPref = 0;
	Main->PutRegistryDword("SearchCriteriaPrefs", m_crPref);	
}

void CSearchCritDlg::OnSearchDefname() 
{
	m_crPref = 1;
	Main->PutRegistryDword("SearchCriteriaPrefs", m_crPref);
}

void CSearchCritDlg::OnSearchDescription() 
{
	m_crPref = 2;
	Main->PutRegistryDword("SearchCriteriaPrefs", m_crPref);
}

void CSearchCritDlg::OnOK() 
{
	m_ceValue.GetWindowText(m_csValue);	
	CDialog::OnOK();
}

BOOL CSearchCritDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (bArea)
		m_cbID.SetWindowText("Coordinate");

	if (m_csValue != "")
		m_ceValue.SetWindowText(m_csValue);	
	m_crPref = Main->GetRegistryDword("SearchCriteriaPrefs", 0);
	
	UpdateData(FALSE);
	return TRUE;
}
