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

// AccountTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "AccountTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemTab property page

IMPLEMENT_DYNCREATE(CAccountTab, CDockingPage)

CAccountTab::CAccountTab() : CDockingPage(CAccountTab::IDD,CMsg("IDS_ACCOUNT"))
{
	//{{AFX_DATA_INIT(CAccountTab)
	iPrivLevel = 0;
	iResdisp = 0;
	m_bPriv02 = FALSE;
	m_bPriv08 = FALSE;
	m_bPriv010 = FALSE;
	m_bPriv020 = FALSE;
	m_bPriv040 = FALSE;
	m_bPriv080 = FALSE;
	m_bPriv0200 = FALSE;
	m_bPriv0400 = FALSE;
	m_bPriv0800 = FALSE;
	m_bPriv02000 = FALSE;
	m_bPriv04000 = FALSE;
	//}}AFX_DATA_INIT
}

CAccountTab::~CAccountTab()
{
}

void CAccountTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountTab)
	DDX_Check(pDX, IDC_PRIV02, m_bPriv02);
	DDX_Check(pDX, IDC_PRIV08, m_bPriv08);
	DDX_Check(pDX, IDC_PRIV010, m_bPriv010);
	DDX_Check(pDX, IDC_PRIV020, m_bPriv020);
	DDX_Check(pDX, IDC_PRIV040, m_bPriv040);
	DDX_Check(pDX, IDC_PRIV080, m_bPriv080);
	DDX_Check(pDX, IDC_PRIV0200, m_bPriv0200);
	DDX_Check(pDX, IDC_PRIV0400, m_bPriv0400);
	DDX_Check(pDX, IDC_PRIV0800, m_bPriv0800);
	DDX_Check(pDX, IDC_PRIV02000, m_bPriv02000);
	DDX_Check(pDX, IDC_PRIV04000, m_bPriv04000);
	DDX_Control(pDX, IDC_PRIVVALUE, m_csPrivValue);

	DDX_Control(pDX, IDC_SETRESDISP, m_cbSetResdisp);
	DDX_Control(pDX, IDC_SETPRIV, m_cbSetPriv);
	DDX_Control(pDX, IDC_SETPRIVS, m_cbSetPrivs);
	DDX_Control(pDX, IDC_RESET_PRIVS, m_cbResetPrivs);

	DDX_Control(pDX, IDC_PRIV02, m_ccPriv02);
	DDX_Control(pDX, IDC_PRIV08, m_ccPriv08);
	DDX_Control(pDX, IDC_PRIV010, m_ccPriv010);
	DDX_Control(pDX, IDC_PRIV020, m_ccPriv020);
	DDX_Control(pDX, IDC_PRIV040, m_ccPriv040);
	DDX_Control(pDX, IDC_PRIV080, m_ccPriv080);
	DDX_Control(pDX, IDC_PRIV0200, m_ccPriv0200);
	DDX_Control(pDX, IDC_PRIV0400, m_ccPriv0400);
	DDX_Control(pDX, IDC_PRIV0800, m_ccPriv0800);
	DDX_Control(pDX, IDC_PRIV02000, m_ccPriv02000);
	DDX_Control(pDX, IDC_PRIV04000, m_ccPriv04000);

	DDX_Control(pDX, IDC_RESDISP0, m_crResdisp0);
	DDX_Control(pDX, IDC_RESDISP1, m_crResdisp1);
	DDX_Control(pDX, IDC_RESDISP2, m_crResdisp2);
	DDX_Control(pDX, IDC_RESDISP3, m_crResdisp3);
	DDX_Control(pDX, IDC_RESDISP4, m_crResdisp4);
	DDX_Control(pDX, IDC_RESDISP5, m_crResdisp5);
	DDX_Control(pDX, IDC_RESDISP6, m_crResdisp6);
	DDX_Control(pDX, IDC_RESDISP7, m_crResdisp7);

	DDX_Control(pDX, IDC_GUESTPRIVS, m_crPlvl0);
	DDX_Control(pDX, IDC_PLAYERPRIVS, m_crPlvl1);
	DDX_Control(pDX, IDC_COUNSELORPRIVS, m_crPlvl2);
	DDX_Control(pDX, IDC_SEERPRIVS, m_crPlvl3);
	DDX_Control(pDX, IDC_GMPRIVS, m_crPlvl4);
	DDX_Control(pDX, IDC_DEVELOPERPRIVS, m_crPlvl5);
	DDX_Control(pDX, IDC_ADMINISTRATORPRIVS, m_crPlvl6);
	DDX_Control(pDX, IDC_OWNERPRIVS, m_crPlvl7);

	DDX_Control(pDX, IDC_GPLVL, m_cgPlvl);
	DDX_Control(pDX, IDC_GPRIV, m_cgPriv);
	DDX_Control(pDX, IDC_GRESDISP, m_cgResdisp);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountTab, CDockingPage)
	//{{AFX_MSG_MAP(CAccountTab)
	ON_BN_CLICKED(IDC_SETPRIV, OnSetpriv)
	ON_BN_CLICKED(IDC_OWNERPRIVS, OnOwnerprivs)
	ON_BN_CLICKED(IDC_ADMINISTRATORPRIVS, OnAdministratorprivs)
	ON_BN_CLICKED(IDC_DEVELOPERPRIVS, OnDeveloperprivs)
	ON_BN_CLICKED(IDC_GMPRIVS, OnGmprivs)
	ON_BN_CLICKED(IDC_SEERPRIVS, OnSeerprivs)
	ON_BN_CLICKED(IDC_COUNSELORPRIVS, OnCounselorprivs)
	ON_BN_CLICKED(IDC_PLAYERPRIVS, OnPlayerprivs)
	ON_BN_CLICKED(IDC_GUESTPRIVS, OnGuestprivs)
	ON_BN_CLICKED(IDC_RESDISP0, OnResdisp0)
	ON_BN_CLICKED(IDC_RESDISP1, OnResdisp1)
	ON_BN_CLICKED(IDC_RESDISP2, OnResdisp2)
	ON_BN_CLICKED(IDC_RESDISP3, OnResdisp3)
	ON_BN_CLICKED(IDC_RESDISP4, OnResdisp4)
	ON_BN_CLICKED(IDC_RESDISP5, OnResdisp5)
	ON_BN_CLICKED(IDC_RESDISP6, OnResdisp6)
	ON_BN_CLICKED(IDC_RESDISP7, OnResdisp7)
	ON_BN_CLICKED(IDC_SETRESDISP, OnSetresdisp)
	ON_BN_CLICKED(IDC_PRIV02, OnPriv02)
	ON_BN_CLICKED(IDC_PRIV08, OnPriv08)
	ON_BN_CLICKED(IDC_PRIV010, OnPriv010)
	ON_BN_CLICKED(IDC_PRIV020, OnPriv020)
	ON_BN_CLICKED(IDC_PRIV040, OnPriv040)
	ON_BN_CLICKED(IDC_PRIV080, OnPriv080)
	ON_BN_CLICKED(IDC_PRIV0200, OnPriv0200)
	ON_BN_CLICKED(IDC_PRIV0400, OnPriv0400)
	ON_BN_CLICKED(IDC_PRIV0800, OnPriv0800)
	ON_BN_CLICKED(IDC_PRIV02000, OnPriv02000)
	ON_BN_CLICKED(IDC_PRIV04000, OnPriv04000)
	ON_BN_CLICKED(IDC_SETPRIVS, OnSetprivs)
	ON_BN_CLICKED(IDC_RESET_PRIVS, OnResetPrivs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountTab message handlers

BOOL CAccountTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppAccountTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CAccountTab;
	Main->m_pcppAccountTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	m_cbSetResdisp.SetWindowText(CMsg("IDS_SETBUTTON"));
	m_cbSetPriv.SetWindowText(CMsg("IDS_SETBUTTON"));
	m_cbSetPrivs.SetWindowText(CMsg("IDS_SETBUTTON"));
	m_cbResetPrivs.SetWindowText(CMsg("IDS_RESETBUTTON"));
	m_ccPriv02.SetWindowText(CMsg("IDS_PRIV00002"));
	m_ccPriv08.SetWindowText(CMsg("IDS_PRIV00008"));
	m_ccPriv010.SetWindowText(CMsg("IDS_PRIV00010"));
	m_ccPriv020.SetWindowText(CMsg("IDS_PRIV00020"));
	m_ccPriv040.SetWindowText(CMsg("IDS_PRIV00040"));
	m_ccPriv080.SetWindowText(CMsg("IDS_PRIV00080"));
	m_ccPriv0200.SetWindowText(CMsg("IDS_PRIV00200"));
	m_ccPriv0400.SetWindowText(CMsg("IDS_PRIV00400"));
	m_ccPriv0800.SetWindowText(CMsg("IDS_PRIV00800"));
	m_ccPriv02000.SetWindowText(CMsg("IDS_PRIV02000"));
	m_ccPriv04000.SetWindowText(CMsg("IDS_PRIV04000"));
	m_crResdisp0.SetWindowText(CMsg("IDS_RESDISP0"));
	m_crResdisp1.SetWindowText(CMsg("IDS_RESDISP1"));
	m_crResdisp2.SetWindowText(CMsg("IDS_RESDISP2"));
	m_crResdisp3.SetWindowText(CMsg("IDS_RESDISP3"));
	m_crResdisp4.SetWindowText(CMsg("IDS_RESDISP4"));
	m_crResdisp5.SetWindowText(CMsg("IDS_RESDISP5"));
	m_crResdisp6.SetWindowText(CMsg("IDS_RESDISP6"));
	m_crResdisp7.SetWindowText(CMsg("IDS_RESDISP7"));
	m_crPlvl0.SetWindowText(CMsg("IDS_PLVL0"));
	m_crPlvl1.SetWindowText(CMsg("IDS_PLVL1"));
	m_crPlvl2.SetWindowText(CMsg("IDS_PLVL2"));
	m_crPlvl3.SetWindowText(CMsg("IDS_PLVL3"));
	m_crPlvl4.SetWindowText(CMsg("IDS_PLVL4"));
	m_crPlvl5.SetWindowText(CMsg("IDS_PLVL5"));
	m_crPlvl6.SetWindowText(CMsg("IDS_PLVL6"));
	m_crPlvl7.SetWindowText(CMsg("IDS_PLVL7"));

	m_cgPlvl.SetWindowText(CMsg("IDS_GPLVL"));
	m_cgPriv.SetWindowText(CMsg("IDS_GPRIV"));
	m_cgResdisp.SetWindowText(CMsg("IDS_GRESDISP"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAccountTab::OnSetActive() 
{
	UpdateData();

	return CDockingPage::OnSetActive();
}

void CAccountTab::OnSetpriv() 
{
	CString csCmd;
	csCmd.Format("%sprivset %d", Main->m_csCommandPrefix, iPrivLevel);
	SendToUO(csCmd);
}

void CAccountTab::OnOwnerprivs() 
{
	iPrivLevel = 7;	
}

void CAccountTab::OnAdministratorprivs() 
{
	iPrivLevel = 6;	
}

void CAccountTab::OnDeveloperprivs() 
{
	iPrivLevel = 5;	
}

void CAccountTab::OnGmprivs() 
{
	iPrivLevel = 4;	
}

void CAccountTab::OnSeerprivs() 
{
	iPrivLevel = 3;	
}

void CAccountTab::OnCounselorprivs() 
{
	iPrivLevel = 2;	
}

void CAccountTab::OnPlayerprivs() 
{
	iPrivLevel = 1;	
}

void CAccountTab::OnGuestprivs() 
{
	iPrivLevel = 0;	
}

void CAccountTab::OnResdisp0()
{
	iResdisp = 0;
}

void CAccountTab::OnResdisp1()
{
	iResdisp = 1;
}

void CAccountTab::OnResdisp2()
{
	iResdisp = 2;
}

void CAccountTab::OnResdisp3()
{
	iResdisp = 3;
}

void CAccountTab::OnResdisp4()
{
	iResdisp = 4;
}

void CAccountTab::OnResdisp5()
{
	iResdisp = 5;
}

void CAccountTab::OnResdisp6()
{
	iResdisp = 6;
}

void CAccountTab::OnResdisp7()
{
	iResdisp = 7;
}

void CAccountTab::OnSetresdisp() 
{
	CString csCmd;
	csCmd.Format("%sset account.resdisp %d", Main->m_csCommandPrefix, iResdisp);
	SendToUO(csCmd);
}

void CAccountTab::OnPriv02()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv02)
		dwVal |= PRIV_GM;
	else
		dwVal ^= PRIV_GM;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv08()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv08)
		dwVal |= PRIV_PAGE;
	else
		dwVal ^= PRIV_PAGE;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv010()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv010)
		dwVal |= PRIV_HEARALL;
	else
		dwVal ^= PRIV_HEARALL;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv020()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv020)
		dwVal |= PRIV_ALLMOVE;
	else
		dwVal ^= PRIV_ALLMOVE;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv040()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv040)
		dwVal |= PRIV_COMBATDETAIL;
	else
		dwVal ^= PRIV_COMBATDETAIL;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv080()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv080)
		dwVal |= PRIV_DEBUG;
	else
		dwVal ^= PRIV_DEBUG;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv0200()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv0200)
		dwVal |= PRIV_SHOWPRIV;
	else
		dwVal ^= PRIV_SHOWPRIV;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv0400()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv0400)
		dwVal |= PRIV_TELNET;
	else
		dwVal ^= PRIV_TELNET;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv0800()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv0800)
		dwVal |= PRIV_JAILED;
	else
		dwVal ^= PRIV_JAILED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv02000()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv02000)
		dwVal |= PRIV_BLOCKED;
	else
		dwVal ^= PRIV_BLOCKED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnPriv04000()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csPrivValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPriv04000)
		dwVal |= PRIV_ALLSHOW;
	else
		dwVal ^= PRIV_ALLSHOW;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csPrivValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CAccountTab::OnSetprivs()
{
	CString csVal;
	m_csPrivValue.GetWindowText(csVal);
	CString csCmd;
	csCmd.Format("%set account.priv %s", Main->m_csCommandPrefix, csVal);
	SendToUO(csCmd);
}

void CAccountTab::OnResetPrivs()
{
	m_bPriv02 = FALSE;
	m_bPriv08 = FALSE;
	m_bPriv010 = FALSE;
	m_bPriv020 = FALSE;
	m_bPriv040 = FALSE;
	m_bPriv080 = FALSE;
	m_bPriv0200 = FALSE;
	m_bPriv0400 = FALSE;
	m_bPriv0800 = FALSE;
	m_bPriv02000 = FALSE;
	m_bPriv04000 = FALSE;
	m_csPrivValue.SetWindowText("00000");
	UpdateData(false);
}