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

#ifndef __REMOTECONSOLE_H__
#define __REMOTECONSOLE_H__

#include "stdafx.h"

class CRemoteConsole
{
public:
	void Command(CString csMessage);
	void Reconnect();
	void Broadcast(CString csMessage);
	CRemoteConsole(HWND hWndParent = NULL);
	virtual ~CRemoteConsole();
	bool IsConnected() {return m_bIsConnected;};
	CString m_csCommand;
	bool m_bCommandPending;
	bool m_bIsConnected;
	bool m_bIsInitialized;
	HWND  m_parentHWnd;
	bool m_bContinue;
	DWORD m_dwThreadID;
	HANDLE m_hThread;
	CString m_csAddress;
	CString m_csPort;
	CString m_csAccount;
	CString m_csPassword;
	bool m_bIsAlive;
protected:
	bool m_bLogging;
	CWinThread * m_pThread;
};

UINT MainThread(LPVOID lpParam);

#endif