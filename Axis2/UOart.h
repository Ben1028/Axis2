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

#if !defined(AFX_UOART_H__8017F402_C6B8_4F3F_B6A5_DB192C01FB65__INCLUDED_)
#define AFX_UOART_H__8017F402_C6B8_4F3F_B6A5_DB192C01FB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comcat.h>
#include <objsafe.h>

class CDefObj
{
public:
	WORD wValue;
	WORD wID;
	int iMul;
};

class CDefArray : public CPtrArray, public CDefObj
{
public:
	int Insert(CDefObj * pDef);
	int Find(WORD wFind);
};

class ArtIdx
{
public:
	DWORD dwLookup;
	DWORD dwSize;
	DWORD dwUnknown;
};

class CHueEntry
{
public:
	WORD wColorTable[32];
	WORD wTableStart;
	WORD wTableEnd;
	CHAR cName[20];
};

class CHueGroup
{
public:
	DWORD dwHeader;
	CHueEntry Hues[8];
};

class CMultiRec
{
public:
	CMultiRec() : hue(0) { }

	WORD wIndex;
	short x;
	short y;
	short z;
	DWORD dwFlags;
	DWORD hue;
};

class ArtAddress
{
public:
	WORD wArtWidth;
	WORD wArtHeight;
	DWORD64 qwAdress;
	DWORD64 qwHash;
	DWORD dwCompressedSize;
};

#pragma pack(1)
struct OldLandTileData
{
	DWORD flags;
	WORD texID;
	char name[20];
};
#pragma pack(pop) 

#pragma pack(1)
struct NewLandTileData
{
	DWORD flags;
	DWORD unk1;
	WORD texID;
	char name[20];
};
#pragma pack(pop) 

#pragma pack(1)
struct OldItemTileData
{
	DWORD Flags;
	char Weight;
	char Quality;
	WORD Unknown;
	char Unknown1;
	char Quantity;
	WORD AnimID;
	char Unknown2;
	char Hue;
	WORD Unknown3;
	char Height;
	char Name[20];
};
#pragma pack(pop) 

#pragma pack(1)
struct NewItemTileData
{
	DWORD Flags;
	DWORD Unknown;
	char Weight;
	char Quality;
	WORD Unknown1;
	char Unknown2;
	char Quantity;
	WORD AnimID;
	char Unknown3;
	char Hue;
	WORD Unknown4;
	char Height;
	char Name[20];
};
#pragma pack(pop) 


/////////////////////////////////////////////////////////////////////////////
// CUOArt class

class CUOArt : public CWnd
{
	OldLandTileData* GetOldLandTileData(int id);
	NewLandTileData* GetNewLandTileData(int id);
	OldItemTileData* GetOldItemTile(int id);
	NewItemTileData* GetNewItemTile(int id);
// Constructor
public:
	CUOArt();
	~CUOArt();

// Implementation
protected:
	void RedrawArt(CPaintDC* pdc, const CRect& rcBounds);
	void DrawArt(short sArtType, CRect bounds, DWORD dwArtIndex, short dx, short dy, short dz, WORD wColor = 0, WORD wFlags = 0);
	void DrawRoomview(CRect bounds, WORD wArtHeight);
	void DrawSwing(CRect bounds, WORD wArtHeight, int idirection = 0);
	void SetInvalid();
	DWORD BlendColors(WORD wBaseColor, WORD wAppliedColor, bool bBlendMode);
	DWORD ScaleColor(WORD wColor);

	BYTE m_TileData[0x30A800];
	WORD m_wArtWidth[0x10000];
	WORD m_wArtHeight[0x10000];
	bool m_bArtDataLoaded;
	bool m_bIsUOAHS;
	void LoadArtData();
	void LoadTiledata();

	CByteArray m_bPixels;
	CByteArray m_bBackGround;
	CRect m_rcSize;
	CPtrArray m_aUopAddress;

	ArtAddress FindUOPEntry(DWORD dwIndex);

	bool m_bIsValid;
	short m_sArtType;
	long m_xOffset;
	long m_yOffset;
	WORD m_wAppliedColour;
	WORD m_wDrawFlags;
	DWORD m_dwArtIndex;
	DWORD m_dwBkColor;

#define F_ROOMVIEW		0x0001
#define F_ENABLESWING   0x0002
#define F_BG_GRASS		0x0004
#define F_SWINGNORTH	0x0010
#define F_SWINGWEST		0x0020
#define F_SWINGNW		0x0100
#define F_SWINGNE		0x0200
#define F_SWINGSW		0x0400
#define F_SWINGSE		0x0800

	//{{AFX_MSG(CUOArt)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Attributes
public:

	int m_wFrame;

	short GetArtType();
	void SetArtType(short);
	long GetArtIndex();
	void SetArtIndex(long);
	short GetArtColor();
	void SetArtColor(short);
	long GetXOffset();
	void SetXOffset(long);
	long GetYOffset();
	void SetYOffset(long);
	short GetDrawFlags();
	void SetDrawFlags(short);
	void ReloadArt();

	void SetObject(short type, long index);
	void SetOffset(long x, long y);
	long yScroll(long offset);
	long xScroll(long offset);
	void Scroll(long x, long y);
	long GetBkColor();
	void SetBkColor(long color);

	CBitmap * OnCreateIcon(int iBitWidth, int iBitHeight, int iID, WORD wAppliedColor, int iType = 1);

	enum ArtTypes
	{
		ART_MULTI,
		ART_ITEM,
		ART_LANDSCAPE,
		ART_NPC,
		ART_NPC2,
		ART_NPC3,
		ART_NPC4,
		ART_NPC5,
		ART_LIGHTS,
	};
};


class CBitmapDC : public CDC
{

public:

  CBitmapDC(int width, int height, COLORREF background = RGB(255,255,255));

  virtual ~CBitmapDC();
  CBitmap *Close();

private:

  CBitmap *m_pBitmap;
  CBitmap *m_pOldBmp;
};

#endif