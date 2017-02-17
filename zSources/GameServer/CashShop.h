// CashShop.h: interface for the CItemShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHSHOP_H__B57A9271_3D21_40E0_B132_82F87B122CB4__INCLUDED_)
#define AFX_CASHSHOP_H__B57A9271_3D21_40E0_B132_82F87B122CB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"

#pragma pack(1)

struct ITEMSHOP_ITEMINFO
{
	WORD	wGUID;
	WORD	wID;
	WORD	wItemGroup;
	WORD	wItemType;
	BYTE	btItemLevel;
	BYTE	btItemDurability;
	BYTE	btItemSkill;
	BYTE	btItemLuck;
	BYTE	btItemOption;
	BYTE	btItemExOption;
	BYTE	btItemSetOption;
	BYTE	btItemSocketCount;
	BYTE	btItemAttribute;
	BYTE	btItemType;
	DWORD	dwItemPeriodTime;
};

struct ITEMSHOP_ITEMLIST
{
	BYTE	btGuid;
	WORD	wItemIndex;
	WORD	wItemSubIndex;
	WORD	wItemOptionSelect;
	WORD	wPackageID;
	BYTE	btCoinType;
	DWORD	wPrice;
	WORD	wItemInfoGUID;
	WORD	wItemInfoID;
	DWORD	ItemCategory;
	DWORD	ItemGoblinPoint;
	BYTE	btEnableForSale;
	BYTE	btEnableForGift;
	BYTE	btRandomItemPackage;
};

struct ITEMSHOP_PACKAGELIST
{
	BYTE	Guid;
	BYTE	PackageID;
	BYTE	ItemSeqNum;
	WORD	wItemInfoGUID;
	WORD	wItemInfoID;
};

struct PMSG_REQ_INGAMESHOPINIT
{
	PBMSG_HEAD2 h;
};

#pragma pack (1)
struct PMSG_ANS_INGAMESHOP_POINT
{
	PBMSG_HEAD2 h;
	BYTE btViewType;
	long double dwTotalCash;
	long double dwCash_C;
	long double dwCash_P;
	long double dwTotalPoint;
	long double GoblinPoint;
};
#pragma pack ()

struct PMSG_ANS_INGAMESHOP_ITEMCOUNT
{
	PBMSG_HEAD2 h;
	WORD TotalItemCount;
	WORD CurrentItemCount;
	WORD PageIndex;
	WORD TotalPage;
};
#pragma pack (1)
struct PMSG_ANS_INGAMESHOP_ITEM
{
	PBMSG_HEAD2	h;
	int	UniqueCode;
	int AuthCode;
	int UniqueValue1;
	int UniqueValue2;
	int UniqueValue3;
	int Unk1;
	int Unk2;
	BYTE Unk3;
};

struct PMSG_ANS_INGAMESHOP_GIFT
{
	PBMSG_HEAD2 h;
	int itemGuid;
	int unk1;
	int unk2_type;
	int groupId;
	int itemId;
	double value;
	BYTE type;

	char szUser[11];
	char szText[200];
};

struct PMSG_REQ_INGAMESHOPOPEN
{
	PBMSG_HEAD2 head;
	BYTE btShopOpenType;	// 4
};

struct PMSG_REQ_INGAMESHOP_INVENTORY
{
	PBMSG_HEAD2	h;
	int	PageIndex;
	BYTE InventoryType;
};

struct PMSG_REQ_INGAMESHOP_ITEMBUY
{
	PBMSG_HEAD2	h;
	int ItemIndex;
	int Category;
	int ItemOpt;
	short ItemID;
	int ItemSubIndex;
};

struct PMSG_ANS_INGAMESHOP_ITEMBUY
{
	PBMSG_HEAD2 h;
	BYTE Result;
	int Unknown;
};

struct PMSG_ANS_INGAMESHOP_ITEMGIFT
{
	PBMSG_HEAD2 h;
	BYTE Result;
	int Unknown;
};

struct PMSG_REQ_INGAMESHOP_ITEMUSE
{
	PBMSG_HEAD2	h;
	int UniqueCode;
	int AuthCode;
	WORD ItemID;
	BYTE Unk;
};

struct PMSG_ANS_INGAMESHOP_ITEMUSE
{
	PBMSG_HEAD2	h;
	BYTE result;
};


#pragma pack ()

#pragma pack (1)
struct PMSG_REQ_CASHITEM_GIFT
{
	PBMSG_HEAD2 h;
	int ItemIndex;
	int Category;
	int ItemOpt;
	int UNK;
	short ItemID;
	int ItemSubIndex;
	BYTE UNK2;
	char szName[11];
	char szText[200];
};
#pragma pack ()
struct PMSG_ANS_CASHITEM_GIFT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};
#pragma pack ()

struct _VER_STRUCT
{
	WORD Ver1;
	WORD Ver2;
	WORD Ver3;
};

struct PMSG_SHOP_VERSION
{
	PBMSG_HEAD2 h;
	_VER_STRUCT ver;
};

struct GP_PERMONSTER_DATA
{
	WORD MonsterID;
	WORD MapNumber;
	WORD Coins;
	WORD CoinType;
};

enum CASH_SHOP_ITEM_USE_RESULT
{
	CS_USE_ITEM_SUCCESS = 0x00,
	CS_USE_ITEM_NOT_EXISTS = 0x01,
	CS_USE_ITEM_CANNOT_USE = 0x02,
	CS_USE_ITEM_TICKET_ERROR = 0x03,
	CS_USE_ITEM_INTERNAL_ERROR = 0x04
};

class CItemShop
{

public:

  	CItemShop();
	virtual ~CItemShop();

	void Initialize();
	void LoadItemInfo(LPSTR pchFilename);
	void LoadItemList(LPSTR pchFilename);
	void LoadPackages(LPSTR pchFilename);
	void LoadGPMonsterData(LPSTR pchFilename);
	void LoadShopOption(LPSTR pchFilename);
	void CashShopOptioNReload();
	BOOL AddUser(LPOBJ lpObj);
	BOOL CGCashShopOpen(LPOBJ lpObj, PMSG_REQ_INGAMESHOPOPEN* lpMsg);
	BOOL CGCashPoint(LPOBJ lpObj);
	void CGCashItemBuy(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMBUY* lpMsg);
	void GCCashItemBuy(LPOBJ lpObj, int ID1, int ID2, int ID3, int Result);
	void GCCashPoint(LPOBJ lpObj);
	void CGCashItemGift(LPOBJ lpObj, PMSG_REQ_CASHITEM_GIFT* lpMsg);
	void GCCashItemGift(LPOBJ lpObj, int ID1, int ID2, int ID3, int Result);
	void GCCashItemBuyResult(LPOBJ lpObj, BYTE btResult);
	BOOL CGCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY* lpMsg);
	void GCCashInventoryItemCount(LPOBJ lpObj, LPBYTE lpRecv);
	void CGCashInventoryItemUse(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMUSE* lpMsg);
	void GCCashInventoryItemUse(LPOBJ lpObj, int Result, int UniqueCode, int AuthCode, int ID1, int ID2, int ID3);
	void GCShopVersion(LPOBJ lpObj);
	BOOL CheckPeriodItemUsed(LPOBJ lpObj, int iItemCode);
	BOOL CheckPeriodItem(int iItemCode);
	bool UsePackage(LPOBJ lpObj, int PackageID);
	LPBYTE GetItemList(int iCategory, int iPageCount);
	void AddCoin(LPOBJ lpObj, char EventType);
	void AddGPMonster(LPOBJ lpMonsterObj);
	void Run();
	bool GetRandomItemFromPackage(int PackageID, int & GUID, int & ID);
	std::vector<ITEMSHOP_ITEMLIST>::iterator FindItemInList(int iIndex, int iSubIndex, int iOption, int iCategory);

private:

	bool bIsCashShop;
	bool bIsGiftSystem;

	int iCashItemCount;	// 4
	int iPackItemCount;
	BOOL bCashItemListReload;	// C
	std::map<int, ITEMSHOP_ITEMINFO> MapItemInfo;
	std::vector<ITEMSHOP_ITEMLIST> VecItemList;	// 20
	std::vector<ITEMSHOP_PACKAGELIST> VecPackageList;
	std::vector<GP_PERMONSTER_DATA> VecGpMonster;

	int	ItemVer1;
	int ItemVer2;
	int ItemVer3;

	int BannerVer1;
	int BannerVer2;
	int BannerVer3;

	int GoblinSystem;
	int GoblinAddCoin;
	int GoblinTime;
	BYTE GoblinCoinType;

	bool IsCoinAdder;
	BYTE CoinAdderType;
	int CoinAddBC;
	int CoinAddDS;
	int CoinAddCC;
	int CoinAddCW;
	int CoinAddNM;
	int CoinAddIT;
	int CoinAddHatchery;
	int CoinAddDG;
	int CoinAddIF;
	int CoinAddKundun;
	int CoinAddMedusa;

	HANDLE hGoblinThread;

	bool CheckBuyCondition(LPOBJ lpObj, int ItemCode);
};

extern CItemShop g_CashShop;

BOOL IsCashItem(int iItemCode);
BOOL IsPremiumItem(int iItemCode);

#endif // !defined(AFX_CASHSHOP_H__B57A9271_3D21_40E0_B132_82F87B122CB4__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

