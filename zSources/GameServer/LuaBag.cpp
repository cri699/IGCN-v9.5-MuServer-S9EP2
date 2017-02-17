//////////////////////////////////////////////////////////////////////
// LuaBag.cpp
#include "stdafx.h"
#include "LuaBag.h"
#include "configread.h"
#include "MuunSystem.h"

CLuaBag gLuaBag;

CLuaBag::CLuaBag() : m_Lua(true)
{
}

CLuaBag::~CLuaBag()
{

}

void CLuaBag::Init()
{
	this->m_Lua.RegisterData(RegisterItemInfo, "RegisterItemInfo", "ItemInfo", sizeof(this->m_ItemInfo));
	this->m_Lua.DoFile(g_ConfigRead.GetPath(ITEMBAG_SCRIPT_FILE));
}

void CLuaBag::InsertItemMap(BAG_ITEM * m_Item)
{
	this->m_ItemInfo[0].Name = "ItemType";
	this->m_ItemInfo[0].Variable = m_Item->btItemType;

	this->m_ItemInfo[1].Name = "ItemIndex";
	this->m_ItemInfo[1].Variable = m_Item->wItemIndex;

	this->m_ItemInfo[2].Name = "ItemMinLevel";
	this->m_ItemInfo[2].Variable = m_Item->btItemMinLevel;

	this->m_ItemInfo[3].Name = "ItemMaxLevel";
	this->m_ItemInfo[3].Variable = m_Item->btItemMaxLevel;

	this->m_ItemInfo[4].Name = "Skill";
	this->m_ItemInfo[4].Variable = m_Item->btIsSkill;

	this->m_ItemInfo[5].Name = "Luck";
	this->m_ItemInfo[5].Variable = m_Item->btIsLuck;

	this->m_ItemInfo[6].Name = "Option";
	this->m_ItemInfo[6].Variable = m_Item->btIsOption;

	this->m_ItemInfo[7].Name = "Exc";
	this->m_ItemInfo[7].Variable = m_Item->btIsExItem;

	this->m_ItemInfo[8].Name = "Anc";
	this->m_ItemInfo[8].Variable = m_Item->btIsSetItem;

	this->m_ItemInfo[9].Name = "Socket";
	this->m_ItemInfo[9].Variable = m_Item->btIsSocketItem;

	this->m_ItemInfo[10].Name = "Elemental";
	this->m_ItemInfo[10].Variable = m_Item->btIsElementalItem;

	this->m_ItemInfo[11].Name = "MuunEvolutionItemType";
	this->m_ItemInfo[11].Variable = m_Item->btMuunEvolutionItemType;

	this->m_ItemInfo[12].Name = "MuunEvolutionItemIndex";
	this->m_ItemInfo[12].Variable = m_Item->wMuunEvolutionItemIndex;

	LPITEM_ATTRIBUTE p = GetItemAttr(ITEMGET(m_Item->btItemType, m_Item->wItemIndex));

	this->m_ItemInfo[13].Name = "KindA";
	this->m_ItemInfo[13].Variable = p->ItemKindA;

	this->m_ItemInfo[14].Name = "Durability";
	this->m_ItemInfo[14].Variable = m_Item->btItemDur;

	this->m_ItemInfo[15].Name = "Duration";
	this->m_ItemInfo[15].Variable = m_Item->dwDuration;
}

BOOL CLuaBag::GetVariableItem(lua_State * Lua)
{
	const char * VarName = luaL_checkstring(Lua, 2);

	for (int i = 0; i < (sizeof(gLuaBag.m_ItemInfo) / sizeof(gLuaBag.m_ItemInfo[0])); i++)
	{
		if (!strcmp(gLuaBag.m_ItemInfo[i].Name, VarName))
		{
			lua_pushinteger(Lua, gLuaBag.m_ItemInfo[i].Variable);
			return true;
		}
	}

	return false;
}

BOOL CLuaBag::SetVariableItem(lua_State * Lua)
{
	const char * VarName = luaL_checkstring(Lua, 2);

	for (int i = 0; i < (sizeof(gLuaBag.m_ItemInfo) / sizeof(gLuaBag.m_ItemInfo[0])); i++)
	{
		if (!strcmp(gLuaBag.m_ItemInfo[i].Name, VarName))
		{
			return true;
		}
	}

	return false;
}

bool CLuaBag::DropCommonBag(int aIndex, BYTE MapNumber, BYTE DropX, BYTE DropY, BAG_ITEM *m_Item)
{
	this->InsertItemMap(m_Item);

	int iResult;
	this->m_Lua.Generic_Call("CommonBagItemDrop", "iiii>i", aIndex, MapNumber, DropX, DropY, &iResult);

	return iResult;
}

bool CLuaBag::DropMonsterBag(int aIndex, int iMonsterIndex, BYTE MapNumber, BYTE DropX, BYTE DropY, BAG_ITEM *m_Item)
{
	this->InsertItemMap(m_Item);

	int iResult;
	this->m_Lua.Generic_Call("MonsterBagItemDrop", "iiiii>i", iMonsterIndex, MapNumber, DropX, DropY, aIndex, &iResult);

	return iResult;
}

bool CLuaBag::DropEventBag(int aIndex, int iMonsterIndex, BYTE MapNumber, BYTE DropX, BYTE DropY, BAG_ITEM *m_Item)
{
	this->InsertItemMap(m_Item);

	int iResult;
	this->m_Lua.Generic_Call("EventBagItemDrop", "iiiii>i", iMonsterIndex, MapNumber, DropX, DropY, aIndex, &iResult);

	return iResult;
}

void CLuaBag::LoadItemBag()
{
	this->m_Lua.Generic_Call("LoadItemBag", ">");
}

bool CLuaBag::MakeItemFromBag(int aIndex, BAG_ITEM * m_Item, CItem & Item, time_t & DurationItem)
{
	this->InsertItemMap(m_Item);

	int SocketCount = 0;
	int MuunEvolutionItemID = -1;
	int Durability = 0;
	int Duration = 0;

	this->m_Lua.Generic_Call("EventBagMakeItem", ">iiiiiiiiiii",
		&Item.m_Level, &Durability, &Item.m_Option1, &Item.m_Option2, &Item.m_Option3,
		&Item.m_NewOption, &Item.m_SetOption, &Item.m_BonusSocketOption, &SocketCount, &MuunEvolutionItemID, &Duration);

	Item.m_Type = ITEMGET(m_Item->btItemType, m_Item->wItemIndex);
	Item.m_Durability = Durability;
	DurationItem = Duration;

	if (g_CMuunSystem.IsStoneofEvolution(Item.m_Type) == true)
	{
		memset(Item.m_SocketOption, -1, sizeof(Item.m_SocketOption));

		Item.m_SocketOption[0] = MuunEvolutionItemID >> 8;
		Item.m_SocketOption[1] = ITEM_GET_INDEX(MuunEvolutionItemID);
	}

	else
	{
		for (int i = 0; i < SocketCount; i++)
		{
			Item.m_SocketOption[i] = 0xFE;
		}

	}

	return true;
}

bool CLuaBag::MakeItemFromBagForGremoryCase(int aIndex, BAG_ITEM * m_Item, _stGremoryCaseItem & Item)
{
	this->InsertItemMap(m_Item);

	int SocketCount = 0;
	int MuunEvolutionItemID = -1;
	int Durability = 0;
	int Duration = 0;

	this->m_Lua.Generic_Call("EventBagMakeItem", ">iiiiiiiii",
		&Item.btItemLevel, &Durability, &Item.btItemSkill, &Item.btItemLuck, &Item.btItemOption,
		&Item.btItemExcOption, &Item.btItemSetOption, &Item.btItemMainAttribute, &Item.btItemSocketCount, &MuunEvolutionItemID, &Duration);

	Item.wItemID = ITEMGET(m_Item->btItemType, m_Item->wItemIndex);
	
	if (Durability == 0)
	{
		Item.btItemDurability = ItemGetDurability(Item.wItemID, Item.btItemLevel, Item.btItemExcOption, Item.btItemSetOption);
	}

	else
	{
		Item.btItemDurability = Durability;
	}

	Item.wMuunEvoItemType = ITEM_GET_TYPE(MuunEvolutionItemID);
	Item.btMuunEvoItemIndex = ITEM_GET_INDEX(MuunEvolutionItemID);

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

