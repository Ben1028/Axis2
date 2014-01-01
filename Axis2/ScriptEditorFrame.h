#pragma once


// CScriptEditorFrame

class CScriptEditorFrame : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CScriptEditorFrame)

public:
	CScriptEditorFrame();
	virtual ~CScriptEditorFrame();

	void ParseLine(int nLineIndex = -1);
	bool ColourLine(CString& strStart, CString& strEnd, CString& strLine,
		long lCharStart, int nOffset, CHARFORMAT& cf);

	CHARFORMAT	m_cfHeader;
	CHARFORMAT	m_cfText;
	CHARFORMAT	m_cfTrigger;
	CHARFORMAT	m_cfComment;
	CHARFORMAT	m_cfVar;
	bool		m_bParseLine;

protected:
	// Overrides
	virtual void PreSubclassWindow();

	// Generated message map functions
	afx_msg UINT OnGetDlgCode();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	DECLARE_MESSAGE_MAP()
};

