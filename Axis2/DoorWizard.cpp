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

// DoorWizard.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "DoorWizard.h"
#include "UOart.h"


// CDoorWizard dialog

IMPLEMENT_DYNAMIC(CDoorWizard, CDialog)

CDoorWizard::CDoorWizard(CWnd* pParent /*=NULL*/)
	: CDialog(CDoorWizard::IDD, pParent)
{

}

CDoorWizard::~CDoorWizard()
{
	for ( int i = 0; i < m_aDoors.GetSize(); i++ )
	{
		CDoorObj * pDoor = (CDoorObj *) m_aDoors.GetAt(i);
		delete pDoor;
	}
	m_aDoors.RemoveAll();
}

void CDoorWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOORNW, m_DoorNW);
	DDX_Control(pDX, IDC_DOORNE, m_DoorNE);
	DDX_Control(pDX, IDC_DOORSW, m_DoorSW);
	DDX_Control(pDX, IDC_DOORSE, m_DoorSE);
	DDX_Control(pDX, IDC_NORTH, m_North);
	DDX_Control(pDX, IDC_WEST, m_West);
	DDX_Control(pDX, IDC_DOORLIST, m_DoorList);
}


BEGIN_MESSAGE_MAP(CDoorWizard, CDialog)
	ON_BN_CLICKED(IDC_NORTH, OnNorth)
	ON_BN_CLICKED(IDC_WEST, OnWest)
	ON_LBN_SELCHANGE(IDC_DOORLIST, OnDoorChange)
	ON_WM_CTLCOLOR()
	ON_STN_DBLCLK(IDC_DOORNW, OnAddDoorNW)
	ON_STN_DBLCLK(IDC_DOORNE, OnAddDoorNE)
	ON_STN_DBLCLK(IDC_DOORSW, OnAddDoorSW)
	ON_STN_DBLCLK(IDC_DOORSE, OnAddDoorSE)
END_MESSAGE_MAP()


BOOL CDoorWizard::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!LoadList())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	for ( int i = 0; i < m_aDoors.GetSize(); i++ )
	{
		CDoorObj * pDoor = (CDoorObj *) m_aDoors.GetAt(i);
		m_DoorList.InsertString(i,pDoor->m_csName);
		m_DoorList.SetItemData(i, (DWORD_PTR)pDoor);
	}

	m_DoorNW.SetArtType(1);
	m_DoorNE.SetArtType(1);
	m_DoorSW.SetArtType(1);
	m_DoorSE.SetArtType(1);

	wFlags = 0x102;
	m_DoorNW.SetDrawFlags(wFlags);
	wFlags = 0x202;
	m_DoorNE.SetDrawFlags(wFlags);
	wFlags = 0x402;
	m_DoorSW.SetDrawFlags(wFlags);
	wFlags = 0x802;
	m_DoorSE.SetDrawFlags(wFlags);

	m_DoorList.SetCurSel(0);
	m_North.SetCheck(1);
	OnNorth();


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

void CDoorWizard::OnCancel() 
{
	Main->m_dlgDoorWiz = NULL;
	CDialog::OnCancel();
}

void CDoorWizard::OnNorth()
{
	wFlags = m_DoorNW.GetDrawFlags();
	wFlags &= ~0x020;
	wFlags |= 0x010;
	m_DoorNW.SetDrawFlags(wFlags);
	m_DoorNW.SetYOffset(10);
	m_DoorNW.SetXOffset(11);
	
	wFlags = m_DoorNE.GetDrawFlags();
	wFlags &= ~0x020;
	wFlags |= 0x010;
	m_DoorNE.SetDrawFlags(wFlags);
	m_DoorNE.SetYOffset(0);
	m_DoorNE.SetXOffset(0);

	wFlags = m_DoorSW.GetDrawFlags();
	wFlags &= ~0x020;
	wFlags |= 0x010;
	m_DoorSW.SetDrawFlags(wFlags);
	m_DoorSW.SetYOffset(0);
	m_DoorSW.SetXOffset(22);

	wFlags = m_DoorSE.GetDrawFlags();
	wFlags &= ~0x020;
	wFlags |= 0x010;
	m_DoorSE.SetDrawFlags(wFlags);
	m_DoorSE.SetYOffset(-11);
	m_DoorSE.SetXOffset(11);

	CDoorObj * pDoor = (CDoorObj *) m_DoorList.GetItemData(m_DoorList.GetCurSel());
	m_DoorNW.SetArtIndex(alltoi(pDoor->m_csNNW));
	m_DoorNE.SetArtIndex(alltoi(pDoor->m_csNNE));
	m_DoorSW.SetArtIndex(alltoi(pDoor->m_csNSW));
	m_DoorSE.SetArtIndex(alltoi(pDoor->m_csNSE));
}

void CDoorWizard::OnWest()
{
	wFlags = m_DoorNW.GetDrawFlags();
	wFlags &= ~0x010;
	wFlags |= 0x020;
	m_DoorNW.SetDrawFlags(wFlags);
	m_DoorNW.SetYOffset(0);
	m_DoorNW.SetXOffset(0);
	
	wFlags = m_DoorNE.GetDrawFlags();
	wFlags &= ~0x010;
	wFlags |= 0x020;
	m_DoorNE.SetDrawFlags(wFlags);
	m_DoorNE.SetYOffset(10);
	m_DoorNE.SetXOffset(-10);

	wFlags = m_DoorSW.GetDrawFlags();
	wFlags &= ~0x010;
	wFlags |= 0x020;
	m_DoorSW.SetDrawFlags(wFlags);
	m_DoorSW.SetYOffset(-10);
	m_DoorSW.SetXOffset(-10);

	wFlags = m_DoorSE.GetDrawFlags();
	wFlags &= ~0x010;
	wFlags |= 0x020;
	m_DoorSE.SetDrawFlags(wFlags);
	m_DoorSE.SetYOffset(0);
	m_DoorSE.SetXOffset(-22);

	CDoorObj * pDoor = (CDoorObj *) m_DoorList.GetItemData(m_DoorList.GetCurSel());
	m_DoorNW.SetArtIndex(alltoi(pDoor->m_csWNW));
	m_DoorNE.SetArtIndex(alltoi(pDoor->m_csWNE));
	m_DoorSW.SetArtIndex(alltoi(pDoor->m_csWSW));
	m_DoorSE.SetArtIndex(alltoi(pDoor->m_csWSE));
}

void CDoorWizard::OnDoorChange()
{
	if (m_DoorList.GetCurSel() == -1)
		return;

	CDoorObj * pDoor = (CDoorObj *) m_DoorList.GetItemData(m_DoorList.GetCurSel());
	
	wFlags = m_DoorNW.GetDrawFlags();
	if (wFlags & F_SWINGNORTH)
	{
		m_DoorNW.SetArtIndex(alltoi(pDoor->m_csNNW));
		m_DoorNE.SetArtIndex(alltoi(pDoor->m_csNNE));
		m_DoorSW.SetArtIndex(alltoi(pDoor->m_csNSW));
		m_DoorSE.SetArtIndex(alltoi(pDoor->m_csNSE));
	}
	else
	{
		m_DoorNW.SetArtIndex(alltoi(pDoor->m_csWNW));
		m_DoorNE.SetArtIndex(alltoi(pDoor->m_csWNE));
		m_DoorSW.SetArtIndex(alltoi(pDoor->m_csWSW));
		m_DoorSE.SetArtIndex(alltoi(pDoor->m_csWSE));
	}
}

bool CDoorWizard::LoadList()
{
	try
	{
		//Load DoorWiz.ini
		CString csFile;
		csFile.Format("%sDoorWiz.ini",Main->m_csRootDirectory);
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
				CDoorObj * pDoor = new (CDoorObj);
				pDoor->m_csName = csLine.SpanExcluding(":");
				csLine = csLine.Mid(csLine.Find(":") + 1);
				pDoor->m_csNNW = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csNNE = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csNSW = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csNSE = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csWNW = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csWNE = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csWSW = csLine.SpanExcluding(",");
				csLine = csLine.Mid(csLine.Find(",") + 1);
				pDoor->m_csWSE = csLine;

				m_aDoors.Add(pDoor);
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

HBRUSH CDoorWizard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	return Main->m_bkbrush;
}
void CDoorWizard::OnAddDoorNW()
{
	CString csCmd;
	csCmd.Format("%sstatic %d", Main->m_csCommandPrefix, m_DoorNW.GetArtIndex());
	SendToUO(csCmd);
}

void CDoorWizard::OnAddDoorNE()
{
	CString csCmd;
	csCmd.Format("%sstatic %d", Main->m_csCommandPrefix, m_DoorNE.GetArtIndex());
	SendToUO(csCmd);
}

void CDoorWizard::OnAddDoorSW()
{
	CString csCmd;
	csCmd.Format("%sstatic %d", Main->m_csCommandPrefix, m_DoorSW.GetArtIndex());
	SendToUO(csCmd);
}

void CDoorWizard::OnAddDoorSE()
{
	CString csCmd;
	csCmd.Format("%sstatic %d", Main->m_csCommandPrefix, m_DoorSE.GetArtIndex());
	SendToUO(csCmd);
}

