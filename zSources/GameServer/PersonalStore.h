//////////////////////////////////////////////////////////////////////
// PersonalStore.h
#ifndef PERSONALSTORE_H
#define PERSONALSTORE_H

#include "prodef.h"
#include "zzzitem.h"
#include "user.h"

#pragma once

struct PMSG_REQ_PSHOP_SETITEMPRICE
{
	PBMSG_HEAD2 h;
	BYTE btItemPos;	// 4
	BYTE sItemPrice1;	// 5
	BYTE sItemPrice2;	// 6
	BYTE sItemPrice3;	// 7
	BYTE sItemPrice4;	// 8
	BYTE sJewelOfBlessPrice1;
	BYTE sJewelOfBlessPrice2;
	BYTE sJewelOfSoulPrice1;
	BYTE sJewelOfSoulPrice2;
	BYTE sJewelOfChaosPrice1;
	BYTE sJewelOfChaosPrice2;
};


struct PMSG_REQ_PSHOP_OPEN
{
	PBMSG_HEAD2 h;
	char szPShopText[36];	// 4
};


struct PMSG_REQ_BUYLIST_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btName[10];	// 6
};

#pragma pack (1)
struct PMSG_REQ_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btName[10];	// 6
	BYTE btItemPos;	// 10
	BYTE btBuyKind;
	int iItemType;
	int PShopItemMoneyValue;
	short PShopItemBlessJewelValue;
	short PShopItemSoulJewelValue;
	short PShopItemChaosJewelValue;
};
#pragma pack ()

struct PMSG_REQ_PSHOPDEAL_CLOSE
{
	PBMSG_HEAD2 h;
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btName[10];	// 6
};

struct PMSG_ANS_PSHOP_VIEWPORT_NOTIFY 
{
	struct PWMSG_HEAD2 h;
	unsigned char btCount;
};

struct PMSG_PSHOP_VIEWPORT_NOTIFY 
{
	unsigned char btNumberH;
	unsigned char btNumberL;
	char szPShopText[36];
};

struct PMSG_ANS_PSHOP_SETITEMPRICE
{
	PBMSG_HEAD2 h;	// C1:3F:01
	BYTE btResult;	// 4
	BYTE btItemPos;	// 5
};

struct PMSG_ANS_PSHOP_TEXT_CHANGED
{
	PBMSG_HEAD2 h;	// C1:3F:10
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btPShopText[36];	// 6
	BYTE btName[10];	// 2A
};

struct PMSG_ANS_PSHOP_OPEN
{
	PBMSG_HEAD2 h;	// C1:3F:02
	BYTE btResult;	// 4
};

struct PMSG_ANS_PSHOP_CLOSE
{
	PBMSG_HEAD2 h;	// C1:3F:03
	BYTE btResult;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
};

struct PMSG_BUYLIST_FROM_PSHOP
{
	BYTE Pos;	// 0
	BYTE ItemInfo[MAX_ITEM_INFO];	// 1
	int PShopItemValue;	// 8
	short BlessValue;
	short SoulValue;
	short ChaosValue;
};

struct PMSG_ANS_BUYLIST_FROM_PSHOP
{
	PWMSG_HEAD2 h;	// C2:3F:[05:13]
	BYTE Result;	// 5
	BYTE NumberH;	// 6
	BYTE NumberL;	// 7
	BYTE btName[10];	// 8
	char szPShopText[36];	// 12
	BYTE btCount;	// 36
};

struct PMSG_ANS_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;	// C1:3F:06
	BYTE Result;	// 3
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE cItemInfo[MAX_ITEM_INFO];	// 7
	BYTE btItemPos;	// E
};

struct PMSG_ANS_SOLDITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;	// C1:3F:08
	BYTE btItemPos;	// 4
	BYTE btName[10];	// 5
};

struct PMSG_REQ_DEALER_CLOSED_SHOP
{
	PBMSG_HEAD2 h;	// C1:3F:12
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
};

struct PMSG_REQ_SEARCH_ITEM_PSHOP
{
	PBMSG_HEAD2 h;
	int iLastCount;
	short sSearchItem;
};

struct PMSG_REQ_PSHOP_LOG
{
	PBMSG_HEAD2 h;
	int iTargetIndex;
	BYTE btLogKind;
};

struct PSHOP_ITEMVALUE_INFO_DS
{
	int nPShopItemInvenNum;
	UINT64 ItemSerial;
	int nMoney;
	short sBlessJewelValue;
	short sSoulJewelValue;
	short sChaosJewelValue;
};

struct PSHOP_ITEMVALUE_INFO
{
	int nPShopItemInvenNum;
	int Empty;
	int nMoney;
	short sBlessJewelValue;
	short sSoulJewelValue;
	short sChaosJewelValue;
};

struct PMSG_ANS_PSHOPITEMVALUE_INFO
{
	PWMSG_HEAD h;
	BYTE btItemCnt;
	int iUserIndex;
	PSHOP_ITEMVALUE_INFO_DS PShopItemValueInfo[32];
};

struct PMSG_UPDATE_PSHOPITEMVALUE_INFO
{
	PWMSG_HEAD h;
	BYTE btItemCnt;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	PSHOP_ITEMVALUE_INFO_DS PShopItemValueInfo[32];
};

struct PMSG_REQ_PSHOPITEMVALUE_INFO
{
	PBMSG_HEAD h;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	int iUserIndex;
};

struct PMSG_DEL_PSHOPITEM
{
	PBMSG_HEAD h;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	int nPShopItemInvenNum;
};

struct PMSG_MOVE_PSHOPITEM
{
	PBMSG_HEAD h;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	int nOldPShopItemInvenNum;
	int nNewPShopItemInvenNum;
};

struct PMSG_PSHOPITEMVALUE_INFO_COUNT
{
	PWMSG_HEAD head;
	BYTE Result;
	BYTE btMoneyCommisionRate;
	BYTE btJewelCommisionRate;
	BYTE btItemCnt;
};

struct PMSG_SEARCH_ITEM_PSHOP
{
	BYTE btNumberH;
	BYTE btNumberL;
	char szName[MAX_ACCOUNT_LEN + 1];
	char szPShopText[37];
};

#pragma pack (1)
struct PMSG_SEARCH_ITEM_PSHOP_COUNT
{
	PWMSG_HEAD2 h;
	int iPShopCnt;
	BYTE btContinueFlag;
};
#pragma pack ()

class CPersonalStore
{
public:
	CPersonalStore();
	virtual ~CPersonalStore();

	void CGPShopReqSetItemPrice(PMSG_REQ_PSHOP_SETITEMPRICE* lpMsg, int aIndex);
	void CGPShopAnsSetItemPrice(int aIndex, BYTE btResult, BYTE btItemPos);
	void CGPShopReqOpen(PMSG_REQ_PSHOP_OPEN* lpMsg, int aIndex);
	void CGPShopAnsOpen(int aIndex, BYTE btResult);
	void CGPShopReqClose(int aIndex);
	void CGPShopAnsClose(int aIndex, BYTE btResult);
	void CGPShopReqBuyList(PMSG_REQ_BUYLIST_FROM_PSHOP* lpMsg, int aSourceIndex);

	void CGPShopAnsBuyList(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend);
	void CGPShopReqBuyItem(LPBYTE lpRecv, int aSourceIndex);

	void CGPShopAnsBuyItem(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult);
	void CGPShopReqCloseDeal(PMSG_REQ_PSHOPDEAL_CLOSE* lpMsg, int aIndex);
	void CGPShopAnsSoldItem(int aSourceIndex, int aTargetIndex, int iItemPos);
	void CGPShopAnsDealerClosedShop(int aSourceIndex, int aTargetIndex);

	void gObjInventoryItemSet_PShop(int aIndex, int itempos, unsigned char set_byte);
	void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);

	void PShop_ViewportListRegenarate(short aIndex);
	bool PShop_CheckInventoryEmpty(short aIndex);

	// Season 8
	void CGReqSearchItemInPShop(PMSG_REQ_SEARCH_ITEM_PSHOP *lpMsg, int aIndex);
	void CGReqPShopLog(PMSG_REQ_PSHOP_LOG *lpMsg, int aIndex);

	void GDRequestPShopItemValue(OBJECTSTRUCT* lpObj, char *szAccountID);
	void GDUpdatePShopItemValue(OBJECTSTRUCT* lpObj);
	void GDAllSavePShopItemValue(OBJECTSTRUCT* lpObj);
	void GDDelPShopItemValue(OBJECTSTRUCT* lpObj, int nPShopItemInvenNum);
	void GDMovePShopItem(OBJECTSTRUCT* lpObj, int nOldPShopItemInvenNum, int nNewPShopItemInvenNum);
	void GDAnsPShopItemValue(PMSG_ANS_PSHOPITEMVALUE_INFO *lpMsg);

	void GCPShopItemValueInfo(int aIndex);

	void GCPShop_AllInfo(short aIndex, int iLastUserCount);
	void GCPShop_SearchItem(short aIndex, WORD sSearchItem, int iLastUserCount);

	bool PShop_CheckExistItemInInventory(short aIndex, WORD sItemType);
};

extern CPersonalStore g_PersonalStore;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

