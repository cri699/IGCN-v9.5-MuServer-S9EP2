//////////////////////////////////////////////////////////////////////
// PacketEncrypt.cpp
// SimpleModulus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PacketEncrypt.h"

CPacketEncrypt g_PacketEncrypt;

CPacketEncrypt::CPacketEncrypt()
{ 

}

CPacketEncrypt::~CPacketEncrypt()
{
	return;
}

char sKeys[] = "bq6uy34gth7yujkw56hrgq135y5yijyuk6yt56yjknaeraghyulo.iujhw4T4SY5IIKIUKJEW4567KMHGK";
DWORD* KEYS = (DWORD*)sKeys;

inline DWORD rol(DWORD x,BYTE shift)
{
	return (x << shift) | (x >> (sizeof(x)*CHAR_BIT - shift));
}

inline DWORD ror(DWORD x,BYTE shift)
{
	return (x >> shift) | (x << (sizeof(x)*CHAR_BIT - shift));
}

void CPacketEncrypt::DecryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo)
{
	LPDWORD lpdwKey = &KEYS[42];

	int DEC2 = lpdwFrom[1];
	int DEC4 = lpdwFrom[3];
	int DEC1 = lpdwFrom[0]-lpdwKey[0];
	int DEC3 = lpdwFrom[2]-lpdwKey[1];
	int tmpDEC1;
	int tmpDEC4;

	lpdwKey -=2;
	int loopcount = 20;

	do
	{
		tmpDEC1 = DEC1;
		DEC1 = DEC4-lpdwKey[0];
		DEC4 = DEC3;
		DEC3 = DEC2-lpdwKey[1];
		lpdwKey-=2;

		DEC2 = (tmpDEC1+tmpDEC1+1)*tmpDEC1;
		tmpDEC4 = (DEC4+DEC4+1)*DEC4;

		tmpDEC4 = rol(tmpDEC4,5);
		DEC2 = rol(DEC2,5);

		DEC3 = ror(DEC3,DEC2&0xFF);
		DEC1 = ror(DEC1,tmpDEC4&0xFF);

		DEC3 ^= tmpDEC4;
		DEC1 ^= DEC2;

		DEC2 = tmpDEC1;
		loopcount--;
	}while(loopcount != 0 );

	DEC4 -= lpdwKey[1];
	DEC2 -= lpdwKey[0];

	lpdwTo[0] = DEC1;
	lpdwTo[1] = DEC2;
	lpdwTo[2] = DEC3;
	lpdwTo[3] = DEC4;
}

void CPacketEncrypt::EncryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo)
{
	LPDWORD lpdwKey = &KEYS[0];

	int tmpENC3 = lpdwFrom[0];
	int ENC3 = lpdwFrom[2];
	int ENC2 = lpdwKey[0]+lpdwFrom[1];
	int ENC4 = lpdwKey[1]+lpdwFrom[3];
	int tmpENC2;
	int tmpENC4;
	int ENC1;

	lpdwKey+= 2;
	int loopcount = 20;
	int esp1c = ENC3;

	do
	{
		tmpENC2 = (ENC2+ENC2+1)*ENC2;
		tmpENC4 = (ENC4+ENC4+1)*ENC4;

		tmpENC2 = rol(tmpENC2,5);
		tmpENC4 = rol(tmpENC4,5);

		ENC3 = tmpENC2;
		ENC3 ^= tmpENC3;
		ENC3 = rol(ENC3,tmpENC4&0xFF);

		tmpENC4 ^= esp1c;
		tmpENC4 = rol(tmpENC4,tmpENC2&0xFF);

		esp1c = ENC4;

		ENC4 = lpdwKey[0];

		tmpENC4 += lpdwKey[1];
		tmpENC3 = ENC2;

		ENC4 += ENC3;

		lpdwKey+=2;
		loopcount--;
		ENC2 = tmpENC4;
	}
	while(loopcount != 0);

	ENC3 = esp1c;
	ENC3 += lpdwKey[1];

	ENC1 = lpdwKey[0];
	ENC1 += tmpENC3;

	lpdwTo[0] = ENC1;
	lpdwTo[1] = ENC2;
	lpdwTo[2] = ENC3;
	lpdwTo[3] = ENC4;
}

int CPacketEncrypt::Decrypt(void* lpDest,void* lpSource,int iSize)
{
	if ( lpDest == NULL)
	{
		return iSize;
	}

	LPBYTE lpTempDest = (LPBYTE)lpDest;
	LPBYTE lpTempSrc = (LPBYTE)lpSource;

	int iResult = 0;
	int iDecLen = 0;

	BYTE btSizeDiff = lpTempSrc[iSize-1];

	if ( iSize > 0 )
	{
		int lOfs = 0;
		do
		{
			this->DecryptBlock((LPDWORD)&lpTempSrc[lOfs],(LPDWORD)&lpTempDest[lOfs]);
			lOfs += 16;
		}
		while( lOfs < iSize );
	}
return (iSize-btSizeDiff);
}

int CPacketEncrypt::Encrypt(void* lpDest,void* lpSource,int iSize)
{
	if ( lpDest == NULL)
	{
		return iSize;
	}

	LPBYTE lpTempDest = (LPBYTE)lpDest;
	LPBYTE lpTempSrc = (LPBYTE)lpSource;

	int iResult = 0;
	int iDecLen = 0;
	int lOfs = 0;

	if ( iSize > 0 )
	{
		do
		{
			this->EncryptBlock((LPDWORD)&lpTempSrc[lOfs],(LPDWORD)&lpTempDest[lOfs]);
			lOfs += 16;
		}
		while( lOfs < iSize );
		lpTempDest[lOfs] = lOfs-iSize;
		lOfs++;
	}

	return lOfs;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

