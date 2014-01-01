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

// AxisLog.cpp: implementation of the CAxisLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Axis2.h"
#include "AxisLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxisLog::CAxisLog()
{
#define OLD_LOG "axis.log.old"
#define NEW_LOG "axis.log"
	try
	{
		CFile::Remove(OLD_LOG);
	}
	catch (CFileException * e)
	{
		e->Delete();
	}
	try
	{
		CFile::Rename(NEW_LOG, OLD_LOG);
	}
	catch (CFileException * e)
	{
		e->Delete();
	}
	// Open a new log file
	fopen_s(&m_pLog, NEW_LOG, "w");
}

CAxisLog::~CAxisLog()
{
	// Close the file
	Add(1,CMsg("IDS_SHUTDOWN"));
	if ( m_pLog != NULL )
		fclose(m_pLog);
}

void CAxisLog::Add(int iFormat, const char *format, ...)
{
	if ( m_pLog == NULL )
		return;
	time_t now;
	now = time(NULL);
	struct tm lt;
	localtime_s(&lt,&now);
	fprintf( m_pLog, "%ld.%ld.%ld %02d:%02d:%02d ::", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec );
	CString csText;
	va_list args;
	va_start(args, format);
	csText.FormatV(format, args);
	vfprintf(m_pLog, format, args);
	va_end(args);
	fprintf(m_pLog, "\n");
	fflush(m_pLog);
	if (Main->m_pcppAxisLogTab)
	{
		CLogTab * pLogTab = (CLogTab *)Main->m_pcppAxisLogTab->m_dcCurrentPage;
		if (pLogTab)
		{
			switch( iFormat )
			{
			case 0:
				pLogTab->AddText(csText, RGB(0, 0, 0));
				break;
			case 1:
				pLogTab->AddText(csText, RGB(255, 0, 0));
				break;
			case 2:
				pLogTab->AddText(csText, RGB(0, 0, 255));
				break;
			default:
				pLogTab->AddText(csText, RGB(0, 0, 0));
			}
		}
	}
}