// WorldMap.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "WorldMap.h"
#include "UOmap.h"

// CWorldMap dialog

IMPLEMENT_DYNAMIC(CWorldMap, CDialog)

CWorldMap::CWorldMap(CWnd* pParent /*=NULL*/)
	: CDialog(CWorldMap::IDD, pParent)
{

}

CWorldMap::~CWorldMap()
{
}

void CWorldMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORLDMAP, m_WorldMap);
}


BEGIN_MESSAGE_MAP(CWorldMap, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CWorldMap message handlers

BOOL CWorldMap::OnInitDialog() 
{
	CDialog::OnInitDialog();
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
	return TRUE;
}

void CWorldMap::OnCancel() 
{
	Main->m_dlgWorldMap = NULL;
	CDialog::OnCancel();
}

void CWorldMap::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	ClientToScreen(&point);
	CRect rect;
	m_WorldMap.GetWindowRect(&rect);
	if ( rect.PtInRect(point) )
	{
		short xc, yc, z;
		xc = (short) point.x - (short) rect.left;
		yc = (short) point.y - (short) rect.top;
		m_WorldMap.CtrlToMap(&xc, &yc);
		if(xc >= m_WorldMap.GetWidth())
			xc = m_WorldMap.GetWidth()-1;
		if(yc >= m_WorldMap.GetHeight())
			yc = m_WorldMap.GetHeight()-1;
		if(xc < 0)
			xc = 0;
		if(yc < 0)
			yc = 0;
		CString csCoords;
		z = m_WorldMap.GetMapHeight(xc, yc);
		csCoords.Format("%d,%d,%d", xc, yc, z);

		CTravelTab * pTravel = (CTravelTab *)Main->m_pcppTravelTab->m_dcCurrentPage;
		pTravel->m_csCoords.SetWindowText(csCoords);
		pTravel->RemoteRecenter(xc, yc);
		m_WorldMap.SetCenter(xc, yc);
		m_WorldMap.UpdateMap();
	}
}

void CWorldMap::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	ClientToScreen(&point);
	CRect rect;
	m_WorldMap.GetWindowRect(&rect);
	if ( rect.PtInRect(point) )
	{
		HMENU hMenu = ::CreatePopupMenu();
		if (NULL != hMenu)
		{
			if (m_WorldMap.GetZoomLevel() == 2)
				::AppendMenu(hMenu, MF_STRING|MF_GRAYED, 1, "Zoom In");
			else
				::AppendMenu(hMenu, MF_STRING, 1, "Zoom In");
			if (m_WorldMap.GetZoomLevel() == -4)
				::AppendMenu(hMenu, MF_STRING|MF_GRAYED, 2, "Zoom Out");
			else
				::AppendMenu(hMenu, MF_STRING, 2, "Zoom Out");
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
					m_WorldMap.ZoomIn();
					m_WorldMap.UpdateMap();
				break;
				}
			case 2:
				{
					m_WorldMap.ZoomOut();
					m_WorldMap.UpdateMap();
				break;
				}
			}
			::DestroyMenu(hMenu);
		}
	}
}

void CWorldMap::UpdateSpawnDisplay(int M)
{
	m_WorldMap.RemoveDrawObjects();
	short sSize = (m_WorldMap.GetZoomLevel()+6)/2;
	for ( int i = 0; i < Main->m_pScripts->m_asaSPAWNchar.GetSize(); i++ )
	{
		CString csSpawn, csTemp, csX, csY, csZ, csM;
		csSpawn = Main->m_pScripts->m_asaSPAWNchar.GetAt(i);
		csX = csSpawn.SpanExcluding(",");
		csTemp = csSpawn.Mid(csSpawn.Find(",") + 1);
		csY = csTemp.SpanExcluding(",");
		if ( csTemp.Find(",") != -1 )
		{
			csTemp = csTemp.Mid(csTemp.Find(",") + 1);
			csZ = csTemp.SpanExcluding(",");
			if ( csTemp.Find(",") != -1 )
			{
				csTemp = csTemp.Mid(csTemp.Find(",") + 1);
				csM = csTemp.SpanExcluding(",");
			}
		}
		short x = (short) atoi(csX);
		short y = (short) atoi(csY);
		//short z = (short) atoi(csZ);
		short m = (short) atoi(csM);
		if (m == M)
			m_WorldMap.AddDrawObject(x, y, 1, sSize, Main->m_dwNPCSpawnColor);
	}
	for ( int j = 0; j < Main->m_pScripts->m_asaSPAWNitem.GetSize(); j++ )
	{
		CString csSpawn, csTemp, csX, csY, csZ, csM;
		csSpawn = Main->m_pScripts->m_asaSPAWNitem.GetAt(j);
		csX = csSpawn.SpanExcluding(",");
		csTemp = csSpawn.Mid(csSpawn.Find(",") + 1);
		csY = csTemp.SpanExcluding(",");
		if ( csTemp.Find(",") != -1 )
		{
			csTemp = csTemp.Mid(csTemp.Find(",") + 1);
			csZ = csTemp.SpanExcluding(",");
			if ( csTemp.Find(",") != -1 )
			{
				csTemp = csTemp.Mid(csTemp.Find(",") + 1);
				csM = csTemp.SpanExcluding(",");
			}
		}
		short x = (short) atoi(csX);
		short y = (short) atoi(csY);
		//short z = (short) atoi(csZ);
		short m = (short) atoi(csM);
		if (m == M)
			m_WorldMap.AddDrawObject(x, y, 1, sSize, Main->m_dwItemSpawnColor);
	}

}

void CWorldMap::UpdateSize()
{
	CRect rectDlg;
	CWnd* pMainWnd = AfxGetMainWnd();
		if(pMainWnd != NULL)
		{
			HWND hCtrl = m_WorldMap.GetSafeHwnd();
			GetWindowRect(&rectDlg);
			short w = m_WorldMap.GetWidth();
			short h = m_WorldMap.GetHeight();
			m_WorldMap.GetDisplaySize(&w, &h);
			::MoveWindow(hCtrl, 0, 0, w, h, TRUE);
			::MoveWindow(m_hWnd, rectDlg.left+((rectDlg.right-rectDlg.left-(w+6))/2), rectDlg.top+((rectDlg.bottom-rectDlg.top-(h+32))/2), w+6, h+32, TRUE);
		}
}