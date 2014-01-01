// ScriptEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Axis2.h"
#include "ScriptEditor.h"


// CScriptEditor dialog

IMPLEMENT_DYNAMIC(CScriptEditor, CDialog)

static DWORD CALLBACK SaveCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*) dwCookie;

   pFile->Write(pbBuff, cb);
   *pcb = cb;

   return 0;
}

CScriptEditor::CScriptEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptEditor::IDD, pParent)
{
}

CScriptEditor::~CScriptEditor()
{
}

void CScriptEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCRIPT_PAGE, m_ceScriptPage);
}


BEGIN_MESSAGE_MAP(CScriptEditor, CDialog)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
END_MESSAGE_MAP()

// CScriptEditor message handlers

BOOL CScriptEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();

		CString csTitle;
		csTitle.Format("Script Editor - %s",pObject->m_csID);
		this->SetWindowText(csTitle);

		CStdioFile * pScript = new CStdioFile;
		if ( !pScript->Open(pObject->m_csFilename, CFile::modeRead | CFile::shareDenyNone) )
		{
			Main->m_log.Add(1,"ERROR: Unable to open file %s for editing", pObject->m_csFilename);
			CString csMessage;
			csMessage.Format("ERROR: Unable to open file %s for editing", pObject->m_csFilename);
			AfxMessageBox(csMessage,MB_ICONHAND);
			CDialog::OnCancel();
		}
		BOOL bStatus = TRUE;

		CString csFindLine;
		switch(pObject->m_bType)
		{
			case TYPE_ITEM:
				csFindLine.Format("[ITEMDEF %s]",pObject->m_csValue);
				break;
			case TYPE_TEMPLATE:
				csFindLine.Format("[TEMPLATE %s]",pObject->m_csValue);
				break;
			case TYPE_CHAR:
				csFindLine.Format("[CHARDEF %s]",pObject->m_csValue);
				break;
			case TYPE_SPAWN:
				csFindLine.Format("[SPAWN %s]",pObject->m_csValue);
				break;
			case TYPE_AREA:
				csFindLine.Format("[AREADEF %s]",pObject->m_csValue);
				break;
			default:
				csFindLine.Format("%s]",pObject->m_csValue);
				break;
		}

		csFindLine = csFindLine.MakeUpper();

		while ( bStatus )
		{
			CString csLine;
			bStatus = pScript->ReadString(csLine);

			if ( !bStatus )
				break;
			CString csLineTest = csLine.MakeUpper();
			if ( csLineTest.Find(csFindLine) != -1 )
				m_iPos = m_ceScriptPage.GetTextLengthEx(GTL_NUMCHARS);

			AddText(csLine,m_ceScriptPage.m_cfText);
			m_ceScriptPage.ParseLine(m_ceScriptPage.GetLineCount()-2);
		}
		pScript->Close();

		m_ceScriptPage.SetSel(m_iPos,m_iPos);
		m_ceScriptPage.SetOptions(ECOOP_OR,ECO_SAVESEL);
	return TRUE;
}

void CScriptEditor::OnSave()
{
		CStdioFile * pScript = new CStdioFile;
		if ( !pScript->Open(pObject->m_csFilename, CFile::modeWrite | CFile::shareDenyNone | CFile::modeCreate) )
		{
			Main->m_log.Add(1,"ERROR: Unable to Save %s", pObject->m_csFilename);
			CString csMessage;
			csMessage.Format("ERROR: Unable to Save %s", pObject->m_csFilename);
			AfxMessageBox(csMessage,MB_ICONHAND);
		}
		else
		{
			EDITSTREAM es;
			es.dwCookie = (DWORD_PTR) pScript;
			es.pfnCallback = SaveCallback; 
			m_ceScriptPage.StreamOut(SF_TEXT, es);
		}
		pScript->Close();
}

void CScriptEditor::AddText(CString csText, CHARFORMAT& cf)
{
	csText += "\n";

	if (m_ceScriptPage)
	{
		int nOldLines = 0, nNewLines = 0, nScroll = 0;
		long nInsertionPoint = m_ceScriptPage.GetWindowTextLength();
		nOldLines = m_ceScriptPage.GetLineCount();
		m_ceScriptPage.SetSel(nInsertionPoint, -1);
		m_ceScriptPage.SetSelectionCharFormat(cf);
		m_ceScriptPage.ReplaceSel(csText);
		nNewLines = m_ceScriptPage.GetLineCount();
		nScroll = nNewLines - nOldLines;
		m_ceScriptPage.LineScroll(nScroll);
		UpdateData(false);
	}
}
