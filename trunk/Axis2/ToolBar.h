#pragma once
#include "btnst.h"
#include "afxwin.h"

// CAxis2LBar dialog

class CAxis2LBar : public CDialog
{
	DECLARE_DYNAMIC(CAxis2LBar)

public:
	CAxis2LBar(CWnd* pParent = NULL);
	virtual ~CAxis2LBar();
	virtual void OnCancel();

// Dialog Data
	enum { IDD = IDD_TOOLBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedToolgeneral();
	afx_msg void OnBnClickedTooltravel();
	afx_msg void OnBnClickedToolspawn();
	afx_msg void OnBnClickedToolnpctweak();
	afx_msg void OnBnClickedToolitems();
	afx_msg void OnBnClickedToolitemtweak();
	afx_msg void OnBnClickedToolaccount();
	afx_msg void OnBnClickedToolmisc();
	afx_msg void OnBnClickedToollauncher();
	afx_msg void OnBnClickedToolcommands();
	afx_msg void OnBnClickedToolreminder();
	afx_msg void OnBnClickedToollogs();
	afx_msg void OnBnClickedToolsettings();
	CButtonST cb_general;
	CButtonST cb_spawn;
	CButtonST cb_travel;
	CButtonST cb_npctweak;
	CButtonST cb_items;
	CButtonST cb_itemtweak;
	CButtonST cb_account;
	CButtonST cb_misc;
	CButtonST cb_launcher;
	CButtonST cb_commands;
	CButtonST cb_reminder;
	CButtonST cb_logs;
	CButtonST cb_settings;
	afx_msg void OnToolbarmenuSettings();
	afx_msg void OnToolbarmenuProfile();
	afx_msg void OnToolbarmenuHelp();
	afx_msg void OnToolbarmenuExit();
};
