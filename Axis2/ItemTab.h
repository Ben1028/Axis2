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

#if !defined(AFX_ITEMTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_ITEMTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemTab.h : header file
//

#include "UOart.h"
#include "DockingPage.h"
#include "btnst.h"

/////////////////////////////////////////////////////////////////////////////
// CItemTab dialog

class CItemTab : public CDockingPage
{
	DECLARE_DYNCREATE(CItemTab)

// Construction
public:
	CItemTab();
	virtual ~CItemTab();
	void FillCategoryTree();
	static int iIDSort;
	static int iNameSort;
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

// Dialog Data
	//{{AFX_DATA(CItemTab)
	enum { IDD = IDD_ITEMS_TAB };
	CListCtrl	m_clcItems;
	CTreeCtrl	m_ctcCategories;
	CStatic	m_csItemID;
	CStatic m_csItemIDDec;
	CUOArt	m_Display;
	BOOL	m_bLockDown;
	CEdit	m_ceZTile;
	CEdit	m_ceNukearg;
	CEdit	m_ceMinTime;
	CEdit	m_ceMaxTime;
	CEdit	m_ceMaxDist;
	CEdit	m_ceAmount;
	CEdit	m_ceNudge;
	CEdit	m_ceSpawnRate;
	CButtonST	m_cbNudgeUp;
	CButtonST	m_cbNudgeDown;
	CButtonST	m_cbMove1;
	CButtonST	m_cbMove2;
	CButtonST	m_cbMove3;
	CButtonST	m_cbMove4;
	CButtonST	m_cbMove5;
	CButtonST	m_cbMove6;
	CButtonST	m_cbMove7;
	CButtonST	m_cbMove8;
	CButton	m_cbLockDown;
	CImageList * m_pImageList;
	CUOArt m_Icon;
	CButtonST cb_finditem;
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
	void FindPrevious();
	void Find(CString csString = "", int iCrit = 0, bool bPrevious = false);
	CString m_csSearchValue;
	int m_iSearchCrit;
	CCategory * pCategory;
	CSubsection * pSubsection;
	HTREEITEM hCat;
	HTREEITEM hSub;
	POSITION pos;
	// Generated message map functions
	//{{AFX_MSG(CItemTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedCategoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCreate();
	afx_msg void OnLockitem();
	afx_msg void OnRemove();
	afx_msg void OnTile();
	afx_msg void OnFlip();
	afx_msg void OnChangeParams();
	afx_msg void OnPlacespawn();
	afx_msg void OnInitspawn();
	afx_msg void OnNuke();
	afx_msg void OnNudgeup();
	afx_msg void OnNudgedown();
	afx_msg void OnMove1();
	afx_msg void OnMove2();
	afx_msg void OnMove3();
	afx_msg void OnMove4();
	afx_msg void OnMove5();
	afx_msg void OnMove6();
	afx_msg void OnMove7();
	afx_msg void OnMove8();
	afx_msg void OnFinditem();
	afx_msg void OnShowQuicklist();
	afx_msg void OnExitQuicklist();
	afx_msg void OnSaveQuicklist(CString csList);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
