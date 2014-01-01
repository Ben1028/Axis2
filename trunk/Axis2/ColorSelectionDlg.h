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

#include "UOart.h"
#include "afxwin.h"

#if !defined(AFX_COLORSELECTIONDLG_H__17C03816_DA53_11D3_A38A_00805FD91B8C__INCLUDED_)
#define AFX_COLORSELECTIONDLG_H__17C03816_DA53_11D3_A38A_00805FD91B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSelectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorSelectionDlg dialog

class CColorSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorSelectionDlg)
// Construction
public:
	WORD GetColorIndex();
	void SetColor(int iColorIndex);
	CColorSelectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnCancel();
	int colorIndex;

// Dialog Data
	//{{AFX_DATA(CColorSelectionDlg)
	enum { IDD = IDD_COLOR_DLG };
	CComboBox	m_ccbHues;
	CComboBox	m_ccbTypeList;
	CEdit	m_ceArtIndex;
	CStatic	m_csHueName;
	CStatic	m_csMovingIndex;
	CStatic	m_csSpectrum;
	CSliderCtrl	m_sShade;
	CStatic	m_csPalette;
	CStatic	m_csColorIndex;
	CUOArt	m_preview;
	CStatic cs_intname;
	CStatic cs_dark;
	CStatic cs_colindex;
	CStatic cs_bright;
	CStatic cs_type;
	CStatic cs_index;
	CButton cb_cancel;
	CButton cb_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillHueNames();
	void LoadSettings();
	void SaveSettings();
	void UpdatePreview();
	WORD m_wColorIndex;
	void UpdateSpectrum();
	void UpdatePalette();
	DWORD ScaleColor(WORD wColor);

	// Generated message map functions
	//{{AFX_MSG(CColorSelectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnSelchangeTypelist();
	afx_msg void OnChangeArtindex();
	afx_msg void OnSelchangeHuelist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSELECTIONDLG_H__17C03816_DA53_11D3_A38A_00805FD91B8C__INCLUDED_)
