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
#include "RemoteConsole.h"
#include "RemoteConsoleLoginDlg.h"

CRemoteConsole::CRemoteConsole(HWND hWndParent)
{
	Main->m_pRConsole = this;
	this->m_parentHWnd = hWndParent;
	m_bCommandPending = false;
	m_bIsConnected = false;
	m_bIsInitialized = false;
	m_bIsAlive = false;
	m_bContinue = true;
	m_csCommand = "";
	m_dwThreadID = 0;
	m_csPort = "";
	m_csAddress = "";
	m_csAccount = "";
	m_csPassword = "";
	CString csSavePath;
	// Try to find out where to connect to
	CRemoteConsoleLoginDlg dlg;
	if ( dlg.DoModal() != IDOK )
		return;
	m_csAddress = dlg.m_csAddress;
	m_csPort = dlg.m_csPort;
	// check the IP
	if ((m_csAddress == "")||(m_csPort == ""))
	{
		AfxMessageBox("Couldn't determine the IP address for the remote server.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_csAccount = dlg.m_csAccount;
	m_csPassword = dlg.m_csPassword;

	m_pThread = AfxBeginThread(&MainThread, this);
	while (!m_bIsInitialized)
		Sleep(50);
	return;
}

CRemoteConsole::~CRemoteConsole()
{
	m_bContinue = false;
	int i = 0;
	while ( m_bIsAlive && i < 100)
	{
			Sleep(100);
			i++;
	}
	
	Main->m_pRConsole = NULL;
}

UINT MainThread(LPVOID lpData)
{
	BYTE szBuffer[MAX_BUFFER];
	CRemoteConsole * pConsole = (CRemoteConsole *) lpData;
	if (pConsole == NULL)
	{
		CString csMessage;
		csMessage.Format("Error creating object CRemoteConsole", MB_OK | MB_ICONEXCLAMATION);
		AfxMessageBox(csMessage, MB_OK | MB_ICONSTOP);
		return 1;
	}
	pConsole->m_bIsAlive = true;
	CSocket sock;
	sock.m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (sock.m_hSocket == INVALID_SOCKET)
	{
		DWORD dwErr = WSAGetLastError();
		CString csMessage;
		csMessage.Format("Error creating socket: dwErr = %ld", dwErr);
		AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;

		pConsole->m_bCommandPending = false;
		pConsole->m_bIsAlive = false;
		return 5;
	}
	//sock.Create();
	if (!sock.Connect(pConsole->m_csAddress, alltoi(pConsole->m_csPort)))
	{
		pConsole->m_bIsInitialized = true;
		char szError[MAX_BUFFER];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, &szError[0], MAX_BUFFER, NULL);
		CString csError;
		csError.Format("%s", szError);
		char crlf [3] = { 0x0d, 0x0a, 0x00 };
		csError.Replace(crlf, "");
		CString csMessage;
		csMessage.Format("Could not connect to the remote console --\nLast Error was \"%s\"\nHost = %s, Port =%s\nThis is usually because the Sphere server isn't running.\n", csError , pConsole->m_csAddress, pConsole->m_csPort);
		memset(&szBuffer[0], 0x00, MAX_BUFFER);
		memcpy(&szBuffer[0], LPCTSTR(csMessage), csMessage.GetLength());
		if ( pConsole->m_parentHWnd != NULL )
			SendMessage(pConsole->m_parentHWnd, WM_RCONSOLEMESSAGE, (WPARAM) &szBuffer[0], 0);

		//AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		pConsole->m_bIsAlive = false;
		return 2;
	}
	// Log on.
	memset(&szBuffer[0], 0x00, sizeof(szBuffer));
	szBuffer[0] = (char) 0x20;
	// Put a little sleep in here
	Sleep( 100 );
	if (!sock.Send(szBuffer, 1))
	{
		CString csMessage;
		DWORD dwErr = GetLastError();
		csMessage.Format("Error while sending login info to the remote console: dwErr = %ld\nIs the Sphere server running?", dwErr);
		AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
		pConsole->m_bIsInitialized = true;
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		pConsole->m_bIsAlive = false;
		return 3;
	}
	// Receive the response
	int i = 0;
	while ( true )
	{
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		// Before we try to receive the data...make sure there is some so we don't get hung
		FD_SET fd;
		struct timeval timeout;
		FD_ZERO(&fd);
		FD_SET(sock.m_hSocket, &fd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		int rc = select(0, &fd, 0, 0, &timeout);
		if ( rc < 0 )
		{
			CString csMessage;
			char szBuffer[MAX_BUFFER];
			strerror_s(szBuffer,MAX_BUFFER,WSAGetLastError());
			csMessage.Format("Socket error on select(): %ld - %s", WSAGetLastError(),szBuffer);
			Main->m_log.Add(1,csMessage);
			AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			pConsole->m_bCommandPending = false;
			pConsole->m_bIsAlive = false;
			return 5;
		}
		if ( rc == 0 && i++ > Main->m_iReceiveTimeout )
		{
			CString csMessage;
			csMessage.Format("Timeout exceeded while receiving data from remote server.");
			Main->m_log.Add(1,csMessage);
			AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			pConsole->m_bCommandPending = false;
			pConsole->m_bIsAlive = false;
			return 6;
		}
		if ( rc > 0 )
			break;
	}
	// Send the username?
	if ( pConsole->m_csAccount != "" )
	{
		Sleep( 100 );
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		memcpy(szBuffer, pConsole->m_csAccount, pConsole->m_csAccount.GetLength());
		szBuffer[pConsole->m_csAccount.GetLength()] = 0x0A;
		if ( !sock.Send(&szBuffer[0], pConsole->m_csAccount.GetLength() + 1))
		{
			CString csMessage;
			DWORD dwErr = GetLastError();
			csMessage.Format("Error while sending account to the remote console: dwErr = %ld\nIs the Sphere server still running?", dwErr);
			AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
			pConsole->m_bIsInitialized = true;
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			pConsole->m_bCommandPending = false;
			pConsole->m_bIsAlive = false;
			return 4;
		}
		
	}
	// Send the password
	Sleep( 100 );
	memset(&szBuffer[0], 0x00, sizeof(szBuffer));
	memcpy(szBuffer, pConsole->m_csPassword, pConsole->m_csPassword.GetLength());
	szBuffer[pConsole->m_csPassword.GetLength()] = 0x0A;
	if ( !sock.Send(&szBuffer[0], pConsole->m_csPassword.GetLength() + 1))
	{
		CString csMessage;
		DWORD dwErr = GetLastError();
		csMessage.Format("Error while sending password to the remote console: dwErr = %ld\nIs the Sphere server still running?", dwErr);
		AfxMessageBox(csMessage, MB_OK | MB_ICONEXCLAMATION);
		pConsole->m_bIsInitialized = true;
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		pConsole->m_bIsAlive = false;
		return 4;
	}
	pConsole->m_bIsConnected = true;
	pConsole->m_bIsInitialized = true;
	i = 0;

	// Here's the main loop
	while (pConsole->m_bContinue)
	{
		FD_SET fd;
		struct timeval timeout;
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		// Before we try to receive the data...make sure there is some so we don't get hung
		FD_ZERO(&fd);
		FD_SET(sock.m_hSocket, &fd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		int rc = select(0, &fd, 0, 0, &timeout);
		if (rc > 0)
		{
			// There is some data waiting
			int nBytes = sock.Receive(&szBuffer[0], sizeof(szBuffer));
			if (nBytes == 0)
				break;
			// Pull out NULLS
			char szNewBuffer[MAX_BUFFER];
			memset( &szNewBuffer[0], 0x00, sizeof(szNewBuffer) );
			int j = 0;
			for (int i = 0; i < nBytes; i++)
				if ( szBuffer[i] != 0x00 )
					szNewBuffer[j++] = szBuffer[i];
			if(pConsole->m_parentHWnd != NULL)
				SendMessage(pConsole->m_parentHWnd, WM_RCONSOLEMESSAGE, (WPARAM) &szNewBuffer[0], 0);
		}
		if (rc < 0)
		{
			// Socket error
			char szError[MAX_BUFFER];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, &szError[0], MAX_BUFFER, NULL);
			CString csError;
			csError.Format("%s", szError);
			char crlf[3] = { 0x0d, 0x0a, 0x00};
			csError.Replace(crlf, "");
			CString csMessage;
			csMessage.Format("Connection lost.  Last Error was \"%s\"\n", csError);
			memset(&szBuffer[0], 0x00, MAX_BUFFER);
			memcpy(&szBuffer[0], LPCTSTR(csMessage), csMessage.GetLength());
			if(pConsole->m_parentHWnd != NULL)
				SendMessage(pConsole->m_parentHWnd, WM_RCONSOLEMESSAGE, (WPARAM) &szBuffer[0], 0);
			break;
		}
		if (pConsole->m_bCommandPending)
		{
			for ( int i = 0; i < pConsole->m_csCommand.GetLength(); i++ )
			{
				char xChar = pConsole->m_csCommand.GetAt(i);
				sock.Send( &xChar, 1 );
			}
			pConsole->m_bCommandPending = false;
			pConsole->m_csCommand = "";
		}
	}
	if ( !pConsole->m_bContinue)
	{
		// We were told to stop
		char szMessage[256];
		sprintf_s(szMessage,256,"Disconnecting from remote server...");
		memset(&szBuffer[0], 0x00, MAX_BUFFER);
		memcpy(&szBuffer[0], &szMessage[0], strlen(szMessage));
		if ( pConsole->m_parentHWnd != NULL )
			SendMessage(pConsole->m_parentHWnd, WM_RCONSOLEMESSAGE, (WPARAM) &szBuffer[0], 0);
	}
	// Disconnect the socket
	closesocket(sock.m_hSocket);

	sock.m_hSocket = INVALID_SOCKET;
	pConsole->m_bCommandPending = false;
	pConsole->m_bIsConnected = false;
	pConsole->m_bIsAlive = false;
	return 0;
}

void CRemoteConsole::Broadcast(CString csMessage)
{
	this->m_csCommand.Format("B %s%c", csMessage, 0x0d);
	this->m_bCommandPending = true;
	if (this->IsConnected())
	{
		int i = 0;
		while(this->m_bCommandPending && i < 100)
		{
			Sleep(100);
			i++;
		}
	}
}

void CRemoteConsole::Reconnect()
{
	this->m_bContinue = true;
	m_pThread = AfxBeginThread(&MainThread, this);
	while (!m_bIsInitialized)
		Sleep(50);
	int i = 0;
	while (!m_bIsConnected)
	{
		Sleep(50);
		i++;
		if ( i > 10 )
			break;
	}
	if ( m_bIsConnected  && m_parentHWnd != NULL)
		SendMessage(m_parentHWnd, WM_RCONSOLERECONNECT, (WPARAM) this, 0);

}

void CRemoteConsole::Command(CString csMessage)
{
	this->m_csCommand.Format("%s%c", csMessage, 0x0d);
	this->m_bCommandPending = true;
	if (this->IsConnected())
	{
		int i = 0;
		while(this->m_bCommandPending && i < 100)
		{
			Sleep(100);
			i++;
		}
	}
}
