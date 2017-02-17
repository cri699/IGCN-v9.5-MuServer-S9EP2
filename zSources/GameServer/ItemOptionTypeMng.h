//////////////////////////////////////////////////////////////////////
// ItemOptionTypeMng.h
#ifndef ITEMOPTIONTYPEMNG_H
#define ITEMOPTIONTYPEMNG_H

#include "zzzitem.h"
#include "user.h"

#define MAX_EXCELLENT_COMMON_OPTIONS 20
#define MAX_EXCELLENT_WING_OPTIONS 40

enum COMMON_EXC_OPTIONS
{
	OPTION_INC_MANA_MONSTER_DIE = 0,
	OPTION_INC_LIFE_MONSTER_DIE = 1,
	OPTION_INC_ATTACK_SPEED = 2,
	OPTION_INC_ATTACK_PERCENT = 3,
	OPTION_INC_ATTACK_LEVEL = 4,
	OPTION_INC_EXCELLENT_DAMAGE = 5,
	OPTION_INC_ZEN_MONSTER_DIE = 6,
	OPTION_INC_DEFENSE_RATE = 7,
	OPTION_INC_DAMAGE_REFLECT = 8,
	OPTION_DEC_VICTIM_DAMAGE = 9,
	OPTION_INC_MAXIMUM_MANA = 10,
	OPTION_INC_MAXIMUM_LIFE = 11
};

struct COMMON_EXT_OPTION_TYPE
{
	void Clear()
	{
		this->OptionID = -1;
		this->ItemKindA_1 = -1;
		this->ItemKindA_2 = -1;
		this->ItemKindA_3 = -1;
		this->OptionNumber = -1;
		this->OptionRate = 0;
		memset(this->OptionName, 0x00, 64);
		this->OptionValue = 0;
	}

	int OptionID;
	int ItemKindA_1;
	int ItemKindA_2;
	int ItemKindA_3;
	int OptionNumber;
	DWORD OptionRate;
	char OptionName[64];
	int OptionValue;
};

struct WING_EXT_OPTION_TYPE
{
	void Clear()
	{
		this->OptionID = -1;
		this->ItemKindA = -1;
		this->ItemKindB = -1;
		this->OptionNumber = -1;
		memset(this->OptionName, 0, 64);
		this->OptionValue = 0;
	}

	int OptionID;
	int ItemKindA;
	int ItemKindB;
	int OptionNumber;
	char OptionName[64];
	int OptionValue;
};

struct ACCESSORY_ITEM_OPTION_VALUE
{
	void Clear()
	{
		this->ItemType = -1;
		this->ItemIndex = -1;
		for (int i = 0; i < 16; ++i)
			this->LevelValue[i] = -1;
	}
	int ItemType;
	int ItemIndex;
	int LevelValue[16];
};

struct EXC_OPTION_RATE
{
	int m_CommonOptionRate[6];
	int m_Wing2ndOptionRate[5];
	int m_Wing3rdOptionRate[4];
	int m_CapeOfLordOptionRate[4];
	int m_CapeOfFighterOptionRate[3];
	int m_MonsterWingsOptionRate[2];
	int m_ConquerorWingsOptionRate[4];
	int m_AngelDevilWingsOptionRate[3];
};

class CItemOptionTypeMng
{
public:

	void Initialize();
	bool LoadScript(char *pchFileName);

	void Initialize_AccessoryItemOption();
	bool LoadAccessoryItemOptionScript(char *pchFileName);
	
	int CommonExcOptionRand(int ItemKindA);

	void CalcExcOptionEffect(OBJECTSTRUCT *lpObj);
	void SetExcOptionEffect(OBJECTSTRUCT *lpObj, int ExcOptionID, LPITEM_ATTRIBUTE lpItem, BYTE btItemPos);

	void CalcWingOptionEffect(OBJECTSTRUCT *lpObj);
	void SetWingOptionEffect(OBJECTSTRUCT *lpObj, int ExcOptionID, LPITEM_ATTRIBUTE lpItem);

	COMMON_EXT_OPTION_TYPE * GetCommonExcOption(int ExcOptionID);
	WING_EXT_OPTION_TYPE * GetWingOption(int ExcOptionID);

	COMMON_EXT_OPTION_TYPE m_CommonExtOptionType[MAX_EXCELLENT_COMMON_OPTIONS];
	WING_EXT_OPTION_TYPE m_WingExtOptionType[MAX_EXCELLENT_WING_OPTIONS];
	ACCESSORY_ITEM_OPTION_VALUE m_AccessoryItemOptionValue[6];
	EXC_OPTION_RATE m_ExcellentOptionRate;
}; 

extern CItemOptionTypeMng g_ItemOptionTypeMng;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

