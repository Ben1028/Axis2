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

#if !defined(AFX_REMOTECONSOLEDLG_H__5D6AFB61_2981_11D3_8723_00805FD91B8C__INCLUDED_)
#define AFX_REMOTECONSOLEDLG_H__5D6AFB61_2981_11D3_8723_00805FD91B8C__INCLUDED_

#include "RemoteConsole.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteConsoleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleDlg dialog

class CRemoteConsoleDlg : public CDialog
{
// Construction
public:
	CString m_csCommand;
	bool m_bIsConnected;
	int m_nIDTimer;
	CStringArray m_saMessages;
	virtual ~CRemoteConsoleDlg();
	CRemoteConsole * m_rConsole;
	CRemoteConsoleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteConsoleDlg)
	enum { IDD = IDD_REMOTECONSOLEDLG };
	CRichEditCtrl	m_creConsole;
	CComboBox	m_ccbCommand;
	CComboBox	m_ccbBroadcast;
	CButton	m_cbLogging;
	CButton	m_cbCommand;
	CButton	m_cbBroadcast;
	CButton	m_cbReconnect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteConsoleDlg)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL Create( UINT IDD, CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	LRESULT OnRConsoleMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnRConsoleReconnect(WPARAM wParam, LPARAM lParam);
// Implementation
protected:
	CString m_csLogFile;
	bool m_bLogging;
	CWnd * m_pParent;
	CRect m_crText;
	CRect m_crConsole;
	CRect m_crBroadcast;
	CRect m_crReConnect;
	CRect m_crCommand;
	CRect m_crCommandText;
	CRect m_crLogging;
	CRect m_crOriginal;

	int iTop, iLeft, iBottom, iRight;

	// Generated message map functions
	//{{AFX_MSG(CRemoteConsoleDlg)
	afx_msg void OnSendtext();
	afx_msg void OnReconnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSendcommand();
	afx_msg void OnEnableLogging();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTECONSOLEDLG_H__5D6AFB61_2981_11D3_8723_00805FD91B8C__INCLUDED_)
