#pragma once


// CDockingPage dialog

class CDockingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDockingPage)

public:
	CDockingPage(){ ASSERT(FALSE); }
	CDockingPage(UINT id, CString csCaption);
	virtual ~CDockingPage();
	bool m_bModifyDlgStylesAndPos;
	bool m_bAlwaysColor;
	CDockingPage * m_dcPropertyPage;
	CDockingPage * m_dcDialogPage;
	CDockingPage * m_dcCurrentPage;
	CString csTitle;
	UINT template_id;

	virtual BOOL OnSetActive();
	virtual void OnCancel();
	void FixTabs(UINT iTab) ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PreSubclassWindow();

	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};
