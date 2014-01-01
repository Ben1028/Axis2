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

// AddDestination.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "AddDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddDestination dialog


CAddDestination::CAddDestination(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDestination::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddDestination)
	m_csCategory = _T("");
	m_csDescription = _T("");
	m_csSubsection = _T("");
	m_csXCoord = _T("");
	m_csYCoord = _T("");
	m_csZCoord = _T("");
	m_csPlane = _T("");
	//}}AFX_DATA_INIT
}


void CAddDestination::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDestination)
	DDX_Control(pDX, IDC_SUBSECTION, m_ccbSubsection);
	DDX_Control(pDX, IDC_DESCRIPTION, m_ceDescription);
	DDX_Control(pDX, IDC_CATEGORY, m_ccbCategory);
	DDX_CBString(pDX, IDC_CATEGORY, m_csCategory);
	DDX_Text(pDX, IDC_DESCRIPTION, m_csDescription);
	DDX_CBString(pDX, IDC_SUBSECTION, m_csSubsection);
	DDX_Text(pDX, IDC_XCOORD, m_csXCoord);
	DDX_Text(pDX, IDC_YCOORD, m_csYCoord);
	DDX_Text(pDX, IDC_ZCOORD, m_csZCoord);
	DDX_Text(pDX, IDC_PLANE, m_csPlane);

	DDX_Control(pDX, IDC_CAT, m_ctCategory);
	DDX_Control(pDX, IDC_SUB, m_ctSusection);
	DDX_Control(pDX, IDC_DESC, m_ctDescription);
	DDX_Control(pDX, IDC_X, m_ctX);
	DDX_Control(pDX, IDC_Y, m_ctY);
	DDX_Control(pDX, IDC_Z, m_ctZ);
	DDX_Control(pDX, IDC_MAP, m_ctMap);
	DDX_Control(pDX, IDOK, m_cbOk);
	DDX_Control(pDX, IDCANCEL, m_cbCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddDestination, CDialog)
	//{{AFX_MSG_MAP(CAddDestination)
	ON_CBN_SELCHANGE(IDC_CATEGORY, OnSelchangeCategory)
	ON_CBN_EDITCHANGE(IDC_CATEGORY, OnEditchangeCategory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddDestination message handlers

void CAddDestination::OnOK() 
{
	UpdateData(true);
	CString csValidate;
	m_ccbCategory.GetWindowText(csValidate);
	if (csValidate == "")
	{
		AfxMessageBox(CMsg("IDS_NOCATSEL"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_ccbSubsection.GetWindowText(csValidate);
	if (csValidate == "")
	{
		AfxMessageBox(CMsg("IDS_NOSUBSEL"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_ceDescription.GetWindowText(csValidate);
	if (csValidate == "")
	{
		AfxMessageBox(CMsg("IDS_NODESC"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_csXCoord.TrimLeft();
	if ((m_csXCoord == "") || (atoi(m_csXCoord) < 0) || (m_csXCoord.SpanIncluding("0123456789-") != m_csXCoord))
	{
		AfxMessageBox(CFMsg(CMsg("IDS_INVALCOORD"),"X"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_csYCoord.TrimLeft();
	if ((m_csYCoord == "") || (atoi(m_csYCoord) < 0) || (m_csYCoord.SpanIncluding("0123456789-") != m_csYCoord))
	{
		AfxMessageBox(CFMsg(CMsg("IDS_INVALCOORD"),"Y"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_csZCoord.TrimLeft();
	if ((m_csZCoord == "") || (m_csZCoord.SpanIncluding("0123456789-") != m_csZCoord) || (atoi(m_csZCoord) < -128) || (atoi(m_csZCoord) > 128))
	{
		AfxMessageBox(CFMsg(CMsg("IDS_INVALCOORD"),"Z"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_csPlane.TrimLeft();
	if ((m_csPlane == "") || (m_csPlane.SpanIncluding("0123456789-") != m_csPlane) || (atoi(m_csPlane) < 0) || (atoi(m_csPlane) > 255))
	{
		AfxMessageBox(CMsg("IDS_INVALMAP"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}


	CDialog::OnOK();
}

BOOL CAddDestination::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(CMsg("IDS_ADDDESTINATION"));
	UpdateData(false);
	if (!m_pCategoryList->IsEmpty())
	{
		POSITION pos = m_pCategoryList->GetHeadPosition();
		while (pos != NULL)
		{
			CCategory * pCategory = (CCategory *) m_pCategoryList->GetNext(pos);
			int iIndex = m_ccbCategory.AddString(pCategory->m_csName);
			m_ccbCategory.SetItemData(iIndex, 0);
		}
		m_ccbCategory.SetCurSel(0);
		this->OnSelchangeCategory();
	}

	this->m_ccbCategory.SelectString(-1, m_csCategory);
	this->OnSelchangeCategory();
	this->m_ccbSubsection.SelectString(-1, m_csSubsection);

	m_ctCategory.SetWindowText(CMsg("IDS_CATEGORY"));
	m_ctSusection.SetWindowText(CMsg("IDS_SUBSECTION"));
	m_ctDescription.SetWindowText(CMsg("IDS_DESCRIPTION"));
	m_ctX.SetWindowText(CMsg("IDS_X"));
	m_ctY.SetWindowText(CMsg("IDS_Y"));
	m_ctZ.SetWindowText(CMsg("IDS_Z"));
	m_ctMap.SetWindowText(CMsg("IDS_MAP"));
	m_cbOk.SetWindowText(CMsg("IDS_OK"));
	m_cbCancel.SetWindowText(CMsg("IDS_CANCEL"));

	return TRUE;
}

void CAddDestination::OnSelchangeCategory() 
{
	if ( m_ccbCategory.GetCurSel() == -1 )
		return;
	CString csCategory;
	m_ccbCategory.GetLBText(m_ccbCategory.GetCurSel(), csCategory);
	if ( (csCategory != "") && (m_ccbCategory.GetItemData(m_ccbCategory.GetCurSel()) == 0))
	{
		m_ccbSubsection.ResetContent();
		CCategory * pCategory = FindCategory(m_pCategoryList, csCategory);
		if (pCategory == NULL)
			return;
		if (!pCategory->m_SubsectionList.IsEmpty())
		{
			POSITION pos = pCategory->m_SubsectionList.GetHeadPosition();
			while (pos != NULL)
			{
				CSubsection * pSubsection = (CSubsection *) pCategory->m_SubsectionList.GetNext(pos);
				int iIndex = m_ccbSubsection.AddString(pSubsection->m_csName);
				m_ccbSubsection.SetItemData(iIndex, 0);
			}
		}
		m_ccbSubsection.SetCurSel(0);
	}
	else
	{
		m_ccbSubsection.ResetContent();
		m_ccbSubsection.SetCurSel(-1);
	}
}

void CAddDestination::OnEditchangeCategory()
{
	m_ccbSubsection.ResetContent();
	CString csCategory;
	m_ccbCategory.GetWindowText(csCategory);
	if (m_ccbCategory.FindStringExact(-1,csCategory) != CB_ERR)
	{
		m_ccbCategory.SelectString(-1,csCategory);
		this->OnSelchangeCategory();
	}
}