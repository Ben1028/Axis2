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

#if !defined(AFX_TRAVELTAB_H__DFB29CFD_D431_11D2_A613_004F4905E937__INCLUDED_)
#define AFX_TRAVELTAB_H__DFB29CFD_D431_11D2_A613_004F4905E937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TravelTab.h : header file
//

#include "UOmap.h"
#include "DockingPage.h"
#include "btnst.h"

/////////////////////////////////////////////////////////////////////////////
// CTravelTab dialog

class CTravelTab : public CDockingPage
{
	DECLARE_DYNCREATE(CTravelTab)

// Construction
public:
	CTravelTab();
	~CTravelTab();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SelectTreeItem(CString csParent, CString csChild);
	void FillCategoryTree();
	void RemoteRecenter(short x, short y, short z = 256, short m = 256);

// Dialog Data
	//{{AFX_DATA(CTravelTab)
	enum { IDD = IDD_TRAVEL_TAB };
	CUOMap		m_TravelMap;
	CListCtrl	m_clcLocations;
	CTreeCtrl	m_ctcCategoryTree;
	CEdit		m_ceMapPlane;
	CStatic		m_csCoords;
	CScrollBar	m_csbXScroll;
	CScrollBar	m_csbYScroll;
	CButton		m_cbEditLoc;
	CButton		m_cbRemoveLoc;
	BOOL		m_bTrack;
	CButtonST	cb_world;
	CButtonST	cb_zoomin;
	CButtonST	cb_zoomout;
	CButtonST	cb_findarea;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTravelTab)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iCurState;
	int m_iCatSeq;
	long xscroll;
	long yscroll;
	long xtempscroll;
	long ytempscroll;
	long m_crosshairIndex;
	void Recenter(short x, short y, short z = 256, short m = 256);
	void Recenter();
	void UpdateSpawnDisplay();
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
	//{{AFX_MSG(CTravelTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedCategorytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLocations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickLocations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedLocations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickLocations(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnGo();
	afx_msg void OnSend();
	afx_msg void OnZoomout();
	afx_msg void OnZoomin();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeMapplane();
	afx_msg void OnFindarea();
	afx_msg void OnAddLoc();
	afx_msg void OnRemoveLoc();
	afx_msg void OnEditLoc();
	afx_msg void OnWorldMap();
	afx_msg void OnLocate();
	afx_msg void OnTrack();
	afx_msg void OnWhere();
	afx_msg void OnShowQuicklist();
	afx_msg void OnExitQuicklist();
	afx_msg void OnSaveQuicklist(CString csList);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static int iCoordSort;
	static int iLocSort;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAVELTAB_H__DFB29CFD_D431_11D2_A613_004F4905E937__INCLUDED_)
