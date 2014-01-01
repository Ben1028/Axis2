// AxisThread.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "AxisThread.h"
#include "ProgressBar.h"


// CAxisThread

IMPLEMENT_DYNCREATE(CAxisThread, CWinThread)

CAxisThread::CAxisThread()
{
}

CAxisThread::~CAxisThread()
{
}

BOOL CAxisThread::InitInstance()
{
	m_pMainWnd=new CProgressBar;
    m_pMainWnd->SetForegroundWindow();
    m_pMainWnd->ShowWindow(SW_SHOW);
	return TRUE;
}

int CAxisThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAxisThread, CWinThread)
END_MESSAGE_MAP()


// CAxisThread message handlers
