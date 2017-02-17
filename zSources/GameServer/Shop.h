//////////////////////////////////////////////////////////////////////
// Shop.h
#ifndef ___SHOP_H
#define ___SHOP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zzzitem.h"

#define MAX_ITEM_IN_SHOP 120

class CShop
{

public:

	CShop();
	virtual ~CShop();

	void Init();
	BOOL LoadShopItem(char* filename);
	int InsertItem(int type, int index, int level, int dur, int op1, int op2, int op3, int newop, int setop, int socketcount, int elemental, int serial);
	int InentoryMapCheck(int sx, int sy, int width,  int height);

public:

	BYTE ShopInventoryMap[MAX_ITEM_IN_SHOP];	// 4
	int ItemCount;	// 7C
	CItem m_item[MAX_ITEM_IN_SHOP];	// 80
	BYTE SendItemData[MAX_ITEM_IN_SHOP*(MAX_ITEM_INFO+1)];	// 5300
	int SendItemDataLen;	// 56C0
};

struct SHOP_DATA
{
	int		iNpcIndex;
	BYTE	btMapNumber;
	BYTE	btPosX;
	BYTE	btPosY;
	BYTE	btDir;
	BYTE	btOnlyForGameMaster;
	BYTE	btIsMossMerchant;
	BYTE	btReqVipLevel;
	BYTE	btBattleCoreShop;
	CShop	m_ShopData;
};

class CShopMng
{
	
public:

	CShopMng();
	virtual ~CShopMng();

	void LoadShopList(char* filename);
	SHOP_DATA * GetShop(OBJECTSTRUCT * lpObj, OBJECTSTRUCT * lpNpc);
	bool IsShopNpc(int iNpcIndex, BYTE btMapNumber, BYTE btPosX, BYTE btPosY);
	void SetShopNpcs();

private:
	
	std::vector<SHOP_DATA> m_vecShopData;
};

extern CShopMng g_ShopMng;

#define MAX_CANCEL_ITEMS_SALE	5

struct PMSG_SHOP_REBUY_LIST_REQ
{
	PBMSG_HEAD2 h;
};

struct PMSG_SHOP_REBUY_LIST_ANS
{
	PWMSG_HEAD2 h;
	BYTE btResult;
	BYTE btCount;
};

#pragma pack (1)
struct SHOP_REBUY_LIST_ITEM
{
	int iLeftTime;
	BYTE btItemInfo[MAX_ITEM_INFO];
	WORD wItemCount;
	DWORD dwItemPrice;
	BYTE btItemNumber;
};
#pragma pack ()

struct PMSG_REQ_REBUY_ITEM
{
	PBMSG_HEAD2 h;
	BYTE btItemIndex;
	DWORD dwLeftTime;
	DWORD dwItemPrice;
	WORD wItemCode;
	BYTE btItemNumber;
};

struct PMSG_ANS_REBUY_ITEM
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct SDHP_REQ_SHOP_REBUY_LIST
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	time_t CurrTime;
};

struct SDHP_SHOP_REBUY_ITEM
{
	BYTE btItemNumber;
	WORD wItemCount;
	time_t SellDate;
	time_t SellExpireDate;
	DWORD dwSellPrice;
	WORD wItemCode;
	BYTE btItemLevel;
	BYTE btItemOp1;
	BYTE btItemOp2;
	BYTE btItemOp3;
	BYTE btItemDur;
	BYTE btItemExcOp;
	BYTE btItemSetOp;
	BYTE btItemHarmonyOp;
	BYTE btItemOpEx;
	BYTE btItemSocketOp[5];
	BYTE btItemBonusSocketOp;
	UINT64 ItemSerial;
}; 

struct SDHP_ANS_SHOP_REBUY_LIST
{
	PWMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	int iCount;
};

struct SDHP_REQ_SHOP_REBUY_ADD_ITEM
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	time_t SellDate;
	time_t SellExpireDate;
	DWORD dwSellPrice;
	WORD wItemCode;
	BYTE btItemLevel;
	BYTE btItemOp1;
	BYTE btItemOp2;
	BYTE btItemOp3;
	BYTE btItemDur;
	BYTE btItemExcOp;
	BYTE btItemSetOp;
	BYTE btItemHarmonyOp;
	BYTE btItemOpEx;
	BYTE btItemSocketOp[5];
	BYTE btItemBonusSocketOp;
	UINT64 ItemSerial;
};

struct SDHP_REQ_SHOP_REBUY_GET_ITEM
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	DWORD dwItemPrice;
	WORD wItemCode;
	BYTE btItemNumber;
};

struct SDHP_ANS_SHOP_REBUY_GET_ITEM
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	BYTE btItemNumber;
	WORD wItemCount;
	DWORD dwSellPrice;
	WORD wItemCode;
	BYTE btItemLevel;
	BYTE btItemOp1;
	BYTE btItemOp2;
	BYTE btItemOp3;
	BYTE btItemDur;
	BYTE btItemExcOp;
	BYTE btItemSetOp;
	BYTE btItemHarmonyOp;
	BYTE btItemOpEx;
	BYTE btItemSocketOp[5];
	BYTE btItemBonusSocketOp;
	UINT64 ItemSerial;
};

struct SDHP_REQ_SHOP_REBUY_DELETE_ITEM
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	DWORD dwItemPrice;
	WORD wItemCode;
	BYTE btItemNumber;
};

struct USER_SHOP_REBUY_ITEM
{
	USER_SHOP_REBUY_ITEM()
	{
		this->Clear();
	}

	void Clear()
	{
		this->btUsed = FALSE;
		this->wItemCode = 0xFFFF;
		this->iLeftTime = 0;
		memset(this->btItemInfo, 0x00, MAX_ITEM_INFO);
		this->wItemCount = 0;
		this->dwItemPrice = 0;
		this->btItemNumber = 0;
	}

	BYTE btUsed;
	WORD wItemCode;
	int iLeftTime;
	BYTE btItemInfo[MAX_ITEM_INFO];
	WORD wItemCount;
	DWORD dwItemPrice;
	BYTE btItemNumber;
};

class CCancelItemSale
{

public:

	CCancelItemSale();
	virtual ~CCancelItemSale();

	void CGReqSoldItemList(int iIndex);
	void GCAnsSoldItemList(int iIndex);
	void CGReqEndCancelItemSale(int iIndex);
	void CGReqReBuyItem(PMSG_REQ_REBUY_ITEM *lpMsg, int iIndex);

	void GDReqSoldItemList(int iIndex);
	void DGAnsSoldItemList(LPBYTE lpRecv);
	void GDReqAddItemToList(int iIndex, CItem Item, DWORD dwSellPrice);
	void GDReqGetReBuyItem(int iIndex, BYTE btItemNumber, WORD wItemCode, DWORD dwItemPrice);
	void DGAnsGetReBuyItem(SDHP_ANS_SHOP_REBUY_GET_ITEM *lpMsg);
	void GDReqDeleteSoldItem(int iIndex, BYTE btItemNumber, WORD wItemCode, DWORD dwItemPrice);
};

extern CCancelItemSale g_CancelItemSale;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

