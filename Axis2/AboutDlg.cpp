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

// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_LICENSE, m_ceLicense);
	DDX_Control(pDX, IDC_COPYRIGHT, m_ceCopyright);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, cb_ok);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(CMsg("IDS_ABOUTBOX"));
	cb_ok.SetWindowText(CMsg("IDS_OK"));

	CString lf, lf2;
	lf.Format("%c%c", 0x0d, 0x0a);
	lf2.Format("%c%c%c%c", 0x0d, 0x0a, 0x0d, 0x0a);

	CString csCopyright = Main->GetVersionTitle();
	csCopyright += lf;
	csCopyright += CFMsg(IDS_COPY1,CTime::GetCurrentTime().GetYear());
	csCopyright += lf;
	csCopyright += CFMsg(IDS_COPY2,Main->GetBuildTimestamp());
	csCopyright += lf;
	csCopyright += CFMsg(IDS_COPY3,SERVER_VERSION);
	csCopyright += lf2;
	csCopyright += CMsg(IDS_COPY4);
	csCopyright += lf;
	csCopyright += CMsg(IDS_COPY5);
	csCopyright += lf;
	csCopyright += CMsg(IDS_COPY6);
	m_ceCopyright.SetWindowText(csCopyright);

	CString csLicense = CMsg(IDS_LICENCE1);
	csLicense += lf2;
	csLicense += CMsg(IDS_LICENCE2);
	csLicense += lf2;
	csLicense += CMsg(IDS_LICENCE3);
	m_ceLicense.SetWindowText(csLicense);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnCancel() 
{
	CDialog::OnCancel();
}
