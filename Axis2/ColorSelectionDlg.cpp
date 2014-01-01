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

// ColorSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ColorSelectionDlg.h"
#include "UOart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorSelectionDlg dialog

IMPLEMENT_DYNAMIC(CColorSelectionDlg, CDialog)

CColorSelectionDlg::CColorSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorSelectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorSelectionDlg)
	//}}AFX_DATA_INIT
}


void CColorSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorSelectionDlg)
	DDX_Control(pDX, IDC_HUELIST, m_ccbHues);
	DDX_Control(pDX, IDC_TYPELIST, m_ccbTypeList);
	DDX_Control(pDX, IDC_ARTINDEX, m_ceArtIndex);
	DDX_Control(pDX, IDC_HUENAME, m_csHueName);
	DDX_Control(pDX, IDC_MOVINGCOLOR, m_csMovingIndex);
	DDX_Control(pDX, IDC_SPECTRUM, m_csSpectrum);
	DDX_Control(pDX, IDC_SHADESLIDER, m_sShade);
	DDX_Control(pDX, IDC_PALETTE, m_csPalette);
	DDX_Control(pDX, IDC_COLORINDEX, m_csColorIndex);
	DDX_Control(pDX, IDC_COLORPREVIEW, m_preview);
	DDX_Control(pDX, IDC_INTERNALNAME, cs_intname);
	DDX_Control(pDX, IDC_DARK, cs_dark);
	DDX_Control(pDX, IDC_COLINDEX, cs_colindex);
	DDX_Control(pDX, IDC_BRIGHT, cs_bright);
	DDX_Control(pDX, IDC_TYPE, cs_type);
	DDX_Control(pDX, IDC_INDEX, cs_index);
	DDX_Control(pDX, IDCANCEL, cb_cancel);
	DDX_Control(pDX, IDOK, cb_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(CColorSelectionDlg)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_TYPELIST, OnSelchangeTypelist)
	ON_EN_CHANGE(IDC_ARTINDEX, OnChangeArtindex)
	ON_CBN_SELCHANGE(IDC_HUELIST, OnSelchangeHuelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSelectionDlg message handlers

BOOL CColorSelectionDlg::OnInitDialog() 
{
	//CWaitCursor hourglass;
	CDialog::OnInitDialog();
	
	SetWindowText(CMsg("IDS_COLORSELECTION"));
	this->m_sShade.SetRange(0, 31, TRUE);
	this->m_sShade.SetPos(25);
	FillHueNames();
	UpdatePalette();
	LoadSettings();
	UpdatePreview();
	UpdateSpectrum();

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

	cs_intname.SetWindowText(CMsg("IDS_INTERNALNAME"));
	cs_dark.SetWindowText(CMsg("IDS_DARK"));
	cs_colindex.SetWindowText(CMsg("IDS_COLORINDEX"));
	cs_bright.SetWindowText(CMsg("IDS_BRIGHT"));
	cs_type.SetWindowText(CMsg("IDS_TYPE"));
	cs_index.SetWindowText(CMsg("IDS_INDEX"));
	cb_cancel.SetWindowText(CMsg("IDS_CANCEL"));
	cb_ok.SetWindowText(CMsg("IDS_OK"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorSelectionDlg::OnCancel() 
{
	Main->m_dlgColorPal = NULL;

	CItemTweakTab * pTweak = (CItemTweakTab *)Main->m_pcppItemTweakTab->m_dcCurrentPage;
	CString csColorIndex;
	csColorIndex.Format("%05x", pTweak->m_wColorIndex);
	Main->PutRegistryString("ColorPreviewColorIndex", csColorIndex);
	CDialog::OnCancel();
}

void CColorSelectionDlg::OnOK() 
{
	CItemTweakTab * pTweak = (CItemTweakTab *)Main->m_pcppItemTweakTab->m_dcCurrentPage;
	CString csIndex;
	m_csColorIndex.GetWindowText(csIndex);
	m_wColorIndex = (WORD) ahextoi(csIndex);

	pTweak->m_wColorIndex = m_wColorIndex;

	if (colorIndex != -1)
	{
		pTweak->m_wQuickColorIndex[colorIndex] = m_wColorIndex;

		CString csIndex;
		csIndex.Format("Index %d", colorIndex);
		Main->PutRegistryDword(csIndex, m_wColorIndex, hRegLocation, REGKEY_PALETTE);
		pTweak->UpdateQuikPick();
	}

	pTweak->DrawSpectrum(m_wColorIndex);

	CDialog::OnOK();
}

DWORD CColorSelectionDlg::ScaleColor(WORD wColor)
{
	DWORD dwNewColor;
	
	dwNewColor = ((((((wColor >> 10) & 0x01f) * 0x0ff / 0x01f))
		| (((((wColor >> 5) & 0x01f) * 0x0ff / 0x01f)) << 8)
		| ((((wColor & 0x01f) * 0x0ff / 0x01f)) << 16)));
	return (dwNewColor);
}

void CColorSelectionDlg::UpdatePalette()
{
	int shadeIndex = this->m_sShade.GetPos();
	CDC * pDC = this->m_csPalette.GetDC();
	CRgn rgn;
	CRect rc;
	m_csPalette.GetClientRect(&rc);
	rgn.CreateRectRgnIndirect(rc);
	pDC->SelectClipRgn(&rgn);
	int iWidth = rc.Width();
	int iHeight = rc.Height();

	iHeight -= iHeight % 8;
	iWidth -= iWidth % 8;

	HBITMAP hOldBitmap = m_csPalette.GetBitmap();
	if ( hOldBitmap )
		DeleteObject(hOldBitmap);

	CByteArray bPixels;
	bPixels.SetSize(iHeight * iWidth * 3);

	// Initialize the bits with the stock window color.
	DWORD dwColor = GetSysColor(COLOR_3DFACE);
	BYTE blue = (BYTE) (( dwColor >> 16 ) & 0xFF);
	BYTE green = (BYTE) (( dwColor >> 8 ) & 0xFF);
	BYTE red = (BYTE) (( dwColor ) & 0xFF);
	for ( int x = 0; x < iWidth; x++ )
	{
		for ( int y = 0; y < iHeight; y++ )
		{
			bPixels.SetAt( ( ( (iWidth * y) + x) * 3), blue);
			bPixels.SetAt( ( ( (iWidth * y) + x) * 3) + 1, green);
			bPixels.SetAt( ( ( (iWidth * y) + x) * 3) + 2, red);
		}
	}

	int cw = iWidth / 50; // Cell width
	int ch = iHeight / 60; // Cell height
	int left = (iWidth / 2) - (25 * cw);
	int right = (iWidth / 2) + (25 * cw);
	int top = (iHeight / 2) - (30 * ch);
	int bottom = (iHeight / 2) + (30 * ch);

	if ( Main->m_aHueGroups.GetSize() > 0 )
	{
		for (int x = left; x < right; x++ )
		{
			for ( int y = top; y < bottom; y++ )
			{
				// Which cell are we in?
				int X = (x - left) / cw;
				int Y = (y - top) / ch;
				int colorIndex = X * 60 + Y;
				if ( colorIndex > 3000 )
					continue;
				int groupIndex = colorIndex / 8;
				int hueIndex = colorIndex % 8;
				if ( Main->m_aHueGroups.GetUpperBound() < groupIndex )
					Main->m_log.Add(1,CMsg("IDS_HUE_OUTOFRANGE"), groupIndex, Main->m_aHueGroups.GetUpperBound() );
				else
				{
					CHueGroup * pGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(groupIndex);
					if ( pGroup == NULL )
						continue;
					CHueEntry Hue = pGroup->Hues[hueIndex];
					DWORD dwColor = ScaleColor(Hue.wColorTable[shadeIndex]);
					BYTE r, g, b;
					b = (BYTE) ((dwColor >> 16) & 0xFF);
					g = (BYTE) ((dwColor >> 8) & 0xFF);
					r = (BYTE) ((dwColor) & 0xFF);
					bPixels.SetAt((( (iWidth * y) + x) * 3), b);
					bPixels.SetAt((( (iWidth * y) + x) * 3) + 1, g);
					bPixels.SetAt((( (iWidth * y) + x) * 3) + 2, r);
				}
			}
		}
	}
	else
	{
		AfxMessageBox(CMsg("IDS_HUE_NOTLOADED"), MB_OK);
		m_sShade.EnableWindow(FALSE);
	}
	
	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( bmi ));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biHeight = -iHeight;	// - Top down
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biBitCount = 24;	// 1,4,8,24
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;	// Not compressed
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter= 0;
	bmi.bmiHeader.biYPelsPerMeter= 0;
	bmi.bmiHeader.biClrUsed 	 = 0;
	bmi.bmiHeader.biClrImportant = 0;

	HBITMAP hBmp = CreateDIBitmap(pDC->GetSafeHdc(), &(bmi.bmiHeader), CBM_INIT, bPixels.GetData(), &bmi, DIB_RGB_COLORS);
	m_csPalette.SetBitmap(hBmp);
	m_csPalette.ReleaseDC(pDC);
}

void CColorSelectionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
}

void CColorSelectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// that darn slider must have scrolled
	if ( pScrollBar->GetSafeHwnd() == this->m_sShade.GetSafeHwnd() )
	{
		UpdatePalette();
		return;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorSelectionDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	this->ClientToScreen(&point);
	CRect rc;
	this->m_csPalette.GetWindowRect(&rc);
	if ( rc.PtInRect(point) )
	{
		this->m_csPalette.ScreenToClient(&point);

		int cw = rc.Width() / 50; // Cell width
		int ch = rc.Height() / 60; // Cell height
		int left = (rc.Width() / 2) - (25 * cw);
		int right = (rc.Width() / 2) + (25 * cw);
		int top = (rc.Height() / 2) - (30 * ch);
		int bottom = (rc.Height() / 2) + (30 * ch);
		if ( point.x < left || point.x >= right || point.y < top || point.y >= bottom )
		{
			m_csColorIndex.SetWindowText("");
			return;
		}
		int X = ( point.x - left) / cw;
		int Y = ( point.y - top) / ch;
		int colorIndex = X * 60 + Y;
		SetColor(colorIndex + 1);
		return;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CColorSelectionDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	this->ClientToScreen(&point);
	CRect rc;
	this->m_csPalette.GetWindowRect(&rc);
	if ( rc.PtInRect(point) )
	{
		this->m_csPalette.ScreenToClient(&point);

		int cw = rc.Width() / 50; // Cell width
		int ch = rc.Height() / 60; // Cell height
		int left = (rc.Width() / 2) - (25 * cw);
		int right = (rc.Width() / 2) + (25 * cw);
		int top = (rc.Height() / 2) - (30 * ch);
		int bottom = (rc.Height() / 2) + (30 * ch);

		if ( point.x < left || point.x >= right || point.y < top || point.y >= bottom )
		{
			m_csMovingIndex.SetWindowText("");
			return;
		}
		int X = ( point.x - left) / cw;
		int Y = ( point.y - top) / ch;
		int colorIndex = X * 60 + Y;
		CString csIndex;
		csIndex.Format("%05x", colorIndex + 1);
		this->m_csMovingIndex.SetWindowText(csIndex);
		return;
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CColorSelectionDlg::UpdateSpectrum()
{
	CString csIndex;
	m_csColorIndex.GetWindowText(csIndex);
	int colorIndex = (int) ahextoi(csIndex);
	colorIndex--;
	if ( colorIndex < 0 || colorIndex > 3000 )
		return;
	int groupIndex = colorIndex / 8;
	int hueIndex = colorIndex % 8;
	if ( Main->m_aHueGroups.GetUpperBound() < groupIndex )
		return;
	CHueGroup * pGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(groupIndex);
	if ( pGroup == NULL )
		return;
	CHueEntry Hue = pGroup->Hues[hueIndex];

	CDC * pDC = this->m_csSpectrum.GetDC();
	CRgn rgn;
	CRect rc;
	m_csSpectrum.GetClientRect(&rc);
	rgn.CreateRectRgnIndirect(rc);
	pDC->SelectClipRgn(&rgn);
	int iWidth = rc.Width();
	int iHeight = rc.Height();

	iHeight -= iHeight % 8;
	iWidth -= iWidth % 8;

	HBITMAP hOldBitmap = m_csSpectrum.GetBitmap();
	if ( hOldBitmap )
		DeleteObject(hOldBitmap);

	CByteArray bPixels;
	bPixels.SetSize(iHeight * iWidth * 3);

	// Initialize the bits with the stock window color.
	DWORD dwColor = GetSysColor(COLOR_3DFACE);
	BYTE blue = (BYTE) (( dwColor >> 16 ) & 0xFF);
	BYTE green = (BYTE) (( dwColor >> 8 ) & 0xFF);
	BYTE red = (BYTE) (( dwColor ) & 0xFF);
	for ( int x = 0; x < iWidth; x++ )
	{
		for ( int y = 0; y < iHeight; y++ )
		{
			bPixels.SetAt( ( ( (iWidth * y) + x) * 3), blue);
			bPixels.SetAt( ( ( (iWidth * y) + x) * 3) + 1, green);
			bPixels.SetAt( ( ( (iWidth * y) + x) * 3) + 2, red);
		}
	}

	int cw = iWidth / 32; // Cell width
	int left = (iWidth / 2) - (16 * cw);
	int right = (iWidth / 2) + (16 * cw);

	for (int x = left; x < right; x++ )
	{
		for ( int y = 0; y < iHeight; y++ )
		{
			// Which cell are we in?
			int shadeIndex = x / cw;
			if (shadeIndex > 0x1f )
				continue;
			DWORD dwColor = ScaleColor(Hue.wColorTable[shadeIndex]);
			BYTE r, g, b;
			b = (BYTE) ((dwColor >> 16) & 0xFF);
			g = (BYTE) ((dwColor >> 8) & 0xFF);
			r = (BYTE) ((dwColor) & 0xFF);
			bPixels.SetAt((( (iWidth * y) + x) * 3), b);
			bPixels.SetAt((( (iWidth * y) + x) * 3) + 1, g);
			bPixels.SetAt((( (iWidth * y) + x) * 3) + 2, r);
		}
	}

	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( bmi ));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biHeight = -iHeight;	// - Top down
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biBitCount = 24;	// 1,4,8,24
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;	// Not compressed
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter= 0;
	bmi.bmiHeader.biYPelsPerMeter= 0;
	bmi.bmiHeader.biClrUsed 	 = 0;
	bmi.bmiHeader.biClrImportant = 0;

	HBITMAP hBmp = CreateDIBitmap(pDC->GetSafeHdc(), &(bmi.bmiHeader), CBM_INIT, bPixels.GetData(), &bmi, DIB_RGB_COLORS);
	m_csSpectrum.SetBitmap(hBmp);
	m_csSpectrum.ReleaseDC(pDC);
	UpdatePreview();

}

WORD CColorSelectionDlg::GetColorIndex()
{
	return m_wColorIndex;
}

void CColorSelectionDlg::OnSelchangeTypelist() 
{
	if ( !this->IsWindowVisible() )
		return;
	SaveSettings();
	UpdatePreview();
}

void CColorSelectionDlg::OnChangeArtindex() 
{
	if ( !this->IsWindowVisible() )
		return;
	SaveSettings();
	UpdatePreview();
}

void CColorSelectionDlg::UpdatePreview()
{
	CString csColor, csArt;
	this->m_csColorIndex.GetWindowText(csColor);
	this->m_ceArtIndex.GetWindowText(csArt);

	switch (this->m_ccbTypeList.GetCurSel())
	{
		case 0:
			m_preview.SetArtType(1);
			break;
		case 1:
			m_preview.SetArtType(3);
			break;
		default:
			m_preview.SetArtType(1);
			break;
	}
	m_preview.SetArtIndex((WORD)ahextoi(csArt));
	m_preview.SetArtColor((WORD) ahextoi(csColor));
	m_preview.SetBkColor(RGB(249,249,251));
}

void CColorSelectionDlg::SaveSettings()
{
	CString csArtIndex, csColorIndex;
	this->m_ceArtIndex.GetWindowText(csArtIndex);
	this->m_csColorIndex.GetWindowText(csColorIndex);
	DWORD dwType = this->m_ccbTypeList.GetCurSel();
	Main->PutRegistryString("ColorPreviewArtIndex", csArtIndex);
	Main->PutRegistryString("ColorPreviewColorIndex", csColorIndex);
	Main->PutRegistryDword("ColorPreviewArtType", dwType);
}

void CColorSelectionDlg::LoadSettings()
{
	CString csValue;
	csValue = Main->GetRegistryString("ColorPreviewArtIndex", "025");
	this->m_ceArtIndex.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ColorPreviewColorIndex", "1");
	this->m_csColorIndex.SetWindowText(csValue);
	DWORD dwValue = Main->GetRegistryDword("ColorPreviewArtType", 0);
	this->m_ccbTypeList.SetCurSel(dwValue);
}

void CColorSelectionDlg::FillHueNames()
{
	for (int i = 0; i <= Main->m_aHueGroups.GetUpperBound(); i++ )
	{
		CHueGroup * pHueGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(i);
		if ( pHueGroup )
		{
			for ( int j = 0; j < 8; j++ )
			{
				CString csHueName = pHueGroup->Hues[j].cName;
				CString csTest = csHueName;
				csTest.MakeLower();
				if ( csTest.SpanIncluding("abcdefghijklmnopqrstuvwxyz 1234567890#()->_") == csTest && csTest != "")
				{
					int iIndex = m_ccbHues.AddString(csHueName);
					m_ccbHues.SetItemData(iIndex, (i * 8) + j + 1);
				}
			}
		}
	}
}

void CColorSelectionDlg::OnSelchangeHuelist() 
{
	int iSel = this->m_ccbHues.GetCurSel();
	CString csHueIndex, csHueName;
	csHueIndex.Format("%04x", m_ccbHues.GetItemData(iSel));
	m_ccbHues.GetLBText(iSel, csHueName);
	m_csColorIndex.SetWindowText(csHueIndex);
	m_csHueName.SetWindowText(csHueName);
	UpdateSpectrum();
	SaveSettings();
}

void CColorSelectionDlg::SetColor(int iColorIndex) 
{
		CString csIndex;
		csIndex.Format("%05x", iColorIndex);
		this->m_csColorIndex.SetWindowText(csIndex);

		iColorIndex --;
		int groupIndex = iColorIndex / 8;
		int hueIndex = iColorIndex % 8;
		if ( Main->m_aHueGroups.GetUpperBound() < groupIndex )
			return;
		CHueGroup * pGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(groupIndex);
		if ( pGroup != NULL )
		{
			CHueEntry Hue = pGroup->Hues[hueIndex];
			this->m_csHueName.SetWindowText(Hue.cName);
		}
		UpdateSpectrum();
		SaveSettings();
}

HBRUSH CColorSelectionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	return Main->m_bkbrush;
}