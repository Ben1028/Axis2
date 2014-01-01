#pragma once

// CScriptInfo dialog

class CScriptInfo : public CDialog
{
	DECLARE_DYNAMIC(CScriptInfo)

public:
	CScriptInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScriptInfo();
	void AddText(CString csText, CHARFORMAT& cf);

	CHARFORMAT	m_cfKey;
	CHARFORMAT	m_cfValue;

	CSObject * pObject;

// Dialog Data
	enum { IDD = IDD_SCRIPT_INFO };
	CRichEditCtrl m_ceScreenInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
