//////////////////////////////////////////////////////////////////////
// EncDec.cpp
#include "stdafx.h"
#include "EncDec.h"
#pragma optimize("g", on)

CEncDec* g_EncDec = new CEncDec;

DWORD __fastcall CEncDec::ComputeCRC(BYTE* data, int size)
{
	int crc, i;

	crc = 0;

	BYTE* ptr = data;
	int count = size;

	while(--count >= 0)
	{
		crc = crc ^ (int)*ptr++ << 8;
		for(i = 0; i < 8; ++i)
			if(crc & 0x8000)
				crc = crc << 1 ^ 0x10213465;
			else
				crc = crc << 1;
	}

	return (crc & 0xFFFFFFFF);
}

DWORD __fastcall CEncDec::ComputeFileCRC(char* file)
{
	FILE* hFile = fopen(file, "rb");

	if(!hFile)
		return 0;

	fseek(hFile, 0, SEEK_END);
	long size = ftell(hFile);
	fseek(hFile, 0, SEEK_SET);

	BYTE* buff = new BYTE[size];
	fread(buff, 1, size, hFile);

	DWORD crc = ComputeCRC(buff, size);

	fclose(hFile);
	delete [] buff;

	return crc;
}

void CEncDec::EncXor32(unsigned char*Buff, int SizeOfHeader, int Len)
{
	BYTE byXorFilter[32] = { 0 };
	byXorFilter[0] = 0xAB;
	byXorFilter[1] = 0x11;
	byXorFilter[2] = 0xCD;
	byXorFilter[3] = 0xFE;
	byXorFilter[4] = 0x18;
	byXorFilter[5] = 0x23;
	byXorFilter[6] = 0xC5;
	byXorFilter[7] = 0xA3;
	byXorFilter[8] = 0xCA;
	byXorFilter[9] = 0x33;
	byXorFilter[10] = 0xC1;
	byXorFilter[11] = 0xCC;
	byXorFilter[12] = 0x66;
	byXorFilter[13] = 0x67;
	byXorFilter[14] = 0x21;
	byXorFilter[15] = 0xF3;
	byXorFilter[16] = 0x32;
	byXorFilter[17] = 0x12;
	byXorFilter[18] = 0x15;
	byXorFilter[19] = 0x35;
	byXorFilter[20] = 0x29;
	byXorFilter[21] = 0xFF;
	byXorFilter[22] = 0xFE;
	byXorFilter[23] = 0x1D;
	byXorFilter[24] = 0x44;
	byXorFilter[25] = 0xEF;
	byXorFilter[26] = 0xCD;
	byXorFilter[27] = 0x41;
	byXorFilter[28] = 0x26;
	byXorFilter[29] = 0x3C;
	byXorFilter[30] = 0x4E;
	byXorFilter[31] = 0x4D;

	for (int i = 0; i<Len; i++)
		Buff[i] ^= (byXorFilter[(i + SizeOfHeader) & 31] ^ Buff[i - 1]);
}

void CEncDec::DecXor32(unsigned char*Buff, int SizeOfHeader, int Len)
{
	BYTE byXorFilter[32] = { 0 };
	byXorFilter[0] = 0xAB;
	byXorFilter[1] = 0x11;
	byXorFilter[2] = 0xCD;
	byXorFilter[3] = 0xFE;
	byXorFilter[4] = 0x18;
	byXorFilter[5] = 0x23;
	byXorFilter[6] = 0xC5;
	byXorFilter[7] = 0xA3;
	byXorFilter[8] = 0xCA;
	byXorFilter[9] = 0x33;
	byXorFilter[10] = 0xC1;
	byXorFilter[11] = 0xCC;
	byXorFilter[12] = 0x66;
	byXorFilter[13] = 0x67;
	byXorFilter[14] = 0x21;
	byXorFilter[15] = 0xF3;
	byXorFilter[16] = 0x32;
	byXorFilter[17] = 0x12;
	byXorFilter[18] = 0x15;
	byXorFilter[19] = 0x35;
	byXorFilter[20] = 0x29;
	byXorFilter[21] = 0xFF;
	byXorFilter[22] = 0xFE;
	byXorFilter[23] = 0x1D;
	byXorFilter[24] = 0x44;
	byXorFilter[25] = 0xEF;
	byXorFilter[26] = 0xCD;
	byXorFilter[27] = 0x41;
	byXorFilter[28] = 0x26;
	byXorFilter[29] = 0x3C;
	byXorFilter[30] = 0x4E;
	byXorFilter[31] = 0x4D;

	for (int i = Len - 1; i >= 0; i--)
		Buff[i] ^= (byXorFilter[(i + SizeOfHeader) & 31] ^ Buff[i - 1]);
}

void CEncDec::BuxConvert(char* buf, int size)
{

	for (int n = 0; n<size; n++)
	{
		buf[n] ^= bBuxCode[n % 3];		// Nice trick from WebZen
	}
}

#pragma optimize("g", off)

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

