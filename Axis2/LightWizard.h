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

#pragma once

#include "UOart.h"

// CLightWizard dialog

class CLightObj
{
public:
	CString m_csName;
	CStringArray m_Light;
};

class CLightWizard : public CDialog
{
	DECLARE_DYNAMIC(CLightWizard)

public:
	CLightWizard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightWizard();
	bool LoadList();
	virtual void OnCancel();

// Dialog Data
	enum { IDD = IDD_LIGHTWIZARD };
	CListBox m_LightList;
	CScrollBar	m_csbScrollLight;
	CUOArt	m_Light1;
	CUOArt	m_Light2;
	CUOArt	m_Light3;
	CSliderCtrl	m_cscLightLevel;
	CStatic	m_csLightValue;

	CPtrArray m_aLights;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	WORD wFlags;
	afx_msg void OnLightChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAddLight1();
	afx_msg void OnAddLight2();
	afx_msg void OnAddLight3();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};
