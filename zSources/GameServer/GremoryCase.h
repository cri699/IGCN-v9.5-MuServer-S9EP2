//////////////////////////////////////////////////////////////////////
// GremoryCase.h
#ifndef GREMORYCASE_H
#define GREMORYCASE_H

#include "prodef.h"
#include "zzzitem.h"

#define MAX_GREMORYCASE_STORAGE_TYPES 2
#define MAX_GREMORYCASE_STORAGE_ITEMS 50

enum GREMORYCASE_STORAGE_TYPE
{
	GC_STORAGE_SERVER = 1,
	GC_STORAGE_CHARACTER = 2
};

enum GREMORYCASE_REWARD_SOURCES
{
	GC_REWARD_CHAOS_CASTLE = 1,
	GC_REWARD_BLOOD_CASTLE = 2,
	GC_REWARD_ILLUSION_TEMPLE = 3,
	GC_REWARD_DOPPELGANGER = 4,
	GC_REWARD_CHAOS_CASTLE_SURVIVAL = 5,
	GC_REWARD_TORMENTED_SQUARE = 6,
	GC_REWARD_ILLUSION_TEMPLE_LEAGUE = 7,
	GC_REWARD_CHAOS_CASTLE_SURVIVAL_2 = 8,
	GC_REWARD_TORMENTED_SQUARE_LEAGUE = 9,
	GC_REWARD_CHAOS_CASTLE_UBF = 10,
	GC_REWARD_GM_REWARD = 100
};

enum GREMORYCASE_MESSAGE_CODES
{
	GC_MESSAGE_ITEM_SENT_TO_STORAGE = 0,
	GC_MESSAGE_ITEM_RECEIVED_TO_INVENTORY = 1,
	GC_MESSAGE_ERROR_1 = 2,
	GC_MESSAGE_ERROR_2 = 3,
	GC_MESSAGE_ITEM_WILL_EXPIRE_SOON = 4,
	GC_MESSAGE_STORAGE_NEARLY_FULL = 5,
	GC_MESSAGE_FULL_NOTICE = 6,
	GC_MESSAGE_MUUN_INVENTORY_FULL = 7,
	GC_MESSAGE_EVENT_INVENTORY_FULL = 8,
	GC_MESSAGE_NORMAL_INVENTORY_FULL = 9,
	GC_MESSAGE_HOLDING_ALREADY_ITEM = 10,
	GC_MESSAGE_NO_DATA_ERROR = 11,
	GC_MESSAGE_DB_ERROR = 12,
	GC_MESSAGE_INVENTORY_FULL = 13,
	GC_MESSAGE_CLOSING_GC_WINDOW = 14
};

struct GREMORYCASE_ITEM_DATA
{
	GREMORYCASE_ITEM_DATA()
	{
		this->Clear();
	}

	void Clear()
	{
		this->btRewardInventory = 0;
		this->btRewardSource = 0;
		this->dwItemGUID = (DWORD)-1;
		this->ItemInfo.Clear();
		this->dwAuthCode = 0;
		this->dwReceiveDate = 0;
		this->dwExpireTime = 0;
	}

	BYTE btRewardInventory;
	BYTE btRewardSource;
	DWORD dwItemGUID;
	CItem ItemInfo;
	DWORD dwAuthCode;
	DWORD dwReceiveDate;
	DWORD dwExpireTime;
};

// CLIENT <-> GS PROTOCOL

struct PMSG_GREMORYCASE_ITEM
{
	BYTE btRewardInventory;
	BYTE btRewardSource;
	DWORD dwItemGUID;
	BYTE btItemInfo[MAX_ITEM_INFO];
	DWORD dwAuthCode;
	DWORD dwExpireTime;
};

struct PMSG_RECEIVE_GREMORYCASE_ITEMLIST // 4F:00 GS->CL
{
	PWMSG_HEAD2 h;
	BYTE btCount;
};

#pragma pack (1)
struct PMSG_RECEIVE_GREMORYCASE_ITEM // 4F:01 GS->CL
{
	PBMSG_HEAD2 h;
	PMSG_GREMORYCASE_ITEM m_ReceivedItem;
};
#pragma pack ()

struct PMSG_ADD_GREMORYCASE_ITEM_TO_INVENTORY_REQ // 4F:02 CL->GS
{
	PBMSG_HEAD2 h;
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
};

struct PMSG_ADD_GREMORYCASE_ITEM_TO_INVENTORY_ANS // 4F:02 GS->CL
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btStorageType;
	WORD wItemID;
	DWORD dwAuthCode;
	DWORD dwItemGUID;
};

struct PMSG_REMOVE_ITEM_FROM_GREMORYCASE // 4F:03 GS->CL
{
	PBMSG_HEAD2 h;
	BYTE btStorageType;
	WORD wItemID;
	DWORD dwAuthCode;
	DWORD dwItemGUID;
}; 

struct PMSG_GREMORYCASE_SENT_NOTICE // 4F:04 GS->CL
{
	PBMSG_HEAD2 h;
	BYTE btNoticeIndex;
};

struct PMSG_GREMORYCASE_OPEN_REQ // 4F:05 CL->GS
{
	PBMSG_HEAD2 h;
};

struct PMSG_GREMORYCASE_OPEN_ANS // 4F:05 GS->CL
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

// DS

struct _stReqGremoryCaseItemList
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
};

struct _stGremoryCaseItem
{
	_stGremoryCaseItem()
	{
		this->btStorageType = 0;
		this->btRewardSource = 0;
		this->btItemGUID = 0;
		this->wItemID = 0;
		this->btItemLevel = 0;
		this->btItemDurability = 0;
		this->btItemSkill = 0;
		this->btItemLuck = 0;
		this->btItemOption = 0;
		this->btItemExcOption = 0;
		this->btItemSetOption = 0;
		this->btItemSocketCount = 0;
		this->btItemMainAttribute = 0;
		this->wMuunEvoItemType = 0;
		this->btMuunEvoItemIndex = 0;
		this->dwAuthCode = 0;
		this->iReceiveDate = 0;
		this->iExpireDate = 0;
	}

	BYTE btStorageType;
	BYTE btRewardSource;
	BYTE btItemGUID;
	WORD wItemID;
	BYTE btItemLevel;
	BYTE btItemDurability;
	BYTE btItemSkill;
	BYTE btItemLuck;
	BYTE btItemOption;
	BYTE btItemExcOption;
	BYTE btItemSetOption;
	BYTE btItemSocketCount;
	BYTE btItemMainAttribute;
	WORD wMuunEvoItemType;
	BYTE btMuunEvoItemIndex;
	DWORD dwAuthCode;
	time_t iReceiveDate;
	time_t iExpireDate;
};

struct _stAnsGremoryCaseItemList
{
	PWMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	BYTE btCount;
};

struct _stReqAddItemToGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	_stGremoryCaseItem m_GremoryCaseItem;
};

struct _stAnsAddItemToGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	_stGremoryCaseItem m_GremoryCaseItem;
};

struct _stReqCheckUseItemGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
};

struct _stAnsCheckUseItemGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
};

struct _stReqDeleteItemFromGremoryCase
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
};

#pragma once
class CGremoryCase
{
public:
	CGremoryCase(void);
	virtual ~CGremoryCase(void);

	void CheckStorageExpiredItems();

	void GDReqStorageItemList(int iIndex);
	void DGAnsStorageItemList(LPBYTE lpRecv);
	void GDReqAddItemToGremoryCase(int iIndex, _stGremoryCaseItem stItem, int iDaysToExpire);
	void DGAnsAddItemToGremoryCase(_stAnsAddItemToGremoryCase * lpMsg);
	void GDReqCheckItemUseGremoryCase(int iIndex, WORD wItemID, DWORD dwItemGUID, DWORD dwAuthCode);
	void DGAnsCheckItemUseGremoryCase(_stAnsCheckUseItemGremoryCase * lpMsg);
	void GDReqDeleteItemFromGremoryCase(int iIndex, WORD wItemID, DWORD dwItemGUID, DWORD dwAuthCode);

	void GCSendStorageItemList(int iIndex);
	void GCSendAddItemToGremoryCase(int iIndex, BYTE btStorageType, BYTE btItemArrayIndex);
	void CGReqGetItemFromGremoryCase(PMSG_ADD_GREMORYCASE_ITEM_TO_INVENTORY_REQ * lpMsg, int iIndex);
	void GCSendDeleteItemFromGremoryCase(int iIndex, BYTE btStorageType, int iItemArrayIndex);
	void CGReqOpenGremoryCase(int iIndex);

private:
	void CheckIsInStorageItemAboutToExpire(int iIndex);
	void CheckIsStorageFull(int iIndex);
};

extern CGremoryCase g_GremoryCase;

#endif


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

