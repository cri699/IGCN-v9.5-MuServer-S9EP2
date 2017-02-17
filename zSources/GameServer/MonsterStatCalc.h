//////////////////////////////////////////////////////////////////////
// MonsterStatCalc.h
#include "user.h"

#pragma once

struct _stMonsterStatRef
{
	_stMonsterStatRef()
	{
		this->m_nMinLevel = 0;
		this->m_nMaxLevel = 0;
		this->m_nLevel = 0;
		this->m_nHp = 0;
		this->m_nMp = 0;
		this->m_nDamageMin = 0;
		this->m_nDamageMax = 0;
		this->m_nDefense = 0;
		this->m_nMagicDefense = 0;
		this->m_nAttackRating = 0;
		this->m_nSuccessfulblocking = 0;
	}

	int m_nMinLevel;
	int m_nMaxLevel;
	int m_nLevel;
	int m_nHp;
	int m_nMp;
	int m_nDamageMin;
	int m_nDamageMax;
	int m_nDefense;
	int m_nMagicDefense;
	int m_nAttackRating;
	int m_nSuccessfulblocking;
};

class CMonsterStatCalc
{
public:
	CMonsterStatCalc(void);
	virtual ~CMonsterStatCalc(void);
	
	void LoadScript(LPSTR Filename);
	BOOL ConvertMonsterStat(LPOBJ lpObj, int nUserMaxLevel);

private:

	bool m_bLoad;
	std::vector<_stMonsterStatRef> m_vtMonsterStatRefAttr;

	
};

extern CMonsterStatCalc g_MonsterStatCalc;


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

