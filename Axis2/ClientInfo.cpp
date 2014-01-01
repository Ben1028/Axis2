/******************************************************************************\
* 
*  Copyright (C) 2006 Benoit Croussette
*
*  Code for client info based on code from:
*  UOCH - NecroToolz 
*  By:
*  Copyright (C) 2004 Daniel 'Necr0Potenc3' Cavalcanti
* 
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
* 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
* 
* 
\******************************************************************************/


// ClientInfo.cpp: implementation of the CClientInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Axis2.h"
#include "ClientInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned char *ClientBuf = NULL;
unsigned int ClientSize = 0;
unsigned int ImageBase;
unsigned int EntryPoint = 0;
unsigned int TimeDateStamp = 0;

//client version
int FirstID = 0, MiddleID = 0, ThirdID = 0, FourthID = 0;
unsigned int StringIDPos = 0, VersionPos = -1, VersionpushPos = -1;
char VersionSig[11] = "%d.%d.%d%s";
CString ClientVersion = CMsg("IDS_UNKNOWN");



void CClientInfo::SetClientBuffer()
// Copy the client to a buffer
{

	IMAGE_DOS_HEADER *idh = NULL;
	IMAGE_FILE_HEADER *ifh = NULL;
	IMAGE_OPTIONAL_HEADER *ioh = NULL;

	HANDLE ClientHandle = NULL;
	DWORD BytesRead = 0;

	ClientHandle = CreateFile(csUOPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
	if(ClientHandle == INVALID_HANDLE_VALUE)
	{
		Main->m_log.Add(1,CMsg("IDS_CLIENT_NOTOPEN"), csUOPath);
		return;
	}

	ClientSize = GetFileSize(ClientHandle, NULL);
	ClientBuf = (unsigned char *)malloc(ClientSize);
	if(ClientBuf == NULL)
	{
		Main->m_log.Add(1,CMsg("IDS_CLIENT_NOMEMORY"), ClientSize, ClientHandle);
		CloseHandle(ClientHandle);
		return;
	}

	ReadFile(ClientHandle, ClientBuf, ClientSize, &BytesRead, NULL);
	CloseHandle(ClientHandle);
	
	// find the imagebase (virtual address of the module in memory when loaded)
	// and the entrypoint
	
	idh = (IMAGE_DOS_HEADER *)ClientBuf;
	ifh = (IMAGE_FILE_HEADER *)(ClientBuf + idh->e_lfanew + sizeof(IMAGE_NT_SIGNATURE));
	ioh = (IMAGE_OPTIONAL_HEADER *)((ClientBuf + idh->e_lfanew + sizeof(IMAGE_NT_SIGNATURE)) + sizeof(IMAGE_FILE_HEADER));

	if((*(DWORD*)(ClientBuf + idh->e_lfanew)) != IMAGE_NT_SIGNATURE)
	{
		Main->m_log.Add(1,CMsg("IDS_CLIENT_NOVALID"));
		PostQuitMessage(0);
		return;
	}

	TimeDateStamp = ifh->TimeDateStamp;
	ImageBase = ioh->ImageBase;
	EntryPoint = ioh->AddressOfEntryPoint;

	return;
}


void CClientInfo::DeleteClientBuffer()
{
	free(ClientBuf);
	ClientBuf = NULL;
	return;
}

CString CClientInfo::GetClientVersion()
//Client Version
{
	
	GetClientRefVerion();
	return ClientVersion;
}

int CClientInfo::GetClientVersion(CString *fileinfovalue)
{
  DWORD whandle;
  DWORD rsize;
  int retval;

  rsize=GetFileVersionInfoSize(csUOPath, &whandle);
  if (rsize>0)
  {
    unsigned char *buffer = new unsigned char[rsize];
    if (GetFileVersionInfo(csUOPath, whandle, rsize, buffer))
    {
      unsigned short *subBlock;
      unsigned int len = 0;
      if (VerQueryValue(buffer, "\\VarFileInfo\\Translation", (void **)&subBlock, &len))
      {
        static char spv[256];
        char *versionInfo;
        len=0;
		sprintf_s(spv,256,"\\StringFileInfo\\%04x%04x\\ProductVersion", subBlock[0], subBlock[1]);
        if (VerQueryValue(buffer, spv, (void **)&versionInfo, &len))
        {
          *fileinfovalue=versionInfo;
          retval=0;
        }
        else
          retval=1;
      }
      else
        retval=2;
    }
    else
      retval=3;
	delete buffer;
  }
  else
    retval=4;

  return retval;
}

void CClientInfo::GetClientRefVerion()
{
		VersionPos = FleXSearch((BYTE*)"1.25.", ClientBuf, 5, ClientSize, 0, 0x100, 1);
	if(VersionPos == -1)
		VersionPos = FleXSearch((BYTE*)"1.26.", ClientBuf, 5, ClientSize, 0, 0x100, 1);
	if(VersionPos == -1)
		VersionPos = FleXSearch((BYTE*)"2.0.", ClientBuf, 4, ClientSize, 0, 0x100, 1);

	if(VersionPos != -1)
		ClientVersion = (const char*)(ClientBuf + VersionPos);
	else
	{
		VersionpushPos = GetTextRef(ClientBuf, ClientSize, ImageBase, VersionSig, TRUE, 1);

		if((int)VersionpushPos < 0)
		{
			if(GetClientVersion(&ClientVersion)>0)
				Main->m_log.Add(1,CMsg("IDS_CLIENT_NOREF"), VersionSig, VersionpushPos);
		}
		else
		{
			/* copy the a.b.c */
			memcpy(&FirstID, (void*)(ClientBuf + VersionpushPos - 1), 1);
			memcpy(&MiddleID, (void*)(ClientBuf + VersionpushPos - 3), 1);
			memcpy(&ThirdID, (void*)(ClientBuf + VersionpushPos - 5), 1);
			/* copy the string's offset	*/
			memcpy(&StringIDPos, (void*)(ClientBuf + VersionpushPos - 10), 4);
			StringIDPos -= ImageBase;
			ClientVersion.Format( "%d.%d.%d%s", FirstID, MiddleID, ThirdID, ClientBuf + StringIDPos );
		}
	}
	return;
}

int CClientInfo::GetClientKey1()
//Login key1
{
	unsigned int KeysPos = GetClientKeyPos(), LKey1 = 0;
	unsigned int CryptPos = GetClientCryptPos();

	if(KeysPos != -1 && CryptPos != -1)
		memcpy(&LKey1, ClientBuf + (KeysPos - 4), sizeof(unsigned int));

	return LKey1;
}

int CClientInfo::GetClientKey2()
//Login key2
{
	unsigned int KeysPos = GetClientKeyPos(), LKey2 = 0;
	unsigned int CryptPos = GetClientCryptPos();

	if(KeysPos != -1 && CryptPos != -1)
		memcpy(&LKey2, ClientBuf + (KeysPos + 2), sizeof(unsigned int));

	return LKey2;
}

int CClientInfo::GetClientKeyPos()
//Login key signature
{
	unsigned char KeysSig[14] = { 0x81, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x4D, 0x89, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x89 };
	unsigned int KeysPos = -1;
	unsigned int CryptPos = GetClientCryptPos();
	if(CryptPos != -1)
		KeysPos = FleXSearch(KeysSig, ClientBuf, sizeof(KeysSig), ClientSize, CryptPos, 0xCC, 1);
		
	return KeysPos;
}

int CClientInfo::GetClientCryptPos()
//Login encryption signature
{
	unsigned char CryptSig[8] = { 0x81, 0xF9, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x8F };
    unsigned int CryptPos = -1;
	CryptPos = FleXSearch(CryptSig, ClientBuf, sizeof(CryptSig), ClientSize, 0, 0x100, 1);
	return CryptPos;
}

int CClientInfo::GetClientCryptFuncPos()
//Login encryption function
{
    unsigned int CryptPos = GetClientCryptPos(), CryptfuncPos = -1;
	if(CryptPos != -1)
		CryptfuncPos = GetFunctionByRef(ClientBuf, ClientSize, CryptPos);
		
	return CryptfuncPos;
}

int CClientInfo::GetClientSize()
//Client executable size
{
	return ClientSize;
}

int CClientInfo::GetClientTimeStamp()
//Client time date stamp
{
	return TimeDateStamp;
}

int CClientInfo::GetClientImage()
//Client image base
{
	return ImageBase;
}

int CClientInfo::GetClientEntryP()
//Client entry point
{
	return EntryPoint;
}



int FleXSearch(BYTE *Src, BYTE *Buf, DWORD SrcSize, DWORD BufSize, DWORD StartAt, int FlexByte, DWORD Which)
{
    DWORD Count = 0, i = 0, j = 0;
	BOOL UseFlex = FlexByte & 100;
	BYTE FByte = FlexByte & 0xff;
    
    for(i = StartAt; i < BufSize - SrcSize; i++)
    {
        /* check if we can read src_size bytes from this location */
        if(IsBadReadPtr((void*)(Buf + 1), SrcSize))
            return 0;
            
		/* compare src_size from src against src_size bytes from buf */
		for(j = 0; j < SrcSize; j++)
		{
			/* if there's a difference and this isn't the flex_byte, move on */
			if((UseFlex && Src[j] != FByte) && Src[j] != Buf[i + j])
				break;
			else if(!UseFlex && Src[j] != Buf[i + j])
				break;

			/* if its the last byte for comparison, everything matched */
			if(j == (SrcSize - 1))
			{
				Count++;
				if(Count >= Which)
					return i;
			}
		}
  }
	/* if it got this far, then couldnt find it */
	return -1;
}


int GetFunctionByRef(BYTE *Buf, DWORD BufSize, DWORD PosInFunction)
{
	DWORD i = 0;

	/* go down from where we are in the buffer till the beginning */
	for(i = PosInFunction; i > 0; i--)
	{
		/* search for a CALL <this position> */
		if(GetFunctionRef(Buf, BufSize, i, 1) != -1)
			return i;
	}

	return -1;
}


int GetFunctionRef(BYTE *Buf, DWORD BufSize, DWORD Function, DWORD Which)
{
	DWORD Count = 0, Call = 0, i = 0;
	char Ref[5] = { (char)0xe8, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };

	for(i = 0; i < BufSize - sizeof(Ref); i++)
    {
		/* if this doesn't even look like a call, skip */
		if(Buf[i] != 0xe8)
			continue;

		/* search for CALL (to-from-5) */
        Call = Function - i - 5;
		memcpy(Ref + 1, &Call, sizeof(DWORD));
		if(!memcmp(Buf + i, Ref, 5))
		{
			Count++;
			if(Count >= Which)
				return i;
		}
	}

	return -1;
}


int GetTextRef(BYTE *Buf, DWORD BufSize, DWORD ImageBase, const char *Text, BOOL Exact, int Which)
{
	UNREFERENCED_PARAMETER(Which);
	char PushText[5] = { 0x68, 0x00, 0x00, 0x00, 0x00 };
	unsigned int TextPos = 0, TextPush = 0;

	/* find the text's position, if Exact is true, it will include the null char in the search */
	TextPos = ImageBase + FleXSearch((BYTE*)Text, Buf, (DWORD)(strlen(Text) + Exact), BufSize, 0, 0x100, 1);
	if(TextPos == ImageBase - 1)
		return -2;

	/* find PUSH <offset of text> */
	memcpy(PushText + 1, &TextPos, sizeof(TextPos));
	TextPush = FleXSearch((BYTE*)PushText, Buf, sizeof(PushText), BufSize, 0, 0x100, 1);

	/* returns -1 if not found */
	return TextPush;
}