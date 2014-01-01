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

// CMultiView dialog

class CMultiView : public CDialog
{
	DECLARE_DYNAMIC(CMultiView)

public:
	CMultiView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMultiView();
	void UpdateSize();
	virtual void OnCancel();
	long l_ArtIndex;

// Dialog Data
	enum { IDD = IDD_MULTIVIEW };
	CUOArt		m_MultiView;
	long xscroll;
	long yscroll;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
