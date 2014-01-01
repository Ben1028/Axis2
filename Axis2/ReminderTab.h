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

#if !defined(AFX_REMINDERTAB_H__E4FC6116_99A5_11D6_95AB_00805FD91B8C__INCLUDED_)
#define AFX_REMINDERTAB_H__E4FC6116_99A5_11D6_95AB_00805FD91B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReminderTab.h : header file
//

#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CReminderTab dialog

class CReminderTab : public CDockingPage
{
	DECLARE_DYNCREATE(CReminderTab)
	
// Construction
public:
	CReminderTab();   // standard constructor
	virtual ~CReminderTab();

// Dialog Data
	//{{AFX_DATA(CReminderTab)
	enum { IDD = IDD_REMINDER_TAB };
	CListCtrl m_clcList;
	CEdit m_ceTitle;
	CRichEditCtrl m_ceText;
	CComboBox m_ccbWeekday;
	CDateTimeCtrl m_cdtpDatePicker;
	CDateTimeCtrl m_cdtpTimePicker;
	CButton m_crMonthly;
	CButton m_crWeekly;
	CButton m_crDaily;
	CButton m_crOnce;
	CButton m_crNever;
	CButton m_cbStartup;
	CButton m_cbAdd;
	CButton m_cbEdit;
	CButton m_cbDelete;
	CButton m_cbSave;
	CButton m_cbCancel;
	BOOL m_bStartup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReminderTab)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int iReminder;
	bool bEditor;
	CTime tNow;
	void LoadReminders();
	// Generated message map functions
	//{{AFX_MSG(CReminderTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnListChange(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnMonthly();
	afx_msg void OnWeekly();
	afx_msg void OnDaily();
	afx_msg void OnOnce();
	afx_msg void OnNever();
	afx_msg void OnStartup();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	afx_msg void OnSave();
	afx_msg void OnCancelReminder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMINDERTAB_H__E4FC6116_99A5_11D6_95AB_00805FD91B8C__INCLUDED_)
