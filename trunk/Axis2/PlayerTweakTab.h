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

#if !defined(AFX_PLAYERTWEAKTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
#define AFX_PLAYERTWEAKTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerTweakTab.h : header file
//

#include "DockingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayerTweakTab dialog

class CPlayerTweakTab : public CDockingPage
{
	DECLARE_DYNCREATE(CPlayerTweakTab)

// Construction
public:
	CPlayerTweakTab();
	virtual ~CPlayerTweakTab();

// Dialog Data
	//{{AFX_DATA(CPlayerTweakTab)
	enum { IDD = IDD_PLAYERTWEAK_TAB };
	CStatic	m_csFlagValue;
	BOOL	m_bInvulnerable;
	BOOL	m_bDead;
	BOOL	m_bFreeze;
	BOOL	m_bInvisible;
	BOOL	m_bSleeping;
	BOOL	m_bWarmode;
	BOOL	m_bReactiveArmor;
	BOOL	m_bPoisoned;
	BOOL	m_bNightsight;
	BOOL	m_bMagicReflect;
	BOOL	m_bPolymorphed;
	BOOL	m_bIncognito;
	BOOL	m_bSpiritSpeak;
	BOOL	m_bInsubstantial;
	BOOL	m_bEmote;
	BOOL	m_bCommCrystal;
	BOOL	m_bHasShield;
	BOOL	m_bArcherCanmove;
	BOOL	m_bStone;
	BOOL	m_bBlank1;
	BOOL	m_bFlying;
	BOOL	m_bBlank2;
	BOOL	m_bHallucinating;
	BOOL	m_bHidden;
	BOOL	m_bInDoors;
	BOOL	m_bCriminal;
	BOOL	m_bConjured;
	BOOL	m_bPet;
	BOOL	m_bSpawned;
	BOOL	m_bSaveParity;
	BOOL	m_bRidden;
	BOOL	m_bMounted;

	CComboBox	m_ccbBrain;
	CComboBox	m_ccbStat;
	CEdit	m_ceStatValue;
	CComboBox	m_ccbSkill;
	CEdit	m_ceSkillValue;
	CComboBox	m_ccbResistence;
	CEdit	m_ceResistenceValue;
	CComboBox	m_ccbEvents;
	CComboBox	m_ccbMisc;
	CEdit	m_ceMiscValue;
	CComboBox	m_ccbTags;
	CEdit	m_ceTagsValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPlayerTweakTab)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int iPrivLevel;
	// Generated message map functions
	//{{AFX_MSG(CPlayerTweakTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnResetflags();
	afx_msg void OnSetflags();

	afx_msg void OnInvulnerable();
	afx_msg void OnDead();
	afx_msg void OnFreeze();
	afx_msg void OnInvisible();
	afx_msg void OnSleep();
	afx_msg void OnWarmode();
	afx_msg void OnReactiveArmor();
	afx_msg void OnPoisoned();
	afx_msg void OnNightsight();
	afx_msg void OnMagicReflect();
	afx_msg void OnPolymorphed();
	afx_msg void OnIncognito();
	afx_msg void OnSpiritspeak();
	afx_msg void OnInsubstantial();
	afx_msg void OnEmote();
	afx_msg void OnCommcrystal();
	afx_msg void OnHasShield();
	afx_msg void OnArcherCanMove();
	afx_msg void OnStone();
	afx_msg void OnBlank1();
	afx_msg void OnFlying();
	afx_msg void OnBlank2();
	afx_msg void OnHallucinating();
	afx_msg void OnHidden();
	afx_msg void OnIndoors();
	afx_msg void OnCriminal();
	afx_msg void OnConjured();
	afx_msg void OnPet();
	afx_msg void OnSpawned();
	afx_msg void OnSaveparity();
	afx_msg void OnRidden();
	afx_msg void OnMounted();

	afx_msg void OnSetbrain();
	afx_msg void OnSetskill();
	afx_msg void OnSetstat();
	afx_msg void OnSetresistence();
	afx_msg void OnSetevent();
	afx_msg void OnRemoveevent();
	afx_msg void OnSetmisc();
	afx_msg void OnSettag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PlayerTweakTAB_H__37A80987_1A71_4552_A7A6_7284EE775CAD__INCLUDED_)
