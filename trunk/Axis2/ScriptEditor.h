#pragma once

#include "ScriptEditorFrame.h"

// CScriptEditor dialog

class CScriptEditor : public CDialog
{
	DECLARE_DYNAMIC(CScriptEditor)

public:
	CScriptEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScriptEditor();
	void AddText(CString csText, CHARFORMAT& cf);

	CSObject * pObject;
	int m_iPos;

// Dialog Data
	enum { IDD = IDD_SCRIPT_EDITOR };
	CScriptEditorFrame m_ceScriptPage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnSave();


	DECLARE_MESSAGE_MAP()
};
