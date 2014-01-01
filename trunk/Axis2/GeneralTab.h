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

#if !defined(AFX_GENERALTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_GENERALTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#include "RemoteConsoleDlg.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralTab.h : header file
//

#include "DockingPage.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CItemTab dialog

class CGeneralTab : public CDockingPage
{
	DECLARE_DYNCREATE(CGeneralTab)

// Construction
public:
	CGeneralTab();
	virtual ~CGeneralTab();
	bool m_bRemoteConsoleCreated;
	bool m_bRemoteConsoleVisible;
	CRemoteConsoleDlg m_rcDlg;
	void HitConsole() { this->OnRconsole(); };
	void OnSetLanguage();

// Dialog Data
	//{{AFX_DATA(CGeneralTab)
	enum { IDD = IDD_GENERAL_TAB };
	CStatic	m_csSliderValue;
	CSliderCtrl	m_cscSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralTab)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralTab)
	virtual BOOL OnInitDialog();

	afx_msg void OnRconsole();
	afx_msg void OnAdmin();
	afx_msg void OnIteminfo();
	afx_msg void OnSetedit();
	afx_msg void OnClients();
	afx_msg void OnServerinfo();
	afx_msg void OnVersion();
	afx_msg void OnLink();
	afx_msg void OnFlip();
	afx_msg void OnShrink();
	afx_msg void OnDupe();
	afx_msg void OnRemove();
	afx_msg void OnNuke();
	afx_msg void OnBuy();
	afx_msg void OnSell();
	afx_msg void OnStock();
	afx_msg void OnPurchases();
	afx_msg void OnSamples();
	afx_msg void OnXrestock();
	afx_msg void OnSnow();
	afx_msg void OnRain();
	afx_msg void OnDry();
	afx_msg void OnSetlight();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnInvulnerable();
	afx_msg void OnSetallmove();
	afx_msg void OnInvisible();
	afx_msg void OnFix();
	afx_msg void OnTele();
	afx_msg void OnSethearall();
	afx_msg void OnGmtoggle();
	afx_msg void OnSetdetail();
	afx_msg void OnNightsight();
	afx_msg void OnSetdebug();
	afx_msg void OnJail();
	afx_msg void OnForgive();
	afx_msg void OnKill();
	afx_msg void OnDisconnect();
	afx_msg void OnResurect();
	afx_msg void OnPageon();
	afx_msg void OnPagelist();
	afx_msg void OnPageplayer();
	afx_msg void OnPagedisconnect();
	afx_msg void OnPagekick();
	afx_msg void OnPageoff();
	afx_msg void OnPagequeue();
	afx_msg void OnPageorigin();
	afx_msg void OnPagejail();
	afx_msg void OnPagedelete();
	afx_msg void OnWorldsave();
	afx_msg void OnSavestatics();
	afx_msg void OnResync();
	afx_msg void OnRestockall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic cg_information;
	CButton cb_admin;
	CButton cb_info;
	CButton cb_edit;
	CButton cb_clients;
	CButton cb_serv_info;
	CButton cb_version;
	//***
	CStatic cg_misc;
	CButton cb_link;
	CButton cb_flip;
	CButton cb_shrink;
	CButton cb_dupe;
	CButton cb_remove;
	CButton cb_nuke;
	//***
	CStatic cg_vendors;
	CButton cb_buy;
	CButton cb_sell;
	CButton cb_inventory;
	CButton cb_purchases;
	CButton cb_samples;
	CButton cb_restock;
	//***
	CStatic cg_weather;
	CButton cb_snow;
	CButton cb_rain;
	CButton cb_dry;
	CButton cb_light_lvl;
	//***
	CStatic cg_self;
	CButton cb_invul;
	CButton cb_allmove;
	CButton cb_invis;
	CButton cb_fix;
	CButton cb_tele;
	CButton cb_hear_all;
	CButton cb_gm_toggle;
	CButton cb_details;
	CButton cb_nightsight;
	CButton cb_debug;
	//***
	CStatic cg_discipline;
	CButton cb_jail;
	CButton cb_forgive;
	CButton cb_kill;
	CButton cb_disconnect;
	CButton cb_resurrect;
	//***
	CStatic cg_gm_page;
	CButton cb_page_on;
	CButton cb_page_list;
	CButton cb_page_goplayer;
	CButton cb_page_disconnect;
	CButton cb_page_ban;
	CButton cb_page_off;
	CButton cb_page_queue;
	CButton cb_page_goorigine;
	CButton cb_page_jail;
	CButton cb_page_delete;
	//***
	CButton cb_save;
	CButton cb_save_statics;
	CButton cb_resync;
	CButton cb_restock_all;
	CButton	m_cbRemoteConsole;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GeneralTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
