// GamblingSystem.h: interface for the CGamblingItemBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMBLINGSYSTEM_H__2874FC7A_D2C6_4A3B_B4F0_3D15EC489DDC__INCLUDED_)
#define AFX_GAMBLINGSYSTEM_H__2874FC7A_D2C6_4A3B_B4F0_3D15EC489DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemBagAttrEx
{

public:

	CItemBagAttrEx()	// line : 26
	{
		this->m_type = 0;
		this->m_index = 0;
		this->m_minLevel = 0;
		this->m_maxLevel = 0;
		this->m_isskill = 0;
		this->m_isluck = 0;
		this->m_isoption = 0;
		this->m_isexitem = 0;
	}	// line : 36

	BYTE m_type;	// 0
	BYTE m_index;	// 1
	BYTE m_minLevel;	// 2
	BYTE m_maxLevel;	// 3
	BYTE m_isskill;	// 4
	BYTE m_isluck;	// 5
	BYTE m_isoption;	// 6
	BYTE m_isexitem;	// 7

};

struct _GAMBLING_RATE
{
	int iSkillRate;
	int iLuckRate;
	int iAddOptionRate;
	int iExcellentRate;
	int iSelectedRate;
	
	_GAMBLING_RATE()
	{
		iSkillRate = 0;
		iLuckRate = 0;
		iAddOptionRate = 0;
		iExcellentRate = 0;
		iSelectedRate = 0;
	}
};

class CGamblingItemBag  
{
	BOOL m_bLoad;

	_GAMBLING_RATE m_stGamblingRate[25];
	
	int m_iAddOption[7];
	int m_iGamblingItemGetRate[5];
	int m_iGamblingOptionCountRate[6];
	int m_iGamblingExOptionRate[6];
	int m_iNormalExOptionRate[6];
	int m_iBagObjectCount;
	int m_iDropRatePerItemCount[25];
	int m_iRateKindCount;
	CItemBagAttrEx BagObject[150];
	bool m_bEnabled;
	int m_iGamblingValue;
	int m_iGamblingShopTime;

public:
	CGamblingItemBag();
	virtual ~CGamblingItemBag();

private:
	int GetBagCount(){		return m_iBagObjectCount;	}
	BYTE GetLevel(int n);
	
	BYTE IsSkill(int n);
	BYTE IsLuck(int n);
	BYTE IsOption(int n);

public:
	void GamblingItemBagInit(char* name);
	void LoadItem(char* script_file);
	BYTE GetGamblingSystemItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY, BYTE btItemType);
	BYTE GamblingOptionRand(int iOptionCount);
	BYTE ExcellentOptionRand();

	bool IsEnabled() { return this->m_bEnabled; }
	int GetGamblingValue() { return this->m_iGamblingValue; }
	int GetGamblingShopTime() { return this->m_iGamblingShopTime; }
};

extern CGamblingItemBag g_GamblingItemBag;
#endif // !defined(AFX_GAMBLINGSYSTEM_H__2874FC7A_D2C6_4A3B_B4F0_3D15EC489DDC__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

