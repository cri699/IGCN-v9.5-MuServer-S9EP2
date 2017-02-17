//////////////////////////////////////////////////////////////////////
// CommonBag.h
#pragma once
#include "Bag.h"

class CCommonBag : public CBag
{
public:
	CCommonBag();
	virtual ~CCommonBag();

	bool IsBag(int aIndex, int ItemID, int ItemLevel);
	bool UseBag(int aIndex, int iParam2);
	bool CheckCondition(int aIndex, int ItemID, int ItemLevel);
	bool UseBag_GremoryCase(int aIndex, int iMonsterIndex, BYTE btStorageType, BYTE btRewardSource, int iExpireDays);
	void SetBagInfo(int ItemID, int ItemLevel);

private:
	int		m_BagItemID;
	int		m_BagItemLevel;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

