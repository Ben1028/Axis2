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

// CSpawnTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "SpawnTab.h"
#include "UOart.h"
#include "Common.h"
#include "SearchCritDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpawnTab property page

IMPLEMENT_DYNCREATE(CSpawnTab, CDockingPage)

int CSpawnTab::iIDSort = 1;
int CSpawnTab::iNameSort = 1;

CSpawnTab::CSpawnTab() : CDockingPage(CSpawnTab::IDD,CMsg("IDS_SPAWN"))
{
	//{{AFX_DATA_INIT(CSpawnTab)
	m_iCatSeq = 0;
	icX = 60;
	icY = 110;
	//}}AFX_DATA_INIT
}

CSpawnTab::~CSpawnTab()
{
}

void CSpawnTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpawnTab)
	DDX_Control(pDX, IDC_CREATURELIST, m_clcCreatures);
	DDX_Control(pDX, IDC_NPCTREE, m_ctcNPC);
	DDX_Control(pDX, IDC_HOMEDIST, m_ceHomedist);
	DDX_Control(pDX, IDC_NPC_ID, m_csNPCId);
	DDX_Control(pDX, IDC_NPC_IDDEC, m_csNPCIdDec);
	DDX_Control(pDX, IDC_MIN_TIME, m_ceMinTime);
	DDX_Control(pDX, IDC_MAX_TIME, m_ceMaxTime);
	DDX_Control(pDX, IDC_MAX_DIST, m_ceMaxDist);
	DDX_Control(pDX, IDC_AMOUNT, m_ceAmount);
	DDX_Control(pDX, IDC_NPCDISPLAY, m_Display);
	DDX_Control(pDX, IDC_FRAME_SELECT, m_FrameSelect);
	DDX_Control(pDX, IDC_FINDNPC, cb_findnpc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpawnTab, CDockingPage)
	//{{AFX_MSG_MAP(CSpawnTab)
	ON_NOTIFY(TVN_SELCHANGED, IDC_NPCTREE, OnSelchangedNpctree)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CREATURELIST, OnColumnclickCreatures)
	ON_NOTIFY(NM_DBLCLK, IDC_CREATURELIST, OnDblclkCreatures)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CREATURELIST, OnItemchangedCreatures)
	ON_NOTIFY(NM_RCLICK, IDC_CREATURELIST, OnRclickCreatures)
	ON_BN_CLICKED(IDC_SUMMON, OnSummon)
	ON_BN_CLICKED(IDC_PLACE_SPAWNPOINT, OnPlacespawn)
	ON_BN_CLICKED(IDC_INIT_SPAWNPOINT, OnInitspawn)
	ON_BN_CLICKED(IDC_SETHOME, OnSethome)
	ON_BN_CLICKED(IDC_SETHOMEDIST, OnSethomedist)
	ON_BN_CLICKED(IDC_SPREMOVE, OnSpremove)
	ON_BN_CLICKED(IDC_SPFREEZE, OnSpfreeze)
	ON_BN_CLICKED(IDC_SHRINK, OnShrink)
	ON_EN_CHANGE(IDC_HOMEDIST, OnChangeParams)
	ON_EN_CHANGE(IDC_AMOUNT, OnChangeParams)
	ON_EN_CHANGE(IDC_MIN_TIME, OnChangeParams)
	ON_EN_CHANGE(IDC_MAX_TIME, OnChangeParams)
	ON_EN_CHANGE(IDC_MAX_DIST, OnChangeParams)
	ON_BN_CLICKED(IDC_FINDNPC, OnFindnpc)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpawnTab message handlers

BOOL CSpawnTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppSpawnTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CSpawnTab;
	Main->m_pcppSpawnTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	CString csValue;
	csValue = Main->GetRegistryString("NPCSpawnAmount", "1");
	m_ceAmount.SetWindowText(csValue);
	csValue = Main->GetRegistryString("NPCHomedist", "5");
	m_ceHomedist.SetWindowText(csValue);
	csValue = Main->GetRegistryString("NPCSpawnMaxDist", "0");
	m_ceMaxDist.SetWindowText(csValue);
	csValue = Main->GetRegistryString("NPCSpawnMaxTime", "20");
	m_ceMaxTime.SetWindowText(csValue);
	csValue = Main->GetRegistryString("NPCSpawnMinTime", "5");
	m_ceMinTime.SetWindowText(csValue);;

	this->m_clcCreatures.InsertColumn(0, "NPC Type", LVCFMT_LEFT, 120, -1);
	this->m_clcCreatures.InsertColumn(1, "ID", LVCFMT_LEFT, 120, -1);
	FillCategoryTree();

	m_pImageList = new CImageList();
	m_pImageList->Create(icX, icY, ILC_COLORDDB, 0, 1);
	m_clcCreatures.SetImageList(m_pImageList, LVSIL_NORMAL);
	m_iCurState = m_clcCreatures.GetView();

	m_Display.SetArtType(3);
	m_Display.SetArtIndex(-1);
	m_Display.SetBkColor(Main->m_dwSpawnBGColor);

	m_FrameSelect.SetRange(-4, 3);
	m_FrameSelect.SetPos(-1);
	m_Display.m_wFrame = -1;

	cb_findnpc.InitButton(IDI_FIND,CMsg("IDS_FINDNPC"));
	cb_findnpc.SetBGColor(Main->crAxisBkg);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpawnTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_FrameSelect.m_hWnd)
	{
		m_Display.m_wFrame = m_FrameSelect.GetPos();
		m_Display.ReloadArt();
		return;
	}
	CDockingPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSpawnTab::OnSummon() 
{
	CString csCmd, csID;
	m_csNPCId.GetWindowText(csID);

	int iSelIndex = this->m_clcCreatures.GetNextItem(-1, LVNI_SELECTED);
	if (iSelIndex == -1)
	{
		this->m_csNPCId.SetWindowText("");
		m_csNPCIdDec.SetWindowText("");
		m_Display.SetArtIndex(-1);
		m_Display.SetArtColor(0);
		AfxMessageBox("No creature is selected.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	this->m_clcCreatures.SetHotItem(iSelIndex);
	CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(iSelIndex);
	if ( pObject->m_bType == TYPE_SPAWN )
	{
		AfxMessageBox("Spawn Groups cannot be summoned.  They must be spawned.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if (IsNumber(csID))
		csCmd.Format("%saddnpc %s", Main->m_csCommandPrefix, csID);
	else
		csCmd.Format("%sadd %s", Main->m_csCommandPrefix, csID);
	SendToUO(csCmd);
}


void CSpawnTab::OnSethome() 
{
	CString csCmd;
	csCmd.Format("%sset home", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}


void CSpawnTab::OnSethomedist() 
{
	CString csVal;
	m_ceHomedist.GetWindowText(csVal);
	CString csCmd;
	csCmd.Format("%sset homedist %s", Main->m_csCommandPrefix, csVal);
	SendToUO(csCmd);
}


void CSpawnTab::OnShrink() 
{
	CString csCmd;
	csCmd.Format("%sshrink", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}


void CSpawnTab::OnSpremove() 
{
	CString csCmd;
	csCmd.Format("%sremove", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}


void CSpawnTab::OnSpfreeze() 
{
	CString csCmd;
	csCmd.Format("%sset flags 4", Main->m_csCommandPrefix);
	SendToUO(csCmd);	
}


void CSpawnTab::OnPlacespawn() 
{
	CString csCmd;
	csCmd.Format("%sadd 01ea7", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}

void CSpawnTab::OnInitspawn() 
{
	CWaitCursor hourglass;
	CString csAmount, csMinTime, csMaxTime, csMaxDist, csID;
	this->m_ceAmount.GetWindowText(csAmount);
	this->m_ceMaxDist.GetWindowText(csMaxDist);
	this->m_ceMinTime.GetWindowText(csMinTime);
	this->m_ceMaxTime.GetWindowText(csMaxTime);
	this->m_csNPCId.GetWindowText(csID);

	if (csID == "")
	{
		AfxMessageBox("No creature is selected.", MB_OK | MB_ICONEXCLAMATION);
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
	csCmd.Format("%sact.type %ld", Main->m_csCommandPrefix, ITEM_SPAWN_CHAR);
	SendToUO(csCmd);

	csCmd.Format("%sact.amount %ld", Main->m_csCommandPrefix, iAmount);
	SendToUO(csCmd);

	csCmd.Format("%sact.more %s", Main->m_csCommandPrefix, csID);
	SendToUO(csCmd);

	csCmd.Format("%sact.morep %ld %ld %ld", Main->m_csCommandPrefix, iMinTime, iMaxTime, iMaxDist);
	SendToUO(csCmd);

	csCmd.Format("%sact.attr %04x", Main->m_csCommandPrefix, ATTR_INVIS | ATTR_MAGIC | ATTR_MOVE_NEVER);
	SendToUO(csCmd);

	csCmd.Format("%sact.timer 1", Main->m_csCommandPrefix);
	SendToUO(csCmd);
}


void CSpawnTab::OnChangeParams() 
{
	UpdateData();
	if ( !this->IsWindowVisible() )
		return;

	CString csAmount, csMinTime, csMaxTime, csHomedist, csDist;

	m_ceAmount.GetWindowText(csAmount);
	m_ceHomedist.GetWindowText(csHomedist);
	m_ceMaxDist.GetWindowText(csDist);
	m_ceMaxTime.GetWindowText(csMaxTime);
	m_ceMinTime.GetWindowText(csMinTime);

	Main->PutRegistryString("NPCSpawnAmount", csAmount);
	Main->PutRegistryString("NPCHomedist", csHomedist);
	Main->PutRegistryString("NPCSpawnMaxDist", csDist);
	Main->PutRegistryString("NPCSpawnMaxTime", csMaxTime);
	Main->PutRegistryString("NPCSpawnMinTime", csMinTime);

}


//***************************************************

void CSpawnTab::FillCategoryTree()
{
	HTREEITEM CategoryParent;
	HTREEITEM SubsectionParent;
	TV_INSERTSTRUCT InsertItem;

	m_clcCreatures.DeleteAllItems();
	m_ctcNPC.DeleteAllItems();
	if (!Main->m_pScripts->m_olNPCs.IsEmpty())
	{
		POSITION pos = Main->m_pScripts->m_olNPCs.GetHeadPosition();
		while (pos != NULL)
		{
			CCategory * pCategory = (CCategory *) Main->m_pScripts->m_olNPCs.GetNext(pos);
			InsertItem.item.mask = TVIF_TEXT;
			InsertItem.item.pszText = (char *)LPCTSTR(pCategory->m_csName);
			InsertItem.item.cchTextMax = pCategory->m_csName.GetLength();
			InsertItem.hParent = NULL;
			InsertItem.hInsertAfter = TVI_SORT;
			CategoryParent = this->m_ctcNPC.InsertItem(&InsertItem);
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
					SubsectionParent = this->m_ctcNPC.InsertItem(&InsertItem);
				}
			}
		}
	}
	m_iCatSeq = Main->m_pScripts->m_iNCatSeq;
}


void CSpawnTab::OnSelchangedNpctree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pResult);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hSelectedItem = NULL;
	HTREEITEM hParentItem = NULL;

	hSelectedItem = this->m_ctcNPC.GetSelectedItem();
	this->m_ctcNPC.SetItemState(hSelectedItem, TVIS_BOLD, TVIS_BOLD);
	hParentItem = this->m_ctcNPC.GetParentItem(hSelectedItem);

	// Do the bold/nobold thing
	if (pNMTreeView != NULL)
	{
		HTREEITEM hOldItem = pNMTreeView->itemOld.hItem;
		if (hOldItem != NULL)
			this->m_ctcNPC.SetItemState(hOldItem, 0, TVIS_BOLD);
	}

	m_iCurState = 1;
	if (m_clcCreatures.GetView() != 0)
		m_clcCreatures.SetView(1);

	this->m_clcCreatures.SetHotItem(-1);
	this->m_clcCreatures.DeleteAllItems();
	this->m_csNPCId.SetWindowText("");
	m_csNPCIdDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);

	if (hParentItem != NULL)
	{
		CString csParent = this->m_ctcNPC.GetItemText(hParentItem);
		CString csSelected = this->m_ctcNPC.GetItemText(hSelectedItem);
		CCategory * pCategory = FindCategory(&Main->m_pScripts->m_olNPCs, csParent);
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
				this->m_clcCreatures.InsertItem(iCount, pObject->m_csDescription, iCount);
				CString cs_ItemID = pObject->m_csID;
				cs_ItemID.MakeLower();
				this->m_clcCreatures.SetItemText(iCount, 1, cs_ItemID);
				this->m_clcCreatures.SetItemData(iCount, (DWORD_PTR)pObject);

				WORD wID = (WORD)-1;
				if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
					wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
				WORD wColor = 0;
				if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
					wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);

				CBitmap *pTempBitmap = m_Icon.OnCreateIcon(icX,icY,wID,wColor,3);
				m_pImageList->Replace(iCount,pTempBitmap,NULL);
				delete pTempBitmap;

				iCount++;
			}
		}
	}

	iNameSort = 1;
	iIDSort = 1;
	this->m_clcCreatures.SortItems(CompareFunc, 0);		// Sort by Description
	iNameSort = -1;
}

void CSpawnTab::OnItemchangedCreatures(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		// this is the one that has been selected
		int iSelIndex = this->m_clcCreatures.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex == -1)
		{
			this->m_csNPCId.SetWindowText("");
			m_csNPCIdDec.SetWindowText("");
			m_Display.SetArtIndex(-1);
			m_Display.SetArtColor(0);
			return;
		}
		this->m_clcCreatures.SetHotItem(iSelIndex);
		CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(iSelIndex);
		if ( !pObject )
			return;
		if ( pObject->m_csFilename == "" )
			return;
		CString cs_NPCID = pObject->m_csID;
		cs_NPCID.MakeLower();
		this->m_csNPCId.SetWindowText(cs_NPCID);

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
		CString cs_NPCIDDec;
		cs_NPCIDDec.Format("ID:0x%X    Hue:0x%X",wDisp,wDispCol);
		this->m_csNPCIdDec.SetWindowText(cs_NPCIDDec);

		if (Main->m_dwShowNPCs)
		{
			WORD wID = -1;
			if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
				wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);

			m_Display.SetArtIndex(wID);

			WORD wColor = 0;
			if (alltoi(pObject->m_csColor,pObject->m_csValue)!= 0)
				wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);
			
			m_Display.SetArtColor(wColor);
		}

		return;
	}

	this->m_csNPCId.SetWindowText("");
	m_csNPCIdDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);
	*pResult = 0;
}

void CSpawnTab::OnColumnclickCreatures(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	this->m_clcCreatures.SortItems(CompareFunc, pNMListView->iSubItem);
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

void CSpawnTab::OnDblclkCreatures(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	int iSelIndex = this->m_clcCreatures.GetNextItem(-1, LVNI_SELECTED);
	if (iSelIndex == -1)
	{
		this->m_csNPCId.SetWindowText("");
		m_csNPCIdDec.SetWindowText("");
		m_Display.SetArtIndex(-1);
		m_Display.SetArtColor(0);
		return;
	}
	CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(iSelIndex);
	if ( !pObject )
		return;

	this->OnSummon();

	*pResult = 0;
}


BOOL CSpawnTab::OnSetActive() 
{
	UpdateData();
	if ( Main->m_pScripts->m_iNCatSeq != m_iCatSeq )
		this->FillCategoryTree();
	WORD wFlags = 0;
	//if ( Main->m_dwRoomView )
	//	wFlags |= F_ROOMVIEW;
	//wFlags |= F_BG_GRASS;
	m_Display.SetDrawFlags(wFlags);
	m_Display.SetBkColor(Main->m_dwSpawnBGColor);

	return CDockingPage::OnSetActive();
}

int CALLBACK CSpawnTab::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

void CSpawnTab::OnFindnpc() 
{
	CSearchCritDlg dlg;
	if (m_csnpcSearchValue != "")
		dlg.m_csValue = m_csnpcSearchValue;
	if ( dlg.DoModal() != IDOK )
		return;
	if (dlg.m_csValue != "")
		Find( dlg.m_csValue, dlg.m_crPref, false );
	return;
}

void CSpawnTab::Find(CString csString, int iCrit, bool bPrevious)
{
	CString csCategory,csSubsection;
	if ( csString != "" )
	{
		// this is a new search
		hnpcCat = this->m_ctcNPC.GetRootItem();
		hnpcSub = this->m_ctcNPC.GetChildItem( hnpcCat );
		csCategory = this->m_ctcNPC.GetItemText(hnpcCat);
		csSubsection = this->m_ctcNPC.GetItemText(hnpcSub);
		pnpcCategory = FindCategory(&Main->m_pScripts->m_olNPCs, csCategory);
		pnpcSubsection = FindSubsection(pnpcCategory, csSubsection);
		npcpos = pnpcSubsection->m_ItemList.GetHeadPosition();
		csString.MakeLower();
		this->m_csnpcSearchValue = csString;
		this->m_inpcSearchCrit = iCrit;
	}

	while ( hnpcCat != NULL )
	{
		if (pnpcCategory != NULL)
		{
			while ( hnpcSub != NULL )
			{
				if (pnpcSubsection != NULL)
				{
					if (!pnpcSubsection->m_ItemList.IsEmpty())
					{
						while (npcpos != NULL)
						{
							CSObject * pItem = NULL;
							if ( !bPrevious )
								pItem = (CSObject *) pnpcSubsection->m_ItemList.GetNext(npcpos);
							else
								pItem = (CSObject *) pnpcSubsection->m_ItemList.GetPrev(npcpos);
							CString csValue;
							if (this->m_inpcSearchCrit == 2)
								csValue = pItem->m_csDescription;
							else if (this->m_inpcSearchCrit == 1)
								csValue = pItem->m_csID;
							else
								csValue = pItem->m_csValue;
							csValue.MakeLower();
							CSObject * pSelection = NULL;
							if ( csValue.Find( this->m_csnpcSearchValue ) != -1 )
								pSelection = pItem;

							if ( pSelection != NULL )
							{
								this->m_ctcNPC.SelectItem( hnpcSub );
								for ( int i = 0; i < this->m_clcCreatures.GetItemCount(); i++ )
								{
									CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(i);
									if ( pObject == pSelection )
									{
										this->m_clcCreatures.SetItemState( i, LVIS_SELECTED, LVIS_SELECTED );
										this->m_clcCreatures.EnsureVisible( i, FALSE );
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
					hnpcSub = this->m_ctcNPC.GetNextItem( hnpcSub, TVGN_NEXT );
					csSubsection = this->m_ctcNPC.GetItemText(hnpcSub);
					pnpcSubsection = FindSubsection(pnpcCategory, csSubsection);
					npcpos = pnpcSubsection->m_ItemList.GetHeadPosition();
				}
				else
				{
					hnpcSub = this->m_ctcNPC.GetNextItem( hnpcSub, TVGN_PREVIOUS );
					csSubsection = this->m_ctcNPC.GetItemText(hnpcSub);
					pnpcSubsection = FindSubsection(pnpcCategory, csSubsection);
					npcpos = pnpcSubsection->m_ItemList.GetTailPosition();
				}
			}
		}
		if ( !bPrevious )
		{
			if (!this->m_ctcNPC.GetNextItem( hnpcCat, TVGN_NEXT ))
			{
				AfxMessageBox("Reached Bottom of list", MB_OK | MB_ICONINFORMATION);
				return;
			}
			hnpcCat = this->m_ctcNPC.GetNextItem( hnpcCat, TVGN_NEXT );
			hnpcSub = this->m_ctcNPC.GetChildItem( hnpcCat );
			csCategory = this->m_ctcNPC.GetItemText(hnpcCat);
			pnpcCategory = FindCategory(&Main->m_pScripts->m_olNPCs, csCategory);
			csSubsection = this->m_ctcNPC.GetItemText(hnpcSub);
			pnpcSubsection = FindSubsection(pnpcCategory, csSubsection);
			npcpos = pnpcSubsection->m_ItemList.GetHeadPosition();
		}
		else
		{
			if (!this->m_ctcNPC.GetNextItem( hnpcCat, TVGN_PREVIOUS ))
			{
				AfxMessageBox("Reached top of list", MB_OK | MB_ICONINFORMATION);
				return;
			}
			hnpcCat = this->m_ctcNPC.GetNextItem( hnpcCat, TVGN_PREVIOUS );
			hnpcSub = this->m_ctcNPC.GetChildItem( hnpcCat );
			while (this->m_ctcNPC.GetNextItem( hnpcSub, TVGN_NEXT ) != NULL)
			{
				hnpcSub = this->m_ctcNPC.GetNextItem( hnpcSub, TVGN_NEXT );
			}
			csCategory = this->m_ctcNPC.GetItemText(hnpcCat);
			pnpcCategory = FindCategory(&Main->m_pScripts->m_olNPCs, csCategory);
			csSubsection = this->m_ctcNPC.GetItemText(hnpcSub);
			pnpcSubsection = FindSubsection(pnpcCategory, csSubsection);
			npcpos = pnpcSubsection->m_ItemList.GetTailPosition();
		}
	}
	AfxMessageBox("No matching item was found", MB_OK | MB_ICONINFORMATION);
}

void CSpawnTab::FindPrevious()
{
	Find("", m_inpcSearchCrit, true);
	return;
}

BOOL CSpawnTab::PreTranslateMessage(MSG* pMsg) 
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

void CSpawnTab::OnRclickCreatures(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pInfo = (NMITEMACTIVATE *)pNMHDR;
	CRect rListCtrl;
	m_clcCreatures.GetWindowRect(&rListCtrl);

	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		if (m_clcCreatures.GetView() == 0)
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
			::AppendMenu(hMenu, MF_STRING, 7, "View NPC Info");
			::AppendMenu(hMenu, MF_STRING, 8, "Edit NPC Script");
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
				m_iCurState = m_clcCreatures.GetView();
				m_clcCreatures.SetView(0);
			break;
			}
		case 2:
			{
				m_clcCreatures.SetView(m_iCurState);
				if (m_iCurState == 2)
					m_clcCreatures.SetColumnWidth(0,150);
			break;
			}
		case 3:
			{
				m_iCurState = 2;
				if (m_clcCreatures.GetView() != 0)
				{
					m_clcCreatures.SetView(2);
					m_clcCreatures.SetColumnWidth(0,150);
				}
				OnShowQuicklist();
			break;
			}
		case 4:
			{
				m_iCurState = 1;
				if (m_clcCreatures.GetView() != 0)
					m_clcCreatures.SetView(1);
				OnExitQuicklist();
			break;
			}
		case 5:
			{
				CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				if (Main->m_pScripts->m_SpawnQuickList.Find(pObject->m_csValue) != -1)
				{
					CString csMessage;
					csMessage.Format("%s is already in the Quicklist",pObject->m_csDescription );
					AfxMessageBox(csMessage);
					return;
				}
				Main->m_pScripts->m_SpawnQuickList.Insert(pObject);
				OnSaveQuicklist("Spawns"); 
			break;
			}
		case 6:
			{
				CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				Main->m_pScripts->m_SpawnQuickList.RemoveAt(Main->m_pScripts->m_SpawnQuickList.Find(pObject->m_csValue));
				this->m_clcCreatures.DeleteItem(pInfo->iItem);
				iNameSort = 1;
				iIDSort = 1;
				this->m_clcCreatures.SortItems(CompareFunc, 0);
				iNameSort = -1;
				OnSaveQuicklist("Spawns");
			break;
			}
		case 7:
			{
				CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(pInfo->iItem);
				if ( !pObject )
					return;
				CScriptInfo * dlg = new CScriptInfo;
				dlg->pObject = pObject;
				dlg->Create(IDD_SCRIPT_INFO);
			break;
			}
		case 8:
			{
				CSObject * pObject = (CSObject *) this->m_clcCreatures.GetItemData(pInfo->iItem);
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

void CSpawnTab::OnSaveQuicklist(CString csList) 
{
	CStringArray aQuicklist;
	for(int iCount = 0; iCount < Main->m_pScripts->m_SpawnQuickList.GetCount(); iCount++)
	{
		CSObject * pObject = (CSObject *) Main->m_pScripts->m_SpawnQuickList.GetAt(iCount);
		aQuicklist.Add(pObject->m_csValue);
		CString csProfileKey;
		csProfileKey.Format("%s\\%s\\Quicklist",REGKEY_PROFILE, Main->m_csCurrentProfile);
		Main->PutRegistryMultiSz(csList, &aQuicklist, hRegLocation, csProfileKey);
	}
}

void CSpawnTab::OnShowQuicklist() 
{
	m_clcCreatures.SetHotItem(-1);
	m_clcCreatures.DeleteAllItems();
	m_csNPCId.SetWindowText("");
	m_csNPCIdDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);

	for(int i=0; i<m_pImageList->GetImageCount(); i++)
	{
		m_pImageList->Remove(i); 
	}
	m_pImageList->SetImageCount((UINT)Main->m_pScripts->m_SpawnQuickList.GetCount());
	for(int iCount = 0; iCount < Main->m_pScripts->m_SpawnQuickList.GetCount(); iCount++)
	{
		CSObject * pObject = (CSObject *) Main->m_pScripts->m_SpawnQuickList.GetAt(iCount);
		this->m_clcCreatures.InsertItem(iCount, pObject->m_csDescription, iCount);
		this->m_clcCreatures.SetItemData(iCount, (DWORD_PTR)pObject);

		WORD wID = -1;
		if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
			wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
		WORD wColor = 0;
		if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
			wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);

		CBitmap *pTempBitmap = m_Icon.OnCreateIcon(icX,icY,wID,wColor,3);
		m_pImageList->Replace(iCount,pTempBitmap,NULL);
		delete pTempBitmap;
	}

	iNameSort = 1;
	iIDSort = 1;
	this->m_clcCreatures.SortItems(CompareFunc, 0);		// Sort by Description
	iNameSort = -1;
}

void CSpawnTab::OnExitQuicklist() 
{
	HTREEITEM hSelectedItem = NULL;
	HTREEITEM hParentItem = NULL;

	m_clcCreatures.SetHotItem(-1);
	m_clcCreatures.DeleteAllItems();
	m_csNPCId.SetWindowText("");
	m_csNPCIdDec.SetWindowText("");
	m_Display.SetArtIndex(-1);
	m_Display.SetArtColor(0);

	hSelectedItem = this->m_ctcNPC.GetSelectedItem();
	hParentItem = this->m_ctcNPC.GetParentItem(hSelectedItem);

	if (hParentItem != NULL)
	{
		CString csParent = this->m_ctcNPC.GetItemText(hParentItem);
		CString csSelected = this->m_ctcNPC.GetItemText(hSelectedItem);
		CCategory * pCategory = FindCategory(&Main->m_pScripts->m_olNPCs, csParent);
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
				this->m_clcCreatures.InsertItem(iCount, pObject->m_csDescription, iCount);
				CString cs_ItemID = pObject->m_csID;
				cs_ItemID.MakeLower();
				this->m_clcCreatures.SetItemText(iCount, 1, cs_ItemID);
				this->m_clcCreatures.SetItemData(iCount, (DWORD_PTR)pObject);

				WORD wID = -1;
				if ( alltoi(pObject->m_csDisplay,pObject->m_csValue) != 0 )
					wID = (WORD) alltoi(pObject->m_csDisplay,pObject->m_csValue);
				WORD wColor = 0;
				if (alltoi(pObject->m_csColor,pObject->m_csValue)!=0)
					wColor = (WORD) alltoi(pObject->m_csColor,pObject->m_csValue);

				CBitmap *pTempBitmap = m_Icon.OnCreateIcon(icX,icY,wID,wColor,3);
				m_pImageList->Replace(iCount,pTempBitmap,NULL);
				delete pTempBitmap;
				iCount++;
			}
		}
	}

	iNameSort = 1;
	iIDSort = 1;
	this->m_clcCreatures.SortItems(CompareFunc, 0);
	iNameSort = -1;
}