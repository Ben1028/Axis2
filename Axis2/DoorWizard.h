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

// CDoorWizard dialog

class CDoorObj
{
public:
	CString m_csName;
	CString m_csNNW;
	CString m_csNNE;
	CString m_csNSW;
	CString m_csNSE;
	CString m_csWNW;
	CString m_csWNE;
	CString m_csWSW;
	CString m_csWSE;
};

class CDoorWizard : public CDialog
{
	DECLARE_DYNAMIC(CDoorWizard)

public:
	CDoorWizard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDoorWizard();
	bool LoadList();
	virtual void OnCancel();

// Dialog Data
	enum { IDD = IDD_DOORWIZARD };
	CUOArt	m_DoorNW;
	CUOArt	m_DoorNE;
	CUOArt	m_DoorSW;
	CUOArt	m_DoorSE;
	CButton m_North;
	CButton m_West;
	CListBox m_DoorList;


	CPtrArray m_aDoors;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	WORD wFlags;
	afx_msg void OnNorth();
	afx_msg void OnWest();
	afx_msg void OnDoorChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAddDoorNW();
	afx_msg void OnAddDoorNE();
	afx_msg void OnAddDoorSW();
	afx_msg void OnAddDoorSE();

	DECLARE_MESSAGE_MAP()
};
