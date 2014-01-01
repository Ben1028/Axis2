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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>		// MFC socket extensions
#include <direct.h>
#include <afxtempl.h>

#include <afxdlgs.h>

#include <afxinet.h>		//Web Connections
#include <afx.h>		//Web Connections

#include "stdio.h"

#include "AxisLog.h"
#include "Common.h"
#include "Ini.h"
#include "BtnST.h"
#include "ClientInfo.h"
#include "ScriptObjects.h"
#include "RemoteConsole.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#ifndef MAX_BUFFER
#define MAX_BUFFER 1024
#endif

#define KS_ESCAPE	1
#define KS_ALT_F4	2

#define SERVER_VERSION "0.56b"

#define SPAWN_MESSAGE_DELAY 700



// Global functions and variables
extern HWND hwndHoGInstance;
extern HWND hwndUOClient;

extern CString csUOPath;
extern CString csMulPath;
extern CString csProfilePath;
extern CStringArray saClientPaths;
extern CStringArray saMulPaths;
extern HKEY hRegLocation;

extern BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
extern BOOL CALLBACK EnumInstanceProc(HWND hWnd, LPARAM lParam);

extern void AjustComboBox(CComboBox* pmyComboBox);

extern bool SendToUO(CString Cmd);
extern bool IsLocalProfile(CString csProfile);

#define REGKEY_AXIS			"Software\\Sphere\\GM Tools"
#define REGKEY_COMMANDS		"SOFTWARE\\Sphere\\GM Tools\\Commands"
#define REGKEY_REMINDER		"SOFTWARE\\Sphere\\GM Tools\\Reminder"
#define REGKEY_PROFILE		"SOFTWARE\\Sphere\\GM Tools\\Profiles"
#define REGKEY_ACCOUNT		"SOFTWARE\\Sphere\\GM Tools\\Accounts"
#define REGKEY_PALETTE		"SOFTWARE\\Sphere\\GM Tools\\Palette"
#define REGKEY_CONSOLE		"SOFTWARE\\Sphere\\GM Tools\\Console"
#define REGKEY_REMOTELOGIN	"SOFTWARE\\Sphere\\GM Tools\\RemoteLogin"
#define REGKEY_LOCATION		"SOFTWARE\\Sphere\\GM Tools\\Location"
#define REGKEY_OVERRIDEPATH	"SOFTWARE\\Sphere\\GM Tools\\OverridePath"

//Items Atributes
#define ATTR_IDENTIFIED		0x0001	// This is the identified name. ???
#define ATTR_DECAY			0x0002	// Timer currently set to decay.
#define ATTR_NEWBIE			0x0004	// Not lost on death or sellable ?
#define ATTR_MOVE_ALWAYS	0x0008	// Always movable (else Default as stored in client) (even if MUL says not movalble) NEVER DECAYS !
#define ATTR_MOVE_NEVER		0x0010	// Never movable (else Default as stored in client) NEVER DECAYS !
#define ATTR_MAGIC			0x0020	// DON'T SET THIS WHILE WORN! This item is magic as apposed to marked or markable.
#define ATTR_OWNED			0x0040	// This is owned by the town. You need to steal it. NEVER DECAYS !
#define ATTR_INVIS			0x0080	// Gray hidden item (to GM's or owners?)
#define ATTR_CURSED			0x0100
#define ATTR_DAMNED			0x0200	// cursed damned unholy
#define ATTR_BLESSED		0x0400
#define ATTR_SACRED			0x0800	// blessed sacred holy
#define ATTR_FORSALE		0x1000	// For sale on a vendor.
#define ATTR_STOLEN			0x2000	// The item is hot. m_uidLink = previous owner.
#define ATTR_CAN_DECAY		0x4000	// This item can decay. but it would seem that it would not (ATTR_MOVE_NEVER etc)
#define ATTR_STATIC			0x8000	// WorldForge merge marker. (not used)

//Players Stat Flags
#define STATF_INVUL			0x00000001	// Invulnerability
#define STATF_DEAD			0x00000002	// Dead!
#define STATF_Freeze		0x00000004	// Paralyzed. (spell)
#define STATF_Invisible		0x00000008	// Invisible (spell).
#define STATF_Sleeping		0x00000010	// You look like a corpse ?
#define STATF_War			0x00000020	// War mode on ?
#define STATF_Reactive		0x00000040	// have reactive armor on.
#define STATF_Poisoned		0x00000080	// Poison level is in the poison object
#define STATF_NightSight	0x00000100	// All is light to you
#define STATF_Reflection	0x00000200	// Magic reflect on.
#define STATF_Polymorph		0x00000400	// We have polymorphed to another form.
#define STATF_Incognito		0x00000800	// Dont show skill titles
#define STATF_SpiritSpeak	0x00001000	// I can hear ghosts clearly.
#define STATF_Insubstantial	0x00002000	// Ghost has not manifest. or GM hidden
#define STATF_EmoteAction	0x00004000	// The creature will emote its actions to it's owners.
#define STATF_Commcrystal	0x00008000	// I have a IT_COMM_CRYSTAL or listening item on me.
#define STATF_HasShield		0x00010000	// Using a shield
#define STATF_Archercanmove	0x00020000	// Playing a Script. (book script)
#define STATF_Stone			0x00040000	// turned to stone.
#define STATF_Blank1		0x00080000	// Unused
#define STATF_Fly			0x00100000	// Flying or running ? (anim)
#define STATF_Blank2		0x00200000	// Unused
#define STATF_Hallucinating	0x00400000	// eat 'shrooms or bad food.
#define STATF_Hidden		0x00800000	// Hidden (non-magical)
#define STATF_InDoors		0x01000000	// we are covered from the rain.
#define STATF_Criminal		0x02000000	// The guards will attack me. (someone has called guards)
#define STATF_Conjured		0x04000000	// This creature is conjured and will expire. (leave no corpse or loot)
#define STATF_Pet			0x08000000	// I am a pet/hirling. check for my owner memory.
#define STATF_Spawned		0x10000000	// I am spawned by a spawn item.
#define STATF_SaveParity	0x20000000	// Has this char been saved or not ?
#define STATF_Ridden		0x40000000	// This is the horse. (don't display me) I am being ridden
#define STATF_OnHorse		0x80000000	// Mounted on horseback.

//Items Atributes
#define PRIV_GM				0x0002	// Act as a GM
#define PRIV_PAGE			0x0008	// Listen to GM Pages
#define PRIV_HEARALL		0x0010	// Hearall
#define PRIV_ALLMOVE		0x0020	// Allmove
#define PRIV_COMBATDETAIL	0x0040	// Show Combat Detail Messages
#define PRIV_DEBUG			0x0080	// Debug Mode
#define PRIV_SHOWPRIV		0x0200	// Show GM Privileges
#define PRIV_TELNET			0x0400	// Telnet Modificator
#define PRIV_JAILED			0x0800	// Jailed
#define PRIV_BLOCKED		0x2000	// Blocked/Banned
#define PRIV_ALLSHOW		0x4000	// Allshow

//Mul Files
enum VERFILE_MULFILE
{
	VERFILE_MAP0,
	VERFILE_MAP1,
	VERFILE_MAP2,
	VERFILE_MAP3,
	VERFILE_MAP4,
	VERFILE_MAP5,

	VERFILE_MAPDIF0,
	VERFILE_MAPDIF1,
	VERFILE_MAPDIF2,
	VERFILE_MAPDIF3,
	VERFILE_MAPDIF4,
	VERFILE_MAPDIF5,

	VERFILE_MAPDIFL0,
	VERFILE_MAPDIFL1,
	VERFILE_MAPDIFL2,
	VERFILE_MAPDIFL3,
	VERFILE_MAPDIFL4,
	VERFILE_MAPDIFL5,

	VERFILE_STATICS0,
	VERFILE_STATICS1,
	VERFILE_STATICS2,
	VERFILE_STATICS3,
	VERFILE_STATICS4,
	VERFILE_STATICS5,

	VERFILE_STAIDX0,
	VERFILE_STAIDX1,
	VERFILE_STAIDX2,
	VERFILE_STAIDX3,
	VERFILE_STAIDX4,
	VERFILE_STAIDX5,

	VERFILE_STADIF0,
	VERFILE_STADIF1,
	VERFILE_STADIF2,
	VERFILE_STADIF3,
	VERFILE_STADIF4,
	VERFILE_STADIF5,

	VERFILE_STADIFI0,
	VERFILE_STADIFI1,
	VERFILE_STADIFI2,
	VERFILE_STADIFI3,
	VERFILE_STADIFI4,
	VERFILE_STADIFI5,

	VERFILE_STADIFL0,
	VERFILE_STADIFL1,
	VERFILE_STADIFL2,
	VERFILE_STADIFL3,
	VERFILE_STADIFL4,
	VERFILE_STADIFL5,

	VERFILE_ART,
	VERFILE_ARTIDX,

	VERFILE_ANIM,
	VERFILE_ANIMIDX,
	VERFILE_ANIM2,
	VERFILE_ANIMIDX2,
	VERFILE_ANIM3,
	VERFILE_ANIMIDX3,
	VERFILE_ANIM4,
	VERFILE_ANIMIDX4,
	VERFILE_ANIM5,
	VERFILE_ANIMIDX5,

	VERFILE_LIGHTIDX,
	VERFILE_LIGHT,

	VERFILE_TILEDATA,
	VERFILE_HUES,
	VERFILE_RADARCOL,

	VERFILE_SOUND,
	VERFILE_SOUNDIDX,
	VERFILE_MUSICLIST,

	VERFILE_BODYCONVDEF,
	VERFILE_BODYDEF,

	VERFILE_MULTIIDX,
	VERFILE_MULTI,

	VERFILE_MAP0_UOP,
	VERFILE_MAP1_UOP,
	VERFILE_MAP2_UOP,
	VERFILE_MAP3_UOP,
	VERFILE_MAP4_UOP,
	VERFILE_MAP5_UOP,

	VERFILE_ART_UOP,

	VERFILE_QTY,
};

//Predefined Item Types
enum ITEM_TYPE		// double click type action.
{
	ITEM_NORMAL = 0,
	ITEM_CONTAINER = 1,		// any unlocked container or corpse. CContainer based
	ITEM_CONTAINER_LOCKED,	// 2 = 
	ITEM_DOOR,				// 3 = door can be opened
	ITEM_DOOR_LOCKED,		// 4 = a locked door.
	ITEM_KEY,				// 5 = 
	ITEM_LIGHT_LIT,			// 6 = Local Light giving object
	ITEM_LIGHT_OUT,			// 7 = Can be lit.
	ITEM_FOOD,				// 8 = edible food. (poisoned food ?)
	ITEM_FOOD_RAW,			// 9 = Must cook raw meat unless your an animal.
	ITEM_ARMOR,				// 10 = some type of armor. (no real action)
	ITEM_WEAPON_MACE_SMITH,	// 11 = Can be used for smithing 
	ITEM_WEAPON_MACE_SHARP,	// 12 = war axe can be used to cut/chop trees.
	ITEM_WEAPON_SWORD,		// 13 = 
	ITEM_WEAPON_FENCE,		// 14 = can't be used to chop trees. (make kindling)
	ITEM_WEAPON_BOW,		// 15 = bow or xbow
	ITEM_WAND,			    // 16 = A magic storage item
	ITEM_TELEPAD,			// 17 = walk on teleport
	ITEM_SWITCH,			// 18 = this is a switch which effects some other object in the world.
	ITEM_BOOK,				// 19 = read this book. (static or dynamic text)
	ITEM_RUNE,				// 20 = can be marked and renamed as a recall rune.
	ITEM_BOOZE,				// 21 = booze
	ITEM_POTION,			// 22 = Some magic effect.	
	ITEM_FIRE,				// 23 = It will burn you.
	ITEM_CLOCK,				// 24 = or a wristwatch
	ITEM_TRAP,				// 25 = walk on trap.
	ITEM_TRAP_ACTIVE,		// 26 = some animation 
	ITEM_MUSICAL,			// 27 = a musical instrument.
	ITEM_SPELL,				// 28 = magic spell effect.
	ITEM_GEM,				// 29 = no use yet 
	ITEM_WATER,				// 30 = This is water (fishable)
	ITEM_CLOTHING,			// 31 = All cloth based wearable stuff,
	ITEM_SCROLL,			// 32 = magic scroll
	ITEM_CARPENTRY,			// 33 = tool of some sort.
	ITEM_SPAWN_CHAR,		// 34 = spawn object. should be invis also.
	ITEM_GAME_PIECE,		// 35 = can't be removed from game.
	ITEM_PORTCULIS,			// 36 = Z delta moving gate. (open)
	ITEM_FIGURINE,			// 37 = magic figure that turns into a creature when activated.
	ITEM_SHRINE,			// 38 = can res you
	ITEM_MOONGATE,			// 39 = linked to other moon gates (hard coded locations)	
	ITEM_CHAIR,				// 40 = Any sort of a chair item. we can sit on.
	ITEM_FORGE,				// 41 = used to smelt ore, blacksmithy etc.
	ITEM_ORE,				// 42 = smelt to ingots.
	ITEM_LOG,				// 43 = make into furniture etc. lumber,logs, 
	ITEM_TREE,				// 44 = can be chopped.
	ITEM_ROCK,				// 45 = can be mined for ore.
	ITEM_CARPENTRY_CHOP,	// 46 = tool of some sort.
	ITEM_MULTI,				// 47 = multi part object like house or ship.
	ITEM_REAGENT,			// 48 = alchemy when clicked ?
	ITEM_SHIP,				// 49 = this is a SHIP MULTI
	ITEM_SHIP_PLANK,		// 50
	ITEM_SHIP_SIDE,			// 51 = Should extend to make a plank
	ITEM_SHIP_SIDE_LOCKED,	// 52
	ITEM_SHIP_TILLER,		// 53 = Tiller man on the ship.
	ITEM_EQ_TRADE_WINDOW,	// 54 = container for the trade window.
	ITEM_FISH,				// 55 = fish can be cut up.
	ITEM_SIGN_GUMP,			// 56 = Things like grave stones and sign plaques
	ITEM_STONE_GUILD,		// 57 = Guild/town stones
	ITEM_ANIM_ACTIVE,		// 58 = active anium that will recycle when done.
	ITEM_ADVANCE_GATE,		// 59 = advance gate. m_AdvanceGateType = CREID_TYPE
	ITEM_CLOTH,				// 60 = bolt or folded cloth
	ITEM_HAIR,				// 61
	ITEM_BEARD,				// 62 = just for grouping purposes.
	ITEM_INGOT,				// 63 = Ingot.
	ITEM_COIN,				// 64 = coin of some sort. gold or otherwise.
	ITEM_PLANT,				// 65 = a plant that will regrow. picked type.
	ITEM_DRINK,				// 66 = some sort of drink (non booze)
	ITEM_ANVIL,				// 67 = for repair.
	ITEM_PORT_LOCKED,		// 68 = this portcullis must be triggered.
	ITEM_SPAWN_ITEM,		// 69 = spawn other items.
	ITEM_TELESCOPE,			// 70 = big telescope pic.
	ITEM_BED_EW,			// 71 = EW facing bed.
	ITEM_BED_NS,			// 72 = NS facing bed
	ITEM_MAP,				// 73 = Map object with pins.
	ITEM_EQ_MEMORY_OBJ,		// 74 = A Char has a memory link to some object. (I am fighting with someone. This records the fight.)
	ITEM_WEAPON_MACE_STAFF,	// 75 = staff type of mace. or just other type of mace.
	ITEM_EQ_HORSE,			// 76 = equipped horse object represents a riding horse to the client.
	ITEM_COMM_CRYSTAL,		// 77 = communication crystal.
	ITEM_GAME_BOARD,		// 78 = this is a container of pieces.
	ITEM_TRASH,				// 79 = delete any object dropped on it.
	ITEM_CANNON_MUZZLE,		// 80 = cannon muzzle. NOT the other cannon parts.
	ITEM_CANNON,			// 81 = the reset of the cannon.
	ITEM_CANNON_BALL,		// 82
	ITEM_PLANT_FRUITING,	// 83 = fruit can be picked off this.
	ITEM_PLANT_IMATURE,		// 84 = can't be picked yet.
	ITEM_RING_REAGENTS,		// 85 = ring of reagents.
	ITEM_CRYSTAL_BALL,		// 86
	ITEM_CASHIERS_CHECK,	// 87 = represents large amounts of money.
	ITEM_MESSAGE,			// 88 = user written message item. (for bboard ussually)
	ITEM_REAGENT_RAW,		// 89 = Freshly grown reagents...not processed yet.
	ITEM_EQ_CLIENT_LINGER,	// 90 = Change player to NPC for a while.
	ITEM_DREAM_GATE,		// 91 = Push you to another shard. (no items transfered, client insta logged out)
	ITEM_ITEM_STONE,		// 92 = Double click for items
	ITEM_METRONOME,			// 93 = ticks once every n secs.
	ITEM_EXPLOSION,			// 94 = async explosion.
	ITEM_EQ_NPC_SCRIPT,		// 95 = Script npc actions in the form of a book.
	ITEM_WEB,				// 96 = walk on this and transform into some other object.
	ITEM_GRASS,				// 97 = can be eaten by grazing animals
	ITEM_AROCK,				// 98 = a rock or boulder. can be thrown by giants.
	ITEM_TRACKER,			// 99 = points to a linked object.
	ITEM_SOUND,				// 100 = this is a sound source.
	ITEM_STONE_TOWN,		// 101 = Town stones. everyone free to join.
	ITEM_WEAPON_MACE_CROOK,	// 102
	ITEM_WEAPON_MACE_PICK,	// 103
	ITEM_LEATHER,			// 104 = Leather or skins of some sort.(not wearable)
	ITEM_SHIP_OTHER,		// 105 = some other part of a ship.
	ITEM_BBOARD,			// 106 = a container and bboard object.
	ITEM_SPELLBOOK,			// 107 = spellbook (with spells)
	ITEM_CORPSE,			// 108 = special type of item.
	ITEM_TRACK_ITEM,		// 109 - track a id or type of item.
	ITEM_TRACK_CHAR,		// 110 = track a char or range of char id's
	ITEM_WEAPON_ARROW,		// 111
	ITEM_WEAPON_BOLT,		// 112
	ITEM_EQ_VENDOR_BOX,		// 113 = an equipped vendor .
	ITEM_EQ_BANK_BOX,		// 114 = an equipped bank box
	ITEM_DEED,				// 115
	ITEM_LOOM,				// 116
	ITEM_BEE_HIVE,			// 117
	ITEM_ARCHERY_BUTTE,		// 118
	ITEM_EQ_MURDER_COUNT,	// 119 = my murder count flag.
	ITEM_EQ_STUCK,			// 120 = we are stuck in a web
	ITEM_TRAP_INACTIVE,		// 121 = a safe trap.
	ITEM_STONE_ROOM,		// 122 = house type stone. (for mapped house regions)
	ITEM_BANDAGE,			// 123 = can be used for healing.
	ITEM_CAMPFIRE,			// 124 = this is a fire but a small one.
	ITEM_MAP_BLANK,			// 125 = blank map.
	ITEM_SPY_GLASS,			// 126
	ITEM_SEXTANT,			// 127
	ITEM_SCROLL_BLANK,		// 128
	ITEM_FRUIT,				// 129
	ITEM_WATER_WASH,		// 130 = water that will not contain fish. (for washing or drinking)
	ITEM_WEAPON_AXE,		// 131 = not the same as a sword. but uses swordsmanship skill
	ITEM_WEAPON_XBOW,		// 132
	ITEM_SPELLICON,			// 133
	ITEM_DOOR_OPEN,			// 134 = You can walk through doors that are open.
	ITEM_MEAT_RAW,			// 135 = raw (uncooked meat) or part of a corpse.
	ITEM_GARBAGE,			// 136 = this is junk.
	ITEM_KEYRING,			// 137
	ITEM_TABLE,				// 138 = a table top
	ITEM_FLOOR,				// 139
	ITEM_ROOF,				// 140
	ITEM_FEATHER,			// 141 = a birds feather
	ITEM_WOOL,				// 142 = Wool cut frm a sheep.
	ITEM_FUR,				// 143
	ITEM_BLOOD,				// 144 = blood of some creature
	ITEM_FOLIAGE,			// 145 = does not go invis when reaped. but will if eaten
	ITEM_GRAIN,				// 146
	ITEM_SCISSORS,			// 147
	ITEM_THREAD,			// 148
	ITEM_YARN,				// 149
	ITEM_SPINWHEEL,			// 150
	ITEM_BANDAGE_BLOOD,		// 151 = must be washed in water to get bandage back.
	ITEM_FISH_POLE,			// 152
	ITEM_SHAFT,				// 153 = used to make arrows and xbolts
	ITEM_LOCKPICK,			// 154
	ITEM_KINDLING,			// 155 = lit to make campfire
	ITEM_TRAIN_DUMMY,		// 156
	ITEM_TRAIN_PICKPOCKET,	// 157
	ITEM_BEDROLL,			// 158
	ITEM_BELLOWS,			// 159
	ITEM_HIDE,				// 160 = hides are cured to make leather.
	ITEM_CLOTH_BOLT,		// 161 = must be cut up to make cloth squares.
	ITEM_LUMBER,			// 162 = logs are plained into decent lumber
	ITEM_PITCHER,			// 163
	ITEM_PITCHER_EMPTY,		// 164
	ITEM_DYE_VAT,			// 165
	ITEM_DYE,				// 166
	ITEM_POTION_EMPTY,		// 167 = empty bottle.
	ITEM_MORTAR,			// 168
	ITEM_HAIR_DYE,			// 169
	ITEM_SEWING_KIT,		// 170
	ITEM_TINKER_TOOLS,		// 171
	ITEM_WALL,				// 172 = wall of a structure.
	ITEM_WINDOW,			// 173 = window for a structure.
	ITEM_COTTON,			// 174 = Cotton from the plant
	ITEM_BONE,				// 175
	ITEM_EQ_SCRIPT,			// 176
	ITEM_SHIP_HOLD,			// 177 = ships hold.
	ITEM_SHIP_HOLD_LOCK,	// 178
	ITEM_LAVA,				// 179
	ITEM_SHIELD,			// 180 = equippable armor.
	ITEM_JEWELRY,			// 181 = equippable.
	ITEM_DIRT,				// 182 = a patch of dirt where i can plant something
	ITEM_SCRIPT,			// 183
	ITEM_SPELLBOOK_NECRO,	// 184 = AOS Necromancy spellbook
	ITEM_SPELLBOOK_PALA,	// 185 = AOS Paladin spellbook
	ITEM_SPELLBOOK_EXTRA,	// 186 = some spellbook for script purposes
	ITEM_QTY,

	ITEM_TRIGGER		= 1000,	// Create custom new script trigger types
};

//Folder Picker Class
class CFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderDialog)

public:
	static WNDPROC m_wndProc;
	virtual void OnInitDone( );
	CString* m_pPath;
	CString m_Title;
	CFolderDialog(CString* pPath, CString csTitle);

protected:
	//{{AFX_MSG(CFolderDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
