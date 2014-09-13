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

// stdafx.cpp : source file that includes just the standard includes
//	Axis2.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "Axis2.h"
#include "AxisLog.h"
#include "ClientInfo.h"
#include "RemoteConsole.h"
#include "ScriptObjects.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <DLGS.H>
#include <WINUSER.H>

HWND hwndUOClient;
HWND hwndHoGInstance;

CString csUOPath;
CString csMulPath;
CString csProfilePath;
CStringArray saClientPaths;
CStringArray saMulPaths;
HKEY hRegLocation;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumInstanceProc(HWND hWnd, LPARAM lParam);

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	// If this is the UO Client, store it and stop
	CWnd * pWnd = CWnd::FromHandle(hWnd);
	CString csTitle;
	pWnd->GetWindowText(csTitle);
	if ( lParam )
	{
		if ((csTitle.Find("Ultima Online") != -1) || (csTitle.Find("UOSA") != -1) || (csTitle.Find(Main->m_csUOTitle) != -1))
		{
			hwndUOClient = hWnd;
			return FALSE;
		}
		else
		{
			hwndUOClient = NULL;
			return TRUE;
		}
	}
	hwndUOClient = NULL;
	return TRUE;
}

BOOL CALLBACK EnumInstanceProc(HWND hWnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	CWnd * pWnd = CWnd::FromHandle(hWnd);
	CString csTitle;
	pWnd->GetWindowText(csTitle);
	if (csTitle.Find(Main->GetVersionTitle()) != -1)
	{
		hwndHoGInstance = hWnd;
		return FALSE;
	}
	else
		return TRUE;
}

void AjustComboBox(CComboBox* pmyComboBox)
{
	CString str;
	CSize sz;
	int dx = 0;
	TEXTMETRIC tm;
	CDC * pDC = pmyComboBox->GetDC();
	CFont * pFont = pmyComboBox->GetFont();

	CFont* pOldFont = pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < pmyComboBox->GetCount(); i++)
	{
	   pmyComboBox->GetLBText(i, str);
	   sz = pDC->GetTextExtent(str);

	   sz.cx += tm.tmAveCharWidth;
   
	  if (sz.cx > dx)
	     dx = sz.cx;
	}

	pDC->SelectObject(pOldFont);
	pmyComboBox->ReleaseDC(pDC);

	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	pmyComboBox->SetDroppedWidth(dx);
}


/////////////////////////////////////////////////////////////////////////////
// Send UO commands

bool SendToUO(CString Cmd)
{
	EnumWindows(EnumWindowsProc, 1);
	if (hwndUOClient)
	{
		// Found the client window
		CWnd * pWnd = CWnd::FromHandle(hwndUOClient);
		if (!IsWindow( hwndUOClient ) )
		{
			EnumWindows(EnumWindowsProc, 1);
			pWnd = CWnd::FromHandle(hwndUOClient);
		}
		for (int i = 0; i < Cmd.GetLength(); i++)
		{
			pWnd->SendMessage(WM_CHAR, Cmd[i], 0);
		}
		pWnd->SendMessage(WM_CHAR, VK_RETURN, 0);
		pWnd->SetForegroundWindow();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//Misc

bool IsLocalProfile(CString csProfile)
{
	if ((csProfile == "<Axis Profile>")||(csProfile == "<None>"))
		return false;
	CString csKey;
	csKey.Format("%s\\%s",REGKEY_PROFILE, csProfile);
	CString csType = Main->GetRegistryString("Type", "Local", hRegLocation, csKey);
	if(csType == "Local")
		return true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// CFolderDialog

IMPLEMENT_DYNAMIC(CFolderDialog, CFileDialog)

WNDPROC CFolderDialog::m_wndProc = NULL;


// Function name	: CFolderDialog::CFolderDialog
// Description	    : Constructor
// Return type		: 
// Argument         : CString* pPath ; represent string where selected folder wil be saved
CFolderDialog::CFolderDialog(CString* pPath, CString csTitle) : CFileDialog(TRUE, NULL, _T("*..*"))
{
	m_pPath = pPath;
	m_Title = csTitle;
}


BEGIN_MESSAGE_MAP(CFolderDialog, CFileDialog)
	//{{AFX_MSG_MAP(CFolderDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Function name	: WindowProcNew
// Description	    : Call this function when user navigate into CFileDialog.
// Return type		: LRESULT
// Argument         : HWND hwnd
// Argument         : UINT message
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CALLBACK WindowProcNew(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message ==  WM_COMMAND)
		if (HIWORD(wParam) == BN_CLICKED)
			if (LOWORD(wParam) == IDOK)
			{
				if (CFileDialog* pDlg = (CFileDialog*)CWnd::FromHandle(hwnd))
				{
					TCHAR path[MAX_PATH];
					GetCurrentDirectory(MAX_PATH, path);
					*((CFolderDialog*)pDlg->GetDlgItem(0))->m_pPath = CString(path);
					pDlg->EndDialog(IDOK);
					return NULL;
				}
			}
	return CallWindowProc(CFolderDialog::m_wndProc, hwnd, message, wParam, lParam);
}

// Function name	: CFolderDialog::OnInitDone
// Description	    : For update the wiew of CFileDialog
// Return type		: void 
void CFolderDialog::OnInitDone()
{
	HideControl(edt1);
	HideControl(stc3);
	HideControl(cmb1);
	HideControl(cmb13);
	HideControl(stc2);
	CWnd* pFD = GetParent();
	CRect rectCancel; pFD->GetDlgItem(IDCANCEL)->GetWindowRect(rectCancel);
	pFD->ScreenToClient(rectCancel);
	CRect rectOK; pFD->GetDlgItem(IDOK)->GetWindowRect(rectOK);
	pFD->ScreenToClient(rectOK);
	pFD->GetDlgItem(IDOK)->SetWindowPos(0,rectCancel.left - rectOK.Width() - 4, rectCancel.top, 0,0, SWP_NOZORDER | SWP_NOSIZE);
	CRect rectList2; pFD->GetDlgItem(lst1)->GetWindowRect(rectList2);
	pFD->ScreenToClient(rectList2);
	pFD->GetDlgItem(lst1)->SetWindowPos(0,0,0,rectList2.Width(), abs(rectList2.top - (rectCancel.top - 4)), SWP_NOMOVE | SWP_NOZORDER);
	SetControlText(IDOK, _T("Select"));
	pFD->SetWindowText(m_Title);
	m_wndProc = (WNDPROC)SetWindowLongPtr(pFD->m_hWnd, GWL_WNDPROC, (__int3264)(LONG_PTR)WindowProcNew);
}