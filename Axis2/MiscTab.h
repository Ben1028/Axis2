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

#if !defined(AFX_MISCTAB_H__37A80977_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_MISCTAB_H__37A80977_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiscTab.h : header file
//

#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CMiscTab dialog

class CMiscTab : public CDockingPage
{
	DECLARE_DYNCREATE(CMiscTab)

// Construction
public:
	CMiscTab();
	virtual ~CMiscTab();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int iNameSort;

// Dialog Data
	//{{AFX_DATA(CMiscTab)
	enum { IDD = IDD_MISC_TAB };
	CListCtrl	m_clcSpellList;
	CListCtrl	m_clcSoundList;
	CListCtrl	m_clcMusicList;
	CStatic		m_csSpellID;
	CStatic		m_csSoundID;
	CStatic		m_csMusicID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMiscTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMiscTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedSpell(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCastSpell();
	afx_msg void OnItemchangedSound(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPlaySound();
	afx_msg void OnItemchangedMusic(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPlayMusic();
	afx_msg void OnSendSound();
	afx_msg void OnSendMusic();
	afx_msg void OnStopSound();
	afx_msg void OnStopMusic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HWND m_Sound;
	HWND m_Music;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISCTAB_H__37A80977_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
