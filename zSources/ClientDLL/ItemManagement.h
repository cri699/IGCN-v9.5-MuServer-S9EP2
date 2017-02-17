//////////////////////////////////////////////////////////////////////
// ItemManagement.h
#ifndef ITEMMANAGEMENT_H
#define ITEMMANAGEMENT_H

#define ITEMTYPE_S8_COMMON	0
#define ITEMTYPE_S8_EQUIPMENT	1
#define ITEMTYPE_S8_SOCKET	2
#define ITEMTYPE_S8_380		3
#define ITEMTYPE_S8_LUCKY	4
#define ITEMTYPE_S8_EVENT	5
#define ITEMTYPE_S8_ANGEL	6
#define ITEMTYPE_S8_CHAOS	7

struct SHOPPRICELIST
{
	BYTE POS;
	UINT64 SellPrice;
}; extern SHOPPRICELIST g_Shop[120];

struct INVPRICELIST
{
	BYTE POS;
	UINT64 BuyPrice;
	UINT64 SellPrice;
}; extern INVPRICELIST g_Inv[237];
struct ITEM_CLASS
{
	int unk1;
	int itemid;
};
class CItemManagement
{
public:
	int Value(int itemid,BYTE type);
}; extern CItemManagement g_Items;

// type 0 = shop, 1- inventory, 2-repair/
int Value(int itemid, BYTE X, BYTE Y, BYTE type, int equipslot, int Equiped);
void SetPrice();
void ParsePricePacket(BYTE * aRecv);
void ParsePriceCMPacket(BYTE * aRecv);
bool __fastcall ItemCategoryCheckForExc(void *pThis, void *edx);
void LuckyItemToolTipFix();
void LuckyItemEnhancementFix();
void ErrtelMixStaffFix();
void ItemLevelReqFix();
void AlterPShopDisplayCurrency();
extern char * pShopPriceTag;
#endif



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

