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

// AxisLog.h: interface for the CAxisLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXISLOG_H__E8C071C9_A64D_49E7_8F5E_86CE307EC301__INCLUDED_)
#define AFX_AXISLOG_H__E8C071C9_A64D_49E7_8F5E_86CE307EC301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAxisLog  
{
public:
	CAxisLog();
	virtual ~CAxisLog();
	void Add(int iFormat, const char * format, ...);
protected:
	FILE * m_pLog;
};

class CLogArray
{
public:
	CString m_csLine;
	CHARFORMAT m_cf;
};

#endif // !defined(AFX_AXISLOG_H__E8C071C9_A64D_49E7_8F5E_86CE307EC301__INCLUDED_)
