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

// LogTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "LogTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogTab property page

IMPLEMENT_DYNCREATE(CLogTab, CDockingPage)

CLogTab::CLogTab() : CDockingPage(CLogTab::IDD,CMsg("IDS_LOGS"))
{
	//{{AFX_DATA_INIT(CLogTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bVisible = false;
}

CLogTab::~CLogTab()
{
}

void CLogTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTab)
	DDX_Control(pDX, IDC_FRAMELOG, m_ceLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogTab, CDockingPage)
	//{{AFX_MSG_MAP(CLogTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTab message handlers

BOOL CLogTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppAxisLogTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CLogTab;
	Main->m_pcppAxisLogTab->m_dcCurrentPage = this;
	m_bAlwaysColor = true;

	CDockingPage::OnInitDialog();

	if((m_bModifyDlgStylesAndPos == true) && m_hWnd)
	{
		POSITION pos = Main->m_olLog.GetHeadPosition();
		long nInsertionPoint = 0;
		while (pos != NULL)
		{
			CLogArray * pLog = (CLogArray *) Main->m_olLog.GetNext(pos);
			m_ceLog.SetSel(nInsertionPoint, -1);
			m_ceLog.SetSelectionCharFormat(pLog->m_cf);
			m_ceLog.ReplaceSel(pLog->m_csLine);
			nInsertionPoint = m_ceLog.GetWindowTextLength();
		}
		m_ceLog.SetSel(nInsertionPoint, -1);
	}

	return TRUE;
}

void CLogTab::AddText(CString csText, COLORREF color)
{
	csText += "\n";
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 160;
	cf.crTextColor = color;
	strcpy_s(cf.szFaceName, _T("Courier New"));

	if (m_ceLog)
	{
		int nOldLines = 0, nNewLines = 0, nScroll = 0;
		long nInsertionPoint = 0;
		nOldLines = m_ceLog.GetLineCount();
		nInsertionPoint = m_ceLog.GetWindowTextLength();
		m_ceLog.SetSel(nInsertionPoint, -1);
		m_ceLog.SetSelectionCharFormat(cf);
		m_ceLog.ReplaceSel(csText);
		nNewLines = m_ceLog.GetLineCount();
		nScroll = nNewLines - nOldLines;
		m_ceLog.LineScroll(nScroll);
		UpdateData(false);
	}
	CLogArray * pLogLine = new (CLogArray);
	pLogLine->m_csLine = csText;
	pLogLine->m_cf = cf;
	if (Main->m_olLog.IsEmpty())
		Main->m_olLog.AddHead(pLogLine);
	else
		Main->m_olLog.AddTail(pLogLine);
}

BOOL CLogTab::OnSetActive() 
{
	POSITION pos = Main->m_olLog.GetHeadPosition();
		long nInsertionPoint = 0;
		while (pos != NULL)
		{
			CLogArray * pLog = (CLogArray *) Main->m_olLog.GetNext(pos);
			m_ceLog.SetSel(nInsertionPoint, -1);
			m_ceLog.SetSelectionCharFormat(pLog->m_cf);
			m_ceLog.ReplaceSel(pLog->m_csLine);
			nInsertionPoint = m_ceLog.GetWindowTextLength();
		}
		m_ceLog.SetSel(nInsertionPoint, -1);
		m_ceLog.SetOptions(ECOOP_OR,ECO_SAVESEL);
	return CDockingPage::OnSetActive();
}