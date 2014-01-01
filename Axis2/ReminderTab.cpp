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

// ReminderTab.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ReminderTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReminderTab property page

IMPLEMENT_DYNCREATE(CReminderTab, CDockingPage)

CReminderTab::CReminderTab() : CDockingPage(CReminderTab::IDD,CMsg("IDS_REMINDER"))
{
	//{{AFX_DATA_INIT(CReminderTab)
	iReminder = 0;
	m_bStartup = 0;
	bEditor = false;
	//}}AFX_DATA_INIT
}

CReminderTab::~CReminderTab()
{
}

void CReminderTab::DoDataExchange(CDataExchange* pDX)
{
	CDockingPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReminderTab)
	DDX_Control(pDX, IDC_WEEKDAY, m_ccbWeekday);
	DDX_Control(pDX, IDC_STARTUP, m_cbStartup);
	DDX_Check(pDX, IDC_STARTUP, m_bStartup);
	DDX_Control(pDX, IDC_ADD, m_cbAdd);
	DDX_Control(pDX, IDC_EDIT, m_cbEdit);
	DDX_Control(pDX, IDC_DELETE, m_cbDelete);
	DDX_Control(pDX, IDC_SAVE, m_cbSave);
	DDX_Control(pDX, IDC_CANCEL, m_cbCancel);
	DDX_Control(pDX, IDC_MONTHLY, m_crMonthly);
	DDX_Control(pDX, IDC_WEEKLY, m_crWeekly);
	DDX_Control(pDX, IDC_DAILY, m_crDaily);
	DDX_Control(pDX, IDC_ONCE, m_crOnce);
	DDX_Control(pDX, IDC_NEVER, m_crNever);
	DDX_Control(pDX, IDC_DATEPICKER, m_cdtpDatePicker);
	DDX_Control(pDX, IDC_TIMEPICKER, m_cdtpTimePicker);
	DDX_Control(pDX, IDC_LIST, m_clcList);
	DDX_Control(pDX, IDC_TITLE, m_ceTitle);
	DDX_Control(pDX, IDC_FRAMETEXT, m_ceText);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReminderTab, CDockingPage)
	//{{AFX_MSG_MAP(CReminderTab)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnListChange)
	ON_BN_CLICKED(IDC_MONTHLY, OnMonthly)
	ON_BN_CLICKED(IDC_WEEKLY, OnWeekly)
	ON_BN_CLICKED(IDC_DAILY, OnDaily)
	ON_BN_CLICKED(IDC_ONCE, OnOnce)
	ON_BN_CLICKED(IDC_NEVER, OnNever)
	ON_BN_CLICKED(IDC_STARTUP, OnStartup)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CANCEL, OnCancelReminder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReminderTab message handlers

BOOL CReminderTab::OnInitDialog() 
{
	m_dcPropertyPage = Main->m_pcppReminderTab;
	if(m_bModifyDlgStylesAndPos == false)
		m_dcDialogPage = new CReminderTab;
	Main->m_pcppReminderTab->m_dcCurrentPage = this;

	CDockingPage::OnInitDialog();

	tNow = CTime::GetCurrentTime();

	m_ccbWeekday.SetCurSel(tNow.GetDayOfWeek()-1);
	m_crNever.SetCheck(1);
	m_cdtpTimePicker.SetFormat("h':'mm tt");
	m_cdtpDatePicker.SetFormat("ddd, MMMM d,  yyyy");
	CTime tMax(2100,1,1,0,0,0);
	m_cdtpDatePicker.SetRange(&tNow, &tMax);

	m_clcList.InsertColumn(0, "Title", LVCFMT_LEFT, 100, -1);
	m_clcList.InsertColumn(1, "Type", LVCFMT_LEFT, 66, -1);
	LoadReminders();

	return TRUE;
}

BOOL CReminderTab::OnSetActive() 
{
	m_clcList.DeleteAllItems();
	LoadReminders();
	return CDockingPage::OnSetActive();
}

void CReminderTab::OnMonthly()
{
	iReminder = 4;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(true);
	m_ccbWeekday.ResetContent();
	CString csDay;
	for (int i = 1; i <= 31; i++)
	{
		csDay.Format("%d", i);
		m_ccbWeekday.AddString(csDay);
	}
	m_ccbWeekday.SetCurSel(tNow.GetDay()-1);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnWeekly()
{
	iReminder = 3;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(true);
	m_ccbWeekday.ResetContent();
	m_ccbWeekday.AddString("Sunday");
	m_ccbWeekday.AddString("Monday");
	m_ccbWeekday.AddString("Tuesday");
	m_ccbWeekday.AddString("Wednesday");
	m_ccbWeekday.AddString("Thursday");
	m_ccbWeekday.AddString("Friday");
	m_ccbWeekday.AddString("Saturday");
	m_ccbWeekday.SetCurSel(tNow.GetDayOfWeek()-1);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnDaily()
{
	iReminder = 2;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnOnce()
{
	iReminder = 1;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(true);
}

void CReminderTab::OnNever()
{
	iReminder = 0;
	m_cbStartup.EnableWindow(false);
	m_cdtpTimePicker.EnableWindow(false);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnStartup()
{
	UpdateData();
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
}

void CReminderTab::OnAdd()
{
	tNow = CTime::GetCurrentTime();
	m_cdtpTimePicker.SetTime(&tNow);
	m_ceText.SetReadOnly(0);
	m_ceTitle.SetReadOnly(0);
	m_ceText.SetWindowText("");
	m_ceTitle.SetWindowText("");
	m_cbSave.EnableWindow(true);
	m_cbCancel.EnableWindow(true);
	m_cbAdd.EnableWindow(false);
	m_crMonthly.EnableWindow(true);
	m_crWeekly.EnableWindow(true);
	m_crDaily.EnableWindow(true);
	m_crOnce.EnableWindow(true);
	m_crNever.EnableWindow(true);
	m_cbEdit.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
	switch(iReminder)
	{
	case 0:
		OnNever();
		break;
	case 1:
		OnOnce();
		break;
	case 2:
		OnDaily();
		break;
	case 3:
		OnWeekly();
		break;
	case 4:
		OnMonthly();
		break;
	}

}

void CReminderTab::OnEdit()
{
	tNow = CTime::GetCurrentTime();
	m_cdtpTimePicker.SetTime(&tNow);
	bEditor = true;
	m_ceText.SetReadOnly(0);
	m_ceTitle.SetReadOnly(0);
	m_cbSave.EnableWindow(true);
	m_cbCancel.EnableWindow(true);
	m_cbAdd.EnableWindow(false);
	m_crMonthly.EnableWindow(true);
	m_crWeekly.EnableWindow(true);
	m_crDaily.EnableWindow(true);
	m_crOnce.EnableWindow(true);
	m_crNever.EnableWindow(true);

	if (m_crNever.GetCheck())
	{
		m_cbStartup.EnableWindow(false);
		m_cdtpTimePicker.EnableWindow(false);
		m_ccbWeekday.EnableWindow(false);
		m_cdtpDatePicker.EnableWindow(false);
	}
	else
	{
		m_cbStartup.EnableWindow(true);
		m_cdtpTimePicker.EnableWindow(m_cbStartup.GetCheck() ? false : true);
		m_ccbWeekday.EnableWindow((m_crWeekly.GetCheck()) || (m_crMonthly.GetCheck()));
		m_cdtpDatePicker.EnableWindow(m_crOnce.GetCheck());
	}

	m_cbEdit.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
}

void CReminderTab::OnDelete()
{
	int iIndex = m_clcList.GetSelectionMark();
	CString csTitle, csKey;
	csTitle = m_clcList.GetItemText(iIndex,0);
	csKey.Format("%s\\%s",REGKEY_REMINDER, csTitle);
	Main->DeleteRegistryKey(csKey,hRegLocation);
	m_clcList.DeleteItem(iIndex);
	m_cbEdit.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
}

void CReminderTab::OnSave()
{
	CString csTitle, csText;
	m_ceTitle.GetWindowText(csTitle);
	m_ceText.GetWindowText(csText);
	if(csTitle == "")
	{
		AfxMessageBox("Title field empty", MB_ICONSTOP);
		return;
	}
	if(csText == "")
	{
		AfxMessageBox("Text empty", MB_ICONSTOP);
		return;
	}

	CString csProfileKey;
	if (bEditor)
	{
		int iIndex = m_clcList.GetSelectionMark();
		csProfileKey.Format("%s\\%s",REGKEY_REMINDER, m_clcList.GetItemText(iIndex,0));
		Main->DeleteRegistryKey(csProfileKey,hRegLocation);
	}
	csProfileKey.Format("%s\\%s",REGKEY_REMINDER, csTitle);

	HKEY hKey;
	if( RegOpenKeyEx(hRegLocation, csProfileKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS )
	{
	//This Reminder already exists, stop here.
		CString csError;
		csError.Format("Reminder '%s' already exists" , csTitle);
		AfxMessageBox(csError, MB_ICONSTOP);
		RegCloseKey(hKey);
		return;
	}

	Main->PutRegistryString("Text",csText,hRegLocation,csProfileKey);
	Main->PutRegistryDword("Type",iReminder,hRegLocation,csProfileKey);
	CTime tReminder;
	switch(iReminder)
	{
	case 0:
		break;
	default:
		{
			if (m_cbStartup.GetCheck())
				Main->PutRegistryDword("Startup",1,hRegLocation,csProfileKey);
			else
			{
				m_cdtpTimePicker.GetTime(tReminder);
				Main->PutRegistryDword("Hour",tReminder.GetHour(),hRegLocation,csProfileKey);
				Main->PutRegistryDword("Minute",tReminder.GetMinute(),hRegLocation,csProfileKey);
			}
		}
	}
	switch(iReminder)
	{
	case 0:
	case 2:
		break;
	case 1:
		{
		m_cdtpDatePicker.GetTime(tReminder);
		Main->PutRegistryDword("Date",(DWORD)tReminder.GetTime(),hRegLocation,csProfileKey);
		break;
		}
	case 3:
	case 4:
		{
		Main->PutRegistryDword("Date",m_ccbWeekday.GetCurSel()+1,hRegLocation,csProfileKey);
		break;
		}
	}

	CString csType;
	switch(iReminder)
	{
	case 0:
		csType = "Never";
		break;
	case 1:
		csType = "Once";
		break;
	case 2:
		csType = "Daily";
		break;
	case 3:
		csType = "Weekly";
		break;
	case 4:
		csType = "Monthly";
		break;
	}

	if (bEditor)
	{
		int iIndex = m_clcList.GetSelectionMark();
		m_clcList.SetItemText(iIndex, 0, csTitle);
		m_clcList.SetItemText(iIndex, 1, csType);
	}
	else
	{
		int iIndex = m_clcList.GetItemCount();
		m_clcList.InsertItem(iIndex, csTitle, 0);
		m_clcList.SetItemText(iIndex, 1, csType);
		m_clcList.SetItemState( iIndex, LVIS_SELECTED, LVIS_SELECTED );
		m_clcList.EnsureVisible( iIndex, FALSE );
		m_clcList.SetHotItem( iIndex );
	}


	m_ceText.SetReadOnly(1);
	m_ceTitle.SetReadOnly(1);
	m_cbSave.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbAdd.EnableWindow(true);
	m_crMonthly.EnableWindow(false);
	m_crWeekly.EnableWindow(false);
	m_crDaily.EnableWindow(false);
	m_crOnce.EnableWindow(false);
	m_crNever.EnableWindow(false);
	m_cbStartup.EnableWindow(false);
	m_cdtpTimePicker.EnableWindow(false);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
	m_cbEdit.EnableWindow(true);
	m_cbDelete.EnableWindow(true);
	bEditor = false;
}

void CReminderTab::OnCancelReminder()
{
	m_ceText.SetReadOnly(1);
	m_ceTitle.SetReadOnly(1);
	m_cbSave.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbAdd.EnableWindow(true);
	m_crMonthly.EnableWindow(false);
	m_crWeekly.EnableWindow(false);
	m_crDaily.EnableWindow(false);
	m_crOnce.EnableWindow(false);
	m_crNever.EnableWindow(false);
	m_cbStartup.EnableWindow(false);
	m_cdtpTimePicker.EnableWindow(false);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
	m_cbEdit.EnableWindow(bEditor);
	m_cbDelete.EnableWindow(bEditor);
	bEditor = false;
}

void CReminderTab::LoadReminders()
{
	HKEY hKey;
	int iIndex = 0;
	CString csTitle, csType, csProfileKey;
	LONG lStatus = RegOpenKeyEx(hRegLocation, REGKEY_REMINDER, 0, KEY_ALL_ACCESS, &hKey);
	if ( lStatus == ERROR_SUCCESS )
	{
		while (lStatus == ERROR_SUCCESS)
		{
			char szBuffer[MAX_PATH];
			DWORD dwSize = sizeof(szBuffer);
			lStatus = RegEnumKeyEx(hKey, iIndex, &szBuffer[0], &dwSize, 0, NULL, NULL, NULL);
			if (lStatus == ERROR_SUCCESS)
			{
				csTitle = szBuffer;
				csProfileKey.Format("%s\\%s",REGKEY_REMINDER, csTitle);
				int iType = Main->GetRegistryDword("Type",0,hRegLocation,csProfileKey);
				switch(iType)
					{
					case 0:
						csType = "Never";
						break;
					case 1:
						csType = "Once";
						break;
					case 2:
						csType = "Daily";
						break;
					case 3:
						csType = "Weekly";
						break;
					case 4:
						csType = "Monthly";
						break;
					}

				m_clcList.InsertItem(iIndex, csTitle, 0);
				m_clcList.SetItemText(iIndex, 1, csType);
			}
			iIndex++;
		}
		RegCloseKey(hKey);
	}
}

void CReminderTab::OnListChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		// this is the one that has been selected
		int iSelIndex = m_clcList.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex == -1)
			return;

		m_clcList.SetHotItem(iSelIndex);

		tNow = CTime::GetCurrentTime();
		m_cdtpTimePicker.SetTime(&tNow);
		m_cdtpDatePicker.SetTime(&tNow);
		m_ccbWeekday.ResetContent();
		m_ccbWeekday.AddString("Sunday");
		m_ccbWeekday.AddString("Monday");
		m_ccbWeekday.AddString("Tuesday");
		m_ccbWeekday.AddString("Wednesday");
		m_ccbWeekday.AddString("Thursday");
		m_ccbWeekday.AddString("Friday");
		m_ccbWeekday.AddString("Saturday");
		m_ccbWeekday.SetCurSel(tNow.GetDayOfWeek()-1);

		CString csTitle, csText, csKey;
		csTitle = m_clcList.GetItemText(iSelIndex,0);
		csKey.Format("%s\\%s",REGKEY_REMINDER, csTitle);
		csText = Main->GetRegistryString("Text","",hRegLocation,csKey);
		m_ceTitle.SetWindowText(csTitle);
		m_ceText.SetWindowText(csText);

		int iType, iHour, iMin, iDate, iStartup;
		iType = Main->GetRegistryDword("Type",0,hRegLocation,csKey);
		iHour = Main->GetRegistryDword("Hour",0,hRegLocation,csKey);
		iMin = Main->GetRegistryDword("Minute",0,hRegLocation,csKey);
		iDate = Main->GetRegistryDword("Date",0,hRegLocation,csKey);
		iStartup = Main->GetRegistryDword("Startup",0,hRegLocation,csKey);

		m_crNever.SetCheck(0);
		m_crOnce.SetCheck(0);
		m_crDaily.SetCheck(0);
		m_crWeekly.SetCheck(0);
		m_crMonthly.SetCheck(0);
		iReminder = iType;

		m_cbStartup.SetCheck(0);
		switch(iType)
		{
		case 0:
			break;
		default:
			{
				if (iStartup)
					m_cbStartup.SetCheck(1);
				else
				{
					CTime tOnce(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),iHour,iMin,tNow.GetSecond());
					m_cdtpTimePicker.SetTime(&tOnce);
				}
			}
		}

		switch(iType)
		{
		case 0:
			{
				m_crNever.SetCheck(1);
			}
			break;
		case 1:
			{
				CTime tOnce((__time64_t)iDate);
				m_crOnce.SetCheck(1);
				m_cdtpDatePicker.SetTime(&tOnce);
			}
			break;
		case 2:
			{
				m_crDaily.SetCheck(1);
			}
			break;
		case 3:
			{
				m_crWeekly.SetCheck(1);
				m_ccbWeekday.SetCurSel(iDate-1);
			}
			break;
		case 4:
			{
				m_crMonthly.SetCheck(1);
				m_ccbWeekday.ResetContent();
				CString csDay;
				for (int i = 1; i <= 31; i++)
				{
					csDay.Format("%d", i);
					m_ccbWeekday.AddString(csDay);
				}
				m_ccbWeekday.SetCurSel(iDate-1);
			}
			break;
		}

		m_ceText.SetReadOnly(1);
		m_ceTitle.SetReadOnly(1);
		m_cbSave.EnableWindow(false);
		m_cbCancel.EnableWindow(false);
		m_cbAdd.EnableWindow(true);
		m_crMonthly.EnableWindow(false);
		m_crWeekly.EnableWindow(false);
		m_crDaily.EnableWindow(false);
		m_crOnce.EnableWindow(false);
		m_crNever.EnableWindow(false);
		m_cbStartup.EnableWindow(false);
		m_cdtpTimePicker.EnableWindow(false);
		m_ccbWeekday.EnableWindow(false);
		m_cdtpDatePicker.EnableWindow(false);
		m_cbEdit.EnableWindow(true);
		m_cbDelete.EnableWindow(true);
		bEditor = false;
		return;
	}
	*pResult = 0;
}