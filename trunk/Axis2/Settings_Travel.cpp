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

// Settings_Travel.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Settings_Travel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsTravel property page

IMPLEMENT_DYNCREATE(CSettingsTravel, CPropertyPage)

CSettingsTravel::CSettingsTravel() : CPropertyPage(CSettingsTravel::IDD)
{
	//{{AFX_DATA_INIT(CSettingsTravel)
	m_bDrawStatics = FALSE;
	m_bDrawDifs = FALSE;
	m_bShowSpawnpoints = FALSE;
	m_bShowMap = FALSE;
	//}}AFX_DATA_INIT
}


CSettingsTravel::~CSettingsTravel()
{
}


void CSettingsTravel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsTravel)
	DDX_Check(pDX, IDC_DRAWSTATICS, m_bDrawStatics);
	DDX_Check(pDX, IDC_DRAWDIFS, m_bDrawDifs);
	DDX_Check(pDX, IDC_SHOWSPWANPOINT, m_bShowSpawnpoints);
	DDX_Check(pDX, IDC_MAPVIEW, m_bShowMap);
	DDX_Control(pDX, IDC_ITEM_COLOR, m_csItemColor);
	DDX_Control(pDX, IDC_NPC_COLOR, m_csNpcColor);
	DDX_Control(pDX, IDC_STATICNPCCOL, m_stNPCCol);
	DDX_Control(pDX, IDC_STATICITEMCOL, m_stItemCol);
	DDX_Control(pDX, IDC_DRAWSTATICS, m_cbDrawStatics);
	DDX_Control(pDX, IDC_DRAWDIFS, m_cbDrawDifs);
	DDX_Control(pDX, IDC_SHOWSPWANPOINT, m_cbShowSpawnpoints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsTravel, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsTravel)
	ON_BN_CLICKED(IDC_DRAWSTATICS, OnDrawStatics)
	ON_BN_CLICKED(IDC_DRAWDIFS, OnDrawDifs)
	ON_BN_CLICKED(IDC_SHOWSPWANPOINT, OnShowSpawnpoints)
	ON_BN_CLICKED(IDC_MAPVIEW, OnShowMap)
	ON_BN_CLICKED(IDC_ITEM_COLOR, OnItemColor)
	ON_BN_CLICKED(IDC_NPC_COLOR, OnNpcColor)
	ON_BN_CLICKED(IDC_RESET_TRAVEL, OnResetTab)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsTravel message handlers

BOOL CSettingsTravel::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bDrawStatics = (BOOL) Main->m_dwDrawStatics;
	m_bDrawDifs = (BOOL) Main->m_dwDrawDifs;
	m_bShowSpawnpoints = (BOOL) Main->m_dwShowSpawnpoints;
	m_dwNPCSpawnColor = Main->m_dwNPCSpawnColor;
	m_dwItemSpawnColor = Main->m_dwItemSpawnColor;
	m_bShowMap = (BOOL) Main->m_dwShowMap;

	m_csItemColor.EnableWindow(m_bShowSpawnpoints && m_bShowMap? true : false);
	m_csNpcColor.EnableWindow(m_bShowSpawnpoints && m_bShowMap? true : false);
	m_stNPCCol.EnableWindow(m_bShowSpawnpoints && m_bShowMap? true : false);
	m_stItemCol.EnableWindow(m_bShowSpawnpoints && m_bShowMap? true : false);

	m_cbDrawStatics.EnableWindow(m_bShowMap? true : false);
	m_cbDrawDifs.EnableWindow(m_bShowMap? true : false);
	m_cbShowSpawnpoints.EnableWindow(m_bShowMap? true : false);

	UpdateData(false);

	return TRUE;
}

void CSettingsTravel::OnItemColor() 
{
	CColorDialog colors;
	if ( colors.DoModal() == IDOK )
		m_dwItemSpawnColor = colors.GetColor();
	SendMessage( WM_PAINT );
	Main->PutRegistryDword("ItemSpawnColor", m_dwItemSpawnColor);
	Main->m_dwItemSpawnColor = m_dwItemSpawnColor;
}

void CSettingsTravel::OnNpcColor() 
{
	CColorDialog colors;
	if ( colors.DoModal() == IDOK )
		m_dwNPCSpawnColor = colors.GetColor();
	SendMessage( WM_PAINT );
	Main->PutRegistryDword("NPCSpawnColor", m_dwNPCSpawnColor);
	Main->m_dwNPCSpawnColor = m_dwNPCSpawnColor;
}

void CSettingsTravel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC * pDC = m_csNpcColor.GetDC();
	CRgn rgn;
	CRect rect;
	m_csNpcColor.GetWindowRect( &rect );
	CBrush * pNewBrush = new (CBrush);
	pNewBrush->CreateSolidBrush( (COLORREF) m_dwNPCSpawnColor );
	CBrush * pOldBrush = pDC->SelectObject(pNewBrush);
	CRect cr;
	m_csNpcColor.GetClientRect(&cr);
	rgn.CreateRectRgnIndirect(cr);
	pDC->SelectClipRgn(&rgn);
	pDC->FillRect(cr, pNewBrush);
	pDC->SelectObject(pOldBrush);
	delete (pNewBrush);
	rgn.DeleteObject();
	m_csNpcColor.ReleaseDC( pDC );

	pDC = m_csItemColor.GetDC();
	m_csItemColor.GetWindowRect( &rect );
	pNewBrush = new (CBrush);
	pNewBrush->CreateSolidBrush( (COLORREF) m_dwItemSpawnColor );
	pOldBrush = pDC->SelectObject(pNewBrush);
	m_csItemColor.GetClientRect(&cr);
	rgn.CreateRectRgnIndirect(cr);
	pDC->SelectClipRgn(&rgn);
	pDC->FillRect(cr, pNewBrush);
	pDC->SelectObject(pOldBrush);
	delete (pNewBrush);
	rgn.DeleteObject();
	m_csItemColor.ReleaseDC( pDC );

}

void CSettingsTravel::OnDrawStatics()
{
	UpdateData();
	Main->PutRegistryDword("DrawStatics", m_bDrawStatics);
	Main->m_dwDrawStatics = m_bDrawStatics;
}

void CSettingsTravel::OnDrawDifs()
{
	UpdateData();
	Main->PutRegistryDword("DrawDifs", m_bDrawDifs);
	Main->m_dwDrawDifs = m_bDrawDifs;
}


void CSettingsTravel::OnShowSpawnpoints()
{
	UpdateData();
	Main->PutRegistryDword("ShowSpawnpoints", m_bShowSpawnpoints);
	Main->m_dwShowSpawnpoints = m_bShowSpawnpoints;
	m_csItemColor.EnableWindow(m_bShowSpawnpoints ? true : false);
	m_csNpcColor.EnableWindow(m_bShowSpawnpoints ? true : false);
	m_stNPCCol.EnableWindow(m_bShowSpawnpoints ? true : false);
	m_stItemCol.EnableWindow(m_bShowSpawnpoints ? true : false);
}

void CSettingsTravel::OnShowMap()
{
	UpdateData();
	Main->PutRegistryDword("ShowMap", m_bShowMap);
	Main->m_dwShowMap = m_bShowMap;
	m_cbDrawStatics.EnableWindow(m_bShowMap ? true : false);
	m_cbDrawDifs.EnableWindow(m_bShowMap ? true : false);
	m_cbShowSpawnpoints.EnableWindow(m_bShowMap ? true : false);
	m_csItemColor.EnableWindow(m_bShowSpawnpoints && m_bShowMap ? true : false);
	m_csNpcColor.EnableWindow(m_bShowSpawnpoints && m_bShowMap ? true : false);
	m_stNPCCol.EnableWindow(m_bShowSpawnpoints && m_bShowMap ? true : false);
	m_stItemCol.EnableWindow(m_bShowSpawnpoints && m_bShowMap ? true : false);
}

void CSettingsTravel::OnResetTab()
{
	if ( AfxMessageBox( "Are you sure you want to reset the Travel settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	Main->LoadIni(0,"DrawStatics");
	m_bDrawStatics = (BOOL) Main->m_dwDrawStatics;
	Main->PutRegistryDword("DrawStatics", m_bDrawStatics);
	Main->LoadIni(0,"DrawDifs");
	m_bDrawDifs = (BOOL) Main->m_dwDrawDifs;
	Main->PutRegistryDword("DrawDifs", m_bDrawDifs);
	Main->LoadIni(0,"ShowSpawnpoints");
	m_bShowSpawnpoints = (BOOL) Main->m_dwShowSpawnpoints;
	Main->PutRegistryDword("ShowSpawnpoints", m_bShowSpawnpoints);
	Main->LoadIni(0,"ShowMap");
	m_bShowMap = (BOOL) Main->m_dwShowMap;
	Main->PutRegistryDword("ShowMap", m_bShowMap);
	Main->LoadIni(0,"NPCSpawnColor");
	m_dwNPCSpawnColor = Main->m_dwNPCSpawnColor;
	Main->PutRegistryDword("NPCSpawnColor", m_dwNPCSpawnColor);
	Main->LoadIni(0,"ItemSpawnColor");
	m_dwItemSpawnColor = Main->m_dwItemSpawnColor;
	Main->PutRegistryDword("ItemSpawnColor", m_dwItemSpawnColor);

	m_csItemColor.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	m_csNpcColor.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	m_stNPCCol.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	m_stItemCol.EnableWindow(Main->m_dwShowSpawnpoints && Main->m_dwShowMap? true : false);
	m_cbDrawStatics.EnableWindow(Main->m_dwShowMap? true : false);
	m_cbDrawDifs.EnableWindow(Main->m_dwShowMap? true : false);
	m_cbShowSpawnpoints.EnableWindow(Main->m_dwShowMap? true : false);

	SendMessage( WM_PAINT );

	UpdateData(false);
}