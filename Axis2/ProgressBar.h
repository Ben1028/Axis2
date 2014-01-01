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

#if !defined(AFX_PROGRESSBAR_H__59DF2C0D_D0BD_45E5_9116_B34861746A45__INCLUDED_)
#define AFX_PROGRESSBAR_H__59DF2C0D_D0BD_45E5_9116_B34861746A45__INCLUDED_

#include "Axis2.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressBar dialog

class CProgressBar : public CDialog
{
// Construction
public:
	CProgressBar(CWnd* pParent = NULL);   // standard constructor
	void SetRange(unsigned short nLower, unsigned short nUpper) { SetRange32((int) nLower, (int) nUpper); }
	void SetRange32(unsigned long nLower, unsigned long nUpper) { m_iLower = nLower; m_iUpper = nUpper; }
	void GetRange(unsigned long& nLower, unsigned long& nUpper) {nLower = m_iLower; nUpper = m_iUpper; }
	int GetPos() { return m_iPos; }
	void SetPos(int iPos) { m_iPos = iPos; Update(); }
	void OffsetPos(int iOffset) { m_iPos += iOffset; Update(); }
	void TopMost() {this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);}

// Dialog Data
	//{{AFX_DATA(CProgressBar)
	enum { IDD = IDD_PROGRESS };
	CStatic	m_csProgBar;
	CStatic	m_csMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	unsigned long m_iLower;
	unsigned long m_iUpper;
	unsigned long m_iPos;
	void Update();
	COLORREF m_cBkg;
	COLORREF m_cFgd;
	CString m_csFontName;
	// Generated message map functions
	//{{AFX_MSG(CProgressBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSBAR_H__59DF2C0D_D0BD_45E5_9116_B34861746A45__INCLUDED_)
