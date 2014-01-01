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
#include "UOmap.h"
#include <comcat.h>
#include <objsafe.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CUOMap, CWnd)
	//{{AFX_MSG_MAP(CUOMap)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUOMap::CUOMap - Constructor

CUOMap::CUOMap()
{

	m_xCenter = UO_X_CENTER;
	m_yCenter = UO_Y_CENTER;
	m_zoomLevel = 0;
	m_dwColorMap = new DWORD[65536];
	m_wDrawFlags = 0;
	m_bIsValid = false;
	iLoaded = false;
	bIsUOP = false;
	iMapPatch = false;
	iStaticPatch = false;

	LoadRadarcol();

	m_mulfile = 0;
	m_maxX = 6144;
	m_maxY = 4096;
}

/////////////////////////////////////////////////////////////////////////////
// CUOMap::~CUOMap - Destructor

CUOMap::~CUOMap()
{
	for ( int i = 0; i < m_aDrawObjects.GetSize(); i++ )
	{
		CDrawObject * pObject = (CDrawObject *) m_aDrawObjects.GetAt(i);
		if ( pObject )
			delete pObject;
	}
	m_aDrawObjects.RemoveAll();
	for (int i = 0; i < m_aDrawRects.GetSize(); i++ )
	{
		CDrawRect * pRect =(CDrawRect *) m_aDrawRects.GetAt(i);
		if ( pRect )
			delete pRect;
	}
	m_aDrawRects.RemoveAll();
	delete [] m_dwColorMap;

	if (iLoaded)
		CloseMap();

	CleanHashArray(m_aMapHash);
	CleanHashArray(m_aStaticHash);

}

void CUOMap::UpdateMap()
{
	SetInvalid();
}

void CUOMap::SetInvalid()
{  
	m_bIsValid = false;
	Invalidate();
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CUOMap::OnPaint - Drawing function

void CUOMap::OnPaint()
{
		CPaintDC pdc(this); // device context for painting
		CRect rcBounds;
		GetClientRect(&rcBounds);
	if ( rcBounds.Width() != m_rcSize.Width() || rcBounds.Height() != m_rcSize.Height() )
	{
		m_rcSize = rcBounds;
	}
	RedrawMap(&pdc, rcBounds);
}

void CUOMap::RedrawMap(CPaintDC* pdc, const CRect& rcBounds)
{
	if (!iLoaded)
		return;
	try
	{
		int RectWidth = rcBounds.Width() - (rcBounds.Width() % 8);
		int RectHeight = rcBounds.Height() - (rcBounds.Height() % 8);
		if ( !m_bIsValid )
		{
			bool bDrawStatics = (m_wDrawFlags & F_DRAWSTATICS) && (m_zoomLevel >= -2);
			bool bDrawDifs = (m_wDrawFlags & F_DRAWDIFS ? true : false);
			int xc = m_xCenter / 8;
			int yc = m_yCenter / 8;

			m_bMapArray.RemoveAll();
			m_bMapArray.SetSize(RectWidth * RectHeight * 3);

			DWORD m_dwBkColor = RGB(249,249,251);
			BYTE br, bg, bb;
			bb = (BYTE) ((m_dwBkColor >> 16) & 0xFF);
			bg = (BYTE) ((m_dwBkColor >> 8) & 0xFF);
			br = (BYTE) ((m_dwBkColor) & 0xFF);
			for ( int i = 0; i < ( RectWidth * RectHeight ); i++ )
			{
				m_bMapArray.SetAt(i * 3, bb);
				m_bMapArray.SetAt(i * 3 + 1, bg);
				m_bMapArray.SetAt(i * 3 + 2, br);
			}

			if ( m_mulfile < 0 || m_mulfile > 5 )
				return;

			int scalefactor;
			if ( m_zoomLevel == -4 )
				scalefactor = 2;	// We can skip alternating blocks
			else
				scalefactor = 1;	// We need to read each block in the range

			// The number of blocks we actually have to read depends on the zoom level.

			int X1, X2, Y1, Y2, wX1, wX2, wY1, wY2;
			// Determine which blocks are at the bounds of the client rectangle
			int iBlockWidth = 0;
			int iBlockHeight = 0;
			int iCellsPerBlock = 8;
			int iPixelsPerCell = 1;
			if ( m_zoomLevel > 0 )
				iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
			if ( m_zoomLevel < 0 )
				iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
			if ( iCellsPerBlock < 1 )
			{
				iCellsPerBlock = 1;
				scalefactor = 2;
			}
			iBlockWidth = (int)(RectWidth * (scalefactor * 1.0/ ( iCellsPerBlock * iPixelsPerCell )));
			iBlockHeight = (int)(RectHeight * (scalefactor * 1.0 / ( iCellsPerBlock * iPixelsPerCell )));

			// Upper left
			wX1 = X1 = xc - iBlockWidth / 2;
			wY1 = Y1 = yc - iBlockHeight / 2;
			// Lower right
			wX2 = X2 = xc + iBlockWidth / 2;
			wY2 = Y2 = yc + iBlockHeight/ 2;

			if (( iBlockWidth ) && ( iBlockWidth < UO_X_BLOCKS ))
			{
				if ( X1 < 0 )
				{
					wX2 = X2 -= X1;
					wX1 = X1 = 0;
				}
				if ( X2 > UO_X_BLOCKS )
				{
					wX1 = X1 -= X2 - UO_X_BLOCKS;
					wX2 = X2 = UO_X_BLOCKS;
				}
			}
			else if ( iBlockWidth )
			{
				int iPadding = ( iBlockWidth - UO_X_BLOCKS )/2;
				wX1 = 0 - iPadding;
				wX2 = UO_X_BLOCKS + iPadding;
				X1 = 0;
				X2 = UO_X_BLOCKS;
			}
			if (( iBlockHeight ) && ( iBlockHeight < UO_Y_BLOCKS ))
			{
				if ( Y1 < 0 )
				{
					wY2 = Y2 -= Y1;
					wY1 = Y1 = 0;
				}
				if ( Y2 > UO_Y_BLOCKS )
				{
					wY1 = Y1 -= Y2 - UO_Y_BLOCKS;
					wY2 = Y2 = UO_Y_BLOCKS;
				}
			}
			else if ( iBlockHeight )
			{
				int iPadding = ( iBlockHeight - UO_Y_BLOCKS )/2;
				wY1 = 0 - iPadding;
				wY2 = UO_Y_BLOCKS + iPadding;
				Y1 = 0;
				Y2 = UO_Y_BLOCKS;
			}

			if ( X2 == X1 )
				X2++;
			if ( Y2 == Y1 )
				Y2++;

			MapBlock *mbArray;
			mbArray = new MapBlock[Y2 - Y1];

			for (int i = X1; i < X2; i+=scalefactor)
			{
				memset(mbArray, 0x00, sizeof(MapBlock) * (Y2 - Y1));
				// Seek to the next block that we need to read
				int iNextBlock = (i * UO_Y_BLOCKS) + Y1;
				if ( (iNextBlock >= 0) && (iNextBlock < (UO_X_BLOCKS * UO_Y_BLOCKS) ) )
				{
					if (bIsUOP)
					{
						for ( int i = 0; i < (Y2 - Y1); i++ )
						{
							mbArray[i] = LoadUopBlock(iNextBlock);
							iNextBlock++;
						}
					}
					else
					{
						// Read the blocks from the default file
						PBYTE pbMapFilePtr = pbMapFile + (sizeof(MapBlock) * iNextBlock);
						memcpy(&mbArray[0], pbMapFilePtr, sizeof(MapBlock) * (Y2 - Y1));
						// Check to see if any of the blocks in this column are patched
						if ( bDrawDifs )
						{
							for ( int p = Y1; p < Y2; p++ )
							{
								int o = p - Y1;
								// Is this a patched block?
								DWORD dwPatchOffset = FindPatch(*paMapHash, iNextBlock + o);
								if ( dwPatchOffset != 0xFFFFFFFF )
								{
									PBYTE pbMapDifPtr = pbMapDifFile + (sizeof(MapBlock) * (dwPatchOffset));
									memcpy(&mbArray[o], pbMapDifPtr, sizeof(MapBlock));
								}
							}
						}
					}
					for (int j = Y1; j < Y2; j+=scalefactor)
					{
						// Read the statics for this block, if applicable
						StaticData statCells[64];
						memset(&statCells[0], 0x00, (sizeof(StaticData) * 64));
						if ( bDrawStatics )
						{
							PBYTE pbIdx = pbStaIdxFile + (((i * UO_Y_BLOCKS) + j) * sizeof(StaticIdx));
							
							StaticIdx index;
							memcpy(&index, pbIdx, sizeof(StaticIdx));
							// Is this block patched?
							bool bPatched = false;
							if ( bDrawDifs )
							{
								DWORD dwPatchOffset = FindPatch(*paStaticHash, (i * UO_Y_BLOCKS) + j);
								if ( dwPatchOffset != 0xFFFFFFFF )
								{
									PBYTE pbDIFIdx = pbStaDifiFile + (dwPatchOffset * sizeof(StaticIdx));
									memcpy(&index, pbDIFIdx, sizeof(StaticIdx));
									bPatched = true;
								}
							}
							if ( index.dwStart != 0xFFFFFFFF )
							{
								// there are statics here
								int numStatics = index.dwLength / sizeof(StaticData);
								PBYTE pbStatics;
								if ( bPatched )
									pbStatics = pbStaDifFile + (index.dwStart);
								else
									pbStatics = pbStaticFile + (index.dwStart);

								StaticData * objects = new StaticData [index.dwLength];
								memcpy(&objects[0], (void*)pbStatics, index.dwLength);
								for ( int m = 0; m < numStatics; m++ )
								{
									// Find out which has the highest z value.
									int statBlock = objects[m].bXoff + (objects[m].bYoff * 8);
									if ( statBlock > 64 )
										continue;
									objects[m].bJunk1 = 0xFF;
									if ( statCells[statBlock].bJunk1 == 0xFF )
									{
										if ( statCells[statBlock].bAlt <= objects[m].bAlt )
											memcpy(&statCells[statBlock], &objects[m], sizeof(StaticData));
									}
									else
										memcpy(&statCells[statBlock], &objects[m], sizeof(StaticData));
								}
								delete [] objects;
							}
						}
						// How much of this block do we need to put into our output array?
						for ( int cx = 0; cx < iCellsPerBlock; cx++ )
						{
							for ( int cy = 0; cy < iCellsPerBlock; cy++ )
							{
								
								int iCellIndex = (cy * 8 / iCellsPerBlock) * 8 + (cx * 8 / iCellsPerBlock); 
								WORD wColor;
								WORD wColorHi = (WORD(mbArray[j - Y1].cells[iCellIndex].bColorHi)) << 8;
								WORD wColorLo = (WORD(mbArray[j - Y1].cells[iCellIndex].bColorLo));
								wColor = wColorHi | wColorLo;
								DWORD dwColor = m_dwColorMap[wColor];
								BYTE bRed, bGreen, bBlue;
								if ( bDrawStatics && (statCells[iCellIndex].bJunk1 == 0xFF) && (statCells[iCellIndex].bAlt >= mbArray[j - Y1].cells[iCellIndex].bAltitude) )
								{
									// Read the map block for this guy as well, to make sure that the static is above ground
									WORD wColor, wColorHi, wColorLo;
									wColorHi = (WORD) ((statCells[iCellIndex].bColorHi) << 8);
									wColorLo = (WORD) statCells[iCellIndex].bColorLo;
									wColor = (WORD) wColorHi | wColorLo;
									wColor += 0x4000;
									dwColor = m_dwColorMap[wColor];
								}
								bRed = (BYTE) ((dwColor & 0x00FF0000) >> 16);
								bGreen = (BYTE) ((dwColor & 0x0000FF00) >> 8);
								bBlue = (BYTE) ((dwColor & 0x000000FF));
								// Do we draw the map color, or a static color?
								for ( int px = 0; px < iPixelsPerCell; px++ )
								{
									for ( int py = 0; py < iPixelsPerCell; py++ )
									{
										int X, Y;
										X = ((i - wX1) / scalefactor * iCellsPerBlock * iPixelsPerCell) + (cx * iPixelsPerCell) + px;
										Y = ((j - wY1) / scalefactor * iCellsPerBlock * iPixelsPerCell) + (cy * iPixelsPerCell) + py;
										if ( X >=0 && X < RectWidth && Y >= 0 && Y < RectHeight )
										{
											int iIndex = ( ( ( Y ) * RectWidth ) + ( X ) ) * 3;
											m_bMapArray.SetAt(iIndex, bRed);
											m_bMapArray.SetAt(iIndex + 1, bGreen);
											m_bMapArray.SetAt(iIndex + 2, bBlue);
										}
									}
								}
							}
						}
					}
				}
			}
			delete [] mbArray;

			int x1, y1, x2, y2;
			x1 = CtrlToMapX(0);
			y1 = CtrlToMapY(0);
			x2 = CtrlToMapX(rcBounds.Width());
			y2 = CtrlToMapY(rcBounds.Height());
			CRect rcMap(x1, y1, x2, y2);
			// Add the user-defined draw rects
			for (int i = 0; i < m_aDrawRects.GetSize(); i++ )
			{
				CDrawRect * pRect = (CDrawRect *) m_aDrawRects.GetAt(i);
				if ( !pRect || pRect->m_bDrawMode >= DRAW_MODE_QTY)
					continue;
				CPoint ptTL, ptBR;
				ptTL = pRect->m_rect.TopLeft();
				ptBR = pRect->m_rect.BottomRight();
				CRect test;
				if ( test.UnionRect(rcMap, pRect->m_rect) == 0)
					continue;
				BYTE bBlue, bRed, bGreen;
				bRed = (BYTE) ((pRect->m_dwColor & 0x00FF0000) >> 16);
				bGreen = (BYTE) ((pRect->m_dwColor & 0x0000FF00) >> 8);
				bBlue = (BYTE) ((pRect->m_dwColor & 0x000000FF));
				int dx, dy;
				dx = ( ( ptTL.x - (wX1 * 8) ) / scalefactor * iCellsPerBlock / 8 ) * iPixelsPerCell;
				dy = ( ( ptTL.y - (wY1 * 8) ) / scalefactor * iCellsPerBlock / 8 ) * iPixelsPerCell;
				for ( int x = 0; x <= pRect->m_rect.Width() / scalefactor * iCellsPerBlock / 8; x++ )
				{
					for ( int y = 0; y <= pRect->m_rect.Height() / scalefactor * iCellsPerBlock / 8; y++ )
					{
						if ( pRect->m_bDrawMode == DRAW_OUTLINE )
						{
							if ( x == 0 || x == (pRect->m_rect.Width() / scalefactor * iCellsPerBlock / 8 ) || y == 0 || y == (pRect->m_rect.Height() / scalefactor * iCellsPerBlock / 8 ) )
							{
								// This is part of the outline
							}
							else
								continue;
						}
						for ( int px = 0; px < iPixelsPerCell; px++ )
						{
							for ( int py = 0; py < iPixelsPerCell; py++ )
							{
								int XR, YR;
								XR = (x * iPixelsPerCell) + px;
								YR = (y * iPixelsPerCell) + py;
								int XC, YC;
								XC = dx + XR;
								YC = dy + YR;
								if ( XC < 0 || XC >= RectWidth )
									continue;
								if ( YC < 0 || YC >= RectHeight )
									continue;
								int iIndex = ( ( YC * RectWidth ) + XC ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								if ( pRect->m_bDrawMode != DRAW_BLEND )
								{
									m_bMapArray.SetAt(iIndex, bRed);
									m_bMapArray.SetAt(iIndex + 1, bGreen);
									m_bMapArray.SetAt(iIndex + 2, bBlue);
								}
								else
								{
									BYTE r, g, b;
									r = m_bMapArray.GetAt(iIndex);
									g = m_bMapArray.GetAt(iIndex + 1);
									b = m_bMapArray.GetAt(iIndex + 2);
									m_bMapArray.SetAt(iIndex, bRed | r);
									m_bMapArray.SetAt(iIndex + 1, bGreen | g);
									m_bMapArray.SetAt(iIndex + 2, bBlue | b);
								}
							}
						}
					}
				}
			}
			// Add the user-defined draw objects
			for (int i = 0; i < m_aDrawObjects.GetSize(); i++ )
			{
				CDrawObject * pObject = (CDrawObject *) m_aDrawObjects.GetAt(i);
				if ( !pObject )
					continue;
				CPoint ptTL, ptBR;
				ptTL.x = pObject->m_pt.x - pObject->m_bSize;
				ptTL.y = pObject->m_pt.y - pObject->m_bSize;
				ptBR.x = pObject->m_pt.x + pObject->m_bSize;
				ptBR.y = pObject->m_pt.y + pObject->m_bSize;
				if ( !rcMap.PtInRect(ptTL) && !rcMap.PtInRect(ptBR) )
					continue;
				BYTE bBlue, bRed, bGreen;
				bRed = (BYTE) ((pObject->m_dwColor & 0x00FF0000) >> 16);
				bGreen = (BYTE) ((pObject->m_dwColor & 0x0000FF00) >> 8);
				bBlue = (BYTE) ((pObject->m_dwColor & 0x000000FF));
				int X, Y;
				X = (pObject->m_pt.x - wX1 * 8) / scalefactor * iPixelsPerCell / ( 8 / iCellsPerBlock);
				Y = (pObject->m_pt.y - wY1 * 8) / scalefactor * iPixelsPerCell / ( 8 / iCellsPerBlock);
				switch ( pObject->m_bDrawType )
				{
				case DRAW_NONE:
					break;
				case DRAW_PLUS:
					{
						for ( int x = -(pObject->m_bSize); x <= pObject->m_bSize; x++ )
						{
							int xp;
							xp = X + x;
							if (( xp >= RectWidth ) || ( xp < 0 ))
								continue;
							int iIndex = ( ( ( Y ) * RectWidth ) + ( xp ) ) * 3;
							if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
								continue;
							m_bMapArray.SetAt(iIndex, bRed);
							m_bMapArray.SetAt(iIndex + 1, bGreen);
							m_bMapArray.SetAt(iIndex + 2, bBlue);
						}
						for ( int y = -(pObject->m_bSize); y <= pObject->m_bSize; y++ )
						{
							int yp;
							yp = Y + y;
							if (( X >= RectWidth ) || ( X < 0 ))
								continue;
							int iIndex = ( ( ( yp ) * RectWidth ) + ( X ) ) * 3;
							if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
								continue;
							m_bMapArray.SetAt(iIndex, bRed);
							m_bMapArray.SetAt(iIndex + 1, bGreen);
							m_bMapArray.SetAt(iIndex + 2, bBlue);
						}
					}
					break;
				case DRAW_SQUARE:
					{
						for ( int x = -(pObject->m_bSize); x <= pObject->m_bSize; x++ )
						{
							for ( int y = -(pObject->m_bSize); y <= pObject->m_bSize; y++ )
							{
								int xp, yp;
								xp = X + x;
								yp = Y + y;
								if (( xp >= RectWidth ) || ( xp < 0 ))
									continue;
								int iIndex = ( ( ( yp ) * RectWidth ) + ( xp ) ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								m_bMapArray.SetAt(iIndex, bRed);
								m_bMapArray.SetAt(iIndex + 1, bGreen);
								m_bMapArray.SetAt(iIndex + 2, bBlue);
							}
						}
					}
					break;
				case DRAW_DIAMOND:
					{
						for ( int x = -(pObject->m_bSize); x <= 0; x++ )
						{
							for ( int y = - ( x + pObject->m_bSize) ; y <= x + pObject->m_bSize; y++ )
							{
								int xp, yp;
								xp = X + x;
								yp = Y + y;
								if (( xp >= RectWidth ) || ( xp < 0 ))
									continue;
								int iIndex = ( ( ( yp ) * RectWidth ) + ( xp ) ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								m_bMapArray.SetAt(iIndex, bRed);
								m_bMapArray.SetAt(iIndex + 1, bGreen);
								m_bMapArray.SetAt(iIndex + 2, bBlue);
							}
						}
						for (int x = 0; x <= pObject->m_bSize; x++ )
						{
							for ( int y = ( x - pObject->m_bSize) ; y <= (pObject->m_bSize - x); y++ )
							{
								int xp, yp;
								xp = X + x;
								yp = Y + y;
								if (( xp >= RectWidth ) || ( xp < 0 ))
									continue;
								int iIndex = ( ( ( yp ) * RectWidth ) + ( xp ) ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								m_bMapArray.SetAt(iIndex, bRed);
								m_bMapArray.SetAt(iIndex + 1, bGreen);
								m_bMapArray.SetAt(iIndex + 2, bBlue);
							}
						}
					}
					break;
				case DRAW_DOT:
					{
						int iIndex = ( ( ( Y ) * RectWidth ) + ( X ) ) * 3;
						if ( iIndex < 0 || iIndex > (RectWidth * RectHeight * 3) )
							continue;
						m_bMapArray.SetAt(iIndex, bRed);
						m_bMapArray.SetAt(iIndex + 1, bGreen);
						m_bMapArray.SetAt(iIndex + 2, bBlue);
					}
					break;
				case DRAW_CIRCLE:
					{
						for ( int x = -(pObject->m_bSize); x <= pObject->m_bSize; x++ )
						{
							int s = pObject->m_bSize;
							int yb = (int) sqrt((double)s * s - (x * x) );
							for ( int y = -yb; y <= yb; y++ )
							{
								int xp, yp;
								xp = X + x;
								yp = Y + y;
								if (( xp >= RectWidth ) || ( xp < 0 ))
									continue;
								int iIndex = ( ( ( yp ) * RectWidth ) + ( xp ) ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								m_bMapArray.SetAt(iIndex, bRed);
								m_bMapArray.SetAt(iIndex + 1, bGreen);
								m_bMapArray.SetAt(iIndex + 2, bBlue);
							}
						}
					}
					break;
				case DRAW_TRIANGLE:
					{
						for ( int x = -(pObject->m_bSize); x <= 0; x++ )
						{
							for ( int y = (- pObject->m_bSize / 4 * x - pObject->m_bSize); y <= (pObject->m_bSize); y++ )
							{
								int xp, yp;
								xp = X + x;
								yp = Y + y;
								if (( xp >= RectWidth ) || ( xp < 0 ))
									continue;
								int iIndex = ( ( ( yp ) * RectWidth ) + ( xp ) ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								m_bMapArray.SetAt(iIndex, bRed);
								m_bMapArray.SetAt(iIndex + 1, bGreen);
								m_bMapArray.SetAt(iIndex + 2, bBlue);
							}
						}
						for (int x = 0; x <= pObject->m_bSize; x++ )
						{
							for ( int y =  (pObject->m_bSize / 4 * x - pObject->m_bSize); y <= (pObject->m_bSize); y++ )
							{
								int xp, yp;
								xp = X + x;
								yp = Y + y;
								if (( xp >= RectWidth ) || ( xp < 0 ))
									continue;
								int iIndex = ( ( ( yp ) * RectWidth ) + ( xp ) ) * 3;
								if ( iIndex < 0 || iIndex >= (RectWidth * RectHeight * 3) )
									continue;
								m_bMapArray.SetAt(iIndex, bRed);
								m_bMapArray.SetAt(iIndex + 1, bGreen);
								m_bMapArray.SetAt(iIndex + 2, bBlue);
							}
						}
					}
					break;
				default:
					break;
				}
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

		SetDIBitsToDevice(pdc->GetSafeHdc(), rcBounds.left, rcBounds.top, RectWidth, RectHeight, 0, 0, 0, RectHeight, m_bMapArray.GetData(), &bmi, DIB_RGB_COLORS);
		m_bIsValid = true;
		return;
	}
	catch (...)
	{
	}
}

DWORD CUOMap::ScaleColor(WORD wColor)
{
	DWORD dwNewColor;
	
	dwNewColor = ((((((wColor >> 10) & 0x01f) * 0x0ff / 0x01f))
		| (((((wColor >> 5) & 0x01f) * 0x0ff / 0x01f)) << 8)
		| ((((wColor & 0x01f) * 0x0ff / 0x01f)) << 16)));
	return (dwNewColor);
}

/////////////////////////////////////////////////////////////////////////////
// Loading

void CUOMap::LoadRadarcol()
{
	CFile cfRadarFile;
	CString csRadarFileName = Main->GetMulPath(VERFILE_RADARCOL);
	memset(&m_dwColorMap[0], 0, sizeof(m_dwColorMap));
	if (cfRadarFile.Open(csRadarFileName, CFile::modeRead | CFile::shareDenyNone))
	{
		WORD wColorArray[65536];
		memset(&wColorArray, 0x00, sizeof(wColorArray));
		cfRadarFile.Read(&wColorArray[0], sizeof(wColorArray));
		cfRadarFile.Close();
		for (long i = 0; i < 65536; i++)
			m_dwColorMap[i] = ScaleColor(wColorArray[i]);
	}
	else
		Main->m_log.Add(1,"ERROR: Unable to open %s", csRadarFileName);
}

void CUOMap::OpenMap()
{
	if (iLoaded)
		CloseMap();
	CString csMapFileName;
	CString csStaticIdxName;
	CString csStaticFileName;

	//Diff support
	CString csMapDifFileName;
	CString csMapDiflFileName;
	CString csStaDifName;
	CString csStaDifiName;
	CString csStaDiflName;

	CString csMapPattern;

	switch ( m_mulfile )
	{
	case 0:
		{
		csMapFileName = Main->GetMulPath(VERFILE_MAP0);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX0);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS0);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF0);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL0);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF0);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI0);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL0);
		break;
		}
	case 1:
		{
		csMapFileName = Main->GetMulPath(VERFILE_MAP1);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX1);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS1);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF1);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL1);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF1);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI1);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL1);
		break;
		}
	case 2:
		{
		csMapFileName = Main->GetMulPath(VERFILE_MAP2);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX2);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS2);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF2);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL2);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF2);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI2);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL2);
		break;
		}
	case 3:
		{
		csMapFileName = Main->GetMulPath(VERFILE_MAP3);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX3);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS3);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF3);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL3);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF3);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI3);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL3);
		break;
		}
	case 4:
		{
		csMapFileName = Main->GetMulPath(VERFILE_MAP4);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX4);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS4);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF4);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL4);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF4);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI4);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL4);
		break;
		}
	case 5:
		{
		csMapFileName = Main->GetMulPath(VERFILE_MAP5);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX5);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS5);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF5);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL5);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF5);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI5);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL5);
		break;
		}
	}

	hMapFile = CreateFile(csMapFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hStaIdxFile = CreateFile(csStaticIdxName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hStaticFile = CreateFile(csStaticFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//Diff support
	hMapDifFile = CreateFile(csMapDifFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hMapDiflFile = CreateFile(csMapDiflFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hStaDifFile = CreateFile(csStaDifName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hStaDifiFile = CreateFile(csStaDifiName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hStaDiflFile = CreateFile(csStaDiflName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	bIsUOP = false;
	if (hMapFile == INVALID_HANDLE_VALUE)
	{
		switch ( m_mulfile )
		{
			case 0:
			{
				csMapFileName = Main->GetMulPath(VERFILE_MAP0_UOP);
				csMapPattern = GetMulFileName(VERFILE_MAP0_UOP);
				csMapPattern =  csMapPattern.SpanExcluding(".");
				break;
			}
			case 1:
			{
				csMapFileName = Main->GetMulPath(VERFILE_MAP1_UOP);
				csMapPattern = GetMulFileName(VERFILE_MAP1_UOP);
				csMapPattern =  csMapPattern.SpanExcluding(".");
				break;
			}
			case 2:
			{
				csMapFileName = Main->GetMulPath(VERFILE_MAP2_UOP);
				csMapPattern = GetMulFileName(VERFILE_MAP2_UOP);
				csMapPattern =  csMapPattern.SpanExcluding(".");
				break;
			}
			case 3:
			{
				csMapFileName = Main->GetMulPath(VERFILE_MAP3_UOP);
				csMapPattern = GetMulFileName(VERFILE_MAP3_UOP);
				csMapPattern =  csMapPattern.SpanExcluding(".");
				break;
			}
			case 4:
			{
				csMapFileName = Main->GetMulPath(VERFILE_MAP4_UOP);
				csMapPattern = GetMulFileName(VERFILE_MAP4_UOP);
				csMapPattern =  csMapPattern.SpanExcluding(".");
				break;
			}
			case 5:
			{
				csMapFileName = Main->GetMulPath(VERFILE_MAP5_UOP);
				csMapPattern = GetMulFileName(VERFILE_MAP5_UOP);
				csMapPattern =  csMapPattern.SpanExcluding(".");
				break;
			}
		}
		hMapFile = CreateFile(csMapFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		bIsUOP = true;
		
	}

	if ( (hMapFile == INVALID_HANDLE_VALUE) || (hStaIdxFile == INVALID_HANDLE_VALUE) || (hStaticFile == INVALID_HANDLE_VALUE) )
	{
		if ( hMapFile != INVALID_HANDLE_VALUE )
			CloseHandle(hMapFile);
		if ( hStaIdxFile != INVALID_HANDLE_VALUE )
			CloseHandle(hStaIdxFile);
		if ( hStaticFile != INVALID_HANDLE_VALUE )
			CloseHandle(hStaticFile);
		if ( hMapDifFile != INVALID_HANDLE_VALUE )
			CloseHandle(hMapDifFile);
		if ( hMapDiflFile != INVALID_HANDLE_VALUE )
			CloseHandle(hMapDiflFile);
		if ( hStaDifFile != INVALID_HANDLE_VALUE )
			CloseHandle(hStaDifFile);
		if ( hStaDifiFile != INVALID_HANDLE_VALUE )
			CloseHandle(hStaDifiFile);
		if ( hStaDiflFile != INVALID_HANDLE_VALUE )
			CloseHandle(hStaDiflFile);

		if ((m_mulfile != 1)||(bIsUOP))
			return;

		csMapFileName = Main->GetMulPath(VERFILE_MAP0);
		csStaticIdxName = Main->GetMulPath(VERFILE_STAIDX0);
		csStaticFileName = Main->GetMulPath(VERFILE_STATICS0);
		//Diff support
		csMapDifFileName = Main->GetMulPath(VERFILE_MAPDIF1);
		csMapDiflFileName = Main->GetMulPath(VERFILE_MAPDIFL1);
		csStaDifName = Main->GetMulPath(VERFILE_STADIF1);
		csStaDifiName = Main->GetMulPath(VERFILE_STADIFI1);
		csStaDiflName = Main->GetMulPath(VERFILE_STADIFL1);

		hMapFile = CreateFile(csMapFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hStaIdxFile = CreateFile(csStaticIdxName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hStaticFile = CreateFile(csStaticFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//Diff support
		hMapDifFile = CreateFile(csMapDifFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hMapDiflFile = CreateFile(csMapDiflFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hStaDifFile = CreateFile(csStaDifName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hStaDifiFile = CreateFile(csStaDifiName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hStaDiflFile = CreateFile(csStaDiflName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if ( (hMapFile == INVALID_HANDLE_VALUE) || (hStaIdxFile == INVALID_HANDLE_VALUE) || (hStaticFile == INVALID_HANDLE_VALUE) )
		{
			if ( hMapFile != INVALID_HANDLE_VALUE )
				CloseHandle(hMapFile);
			if ( hStaIdxFile != INVALID_HANDLE_VALUE )
				CloseHandle(hStaIdxFile);
			if ( hStaticFile != INVALID_HANDLE_VALUE )
				CloseHandle(hStaticFile);
			if ( hMapDifFile != INVALID_HANDLE_VALUE )
				CloseHandle(hMapDifFile);
			if ( hMapDiflFile != INVALID_HANDLE_VALUE )
				CloseHandle(hMapDiflFile);
			if ( hStaDifFile != INVALID_HANDLE_VALUE )
				CloseHandle(hStaDifFile);
			if ( hStaDifiFile != INVALID_HANDLE_VALUE )
				CloseHandle(hStaDifiFile);
			if ( hStaDiflFile != INVALID_HANDLE_VALUE )
				CloseHandle(hStaDiflFile);
			return;
		}
	}
	hMapFileMapping = CreateFileMapping(hMapFile, NULL, PAGE_READONLY, 0, 0, NULL);
	hStaIdxFileMapping = CreateFileMapping(hStaIdxFile, NULL, PAGE_READONLY, 0, 0, NULL);
	hStaticFileMapping = CreateFileMapping(hStaticFile, NULL, PAGE_READONLY, 0, 0, NULL);
	pbMapFile = (PBYTE) MapViewOfFile(hMapFileMapping, FILE_MAP_READ, 0, 0, 0);
	pbStaIdxFile = (PBYTE) MapViewOfFile(hStaIdxFileMapping, FILE_MAP_READ, 0, 0, 0);
	pbStaticFile = (PBYTE) MapViewOfFile(hStaticFileMapping, FILE_MAP_READ, 0, 0, 0);

	//Diff support
	if ( hMapDifFile != INVALID_HANDLE_VALUE )
	{
		hMapDifFileMapping = CreateFileMapping(hMapDifFile, NULL, PAGE_READONLY, 0, 0, NULL);
		pbMapDifFile = (PBYTE) MapViewOfFile(hMapDifFileMapping, FILE_MAP_READ, 0, 0, 0);
	}
	if ( hMapDiflFile != INVALID_HANDLE_VALUE )
	{
		hMapDiflFileMapping = CreateFileMapping(hMapDiflFile, NULL, PAGE_READONLY, 0, 0, NULL);
		pbMapDiflFile = (PBYTE) MapViewOfFile(hMapDiflFileMapping, FILE_MAP_READ, 0, 0, 0);
	}
	if ( hStaDifFile != INVALID_HANDLE_VALUE )
	{
		hStaDifFileMapping = CreateFileMapping(hStaDifFile, NULL, PAGE_READONLY, 0, 0, NULL);
		pbStaDifFile = (PBYTE) MapViewOfFile(hStaDifFileMapping, FILE_MAP_READ, 0, 0, 0);
	}
	if ( hStaDifiFile != INVALID_HANDLE_VALUE )
	{
		hStaDifiFileMapping = CreateFileMapping(hStaDifiFile, NULL, PAGE_READONLY, 0, 0, NULL);
		pbStaDifiFile = (PBYTE) MapViewOfFile(hStaDifiFileMapping, FILE_MAP_READ, 0, 0, 0);
	}
	if ( hStaDiflFile != INVALID_HANDLE_VALUE )
	{
		hStaDiflFileMapping = CreateFileMapping(hStaDiflFile, NULL, PAGE_READONLY, 0, 0, NULL);
		pbStaDiflFile = (PBYTE) MapViewOfFile(hStaDiflFileMapping, FILE_MAP_READ, 0, 0, 0);
	}

	iMapPatch = LoadPatchHash(csMapDiflFileName, m_aMapHash);
	iStaticPatch = LoadPatchHash(csStaDiflName, m_aStaticHash); 

	paMapHash = &m_aMapHash;
	paStaticHash = &m_aStaticHash;

	if (bIsUOP) //Parse file for quick reference
	{
		DWORD dwUOPHashLo, dwUOPHashHi, dwCompressedSize, dwHeaderLenght;
		DWORD dwFilesInBlock, dwTotalFiles, dwLoop;
		DWORD64 dwUOPPtr;
		PBYTE pbMapFilePtr = pbMapFile + (sizeof(DWORD)*3);
		memcpy(&dwUOPHashLo, pbMapFilePtr, sizeof(DWORD));
		memcpy(&dwUOPHashHi, pbMapFilePtr+(sizeof(DWORD)), sizeof(DWORD));
		dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
		memcpy(&dwTotalFiles, pbMapFilePtr+(sizeof(DWORD)*3), sizeof(DWORD));
		pbMapFilePtr = pbMapFile + dwUOPPtr;
		dwLoop = dwTotalFiles;

		while (dwUOPPtr > 0)
		{
			memcpy(&dwFilesInBlock, pbMapFilePtr, sizeof(DWORD));
			memcpy(&dwUOPHashLo, pbMapFilePtr+sizeof(DWORD), sizeof(DWORD));
			memcpy(&dwUOPHashHi, pbMapFilePtr+(sizeof(DWORD)*2), sizeof(DWORD));
			pbMapFilePtr = pbMapFile + dwUOPPtr + (sizeof(DWORD)*3);

			dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;

			while ((dwFilesInBlock > 0)&&(dwTotalFiles > 0))
			{
					dwTotalFiles--;
					dwFilesInBlock--;

					memcpy(&dwUOPHashLo, pbMapFilePtr, sizeof(DWORD));
					memcpy(&dwUOPHashHi, pbMapFilePtr + sizeof(DWORD), sizeof(DWORD));
					memcpy(&dwHeaderLenght, pbMapFilePtr + (sizeof(DWORD)*2), sizeof(DWORD));
					memcpy(&dwCompressedSize, pbMapFilePtr + (sizeof(DWORD)*3), sizeof(DWORD));


					MapAddress * pMapAddress = new (MapAddress);
					pMapAddress->qwAdress = (((__int64)dwUOPHashHi << 32) + dwUOPHashLo) + dwHeaderLenght;

					memcpy(&dwUOPHashLo, pbMapFilePtr + (sizeof(DWORD)*5), sizeof(DWORD));
					memcpy(&dwUOPHashHi, pbMapFilePtr + (sizeof(DWORD)*6), sizeof(DWORD));
					__int64 qwHash = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
					
					CString csFile;
					for (int x = 0; x < dwLoop; x++)
					{
						csMapPattern.MakeLower();
						csFile.Format("build/%s/%.8d.dat", csMapPattern, x);
						if (HashFileName(csFile) == qwHash)
						{
							pMapAddress->dwFirstBlock = x*4096;
							pMapAddress->dwLastBlock = (x*4096)+(dwCompressedSize / 196)-1;
							m_aUopAddress.SetAtGrow(x,pMapAddress);
							break;
						}
					}
					
					pbMapFilePtr += 34;
			}

			pbMapFilePtr = pbMapFile + dwUOPPtr;
		}
	}


	iLoaded = true;

}

MapBlock CUOMap::LoadUopBlock(int iBlock)
{
	MapBlock mbData;
	for ( int i = 0; i < m_aUopAddress.GetSize(); i++ )
	{
		MapAddress * pMapAddress = (MapAddress *) m_aUopAddress.GetAt(i);
		if (!pMapAddress)
			continue;
		if (( iBlock <= pMapAddress->dwLastBlock ) && ( iBlock >= pMapAddress->dwFirstBlock ))
		{
			DWORD64 offset = pMapAddress->qwAdress;
			offset += (iBlock - pMapAddress->dwFirstBlock)*196;
			PBYTE pbMapFilePtr = pbMapFile + offset;
			memcpy(&mbData, pbMapFilePtr, sizeof(MapBlock));
			break;
		}
	}
	return mbData;
}

void CUOMap::CloseMap()
{
	if (iLoaded)
	{
		for ( int i = 0; i < m_aUopAddress.GetSize(); i++ )
		{
			MapAddress * pMapAddress = (MapAddress *) m_aUopAddress.GetAt(i);
			if ( pMapAddress )
				delete pMapAddress;
		}
		m_aUopAddress.RemoveAll();

		UnmapViewOfFile(pbMapFile);
		UnmapViewOfFile(pbStaIdxFile);
		UnmapViewOfFile(pbStaticFile);
		UnmapViewOfFile(pbMapDifFile);
		UnmapViewOfFile(pbMapDiflFile);
		UnmapViewOfFile(pbStaDifFile);
		UnmapViewOfFile(pbStaDifiFile);
		UnmapViewOfFile(pbStaDiflFile);

		if ( hMapFile != INVALID_HANDLE_VALUE )
		{
			if ( hMapFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hMapFileMapping);
			CloseHandle(hMapFile);
		}
		if ( hStaIdxFile != INVALID_HANDLE_VALUE )
		{
			if ( hStaIdxFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hStaIdxFileMapping);
			CloseHandle(hStaIdxFile);
		}
		if ( hStaticFile != INVALID_HANDLE_VALUE )
		{
			if ( hStaticFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hStaticFileMapping);
			CloseHandle(hStaticFile);
		}
		if ( hMapDifFile != INVALID_HANDLE_VALUE )
		{
			if ( hMapDifFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hMapDifFileMapping);
			CloseHandle(hMapDifFile);
		}
		if ( hMapDiflFile != INVALID_HANDLE_VALUE )
		{
			if ( hMapDiflFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hMapDiflFileMapping);
			CloseHandle(hMapDiflFile);
		}
		if ( hStaDifFile != INVALID_HANDLE_VALUE )
		{
			if ( hStaDifFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hStaDifFileMapping);
			CloseHandle(hStaDifFile);
		}
		if ( hStaDifiFile != INVALID_HANDLE_VALUE )
		{
			if ( hStaDifiFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hStaDifiFileMapping);
			CloseHandle(hStaDifiFile);
		}
		if ( hStaDiflFile != INVALID_HANDLE_VALUE )
		{
			if ( hStaDiflFileMapping != INVALID_HANDLE_VALUE )
				CloseHandle(hStaDiflFileMapping);
			CloseHandle(hStaDiflFile);
		}
	}
	iLoaded = false;
}


/////////////////////////////////////////////////////////////////////////////
// CDraw*

CDrawObject::CDrawObject()
{
	m_pt.x = 0;
	m_pt.y = 0;
	m_bDrawType = 0;
	m_bSize = 0;
	m_dwColor = 0;
}

CDrawRect::CDrawRect()
{
	m_bDrawMode = 0;
	m_dwColor = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CUOMap properties

void CUOMap::ReSetCenter(short x, short y)
{
	if ( m_xCenter == x && m_yCenter == y )
		return;
	if ( x >= 0 && x < UO_X_SIZE && y >= 0 && y < UO_Y_SIZE )
	{
		m_xCenter = x;
		m_yCenter = y;
	}
}


short CUOMap::GetZoomLevel()
{
	return m_zoomLevel;
}

void CUOMap::SetZoomLevel(short propVal)
{
	if ( m_zoomLevel == propVal )
		return;
	if ( propVal >= -4 && propVal <= 4 )
		m_zoomLevel = propVal;
	else
		return;
}

short CUOMap::GetXCenter()
{
	return m_xCenter;
}

void CUOMap::SetXCenter(short propVal)
{
	if ( m_xCenter == propVal )
		return;
	if ( propVal >= 0 && propVal < UO_X_SIZE )
		m_xCenter = propVal;
	else
		return;
}

short CUOMap::GetYCenter()
{
	return m_yCenter;
}

void CUOMap::SetYCenter(short propVal)
{
	if ( m_yCenter == propVal )
		return;
	if ( propVal >= 0 && propVal < UO_Y_SIZE )
		m_yCenter = propVal;
	else
		return;
}

short CUOMap::GetWidth()
{
	return m_maxX;
}

void CUOMap::SetWidth(short propVal)
{
	if ( m_maxX == propVal )
		return;
	m_maxX = propVal;
}

short CUOMap::GetHeight()
{
	return m_maxY;
}

void CUOMap::SetHeight(short propVal)
{
	if ( m_maxY == propVal )
		return;
	m_maxY = propVal;
}

void CUOMap::ZoomIn()
{
	if ( m_zoomLevel < 2 )
		m_zoomLevel++;
	else
		return;
}

void CUOMap::ZoomOut()
{
	if ( m_zoomLevel > -4 )
		m_zoomLevel--;
	else
		return;
}

void CUOMap::SetCenter(short x, short y)
{
	if ( m_xCenter == x && m_yCenter == y )
		return;
	if ( x >= 0 && x <= UO_X_SIZE && y >= 0 && y <= UO_Y_SIZE )
	{
		m_xCenter = x;
		m_yCenter = y;
	}
}

void CUOMap::Scroll(short direction, short distance)
{
	int x, y;
	switch ( direction )
	{
	case SCROLL_E:
	case SCROLL_NE:
	case SCROLL_SE:
		x = distance;
		break;
	case SCROLL_W:
	case SCROLL_NW:
	case SCROLL_SW:
		x = - distance;
		break;
	default:
		x = 0;
	}
	switch (direction)
	{
	case SCROLL_N:
	case SCROLL_NE:
	case SCROLL_NW:
		y = - distance;
		break;
	case SCROLL_S:
	case SCROLL_SE:
	case SCROLL_SW:
		y = distance;
		break;
	default:
		y = 0;
	}
	if ( x == 0 && y == 0 )
		return;
	m_xCenter += x;
	m_yCenter += y;
	if ( m_xCenter < 0 )
		m_xCenter = 0;
	if ( m_xCenter >= UO_X_SIZE )
		m_xCenter = UO_X_SIZE;
	if ( m_yCenter < 0 )
		m_yCenter = 0;
	if ( m_yCenter >= UO_Y_SIZE )
		m_yCenter = UO_Y_SIZE;
}

short CUOMap::GetMapFile()
{
	return m_mulfile;
}

void CUOMap::SetMapFile(short propVal)
{
	m_mulfile = propVal;
	OpenMap();
}

void CUOMap::LoadMap(short m, short w, short h)
{
	m_maxX = w;
	m_maxY = h;
	if (m_maxX == 0)
		m_maxX = 6144;
	if (m_maxY == 0)
		m_maxY = 4096;

	// Make sure the current coords are within the bounds
	if ( m_xCenter > m_maxX )
		m_xCenter = UO_X_CENTER;
	if ( m_yCenter > m_maxY )
		m_yCenter = UO_Y_CENTER;
	SetMapFile(m);
}

short CUOMap::GetMapHeight(short x, short y)
{
	int XBlock, YBlock, XCell, YCell;
	XBlock = x / 8;
	YBlock = y / 8;
	XCell = x % 8;
	YCell = y % 8;
	int BlockNumber = (XBlock * UO_Y_BLOCKS) + YBlock;
	int cell = (XCell * 8) + YCell;
	
	if ((x > m_maxX) || (y > m_maxY))
		return 0;

	if (bIsUOP)
	{
		MapBlock block;
		block = LoadUopBlock(BlockNumber);
		return (short) block.cells[cell].bAltitude;
	}
	else
	{
		DWORD dwIndex = 0xFFFFFFFF;
		CString sFile;

		// Is this a patched block?
		if (m_wDrawFlags & F_DRAWDIFS)
		{
			switch ( m_mulfile )
			{
			case 0:
				sFile = Main->GetMulPath(VERFILE_MAPDIF0);
				break;
			case 1:
				sFile = Main->GetMulPath(VERFILE_MAPDIF1);
				break;
			case 2:
				sFile = Main->GetMulPath(VERFILE_MAPDIF2);
				break;
			case 3:
				sFile = Main->GetMulPath(VERFILE_MAPDIF3);
				break;
			case 4:
				sFile = Main->GetMulPath(VERFILE_MAPDIF4);
				break;
			case 5:
				sFile = Main->GetMulPath(VERFILE_MAPDIF5);
				break;
			}
			if (iMapPatch)
				dwIndex = FindPatch(*paMapHash, (DWORD) BlockNumber);
		}

		if ( dwIndex == 0xFFFFFFFF )
		{
			switch (m_mulfile)
			{
			case 0:
				sFile = Main->GetMulPath(VERFILE_MAP0);
				break;
			case 1:
				sFile = Main->GetMulPath(VERFILE_MAP1);
				break;
			case 2:
				sFile = Main->GetMulPath(VERFILE_MAP2);
				break;
			case 3:
				sFile = Main->GetMulPath(VERFILE_MAP3);
				break;
			case 4:
				sFile = Main->GetMulPath(VERFILE_MAP4);
				break;
			case 5:
				sFile = Main->GetMulPath(VERFILE_MAP5);
				break;
			}
			dwIndex = BlockNumber;
		}

		CFile fMap;
		MapBlock block;

		if ( !fMap.Open(sFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			return 0;
		if (fMap.Seek(dwIndex * sizeof(MapBlock), CFile::begin))
		{
			if (fMap.Read(&block, sizeof(block)))
			{
				fMap.Close();
				return (short) block.cells[cell].bAltitude;
			}
		}
		return 0;
	}
}

void CUOMap::GetCenter(short* x, short* y)
{
	*x = GetXCenter();
	*y = GetYCenter();
}

short CUOMap::GetDrawFlags()
{
	return m_wDrawFlags;
}

void CUOMap::SetDrawFlags(short propVal)
{
	m_wDrawFlags = (WORD) propVal;
}

///////////////////////////////////////////////////////////
//Controle - Map interaction

void CUOMap::CtrlToMap(short* x, short* y)
{
	*x = CtrlToMapX(*x);
	*y = CtrlToMapY(*y);
}

void CUOMap::MapToCtrl(short* x, short* y)
{
	*x = MapToCtrlX(*x);
	*y = MapToCtrlY(*y);
}


short CUOMap::CtrlToMapX(short x)
{
	// Convert from the control reference frame to map reference frame
	CRect rcMap;
	GetClientRect(&rcMap);
	int RectWidth = rcMap.Width() - rcMap.Width() % 8;
	int xc = m_xCenter / 8;
	int scalefactor = 1;
	int iCellsPerBlock = 8;
	int iPixelsPerCell = 1;
	if ( m_zoomLevel > 0 )
		iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
	if ( m_zoomLevel < 0 )
		iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
	if ( iCellsPerBlock < 1 )
	{
		iCellsPerBlock = 1;
		scalefactor = 2;
	}
	int iBlockWidth = (int)(RectWidth * (scalefactor * 1.0/ ( iCellsPerBlock * iPixelsPerCell )));
	int wX1 = xc - iBlockWidth / 2;
	int wX2 = xc + iBlockWidth / 2;

	if (( iBlockWidth ) && ( iBlockWidth < UO_X_BLOCKS ))
	{
		if ( wX1 < 0 )
		{
			wX1 = 0;
		}
		if ( wX2 > UO_X_BLOCKS )
		{
			wX1 -= wX2 - UO_X_BLOCKS;
		}
	}
	else if ( iBlockWidth )
	{
		int iPadding = ( iBlockWidth - UO_X_BLOCKS )/2;
		wX1 = 0 - iPadding;
	}

	int xOffset = x - rcMap.CenterPoint().x;
	// From the zoom level, we should know how many pixels per cell and cells per block we are using
	int factor = iPixelsPerCell * 8 * scalefactor / iCellsPerBlock;
	if ( m_zoomLevel >= 0 )
		xOffset = x / factor;
	else
		xOffset = x * factor;
	x = wX1 * 8 + xOffset;
	//x = m_xCenter + xOffset;
	return x;
}

short CUOMap::CtrlToMapY(short y)
{
	// Convert from the control reference frame to map reference frame
	CRect rcMap;
	GetClientRect(&rcMap);
	int RectHeight = rcMap.Height() - rcMap.Height() % 8;
	int yc = m_yCenter / 8;
	int scalefactor = 1;
	int iCellsPerBlock = 8;
	int iPixelsPerCell = 1;
	if ( m_zoomLevel > 0 )
		iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
	if ( m_zoomLevel < 0 )
		iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
	if ( iCellsPerBlock < 1 )
	{
		iCellsPerBlock = 1;
		scalefactor = 2;
	}
	int iBlockHeight = (int)(RectHeight * (scalefactor * 1.0 / ( iCellsPerBlock * iPixelsPerCell )));
	int wY1 = yc - iBlockHeight / 2;
	int wY2 = yc + iBlockHeight / 2;

	if (( iBlockHeight ) && ( iBlockHeight < UO_Y_BLOCKS ))
	{
		if ( wY1 < 0 )
		{
			wY1 = 0;
		}
		if ( wY2 > UO_Y_BLOCKS )
		{
			wY1 -= wY2 - UO_Y_BLOCKS;
		}
	}
	else if ( iBlockHeight )
	{
		int iPadding = ( iBlockHeight - UO_Y_BLOCKS )/2;
		wY1 = 0 - iPadding;
	}

	int yOffset = y - rcMap.CenterPoint().y;
	// From the zoom level, we should know how many pixels per cell and cells per block we are using
	int factor = iPixelsPerCell * 8 * scalefactor / iCellsPerBlock;
	if ( m_zoomLevel >= 0 )
		yOffset = y / factor;
	else
		yOffset = y * factor;
	y = wY1 * 8 + yOffset;
	//y = m_yCenter + yOffset;
	return y;
}

short CUOMap::MapToCtrlX(short x)
{
	// Convert from map reference frame to control reference frame
	CRect rcMap;
	GetClientRect(&rcMap);
	int RectWidth = rcMap.Width() - rcMap.Width() % 8;
	int xc = m_xCenter / 8;
	// From the zoom level, we should know how many pixels per cell and cells per block we are using
	int scalefactor = 1;
	int iCellsPerBlock = 8;
	int iPixelsPerCell = 1;
	if ( m_zoomLevel > 0 )
		iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
	if ( m_zoomLevel < 0 )
		iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
	if ( iCellsPerBlock < 1 )
	{
		iCellsPerBlock = 1;
		scalefactor = 2;
	}
	int iBlockWidth = (int)(RectWidth * (scalefactor * 1.0/ ( iCellsPerBlock * iPixelsPerCell )));
	int wX1 = xc - iBlockWidth / 2;
	int xOffset = x - (wX1 * 8);
	int factor = iPixelsPerCell * 8 * scalefactor / iCellsPerBlock;
	if ( m_zoomLevel > 0 )
		xOffset *= factor;
	else if ( m_zoomLevel <= 0 )
		xOffset /= factor;
	return xOffset;
}

short CUOMap::MapToCtrlY(short y)
{
	// Convert from map reference frame to control reference frame
	CRect rcMap;
	GetClientRect(&rcMap);
	int RectHeight = rcMap.Height() - rcMap.Height() % 8;
	int yc = m_yCenter / 8;
	// From the zoom level, we should know how many pixels per cell and cells per block we are using
	int scalefactor = 1;
	int iCellsPerBlock = 8;
	int iPixelsPerCell = 1;
	if ( m_zoomLevel > 0 )
		iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
	if ( m_zoomLevel < 0 )
		iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
	if ( iCellsPerBlock < 1 )
	{
		iCellsPerBlock = 1;
		scalefactor = 2;
	}
	int iBlockHeight = (int)(RectHeight * (scalefactor * 1.0 / ( iCellsPerBlock * iPixelsPerCell )));
	int wY1 = yc - iBlockHeight / 2;
	int yOffset = y - (wY1 * 8);
	int factor = iPixelsPerCell * 8 * scalefactor / iCellsPerBlock;
	if ( m_zoomLevel > 0 )
		yOffset *= factor;
	else if ( m_zoomLevel <= 0 )
		yOffset /= factor;
	return yOffset;
}

void CUOMap::GetDisplaySize(short* x, short* y)
{
	*x = GetDisplayWidth();
	*y = GetDisplayHeight();
}

short CUOMap::GetDisplayWidth()
{

	int iCellsPerBlock = 8;
	int iPixelsPerCell = 1;
	if ( m_zoomLevel > 0 )
		iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
	if ( m_zoomLevel < 0 )
		iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
	if ( iCellsPerBlock < 1 )
		iCellsPerBlock = 1;
	int iOffset = (GetWidth() % 16)/8;
	int xSize = (UO_X_BLOCKS * iCellsPerBlock * iPixelsPerCell)-(iOffset * iCellsPerBlock * iPixelsPerCell);
	return xSize;
}

short CUOMap::GetDisplayHeight()
{
	int iCellsPerBlock = 8;
	int iPixelsPerCell = 1;
	if ( m_zoomLevel > 0 )
		iPixelsPerCell = (int)pow((double)2, m_zoomLevel);
	if ( m_zoomLevel < 0 )
		iCellsPerBlock = 8 / ( (int)pow((double)2, abs(m_zoomLevel)));
	if ( iCellsPerBlock < 1 )
		iCellsPerBlock = 1;
	int iOffset = (GetHeight() % 16)/8;
	int ySize = (UO_Y_BLOCKS * iCellsPerBlock * iPixelsPerCell)-(iOffset * iCellsPerBlock * iPixelsPerCell);
	return ySize;
}

///////////////////////////////////////////////////////////
//DrawObjects commands

long CUOMap::AddDrawObject(short x, short y, short type, short size, long color)
{
	// Verify that the values are good
	if ( x < 0 || y < 0 || type < 0 || type > 255 || size < 0 || size > 255 || color < 0 || color > 0x00FFFFFF )
		return -1;
	CDrawObject * pObject = new CDrawObject;
	pObject->m_pt.x = x;
	pObject->m_pt.y = y;
	pObject->m_bDrawType = (BYTE) type;
	pObject->m_bSize = (BYTE) size;
	pObject->m_dwColor = (DWORD) color;
	INT_PTR idx = m_aDrawObjects.Add((CObject*) pObject);
	return (long)idx;
}

BOOL CUOMap::RemoveDrawObject(short x, short y, short type, short size, long color)
{
	if ( x < 0 || y < 0 || type < 0 || type > 255 || size < 0 || size > 255 || color < 0 || color > 0x00FFFFFF )
		return FALSE;
	for ( int i = 0; i < m_aDrawObjects.GetSize(); i++ )
	{
		CDrawObject * pObject = (CDrawObject *) m_aDrawObjects.GetAt(i);
		if ( !pObject )
			continue;
		if ( pObject->m_pt.x == x && pObject->m_pt.y == y && pObject->m_bSize == (BYTE) size && pObject->m_bDrawType == (BYTE) type && pObject->m_dwColor == (DWORD) color )
		{
			delete pObject;
			m_aDrawObjects.RemoveAt(i);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CUOMap::RemoveDrawObjectAt(long index)
{
	if ( index < 0 || index > m_aDrawObjects.GetUpperBound() )
		return FALSE;
	CDrawObject * pObject = (CDrawObject *) m_aDrawObjects.GetAt(index);
	if ( !pObject )
		return FALSE;
	delete pObject;
	m_aDrawObjects.RemoveAt(index);
	return TRUE;
}

void CUOMap::RemoveDrawObjects()
{
	for ( int i = 0; i < m_aDrawObjects.GetSize(); i++ )
	{
		CDrawObject * pObject = (CDrawObject *) m_aDrawObjects.GetAt(i);
		if ( pObject )
			delete pObject;
	}
	m_aDrawObjects.RemoveAll();
	return;
}

///////////////////////////////////////////////////////////
//DrawRect commands

long CUOMap::AddDrawRect(short xleft, short ytop, short width, short height, short mode, long color)
{
	// Validate the data
	if ( xleft < 0 || ytop < 0 || width < 0 || height < 0 || mode < 0 || color < 0 || color > 0x00FFFFFF )
		return -1;
	CDrawRect * pRect = new CDrawRect;
	pRect->m_rect.SetRect(xleft, ytop, xleft + width, ytop + height);
	pRect->m_bDrawMode = (BYTE) mode;
	pRect->m_dwColor = (DWORD) color;
	INT_PTR index = m_aDrawRects.Add((CObject *) pRect);
	return (long)index;
}

BOOL CUOMap::RemoveDrawRect(short xleft, short ytop, short width, short height, short mode, long color)
{
	if ( xleft < 0 || ytop < 0 || width < 0 || height < 0 || mode < 0 || color < 0 || color > 0x00FFFFFF )
		return FALSE;
	for (int i = 0; i < m_aDrawRects.GetSize(); i++)
	{
		CDrawRect * pRect = (CDrawRect *) m_aDrawRects.GetAt(i);
		if ( !pRect )
			continue;
		if ( pRect->m_rect.left == xleft && pRect->m_rect.top == ytop && pRect->m_rect.Width() == width && pRect->m_rect.Height() == height
			&& pRect->m_bDrawMode == (BYTE) mode && pRect->m_dwColor == (DWORD) color )
		{
			delete pRect;
			m_aDrawRects.RemoveAt(i);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CUOMap::RemoveDrawRectAt(long index)
{
	if ( index < 0 || index > m_aDrawRects.GetUpperBound() )
		return FALSE;
	CDrawRect * pRect = (CDrawRect *) m_aDrawRects.GetAt(index);
	if ( !pRect )
		return FALSE;
	delete pRect;
	m_aDrawRects.RemoveAt(index);
	return TRUE;
}

void CUOMap::RemoveDrawRects()
{
	for ( int i = 0; i < m_aDrawRects.GetSize(); i++ )
	{
		CDrawRect * pRect = (CDrawRect *) m_aDrawRects.GetAt(i);
		if ( pRect )
			delete pRect;
	}
	m_aDrawRects.RemoveAll();
	return;
}

////////////////////////////////////////////
//Patches

BOOL CUOMap::LoadPatchHash(CString sFile, CPtrArray &array)
{
	CleanHashArray(array);
	CFile file;
	if ( !file.Open(sFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		return false;
	DWORD i = 0;
	while ( TRUE )
	{
		DWORD dwIndex;
		if ( ! file.Read(&dwIndex, sizeof(DWORD)) )
			break;
		InsertHash(array, dwIndex, i);
		i++;
	}
	file.Close();	
	return true;
}

void CUOMap::CleanHashArray(CPtrArray &array)
{
	for ( int i = 0; i < array.GetSize(); i++ )
	{
		PatchHash * pHash = (PatchHash*) array.GetAt(i);
		if ( pHash )
			delete pHash;
	}
	array.RemoveAll();
}

void CUOMap::InsertHash(CPtrArray &array, DWORD lookup, DWORD index)
{

	PatchHash * pNewHash = new PatchHash;
	pNewHash->dwIndex = index;
	pNewHash->dwLookup = lookup;

	INT_PTR iLower = 0;
	INT_PTR iUpper = array.GetUpperBound();
	if ( iUpper == -1 )
	{
		array.InsertAt(0, pNewHash);
		return;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		PatchHash * pTest = (PatchHash *) array.GetAt(iIndex);
		if ( pTest->dwLookup > pNewHash->dwLookup )
		{
			iCompare = 0;
			iUpper = iIndex - 1;
		}
		else
		{
			if ( pTest->dwLookup == pNewHash->dwLookup )
			{
				// Who came up with this brilliant scheme?
				delete pTest;
				array.SetAt(iIndex, pNewHash);
				return;
			}
			iCompare = 1;
			iLower = iIndex + 1;
		}
	}
	iIndex += iCompare;
	array.InsertAt(iIndex, (void *) pNewHash);
	return;
}

DWORD CUOMap::FindPatch(CPtrArray &array, DWORD lookup)
{
	if ( array.GetSize() == 0 )
		return 0xFFFFFFFF;
	INT_PTR iLower = 0;
	INT_PTR iUpper = array.GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		PatchHash * pTest = (PatchHash *) array.GetAt(iIndex);
		if ( pTest->dwLookup == lookup )
			return pTest->dwIndex;
		if ( pTest->dwLookup > lookup )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return 0xFFFFFFFF;
}
