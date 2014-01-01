// ToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ToolBar.h"

// CAxis2LBar dialog

IMPLEMENT_DYNAMIC(CAxis2LBar, CDialog)

CAxis2LBar::CAxis2LBar(CWnd* pParent /*=NULL*/)
	: CDialog(CAxis2LBar::IDD, pParent)
{

}

CAxis2LBar::~CAxis2LBar()
{
}

void CAxis2LBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TOOLSPAWN, cb_spawn);
	DDX_Control(pDX, IDC_TOOLTRAVEL, cb_travel);
	DDX_Control(pDX, IDC_TOOLGENERAL, cb_general);
	DDX_Control(pDX, IDC_TOOLNPCTWEAK, cb_npctweak);
	DDX_Control(pDX, IDC_TOOLITEMS, cb_items);
	DDX_Control(pDX, IDC_TOOLITEMTWEAK, cb_itemtweak);
	DDX_Control(pDX, IDC_TOOLACCOUNT, cb_account);
	DDX_Control(pDX, IDC_TOOLMISC, cb_misc);
	DDX_Control(pDX, IDC_TOOLLAUNCHER, cb_launcher);
	DDX_Control(pDX, IDC_TOOLCOMMANDS, cb_commands);
	DDX_Control(pDX, IDC_TOOLREMINDER, cb_reminder);
	DDX_Control(pDX, IDC_TOOLLOGS, cb_logs);
	DDX_Control(pDX, IDC_TOOLSETTINGS, cb_settings);
}


BEGIN_MESSAGE_MAP(CAxis2LBar, CDialog)
	ON_BN_CLICKED(IDC_TOOLGENERAL, OnBnClickedToolgeneral)
	ON_BN_CLICKED(IDC_TOOLTRAVEL, OnBnClickedTooltravel)
	ON_BN_CLICKED(IDC_TOOLSPAWN, OnBnClickedToolspawn)
	ON_BN_CLICKED(IDC_TOOLNPCTWEAK, OnBnClickedToolnpctweak)
	ON_BN_CLICKED(IDC_TOOLITEMS, OnBnClickedToolitems)
	ON_BN_CLICKED(IDC_TOOLITEMTWEAK, OnBnClickedToolitemtweak)
	ON_BN_CLICKED(IDC_TOOLACCOUNT, OnBnClickedToolaccount)
	ON_BN_CLICKED(IDC_TOOLMISC, OnBnClickedToolmisc)
	ON_BN_CLICKED(IDC_TOOLLAUNCHER, OnBnClickedToollauncher)
	ON_BN_CLICKED(IDC_TOOLCOMMANDS, OnBnClickedToolcommands)
	ON_BN_CLICKED(IDC_TOOLREMINDER, OnBnClickedToolreminder)
	ON_BN_CLICKED(IDC_TOOLLOGS, OnBnClickedToollogs)
	ON_BN_CLICKED(IDC_TOOLSETTINGS, OnBnClickedToolsettings)
	ON_COMMAND(ID_TOOLBARMENU_SETTINGS, &CAxis2LBar::OnToolbarmenuSettings)
	ON_COMMAND(ID_TOOLBARMENU_PROFILE, &CAxis2LBar::OnToolbarmenuProfile)
	ON_COMMAND(ID_TOOLBARMENU_HELP, &CAxis2LBar::OnToolbarmenuHelp)
	ON_COMMAND(ID_TOOLBARMENU_EXIT, &CAxis2LBar::OnToolbarmenuExit)
END_MESSAGE_MAP()


// CAxis2LBar message handlers

BOOL CAxis2LBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	cb_general.InitButton(IDI_GENERAL,CMsg("IDS_GENERAL")+" "+CMsg("IDS_TAB"));
	cb_travel.InitButton(IDI_WORLD,CMsg("IDS_TRAVEL")+" "+CMsg("IDS_TAB"));
	cb_spawn.InitButton(IDI_SPAWN,CMsg("IDS_SPAWN")+" "+CMsg("IDS_TAB"));
	cb_npctweak.InitButton(IDI_CHARTWEAK,CMsg("IDS_PLAYER_TWEAK")+" "+CMsg("IDS_TAB"));
	cb_items.InitButton(IDI_ITEMS,CMsg("IDS_ITEMS")+" "+CMsg("IDS_TAB"));
	cb_itemtweak.InitButton(IDI_ITEMTWEAK,CMsg("IDS_ITEM_TWEAK")+" "+CMsg("IDS_TAB"));
	cb_account.InitButton(IDI_ACCOUNT,CMsg("IDS_ACCOUNT")+" "+CMsg("IDS_TAB"));
	cb_misc.InitButton(IDI_MISC,CMsg("IDS_MISC")+" "+CMsg("IDS_TAB"));
	cb_launcher.InitButton(IDI_LAUNCHER,CMsg("IDS_LAUNCHER")+" "+CMsg("IDS_TAB"));
	cb_commands.InitButton(IDI_COMMANDS,CMsg("IDS_COMMANDS")+" "+CMsg("IDS_TAB"));
	cb_reminder.InitButton(IDI_REMINDER,CMsg("IDS_REMINDER")+" "+CMsg("IDS_TAB"));
	cb_logs.InitButton(IDI_LOGS,CMsg("IDS_LOGS")+" "+CMsg("IDS_TAB"));
	cb_settings.InitButton(IDI_SETTINGS,CMsg("IDS_SETTINGS"),IDR_MENU2,m_hWnd);
	CRect rectDlg;
	int X, Y;
	X = GetSystemMetrics(SM_CXFULLSCREEN);
	Y = GetSystemMetrics(SM_CYFULLSCREEN);
	GetWindowRect(rectDlg);
	int nXPos = (X/2)-(rectDlg.Width()/2);
	int nYPos = Y - rectDlg.Height();
	::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
	return TRUE;
}

void CAxis2LBar::OnCancel() 
{
	this->ShowWindow(SW_HIDE);
	Main->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
	CDialog::OnCancel();
}
void CAxis2LBar::OnBnClickedToolgeneral()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppGeneralTab);
	cb_general.SetTooltipText(CMsg("IDS_GENERAL")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppGeneralTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppGeneralTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppGeneralTab->m_dcDialogPage->Create(IDD_GENERAL_TAB);
		dlg->RemovePage(Main->m_pcppGeneralTab);
	}
}

void CAxis2LBar::OnBnClickedTooltravel()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppTravelTab);
	cb_travel.SetTooltipText(CMsg("IDS_TRAVEL")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppTravelTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppTravelTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppTravelTab->m_dcDialogPage->Create(IDD_TRAVEL_TAB);
		dlg->RemovePage(Main->m_pcppTravelTab);
	}
}

void CAxis2LBar::OnBnClickedToolspawn()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppSpawnTab);
	cb_spawn.SetTooltipText(CMsg("IDS_SPAWN")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppSpawnTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppSpawnTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppSpawnTab->m_dcDialogPage->Create(IDD_SPAWN_TAB);
		dlg->RemovePage(Main->m_pcppSpawnTab);
	}
}

void CAxis2LBar::OnBnClickedToolnpctweak()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppPlayerTweakTab);
	cb_npctweak.SetTooltipText(CMsg("IDS_PLAYER_TWEAK")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppPlayerTweakTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppPlayerTweakTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppPlayerTweakTab->m_dcDialogPage->Create(IDD_PLAYERTWEAK_TAB);
		dlg->RemovePage(Main->m_pcppPlayerTweakTab);
	}
}

void CAxis2LBar::OnBnClickedToolitems()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppItemTab);
	cb_items.SetTooltipText(CMsg("IDS_ITEMS")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppItemTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppItemTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppItemTab->m_dcDialogPage->Create(IDD_ITEMS_TAB);
		dlg->RemovePage(Main->m_pcppItemTab);
	}
}

void CAxis2LBar::OnBnClickedToolitemtweak()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppItemTweakTab);
	cb_itemtweak.SetTooltipText(CMsg("IDS_ITEM_TWEAK")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppItemTweakTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppItemTweakTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppItemTweakTab->m_dcDialogPage->Create(IDD_ITEMTWEAK_TAB);
		dlg->RemovePage(Main->m_pcppItemTweakTab);
	}
}

void CAxis2LBar::OnBnClickedToolaccount()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppAccountTab);
	cb_account.SetTooltipText(CMsg("IDS_ACCOUNT")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppAccountTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppAccountTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppAccountTab->m_dcDialogPage->Create(IDD_ACCOUNT_TAB);
		dlg->RemovePage(Main->m_pcppAccountTab);
	}
}

void CAxis2LBar::OnBnClickedToolmisc()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppMiscTab);
	cb_misc.SetTooltipText(CMsg("IDS_MISC")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppMiscTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppMiscTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppMiscTab->m_dcDialogPage->Create(IDD_MISC_TAB);
		dlg->RemovePage(Main->m_pcppMiscTab);
	}
}

void CAxis2LBar::OnBnClickedToollauncher()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppLauncherTab);
	cb_launcher.SetTooltipText(CMsg("IDS_LAUNCHER")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppLauncherTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppLauncherTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppLauncherTab->m_dcDialogPage->Create(IDD_LAUNCHER_TAB);
		dlg->RemovePage(Main->m_pcppLauncherTab);
	}
}

void CAxis2LBar::OnBnClickedToolcommands()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppCommandsTab);
	cb_commands.SetTooltipText(CMsg("IDS_COMMANDS")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppCommandsTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppCommandsTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppCommandsTab->m_dcDialogPage->Create(IDD_COMMANDS_TAB);
		dlg->RemovePage(Main->m_pcppCommandsTab);
	}
}

void CAxis2LBar::OnBnClickedToolreminder()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppReminderTab);
	cb_reminder.SetTooltipText(CMsg("IDS_REMINDER")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppReminderTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppReminderTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppReminderTab->m_dcDialogPage->Create(IDD_REMINDER_TAB);
		dlg->RemovePage(Main->m_pcppReminderTab);
	}
}

void CAxis2LBar::OnBnClickedToollogs()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->SetActivePage(Main->m_pcppAxisLogTab);
	cb_logs.SetTooltipText(CMsg("IDS_LOGS")+" "+CMsg("IDS_TAB"));
	if (Main->m_pcppAxisLogTab->m_dcCurrentPage->m_bModifyDlgStylesAndPos != true)
	{
		Main->m_pcppAxisLogTab->m_dcDialogPage->m_bModifyDlgStylesAndPos = true;
		Main->m_pcppAxisLogTab->m_dcDialogPage->Create(IDD_LOG_TAB);
		dlg->RemovePage(Main->m_pcppAxisLogTab);
	}
}

void CAxis2LBar::OnBnClickedToolsettings()
{
	cb_settings.SetTooltipText(CMsg("IDS_SETTINGS"));
}

void CAxis2LBar::OnToolbarmenuSettings()
{
	CSettingsDlg dlg(CMsg("IDS_SETTINGS"));
	dlg.m_psh.dwFlags = PSH_NOAPPLYNOW | PSH_PROPSHEETPAGE;
	dlg.EnableStackedTabs( false );
	dlg.AddPage(Main->m_pcppSetGeneral);
	dlg.AddPage(Main->m_pcppSetPaths);
	dlg.AddPage(Main->m_pcppSetItem);
	dlg.AddPage(Main->m_pcppSetTravel);
	dlg.AddPage(Main->m_pcppSetSpawn);
	dlg.AddPage(Main->m_pcppSetOverridePaths);

	dlg.SetActivePage(0);
	dlg.DoModal();
}

void CAxis2LBar::OnToolbarmenuProfile()
{
	if (Main->m_dlgProfile)
		delete Main->m_dlgProfile;
	Main->m_dlgProfile = new CProfileDLG;
	Main->m_dlgProfile->Create(IDD_PROFILE_DLG);
}

void CAxis2LBar::OnToolbarmenuHelp()
{
	HtmlHelp((DWORD_PTR)"Axis2.chm::/Welcome.htm", HH_DISPLAY_TOPIC);
}

void CAxis2LBar::OnToolbarmenuExit()
{
	CPropertySheet * dlg = (CPropertySheet*)this->GetParent();
	dlg->PressButton(PSBTN_OK);
}
