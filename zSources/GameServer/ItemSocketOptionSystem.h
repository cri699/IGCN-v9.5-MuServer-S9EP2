// ItemSocketOptionSystem.h: interface for the CItemSocketOptionSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSOCKETOPTIONSYSTEM_H__A18208BC_3285_4EB6_9540_22B7877904EF__INCLUDED_)
#define AFX_ITEMSOCKETOPTIONSYSTEM_H__A18208BC_3285_4EB6_9540_22B7877904EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "itemsocketoptiondefine.h"
#include "zzzitem.h"
#include "TRandomPoolMgr.h"
#include "ItemSeedSphere.h"

class OBJECTSTRUCT;
typedef OBJECTSTRUCT* LPOBJECTSTRUCT;

typedef struct BONUS_SOCKET_OPTION
{
	BYTE OptionIndex;
	BYTE EffectType;
	BYTE ItemTypeBegin;
	BYTE ItemTypeEnd;
	BYTE OptionValueType;
	WORD OptionValue;
	int ExtractRate;
	BYTE SocketSlotType[5];
	BYTE SocketSlotCount;

	void Clear()
	{
		OptionIndex = BONUS_OPTION_WEAPON_ATTACK_POWER;
		EffectType = SEED_EFFECT_TYPE_FIRE_BEGIN;
		ItemTypeBegin = 0;
		ItemTypeEnd = 0;
		OptionValueType = 0;
		OptionValue = 0;
		ExtractRate = 0;

		for(int i=0; i<5; i++)
		{
			SocketSlotType[i] = 0;
		}
		
		SocketSlotCount = 0;
	}

}BONUS_SOCKET_OPTION, *LPBONUS_SOCKET_OPTION;

struct SET_SOCKET_OPTION
{
	BYTE OptionIndex;
	BYTE OptionValueType;
	WORD OptionValue;
	BYTE EffectType;
	int ExtractRate;
	BYTE SeedTypeTable[6];
	void Clear();
};

struct CUSTOM_SOCKET_LIST
{
	int iItemCode;
};

class CItemSocketOptionSystem  
{
	CSeedItem m_SeedData[50];
	CSphereItem m_SphereData[5];

	BONUS_SOCKET_OPTION m_BonusSocketOptionData[12];
	SET_SOCKET_OPTION m_SetSocketOptionData[2];

	TRandomPoolMgr m_SeedExtractPool;

	TRandomPoolMgr m_TwoHandWeaponMakeSocketSlotCountPool;
	std::map<int, TRandomPoolMgr> m_mapSocketSlotCountPool;

	BOOL m_SphereDropOn;
	int m_SphereDropRate[5];
	int m_SphereDropLevel[5];

	BOOL m_TetraDropOn;
	int m_TetraDropRate[3];
	int m_TetraDropMinLevel[3];
	int m_TetraDropMaxLevel[3];

	int m_MakeSocketSlotRate[5];
	int m_TwoHandWeaponMakeSocketSlotRate[5];

public:
	void Initialize();
	
	bool LoadOptionScript(char* pchFileName);
	bool LoadSocketSlotRateFile(char *pchFileName);
	void LoadScript();
	
	bool IsEnableSocketItem(int ItemCode);
	bool IsEnableSocketItem(CItem* lpItemData);
	
	BYTE GetEmptySlotCount(CItem* lpItemData);
	bool GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE& pbSeedType, BYTE& pbEffectValueType, WORD& pwEffectValue);
	void GetSocketOption(CItem* lpItemData, BYTE *pbSocketOption, BYTE& pbSocketBonusOption);
	
	bool IsSocketItem(CItem* lpItemData);
	bool IsSeedItem(int ItemCode);
	bool IsSphereItem(int ItemCode);
	bool IsSeedSphereItem(int ItemCode);
	
	BYTE GetSeedIndex(int ItemCode, BYTE ItemLevel);
	BYTE GetSphereLevel(int ItemCode);
	
	bool CheckItemForSameSeedType(CItem* lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel);
	
	BYTE GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel);
	DWORD GetSeedSphereValue(BYTE btSeedSphereIndex);
	
	bool CheckSetSeedSphereItemType(CItem* lpItemData, BYTE btSeedSphereIndex);
	
	bool IsEnableDropSocketItemMap(int iMapIndex);
	
	int GetSphereDropInfo(int iMonsterLevel);
	int GetTetraDropInfo(int iMonsterLevel);
	
	BYTE GetSeedIndexForSeedSpearItemCode(WORD ItemCode, BYTE ItemLevel);
	BYTE GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel);
	
	bool MakeSeedSphere(CSeedSphereItem* lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel);
	
	CSeedItem* GetSeedData(BYTE btSeedIndex);
	CSphereItem* GetSphereData(BYTE btSphereLevel);
	
	BYTE GetExtractSeed();
	BYTE GetSocketType(BYTE btSeedSphereIndex);
	int GetSeedSphereLevel(BYTE btSeedSphereIndex);
	
	LPBONUS_SOCKET_OPTION GetBonusSocketOption(BYTE btBonusSocketOptionIndex);
	
	int ApplySeedSphereEffect(LPOBJECTSTRUCT lpObj);
	void ApplyBonusOptionEffect(LPOBJECTSTRUCT lpObj);
	void ApplySetOptionEffect(LPOBJECTSTRUCT lpObj);
	
	bool SetApplySocketEffect(LPOBJECTSTRUCT lpObj);
	void SetClearSocketEffect(LPOBJECTSTRUCT lpObj);
	
	void SetEffect(LPOBJECTSTRUCT lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void ClearEffect(LPOBJECTSTRUCT lpObj, LPSOCKET_OPTION_LIST lpOptionData);
	
	void _SetFireEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetWaterEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetIceEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetWindEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetThunderEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetEarthEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetUniqueEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetSetEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	
	int CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType);
	
	void SetRequireStatOption(CItem* lpItemdata);
	
	void MakeSocketSlot(CItem* lpItemData, BYTE btSocketSlotCount);
	
	BYTE GetMakeSocketSlotCount(int iItemCode);
	
	void ClearUserData(LPOBJECTSTRUCT lpObj);

	bool SetSocketSlot(CItem* lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex);
	bool SetSocketSlot(CItem* lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex);
	bool SetSocketSlot(CItem* lpTargetItem, CItem* lpSeedSphere, BYTE btSocketSlotIndex);
	
	bool SetBonusSocketOption(CItem* lpItemData);
	
	bool ClearSocketSlot(CItem* lpTargetItem, BYTE btSocketSlotIndex);

	CItemSocketOptionSystem();
	virtual ~CItemSocketOptionSystem();

	int m_RequireMoneyForSeedExtract;
	int m_RequireMoneyForSeedSphereComposite;
	int m_RequireMoneyForSeedSphereSet;
	int m_RequireMoneyForSeedSphereRemove;
};

extern	CItemSocketOptionSystem	g_SocketOptionSystem;

#endif // !defined(AFX_ITEMSOCKETOPTIONSYSTEM_H__A18208BC_3285_4EB6_9540_22B7877904EF__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

