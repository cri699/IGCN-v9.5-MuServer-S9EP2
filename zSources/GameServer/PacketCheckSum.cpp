//////////////////////////////////////////////////////////////////////
// PacketCheckSum.cpp
#include "stdafx.h"
#include "PacketCheckSum.h"
#include "TLog.h"
#include "configread.h"
#include "GameMain.h"
// GS-N 0.99.60T 0x00452B80
// GS-N	1.00.18	JPN	0x00461F60	-	Completed

CPacketCheckSum gPacketCheckSum;


CPacketCheckSum::CPacketCheckSum()
{
	this->PaketChecksum = NULL;
}



CPacketCheckSum::~CPacketCheckSum()
{
	delete [] PaketChecksum;
	return;
}


void CPacketCheckSum::Init()
{
	if ( this->PaketChecksum != NULL )
	{
		delete [] this->PaketChecksum;
	}

	this->PaketChecksum = new PAKETCHECKSUM[g_ConfigRead.server.GetObjectMax()];

	this->m_ChecksumTableAllClearState = 0;
	memset(this->m_ChecksumTableClearState, 0, sizeof(this->m_ChecksumTableClearState));
	memset(this->m_ChecksumTableAVGCount, 0, sizeof(this->m_ChecksumTableAVGCount));
	memset(this->m_ChecksumTableAVG, 0, sizeof(this->m_ChecksumTableAVG));
	memset(this->PaketChecksum, 0, sizeof(this->PaketChecksum));
}



void CPacketCheckSum::Check(int aIndex)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return;
	}

	for ( int i=0 ; i<MAX_PACKET_CHECKSUM_FUNCTION_INDEX ; i++)
	{
		if ( this->PaketChecksum[aIndex].Check[i] == 0 )
		{
			char szPacketError[256];

			g_Log.Add("PacketCheckSum Error [%d][%s][%s] %d",
				aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name, i);
			
			wsprintf(szPacketError, "PacketCheckSum Error %d", i);

			this->ClearCheckSum(aIndex);

			if ( gDisconnectHackUser != FALSE )
			{
				gObjCloseSet(aIndex, 0);
			}

			return;
		}
	}

	g_Log.Add("Check PacketCheckSum [%d][%s][%s]", aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name);
	this->ClearCheckSum(aIndex);
}


void CPacketCheckSum::ClearCheckSum(int aIndex)
{
	memset(&this->PaketChecksum[aIndex], 0, sizeof(PAKETCHECKSUM) );
}


BOOL CPacketCheckSum::Add(int aIndex, int funcindex, DWORD checksum)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return FALSE;
	}

	if ( this->m_ChecksumTableAllClearState == 0 )
	{
		this->PaketChecksum[aIndex].Check[funcindex] = checksum;
		return TRUE;
	}
	
	if ( this->m_ChecksumTable[funcindex] == checksum )
	{
		this->PaketChecksum[aIndex].Check[funcindex] = checksum;
	}
	else
	{
		char szPacketError[256];

		g_Log.Add("PacketCheckSum : Compare Fail : [%d][%s][%s] (%d,%d)", aIndex,
			gObj[aIndex].AccountID, gObj[aIndex].Name, funcindex, checksum);

		wsprintf(szPacketError, "PacketCheckSum : Compare Fail : (%d,%d)", funcindex, checksum);

		if ( gDisconnectHackUser != FALSE )
		{
			gObjCloseSet(aIndex, 0);
		}
	}

	return TRUE;
}



void CPacketCheckSum::AddCheckSum(int aIndex, int funcindex, DWORD checksum)
{
	if ( funcindex < 0 || funcindex >= MAX_PACKET_CHECKSUM_FUNCTION_INDEX )
	{
		g_Log.Add("PacketCheckSum : Invalid FuncIndex : %d, %d", aIndex, funcindex);
		return;
	}

	this->Add(aIndex, funcindex, checksum);

	if ( this->m_ChecksumTableAllClearState != 0 )
	{
		return;
	}

	if ( this->m_ChecksumTableClearState[funcindex] != 0 )
	{
		return;
	}

	this->m_ChecksumTableAVG[funcindex][this->m_ChecksumTableAVGCount[funcindex] % MAX_CHECKSUM_TABLE_AVG] = checksum;
	this->m_ChecksumTableAVGCount[funcindex]++;

	g_Log.Add("Insert PacketCheckSum %d, %d,%d", funcindex, this->m_ChecksumTableAVGCount[funcindex], checksum);

	if ( this->m_ChecksumTableAVGCount[funcindex] >= MAX_CHECKSUM_TABLE_AVG )
	{
		this->SetClearChecksumFunc(funcindex);
	}
}



void CPacketCheckSum::SetClearChecksumFunc(int funcindex)
{
	int iCount = 0;
	int Checksum = this->m_ChecksumTableAVG[funcindex][0];

	for ( int i=0;i<MAX_CHECKSUM_TABLE_AVG;i++)
	{
		if ( this->m_ChecksumTableAVG[funcindex][i] == Checksum )
		{
			iCount++;
		}
	}

	if ( iCount >= MAX_CHECKSUM_TABLE_AVG-1 )
	{
		this->m_ChecksumTableClearState[funcindex] = 1;
		this->m_ChecksumTableAllClearState = TRUE;

		for (int i=0;i<MAX_PACKET_CHECKSUM_FUNCTION_INDEX;i++)
		{
			if ( this->m_ChecksumTableClearState[i] == 0 )
			{
				this->m_ChecksumTableAllClearState = FALSE;
			}

			this->m_ChecksumTable[i] = this->m_ChecksumTableAVG[i][0];
		}

		if ( this->m_ChecksumTableAllClearState != FALSE )
		{
			g_Log.Add("Complete Init Checksum");

			for (int i=0;i<MAX_PACKET_CHECKSUM_FUNCTION_INDEX;i++)
			{
				this->m_ChecksumTable[i] = this->m_ChecksumTableAVG[i][0];
				g_Log.Add("%d, %d", i, this->m_ChecksumTable[i]);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

