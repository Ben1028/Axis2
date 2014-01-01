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

// ProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressBar dialog


CProgressBar::CProgressBar(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressBar)
	//}}AFX_DATA_INIT
	m_iUpper = 100;
	m_iLower = 0;
	m_iPos = 0;
	m_cBkg = GetSysColor(COLOR_3DFACE);
	m_cFgd = 0x00800000;
	NONCLIENTMETRICS met;
	memset(&met, 0, sizeof(met));
	met.cbSize = sizeof(met);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, met.cbSize, (void *) &met, 0);
	m_csFontName.Format("%s", met.lfStatusFont.lfFaceName);
}


void CProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressBar)
	DDX_Control(pDX, IDC_PROGBAR, m_csProgBar);
	DDX_Control(pDX, IDC_STATUS_MESSAGE, m_csMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressBar, CDialog)
	//{{AFX_MSG_MAP(CProgressBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressBar message handlers

void CProgressBar::Update()
{
	if ( m_iUpper == m_iLower )
		return;
	CDC * pDC = this->m_csProgBar.GetDC();

	pDC->SetTextAlign(TA_CENTER);
	CString csPercent;
	csPercent.Format("%ld%%", ( (m_iPos * 100) / (m_iUpper - m_iLower)) );

	CRect rect;
	m_csProgBar.GetClientRect(&rect);
	CRgn clip, clip2;
	COLORREF oldBkg, oldText;
	oldBkg = pDC->SetBkColor(m_cFgd);
	oldText = pDC->SetTextColor(m_cBkg);
	CBrush * pFore = new CBrush(m_cFgd);
	CBrush * pBack = new CBrush(m_cBkg);
	CFont * pFont = new CFont();
	pFont->CreateFont(15, 0, 0, 0, 600, 0, 0, 0, 0, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, DEFAULT_PITCH, m_csFontName);
	CFont * pOldFont = pDC->SelectObject(pFont);
	int right = rect.left + ( rect.Width() * m_iPos / (m_iUpper - m_iLower) );
	clip.CreateRectRgn(rect.left, rect.top, right, rect.bottom);
	pDC->SelectClipRgn(&clip);
	pDC->FillRect(rect, pFore);
	pDC->ExtTextOut( ((rect.right - rect.left) / 2), rect.top - 1, ETO_CLIPPED, &rect, csPercent, NULL);
	clip2.CreateRectRgn(right, rect.top, rect.right, rect.bottom);
	pDC->SelectClipRgn(&clip2);
	pDC->FillRect(rect, pBack);
	pDC->SetBkColor(m_cBkg);
	pDC->SetTextColor(m_cFgd);
	pDC->ExtTextOut( ((rect.right - rect.left) / 2), rect.top - 1, ETO_CLIPPED, &rect, csPercent, NULL);
	pDC->SelectObject(pOldFont);
	delete pFore;
	delete pBack;
	delete pFont;
	pDC->SetBkColor(oldBkg);
	pDC->SetTextColor(oldText);
	m_csProgBar.ReleaseDC(pDC);
	this->TopMost();
}

void CProgressBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Update();	
	// Do not call CDialog::OnPaint() for painting messages
}