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

// LightWizard.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "LightWizard.h"
#include "UOart.h"


// CLightWizard dialog

IMPLEMENT_DYNAMIC(CLightWizard, CDialog)

CLightWizard::CLightWizard(CWnd* pParent /*=NULL*/)
	: CDialog(CLightWizard::IDD, pParent)
{

}

CLightWizard::~CLightWizard()
{
	for ( int i = 0; i < m_aLights.GetSize(); i++ )
	{
		CLightObj * pLight = (CLightObj *) m_aLights.GetAt(i);
		delete pLight;
	}
	m_aLights.RemoveAll();
}

void CLightWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHTLIST, m_LightList);
	DDX_Control(pDX, IDC_SCROLLLIGHT, m_csbScrollLight);
	DDX_Control(pDX, IDC_LIGHT1, m_Light1);
	DDX_Control(pDX, IDC_LIGHT2, m_Light2);
	DDX_Control(pDX, IDC_LIGHT3, m_Light3);
	DDX_Control(pDX, IDC_LIGHTLEVEL, m_cscLightLevel);
	DDX_Control(pDX, IDC_LIGHTVALUE, m_csLightValue);
}


BEGIN_MESSAGE_MAP(CLightWizard, CDialog)
	ON_LBN_SELCHANGE(IDC_LIGHTLIST, OnLightChange)
	ON_STN_DBLCLK(IDC_LIGHT1, OnAddLight1)
	ON_STN_DBLCLK(IDC_LIGHT2, OnAddLight2)
	ON_STN_DBLCLK(IDC_LIGHT3, OnAddLight3)
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()


BOOL CLightWizard::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!LoadList())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	for ( int i = 0; i < m_aLights.GetSize(); i++ )
	{
		CLightObj * pLight = (CLightObj *) m_aLights.GetAt(i);
		m_LightList.InsertString(i,pLight->m_csName);
		m_LightList.SetItemData(i, (DWORD_PTR)pLight);
	}

	m_Light1.SetArtType(8);
	m_Light2.SetArtType(8);
	m_Light3.SetArtType(8);

	wFlags = F_BG_GRASS;
	m_Light1.SetDrawFlags(wFlags);
	m_Light2.SetDrawFlags(wFlags);
	m_Light3.SetDrawFlags(wFlags);

	m_cscLightLevel.SetRange(0, 30);
	m_cscLightLevel.SetPos(21);
	CString csValue;
	csValue.Format("%02ld", m_cscLightLevel.GetPos());
	m_csLightValue.SetWindowText(csValue);
	m_csbScrollLight.EnableWindow(false);
	m_LightList.SetCurSel(0);
	OnLightChange();

	CRect rectFrame, rectDlg;
	CWnd* pMainWnd = AfxGetMainWnd();
	if(pMainWnd != NULL)
	{
		pMainWnd->GetClientRect(rectFrame);
		pMainWnd->ClientToScreen(rectFrame);
		GetWindowRect(rectDlg);
		int nXPos = rectFrame.left + (rectFrame.Width() / 2) - (rectDlg.Width() / 2);
		int nYPos = rectFrame.top + (rectFrame.Height() / 2) - (rectDlg.Height() / 2);
		::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
	}
	return TRUE;

}

void CLightWizard::OnCancel() 
{
	Main->m_dlgLightWiz = NULL;
	CDialog::OnCancel();
}

void CLightWizard::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_cscLightLevel.m_hWnd)
	{
		CString csValue;
		csValue.Format("%02ld", m_cscLightLevel.GetPos());
		m_csLightValue.SetWindowText(csValue);
		m_Light1.ReloadArt();
		m_Light2.ReloadArt();
		m_Light3.ReloadArt();
		return;
	}

	if (pScrollBar->m_hWnd == m_csbScrollLight.m_hWnd)
	{
		int minpos;
		int maxpos;
		pScrollBar->GetScrollRange(&minpos, &maxpos); 
		int curpos = pScrollBar->GetScrollPos();
		switch (nSBCode)
		{
		case SB_TOP:      // Scroll to far left.
			curpos = minpos;
			break;
		case SB_BOTTOM:      // Scroll to far right.
			curpos = maxpos;
			break;
		case SB_ENDSCROLL:   // End scroll.
			break;
		case SB_LINEUP:      // Scroll left.
			if (curpos > minpos)
				curpos--;
			break;
		case SB_LINEDOWN:   // Scroll right.
			if (curpos < maxpos)
				curpos++;
			break;
		case SB_PAGEUP:    // Scroll one page left.
		{
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			if (curpos > minpos)
				curpos = max(minpos, curpos - (int) info.nPage);
		}
			break;
		case SB_PAGEDOWN:      // Scroll one page right.
		{
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);
			if (curpos < maxpos)
				curpos = min(maxpos, curpos + (int) info.nPage);
		}
			break;
		case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
			curpos = nPos;      // of the scroll box at the end of the drag operation.
			break;
		case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
			curpos = nPos;     // position that the scroll box has been dragged to.
			break;
		}

		pScrollBar->SetScrollPos(curpos);

		if (m_LightList.GetCurSel() == -1)
			return;
		CLightObj * pLight = (CLightObj *) m_LightList.GetItemData(m_LightList.GetCurSel());
		m_Light1.SetArtIndex(alltoi(pLight->m_Light.GetAt(curpos)));
		m_Light2.SetArtIndex(alltoi(pLight->m_Light.GetAt(curpos+1)));
		m_Light3.SetArtIndex(alltoi(pLight->m_Light.GetAt(curpos+2)));
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLightWizard::OnLightChange()
{
		if (m_LightList.GetCurSel() == -1)
		return;

	CLightObj * pLight = (CLightObj *) m_LightList.GetItemData(m_LightList.GetCurSel());
	m_Light1.SetArtIndex(alltoi(pLight->m_Light.GetAt(0)));

	if (pLight->m_Light.GetCount() > 1)
		m_Light2.SetArtIndex(alltoi(pLight->m_Light.GetAt(1)));
	else
		m_Light2.SetArtIndex(-1);

	if (pLight->m_Light.GetCount() > 2)
		m_Light3.SetArtIndex(alltoi(pLight->m_Light.GetAt(2)));
	else
		m_Light3.SetArtIndex(-1);

	if (pLight->m_Light.GetCount() > 3)
	{
		m_csbScrollLight.EnableWindow(true);
		m_csbScrollLight.SetScrollRange(0, (int)pLight->m_Light.GetCount()-3);
		m_csbScrollLight.SetScrollPos(0);
	}
	else
		m_csbScrollLight.EnableWindow(false);
}

bool CLightWizard::LoadList()
{
	try
	{
		//Load LightWiz.ini
		CString csFile;
		csFile.Format("%sLightWiz.ini",Main->m_csRootDirectory);
		CStdioFile pFile;
		if ( !pFile.Open(csFile, CFile::modeRead | CFile::shareDenyNone) )
		{
			Main->m_log.Add(1,"ERROR: Unable to open file %s", csFile);
			return false;
		}

		BOOL bStatus = TRUE;
		while ( bStatus )
		{
			CString csLine;
			bStatus = pFile.ReadString(csLine);

			if ( !bStatus )
				break;

			csLine = csLine.SpanExcluding("//");
			csLine.Trim(); 
			if ( csLine != "" )
			{
				CLightObj * pLight = new (CLightObj);
				pLight->m_csName = csLine.SpanExcluding(":");
				csLine = csLine.Mid(csLine.Find(":") + 1);
				pLight->m_Light.Add(csLine.SpanExcluding(","));
				while (csLine.Find(",") != -1)
				{
					csLine = csLine.Mid(csLine.Find(",") + 1);
					pLight->m_Light.Add(csLine.SpanExcluding(","));
				}

				m_aLights.Add(pLight);
			}
		}
	}
	catch (CFileException *e)
	{
		Main->m_log.Add(1,"ERROR: Caught an exception while reading the file %s.  Cause code = %ld", e->m_strFileName, e->m_cause);
		e->Delete();
	}
	return true;
}

HBRUSH CLightWizard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	return Main->m_bkbrush;
}


void CLightWizard::OnAddLight1()
{
	CString csCmd;
	csCmd.Format("%sset morez %d", Main->m_csCommandPrefix, m_Light1.GetArtIndex());
	SendToUO(csCmd);
}

void CLightWizard::OnAddLight2()
{
	CString csCmd;
	csCmd.Format("%sset morez %d", Main->m_csCommandPrefix, m_Light2.GetArtIndex());
	SendToUO(csCmd);
}

void CLightWizard::OnAddLight3()
{
	CString csCmd;
	csCmd.Format("%sset morez %d", Main->m_csCommandPrefix, m_Light3.GetArtIndex());
	SendToUO(csCmd);
}

