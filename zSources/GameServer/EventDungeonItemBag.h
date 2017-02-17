//////////////////////////////////////////////////////////////////////
// EventDungeonItemBag.h
#pragma once

struct CEventItemBagAttr
{
	CEventItemBagAttr()
	{
		this->m_btType = 0;
		this->m_btIndex = 0;
		this->m_btMinLevel = 0;
		this->m_btMaxLevel = 0;
		this->m_btIsSkill = 0;
		this->m_btIsLuck = 0;
		this->m_btIsOption = 0;
		this->m_btIsExItem = 0;
		this->m_btIsSetItem = 0;
		this->m_nDropRate = 0;
	}

	BYTE m_btType;
	BYTE m_btIndex;
	BYTE m_btMinLevel;
	BYTE m_btMaxLevel;
	BYTE m_btIsSkill;
	BYTE m_btIsLuck;
	BYTE m_btIsOption;
	BYTE m_btIsExItem;
	BYTE m_btIsSetItem;
	DWORD m_nDropRate;
};

struct _stEventDungeonItemInfo
{
	_stEventDungeonItemInfo()
	{
		this->m_bUse = FALSE;
		this->m_nLevelIndex = 0;
		this->m_nMinLevel = 0;
		this->m_nMaxLevel = 0;
		this->m_nDropZen = -1;
		this->m_nDropCount = 0;
	}

	BOOL m_bUse;
	DWORD m_nLevelIndex;
	DWORD m_nMinLevel;
	DWORD m_nMaxLevel;
	int m_nDropZen;
	DWORD m_nDropCount;
	DWORD m_nExOptionAddCntRate;
	std::vector<CEventItemBagAttr> m_vtItemBagAttr;
};

class CEventDungeonItemBag
{
public:
	CEventDungeonItemBag(void);
	virtual ~CEventDungeonItemBag(void);

	void LoadScript(char *szFileName);
	int DropEventItem(int nIndex, int nMaxUserLevel);

private:

	int GetLevelIndex(int nUserLevel);
	int GetRegItemCount(int nUserLevel);
	int GetDropZen(int nUserLevel);
	int GetDropItemCount(int nUserLevel);
	int GetExcDropRate(int nUserLevel);
	int GetLevel(int nMinLevel, int nMaxLevel);
	int CreateNormalItem(int nIndex, int nMaxUserLevel, CEventItemBagAttr BagObject);
	int CreateSetItem(int nIndex, CEventItemBagAttr BagObject);

	BOOL						m_bLoad;
	_stEventDungeonItemInfo		eventDungeonItemInfo[10];
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

