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

// ItemTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ItemTab.h"
#include "UOart.h"
#include "Common.h"
#include "SearchCritDlg.h"
#include "MultiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemTab property page

IMPLEMENT_DYNCREATE(CItemTab, CDockingPage)

int CItemTab::iIDSort = 1;
int CItemTab::iNameSort = 1;

CItemTab::CItemTab() : CDockingPage(CItemTab::IDD,CMsg("IDS_ITEMS"))
{
	//{{AFX_DATA_INIT(CItemTab)
	m_bLockDown = FALSE;
	m_iCatSeq = 0;
	icX = 60;
	icY = 110;
	//}}AFX_DATA_INIT
}

CItemTab::~CItemTab()
{
}

void CItemTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemTab)
	DDX_Control(pDX, IDC_NUDGEUP, m_cbNudgeUp);
	DDX_Control(pDX, IDC_NUDGEDOWN, m_cbNudgeDown);
	DDX_Control(pDX, IDC_ITEMS, m_clcItems);
	DDX_Control(pDX, IDC_CATEGORY_TREE, m_ctcCategories);
	DDX_Control(pDX, IDC_ITEMID, m_csItemID);
	DDX_Control(pDX, IDC_ITEMIDDEC, m_csItemIDDec);
	DDX_Control(pDX, IDC_LOCKITEM, m_cbLockDown);
	DDX_Control(pDX, IDC_DISPLAY, m_Display);
	DDX_Control(pDX, IDC_ZTILE, m_ceZTile);
	DDX_Control(pDX, IDC_NUKEARG, m_ceNukearg);
	DDX_Control(pDX, IDC_MINTIME, m_ceMinTime);
	DDX_Control(pDX, IDC_MAXTIME, m_ceMaxTime);
	DDX_Control(pDX, IDC_MAXDIST, m_ceMaxDist);
	DDX_Control(pDX, IDC_AMOUNT, m_ceAmount);
	DDX_Control(pDX, IDC_SPAWNRATE, m_ceSpawnRate);
	DDX_Control(pDX, IDC_NUDGEAMOUNT, m_ceNudge);
	DDX_Control(pDX, IDC_MOVE1, m_cbMove1);
	DDX_Control(pDX, IDC_MOVE2, m_cbMove2);
	DDX_Control(pDX, IDC_MOVE3, m_cbMove3);
	DDX_Control(pDX, IDC_MOVE4, m_cbMove4);
	DDX_Control(pDX, IDC_MOVE5, m_cbMove5);
	DDX_Control(pDX, IDC_MOVE6, m_cbMove6);
	DDX_Control(pDX, IDC_MOVE7, m_cbMove7);
	DDX_Control(pDX, IDC_MOVE8, m_cbMove8);
	DDX_Check(pDX, IDC_LOCKITEM, m_bLockDown);
	DDX_Control(pDX, IDC_FINDITEM, cb_finditem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemTab, CDockingPage)
	//{{AFX_MSG_MAP(CItemTab)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CATEGORY_TREE, OnSelchangedCategoryTree)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEMS, OnItemchangedItems)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ITEMS, OnColumnclickItems)
	ON_NOTIFY(NM_DBLCLK, IDC_ITEMS, OnDblclkItems)
	ON_NOTIFY(NM_RCLICK, IDC_ITEMS, OnRclickItems)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_LOCKITEM, OnLockitem)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_TILE, OnTile)
	ON_BN_CLICKED(IDC_FLIP, OnFlip)
	ON_BN_CLICKED(IDC_PLACESPAWN, OnPlacespawn)
	ON_BN_CLICKED(IDC_INITSPAWN, OnInitspawn)
	ON_BN_CLICKED(IDC_NUKE, OnNuke)
	ON_BN_CLICKED(IDC_NUDGEUP, OnNudgeup)
	ON_BN_CLICKED(IDC_NUDGEDOWN, OnNudgedown)
	ON_BN_CLICKED(IDC_MOVE1, OnMove1)
	ON_BN_CLICKED(IDC_MOVE2, OnMove2)
	ON_BN_CLICKED(IDC_MOVE3, OnMove3)
	ON_BN_CLICKED(IDC_MOVE4, OnMove4)
	ON_BN_CLICKED(IDC_MOVE5, OnMove5)
	ON_BN_CLICKED(IDC_MOVE6, OnMove6)
	ON_BN_CLICKED(IDC_MOVE7, OnMove7)
	ON_BN_CLICKED(IDC_MOVE8, OnMove8)
	ON_EN_CHANGE(IDC_MAXTIME, OnChangeParams)
	ON_EN_CHANGE(IDC_MINTIME, OnChangeParams)
	ON_EN_CHANGE(IDC_SPAWNRATE, OnChangeParams)
	ON_EN_CHANGE(IDC_AMOUNT, OnChangeParams)
	ON_EN_CHANGE(IDC_NUDGEAMOUNT, OnChangeParams)
	ON_EN_CHANGE(IDC_ZTILE, OnChangeParams)
	ON_EN_CHANGE(IDC_MAXDIST, OnChangeParams)
	ON_BN_CLICKED(IDC_FINDITEM, OnFinditem)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemTab message handlers

BOOL CItemTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppItemTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CItemTab;
	Main->m_pcppItemTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	CString csValue;
	DWORD dwValue;
	dwValue = Main->GetRegistryDword("LockDownItems", 0);
	m_cbLockDown.SetCheck(dwValue);
	csValue = Main->GetRegistryString("ItemSpawnMaxAmount", "1");
	m_ceAmount.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ItemSpawnMaxTime", "240");
	m_ceMaxTime.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ItemSpawnMinTime", "30");
	m_ceMinTime.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ItemSpawnRate", "0");
	m_ceSpawnRate.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ItemSpawnMaxDist", "0");
	m_ceMaxDist.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ItemNudgeAmount", "1");
	m_ceNudge.SetWindowText(csValue);
	csValue = Main->GetRegistryString("ItemTileZ", "0");
	m_ceZTile.SetWindowText(csValue);

	this->m_clcItems.InsertColumn(0, "Item Description", LVCFMT_LEFT, 140, -1);
	this->m_clcItems.InsertColumn(1, "ID", LVCFMT_LEFT, 140, -1);
	FillCategoryTree();

	m_pImageList = new CImageList();
	m_pImageList->Create(icX, icY, ILC_COLORDDB, 0, 1);
	m_clcItems.SetImageList(m_pImageList, LVSIL_NORMAL);
	m_iCurState = m_clcItems.GetView();

	WORD wFlags = 0;
	if ( Main->m_dwRoomView )
		wFlags |= F_ROOMVIEW;
	m_Display.SetDrawFlags(wFlags);
	m_Display.SetArtType(1);
	m_Display.SetArtIndex(-1);
	m_Display.SetBkColor(Main->m_dwItemBGColor);

	m_cbNudgeDown.InitButton(IDI_ARROWDOWN,CMsg("IDS_NUDGEDOWN"));
	m_cbNudgeDown.SetBGColor(Main->crAxisBkg);
	m_cbNudgeUp.InitButton(IDI_ARROWUP,CMsg("IDS_NUDGEUP"));
	m_cbNudgeUp.SetBGColor(Main->crAxisBkg);

	m_cbMove1.InitButton(IDI_SARROWUP,CMsg("IDS_UP"));
	m_cbMove1.SetBGColor(Main->crAxisBkg);
	m_cbMove3.InitButton(IDI_SARROWRIGHT,CMsg("IDS_RIGHT"));
	m_cbMove3.SetBGColor(Main->crAxisBkg);
	m_cbMove5.InitButton(IDI_SARROWDOWN,CMsg("IDS_DOWN"));
	m_cbMove5.SetBGColor(Main->crAxisBkg);
	m_cbMove7.InitButton(IDI_SARROWLEFT,CMsg("IDS_LEFT"));
	m_cbMove7.SetBGColor(Main->crAxisBkg);

	m_cbMove2.InitButton(IDI_SARROWRIGHTU,CMsg("IDS_RIGHTUP"));
	m_cbMove2.SetBGColor(Main->crAxisBkg);
	m_cbMove4.InitButton(IDI_SARROWDOWNR,CMsg("IDS_DOWNRIGHT"));
	m_cbMove4.SetBGColor(Main->crAxisBkg);
	m_cbMove6.InitButton(IDI_SARROWLEFTD,CMsg("IDS_LEFTDOWN"));
	m_cbMove6.SetBGColor(Main->crAxisBkg);
	m_cbMove8.InitButton(IDI_SARROWUPL,CMsg("IDS_UPLEFT"));
	m_cbMove8.SetBGColor(Main->crAxisBkg);

	cb_finditem.InitButton(IDI_FIND,CMsg("IDS_FINDITEM"));
	cb_finditem.SetBGColor(Main->crAxisBkg);

	//m_Display.SetArtType(1);
	//m_Display.SetArtIndex(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemTab::OnCreate() 
{
	CString csValue;
	this->m_csItemID.GetWindowText(csValue);
	if ( csValue == "" )
	{
		AfxMessageBox("Selected entry is not an item or a template", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	CString csCmd;
	if (m_bLockDown)
		csCmd.Format("%sstatic %s", Main->m_csCommandPrefix, csValue);
	else
		csCmd.Format("%sadd %s", Main->m_csCommandPrefix, csValue);
	SendToUO(csCmd);
}

void CItemTab::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	CRect rect;
	m_Display.GetWindowRect(&rect);
	if ( rect.PtInRect(point) )
	{
		if (m_Display.GetArtType() == 0)
		{
			HMENU hMenu = ::CreatePopupMenu();
			if (NULL != hMenu)
				::AppendMenu(hMenu, MF_STRING, 1, "Open Multi Viewer");

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
					if (Main->m_dlgMultiView)
						delete Main->m_dlgMultiView;
					Main->m_dlgMultiView = new CMultiView;
					Main->m_dlgMultiView->l_ArtIndex = m_Display.GetArtIndex();
					Main->m_dlgMultiView->Create(IDD_MULTIVIEW);
				}
			}
			::DestroyMenu(hMenu);
		}
	}
	else
	{
		ScreenToClient(&point);
		CDockingPage::OnRButtonDown(nFlags, point);
	}
}


void CItemTab::OnLockitem() 
{
	UpdateData();
	Main->PutRegistryDword("LockDownItems", m_bLockDown);
}

void CItemTab::OnRemove() 
{
	CString csCmd;
	csCmd.Format("%sremove", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}

void CItemTab::OnTile() 
{
	CString csValue, csID;
	this->m_csItemID.GetWindowText(csID);
	if (csID == "")
	{
		AfxMessageBox("No item is selected.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	this->m_ceZTile.GetWindowText(csValue);
	if (csValue.SpanIncluding("0123456789-") != csValue)
	{
		AfxMessageBox("Invalid value in the z-level field.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if (csValue == "")
		csValue = "0";
	CString csCmd;
	csCmd.Format("%stile %s %s", Main->m_csCommandPrefix, csValue, csID);
	SendToUO(csCmd);
}

void CItemTab::OnFlip() 
{
	CString csCmd;
	csCmd.Format("%sxflip", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}

void CItemTab::OnPlacespawn() 
{
	CString csCmd;
	csCmd.Format("%sadd 01ea7", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}

void CItemTab::OnNuke() 
{
	CString csNukeArg, csCmd;
	this->m_ceNukearg.GetWindowText(csNukeArg);
	csCmd.Format("%snuke %s", Main->m_csCommandPrefix, csNukeArg);
	SendToUO(csCmd);
}

void CItemTab::OnNudgeup() 
{
	CString csAmount, csCmd;
	this->m_ceNudge.GetWindowText(csAmount);
	csCmd.Format("%snudgeup %s", Main->m_csCommandPrefix, csAmount);
	SendToUO(csCmd);
}

void CItemTab::OnNudgedown() 
{
	CString csAmount, csCmd;
	this->m_ceNudge.GetWindowText(csAmount);
	csCmd.Format("%snudgedown %s", Main->m_csCommandPrefix, csAmount);
	SendToUO(csCmd);
}

void CItemTab::OnMove1()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	if (!csMove)
		csMove = "0";
	csCmd.Format("%sxmove 0 -%s", Main->m_csCommandPrefix, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove2()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	if (!csMove)
		csMove = "0";
	csCmd.Format("%sxmove %s -%s", Main->m_csCommandPrefix, csMove, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove3()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	csCmd.Format("%sxmove %s 0", Main->m_csCommandPrefix, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove4()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	if (!csMove)
		csMove = "0";
	csCmd.Format("%sxmove %s %s", Main->m_csCommandPrefix, csMove, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove5()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	csCmd.Format("%sxmove 0 %s", Main->m_csCommandPrefix, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove6()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	if (!csMove)
		csMove = "0";
	csCmd.Format("%sxmove -%s %s", Main->m_csCommandPrefix, csMove, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove7()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	if (!csMove)
		csMove = "0";
	csCmd.Format("%sxmove -%s 0", Main->m_csCommandPrefix, csMove);
	SendToUO(csCmd);
}

void CItemTab::OnMove8()
{
	CString csMove, csCmd;
	this->m_ceNudge.GetWindowText(csMove);
	if (!csMove)
		csMove = "0";
	csCmd.Format("%sxmove -%s -%s", Main->m_csCommandPrefix, csMove, csMove);
	SendToUO(csCmd);
}


void CItemTab::OnChangeParams() 
{
	UpdateData();
	if ( !this->IsWindowVisible() )
		return;

	CString csAmount, csMinTime, csMaxTime, csRate, csDist, csZHeight, csNudge;

	m_ceAmount.GetWindowText(csAmount);
	m_ceMaxDist.GetWindowText(csDist);
	m_ceMaxTime.GetWindowText(csMaxTime);
	m_ceMinTime.GetWindowText(csMinTime);
	m_ceNudge.GetWindowText(csNudge);
	m_ceSpawnRate.GetWindowText(csRate);
	m_ceZTile.GetWindowText(csZHeight);


	Main->PutRegistryString("ItemSpawnMaxAmount", csAmount);
	Main->PutRegistryString("ItemSpawnMaxTime", csMaxTime);
	Main->PutRegistryString("ItemSpawnMinTime", csMinTime);
	Main->PutRegistryString("ItemSpawnMaxDist", csDist);
	Main->PutRegistryString("ItemSpawnRate", csRate);
	Main->PutRegistryString("ItemNudgeAmount", csNudge);
	Main->PutRegistryString("ItemTileZ", csZHeight);

}

void CItemTab::OnInitspawn() 
{
	CWaitCursor hourglass;
	CString csAmount, csMinTime, csMaxTime, csMaxDist, csID, csSpawnRate;
	this->m_ceAmount.GetWindowText(csAmount);
	this->m_ceMaxDist.GetWindowText(csMaxDist);
	this->m_ceMinTime.GetWindowText(csMinTime);
	this->m_ceMaxTime.GetWindowText(csMaxTime);
	this->m_csItemID.GetWindowText(csID);
	this->m_ceSpawnRate.GetWindowText(csSpawnRate);

	if (csID == "")
	{
		AfxMessageBox("No item is selected.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int iAmount, iMinTime, iMaxTime, iMaxDist;
	iAmount = atoi(csAmount);
	if (iAmount == 0)
		iAmount = 1;
	iMaxDist = atoi(csMaxDist);
	iMinTime = atoi(csMinTime);
	iMaxTime = atoi(csMaxTime);
	if (iMaxTime <= iMinTime)
		iMaxTime = iMinTime + 1;

	CString csCmd;
	csCmd.Format("%sact.type %ld", Main->m_csCommandPrefix, ITEM_SPAWN_ITEM);
	SendToUO(csCmd);

	csCmd.Format("%sact.amount %ld", Main->m_csCommandPrefix, iAmount);
	SendToUO(csCmd);
	Sleep(SPAWN_MESSAGE_DELAY);

	csCmd.Format("%sact.more %s", Main->m_csCommandPrefix, csID);
	SendToUO(csCmd);

	csCmd.Format("%sact.more2 %s", Main->m_csCommandPrefix, csSpawnRate);
	SendToUO(csCmd);

	csCmd.Format("%sact.morep %ld %ld %ld", Main->m_csCommandPrefix, iMinTime, iMaxTime, iMaxDist);
	SendToUO(csCmd);

	csCmd.Format("%sact.attr %04x", Main->m_csCommandPrefix, ATTR_INVIS | ATTR_MAGIC | ATTR_MOVE_NEVER);
	SendToUO(csCmd);

	csCmd.Format("%sact.timer 1", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}

//***************************************************

void CItemTab::FillCategoryTree()
{
	HTREEITEM CategoryParent;
	HTREEITEM SubsectionParent;
	TV_INSERTSTRUCT InsertItem;

	m_clcItems.DeleteAllItems();
	m_ctcCategories.DeleteAllItems();
	if (!Main->m_pScripts->m_olItems.IsEmpty())
	{
		POSITION pos = Main->m_pScripts->m_olItems.GetHeadPosition();
		while (pos != NULL)
		{
			CCategory * pCategory = (CCategory *) Main->m_pScripts->m_olItems.GetNext(pos);
			InsertItem.item.mask = TVIF_TEXT;
			InsertItem.item.pszText = (char *)LPCTSTR(pCategory->m_csName);
			InsertItem.item.cchTextMax = pCategory->m_csName.GetLength();
			InsertItem.hParent = NULL;
			InsertItem.hInsertAfter = TVI_SORT;
			CategoryParent = this->m_ctcCategories.InsertItem(&InsertItem);
			if (!pCategory->m_SubsectionList.IsEmpty())
			{
				POSITION sPos = pCategory->m_SubsectionList.GetHeadPosition();
				while (sPos != NULL)
				{
					CSubsection * pSubsection = (CSubsection *) pCategory->m_SubsectionList.GetNext(sPos);
					InsertItem.item.mask = TVIF_TEXT;
					InsertItem.item.pszText = (char *)LPCTSTR(pSubsection->m_csName);
					InsertItem.item.cchTextMax = pSubsection->m_csName.GetLength();
					InsertItem.hParent = CategoryParent;
					InsertItem.hInsertAfter = TVI_SORT;
					SubsectionParent = this->m_ctcCategories.InsertItem(&InsertItem);
				}
			}
		}
	}
	m_iCatSeq = Main->m_pScripts->m_iICatSeq;
}


void CItemTab::OnSelchangedCategoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pResult);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hSelectedItem = NULL;
	HTREEITEM hParentItem = NULL;

	hSelectedItem = this->m_ctcCategories.GetSelectedItem();
	this->m_ctcCategories.SetItemState(hSelectedItem, TVIS_BOLD, TVIS_BOLD);
	hParentItem = this->m_ctcCategories.GetParentItem(hSelectedItem);

	// Do the bold/nobold thing
	if (pNMTreeView != NULL)
	{
		HTREEITEM hOldItem = pNMTreeView->itemOld.hItem;
		if (hOldItem != NULL)
			this->m_ctcCategories.SetItemState(hOldItem, 0, TVIS_BOLD);
	}

	m_iCurState = 1;
	if (m_clcItems.GetView() != 0)
		m_clcItems.SetView(1);

	m_clcItems.SetHotItem(-1);
	m_clcItems.DeleteAllItems();
	m_csItemID.SetWindowText("");
	m_csItemIDDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);

	if (hParentItem != NULL)
	{
		CString csParent = this->m_ctcCategories.GetItemText(hParentItem);
		CString csSelected = this->m_ctcCategories.GetItemText(hSelectedItem);
		CCategory * pCategory = FindCategory(&Main->m_pScripts->m_olItems, csParent);
		if (pCategory == NULL)
			return;
		CSubsection * pSubsection = FindSubsection(pCategory, csSelected);
		if (!pSubsection->m_ItemList.IsEmpty())
		{
			for(int i=0; i<m_pImageList->GetImageCount(); i++)
			{
				m_pImageList->Remove(i); 
			}
			m_pImageList->SetImageCount((UINT)pSubsection->m_ItemList.GetCount());
			POSITION pos = pSubsection->m_ItemList.GetHeadPosition();
			int iCount = 0;
			while (pos != NULL)
			{
				CSObject * pObject = (CSObject *) pSubsection->m_ItemList.GetNext(pos);
				this->m_clcItems.InsertItem(iCount, pObject->m_csDescription, iCount);
				CString cs_ItemID = pObject->m_csID;
				cs_ItemID.MakeLower();
				this->m_clcItems.SetItemText(iCount, 1, cs_ItemID);
				this->m_clcItems.SetItemData(iCount, (DWORD_PTR)pObject);

				WORD wID = -1;
				if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
					wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
				WORD wColor = 0;
				if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
					wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);

				if (pObject->m_bType != TYPE_MULTI)
				{
					CBitmap *pTempBitmap = m_Icon.OnCreateIcon(icX,icY,wID,wColor);
					m_pImageList->Replace(iCount,pTempBitmap,NULL);
					delete pTempBitmap;
				}
				iCount++;
			}
		}
	}

	iNameSort = 1;
	iIDSort = 1;
	this->m_clcItems.SortItems(CompareFunc, 0);		// Sort by Description
	iNameSort = -1;
}

void CItemTab::OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		// this is the one that has been selected
		int iSelIndex = this->m_clcItems.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex == -1)
		{
			this->m_csItemID.SetWindowText("");
			m_csItemIDDec.SetWindowText("");
			m_Display.SetArtIndex(-1);
			m_Display.SetArtColor(0);
			return;
		}
		this->m_clcItems.SetHotItem(iSelIndex);
		CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(iSelIndex);
		if ( !pObject )
			return;
		if ( pObject->m_csFilename == "" )
			return;

		CString cs_ItemID = pObject->m_csID;
		cs_ItemID.MakeLower();
		this->m_csItemID.SetWindowText(cs_ItemID);

		DWORD wDisp = -1;
		if ( alltoi(pObject->m_csID,pObject->m_csValue) != 0 )
			wDisp = (DWORD) alltoi(pObject->m_csID,pObject->m_csValue);
		if (wDisp == 0xFFFFFFFF)
		{
			if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
			wDisp = (DWORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
		}
		DWORD wDispCol = 0;
		if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
			wDispCol = (DWORD) alltoi(pObject->m_csColor,pObject->m_csValue);
		CString cs_ItemIDDec;
		cs_ItemIDDec.Format("ID:0x%X    Hue:0x%X",wDisp,wDispCol);
		this->m_csItemIDDec.SetWindowText(cs_ItemIDDec);

		if (Main->m_dwShowItems)
		{
			WORD wID = -1;
			if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
				wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);

			if (pObject->m_bType == TYPE_MULTI)
				m_Display.SetArtType(0);
			else
				m_Display.SetArtType(1);

			m_Display.SetArtIndex(wID);

			WORD wColor = 0;
			if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
				wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);
			
			m_Display.SetArtColor(wColor);
		}

		return;
	}
	this->m_csItemID.SetWindowText("");
	m_csItemIDDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);
	*pResult = 0;
}

void CItemTab::OnColumnclickItems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	this->m_clcItems.SortItems(CompareFunc, pNMListView->iSubItem);
	if (pNMListView->iSubItem == 0)
	{
		switch(iNameSort)
		{
		case -1:
			iNameSort = 1;
			break;
		case 0:
			iNameSort = 1;
			break;
		case 1:
			iNameSort = -1;
			break;
		}
	}
	if (pNMListView->iSubItem == 1)
	{
		switch(iIDSort)
		{
		case -1:
			iIDSort = 1;
			break;
		case 0:
			iIDSort = 1;
			break;
		case 1:
			iIDSort = -1;
			break;
		}
	}
	*pResult = 0;
}

void CItemTab::OnDblclkItems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	int iSelIndex = this->m_clcItems.GetNextItem(-1, LVNI_SELECTED);
	if (iSelIndex == -1)
	{
		this->m_csItemID.SetWindowText("");
		m_csItemIDDec.SetWindowText("");
		m_Display.SetArtIndex(-1);
		m_Display.SetArtColor(0);
		return;
	}
	CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(iSelIndex);
	if ( !pObject )
		return;

	this->OnCreate();

	*pResult = 0;
}


BOOL CItemTab::OnSetActive() 
{
	UpdateData();
	if ( Main->m_pScripts->m_iICatSeq != m_iCatSeq )
		this->FillCategoryTree();
	WORD wFlags = 0;
	if ( Main->m_dwRoomView )
		wFlags |= F_ROOMVIEW;
	//wFlags |= F_BG_GRASS;
	m_Display.SetDrawFlags(wFlags);
	m_Display.SetBkColor(Main->m_dwItemBGColor);

	return CDockingPage::OnSetActive();
}

int CALLBACK CItemTab::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSObject * pObject1 = (CSObject *) lParam1;
	CSObject * pObject2 = (CSObject *) lParam2;

	if ( pObject1 && pObject2 )
	{
		if (lParamSort == 0)
		{
			if (iNameSort == 0)
				return (lParam1 < lParam2 ? 1 : -1);
			if (iNameSort == 1)
			{
				if (pObject1->m_csDescription < pObject2->m_csDescription)
					return -1;
				else if (pObject1->m_csDescription > pObject2->m_csDescription)
					return 1;
				else
					return 0;
			}
			if (iNameSort == -1)
			{
				if (pObject1->m_csDescription < pObject2->m_csDescription)
					return 1;
				else if (pObject1->m_csDescription > pObject2->m_csDescription)
					return -1;
				else
					return 0;
			}
		}
		else
		{
			if (iIDSort == 0)
				return (lParam1 < lParam2 ? 1 : -1);
			if (iIDSort == 1)
				return (pObject1->m_csID < pObject2->m_csID ? -1 : 1);
			if (iIDSort == -1)
				return (pObject1->m_csID < pObject2->m_csID ? 1 : -1);
		}
	}
	return 0;
}

void CItemTab::OnFinditem() 
{
	CSearchCritDlg dlg;
	if (m_csSearchValue != "")
		dlg.m_csValue = m_csSearchValue;
	if ( dlg.DoModal() != IDOK )
		return;
	if (dlg.m_csValue != "")
		Find( dlg.m_csValue, dlg.m_crPref, false );
	return;
}

void CItemTab::Find(CString csString, int iCrit, bool bPrevious)
{
	CString csCategory,csSubsection;
	if ( csString != "" )
	{
		// this is a new search
		hCat = this->m_ctcCategories.GetRootItem();
		hSub = this->m_ctcCategories.GetChildItem( hCat );
		csCategory = this->m_ctcCategories.GetItemText(hCat);
		csSubsection = this->m_ctcCategories.GetItemText(hSub);
		pCategory = FindCategory(&Main->m_pScripts->m_olItems, csCategory);
		pSubsection = FindSubsection(pCategory, csSubsection);
		pos = pSubsection->m_ItemList.GetHeadPosition();
		csString.MakeLower();
		this->m_csSearchValue = csString;
		this->m_iSearchCrit = iCrit;
	}

	while ( hCat != NULL )
	{
		if (pCategory != NULL)
		{
			while ( hSub != NULL )
			{
				if (pSubsection != NULL)
				{
					if (!pSubsection->m_ItemList.IsEmpty())
					{
						while (pos != NULL)
						{
							CSObject * pItem = NULL;
							if ( !bPrevious )
								pItem = (CSObject *) pSubsection->m_ItemList.GetNext(pos);
							else
								pItem = (CSObject *) pSubsection->m_ItemList.GetPrev(pos);
							CString csValue;
							if (this->m_iSearchCrit == 2)
								csValue = pItem->m_csDescription;
							else if (this->m_iSearchCrit == 1)
								csValue = pItem->m_csID;
							else
								csValue = pItem->m_csValue;
							csValue.MakeLower();
							CSObject * pSelection = NULL;


							if ( csValue.Find( this->m_csSearchValue ) != -1 )
								pSelection = pItem;

							if ( pSelection != NULL )
							{
								this->m_ctcCategories.SelectItem( hSub );
								for ( int i = 0; i < this->m_clcItems.GetItemCount(); i++ )
								{
									CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(i);
									if ( pObject == pSelection )
									{
										this->m_clcItems.SetItemState( i, LVIS_SELECTED, LVIS_SELECTED );
										this->m_clcItems.EnsureVisible( i, FALSE );
										return;
									}
								}
								return;
							}
						}
					}
				}
				if ( !bPrevious )
				{
					hSub = this->m_ctcCategories.GetNextItem( hSub, TVGN_NEXT );
					csSubsection = this->m_ctcCategories.GetItemText(hSub);
					pSubsection = FindSubsection(pCategory, csSubsection);
					pos = pSubsection->m_ItemList.GetHeadPosition();
				}
				else
				{
					hSub = this->m_ctcCategories.GetNextItem( hSub, TVGN_PREVIOUS );
					csSubsection = this->m_ctcCategories.GetItemText(hSub);
					pSubsection = FindSubsection(pCategory, csSubsection);
					pos = pSubsection->m_ItemList.GetTailPosition();
				}
			}
		}
		if ( !bPrevious )
		{
			if (!this->m_ctcCategories.GetNextItem( hCat, TVGN_NEXT ))
			{
				AfxMessageBox("Reached Bottom of list", MB_OK | MB_ICONINFORMATION);
				return;
			}
			hCat = this->m_ctcCategories.GetNextItem( hCat, TVGN_NEXT );
			hSub = this->m_ctcCategories.GetChildItem( hCat );
			csCategory = this->m_ctcCategories.GetItemText(hCat);
			pCategory = FindCategory(&Main->m_pScripts->m_olItems, csCategory);
			csSubsection = this->m_ctcCategories.GetItemText(hSub);
			pSubsection = FindSubsection(pCategory, csSubsection);
			pos = pSubsection->m_ItemList.GetHeadPosition();
		}
		else
		{
			if (!this->m_ctcCategories.GetNextItem( hCat, TVGN_PREVIOUS ))
			{
				AfxMessageBox("Reached top of list", MB_OK | MB_ICONINFORMATION);
				return;
			}
			hCat = this->m_ctcCategories.GetNextItem( hCat, TVGN_PREVIOUS );
			hSub = this->m_ctcCategories.GetChildItem( hCat );
			while (this->m_ctcCategories.GetNextItem( hSub, TVGN_NEXT ) != NULL)
			{
				hSub = this->m_ctcCategories.GetNextItem( hSub, TVGN_NEXT );
			}
			csCategory = this->m_ctcCategories.GetItemText(hCat);
			pCategory = FindCategory(&Main->m_pScripts->m_olItems, csCategory);
			csSubsection = this->m_ctcCategories.GetItemText(hSub);
			pSubsection = FindSubsection(pCategory, csSubsection);
			pos = pSubsection->m_ItemList.GetTailPosition();
		}
	}
	AfxMessageBox("No matching item was found", MB_OK | MB_ICONINFORMATION);
}

void CItemTab::FindPrevious()
{
	Find("", m_iSearchCrit, true);
	return;
}

BOOL CItemTab::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_F3 )
		{
			if ( GetAsyncKeyState(VK_SHIFT) )
				FindPrevious();
			else
				Find();
			return TRUE;
		}
	}
	return CDockingPage::PreTranslateMessage(pMsg);
}
void CItemTab::OnRclickItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pInfo = (NMITEMACTIVATE *)pNMHDR;
	CRect rListCtrl;
	m_clcItems.GetWindowRect(&rListCtrl);

	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		if (m_clcItems.GetView() == 0)
			::AppendMenu(hMenu, MF_STRING|MF_USECHECKBITMAPS|MF_CHECKED, 2, "Overview Mode");
		else
			::AppendMenu(hMenu, MF_STRING|MF_USECHECKBITMAPS|MF_UNCHECKED, 1, "Overview Mode");

		if (m_iCurState == 1)
			::AppendMenu(hMenu, MF_STRING|MF_USECHECKBITMAPS|MF_UNCHECKED, 3, "Show Quicklist");
		else
			::AppendMenu(hMenu, MF_STRING|MF_USECHECKBITMAPS|MF_CHECKED, 4, "Show Quicklist");

		if (pInfo->iItem != -1)
		{
			::AppendMenu(hMenu, MF_MENUBREAK, NULL, NULL);
			::AppendMenu(hMenu, MF_STRING, 7, "View Item Info");
			::AppendMenu(hMenu, MF_STRING, 8, "Edit Item Script");
			::AppendMenu(hMenu, MF_MENUBREAK, NULL, NULL);
			if (m_iCurState != 2)
				::AppendMenu(hMenu, MF_STRING, 5, "Add to Quicklist");
			else
				::AppendMenu(hMenu, MF_STRING, 6, "Remove from Quicklist");
		}
		int sel = ::TrackPopupMenuEx(hMenu, 
				TPM_CENTERALIGN | TPM_RETURNCMD,
				pInfo->ptAction.x + rListCtrl.left,
				pInfo->ptAction.y + rListCtrl.top,
				m_hWnd,
				NULL);
		switch (sel)
		{
		case 1:
			{
				m_iCurState = m_clcItems.GetView();
				m_clcItems.SetView(0);
			break;
			}
		case 2:
			{
				m_clcItems.SetView(m_iCurState);
				if (m_iCurState == 2)
					m_clcItems.SetColumnWidth(0,150);
			break;
			}
		case 3:
			{
				m_iCurState = 2;
				if (m_clcItems.GetView() != 0)
				{
					m_clcItems.SetView(2);
					m_clcItems.SetColumnWidth(0,150);
				}
				OnShowQuicklist();
			break;
			}
		case 4:
			{
				m_iCurState = 1;
				if (m_clcItems.GetView() != 0)
					m_clcItems.SetView(1);
				OnExitQuicklist();
			break;
			}
		case 5:
			{
				CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				if (Main->m_pScripts->m_ItemQuickList.Find(pObject->m_csValue) != -1)
				{
					CString csMessage;
					csMessage.Format("%s is already in the Quicklist",pObject->m_csDescription );
					AfxMessageBox(csMessage);
					return;
				}
				Main->m_pScripts->m_ItemQuickList.Insert(pObject);
				OnSaveQuicklist("Items"); 
			break;
			}
		case 6:
			{
				CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				Main->m_pScripts->m_ItemQuickList.RemoveAt(Main->m_pScripts->m_ItemQuickList.Find(pObject->m_csValue));
				this->m_clcItems.DeleteItem(pInfo->iItem);
				iNameSort = 1;
				iIDSort = 1;
				this->m_clcItems.SortItems(CompareFunc, 0);
				iNameSort = -1;
				OnSaveQuicklist("Items");
			break;
			}
		case 7:
			{
				CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				CScriptInfo * dlg = new CScriptInfo;
				dlg->pObject = pObject;
				dlg->Create(IDD_SCRIPT_INFO);
			break;
			}
		case 8:
			{
				CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				CScriptEditor * dlg = new CScriptEditor;
				dlg->pObject = pObject;
				dlg->Create(IDD_SCRIPT_EDITOR);
			break;
			}
		}
		::DestroyMenu(hMenu);
	}


	*pResult = 0;
}

void CItemTab::OnSaveQuicklist(CString csList) 
{
	CStringArray aQuicklist;
	for(int iCount = 0; iCount < Main->m_pScripts->m_ItemQuickList.GetCount(); iCount++)
	{
		CSObject * pObject = (CSObject *) Main->m_pScripts->m_ItemQuickList.GetAt(iCount);
		aQuicklist.Add(pObject->m_csValue);
		CString csProfileKey;
		csProfileKey.Format("%s\\%s\\Quicklist",REGKEY_PROFILE, Main->m_csCurrentProfile);
		Main->PutRegistryMultiSz(csList, &aQuicklist, hRegLocation, csProfileKey);
	}
}

void CItemTab::OnShowQuicklist() 
{
	m_clcItems.SetHotItem(-1);
	m_clcItems.DeleteAllItems();
	m_csItemID.SetWindowText("");
	m_csItemIDDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);

	for(int i=0; i<m_pImageList->GetImageCount(); i++)
	{
		m_pImageList->Remove(i); 
	}
	m_pImageList->SetImageCount((UINT)Main->m_pScripts->m_ItemQuickList.GetCount());
	for(int iCount = 0; iCount < Main->m_pScripts->m_ItemQuickList.GetCount(); iCount++)
	{
		CSObject * pObject = (CSObject *) Main->m_pScripts->m_ItemQuickList.GetAt(iCount);
		this->m_clcItems.InsertItem(iCount, pObject->m_csDescription, iCount);
		this->m_clcItems.SetItemData(iCount, (DWORD_PTR)pObject);

		WORD wID = -1;
		if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
			wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
		WORD wColor = 0;
		if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
			wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);

		CBitmap *pTempBitmap = m_Icon.OnCreateIcon(icX,icY,wID,wColor);
		m_pImageList->Replace(iCount,pTempBitmap,NULL);
		delete pTempBitmap;
	}

	iNameSort = 1;
	iIDSort = 1;
	this->m_clcItems.SortItems(CompareFunc, 0);		// Sort by Description
	iNameSort = -1;
}

void CItemTab::OnExitQuicklist() 
{
	HTREEITEM hSelectedItem = NULL;
	HTREEITEM hParentItem = NULL;

	m_clcItems.SetHotItem(-1);
	m_clcItems.DeleteAllItems();
	m_csItemID.SetWindowText("");
	m_csItemIDDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);

	hSelectedItem = this->m_ctcCategories.GetSelectedItem();
	hParentItem = this->m_ctcCategories.GetParentItem(hSelectedItem);

	if (hParentItem != NULL)
	{
		CString csParent = this->m_ctcCategories.GetItemText(hParentItem);
		CString csSelected = this->m_ctcCategories.GetItemText(hSelectedItem);
		CCategory * pCategory = FindCategory(&Main->m_pScripts->m_olItems, csParent);
		if (pCategory == NULL)
			return;
		CSubsection * pSubsection = FindSubsection(pCategory, csSelected);
		if (!pSubsection->m_ItemList.IsEmpty())
		{
			for(int i=0; i<m_pImageList->GetImageCount(); i++)
			{
				m_pImageList->Remove(i); 
			}
			m_pImageList->SetImageCount((UINT)pSubsection->m_ItemList.GetCount());
			POSITION pos = pSubsection->m_ItemList.GetHeadPosition();
			int iCount = 0;
			while (pos != NULL)
			{
				CSObject * pObject = (CSObject *) pSubsection->m_ItemList.GetNext(pos);
				this->m_clcItems.InsertItem(iCount, pObject->m_csDescription, iCount);
				CString cs_ItemID = pObject->m_csID;
				cs_ItemID.MakeLower();
				this->m_clcItems.SetItemText(iCount, 1, cs_ItemID);
				this->m_clcItems.SetItemData(iCount, (DWORD_PTR)pObject);

				WORD wID = -1;
				if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
					wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
				WORD wColor = 0;
				if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
					wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);

				CBitmap *pTempBitmap = m_Icon.OnCreateIcon(icX,icY,wID,wColor);
				m_pImageList->Replace(iCount,pTempBitmap,NULL);
				delete pTempBitmap;
				iCount++;
			}
		}
	}

	iNameSort = 1;
	iIDSort = 1;
	this->m_clcItems.SortItems(CompareFunc, 0);
	iNameSort = -1;
}