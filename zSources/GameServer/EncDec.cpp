//////////////////////////////////////////////////////////////////////
// EncDec.cpp
#include "stdafx.h"
#include "EncDec.h"
#include "configread.h"

#define E 2.71828183
#define PI 3.14159265

/*1.
[xorSeed][subSeed]*/

CEncDec* g_EncDec = new CEncDec;

CEncDec::CEncDec()
{
	m_CGInitialized = false;

	m_GCInitialized = false;
}

void CEncDec::OnConnect()
{

	m_CGInitialized = false;
	m_GCInitialized = false;
}

void CEncDec::CreateServerKey(DWORD xorSeed, DWORD subSeed)
{
	m_GCInitialized = true;
	xorSeed*=(xorSeed+1);
	subSeed*=(subSeed+1);
	for(int i = 0; i < 256; i++)
	{
		double x = (xorSeed+1)*PI;
		x -= (int)x; // x = x mod 1, x<0,1)
		xorSeed+= x*0x7fff;
		m_GCXorKey[i] = xorSeed;
	}

	for(int i = 0; i < 256; i++)
	{
		double x = (subSeed+1)*E;
		x -= (int)x; // x = x mod 1, x<0,1)
		subSeed+= x*0x7fff;
		m_GCSubKey[i] = subSeed;
	}
}

void CEncDec::CreateClientKey(DWORD xorSeed, DWORD subSeed)
{
	m_CGInitialized = true;
	xorSeed*=(xorSeed+1);
	subSeed*=(subSeed+1);
	for(int i = 0; i < 256; i++)
	{
		double x = (xorSeed+1)*PI;
		x -= (int)x; // x = x mod 1, x<0,1)
		xorSeed+= x*0x7fff;
		m_CGXorKey[i] = xorSeed;
	}

	for(int i = 0; i < 256; i++)
	{
		double x = (subSeed+1)*E;
		x -= (int)x; // x = x mod 1, x<0,1)
		subSeed+= x*0x7fff;
		m_CGSubKey[i] = subSeed;
	}
}

void CEncDec::EncCGBuff(BYTE* buff, int len)
{
	if(!m_CGInitialized)
		return;
	for(int i = 0; i < len; i++)
	{
		buff[i] = buff[i]^m_CGXorKey[i%256];
		buff[i] = buff[i]-m_CGSubKey[i%256];
	}
}

bool CEncDec::DecCGBuff(BYTE* buff, int len)
{
	if(g_ConfigRead.data.common.Version >= 1)
	{
		return true;
	}

	if(!m_CGInitialized)
	{
		if(buff[0] != 0xFA)
			return false;
		return true;
	}
	if(g_ConfigRead.data.common.MainProtocolCrypt == false)
	{
		return true;
	}
	for(int i = 0; i < len; i++)
	{
		buff[i] = buff[i]+m_CGSubKey[i%256];
		buff[i] = buff[i]^m_CGXorKey[i%256];
	}
	return true;
}

void CEncDec::EncGCBuff(BYTE* buff, int len)
{
	if(!m_GCInitialized)
		return;
	for(int i = 0; i < len; i++)
	{
		buff[i] = buff[i]^m_GCXorKey[i%256];
		buff[i] = buff[i]-m_GCSubKey[i%256];
	}
}

void CEncDec::DecGCBuff(BYTE* buff, int len)
{
	if(!m_GCInitialized)
		return;
	for(int i = 0; i < len; i++)
	{
		buff[i] = buff[i]+m_GCSubKey[i%256];
		buff[i] = buff[i]^m_GCXorKey[i%256];
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

