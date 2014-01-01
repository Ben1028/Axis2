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

#if !defined(AFX_PROFILEDLG_H__EF7C1985_FBEC_4F48_AC29_BE293E2FDD4A__INCLUDED_)
#define AFX_PROFILETDLG_H__EF7C1985_FBEC_4F48_AC29_BE293E2FDD4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Profile_DLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProfileDLG

class CProfileDLG : public CDialog
{
	DECLARE_DYNAMIC(CProfileDLG)

// Construction
public:
	CProfileDLG(CWnd* pParent = NULL);
	virtual ~CProfileDLG();
	virtual void OnCancel();

// Dialog Data
	//{{AFX_DATA(CSettingsTab)
	enum { IDD = IDD_PROFILE_DLG };
	CListBox	m_clbProfiles;
	CTreeCtrl	m_ctcScripts;
	CButton		m_cbAdd;
	CButton		m_cbEdit;
	CButton		m_cbDelete;
	CButton		m_cbDefProfile;
	CButton		m_cbWebProfile;
	CButton		m_cbLoadResource;
	CButton		m_cbSave;
	CButton		m_cbCancel;
	CButton		m_cbSaveScripts;
	CButton		m_cbBrowse;
	CButton		m_cbLoad;
	CEdit		m_ceName;
	CEdit		m_ceURL;
	CEdit		m_ceBaseDir;
	CEdit		m_ceCurProfile;
	BOOL		m_bDefProfile;
	BOOL		m_bWebProfile;
	BOOL		m_bLoadResource;
	CStatic		m_stName;
	CStatic		m_stDir;
	CStatic		m_stURL;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iCurProfileSel;
	CImageList m_ilIcons;
	int m_iFolderIndex;
	int m_iDocIndex;
	HICON m_hDocIcon;
	HICON m_hFolderIcon;
	void ParseDirectory(LPCTSTR pszPath, HTREEITEM hNode, CStringArray * pSelections);
	bool IsSelected(CStringArray * pSelections, CString csFile);
	void UpdateProfile();
	void GetSelections(HTREEITEM hNode, CString csRootDir, CStringArray * pSelections);
	void CheckChildren(HTREEITEM hNode, int check);
	//{{AFX_MSG(CProfileDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeProfiles();
	afx_msg void OnClickAvailablescripts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTvCheckbox(WPARAM wp, LPARAM lp);
	afx_msg void OnDefprofile();
	afx_msg void OnLoadResource();
	afx_msg void OnWebprofile();
	afx_msg void OnNewprofile();
	afx_msg void OnEditprofile();
	afx_msg void OnDeleteprofile();
	afx_msg void OnSaveprofile();
	afx_msg void OnCancelprofile();
	afx_msg void OnSaveScriptprofile();
	afx_msg void OnDirbrowse();
	afx_msg void OnLoadScripts();
	afx_msg void OnExport();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEDLG_H__EF7C1985_FBEC_4F48_AC29_BE293E2FDD4A__INCLUDED_)
