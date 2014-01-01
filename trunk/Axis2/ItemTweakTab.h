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

#if !defined(AFX_ITEMTWEAKTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_ITEMTWEAKTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemTweakTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemTab dialog

/*class CNewUIntArray : public CUIntArray
{
public:
	int Insert(int inum, int pos = -1);
};*/

class CItemTweakTab : public CDockingPage
{
	DECLARE_DYNCREATE(CItemTweakTab)

// Construction
public:
	CItemTweakTab();
	virtual ~CItemTweakTab();

	//CNewUIntArray Order;

// Dialog Data
	//{{AFX_DATA(CItemTweakTab)
	enum { IDD = IDD_ITEMTWEAK_TAB };
	CButtonST	m_cbPalette;
	CStatic	m_csSpectrum;
	CStatic	m_csQuickPalette;
	CStatic	m_csAttrValue;
	BOOL	m_bIdentified;
	BOOL	m_bDecay;
	BOOL	m_bNewbie;
	BOOL	m_bAlwaysMoveable;
	BOOL	m_bNeverMoveable;
	BOOL	m_bMagic;
	BOOL	m_bOwnedByTown;
	BOOL	m_bInvisible;
	BOOL	m_bCursed;
	BOOL	m_bDamned;
	BOOL	m_Blessed;
	BOOL	m_Sacred;
	BOOL	m_ForSale;
	BOOL	m_Stolen;
	BOOL	m_CanDecay;
	BOOL	m_Statics;

	CComboBox	m_ccbTypes;
	CComboBox	m_ccbMisc;
	CEdit	m_ceMiscValue;
	CComboBox	m_ccbTags;
	CEdit	m_ceTagsValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CItemTweakTab)
	public:
	virtual BOOL OnSetActive();
	WORD m_wColorIndex;
	void DrawSpectrum(WORD wColor);
	WORD m_wQuickColorIndex[64];
	void UpdateQuikPick();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD ScaleColor(WORD wColor);
	// Generated message map functions
	//{{AFX_MSG(CItemTweakTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemcolor();
	afx_msg void OnItpalette();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnResetAttr();
	afx_msg void OnSetattr();
	afx_msg void OnIdentified();
	afx_msg void OnDecay();
	afx_msg void OnNewbie();
	afx_msg void OnAlwaysmoveable();
	afx_msg void OnNevermoveable();
	afx_msg void OnMagic();
	afx_msg void OnOwnedbytown();
	afx_msg void OnInvisible();
	afx_msg void OnCursed();
	afx_msg void OnDamned();
	afx_msg void OnBlessed();
	afx_msg void OnSacred();
	afx_msg void OnForSale();
	afx_msg void OnStolen();
	afx_msg void OnCanDecay();
	afx_msg void OnStatics();

	afx_msg void OnSettypes();
	afx_msg void OnSetevents();
	afx_msg void OnSetmisc();
	afx_msg void OnSettag();
	afx_msg void OnOpenDoorWizard();
	afx_msg void OnOpenLightWizard();
	//afx_msg void OnRandomOLG();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ItemTweakTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
