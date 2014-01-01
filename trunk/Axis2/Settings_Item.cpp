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

// Settings_Item.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Settings_Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsItem property page

IMPLEMENT_DYNCREATE(CSettingsItem, CPropertyPage)

CSettingsItem::CSettingsItem() : CPropertyPage(CSettingsItem::IDD)
{
	//{{AFX_DATA_INIT(CSettingsItem)
	m_bRoomView = FALSE;
	m_bShowItems = FALSE;
	//}}AFX_DATA_INIT
}


CSettingsItem::~CSettingsItem()
{
}


void CSettingsItem::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsItem)
	DDX_Check(pDX, IDC_ROOMVIEW, m_bRoomView);
	DDX_Check(pDX, IDC_ITEMVIEW, m_bShowItems);
	DDX_Control(pDX, IDC_ROOMVIEW, m_cbRoomView);
	DDX_Control(pDX, IDC_ITEMBG_COLOR, m_csBGColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsItem, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsItem)
	ON_BN_CLICKED(IDC_ROOMVIEW, OnRoomView)
	ON_BN_CLICKED(IDC_ITEMVIEW, OnShowItems)
	ON_BN_CLICKED(IDC_ITEMBG_COLOR, OnBGColor)
	ON_BN_CLICKED(IDC_RESET_ITEMS, OnResetTab)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsItem message handlers

BOOL CSettingsItem::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bRoomView = (BOOL) Main->m_dwRoomView;
	m_bShowItems = (BOOL) Main->m_dwShowItems;
	m_dwItemBGColor = Main->m_dwItemBGColor;

	m_cbRoomView.EnableWindow(m_bShowItems? true : false);

	UpdateData(false);

	return TRUE;
}

void CSettingsItem::OnResetTab()
{
	if ( AfxMessageBox( "Are you sure you want to reset the Items settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	Main->LoadIni(0,"RoomView");
	m_bRoomView = (BOOL) Main->m_dwRoomView;
	Main->PutRegistryDword("RoomView", m_bRoomView);
	Main->LoadIni(0,"ShowItems");
	m_bShowItems = (BOOL) Main->m_dwShowItems;
	Main->PutRegistryDword("ShowItems", m_bShowItems);
	m_cbRoomView.EnableWindow(Main->m_dwShowItems? true : false);
	Main->LoadIni(0,"ItemBGColor");
	m_dwItemBGColor = Main->m_dwItemBGColor;
	Main->PutRegistryDword("ItemBGColor", m_dwItemBGColor);
	SendMessage( WM_PAINT );

	UpdateData(false);
}

void CSettingsItem::OnRoomView()
{
	UpdateData();
	Main->PutRegistryDword("RoomView", m_bRoomView);
	Main->m_dwRoomView = m_bRoomView;
}

void CSettingsItem::OnShowItems()
{
	UpdateData();
	Main->PutRegistryDword("ShowItems", m_bShowItems);
	Main->m_dwShowItems = m_bShowItems;
	m_cbRoomView.EnableWindow(m_bShowItems ? true : false);
}

void CSettingsItem::OnBGColor() 
{
	CColorDialog colors;
	if ( colors.DoModal() == IDOK )
		m_dwItemBGColor = colors.GetColor();
	SendMessage( WM_PAINT );
	Main->PutRegistryDword("ItemBGColor", m_dwItemBGColor);
	Main->m_dwItemBGColor = m_dwItemBGColor;
}

void CSettingsItem::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC * pDC = m_csBGColor.GetDC();
	CRgn rgn;
	CRect rect;
	m_csBGColor.GetWindowRect( &rect );
	CBrush * pNewBrush = new (CBrush);
	pNewBrush->CreateSolidBrush( (COLORREF) m_dwItemBGColor );
	CBrush * pOldBrush = pDC->SelectObject(pNewBrush);
	CRect cr;
	m_csBGColor.GetClientRect(&cr);
	rgn.CreateRectRgnIndirect(cr);
	pDC->SelectClipRgn(&rgn);
	pDC->FillRect(cr, pNewBrush);
	pDC->SelectObject(pOldBrush);
	delete (pNewBrush);
	rgn.DeleteObject();
	m_csBGColor.ReleaseDC( pDC );

}