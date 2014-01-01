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

// PlayerTweakTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "PlayerTweakTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemTab property page

IMPLEMENT_DYNCREATE(CPlayerTweakTab, CDockingPage)

CPlayerTweakTab::CPlayerTweakTab() : CDockingPage(CPlayerTweakTab::IDD,CMsg("IDS_PLAYER_TWEAK"))
{
	//{{AFX_DATA_INIT(CPlayerTweakTab)
	m_bInvulnerable = FALSE;
	m_bDead = FALSE;
	m_bFreeze = FALSE;
	m_bInvisible = FALSE;
	m_bSleeping = FALSE;
	m_bWarmode = FALSE;
	m_bReactiveArmor = FALSE;
	m_bPoisoned = FALSE;
	m_bNightsight = FALSE;
	m_bMagicReflect = FALSE;
	m_bPolymorphed = FALSE;
	m_bIncognito = FALSE;
	m_bSpiritSpeak = FALSE;
	m_bInsubstantial = FALSE;
	m_bEmote = FALSE;
	m_bCommCrystal = FALSE;
	m_bHasShield = FALSE;
	m_bArcherCanmove = FALSE;
	m_bStone = FALSE;
	m_bBlank1 = FALSE;
	m_bFlying = FALSE;
	m_bBlank2 = FALSE;
	m_bHallucinating = FALSE;
	m_bHidden = FALSE;
	m_bInDoors = FALSE;
	m_bCriminal = FALSE;
	m_bConjured = FALSE;
	m_bPet = FALSE;
	m_bSpawned = FALSE;
	m_bSaveParity = FALSE;
	m_bRidden = FALSE;
	m_bMounted = FALSE;
	//}}AFX_DATA_INIT
}

CPlayerTweakTab::~CPlayerTweakTab()
{
}

void CPlayerTweakTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerTweakTab)
	DDX_Control(pDX, IDC_FLAG_VALUE, m_csFlagValue);
	DDX_Check(pDX, IDC_INVULNERABLE, m_bInvulnerable);
	DDX_Check(pDX, IDC_DEAD, m_bDead);
	DDX_Check(pDX, IDC_FREEZE, m_bFreeze);
	DDX_Check(pDX, IDC_INVISIBLE, m_bInvisible);
	DDX_Check(pDX, IDC_SLEEP, m_bSleeping);
	DDX_Check(pDX, IDC_WARMODE, m_bWarmode);
	DDX_Check(pDX, IDC_REACTIVE_ARMOR, m_bReactiveArmor);
	DDX_Check(pDX, IDC_POISONED, m_bPoisoned);
	DDX_Check(pDX, IDC_NIGHTSIGHT, m_bNightsight);
	DDX_Check(pDX, IDC_MAGIC_REFLECT, m_bMagicReflect);
	DDX_Check(pDX, IDC_POLYMORPHED, m_bPolymorphed);
	DDX_Check(pDX, IDC_INCOGNITO, m_bIncognito);
	DDX_Check(pDX, IDC_SPIRITSPEAK, m_bSpiritSpeak);
	DDX_Check(pDX, IDC_INSUBSTANTIAL, m_bInsubstantial);
	DDX_Check(pDX, IDC_EMOTE, m_bEmote);
	DDX_Check(pDX, IDC_COMMCRYSTAL, m_bCommCrystal);
	DDX_Check(pDX, IDC_HAS_SHIELD, m_bHasShield);
	DDX_Check(pDX, IDC_ARCHCANMOVE, m_bArcherCanmove);
	DDX_Check(pDX, IDC_STONE, m_bStone);
	DDX_Check(pDX, IDC_BLANK1, m_bBlank1);
	DDX_Check(pDX, IDC_FLYING, m_bFlying);
	DDX_Check(pDX, IDC_BLANK2, m_bBlank2);
	DDX_Check(pDX, IDC_HALLUCINATING, m_bHallucinating);
	DDX_Check(pDX, IDC_HIDDEN, m_bHidden);
	DDX_Check(pDX, IDC_INDOORS, m_bInDoors);
	DDX_Check(pDX, IDC_CRIMINAL, m_bCriminal);
	DDX_Check(pDX, IDC_CONJURED, m_bConjured);
	DDX_Check(pDX, IDC_PET, m_bPet);
	DDX_Check(pDX, IDC_SPAWNED, m_bSpawned);
	DDX_Check(pDX, IDC_SAVEPARITY, m_bSaveParity);
	DDX_Check(pDX, IDC_RIDDEN, m_bRidden);
	DDX_Check(pDX, IDC_MOUNTED, m_bMounted);

	DDX_Control(pDX, IDC_BRAIN, m_ccbBrain);
	DDX_Control(pDX, IDC_SKILL, m_ccbSkill);
	DDX_Control(pDX, IDC_SKILLVALUE, m_ceSkillValue);
	DDX_Control(pDX, IDC_STATS, m_ccbStat);
	DDX_Control(pDX, IDC_STATVALUE, m_ceStatValue);
	DDX_Control(pDX, IDC_RESISTENCE, m_ccbResistence);
	DDX_Control(pDX, IDC_RESISTENCEVALUE, m_ceResistenceValue);
	DDX_Control(pDX, IDC_EVENTS, m_ccbEvents);
	DDX_Control(pDX, IDC_MISC, m_ccbMisc);
	DDX_Control(pDX, IDC_MISCVALUE, m_ceMiscValue);
	DDX_Control(pDX, IDC_TAG, m_ccbTags);
	DDX_Control(pDX, IDC_TAGVALUE, m_ceTagsValue);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerTweakTab, CDockingPage)
	//{{AFX_MSG_MAP(CPlayerTweakTab)
	ON_BN_CLICKED(IDC_RESETFLAGS, OnResetflags)
	ON_BN_CLICKED(IDC_SETFLAGS, OnSetflags)

	ON_BN_CLICKED(IDC_INVULNERABLE, OnInvulnerable)
	ON_BN_CLICKED(IDC_DEAD, OnDead)
	ON_BN_CLICKED(IDC_FREEZE, OnFreeze)
	ON_BN_CLICKED(IDC_INVISIBLE, OnInvisible)
	ON_BN_CLICKED(IDC_SLEEP, OnSleep)
	ON_BN_CLICKED(IDC_WARMODE, OnWarmode)
	ON_BN_CLICKED(IDC_REACTIVE_ARMOR, OnReactiveArmor)
	ON_BN_CLICKED(IDC_POISONED, OnPoisoned)
	ON_BN_CLICKED(IDC_NIGHTSIGHT, OnNightsight)
	ON_BN_CLICKED(IDC_MAGIC_REFLECT, OnMagicReflect)
	ON_BN_CLICKED(IDC_POLYMORPHED, OnPolymorphed)
	ON_BN_CLICKED(IDC_INCOGNITO, OnIncognito)
	ON_BN_CLICKED(IDC_SPIRITSPEAK, OnSpiritspeak)
	ON_BN_CLICKED(IDC_INSUBSTANTIAL, OnInsubstantial)
	ON_BN_CLICKED(IDC_EMOTE, OnEmote)
	ON_BN_CLICKED(IDC_COMMCRYSTAL, OnCommcrystal)
	ON_BN_CLICKED(IDC_HAS_SHIELD, OnHasShield)
	ON_BN_CLICKED(IDC_ARCHCANMOVE, OnArcherCanMove)
	ON_BN_CLICKED(IDC_STONE, OnStone)
	ON_BN_CLICKED(IDC_BLANK1, OnBlank1)
	ON_BN_CLICKED(IDC_FLYING, OnFlying)
	ON_BN_CLICKED(IDC_BLANK2, OnBlank2)
	ON_BN_CLICKED(IDC_HALLUCINATING, OnHallucinating)
	ON_BN_CLICKED(IDC_HIDDEN, OnHidden)
	ON_BN_CLICKED(IDC_INDOORS, OnIndoors)
	ON_BN_CLICKED(IDC_CRIMINAL, OnCriminal)
	ON_BN_CLICKED(IDC_CONJURED, OnConjured)
	ON_BN_CLICKED(IDC_PET, OnPet)
	ON_BN_CLICKED(IDC_SPAWNED, OnSpawned)
	ON_BN_CLICKED(IDC_SAVEPARITY, OnSaveparity)
	ON_BN_CLICKED(IDC_RIDDEN, OnRidden)
	ON_BN_CLICKED(IDC_MOUNTED, OnMounted)

	ON_BN_CLICKED(IDC_SETBRAIN, OnSetbrain)
	ON_BN_CLICKED(IDC_SETSKILL, OnSetskill)
	ON_BN_CLICKED(IDC_SETSTAT, OnSetstat)
	ON_BN_CLICKED(IDC_SETRESISTENCE, OnSetresistence)
	ON_BN_CLICKED(IDC_ADDEVENT, OnSetevent)
	ON_BN_CLICKED(IDC_REMOVEEVENT, OnRemoveevent)
	ON_BN_CLICKED(IDC_SETMISC, OnSetmisc)
	ON_BN_CLICKED(IDC_SETTAG, OnSettag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerTweakTab message handlers

BOOL CPlayerTweakTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppPlayerTweakTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CPlayerTweakTab;
	Main->m_pcppPlayerTweakTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	for ( int i = 0; i <= Main->m_pScripts->m_asaNPCBrains.GetUpperBound(); i++ )
		m_ccbBrain.AddString(Main->m_pScripts->m_asaNPCBrains[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaEvents.GetUpperBound(); i++ )
		m_ccbEvents.AddString(Main->m_pScripts->m_asaEvents[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaNPCStats.GetUpperBound(); i++ )
		m_ccbStat.AddString(Main->m_pScripts->m_asaNPCStats[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaNPCResistences.GetUpperBound(); i++ )
		m_ccbResistence.AddString(Main->m_pScripts->m_asaNPCResistences[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaNPCMisc.GetUpperBound(); i++ )
		m_ccbMisc.AddString(Main->m_pScripts->m_asaNPCMisc[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaNPCTags.GetUpperBound(); i++ )
		m_ccbTags.AddString(Main->m_pScripts->m_asaNPCTags[i]);
	for ( int i = 0; i <= Main->m_pScripts->m_asaNPCSkills.GetUpperBound(); i++ )
	{
		//Load TAG.OVERRIDE.SKILLCAP_*
		CString csTag, csComment, csSkill;
		csSkill = Main->m_pScripts->m_asaNPCSkills[i].Mid(Main->m_pScripts->m_asaNPCSkills[i].Find("(")+1);
		csSkill = csSkill.SpanExcluding(")");
		csSkill.Trim();
		csComment = Main->m_pScripts->m_asaNPCSkills[i].SpanExcluding("(");
		csComment.Trim();
		csTag.Format("TAG.OVERRIDE.SKILLCAP_%s (%s)",csSkill ,csComment);
		m_ccbTags.AddString(csTag);

		//Load SKILLLOCK.*
		csTag.Format("SKILLLOCK.%s (%s)",csSkill ,csComment);
		m_ccbMisc.AddString(csTag);

		m_ccbSkill.AddString(Main->m_pScripts->m_asaNPCSkills[i]);
	}

	m_ccbBrain.SetCurSel(0);
	m_ccbEvents.SetCurSel(0);
	m_ccbSkill.SetCurSel(0);
	m_ccbStat.SetCurSel(0);
	m_ccbResistence.SetCurSel(0);
	m_ccbMisc.SetCurSel(0);
	m_ccbTags.SetCurSel(0);

	AjustComboBox(&m_ccbBrain);
	AjustComboBox(&m_ccbEvents);
	AjustComboBox(&m_ccbSkill);
	AjustComboBox(&m_ccbStat);
	AjustComboBox(&m_ccbResistence);
	AjustComboBox(&m_ccbMisc);
	AjustComboBox(&m_ccbTags);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPlayerTweakTab::OnSetActive() 
{
	UpdateData();
	if ( m_ccbBrain.GetCount() != Main->m_pScripts->m_asaNPCBrains.GetUpperBound()+1)
	{
		m_ccbBrain.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCBrains.GetUpperBound(); i++ )
			m_ccbBrain.AddString(Main->m_pScripts->m_asaNPCBrains[i]);
		m_ccbBrain.SetCurSel(0);
		AjustComboBox(&m_ccbBrain);
	}
	if ( m_ccbEvents.GetCount() != Main->m_pScripts->m_asaEvents.GetUpperBound()+1)
	{
		m_ccbEvents.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaEvents.GetUpperBound(); i++ )
			m_ccbEvents.AddString(Main->m_pScripts->m_asaEvents[i]);
		m_ccbEvents.SetCurSel(0);
		AjustComboBox(&m_ccbEvents);
	}
	if ( m_ccbStat.GetCount() != Main->m_pScripts->m_asaNPCStats.GetUpperBound()+1)
	{
		m_ccbStat.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCStats.GetUpperBound(); i++ )
			m_ccbStat.AddString(Main->m_pScripts->m_asaNPCStats[i]);
		m_ccbStat.SetCurSel(0);
		AjustComboBox(&m_ccbStat);
	}
	if ( m_ccbResistence.GetCount() != Main->m_pScripts->m_asaNPCResistences.GetUpperBound()+1)
	{
		m_ccbResistence.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCResistences.GetUpperBound(); i++ )
			m_ccbResistence.AddString(Main->m_pScripts->m_asaNPCResistences[i]);
		m_ccbResistence.SetCurSel(0);
		AjustComboBox(&m_ccbResistence);
	}
	if ( m_ccbMisc.GetCount() != Main->m_pScripts->m_asaNPCMisc.GetUpperBound()+Main->m_pScripts->m_asaNPCSkills.GetUpperBound()+2)
	{
		m_ccbMisc.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCMisc.GetUpperBound(); i++ )
			m_ccbMisc.AddString(Main->m_pScripts->m_asaNPCMisc[i]);
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCSkills.GetUpperBound(); i++ )
		{
			//Load SKILLLOCK.*
			CString csTag, csComment, csSkill;
			csSkill = Main->m_pScripts->m_asaNPCSkills[i].Mid(Main->m_pScripts->m_asaNPCSkills[i].Find("(")+1);
			csSkill = csSkill.SpanExcluding(")");
			csSkill.Trim();
			csComment = Main->m_pScripts->m_asaNPCSkills[i].SpanExcluding("(");
			csComment.Trim();
			csTag.Format("SKILLLOCK.%s (%s)",csSkill ,csComment);
			m_ccbMisc.AddString(csTag);
		}
		m_ccbMisc.SetCurSel(0);
		AjustComboBox(&m_ccbMisc);
	}
	if ( m_ccbTags.GetCount() != Main->m_pScripts->m_asaNPCTags.GetUpperBound()+Main->m_pScripts->m_asaNPCSkills.GetUpperBound()+2)
	{
		m_ccbTags.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCTags.GetUpperBound(); i++ )
			m_ccbTags.AddString(Main->m_pScripts->m_asaNPCTags[i]);
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCSkills.GetUpperBound(); i++ )
		{
			//Load TAG.OVERRIDE.SKILLCAP_*
			CString csTag, csComment, csSkill;
			csSkill = Main->m_pScripts->m_asaNPCSkills[i].Mid(Main->m_pScripts->m_asaNPCSkills[i].Find("(")+1);
			csSkill = csSkill.SpanExcluding(")");
			csSkill.Trim();
			csComment = Main->m_pScripts->m_asaNPCSkills[i].SpanExcluding("(");
			csComment.Trim();
			csTag.Format("TAG.OVERRIDE.SKILLCAP_%s (%s)",csSkill ,csComment);
			m_ccbTags.AddString(csTag);
		}
		m_ccbTags.SetCurSel(0);
		AjustComboBox(&m_ccbTags);
	}
	if ( m_ccbSkill.GetCount() != Main->m_pScripts->m_asaNPCSkills.GetUpperBound()+1)
	{
		m_ccbSkill.ResetContent();
		for ( int i = 0; i <= Main->m_pScripts->m_asaNPCSkills.GetUpperBound(); i++ )
			m_ccbSkill.AddString(Main->m_pScripts->m_asaNPCSkills[i]);
		m_ccbSkill.SetCurSel(0);
		AjustComboBox(&m_ccbSkill);
	}

	return CDockingPage::OnSetActive();
}

void CPlayerTweakTab::OnResetflags() 
{
	m_bInvulnerable = FALSE;
	m_bDead = FALSE;
	m_bFreeze = FALSE;
	m_bInvisible = FALSE;
	m_bSleeping = FALSE;
	m_bWarmode = FALSE;
	m_bReactiveArmor = FALSE;
	m_bPoisoned = FALSE;
	m_bNightsight = FALSE;
	m_bMagicReflect = FALSE;
	m_bPolymorphed = FALSE;
	m_bIncognito = FALSE;
	m_bSpiritSpeak = FALSE;
	m_bInsubstantial = FALSE;
	m_bEmote = FALSE;
	m_bCommCrystal = FALSE;
	m_bHasShield = FALSE;
	m_bArcherCanmove = FALSE;
	m_bStone = FALSE;
	m_bBlank1 = FALSE;
	m_bFlying = FALSE;
	m_bBlank2 = FALSE;
	m_bHallucinating = FALSE;
	m_bHidden = FALSE;
	m_bInDoors = FALSE;
	m_bCriminal = FALSE;
	m_bConjured = FALSE;
	m_bPet = FALSE;
	m_bSpawned = FALSE;
	m_bSaveParity = FALSE;
	m_bRidden = FALSE;
	m_bMounted = FALSE;
	m_csFlagValue.SetWindowText("000000000");
	UpdateData(false);
}

void CPlayerTweakTab::OnSetflags() 
{
	CString csVal;
	m_csFlagValue.GetWindowText(csVal);
	CString csCmd;
	csCmd.Format("%sset flags %s", Main->m_csCommandPrefix, csVal);
	SendToUO(csCmd);
}

void CPlayerTweakTab::OnInvulnerable() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bInvulnerable)
		dwVal |= STATF_INVUL;
	else
		dwVal ^= STATF_INVUL;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);
}

void CPlayerTweakTab::OnDead() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bDead)
		dwVal |= STATF_DEAD;
	else
		dwVal ^= STATF_DEAD;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnFreeze() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bFreeze)
		dwVal |= STATF_Freeze;
	else
		dwVal ^= STATF_Freeze;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnInvisible() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bInvisible)
		dwVal |= STATF_Invisible;
	else
		dwVal ^= STATF_Invisible;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnSleep() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bSleeping)
		dwVal |= STATF_Sleeping;
	else
		dwVal ^= STATF_Sleeping;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnWarmode() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bWarmode)
		dwVal |= STATF_War;
	else
		dwVal ^= STATF_War;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnReactiveArmor() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bReactiveArmor)
		dwVal |= STATF_Reactive;
	else
		dwVal ^= STATF_Reactive;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnPoisoned() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPoisoned)
		dwVal |= STATF_Poisoned;
	else
		dwVal ^= STATF_Poisoned;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnNightsight() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bNightsight)
		dwVal |= STATF_NightSight;
	else
		dwVal ^= STATF_NightSight;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnMagicReflect() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bMagicReflect)
		dwVal |= STATF_Reflection;
	else
		dwVal ^= STATF_Reflection;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnPolymorphed() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPolymorphed)
		dwVal |= STATF_Polymorph;
	else
		dwVal ^= STATF_Polymorph;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnIncognito() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bIncognito)
		dwVal |= STATF_Incognito;
	else
		dwVal ^= STATF_Incognito;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnSpiritspeak() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bSpiritSpeak)
		dwVal |= STATF_SpiritSpeak;
	else
		dwVal ^= STATF_SpiritSpeak;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnInsubstantial() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bInsubstantial)
		dwVal |= STATF_Insubstantial;
	else
		dwVal ^= STATF_Insubstantial;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnEmote() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bEmote)
		dwVal |= STATF_EmoteAction;
	else
		dwVal ^= STATF_EmoteAction;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnCommcrystal() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bCommCrystal)
		dwVal |= STATF_Commcrystal;
	else
		dwVal ^= STATF_Commcrystal;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);
}

void CPlayerTweakTab::OnHasShield() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bHasShield)
		dwVal |= STATF_HasShield;
	else
		dwVal ^= STATF_HasShield;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnArcherCanMove() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bArcherCanmove)
		dwVal |= STATF_Archercanmove;
	else
		dwVal ^= STATF_Archercanmove;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);
}

void CPlayerTweakTab::OnStone() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bStone)
		dwVal |= STATF_Stone;
	else
		dwVal ^= STATF_Stone;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnBlank1() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bBlank1)
		dwVal |= STATF_Blank1;
	else
		dwVal ^= STATF_Blank1;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnFlying() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bFlying)
		dwVal |= STATF_Fly;
	else
		dwVal ^= STATF_Fly;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnBlank2() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bBlank2)
		dwVal |= STATF_Blank2;
	else
		dwVal ^= STATF_Blank2;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnHallucinating() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bHallucinating)
		dwVal |= STATF_Hallucinating;
	else
		dwVal ^= STATF_Hallucinating;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnHidden() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bHidden)
		dwVal |= STATF_Hidden;
	else
		dwVal ^= STATF_Hidden;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnIndoors() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bInDoors)
		dwVal |= STATF_InDoors;
	else
		dwVal ^= STATF_InDoors;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);
}

void CPlayerTweakTab::OnCriminal() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bCriminal)
		dwVal |= STATF_Criminal;
	else
		dwVal ^= STATF_Criminal;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnConjured() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bConjured)
		dwVal |= STATF_Conjured;
	else
		dwVal ^= STATF_Conjured;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnPet() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bPet)
		dwVal |= STATF_Pet;
	else
		dwVal ^= STATF_Pet;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnSpawned() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bSpawned)
		dwVal |= STATF_Spawned;
	else
		dwVal ^= STATF_Spawned;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnSaveparity() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bSaveParity)
		dwVal |= STATF_SaveParity;
	else
		dwVal ^= STATF_SaveParity;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);		
}

void CPlayerTweakTab::OnRidden() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bRidden)
		dwVal |= STATF_Ridden;
	else
		dwVal ^= STATF_Ridden;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);		
}

void CPlayerTweakTab::OnMounted() 
{
	UpdateData(true);
	CString csCurrentVal;
	m_csFlagValue.GetWindowText(csCurrentVal);
	DWORD dwVal = ahextoi(csCurrentVal);
	if (m_bMounted)
		dwVal |= STATF_OnHorse;
	else
		dwVal ^= STATF_OnHorse;
	CString csNewVal;
	csNewVal.Format("%09x", dwVal);
	m_csFlagValue.SetWindowText(csNewVal);
	UpdateData(false);	
}

void CPlayerTweakTab::OnSetbrain() 
{
	CString csBrain;
	m_ccbBrain.GetWindowText(csBrain);
	csBrain = csBrain.SpanExcluding("(");
	if (csBrain != "")
	{
		CString csCmd;
		csCmd.Format("%sset NPC %s", Main->m_csCommandPrefix, csBrain);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnSetskill() 
{
	CString csSkill, csValue;
	m_ccbSkill.GetWindowText(csSkill);
	m_ceSkillValue.GetWindowText(csValue);
	csSkill = csSkill.SpanExcluding("(");
	if (csSkill != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csSkill, csValue);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnSetstat() 
{
	CString csStat, csValue;
	m_ccbStat.GetWindowText(csStat);
	m_ceStatValue.GetWindowText(csValue);
	csStat = csStat.SpanExcluding("(");
	if (csStat != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csStat, csValue);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnSetresistence() 
{
	CString csRes, csValue;
	m_ccbResistence.GetWindowText(csRes);
	m_ceResistenceValue.GetWindowText(csValue);
	csRes = csRes.SpanExcluding("(");
	if (csRes != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csRes, csValue);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnSetevent() 
{
	CString csEvent;
	m_ccbEvents.GetWindowText(csEvent);
	csEvent = csEvent.SpanExcluding("(");
	if (csEvent != "")
	{
		CString csCmd;
		csCmd.Format("%sxevents +%s", Main->m_csCommandPrefix, csEvent);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnRemoveevent() 
{
	CString csEvent;
	m_ccbEvents.GetWindowText(csEvent);
	csEvent = csEvent.SpanExcluding("(");
	if (csEvent != "")
	{
		CString csCmd;
		csCmd.Format("%sxevents -%s", Main->m_csCommandPrefix, csEvent);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnSetmisc() 
{
	CString csMisc, csValue;
	m_ccbMisc.GetWindowText(csMisc);
	m_ceMiscValue.GetWindowText(csValue);
	csMisc = csMisc.SpanExcluding("(");
	if (csMisc != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csMisc, csValue);
		SendToUO(csCmd);
	}
}

void CPlayerTweakTab::OnSettag() 
{
	CString csTag, csValue;
	m_ccbTags.GetWindowText(csTag);
	m_ceTagsValue.GetWindowText(csValue);
	csTag = csTag.SpanExcluding("(");
	if (csTag != "" && csValue != "")
	{
		CString csCmd;
		csCmd.Format("%sset %s %s", Main->m_csCommandPrefix, csTag, csValue);
		SendToUO(csCmd);
	}
}