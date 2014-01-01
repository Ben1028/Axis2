#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl Class
// Written by: Brooks Younce
// Date: Jan 19 2007
// XP style compatability idea modified from Luuk Weltevreden http://www.codeproject.com/wtl/ThemedDialog.asp
// This class allows easy inserting of controls into your Tab control, with minimal coding.
// Free for public use, but must not be misrepresented, please leave comments attached.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define P_TOP	0x10000000L //lineup to the top of the last control
#define P_BELOW	0x80000000L	//lineup to below last control
#define P_RIGHT	0x40000000L //lineup to the right of the last control
#define P_LEFT	0x20000000L //lineup to the left of the last control

#define MARGIN_TOP 27
#define MARGIN_LEFT 7

#define WM_BUTTONPRESSED WM_USER+500
#define WM_UPDOWN WM_USER+501
// CMyTabCtrl dialog

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();


protected:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:

	HBRUSH m_hBrush;
	BOOL m_bThemeActive;
	CObArray obArray;		//to keep track of objects which need deleted later
	int iLastBottom;		//for autospacing (bottom pos of last control inserted)
	int iLastRight;			//for autospacing (right pos of last control inserted)
	int iLastLeft;			//for autospacing (left pos of last control inserted)
	int iLastTop;			//for autospacing (top pos of last control inserted)
	int iCurrentTab;		//current tab in focus
	int iCtrl;				//current control in focus

	void Init();			//initialize the tab control
	void ShowTab(int iTab);
	void ClearTab(int iTab);
	void ResetTab(int iTab);
	void SpreadTabs(int iTab);
	void FocusTab(int iTab);
	void SortButtons(int iTab);
	void RemoveButton(int nID);
	int GetButtonCount(int iTab);
	LRESULT ButtonPressed(WPARAM w, LPARAM l);	//mouse button clicked on a button control
	LRESULT UpDownButton(WPARAM w, LPARAM l);		//down button pressed on keyboard
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

	//control creation functions
	void CreateButton(LPCTSTR sCaption, int nID, int iTab, UINT uLocation = 0, int iX = 0, int iY = 0, int iLen = 50);

	//placment helpers
	int BottomOf(int nID);
	int RightOf(int nID);
	int LeftOf(int nID);
	int TopOf(int nID);

	//get value helpers
	CString GetItemText(int nID);
	int GetItemTextLength(int nID);
	int GetItemTextAsInt(int nID);
	BOOL GetItemCheckState(int nID);

	struct ITEM
	{
		CWnd* wnd;		//our control
		int iTab;		//which tab it belongs to
		BOOL bTabStop;	//does this control allow tabstop
		int iID;		//ID of the control
	};
	
};
