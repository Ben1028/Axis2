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

#if !defined(AFX_SPAWNTAB_H__37A80987_1A73_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_SPAWNTAB_H__37A80987_1A73_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpawnTab.h : header file
//

#include "UOart.h"
#include "DockingPage.h"
#include "btnst.h"

/////////////////////////////////////////////////////////////////////////////
// CSpawnTab dialog

class CSpawnTab : public CDockingPage
{
	DECLARE_DYNCREATE(CSpawnTab)

// Construction
public:
	CSpawnTab();
	virtual ~CSpawnTab();
	void FillCategoryTree();
	static int iIDSort;
	static int iNameSort;
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);;

// Dialog Data
	//{{AFX_DATA(CItemTab)
	enum { IDD = IDD_SPAWN_TAB };
	CListCtrl	m_clcCreatures;
	CTreeCtrl	m_ctcNPC;
	CEdit	m_ceHomedist;
	CStatic	m_csNPCId;
	CStatic	m_csNPCIdDec;
	CEdit	m_ceMinTime;
	CEdit	m_ceMaxTime;
	CEdit	m_ceMaxDist;
	CEdit	m_ceAmount;
	CUOArt	m_Display;
	CImageList * m_pImageList;
	CUOArt m_Icon;
	CSliderCtrl	m_FrameSelect;
	CButtonST cb_findnpc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CItemTab)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iCatSeq;
	int icX;
	int icY;
	int m_iCurState;
	CString m_csnpcSearchValue;
	int m_inpcSearchCrit;
	CCategory * pnpcCategory;
	CSubsection * pnpcSubsection;
	HTREEITEM hnpcCat;
	HTREEITEM hnpcSub;
	POSITION npcpos;
	void Find(CString csString = "", int iCrit = 0, bool bPrevious = false);
	void FindPrevious();
	// Generated message map functions
	//{{AFX_MSG(CItemTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedNpctree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCreatures(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCreatures(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickCreatures(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickCreatures(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeParams();
	afx_msg void OnSummon();
	afx_msg void OnPlacespawn();
	afx_msg void OnInitspawn();
	afx_msg void OnSethome();
	afx_msg void OnSethomedist();
	afx_msg void OnSpremove();
	afx_msg void OnSpfreeze();
	afx_msg void OnShrink();
	afx_msg void OnFindnpc();
	afx_msg void OnShowQuicklist();
	afx_msg void OnExitQuicklist();
	afx_msg void OnSaveQuicklist(CString csList);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
