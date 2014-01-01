// ScriptEditorFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ScriptEditorFrame.h"

#define COMMENT_START	_T("//")
#define HEADER_START	_T("[")
#define HEADER_END		_T("]")
#define VARIABLE_START	_T("<")
#define VARIABLE_END	_T(">")
#define TRIGGER_START	_T("ON=@")
#define END_LINE		_T("\r\n")

// CScriptEditorFrame

IMPLEMENT_DYNAMIC(CScriptEditorFrame, CRichEditCtrl)

CScriptEditorFrame::CScriptEditorFrame()
{
	m_bParseLine			= false;
	// Common values
	UINT	uiSize			= sizeof(CHARFORMAT);
	DWORD	dwMask			= CFM_COLOR | CFM_FACE | CFM_SIZE;
	LONG	lHeight			= 160;	// 8 point => 160 * (1/20)

	// Initialise the Header CHARFORMAT
	m_cfHeader.cbSize			= uiSize;
	m_cfHeader.dwMask			= dwMask;
	m_cfHeader.dwEffects		= 0;
	m_cfHeader.yHeight			= lHeight;
	m_cfHeader.crTextColor		= RGB(155, 0, 0);
	strcpy(m_cfHeader.szFaceName,	_T("Courier New"));

	// Initialise the Text CHARFORMAT
	m_cfText.cbSize			= uiSize;
	m_cfText.dwMask			= dwMask;
	m_cfText.dwEffects		= 0;
	m_cfText.yHeight			= lHeight;
	m_cfText.crTextColor	= RGB(0, 0, 0);
	strcpy(m_cfText.szFaceName,	_T("Courier New"));

	// Initialise the Trigger CHARFORMAT
	m_cfTrigger.cbSize			= uiSize;
	m_cfTrigger.dwMask			= dwMask;
	m_cfTrigger.dwEffects		= 0;
	m_cfTrigger.yHeight			= lHeight;
	m_cfTrigger.crTextColor		= RGB(0,0,200);
	strcpy(m_cfTrigger.szFaceName,	_T("Courier New"));

	// Initialise the Variable CHARFORMAT
	m_cfVar.cbSize			= uiSize;
	m_cfVar.dwMask			= dwMask;
	m_cfVar.dwEffects		= 0;
	m_cfVar.yHeight			= lHeight;
	m_cfVar.crTextColor		= RGB(200,0,200);
	strcpy(m_cfVar.szFaceName,	_T("Courier New"));

	// Initialise the Comments CHARFORMAT
	m_cfComment.cbSize			= uiSize;
	m_cfComment.dwMask			= dwMask;
	m_cfComment.dwEffects		= 0;
	m_cfComment.yHeight			= lHeight;
	m_cfComment.crTextColor		= RGB(0, 200, 0);
	strcpy(m_cfComment.szFaceName,	_T("Courier New"));

}

CScriptEditorFrame::~CScriptEditorFrame()
{
}


BEGIN_MESSAGE_MAP(CScriptEditorFrame, CRichEditCtrl)
	ON_WM_GETDLGCODE()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
END_MESSAGE_MAP()



// CScriptEditorFrame message handlers

void CScriptEditorFrame::OnChange() 
{
	// Parse the current line if necessary
	if(true == m_bParseLine)
	{
		SetRedraw(FALSE);
		ParseLine();
		m_bParseLine = false;
		SetRedraw(TRUE);
		Invalidate(FALSE);
	}
}

UINT CScriptEditorFrame::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}

void CScriptEditorFrame::PreSubclassWindow() 
{
	CRichEditCtrl::PreSubclassWindow();

	// Set the event mask to include ENM_CHANGE
	long lMask = GetEventMask();
	lMask |= ENM_CHANGE;
	SetEventMask(lMask);
}

int CScriptEditorFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the event mask to include ENM_CHANGE
	long lMask = GetEventMask();
	lMask |= ENM_CHANGE;
	SetEventMask(lMask);

	return 0;
}

void CScriptEditorFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_RETURN :
		break;
	case VK_TAB :
		break;
	default :
		m_bParseLine = true;
		break;
	}

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CScriptEditorFrame::ParseLine(int nLineIndex)
{
	// Store the current selection position
	CHARRANGE crCurrent;
	GetSel(crCurrent);

	// Get the line index for the current line (if -1 specified)
	if(-1 == nLineIndex)
	{
		nLineIndex = LineFromChar(nLineIndex);
	}

	// Get the start and end point of the specified line
	long lSelStart	= LineIndex(nLineIndex);
	SetSel(lSelStart,GetTextLengthEx(GTL_NUMCHARS));
	CString strLineText = GetSelText();
	strLineText = strLineText.SpanExcluding(_T("\r\n"));
	strLineText = strLineText.SpanExcluding(_T("\n"));
	strLineText = strLineText.SpanExcluding(_T("\r"));
	int nLineLength = strLineText.GetLength();
	long lSelEnd	= lSelStart + nLineLength;
	SetSel(lSelStart,lSelEnd);

	if(nLineLength > 0)
	{
		// Find the starting position of Headers, Triggers and Comments
		CString strLineTextTest = strLineText.MakeUpper();
		int nCommentStart	= strLineTextTest.Find(COMMENT_START);
		int nHeaderStart	= strLineTextTest.Find(HEADER_START);
		int nTriggerStart	= strLineTextTest.Find(TRIGGER_START);
		int nVarStart		= strLineTextTest.Find(VARIABLE_START);

		// Colour as Text if there are no Headers, Triggers or Comments
		if((-1 == nCommentStart) && (-1 == nHeaderStart) && (-1 == nTriggerStart) && (-1 == nVarStart))
			SetSelectionCharFormat(m_cfText);
		else
		{
			// Colour the whole line as Text first
			SetSelectionCharFormat(m_cfText);

			// Then colour any Header
			ColourLine(CString(HEADER_START), CString(HEADER_END), 
				strLineTextTest, lSelStart, 0, m_cfHeader);
			// Then colour any Trigger
			ColourLine(CString(TRIGGER_START), CString(END_LINE), 
				strLineTextTest, lSelStart, 0, m_cfTrigger);
			// Then colour any Variables
			ColourLine(CString(VARIABLE_START), CString(VARIABLE_END), 
				strLineTextTest, lSelStart, 0, m_cfVar);
			// Then colour any Comments
			ColourLine(CString(COMMENT_START), CString(END_LINE), 
				strLineTextTest, lSelStart, 0, m_cfComment);
		}
	}
	// Restore the original selection
	SetSel(crCurrent);
}

bool CScriptEditorFrame::ColourLine(CString& strStart, CString& strEnd, 
									  CString& strLine, long lCharStart, 
									  int nOffset, CHARFORMAT& cf)
{
	// Look for the start marker
	int nStart	= -1;
	int nEnd	= -1;
	nStart = strLine.Find(strStart, nOffset);
	if(-1 != nStart)
	{
		nEnd = nStart;
		// Start marker found. Loop through each start marker
		while(-1 != nStart)
		{
			// Look for corresponding end marker
			nEnd = strLine.Find(strEnd, nEnd + strStart.GetLength());
			//nEnd = strLine.Find(strEnd, nStart + strStart.GetLength());
			if(-1 != nEnd)
			{
				// End marker found. Colour this section.
				SetSel(lCharStart + nStart, 
					lCharStart + nEnd + strEnd.GetLength());
				SetSelectionCharFormat(cf);
				
				// Look for next start marker
				nStart = strLine.Find(strStart, nStart + strStart.GetLength());
				//nStart = strLine.Find(strStart, nEnd + strStart.GetLength());
			}
			else
			{
				// End marker not found. Colour to end and return true.
				SetSel(lCharStart + nStart, lCharStart + strLine.GetLength());
				SetSelectionCharFormat(cf);
				return true;	// Waiting for end marker
			}
		}
	}
	return false;	// Not waiting for end marker
}