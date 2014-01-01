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

// Common.cpp : implementation file
//


#include "StdAfx.h"
#include "Axis2.h"
#include "Common.h"
#include "ScriptObjects.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char * pMulFileBlocks[VERFILE_QTY] =	// static
{
	"map0.mul",
	"map1.mul",
	"map2.mul",
	"map3.mul",
	"map4.mul",
	"map5.mul",

	"mapdif0.mul",
	"mapdif1.mul",
	"mapdif2.mul",
	"mapdif3.mul",
	"mapdif4.mul",
	"mapdif5.mul",

	"mapdifl0.mul",
	"mapdifl1.mul",
	"mapdifl2.mul",
	"mapdifl3.mul",
	"mapdifl4.mul",
	"mapdifl5.mul",

	"statics0.mul",
	"statics1.mul",
	"statics2.mul",
	"statics3.mul",
	"statics4.mul",
	"statics5.mul",

	"staidx0.mul",
	"staidx1.mul",
	"staidx2.mul",
	"staidx3.mul",
	"staidx4.mul",
	"staidx5.mul",

	"stadif0.mul",
	"stadif1.mul",
	"stadif2.mul",
	"stadif3.mul",
	"stadif4.mul",
	"stadif5.mul",

	"stadifi0.mul",
	"stadifi1.mul",
	"stadifi2.mul",
	"stadifi3.mul",
	"stadifi4.mul",
	"stadifi5.mul",

	"stadifl0.mul",
	"stadifl1.mul",
	"stadifl2.mul",
	"stadifl3.mul",
	"stadifl4.mul",
	"stadifl5.mul",

	"art.mul",
	"artidx.mul",

	"anim.mul",
	"anim.idx",
	"anim2.mul",
	"anim2.idx",
	"anim3.mul",
	"anim3.idx",
	"anim4.mul",
	"anim4.idx",
	"anim5.mul",
	"anim5.idx",

	"lightidx.mul",
	"light.mul",

	"tiledata.mul",

	"hues.mul",

	"radarcol.mul",

	"sound.mul",
	"soundidx.mul",
	"config.txt", //Music list

	"bodyconv.def",
	"body.def",

	"multi.idx",
	"multi.mul",

	"map0LegacyMUL.uop",
	"map1LegacyMUL.uop",
	"map2LegacyMUL.uop",
	"map3LegacyMUL.uop",
	"map4LegacyMUL.uop",
	"map5LegacyMUL.uop",

	"artLegacyMUL.uop",

};

CString GetMulFileName(int iIndex)
{
	CString csRet;
	if (iIndex >= VERFILE_QTY)
	{
		//Do nothing...  Not set.
	}
	else
	{
		csRet = pMulFileBlocks[iIndex];
	}
	return csRet;
}

//******************
//Default Table Objects


CTObject::CTObject()
{
	m_bType = 0;
}

CTObject::~CTObject()
{
}


//******************
//Default Script Objects

const char * pSObjectTags [11] =
{
	"CATEGORY",		//0
	"SUBSECTION",	//1
	"DESCRIPTION",	//2
	"DUPEITEM",		//3
	"DEFNAME",		//4
	"ID",			//5
	"COLOR",		//6
	"NAME",			//7
	"GROUP",		//8
	"P",			//9
	"POINT",		//10
};

CSObject::CSObject()
{
	this->m_csCategory = "<none>";
	this->m_csSubsection = "<none>";
	this->m_csDescription = "<unnamed>";
}

CSObject::~CSObject()
{
}

CString CSObject::ReadBlock(CStdioFile &csfInput)
{
	CString csLine;
	BOOL bStatus = TRUE;
	BOOL bIgnore = FALSE;
	while ( bStatus )
	{
		bStatus = csfInput.ReadString(csLine);
		if ( !bStatus )
			break;
		if ( csLine.Find("[") == 0 )
		{
			break;
		}
		if ( ( csLine.Find("=") == -1) || (csLine.Find("=") == csLine.GetLength()))
			continue;
		if ( ( csLine.Find("ON=@") != -1) && ( ! _strcmpi( csLine.Mid(csLine.Find("ON=@")+4), "Create" )))
			bIgnore = FALSE;
		else if ( csLine.Find("ON=@") != -1)
			bIgnore = TRUE;
		else if ( csLine.Find("DESCRIPTION=@") != -1)
			bIgnore = FALSE;

		CString csTag = csLine.SpanExcluding("=");
		CString csValue = csLine.Mid(csLine.Find("=") + 1);
		csTag.Trim();
		csValue.Trim();
		switch (FindTable(csTag, pSObjectTags, 11))
		{
		case 0: //CATEGORY
			this->m_csCategory = csValue;
			break;
		case 1: //SUBSECTION
			this->m_csSubsection = csValue;
			break;
		case 2: //DESCRIPTION
			this->m_csDescription = csValue;
			break;
		case 3: //DUPEITEM
			this->m_csDupeItem = csValue;
			break;
		case 4: //DEFNAME
		{
			if(csValue.CompareNoCase(this->m_csValue) != 0)
			{
				this->m_csID = csValue;
				int iFind = Main->m_pScripts->m_aDefList.Find(csValue);
				if ( iFind != -1 )
				{
					CSObject * pFind = (CSObject *) Main->m_pScripts->m_aDefList.GetAt(iFind);
					pFind->m_csID = this->m_csValue;
				}
				else
				{
					CSObject * pDef = new CSObject;
					pDef->m_csValue = csValue;
					pDef->m_csID = this->m_csValue;
					Main->m_pScripts->m_aDefList.Insert(pDef);
				}
			}
		}
			break;
		case 5: //ID
		{
			if (bIgnore)
				break;

			if(csValue.CompareNoCase(this->m_csValue) != 0)
			{
				this->m_csDisplay = csValue;
				int iFind = Main->m_pScripts->m_aDefList.Find(this->m_csValue);
				if ( iFind != -1 )
				{
					CSObject * pFind = (CSObject *) Main->m_pScripts->m_aDefList.GetAt(iFind);
					pFind->m_csID = csValue;
				}
				else
				{
					CSObject * pDef = new CSObject;
					pDef->m_csValue = this->m_csValue;
					pDef->m_csID = csValue;
					Main->m_pScripts->m_aDefList.Insert(pDef);
				}
			}
		}
			break;
		case 6: //COLOR
		{
			if (bIgnore)
				break;
			this->m_csColor = csValue;
			break;
		}
		case 7: //NAME
		{
			if (bIgnore)
				break;
			if ((this->m_bType == TYPE_AREA)||(this->m_bType == TYPE_SPELL))
				this->m_csDescription = csValue;
		}
			break;
		case 8: //GROUP
		{
			if (bIgnore)
				break;
			if (this->m_bType == TYPE_AREA)
				this->m_csSubsection = csValue;
			break;
		}
		case 9: //P
		{
			if (bIgnore)
				break;
			if (this->m_bType == TYPE_AREA)
			{
				CString csX, csY, csZ, csM;
				csX = csValue.SpanExcluding(",");
				csY = csValue.Mid(csValue.Find(",") + 1);
				if (csY.Find(",") != -1)
					csZ = csY.Mid(csY.Find(",") + 1);
				else
					csZ = "0";
				csY = csY.SpanExcluding(",");
				if (csZ.Find(",") != -1)
					csM = csZ.Mid(csZ.Find(",") + 1);
				else
					csM = "0";
				csZ = csZ.SpanExcluding(",");
				this->m_csID = CFMsg("%1,%2,%3", csX, csY, csZ);
				this->m_csDisplay = csM;
				this->m_csCategory = CFMsg(CMsg("IDS_MAP_REGION"), atoi(csM));
			}
			break;
		}
		case 10: //POINT
		{
			if (bIgnore)
				break;
			if (this->m_bType == TYPE_AREA)
			{
				CString csX, csY, csZ, csM, csTemp;
				csX = csValue.SpanExcluding(",");
				csY = csValue.Mid(csValue.Find(",") + 1);
				if (csY.Find(",") != -1)
					csZ = csY.Mid(csY.Find(",") + 1);
				else
					csZ = "0";
				csY = csY.SpanExcluding(",");
				if (csZ.Find(",") != -1)
					csM = csZ.Mid(csZ.Find(",") + 1);
				else
					csM = "0";
				csZ = csZ.SpanExcluding(",");
				csTemp.Format("%s,%s,%s", csX, csY, csZ);
				this->m_csID = csTemp;
				this->m_csDisplay = csM;
				break;
			}
		}
		}
	}
	return csLine;
}

void CSObject::WriteBlock(CStdioFile &csfInput)
{
	CString csLine;
	switch (this->m_bType)
	{
	case 0:
	case 3:
		break;
	case 1:
		csLine.Format("[ITEMDEF %s] \n",this->m_csValue);
		break;
	case 2:
		csLine.Format("[CHARDEF %s] \n",this->m_csValue);
		break;
	case 4:
		csLine.Format("[AREADEF %s] \n",this->m_csValue);
		break;
	case 5:
		csLine.Format("[SPAWN %s] \n",this->m_csValue);
		break;
	case 6:
		csLine.Format("[TEMPLATE %s] \n",this->m_csValue);
		break;
	case 7:
		csLine.Format("[SPELL %s] \n",this->m_csValue);
		break;
	case 8:
		csLine.Format("[MULTIDEF %s] \n",this->m_csValue);
		break;
	}
	csfInput.WriteString(csLine);

	if (this->m_csID != "" )
	{
		csLine.Format("DEFNAME = %s \n",this->m_csID);
		csfInput.WriteString(csLine);
	}
	if (this->m_csDisplay != "" )
	{
		csLine.Format("ID = %s \n",this->m_csDisplay);
		csfInput.WriteString(csLine);
	}
	if (this->m_csCategory != "" )
	{
		csLine.Format("CATEGORY = %s \n",this->m_csCategory);
		csfInput.WriteString(csLine);
	}
	if (this->m_csSubsection != "" )
	{
		csLine.Format("SUBSECTION = %s \n",this->m_csSubsection);
		csfInput.WriteString(csLine);
	}
	if (this->m_csDescription != "" )
	{
		csLine.Format("DESCRIPTION = %s \n",this->m_csDescription);
		csfInput.WriteString(csLine);
	}
	if (this->m_csColor != "" )
	{
		csLine.Format("COLOR = %s \n",this->m_csColor);
		csfInput.WriteString(csLine);
	}
	if (this->m_csDupeItem != "" )
	{
		csLine.Format("DUPEITEM = %s \n",this->m_csDupeItem);
		csfInput.WriteString(csLine);
	}
	csfInput.WriteString("\n");
}

//******************
//Script Array

CScriptArray::CScriptArray()
{
}

CScriptArray::~CScriptArray()
{
	CPtrArray::RemoveAll();
}

CString CScriptArray::GetDef(CString csName)
{
	CSimpleArray<int> iLoopControl;
	int i = Main->m_pScripts->m_aDefList.Find(csName);
	while ( i != -1 )
	{
		iLoopControl.Add(i);
		CSObject * pTemp = (CSObject *) Main->m_pScripts->m_aDefList.GetAt(i);
		csName = pTemp->m_csID;
		i = Main->m_pScripts->m_aDefList.Find(csName);
		if (iLoopControl.Find(i) != -1)
			i = -1;
	}
	return csName;
}

int CScriptArray::Find(CString csName)
{
	if ( this->GetSize() == 0 )
		return -1;

	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CSObject * pTest = (CSObject *) this->GetAt(iIndex);
		CString csExisting = pTest->m_csValue;
		if ( csExisting.CompareNoCase(csName) == 0 )
				return (int)iIndex;
		if ( csExisting.CompareNoCase(csName) > 0 )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return -1;
}

int CScriptArray::Insert(CTObject * pScript)
{
	if (( pScript->m_csValue == "" )&&( !pScript->m_bCustom ))
	{
		delete pScript;
		return -1;
	}
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	if ( iUpper == -1 )
	{
		this->InsertAt(0, pScript);
		return 0;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CSObject * pTest = (CSObject *) this->GetAt(iIndex);
		if ( pTest->m_csValue.CompareNoCase(pScript->m_csValue) > 0 )
		{
			iCompare = 0;
			iUpper = iIndex - 1;
		}
		else
		{
			iCompare = 1;
			iLower = iIndex + 1;
		}
	}
	iIndex += iCompare;
	this->InsertAt(iIndex, (void *) pScript);
	return (int)iIndex;
}

//******************
//String Array

int CAdvStringArray::Find(CString csName)
{
	if ( this->GetSize() == 0 )
		return -1;
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CString csExisting = this->GetAt(iIndex);
		if ( csExisting.CompareNoCase(csName) == 0 )
			return (int)iIndex;
		if ( csExisting.CompareNoCase(csName) > 0 )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return -1;
}

int CAdvStringArray::Insert(CString csName)
{
	if ( csName == "" )
		return -1;
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	if ( iUpper == -1 )
	{
		this->InsertAt(0, csName);
		return 0;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CString csTest = this->GetAt(iIndex);
		if ( csTest.CompareNoCase(csName) > 0 )
		{
			iCompare = 0;
			iUpper = iIndex - 1;
		}
		else
		{
			iCompare = 1;
			iLower = iIndex + 1;
		}
	}
	iIndex += iCompare;
	this->InsertAt(iIndex, csName);
	return (int)iIndex;
}

CString CAdvStringArray::GetValue(CString csKey)
{
	if ( this->GetSize() == 0 )
		return _T("Error!");
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CString csEntry = this->GetAt(iIndex);
		csEntry = csEntry.SpanExcluding("=");
		if ( csEntry.CompareNoCase(csKey) == 0 )
		{
			csKey = this->GetAt(iIndex);
			CString csValue = csKey.Mid(csKey.Find("=") + 1);
			return csValue;
		}
		if ( csEntry.CompareNoCase(csKey) > 0 )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return csKey;
}

//******************
//Objects Categories

CCategory::CCategory()
{
	this->m_csName = "<none>";
}

CSubsection::CSubsection()
{
	this->m_csName = "<none>";
}

CCategory * FindCategory(CPtrList * pPtrList, CString csName, bool bCreate)
{
	CCategory * pCategory = NULL;
	if (!pPtrList->IsEmpty())
	{
		POSITION pos = pPtrList->GetHeadPosition();
		while (pos != NULL)
		{
			CCategory * pTest = (CCategory *) pPtrList->GetNext(pos);
			if (pTest->m_csName == csName)
			{
				pCategory = pTest;
				break;
			}
		}
	}
	if (pCategory == NULL && bCreate == true)
	{
		// Category didn't exist.  Create a new one
		pCategory = new (CCategory);
		pCategory->m_csName = csName;
		if (pPtrList->IsEmpty())
			pPtrList->AddHead(pCategory);
		else
			pPtrList->AddTail(pCategory);
	}
	return pCategory;
}

CSubsection * FindSubsection(CCategory * pCategory, CString csName, bool bCreate)
{
	CSubsection * pSubsection = NULL;
	if (!pCategory->m_SubsectionList.IsEmpty())
	{
		POSITION pos = pCategory->m_SubsectionList.GetHeadPosition();
		while (pos != NULL)
		{
			CSubsection * pTest = (CSubsection *) pCategory->m_SubsectionList.GetNext(pos);
			if (pTest->m_csName == csName)
			{
				pSubsection = pTest;
				break;
			}
		}
	}
	if (pSubsection == NULL && bCreate == true)
	{
		// Subsection didn't exist.  Create a new one
		pSubsection = new (CSubsection);
		pSubsection->m_csName = csName;
		if (pCategory->m_SubsectionList.IsEmpty())
			pCategory->m_SubsectionList.AddHead(pSubsection);
		else
			pCategory->m_SubsectionList.AddTail(pSubsection);
	}
	return pSubsection;
}


//******************


int FindTable( const char * pFind, const char * const * ppTable, int iCount, int iElemSize )
{
	for ( int i=0; i<iCount; i++ )
	{
		if ( ! _strcmpi( *ppTable, pFind ))
			return( i );
		ppTable = (const char * const *)((( const BYTE*) ppTable ) + iElemSize );
	}
	return( -1 );
}

DWORD alltoi( const char * pszStr, const char * pszStrComp, int iSkip)
{
	CString csText, csTextComp, csTemp;
	csText = pszStr;
	csTextComp = pszStrComp;
	DWORD dwTemp;

	if (csText == csTextComp)
		return(ahextoi(csText));
	
	int i = Main->m_pScripts->m_aDefList.Find(csText);
	if ( i != -1 )
	{
		CSObject * pTemp = (CSObject *) Main->m_pScripts->m_aDefList.GetAt(i);
		dwTemp = alltoi( pTemp->m_csID, csTextComp );
		return( dwTemp );
	}
	else
	{
		csText.Trim();
		if(csText.Find('{') != -1)
		{
			if (iSkip == 1)
			{
				csTemp = csText.Mid(csText.ReverseFind('{')+1);
				csTemp = csTemp.SpanExcluding("}");
				dwTemp = alltoi( csTemp, csTextComp );
			}
			else
			{
				csTemp = csText.Mid(csText.Find("{")+1);
				csTemp = csTemp.Left(csTemp.ReverseFind('}'));
				dwTemp = alltoi( csTemp, csTextComp, 1 );
			}
			return( dwTemp );
		}
		else if (csText.Find(' ') != -1)
		{
			DWORD dwMin, dwMax;
			csTemp = csText.SpanExcluding(" ");
			if(ahextoi(csTemp) != 0)
			{
				dwMin = ahextoi(csTemp);
				csTemp = csText.Mid(csText.Find(' ') + 1);
				dwMax = ahextoi(csTemp);
				return (getrand(dwMin,dwMax));
			}
			else
			{
				dwTemp = alltoi( csTemp, csTextComp );
				return( dwTemp );
			}

		}
		else
			return(ahextoi(csText));
	}
}

DWORD getrand( DWORD dwMin, DWORD dwMax )
{
	if(dwMin == dwMax)
		return dwMin;
	srand((unsigned int)time(NULL));
	return ((DWORD) (rand() % (dwMax - dwMin) ) + dwMin);
}

DWORD ahextoi( const char * pszStr ) // Convert hex string to integer
{
	// Unfortunatly the library func cant handle the number FFFFFFFF
	// char * sstop; return( strtol( s, &sstop, 16 ));

	GETNONWHITESPACE( pszStr );

	DWORD val = 0;
	CString cs_test = pszStr;
	if (cs_test.GetAt(0) != '0')
	{
		while ( 1 )
		{ 
			char ch = *pszStr;
				if (( ch > '32' && ch <'0' ) || ( ch > '9' && ch <'127' ))
				{
					val = 0;
					break;
				}
			if ( ch < '0' || ch > '9' ) break;

			ch -= '0';
			val *= 0x0a;
			val += ch;
			pszStr ++;
		}
		return( val );
	}
	else
	{
		while ( 1 )
		{
			char ch = *pszStr;
			if ( ch >= '0' && ch <= '9' )
				ch -= '0';
			else
			{
				ch |= 0x20;
				if (( ch > '32' && ch <'a' ) || ( ch > 'f' && ch <'127' ))
				{
					val = 0;
					break;
				}		

				if ( ch > 'f' || ch <'a' ) break;
				ch -= 'a' - 10;
			}
			val *= 0x10;
			val += ch;
			pszStr ++;
		}
		return( val );
	}
}

bool IsNumber( const char * pszStr )
{

	GETNONWHITESPACE( pszStr );
	CString cs_test = pszStr;
	while ( 1 )
	{ 
		char ch = *pszStr;
		if (( ch > '32' && ch <'0' ) || ( ch > '9' && ch <'127' )) return false;
		if ( ch < '0' || ch > '9' ) break;
		pszStr ++;
	}
	return true;
}

CString Encrypt(CString csValue)
{
	CString csKey = _T("LmheGeEBuBeiZuElnbiASnuZenCLMAKa");
	CString csEncrypted = csValue;

	for (int i = 0; i < csValue.GetLength(); i++)
	{
		csKey.SetAt(i, csKey[i] ^ 128);
		csEncrypted.SetAt(i, csValue[i] ^ csKey[i]);
	}
	
	return csEncrypted;
}

CString EncryptUO(CString csValue)
{
	CString csEncrypted = csValue;

	for (int i = 0; i < csValue.GetLength(); i++)
	{
		char cNew = csValue[i];
		cNew += 0x0d;
		if ( cNew > 0x7f )
			cNew -= 0x5f;
		csEncrypted.SetAt(i, cNew);
	}
	return csEncrypted;
}

bool ReplaceFiles( CString csNewFile, CString csOldFile )
{
	// Loop through this a few times
	int i = 0;
	bool bSuccess = false;
	while ( i < 5 && !bSuccess )
	try
	{
		CFileStatus st;
		if ( CFile::GetStatus( csOldFile, st ) )
			CFile::Remove( csOldFile );
		CFile::Rename( csNewFile, csOldFile );
		bSuccess = true;
	}
	catch ( CFileException * e )
	{
		i++;
		if ( i < 5 )
		{
			Sleep( 500 );
			e->Delete();
		}
		else
		{
			CString csMessage;
			char szCause[MAX_BUFFER];
			memset(szCause, 0x00, sizeof(szCause));
			e->GetErrorMessage( &szCause[0], MAX_BUFFER - 1 );
			csMessage.Format(CMsg("IDS_FILE_EXMOVE"), szCause);
			csMessage+= CMsg("IDS_FILE_RENAME");
			e->Delete();
			if ( AfxMessageBox( csMessage, MB_YESNO | MB_ICONQUESTION ) == IDYES )
			{
				CString csBackup;
				CTime ct = CTime::GetCurrentTime();
				csBackup.Format("%s.%s", csOldFile, ct.Format("%Y%m%d%H%M%S"));
				try
				{
					CFile::Rename( csOldFile, csBackup );
					CFile::Rename( csNewFile, csOldFile );
					CString csMessage;
					csMessage.Format(CMsg("IDS_FILE_RENAMESUCCESS"), csBackup);
					AfxMessageBox(csMessage, MB_OK | MB_ICONINFORMATION);
					return true;
				}
				catch ( CFileException * e )
				{
					e->Delete();
					AfxMessageBox(CMsg("IDS_FILE_RENAMEFAILED"), MB_OK | MB_ICONSTOP );
					return false;
				}
			}
			return false;
		}
	}
	return true;
}

UINT LoadProfileThread(LPVOID pParam)
{
	INT_PTR idefault = (INT_PTR)pParam;
	CString sProfile;
	if (idefault)
		sProfile = Main->GetRegistryString("Default Profile");
	else
	{
		sProfile = Main->m_csCurentProfile;
	}

	Main->m_pScripts->LoadProfile(sProfile);
	if (sProfile != CMsg("IDS_NONE"))
	{
		Main->PutRegistryString("Last Profile Loaded", sProfile);
		Main->UpdateProfileMenu();
	}
	Main->UpdateProfileSettings();
	return 0;
}

UINT TrackingThread(LPVOID pParam)
{
	UNREFERENCED_PARAMETER(pParam);
	DWORD dwProcessID;
	HANDLE m_hProcess;
	LPVOID lpAddress;
	CString csCoords;
	byte * pointer = new byte[Main->sizeX];
	byte * pointer1 = new byte[Main->sizeY];
	byte * pointer2 = new byte[Main->sizeZ];
	byte * pointer3 = new byte[Main->sizeM];

	if (!Main->calibrated)
		Calibrate();

	while (Main->calibrated)
	{
		EnumWindows(EnumWindowsProc, 1);
		if ((Main->bMemoryAccess) && (hwndUOClient))
		{

			GetWindowThreadProcessId(hwndUOClient, &dwProcessID);
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);

			
			lpAddress = (LPVOID)((DWORD_PTR)Main->LocX);
			ReadProcessMemory(m_hProcess, lpAddress, pointer, Main->sizeX, NULL);
			int x = (short)(pointer[0] | (pointer[1] << 8));

			lpAddress = (LPVOID)((DWORD_PTR)Main->LocY);
			ReadProcessMemory(m_hProcess, lpAddress, pointer1, Main->sizeY, NULL);
			int y = (short)(pointer1[0] | (pointer1[1] << 8));

			lpAddress = (LPVOID)((DWORD_PTR)Main->LocZ);
			ReadProcessMemory(m_hProcess, lpAddress, pointer2, Main->sizeZ, NULL);
			int z = (short)(pointer2[0] | (pointer2[1] << 8));

			lpAddress = (LPVOID)((DWORD_PTR)Main->LocM);
			ReadProcessMemory(m_hProcess, lpAddress, pointer3, Main->sizeM, NULL);
			int m = (byte)pointer3[0];

			csCoords.Format("%d",m);
			CTravelTab * pTravel = (CTravelTab *)Main->m_pcppTravelTab->m_dcCurrentPage;
			pTravel->m_ceMapPlane.SetWindowText(csCoords);

			pTravel->RemoteRecenter(x,y,z);
			CloseHandle( m_hProcess );
			Sleep(1000);
		}
		else
		{
			Main->calibrated = false;
			break;
		}
	}
	return 0;
}

bool Calibrate()
{
	int LocX = 0;
	int sizeX = 0;
	int LocY = 0;
	int sizeY = 0;
	int LocZ = 0;
	int sizeZ = 0;
	int LocM = 0;
	int sizeM = 0;

	//Location
	byte mask[] = { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00 };
	byte vals[] = { 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0x66, 0x89, 0x5A, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x66, 0x89, 0x78, 0x00, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x66, 0x89, 0x71, 0x00 };
	byte ptrX[] = { 0x02, 0x04, 0x04, 0x0C, 0x01, 0x02 };
	byte ptrY[] = { 0x0E, 0x04, 0x04, 0x15, 0x01, 0x02 };
	byte ptrZ[] = { 0x18, 0x04, 0x04, 0x1F, 0x01, 0x02 };
	//Map
	byte mask2[] = { 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	byte vals2[] = { 0xA0, 0x00, 0x00, 0x00, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x0D };
	byte ptrM[] = { 0x01, 0x04, 0xFF, 0xFF, 0xFF, 0x01 };
	int ptr = CalibrateEntry(mask, vals, sizeof(mask));

	if ( ptr != 0 )
	{
		LocX = CalibrateCoord(ptrX, ptr); sizeX = ptrX[5];
		LocY = CalibrateCoord(ptrY, ptr); sizeY = ptrY[5];
		LocZ = CalibrateCoord(ptrZ, ptr); sizeZ = ptrZ[5];
	}
	int ptr2 = CalibrateEntry(mask2, vals2, sizeof(mask2));
	if ( ptr2 != 0 )
		LocM = CalibrateCoord(ptrM, ptr2); sizeM = ptrM[5];

	//New Clients Calibration
	if (( LocX ==0 )||( LocY ==0 )||( LocZ ==0 )||( LocM ==0 ))
	{
		byte mask3[] = { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF };
		byte vals3[] = { 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00, 0x53, 0x8B, 0x1D, 0x34, 0x50, 0x61, 0x00, 0x55, 0x8B, 0x2d };
		byte ptrXYZ[] = { 0x02, 0x04, 0xFF, 0xFF, 0xFF };

		int ptr3 = CalibrateEntry(mask3, vals3, sizeof(mask3));

		if ( ptr3 != 0 )
		{
			LocX = CalibrateCoord(ptrXYZ, ptr3); sizeX = 2;
			LocY = LocX-4; sizeY = 2;
			LocZ = LocX-8; sizeZ = 2;
			LocM = LocX+4; sizeM = 1;
		}
	}

	if (( LocX !=0 )&&( LocY !=0 )&&( LocZ !=0 )&&( LocM !=0 ))
	{
		Main->LocX = LocX;
		Main->LocY = LocY;
		Main->LocZ = LocZ;
		Main->LocM = LocM;
		Main->sizeX = sizeX;
		Main->sizeY = sizeY;
		Main->sizeZ = sizeZ;
		Main->sizeM = sizeM;
		Main->calibrated = true;
		return true;
	}
	Main->calibrated = false;
	return false;
}

int CalibrateCoord(byte * ptrX, int MaskLocation)
{
	EnumWindows(EnumWindowsProc, 1);
	if ((Main->bMemoryAccess) && (hwndUOClient))
	{
		DWORD dwProcessID;
		HANDLE m_hProcess;
		LPVOID lpAddress;
		int coordPointer = 0;

		GetWindowThreadProcessId(hwndUOClient, &dwProcessID);
		m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);

		if (MaskLocation)
		{
			lpAddress = (LPVOID)((DWORD_PTR)MaskLocation + ptrX[0]);
			byte * pointer = new byte[ptrX[1]];
			ReadProcessMemory(m_hProcess, lpAddress, pointer, ptrX[1], NULL);
			switch (ptrX[1])
			{
				case 1: coordPointer = (byte)pointer[0]; break;
				case 2: coordPointer = (short)(pointer[0] | (pointer[1] << 8)); break;
				case 4: coordPointer = (int)(pointer[0] | (pointer[1] << 8) | (pointer[2] << 16) | (pointer[3] << 24)); break;
			}
			byte * pointer2 = new byte[ptrX[2]];
			if ( ptrX[2] < 0xFF )
			{
				lpAddress = (LPVOID)((DWORD_PTR)coordPointer);
				ReadProcessMemory(m_hProcess, lpAddress, pointer2, ptrX[2], NULL);
				switch (ptrX[2])
				{
					case 1: coordPointer = (byte)pointer2[0]; break;
					case 2: coordPointer = (short)(pointer2[0] | (pointer2[1] << 8)); break;
					case 4: coordPointer = (int)(pointer2[0] | (pointer2[1] << 8) | (pointer2[2] << 16) | (pointer2[3] << 24)); break;
				}
			}
			byte * pointer3 = new byte[ptrX[4]];
			if ( ptrX[3] < 0xFF )
			{
				lpAddress = (LPVOID)((DWORD_PTR)MaskLocation + ptrX[3]);
				ReadProcessMemory(m_hProcess, lpAddress, pointer3, ptrX[4], NULL);
				switch (ptrX[4])
				{
					case 1: coordPointer += (byte)pointer3[0]; break;
					case 2: coordPointer += (short)(pointer3[0] | (pointer3[1] << 8)); break;
					case 4: coordPointer += (int)(pointer3[0] | (pointer3[1] << 8) | (pointer3[2] << 16) | (pointer3[3] << 24)); break;
				}
			}
		}
		CloseHandle( m_hProcess );
		return coordPointer;
	}
	return 0;
}


int CalibrateEntry(byte * mask, byte * vals, int size)
{
	EnumWindows(EnumWindowsProc, 1);
	if ((Main->bMemoryAccess) && (hwndUOClient))
	{
		const int chunkSize = 4096;
		int readSize = chunkSize + size;
		DWORD dwProcessID;
		HANDLE m_hProcess;
		LPVOID lpAddress;

		GetWindowThreadProcessId(hwndUOClient, &dwProcessID);
		m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);

		byte * read = new byte[readSize];

		for ( int i = 0;; ++i )
		{
			lpAddress = (LPVOID)((DWORD_PTR)0x400000 + (i * chunkSize));;
			if ( !ReadProcessMemory(m_hProcess, lpAddress, read, readSize, NULL) )
				break;

			for ( int j = 0; j < chunkSize; ++j )
			{
				bool ok = true;

				for ( int k = 0; ok && k < size; ++k )
					ok = ( (read[j + k] & mask[k]) == vals[k] );
				
				if ( ok )
				{
					CloseHandle( m_hProcess );
					return 0x400000 + (i * chunkSize) + j;
				}
			}
		}
		CloseHandle( m_hProcess );
	}
	return 0;
}

bool AdjustPrivileges() 
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	TOKEN_PRIVILEGES oldtp;
	DWORD dwSize = sizeof(TOKEN_PRIVILEGES);
	LUID luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
			return true;
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		CloseHandle(hToken);
		return false;
	}

	ZeroMemory(&tp, sizeof(tp));
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	/* Adjust Token Privileges */
	if (!AdjustTokenPrivileges (hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &oldtp, &dwSize))
	{
		CloseHandle(hToken);
		return false;
	}
	// close handles
    CloseHandle(hToken);

	return true;
}

CString GetInternetCodeString(DWORD dwRet)
{
	return CMsg(CFMsg("IDS_HTMLERROR_%1!d!",dwRet));
}

void ReminderLaunch(int iStartup)
{
	CTime tNow = CTime::GetCurrentTime();
	HKEY hKey;
	int iIndex = 0;
	int iType, iHour, iMin, iStart;
	DWORD dwTime;
	CString csTitle, csText, csKey;
	LONG lStatus = RegOpenKeyEx(hRegLocation, REGKEY_REMINDER, 0, KEY_ALL_ACCESS, &hKey);
	if ( lStatus == ERROR_SUCCESS )
	{
		while ( lStatus == ERROR_SUCCESS )
		{
			char szBuffer[MAX_PATH];
			DWORD dwSize = sizeof(szBuffer);
			lStatus = RegEnumKeyEx(hKey, iIndex, &szBuffer[0], &dwSize, 0, NULL, NULL, NULL);
			if (lStatus == ERROR_SUCCESS)
			{
				csTitle = szBuffer;
				csKey.Format("%s\\%s",REGKEY_REMINDER, csTitle);
				if (Main->GetRegistryDword("LastRead",-1,hRegLocation,csKey)!= tNow.GetDay())
				{
					iType = Main->GetRegistryDword("Type",0,hRegLocation,csKey);
					switch(iType)
					{
					case 0:
						break;
					case 1:
						{
							if (Main->GetRegistryDword("Date",-1,hRegLocation,csKey) <= tNow.GetTime())
							{
								iStart = Main->GetRegistryDword("Startup",0,hRegLocation,csKey);
								if ((iStart) && (iStartup))
								{
									CReminder * rObject = new CReminder;
									rObject->m_csTitle = csTitle;
									rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
									rObject->m_dwTime = 0;
									rObject->bOnce = true;
									AfxBeginThread(ReminderThread,rObject);
								}
								else if (!iStart)
								{
									iHour = Main->GetRegistryDword("Hour",0,hRegLocation,csKey);
									iMin = Main->GetRegistryDword("Minute",0,hRegLocation,csKey);
									dwTime = ((((iHour - tNow.GetHour())*60) + (iMin - tNow.GetMinute()))*60000) - (tNow.GetSecond() * 1000);
									if ((int)dwTime < 0)
										dwTime = 0;
									CReminder * rObject = new CReminder;
									rObject->m_csTitle = csTitle;
									rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
									rObject->m_dwTime = dwTime;
									rObject->bOnce = true;
									if ((int)dwTime < 30000)
										AfxBeginThread(ReminderThread,rObject);
								}
							}
						}
						break;
					case 2:
						{
							iStart = Main->GetRegistryDword("Startup",0,hRegLocation,csKey);
							if ((iStart) && (iStartup))
							{
								CReminder * rObject = new CReminder;
								rObject->m_csTitle = csTitle;
								rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
								rObject->m_dwTime = 0;
								rObject->bOnce = false;
								AfxBeginThread(ReminderThread,rObject);
							}
							else if (!iStart)
							{
								iHour = Main->GetRegistryDword("Hour",0,hRegLocation,csKey);
								iMin = Main->GetRegistryDword("Minute",0,hRegLocation,csKey);
								dwTime = ((((iHour - tNow.GetHour())*60) + (iMin - tNow.GetMinute()))*60000) - (tNow.GetSecond() * 1000);
								if ((int)dwTime < 0)
									dwTime = 0;
								CReminder * rObject = new CReminder;
								rObject->m_csTitle = csTitle;
								rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
								rObject->m_dwTime = dwTime;
								rObject->bOnce = false;
								if ((int)dwTime < 30000)
									AfxBeginThread(ReminderThread,rObject);
							}
						}
						break;
					case 3:
						{
							if (Main->GetRegistryDword("Date",-1,hRegLocation,csKey) == tNow.GetDayOfWeek())
							{
								iStart = Main->GetRegistryDword("Startup",0,hRegLocation,csKey);
								if ((iStart) && (iStartup))
								{
									CReminder * rObject = new CReminder;
									rObject->m_csTitle = csTitle;
									rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
									rObject->m_dwTime = 0;
									rObject->bOnce = false;
									AfxBeginThread(ReminderThread,rObject);
								}
								else if (!iStart)
								{
									iHour = Main->GetRegistryDword("Hour",0,hRegLocation,csKey);
									iMin = Main->GetRegistryDword("Minute",0,hRegLocation,csKey);
									dwTime = ((((iHour - tNow.GetHour())*60) + (iMin - tNow.GetMinute()))*60000) - (tNow.GetSecond() * 1000);
									if ((int)dwTime < 0)
										dwTime = 0;
									CReminder * rObject = new CReminder;
									rObject->m_csTitle = csTitle;
									rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
									rObject->m_dwTime = dwTime;
									rObject->bOnce = false;
									if ((int)dwTime < 30000)
										AfxBeginThread(ReminderThread,rObject);
								}
							}
						}
						break;
					case 4:
						{
							if (Main->GetRegistryDword("Date",-1,hRegLocation,csKey) == tNow.GetDay())
							{
								iStart = Main->GetRegistryDword("Startup",0,hRegLocation,csKey);
								if ((iStart) && (iStartup))
								{
									CReminder * rObject = new CReminder;
									rObject->m_csTitle = csTitle;
									rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
									rObject->m_dwTime = 0;
									rObject->bOnce = false;
									AfxBeginThread(ReminderThread,rObject);
								}
								else if (!iStart)
								{
									iHour = Main->GetRegistryDword("Hour",0,hRegLocation,csKey);
									iMin = Main->GetRegistryDword("Minute",0,hRegLocation,csKey);
									dwTime = ((((iHour - tNow.GetHour())*60) + (iMin - tNow.GetMinute()))*60000) - (tNow.GetSecond() * 1000);
									if ((int)dwTime < 0)
										dwTime = 0;
									CReminder * rObject = new CReminder;
									rObject->m_csTitle = csTitle;
									rObject->m_csText = Main->GetRegistryString("Text","",hRegLocation,csKey);;
									rObject->m_dwTime = dwTime;
									rObject->bOnce = false;
									if ((int)dwTime < 30000)
										AfxBeginThread(ReminderThread,rObject);
								}
							}
						}
						break;
					}
				}
			}
			iIndex++;
		}
		RegCloseKey(hKey);
	}
}

UINT ReminderThread(LPVOID pParam)
{
	CReminder * rObject = (CReminder*)pParam;
	Sleep(rObject->m_dwTime);

	CReminderDLG dlg;
	dlg.rObject = rObject;
	dlg.DoModal();
	return 0;
}

UINT Reminder(LPVOID pParam)
{
	UNREFERENCED_PARAMETER(pParam);
	ReminderLaunch(1);
	Sleep(30000);

	while(true)
	{
		Sleep(30000);
		ReminderLaunch();
	}

	return 0;
}

//String table functions

CMsg::CMsg(UINT nID)
{
  if (!LoadString(nID))
  {
    TRACE(_T("Unknown resource string id : %d\n"),nID);
    ASSERT(false);
    CString::operator=(_T("???"));
  }
}

CFMsg::CFMsg(UINT nFormatID,...)
{
	CString strFormat;
	if (!strFormat.LoadString(nFormatID))
  {
    TRACE(_T("Unknown resource string id : %d\n"),nFormatID);
    ASSERT(false);
    CString::operator=(_T("???"));
  }
  else
  {
	  va_list argList;
	  va_start(argList, nFormatID);
	  LPTSTR lpszTemp;
	  if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		                    strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
		    lpszTemp == NULL)
	  {
		  AfxThrowMemoryException();
	  }
	  CString::operator=(lpszTemp);

	  LocalFree(lpszTemp);
	  va_end(argList);
  }
}

CFMsg::CFMsg(LPCTSTR lpszFormat,...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	LPTSTR lpszTemp;
	if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
		lpszTemp == NULL)
	{
		AfxThrowMemoryException();
	}

	CString::operator=(lpszTemp);

	LocalFree(lpszTemp);
	va_end(argList);
}

CMsg::CMsg(LPCTSTR lpszString)
{
    CString::operator=(_T(Main->pLng.GetValue(lpszString)));
}

__int64 HashFileName(CString csFile)
{
	UINT eax, ecx, edx, ebx, esi, edi;

	eax = ecx = edx = ebx = esi = edi = 0;
	ebx = edi = esi = (__int32) csFile.GetLength() + 0xDEADBEEF;

	int i = 0;

	for ( i = 0; i + 12 < csFile.GetLength(); i += 12 )
	{
		edi = (__int32) ( ( csFile[ i + 7 ] << 24 ) | ( csFile[ i + 6 ] << 16 ) | ( csFile[ i + 5 ] << 8 ) | csFile[ i + 4 ] ) + edi;
		esi = (__int32) ( ( csFile[ i + 11 ] << 24 ) | ( csFile[ i + 10 ] << 16 ) | ( csFile[ i + 9 ] << 8 ) | csFile[ i + 8 ] ) + esi;
		edx = (__int32) ( ( csFile[ i + 3 ] << 24 ) | ( csFile[ i + 2 ] << 16 ) | ( csFile[ i + 1 ] << 8 ) | csFile[ i ] ) - esi;

		edx = ( edx + ebx ) ^ ( esi >> 28 ) ^ ( esi << 4 );
		esi += edi;
		edi = ( edi - edx ) ^ ( edx >> 26 ) ^ ( edx << 6 );
		edx += esi;
		esi = ( esi - edi ) ^ ( edi >> 24 ) ^ ( edi << 8 );
		edi += edx;
		ebx = ( edx - esi ) ^ ( esi >> 16 ) ^ ( esi << 16 );
		esi += edi;
		edi = ( edi - ebx ) ^ ( ebx >> 13 ) ^ ( ebx << 19 );
		ebx += esi;
		esi = ( esi - edi ) ^ ( edi >> 28 ) ^ ( edi << 4 );
		edi += ebx;
	}

	if ( csFile.GetLength() - i > 0 )
	{
		switch ( csFile.GetLength() - i )
		{
			case 12:
				esi += (__int32) csFile[ i + 11 ] << 24;
				//goto case 11;
			case 11:
				esi += (__int32) csFile[ i + 10 ] << 16;
				//goto case 10;
			case 10:
				esi += (__int32) csFile[ i + 9 ] << 8;
				//goto case 9;
			case 9:
				esi += (__int32) csFile[ i + 8 ];
				//goto case 8;
			case 8:
				edi += (__int32) csFile[ i + 7 ] << 24;
				//goto case 7;
			case 7:
				edi += (__int32) csFile[ i + 6 ] << 16;
				//goto case 6;
			case 6:
				edi += (__int32) csFile[ i + 5 ] << 8;
				//goto case 5;
			case 5:
				edi += (__int32) csFile[ i + 4 ];
				//goto case 4;
			case 4:
				ebx += (__int32) csFile[ i + 3 ] << 24;
				//goto case 3;
			case 3:
				ebx += (__int32) csFile[ i + 2 ] << 16;
				//goto case 2;
			case 2:
				ebx += (__int32) csFile[ i + 1 ] << 8;
				//goto case 1;
			case 1:		
				ebx += (__int32) csFile[ i ];
				break;			
		}

		esi = ( esi ^ edi ) - ( ( edi >> 18 ) ^ ( edi << 14 ) );
		ecx = ( esi ^ ebx ) - ( ( esi >> 21 ) ^ ( esi << 11 ) );
		edi = ( edi ^ ecx ) - ( ( ecx >> 7 ) ^ ( ecx << 25 ) );
		esi = ( esi ^ edi ) - ( ( edi >> 16 ) ^ ( edi << 16 ) );
		edx = ( esi ^ ecx ) - ( ( esi >> 28 ) ^ ( esi << 4 ) );
		edi = ( edi ^ edx ) - ( ( edx >> 18 ) ^ ( edx << 14 ) );
		eax = ( esi ^ edi ) - ( ( edi >> 8 ) ^ ( edi << 24 ) );

		return ( (__int64) edi << 32 ) | eax;
	}

	return ( (__int64) esi << 32 ) | eax;
}