// DockingPage.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "DockingPage.h"


// CDockingPage dialog

IMPLEMENT_DYNAMIC(CDockingPage, CPropertyPage)

CDockingPage::CDockingPage(UINT id, CString csCaption)
	: CPropertyPage(id)
{
	template_id = id;
	m_psp.pszTitle = m_strCaption = csCaption;
	m_psp.dwFlags |= PSP_USETITLE;
	m_bModifyDlgStylesAndPos = false;
	csTitle = csCaption+" "+CMsg("IDS_TAB");
	m_dcDialogPage = NULL;
}

CDockingPage::~CDockingPage()
{
	delete m_dcDialogPage;
}

void CDockingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDockingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CDockingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if((m_bModifyDlgStylesAndPos == true) && m_hWnd)
	{
		SetWindowText(csTitle);
		CRect rectFrame, rectDlg;
		CWnd* pMainWnd = AfxGetMainWnd();
		if(pMainWnd != NULL)
		{
			pMainWnd->GetClientRect(rectFrame);
			pMainWnd->ClientToScreen(rectFrame);
			GetWindowRect(rectDlg);
			int nXPos = rectFrame.left + (rectFrame.Width() / 2) - (rectDlg.Width() / 2);
			int nYPos = rectFrame.top + (rectFrame.Height() / 2) - (rectDlg.Height() / 2);
			::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
		}
	}
	return TRUE;
}

void CDockingPage::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	ClientToScreen(&point);
	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		if (m_bModifyDlgStylesAndPos == true)
			::AppendMenu(hMenu, MF_STRING, 2, CMsg("IDS_DOCK"));
		else
			::AppendMenu(hMenu, MF_STRING, 1, CMsg("IDS_UNDOCK"));
		int sel = ::TrackPopupMenuEx(hMenu, 
				TPM_CENTERALIGN | TPM_RETURNCMD,
				point.x,
				point.y,
				m_hWnd,
				NULL);
		switch (sel)
		{
		case 1:
			{
				CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
				dlg->RemovePage(this);
				m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
				m_dcDialogPage->Create(template_id);
			break;
			}
		case 2:
			{
			OnCancel();
			break;
			}
		}
		::DestroyMenu(hMenu);
	}
}

void CDockingPage::OnCancel() 
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->AddPage(m_dcPropertyPage);
	FixTabs(template_id);
	dlg->SetActivePage(m_dcPropertyPage);
	CDialog::OnCancel();
}

void CDockingPage::PreSubclassWindow() 
{
	if (m_bModifyDlgStylesAndPos == true)
	{
		if(m_hWnd != NULL)
		{
			LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
			lStyle &= ~WS_CHILD;
			lStyle &= ~WS_DISABLED;

			lStyle |= WS_POPUP;
			lStyle |= WS_VISIBLE;
			lStyle |= WS_SYSMENU;
			lStyle |= WS_MINIMIZEBOX;
			SetWindowLong(m_hWnd, GWL_STYLE, lStyle);  
		}
	}

	CPropertyPage::PreSubclassWindow();
}

HBRUSH CDockingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	if((m_bModifyDlgStylesAndPos == true)||(m_bAlwaysColor == true))
		return Main->m_bkbrush;
	return hbr;
}

void CDockingPage::FixTabs(UINT iTab)
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	int iCurrent = 0;
	for(int ipages = 0; ipages < dlg->GetPageCount(); ipages++)
	{
		CPropertyPage * TestPage = dlg->GetPage(iCurrent);
		bool remove = false;

		switch (iTab)
		{
		case IDD_LOG_TAB:
			return;
		case IDD_GENERAL_TAB:
			{
				if(TestPage == Main->m_pcppTravelTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_TRAVEL_TAB:
			{
				if(TestPage == Main->m_pcppSpawnTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_SPAWN_TAB:
			{
				if(TestPage == Main->m_pcppPlayerTweakTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_PLAYERTWEAK_TAB:
			{
				if(TestPage == Main->m_pcppItemTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_ITEMS_TAB:
			{
				if(TestPage == Main->m_pcppItemTweakTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_ITEMTWEAK_TAB:
			{
				if(TestPage == Main->m_pcppAccountTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_ACCOUNT_TAB:
			{
				if(TestPage == Main->m_pcppMiscTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_MISC_TAB:
			{
				if(TestPage == Main->m_pcppLauncherTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_LAUNCHER_TAB:
			{
				if(TestPage == Main->m_pcppCommandsTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_COMMANDS_TAB:
			{
				if(TestPage == Main->m_pcppReminderTab)
				{
					remove = true;
					break;
				}
			}
		case IDD_REMINDER_TAB:
			{
				if(TestPage == Main->m_pcppAxisLogTab)
				{
					remove = true;
					break;
				}
			}
		}
		
		if( remove )
		{
			dlg->RemovePage(TestPage);
			dlg->AddPage(TestPage);
		}
		else
			iCurrent++;
	}
}

BOOL CDockingPage::OnSetActive()
{
	Main->m_pMainWnd->SetWindowPos(Main->m_dwAlwaysOnTop ? &CWnd::wndTopMost : &CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return CPropertyPage::OnSetActive();
}
