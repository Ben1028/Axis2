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

#include "stdafx.h"
#include "Axis2.h"
#include "UOart.h"
#include <comcat.h>
#include <objsafe.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CUOArt, CWnd)
	//{{AFX_MSG_MAP(CUOArt)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUOArt::CUOArt - Constructor

CUOArt::CUOArt()
{
	m_bIsValid = false;	
	m_wAppliedColour = 0;
	m_dwArtIndex = 0;
	m_sArtType = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_wDrawFlags = 0;
	m_bArtDataLoaded = false;
	LoadArtData();
	LoadTiledata();
}


/////////////////////////////////////////////////////////////////////////////
// CUOArtCtrl::~CUOArtCtrl - Destructor

CUOArt::~CUOArt()
{
	m_bPixels.RemoveAll();
}

void CUOArt::SetInvalid()
{  
	m_bIsValid = false;
	Invalidate();
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CUOArt::OnPaint - Drawing function

void CUOArt::OnPaint() 
{
		CPaintDC pdc(this); // device context for painting
		CRect rcBounds;
		GetClientRect(&rcBounds);
		if ( rcBounds.Width() != m_rcSize.Width() || rcBounds.Height() != m_rcSize.Height() )
		{
			SetInvalid();
			m_rcSize = rcBounds;
		}
		RedrawArt(&pdc, rcBounds);
}

void CUOArt::RedrawArt(CPaintDC* pdc, const CRect& rcBounds)
{
	int RectWidth = rcBounds.Width() - (rcBounds.Width() % 8);
	int RectHeight = rcBounds.Height() - (rcBounds.Height() % 8);
	CRect rcNewRect(0, 0, RectWidth, RectHeight);
	if ( !m_bIsValid )
	{
		m_bPixels.RemoveAll();
		m_bPixels.SetSize(RectWidth * RectHeight * 3);

		BYTE r, g, b;
		b = (BYTE) ((m_dwBkColor >> 16) & 0xFF);
		g = (BYTE) ((m_dwBkColor >> 8) & 0xFF);
		r = (BYTE) ((m_dwBkColor) & 0xFF);
		for ( int i = 0; i < ( RectWidth * RectHeight ); i++ )
		{
			m_bPixels.SetAt(i * 3, b);
			m_bPixels.SetAt(i * 3 + 1, g);
			m_bPixels.SetAt(i * 3 + 2, r);
		}

		// Paint the background first...
		BITMAPINFO bmi;
		memset( &bmi, 0, sizeof( bmi ));
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biHeight = -RectHeight;	// - Top down
		bmi.bmiHeader.biWidth = RectWidth;

		bmi.bmiHeader.biBitCount = 24;	// 1,4,8,24
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biCompression = BI_RGB;	// Not compressed
		bmi.bmiHeader.biSizeImage = 0;
		bmi.bmiHeader.biXPelsPerMeter= 0;
		bmi.bmiHeader.biYPelsPerMeter= 0;
		bmi.bmiHeader.biClrUsed 	 = 0;
		bmi.bmiHeader.biClrImportant = 0;

		SetDIBitsToDevice(pdc->GetSafeHdc(), rcBounds.left, rcBounds.top, RectWidth, RectHeight, 0, 0, 0, RectHeight, m_bPixels.GetData(), &bmi, DIB_RGB_COLORS);
		//background end

		if	( m_wDrawFlags & F_BG_GRASS )
		{
			int stagger = 0;
			for(int xoff = -(RectWidth/2); xoff <= (RectWidth/2)+22; xoff += 22)
			{
				if (stagger)
					stagger = 0;
				else
					stagger = 22;
				for(int yoff = -(RectHeight/2)+stagger; yoff <= (RectHeight/2)+22; yoff += 44)
				{
					DrawArt(2, rcNewRect, 0x3, xoff, yoff, 0, 0, m_wDrawFlags);
				}
			}
		}


		if (m_dwArtIndex == -1)
			return;

		DWORD dwIndex;
		// Make sure the applied color setting is correct

		switch( m_sArtType )
		{
		case ART_ITEM:
		case ART_NPC:
		case ART_NPC2:
		case ART_NPC3:
		case ART_NPC4:
		case ART_NPC5:
			break;
		default:
			m_wAppliedColour = 0;
			break;
		}
		// What we do here depends on the art type
		dwIndex = m_dwArtIndex;

		switch( m_sArtType )
		{
		case ART_MULTI:
			dwIndex = m_dwArtIndex; // - 0x8000;
			if ( m_dwArtIndex >= 0x1FF6 )
				return;
			break;
		case ART_ITEM:
			dwIndex = m_dwArtIndex + 0x4000;
			if ( m_dwArtIndex >= 0x8000 )
				return;
			break;
		case ART_LANDSCAPE:
			if ( m_dwArtIndex >= 0x4000 )
				return;
			break;

		case ART_NPC:
		case ART_NPC2:
		case ART_NPC3:
		case ART_NPC4:
		case ART_NPC5:
			{
				if ( m_dwArtIndex >= 0x1000 )
					return;
				DWORD dwArtIndex = m_dwArtIndex;
				// See if we find this guy in body.bef
				int i = Main->m_daBodydef.Find((WORD) m_dwArtIndex);
				if ( i != -1 )
				{
					CDefObj * pDef = (CDefObj *) Main->m_daBodydef.GetAt(i);
					dwArtIndex = pDef->wID;
				}
				int iMul = 1;
				// Check to see which anim*.mul
				int j = Main->m_daBodyConv.Find((WORD) m_dwArtIndex);
				if ( j != -1 )
				{
					CDefObj * pMulDef = (CDefObj *) Main->m_daBodyConv.GetAt(j);
					dwArtIndex = pMulDef->wID;
					iMul = pMulDef->iMul;
				}

				switch ( iMul )
				{
				case 1:
					{
						m_sArtType = ART_NPC;
						if ( dwArtIndex < 0xC8 )		//High Details
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x190 )	//Low Details
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else							//Humans - Wearables
							dwIndex = ( dwArtIndex - 0x190) * 175 + 35000;
					}
					break;
				case 2:
					{
						// Anim2
						m_sArtType = ART_NPC2;
						if ( dwArtIndex == 0x44 )		//Kirin Fix
							dwIndex = 13420;
						else if ( dwArtIndex < 0xC8 )	//High Details
							dwIndex = dwArtIndex * 110;
						else							//Low Details
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
					}
					break;
				case 3:
					{
						// Anim3
						m_sArtType = ART_NPC3;
						if ( dwArtIndex == 0x5F )		//Turkey Fix
							dwIndex = 15175;
						else if ( dwArtIndex < 0x190 )	//High Details
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x258 )	//Low Details
							dwIndex = ( dwArtIndex - 0x190 ) * 65 + 44000;
						else							//Humans - Wearables
							dwIndex = ( dwArtIndex - 0x258 ) * 175 + 70000;
					}
					break;
				case 4:
					{
						// Anim4
						m_sArtType = ART_NPC4;
						if ( dwArtIndex < 0xC8 )		//High Details
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x190 )	//Low Details
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else							//Humans - Wearables
							dwIndex = ( dwArtIndex - 0x190) * 175 + 35000;
					}
					break;
				case 5:
					{
						// Anim5
						m_sArtType = ART_NPC5;
						if ( dwArtIndex == 0x22 )		//Chimera Fix
							dwIndex = 11210;
						else if ( dwArtIndex < 0xC8 )	//High Details
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x190 )	//Low Details
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else							//Humans - Wearables
							dwIndex = ( dwArtIndex - 0x190) * 175 + 35000;
					}
					break;
				}
				if (m_wFrame > 0)
					dwIndex += m_wFrame;
				else
					dwIndex -= m_wFrame;
			}
			break;
		case ART_LIGHTS:
			{
				if ( m_dwArtIndex >= 100 )
					return;
				DrawArt(1, rcNewRect, 0x4b22, 0, 0, 0, 0, m_wDrawFlags);
			}
			break;
		}

		if ( m_wDrawFlags & F_SWINGNORTH )
		{
			WORD wFlags = m_wDrawFlags;
			wFlags &= ~0x012;
			if	( m_wDrawFlags & F_SWINGNW )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, -22, -22, 0, 1153, wFlags);
			if	( m_wDrawFlags & F_SWINGNE )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, 44, 0, 0, 1153, wFlags);
			if	( m_wDrawFlags & F_SWINGSW )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, -44, 0, 0, 1153, wFlags);
		}
		if ( m_wDrawFlags & F_SWINGWEST )
		{
			WORD wFlags = m_wDrawFlags;
			wFlags &= ~0x022;
			if	( m_wDrawFlags & F_SWINGNW )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, 0, 0, 0, 1153, wFlags);
			if	( m_wDrawFlags & F_SWINGNE )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, 22, -22, 0, 1153, wFlags);
			if	( m_wDrawFlags & F_SWINGSE )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, 44, 0, 0, 1153, wFlags);
		}

		DrawArt(m_sArtType, rcNewRect, dwIndex, 0, 0, 0, m_wAppliedColour, m_wDrawFlags);

		if ( m_wDrawFlags & F_SWINGNORTH )
		{
			WORD wFlags = m_wDrawFlags;
			wFlags &= ~0x012;
			if	( m_wDrawFlags & F_SWINGSE )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, 0, 34, 0, 1153, wFlags);
		}
		if ( m_wDrawFlags & F_SWINGWEST )
		{
			WORD wFlags = m_wDrawFlags;
			wFlags &= ~0x022;
			if	( m_wDrawFlags & F_SWINGSW )
				DrawArt(m_sArtType, rcNewRect, dwIndex+1, 0, 34, 0, 1153, wFlags);
		}
	}
	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( bmi ));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biHeight = -RectHeight;	// - Top down
	bmi.bmiHeader.biWidth = RectWidth;
	bmi.bmiHeader.biBitCount = 24;	// 1,4,8,24
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;	// Not compressed
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter= 0;
	bmi.bmiHeader.biYPelsPerMeter= 0;
	bmi.bmiHeader.biClrUsed 	 = 0;
	bmi.bmiHeader.biClrImportant = 0;

	SetDIBitsToDevice(pdc->GetSafeHdc(), rcBounds.left, rcBounds.top, RectWidth, RectHeight, 0, 0, 0, RectHeight, m_bPixels.GetData(), &bmi, DIB_RGB_COLORS);
	m_bIsValid = true;
	return;
}

void CUOArt::DrawArt(short sArtType, CRect bounds, DWORD dwArtIndex, short dx, short dy, short dz, WORD wAppliedColor, WORD wFlags)
{
	UNREFERENCED_PARAMETER(dz);
	CFile fData, fIndex;
	CString csDataFile, csIndexFile;

	switch ( sArtType )
	{
	case ART_MULTI:
		csDataFile = Main->GetMulPath(VERFILE_MULTI);
		csIndexFile = Main->GetMulPath(VERFILE_MULTIIDX);
		break;
	case ART_ITEM:
	case ART_LANDSCAPE:
		csDataFile = Main->GetMulPath(VERFILE_ART);
		csIndexFile = Main->GetMulPath(VERFILE_ARTIDX);
		break;
	case ART_NPC:
		csDataFile = Main->GetMulPath(VERFILE_ANIM);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX);
		break;
	case ART_NPC2:
		csDataFile = Main->GetMulPath(VERFILE_ANIM2);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX2);
		break;
	case ART_NPC3:
		csDataFile = Main->GetMulPath(VERFILE_ANIM3);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX3);
		break;
	case ART_NPC4:
		csDataFile = Main->GetMulPath(VERFILE_ANIM4);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX4);
		break;
	case ART_NPC5:
		csDataFile = Main->GetMulPath(VERFILE_ANIM5);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX5);
		break;
	case ART_LIGHTS:
		csDataFile = Main->GetMulPath(VERFILE_LIGHT);
		csIndexFile = Main->GetMulPath(VERFILE_LIGHTIDX);
		break;
	}
	bool isUOP = false;

	if ( !fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		//maybe it's in .uop format?
		if ((sArtType == ART_ITEM)||(sArtType == ART_LANDSCAPE))
		{
			csDataFile = Main->GetMulPath(VERFILE_ART_UOP);
			if ( fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			{
				isUOP = true;
			}
			else
				return;
		}
		else
			return;
	}
	ArtIdx indexRec;

	if (!isUOP)
	{
		if ( !fIndex.Open(csIndexFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		{
			fData.Close();
			return;
		}

		if (fIndex.Seek(dwArtIndex * sizeof(ArtIdx), CFile::begin) > fIndex.GetLength())
			return;
		fIndex.Read(&indexRec, sizeof(ArtIdx));
	}
	else
	{
		ArtAddress pArt = FindUOPEntry(dwArtIndex);
		indexRec.dwLookup = (DWORD)pArt.qwAdress;
		indexRec.dwSize = pArt.dwCompressedSize;
	}

	if (( indexRec.dwLookup != 0xFFFFFFFF )&&( indexRec.dwSize > 0 ))
	{
		DWORD dwType;
		switch ( sArtType )
		{
		case ART_LANDSCAPE:
			dwType = 0;
			break;
		case ART_ITEM:
			dwType = 1;
			break;
		case ART_NPC:
		case ART_NPC2:
		case ART_NPC3:
		case ART_NPC4:
		case ART_NPC5:
			dwType = 2;
			break;
		case ART_LIGHTS:
			dwType = 3;
			break;
		case ART_MULTI:
			dwType = 4;
			break;
		default:
			dwType = 0xFFFF;
			break;
		}

		switch (dwType)
		{
		case 0:
			{
				//Land Data
				try
				{
					BYTE * bData = NULL;
					DWORD dwOffset = 4;
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);

					// Raw data
					short wArtWidth, wArtHeight;
					wArtWidth = wArtHeight = 44;
					for ( int y = 0; y < 45; y++ )
					{
						int width = y < 23 ? (y * 2) : ((45 - y) * 2);
						for ( int x = (22 - width / 2); x < (22 + width / 2); x++ )
						{
							WORD wColor;
							memcpy(&wColor, &bData[dwOffset], 2);
							dwOffset += 2;
							int X, Y;
							X = m_xOffset + dx + x + ((bounds.Width() - wArtWidth) / 2 );
							Y = m_yOffset + dy + y + ((bounds.Height() / 2) - (wArtHeight / 2));
							if ( X >= 0 && X < bounds.Width() && Y >= 0 && Y < bounds.Height() )
							{
								DWORD dwColor = BlendColors(wColor, wAppliedColor, false);
								BYTE r, g, b;
								b = (BYTE) ((dwColor >> 16) & 0xFF);
								g = (BYTE) ((dwColor >> 8) & 0xFF);
								r = (BYTE) ((dwColor) & 0xFF);
								m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3), b);
								m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 1, g);
								m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 2, r);
							}
						}
					}
					
				}
				catch(...)
				{
				}
			}
			break;
		case 1:
			{
				//Item Data
				try
				{
					BYTE * bData = NULL;
					DWORD dwOffset = 4;
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);

					// Regular item data
					WORD wArtWidth, wArtHeight;
					memcpy(&wArtWidth, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&wArtHeight, &bData[dwOffset], 2);
					dwOffset += 2;
					if ( wArtWidth == 0 || wArtWidth > 1024 || wArtHeight == 0 || wArtHeight > 1024 )
						throw 1;
					WORD * lineStart = new WORD [wArtHeight];
					for ( int i = 0; i < wArtHeight; i++ )
					{
						memcpy(&lineStart[i], &bData[dwOffset], 2);
						dwOffset += 2;
					}
					DWORD dataStart = dwOffset; //fData.GetPosition();
					int x, y;
					x = y = 0;
					// Draw the roomview thing?
					if ( wFlags & F_ROOMVIEW )
						DrawRoomview(bounds, wArtHeight);
					while ( y < wArtHeight )
					{
						WORD xOffset, xRun;
						memcpy(&xOffset, &bData[dwOffset], 2);
						dwOffset += 2;
						memcpy(&xRun, &bData[dwOffset], 2);
						dwOffset += 2;
						if ( (xRun + xOffset) > 2048 )
							break;
						else
						{
							if ( ( xRun + xOffset ) != 0 )
							{
								x+= xOffset;
								WORD * wColor = new WORD [xRun];
								memcpy(&wColor[0], &bData[dwOffset], 2 * xRun);
								dwOffset += 2 * xRun;
								for (int iCount = 0; iCount < xRun; iCount++)
								{
									int X, Y;
									X = m_xOffset + dx + (x + iCount) + ((bounds.Width() - wArtWidth) / 2 );
									Y = m_yOffset + dy + y + ((bounds.Height() - wArtHeight) / 2);
									if ( X >= 0 && X < bounds.Width() && Y >= 0 && Y < bounds.Height() )
									{
										DWORD dwColor = BlendColors(wColor[iCount], wAppliedColor, false);
										BYTE r, g, b;
										b = (BYTE) ((dwColor >> 16) & 0xFF);
										g = (BYTE) ((dwColor >> 8) & 0xFF);
										r = (BYTE) ((dwColor) & 0xFF);
										m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3), b);
										m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 1, g);
										m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 2, r);
									}
								}
								delete [] wColor;
								x += xRun;
							}
							else
							{
								x = 0;
								y++;
								dwOffset = lineStart[y] * 2 + dataStart;
							}
						}
					}
					if ( wFlags & F_ENABLESWING )
						DrawSwing(bounds, wArtHeight);
					delete [] lineStart;
					delete [] bData;
				}
				catch(...)
				{
				}
			}
			break;
		case 2:
			{
				// NPC Data
				try
				{
					BYTE * bData = NULL;
					DWORD dwOffset = 0;
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);
					WORD wPalette[256];
					memcpy(&wPalette[0], &bData[dwOffset], sizeof(WORD) * 256);
					dwOffset += sizeof(WORD) * 256;
					DWORD dwFrameCount;
					memcpy(&dwFrameCount, &bData[dwOffset], 4);
					dwOffset += 4;
					DWORD * frameOffsets = new DWORD [dwFrameCount];
					memcpy(&frameOffsets[0], &bData[dwOffset], sizeof(DWORD) * dwFrameCount);
					// Read the frame we want
					dwOffset = 256 * sizeof(WORD) + frameOffsets[0];
					short imageCenterX, imageCenterY, width, height;
					memcpy(&imageCenterX, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&imageCenterY, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&width, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&height, &bData[dwOffset], 2);
					dwOffset += 2;
					WORD previousLine = 0xFF;
					int Y = m_yOffset + (bounds.Height() / 2) - (height/2);
					while ( true )
					{
						short header, offset;
						memcpy(&header, &bData[dwOffset], 2);
						dwOffset += 2;
						memcpy(&offset, &bData[dwOffset], 2);
						dwOffset += 2;
						if ( ( header == 0x7FFF ) || ( offset == 0x7FFF) )
							break;
						WORD wRunLength = header & 0x0FFF;
						WORD wLineNum = (header >> 12) & 0x000f;
						//WORD wUnknown = offset & 0x03F;
						WORD wTmp = offset & 0x8000;
						offset = (wTmp) | (offset >> 6);
						int X = m_xOffset + (bounds.Width() / 2) - imageCenterX + offset;
						if ( ( previousLine != 0xFF ) && ( wLineNum != previousLine ) )
							Y++;
						previousLine = wLineNum;
						for ( int j = 0; j < wRunLength; j++ )
						{
							BYTE bIndex = 0;
							memcpy(&bIndex, &bData[dwOffset], 1);
							dwOffset++;
							DWORD dwColor = BlendColors(wPalette[bIndex], m_wAppliedColour, false);
							{
								BYTE r, g, b;
								b = (BYTE) ((dwColor >> 16) & 0xFF);
								g = (BYTE) ((dwColor >> 8) & 0xFF);
								r = (BYTE) ((dwColor) & 0xFF);
								if (m_wFrame > 0)
									X = m_xOffset + (bounds.Width() / 2 ) - offset - j;
								else
									X = m_xOffset + (bounds.Width() / 2 ) + offset + j;
								if ( X >= 0 && X < bounds.Width() && Y >=0 && Y < bounds.Height() )
								{
									m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3), b);
									m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 1, g);
									m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 2, r);
								}
							}
						}
					}
					delete [] frameOffsets;
					delete [] bData;
				}
				catch(...)
				{
				}
			}
			break;
		case 3:
			{
				// Light Data
				try
				{
					BYTE bLevel = Main->m_dlgLightWiz->m_cscLightLevel.GetPos();
					BYTE bLightLevel;
					BYTE r, g, b, rs, gs, bs;
					BYTE bColor;
					WORD wSize;
					BYTE * bData = NULL;
					DWORD dwOffset = 0;
					wSize = (WORD) (indexRec.dwUnknown & 0xFFFF);
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);
					for ( int y = 0; y < bounds.Height(); y++ )
					{
						for ( int x = 0; x < bounds.Width(); x++ )
						{
							bLightLevel = (bLevel * 100) / 0x1e;
							r = m_bPixels.GetAt(( ((bounds.Width() * y) + x) * 3) + 2);
							g = m_bPixels.GetAt(( ((bounds.Width() * y) + x) * 3) + 1);
							b = m_bPixels.GetAt( ((bounds.Width() * y) + x) * 3);
					
							int X, Y;
							X = m_xOffset + ((bounds.Width() - wSize)/2);
							Y = 30 + m_yOffset + ((bounds.Height() - wSize)/2);
							if ((x >= X) && (x < X + wSize) && (y >= Y) && (y < Y + wSize))
							{
								dwOffset = (x-X)+(wSize*(y-Y));
								memcpy(&bColor, &bData[dwOffset], 1);
								int iLocalLevel;
								if ((bColor > 0x00) && (bColor <= 0x1e))
								{
									iLocalLevel = bLevel - bColor;
									if (iLocalLevel <= 0)
										bLightLevel = 0;
									else
										bLightLevel = (iLocalLevel * 100) / 0x1e;
								}
								if ((bColor > 0xe1) && (bColor <= 0xff))
								{
									iLocalLevel = bLevel + (0xff - bColor);
									if (iLocalLevel >= 0x1e)
										bLightLevel = 100;
									else
										bLightLevel = (iLocalLevel * 100) / 0x1e;
								}
							}

							rs = r * bLightLevel / 100;
							gs = g * bLightLevel / 100;
							bs = b * bLightLevel / 100;
							m_bPixels.SetAt((( (bounds.Width() * y) + x) * 3), b - bs);
							m_bPixels.SetAt((( (bounds.Width() * y) + x) * 3) + 1, g - gs);
							m_bPixels.SetAt((( (bounds.Width() * y) + x) * 3) + 2, r - rs);
						}
					}
					delete [] bData;
				}
				catch(...)
				{
				}
			}
			break;
		case 4:
			{
				// Multi data
				WORD wItemCount = 0;
				CMultiRec ** items = NULL;
				
				fData.Seek(indexRec.dwLookup, CFile::begin);
				wItemCount = (WORD) (indexRec.dwSize / sizeof(CMultiRec));
				items = new CMultiRec * [wItemCount];
				for ( WORD i = 0; i < wItemCount; i++ )
				{
					CMultiRec * pItem = new CMultiRec;
					fData.Read(pItem, sizeof(CMultiRec));
					items[i] = pItem;
				}

				int x1, x2, y1, y2;
				x1 = x2 = y1 = y2 = 0;
				for (WORD i = 0; i < wItemCount; i++ )
				{
					if ( items[i]->x < x1 )
						x1 = items[i]->x;
					else if ( items[i]->x > x2 )
						x2 = items[i]->x;
					if ( items[i]->y < y1 )
						y1 = items[i]->y;
					else if ( items[i]->y > y2 )
						y2 = items[i]->y;
				}
				int X, Y;
				X = x2 - x1 + 1;
				Y = y2 - y1 + 1;
				CPtrList * grid = new CPtrList [X * Y];
				for ( int i = 0; i < wItemCount; i++ )
				{
					int x, y;
					// First, we need to get the item height from the tileinfo file.
					DWORD tileOffset = 0x68800;	// Skip over the land data
					tileOffset += ( items[i]->wIndex / 32 ) * 1188 + 4 + ( (items[i]->wIndex % 32 ) * 37 );
					BYTE bItemHeight = m_tiledata[tileOffset + 16];
					items[i]->dwFlags = bItemHeight;
					x = items[i]->x - x1;
					y = items[i]->y - y1;
					int index = (y * X) + x;
					if ( grid[index].IsEmpty() )
						grid[index].AddHead((void *)items[i]);
					else
					{
						// Where does this one go?  Compare the heights of the items....
						bool bInserted = false;
						POSITION pos = grid[index].GetHeadPosition();
						while ( pos )
						{
							POSITION oldPos = pos;
							CMultiRec * pTest = (CMultiRec *) grid[index].GetNext(pos);
							if (( items[i]->x > pTest->x ) || ( items[i]->y < pTest->y ))
								continue;
							else if ( ( items[i]->x == pTest->x ) && ( items[i]->y == pTest->y ) && (( items[i]->dwFlags + items[i]->z ) >= ( pTest->dwFlags + pTest->z) ))
								continue;
							else
							{
								// Insert it here
								grid[index].InsertBefore(oldPos, (void *)items[i]);
								bInserted = true;
								break;
							}
						}
						if ( !bInserted )
							grid[index].AddTail((void *)items[i]);
					}
				}
				// Now draw these guys
				for ( int y = y1; y <= y2; y++ )
				{
					for ( int x = x1; x <= x2; x++ )
					{
						X = x - x1;
						Y = y - y1;
						int index = ( (y - y1) * (x2 - x1 + 1) ) + (x - x1);
						if ( !grid[index].IsEmpty() )
						{
							POSITION pos = grid[index].GetHeadPosition();
							while ( pos )
							{
								CMultiRec * pItem = (CMultiRec * ) grid[index].GetNext(pos);
								if ( !pItem )
									continue;
								int DX = ( pItem->x - pItem->y ) * 22;
								int DY = ( pItem->x + pItem->y ) * 22 - (m_wArtHeight[pItem->wIndex + 0x4000] >> 1) - ( pItem->z * 4 );
 								DrawArt(ART_ITEM, bounds, (WORD) pItem->wIndex + 0x4000, DX, DY, 0);
							}
						}
					}
				}
				for ( int i = 0; i < wItemCount; i++ )
				{
					CMultiRec * pItem = items[i];
					delete pItem;
				}
				delete [] items;
				delete [] grid;
			}
			break;
		case 0xFFFF:
			{
				// Raw data
				if ( indexRec.dwSize <= 4 )
					break;
				BYTE * bData = new BYTE [indexRec.dwSize - 4];
				fData.Read(&bData[0], indexRec.dwSize - 4);
				short wArtWidth, wArtHeight;
				wArtWidth = wArtHeight = 44;
				DWORD dwOffset = 0;
				for ( int y = 0; y < 45; y++ )
				{
					int width = y < 23 ? (y * 2) : ((45 - y) * 2);
					for ( int x = (22 - width / 2); x < (22 + width / 2); x++ )
					{
						WORD wColor;
						memcpy(&wColor, &bData[dwOffset], 2);
						dwOffset += 2;
						int X, Y;
						X = m_xOffset + dx + x + ((bounds.Width() - wArtWidth) / 2 );
						Y = m_yOffset + dy + y + ((bounds.Height() / 2) - (wArtHeight / 2));
						if ( X >= 0 && X < bounds.Width() && Y >= 0 && Y < bounds.Height() )
						{
							DWORD dwColor = BlendColors(wColor, 0, false);
							BYTE r, g, b;
							b = (BYTE) ((dwColor >> 16) & 0xFF);
							g = (BYTE) ((dwColor >> 8) & 0xFF);
							r = (BYTE) ((dwColor) & 0xFF);
							m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3), b);
							m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 1, g);
							m_bPixels.SetAt((( (bounds.Width() * Y) + X) * 3) + 2, r);
						}
					}
				}
				delete [] bData;
			}
			break;
		}
	}
	fData.Close();
	fIndex.Close();
}

ArtAddress CUOArt::FindUOPEntry(DWORD dwIndex)
{
	CString csArtPattern = GetMulFileName(VERFILE_ART_UOP);
	csArtPattern =  csArtPattern.SpanExcluding(".");
	csArtPattern.MakeLower();
	CString csFile;
	csFile.Format("build/%s/%.8d.tga", csArtPattern, dwIndex);
	DWORD64 qwFileHash = HashFileName(csFile);
	csFile.Format("build/%s/%.8d.tga", csArtPattern, 0x4000);
	DWORD64 qwNullFileHash = HashFileName(csFile);
	ArtAddress* pArtNull = NULL;
	for ( int i = 0; i < m_aUopAddress.GetSize(); i++ )
	{
		ArtAddress* pArt = (ArtAddress *) m_aUopAddress.GetAt(i);
		if ( pArt )
		{
			if (qwFileHash == pArt->qwHash)
				return *pArt;
			if (qwNullFileHash == pArt->qwHash)
				pArtNull = pArt;
		}
	}
return *pArtNull;
}

void CUOArt::LoadTiledata()
{
	CFile cfTiledata;
	CString csFile = Main->GetMulPath(VERFILE_TILEDATA);
	if ( cfTiledata.Open(csFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		cfTiledata.Read(&m_tiledata, 0x191800);
		cfTiledata.Close();
	}
}

void CUOArt::LoadArtData()
{
	if ( m_bArtDataLoaded )
		return;
	CFile cfArtIdx, cfArtMul;
	CString csIdxFile, csMulFile;
	csIdxFile = Main->GetMulPath(VERFILE_ARTIDX);
	csMulFile = Main->GetMulPath(VERFILE_ART);
	DWORD * dwIdx = new DWORD [0x30000];
	if ( cfArtIdx.Open(csIdxFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		cfArtIdx.Read(&dwIdx[0], 0xC0000);
		cfArtIdx.Close();

		if ( cfArtMul.Open(csMulFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		{
			for ( int i = 0; i < 0x10000; i++ )
			{
				if ( dwIdx[i * 3] != 0xFFFFFFFF )
				{
					cfArtMul.Seek(dwIdx[i * 3], CFile::begin);
					BYTE bData[8];
					cfArtMul.Read(&bData[0], 8);
					DWORD dwFlag;
					memcpy(&dwFlag, &bData[0], 4);
					if ( dwFlag == 0 || dwFlag > 0xFFFF )
					{
						m_wArtHeight[i] = 44;
						m_wArtWidth[i] = 44;
					}
					else
					{
						memcpy(&m_wArtWidth[i], &bData[4], 2);
						memcpy(&m_wArtHeight[i], &bData[6], 2);
					}
				}
				else
				{
					m_wArtHeight[i] = 44;
					m_wArtWidth[i] = 44;
				}
			}
			cfArtMul.Close();
		}
	}
	else
	{
		csMulFile = Main->GetMulPath(VERFILE_ART_UOP);
		if ( cfArtMul.Open(csMulFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		{
			//Parse UOP art file for quick reference
			DWORD dwUOPHashLo, dwUOPHashHi, dwCompressedSize, dwHeaderLenght;
			DWORD dwFilesInBlock, dwTotalFiles;
			DWORD64 dwUOPPtr;
			cfArtMul.Seek((sizeof(DWORD)*3), CFile::begin);
			cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
			cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
			dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
			cfArtMul.Seek(sizeof(DWORD), CFile::current);
			cfArtMul.Read(&dwTotalFiles, sizeof(DWORD));
			cfArtMul.Seek(dwUOPPtr, CFile::begin);

			while (dwUOPPtr > 0)
			{
				cfArtMul.Read(&dwFilesInBlock, sizeof(DWORD));
				cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
				cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
				dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;


				while ((dwFilesInBlock > 0)&&(dwTotalFiles > 0))
				{
						dwTotalFiles--;
						dwFilesInBlock--;
						ArtAddress * pArtAddress = new (ArtAddress);

						cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
						cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
						cfArtMul.Read(&dwHeaderLenght, sizeof(DWORD));
						pArtAddress->qwAdress = (((__int64)dwUOPHashHi << 32) + dwUOPHashLo) + dwHeaderLenght;

						cfArtMul.Read(&dwCompressedSize, sizeof(DWORD));
						pArtAddress->dwCompressedSize = dwCompressedSize;

						cfArtMul.Seek(sizeof(DWORD), CFile::current);
						cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
						cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
						pArtAddress->qwHash = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
						
						m_aUopAddress.Add(pArtAddress);
						
						cfArtMul.Seek(sizeof(DWORD)+sizeof(WORD), CFile::current);
				}

				cfArtMul.Seek(dwUOPPtr, CFile::begin);
			}


		}
	}
	delete [] dwIdx;
	m_bArtDataLoaded = true;
}

void CUOArt::DrawRoomview(CRect bounds, WORD wArtHeight)
{
	int iWidth = bounds.Width();
	int iHeight = bounds.Height();
	int iXCenter = (iWidth / 2) + m_xOffset;
	int iYCenter = (iHeight / 2) + (wArtHeight / 2) + m_yOffset;
	int x, y;
	// Draw some lines...
	for ( x = 0; x <= iXCenter; x++ )
	{
		y = ( iWidth * x )/( 2 * iXCenter ) + iYCenter - (iWidth / 2);
		int iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
		y = ( -iWidth * x )/( 2 * iXCenter ) + iYCenter + (iWidth / 2) - 44;
		iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
	}
	for ( x = iXCenter; x < iWidth; x++)
	{
		y = ( iWidth * x )/( 2 * ( iXCenter - iWidth )) + iYCenter + (iWidth / 2);
		int iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
		y = ( -iWidth * x )/( 2 * ( iXCenter - iWidth )) + iYCenter - (iWidth / 2) - 44;
		iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
	}
	x = iXCenter - 22;
	for ( y = 0; y < (iYCenter - 22); y++ )
	{
		int iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
	}
	x = iXCenter + 22;
	for ( y = 0; y < (iYCenter - 22); y++ )
	{
		int iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
	}
	x = iXCenter;
	for ( y = 0; y < (iYCenter - 44); y++ )
	{
		int iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
	}
	y = 0;
	for ( x = (iXCenter - 22); x <= (iXCenter + 22); x++ )
	{
		int iIndex = (iWidth * y) + x;
		if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
		{
			m_bPixels.SetAt(( iIndex * 3), 0);
			m_bPixels.SetAt(( iIndex * 3) + 1, 0);
			m_bPixels.SetAt(( iIndex * 3) + 2, 0);
		}
	}
	// Now fill in one region with color eeedea
	for ( x = ( iXCenter - 22 ); x <= ( iXCenter + 22 ); x++ )
	{
		// This one is symmetric
		for ( int Y = 0; Y < iHeight / 2; Y++ )
		{
			y = iYCenter - 22 - Y;
			int iIndex = (iWidth * y) + x;
			if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
			{
				if ( m_bPixels.GetAt(iIndex * 3) == 0 )
					break;
				m_bPixels.SetAt(( iIndex * 3), 0xea);
				m_bPixels.SetAt(( iIndex * 3) + 1, 0xed);
				m_bPixels.SetAt(( iIndex * 3) + 2, 0xee);
			}
			y = iYCenter - 22 + Y;
			iIndex = (iWidth * y) + x;
			if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
			{
				if ( m_bPixels.GetAt(iIndex * 3) == 0 )
					break;
				m_bPixels.SetAt(( iIndex * 3), 0xea);
				m_bPixels.SetAt(( iIndex * 3) + 1, 0xed);
				m_bPixels.SetAt(( iIndex * 3) + 2, 0xee);
			}
		}
	}
	// Now fill in a region with color 6f6d65
	for ( x = ( iXCenter - 22 ); x < iXCenter; x++ )
	{
		for ( y = 1; y < iYCenter; y++ )
		{
			int iIndex = (iWidth * y) + x;
			if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
			{
				if ( m_bPixels.GetAt(iIndex * 3) == 0 )
					break;
				m_bPixels.SetAt(( iIndex * 3), 0x65);
				m_bPixels.SetAt(( iIndex * 3) + 1, 0x6d);
				m_bPixels.SetAt(( iIndex * 3) + 2, 0x6f);
			}
		}
	}
	// Now fill in a region with color aaaaaa
	for ( x = ( iXCenter + 1); x < (iXCenter + 22); x++ )
	{
		for ( y = 1; y < iYCenter; y++ )
		{
			int iIndex = (iWidth * y) + x;
			if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
			{
				if ( m_bPixels.GetAt(iIndex * 3) == 0 )
					break;
				m_bPixels.SetAt(( iIndex * 3), 0xaa);
				m_bPixels.SetAt(( iIndex * 3) + 1, 0xaa);
				m_bPixels.SetAt(( iIndex * 3) + 2, 0xaa);
			}
		}
	}
}


void CUOArt::DrawSwing(CRect bounds, WORD wArtHeight, int idirection)
{
	UNREFERENCED_PARAMETER(idirection);
	int OffX = m_xOffset;
	int OffY = m_yOffset;
	if ( m_wDrawFlags & F_SWINGNORTH )
	{
		if	( m_wDrawFlags & F_SWINGNE )
		{
			OffX += 22;
			OffY += 22;
		}
		if	( m_wDrawFlags & F_SWINGSW )
		{
			OffX -= 22;
			OffY += 22;
		}
		if	( m_wDrawFlags & F_SWINGSE )
			OffY += 44;
	}
	if ( m_wDrawFlags & F_SWINGWEST )
	{
		if	( m_wDrawFlags & F_SWINGNW )
		{
			OffX -= 22;
			OffY += 22;
		}
		if	( m_wDrawFlags & F_SWINGSW )
			OffY += 44;
		if	( m_wDrawFlags & F_SWINGSE )
		{
			OffX += 22;
			OffY += 22;
		}
	}

	int iWidth = bounds.Width();
	int iHeight = bounds.Height();
	int iXCenter = (iWidth / 2) + OffX;
	int iYCenter = (iHeight / 2) + (wArtHeight / 2) - 22 + OffY;
	int x, y = 0;

	if ( m_wDrawFlags & F_SWINGNORTH )
	{
		if	( m_wDrawFlags & F_SWINGNW )
		{
			for ( y = iYCenter - 22 - (wArtHeight - 24); y < 22 + iYCenter - (wArtHeight - 24); y++)
			{
				x = iXCenter - 22 + (int)sqrt((double) (968 - (int)pow((iYCenter - (wArtHeight - 24)) - y,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
		if	( m_wDrawFlags & F_SWINGNE )
		{
			for ( x = iXCenter - 44; x < iXCenter; x++)
			{
				y = iYCenter - (wArtHeight - 24) - (int)sqrt((double) (968 - (int)pow(iXCenter - x - 22,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
		if	( m_wDrawFlags & F_SWINGSW )
		{
			for ( x = iXCenter - 22; x < (22 + iXCenter); x++)
			{
				y = iYCenter - 22 - (wArtHeight - 24) + (int)sqrt((double) (968 - (int)pow(iXCenter - x,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
		if	( m_wDrawFlags & F_SWINGSE )
		{
			for ( y = iYCenter - 44 - (wArtHeight - 24); y < iYCenter - (wArtHeight - 24); y++)
			{
				x = iXCenter - (int)sqrt((double) (968 - (int)pow((iYCenter - (wArtHeight - 24) - 22) - y,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
	}

	if ( m_wDrawFlags & F_SWINGWEST )
	{
		if	( m_wDrawFlags & F_SWINGNW )
		{
			for ( x = iXCenter; x < iXCenter + 44; x++)
			{
				y = iYCenter - (wArtHeight - 24) - (int)sqrt((double) (968 - (int)pow(iXCenter - x + 22,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
		if	( m_wDrawFlags & F_SWINGNE )
		{
			for ( y = iYCenter - 22 - (wArtHeight - 24); y < 22 + iYCenter - (wArtHeight - 24); y++)
			{
				x = iXCenter + 22 - (int)sqrt((double) (968 - (int)pow((iYCenter - (wArtHeight - 24)) - y,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
		if	( m_wDrawFlags & F_SWINGSW )
		{
			for ( y = iYCenter - 44 - (wArtHeight - 24); y < iYCenter - (wArtHeight - 24); y++)
			{
				x = iXCenter + (int)sqrt((double) (968 - (int)pow((iYCenter - (wArtHeight - 24) - 22) - y,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
		if	( m_wDrawFlags & F_SWINGSE )
		{
			for ( x = iXCenter - 22; x < (22 + iXCenter); x++)
			{
				y = iYCenter - 22 - (wArtHeight - 24) + (int)sqrt((double) (968 - (int)pow(iXCenter - x,(double)2)));
				int iIndex = (iWidth * y) + x;
				if ( iIndex >= 0 && (iIndex * 3) < m_bPixels.GetUpperBound() )
				{
					m_bPixels.SetAt(( iIndex * 3), 0xff);
					m_bPixels.SetAt(( iIndex * 3) + 1, 0);
					m_bPixels.SetAt(( iIndex * 3) + 2, 0);
				}
			}
		}
	}
}

//*************
//Hues

DWORD CUOArt::ScaleColor(WORD wColor)
{
	DWORD dwNewColor;
	
	dwNewColor = ((((((wColor >> 10) & 0x01f) * 0x0ff / 0x01f))
		| (((((wColor >> 5) & 0x01f) * 0x0ff / 0x01f)) << 8)
		| ((((wColor & 0x01f) * 0x0ff / 0x01f)) << 16)));
	return (dwNewColor);
}

DWORD CUOArt::BlendColors(WORD wBaseColor, WORD wAppliedColor, bool bBlendMode)
{
	if ( wAppliedColor == 0 || wAppliedColor > ( Main->m_aHueGroups.GetUpperBound() * 8 ))
		return ScaleColor(wBaseColor);
	wAppliedColor--;
	DWORD dwBase = ScaleColor(wBaseColor);

	WORD r, g, b;
	r = (WORD) (( dwBase & 0x00FF0000 ) >> 16);
	g = (WORD) (( dwBase & 0x0000FF00 ) >> 8);
	b = (WORD) ( dwBase & 0x000000FF );
	if ( bBlendMode && r != g && r != b)
		return dwBase;
	WORD wOutput = ( wBaseColor >> 10 );
	wOutput += ( wBaseColor >> 5 ) & 0x1f;
	wOutput += ( wBaseColor ) & 0x1f;
	wOutput /= 3;
	//WORD gray = ( wOutput << 10 ) | ( wOutput << 5 ) | wOutput;
	int groupIndex = wAppliedColor / 8;
	int hueIndex = wAppliedColor % 8;
	CHueGroup * pGroup = (CHueGroup *) Main->m_aHueGroups.GetAt(groupIndex);
	if ( pGroup == NULL )
		return dwBase;
	CHueEntry Hue = (CHueEntry) pGroup->Hues[hueIndex];
	return ScaleColor(Hue.wColorTable[wOutput]);
}

/////////////////////////////////////////////////////////////////////////////
// CDefArray

int CDefArray::Find(WORD wFind)
{
	if ( this->GetSize() == 0 )
		return -1;
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CDefObj * pDef = (CDefObj *) this->GetAt(iIndex);
		WORD wExisting = pDef->wValue;
		if ( wExisting == wFind )
			return (int)iIndex;
		if ( wExisting > wFind )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return -1;
}

int CDefArray::Insert(CDefObj * pDef)
{
	if ( pDef->wValue == 0 )
	{
		delete pDef;
		return -1;
	}
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	if ( iUpper == -1 )
	{
		this->InsertAt(0, pDef);
		return 0;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CDefObj * pTest = (CDefObj *) this->GetAt(iIndex);
		if ( pTest->wValue > pDef->wValue)
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
	this->InsertAt(iIndex, (void *) pDef);
	return (int)iIndex;
}

/////////////////////////////////////////////////////////////////////////////
// CUOArt properties


short CUOArt::GetArtType()
{
	return m_sArtType;
}

void CUOArt::SetArtType(short propVal)
{
	if ( propVal != m_sArtType )
	{
		m_sArtType = propVal;
		SetInvalid();
	}
}

long CUOArt::GetArtIndex()
{
	return m_dwArtIndex;
}

void CUOArt::SetArtIndex(long propVal)
{
	if ( (DWORD) propVal != m_dwArtIndex )
	{
		m_dwArtIndex = (DWORD) propVal;
		SetInvalid();
	}
}

void CUOArt::ReloadArt()
{
	SetInvalid();
}

short CUOArt::GetArtColor()
{
	return m_wAppliedColour;
}

void CUOArt::SetArtColor(short propVal)
{
	if ( ((WORD) propVal) != m_wAppliedColour )
	{
		m_wAppliedColour = (WORD) propVal;
		SetInvalid();
	}
}

long CUOArt::GetXOffset()
{
	return m_xOffset;
}

void CUOArt::SetXOffset(long propVal)
{
	m_xOffset = propVal;
	SetInvalid();
}

long CUOArt::GetYOffset()
{
	return m_yOffset;
}

void CUOArt::SetYOffset(long propVal)
{
	m_yOffset = propVal;
	SetInvalid();
}

short CUOArt::GetDrawFlags()
{
	return m_wDrawFlags;
}

void CUOArt::SetDrawFlags(short propVal)
{
	m_wDrawFlags = (WORD) propVal;
	SetInvalid();
}

void CUOArt::SetObject(short type, long index)
{
	m_dwArtIndex = index;
	m_sArtType = type;
	SetInvalid();
}


void CUOArt::SetOffset(long x, long y)
{
	m_xOffset = x;
	m_yOffset = y;
	SetInvalid();
}

long CUOArt::yScroll(long offset)
{
	m_yOffset += offset;
	SetInvalid();
	return m_yOffset;
}

long CUOArt::xScroll(long offset)
{
	m_xOffset += offset;
	SetInvalid();
	return m_xOffset;
}

void CUOArt::Scroll(long x, long y)
{
	m_xOffset += x;
	m_yOffset += y;
	SetInvalid();
}

long CUOArt::GetBkColor()
{
	return (long) m_dwBkColor;
}

void CUOArt::SetBkColor(long color)
{
	m_dwBkColor = (DWORD) color;
	SetInvalid();
}

CBitmap * CUOArt::OnCreateIcon(int iBitWidth, int iBitHeight, int iID, WORD wAppliedColor, int iType)
{
	CBitmapDC m_cIcon(iBitWidth, iBitHeight, RGB(255,255,255));

	DWORD dwIndex;
	switch( iType )
		{
		case ART_MULTI:
			dwIndex = iID;
			return m_cIcon.Close();
			break;
		case ART_ITEM:
			dwIndex = iID + 0x4000;
			if ( iID >= 0x8000 )
				return m_cIcon.Close();
			break;
		case ART_LANDSCAPE:
			dwIndex = iID;
			if ( iID >= 0x4000 )
				return m_cIcon.Close();
			break;
		case ART_NPC:
		case ART_NPC2:
		case ART_NPC3:
		case ART_NPC4:
		case ART_NPC5:
			{
				if ( iID >= 0x1000 )
					return m_cIcon.Close();
				DWORD dwArtIndex = iID;
				// See if we find this guy in body.bef
				int i = Main->m_daBodydef.Find((WORD) iID);
				if ( i != -1 )
				{
					CDefObj * pDef = (CDefObj *) Main->m_daBodydef.GetAt(i);
					dwArtIndex = pDef->wID;
				}
				int iMul = 1;
				// Check to see which anim*.mul
				int j = Main->m_daBodyConv.Find((WORD) iID);
				if ( j != -1 )
				{
					CDefObj * pMulDef = (CDefObj *) Main->m_daBodyConv.GetAt(j);
					dwArtIndex = pMulDef->wID;
					iMul = pMulDef->iMul;
				}

				switch ( iMul )
				{
				case 1:
					{
						iType = ART_NPC;
						if ( dwArtIndex < 0xC8 )
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x190 )
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else
							dwIndex = ( dwArtIndex - 0x190) * 175 + 35000;
					}
					break;
				case 2:
					{
						// Anim2
						iType = ART_NPC2;
						if ( dwArtIndex == 0x44 ) //Kirrin Fix
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else if ( dwArtIndex < 0xC8 )
							dwIndex = dwArtIndex * 110;
						else
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
					}
					break;
				case 3:
					{
						// Anim3
						iType = ART_NPC3;
						if ( dwArtIndex < 0x2BC )
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x578 )
							dwIndex = ( dwArtIndex - 0x2BC ) * 65 + 77000;
						else
							dwIndex = ( dwArtIndex - 0x578 ) * 175 + 122500;
					}
					break;
				case 4:
					{
						// Anim4
						iType = ART_NPC4;
						if ( dwArtIndex < 0xC8 )
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x190 )
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else
							dwIndex = ( dwArtIndex - 0x190) * 175 + 35000;
					}
					break;
				case 5:
					{
						// Anim5
						iType = ART_NPC5;
						if ( dwArtIndex == 0x22 ) //Chimera Fix
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else if ( dwArtIndex < 0xC8 )
							dwIndex = dwArtIndex * 110;
						else if ( dwArtIndex < 0x190 )
							dwIndex = ( dwArtIndex - 0xC8 ) * 65 + 22000;
						else
							dwIndex = ( dwArtIndex - 0x190) * 175 + 35000;
					}
					break;
				}
				dwIndex += 1;
			}
			break;
		}

	CFile fData, fIndex;
	CString csDataFile, csIndexFile;

	DWORD dwType;
	switch ( iType )
	{
	case ART_MULTI:
		return 0;
	case ART_LANDSCAPE:
		csDataFile = Main->GetMulPath(VERFILE_ART);
		csIndexFile = Main->GetMulPath(VERFILE_ARTIDX);
		dwType = 0;
		break;
	case ART_ITEM:
	//case ART_LANDSCAPE:
		csDataFile = Main->GetMulPath(VERFILE_ART);
		csIndexFile = Main->GetMulPath(VERFILE_ARTIDX);
		dwType = 1;
		break;
	case ART_NPC:
		csDataFile = Main->GetMulPath(VERFILE_ANIM);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX);
		dwType = 2;
		break;
	case ART_NPC2:
		csDataFile = Main->GetMulPath(VERFILE_ANIM2);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX2);
		dwType = 2;
		break;
	case ART_NPC3:
		csDataFile = Main->GetMulPath(VERFILE_ANIM3);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX3);
		dwType = 2;
		break;
	case ART_NPC4:
		csDataFile = Main->GetMulPath(VERFILE_ANIM4);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX4);
		dwType = 2;
		break;
	case ART_NPC5:
		csDataFile = Main->GetMulPath(VERFILE_ANIM5);
		csIndexFile = Main->GetMulPath(VERFILE_ANIMIDX5);
		dwType = 2;
		break;
	}

	if ( !fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		return m_cIcon.Close();

	if ( !fIndex.Open(csIndexFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		fData.Close();
		return m_cIcon.Close();
	}
	fIndex.Seek(dwIndex * sizeof(ArtIdx), CFile::begin);
	ArtIdx indexRec;
	fIndex.Read(&indexRec, sizeof(ArtIdx));

	if (( indexRec.dwLookup != 0xFFFFFFFF )&&( indexRec.dwSize > 0 ))
	{
		switch (dwType)
		{
		case 0:
			{
				try
				{
					BYTE * bData = NULL;
					DWORD dwOffset = 4;
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);

					//Raw data
					short wArtWidth, wArtHeight;
					wArtWidth = wArtHeight = 44;
					for ( int y = 0; y < 45; y++ )
					{
						int width = y < 23 ? (y * 2) : ((45 - y) * 2);
						for ( int x = (22 - width / 2); x < (22 + width / 2); x++ )
						{
							WORD wColor;
							memcpy(&wColor, &bData[dwOffset], 2);
							dwOffset += 2;
							int X, Y;
							X = x + ((iBitWidth - wArtWidth) / 2 );
							Y = y + ((iBitHeight / 2) - (wArtHeight / 2));
							if ( X >= 0 && X < iBitWidth && Y >= 0 && Y < iBitHeight )
							{
								DWORD dwColor = BlendColors(wColor, wAppliedColor, false);
								BYTE r, g, b;
								b = (BYTE) ((dwColor >> 16) & 0xFF);
								g = (BYTE) ((dwColor >> 8) & 0xFF);
								r = (BYTE) ((dwColor) & 0xFF);
								m_cIcon.SetPixelV(X,Y,RGB(r,g,b));
							}
						}
					}
				}
				catch(...)
				{
				}
			}
			break;
		case 1:
			{
				try
				{
					BYTE * bData = NULL;
					DWORD dwOffset = 4;
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);
			
					// Regular item data
					WORD wArtWidth, wArtHeight;
					memcpy(&wArtWidth, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&wArtHeight, &bData[dwOffset], 2);
					dwOffset += 2;
					if ( wArtWidth == 0 || wArtWidth > 1024 || wArtHeight == 0 || wArtHeight > 1024 )
						throw 1;
					WORD * lineStart = new WORD [wArtHeight];
					for ( int i = 0; i < wArtHeight; i++ )
					{
						memcpy(&lineStart[i], &bData[dwOffset], 2);
						dwOffset += 2;
					}
					DWORD dataStart = dwOffset;
					int x, y;
					x = y = 0;
					while ( y < wArtHeight )
					{
						WORD xOffset, xRun;
						memcpy(&xOffset, &bData[dwOffset], 2);
						dwOffset += 2;
						memcpy(&xRun, &bData[dwOffset], 2);
						dwOffset += 2;
						if ( (xRun + xOffset) > 2048 )
							break;
						else
						{
							if ( ( xRun + xOffset ) != 0 )
							{
								x += xOffset;
								WORD * wColor = new WORD [xRun];
								memcpy(&wColor[0], &bData[dwOffset], 2 * xRun);
								dwOffset += 2 * xRun;
								for (int iCount = 0; iCount < xRun; iCount++)
								{
									int X, Y;
									X = (x + iCount) + ((iBitWidth - wArtWidth) / 2 );
									Y = y + ((iBitHeight - wArtHeight) / 2);
									if ( X >= 0 && X < iBitWidth && Y >= 0 && Y < iBitHeight )
									{
										DWORD dwColor = BlendColors(wColor[iCount], wAppliedColor, false);
										BYTE r, g, b;
										b = (BYTE) ((dwColor >> 16) & 0xFF);
										g = (BYTE) ((dwColor >> 8) & 0xFF);
										r = (BYTE) ((dwColor) & 0xFF);
										m_cIcon.SetPixelV(X,Y,RGB(r,g,b));
									}
								}
								delete [] wColor;
								x += xRun;
							}
							else
							{
								x = 0;
								y++;
								dwOffset = lineStart[y] * 2 + dataStart;
							}
						}
					}
					delete [] lineStart;
					delete [] bData;
				}
				catch(...)
				{
				}
			}
			break;
		case 2:
			{
				// NPC Data
				try
				{
					BYTE * bData = NULL;
					DWORD dwOffset = 0;
					fData.Seek(indexRec.dwLookup, CFile::begin);
					bData = new BYTE [indexRec.dwSize];
					fData.Read(&bData[0], indexRec.dwSize);
					WORD wPalette[256];
					memcpy(&wPalette[0], &bData[dwOffset], sizeof(WORD) * 256);
					dwOffset += sizeof(WORD) * 256;
					DWORD dwFrameCount;
					memcpy(&dwFrameCount, &bData[dwOffset], 4);
					dwOffset += 4;
					DWORD * frameOffsets = new DWORD [dwFrameCount];
					memcpy(&frameOffsets[0], &bData[dwOffset], sizeof(DWORD) * dwFrameCount);
					// Read the frame we want
					dwOffset = 256 * sizeof(WORD) + frameOffsets[0];
					short imageCenterX, imageCenterY, width, height;
					memcpy(&imageCenterX, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&imageCenterY, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&width, &bData[dwOffset], 2);
					dwOffset += 2;
					memcpy(&height, &bData[dwOffset], 2);
					dwOffset += 2;
					WORD previousLine = 0xFF;
					int Y = m_yOffset + (iBitHeight / 2) - (height/2);
					while ( true )
					{
						short header, offset;
						memcpy(&header, &bData[dwOffset], 2);
						dwOffset += 2;
						memcpy(&offset, &bData[dwOffset], 2);
						dwOffset += 2;
						if ( ( header == 0x7FFF ) || ( offset == 0x7FFF) )
							break;
						WORD wRunLength = header & 0x0FFF;
						WORD wLineNum = (header >> 12) & 0x000f;
						//WORD wUnknown = offset & 0x03F;
						WORD wTmp = offset & 0x8000;
						offset = (wTmp) | (offset >> 6);
						int X = (iBitWidth / 2) - imageCenterX + offset;
						if ( ( previousLine != 0xFF ) && ( wLineNum != previousLine ) )
							Y++;
						previousLine = wLineNum;
						for ( int j = 0; j < wRunLength; j++ )
						{
							BYTE bIndex = 0;
							memcpy(&bIndex, &bData[dwOffset], 1);
							dwOffset++;
							X = (iBitWidth / 2 ) + offset + j;
							if ( X >= 0 && X < iBitWidth && Y >=0 && Y < iBitHeight )
							{
								DWORD dwColor = BlendColors(wPalette[bIndex], wAppliedColor, false);
								BYTE r, g, b;
								b = (BYTE) ((dwColor >> 16) & 0xFF);
								g = (BYTE) ((dwColor >> 8) & 0xFF);
								r = (BYTE) ((dwColor) & 0xFF);
								m_cIcon.SetPixelV(X,Y,RGB(r,g,b));
							}
						}
					}
					delete [] frameOffsets;
					delete [] bData;
				}
				catch(...)
				{
				}
			}
			break;
		}
	}
	fData.Close();
	fIndex.Close();

	return m_cIcon.Close();
}

CBitmapDC::	CBitmapDC(int width, int height, COLORREF background):
  CDC(),
    m_pBitmap(NULL),
    m_pOldBmp(NULL)
{
  CWindowDC dc(NULL);
  m_pBitmap = new CBitmap();
  VERIFY(m_pBitmap->CreateCompatibleBitmap(&dc, width, height)); 

  VERIFY(CreateCompatibleDC(&dc));
  m_pOldBmp = SelectObject(m_pBitmap);
  CDC::SetMapMode(dc.GetMapMode());

  FillSolidRect(0, 0, width, height, background);
}

CBitmapDC::~CBitmapDC()
{
  if (m_pBitmap) {
    CBitmap *pBitmap;
    pBitmap = Close();
    delete pBitmap;
    pBitmap = NULL;
  }
}

CBitmap *CBitmapDC::Close()
{
  ASSERT(m_pBitmap);
  CBitmap *pBitmap = m_pBitmap;
  SelectObject(m_pOldBmp);
  m_pBitmap = NULL;
  return pBitmap;
}