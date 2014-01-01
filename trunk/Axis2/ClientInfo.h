/******************************************************************************\
* 
*  Copyright (C) 2006 Benoit Croussette
*
*  Code for client info based on code from:
*  UOCH - NecroToolz 
*  By:
*  Copyright (C) 2004 Daniel 'Necr0Potenc3' Cavalcanti
* 
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
* 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
* 
* 
\******************************************************************************/


// ClientInfo.h: interface for the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTINFO_H__E8C071C9_A64D_49E7_8F5E_86CE307EC301__INCLUDED_)
#define AFX_CLIENTINFO_H__E8C071C9_A64D_49E7_8F5E_86CE307EC301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int FleXSearch(BYTE *Src, BYTE *Buf, DWORD SrcSize, DWORD BufSize, DWORD StartAt, int FlexByte, DWORD Which);
int GetFunctionByRef(BYTE *Buf, DWORD BufSize, DWORD PosInFunction);
int GetFunctionRef(BYTE *Buf, DWORD BufSize, DWORD Function, DWORD Which);
int GetTextRef(BYTE *Buf, DWORD BufSize, DWORD ImageBase, const char *Text, BOOL Exact, int Which);

class CClientInfo 
{
public:
	void SetClientBuffer();
	void DeleteClientBuffer();
	CString GetClientVersion();
	int GetClientVersion(CString *fileinfovalue);
	void GetClientRefVerion();
	int GetClientKey1();
	int GetClientKey2();
	int GetClientKeyPos();
	int GetClientCryptPos();
	int GetClientCryptFuncPos();
	int GetClientSize();
	int GetClientTimeStamp();
	int GetClientImage();
	int GetClientEntryP();
protected:
};

#endif // !defined(AFX_CLIENTINFO_H__E8C071C9_A64D_49E7_8F5E_86CE307EC301__INCLUDED_)
