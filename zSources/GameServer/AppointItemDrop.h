//////////////////////////////////////////////////////////////////////
// AppointItemDrop.h
#ifndef APPOINTTITEMDROP_H
#define APPOINTTITEMDROP_H

struct _ST_APPOINTITEMDROP_DROP_ITEM
{
	_ST_APPOINTITEMDROP_DROP_ITEM()
	{
		this->iDropItemGroup = 0;
		this->iType = 0;
		this->iIndex = 0;
		this->iMinLevel = 0;
		this->iMaxLevel = 0;
		this->iSkill = 0;
		this->iLuck = 0;
		this->iAddoption = 0;
		this->iExcelent = 0;
		this->iDropRate = 0;
	}

	int iDropItemGroup;
	int iType;
	int iIndex;
	int iMinLevel;
	int iMaxLevel;
	int iSkill;
	int iLuck;
	int iAddoption;
	int iExcelent;
	int iDropRate;
};

struct _ST_APP_MONSTER_ITEMGROUP_LINK
{
	_ST_APP_MONSTER_ITEMGROUP_LINK()
	{
		this->iMonsterIndex = 0;
		this->iDropItemGroup = 0;
	}

	int iMonsterIndex;
	int iDropItemGroup;
};

struct _ST_APP_DROP_ITEM_GROUP
{
	_ST_APP_DROP_ITEM_GROUP()
	{
		this->iDropItemGroup = 0;
		this->iDropRateDenominator = 1;
		this->iTotalDropRate = 0;
		this->vecDropItem.clear();
	}

	int iDropItemGroup;
	int iDropRateDenominator;
	int iTotalDropRate;
	std::vector<_ST_APPOINTITEMDROP_DROP_ITEM> vecDropItem;
};

class CAppointItemDrop
{
public:
	CAppointItemDrop();
	~CAppointItemDrop();

	void LoadAppointItemDropScript();
	int AppointItemDrop(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj);

private:

	bool m_bIsSuccessReadScript;
	std::map<int, _ST_APP_MONSTER_ITEMGROUP_LINK> m_mapMonsterItemGroupLink;
	std::map<int, _ST_APP_DROP_ITEM_GROUP> m_mapItemDropInfo;
}; 

extern CAppointItemDrop g_CAppointItemDrop;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

