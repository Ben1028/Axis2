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

// Settings_Spawn.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "Settings_Spawn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsSpawn property page

IMPLEMENT_DYNCREATE(CSettingsSpawn, CPropertyPage)

CSettingsSpawn::CSettingsSpawn() : CPropertyPage(CSettingsSpawn::IDD)
{
	//{{AFX_DATA_INIT(CSettingsSpawn)
	m_bShowNPCs = FALSE;
	//}}AFX_DATA_INIT
}


CSettingsSpawn::~CSettingsSpawn()
{
}


void CSettingsSpawn::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsSpawn)
	DDX_Check(pDX, IDC_NPCVIEW, m_bShowNPCs);
	DDX_Control(pDX, IDC_SPAWNBG_COLOR, m_csBGColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsSpawn, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsSpawn)
	ON_BN_CLICKED(IDC_NPCVIEW, OnShowNPCs)
	ON_BN_CLICKED(IDC_RESET_SPAWN, OnResetTab)
	ON_BN_CLICKED(IDC_SPAWNBG_COLOR, OnBGColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsSpawn message handlers

BOOL CSettingsSpawn::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bShowNPCs = (BOOL) Main->m_dwShowNPCs;

	m_dwSpawnBGColor = Main->m_dwSpawnBGColor;

	UpdateData(false);

	return TRUE;
}

void CSettingsSpawn::OnShowNPCs()
{
	UpdateData();
	Main->PutRegistryDword("ShowNPCs", m_bShowNPCs);
	Main->m_dwShowNPCs = m_bShowNPCs;
}

void CSettingsSpawn::OnResetTab()
{
	if ( AfxMessageBox( "Are you sure you want to reset the Spawn settings to default?", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;

	Main->LoadIni(0,"ShowNPCs");
	m_bShowNPCs = (BOOL) Main->m_dwShowNPCs;
	Main->PutRegistryDword("ShowNPCs", m_bShowNPCs);
	Main->LoadIni(0,"SpawnBGColor");
	m_dwSpawnBGColor = Main->m_dwSpawnBGColor;
	Main->PutRegistryDword("SpawnBGColor", m_dwSpawnBGColor);
	SendMessage( WM_PAINT );
	UpdateData(false);
}

void CSettingsSpawn::OnBGColor() 
{
	CColorDialog colors;
	if ( colors.DoModal() == IDOK )
		m_dwSpawnBGColor = colors.GetColor();
	SendMessage( WM_PAINT );
	Main->PutRegistryDword("SpawnBGColor", m_dwSpawnBGColor);
	Main->m_dwSpawnBGColor = m_dwSpawnBGColor;
}

void CSettingsSpawn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC * pDC = m_csBGColor.GetDC();
	CRgn rgn;
	CRect rect;
	m_csBGColor.GetWindowRect( &rect );
	CBrush * pNewBrush = new (CBrush);
	pNewBrush->CreateSolidBrush( (COLORREF) m_dwSpawnBGColor );
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