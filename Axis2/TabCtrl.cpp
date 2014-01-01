// MyTabCtrl.cpp : implementation file
//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl Class
// Written by: Brooks Younce
// Date: Jan 19 2007
// XP style compatability idea modified from Luuk Weltevreden http://www.codeproject.com/wtl/ThemedDialog.asp
// This class allows easy inserting of controls into your Tab control, with minimal coding.
// Free for public use, but must not be misrepresented, please leave comments attached.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Axis2.h"
#include "TabCtrl.h"


// CMyTabCtrl dialog

IMPLEMENT_DYNAMIC(CMyTabCtrl, CDialog)

CMyTabCtrl::CMyTabCtrl()
{
}

CMyTabCtrl::~CMyTabCtrl()
{
	if(m_hBrush){::DeleteObject(m_hBrush);}
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
	ON_MESSAGE(WM_BUTTONPRESSED,ButtonPressed)
	ON_MESSAGE(WM_UPDOWN,UpDownButton)
END_MESSAGE_MAP()


// CMyTabCtrl message handlers

void CMyTabCtrl::OnDestroy()
{
	CTabCtrl::OnDestroy();

	// TODO: Add your message handler code here
	//CLEAN UP MEMORY
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		pItem->wnd->DestroyWindow();
		delete pItem->wnd; //cleanup item from struct
		pItem->wnd = NULL;
		delete pItem;	   //cleanup struct	
		pItem = NULL;
	}
}

void CMyTabCtrl::CreateButton(LPCTSTR sCaption, int nID, int iTab, UINT uLocation, int iX, int iY, int iLen)
{
	CRect rc;
	GetClientRect(&rc);//GetWindowRect
	int x = rc.left + MARGIN_LEFT;
	int y = rc.top + MARGIN_TOP;
	if(uLocation & P_BELOW){y = iLastBottom;}
	if(uLocation & P_RIGHT){x = iLastRight;}
	if(uLocation & P_LEFT){x = iLastLeft;}
	if(uLocation & P_TOP){y = iLastTop;}
	x += iX;
	y += iY;

	CDC* dcPtr = GetDC();
	CFont* fontPtr = GetFont();
	fontPtr = dcPtr->SelectObject(fontPtr);
	CSize sz = dcPtr->GetTextExtent(sCaption, (int)strlen(sCaption));
	fontPtr = dcPtr->SelectObject(fontPtr);
	ReleaseDC(dcPtr);
	sz.cy += 8;

	if(iLen != 0){sz.cx = iLen;}
	CButton* pButton = new CButton;
	pButton->Create(sCaption,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,CRect(x,y,x+sz.cx,y+sz.cy),this,nID);
	pButton->SetFont(GetFont());
	if(iTab == GetCurFocus()){pButton->ShowWindow(SW_SHOW);}else{pButton->ShowWindow(SW_HIDE);}
	iLastBottom = y + sz.cy;
	iLastRight = x + sz.cx;
	iLastLeft = x;
	iLastTop = y;
	//save the item struct to the object array
	ITEM* pItem = new ITEM;
	pItem->bTabStop = TRUE;
	pItem->iTab = iTab;
	pItem->wnd = (CWnd*)pButton;
	pItem->iID = nID;
	obArray.Add((CObject*)pItem);
}

int CMyTabCtrl::BottomOf(int nID)
{
	CRect rc;
	GetDlgItem(nID)->GetWindowRect(&rc);
	ScreenToClient(rc);
	return rc.bottom - MARGIN_TOP;
}

int CMyTabCtrl::RightOf(int nID)
{
	CRect rc;
	GetDlgItem(nID)->GetWindowRect(&rc);
	ScreenToClient(rc);
	return rc.right - MARGIN_LEFT;
}

int CMyTabCtrl::LeftOf(int nID)
{
	CRect rc;
	GetDlgItem(nID)->GetWindowRect(&rc);
	ScreenToClient(rc);
	return rc.left - MARGIN_LEFT;
}

int CMyTabCtrl::TopOf(int nID)
{
	CRect rc;
	GetDlgItem(nID)->GetWindowRect(&rc);
	ScreenToClient(rc);
	return rc.top - MARGIN_TOP;
}

CString CMyTabCtrl::GetItemText(int nID)
{
	CString s;
	GetDlgItem(nID)->GetWindowText(s);
	return s;
}

int CMyTabCtrl::GetItemTextLength(int nID)
{
	CString s;
	GetDlgItem(nID)->GetWindowText(s);
	return s.GetLength();
}

int CMyTabCtrl::GetItemTextAsInt(int nID)
{
	CString s;
	GetDlgItem(nID)->GetWindowText(s);
	return atoi(s);
}

BOOL CMyTabCtrl::GetItemCheckState(int nID)
{ 
	return (BOOL)IsDlgButtonChecked(nID);
}

void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	ShowTab( GetCurFocus() );
}

void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	ShowTab( GetCurFocus() );
	CCommandsTab * pCommands = (CCommandsTab *)Main->m_pcppCommandsTab->m_dcCurrentPage;
	pCommands->iEditBtn = 0;
	pCommands->iRemoveBtn = 0;
	CString csLabel;
	csLabel = pCommands->csaTabs.GetAt(GetCurFocus());
	pCommands->m_ceTabText.SetWindowText(csLabel);
	*pResult = 0;
}

void CMyTabCtrl::ShowTab(int iTab)
{

	iCtrl = -1;//reset any control which may be selected in a tab

	if(iCurrentTab == iTab){return;}//already focused on selected tab
	
	//save old tab location so we can hide its controls
	int iOldTab = iCurrentTab;
	iCurrentTab = iTab;
		
	//check if tab is in focus, if not, put it in focus
	int t = GetCurFocus();
	if(iTab != t){SetCurSel(iTab);}

	//hide old tabs controls, show the selected tab's controls
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab == iOldTab){pItem->wnd->ShowWindow(SW_HIDE);}
		else if(pItem->iTab == iCurrentTab){pItem->wnd->ShowWindow(SW_SHOW);}
	}
}

void CMyTabCtrl::ClearTab(int iTab)
{
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab == iTab)
		{
			pItem->wnd->DestroyWindow();
			pItem->iTab = -1;
		}
	}

}

void CMyTabCtrl::ResetTab(int iTab)
{
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab > iTab)
			pItem->iTab -= 1;
	}

}

void CMyTabCtrl::SpreadTabs(int iTab)
{
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab >= iTab)
			pItem->iTab += 1;
	}

}

void CMyTabCtrl::FocusTab(int iTab)
{

	iCtrl = -1;//reset any control which may be selected in a tab

	iCurrentTab = iTab;

	int t = GetCurFocus();
	if(iTab != t){SetCurSel(iTab);}

	//show the selected tab's controls
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab == iTab)
		{
			pItem->wnd->ShowWindow(SW_SHOW);
		}
	}
}

void CMyTabCtrl::SortButtons(int iTab)
{
	//Sort the selected tab's controls
	INT_PTR iCount = obArray.GetCount();
	int X, Y;
	CRect rcTab;
	GetClientRect(&rcTab);//Get Tab Rect
	X = rcTab.left + MARGIN_LEFT;
	Y = rcTab.top + MARGIN_TOP;
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab == iTab)
		{
			CRect rc;
			pItem->wnd->GetClientRect(&rc);
			pItem->wnd->MoveWindow(X, Y, rc.right, rc.bottom);
			Y += 25;
			if (Y > rcTab.bottom - rc.bottom)
			{
				Y = rcTab.top + MARGIN_TOP;
				X += 81;
			}
		}
	}
}

int CMyTabCtrl::GetButtonCount(int iTab)
{
	//Count Buttons on selected tab
	int ibtn = 0;
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iTab == iTab)
			ibtn++;
	}
	return ibtn;
}

void CMyTabCtrl::RemoveButton(int nID)
{
	INT_PTR iCount = obArray.GetCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		ITEM* pItem = (ITEM*)obArray[i];
		if(pItem->iID == nID)
		{
			pItem->wnd->DestroyWindow();
			pItem->iTab = -1;
		}
	}
}

HBRUSH CMyTabCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTabCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(m_bThemeActive && nCtlColor == CTLCOLOR_STATIC)
	{
		CRect rc;
		// Set the background mode to transparent
		::SetBkMode(pDC->m_hDC,TRANSPARENT);
		return m_hBrush;
		//return ::CreateSolidBrush(RGB(255,0,0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CMyTabCtrl::Init()
{
	iCurrentTab = 0;
	iLastBottom = 0;
	iLastRight = 0;
	iLastLeft = 0;
	iLastTop = 0;
	iCtrl = -1;
	m_hBrush = NULL;

	//UpdateBackgroundBrush
	try
	{
		HMODULE hinstDll;
		// Check if the application is themed
		hinstDll = LoadLibrary(_T("UxTheme.dll"));
		if(hinstDll)
		{
			typedef BOOL (*ISAPPTHEMEDPROC)();
			ISAPPTHEMEDPROC pIsAppThemed;
			pIsAppThemed = (ISAPPTHEMEDPROC)::GetProcAddress(hinstDll, "IsAppThemed");
			if(pIsAppThemed){m_bThemeActive = pIsAppThemed();}
			FreeLibrary(hinstDll);
		}

		// Destroy old brush
		if(m_hBrush){::DeleteObject(m_hBrush);}
		m_hBrush = NULL;

		// Only do this if the theme is active
		if(m_bThemeActive)
		{
			RECT rc;
			// Get tab control dimensions
			GetWindowRect(&rc);//m_ctab	
			// Get the tab control DC
			CDC* pDC = GetDC();	//m_ctab		
			HDC hDC = pDC->m_hDC;//m_ctab		
			// Create a compatible DC
			HDC hDCMem = ::CreateCompatibleDC(hDC);
			HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
			HBITMAP hBmpOld = (HBITMAP)(::SelectObject(hDCMem, hBmp));
			// Tell the tab control to paint in our DC
			SendMessage(WM_PRINTCLIENT, (WPARAM)(hDCMem), (LPARAM)(PRF_ERASEBKGND | PRF_CLIENT | PRF_NONCLIENT));//m_cTab			
			// Create a pattern brush from the bitmap selected in our DC
			//m_hBrush = ::CreatePatternBrush(hBmp);
			// Create a Soild brush from a specific pixel which the exact color we need
			m_hBrush = ::CreateSolidBrush(::GetPixel(hDCMem,5,5));
			// Restore the bitmap
			::SelectObject(hDCMem, hBmpOld);
			// Cleanup
			::DeleteObject(hBmp);
			::DeleteDC(hDCMem);
			ReleaseDC(pDC);//m_ctab
		}
	}
	catch(...)
	{
	}
}

//intercept messages before they are dispatched
BOOL CMyTabCtrl::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_DOWN)
		{
			PostMessage(WM_UPDOWN,0,0);
			return TRUE;
		}
		if(pMsg->wParam == VK_UP)
		{
			PostMessage(WM_UPDOWN,1,0);
			return TRUE;
		}
		else if(pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_TAB)
		{
			iCtrl = -1;//unselect any child control which may be selected
		}
		else if(pMsg->wParam == VK_ESCAPE)//pMsg->wParam == VK_RETURN 
		{return TRUE;}
	}
	else if(pMsg->message == WM_KEYUP)
	{
		if(pMsg->wParam == VK_SPACE)
		{
			PostMessage(WM_BUTTONPRESSED,(WPARAM)pMsg->hwnd,1);
		}
	}
	else if(pMsg->message == WM_LBUTTONUP)
	{
		PostMessage(WM_BUTTONPRESSED,(WPARAM)pMsg->hwnd,0);
	}
	return CTabCtrl::PreTranslateMessage(pMsg);
}

LRESULT CMyTabCtrl::UpDownButton(WPARAM w, LPARAM l)
{
	UNREFERENCED_PARAMETER(l);
	int iCurTab = GetCurFocus();
	int iCount = (int)obArray.GetCount();

	int iOldPos = iCtrl;//save old pos

	//increment or deincrement
	if(w == 0){iCtrl++;}//down button
	else if(w == 1){iCtrl--;}//up button

	//overflow check
	if(iCtrl >= iCount){iCtrl = iCount-1;}
	if(iCtrl < 0){iCtrl = 0;}

	if(w == 0)//down button
	{
		//-------------------------------
		for(int i=iCtrl; i<iCount; i++)//spin until we get to our current tab
		{
			ITEM* pItem = (ITEM*)obArray[i];
			if(pItem->iTab == iCurTab)//scroll through current tab controls only
			{
				if(pItem->bTabStop)
				{
					pItem->wnd->SetFocus(); 
					iCtrl = i;
					return 0;
				}			
			}	
		}
		//-------------------------------
	}
	else
	{
		//-------------------------------
		for(int i=iCtrl; i>=0; i--)//spin until we get to our current tab
		{
			ITEM* pItem = (ITEM*)obArray[i];
			if(pItem->iTab == iCurTab)//scroll through current tab controls only
			{
				if(pItem->bTabStop)
				{
					pItem->wnd->SetFocus(); 
					iCtrl = i;
					return 0;
				}			
			}	
		}
		//-------------------------------
	}
	iCtrl = iOldPos; //no control was found to set focus to, we are end the end of the control set, backup

	return 0;
}

LRESULT CMyTabCtrl::ButtonPressed(WPARAM w, LPARAM l)
{
	int nID = 0;
	HWND h = (HWND)w;
	BOOL bKeyPress = (BOOL)l;
	if(h == NULL){return 0;}
	CPoint cur;
	CRect rc;
	::GetCursorPos(&cur);	
	::GetWindowRect(h,rc);
	CWnd* pWnd = CWnd::FromHandle(h);
	//make sure mouse is inside of button when released
	if((cur.x > rc.left && cur.x < rc.right && cur.y > rc.top && cur.y < rc.bottom) || bKeyPress)
	{		
		//do we have a normal push button?
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP;
		DWORD dw = pWnd->GetStyle();
		dw &= ~(dwStyle);//remove all the styles from dw
		if(dw <= 1)//regular pushbutton
		{			
			nID = pWnd->GetDlgCtrlID();
			GetParent()->PostMessage(WM_BUTTONPRESSED,nID,0);
		}
	}
	return 0;
}