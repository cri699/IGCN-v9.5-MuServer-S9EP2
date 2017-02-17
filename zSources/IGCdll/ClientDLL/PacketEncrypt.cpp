//////////////////////////////////////////////////////////////////////
// PacketEncrypt.cpp
// SimpleModulus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PacketEncrypt.h"

static BYTE btKey[32] = {0x7A, 0x2C, 0x74, 0x6D, 0xB5, 0x4F, 0xF7, 0xAF, 0x4A, 0x18, 0x8D, 0x94, 0x7A, 0xE4, 0x71, 0x01, 0x44, 0x19, 0xE6, 0x83, 0x68, 0x46, 0x86, 0xDB, 0xBE, 0x6D, 0xD9, 0x9C, 0x8C, 0x3C, 0x08, 0x40 };
CPacketEncrypt g_PacketEncrypt;

CPacketEncrypt::CPacketEncrypt()
{ 
	this->enc.SetKey(btKey, 32);
	this->dec.SetKey(btKey, 32);
}

CPacketEncrypt::~CPacketEncrypt()
{
	return;
}

int CPacketEncrypt::Decrypt(BYTE * lpDest, BYTE * lpSource, int iSize)
{
	
	int remainder = iSize % this->dec.BLOCKSIZE;
	int outLen = 0;

	if (remainder == 1)
	{
		int paddingSize = lpSource[iSize - 1];
		outLen = iSize - paddingSize - 1;

		if (lpDest != NULL)
		{
			if (outLen > 0 && outLen <= 4096)
			{
				BYTE obuf[4096];
				BYTE ibuf[4096];

				memset(ibuf, 0, iSize);
				memset(obuf, 0, iSize);

				memcpy(ibuf, lpSource, iSize);

				for (int i = 0; i < outLen; i += this->dec.BLOCKSIZE)
				{
					this->dec.ProcessBlock(&ibuf[i], &obuf[i]);
				}

				memcpy(lpDest, obuf, outLen);
			}
		}

		return outLen;
	}
       
	return -1;
	
}

int CPacketEncrypt::Encrypt(BYTE * lpDest, BYTE * lpSource, int iSize)
{
	
	int paddingSize = 0;
	int remainder = iSize % this->enc.BLOCKSIZE;

	if (remainder != 0)
	{
		paddingSize = this->enc.BLOCKSIZE - remainder;
	}

	int outLen = iSize + paddingSize + 1;

	if (lpDest != NULL)
	{
		BYTE padding[64];
		BYTE obuf[4096];
		BYTE tbuf[4096];

		memset(padding, 0, sizeof(padding));
		memset(tbuf, 0, outLen);
		memset(obuf, 0, outLen);

		memcpy(tbuf, lpSource, iSize);
		memcpy(&tbuf[iSize], padding, paddingSize);

		for (int i = 0; i < outLen; i += this->enc.BLOCKSIZE)
		{
			this->enc.ProcessBlock(&tbuf[i], &obuf[i]);
		}

		obuf[outLen - 1] = paddingSize;
		memcpy(lpDest, obuf, outLen);
	}

	return outLen;
	
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

