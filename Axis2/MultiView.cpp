// WorldMap.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "MultiView.h"
#include "UOart.h"

// CMultiView dialog

IMPLEMENT_DYNAMIC(CMultiView, CDialog)

CMultiView::CMultiView(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiView::IDD, pParent)
{

}

CMultiView::~CMultiView()
{
}

void CMultiView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MULTIVIEW, m_MultiView);
}


BEGIN_MESSAGE_MAP(CMultiView, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMultiView message handlers

BOOL CMultiView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rectFrame, rectDlg;
	CWnd* pMainWnd = AfxGetMainWnd();
	if(pMainWnd != NULL)
	{
		m_MultiView.SetArtIndex(l_ArtIndex);
		m_MultiView.SetArtType(0);
		m_MultiView.SetBkColor(Main->m_dwItemBGColor);
		pMainWnd->GetClientRect(rectFrame);
		pMainWnd->ClientToScreen(rectFrame);
		GetWindowRect(rectDlg);
		int nXPos = rectFrame.left + (rectFrame.Width() / 2) - (rectDlg.Width() / 2);
		int nYPos = rectFrame.top + (rectFrame.Height() / 2) - (rectDlg.Height() / 2);
		::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
	}
	return TRUE;
}

void CMultiView::OnCancel() 
{
	Main->m_dlgMultiView = NULL;
	CDialog::OnCancel();
}

void CMultiView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	CRect rect;
	m_MultiView.GetWindowRect(&rect);
	if ( rect.PtInRect(point) )
	{
		xscroll = point.x-rect.left;
		yscroll = point.y-rect.top;
	}
	else
	{
		ScreenToClient(&point);
		CDialog::OnLButtonDown(nFlags, point);
	}
}

void CMultiView::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	CRect rect;
	m_MultiView.GetWindowRect(&rect);
	if ( (rect.PtInRect(point)) && (nFlags & MK_LBUTTON) )
	{
		short xs, ys;
		xs = (short) point.x - (short) rect.left - (short) xscroll;
		ys = (short) point.y - (short) rect.top - (short) yscroll;
		xscroll = point.x-rect.left;
		yscroll = point.y-rect.top;

		m_MultiView.Scroll(xs, ys);
	}
}