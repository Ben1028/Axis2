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

// ScriptObjects.cpp: implementation of the CScriptObjects class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Axis2.h"
#include "ProgressBar.h"
#include "ScriptObjects.h"
#include "RemoteProfileLoginDlg.h"
#include "Common.h"
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptObjects::CScriptObjects()
{
	m_pDlg = NULL;
	m_iICatSeq = 0;
	m_iNCatSeq = 0;
	m_iACatSeq = 0;
}

CScriptObjects::~CScriptObjects()
{
	Unload(&m_aItems);
	Unload(&m_aNPCs);
	Unload(&m_aAreas);
	Unload(&m_aDefList);

	RemoveObjectsCategories(&m_olItems, "Items");
	RemoveObjectsCategories(&m_olNPCs, "NPCs");
	RemoveObjectsCategories(&m_olAreas, "Locations");

	m_asaNPCBrains.RemoveAll();
	m_asaNPCSkills.RemoveAll();
	m_asaNPCStats.RemoveAll();
	m_asaNPCResistences.RemoveAll();
	m_asaNPCMisc.RemoveAll();
	m_asaNPCTags.RemoveAll();

	m_asaITEMTypes.RemoveAll();
	m_asaITEMProps.RemoveAll();
	m_asaITEMTags.RemoveAll();

	m_asaFunctions.RemoveAll();
	m_asaEvents.RemoveAll();

	m_asaSPAWNitem.RemoveAll();
	m_asaSPAWNchar.RemoveAll();
}



enum RES_TYPE	// all the script resource blocks we want to deal with !
{
	// NOTE: SPHERE.INI, SPHERETABLE.SCP are read at start.
	// All other files are indexed from the SPEECHFILES and SCPFILES directories.
	// (SI) = Single instance types.
	// (SL) = single line multiple definitions.
	// Alphabetical order.
	RES_UNKNOWN = 0,	// Not to be used.
	RES_ADVANCE,		// Define the advance rates for stats.
	RES_AREA,			// Complex region. (w/extra tags)
	RES_AREADEF,		// Complex region. (w/ extra tags)
	RES_BLOCKIP,		// (SL) A list of IP's to block.
	RES_BOOK,			// A book or a page from a book.
	RES_CHARDEF,		// Define a char type. (overlap with RES_SPAWN)
	RES_COMMENT,		// A commented out block type.
	RES_DEFNAME,		// (SL) Just add a bunch of new defs and equivs str/values.
	RES_DEFNAMES,		// (SL) Just add a bunch of new defs and equivs str/values.  -- Same as DEFNAME, but for 99+
	RES_DEFMESSAGE,		// New 56a message block
	RES_DIALOG,			// A scriptable gump dialog, text or handler block.
	RES_EVENTS,			// An Event handler block with the trigger type in it. ON=@Death etc.
	RES_FAME,
	RES_FUNCTION,		// Define a new command verb script that applies to a char.
	RES_GLOBALS,
	RES_GMPAGE,			// A GM page. (SAVED in World)
	RES_ITEMDEF,		// Define an item type. (overlap with RES_TEMPLATE)
	RES_KARMA,
	RES_LOCATION,		// Hogloc.scp location format.
	RES_MENU,			// General scriptable menus.
	RES_MOONGATES,		// (SL) Define where the moongates are.
	RES_MULTIDEF,
	RES_NAMES,			// A block of possible names for a NPC type. (read as needed)
	RES_NEWBIE,			// Triggers to execute on Player creation (based on skills selected)
	RES_NOTOTITLES,		// (SI) Define the noto titles used.
	RES_OBSCENE,		// (SL) A list of obscene words.
	RES_PLEVEL,			// Define the list of commands that a PLEVEL can access. (or not access)
	RES_REGIONRESOURCE,	// Define an Ore type.
	RES_REGIONTYPE,		// Triggers etc. that can be assinged to a RES_AREA
	RES_RESOURCES,		// (SL) list of all the resource files we should index !
	RES_ROOM,			// Non-complex region. (no extra tags)
	RES_ROOMDEF,		// Non-complex region. (no extra tags)
	RES_RUNES,			// (SI) Define list of the magic runes.
	RES_SECTOR,			// Make changes to a sector. (SAVED in World)
	RES_SERVERS,		// List a number of servers in 3 line format. (Phase this out)
	RES_SKILL,			// Define attributes for a skill (how fast it raises etc)
	RES_SKILLCLASS,		// Define specifics for a char with this skill class. (ex. skill caps)
	RES_SKILLMENU,		// A menu that is attached to a skill. special arguments over other menus.
	RES_SPAWN,			// Define a list of NPC's and how often they may spawn.
	RES_SPEECH,			// A speech block with ON=*blah* in it.
	RES_SPELL,			// Define a magic spell. (0-64 are reserved)
	RES_SPHERE,			// Main Server INI block
	RES_STARTS,			// (SI) List of starting locations for newbies.
	RES_TELEPORTERS,	// (SL) Where are the teleporters in the world ? dungeon transports etc.
	RES_TEMPLATE,		// Define lists of items. (for filling loot etc)
	RES_TIMERF,
	RES_TYPEDEF,			// Define a trigger block for a RES_WORLDITEM m_type.
	RES_TYPEDEFS,
	RES_WEBPAGE,		// Define a web page template.
	RES_WORLDCHAR,		// Define instance of char in the world. (SAVED in World)
	RES_WORLDITEM,		// Define instance of item in the world. (SAVED in World)
	RES_WI,
	RES_WC,
	RES_WS,
	RES_EOF,
	RES_QTY,			// Don't care
};

const char * pResourceBlocks[RES_QTY] =	// static
{
	"AAAUNUSED",		// unused / unknown.
	"ADVANCE",			// Define the advance rates for stats.
	"AREA",				//Same as areadef
	"AREADEF",			// Complex region. (w/ extra tags)
	"BLOCKIP",			// (SL) A list of IP's to block.
	"BOOK",				// A book or a page from a book.
	"CHARDEF",			// Define a char type.
	"COMMENT",			// A commented out block type.
	"DEFNAME",			// (SL) Just add a bunch of new defs and equivs str/values.
	"DEFNAMES",			// (SL) Just add a bunch of new defs and equivs str/values. Same as DEFNAME, but for 99+
	"DEFMESSAGE",		// New 56a message block
	"DIALOG",			// A scriptable gump dialog", text or handler block.
	"EVENTS",			// (SL) Preload these Event files.
	"FAME",
	"FUNCTION",			// Define a new command verb script that applies to a char.
	"GLOBALS",
	"GMPAGE",			// A GM page. (SAVED in World)
	"ITEMDEF",			// Define an item type
	"KARMA",
	"LOCATION",			// Hogloc.scp location format.
	"MENU",				// General scriptable menus.
	"MOONGATES",		// (SL) Define where the moongates are.
	"MULTIDEF",
	"NAMES",			// A block of possible names for a NPC type. (read as needed)
	"NEWBIE",			// Triggers to execute on Player creation (based on skills selected)
	"NOTOTITLES",		// (SI) Define the noto titles used.
	"OBSCENE",			// (SL) A list of obscene words.
	"PLEVEL",			// Define the list of commands that a PLEVEL can access. (or not access)
	"REGIONRESOURCE",	// Define Ore types.
	"REGIONTYPE",		// Triggers etc. that can be assinged to a "AREA
	"RESOURCES",		// (SL) list of all the resource files we should index !
	"ROOM",				// Same as Room
	"ROOMDEF",			// Non-complex region. (no extra tags)
	"RUNES",			// (SI) Define list of the magic runes.
	"SECTOR",			// Make changes to a sector. (SAVED in World)
	"SERVERS",			// List a number of servers in 3 line format.
	"SKILL",			// Define attributes for a skill (how fast it raises etc)
	"SKILLCLASS",		// Define class specifics for a char with this skill class.
	"SKILLMENU",		// A menu that is attached to a skill. special arguments over other menus.
	"SPAWN",			// Define a list of NPC's and how often they may spawn.
	"SPEECH",			// (SL) Preload these speech files.
	"SPELL",			// Define a magic spell. (0-64 are reserved)
	"SPHERE",			// Main Server INI block
	"STARTS",			// (SI) List of starting locations for newbies.
	"TELEPORTERS",		// (SL) Where are the teleporteres in the world ?
	"TEMPLATE",			// Define a list of items. (for filling loot etc)
	"TIMERF",
	"TYPEDEF",			// Define a trigger block for a "WORLDITEM m_type.
	"TYPEDEFS",
	"WEBPAGE",			// Define a web page template.
	"WORLDCHAR",		// Define instance of char in the world. (SAVED in World)
	"WORLDITEM",		// Define instance of item in the world. (SAVED in World)
	"WI",
	"WC",
	"WS",
	"EOF",
};

const char * pRestricted [4] =
{
	"MySqlDatabase",		//0
	"MySqlHost",			//1
	"MySqlPassword",		//2
	"MySqlUser",			//3
};


bool CScriptObjects::LoadFile(CStdioFile * pFile, bool bResource, bool bProgress)
{
	if(!pFile)
		return false;

	CString csFile = pFile->GetFilePath();
	if(m_asaLoadedScripts.Find(csFile) != -1)
		return false;

	m_asaLoadedScripts.Add(csFile);

	try
	{
		Main->m_log.Add(2,"Loading file %s", csFile);

		CWaitCursor hourglass;
		CString csMessage;
		csMessage.Format("Reading %s", csFile);

		if ( m_pDlg )
		{
			m_pDlg->m_csMessage.SetWindowText(csMessage);
			m_pDlg->SetPos(0);
			m_pDlg->SetRange32(0, (DWORD) pFile->GetLength());
		}

		int j = 0;
		BOOL bStatus = TRUE;
		while ( bStatus )
		{
			CString csLine;
			bStatus = pFile->ReadString(csLine);

			if ( !bStatus )
				break;

			if ( csLine.Find("[") == 0 )
			{
				while ( bStatus )
				{
					if ( !bStatus )
						break;
					if ( csLine.Find("[") == 0 )
					{
						CString csKey = csLine.Mid(1);
						csKey = csKey.SpanExcluding("]");
						CString csIndex;
						if ( csKey.Find(" ") != -1 )
							csIndex = csKey.Mid(csKey.Find(" ") + 1);
						CString csType = csKey.SpanExcluding(" ");
						int resource = FindTable(csType, pResourceBlocks, RES_QTY);	

						switch (resource)
						{
						//Ignore these blocks
						case RES_UNKNOWN:
						case RES_ADVANCE:
						case RES_BLOCKIP:
						case RES_BOOK:
						case RES_COMMENT:
						case RES_DIALOG:
						case RES_FAME:
						case RES_GLOBALS:
						case RES_GMPAGE:
						case RES_KARMA:
						case RES_MENU:
						case RES_MOONGATES:
						case RES_NAMES:
						case RES_NEWBIE:
						case RES_NOTOTITLES:
						case RES_OBSCENE:
						case RES_PLEVEL:
						case RES_REGIONRESOURCE:
						case RES_REGIONTYPE:
						case RES_RUNES:
						case RES_SECTOR:
						case RES_SERVERS:
						case RES_SKILLCLASS:
						case RES_SKILLMENU:
						case RES_SPEECH:
						case RES_STARTS:
						case RES_TELEPORTERS:
						case RES_TIMERF:
						case RES_WEBPAGE:
						case RES_WORLDCHAR:
						case RES_WC:
						case RES_WS:
						case RES_EOF:
						case RES_QTY:
							bStatus = pFile->ReadString(csLine);
						break;
						case RES_ITEMDEF:
							{
								CSObject * pItem = new CSObject;
								pItem->m_bType = TYPE_ITEM;
								pItem->m_csID = csIndex;
								pItem->m_csValue = csIndex;
								pItem->m_csDisplay = csIndex;
								pItem->m_csFilename = csFile;	

								csLine = pItem->ReadBlock(*pFile);

								int iOld = m_aItems.FindSimilar(pItem); // Similar lookup respects type. This is required since items and multis share thier id range.
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aItems.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_ITEM"), pItem->m_csID, pItem->m_csDisplay));
									m_aItems.RemoveAt(iOld);
									delete pOld;
								}
								m_aItems.Insert(pItem);
							}
							break;
						case RES_MULTIDEF:
							{
								CSObject * pItem = new CSObject;
								pItem->m_bType = TYPE_MULTI;
								pItem->m_csID = csIndex;
								pItem->m_csValue = csIndex;
								pItem->m_csDisplay = csIndex;
								pItem->m_csFilename = csFile;	

								csLine = pItem->ReadBlock(*pFile);

								int iOld = m_aItems.FindSimilar(pItem); // Similar lookup respects type. This is required since items and multis share thier id range.
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aItems.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_MULTI"), pItem->m_csID, pItem->m_csDisplay));
									m_aItems.RemoveAt(iOld);
									delete pOld;
								}
								m_aItems.Insert(pItem);
							}
							break;
						case RES_TEMPLATE:
							{
								CSObject * pTempl = new CSObject;
								pTempl->m_bType = TYPE_TEMPLATE;
								pTempl->m_csID = csIndex;
								pTempl->m_csValue = csIndex;
								pTempl->m_csDisplay = "01";
								pTempl->m_csFilename = csFile;	

								csLine = pTempl->ReadBlock(*pFile);

								int iOld = m_aItems.Find(pTempl->m_csValue);
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aItems.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_TEMPLATE"), pTempl->m_csID, pTempl->m_csDisplay));
									m_aItems.RemoveAt(iOld);
									delete pOld;
								}
								m_aItems.Insert(pTempl);
							}
							break;
						case RES_CHARDEF:
							{
								CSObject * pNPC = new CSObject;
								pNPC->m_bType = TYPE_CHAR;
								pNPC->m_csID = csIndex;
								pNPC->m_csValue = csIndex;
								pNPC->m_csDisplay = csIndex;
								pNPC->m_csFilename = csFile;

								csLine = pNPC->ReadBlock(*pFile);

								int iOld = m_aNPCs.Find(pNPC->m_csValue);
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aNPCs.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_CHAR"), pNPC->m_csID, pNPC->m_csDisplay));
									m_aNPCs.RemoveAt(iOld);
									delete pOld;
								}
								m_aNPCs.Insert(pNPC);
							}
							break;
						case RES_SPAWN:
							{
								CSObject * pSpawn = new CSObject;
								pSpawn->m_bType = TYPE_SPAWN;
								pSpawn->m_csID = csIndex;
								pSpawn->m_csValue = csIndex;
								pSpawn->m_csDisplay = "01";
								pSpawn->m_csFilename = csFile;

								csLine = pSpawn->ReadBlock(*pFile);

								int iOld = m_aNPCs.Find(pSpawn->m_csValue);
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aNPCs.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_SPAWN"), pSpawn->m_csID, pSpawn->m_csDisplay));
									m_aNPCs.RemoveAt(iOld);
									delete pOld;
								}
								m_aNPCs.Insert(pSpawn);
							}
							break;
						case RES_AREA:
						case RES_AREADEF:
						case RES_ROOM:
						case RES_ROOMDEF:
						case RES_LOCATION:
							{
								CSObject * pArea = new CSObject;
								pArea->m_bType = TYPE_AREA;
								pArea->m_csValue = csIndex;
								pArea->m_csFilename = csFile;

								csLine = pArea->ReadBlock(*pFile);

								int iOld = m_aAreas.Find(pArea->m_csValue);
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aAreas.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_LOCATION"), pArea->m_csID, pArea->m_csValue));
									m_aAreas.RemoveAt(iOld);
									delete pOld;
								}
								m_aAreas.Insert(pArea);
							}
							break;
						case RES_DEFNAME:
						case RES_SPHERE:
						case RES_DEFNAMES:
						case RES_DEFMESSAGE:
							{
								while ( bStatus )
								{
									bStatus = pFile->ReadString(csLine);
									if ( !bStatus )
										break;
									if ( csLine.Find("[") == 0 )
									{
										break;
									}
									csLine = csLine.SpanExcluding("//");
									csLine.Trim(); 
									if ( csLine != "" )
									{
										//load brain section separately
										if (csIndex.CompareNoCase("brains") == 0)
										{
											CString csBrain, csValue, csTemp;
											csTemp = csLine.SpanExcluding(" \t=");
											csValue = csLine.Mid(csLine.FindOneOf(" \t="));
											csValue.Trim();
											if ((csTemp != "") && (csValue != ""))
											{
												csBrain.Format("%s (%s)",csTemp,csValue);
												m_asaNPCBrains.Insert(csBrain);
											}
											continue;
										}
										CSObject * pDef = new CSObject;
										pDef->m_bType = TYPE_DEF;
										CString csTemp;
										csTemp = csLine.SpanExcluding(" \t=");
										pDef->m_csValue = csTemp;

										if ( pDef->m_csValue.GetLength() == csLine.GetLength() )
										{
											delete pDef;
											continue;
										}

										int restricted = FindTable(pDef->m_csValue, pRestricted, 4);
										if (restricted != -1)
										{
											delete pDef;
											continue;
										}
								
										csTemp = csLine.Mid(pDef->m_csValue.GetLength() + 1);
										csTemp.Trim();
										if( csTemp.FindOneOf(" \t="))
											csTemp = csTemp.Mid(csTemp.FindOneOf(" \t=") + 1);
										csTemp.Trim();

										if(csTemp.Find('{') != -1)
										{
											csTemp = csTemp.Mid(csTemp.Find("{")+1);
											csTemp = csTemp.Left(csTemp.ReverseFind('}'));
											if(csTemp.Find('{') != -1)
											{
												csTemp = csTemp.Mid(csTemp.ReverseFind('{')+1);
												csTemp = csTemp.SpanExcluding("}");	
											}
										}

										pDef->m_csID = csTemp;

										if ( pDef->m_csValue == "" )
										{
											delete pDef;
											continue;
										}

										if ( pDef->m_csValue == pDef->m_csID )
										{
											delete pDef;
											continue;
										}

										int iOld = m_aDefList.Find(pDef->m_csValue);
										if ( iOld != -1 )
										{
											CSObject * pOld = (CSObject *) m_aDefList.GetAt(iOld);
											m_aDefList.RemoveAt(iOld);
											delete pOld;
										}
										m_aDefList.Insert(pDef);
									}
								}
							}
							break;
						case RES_TYPEDEFS:
							{
								while ( bStatus )
								{
									bStatus = pFile->ReadString(csLine);
									if ( !bStatus )
										break;
									if ( csLine.Find("[") == 0 )
									{
										break;
									}
									csLine = csLine.SpanExcluding("//");
									csLine.Trim(); 
									if ( csLine != "" )
									{
											CString csType, csValue;
											csType = csLine.SpanExcluding(" \t=");
											int iOld = m_asaITEMTypes.Find(csType);
											if ( iOld != -1 )
												m_asaITEMTypes.RemoveAt(iOld);
											m_asaITEMTypes.Insert(csType);
									}
								}
							}
							break;
						case RES_TYPEDEF:
							{
								int iOld = m_asaITEMTypes.Find(csIndex);
								if ( iOld != -1 )
									m_asaITEMTypes.RemoveAt(iOld);
								m_asaITEMTypes.Insert(csIndex);
								bStatus = pFile->ReadString(csLine);
							}
							break;
						case RES_EVENTS:
							{
								int iOld = m_asaEvents.Find(csIndex);
								if ( iOld != -1 )
									m_asaEvents.RemoveAt(iOld);
								m_asaEvents.Insert(csIndex);
								bStatus = pFile->ReadString(csLine);
							}
							break;
						case RES_FUNCTION:
							{
								int iOld = m_asaFunctions.Find(csIndex);
								if ( iOld != -1 )
									m_asaFunctions.RemoveAt(iOld);
								m_asaFunctions.Insert(csIndex);
								bStatus = pFile->ReadString(csLine);
							}
							break;
						case RES_SPELL:
							{
								CSObject * pSpell = new CSObject;
								pSpell->m_bType = TYPE_SPELL;
								pSpell->m_csValue = csIndex;
								pSpell->m_csFilename = csFile;

								csLine = pSpell->ReadBlock(*pFile);

								int iOld = m_aSpellList.Find(pSpell->m_csValue);
								if ( iOld != -1 )
								{
									CSObject * pOld = (CSObject *) m_aSpellList.GetAt(iOld);
									Main->m_log.Add(1, CFMsg(CMsg("IDS_ID_DUPLICATE_SPELL"), pSpell->m_csID, pSpell->m_csDisplay));
									m_aSpellList.RemoveAt(iOld);
									delete pOld;
								}
								m_aSpellList.Insert(pSpell);
							}
							break;
						case RES_SKILL:
							{
								while ( bStatus )
								{
									bStatus = pFile->ReadString(csLine);
									if ( !bStatus )
										break;
									if ( csLine.Find("[") == 0 )
									{
										break;
									}
									csLine = csLine.SpanExcluding("//");
									csLine.Trim();
									if ( csLine != "" )
									{
										CString csKey, csTemp;
										csTemp = csLine.SpanExcluding(" \t=");

										if ( csTemp.CompareNoCase("Key") != 0 )
											continue;
								
										csTemp = csLine.Mid(csTemp.GetLength() + 1);
										csTemp.Trim();
										if( csTemp.FindOneOf(" \t="))
											csTemp = csTemp.Mid(csTemp.FindOneOf(" \t=") + 1);
										csTemp.Trim();
										csKey.Format("%s (%s)", csTemp, csIndex);
										if (csKey != "")
											m_asaNPCSkills.Insert(csKey);
									}
								}
							}
							break;
						case RES_WORLDITEM:
						case RES_WI:
							{
								if (csIndex.CompareNoCase("i_worldgem_bit") == 0)
								{
									int iType = ITEM_SPAWN_CHAR;
									CString csPos;
									while ( bStatus )
									{
										bStatus = pFile->ReadString(csLine);
										if ( !bStatus )
											break;
										if ( csLine.Find("[") == 0 )
										{
											break;
										}
										csLine = csLine.SpanExcluding("//");
										csLine.Trim();
										if ( csLine != "" )
										{
											CString csTemp;
											csTemp = csLine.SpanExcluding(" \t=");

											if ( csTemp.CompareNoCase("TYPE") == 0 )
											{
												csTemp = csLine.Mid(csTemp.GetLength() + 1);
												csTemp.Trim();
												if( csTemp.FindOneOf(" \t="))
													csTemp = csTemp.Mid(csTemp.FindOneOf(" \t=") + 1);
												csTemp.Trim();
												if ( csTemp.CompareNoCase("t_spawn_item") == 0 )
													iType = ITEM_SPAWN_ITEM;
												else
													iType = 0;
											}
									
											if ( csTemp.CompareNoCase("P") == 0 )
											{
												csPos = csLine.Mid(csTemp.GetLength() + 1);
												csPos.Trim();
												if( csPos.FindOneOf(" \t="))
												{
													csPos = csPos.Mid(csPos.FindOneOf(" \t=") + 1);
													csPos.Trim();
												}
											}
										}
									}
									if (iType == ITEM_SPAWN_ITEM)
										m_asaSPAWNitem.Insert(csPos);
									else if (iType == ITEM_SPAWN_CHAR)
										m_asaSPAWNchar.Insert(csPos);
								}
								else
									bStatus = pFile->ReadString(csLine);
							}
							break;
						case RES_RESOURCES:
							{
								if(bResource)
								{
									while ( bStatus )
									{
										bStatus = pFile->ReadString(csLine);
										if ( !bStatus )
											break;
										if ( csLine.Find("[") == 0 )
										{
											break;
										}
										csLine = csLine.SpanExcluding("//");
										csLine.Trim(); 
										if ( csLine != "" )
										{
											CString csLoadFile;
											csLoadFile.Format("%s\\%s",csFile.Left(csFile.ReverseFind('\\')),csLine.Mid(csLine.FindOneOf("/\\")+1));
											csLoadFile.Replace('/', '\\');
											if( csLine.Right(4).CompareNoCase(".scp") == 0)
											{
												CStdioFile * pLoadFile = new CStdioFile;
												if ( !pLoadFile->Open(csLoadFile, CFile::modeRead | CFile::shareDenyNone) )
												{
													Main->m_log.Add(1, CFMsg(CMsg("IDS_WARNING_NOOPEN"), csLoadFile));
													continue;
												}
												LoadFile(pLoadFile);
												pLoadFile->Close();
											}
											else
											{
												CString csLoadDir = csLoadFile.Left(csLoadFile.ReverseFind('\\'));
												LoadSingleDirectory(csLoadDir);
											}
										}
									}
								}
								bStatus = pFile->ReadString(csLine);
							}
							break;
						default:
							Main->m_log.Add(1, CFMsg(CMsg("IDS_WARNING_UNKNOWN_SECTION"), csLine, csFile));
							bStatus = pFile->ReadString(csLine);
							break;
						}
					}
					else
					{
						bStatus = pFile->ReadString(csLine);
					}

					if ((bProgress)&&( m_pDlg ))
					{
						j++;
						if ( j % 10 == 0 && m_pDlg)
						{
							m_pDlg->SetPos((DWORD) pFile->GetPosition());
						}
					}
				}
			}
		}
	}
	catch (CFileException *e)
	{
		Main->m_log.Add(1,"ERROR: Caught an exception while reading the file %s.  Cause code = %ld", e->m_strFileName, e->m_cause);
		e->Delete();
	}
	if (m_pDlg)
		m_pDlg->SetPos((DWORD) pFile->GetLength());
	return true;
}

void CScriptObjects::LoadQuicklist(CString csList, CScriptArray * pObjList, CScriptArray * pDestList)
{
	CString csKey;
	csKey.Format("%s\\%s\\Quicklist",REGKEY_PROFILE, Main->m_csCurentProfile);
	CStringArray csaList;
	Main->GetRegistryMultiSz(csList, &csaList, hRegLocation, csKey);

	CreateProgressDialog();

	Main->m_log.Add(0,"Loading %s Quicklist", csList);
	CWaitCursor hourglass;
	m_pDlg->SetRange(0,(unsigned short) csaList.GetSize());
	m_pDlg->SetPos(0);
	m_pDlg->SetWindowText("Loading");
	CString csMessage;
	csMessage.Format("Loading %s Quicklist", csList);
	m_pDlg->m_csMessage.SetWindowText(csMessage);

	if ( csaList.GetSize() > 0 )
	{
		for ( int i = 0; i <= csaList.GetUpperBound(); i++ )
		{
			if ( i % 0x100 )
				m_pDlg->SetPos(i);
			CString csObj = csaList.GetAt(i);
			int iLocated = pObjList->Find(csObj);
			if (iLocated != -1)
			{
				CSObject * pObject = (CSObject *) pObjList->GetAt(iLocated);
				pDestList->Insert(pObject);
			}
		}
	}
	DestroyProgressDialog();
}

void CScriptObjects::LoadProfile(CString csProfile)
{
		Main->m_csCurentProfile = csProfile;

		//No Profile
		if (csProfile == "<None>")
			return;

		Main->m_log.Add(0,"Loading %s Profile", csProfile);
		CreateProgressDialog();

		LoadCustomLocations();

		//Default Profile
		if (csProfile == "<Axis Profile>")
		{
			LoadProfileDirectory(csProfilePath);
			CategorizeObjects(&m_aItems, &m_olItems, "Items", &m_iICatSeq);
			CategorizeObjects(&m_aNPCs, &m_olNPCs, "NPCs", &m_iNCatSeq);
			CategorizeObjects(&m_aAreas, &m_olAreas, "Locations", &m_iACatSeq);
			LoadQuicklist("Items", &m_aItems, &m_ItemQuickList);
			LoadQuicklist("Spawns", &m_aNPCs, &m_SpawnQuickList);
			LoadQuicklist("Area", &m_aAreas, &m_AreaQuickList);
			DestroyProgressDialog();
			return;
		}

		//Local Profile
		CString csKey;
		csKey.Format("%s\\%s",REGKEY_PROFILE, csProfile);
		if(IsLocalProfile(csProfile))
		{
			CStringArray csaScripts;
			Main->GetRegistryMultiSz("Selected Scripts", &csaScripts, hRegLocation, csKey);
			DWORD dwLoadResource = Main->GetRegistryDword("Load Resource", 0, hRegLocation, csKey);
			if ( csaScripts.GetSize() > 0 )
			{
				for ( int i = 0; i <= csaScripts.GetUpperBound(); i++ )
				{
					CString csFile = csaScripts.GetAt(i);
					csFile.Replace('/', '\\');
					CStdioFile * pLocalFile = new CStdioFile;
					if ( !pLocalFile->Open(csFile, CFile::modeRead | CFile::shareDenyNone) )
					{
						Main->m_log.Add(1,"ERROR: Unable to open file %s", csFile);
						continue;
					}
					if(dwLoadResource)
						LoadFile(pLocalFile, 1);
					else
						LoadFile(pLocalFile);
					pLocalFile->Close();
					delete pLocalFile;
				}
			}
			CategorizeObjects(&m_aItems, &m_olItems, "Items", &m_iICatSeq);
			CategorizeObjects(&m_aNPCs, &m_olNPCs, "NPCs", &m_iNCatSeq);
			CategorizeObjects(&m_aAreas, &m_olAreas, "Locations", &m_iACatSeq);
			LoadQuicklist("Items", &m_aItems, &m_ItemQuickList);
			LoadQuicklist("Spawns", &m_aNPCs, &m_SpawnQuickList);
			LoadQuicklist("Area", &m_aAreas, &m_AreaQuickList);
			DestroyProgressDialog();
			return;
		}

		//Web Profile
		else
		{
			CString csURL = Main->GetRegistryString("URL", "", hRegLocation, csKey);
			CInternetSession WebSession("Axis2 - Web Profile");
			//CStdioFile * pFile = NULL;
				try
				{
					CString csInventory;
					CString strServerName;
					CString strObject;
					CString csUsername;
					CString csPassword;
					INTERNET_PORT nPort;
					DWORD dwServiceType;
					DWORD dwFlags = ICU_BROWSER_MODE;

					// check to see if this is a reasonable URL
					if (!AfxParseURLEx(csURL, dwServiceType, strServerName, strObject, nPort, csUsername, csPassword, dwFlags)
						|| (dwServiceType != INTERNET_SERVICE_HTTP && dwServiceType != INTERNET_SERVICE_FTP))
					{
						AfxMessageBox("Error: (Unknown service type) Only http:// and ftp:// allowed");
						if(WebSession)
							WebSession.Close();
						DestroyProgressDialog();
						return;
					}
					//Http Connection
					if(dwServiceType == INTERNET_SERVICE_HTTP)
					{
						//Find Inventory File
						bool bDirect = false;
						csInventory = strObject.Mid(strObject.ReverseFind('/'));
						if(csInventory.Find('.') != -1)
						{
							csInventory = csURL;
							bDirect = true;
						}
						else
							csInventory.Format("%s/AxisSvr.ini",csURL);

						CHttpFile * pHttpFile = (CHttpFile*) WebSession.OpenURL(csInventory, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_EXISTING_CONNECT );
						DWORD dwRet;
						pHttpFile->QueryInfoStatusCode(dwRet);

						while (dwRet == HTTP_STATUS_DENIED)
						{
							CRemoteProfileLoginDlg dlg;
							if ( dlg.DoModal() == IDOK )
							{
								csUsername = dlg.m_csAccountName;
								csPassword = dlg.m_csPassword;
								CString scTemp;
								scTemp.Format("http://%s:%s@%s",csUsername,csPassword,csInventory.Mid(7));
								pHttpFile = (CHttpFile*) WebSession.OpenURL(scTemp, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_EXISTING_CONNECT );
								pHttpFile->QueryInfoStatusCode(dwRet);
								if (dwRet != HTTP_STATUS_OK)
								{
									CString csMessage;
									csMessage.Format("Error: (%d) %s", dwRet, GetInternetCodeString(dwRet));
									AfxMessageBox(csMessage);
								}
							}
							else
							{
								if(WebSession)
									WebSession.Close();
								DestroyProgressDialog();
								return;
							}
						}

						if (dwRet != HTTP_STATUS_OK)
						{
							CString csMessage;
							csMessage.Format("Error: (%d) %s", dwRet, GetInternetCodeString(dwRet));
							AfxMessageBox(csMessage);
						}
						else
						{
							if(bDirect)
							{
								if(pHttpFile)
									pHttpFile->Close();
								CStdioFile * pFile = WebSession.OpenURL(csInventory, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_EXISTING_CONNECT );
								if(pFile)
								{
									LoadFile(pFile, 0, 0);
									pFile->Close();
								}
								else
									Main->m_log.Add(1,"ERROR: Unable to open %s", csInventory);
							}
							else if(pHttpFile)
							{
								CString csLine;
								while (pHttpFile->ReadString(csLine))
								{
									if(csLine != "")
									{
										CString csSubFile;
										csSubFile.Format("%s/%s",csURL,csLine );
										CStdioFile * pFile = WebSession.OpenURL(csSubFile, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_EXISTING_CONNECT );
										if(pFile)
										{
											LoadFile(pFile, 0, 0);
											pFile->Close();
										}
										else
											Main->m_log.Add(1,"ERROR: Unable to open %s", csLine);
									}
								}
							}
						}
						if(pHttpFile)
							pHttpFile->Close();
					}

					//FTP Connection
					else
					{
						if ((csUsername.GetLength() == 0)||(csPassword.GetLength() == 0))
						{
							CRemoteProfileLoginDlg dlg;
							if ( dlg.DoModal() == IDOK )
							{
								csUsername = dlg.m_csAccountName;
								csPassword = dlg.m_csPassword;
							}
							else
							{
								if(WebSession)
									WebSession.Close();
								DestroyProgressDialog();
								return;
							}
						}

						CFtpConnection* pConnect = NULL;
						pConnect = WebSession.GetFtpConnection(strServerName, csUsername, csPassword, nPort, 1);

						//Find Inventory File
						bool bDirect = false;
						csInventory = strObject.Mid(strObject.ReverseFind('/'));
						if(csInventory.Find('.') != -1)
						{
							csInventory = strObject;
							bDirect = true;
						}
						else
							csInventory.Format("%sAxisSvr.ini",strObject);

						CFtpFileFind bOpen(pConnect);
						if(bOpen.FindFile(csInventory))
						{
							if(bDirect)
							{
								CStdioFile * pFile = (CStdioFile *) pConnect->OpenFile(csInventory,GENERIC_READ,FTP_TRANSFER_TYPE_ASCII);
								if(pFile)
								{
									LoadFile(pFile, 0, 0);
									pFile->Close();
								}
								else
									Main->m_log.Add(1,"ERROR: Unable to open %s", csInventory);
							}
							else if(pConnect->GetFile(csInventory,"AxisSvr.ini",0,FILE_ATTRIBUTE_TEMPORARY))
							{
								CStdioFile pTemp;
								if ( pTemp.Open("AxisSvr.ini", CFile::modeRead | CFile::shareDenyNone) )
								{
									CString csLine;
									while (pTemp.ReadString(csLine))
									{
										if(csLine != "")
										{
											CStdioFile * pFile = (CStdioFile *) pConnect->OpenFile(csLine,GENERIC_READ,FTP_TRANSFER_TYPE_ASCII);
											if(pFile)
											{
												LoadFile(pFile, 0, 0);
												pFile->Close();
											}
											else
												Main->m_log.Add(1,"ERROR: Unable to open %s", csLine);
										}
									}
									pTemp.Close();
									pTemp.Remove("AxisSvr.ini");
								}
								else
								{
									AfxMessageBox("Error: Unable to open inventory file AxisSvr.ini");
								}
							}
							else
							{
								CString csMessage;
								csMessage.Format("Error: Unable to Access inventory file %s", csInventory);
								AfxMessageBox(csMessage);
							}
						}
						else
						{
							CString csMessage;
							csMessage.Format("Error: Unable to find inventory file %s", csInventory);
							AfxMessageBox(csMessage);
						}
						if(pConnect)
							pConnect->Close();
					}
				}
				catch (CInternetException* pEx)
				{
					TCHAR szErr[1024];
					pEx->GetErrorMessage(szErr, 1024);
					CString csMessage;
					csMessage.Format("Error: (%d) %s",pEx->m_dwError, szErr);
					AfxMessageBox(csMessage);
					pEx->Delete();
				}
			if(WebSession)
				WebSession.Close();
			CategorizeObjects(&m_aItems, &m_olItems, "Items", &m_iICatSeq);
			CategorizeObjects(&m_aNPCs, &m_olNPCs, "NPCs", &m_iNCatSeq);
			CategorizeObjects(&m_aAreas, &m_olAreas, "Locations", &m_iACatSeq);
			LoadQuicklist("Items", &m_aItems, &m_ItemQuickList);
			LoadQuicklist("Spawns", &m_aNPCs, &m_SpawnQuickList);
			LoadQuicklist("Area", &m_aAreas, &m_AreaQuickList);
			DestroyProgressDialog();
		}
}

void CScriptObjects::LoadProfileDirectory(CString csPath)
{
	WIN32_FIND_DATA findData;
	memset(&findData, 0x00, sizeof(findData));
	CString csTestFile;
	csTestFile.Format("%s\\*", csPath);
	HANDLE hSearch = FindFirstFile(csTestFile, &findData);
	if ( hSearch != INVALID_HANDLE_VALUE )
	{
		BOOL bStatus = TRUE;
		while (bStatus)
		{
			if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if ( strcmp(findData.cFileName, "..") != 0 && strcmp(findData.cFileName, ".") != 0 )
				{
					CString csNewPath;
					csNewPath.Format("%s\\%s", csPath, findData.cFileName);
					LoadProfileDirectory(csNewPath);
				}
			}
			else
			{
				CString csFileName = _T(findData.cFileName);
				if (( csFileName.Right(4).CompareNoCase(".scp") == 0)||(csFileName.CompareNoCase("sphere.ini")==0))
				{
					if (( csFileName.Mid(0,7).CompareNoCase("sphereb") == 0) && (IsNumber(csFileName.Mid(7,2))))
					{
						bStatus = FindNextFile(hSearch, &findData);
						continue;
					}

					CString csFullPath;
					csFullPath.Format("%s\\%s", csPath, findData.cFileName);
					CStdioFile * pDefaultFile = new CStdioFile;
					if ( pDefaultFile->Open(csFullPath, CFile::modeRead | CFile::shareDenyNone) )
					{
						LoadFile(pDefaultFile, 0);
						pDefaultFile->Close();
					}
					else
					{	
						Main->m_log.Add(1,"ERROR: Unable to open file %s", csFullPath);
					}
				}
			}
			bStatus = FindNextFile(hSearch, &findData);
		}
		FindClose(hSearch);
	}
}

void CScriptObjects::LoadSingleDirectory(CString csPath)
{
	WIN32_FIND_DATA findData;
	memset(&findData, 0x00, sizeof(findData));
	CString csTestFile;
	csTestFile.Format("%s\\*", csPath);
	HANDLE hSearch = FindFirstFile(csTestFile, &findData);
	if ( hSearch != INVALID_HANDLE_VALUE )
	{
		BOOL bStatus = TRUE;
		while (bStatus)
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				CString csFileName = _T(findData.cFileName);
				if ( csFileName.Right(4).CompareNoCase(".scp") == 0)
				{
					CString csFullPath;
					csFullPath.Format("%s\\%s", csPath, findData.cFileName);
					CStdioFile * pFile = new CStdioFile;
					if ( pFile->Open(csFullPath, CFile::modeRead | CFile::shareDenyNone) )
					{
						LoadFile(pFile, 0);
						pFile->Close();
					}
					else
					{	
						Main->m_log.Add(1,"ERROR: Unable to open file %s", csFullPath);
					}
				}
			}
			bStatus = FindNextFile(hSearch, &findData);
		}
		FindClose(hSearch);
	}
}

void CScriptObjects::UnloadProfile()
{
	delete Main->m_pScripts;
	Main->m_pScripts = new CScriptObjects;
}

void CScriptObjects::Unload(CScriptArray * pObjList)
{
	for ( int i = 0; i < pObjList->GetSize(); i++ )
	{
		CSObject * pDef = (CSObject *) pObjList->GetAt(i);
		if ( pDef )
			delete pDef;
	}
	pObjList->RemoveAll();
}


void CScriptObjects::CreateProgressDialog()
{
	if ( m_pDlg )
		return;
	m_pDlg = new CProgressBar;
	m_pDlg->Create(IDD_PROGRESS);
	m_bDeleteDialog = true;
}


void CScriptObjects::DestroyProgressDialog()
{
	if ( !m_bDeleteDialog )
		return;
	if ( !m_pDlg )
		return;
	m_pDlg->DestroyWindow();
	delete m_pDlg;
	m_pDlg = NULL;
}

void CScriptObjects::RemoveObjectsCategories(CPtrList * pCatList, CString csName)
{
	Main->m_log.Add(0,"Clearing %s categories", csName);
	if (!pCatList->IsEmpty())
	{
		POSITION catPos = pCatList->GetHeadPosition();
		while (catPos != NULL)
		{
			CCategory * pCat = (CCategory *) pCatList->GetNext(catPos);
			if (! pCat->m_SubsectionList.IsEmpty())
			{
				POSITION subPos = pCat->m_SubsectionList.GetHeadPosition();
				while (subPos != NULL)
				{
					CSubsection *pSub = (CSubsection *) pCat->m_SubsectionList.GetNext(subPos);
					delete (pSub);
				}
			}
			delete (pCat);
		}
	}
	pCatList->RemoveAll();
}

void CScriptObjects::CategorizeObjects(CScriptArray * pObjList, CPtrList * pCatList, CString csName, int * m_iCatSeq)
{
	CreateProgressDialog();

	Main->m_log.Add(0,"Categorizing %s", csName);
	CWaitCursor hourglass;
	m_pDlg->SetRange(0,(unsigned short) pObjList->GetSize());
	m_pDlg->SetPos(0);
	m_pDlg->SetWindowText("Loading");
	CString csMessage;
	csMessage.Format("Categorizing %s", csName);
	m_pDlg->m_csMessage.SetWindowText(csMessage);

	// Go through the item array and organize it into categories.
	for ( int i = 0; i < pObjList->GetSize(); i++ )
	{
		if ( i % 0x100 )
			m_pDlg->SetPos(i);
		CSObject * pItem = (CSObject *) pObjList->GetAt(i);
		if ( pItem )
		{
			if ( pItem->m_csDupeItem != "" )
			{
				int iIndex = pObjList->Find(pItem->m_csDupeItem);
				if ( iIndex == -1 )
					iIndex = pObjList->Find(pObjList->GetDef(pItem->m_csDupeItem));
				if ( iIndex != -1 )
				{
					CSObject * pDupe = (CSObject *) pObjList->GetAt(iIndex);
					if ( pItem->m_csCategory == "<none>" )
						pItem->m_csCategory = pDupe->m_csCategory;
					if ( pItem->m_csSubsection == "<none>" )
						pItem->m_csSubsection = pDupe->m_csSubsection;
					if ( pItem->m_csDescription.Find("<unnamed>") != -1 )
					{
						CString csDesc;
						csDesc.Format("%s - (Dupe)", pDupe->m_csDescription);
						pItem->m_csDescription = csDesc;
					}
					if ( ahextoi(pItem->m_csColor) == 0 )
						pItem->m_csColor = pDupe->m_csColor;
				}
			}
			CString cs_TempCat, cs_Cat;
			if ( pItem->m_csDescription.Find("@") != -1 )
				pItem->m_csDescription.Replace("@", pItem->m_csSubsection);
			if ( pItem->m_csCategory == "<none>" )
			{
				cs_TempCat.Format("<uncategorized %s>",csName);
				cs_TempCat.MakeLower();
				pItem->m_csCategory = cs_TempCat;
			}
			else
			{
				cs_TempCat = pItem->m_csCategory;
				cs_TempCat.MakeLower();
				cs_Cat = cs_TempCat.Left(1);
				cs_Cat.MakeUpper();
				cs_Cat = cs_Cat + cs_TempCat.Mid(1);
				pItem->m_csCategory = cs_Cat;
			}
			if ( pItem->m_csSubsection == "<none>" )
			{
				cs_TempCat = pItem->m_csFilename;
				cs_TempCat = cs_TempCat.Mid(cs_TempCat.ReverseFind('\\')+1);
				cs_TempCat.MakeLower();
				cs_Cat = cs_TempCat.Left(1);
				cs_Cat.MakeUpper();
				cs_Cat = cs_Cat + cs_TempCat.Mid(1);
				pItem->m_csSubsection = cs_Cat;
			}
			else
			{
				cs_TempCat = pItem->m_csSubsection;
				cs_TempCat.MakeLower();
				cs_Cat = cs_TempCat.Left(1);
				cs_Cat.MakeUpper();
				cs_Cat = cs_Cat + cs_TempCat.Mid(1);
				pItem->m_csSubsection = cs_Cat;
			}
			if ( pItem->m_csDescription == "<unnamed>" )
				pItem->m_csDescription = pItem->m_csID;
			else
			{
				cs_TempCat = pItem->m_csDescription;
				cs_TempCat.MakeLower();
				cs_Cat = cs_TempCat.Left(1);
				cs_Cat.MakeUpper();
				cs_Cat = cs_Cat + cs_TempCat.Mid(1);
				pItem->m_csDescription = cs_Cat;
			}

			CCategory * pCategory = FindCategory(pCatList, pItem->m_csCategory);
			CSubsection * pSubsection = FindSubsection(pCategory, pItem->m_csSubsection);

			POSITION pos = pSubsection->m_ItemList.GetHeadPosition();
			POSITION TrackPos = pos;

			if (pos == NULL)
				pSubsection->m_ItemList.AddHead(pItem);
			else
			{
				CSObject * pTest = (CSObject *) pSubsection->m_ItemList.GetAt(pos);

				while (pos && pTest->m_csDescription < pItem->m_csDescription)
				{
					TrackPos = pos;
					pTest = (CSObject *) pSubsection->m_ItemList.GetNext(pos);
				}
				
				if (pos == NULL)
					pSubsection->m_ItemList.AddTail(pItem);
				else if (pTest->m_csDescription == pItem->m_csDescription)
				{
					pSubsection->m_ItemList.InsertAfter(TrackPos,pItem);
				}
				else
					pSubsection->m_ItemList.InsertBefore(TrackPos,pItem);
			}
		}
	}
	DestroyProgressDialog();
	srand( (unsigned)time( NULL ) );
	*m_iCatSeq = rand();
}

void CScriptObjects::LoadCustomLocations()
{
	HKEY hCatKey;
	int iCatIndex = 0;
	int iSubIndex = 0;
	int iDescIndex = 0;
	CString csCategory, csSubsection, csDescription;
	LONG lCatStatus = RegOpenKeyEx(hRegLocation, REGKEY_LOCATION, 0, KEY_ALL_ACCESS, &hCatKey);
	if ( lCatStatus == ERROR_SUCCESS )
	{
		while (lCatStatus == ERROR_SUCCESS)
		{
			char szBuffer[MAX_PATH];
			DWORD dwSize = sizeof(szBuffer);
			lCatStatus = RegEnumKeyEx(hCatKey, iCatIndex, &szBuffer[0], &dwSize, 0, NULL, NULL, NULL);
			if (lCatStatus == ERROR_SUCCESS)
			{
				csCategory = szBuffer;

				HKEY hSubKey;
				CString csSubKey;
				csSubKey.Format("%s\\%s", REGKEY_LOCATION, csCategory);
				LONG lSubStatus = RegOpenKeyEx(hRegLocation, csSubKey, 0, KEY_ALL_ACCESS, &hSubKey);
				if ( lSubStatus == ERROR_SUCCESS )
				{
					while (lSubStatus == ERROR_SUCCESS)
					{
						char szSubBuffer[MAX_PATH];
						DWORD dwSize = sizeof(szSubBuffer);
						lSubStatus = RegEnumKeyEx(hSubKey, iSubIndex, &szSubBuffer[0], &dwSize, 0, NULL, NULL, NULL);
						if (lSubStatus == ERROR_SUCCESS)
						{
							csSubsection = szSubBuffer;

							HKEY hDescKey;
							CString csDescKey;
							csDescKey.Format("%s\\%s", csSubKey, csSubsection);
							LONG lDescStatus = RegOpenKeyEx(hRegLocation, csDescKey, 0, KEY_ALL_ACCESS, &hDescKey);
							if ( lDescStatus == ERROR_SUCCESS )
							{
								while (lDescStatus == ERROR_SUCCESS)
								{
									char szDescBuffer[MAX_PATH];
									DWORD dwSize = sizeof(szDescBuffer);
									lDescStatus = RegEnumKeyEx(hDescKey, iDescIndex, &szDescBuffer[0], &dwSize, 0, NULL, NULL, NULL);
									if (lDescStatus == ERROR_SUCCESS)
									{
										csDescription = szDescBuffer;

										CSObject * pArea = new CSObject;
										pArea->m_bType = TYPE_AREA;
										pArea->m_bCustom = true;
										pArea->m_csCategory = csCategory;
										pArea->m_csSubsection = csSubsection;
										pArea->m_csDescription = csDescription;
										CString csValueKey;
										csValueKey.Format("%s\\%s", csDescKey, csDescription);
										pArea->m_csID = Main->GetRegistryString("Point", "", hRegLocation, csValueKey);
										pArea->m_csDisplay = Main->GetRegistryString("Map", "", hRegLocation, csValueKey);
										m_aAreas.Insert(pArea);
									}
									iDescIndex++;
								}
								RegCloseKey(hDescKey);
							}
						}
						iSubIndex++;
					}
					RegCloseKey(hSubKey);
				}
			}
			iCatIndex++;
		}
		RegCloseKey(hCatKey);
	}
}