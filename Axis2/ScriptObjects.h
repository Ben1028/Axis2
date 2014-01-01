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

// ScriptObjects.h: interface for the CScriptObjects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTOBJECT_H__E8C071C9_A64D_49E7_8F5F_86CE307EC301__INCLUDED_)
#define AFX_SCRIPTOBJECT_H__E8C071C9_A64D_49E7_8F5F_86CE307EC301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "stdafx.h"
//#include "Axis2.h"
#include "Common.h"


class CScriptObjects
{
public:
	CScriptObjects();
	virtual ~CScriptObjects();

	void LoadQuicklist(CString csList, CScriptArray * pObjList, CScriptArray * pDestList);
	void LoadProfile(CString csProfile);
	void LoadProfileDirectory(CString csPath);
	void LoadSingleDirectory(CString csPath);
	bool LoadFile(CStdioFile * pFile, bool bResource = 0, bool bProgress = 1);
	void UnloadProfile();
	void Unload(CScriptArray * pObjList);
	void LoadCustomLocations();

	class CProgressBar * m_pDlg;
	void CreateProgressDialog();
	void DestroyProgressDialog();

	void RemoveObjectsCategories(CPtrList * pCatList, CString csName);
	void CategorizeObjects(CScriptArray * pObjList, CPtrList * pCatList, CString csName, int * m_iCatSeq);

	CScriptArray m_aItems;
	CScriptArray m_aNPCs;
	CScriptArray m_aDefList;
	CScriptArray m_aAreas;
	CScriptArray m_aSpellList;

	CPtrList m_olItems;
	CPtrList m_olNPCs;
	CPtrList m_olAreas;

	CScriptArray m_ItemQuickList;
	CScriptArray m_SpawnQuickList;
	CScriptArray m_AreaQuickList;

	CAdvStringArray m_asaNPCBrains;
	CAdvStringArray m_asaNPCSkills;
	CAdvStringArray m_asaNPCStats;
	CAdvStringArray m_asaNPCResistences;
	CAdvStringArray m_asaNPCMisc;
	CAdvStringArray m_asaNPCTags;

	CAdvStringArray m_asaITEMTypes;
	CAdvStringArray m_asaITEMProps;
	CAdvStringArray m_asaITEMTags;

	CAdvStringArray m_asaFunctions;
	CAdvStringArray m_asaEvents;

	CAdvStringArray m_asaSPAWNitem;
	CAdvStringArray m_asaSPAWNchar;

	CAdvStringArray m_asaLoadedScripts;

	int m_iICatSeq;
	int m_iNCatSeq;
	int m_iACatSeq;


protected:
	bool m_bDeleteDialog;
};

#endif //!defined(AFX_SCRIPTOBJECT_H__E8C071C9_A64D_49E7_8F5F_86CE307EC301__INCLUDED_)

