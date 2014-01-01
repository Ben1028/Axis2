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

// ItemTweakTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ItemTweakTab.h"
#include "ColorSelectionDlg.h"
#include "UOart.h"
#include "DoorWizard.h"
#include "LightWizard.h"
//#include "Random.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemTab property page

IMPLEMENT_DYNCREATE(CItemTweakTab, CDockingPage)

CItemTweakTab::CItemTweakTab() : CDockingPage(CItemTweakTab::IDD,CMsg("IDS_ITEM_TWEAK"))
{
	//{{AFX_DATA_INIT(CItemTweakTab)
	m_bIdentified = FALSE;
	m_bDecay = FALSE;
	m_bNewbie = FALSE;
	m_bAlwaysMoveable = FALSE;
	m_bNeverMoveable = FALSE;
	m_bMagic = FALSE;
	m_bOwnedByTown = FALSE;
	m_bInvisible = FALSE;
	m_bCursed = FALSE;
	m_bDamned = FALSE;
	m_Blessed = FALSE;
	m_Sacred = FALSE;
	m_ForSale = FALSE;
	m_Stolen = FALSE;
	m_CanDecay = FALSE;
	m_Statics = FALSE;
	//}}AFX_DATA_INIT
}

CItemTweakTab::~CItemTweakTab()
{
}

void CItemTweakTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemTweakTab)
	DDX_Control(pDX, IDC_ITPALETTE, m_cbPalette);
	DDX_Control(pDX, IDC_ITSPECTRUM, m_csSpectrum);
	DDX_Control(pDX, IDC_QUICKPICK, m_csQuickPalette);
	DDX_Control(pDX, IDC_ATTRVALUE, m_csAttrValue);
	DDX_Check(pDX, IDC_IDENTIFIED, m_bIdentified);
	DDX_Check(pDX, IDC_DECAY, m_bDecay);
	DDX_Check(pDX, IDC_NEWBIE, m_bNewbie);
	DDX_Check(pDX, IDC_ALWAYSMOVEABLE, m_bAlwaysMoveable);
	DDX_Check(pDX, IDC_NEVERMOVEABLE, m_bNeverMoveable);
	DDX_Check(pDX, IDC_MAGIC, m_bMagic);
	DDX_Check(pDX, IDC_OWNEDBYTOWN, m_bOwnedByTown);
	DDX_Check(pDX, IDC_INVISIBLE, m_bInvisible);
	DDX_Check(pDX, IDC_CURSED, m_bCursed);
	DDX_Check(pDX, IDC_DAMNED, m_bDamned);
	DDX_Check(pDX, IDC_BLESSED, m_Blessed);
	DDX_Check(pDX, IDC_SACRED, m_Sacred);
	DDX_Check(pDX, IDC_FORSALE, m_ForSale);
	DDX_Check(pDX, IDC_STOLEN, m_Stolen);
	DDX_Check(pDX, IDC_CANDECAY, m_CanDecay);
	DDX_Check(pDX, IDC_STATICFREEZE, m_Statics);

	DDX_Control(pDX, IDC_TYPES, m_ccbTypes);
	DDX_Control(pDX, IDC_ITEMMISC, m_ccbMisc);
	DDX_Control(pDX, IDC_ITEMMISCVALUE, m_ceMiscValue);
	DDX_Control(pDX, IDC_ITEMTAG, m_ccbTags);
	DDX_Control(pDX, IDC_ITEMTAGVALUE, m_ceTagsValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemTweakTab, CDockingPage)
	//{{AFX_MSG_MAP(CItemTweakTab)
	ON_BN_CLICKED(IDC_ITEMCOLOR, OnItemcolor)
	ON_BN_CLICKED(IDC_ITPALETTE, OnItpalette)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RESET_ATTR, OnResetAttr)
	ON_BN_CLICKED(IDC_SETATTR, OnSetattr)

	ON_BN_CLICKED(IDC_IDENTIFIED, OnIdentified)
	ON_BN_CLICKED(IDC_DECAY, OnDecay)
	ON_BN_CLICKED(IDC_NEWBIE, OnNewbie)
	ON_BN_CLICKED(IDC_ALWAYSMOVEABLE, OnAlwaysmoveable)
	ON_BN_CLICKED(IDC_NEVERMOVEABLE, OnNevermoveable)
	ON_BN_CLICKED(IDC_MAGIC, OnMagic)
	ON_BN_CLICKED(IDC_OWNEDBYTOWN, OnOwnedbytown)
	ON_BN_CLICKED(IDC_INVISIBLE, OnInvisible)
	ON_BN_CLICKED(IDC_CURSED, OnCursed)
	ON_BN_CLICKED(IDC_DAMNED, OnDamned)
	ON_BN_CLICKED(IDC_BLESSED, OnBlessed)
	ON_BN_CLICKED(IDC_SACRED, OnSacred)
	ON_BN_CLICKED(IDC_FORSALE, OnForSale)
	ON_BN_CLICKED(IDC_STOLEN, OnStolen)
	ON_BN_CLICKED(IDC_CANDECAY, OnCanDecay)
	ON_BN_CLICKED(IDC_STATICFREEZE, OnStatics)

	ON_BN_CLICKED(IDC_SETTYPES, OnSettypes)
	ON_BN_CLICKED(IDC_SETEVENTS, OnSetevents)
	ON_BN_CLICKED(IDC_ITEMSETMISC, OnSetmisc)
	ON_BN_CLICKED(IDC_ITEMSETTAG, OnSettag)
	ON_BN_CLICKED(IDC_OPENDOORWIZ, OnOpenDoorWizard)
	ON_BN_CLICKED(IDC_OPENLIGHTWIZ, OnOpenLightWizard)

	//ON_BN_CLICKED(IDC_RANDOMOLG, OnRandomOLG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemTweakTab message handlers

BOOL CItemTweakTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppItemTweakTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CItemTweakTab;
	Main->m_pcppItemTweakTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	for ( int i = 0; i <= Main->m_pScripts->m_asaITEMTypes.GetUpperBound(); i++ )
		m_ccbTypes.AddString(Main->m_pScripts->m_asaITEMTypes[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaITEMProps.GetUpperBound(); i++ )
		m_ccbMisc.AddString(Main->m_pScripts->m_asaITEMProps[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaITEMTags.GetUpperBound(); i++ )
		m_ccbTags.AddString(Main->m_pScripts->m_asaITEMTags[i]);

	m_ccbTypes.SetCurSel(0);
	m_ccbMisc.SetCurSel(0);
	m_ccbTags.SetCurSel(0);
	AjustComboBox(&m_ccbTypes);
	AjustComboBox(&m_ccbMisc);
	AjustComboBox(&m_ccbTags);

	for (int Index = 0; Index < 64; Index++)
	{
		CString csIndex;
		csIndex.Format("Index %d", Index);
		m_wQuickColorIndex[Index] = (WORD) Main->GetRegistryDword(csIndex, (Index*6)+2, hRegLocation, REGKEY_PALETTE);
	}
	UpdateQuikPick();
	m_wColorIndex = (WORD) ahextoi(Main->GetRegistryString("ColorPreviewColorIndex", "1"));
	DrawSpectrum(m_wColorIndex);
	m_cbPalette.InitButton(IDI_PALETTE,CMsg("IDS_PALETTE"));
	m_cbPalette.SetBGColor(Main->crAxisBkg);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemTweakTab::OnItemcolor() 
{
	if ( m_wColorIndex > 0 && m_wColorIndex < 3000 )
	{
		CString csCommand;
		csCommand.Format("%sset color %05x", Main->m_csCommandPrefix, m_wColorIndex);
		SendToUO(csCommand);
	}
}

void CItemTweakTab::OnItpalette() 
{
	if (Main->m_dlgColorPal)
		delete Main->m_dlgColorPal;
	Main->m_dlgColorPal = new CColorSelectionDlg;
	Main->m_dlgColorPal->Create(IDD_COLOR_DLG);
	Main->m_dlgColorPal->colorIndex = -1;
}

void CItemTweakTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	this->ClientToScreen(&point);
	CRect rc;
	this->m_csQuickPalette.GetWindowRect(&rc);
	if ( rc.PtInRect(point) )
	{
		if ( point.x < rc.left || point.x > rc.right || point.y < rc.top || point.y > rc.bottom )
			return;

		int cw = rc.Width() / 8; // Cell width
		int ch = rc.Height() / 8; // Cell height
		int X = ( point.x - rc.left) / cw;
		int Y = ( point.y - rc.top) / ch;
		int colorIndex = Y * 8 + X;
		CString csColorIndex;
		csColorIndex.Format("%05x", m_wQuickColorIndex[colorIndex]);
		Main->PutRegistryString("ColorPreviewColorIndex", csColorIndex);
		m_wColorIndex = m_wQuickColorIndex[colorIndex];
		DrawSpectrum(m_wQuickColorIndex[colorIndex]);
		return;
	}
	CDockingPage::OnLButtonDown(nFlags, point);
}

void CItemTweakTab::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	CRect rc;
	m_csQuickPalette.GetWindowRect(&rc);
	if ( rc.PtInRect(point) )
	{
		if ( point.x < rc.left || point.x > rc.right || point.y < rc.top || point.y > rc.bottom )
			return;

		int cw = rc.Width() / 8; // Cell width
		int ch = rc.Height() / 8; // Cell height
		int X = ( point.x - rc.left) / cw;
		int Y = ( point.y - rc.top) / ch;
		int colorIndex = Y * 8 + X;

		CString csColorIndex;
		csColorIndex.Format("%05x", m_wQuickColorIndex[colorIndex]);
		Main->PutRegistryString("ColorPreviewColorIndex", csColorIndex);

		if (Main->m_dlgColorPal)
			delete Main->m_dlgColorPal;
		Main->m_dlgColorPal = new CColorSelectionDlg;
		Main->m_dlgColorPal->Create(IDD_COLOR_DLG);
		Main->m_dlgColorPal->colorIndex = colorIndex;
	}
	else
	{
		ScreenToClient(&point);
		CDockingPage::OnRButtonDown(nFlags, point);
	}
}

BOOL CItemTweakTab::OnSetActive() 
{
	UpdateData();
	if ( m_ccbTypes.GetCount() != Main->m_pScripts->m_asaITEMTypes.GetUpperBound()+1)
	{
		m_ccbTypes.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaITEMTypes.GetUpperBound(); i++ )
			m_ccbTypes.AddString(Main->m_pScripts->m_asaITEMTypes[i]);
		m_ccbTypes.SetCurSel(0);
		AjustComboBox(&m_ccbTypes);
	}
	if ( m_ccbMisc.GetCount() != Main->m_pScripts->m_asaITEMProps.GetUpperBound()+1)
	{
		m_ccbMisc.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaITEMProps.GetUpperBound(); i++ )
			m_ccbMisc.AddString(Main->m_pScripts->m_asaITEMProps[i]);
		m_ccbMisc.SetCurSel(0);
		AjustComboBox(&m_ccbMisc);
	}
	if ( m_ccbTags.GetCount() != Main->m_pScripts->m_asaITEMTags.GetUpperBound()+1)
	{
		m_ccbTags.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaITEMTags.GetUpperBound(); i++ )
			m_ccbTags.AddString(Main->m_pScripts->m_asaITEMTags[i]);
		m_ccbTags.SetCurSel(0);
		AjustComboBox(&m_ccbTags);
	}

	return CDockingPage::OnSetActive();
}

void CItemTweakTab::UpdateQuikPick()
{
	HBITMAP hOldBmp = m_csQuickPalette.GetBitmap();
	if ( hOldBmp )
		DeleteObject(hOldBmp);
	// Create the bitmap for this guy.
	CRect rc;
	m_csQuickPalette.GetClientRect(&rc);

	CByteArray bPixels;
	bPixels.SetSize((rc.Width()+1) * (rc.Height()+1) * 3);

	for (int Index = 0; Index < 64; Index++)
	{
		int x1 = (Index-(8*(Index/8)))*15;
		int x2 = x1+15;
		int y1 = (Index/8)*15;
		int y2 = y1+15;
		
		if (!Main->m_aHueGroups.IsEmpty())
		{
			WORD wColor = m_wQuickColorIndex[Index];
			if ( wColor > 0 && wColor < 3000 )
			{
				int groupIndex = (wColor - 1) / 8;
				int hueIndex = (wColor - 1) % 8;
				CHueGroup * pGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(groupIndex);
				if ( pGroup )
				{
					CHueEntry Hue = pGroup->Hues[hueIndex];
					for ( int x = x1; x < x2; x++ )
					{
						for ( int y = y1; y < y2; y++ )
						{
							int shadeIndex = ((x-x1) * 2)+2;
							if ( shadeIndex > 0x1f ) 
								continue;
							DWORD dwColor;
							if ((x == x1)||(y == y1))
								dwColor = RGB(155,155,155);
							else if ((x == x2-1)||(y == y2-1))
								dwColor = RGB(255,255,255);
							else
								dwColor = ScaleColor(Hue.wColorTable[shadeIndex]);
		
							BYTE r, g, b;
							b = (BYTE) ((dwColor >> 16) & 0xFF);
							g = (BYTE) ((dwColor >> 8) & 0xFF);
							r = (BYTE) ((dwColor) & 0xFF);
							bPixels.SetAt((( ((rc.Width()) * y) + x) * 3), b);
							bPixels.SetAt((( ((rc.Width()) * y) + x) * 3) + 1, g);
							bPixels.SetAt((( ((rc.Width()) * y) + x) * 3) + 2, r);
						}
					}
				}
			}
		}
		else
		{
			for ( int x = x1; x < x2; x++ )
			{
				for ( int y = y1; y < y2; y++ )
				{
					DWORD dwColor;
					if ((x == x1)||(y == y1))
						dwColor = RGB(155,155,155);
					else if ((x == x2-1)||(y == y2-1))
						dwColor = RGB(255,255,255);
					else
						dwColor = RGB(200,200,200);

					BYTE r, g, b;
					b = (BYTE) ((dwColor >> 16) & 0xFF);
					g = (BYTE) ((dwColor >> 8) & 0xFF);
					r = (BYTE) ((dwColor) & 0xFF);
					bPixels.SetAt((( ((rc.Width()) * y) + x) * 3), b);
					bPixels.SetAt((( ((rc.Width()) * y) + x) * 3) + 1, g);
					bPixels.SetAt((( ((rc.Width()) * y) + x) * 3) + 2, r);
				}
			}
		}
	}

	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( bmi ));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biHeight = -rc.Height();	// - Top down
	bmi.bmiHeader.biWidth = rc.Width();
	bmi.bmiHeader.biBitCount = 24;	// 1,4,8,24
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;	// Not compressed
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter= 0;
	bmi.bmiHeader.biYPelsPerMeter= 0;
	bmi.bmiHeader.biClrUsed 	 = 0;
	bmi.bmiHeader.biClrImportant = 0;

	CDC * pDC = m_csQuickPalette.GetDC();
	//int iLines = SetDIBitsToDevice(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), 0, 0, 0, rc.Height(), bPixels.GetData(), &bmi, DIB_RGB_COLORS);
	HBITMAP hBmp = CreateDIBitmap(pDC->GetSafeHdc(), &(bmi.bmiHeader), CBM_INIT, bPixels.GetData(), &bmi, DIB_RGB_COLORS);
	m_csQuickPalette.SetBitmap(hBmp);
	m_csQuickPalette.ReleaseDC(pDC);
	return;
}

DWORD CItemTweakTab::ScaleColor(WORD wColor)
{
	DWORD dwNewColor;
	
	dwNewColor = ((((((wColor >> 10) & 0x01f) * 0x0ff / 0x01f))
		| (((((wColor >> 5) & 0x01f) * 0x0ff / 0x01f)) << 8)
		| ((((wColor & 0x01f) * 0x0ff / 0x01f)) << 16)));
	return (dwNewColor);
}

void CItemTweakTab::DrawSpectrum(WORD wColor)
{
	HBITMAP hOldBmp = m_csSpectrum.GetBitmap();
	if ( hOldBmp )
		DeleteObject(hOldBmp);
	// Create the bitmap for this guy.
	CRect rc;
	m_csSpectrum.GetClientRect(&rc);
	int iHeight = rc.Height();
	int iWidth = rc.Width();
	iHeight -= iHeight % 8;
	iWidth -= iWidth % 8;

	CByteArray bPixels;
	bPixels.SetSize(iWidth * iHeight * 3);
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

	if (!Main->m_aHueGroups.IsEmpty())
	{
		if ( wColor > 0 && wColor < 3000 )
		{
			int groupIndex = (wColor - 1) / 8;
			int hueIndex = (wColor - 1) % 8;
			CHueGroup * pGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(groupIndex);
			if ( pGroup )
			{
				CHueEntry Hue = pGroup->Hues[hueIndex];

				int cw = iWidth / 32; // Cell width
				int left = (iWidth / 2) - (16 * cw);
				int right = (iWidth / 2) + (16 * cw);

				for ( int x = left; x < right; x++ )
				{
					for ( int y = 0; y < iHeight; y++ )
					{
						// Which cell are we in?
						int shadeIndex = x / cw;
						if ( shadeIndex > 0x1f ) 
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
			}
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

	CDC * pDC = m_csSpectrum.GetDC();

	HBITMAP hBmp = CreateDIBitmap(pDC->GetSafeHdc(), &(bmi.bmiHeader), CBM_INIT, bPixels.GetData(), &bmi, DIB_RGB_COLORS);
	m_csSpectrum.SetBitmap(hBmp);
	m_csSpectrum.ReleaseDC(pDC);
}

void CItemTweakTab::OnResetAttr() 
{
	m_bIdentified = FALSE;
	m_bDecay = FALSE;
	m_bNewbie = FALSE;
	m_bAlwaysMoveable = FALSE;
	m_bNeverMoveable = FALSE;
	m_bMagic = FALSE;
	m_bOwnedByTown = FALSE;
	m_bInvisible = FALSE;
	m_bCursed = FALSE;
	m_bDamned = FALSE;
	m_Blessed = FALSE;
	m_Sacred = FALSE;
	m_ForSale = FALSE;
	m_Stolen = FALSE;
	m_CanDecay = FALSE;
	m_Statics = FALSE;
	m_csAttrValue.SetWindowText("00000");
	UpdateData(false);
}

void CItemTweakTab::OnSetattr() 
{
	CString csVal;
	m_csAttrValue.GetWindowText(csVal);
	CString csCmd;
	csCmd.Format("%sset attr %s", Main->m_csCommandPrefix, csVal);
	SendToUO(csCmd);
}

void CItemTweakTab::OnIdentified() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_IDENTIFIED;
	else
		dwVal ^= ATTR_IDENTIFIED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnDecay()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_DECAY;
	else
		dwVal ^= ATTR_DECAY;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnNewbie()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_NEWBIE;
	else
		dwVal ^= ATTR_NEWBIE;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnAlwaysmoveable()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_MOVE_ALWAYS;
	else
		dwVal ^= ATTR_MOVE_ALWAYS;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnNevermoveable()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_MOVE_NEVER;
	else
		dwVal ^= ATTR_MOVE_NEVER;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnMagic()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_MAGIC;
	else
		dwVal ^= ATTR_MAGIC;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnOwnedbytown()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_OWNED;
	else
		dwVal ^= ATTR_OWNED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnInvisible()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_INVIS;
	else
		dwVal ^= ATTR_INVIS;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnCursed()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_CURSED;
	else
		dwVal ^= ATTR_CURSED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnDamned()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_DAMNED;
	else
		dwVal ^= ATTR_DAMNED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnBlessed()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_BLESSED;
	else
		dwVal ^= ATTR_BLESSED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnSacred()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_SACRED;
	else
		dwVal ^= ATTR_SACRED;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnForSale()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_FORSALE;
	else
		dwVal ^= ATTR_FORSALE;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnStolen()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_STOLEN;
	else
		dwVal ^= ATTR_STOLEN;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnCanDecay()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_CAN_DECAY;
	else
		dwVal ^= ATTR_CAN_DECAY;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnStatics()
{
	UpdateData(true);
	CString csCurrentVal;
	m_csAttrValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIdentified)
		dwVal |= ATTR_STATIC;
	else
		dwVal ^= ATTR_STATIC;
	CString csNewVal;
	csNewVal.Format("%05x", dwVal);
	m_csAttrValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CItemTweakTab::OnSettypes() 
{
	CString csType;
	m_ccbTypes.GetWindowText(csType);
	if (csType != "")
	{
		CString csCmd;
		csCmd.Format("%sset Type %s", Main->m_csCommandPrefix, csType);
		SendToUO(csCmd);
	}
}

void CItemTweakTab::OnSetevents() 
{
	CString csEvent;
	m_ccbTypes.GetWindowText(csEvent);
	if (csEvent != "")
	{
		CString csCmd;
		csCmd.Format("%sEvents +%s", Main->m_csCommandPrefix, csEvent);
		SendToUO(csCmd);
	}
}

void CItemTweakTab::OnSetmisc() 
{
	CString csMisc, csValue;
	m_ccbMisc.GetWindowText(csMisc);
	m_ceMiscValue.GetWindowText(csValue);
	csMisc = csMisc.SpanExcluding("(");
	if (csMisc != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csMisc, csValue);
		SendToUO(csCmd);
	}
}

void CItemTweakTab::OnSettag() 
{
	CString csTag, csValue;
	m_ccbTags.GetWindowText(csTag);
	m_ceTagsValue.GetWindowText(csValue);
	csTag = csTag.SpanExcluding("(");
	if (csTag != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csTag, csValue);
		SendToUO(csCmd);
	}
}

void CItemTweakTab::OnOpenDoorWizard()
{
	if (Main->m_dlgDoorWiz)
			delete Main->m_dlgDoorWiz;
	Main->m_dlgDoorWiz = new CDoorWizard;
	Main->m_dlgDoorWiz->Create(IDD_DOORWIZARD);
}

void CItemTweakTab::OnOpenLightWizard()
{
	if (Main->m_dlgLightWiz)
		delete Main->m_dlgLightWiz;
	Main->m_dlgLightWiz = new CLightWizard;
	Main->m_dlgLightWiz->Create(IDD_LIGHTWIZARD);
}

/*int CNewUIntArray::Insert(int inum, int pos)
{
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	if ( iUpper == -1 )
	{
		this->InsertAt(0, inum);
		return 0;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;

	if (pos > -1)
	{
		iIndex = Main->m_pcppItemTweakTab->Order.Insert(pos);
	}
	else
	{
		while ( iLower <= iUpper )
		{
			iIndex = (iUpper + iLower ) / 2;
			int itest = this->GetAt(iIndex);
			if ( itest > inum )
			{
				iCompare = 0;
				iUpper = iIndex - 1;
			}
			else
			{
				iCompare = 1;
				iLower = iIndex + 1;
			}
		}
		iIndex += iCompare;
	}
	this->InsertAt(iIndex, inum);
	return (int)iIndex;
}

void CItemTweakTab::OnRandomOLG()
{  
	int c = 10000;
	Order.RemoveAll();
	CNewUIntArray numb,result;
for (int i=0; i<c; i++)
{
	CRandomMT MTrand(rand());
	numb.Insert(MTrand.RandomRange(1,50));
}

int j=0;
for (int i=0; i<c-1; i++)
{
    if (numb.GetAt(i) == numb.GetAt(i+1))
	{
		j+=1;
	}
    else
    {
		result.Insert(numb.GetAt(i),j);
        j=0;
    }
}

int test = result.GetAt(result.GetUpperBound()-6);
int test2 = result.GetAt(result.GetUpperBound()-1);
int test3 = result.GetAt(result.GetUpperBound()-2);
int test4 = result.GetAt(result.GetUpperBound()-3);
int test5 = result.GetAt(result.GetUpperBound()-4);
int test6 = result.GetAt(result.GetUpperBound()-5);

Main->m_log.Add(1,"%d %d %d %d %d %d", test,test2,test3,test4,test5,test6);
}*/