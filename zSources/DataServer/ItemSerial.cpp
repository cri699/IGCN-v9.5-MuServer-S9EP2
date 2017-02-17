//////////////////////////////////////////////////////////////////////
// ItemSerial.cpp
#include "stdafx.h"
#include "ItemSerial.h"
#include "DataServer.h"
#include "DSprotocol.h"
#include "TLog.h"

CItemSerial g_ItemSerial;

CItemSerial::CItemSerial(void)
{
	InitializeCriticalSection(&this->m_ItemSerialCriti);
}

CItemSerial::~CItemSerial(void)
{
	DeleteCriticalSection(&this->m_ItemSerialCriti);
}

bool CItemSerial::Init()
{
	if (this->m_Query.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] - DATA SERVER CANNOT CONNECT TO MSSQL");
		return false;
	}

	this->m_dwItemSerial = 0;
	this->m_dwLastSerial = 0;
	return true;
}

UINT64 CItemSerial::MakeSerial()
{
	if( this->GetSerialFromDB(this->m_dwItemSerial, 3000) == FALSE )
	{
		return FALSE;
	}

	this->m_dwLastSerial = this->m_dwItemSerial + 3000;
	return 1;
}

UINT64 CItemSerial::GenerateSerial()
{
	EnterCriticalSection(&this->m_ItemSerialCriti);
	
	if ( this->m_dwItemSerial >= this->m_dwLastSerial )
	{
		this->MakeSerial();
	}

	UINT64 retserial = this->m_dwItemSerial;

	this->m_dwItemSerial++;

	LeaveCriticalSection(&this->m_ItemSerialCriti);

	return retserial;
}

BOOL CItemSerial::GetSerialFromDB(UINT64 & serial, int MakeSerialCount)
{
	this->m_Query.ExecQuery("WZ_GetItemSerial2 '%d'", MakeSerialCount);

	if(this->m_Query.Fetch() == SQL_NO_DATA)
	{
		this->m_Query.Close();
		return FALSE;
	}

	serial = this->m_Query.GetAsInteger64("ItemSerial");
	this->m_Query.Close();

	return TRUE;
}

UINT64 CItemSerial::GetSerialCount()
{
	return this->m_dwItemSerial;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

