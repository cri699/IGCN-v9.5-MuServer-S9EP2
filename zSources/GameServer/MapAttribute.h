//////////////////////////////////////////////////////////////////////
// MapAttribute.h
#include "MapClass.h"

#pragma once

struct MAP_ATTRIBUTE
{
	int PvPType;
	int ItemRateDropBonus;
	float ExpBonus;
	float MasterExpBonus;
	int IsVipMap;
	int PvMType;
	int PkIncrease;
	int GensSet;
	int UserRegen;
	int BlockEntry;

	void Clear()
	{
		this->PvPType = 0;
		this->ItemRateDropBonus = 0;
		this->ExpBonus = 0.0;
		this->MasterExpBonus = 0.0;
		this->IsVipMap = 0;
		this->PvMType = 0;
		this->PkIncrease = 0;
		this->GensSet = -1;
		this->UserRegen = 0;
		this->BlockEntry = 0;
	}
};

class CMapAttribute
{
public:
	CMapAttribute(void);
	~CMapAttribute(void);

	void LoadFile(char * szFile);
	
	int GetPvPState(int MapNumber) { return this->m_MapAttr[MapNumber].PvPType; }
	float GetExpBonus(int MapNumber) { return this->m_MapAttr[MapNumber].ExpBonus; }
	float GetMasterExpBonus(int MapNumber) { return this->m_MapAttr[MapNumber].MasterExpBonus; }
	int GetItemDropBonus(int MapNumber) { return this->m_MapAttr[MapNumber].ItemRateDropBonus; }
	int CheckVIP(int MapNumber) { return this->m_MapAttr[MapNumber].IsVipMap; }
	int CheckPkInc(int MapNumber) { return this->m_MapAttr[MapNumber].PkIncrease; }
	int CheckUserRegen(int MapNumber) { return this->m_MapAttr[MapNumber].UserRegen; }
	int CheckBlockEntry(int MapNumber) { return this->m_MapAttr[MapNumber].BlockEntry; }

private:

	MAP_ATTRIBUTE m_MapAttr[MAX_NUMBER_MAP];
};

extern CMapAttribute g_MapAttr;

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

