//////////////////////////////////////////////////////////////////////
// UpgradeCmd.cpp
#include "stdafx.h"
#include "UpgradeCmd.h"
#include "zzzitem.h"
#include "TLog.h"
#include "user.h"
#include "DSProtocol.h"
#include "SetItemOption.h"
#include "ItemOptionTypeMng.h"

CUpgradeCmd g_UpgradeCmd;

CUpgradeCmd::CUpgradeCmd()
{
	this->m_bSystemEnable = false;
}

CUpgradeCmd::~CUpgradeCmd()
{
}

void CUpgradeCmd::LoadFile(char * szFile)
{
	this->m_mapUpgradeData.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		this->m_bSystemEnable = false;
		return;
	}

	pugi::xml_node main = file.child("JoinMu");

	this->m_bSystemEnable = main.attribute("Enable").as_bool();

	UPGRADECMD_DATA m_Upgrade;

	for (pugi::xml_node need_item = main.child("NeedItem"); need_item; need_item = need_item.next_sibling())
	{
		for (pugi::xml_node item = need_item.child("Item"); item; item = item.next_sibling())
		{
			m_Upgrade.iUpgradeID = item.attribute("ID").as_int();
			BYTE btItemCat = item.attribute("Cat").as_int();
			WORD wItemIndex = item.attribute("Index").as_int();

			m_Upgrade.m_NeedItem.wItemID = ITEMGET(btItemCat, wItemIndex);

			if (m_Upgrade.m_NeedItem.wItemID == (WORD)-1)
			{
				g_Log.MsgBox("Upgrade Item Cmd - Wrong Need Item found (System disabled) (%d/%d)", btItemCat, wItemIndex);
				this->m_bSystemEnable = false;
				return;
			}

			m_Upgrade.m_NeedItem.btItemLevel = item.attribute("Level").as_int();
			m_Upgrade.m_NeedItem.btOption = item.attribute("Option").as_int();
			m_Upgrade.m_NeedItem.btSetOption = item.attribute("SetOption").as_int();
			m_Upgrade.m_NeedItem.btExc = item.attribute("Exc").as_int();
			m_Upgrade.m_NeedItem.btSocketOption[0] = item.attribute("Socket1").as_int();
			m_Upgrade.m_NeedItem.btSocketOption[1] = item.attribute("Socket2").as_int();
			m_Upgrade.m_NeedItem.btSocketOption[2] = item.attribute("Socket3").as_int();
			m_Upgrade.m_NeedItem.btSocketOption[3] = item.attribute("Socket4").as_int();
			m_Upgrade.m_NeedItem.btSocketOption[4] = item.attribute("Socket5").as_int();

			this->m_mapUpgradeData.insert(std::pair<int, UPGRADECMD_DATA>(m_Upgrade.iUpgradeID, m_Upgrade));
		}
	}

	std::map<int, UPGRADECMD_DATA>::iterator Iter;

	for (pugi::xml_node get_item = main.child("GetItem"); get_item; get_item = get_item.next_sibling())
	{
		for (pugi::xml_node item = get_item.child("Item"); item; item = item.next_sibling())
		{
			int iUpgradeID = item.attribute("ID").as_int();
			Iter = this->m_mapUpgradeData.find(iUpgradeID);

			if (Iter == this->m_mapUpgradeData.end())
			{
				g_Log.MsgBox("Upgrade Item Cmd - Wrong Get Item found (not matched with Need Item) (System disabled) (%d)", iUpgradeID);
				this->m_bSystemEnable = false;
				return;
			}

			BYTE btItemCat = item.attribute("Cat").as_int();
			WORD wItemIndex = item.attribute("Index").as_int();

			Iter->second.m_GetItem.wItemID = ITEMGET(btItemCat, wItemIndex);

			if (Iter->second.m_GetItem.wItemID == (WORD)-1)
			{
				g_Log.MsgBox("Upgrade Item Cmd - Wrong Get Item found (System disabled) (%d/%d)", btItemCat, wItemIndex);
				this->m_bSystemEnable = false;
				return;
			}

			LPITEM_ATTRIBUTE p = GetItemAttr(Iter->second.m_GetItem.wItemID);

			if (p == NULL)
			{
				g_Log.MsgBox("Upgrade Item Cmd - Wrong Get Item found (Not exists in ItemList.xml) (System disabled) (%d/%d)", btItemCat, wItemIndex);
				this->m_bSystemEnable = false;
				return;
			}

			Iter->second.m_GetItem.btItemLevel = item.attribute("Level").as_int();
			Iter->second.m_GetItem.btOption = item.attribute("Option").as_int();
			Iter->second.m_GetItem.btSetOption = item.attribute("SetOption").as_int();
			Iter->second.m_GetItem.btExc = item.attribute("Exc").as_int();
			Iter->second.m_GetItem.btSocketOption[0] = item.attribute("Socket1").as_int();
			Iter->second.m_GetItem.btSocketOption[1] = item.attribute("Socket2").as_int();
			Iter->second.m_GetItem.btSocketOption[2] = item.attribute("Socket3").as_int();
			Iter->second.m_GetItem.btSocketOption[3] = item.attribute("Socket4").as_int();
			Iter->second.m_GetItem.btSocketOption[4] = item.attribute("Socket5").as_int();
		}
	}
}

int CUpgradeCmd::DoUpgrade(int aIndex)
{
	if (this->m_bSystemEnable == false)
	{
		return -1;
	}

	if (!ObjectMaxRange(aIndex))
	{
		return -1;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for (int i = INVETORY_WEAR_SIZE; i < INVENTORY_SIZE - 1; i++) // Pentagram excluded, so -1
	{
		if (PSHOP_RANGE(i) == TRUE)
		{
			continue;
		}

		if (lpObj->pInventory[i].IsItem() == false)
		{
			continue;
		}

		for (std::map<int, UPGRADECMD_DATA>::iterator It = this->m_mapUpgradeData.begin(); It != this->m_mapUpgradeData.end(); It++)
		{
			if (It->second.m_NeedItem.wItemID != lpObj->pInventory[i].m_Type)
			{
				continue;
			}

			if (It->second.m_NeedItem.btItemLevel != lpObj->pInventory[i].m_Level)
			{
				continue;
			}

			if (It->second.m_NeedItem.btOption != lpObj->pInventory[i].m_Option3)
			{
				continue;
			}

			if (It->second.m_NeedItem.btSetOption != lpObj->pInventory[i].m_SetOption)
			{
				continue;
			}

			if (It->second.m_NeedItem.btExc != lpObj->pInventory[i].m_NewOption)
			{
				continue;
			}

			if (memcmp(It->second.m_NeedItem.btSocketOption, lpObj->pInventory[i].m_SocketOption, 5) != 0)
			{
				continue;
			}

			if (CheckInventoryEmptySpace(lpObj, 4, 4) == FALSE)
			{
				MsgOutput(aIndex, "You need 4x4 empty space in inventory");
				return 0;
			}

			CItem CopyItem = lpObj->pInventory[i];
			CItem NewItem;

			gObjInventoryDeleteItem(aIndex, i);
			GSProtocol.GCInventoryItemDeleteSend(aIndex, i, 1);

			NewItem.m_Type = It->second.m_GetItem.wItemID;
			NewItem.m_Level = It->second.m_GetItem.btItemLevel;
			NewItem.m_Option1 = CopyItem.m_Option1;
			NewItem.m_Option2 = CopyItem.m_Option2;
			NewItem.m_Option3 = It->second.m_GetItem.btOption;

			if (It->second.m_GetItem.btSetOption == (BYTE)-1)
			{
				NewItem.m_SetOption = gSetItemOption.GenSetOption(NewItem.m_Type);
			}

			else
			{
				NewItem.m_SetOption = It->second.m_GetItem.btSetOption;
			}

			if (It->second.m_GetItem.btExc == (BYTE)-1)
			{
				NewItem.m_NewOption = g_ItemOptionTypeMng.CommonExcOptionRand(ItemAttribute[NewItem.m_Type].ItemKindA);
			}

			else if (It->second.m_GetItem.btExc == (BYTE)-3)
			{
				NewItem.m_NewOption = CopyItem.m_NewOption;
			}

			else
			{
				NewItem.m_NewOption = It->second.m_GetItem.btExc;
			}

			for (int j = 0; j < 5; j++)
			{
				if (It->second.m_GetItem.btSocketOption[j] == (BYTE)-3)
				{
					NewItem.m_SocketOption[j] = CopyItem.m_SocketOption[j];
				}

				else
				{
					NewItem.m_SocketOption[j] = It->second.m_GetItem.btSocketOption[j];
				}
			}

			NewItem.m_Durability = ItemGetDurability(NewItem.m_Type, NewItem.m_Level, NewItem.m_NewOption, NewItem.m_SetOption);

			ItemSerialCreateSend(aIndex, 235, 0, 0, NewItem.m_Type, NewItem.m_Level, NewItem.m_Durability, NewItem.m_Option1, NewItem.m_Option2, NewItem.m_Option3, aIndex, NewItem.m_NewOption, NewItem.m_SetOption, 0, NewItem.m_SocketOption, 0);
			return 1;
		}
	}
	
	return 0;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

