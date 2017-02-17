//////////////////////////////////////////////////////////////////////
// MuunSystem.h
#ifndef MUUNSYSTEM_H
#define MUUNSYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MuunInfoMng.h"
#include "DSProtocol.h"
#include "ObjBaseAttack.h"

#define MAX_MUUN_ENERGY_GENERATOR_POINT		8
#define MAX_MUUN_EXCHANGE_INFO				10
#define MAX_MUUN_EXCHANGE_ITEMBAG			20

struct _tagEnergyGeneratorPoint
{
	_tagEnergyGeneratorPoint()
	{
		this->iRank = -1;
		this->iRankPt = 0;
		this->iMuunLv = -1;
		this->iMuunLvPt = 0;
	}

	int iRank;
	int iRankPt;
	int iMuunLv;
	int iMuunLvPt;
};

struct _tagMuunExchangeInfo
{
	_tagMuunExchangeInfo()
	{
		this->iItemType = 0;
		this->iItemIndex = 0;
		this->iInvenChk = 0;
		this->iItemCnt = 0;
		this->iItemBagIndex = 0;
	}

	int iItemType;
	int iItemIndex;
	int iInvenChk;
	int iItemCnt;
	int iItemBagIndex;
};

struct _tagMuunExchangeItembag
{
	_tagMuunExchangeItembag()
	{
		this->iIndex = 0;
		this->iItemType = 0;
		this->iItemIndex = 0;
		this->iMinLv = 0;
		this->iMaxLv = 0;
		this->iSkill = 0;
		this->iLuck = 0;
		this->iAddOpt = 0;
		this->iExItem = 0;
		this->iStoneMuunItemType = 0;
		this->iStoneMuunItemIndex = 0;
		this->iInvenChk = 0;
	}

	int iIndex;
	int iItemType;
	int iItemIndex;
	int iMinLv;
	int iMaxLv;
	int iSkill;
	int iLuck;
	int iAddOpt;
	int iExItem;
	int iStoneMuunItemType;
	int iStoneMuunItemIndex;
	int iInvenChk;
};

struct MUUNITEMDATA
{
	void Clear()
	{
		this->bIsUsed = 0;
		this->dwSerial = 0;
		this->pCMuunInfo = 0;
		this->wItemCode = 0;
		this->btOptionType = 0;
	}

	bool bIsUsed;
	WORD wItemCode;
	BYTE btOptionType;
	UINT64 dwSerial;
	CMuunInfo *pCMuunInfo;
};

struct MUUN_ITEM_PERIOD_DATA
{
	void Clear()
	{
		this->bIsUsed = false;

		for (int i = 0; i < MUUN_INVENTORY_SIZE; i++)
		{
			this->ItemData[i].Clear();
		}
	}

	bool bIsUsed;
	char btUsedDataCount;
	WORD wUserIndex;
	DWORD dwUserGUID;
	OBJECTSTRUCT *lpUserObj;
	char chAccountID[MAX_ACCOUNT_LEN+1];
	char chCharacterName[MAX_ACCOUNT_LEN+1];
	MUUNITEMDATA ItemData[MUUN_INVENTORY_SIZE];

};

class CMuunAttack : public CObjBaseAttack
{
public:
	CMuunAttack();
	virtual ~CMuunAttack();

	void SendAttackMsg(int aIndex, int aTargetIndex, int SubCode, int SubCode2);
	void SkillProc(LPOBJ lpObj);

	bool DamageAbsorb(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode2);
	bool Stun(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode2);
	bool Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode2);

	int GetAttackDamage(LPOBJ lpObj, int targetDefense, int SubCode2, LPOBJ lpTargetObj);
	int GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
};

class CMuunSystem
{
public:

	CMuunSystem();
	virtual ~CMuunSystem();

	void Initialize();

	bool LoadScriptMuunSystemInfo(char *lpszFileName);
	bool LoadScriptMuunSystemOption(char *lpszFileName);
	bool LoadScriptMuunExchange(char *lpszFileName);

	void MuunItemDamage(OBJECTSTRUCT *lpObj, int damage);
	int MuunItemEquipment(int aIndex, int iPos, int iSource);

	void SetMuunItemAddPeriodData(OBJECTSTRUCT *lpObj, int iMuunItemNum, UINT64 dwSerial);
	bool SetUserMuunEffect(OBJECTSTRUCT *lpObj, int iMuunItmeNum, int iMuunLv, int iEquipPos);
	void SetAddOptTypeValue(_tagMUUN_EFFECT_LIST *pUserMuunEffect);
	bool RemoveUserMuunEffect(OBJECTSTRUCT *lpObj, int iEquipPos);

	bool GetMuunItemValueOfOptType(OBJECTSTRUCT *lpObj, int iMuunOtpIndex, int *EffectValue1, int *EffectValue2);

	void GDReqLoadMuunInvenItem(OBJECTSTRUCT *obj);
	void DGLoadMuunInvenItem(_tagSDHP_ANS_DBMUUN_INVEN_LOAD *lpMsg);
	void GDReqSaveMuunInvenItem(OBJECTSTRUCT *obj);

	void GCSendConditionStatus(int aIndex, int iPos, int iStatus);

	bool IsMuunItem(CItem *pCitem);
	bool IsMuunItem(int iItemNum);

	bool IsStoneofEvolution(CItem *pCitem);
	bool IsStoneofEvolution(int iItemNum);

	int GetEvolutionMuunItemIndex(int iItemNum);
	int GetBeforeEvolutionMuunItemIndex(int iItemNum);
	int GetMuunRankOfMuunInfo(int iItemNum);

	void CGMuunInventoryUseItemRecv(PMSG_USEITEM_MUUN_INVEN *lpMsg, int aIndex);

	bool MuunItemEvolution(OBJECTSTRUCT *lpObj, int source, int target);
	bool MuunItemLevelUp(OBJECTSTRUCT *lpObj, int source, int target);
	bool MuunItemLifeGem(OBJECTSTRUCT *lpObj, int source, int target);
	bool MuunItemEnergyGenerator(OBJECTSTRUCT *lpObj, int source, int target);

	void GCMuunInventoryUseItemResult(int aIndex, int iUseType, int iResult);

	bool RemoveMuunItemPeriodInfo(OBJECTSTRUCT *lpObj);

	bool IsCorrectUser(OBJECTSTRUCT *lpObj);

	int AddMuunItmePeriodData(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial, int iDuration, CMuunInfo *pCMuunInfo);
	int AddMuunItemPeriodInfo(OBJECTSTRUCT *lpObj);
	bool ClearPeriodMuunItemData(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial);
	bool RemovePeriodMunnItemData(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial);

	bool SetDisableMuunItemToExpire(OBJECTSTRUCT *lpObj, int iInventoryPosition);

	void CheckMuunItemConditionLevelUp(OBJECTSTRUCT *lpObj);
	void CheckMuunItemMoveMapConditionMap(OBJECTSTRUCT *lpObj, int iMapNumber);
	void CheckEquipMuunItemCondition(OBJECTSTRUCT *lpObj);
	void CheckEquipMuunItemConditionProc(OBJECTSTRUCT *lpObj);

	void CalCharacterStat(int aIndex, CMuunInfo *pCMuunInfo);
	void CalCharacterStat(int aIndex, int iOptType);

	int CheckMuunItemCondition(OBJECTSTRUCT *obj, _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo);
	int CheckMuunItemConditionProc(_tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo);
	int ChkMuunOptConditionTime(_tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo);
	int ChkMuunOptConditionDay(_tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo);
	int ChkMuunOptConditionLevel(OBJECTSTRUCT *obj, _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo);
	int ChkMuunOptConditionMap(OBJECTSTRUCT *obj, _tagMUUN_EFFECT_LIST *pUserMuunEffect, CMuunInfo *pCMuunInfo);

	int GetItemFromMuunInventory(OBJECTSTRUCT *lpObj, WORD wItemCode, UINT64 dwSerial);

	void CGMuunExchangeItem(PMSG_REQ_MUUN_EXCHANGE *lpMsg, int aIndex);

	bool ChkMuunExchangeInvenNeedItem(int & iItemCnt, int iInvenPos, int iNeedItemCnt, int iInvenItemNum, int iNeedItemNum, int *ItemPos);
	bool ChkMuunExchangeInvenNeedItem(LPOBJ lpObj, int iSelect, int *ItemPos);

	void SendMsgMuunExchange(int aIndex, int iResult);
	bool ChkMuunExchangeInvenEmpty(LPOBJ lpObj, int iSelect);

	bool GDMuunExchangeInsertInven(LPOBJ lpObj, int iSelect);
	BYTE DGMuunExchangeInsertInven(LPOBJ lpObj, CItem CreateItem, int iSelect);
	bool ChkAndDelItemMuunExchange(LPOBJ lpObj, int iSelect);

	void SetTarget(LPOBJ lpObj, int aTargetIndex);
	void ReSetTarget(LPOBJ lpObj, int aTargetIndex);

	void CGReqRideSelect(PMSG_MUUN_RIDE_SELECT *lpMsg, int aIndex);

	void SkillProc(LPOBJ lpObj);

	time_t GetCurrentDate();
	time_t GetExpireDate(int iDuration);
	bool CheckExpireDate(time_t dwItemExpireDate);
	time_t GetLeftDate(time_t ExpireDate);
	bool IsMuunExpireDate(int iType);

	void Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf *lpMagic, int SubCode, int SubCode2);

	bool CheckAddOptionExpireDate(time_t StartDate, time_t EndDate);
	void MsgIsMuunItemActive(OBJECTSTRUCT *lpObj, int iPos);

	void CheckMuunItemPeriodData(OBJECTSTRUCT *lpObj);

	CMuunInfoMng					m_MuunInfoMng;
	MUUN_ITEM_PERIOD_DATA			*m_MuunItemPeriodData;
	CMuunAttack						m_MuunAttack;
	_tagEnergyGeneratorPoint		m_stEnergyGeneratorPoint[MAX_MUUN_ENERGY_GENERATOR_POINT];
	int								m_iStoneOfEvolutionPt;
	_tagMuunExchangeInfo			m_stMuunExchangeInfo[MAX_MUUN_EXCHANGE_INFO];
	_tagMuunExchangeItembag			m_stMuunExchangeItembag[MAX_MUUN_EXCHANGE_ITEMBAG];

}; extern CMuunSystem g_CMuunSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

