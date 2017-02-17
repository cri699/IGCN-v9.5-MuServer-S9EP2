//////////////////////////////////////////////////////////////////////
// CUserStatistics.cpp
#include "stdafx.h"
#include "CUserStatistics.h"
#include "DSProtocol.h"
#include "DBSockMng.h"

CUserStatistics::CUserStatistics(char Name[11], int aIndex)
{
	strcpy(this->Name, Name);
	this->aIndex = aIndex;
	this->UserData = new CUserStatisticsData;
	
	STAT_REQ_JOIN_USER_DATA pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xC0, 0x00, sizeof(STAT_REQ_JOIN_USER_DATA));
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

CUserStatistics::~CUserStatistics()
{
	this->SaveData();
	delete this->UserData;
}

bool CUserStatistics::LoadData(CUserStatisticsData *LoadUserData)
{
	this->UserData = LoadUserData;
	return true;
}

void CUserStatistics::SaveData()
{
	STAT_REQ_ANS_SAVE_LOAD_DATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC0, 0x02, sizeof(STAT_REQ_ANS_SAVE_LOAD_DATA));
	pMsg.aIndex = this->aIndex;
	strcpy(pMsg.Name, this->Name);
	pMsg.UserData = *this->UserData;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

