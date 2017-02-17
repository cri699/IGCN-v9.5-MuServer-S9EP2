// CashShop.cpp: implementation of the CItemShop class.
// GS-N 1.00.18 JPN	0x00585620	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashShop.h"
#include "GameServer.h"
#include "GameMain.h"
#include "TLog.h"
#include "DSProtocol.h"
#include "SProtocol.h"
#include "winutil.h"
#include "gObjMonster.h"
#include "LogToFile.h"
#include "configread.h"
#include "PeriodItemEx.h"
#include "PentagramSystem.h"
#include "ItemSocketOptionSystem.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "IniReader.h"

CItemShop g_CashShop;
static CLogToFile * ITEMSHOP_LOG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CItemShop::CItemShop()
{
	this->MapItemInfo.clear();
	this->VecItemList.clear();
	this->VecPackageList.clear();
	this->hGoblinThread = NULL;
	return;
}

CItemShop::~CItemShop()
{
	delete ITEMSHOP_LOG;
	return;
}


void CItemShop::Initialize()
{
	this->bCashItemListReload = FALSE;

	if(ITEMSHOP_LOG == NULL)
	{
		ITEMSHOP_LOG = new CLogToFile("ITEMSHOP_LOG", g_ConfigRead.server.GetCashShopLogPath(), 256);
	}

	CIniReader ReadCashShop(g_ConfigRead.GetPath("IGC_CashShop.ini"));

	this->bIsCashShop = ReadCashShop.ReadInt("CashShop", "CashShopEnable", 1);
	this->bIsGiftSystem = ReadCashShop.ReadInt("CashShop", "GiftSystemEnable", 1);
	
	this->ItemVer1 = ReadCashShop.ReadInt("CashShop", "ItemShopItemVersion1", 512);
	this->ItemVer2 = ReadCashShop.ReadInt("CashShop", "ItemShopItemVersion2", 2011);
	this->ItemVer3 = ReadCashShop.ReadInt("CashShop", "ItemShopItemVersion3", 011);

	this->BannerVer1 = ReadCashShop.ReadInt("CashShop", "ItemShopBannerVersion1", 583);
	this->BannerVer2 = ReadCashShop.ReadInt("CashShop", "ItemShopBannerVersion2", 2011);
	this->BannerVer3 = ReadCashShop.ReadInt("CashShop", "ItemShopBannerVersion3", 001);

	this->GoblinSystem = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinPoint", 1);
	this->GoblinAddCoin = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinAddCoin", 5);
	this->GoblinTime = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinAddTime", 30);
	this->GoblinCoinType = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinCoinType", 2);

	this->IsCoinAdder = ReadCashShop.ReadInt("CashShop", "CoinAdder", 0);
	this->CoinAdderType = ReadCashShop.ReadInt("CashShop", "CoinAddType", 0);
	this->CoinAddBC = ReadCashShop.ReadInt("CashShop", "CoinAddBC", 0);
	this->CoinAddDS = ReadCashShop.ReadInt("CashShop", "CoinAddDS", 0);
	this->CoinAddCC = ReadCashShop.ReadInt("CashShop", "CoinAddCC", 0);
	this->CoinAddCW = ReadCashShop.ReadInt("CashShop", "CoinAddCW", 0);
	this->CoinAddNM = ReadCashShop.ReadInt("CashShop", "CoinAddNM", 0);
	this->CoinAddIT = ReadCashShop.ReadInt("CashShop", "CoinAddIT", 0);
	this->CoinAddHatchery = ReadCashShop.ReadInt("CashShop", "CoinAddHatchery", 0);
	this->CoinAddDG = ReadCashShop.ReadInt("CashShop", "CoinAddDG", 0);
	this->CoinAddIF = ReadCashShop.ReadInt("CashShop", "CoinAddIF", 0);
	this->CoinAddKundun = ReadCashShop.ReadInt("CashShop","CoinAddKundun", 0);
	this->CoinAddMedusa = ReadCashShop.ReadInt("CashShop", "CoinAddMedusa", 0);

	g_Log.Add("[ItemShop] Initializing ItemShop with Version: %d/%d/%d (Image Version: %d/%d/%d)", this->ItemVer1, this->ItemVer2, this->ItemVer3,
		this->BannerVer1, this->BannerVer2, this->BannerVer3);

	char szTemp[256];

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_CashItem_Info.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadItemInfo(g_ConfigRead.GetPath(szTemp));

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_CashItem_List.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadItemList(g_ConfigRead.GetPath(szTemp));

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_CashItem_Package.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadPackages(g_ConfigRead.GetPath(szTemp));

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_MonsterGP_Info.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadGPMonsterData(g_ConfigRead.GetPath(szTemp));

	if (this->GoblinSystem)
	{
		g_Log.Add("[ItemShop] Goblin Point System Enabled (AddCoin:%d) (Delay:%d seconds) (CoinType:%d)", this->GoblinAddCoin, this->GoblinTime, this->GoblinCoinType);
	}
}

void CItemShop::Run()
{
	if(!this->GoblinSystem) return;

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE) return;

	for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if(gObj[i].Connected > PLAYER_LOGGED && gObj[i].m_bOff == false)
		{
			if((GetTickCount() - gObj[i].m_PlayerData->m_GoblinTime) >= (this->GoblinTime * 1000))
			{
				GDReqInGameShopPointAdd(i, this->GoblinCoinType, this->GoblinAddCoin);
				gObj[i].m_PlayerData->m_GoblinTime = GetTickCount();
			}
		}
	}
}

void CItemShop::CashShopOptioNReload()
{
	this->bCashItemListReload = TRUE;

	g_Log.Add("[ItemShop] Reload Info - START");

	CIniReader ReadCashShop(g_ConfigRead.GetPath("IGC_CashShop.ini"));

	this->ItemVer1 = ReadCashShop.ReadInt("CashShop", "ItemShopItemVersion1", 512);
	this->ItemVer2 = ReadCashShop.ReadInt("CashShop", "ItemShopItemVersion2", 2011);
	this->ItemVer3 = ReadCashShop.ReadInt("CashShop", "ItemShopItemVersion3", 011);

	this->BannerVer1 = ReadCashShop.ReadInt("CashShop", "ItemShopBannerVersion1", 583);
	this->BannerVer2 = ReadCashShop.ReadInt("CashShop", "ItemShopBannerVersion2", 2011);
	this->BannerVer3 = ReadCashShop.ReadInt("CashShop", "ItemShopBannerVersion3", 001);

	this->GoblinSystem = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinPoint", 1);
	this->GoblinAddCoin = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinAddCoin", 5);
	this->GoblinTime = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinAddTime", 30);
	this->GoblinCoinType = ReadCashShop.ReadInt("CashShop", "ItemShopGoblinCoinType", 2);

	char szTemp[256];

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_CashItem_Info.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadItemInfo(g_ConfigRead.GetPath(szTemp));

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_CashItem_List.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadItemList(g_ConfigRead.GetPath(szTemp));

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_CashItem_Package.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadPackages(g_ConfigRead.GetPath(szTemp));

	sprintf(szTemp, "\\CashShop\\%d.%d.%d\\IGC_MonsterGP_Info.xml", this->ItemVer1, this->ItemVer2, this->ItemVer3);
	this->LoadGPMonsterData(g_ConfigRead.GetPath(szTemp));

	g_Log.Add("[ItemShop] Reload Info - END");

	this->bCashItemListReload = FALSE;
}



void CItemShop::LoadItemInfo(LPSTR pchFilename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFilename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ItemShop][ItemInfo] List load failed. [%s] [%s]", pchFilename, res.description());
		return;
	}

	this->MapItemInfo.clear();

	ITEMSHOP_ITEMINFO ItemInfo;

	pugi::xml_node main = file.child("CashItemInfo");

	for (pugi::xml_node item = main.child("Item"); item; item = item.next_sibling())
	{
		ItemInfo.wGUID = item.attribute("GUID").as_int();
		ItemInfo.wID = item.attribute("ID").as_int();
		ItemInfo.wItemGroup = item.attribute("Cat").as_int();
		ItemInfo.wItemType = item.attribute("Index").as_int();
		ItemInfo.btItemLevel = item.attribute("Level").as_int();
		ItemInfo.btItemDurability = item.attribute("Durability").as_int();
		ItemInfo.btItemSkill = item.attribute("Skill").as_int();
		ItemInfo.btItemLuck = item.attribute("Luck").as_int();
		ItemInfo.btItemOption = item.attribute("Option").as_int();
		ItemInfo.btItemExOption = item.attribute("Exc").as_int();
		ItemInfo.btItemSetOption = item.attribute("Set").as_int();
		ItemInfo.btItemSocketCount = item.attribute("SocketCount").as_int();
		ItemInfo.btItemAttribute = item.attribute("Element").as_int();
		ItemInfo.btItemType = item.attribute("Type").as_int();
		ItemInfo.dwItemPeriodTime = item.attribute("Duration").as_int();

		if (this->MapItemInfo.insert(std::pair<int, ITEMSHOP_ITEMINFO>(ItemInfo.wID, ItemInfo)).second == false)
		{
			g_Log.AddC(TColor::Red, "[ItemShop][LoadItemInfo] ERROR: Duplicated Entry in config file: ID:%d",
				ItemInfo.wID);
		}
	}

	g_Log.Add("[ItemShop][Load ItemInfo] - Complete! - Total:%d",
		this->MapItemInfo.size());
}

void CItemShop::LoadItemList(LPSTR pchFilename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFilename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ItemShop][ItemList] List load failed. [%s] [%s]", pchFilename, res.description());
		return;
	}

	this->VecItemList.clear();

	ITEMSHOP_ITEMLIST ItemList;

	pugi::xml_node main = file.child("CashItemList");

	for (pugi::xml_node item = main.child("Item"); item; item = item.next_sibling())
	{
		ItemList.btGuid = item.attribute("GUID").as_int();
		ItemList.wItemIndex = item.attribute("iIndex").as_int();
		ItemList.wItemSubIndex = item.attribute("iSubIndex").as_int();
		ItemList.wItemOptionSelect = item.attribute("OptionSelect").as_int();
		ItemList.wPackageID = item.attribute("PackageID").as_int();
		ItemList.btCoinType = item.attribute("CoinType").as_int();
		ItemList.wPrice = item.attribute("CoinValue").as_int();
		ItemList.wItemInfoGUID = item.attribute("UniqueID1").as_int();
		ItemList.wItemInfoID = item.attribute("UniqueID2").as_int();
		ItemList.ItemCategory = item.attribute("ShopCategory").as_int();
		ItemList.ItemGoblinPoint = item.attribute("GPRewardValue").as_int();
		ItemList.btEnableForSale = item.attribute("CanBuy").as_int();
		ItemList.btEnableForGift = item.attribute("CanGift").as_int();
		ItemList.btRandomItemPackage = item.attribute("RandomItemSelect").as_int();

		this->VecItemList.push_back(ItemList);
	}

	g_Log.Add("[ItemShop][Load ItemList] - Complete! - Total:%d",
		this->VecItemList.size());
}

void CItemShop::LoadPackages(LPSTR pchFilename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFilename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ItemShop][PackageList] File load fail: %s [%s]", pchFilename, res.description());
		return;
	}

	ITEMSHOP_PACKAGELIST PackInfo;

	this->VecPackageList.clear();

	pugi::xml_node main = file.child("CashItemPackage");

	for (pugi::xml_node package = main.child("Package"); package; package = package.next_sibling())
	{
		PackInfo.Guid = package.attribute("GUID").as_int();
		PackInfo.PackageID = package.attribute("ID").as_int();
		PackInfo.ItemSeqNum = package.attribute("ItemSequence").as_int();
		PackInfo.wItemInfoGUID = package.attribute("UniqueID1").as_int();
		PackInfo.wItemInfoID = package.attribute("UniqueID2").as_int();

		if (this->MapItemInfo.find(PackInfo.wItemInfoID) == this->MapItemInfo.end())
		{
			g_Log.AddC(TColor::Red, "[ItemShop][Load ItemList] ERROR: ItemInfo not exist (GUID:%d ID:%d)",
				PackInfo.wItemInfoGUID, PackInfo.wItemInfoID);
		}

		else
		{
			this->VecPackageList.push_back(PackInfo);
		}
	}

	g_Log.Add("[ItemShop][Package Load List] Complete! - Total:%d", this->VecPackageList.size());
}

void CItemShop::LoadGPMonsterData(LPSTR pchFilename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFilename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ItemShop][GPMonsterList] File load fail: %s [%s]", pchFilename, res.description());
		return;
	}

	GP_PERMONSTER_DATA GPInfo;
	this->VecGpMonster.clear();

	pugi::xml_node main = file.child("MonsterGPInfo");

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		if (monster.attribute("Enable").as_bool() == false)
		{
			continue;
		}

		GPInfo.MonsterID = monster.attribute("Index").as_int();
		GPInfo.MapNumber = monster.attribute("MapNumber").as_int();
		GPInfo.Coins = monster.attribute("CoinValue").as_int();
		GPInfo.CoinType = monster.attribute("CoinType").as_int();

		this->VecGpMonster.push_back(GPInfo);
	}

	g_Log.Add("[ItemShop][GP Load List] Complete! - Total:%d", this->VecGpMonster.size());
}

BOOL CItemShop::AddUser(LPOBJ lpObj)
{
	PMSG_REQ_INGAMESHOPINIT pInit;
	PHeadSubSetB((LPBYTE)&pInit, 0xD2, 0x00, sizeof(pInit));
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pInit, sizeof(pInit));
	this->GCShopVersion(lpObj);
	lpObj->m_PlayerData->m_GoblinTime = GetTickCount();
	this->CGCashPoint(lpObj);
	return TRUE;
}

struct PMSG_ANS_CASHSHOPOPEN
{
	PBMSG_HEAD2 head;	// C1:D2:02
	BYTE btResult;	// 4
};

BOOL CItemShop::CGCashShopOpen(LPOBJ lpObj, PMSG_REQ_INGAMESHOPOPEN * lpMsg)
{
	PMSG_ANS_CASHSHOPOPEN pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x02, sizeof(PMSG_ANS_CASHSHOPOPEN));

	if ( this->bCashItemListReload == TRUE )
	{
		pMsg.btResult = 6;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	if (this->bIsCashShop == FALSE)
	{
		pMsg.btResult = 6;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}

	if ( lpObj->Connected <= PLAYER_LOGGED || lpObj->CloseCount != -1 || lpObj->Type != OBJ_USER )
		return FALSE;

	if ( lpObj->m_PlayerData->m_bSecurityCheck == false )
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,394));
		pMsg.btResult = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) || DS_MAP_RANGE(lpObj->MapNumber) || 
		CC_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpObj->MapNumber) || 
		ITL_MAP_RANGE(lpObj->MapNumber) || DG_MAP_RANGE(lpObj->MapNumber) ||
		IMPERIAL_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL )
	{
		pMsg.btResult = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		pMsg.btResult = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	if ( lpMsg->btShopOpenType == 0 && lpObj->m_IfState.use != 0)
	{
		pMsg.btResult = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	else if ( lpMsg->btShopOpenType == 1 && (lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 19) )
	{
		pMsg.btResult = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

		return FALSE;
	}

	BYTE btResult;

	if ( lpMsg->btShopOpenType == 0 )
	{
		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 19;
		lpObj->m_IfState.state = 1;

		btResult = 1;
		g_Log.Add("[ItemShop] [%s][%s] Opened shop", lpObj->AccountID, lpObj->Name);
	}

	else if ( lpMsg->btShopOpenType == 1 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;
		lpObj->m_IfState.state = 0;

		btResult = 0;
		g_Log.Add("[ItemShop] [%s][%s] Closed shop", lpObj->AccountID, lpObj->Name);
	}

	pMsg.btResult = btResult;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

	return TRUE;
}

BOOL CItemShop::CGCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY *lpMsg)
{
	if (this->bIsCashShop == FALSE)
	{
		return FALSE;
	}

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID) )
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}
	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return FALSE;

	GDReqInGameShopItemList(lpObj->m_Index, lpMsg->InventoryType == 0x53 ? 1 : 2, lpMsg->PageIndex);
	return true;
}

BOOL CItemShop::CGCashPoint(LPOBJ lpObj)
{
	if (this->bIsCashShop == FALSE)
	{
		return FALSE;
	}

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return FALSE;

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		return FALSE;
	}

	GDReqInGameShopPoint(lpObj->m_Index);
	return TRUE;
}

void CItemShop::GCCashPoint(LPOBJ lpObj)
{
	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	PMSG_ANS_INGAMESHOP_POINT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x01, sizeof(PMSG_ANS_INGAMESHOP_POINT));

	pMsg.btViewType = 0;
	pMsg.GoblinPoint = lpObj->m_PlayerData->m_GoblinPoint;
	pMsg.dwCash_C = lpObj->m_PlayerData->m_WCoinC;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CItemShop::GCCashInventoryItemCount(LPOBJ lpObj, LPBYTE lpRecv)
{
	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	ISHOP_ANS_ITEMLIST *lpMsg = (ISHOP_ANS_ITEMLIST *)(lpRecv);
	ISHOP_ITEMLIST *lpItem = (ISHOP_ITEMLIST *)(lpRecv+sizeof(ISHOP_ANS_ITEMLIST));

	PMSG_ANS_INGAMESHOP_ITEMCOUNT pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x06, sizeof(pMsg));

	if(lpMsg->Count == 0)
	{
		pMsg.PageIndex = 0;
		pMsg.TotalPage = 0;
	}
	else
	{
		pMsg.TotalPage = lpMsg->Count / 8;
		if(lpMsg->Count % 8 != 0)
		{
			pMsg.TotalPage++;
		}

		pMsg.PageIndex = lpMsg->InvNum;
	}

	pMsg.TotalItemCount = lpMsg->Count;
	pMsg.CurrentItemCount = 0;

	int CurrItem = (pMsg.PageIndex-1)*8;

	while ( pMsg.CurrentItemCount != 8 )
	{
		pMsg.CurrentItemCount++;
		if(pMsg.CurrentItemCount >= (lpMsg->Count-CurrItem) ) break;
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if(lpMsg->Count > 0)
	{
		if(lpMsg->InvType == 1)
		{
			PMSG_ANS_INGAMESHOP_ITEM pItem = {0};
			PHeadSubSetB((LPBYTE)&pItem, 0xD2, 0x0D, sizeof(pItem));
			pItem.Unk3 = 0x50;

			int StartItem = (lpMsg->InvNum-1)*8;
			int count = 0;

			while ( count != 8 )
			{
				lpItem = (ISHOP_ITEMLIST *)(lpRecv+sizeof(ISHOP_ANS_ITEMLIST)+((StartItem+count)*sizeof(ISHOP_ITEMLIST)));

				pItem.UniqueCode = lpItem->UniqueCode;
				pItem.AuthCode = lpItem->AuthCode;
				pItem.UniqueValue1 = lpItem->UniqueID1;
				pItem.UniqueValue2 = lpItem->UniqueID2;
				pItem.UniqueValue3 = lpItem->UniqueID3;

				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pItem, pItem.h.size);
			
				count++;
				if(count >= (lpMsg->Count-StartItem) ) break;
			}
		}
		else if(lpMsg->InvType == 2)
		{
			PMSG_ANS_INGAMESHOP_GIFT pItem = {0};
			PHeadSubSetB((LPBYTE)&pItem, 0xD2, 0x0E, sizeof(pItem));
			pItem.type = 0x50;

			int StartItem = (lpMsg->InvNum-1)*8;
			int count = 0;

			while ( count != 8 )
			{
				lpItem = (ISHOP_ITEMLIST *)(lpRecv+sizeof(ISHOP_ANS_ITEMLIST)+((StartItem+count)*sizeof(ISHOP_ITEMLIST)));

				pItem.itemGuid = lpItem->UniqueCode;
				pItem.unk1 = lpItem->AuthCode;
				pItem.unk2_type = lpItem->UniqueID1;
				pItem.groupId = lpItem->UniqueID2;
				pItem.itemId = lpItem->UniqueID3;
				memcpy(pItem.szUser, lpItem->GiftName, 10);
				memcpy(pItem.szText, lpItem->Message, 200);

				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pItem, pItem.h.size);
			
				count++;
				if(count >= lpMsg->Count ) break;
			}
		}
	}
}

void CItemShop::CGCashInventoryItemUse(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMUSE *lpMsg)
{
	PMSG_ANS_INGAMESHOP_ITEMUSE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0B, sizeof(pMsg));

	if (this->bIsCashShop == FALSE)
	{
		return;
	}

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return;

	ITEM_ATTRIBUTE * lpItem = GetItemAttr(lpMsg->ItemID);

	if(lpItem == NULL)
	{
		pMsg.result = CS_USE_ITEM_INTERNAL_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[ItemShop][Bug Tracker] [%s][%s] Item doesn't exist (%d)", lpObj->AccountID, lpObj->Name, lpMsg->ItemID);
		return;
	}

	if(lpItem->ItemKindA == 11 && CheckEventInventoryEmptySpace(lpObj, lpItem->Height, lpItem->Width) == false)
	{
		g_Log.Add("[ItemShop][Bug Tracker] [%s][%s] Event Inventory space isn't sufficient (%s)(%d)(%d)", lpObj->AccountID, lpObj->Name, lpItem->Name, lpItem->Height, lpItem->Width);
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	else if(lpItem->ItemKindA == 12 && gObjChkMuunInventoryEmpty(lpObj) == false)
	{
		g_Log.Add("[ItemShop][Bug Tracker] [%s][%s] Muun Inventory space isn't sufficient (%s)(%d)(%d)", lpObj->AccountID, lpObj->Name, lpItem->Name, lpItem->Height, lpItem->Width);
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	else if ( ::CheckInventoryEmptySpace(lpObj, lpItem->Height, lpItem->Width ) == 0 )
	{
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[ItemShop][Bug Tracker] [%s][%s] Inventory space isn't sufficient (%s)(%d)(%d)", lpObj->AccountID, lpObj->Name, lpItem->Name, lpItem->Height, lpItem->Width);
		return;
	}

	if(this->CheckBuyCondition(lpObj, lpMsg->ItemID) == false)
	{
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[ItemShop][Bug Tracker] [%s][%s] Cannot use this item: (%s)", lpObj->AccountID, lpObj->Name, lpItem->Name);
		return;
	}

	GDReqInGameShopItemUse(lpObj->m_Index, lpMsg->UniqueCode, lpMsg->AuthCode);
	g_Log.Add("[ItemShop] [%s][%s] Request to Use Item (Code:%d) (Auth:%d) (Name:%s)", lpObj->AccountID,lpObj->Name, lpMsg->UniqueCode, lpMsg->AuthCode, lpItem->Name);
}

void CItemShop::GCCashInventoryItemUse(LPOBJ lpObj, int Result, int UniqueCode, int AuthCode, int ID1, int ID2, int ID3)
{
	PMSG_ANS_INGAMESHOP_ITEMUSE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0B, sizeof(pMsg));

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return;

	std::map<int, ITEMSHOP_ITEMINFO>::iterator Iter = this->MapItemInfo.find(ID3);

	if(Iter == this->MapItemInfo.end())
	{
		g_Log.Add("[ItemShop] [%s][%s] Using Item Failed: Item not found in CashItemList (ItemInfo) (%d/%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, UniqueCode, AuthCode);
		pMsg.result = CS_USE_ITEM_INTERNAL_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	ITEM_ATTRIBUTE * lpItemInfo = GetItemAttr(ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType));

	if(lpItemInfo == NULL)
	{
		g_Log.Add("[ItemShop] [%s][%s] Using Item Failed: Item not found in ItemList (%d/%d) (ID:%d/%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, Iter->second.wItemGroup, Iter->second.wItemType, UniqueCode, AuthCode);
		pMsg.result = CS_USE_ITEM_INTERNAL_ERROR;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(lpItemInfo->ItemKindA == 11 && CheckEventInventoryEmptySpace(lpObj, lpItemInfo->Height, lpItemInfo->Width) == false)
	{
		GDReqInGameShopItemRollbackUse(lpObj->m_Index, UniqueCode, AuthCode);
		g_Log.Add("[ItemShop] [%s][%s] Using Item Failed: Not enough space in Event Inventory (%d/%d) (ID:%d/%d) (W:%d) (H:%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, Iter->second.wItemGroup, Iter->second.wItemType, lpItemInfo->Width, lpItemInfo->Height, UniqueCode, AuthCode);
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	else if(lpItemInfo->ItemKindA == 12 && gObjChkMuunInventoryEmpty(lpObj) == false)
	{
		GDReqInGameShopItemRollbackUse(lpObj->m_Index, UniqueCode, AuthCode);
		g_Log.Add("[ItemShop] [%s][%s] Using Item Failed: Not enough space in Muun Inventory (%d/%d) (ID:%d/%d) (W:%d) (H:%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, Iter->second.wItemGroup, Iter->second.wItemType, lpItemInfo->Width, lpItemInfo->Height, UniqueCode, AuthCode);
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	else if(CheckInventoryEmptySpace(lpObj, lpItemInfo->Height, lpItemInfo->Width) == false)
	{
		GDReqInGameShopItemRollbackUse(lpObj->m_Index, UniqueCode, AuthCode);
		g_Log.Add("[ItemShop] [%s][%s] Using Item Failed: Not enough space in Inventory (%d/%d) (ID:%d/%d) (W:%d) (H:%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, Iter->second.wItemGroup, Iter->second.wItemType, lpItemInfo->Width, lpItemInfo->Height, UniqueCode, AuthCode);
		pMsg.result = CS_USE_ITEM_CANNOT_USE;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(Result == 0)
	{
		g_Log.Add("[ItemShop] [%s][%s] Using Item Failed. (%d/%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, UniqueCode, AuthCode);
		pMsg.result = CS_USE_ITEM_NOT_EXISTS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(Iter->second.btItemType == 1)
	{
		g_PeriodItemEx.SetPeriodItemInfo(lpObj, ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType), 0, Iter->second.dwItemPeriodTime*60);
	}

	else if(Iter->second.btItemType == 2)
	{
		DWORD periodtime = Iter->second.dwItemPeriodTime*60;
		ItemSerialCreateSend(lpObj->m_Index, 236, NULL, NULL, ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType), Iter->second.btItemLevel, 
			Iter->second.btItemDurability, Iter->second.btItemSkill, Iter->second.btItemLuck, Iter->second.btItemOption,
			Iter->second.dwItemPeriodTime, Iter->second.btItemExOption, Iter->second.btItemSetOption, periodtime, 0, 0);
	}

	else if(Iter->second.btItemType == 3)
	{
		if(ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType) == ITEMGET(14,91))
		{
			GDCharCardBuy(lpObj->AccountID, 0);
		}

		else if(ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType) == ITEMGET(14,169))
		{
			GDCharCardBuy(lpObj->AccountID, 1);
		}

		else
		{
			pMsg.result = CS_USE_ITEM_CANNOT_USE;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.AddC(TColor::Red,  "[ItemShop] (%s)(%s) ERROR: Trying to use UNKNOWN character card (%d/%d)", lpObj->AccountID, lpObj->Name, Iter->second.wItemGroup, Iter->second.wItemType);
			return;
		}
	}

	else if(Iter->second.btItemType == 4)
	{
		if (lpObj->m_PlayerData->VipType == 0 && Iter->second.btItemLevel == 0)
		{
			GDReqInGameShopItemRollbackUse(lpObj->m_Index, UniqueCode, AuthCode);
			pMsg.result = CS_USE_ITEM_TICKET_ERROR;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.AddC(TColor::Red, "[ItemShop] (%s)(%s) ERROR: Non-VIP User cannot buy a ticket with Vip Level == 0", lpObj->AccountID, lpObj->Name, Iter->second.wItemGroup, Iter->second.wItemType);
			return;
		}

		if (lpObj->m_PlayerData->VipType != 0 && lpObj->m_PlayerData->VipType > Iter->second.btItemLevel)
		{
			GDReqInGameShopItemRollbackUse(lpObj->m_Index, UniqueCode, AuthCode);
			pMsg.result = CS_USE_ITEM_TICKET_ERROR;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.AddC(TColor::Red,  "[ItemShop] (%s)(%s) ERROR: Trying to use VIP card with lower VIP level while user has higher VIP level (%d/%d)", lpObj->AccountID, lpObj->Name, Iter->second.wItemGroup, Iter->second.wItemType);
			return;
		}

		GJVipBuy(lpObj->AccountID, Iter->second.btItemLevel, Iter->second.dwItemPeriodTime);
	}

	else
	{
		if (lpItemInfo->ItemKindA == 11)
		{
			ItemSerialCreateSend(lpObj->m_Index, 226, NULL, NULL, ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType), Iter->second.btItemLevel, 0, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		}

		else if (lpItemInfo->ItemKindA == 12)
		{
			BYTE SocketOption[5] = { -1, -1, -1, -1, -1 };
			ItemSerialCreateSend(lpObj->m_Index, 224, NULL, NULL, ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType), Iter->second.btItemLevel, Iter->second.btItemDurability, 0, 0, 0, lpObj->m_Index, 0, 0, 0, SocketOption, 0);
		}

		else
		{
			BYTE SocketOption[5] = { -1, -1, -1, -1, -1 }; 
			int SocketCount = Iter->second.btItemSocketCount;

			if (g_PentagramSystem.IsPentagramItem(ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType)) == TRUE ||
				g_SocketOptionSystem.IsEnableSocketItem(ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType)) == TRUE)
			{
				if (SocketCount < 1)
				{
					SocketCount = 1;
				}
			}

			for (int i = 0; i < SocketCount; i++)
			{
				SocketOption[i] = 0xFE;
			}

			int MainAttribute = 0;

			if (Iter->second.btItemAttribute == static_cast<unsigned char>(-1))		{
				MainAttribute = 1 + rand() % 5;
			}

			else
			{
				MainAttribute = Iter->second.btItemAttribute;
			}

			ItemSerialCreateSend(lpObj->m_Index, 235, NULL, NULL, ITEMGET(Iter->second.wItemGroup, Iter->second.wItemType), Iter->second.btItemLevel,
				Iter->second.btItemDurability, Iter->second.btItemSkill, Iter->second.btItemLuck, Iter->second.btItemOption,
				lpObj->m_Index, Iter->second.btItemExOption, Iter->second.btItemSetOption, 0, SocketOption, MainAttribute);
		}
	}

	GDReqInGameShopItemDelete(lpObj->m_Index, UniqueCode, AuthCode);

	pMsg.result = CS_USE_ITEM_SUCCESS;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[ItemShop] [%s][%s] Using Item Success: (%d/%d) (ID:%d/%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, Iter->second.wItemGroup, Iter->second.wItemType, UniqueCode, AuthCode);
	ITEMSHOP_LOG->Output("[ItemShop] [%s][%s] Using Item Success: (%d/%d) (ID:%d/%d) (Code:%d) (Auth:%d)", lpObj->AccountID, lpObj->Name, ID2, ID3, Iter->second.wItemGroup, Iter->second.wItemType, UniqueCode, AuthCode);

	if (Iter->second.btItemType == 4)
	{
		GSProtocol.GCJoinBillCheckSend(lpObj->AccountID, lpObj->m_Index);
	}
}

void CItemShop::CGCashItemBuy(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMBUY *lpMsg)
{
	std::vector<ITEMSHOP_ITEMLIST>::iterator Iter;
	std::map<int, ITEMSHOP_ITEMINFO>::iterator Iter2;
	PMSG_ANS_INGAMESHOP_ITEMBUY pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x03, sizeof(pMsg));

	if ( this->bCashItemListReload == TRUE )
		return;

	if (this->bIsCashShop == FALSE)
		return;

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return;

	Iter = this->FindItemInList(lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->ItemOpt, lpMsg->Category);

	if(Iter == this->VecItemList.end())
	{
		pMsg.Result = 6;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.AddC(TColor::Red,  "[ItemShop] WARNING: Trying to buy undefined Item (INDEX:%d SUBINDEX:%d CATEGORY:%d ItemID:%d BUYOPTION:%d", lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);
		return;
	}

	if(Iter->wPackageID == 0)
	{
		Iter2 = this->MapItemInfo.find(Iter->wItemInfoID); //chnaged Iter->wItemInfoGUID to Iter->wItemInfoID
		if(Iter2 == this->MapItemInfo.end())
		{
			pMsg.Result = 6;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.AddC(TColor::Red,  "[ItemShop] WARNING: Trying to buy undefined Item (ItemInfo) (INDEX:%d SUBINDEX:%d CATEGORY:%d ItemID:%d BUYOPTION:%d", lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);
			return;
		}
	}

	if(Iter->btEnableForSale == FALSE)
	{
		pMsg.Result = 6;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed - Item (%d/%d) is not for sale", lpObj->AccountID, lpObj->Name, lpMsg->ItemIndex, lpMsg->ItemSubIndex);
		return;
	}

	if(this->CheckBuyCondition(lpObj, lpMsg->ItemID) == false)
	{
		pMsg.Result = 6;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed - Item (%d/%d): User does not meet all requirements", lpObj->AccountID, lpObj->Name, lpMsg->ItemIndex, lpMsg->ItemSubIndex);
		return;
	}

	int UserCoin;

	switch ( Iter->btCoinType )
	{
		case 0:
			UserCoin = lpObj->m_PlayerData->m_WCoinC;
			break;
		case 1:
			UserCoin = lpObj->m_PlayerData->m_WCoinP;
			break;
		case 2:
			UserCoin = lpObj->m_PlayerData->m_GoblinPoint;
			break;
		default:
			g_Log.AddC(TColor::Red,  "[ItemShop] ERROR: Unknown Coin Type:%d for Item (%d)(%d)", Iter->btCoinType, lpMsg->ItemIndex, lpMsg->ItemSubIndex);
			UserCoin = 0;
			break;
	}
	
	if(UserCoin < Iter->wPrice)
	{
		pMsg.Result = 1;
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed - Item (%d/%d): not enough money to purchase (need:%d/have:%d)", lpObj->AccountID, lpObj->Name, lpMsg->ItemIndex, lpMsg->ItemSubIndex, Iter->wPrice, UserCoin);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(Iter->wPackageID > 0)
	{
		if(Iter->btRandomItemPackage == TRUE)
		{
			int GUID = -1, ID = -1;
			bool bResult = this->GetRandomItemFromPackage(Iter->wPackageID, GUID, ID);

			if(GUID == -1 || ID == -1)
			{
				pMsg.Result = 6;
				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				g_Log.AddC(TColor::Blue, "[ItemShop] ERROR: [PackageID:%d] Cannot get RandomItem (GUID == -1 || ID == -1)",Iter->wPackageID);
				return;
			}

			GDReqInGameShopItemBuy(lpObj->m_Index, 673, GUID, ID,
				Iter->wPrice, Iter->btCoinType);
		}

		else
		{

			std::vector<ITEMSHOP_PACKAGELIST>::iterator It;
			BYTE cBUFF[65535];
			ISHOP_ITEM_BUY_PACKAGE * pMsg = (ISHOP_ITEM_BUY_PACKAGE *)(cBUFF);
			ISHOP_ITEM_PACKAGE * pItem;

			pMsg->Count = 0;

			for( It = this->VecPackageList.begin() ; It != this->VecPackageList.end() ; It++ )
			{
				if(It->PackageID == Iter->wPackageID)
				{
					pItem = (ISHOP_ITEM_PACKAGE *)(cBUFF + sizeof(ISHOP_ITEM_BUY_PACKAGE) + (pMsg->Count * sizeof(ISHOP_ITEM_PACKAGE)));

					pItem->ID1 = 673;
					pItem->ID2 = It->wItemInfoGUID;
					pItem->ID3 = It->wItemInfoID;

					pMsg->Count++;
				}
			}
		
			WORD PacketSize = sizeof(ISHOP_ITEM_BUY_PACKAGE) + (pMsg->Count * sizeof(ISHOP_ITEM_PACKAGE));

			pMsg->CoinType = Iter->btCoinType;
			pMsg->Price = Iter->wPrice;

			pMsg->aIndex = lpObj->m_Index;
			memcpy(pMsg->AccountID, lpObj->AccountID, 11);

			pMsg->h.c = 0xC2;
			pMsg->h.headcode = 0xD8;
			pMsg->h.subcode = 0x01;
			pMsg->h.sizeH = HIBYTE(PacketSize);
			pMsg->h.sizeL = LOBYTE(PacketSize);

			wsDataCli.DataSend((PCHAR)&cBUFF, PacketSize);

			g_Log.Add("[ItemShop] (%s)(%s) Purchase Request: [Package ID: %d] [Items: %d]", lpObj->AccountID, lpObj->Name, Iter->wPackageID, pMsg->Count);
			ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Request: [Package ID: %d] [Items: %d]", lpObj->AccountID, lpObj->Name, Iter->wPackageID, pMsg->Count);
		}
	}

	else
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Request: (GUID:%d) (ID:%d) (Price:%d) (CoinType:%d)", lpObj->AccountID, lpObj->Name, Iter->wItemInfoGUID, Iter->wItemInfoID, Iter->wPrice, Iter->btCoinType);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Request: (GUID:%d) (ID:%d) (Price:%d) (CoinType:%d)", lpObj->AccountID, lpObj->Name, Iter->wItemInfoGUID, Iter->wItemInfoID, Iter->wPrice, Iter->btCoinType);
	
		GDReqInGameShopItemBuy(lpObj->m_Index, 673, Iter->wItemInfoGUID, Iter->wItemInfoID,
			Iter->wPrice, Iter->btCoinType);
	}

	if(Iter->ItemGoblinPoint > 0)
	{
		GDReqInGameShopPointAdd(lpObj->m_Index, 2, Iter->ItemGoblinPoint);
	}
}

void CItemShop::AddCoin(LPOBJ lpObj, char EventType)
{
	if ( !this->IsCoinAdder )
	{
		return;
	}	
	
	int Coin = 0;

	switch ( EventType )
	{
		case 0:
			Coin = this->CoinAddBC;
			break;
		case 1:
			Coin = this->CoinAddDS;
			break;
		case 2:
			Coin = this->CoinAddCC;
			break;
		case 4:
			Coin = this->CoinAddCW;
			break;
		case 5:
			Coin = this->CoinAddNM;
			break;
		case 6:
			Coin = this->CoinAddIT;
			break;
		case 8:
			Coin = this->CoinAddDG;
			break;
		case 9:
			Coin = this->CoinAddIF;
			break;
	}

	GDReqInGameShopPointAdd(lpObj->m_Index, this->CoinAdderType, Coin);
	g_Log.Add("[ItemShop] (%s)(%s) Add Coin: (Type:%d) (Amount:%d) (Event:%d)", lpObj->AccountID, lpObj->Name, this->CoinAdderType, Coin, EventType);
}

void CItemShop::CGCashItemGift(LPOBJ lpObj, PMSG_REQ_CASHITEM_GIFT *lpMsg)
{
	std::vector<ITEMSHOP_ITEMLIST>::iterator Iter;
	std::map<int, ITEMSHOP_ITEMINFO>::iterator Iter2;
	PMSG_ANS_INGAMESHOP_ITEMGIFT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x04, sizeof(pMsg));

	if ( this->bCashItemListReload == TRUE )
		return;

	if (this->bIsCashShop == FALSE)
		return;

	if (this->bIsGiftSystem == FALSE)
	{
		pMsg.Result = 7;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		g_Log.AddC(TColor::Red, "Error-L1 : Request to receive Warehouse information doesn't match the user [%s][%d]", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return;

	Iter = this->FindItemInList(lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->ItemOpt, lpMsg->Category);

	if(Iter == this->VecItemList.end()) 
	{
		pMsg.Result = 7;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.AddC(TColor::Red,  "[ItemShop] WARNING: Trying to gift undefined Item (INDEX:%d SUBINDEX:%d CATEGORY:%d ItemID:%d BUYOPTION:%d", lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);
		return;
	}

	if(Iter->wPackageID == 0)
	{
		Iter2 = this->MapItemInfo.find(Iter->wItemInfoID);

		if(Iter2 == this->MapItemInfo.end())
		{
			pMsg.Result = 7;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.AddC(TColor::Red,  "[ItemShop] WARNING: Trying to gift undefined Item (ItemInfo) (INDEX:%d SUBINDEX:%d CATEGORY:%d ItemID:%d BUYOPTION:%d", lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);
			return;
		}
	}

	if(Iter->btEnableForGift == FALSE)
	{
		pMsg.Result = 7;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed (Gift) - Item (%d/%d) cannot be sent as gift", lpObj->AccountID, lpObj->Name, lpMsg->ItemIndex, lpMsg->ItemSubIndex);
		return;
	}

	double UserCoin;

	switch ( Iter->btCoinType )
	{
		case 0:
			UserCoin = lpObj->m_PlayerData->m_WCoinC;
			break;
		case 1:
			UserCoin = lpObj->m_PlayerData->m_WCoinP;
			break;
		case 2:
			UserCoin = lpObj->m_PlayerData->m_GoblinPoint;
			break;
		default:
			g_Log.AddC(TColor::Red,  "[ItemShop] ERROR: Unknown Coin Type:%d for Item (%d)(%d)", Iter->btCoinType, lpMsg->ItemIndex, lpMsg->ItemSubIndex);
			UserCoin = 0.0;
			break;
	}
	
	if(UserCoin < Iter->wPrice)
	{
		pMsg.Result = 1;
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed (Gift) - Item (%d/%d): not enough money to purchase (need:%d/have:%d)", lpObj->AccountID, lpObj->Name, lpMsg->ItemIndex, lpMsg->ItemSubIndex, Iter->wPrice, UserCoin);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(Iter->wPackageID > 0)
	{
		if(Iter->btRandomItemPackage == TRUE)
		{
			int GUID = -1, ID = -1;
			bool bResult = this->GetRandomItemFromPackage(Iter->wPackageID, GUID, ID);

			if(GUID == -1 || ID == -1)
			{
				pMsg.Result = 7;
				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				g_Log.AddC(TColor::Blue, "[ItemShop] ERROR: [PackageID:%d] Cannot get RandomItem (GUID == -1 || ID == -1)",Iter->wPackageID);
				return;
			}

			GDReqInGameShopItemGift(lpObj->m_Index, 673, GUID, ID,
				Iter->wPrice, Iter->btCoinType, lpMsg->szName, lpMsg->szText);
		}

		else
		{

			std::vector<ITEMSHOP_PACKAGELIST>::iterator It;
			BYTE cBUFF[65535];
			ISHOP_ITEM_GIFT_PACKAGE * pMsg = (ISHOP_ITEM_GIFT_PACKAGE *)(cBUFF);
			ISHOP_ITEM_PACKAGE * pItem;

			pMsg->Count = 0;

			for( It = this->VecPackageList.begin() ; It != this->VecPackageList.end() ; It++ )
			{
				if(It->PackageID == Iter->wPackageID)
				{
					pItem = (ISHOP_ITEM_PACKAGE *)(cBUFF + sizeof(ISHOP_ITEM_GIFT_PACKAGE) + (pMsg->Count * sizeof(ISHOP_ITEM_PACKAGE)));

					pItem->ID1 = 673;
					pItem->ID2 = It->wItemInfoGUID;
					pItem->ID3 = It->wItemInfoID;

					pMsg->Count++;
				}
			}
		
			WORD PacketSize = sizeof(ISHOP_ITEM_GIFT_PACKAGE) + (pMsg->Count * sizeof(ISHOP_ITEM_PACKAGE));

			pMsg->CoinType = Iter->btCoinType;
			pMsg->Price = Iter->wPrice;

			pMsg->aIndex = lpObj->m_Index;
			memcpy(pMsg->AccountID, lpObj->AccountID, 11);
			memcpy(pMsg->Name, lpObj->Name, 11);
			memcpy(pMsg->TargetName, lpMsg->szName, 11);
			memcpy(pMsg->Message, lpMsg->szText, 200);

			pMsg->h.c = 0xC2;
			pMsg->h.headcode = 0xD8;
			pMsg->h.subcode = 0x02;
			pMsg->h.sizeH = HIBYTE(PacketSize);
			pMsg->h.sizeL = LOBYTE(PacketSize);

			wsDataCli.DataSend((PCHAR)&cBUFF, PacketSize);

			g_Log.Add("[ItemShop] (%s)(%s) Purchase Request (Gift): [Package ID: %d] [Items: %d] [Target:%s]", lpObj->AccountID, lpObj->Name, Iter->wPackageID, pMsg->Count, lpMsg->szName);
			ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Request (Gift): [Package ID: %d] [Items: %d] [Target:%s]", lpObj->AccountID, lpObj->Name, Iter->wPackageID, pMsg->Count, lpMsg->szName);
		}
	}

	else
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Request (Gift): (GUID:%d) (ID:%d) (Price:%d) (CoinType:%d) (Target:%s)", lpObj->AccountID, lpObj->Name, Iter->wItemInfoGUID, Iter->wItemInfoID, Iter->wPrice, Iter->btCoinType, lpMsg->szName);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Request (Gift): (GUID:%d) (ID:%d) (Price:%d) (CoinType:%d) (Target:%s)", lpObj->AccountID, lpObj->Name, Iter->wItemInfoGUID, Iter->wItemInfoID, Iter->wPrice, Iter->btCoinType, lpMsg->szName);
	

		GDReqInGameShopItemGift(lpObj->m_Index, 673, Iter->wItemInfoGUID, Iter->wItemInfoID,
			Iter->wPrice, Iter->btCoinType, lpMsg->szName, lpMsg->szText);
	}

	if(Iter->ItemGoblinPoint > 0)
	{
		GDReqInGameShopPointAdd(lpObj->m_Index, 2, Iter->ItemGoblinPoint);
	}
}

void CItemShop::GCCashItemBuy(LPOBJ lpObj, int ID1, int ID2, int ID3, int Result)
{
	PMSG_ANS_INGAMESHOP_ITEMBUY pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x03, sizeof(pMsg));

	std::map<int, ITEMSHOP_ITEMINFO>::iterator Iter = this->MapItemInfo.find(ID3);

	if(Result == 3 || Iter == this->MapItemInfo.end() )
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed (%d) - Item does not found in CashItemList (ItemInfo) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Failed (%d) - Item does not found in CashItemList (ItemInfo) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		pMsg.Result = 2;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else if(Result == 2)
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed (%d) -  Not enough money (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Failed (%d) - Not enough money (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		pMsg.Result = 1;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else if(Result == 1)
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Success (%d) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Success (%d) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		
		pMsg.Result = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
		pList.InventoryType = 0x53;
		pList.PageIndex = 1;

		this->CGCashPoint(lpObj);
		this->CGCashInventoryItemCount(lpObj, &pList);
	}
}

void CItemShop::GCCashItemGift(LPOBJ lpObj, int ID1, int ID2, int ID3, int Result)
{
	PMSG_ANS_INGAMESHOP_ITEMGIFT pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x04, sizeof(pMsg));

	if(Result == 3)
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed (Gift) (%d) - Item does not found in CashItemList (ItemInfo) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Failed (Gift) (%d) - Item does not found in CashItemList (ItemInfo) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		pMsg.Result = 3;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else if(Result == 2)
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Failed (Gift) (%d) -  Not enough money (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Failed (Gift) (%d) - Not enough money (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		pMsg.Result = 1;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	else if(Result == 1)
	{
		g_Log.Add("[ItemShop] (%s)(%s) Purchase Success (Gift) (%d) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		ITEMSHOP_LOG->Output("[ItemShop] (%s)(%s) Purchase Success (Gift) (%d) (GUID:%d)(ID:%d)", lpObj->AccountID, lpObj->Name, Result, ID2, ID3);
		
		pMsg.Result = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		this->CGCashPoint(lpObj);
	}
}

void CItemShop::GCShopVersion(LPOBJ lpObj)
{
	PMSG_SHOP_VERSION pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0C, sizeof(pMsg));
	pMsg.ver.Ver1 = this->ItemVer1;
	pMsg.ver.Ver2 = this->ItemVer2;
	pMsg.ver.Ver3 = this->ItemVer3;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x15, sizeof(pMsg));
	pMsg.ver.Ver1 = this->BannerVer1;
	pMsg.ver.Ver2 = this->BannerVer2;
	pMsg.ver.Ver3 = this->BannerVer3;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

BOOL IsCashItem(int iItemCode)
{
	switch (iItemCode)
	{
	case ITEMGET(13, 43): return TRUE;
	case ITEMGET(13, 44): return TRUE;
	case ITEMGET(13, 45): return TRUE;

	case ITEMGET(13, 46): return TRUE;
	case ITEMGET(13, 47): return TRUE;
	case ITEMGET(13, 48): return TRUE;

	case ITEMGET(14, 53): return TRUE;
	case ITEMGET(14, 54): return TRUE;

	case ITEMGET(14, 58): return TRUE;
	case ITEMGET(14, 59): return TRUE;
	case ITEMGET(14, 60): return TRUE;
	case ITEMGET(14, 61): return TRUE;
	case ITEMGET(14, 62): return TRUE;

	case ITEMGET(13, 54):
	case ITEMGET(13, 55):
	case ITEMGET(13, 56):
	case ITEMGET(13, 57):
	case ITEMGET(13, 58):
	case ITEMGET(13, 59): //Seal of Mobility
	case ITEMGET(13, 60): //Indulgence
	case ITEMGET(13, 61): //Illusion Temple Ticket

	case ITEMGET(14, 70):
	case ITEMGET(14, 71):

	case ITEMGET(14, 72):
	case ITEMGET(14, 73):
	case ITEMGET(14, 74):
	case ITEMGET(14, 75):
	case ITEMGET(14, 76):
	case ITEMGET(14, 77):
	case ITEMGET(14, 78):
	case ITEMGET(14, 79):
	case ITEMGET(14, 80):
	case ITEMGET(14, 81):
	case ITEMGET(14, 82):
	case ITEMGET(14, 83):

	case ITEMGET(14, 94): //Medium Elite Potion
		return TRUE;
	case ITEMGET(13, 93): //Seal of Master Ascension
	case ITEMGET(13, 94): //Seal of Master Wealth

		//Season3 add-on
	case ITEMGET(13, 62): //Seal of Healing
	case ITEMGET(13, 63): //Seal of Divinity
	case ITEMGET(14, 97): //Scroll of Battle
	case ITEMGET(14, 98): //Scroll of Strenghthener
		return TRUE;
	}
	return FALSE;
}

BOOL IsPremiumItem(int iItemCode) //00631460
{
	switch (iItemCode)
	{
	case ITEMGET(14,54): // Chaos Card
	case ITEMGET(13,64): // Demon
	case ITEMGET(13,65): // Spirit of Guardian
	case ITEMGET(14,96): // Talisman of Chaos Assembly
	case ITEMGET(13,76): // Panda Ring
	case ITEMGET(13,77): // Panda Ring
	case ITEMGET(13,78): // Panda Ring
	case ITEMGET(13,80): // Panda Pet
	case ITEMGET(13,106): // Pet Unicorn
	case ITEMGET(13,122): // Skeleton Transformation Ring
	case ITEMGET(13,123): // Pet Skeleton
		return TRUE;
	}
	return FALSE;
}

bool CItemShop::CheckBuyCondition(LPOBJ lpObj, int ItemCode)
{
	if(ItemCode == ITEMGET(14,91) && (lpObj->EnableCharacterCreate & 1) == 1)
		return false;

	if(ItemCode == ITEMGET(14,169) && (lpObj->EnableCharacterCreate & 8) == 8)
		return false;

	if(ItemCode == ITEMGET(14,162) && lpObj->m_PlayerData->m_InventoryExpansion == 2)
		return false;

	if(ItemCode == ITEMGET(14,163) && lpObj->m_PlayerData->m_WarehouseExpansion == 1)
		return false;

	if((ItemCode == ITEMGET(13,93) || ItemCode == ITEMGET(13,94) || ItemCode == ITEMGET(13,181)
		|| ItemCode == ITEMGET(14,225) || ItemCode == ITEMGET(14,226) || ItemCode == ITEMGET(14,227)
		|| ItemCode == ITEMGET(14,228) || ItemCode == ITEMGET(14,229) || ItemCode == ITEMGET(14,230)
		|| ItemCode == ITEMGET(14,231) || ItemCode == ITEMGET(14,232) || ItemCode == ITEMGET(14,235))
		&& lpObj->m_PlayerData->ChangeUP != 2)
		return false;

	return true;
}

bool CItemShop::GetRandomItemFromPackage(int PackageID, int & GUID, int & ID)
{
	std::map<int, ITEMSHOP_PACKAGELIST> m_items;

	for(std::vector<ITEMSHOP_PACKAGELIST>::iterator It = this->VecPackageList.begin(); It != this->VecPackageList.end(); It++)
	{
		if(It->PackageID == PackageID)
		{
			m_items.insert(std::pair<int, ITEMSHOP_PACKAGELIST>(It->ItemSeqNum, *It));
		}
	}

	int count = m_items.size();

	if(count == 0)
	{
		return false;
	}

	int item = rand()%count;

	GUID = m_items[item].wItemInfoGUID;
	ID = m_items[item].wItemInfoID;

	return true;
}

std::vector<ITEMSHOP_ITEMLIST>::iterator CItemShop::FindItemInList(int iIndex, int iSubIndex, int iOption, int iCategory)
{
	std::vector<ITEMSHOP_ITEMLIST>::iterator It;

	for(It = this->VecItemList.begin(); It != this->VecItemList.end(); It++)
	{
		if(It->wItemIndex == iIndex && It->wItemSubIndex == iSubIndex && It->wItemOptionSelect == iOption && It->ItemCategory == iCategory)
		{
			return It;
		}
	}

	return this->VecItemList.end();
}

void CItemShop::AddGPMonster(LPOBJ lpMonsterObj)
{
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpMonsterObj);

	if(MaxHitUser == -1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[MaxHitUser];

	if(lpMonsterObj->Type != OBJ_MONSTER ||
		lpObj->Type != OBJ_USER)
	{
		return;
	}

	for(std::vector<GP_PERMONSTER_DATA>::iterator It = this->VecGpMonster.begin(); It != this->VecGpMonster.end(); It++)
	{
		if(It->MonsterID == lpMonsterObj->Class && (It->MapNumber == lpMonsterObj->MapNumber || It->MapNumber == 0xFFFF))
		{
			GDReqInGameShopPointAdd(lpObj->m_Index, It->CoinType, It->Coins);
			return;
		}
	}
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

