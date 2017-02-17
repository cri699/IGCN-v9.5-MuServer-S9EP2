//////////////////////////////////////////////////////////////////////
// CUserStatisticsData.cpp
#include "StdAfx.h"
#include "CUserStatisticsData.h"

void CUserStatisticsData::IncrementMonsterKillCount(int MonsterType)
{
	InterlockedIncrement(reinterpret_cast<long *>(&this->MonsterKillCount));

	switch(MonsterType)
	{
		case 0 : InterlockedIncrement(reinterpret_cast<long *>(&this->GoldenMonsterKillCount)); break;
		case 1 : InterlockedIncrement(reinterpret_cast<long *>(&this->BossKillCount)); break;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

