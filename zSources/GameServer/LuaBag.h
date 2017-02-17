//////////////////////////////////////////////////////////////////////
// LuaBag.h
#ifndef LUABAG_H
#define LUABAG_H

#include "Bag.h"
#include "MuLua.h"
#include "zzzitem.h"
#include "GremoryCase.h"

#pragma once

#define MAX_ITEM_VARIABLES 16
#define ITEMBAG_SCRIPT_FILE "\\Scripts\\ItemBags\\ItemBagScript.lua"

static struct ItemMapInt
{
	const char *	Name;
	int				Variable;
};

class CLuaBag
{
public:
	CLuaBag();
	virtual ~CLuaBag();

	void Init();

	void InsertItemMap(BAG_ITEM * m_Item);
	static BOOL	GetVariableItem(lua_State * Lua);
	static BOOL	SetVariableItem(lua_State * Lua);

	bool DropCommonBag(int aIndex, BYTE MapNumber, BYTE DropX, BYTE DropY, BAG_ITEM * m_Item);
	bool DropMonsterBag(int aIndex, int iMonsterIndex, BYTE MapNumber, BYTE DropX, BYTE DropY, BAG_ITEM * m_Item);
	bool DropEventBag(int aIndex, int iMonsterIndex, BYTE MapNumber, BYTE DropX, BYTE DropY, BAG_ITEM * m_Item);
	bool MakeItemFromBag(int aIndex, BAG_ITEM * m_Item, CItem & Item, time_t & Duration);
	bool MakeItemFromBagForGremoryCase(int aIndex, BAG_ITEM * m_Item, _stGremoryCaseItem & Item);

	void LoadItemBag();

	ItemMapInt			m_ItemInfo[MAX_ITEM_VARIABLES];
	MULua				m_Lua;
};

extern CLuaBag gLuaBag;

const struct luaL_Reg RegisterItemInfo[] =
{
	{ "__index",		CLuaBag::GetVariableItem },
	{ "__newindex",		CLuaBag::SetVariableItem },
	{ NULL,				NULL },
};

#endif


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

