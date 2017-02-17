//////////////////////////////////////////////////////////////////////
// MapItem.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#include "stdafx.h"
//#include "MapItem.h"
#include "GameMain.h"

// GS-N 0.99.60T 0x0047BE60 - Completed
//	GS-N	1.00.18	JPN	0x00493830	-	Completed

CMapItem::CMapItem()
{
	this->Init();
}



CMapItem::~CMapItem()
{
	return;

}



void CMapItem::Init()
{
	this->VPCount = 0;
	this->m_State = 0;

	for ( int n=0;n<MAX_VIEWPORT;n++ )
	{
		memset(&this->VpPlayer[n], 0, sizeof(VIEWPORT_PLAYER_STRUCT));
	}
}



void CMapItem::CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, UINT64 number, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption)
{
	this->Init();

	this->m_Level = level;
	this->m_Durability = dur;
	this->Convert(type, Option1, Option2, Option3, NOption, SOption, ItemEffectEx, SocketOption, SocketBonusOption, 0, CURRENT_DB_VERSION);
	this->px = x;
	this->py= y;
	this->live = true;
	this->Give = false;
	this->m_State = 1;

	if ( this->m_QuestItem != false )
	{
		this->m_Time = GetTickCount() + 60000;
		this->m_LootTime = GetTickCount() + 600000;
		this->m_Number = number;
	}
	else
	{
		this->m_Time = GetTickCount() + gItemDisapearTime*1000;//120000; // 2m inutes
		this->m_LootTime = GetTickCount() + gLootingTime*1000;
		this->m_Number = number;
	}

}

void CMapItem::DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, UINT64 number, int PetLevel, UINT64 PetExp, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption)
{
	this->Init();

	this->m_Level = level;
	this->m_Durability = dur;
	this->Convert(type, Option1, Option2, Option3, NOption, SOption, ItemEffectEx, SocketOption, SocketBonusOption, 0, CURRENT_DB_VERSION);
	this->SetPetItemInfo(PetLevel, PetExp);
	this->px = x;
	this->py= y;
	this->live = true;
	this->Give = false;
	this->m_State = 1;

	if ( this->m_QuestItem != false )
	{
		if ( type == ITEMGET(14,11) )	// Box Of luck
		{
			this->m_Time = GetTickCount() + 1000;
		}
		else
		{
			this->m_Time = GetTickCount() + 60000;
		}

		this->m_LootTime = GetTickCount() + 600000;
		this->m_Number = number;
	}
	else 
	{
		if ( type == ITEMGET(14,11) )
		{
			this->m_Time = GetTickCount() + 1000;
		}
		else
		{
			this->m_Time = GetTickCount() + gItemDisapearTime*1000;
		}

		this->m_LootTime = GetTickCount() + gLootingTime*1000;
		this->m_Number = number;
	}

	if ( type == ITEMGET(13,19) ) // Absolute Weapon of Archangel
	{
		if ( ( level  < 0)?FALSE:(level>2)?FALSE:TRUE)
		{
			this->m_Time = GetTickCount() + 900000;
			this->m_LootTime = GetTickCount();
		}
	}

	if (type == ITEMGET(14, 63))
	{
		this->m_Time = GetTickCount() + 20000;
	}

	if (type == ITEMGET(14, 223))
	{
		this->m_Time = GetTickCount() + 300000;
	}
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

