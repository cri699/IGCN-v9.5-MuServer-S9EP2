// SimpleModulus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleModulus.h"

#define EX700 1

#if EX700 == 1
//#pragma warning(disable : 4244)

// SimpleModulus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleModulus.h"

DWORD CSimpleModulus::s_dwSaveLoadXOR[4]={0x3F08A79B, 0xE25CC287, 0x93D27AB9, 0x20DEA7BF};

CSimpleModulus::CSimpleModulus()	// Completed
{ 
	this->Init(); 
}

CSimpleModulus::~CSimpleModulus()	// Completed
{
	return;
}

VOID CSimpleModulus::Init()	// Completed
{
	memset(m_ProtocolFix,0,sizeof(m_ProtocolFix));
	memset(this->m_dwEncryptionKey, 0, sizeof(this->m_dwEncryptionKey));
	memset(this->m_dwModulus, 0, sizeof(this->m_dwModulus));
	memset(this->m_dwDecryptionKey, 0, sizeof(this->m_dwDecryptionKey));

	m_ProtocolFix[0xF1] = &CSimpleModulus::FixPacket_F1;
	m_ProtocolFix[0xF3] = &CSimpleModulus::FixPacket_F3;
	m_ProtocolFix[0x0E] = &CSimpleModulus::FixPacket_0E;
	m_ProtocolFix[0x03] = &CSimpleModulus::FixPacket_03;
}

DWORD KEYS[] = {
	0xE2CB2C47,0x676CCC60,0x59C06192,0x8AB3B66D,0x23A82990,0x4DA877B9,0x254A0788,0x83EFE892,
	0xF0D21807,0x527CED5D,0x9A165576,0xB51ED51A,0x65298BF3,0x5EACAF1A,0x004F0813,0xB30F3FF9,
	0x7751A486,0x83A69183,0x94B9397C,0x0D3F9C51,0x3A3395F4,0xEBF511C5,0x954C2EB4,0x082E330B,
	0x94F0A80A,0xC661B594,0xE9744643,0x2EB9425B,0x630A3121,0x0F8758D8,0x5424534C,0xCB251A98,
	0xD422BA68,0x4A4487F9,0x6D124452,0x5A7E6FAF,0x9ADB16B7,0xFF1583DA,0x8B232CEC,0x1FC6BC3C,
	0x01FC75E5,0x39036FF6,0xDC4B0869,0xC7808981,
};

DWORD rol(DWORD x,BYTE shift)
{
	return (x << shift) | (x >> (sizeof(x)*CHAR_BIT - shift));
}

DWORD ror(DWORD x,BYTE shift)
{
	return (x >> shift) | (x << (sizeof(x)*CHAR_BIT - shift));
}

void CSimpleModulus::DecryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo)
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

void CSimpleModulus::EncryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo)
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

int CSimpleModulus::Decrypt(void* lpDest,void* lpSource,int iSize)
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

int CSimpleModulus::Encrypt(void* lpDest,void* lpSource,int iSize)
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

/*int CSimpleModulus::EncryptBlock(void*lpDest,void*lpSource,int iSize)
{
	DWORD dwEncBuffer[4];
	DWORD dwEncValue=0;	// TempVar1

	LPBYTE lpEncDest = (LPBYTE)lpDest;
	LPBYTE lpEncSource = (LPBYTE)lpSource;

	memset(lpEncDest, 0, 11);

	for ( int i=0;i<4;i++)
	{
		dwEncBuffer[i]=((this->m_dwXORKey[i]^((WORD*)lpEncSource)[i]^dwEncValue)*this->m_dwEncryptionKey[i])%this->m_dwModulus[i];
		dwEncValue=dwEncBuffer[i]&0xFFFF;
	}

	for ( i=0;i<3;i++)
	{
		dwEncBuffer[i] = dwEncBuffer[i] ^ this->m_dwXORKey[i] ^ ( dwEncBuffer[i+1] & 0xFFFF );
	}

	int iBitPos = 0;

	for ( i=0;i<4;i++)
	{
		iBitPos = this->AddBits(lpDest, iBitPos, &dwEncBuffer[i], 0, 16);
		iBitPos = this->AddBits(lpDest, iBitPos, &dwEncBuffer[i], 22, 2);
	}

	BYTE btCheckSum = 0xF8;
	
	for ( i=0;i<8;i++)
		btCheckSum ^= lpEncSource[i];

	((LPBYTE)&dwEncValue)[1] = btCheckSum ; 
	((LPBYTE)&dwEncValue)[0] = btCheckSum ^ iSize ^ 0x3D; 

	return this->AddBits(lpDest, iBitPos, &dwEncValue, 0, 16);
}

int CSimpleModulus::Encrypt(void * lpDest, void * lpSource, int iSize)	// Emulated - Completed
{
	int iTempSize = iSize;
	int iTempSize2;
	int iOriSize ;
	unsigned char * lpTempDest = (LPBYTE)lpDest;
	unsigned char * lpTempSource = (LPBYTE)lpSource;
	
	int iDec = ((iSize+7)/8);
	iSize = (iDec + iDec*4)*2 + iDec;

	if ( lpDest != NULL )
	{
		iOriSize = iTempSize;
		for ( int i=0; i<iTempSize; i+=8,iOriSize-=8,lpTempDest+=11)
		{
			iTempSize2 = iOriSize;
			if ( iOriSize >= 8 )
				iTempSize2 = 8;

			this->EncryptBlock(lpTempDest, lpTempSource+i, iTempSize2);
		}
	}

	return iSize;
}*/

int CSimpleModulus::AddBits(void*lpDest,int iDestBitPos,void*lpSource,int iBitSourcePos,int iBitLen)	// Completed Emulated
{
	// Get Buffer Lens
	int iSourceBufferBitLen = iBitLen + iBitSourcePos ;
	int iTempBufferLen = this->GetByteOfBit(iSourceBufferBitLen-1);
	iTempBufferLen += 1 - this->GetByteOfBit(iBitSourcePos);

	// Copy the Source Buffer
	unsigned char * pTempBuffer = new unsigned char[iTempBufferLen+1];
	memset(pTempBuffer, 0, iTempBufferLen+1);
	memcpy(pTempBuffer, (unsigned char *)lpSource+ this->GetByteOfBit(iBitSourcePos), iTempBufferLen);

	// Save the Last ibt if exist
	if ( (iSourceBufferBitLen%8 ) != 0 )
	{
		pTempBuffer[iTempBufferLen - 1] &= 255 << (8 - (iSourceBufferBitLen%8));
	}

	// Get the Values to Shift
	int iShiftLeft = (iBitSourcePos%8);
	int iShiftRight = (iDestBitPos%8);

	// Shift the Values to Add the right space of the desired bits
	this->Shift(pTempBuffer, iTempBufferLen, -iShiftLeft);
	this->Shift(pTempBuffer, iTempBufferLen+1, iShiftRight);

	// Copy the the bits of Source to the Dest
	int iNewTempBufferLen = (( iShiftRight <= iShiftLeft )?0:1) + iTempBufferLen;
	unsigned char * TempDist = (unsigned char *)lpDest + this->GetByteOfBit(iDestBitPos);

	for ( int i=0;i<iNewTempBufferLen;i++)
	{
		TempDist[i] |= pTempBuffer[i];
	}

	// Delete the temp Buffer
	delete [] pTempBuffer;

	// Return the number of bits of the new Dest Buffer
	return iDestBitPos + iBitLen;
}


VOID CSimpleModulus::Shift(void*lpBuff,int iSize,int ShiftLen) // Need To be Checked
{
	unsigned char * TempBuff = (unsigned char *)lpBuff;

	// Case no Shift Len
	if ( ShiftLen != 0  )	
	{
		// Shift Right
		if ( ShiftLen > 0 )
		{
			if ( (iSize -1 ) > 0 )
			{
				for ( int i=(iSize-1); i>0 ; i--)
				{
					TempBuff[i] = (TempBuff[i-1]<<((8 - ShiftLen))) | (TempBuff[i]>>ShiftLen );
				}
			}

			TempBuff[0] >>=  ShiftLen;
		}
		else	// Shift Left
		{
			ShiftLen = - ShiftLen;

			if ( (iSize-1) > 0 )
			{
				for ( int i=0; i<(iSize-1) ;i++)
				{
					TempBuff[i] = (TempBuff[i+1]>>((8 - ShiftLen))) | (TempBuff[i]<<ShiftLen );
				}
			}

			TempBuff[iSize-1] <<= ShiftLen;
		}
	}
}

int CSimpleModulus::GetByteOfBit(int btByte)
{
	return btByte >> 3;
}

BOOL CSimpleModulus::SaveAllKey(LPSTR lpszFileName )
{
	return this->SaveKey(lpszFileName, 4370, TRUE, TRUE, TRUE, TRUE);
}

BOOL CSimpleModulus::LoadAllKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, TRUE, TRUE, TRUE);
}

BOOL CSimpleModulus::SaveEncryptionKey(LPSTR lpszFileName)
{
	return this->SaveKey(lpszFileName, 4370, TRUE, TRUE, FALSE, TRUE);
}

BOOL CSimpleModulus::LoadEncryptionKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, TRUE, FALSE ,TRUE);
}

BOOL CSimpleModulus::SaveDecryptionKey(LPSTR lpszFileName)
{
	return this->SaveKey(lpszFileName, 4370, TRUE, FALSE, TRUE, TRUE);
}

BOOL CSimpleModulus::LoadDecryptionKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, FALSE ,TRUE ,TRUE);
}

BOOL CSimpleModulus::SaveKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bSaveModulus, BOOL bSaveEncKey, BOOL bSaveDecKey, BOOL bSaveXORKey)
{
	// MAde by Deathway
	ENCDEC_FILEHEADER HeaderBuffer;
	int iSize;
	DWORD XORTable[4];
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);

	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	HeaderBuffer.sFileHeader = wFileHeader;
	HeaderBuffer.dwSize = ( bSaveModulus + bSaveEncKey + bSaveDecKey + bSaveXORKey ) * sizeof(XORTable) + sizeof(ENCDEC_FILEHEADER);

	WriteFile( hFile, &HeaderBuffer, sizeof(ENCDEC_FILEHEADER), (ULONG*)&iSize, NULL);

	if ( bSaveModulus != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwModulus[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	if ( bSaveEncKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwEncryptionKey[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	if ( bSaveDecKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwDecryptionKey[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	if ( bSaveXORKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwXORKey[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}


BOOL CSimpleModulus::LoadKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bLoadModulus, BOOL bLoadEncKey, BOOL bLoadDecKey, BOOL bLoadXORKey)
{
	ENCDEC_FILEHEADER HeaderBuffer;
	int iSize;
	DWORD XORTable[4];
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);

	if ( hFile != INVALID_HANDLE_VALUE )
	{
		ReadFile( hFile, &HeaderBuffer, sizeof(ENCDEC_FILEHEADER), (ULONG*)&iSize, NULL);
	}

	if ( HeaderBuffer.sFileHeader == wFileHeader  )
	{
		if ( (HeaderBuffer.dwSize) == (int)((( bLoadModulus + bLoadEncKey + bLoadDecKey + bLoadXORKey )*sizeof(XORTable))+sizeof(ENCDEC_FILEHEADER)) )
		{
			if ( bLoadModulus != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwModulus[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			if ( bLoadEncKey != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwEncryptionKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			if ( bLoadDecKey != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwDecryptionKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			if ( bLoadXORKey != FALSE )
			{
				ReadFile(hFile, XORTable,sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwXORKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}

			CloseHandle(hFile);
			return TRUE;
		}
	}
	else
	{
		CloseHandle(hFile);
	}
	return FALSE;
}

void CSimpleModulus::PacketFix(LPBYTE lpMsg,BYTE Headcode,int iSize)
{

	if( !m_ProtocolFix[Headcode] )
		return;

	int headsize = (lpMsg[0]==0xC1) ? 3 : 4;
	int bodysize = iSize-headsize;

	m_ProtocolFix[Headcode](&lpMsg[headsize],bodysize);
}

#define UNPROTECT1(val,x1,x2)	((val-((val>>x1)<<x1))<<x2 | (val>>x1))

void CSimpleModulus::FixPacket_03(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{
		btmp1 = lpBody[1];
		lpBody[1] = lpBody[2];
		lpBody[2] = btmp1;

		btmp1 = (lpBody[0]>>3)&1;
		btmp2 = (lpBody[0]>>5)&1;

		(btmp1) ? lpBody[0] |= 32 : lpBody[0] &=~32;
		(btmp2) ? lpBody[0] |= 8 : lpBody[0] &=~8;

		lpBody[3] = (lpBody[3]<<6) | (lpBody[3]>>2);
		lpBody[2] = (lpBody[2]<<1) | (lpBody[2]>>7);
		lpBody[1] = (lpBody[1]<<6) | (lpBody[1]>>2);

		btmp1 = lpBody[1];
		lpBody[1] = lpBody[3];
		lpBody[3] = btmp1;
	}
}

void CSimpleModulus::FixPacket_0E(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{

	}
	else if( iSize < 16 )
	{
		lpBody[3] = (lpBody[3]<<5) | (lpBody[3]>>3);
		lpBody[7] = (lpBody[7]<<3) | (lpBody[7]>>5);

		btmp1 = (lpBody[3]>>3)&1;
		btmp2 = (lpBody[3]>>2)&1;

		(btmp1) ? lpBody[3] |= 4 : lpBody[3] &= ~4;
		(btmp2) ? lpBody[3] |= 8 : lpBody[3] &= ~8;

		btmp1 = lpBody[3];
		lpBody[3] = lpBody[5];
		lpBody[5] = btmp1;

		btmp1 = lpBody[7];
		lpBody[7] = lpBody[0];
		lpBody[0] = btmp1;

		lpBody[6] = (lpBody[6]<<4) | (lpBody[6]>>4);

		btmp1 = lpBody[3];
		lpBody[3] = lpBody[4];
		lpBody[4] = btmp1;

		lpBody[6] ^= 135;
	}
	else if( iSize < 32 )
	{

	}
	else
	{

	}
}

void CSimpleModulus::FixPacket_F1(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{
		btmp1 = lpBody[3];
		lpBody[3] = lpBody[0];
		lpBody[0] = btmp1;

		btmp1 = (lpBody[3]>>6)&1;
		btmp2 = (lpBody[3]>>3)&1;

		if( btmp1 )
			lpBody[3] |= 8;
		else
			lpBody[3] &= ~8;

		if( btmp2 )
			lpBody[3] |= 64;
		else
			lpBody[3] &= ~64;

		btmp1 = (lpBody[1]>>2)&1;
		btmp2 = (lpBody[1]>>5)&1;

		if( btmp1 )
			lpBody[1] |= 32;
		else
			lpBody[1] &= ~32;

		if( btmp2 )
			lpBody[1] |= 4;
		else
			lpBody[1] &= ~4;

		lpBody[0] ^= 133;

		lpBody[0] = (lpBody[0]-((lpBody[0]>>2)<<2))<<6 | (lpBody[0]>>2);
		lpBody[3] = (lpBody[3]-((lpBody[3]>>7)<<7))<<1 | (lpBody[3]>>7);
		lpBody[1] = (lpBody[1]-((lpBody[1]>>2)<<2))<<6 | (lpBody[1]>>2);

		btmp1 = (lpBody[3]>>6)&1;
		btmp2 = (lpBody[3]>>5)&1;

		if( btmp1 )
			lpBody[3] |= 32;
		else
			lpBody[3] &= ~32;

		if( btmp2 )
			lpBody[3] |= 64;
		else
			lpBody[3] &= ~64;

		lpBody[2] = (lpBody[2]-((lpBody[2]>>2)<<2))<<6 | (lpBody[2]>>2);

		btmp1 = (lpBody[2]>>7)&1;
		btmp2 = (lpBody[2]>>6)&1;

		if( btmp1 )
			lpBody[2] |= 64;
		else
			lpBody[2] &= ~64;

		if( btmp2 )
			lpBody[2] |= 128;
		else
			lpBody[2] &= ~128;

		btmp1 = (lpBody[2]>>1)&1;
		btmp2 = (lpBody[2]>>7)&1;

		if( btmp1 )
			lpBody[2] |= 128;
		else
			lpBody[2] &= ~128;

		if( btmp2 )
			lpBody[2] |= 8;
		else
			lpBody[2] &= ~8;
	}
	else
	{
		lpBody[30] = (lpBody[30]-((lpBody[30]>>6)<<6))<<2 | (lpBody[30]>>6);
		lpBody[16] ^= 103;

		btmp1 = (lpBody[16]>>3)&1;
		btmp2 = (lpBody[16]>>7)&1;

		if( btmp1 )
			lpBody[16] |= 128;
		else
			lpBody[16] &= ~128;
	
		if( btmp2 )
			lpBody[16] |= 8;
		else
			lpBody[16] &= ~8;

		lpBody[6] = (lpBody[6]-((lpBody[6]>>2)<<2))<<6 | (lpBody[6]>>2);
		lpBody[31] ^= 73;

		btmp1 = (lpBody[3]>>3)&1;
		btmp2 = (lpBody[3]>>2)&1;

		if( btmp1 )
			lpBody[3] |= 4;
		else
			lpBody[3] &= ~4;

		if( btmp2 )
			lpBody[3] |= 8;
		else
			lpBody[3] &= ~8;

		lpBody[29] ^= 228;

		btmp1 = lpBody[8];
		lpBody[8] = lpBody[16];
		lpBody[16] = btmp1;

		btmp1 = lpBody[13];
		lpBody[13] = lpBody[10];
		lpBody[10] = btmp1;
	}
}

void CSimpleModulus::FixPacket_F3(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{
		lpBody[0] = (lpBody[0]-((lpBody[0]>>4)<<4))<<4 | (lpBody[0]>>4);

		btmp1 = lpBody[1];
		lpBody[1] = lpBody[0];
		lpBody[0] = btmp1;

		lpBody[3] ^= 95;
		lpBody[3] ^= 192;
		lpBody[3] ^= 1;
		lpBody[3] ^= 75;

		btmp1 = lpBody[2];
		lpBody[2] = lpBody[0];
		lpBody[0] = btmp1;

		btmp1 = (lpBody[3]>>4)&1;
		btmp2 = (lpBody[3]>>7)&1;

		(btmp1) ? lpBody[3] |= 128 : lpBody[3] &=~128;
		(btmp2) ? lpBody[3] |= 16 : lpBody[3] &=~16;

		lpBody[2] ^= 46;
	}
	else if( iSize < 16 )
	{
		btmp1 = lpBody[7];
		lpBody[7] = lpBody[0];
		lpBody[0] = btmp1;

		lpBody[6] ^= 75;

		btmp1 = (lpBody[1]>>6)&1;
		btmp2 = (lpBody[1]>>4)&1;

		(btmp1) ? lpBody[1] |= 4 : lpBody[1] &=~4;
		(btmp2) ? lpBody[1] |= 64 : lpBody[1] &=~64;

		btmp1 = lpBody[1];
		lpBody[1] = lpBody[4];
		lpBody[4] = btmp1;

		btmp1 = (lpBody[3]>>1)&1;
		btmp2 = (lpBody[3]>>7)&1;

		(btmp1) ? lpBody[3] |= 128 : lpBody[3] &= ~128;
		(btmp2) ? lpBody[3] |= 2 : lpBody[3] &= ~2;

		btmp1 = (lpBody[0]>>3)&1;
		btmp2 = (lpBody[0]>>1)&1;

		(btmp1) ? lpBody[0] |= 2 : lpBody[0] &= ~2;
		(btmp2) ? lpBody[0] |= 8 : lpBody[0] &= ~8;

		btmp1 = lpBody[6];
		lpBody[6] = lpBody[3];
		lpBody[3] = btmp1;
	}
	else if( iSize < 32 )
	{
		btmp1 = lpBody[6];
		lpBody[6] = lpBody[2];
		lpBody[2] = btmp1;

		lpBody[4] = (lpBody[4]-((lpBody[4]>>5)<<5))<<3 | (lpBody[4]>>5);
		lpBody[6] = (lpBody[6]-((lpBody[6]>>7)<<7))<<1 | (lpBody[6]>>7);
		lpBody[14] ^= 147;

		lpBody[13] = (lpBody[13]-((lpBody[13]>>1)<<1))<<7 | (lpBody[13]>>1);
		lpBody[8] = (lpBody[8]-((lpBody[8]>>7)<<7))<<1 | (lpBody[8]>>7);
	}
	else
	{
		lpBody[30] = (lpBody[30]-((lpBody[30]>>5)<<5))<<3 | (lpBody[30]>>5);
		lpBody[31] ^= 130;
		lpBody[18] ^= 244;
		lpBody[8] = (lpBody[8]-((lpBody[8]>>5)<<5))<<3 | (lpBody[8]>>5);
	}
}

/*
void CSimpleModulus::PacketFix(LPBYTE lpMsg,BYTE Headcode,int iSize)
{

	if( !m_ProtocolFix[Headcode].Decrypt )
		return;

	int headsize = (lpMsg[0]==0xC1) ? 3 : 4;
	int bodysize = iSize-headsize;

	m_ProtocolFix[Headcode].Decrypt(&lpMsg[headsize],bodysize);
}

void CSimpleModulus::FixPacket_F1(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{
		btmp1 = lpBody[3];
		lpBody[3] = lpBody[0];
		lpBody[0] = btmp1;

		btmp1 = (lpBody[3]>>6)&1;
		btmp2 = (lpBody[3]>>3)&1;

		if( btmp1 )
			lpBody[3] |= 8;
		else
			lpBody[3] &= ~8;

		if( btmp2 )
			lpBody[3] |= 64;
		else
			lpBody[3] &= ~64;

		btmp1 = (lpBody[1]>>2)&1;
		btmp2 = (lpBody[1]>>5)&1;

		if( btmp1 )
			lpBody[1] |= 32;
		else
			lpBody[1] &= ~32;

		if( btmp2 )
			lpBody[1] |= 4;
		else
			lpBody[1] &= ~4;

		lpBody[0] ^= 133;

		lpBody[0] = (lpBody[0]-((lpBody[0]>>2)<<2))<<6 | (lpBody[0]>>2);
		lpBody[3] = (lpBody[3]-((lpBody[3]>>7)<<7))<<1 | (lpBody[3]>>7);
		lpBody[1] = (lpBody[1]-((lpBody[1]>>2)<<2))<<6 | (lpBody[1]>>2);

		btmp1 = (lpBody[3]>>6)&1;
		btmp2 = (lpBody[3]>>5)&1;

		if( btmp1 )
			lpBody[3] |= 32;
		else
			lpBody[3] &= ~32;

		if( btmp2 )
			lpBody[3] |= 64;
		else
			lpBody[3] &= ~64;

		lpBody[2] = (lpBody[2]-((lpBody[2]>>2)<<2))<<6 | (lpBody[2]>>2);

		btmp1 = (lpBody[2]>>7)&1;
		btmp2 = (lpBody[2]>>6)&1;

		if( btmp1 )
			lpBody[2] |= 64;
		else
			lpBody[2] &= ~64;

		if( btmp2 )
			lpBody[2] |= 128;
		else
			lpBody[2] &= ~128;

		btmp1 = (lpBody[2]>>1)&1;
		btmp2 = (lpBody[2]>>7)&1;

		if( btmp1 )
			lpBody[2] |= 128;
		else
			lpBody[2] &= ~128;

		if( btmp2 )
			lpBody[2] |= 8;
		else
			lpBody[2] &= ~8;
	}
	else
	{
		lpBody[30] = (lpBody[30]-((lpBody[30]>>6)<<6))<<2 | (lpBody[30]>>6);
		lpBody[16] ^= 103;

		btmp1 = (lpBody[16]>>3)&1;
		btmp2 = (lpBody[16]>>7)&1;

		if( btmp1 )
			lpBody[16] |= 128;
		else
			lpBody[16] &= ~128;
	
		if( btmp2 )
			lpBody[16] |= 8;
		else
			lpBody[16] &= ~8;

		lpBody[6] = (lpBody[6]-((lpBody[6]>>2)<<2))<<6 | (lpBody[6]>>2);
		lpBody[31] ^= 73;

		btmp1 = (lpBody[3]>>3)&1;
		btmp2 = (lpBody[3]>>2)&1;

		if( btmp1 )
			lpBody[3] |= 4;
		else
			lpBody[3] &= ~4;

		if( btmp2 )
			lpBody[3] |= 8;
		else
			lpBody[3] &= ~8;

		lpBody[29] ^= 228;

		btmp1 = lpBody[8];
		lpBody[8] = lpBody[16];
		lpBody[16] = btmp1;

		btmp1 = lpBody[13];
		lpBody[13] = lpBody[10];
		lpBody[10] = btmp1;
	}
}

void CSimpleModulus::FixPacket_F3(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{

	}
	else if( iSize < 16 )
	{
		btmp1 = lpBody[7];
		lpBody[7] = lpBody[0];
		lpBody[0] = btmp1;

		lpBody[6] ^= 75;

		btmp1 = (lpBody[1]>>6)&1;
		btmp2 = (lpBody[1]>>4)&1;

		if( btmp1 )
			lpBody[1] |= 16;
		else
			lpBody[1] &= ~16;

		if( btmp2 )
			lpBody[1] |= 64;
		else
			lpBody[1] &= ~64;

		btmp1 = lpBody[1];
		lpBody[1] = lpBody[4];
		lpBody[4] = btmp1;

		btmp1 = (lpBody[3]>>1)&1;
		btmp2 = (lpBody[3]>>7)&1;

		if( btmp1 )
			lpBody[3] |= 128;
		else
			lpBody[3] &= ~128;

		if( btmp2 )
			lpBody[3] |= 2;
		else
			lpBody[3] &= ~2;

		btmp1 = (lpBody[0]>>3)&1;
		btmp2 = (lpBody[0]>>1)&1;

		if( btmp1 )
			lpBody[0] |= 2;
		else
			lpBody[0] &= ~2;

		if( btmp2 )
			lpBody[0] |= 8;
		else
			lpBody[0] &= ~8;

		btmp1 = lpBody[6];
		lpBody[6] = lpBody[3];
		lpBody[3] = btmp1;
	}
}*/

#else

// SimpleModulus.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleModulus.h"

DWORD CSimpleModulus::s_dwSaveLoadXOR[4]={0x3F08A79B, 0xE25CC287, 0x93D27AB9, 0x20DEA7BF};

//#pragma warning(disable : 4244)

CSimpleModulus::CSimpleModulus()	// Completed
{ 
	this->Init(); 
}

CSimpleModulus::~CSimpleModulus()	// Completed
{
	return;
}

VOID CSimpleModulus::Init()	// Completed
{
	memset(this->m_dwEncryptionKey, 0, sizeof(this->m_dwEncryptionKey));
	memset(this->m_dwModulus, 0, sizeof(this->m_dwModulus));
	memset(this->m_dwDecryptionKey, 0, sizeof(this->m_dwDecryptionKey));
} 

int CSimpleModulus::Encrypt(void * lpDest, void * lpSource, int iSize)	// Emulated - Completed
{
	int iTempSize = iSize;
	int iTempSize2;
	int iOriSize ;
	unsigned char * lpTempDest = (LPBYTE)lpDest;
	unsigned char * lpTempSource = (LPBYTE)lpSource;
	
	int iDec = ((iSize+7)/8);
	iSize = (iDec + iDec*4)*2 + iDec;

	if ( lpDest != NULL )
	{
		iOriSize = iTempSize;
		for ( int i=0; i<iTempSize; i+=8,iOriSize-=8,lpTempDest+=11)
		{
			iTempSize2 = iOriSize;
			if ( iOriSize >= 8 )
				iTempSize2 = 8;

			this->EncryptBlock(lpTempDest, lpTempSource+i, iTempSize2);
		}
	}

	return iSize;
}

int CSimpleModulus::Decrypt(void * lpDest, void * lpSource, int iSize)
{
	if ( lpDest == NULL)
	{
		return iSize*8/11;
	}

	LPBYTE lpTempDest = (LPBYTE)lpDest;
	LPBYTE lpTempSrc = (LPBYTE)lpSource;

	int iResult = 0;
	int iDecLen = 0;

	if ( iSize > 0 )
	{
		while ( iDecLen < iSize )
		{
			int iTempResult = this->DecryptBlock(lpTempDest, lpTempSrc);

			if ( iResult < 0 )
			{
				return iResult;
			}

			iResult += iTempResult;
			iDecLen += 11;
			lpTempSrc += 11;
			lpTempDest += 8;

		}
	}

	return iResult;
}


int CSimpleModulus::EncryptBlock(void*lpDest,void*lpSource,int iSize)
{
	DWORD dwEncBuffer[4];
	DWORD dwEncValue=0;	// TempVar1

	LPBYTE lpEncDest = (LPBYTE)lpDest;
	LPBYTE lpEncSource = (LPBYTE)lpSource;

	memset(lpEncDest, 0, 11);

	for ( int i=0;i<4;i++)
	{
		dwEncBuffer[i]=((this->m_dwXORKey[i]^((WORD*)lpEncSource)[i]^dwEncValue)*this->m_dwEncryptionKey[i])%this->m_dwModulus[i];
		dwEncValue=dwEncBuffer[i]&0xFFFF;
	}

	for ( int i=0;i<3;i++)
	{
		dwEncBuffer[i] = dwEncBuffer[i] ^ this->m_dwXORKey[i] ^ ( dwEncBuffer[i+1] & 0xFFFF );
	}

	int iBitPos = 0;

	for ( int i=0;i<4;i++)
	{
		iBitPos = this->AddBits(lpDest, iBitPos, &dwEncBuffer[i], 0, 16);
		iBitPos = this->AddBits(lpDest, iBitPos, &dwEncBuffer[i], 22, 2);
	}

	BYTE btCheckSum = 0xF8;
	
	for ( int i=0;i<8;i++)
		btCheckSum ^= lpEncSource[i];

	((LPBYTE)&dwEncValue)[1] = btCheckSum ; 
	((LPBYTE)&dwEncValue)[0] = btCheckSum ^ iSize ^ 0x3D; 

	return this->AddBits(lpDest, iBitPos, &dwEncValue, 0, 16);
}

int CSimpleModulus::DecryptBlock(void*lpDest,void*lpSource)
{
	memset(lpDest, 0, 8);
	DWORD dwDecBuffer[4]= {0};
	int iBitPosition = 0;

	LPBYTE lpDecDest = (LPBYTE)lpDest;
	LPBYTE lpDecSource = (LPBYTE)lpSource;

	for(int i=0;i<4;i++)
	{
		this->AddBits(&dwDecBuffer[i], 0, lpDecSource, iBitPosition, 16);
		iBitPosition += 16;
		this->AddBits(&dwDecBuffer[i], 22, lpDecSource, iBitPosition, 2);
		iBitPosition += 2;
	}

	for(int i=2;i>=0;i--)
	{
		dwDecBuffer[i] = dwDecBuffer[i] ^ this->m_dwXORKey[i] ^ (dwDecBuffer[i+1]&0xFFFF);
	}

	DWORD Temp=0,Temp1;

	for(int i=0;i<4;i++)
	{
		Temp1 = (( this->m_dwDecryptionKey[i] * (dwDecBuffer[i])) % ( this->m_dwModulus[i])) ^ this->m_dwXORKey[i]^Temp;
		Temp = dwDecBuffer[i]&0xFFFF;
		((WORD*)lpDecDest)[i] = (Temp1);
	}

	dwDecBuffer[0]=0;
	this->AddBits(&dwDecBuffer[0], 0, lpDecSource, iBitPosition, 16);
	((LPBYTE)dwDecBuffer)[0]=((LPBYTE)dwDecBuffer)[1]^ ((LPBYTE)dwDecBuffer)[0]^0x3D;

	BYTE btCheckSum=0xF8;

	for(int i=0;i<8;i++)
		btCheckSum ^= lpDecDest[i];

	if( btCheckSum != ((LPBYTE)dwDecBuffer)[1] )
		return -1;
	
	return ((LPBYTE)dwDecBuffer)[0];
}

int CSimpleModulus::AddBits(void*lpDest,int iDestBitPos,void*lpSource,int iBitSourcePos,int iBitLen)	// Completed Emulated
{
	// Get Buffer Lens
	int iSourceBufferBitLen = iBitLen + iBitSourcePos ;
	int iTempBufferLen = this->GetByteOfBit(iSourceBufferBitLen-1);
	iTempBufferLen += 1 - this->GetByteOfBit(iBitSourcePos);

	// Copy the Source Buffer
	unsigned char * pTempBuffer = new unsigned char[iTempBufferLen+1];
	memset(pTempBuffer, 0, iTempBufferLen+1);
	memcpy(pTempBuffer, (unsigned char *)lpSource+ this->GetByteOfBit(iBitSourcePos), iTempBufferLen);

	// Save the Last ibt if exist
	if ( (iSourceBufferBitLen%8 ) != 0 )
	{
		pTempBuffer[iTempBufferLen - 1] &= 255 << (8 - (iSourceBufferBitLen%8));
	}

	// Get the Values to Shift
	int iShiftLeft = (iBitSourcePos%8);
	int iShiftRight = (iDestBitPos%8);

	// Shift the Values to Add the right space of the desired bits
	this->Shift(pTempBuffer, iTempBufferLen, -iShiftLeft);
	this->Shift(pTempBuffer, iTempBufferLen+1, iShiftRight);

	// Copy the the bits of Source to the Dest
	int iNewTempBufferLen = (( iShiftRight <= iShiftLeft )?0:1) + iTempBufferLen;
	unsigned char * TempDist = (unsigned char *)lpDest + this->GetByteOfBit(iDestBitPos);

	for ( int i=0;i<iNewTempBufferLen;i++)
	{
		TempDist[i] |= pTempBuffer[i];
	}

	// Delete the temp Buffer
	delete [] pTempBuffer;

	// Return the number of bits of the new Dest Buffer
	return iDestBitPos + iBitLen;
}


VOID CSimpleModulus::Shift(void*lpBuff,int iSize,int ShiftLen) // Need To be Checked
{
	unsigned char * TempBuff = (unsigned char *)lpBuff;

	// Case no Shift Len
	if ( ShiftLen != 0  )	
	{
		// Shift Right
		if ( ShiftLen > 0 )
		{
			if ( (iSize -1 ) > 0 )
			{
				for ( int i=(iSize-1); i>0 ; i--)
				{
					TempBuff[i] = (TempBuff[i-1]<<((8 - ShiftLen))) | (TempBuff[i]>>ShiftLen );
				}
			}

			TempBuff[0] >>=  ShiftLen;
		}
		else	// Shift Left
		{
			ShiftLen = - ShiftLen;

			if ( (iSize-1) > 0 )
			{
				for ( int i=0; i<(iSize-1) ;i++)
				{
					TempBuff[i] = (TempBuff[i+1]>>((8 - ShiftLen))) | (TempBuff[i]<<ShiftLen );
				}
			}

			TempBuff[iSize-1] <<= ShiftLen;
		}
	}
}

int CSimpleModulus::GetByteOfBit(int btByte)
{
	return btByte >> 3;
}

BOOL CSimpleModulus::SaveAllKey(LPSTR lpszFileName )
{
	return this->SaveKey(lpszFileName, 4370, TRUE, TRUE, TRUE, TRUE);
}

BOOL CSimpleModulus::LoadAllKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, TRUE, TRUE, TRUE);
}

BOOL CSimpleModulus::SaveEncryptionKey(LPSTR lpszFileName)
{
	return this->SaveKey(lpszFileName, 4370, TRUE, TRUE, FALSE, TRUE);
}

BOOL CSimpleModulus::LoadEncryptionKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, TRUE, FALSE ,TRUE);
}

BOOL CSimpleModulus::SaveDecryptionKey(LPSTR lpszFileName)
{
	return this->SaveKey(lpszFileName, 4370, TRUE, FALSE, TRUE, TRUE);
}

BOOL CSimpleModulus::LoadDecryptionKey(LPSTR lpszFileName)
{
	return this->LoadKey(lpszFileName, 4370, TRUE, FALSE ,TRUE ,TRUE);
}

BOOL CSimpleModulus::SaveKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bSaveModulus, BOOL bSaveEncKey, BOOL bSaveDecKey, BOOL bSaveXORKey)
{
	// MAde by Deathway
	ENCDEC_FILEHEADER HeaderBuffer;
	int iSize;
	DWORD XORTable[4];
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);

	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}

	HeaderBuffer.sFileHeader = wFileHeader;
	HeaderBuffer.dwSize = ( bSaveModulus + bSaveEncKey + bSaveDecKey + bSaveXORKey ) * sizeof(XORTable) + sizeof(ENCDEC_FILEHEADER);

	WriteFile( hFile, &HeaderBuffer, sizeof(ENCDEC_FILEHEADER), (ULONG*)&iSize, NULL);

	if ( bSaveModulus != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwModulus[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	if ( bSaveEncKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwEncryptionKey[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	if ( bSaveDecKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwDecryptionKey[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	if ( bSaveXORKey != FALSE )
	{
		for ( int n = 0 ; n < 4 ; n++ )
		{
			XORTable[n] = this->m_dwXORKey[n] ^ s_dwSaveLoadXOR[n];
		}
		WriteFile(hFile, &XORTable, sizeof(XORTable), (ULONG*)&iSize, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}


BOOL CSimpleModulus::LoadKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bLoadModulus, BOOL bLoadEncKey, BOOL bLoadDecKey, BOOL bLoadXORKey)
{
	ENCDEC_FILEHEADER HeaderBuffer;
	int iSize;
	DWORD XORTable[4];
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0x80, NULL);

	if ( hFile != INVALID_HANDLE_VALUE )
	{
		ReadFile( hFile, &HeaderBuffer, sizeof(ENCDEC_FILEHEADER), (ULONG*)&iSize, NULL);
	}

	if ( HeaderBuffer.sFileHeader == wFileHeader  )
	{
		if ( (HeaderBuffer.dwSize) == (int)((( bLoadModulus + bLoadEncKey + bLoadDecKey + bLoadXORKey )*sizeof(XORTable))+sizeof(ENCDEC_FILEHEADER)) )
		{
			if ( bLoadModulus != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwModulus[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			if ( bLoadEncKey != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwEncryptionKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			if ( bLoadDecKey != FALSE )
			{
				ReadFile(hFile, XORTable, sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwDecryptionKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}
			if ( bLoadXORKey != FALSE )
			{
				ReadFile(hFile, XORTable,sizeof(XORTable) , (ULONG*)&iSize, NULL);
				for ( int n = 0 ; n<4;n++)
				{
					this->m_dwXORKey[n] = s_dwSaveLoadXOR[n] ^ XORTable[n];
				}
			}

			CloseHandle(hFile);
			return TRUE;
		}
	}
	else
	{
		CloseHandle(hFile);
	}
	return FALSE;
}

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

