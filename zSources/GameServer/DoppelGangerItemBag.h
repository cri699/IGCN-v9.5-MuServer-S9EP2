//////////////////////////////////////////////////////////////////////
// DoppelGangerItemBag.h
#ifndef DOPPELGANGERITEMBAG_H
#define DOPPELGANGERITEMBAG_H

#include "GamblingSystem.h"
#include "user.h"

struct __tagDOPPELGANGER_DROP_ITEM_GROUP_INFO
{
	__tagDOPPELGANGER_DROP_ITEM_GROUP_INFO()
	{
		this->nGroupIndex = 0;
		this->nSkillRate = 0;
		this->nLuckRate = 0;
		this->nAddOptRate = 0;
		this->nExOptRate = 0;
	}

	int nGroupIndex;
	int nSkillRate;
	int nLuckRate;
	int nAddOptRate;
	int nExOptRate;
	std::vector<CItemBagAttrEx> vtDropItemGroup;
};

#pragma once
class CDoppelGangerItemBag
{
public:
	CDoppelGangerItemBag(void);
	virtual ~CDoppelGangerItemBag(void);

	void DoppelgangerItemBagInit();
	void LoadDoppelgangerItemBag(LPSTR lpFile);
	int DoppelgangerItemDrop(OBJECTSTRUCT *lpObj, int nItemBagIndex, int nDropRate, BYTE btFlag);

private:
	BOOL														m_bLoad;
	std::map<int, __tagDOPPELGANGER_DROP_ITEM_GROUP_INFO>		vtDPItemBag;
	int															m_nAddOptRate[7];
	int															m_nExItemAddOptRate[6];
	int															m_nExItemOptKindRate[6];
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

