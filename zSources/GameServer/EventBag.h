//////////////////////////////////////////////////////////////////////
// EventBag.h
#pragma once
#include "Bag.h"

class CEventBag : public CBag
{
public:
	CEventBag();
	virtual ~CEventBag();

	bool IsBag(int aIndex, int EventID, int iParam2);
	bool UseBag(int aIndex, int iMonsterIndex);
	bool CheckCondition(int aIndex, int w, int h);
	bool UseBag_GremoryCase(int aIndex, int iMonsterIndex, BYTE btStorageType, BYTE btRewardSource, int iExpireDays);
	void SetBagInfo(int EventID, int iParam2);

private:
	int EventID;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

