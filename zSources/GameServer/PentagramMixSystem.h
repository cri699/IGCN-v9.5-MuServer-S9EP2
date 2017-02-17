//////////////////////////////////////////////////////////////////////
// PentagramMixSystem.h
#ifndef PENTAGRAM_MIX_SYSTEM_H
#define PENTAGRAM_MIX_SYSTEM_H

#include "user.h"
#include "zzzitem.h"
#include "PentagramSystem.h"

#define PENTAGRAM_MIX_UPGRADE_FAILED				0xE1
#define PENTAGRAM_MIX_REFINE_FAILED					0xE2
#define PENTAGRAM_MIX_FAILED_WITH_TALISMAN			0xE3
#define PENTAGRAM_MIX_REFINE_SUCCESS_NOTFOUND		0xF8
#define PENTAGRAM_MIX_NOT_ENOUGH_ZEN				0xF9
#define PENTAGRAM_MIX_ITEMS_COUNT_MISMATCH			0xFA
#define PENTAGRAM_MIX_ATTRIBUTE_MISMATCH			0xFB
#define PENTAGRAM_MIX_REFINE_NOTFOUND				0xFC
#define PENTAGRAM_MIX_PSHOP_OPEN					0xFD
#define PENTAGRAM_MIX_ALREADY_OPEN					0xFE

struct REFINE_OPTION
{
	void Clear()
	{
		this->RefineIndex = 0;
		this->RefineSuccessRate = 0;

		for (int i = 0; i < 4; i++)
		{
			this->NeedItem_Type[i] = 0;
			this->NeedItem_Index[i] = 0;
			this->NeedItem_Num[i] = 0;
		}

		this->NeedMoney = 0;
	}

	int RefineIndex;
	int RefineSuccessRate;

	int NeedItem_Type[4];
	int NeedItem_Index[4];
	int NeedItem_Num[4];

	int NeedMoney;
};

struct REFINE_SUCCESS_OPTION
{
	void Clear()
	{
		this->SuccessRefineIndex = 0;

		for (int i = 0; i < 5; i++)
		{
			this->Success_AcquisitionRate[i] = 0;
			this->Success_AcquisitionItem_Type[i] = 0;
			this->Success_AcquisitionItem_Index[i] = 0;
			this->Success_AcquisitionItem_Num[i] = 0;
		}
	}

	int SuccessRefineIndex;

	int Success_AcquisitionRate[5];
	int Success_AcquisitionItem_Type[5];
	int Success_AcquisitionItem_Index[5];
	int Success_AcquisitionItem_Num[5];
};

struct JEWEL_LEVEL_UPGRADE_NEED_ITEM
{
	void Clear()
	{
		this->TargetItemLevel = 0;
		this->MainSourceItemLevel = 0;
		this->MainSourceItemNum = 0;

		for (int i = 0; i < 4; i++)
		{
			this->NeedSourceItem_Type[i] = 0;
			this->NeedSourceItem_Index[i] = 0;
			this->NeedSourceItem_Num[i] = 0;
		}

		this->NeedMoney = 0;
	}

	int TargetItemLevel;
	int MainSourceItemLevel;
	int MainSourceItemNum;

	int NeedSourceItem_Type[4];
	int NeedSourceItem_Index[4];
	int NeedSourceItem_Num[4];

	int NeedMoney;
};

struct JEWEL_RANK_UPGRADE_NEED_ITEM
{
	void Clear()
	{
		this->TargetItemRank = 0;
		this->MainSourceItemRank = 0;
		this->MainSourceItemNum = 0;

		for (int i = 0; i < 4; i++)
		{
			this->NeedSourceItem_Type[i] = 0;
			this->NeedSourceItem_Index[i] = 0;
			this->NeedSourceItem_Num[i] = 0;
		}

		this->NeedMoney = 0;
	}

	int TargetItemRank;
	int MainSourceItemRank;
	int MainSourceItemNum;

	int NeedSourceItem_Type[4];
	int NeedSourceItem_Index[4];
	int NeedSourceItem_Num[4];

	int NeedMoney;
};

struct PENTAGRAM_JEWEL_ITEM_OPTION
{
	void Clear()
	{
		this->JewelIndex = 0;
		this->PentagramJewel_Type = 0;
		this->PentagramJewel_Index = 0;
		this->ItemRank = 0;
		this->RankOptionNum = 0;
		this->RankOptionNumRate = 0;
		this->RON = 0;

		for (int i = 0; i < 11; i++)
		{
			this->LevelValue[i] = 0;
			this->LevelUpgradeValue[i] = 0;
		}
	}

	int JewelIndex;
	int PentagramJewel_Type;
	int PentagramJewel_Index;
	int ItemRank;
	int RankOptionNum;
	int RankOptionNumRate;
	int RON;

	int LevelValue[11];
	int LevelUpgradeValue[11];
};

struct JEWEL_RANK_UPGRADE_DETERMINE_RATE
{
	void Clear()
	{
		this->RankStep = 0;

		for (int i = 0; i < 4; i++)
		{
			this->RankRate[i] = 0;
		}

		this->RankUpgradeSuccessRate = 0;
	}

	int RankStep;
	int RankRate[4];
	int RankUpgradeSuccessRate;
};

class CPentagramMixSystem
{
public:
	CPentagramMixSystem(void);
	virtual ~CPentagramMixSystem(void);
	friend CPentagramSystem;

	void Initialize_MixNeedSource();
	void Initialize_JewelOption();

	bool LoadMixNeedSourceScript(char *pchFileName);
	bool LoadJewelOptionScript(char *pchFileName);

	int PentagramMixBoxInit(OBJECTSTRUCT* lpObj);
	BYTE PentagramJewelRefine(int iIndex, int iRefineType);
	BYTE PentagramJewel_Upgrade(int iIndex, int iUpgradeType, int iTargetValue);
	int CheckLevelCondition(CItem *pItem, WORD pLevel, BYTE pOpt1, BYTE pOpt2, BYTE pOpt3, BYTE pSetOpt, BYTE pExOpt);
	void LogPentagramItem(OBJECTSTRUCT *lpObj, char *sLogType);

public:
	REFINE_OPTION m_RefineOptionData[18];
	REFINE_SUCCESS_OPTION m_RefineSuccessOptionData[18];
	JEWEL_LEVEL_UPGRADE_NEED_ITEM m_JewelLevelUpgradeNeedItemData[11];
	JEWEL_RANK_UPGRADE_NEED_ITEM m_JewelRankUpgradeNeedItemData[4];
	JEWEL_RANK_UPGRADE_DETERMINE_RATE m_JewelRankUpgradeDetermineRate[4];
	PENTAGRAM_JEWEL_ITEM_OPTION m_JewelItemOptionData[250];
};

extern CPentagramMixSystem g_PentagramMixSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

