//////////////////////////////////////////////////////////////////////
// MonsterBag.h
#pragma once
#include "Bag.h"
class CMonsterBag :
	public CBag
{
public:
	CMonsterBag();
	virtual ~CMonsterBag();

	bool IsBag(int aIndex, int MonsterClass, int iParam2);
	bool UseBag(int aIndex, int iMonsterIndex);
	bool CheckCondition(int aIndex, int MonsterClass, int iParam2);
	bool UseBag_GremoryCase(int aIndex, int iMonsterIndex, BYTE btStorageType, BYTE btRewardSource, int iExpireDays);
	void SetBagInfo(int MonsterClass, int iParam2);

private:
	int		m_BagMonsterClass;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

