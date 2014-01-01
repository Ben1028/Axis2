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

#include "stdafx.h"
#include "Axis2.h"
#include "RemoteConsoleDlg.h"
#include "RemoteConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleDlg dialog


CRemoteConsoleDlg::CRemoteConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteConsoleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteConsoleDlg)
	CWaitCursor hourglass;
	this->m_bIsConnected = false;
	m_bLogging = false;
	m_csLogFile = "";
	m_rConsole = NULL;
	//}}AFX_DATA_INIT
}

CRemoteConsoleDlg::~CRemoteConsoleDlg()
{
	if (m_rConsole != NULL)
	{
		m_rConsole->m_bContinue = false;
		int i = 0;
		while (m_rConsole->IsConnected() && i < 100)
		{
			Sleep(100);
			i++;
		}
		delete m_rConsole;
	}
}

void CRemoteConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteConsoleDlg)
	DDX_Control(pDX, IDC_COMMAND_SEND, m_ccbCommand);
	DDX_Control(pDX, IDC_CONSOLE_SEND, m_ccbBroadcast);
	DDX_Control(pDX, IDC_ENABLE_LOGGING, m_cbLogging);
	DDX_Control(pDX, IDC_SENDCOMMAND, m_cbCommand);
	DDX_Control(pDX, IDC_SENDTEXT, m_cbBroadcast);
	DDX_Control(pDX, IDC_RE_CONSOLE, m_creConsole);
	DDX_Control(pDX, IDC_RECONNECT, m_cbReconnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteConsoleDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteConsoleDlg)
	ON_BN_CLICKED(IDC_SENDTEXT, OnSendtext)
	ON_BN_CLICKED(IDC_RECONNECT, OnReconnect)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SENDCOMMAND, OnSendcommand)
	ON_BN_CLICKED(IDC_ENABLE_LOGGING, OnEnableLogging)
	ON_MESSAGE(WM_RCONSOLEMESSAGE, OnRConsoleMessage)
	ON_MESSAGE(WM_RCONSOLERECONNECT, OnRConsoleReconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleDlg message handlers

BOOL CRemoteConsoleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (Main->m_pRConsole == NULL)
		this->m_rConsole = new CRemoteConsole(this->GetSafeHwnd());
	else
	{
		this->m_rConsole = Main->m_pRConsole;
		this->m_rConsole->m_parentHWnd = this->GetSafeHwnd();
	}
	this->m_bIsConnected = this->m_rConsole->IsConnected();
	if (this->m_bIsConnected)
		this->m_cbReconnect.SetWindowText("Disconnect");
	this->m_ccbBroadcast.SetFocus();
	this->m_nIDTimer = 4000;
	this->m_creConsole.LimitText(0x7FFFFFFF);

	// Get the current positions of all the windows
	GetWindowRect(m_crOriginal);
	m_creConsole.GetWindowRect(m_crConsole);
	m_cbReconnect.GetWindowRect(m_crReConnect);
	m_cbLogging.GetWindowRect(m_crLogging);
	m_ccbBroadcast.GetWindowRect(m_crText);
	m_ccbCommand.GetWindowRect(m_crCommandText);
	m_cbBroadcast.GetWindowRect(m_crBroadcast);
	m_cbCommand.GetWindowRect(m_crCommand);

	ScreenToClient(m_crOriginal);
	ScreenToClient(m_crConsole);
	ScreenToClient(m_crReConnect);
	ScreenToClient(m_crLogging);
	ScreenToClient(m_crText);
	ScreenToClient(m_crCommandText);
	ScreenToClient(m_crBroadcast);
	ScreenToClient(m_crCommand);

	iTop = m_crConsole.top - m_crOriginal.top;
	iLeft = m_crConsole.left - m_crOriginal.left;
	iRight = m_crConsole.right - m_crOriginal.right;
	iBottom = m_crText.bottom - m_crOriginal.bottom;

	CStringArray sa;
	Main->GetRegistryMultiSz("RCBroadcasts", &sa);
	m_ccbBroadcast.ResetContent();
	for ( int i = 0; i < sa.GetSize(); i++ )
		m_ccbBroadcast.AddString(sa.GetAt(i));

	Main->GetRegistryMultiSz("RCCommands", &sa);
	m_ccbCommand.ResetContent();
	for (int i = 0; i < sa.GetSize(); i++ )
		m_ccbCommand.AddString(sa.GetAt(i));

	SetTimer(this->m_nIDTimer, 1000, NULL);	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteConsoleDlg::OnSendtext() 
{
	CString csMessage;
	this->m_ccbBroadcast.GetWindowText(csMessage);
	if ( csMessage.GetLength() > 75 )
	{
		AfxMessageBox("Command too long", MB_OK);
		return;
	}
	m_rConsole->Broadcast(csMessage);
	// Add the string to the listbox
	if ( m_ccbBroadcast.FindString(-1, csMessage) == -1 )
		m_ccbBroadcast.AddString(csMessage);
	// Add the values in here to the registry
	CStringArray sa;
	for ( int i = 0; i < m_ccbBroadcast.GetCount(); i++ )
	{
		CString csValue;
		m_ccbBroadcast.GetLBText(i, csValue);
		sa.Add(csValue);
	}
	Main->PutRegistryMultiSz("RCBroadcasts", &sa);
}

void CRemoteConsoleDlg::OnReconnect() 
{
	if (m_bIsConnected)
	{
		CWaitCursor hourglass;
		m_bIsConnected = false;
		this->m_rConsole->m_bContinue = false;
		this->m_cbReconnect.SetWindowText("ReConnect");
	}
	else
	{
		if (m_rConsole != NULL)
		{
			m_bIsConnected = true;
			this->m_cbReconnect.SetWindowText("Disconnect");
			delete m_rConsole;
			m_rConsole = new CRemoteConsole(this->GetSafeHwnd());
			SetTimer(this->m_nIDTimer, 1000, NULL);
		}
	}
}

void CRemoteConsoleDlg::OnTimer(UINT nIDEvent) 
{
	// Timer expired...Check to see if we are still connected
	if (this->m_rConsole->IsConnected())
	{
		// Reset the timer?
	}
	else
	{
		this->m_cbReconnect.SetWindowText("ReConnect");
		this->m_bIsConnected = false;
		// Kill the timer
		KillTimer(nIDEvent);
	}
	CString csTitle;
	csTitle.Format("Remote Console - %s (%s)", m_rConsole->m_csAccount, m_rConsole->IsConnected() ? "connected" : "not connected");
	this->SetWindowText(csTitle);

	CDialog::OnTimer(nIDEvent);
}

LRESULT CRemoteConsoleDlg::OnRConsoleMessage(WPARAM wParam, LPARAM lParam) 
{
	UNREFERENCED_PARAMETER(lParam);
	char * pText = (char *) wParam;
	if (pText == 0x00)
		return 0;
	char crlf [] = {0x0d, 0x0a, 0x00};
	//char eol [] = {0x0a, 0x00};

	char * pNewText = new char [strlen(pText) + 1];
	memset(&pNewText[0], 0x00, strlen(pText) + 1);

	int iPtr = 0;

	int iScrollCnt = 0;
	for ( int i = 0; i < (int) strlen(pText); i++ )
	{
		if (*(pText + i) == 0x08 && iPtr > 0)
		{
			// backspace
			iPtr--;
			pNewText[iPtr] = 0x00;
		}
		else
		{
			if ( *(pText + i) != 0x08 )
			{
				if ( *(pText + i) == 0x0d )
					iScrollCnt++;
				pNewText[iPtr] = *(pText + i);
				iPtr++;
			}
		}
	}
	pNewText[iPtr] = 0x00;
	// Log it
	try
	{
		if ( m_bLogging && m_csLogFile != "" )
		{
			CStdioFile file;
			CString csText = _T(pNewText);
			csText.Replace(crlf, "\n");
			if ( file.Open(m_csLogFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText | CFile::shareDenyWrite) )
			{
				file.SeekToEnd();
				file.WriteString(csText);
				file.Close();
			}
		}
	}
	catch (CFileException * e)
	{
		e->Delete();
	}
	int lastLine = m_creConsole.GetLineCount();
	int lastchar = m_creConsole.LineIndex(lastLine);
	m_creConsole.SetSel(lastchar, lastchar);
	m_creConsole.ReplaceSel(pNewText);
	m_creConsole.LineScroll(iScrollCnt);

	delete [] pNewText;
	return 0;
}


BOOL CRemoteConsoleDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (nID == 2 )
	{
		CGeneralTab *pParent = ( CGeneralTab * ) m_pParent;
		pParent->HitConsole();
		return true;
	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CRemoteConsoleDlg::Create( UINT IDD, CWnd* pParentWnd) 
{
	this->m_pParent = pParentWnd;	
	return CDialog::Create(IDD, pParentWnd);
}

LRESULT CRemoteConsoleDlg::OnRConsoleReconnect(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	this->m_cbReconnect.SetWindowText("Disconnect");
	this->m_bIsConnected = true;
	SetTimer(this->m_nIDTimer, 1000, NULL);
	this->m_rConsole = ( CRemoteConsole * ) wParam;
	return 0;
}

void CRemoteConsoleDlg::OnSize(UINT nType, int cx, int cy) 
{

	// Make sure we aren't trying to be narrower or shorter than the original
	if ( cx < m_crOriginal.Width() )
		cx = m_crOriginal.Width();
	if ( cy < m_crOriginal.Height() )
		cy = m_crOriginal.Height();

	int dx, dy;
	dx = 0; dy = 0;
	CRect crNew;
	GetWindowRect(crNew);
	ScreenToClient(crNew);
	dx = crNew.Width() - m_crOriginal.Width();
	dy = crNew.Height() - m_crOriginal.Height();
	// Resize the console window
	m_creConsole.MoveWindow(m_crConsole.left , m_crConsole.top, m_crConsole.Width() + dx, m_crConsole.Height() + dy);
	// Move the other windows
	m_cbReconnect.MoveWindow(m_crReConnect.left + dx, m_crReConnect.top + dy, m_crReConnect.Width(), m_crReConnect.Height());
	m_ccbBroadcast.MoveWindow(m_crText.left, m_crText.top + dy, m_crText.Width() + dx, m_crText.Height());
	m_cbBroadcast.MoveWindow(m_crBroadcast.left, m_crBroadcast.top + dy, m_crBroadcast.Width(), m_crBroadcast.Height());
	m_cbCommand.MoveWindow(m_crCommand.left, m_crCommand.top + dy, m_crCommand.Width(), m_crCommand.Height());
	m_ccbCommand.MoveWindow(m_crCommandText.left, m_crCommandText.top + dy, m_crCommandText.Width() + dx, m_crCommandText.Height());
	m_cbLogging.MoveWindow(m_crLogging.left + dx, m_crLogging.top + dy, m_crLogging.Width(), m_crLogging.Height());

	CDialog::OnSize(nType, cx, cy);
	this->Invalidate();
}

void CRemoteConsoleDlg::OnSendcommand() 
{
	CString csMessage;
	this->m_ccbCommand.GetWindowText(csMessage);
	if ( csMessage.GetLength() > 75 )
	{
		AfxMessageBox("Command too long.", MB_OK);
		return;
	}
	m_rConsole->Command(csMessage);
	this->m_ccbCommand.SetWindowText("");
	// Add the string to the listbox
	if ( m_ccbCommand.FindString(-1, csMessage) == -1 )
		m_ccbCommand.AddString(csMessage);
	// Add the values in here to the registry
	CStringArray sa;
	for ( int i = 0; i < m_ccbCommand.GetCount(); i++ )
	{
		CString csValue;
		m_ccbCommand.GetLBText(i, csValue);
		sa.Add(csValue);
	}
	Main->PutRegistryMultiSz("RCCommands", &sa);
}

void CRemoteConsoleDlg::OnEnableLogging() 
{
	m_csLogFile = "";
	m_bLogging = !m_bLogging;
	if ( m_bLogging )
	{
		// Find out which file to log to
		CFileDialog dlg(FALSE, ".log", NULL, OFN_HIDEREADONLY, "Log Files (*.log)|*.log||" , NULL);
		if ( dlg.DoModal() == IDOK )
			m_csLogFile = dlg.GetFileName();
	}
	if ( m_csLogFile == "" )
	{
		m_bLogging = false;
		m_cbLogging.SetCheck(0);
	}
}