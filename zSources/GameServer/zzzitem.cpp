//////////////////////////////////////////////////////////////////////
// zzzitem.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x004E5740 - Status Completed :)
// GS-N	1.00.18	JPN	0x0051E110	-	Completed

#include "StdAfx.h"
#include "zzzitem.h"
#include "SetItemOption.h"
#include "TLog.h"
#include "JewelOfHarmonySystem.h"
#include "ItemSystemFor380.h"
#include "user.h"
#include "ItemSocketOptionSystem.h"
#include "configread.h"
#include "GameMain.h"
#include "classdef.h"
#include "LuckyItemManager.h"
#include "PentagramSystem.h"
#include "MuunSystem.h"

ITEM_ATTRIBUTE ItemAttribute[MAX_ITEMS];
CPetItemExp gPetItemExp;
int g_MaxItemIndexOfEachItemType[MAX_TYPE_ITEMS];
RESETITEMLIST gObjResetItem[1000];

CItem::CItem()
{
	this->Clear();
}

void CItem::Clear()	// Fine
{
	this->m_Type =-1;
	this->m_Level =0;
	this->m_Part=0;
	this->m_Class=0;
	this->m_TwoHand=0;
	this->m_AttackSpeed=0;
	this->m_DamageMin=0;
	this->m_DamageMax=0;
	this->m_SuccessfulBlocking=0;
	this->m_Defense=0;
	this->m_MagicDefense =0;
	this->m_Durability=0;
	this->m_SpecialNum=0;
	this->m_Value=0;
	this->m_Option1 =0;
	this->m_Option2 =0;
	this->m_Option3 =0;
	this->m_NewOption =0;
	this->m_Number =0;
	this->m_DurabilitySmall =0;
	this->m_iPShopValue=-1;
	this->m_bItemExist=true;
	this->m_CurrentDurabilityState=-1;
	this->m_SetOption=0;
	this->m_QuestItem=false;

	memset(this->m_Special, 0, sizeof(this->m_Special));
	memset(this->m_SpecialValue, 0, sizeof(this->m_SpecialValue));
	memset(this->m_Resistance, 0, sizeof(this->m_Resistance));

	this->m_IsLoadPetItemInfo=0;
	this->m_PetItem_Level =1;
	this->m_PetItem_Exp =0;
	this->m_Leadership=0;
	this->m_JewelOfHarmonyOption = 0;
	this->m_ItemOptionEx = 0;
	this->m_BonusSocketOption = -1;
	memset(this->m_SocketOption, -1, sizeof(this->m_SocketOption));
	this->m_PeriodItemOption = 0;
	this->m_ImproveDurabilityRate = 0;
	this->m_wPShopBlessValue = 0;
	this->m_wPShopSoulValue = 0;
	this->m_wPShopChaosValue = 0;
}

BOOL CItem::IsItem() // Good
{
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int CItem::IsSetItem() // Good
{
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		return this->m_SetOption & 3;	// Limit of the SetItem
	}
}

void CItem::Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, BYTE PeriodItemOption, BYTE DbVersion)
{
	int _type;
	int ItemLevel;
	LPITEM_ATTRIBUTE p;
	int SOption;
	int SOptionStatValue;
	int iChaosItem;
	int i;
	
	_type = type;
	
	if ( _type >MAX_ITEMS-1)
	{
		g_Log.AddC(TColor::Red,  "error-L1 : ItemIndex error %d", _type);
	}
	
	p=&ItemAttribute[_type];
	this->m_serial=ItemAttribute[_type].Serial ;
	this->m_Type=type;
	
	if ( (DbVersion ) <= 2)
	{
		this->m_Type=_type;
	}

	this->m_NewOption =Attribute2;
	
	if (p->OptionFlag  == 0)
	{
		this->m_NewOption=0;
	}

	if ( ((_type >= ITEMGET(12,3) ) && (_type <= ITEMGET(12,6) ) ) || ((_type >= ITEMGET(12,36) ) && (_type <= ITEMGET(12,40) ) ) || (_type == ITEMGET(13,30) ) || (_type == ITEMGET(12,41)) || (_type == ITEMGET(12,42)) || (_type == ITEMGET(12,43)) || (_type == ITEMGET(12,49)) || (_type == ITEMGET(12,50)) || ((_type >= ITEMGET(12,262) && _type <= ITEMGET(12,265))) ) //Third Wings Fix
	{
		Attribute2=0;
	}

	if ( _type == ITEMGET(13,37) )
		Attribute2 = 0;

	SetOption = SetOption & 15;
		
	SOption=SetOption & 3;
	SOptionStatValue=0;

	if (SOption != 1 && SOption != 2)
	{
		SOption=0;
	}

	if ( (( (SetOption>>2) & 3) != 1) && ( ((SetOption>>2) & 3) != 2) )
	{
		SOption=0;
	}

	if ( gSetItemOption.IsSetItem(_type) != 0 && SetOption != 255 && SOption != 0)
	{
		this->m_SetOption = SetOption;

		if ( this->m_SetOption != 0 )
		{
			//this->m_NewOption = 0;
			//Attribute2 = 0;
			SOptionStatValue = (SetOption>>2)&3;
		}
	}
	else
	{
		this->m_SetOption = 0;
	}
	
	this->m_ItemOptionEx = ItemEffectEx;

	if (g_SocketOptionSystem.IsEnableSocketItem(this) == true)
	{
		if (SocketOption)
		{
			this->m_BonusSocketOption = SocketBonusOption;

			for (int i = 0; i < 5; i++)
			{
				this->m_SocketOption[i] = SocketOption[i];
			}
		}

		if (g_SocketOptionSystem.IsSocketItem(this) == true)
		{
			this->m_JewelOfHarmonyOption = 0;
			this->m_ItemOptionEx = 0;

			if (g_ConfigRead.data.common.SocketExcLimit == false)
			{
				Attribute2 = 0;
				this->m_NewOption = 0;
			}
		}
	}

	else if (g_PentagramSystem.IsPentagramItem(this) == true || g_PentagramSystem.IsPentagramJewel(this) == true)
	{
		if (SocketOption)
		{
			this->m_BonusSocketOption = SocketBonusOption;

			for (int i = 0; i < 5; i++)
			{
				this->m_SocketOption[i] = SocketOption[i];
			}
		}

		this->m_ItemOptionEx = 0;
		this->m_JewelOfHarmonyOption = 0;
		this->m_SetOption = 0;
		Attribute2 = 0;
		this->m_NewOption = 0;
	}

	else if (g_CMuunSystem.IsMuunItem(this) == true)
	{
		if (SocketOption)
		{
			this->m_BonusSocketOption = SocketBonusOption;

			for (int j = 0; j < 5; ++j)
			{
				this->m_SocketOption[j] = SocketOption[j];
			}
		}

		this->m_ItemOptionEx = 0;
		this->m_SetOption = 0;
		Attribute2 = 0;
		this->m_NewOption = 0;
		this->m_JewelOfHarmonyOption = 0;
	}

	else if (g_CMuunSystem.IsStoneofEvolution(this) == true)
	{
		if (SocketOption)
		{
			this->m_BonusSocketOption = 0;

			for (int k = 0; k < 5; ++k)
			{
				this->m_SocketOption[k] = SocketOption[k];
			}
		}

		this->m_ItemOptionEx = 0;
		this->m_SetOption = 0;
		Attribute2 = 0;
		this->m_NewOption = 0;
		this->m_JewelOfHarmonyOption = 0;
	}

	else
	{
		this->m_BonusSocketOption = 0;

		for (int i = 0; i < 5; i++)
		{
			this->m_SocketOption[i] = 0xFF;
		}
	}

	this->m_PeriodItemOption = PeriodItemOption;
	memset( this->m_SkillResistance , 0, sizeof(this->m_SkillResistance) );
	
	if ( _type == ITEMGET(0,19) || _type == ITEMGET(4,18) || _type == ITEMGET(5,10) || _type == ITEMGET(2,13))
	{
		Attribute2=0;
	}

	iChaosItem = 0;

	if ( this->m_Type == ITEMGET(2,6) )
	{
		iChaosItem = 15;
	}
	else if ( this->m_Type == ITEMGET(5,7) )
	{
		iChaosItem = 25;
	}
	else if ( this->m_Type == ITEMGET(4,6) )
	{
		iChaosItem = 30;
	}

	if ( ItemGetDurability(this->m_Type, 0, 0, 0) != 0 )
	{
		this->m_BaseDurability = (float)ItemGetDurability(this->m_Type, this->m_Level, Attribute2 & 0x7F, this->m_SetOption );
	}
	else
	{
		this->m_BaseDurability = 0;
	}

	this->m_DurabilityState[0] = this->m_BaseDurability * 0.5f;
	this->m_DurabilityState[1] = this->m_BaseDurability * 0.3f;
	this->m_DurabilityState[2] = this->m_BaseDurability * 0.2f;
	if (g_SocketOptionSystem.IsSeedSphereItem(this->m_Type))
		this->m_Durability = ((this->m_Type % 512 - 100) / 6 + 1);
	this->m_Value = p->Value;
	this->m_AttackSpeed = p->AttackSpeed;
	this->m_TwoHand = p->TwoHand;
	this->m_DamageMin = p->DamageMin;
	this->m_DamageMax = p->DamageMax;
	this->m_SuccessfulBlocking = p->SuccessfulBlocking;
	this->m_Defense = p->Defense;
	this->m_MagicDefense = p->MagicDefense;
	this->m_WalkSpeed = p->WalkSpeed;
	this->m_serial = p->Serial;
	this->m_QuestItem = (bool)p->QuestItem;
	this->m_RequireLeaderShip = 0;

	if((type >> 9) != 5 || p->ItemSlot != 1)
	{
		this->m_Magic = p->MagicPW;
		this->m_CurseSpell = 0;
	}
	else
	{
		this->m_Magic = 0;
		this->m_CurseSpell = p->MagicPW;
	}

	if ( this->m_Durability == 0 )
	{
		this->m_CurrentDurabilityState = 1;
	}
	else if ( this->m_Durability < this->m_DurabilityState[2] )
	{
		this->m_CurrentDurabilityState = (float)0.5;
	}
	else if ( this->m_Durability < this->m_DurabilityState[1] )
	{
		this->m_CurrentDurabilityState = (float)0.3;
	}
	else if ( this->m_Durability < this->m_DurabilityState[0] )
	{
		this->m_CurrentDurabilityState = (float)0.2;
	}
	else
	{
		this->m_CurrentDurabilityState = 0.0;
	}

	memcpy(this->m_RequireClass, p->RequireClass , sizeof(this->m_RequireClass));

	for ( i = 0; i<MAX_ITEM_SPECIAL_ATTRIBUTE ; i++ )
	{
		this->m_Resistance[i] = p->Resistance[i] * this->m_Level;
	}

	ItemLevel = p->Level;

	if ( ( Attribute2 & 0x3F) > 0 )
	{
		ItemLevel = p->Level + 25;
	}
	else
	{
		if (this->m_SetOption != 0 )
		{
			ItemLevel = p->Level + 25;
		}
	}

	if ( p->RequireStrength != 0 )
	{
		this->m_RequireStrength = ((p->RequireStrength * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
	}
	else
	{
		this->m_RequireStrength = 0;
	}

	if ( p->RequireDexterity != 0 )
	{
		this->m_RequireDexterity = ((p->RequireDexterity * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
	}
	else
	{
		this->m_RequireDexterity  = 0;
	}

	if ( p->RequireEnergy != 0 )
	{
		this->m_RequireEnergy = ((p->RequireEnergy * (ItemLevel + this->m_Level * 3 ) ) * 4) / 100 + 20;
	}
	else
	{
		this->m_RequireEnergy = 0;
	}

	if( p->RequireEnergy != 0 )
	{
		if((_type >> 9) == 5 && p->ItemSlot == 1) //Season3 add-on
		{
			this->m_RequireEnergy = ((p->RequireEnergy * (ItemLevel + this->m_Level) ) * 3) / 100 + 20;
		}
	}
	else
	{
		this->m_RequireEnergy = 0;
	}

	if ( _type >= ITEMGET(0,0) && _type < ITEMGET(12,0) )
	{
		if ( p->RequireVitality != 0 )
		{
			this->m_RequireVitality = ((p->RequireVitality * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
		}
		else
		{
			this->m_RequireVitality = 0;
		}

		if ( p->RequireLeadership != 0 )
		{
			this->m_RequireLeaderShip = ((p->RequireVitality * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
		}
		else
		{
			this->m_RequireLeaderShip = 0;
		}
	}

	if( _type == ITEMGET(13,4) || _type == ITEMGET(13,5) )
	{
		if(this->m_PetItem_Level > MAX_PET_LEVEL)
		{
			this->m_PetItem_Level = MAX_PET_LEVEL;
		}
	}

	if ( _type == ITEMGET(13,5) )
		this->m_RequireLeaderShip = this->m_PetItem_Level * 10 + 185;

	if ( g_kJewelOfHarmonySystem.IsActive(this) == TRUE )
	{
		BYTE btOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(this);

		if ( btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR )
			this->m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR);
		else if ( btOption == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE )
			this->m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE);
		else if ( btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX )
			this->m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX);
		else if ( btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR )
			this->m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR);
	}

	else //WZ Fix for Season3.0
	{
		this->m_HJOpStrength = 0;
		this->m_HJOpDexterity = 0;
	}

	g_SocketOptionSystem.SetRequireStatOption(this);

	if ( p->RequireLevel != 0 )
	{
		if ( _type == ITEMGET(13,4) )	// DarkHorse
		{
			this->m_RequireLevel = this->m_PetItem_Level * 2 + 218;
		}

		else if ( (_type >= ITEMGET(12,3) && _type <= ITEMGET(12,6)) || _type == ITEMGET(12,42) || _type == ITEMGET(12,49))
		{
			this->m_RequireLevel = p->RequireLevel + (this->m_Level * 5);
		}

		else if ( (_type >= ITEMGET(12,36) && _type <= ITEMGET(12,40)) || _type == ITEMGET(12,43) || _type == ITEMGET(12,50) || _type == ITEMGET(12,270)) //Third Wings Fix Required Level
		{
			this->m_RequireLevel = p->RequireLevel;
		}

		else if ( _type >= ITEMGET(12,7) && _type <= ITEMGET(12,24) )
		{
			this->m_RequireLevel = p->RequireLevel;
		}

		else if (_type >= ITEMGET(12, 262) && _type <= ITEMGET(12, 265))
		{
			this->m_RequireLevel = p->RequireLevel + this->m_Level * 4;
		}

		else if (_type == ITEMGET(12, 266) || _type == ITEMGET(12, 267) || _type == ITEMGET(12,268))
		{
			this->m_RequireLevel = p->RequireLevel;
		}

		else if (_type == ITEMGET(12, 269))
		{
			this->m_RequireLevel = p->RequireLevel * this->m_Level * 4;
		}

		else if ( _type >= ITEMGET(12,51) && _type <= ITEMGET(12,52) )
		{
			this->m_RequireLevel = p->RequireLevel + this->m_Level * 4;
			if(this->m_RequireLevel > 400)
			{
				this->m_RequireLevel= 400;
			}
			
		}

		else if ( _type >= ITEMGET(0,0) && _type < ITEMGET(12,0) )
		{
			if ( p->RequireLevel )
				this->m_RequireLevel = p->RequireLevel;
		}

		else
		{
			this->m_RequireLevel = p->RequireLevel + this->m_Level * 4;
		}
	}
	else
	{
		this->m_RequireLevel = 0;
	}

	if ( this->m_Type == ITEMGET(13,10) )
	{
		if ( ItemLevel <= 2 )
		{
			this->m_RequireLevel = 20;
		}
		else
		{
			this->m_RequireLevel = 50;
		}
	}

	if ( (Attribute2 & 0x3F) > 0 ) // TODO (WING) 2
	{
		if ( this->m_RequireLevel > 0 )
		{
			if ( (_type >= ITEMGET(12,36) && _type <= ITEMGET(12,40)) || this->m_Type == ITEMGET(12,43) || this->m_Type == ITEMGET(12,50) || this->m_Type == ITEMGET(12,270) ) //Third Wings Fix Required Level
			{
				this->m_RequireLevel;
			}
			else if ( _type < ITEMGET(0,0) || _type >= ITEMGET(12,0) && !((_type >= ITEMGET(12,36) && _type <= ITEMGET(12,40)) || this->m_Type == ITEMGET(12,43) || this->m_Type == ITEMGET(12,50) || this->m_Type == ITEMGET(12,266) || this->m_Type == ITEMGET(12,267) || this->m_Type == ITEMGET(12,268) || this->m_Type == ITEMGET(12,270)) ) //Third Wings Fix Anti-Hack
			{
				this->m_RequireLevel += 20;
			}
		}
	}

	if (this->m_RequireLevel > 400)
	{
		this->m_RequireLevel = 400;
	}

	if ( this->m_SetOption != 0 )
	{
		ItemLevel = p->Level + 30;
	}

	this->m_Leadership = 0;

	if ( this->m_DamageMax > 0 )
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_DamageMax += (this->m_DamageMin * 25) / p->Level + 5;
			this->m_DamageMax += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					this->m_DamageMax += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					this->m_DamageMax += (this->m_DamageMin * 25) / p->Level + 5;
				}
			}
		}

		if (g_PentagramSystem.IsPentagramItem(this) == true)
		{
			this->m_DamageMax += this->m_Level * 3;
		}

		else
		{
			this->m_DamageMax += this->m_Level * 3;
		}

		if ( this->m_Level >= 10 )
		{
			this->m_DamageMax += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( this->m_DamageMin > 0 )	
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_DamageMin += (this->m_DamageMin * 25) / p->Level + 5;
			this->m_DamageMin += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					this->m_DamageMin += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					this->m_DamageMin += (this->m_DamageMin * 25) / p->Level + 5;
				}
			}
		}

		if (g_PentagramSystem.IsPentagramItem(this) == true)
		{
			this->m_DamageMin += this->m_Level * 3;
		}

		else
		{
			this->m_DamageMin += this->m_Level * 3;
		}

		if ( this->m_Level >= 10 )
		{
			this->m_DamageMin += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( this->m_Magic > 0 )	
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_Magic += (this->m_Magic * 25) / p->Level + 5;
			this->m_Magic += ItemLevel / 60 + 2;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					this->m_Magic += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					this->m_Magic += (this->m_Magic * 25) / p->Level + 5;
				}
			}
		}

		this->m_Magic += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_Magic += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if( this->m_CurseSpell > 0) //Season3 Summoner
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_CurseSpell += (this->m_CurseSpell * 25) / p->Level + 5;
			this->m_CurseSpell += ItemLevel / 60 + 2;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 ) //season4 changed
				{
					this->m_CurseSpell += iChaosItem; //season4 changed
				}
				else if ( p->Level != 0 )
				{
					this->m_CurseSpell += (this->m_CurseSpell * 25) / p->Level + 5;
				}
			}
		}

		this->m_CurseSpell += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_CurseSpell += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( p->SuccessfulBlocking > 0 )	
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_SuccessfulBlocking += (this->m_SuccessfulBlocking * 25) / p->Level + 5;
			this->m_SuccessfulBlocking += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( p->Level != 0 )
				{
					this->m_SuccessfulBlocking += (this->m_SuccessfulBlocking * 25) / p->Level + 5;
				}
			}
		}

		this->m_SuccessfulBlocking += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_SuccessfulBlocking += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( p->Defense > 0 )	
	{
		if ( this->m_Type >= ITEMGET(6,0) && this->m_Type < ITEMGET(7,0) )
		{
			this->m_Defense += this->m_Level;

			if ( this->m_SetOption != 0 && ItemLevel != 0 )
			{
				this->m_Defense += (this->m_Defense * 20) / ItemLevel + 2;
			}
		}
		else 
		{
			if ( this->m_SetOption != 0 && ItemLevel != 0 )
			{
				this->m_Defense += (this->m_Defense * 12) / p->Level + (p->Level / 5 ) + 4;
				this->m_Defense += (this->m_Defense * 3) / ItemLevel  + ( ItemLevel / 30 ) + 2;
			}
			else if ( (Attribute2 & 0x3F) > 0 )
			{
				if ( p->Level != 0 )
				{
					this->m_Defense += (this->m_Defense * 12) / p->Level + (p->Level / 5 ) + 4;
				}
			}

			if ( ( _type >= ITEMGET(12,36) && _type <= ITEMGET(12,40)) || _type == ITEMGET(12,43) || _type == ITEMGET(12,50) || _type == ITEMGET(12,268) )
			{
				this->m_Defense += this->m_Level * 4;

				if ( this->m_Level >= 10 )
				{
					this->m_Defense += (this->m_Level - 9);
				}
			}

			else if ( ( _type >= ITEMGET(12,3) && _type <= ITEMGET(12,6)) || _type == ITEMGET(13,30) || _type == ITEMGET(13,4) )
			{
				this->m_Defense += this->m_Level * 2;
			}

			else
			{
				this->m_Defense += this->m_Level * 3;
			}

			if ( this->m_Level >= 10 )
			{
				this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
			}
		}
	}

	switch ( _type ) // Wings
	{
		case ITEMGET(12,0):
		case ITEMGET(12,1):
		case ITEMGET(12,2):
		case ITEMGET(12,41):
			this->m_Defense += this->m_Level*3;
			if(this->m_Level >= 10)
				this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
			break;
		case ITEMGET(12,3):
		case ITEMGET(12,4):
		case ITEMGET(12,5):
		case ITEMGET(12,6):
		case ITEMGET(12,42):
		case ITEMGET(12,49):
		case ITEMGET(12,269):
			this->m_Defense += this->m_Level*2;
			if(this->m_Level >= 10)
				this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2 + (this->m_Level - 9);
			break;
		case ITEMGET(13,30):
			this->m_Defense += this->m_Level*2 + 15;
			if(this->m_Level >= 10)
				this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2 + (this->m_Level - 9);
			break;

		case ITEMGET(12,36):
		case ITEMGET(12,37):
		case ITEMGET(12,38):
		case ITEMGET(12,39):
		case ITEMGET(12,40):
		case ITEMGET(12,43):
		case ITEMGET(12,50):
		case ITEMGET(12,270):
			this->m_Defense += this->m_Level*4;
			if ( this->m_Level >= 10 )
			{
				this->m_Defense += (this->m_Level - 8) * (this->m_Level - 9) / 2;
			}
			break;
		case ITEMGET(12,262):
		case ITEMGET(12,263):
		case ITEMGET(12,264):
		case ITEMGET(12,265):
			this->m_Defense += this->m_Level*3;
			if ( this->m_Level >= 10 )
			{
				this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
			}
			break;

	}

	if ( p->MagicDefense > 0 )
	{
		this->m_MagicDefense += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_MagicDefense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	this->m_Level &= 15;

	memset(this->m_Special, 0, sizeof(this->m_Special));
	memset(this->m_SpecialValue, 0, sizeof(this->m_SpecialValue));
	this->m_SpecialNum = 0;
	this->m_Option1 = 0;
	this->m_Option2 = 0;
	this->m_Option3 = 0;
	this->m_SkillChange = FALSE;

	if ( Option1	!= 0 )
	{
		if ( p->SkillType != 0 )
		{
			if ( p->SkillType == 66 )
			{
				this->m_SkillChange = TRUE;
				this->m_Special[this->m_SpecialNum] = 0;
				this->m_Option1 = 1;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = p->SkillType;
				this->m_Option1 = 1;
			}
		}
	}

	if ( this->m_Type == ITEMGET(13,3) )	// Dinorant
	{
		this->m_Option1 = TRUE;
		this->m_Special[this->m_SpecialNum] = 49;
	}

	if ( this->m_Type == ITEMGET(13,4) )	//Dark Horse
	{
		this->m_Option1 = TRUE;
		this->m_Special[this->m_SpecialNum] = 62;
	}

	if ( this->m_Type == ITEMGET(13,37) )
	{
		this->m_Option1 = TRUE;
		this->m_Special[this->m_SpecialNum] = 76;
	}

	if (this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,220))
	{
		this->m_Special[this->m_SpecialNum] = 106;
	}

	this->m_SpecialNum++;

	if ( Option2 != 0 )
	{
		if ( _type >= ITEMGET(0,0) && _type < ITEMGET(12,0) )
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if ( (_type >= ITEMGET(12,0) && _type <= ITEMGET(12,6)) || (_type >= ITEMGET(12,41) && _type <= ITEMGET(12,42)) || _type == ITEMGET(12,49) || _type == ITEMGET(12,269) )	// Wings
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if ( _type == ITEMGET(13,30) )	// Cape of Lord
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if ( (_type >= ITEMGET(12,36) && _type <= ITEMGET(12,40)) || _type == ITEMGET(12,43) || _type == ITEMGET(12,50) || _type == ITEMGET(12,270) )	// Third Wings
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if ( (_type >= ITEMGET(12,41) && _type <= ITEMGET(12,43)) )
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if ( (_type >= ITEMGET(12,262) && _type <= ITEMGET(12,265)) )
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if (_type == ITEMGET(12,266) || _type == ITEMGET(12,267) || _type == ITEMGET(12,268))
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

	}

	if (this->m_Type >= ITEMGET(12, 200) && this->m_Type <= ITEMGET(12, 220))
	{
		this->m_Special[this->m_SpecialNum] = 107;
	}

	this->m_SpecialNum++;

	if ( Option3 != 0 )
	{
		if ( _type >= ITEMGET(0,0) && _type < ITEMGET(5,0) )
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( _type >= ITEMGET(5,0) && _type < ITEMGET(6,0) )
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		
		if ( _type >= ITEMGET(6,0) && _type < ITEMGET(7,0) )
		{
			this->m_Special[this->m_SpecialNum] = 82;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( _type >= ITEMGET(7,0) && _type < ITEMGET(12,0) )
		{
			this->m_Special[this->m_SpecialNum] = 83;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( (_type >= ITEMGET(13,8) && _type < ITEMGET(13,14)) || (_type >= ITEMGET(13,20) && _type <= ITEMGET(13,28)) || (_type >= ITEMGET(13,109) && _type <= ITEMGET(13,115)) || (_type >= ITEMGET(13,171) && _type <= ITEMGET(13,176)) )
		{
			if ( _type == ITEMGET(13,24) ) // Ring of MAgic
			{
				this->m_Special[this->m_SpecialNum] = 172;
				this->m_Option3 = Option3;
			}
			else if ( _type == ITEMGET(13,28) ) // Pendant of abilit
			{
				this->m_Special[this->m_SpecialNum] = 173;
				this->m_Option3 = Option3;
			}
			else  
			{
				this->m_Special[this->m_SpecialNum] = 85;
				this->m_Option3 = Option3;
			}
		}
		
		if ( _type == ITEMGET(13,30) || _type == ITEMGET(12,130) ) // Cape of lord 
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( _type == ITEMGET(12,0) || _type == ITEMGET(12,132) )	// Wing elf
		{
			this->m_Special[this->m_SpecialNum] = 85;
			this->m_Option3 = Option3;
		}
		else if ( _type == ITEMGET(12,1) || _type == ITEMGET(12,133) ) // wing Heaven
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,2) || _type == ITEMGET(12,134) ) // wing devil
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,49) )
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			//WTF?? Inverted Values??
			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
		}

		else if ( _type == ITEMGET(12,3) ) // wing spitits
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			//WTF?? Inverted Values??
			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}

		}
		else if ( _type == ITEMGET(12,38) ) // Third Wings "Wing of Illusion"
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}

		}
		else if ( _type == ITEMGET(12,4) ) // Wings of Soul
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 81; // Additional Magic
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

		}
		else if ( _type == ITEMGET(12,37) ) // Third Wings "Wing of Vortex"
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) // JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Magic enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 81; // Additional Magic
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) // Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) // JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; // Additional Defense
			}

		}
		else if ( _type == ITEMGET(12,5) ) // Wings of Dragon
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Damage "Take no Effect"
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

		}
		else if ( _type == ITEMGET(12,36) ) // Third Wings "Wing of Storm"
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) // JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; // Additional Defense
			}

		}
		else if ( _type == ITEMGET(12,6) ) // Wing of Darkness
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 81; // Additional Magic
			}

		}
		else if ( _type == ITEMGET(12,49) ) // Warrior Cloak
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}

		}

		else if ( _type == ITEMGET(12,39) ) // Third Wings "Wing of Downcast"
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 81; //Additional Magic
			}

		}
		else if ( _type == ITEMGET(13,3) ) // dinorant
		{
			this->m_Option3 = Option3;
	
			if ( (this->m_Option3&0x02) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x67;
				this->m_SpecialNum++;
			}
		
			if ( (this->m_Option3&0x04) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x61;
				this->m_SpecialNum++;
				this->m_AttackSpeed+=5;
			}
		
			if ( (this->m_Option3&0x01) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x68;
			}
		}

		if ( _type == ITEMGET(13,30) ) // Cape ofLord
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;
			this->m_Special[this->m_SpecialNum] = 0x50;
		}

		if ( _type == ITEMGET(12,40) ) // Third Wings "Mantle of Monarch"
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) // JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; // Additional Defense
			}

		}
		if ( _type == ITEMGET(12,50) ) // Third Wings "Reigning Cloak"
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) // JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; // Additional Defense
			}

		}

		else if ( _type == ITEMGET(12,262) ) // Cape of Death 2.5
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x10) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

		}

		else if ( _type == ITEMGET(12,263) ) // Wing of Chaos 2.5
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x10) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

		}

		else if ( _type == ITEMGET(12,264) ) // Wing Of Magic 2.5
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x10) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 81; // Additional Wizardry Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

		}

		else if ( _type == ITEMGET(12,265) ) // Wing Of Life 2.5
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x10) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

		}

		else if (_type == ITEMGET(12, 269)) // Cloak Of Limit
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength += Option3 * 4;

			if ((this->m_NewOption & 0x20) != 0)
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Damage
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}
		}

		else if (_type == ITEMGET(12, 270)) // Cloak of Transcendence
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength += Option3 * 4;

			if ((this->m_NewOption & 0x20) >= 0 && (this->m_NewOption & 0x10) != 0) 
			{
				this->m_Special[this->m_SpecialNum] = 80; // Additional Magic
			}

			else if ((this->m_NewOption & 0x20) == 0 && (this->m_NewOption & 0x10) == 0)
			{
				this->m_Special[this->m_SpecialNum] = 85; // Recover Life
			}

			else if ((this->m_NewOption & 0x20) != 0 && (this->m_NewOption & 0x10) == 0) 
			{
				this->m_Special[this->m_SpecialNum] = 83; // Additional Defense
			}
		}

		else if (_type == ITEMGET(12,266) || _type == ITEMGET(12,267) || _type == ITEMGET(12,268))
		{
			this->m_Special[this->m_SpecialNum] = 85;
			this->m_Option3 = Option3;
		}

		if ( _type == ITEMGET(12,41) || _type == ITEMGET(12,131) )
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength += Option3 * 4;
			this->m_Special[this->m_SpecialNum] = 81;
		}

		if ( _type == ITEMGET(12,42) )
		{
			this->m_Option3 = Option3;
			this->m_RequireStrength += Option3 * 4;
			if(this->m_NewOption&0x20)
			{
				this->m_Special[this->m_SpecialNum] = 81;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 113;
			}
		}

		if ( _type == ITEMGET(12,43) )
		{
			this->m_Option3 = Option3;
			this->m_Special[this->m_SpecialNum] = 85;
			if(this->m_NewOption&0x10)
			{
				this->m_Special[this->m_SpecialNum] = 81;
			}
			else if(this->m_NewOption&0x20)
			{
				this->m_Special[this->m_SpecialNum] = 113;
			}
		}
	}

	if (this->m_Type >= ITEMGET(12, 200) && this->m_Type <= ITEMGET(12, 220))
	{
		this->m_Special[this->m_SpecialNum] = 108;
	}

	this->m_SpecialNum++;

	if ( (_type >=ITEMGET(6,0) && _type <ITEMGET(12,0) ) || ( _type >=ITEMGET(13,8) && _type <= ITEMGET(13,9)) || (_type>=ITEMGET(13,21) && _type <= ITEMGET(13,24)) || 
		(_type == ITEMGET(13,110)) || (_type == ITEMGET(13,111)) || (_type == ITEMGET(13,112)) || (_type >= ITEMGET(13,173) && _type <= ITEMGET(13,176)) )
	{
		if ( ((this->m_NewOption>>5)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x56;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>4)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x57;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>3)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x58;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>2)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x59;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>1)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x5A;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x5B;
			this->m_SpecialNum++;
		}
	}

	if ( (_type >=ITEMGET(0,0) && _type <ITEMGET(6,0) ) || ( _type >=ITEMGET(13,12) && _type <= ITEMGET(13,13)) || (_type>=ITEMGET(13,25) && _type <= ITEMGET(13,28)) ||
		_type == ITEMGET(13,109) || _type == ITEMGET(13,113) || _type == ITEMGET(13,114) || _type == ITEMGET(13,115) || (_type >= ITEMGET(13,171) && _type <= ITEMGET(13,172)) )
	{
		if ( ((this->m_NewOption>>5)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x5C;
			this->m_SpecialNum++;
		}

		if ( (_type >=ITEMGET(5,0) && _type <ITEMGET(6,0) ) || (_type ==ITEMGET(13,12) ) || (_type == ITEMGET(13,25)) || (_type == ITEMGET(13,27)) || (_type == ITEMGET(13,109)) || (_type == ITEMGET(13,115)) )
		{
			if ( ((this->m_NewOption>>4)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x5F;
				this->m_SpecialNum++;
			}
			if ( ((this->m_NewOption>>3)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x60;
				this->m_SpecialNum++;
			}
		}
		else
		{
			if ( ((this->m_NewOption>>4)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x5D;
				this->m_SpecialNum++;
			}
			if ( ((this->m_NewOption>>3)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x5E;
				this->m_SpecialNum++;
			}
		}

		if ( ((this->m_NewOption>>2)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x61;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>1)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x62;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x63;
			this->m_SpecialNum++;
		}
	}

	if ( ((_type >= ITEMGET(12,3)) && (_type <= ITEMGET(12,6))) || (_type ==ITEMGET(13,30)) || (_type == ITEMGET(12,42) || _type == ITEMGET(12,269)) )
	{
		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x64;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&2) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x65;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&4) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x66;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&8) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x69;
			this->m_SpecialNum++;
		}
	}

	if ( _type == ITEMGET(12,49) )
	{
		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x64;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&2) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x65;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&4) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x66;
			this->m_SpecialNum++;
		}
	}

	if ( ((_type >= ITEMGET(12,36)) && (_type <= ITEMGET(12,40))) || (_type ==ITEMGET(12,43)) || (_type == ITEMGET(12,50) || _type == ITEMGET(12,270)))
	{
		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6C;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&2) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6D;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&4) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6E;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&8) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6F;
			this->m_SpecialNum++;
		}
	}

	if ( (_type >= ITEMGET(12,262)) && (_type <= ITEMGET(12,265)) )
	{
		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6C;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&4) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6e;
			this->m_SpecialNum++;
		}
	}

	if ( _type == ITEMGET(12,268) )
	{
		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6C;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&2) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6D;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&4) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x6E;
			this->m_SpecialNum++;
		}
	}

	if ( _type == ITEMGET(13,37) )
	{
		if ( (this->m_NewOption &1) )
		{
			this->m_Special[this->m_SpecialNum] = 0x6A;
			this->m_SpecialNum++;
		}
		else if ( (this->m_NewOption &2) )
		{
			this->m_Special[this->m_SpecialNum] = 0x6B;
			this->m_SpecialNum++;
		}
	}

	if ( SOptionStatValue != 0 )
	{
		if (p->SetAttr != 0 )
		{
			this->m_Special[this->m_SpecialNum] = p->SetAttr + 0xC3;
			this->m_SetAddStat = SOptionStatValue * 5;
			this->m_SpecialNum++;
		}
	}

	this->m_Part = p->ItemSlot;
	this->Value();

	if ( g_PentagramSystem.IsPentagramItem(this) == TRUE )
	{
		this->m_CurrentDurabilityState = 0.0;
	}

	if ( this->m_Type != ITEMGET(4,7) && this->m_Type != ITEMGET(4,15) ) // Arrows and Bolts
	{
		this->m_DamageMinOrigin = this->m_DamageMin;
		this->m_DefenseOrigin = this->m_Defense;
		this->m_DamageMin -= (int)(this->m_DamageMin * this->m_CurrentDurabilityState);
		this->m_DamageMax -= (int)(this->m_DamageMax * this->m_CurrentDurabilityState);
		this->m_Defense -= (int)(this->m_Defense * this->m_CurrentDurabilityState);
		this->m_SuccessfulBlocking -= (int)(this->m_SuccessfulBlocking * this->m_CurrentDurabilityState);

		if ( this->m_Durability < 1.0f )
		{
			this->m_AttackSpeed = 0;
		}
	}
	
	if ( this->m_Durability == 0.0f )
	{
		memset(this->m_Resistance , 0, sizeof(this->m_Resistance));
	}
}

 
int CItem::GetLevel() 
{
	return ItemAttribute[this->m_Type].Level;
}

UINT64 CItem::GetNumber()
{
	return this->m_Number;
}


void CItem::Value()
{
	if (this->m_Type == -1)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		this->m_SellMoney = 1;
		this->m_BuyMoney = 1;
		return;
	}

	LPITEM_ATTRIBUTE p = &ItemAttribute[this->m_Type];

	if (p->BuyMoney != 0)
	{
		this->m_SellMoney = p->BuyMoney;
		this->m_BuyMoney = p->BuyMoney;
		this->m_SellMoney /= g_ConfigRead.itemPrices.btItemSellPriceDivisor;

		if (this->m_SellMoney < 1000)
		{
			if (this->m_SellMoney >= 100)
			{
				this->m_SellMoney = 10 * (this->m_SellMoney / 10);
			}
		}

		else
		{
			this->m_SellMoney = 100 * (this->m_SellMoney / 100);
		}

		if (this->m_BuyMoney < 1000)
		{
			if (this->m_BuyMoney >= 100)
			{
				this->m_BuyMoney = 10 * (this->m_BuyMoney / 10);
			}
		}

		else
		{
			this->m_BuyMoney = 100 * (this->m_BuyMoney / 100);
		}

		return;
	}

	if (IsJumpingEventItem(this->m_Type) == TRUE)
	{
		this->m_SellMoney = 0;
		this->m_BuyMoney = 0;
		return;
	}

	int Level = sqrt((float)this->m_Level);
	UINT64 Gold = 0;
	int Type = (this->m_Type)/MAX_SUBTYPE_ITEMS;
	int Level2 = p->Level + this->m_Level*3;
	int excellent = 0;

	for (int i=0;i<this->m_SpecialNum;i++)
	{
		switch ( this->m_Special[i] )
		{
			case 0x56:	// 86.
			case 0x57:
			case 0x58:
			case 0x59:
			case 0x5A:
			case 0x5B:
			case 0x5C:
			case 0x5D:
			case 0x5E:
			case 0x5F:
			case 0x60:
			case 0x61:
			case 0x62:
			case 0x63:	// 99.
			case 0x67:
			case 0x69:
			case 0x72:
				excellent = 1;
				break;
			default:
				continue;
		}
	}

	if ( excellent != 0 )
	{
		Level2 += 25;
	}

	if ( this->m_Type == ITEMGET(4,15) ) // Arrow
	{
		int levelgold = 70;

		switch ( this->m_Level )
		{
			case 1:
				levelgold = 1200;
				break;
			case 2:
				levelgold = 2000;
				break;
			case 3:
				levelgold = 2800;
				break;
		}

		if ( this->m_Durability > 0.0f )
		{
			Gold = levelgold * this->m_Durability/p->Durability;
		}
	}

	else if ( this->m_Type == ITEMGET(4,7) ) // Bolts
	{
		int levelgold;

		if ( this->m_Durability > 0.0f )
		{
			levelgold = 100;

			switch ( this->m_Level )
			{
				case 1:
					levelgold = 1400;
					break;
				case 2:
					levelgold = 2200;
					break;
				case 3:
					levelgold = 3000;
					break;
			}
		
			Gold = levelgold * this->m_Durability/p->Durability;
		}
	}

	if (this->m_Type == ITEMGET(14,13))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfBlessPrice;
	}

	else if (this->m_Type == ITEMGET(14,14))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfSoulPrice;
	}

	else if (this->m_Type == ITEMGET(12,15))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfChaosPrice;
	}

	else if (this->m_Type == ITEMGET(14,16))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfLifePrice;
	}

	else if (this->m_Type == ITEMGET(14,22))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfCreationPrice;
	}

	else if (this->m_Type == ITEMGET(12,136))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfLifePrice * 10 * (this->m_Level + 1);
	}

	else if (this->m_Type == ITEMGET(12,137))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfCreationPrice * 10 * (this->m_Level + 1);
	}

	else if (this->m_Type == ITEMGET(12,138))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfGuardianPrice * 10 * (this->m_Level + 1);
	}

	else if (this->m_Type == ITEMGET(12,139) || this->m_Type == ITEMGET(12,140) || this->m_Type == ITEMGET(12,142) || this->m_Type == ITEMGET(12,143))
	{
		Gold = 186000 * (this->m_Level + 1);
	}

	else if (this->m_Type == ITEMGET(12,141))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfChaosPrice * 10 * (this->m_Level + 1);
	}

	else if (this->m_Type == ITEMGET(12,30))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfBlessPrice * 10 * (this->m_Level + 1);
	}
	
	else if (this->m_Type == ITEMGET(12,31))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfSoulPrice * 10 * (this->m_Level + 1);
	}

	else if (this->m_Type == ITEMGET(13,15))
	{
		Gold = 33000000;
	}

	else if (this->m_Type == ITEMGET(13,14))
	{
		if ( this->m_Level == 1 )
		{
			Gold = g_ConfigRead.itemPrices.iCrestOfMonarchPrice;
		}
		else
		{
			Gold = g_ConfigRead.itemPrices.iLochFeatherPrice;
		}
	}

	else if (this->m_Type == ITEMGET(14,31))
	{
		Gold = g_ConfigRead.itemPrices.iJewelOfGuardianPrice;
	}

	else if (this->m_Type == ITEMGET(14,7))
	{
		Gold = (int)(this->m_Durability) * 900000;
	}

	else if (this->m_Type == ITEMGET(13,11))
	{
		if (this->m_Level == 1)
		{
			Gold = 2400000;
		}
	}
	else if (this->m_Type == ITEMGET(13,7))
	{
		if (this->m_Level == 0)
		{
			Gold = 1500000;
		}
		else if (this->m_Level == 1)
		{
			Gold = 1200000;
		}
	}

	else if (this->m_Type == ITEMGET(13,32)) 
	{
		Gold = (int)this->m_Durability * 150;
	}

	else if (this->m_Type == ITEMGET(13,33)) 
	{
		Gold = (int)this->m_Durability * 300;
	}

	else if (this->m_Type == ITEMGET(13,34)) 
	{
		Gold = (int)this->m_Durability * 3000;
	}

	else if (this->m_Type == ITEMGET(13,35)) 
	{
		Gold = 30000;
	}

	else if (this->m_Type == ITEMGET(13,36)) 
	{
		Gold = 90000;
	}

	else if (this->m_Type == ITEMGET(13,37)) 
	{
		Gold = 150000;
	}

	else if (this->m_Type == ITEMGET(13,239))
	{
		Gold = 900000;
	}

	else if (this->m_Type == ITEMGET(14,101))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,102))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,103))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,104))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,105))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,106))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,107))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,108))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,109))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(12,144) || this->m_Type == ITEMGET(12,146))
	{
		this->m_SellMoney = (int)this->m_Durability * 100000;
		this->m_BuyMoney = (int)this->m_Durability * 100000 * 3;
		return;
	}

	else if (this->m_Type == ITEMGET(13,210))
	{
		this->m_SellMoney = g_ConfigRead.itemPrices.iWereRabbitEggPrice;
		return;
	}

	else if (this->m_Type == ITEMGET(14,35)) 
	{
		Gold = (int)this->m_Durability * 1800;
	}

	else if (this->m_Type == ITEMGET(14,36)) 
	{
		Gold = (int)this->m_Durability * 3900;
	}

	else if (this->m_Type == ITEMGET(14,37)) 
	{
		Gold = (int)this->m_Durability * 6000;
	}

	else if (this->m_Type == ITEMGET(14,38)) 
	{
		Gold = (int)this->m_Durability * 2500;
	}

	else if (this->m_Type == ITEMGET(14,39)) 
	{
		Gold = (int)this->m_Durability * 5000;
	}

	else if (this->m_Type == ITEMGET(14,40)) 
	{
		Gold = (int)this->m_Durability * 7500;
	}

	else if (this->m_Type == ITEMGET(14,110))
	{
		Gold = (int)this->m_Durability * 30000;
	}

	else if (this->m_Type == ITEMGET(14,111))
	{
		Gold = (int)this->m_Durability * 600000;
	}

	else if (this->m_Type >= ITEMGET(12,130) && this->m_Type <= ITEMGET(12,135))
	{
		Gold = 240;
	}

	else if (this->m_Type == ITEMGET(13,76))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(13,77))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(13,78))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(13,163))
	{
		Gold = 6000;
	}

	else if (this->m_Type == ITEMGET(13,164))
	{
		Gold = 6000;
	}

	else if (this->m_Type == ITEMGET(13,165))
	{
		Gold = 6000;
	}

	else if (this->m_Type == ITEMGET(13,166))
	{
		Gold = 6000;
	}

	else if (this->m_Type == ITEMGET(13,169))
	{
		Gold = 90000;
	}

	else if (this->m_Type == ITEMGET(13,170))
	{
		Gold = 90000;
	}

	else if (this->m_Type == ITEMGET(13,20))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(13,80))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(13,64))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(13,65))
	{
		Gold = 3000;
	}

	else if (this->m_Type >= ITEMGET(13,109) && this->m_Type <= ITEMGET(13,115))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(14,112))
	{
		Gold = 3000;
	}

	else if (this->m_Type == ITEMGET(14,113))
	{
		Gold = 3000;
	}

	else if (this->m_Type >= ITEMGET(14,121) && this->m_Type <= ITEMGET(14,124))
	{
		Gold = 3000;
	}

	else if (this->m_Type >= ITEMGET(13,122) && this->m_Type <= ITEMGET(13,123))
	{
		Gold = 6000;
	}

	else if (this->m_Type == ITEMGET(13,3))
	{
		Gold = 960000;

		for ( int k=0;k<3;k++)
		{
			if ( (this->m_Option3 & (1<<k)) != 0 )
			{
				Gold += 300000;
			}
		}
	}

	else if (this->m_Type == ITEMGET(14,17))
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 10000;
				break;
			case 2:
				Gold = 50000;
				break;
			case 3:
				Gold = 100000;
				break;
			case 4:
				Gold = 300000;
				break;
			case 5:
				Gold = 500000;
				break;
			case 6:
				Gold = 800000;
				break;
			case 7:
				Gold = 1000000;
				break;
			default:
				Gold = 30000;
				break;
		}
	}
	
	else if (this->m_Type == ITEMGET(14,18))
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 75000;
				break;
			case 3:
				Gold = 150000;
				break;
			case 4:
				Gold = 450000;
				break;
			case 5:
				Gold = 750000;
				break;
			case 6:
				Gold = 1200000;
				break;
			case 7:
				Gold = 1500000;
				break;
			default:
				Gold = 30000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(14,19))
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 60000;
				break;
			case 2:
				Gold = 84000;
				break;
			case 3:
				Gold = 120000;
				break;
			case 4:
				Gold = 180000;
				break;
			case 5:
				Gold = 240000;
				break;
			case 6:
				Gold = 300000;
				break;
			case 7:
				Gold = 180000;
				break;
			default:
				Gold = 120000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(14,20))
	{
		Gold = 900;
	}

	else if (this->m_Type == ITEMGET(14,21))
	{
		switch ( this->m_Level )
		{
			case 0:	
				Gold = 9000;
				this->m_Durability = this->m_BaseDurability;
			case 1:
				Gold = 9000;
				this->m_Durability = this->m_BaseDurability;
				break;
			case 3:
				Gold = (int)(this->m_Durability) * 3900;
				break;
			default:
				Gold = 9000;
				this->m_Durability = this->m_BaseDurability;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(14,100))
	{
		Gold = (int)this->m_Durability * 300;
	}

	else if (this->m_Type == ITEMGET(14,9) && this->m_Level == 1)
	{
		Gold = 1000;
	}

	else if (this->m_Type == ITEMGET(13,18))
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 150000;
				break;
			case 2:
				Gold = 660000;
				break;
			case 3:
				Gold = 720000;
				break;
			case 4:
				Gold = 780000;
				break;
			case 5:
				Gold = 840000;
				break;
			case 6:
				Gold = 900000;
				break;
			case 7:
				Gold = 960000;
				break;
			case 8:
				Gold = 1020000;
				break;
			default:
				Gold = 600000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(13,16) || this->m_Type == ITEMGET(13,17)) 
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 10000;
				break;
			case 2:
				Gold = 50000;
				break;
			case 3:
				Gold = 100000;
				break;
			case 4:
				Gold = 300000;
				break;
			case 5:
				Gold = 500000;
				break;
			case 6:
				Gold = 800000;
				break;
			case 7:
				Gold = 1000000;
				break;
			case 8:
				Gold = 1200000;
				break;
			default:
				Gold = 15000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(13,29))
	{
		Gold = 5000;
	}

	else if (this->m_Type == ITEMGET(13,20))
	{
		switch ( this->m_Level )
		{
			case 0:
				Gold = 30000;
				break;
			default:
				Gold = 0;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(13,31))
	{
		switch ( this->m_Level )
		{	
			case 0:
				Gold = 9000000;
				break;
			case 1:
				Gold = 4500000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(14,28))
	{
		Gold = 210000;
	}

	else if (this->m_Type == ITEMGET(14,29))
	{
		Gold = (int)this->m_Durability * 10000 * 3;
	}

	else if (this->m_Type == ITEMGET(14,45) ||
		      this->m_Type == ITEMGET(14,46) ||
			  this->m_Type == ITEMGET(14,47) ||
			  this->m_Type == ITEMGET(14,48) ||
			  this->m_Type == ITEMGET(14,49) ||
			  this->m_Type == ITEMGET(14,50))
	{
		Gold = (int)this->m_Durability * 50 * 3;
	}

	else if (this->m_Type == ITEMGET(12,26))
	{
		switch ( this->m_Level )
		{
			case 0:
				Gold = 60000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(14,51))
	{
		Gold = 200000;
	}

	else if (this->m_Type == ITEMGET(13,49) || this->m_Type == ITEMGET(13,50) || this->m_Type == ITEMGET(13,51))
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 500000;
				break;
			case 2:
				Gold = 600000;
				break;
			case 3:
				Gold = 800000;
				break;
			case 4:
				Gold = 1000000;
				break;
			case 5:
				Gold = 1200000;
				break;
			case 6:
				Gold = 1400000;
				break;
			default:
				Gold = 9000;
				break;
		}
	}

	else if (this->m_Type == ITEMGET(13,52) || this->m_Type == ITEMGET(13,53))
	{
		Gold = 3000000;
	}

	else if (this->m_Type == ITEMGET(13,145))
	{
		Gold = (int)this->m_Durability * 30000;
	}

	else if (this->m_Type == ITEMGET(13,146))
	{
		Gold = 600000;
	}

	else if (this->m_Type == ITEMGET(13,147))
	{
		Gold = (int)this->m_Durability * 300000;
	}

	else if (this->m_Type == ITEMGET(14,23) ||
			this->m_Type == ITEMGET(14,24) ||
			this->m_Type == ITEMGET(14,25) ||
			this->m_Type == ITEMGET(14,26) ||
			this->m_Type == ITEMGET(14,65) ||
			this->m_Type == ITEMGET(14,66) ||
			this->m_Type == ITEMGET(14,67) ||
			this->m_Type == ITEMGET(14,68))
	{
		Gold = 9000;
	}

	else if (p->Value > 0)
	{
		Gold += (p->Value * p->Value * 10)/12;

		if ( this->m_Type >= ITEMGET(14,0) && this->m_Type <= ITEMGET(14,8) )	// Potions + Antidote
		{
			if ( this->m_Level > 0 )
			{
				Gold *= (__int64)(pow(2.0f, Level)) ;
			}

			Gold = Gold/10*10;
			Gold *= (UINT64)this->m_Durability;
			this->m_BuyMoney = Gold;
			this->m_SellMoney = Gold/g_ConfigRead.itemPrices.btItemSellPriceDivisor;
			this->m_SellMoney = this->m_SellMoney/10*10;
			return;
		}
	}

	else if (g_PentagramSystem.IsPentagramItem(this->m_Type) == true)
	{
		Gold = this->m_Level * 10 * this->m_Level * (this->m_Level + 50) * p->RequireLevel + 100000;
	}

	else if ((Type == 12 && (this->m_Type > ITEMGET(12,6) && this->m_Type < ITEMGET(12,36))) ||
			 (Type == 12 && (this->m_Type > ITEMGET(12,43) && this->m_Type < ITEMGET(12,49))) ||
			 (Type == 12 && (this->m_Type > ITEMGET(12,50) && this->m_Type < ITEMGET(12,262))) ||
			 (Type == 12 && (this->m_Type > ITEMGET(12,265))) ||
			 (Type == 13 && (this->m_Type != ITEMGET(13,30))) ||
			 (Type == 15))
	{
		Gold = Level2*Level2*Level2+100;

		for (int u=0;u<this->m_SpecialNum;u++)
		{
			switch ( this->m_Special[u] )
			{
				case 0x55:
					Gold += Gold*this->m_Option3;
			}
		}
	}

	else
	{
		if (this->m_Type == ITEMGET(14,63))
		{
			Gold = 200000;
		}

		else if (this->m_Type == ITEMGET(14,85) || this->m_Type == ITEMGET(14,86) || this->m_Type == ITEMGET(14,87) || this->m_Type == ITEMGET(14,90))
		{
			Gold = (int)this->m_Durability * 300;
		}

		else if (this->m_Type == ITEMGET(14,141))
		{
			Gold = 672000;
		}

		else if (this->m_Type == ITEMGET(14,142))
		{
			Gold = 546000;
		}

		else if (this->m_Type == ITEMGET(14,143))
		{
			Gold = 471000;
		}

		else if (this->m_Type == ITEMGET(14,144))
		{
			Gold = 363000;
		}

		else if (this->m_Type == ITEMGET(14,176) || this->m_Type == ITEMGET(14,177) || this->m_Type == ITEMGET(14,178) || this->m_Type == ITEMGET(14,179))
		{
			Gold = Level2 * Level2 * (Level2 + 40) / 8 + 100;
		}

		else if (this->m_Type == ITEMGET(0,41))
		{
			Gold = 100000;
		}

		else if (this->m_Type >= ITEMGET(14,217) && this->m_Type <= ITEMGET(14,221))
		{
			Gold = 300000;
		}

		else if (this->m_Type >= ITEMGET(14,234))
		{
			Gold = 3000000;
		}

		switch ( this->m_Level )
		{
			case  5:	Level2 +=   4;	break;
			case  6:	Level2 +=  10;	break;
			case  7:	Level2 +=  25;	break;
			case  8:	Level2 +=  45;	break;
			case  9:	Level2 +=  65;	break;
			case 10:	Level2 +=  95;	break;
			case 11:	Level2 += 135;	break;
			case 12:	Level2 += 185;	break;
			case 13:	Level2 += 245;	break;
			case 14:	Level2 += 305;	break;
			case 15:	Level2 += 365;	break;
		}

		if (Type == 12 && this->m_Type <= ITEMGET(12,6))
		{
			Gold = 40000000 + 11 * Level2 * Level2 * (Level2 + 40);
		}
		
		else if (Type == 12 && ((this->m_Type >= ITEMGET(12,36) && this->m_Type <= ITEMGET(12,40)) || this->m_Type == ITEMGET(12,50)))
		{
			Gold = 40000000 + 11 * Level2 * Level2 * (Level2 + 40);
		}

		else if (Type == 12 && (this->m_Type >= ITEMGET(12,41) && this->m_Type <= ITEMGET(12,43)))
		{
			Gold = 40000000 + 11 * Level2 * Level2 * (Level2 + 40);
		}

		else if (Type == 12 && (this->m_Type >= ITEMGET(12,262) && this->m_Type <= ITEMGET(12,265)))
		{
			Gold = 40000000 + 11 * Level2 * Level2 * (Level2 + 40);
		}

		else if (this->m_Type == ITEMGET(13,30) || this->m_Type == ITEMGET(12,49))
		{
			Gold = 40000000 + 11 * Level2 * Level2 * (Level2 + 40);
		}

		else
		{
			Gold = Level2 * Level2 * (Level2 + 40) / 8 + 100;
		}

		if ( Type >= 0 && Type <=  6 )
		{
			if ( p->TwoHand == 0 )
			{
				Gold = Gold*80/100;
			}
		}

		for ( int g=0;g<this->m_SpecialNum;g++)
		{
			switch ( this->m_Special[g] )
			{
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				case 24:
				case 56:
				case 260:
				case 261:
					Gold += (UINT64)(Gold * 1.5);
					break;

				case 80:
				case 81:
				case 83:
				case 85:
				case 113:
					switch ( this->m_Option3 )
					{
						case 1:
							Gold += (UINT64)(Gold * 6.0 / 10.0);
							break;

						case 2:
							Gold += (UINT64)(Gold * 14.0 / 10.0);
							break;

						case 3:
							Gold += (UINT64)(Gold * 28.0 / 10.0);
							break;

						case 4:
							Gold += (UINT64)(Gold * 56.0 / 10.0);
							break;
					}
					break;

				case 82:

					switch ( this->m_Option3 )
					{
					case 1:
							Gold += (UINT64)(Gold * 6.0 / 10.0);
							break;

						case 2:
							Gold += (UINT64)(Gold * 14.0 / 10.0);
							break;

						case 3:
							Gold += (UINT64)(Gold * 28.0 / 10.0);
							break;

						case 4:
							Gold += (UINT64)(Gold * 56.0 / 10.0);
							break;
					}
					break;

				case 84:
					Gold += (UINT64)(Gold * 25.0 / 100.0);
					break;

				case 86:
				case 87:
				case 88:
				case 89:
				case 90:
				case 91:
				case 92:
				case 93:
				case 94:
				case 95:
				case 96:
				case 97:
				case 98:
				case 99:
					Gold += Gold;
					break;

				case 100:
				case 101:
				case 102:
				case 103:
				case 104:
				case 105:
				case 108:
				case 109:
				case 110:
				case 111:
				case 114:
					Gold += (UINT64)(Gold * 25.0 / 100.0);
					break;

			}
		}
	}

	if (g_kItemSystemFor380.Is380OptionItem(this) == TRUE)
	{
		Gold += (UINT64)(Gold * 16.0 / 100.0);
	}

	if (g_SocketOptionSystem.IsSocketItem(this) == TRUE)
	{
		int iSocketSlotCount = 0;
		DWORD dwSeedSphereValue = 0;

		for (int i=0;i<5;i++)
		{
			if (this->m_SocketOption[i] != 0xFF &&
				this->m_SocketOption[i] != 0xFE)
			{
				dwSeedSphereValue += g_SocketOptionSystem.GetSeedSphereValue(this->m_SocketOption[i]);
				iSocketSlotCount++;
			}
		}

		Gold += (Gold * (8 * iSocketSlotCount) / 10.0) + dwSeedSphereValue;
	}

	if (g_PentagramSystem.IsPentagramItem(this) == TRUE)
	{
		int iPentagramSocketSlotCount = 0;

		for (int i=0;i<5;i++)
		{
			if (this->m_SocketOption[i] != 0xFF)
			{
				iPentagramSocketSlotCount++;
			}
		}

		Gold = (Gold * (iPentagramSocketSlotCount * 0.8));
	}

	if (g_PentagramSystem.IsPentagramJewel(this) == TRUE)
	{
		int iRankCount = 0;
		int iTotalLevel = 0;

		for (int i=0;i<5;i++)
		{
			if (this->m_SocketOption[i] != 0xFF)
			{
				iTotalLevel += (this->m_SocketOption[i] & 0xF0) >> 4;
				iRankCount++;
			}
		}

		int iAddLevel = 5 * iTotalLevel + p->Level;
		Gold = (iAddLevel / 8 * iAddLevel * (iAddLevel + 40) + 100) * (2 * iRankCount);
	}

	if (Type == 12 && this->m_Type == ITEMGET(12,266))
	{
		Gold = 300000000;
	}

	else if (Type == 12 && this->m_Type == ITEMGET(12,267))
	{
		Gold = 3000000;
	}

	if (Gold > 3000000000)
	{
		Gold = 3000000000;
	}

	this->m_BuyMoney = Gold;
	this->m_SellMoney = Gold;
	this->m_SellMoney /= g_ConfigRead.itemPrices.btItemSellPriceDivisor;

	if ( (this->m_Type < ITEMGET(14,0) || this->m_Type >  ITEMGET(14,8)) &&
		  this->m_Type != ITEMGET(13,20) &&
		  this->m_Type != ITEMGET(14,28) &&
		  this->m_Type != ITEMGET(14,29) &&
		  this->m_Type != ITEMGET(14,21) &&
		  this->m_Type != ITEMGET(13,15) &&
		  this->m_Type != ITEMGET(13,16) &&
		  this->m_Type != ITEMGET(13,17) &&
		  this->m_Type != ITEMGET(13,18) &&
		  this->m_Type != ITEMGET(13,11) &&
		  this->m_Type != ITEMGET(13,7)  &&
		  this->m_Type != ITEMGET(13,32) &&
		  this->m_Type != ITEMGET(13,33) &&
		  this->m_Type != ITEMGET(13,34) &&
		  this->m_Type != ITEMGET(13,35) &&
		  this->m_Type != ITEMGET(13,36) &&
		  this->m_Type != ITEMGET(13,37) &&
		  this->m_Type != ITEMGET(14,45) &&
		  this->m_Type != ITEMGET(14,46) &&
		  this->m_Type != ITEMGET(14,47) &&
		  this->m_Type != ITEMGET(14,48) &&
		  this->m_Type != ITEMGET(14,49) &&
		  this->m_Type != ITEMGET(14,100) &&
		  this->m_Type != ITEMGET(14,85) &&
		  this->m_Type != ITEMGET(14,86) &&
		  this->m_Type != ITEMGET(14,87) &&
		  this->m_Type != ITEMGET(14,90) &&
		  this->m_Type != ITEMGET(14,110) &&
		  this->m_Type != ITEMGET(14,101) &&
		  this->m_Type != ITEMGET(13,76) &&
		  this->m_Type != ITEMGET(13,77) &&
		  this->m_Type != ITEMGET(13,78) &&
		  this->m_Type != ITEMGET(13,80) &&
		  this->m_Type != ITEMGET(13,122) &&
		  this->m_Type != ITEMGET(13,123) &&
		  this->m_Type != ITEMGET(13,163) &&
		  this->m_Type != ITEMGET(13,164) &&
		  this->m_Type != ITEMGET(13,165) &&
		  this->m_Type != ITEMGET(13,166) &&
		  this->m_Type != ITEMGET(13,169) &&
		  this->m_Type != ITEMGET(13,170) &&
		  !(this->m_Type >= ITEMGET(13,109) && this->m_Type <= ITEMGET(13,115)) &&
		  !(this->m_Type >= ITEMGET(13,128) && this->m_Type <= ITEMGET(13,134)) &&
		  this->m_Type != ITEMGET(13,169) &&
		  this->m_Type != ITEMGET(13,170) &&
		  this->m_Type < ITEMGET(14,151) &&
		  this->m_Type <= ITEMGET(14,159) &&
		  !(this->m_Type >= ITEMGET(12,130) && this->m_Type <= ITEMGET(12,135)) &&
		  !g_PentagramSystem.IsPentagramItem(this->m_Type) &&
		  !g_PentagramSystem.IsPentagramJewel(this->m_Type) &&
		  !(this->m_Type >= ITEMGET(13,145) && this->m_Type <= ITEMGET(13,147)) &&
		  this->m_Type != ITEMGET(12,266) &&
		  this->m_Type != ITEMGET(12,267) &&
		  this->m_BaseDurability != 0.0)
	{
		float persent = 1.0 - this->m_Durability / this->m_BaseDurability;
		this->m_SellMoney -= (this->m_SellMoney * 0.6 * persent);
	}

	if ( this->m_SellMoney >= 1000 )
	{
		this->m_SellMoney = this->m_SellMoney/100*100;
	}
	else if ( this->m_SellMoney >= 100 )
	{
		this->m_SellMoney = this->m_SellMoney/10*10;
	}

	if ( this->m_BuyMoney >= 1000 )
	{
		this->m_BuyMoney = this->m_BuyMoney/100*100;
	}
	else if ( this->m_BuyMoney >= 100 )
	{
		this->m_BuyMoney = this->m_BuyMoney/10*10;
	}

	if (g_ConfigRead.data.common.joinmuSellPriceZero == true)
	{
		this->m_SellMoney = 0;
	}
}


void CItem::OldValue()
{
	if ( this->m_Type == -1 )
	{
		return;
	}

	int Gold = 0;

	if ( this->m_Type == ITEMGET(14,13) ) // Bless
	{
		Gold = 100000;
	}
	else if ( this->m_Type == ITEMGET(14,14) ) // Soul
	{
		Gold = 70000;
	}
	else if ( this->m_Type == ITEMGET(12,15) ) // Chaos
	{
		Gold = 40000;
	}
	else if ( this->m_Type == ITEMGET(14,22) ) // Creation
	{
		Gold = 450000;
	}
	else if ( this->m_Type == ITEMGET(14,16) ) // Life
	{
		Gold = 450000;
	}
	else
	{
		this->m_OldBuyMoney = this->m_BuyMoney;
		return;
	}

	this->m_OldBuyMoney = Gold;

	if ( this->m_OldBuyMoney >= 1000 )
	{
		this->m_OldBuyMoney = this->m_OldBuyMoney /100*100;
	}
	else if ( this->m_OldBuyMoney >= 100 )
	{
		this->m_OldBuyMoney = this->m_OldBuyMoney / 10 * 10;
	}
}

BOOL CItem::GetSize(int & w, int & h)
{
	w=ItemAttribute[this->m_Type].Width  ;
	h=ItemAttribute[this->m_Type].Height  ;

	return 1;
}

int CItem::ItemDamageMin()
{
	int damagemin;
	
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		damagemin=this->m_DamageMinOrigin;
		this->PlusSpecial(&damagemin, 80);
		return damagemin;
	}
}

int CItem::ItemDefense()
{
	int Defense;
	
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		if (this->m_IsValidItem == false)
		{
			return 0;
		}
		else
		{
			Defense=this->m_Defense;
			this->PlusSpecial(&Defense, 83);
			return Defense;
		}
	}
}




BOOL CItem::IsClass(char aClass, int ChangeUP)
{
	if ((aClass < 0) || (aClass >= MAX_TYPE_PLAYER ))
	{
		g_Log.Add("CItem: Invalid class: %d (%s %d)", aClass, __FILE__, __LINE__);
		return 0;
	}
	
	int requireclass=this->m_RequireClass[aClass];
	if (requireclass == 0)
	{
		g_Log.Add("CItem: Wrong Class (%d)", aClass);
		return 0;
	}
	if (requireclass > 1)
	{
		if ( requireclass > (ChangeUP+1) )
		{
			g_Log.Add("CItem: Wrong Quest State");
			return 0;
		}
	}
	return 1;
}

LPSTR CItem::GetName()
{
	return &ItemAttribute[this->m_Type].Name[0];
}


void CItem::PlusSpecialSetRing(BYTE * Value)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->IsSetItem() == FALSE )
	{
		return;
	}

	if ( this->m_Durability == 0.0f)
	{
		return;
	}

	if ( this->m_IsValidItem == false )
	{
		return;
	}

	LPITEM_ATTRIBUTE p = &ItemAttribute[this->m_Type];

	if ( p->ResistanceType == -1 )
	{
		return;
	}

	BYTE iValue = 5;
	Value[p->ResistanceType] += iValue - (BYTE)( iValue * this->m_CurrentDurabilityState);

}


void CItem::PlusSpecial(int * Value, int Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f)
	{
		return;
	}

	if ( this->m_IsValidItem == false )
	{
		return;
	}

	for (int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			switch ( Special )
			{
				case 82:
					{
						int lc3 = this->m_Option3*5;
						*Value += lc3 - (int)( lc3 * this->m_CurrentDurabilityState );
					}
					break;

				case 84:
					* Value += 4;
					break;

				case 81:
				case 80:
				case 83:
				case 113:
					{
						int lc4 = this->m_Option3*4;
						*Value += lc4 - (int)( lc4 * this->m_CurrentDurabilityState );
					}
					break;

				case 100:
				case 101:
					*Value =  *Value + this->m_Level * 5 + 50;
					break;

				case 103:
					*Value += 50;
					break;

				case 105:
					*Value =  *Value + this->m_Level * 5 + 10;
					break;

				default:
					*Value = *Value + (this->m_Option3*4);
					break;
			}
		}
	}
}

void CItem::PlusSpecialPercent(int * Value, int Special, WORD Percent)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	for (int i=0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			*Value += (*Value * Percent)/ 100;
		}
	}
}


void CItem::PlusSpecialPercentEx(int * Value, int SourceValue, int Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f )
	{
		return;
	}

	if ( this->m_IsValidItem == false  )
	{
		return;
	}

	for ( int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			int lc3 = SourceValue * this->m_Option3/100;
			*Value += lc3 - (int)(lc3 * this->m_CurrentDurabilityState );
		}
	}
}

void CItem::PlusSpecialPercentEx(float * Value, float SourceValue, float Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f )
	{
		return;
	}

	if ( this->m_IsValidItem == false  )
	{
		return;
	}

	for ( int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			int lc3 = SourceValue * this->m_Option3/100;
			*Value += lc3 - (int)(lc3 * this->m_CurrentDurabilityState );
		}
	}
}

void CItem::SetItemPlusSpecialStat(int * Value, int Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f )
	{
		return;
	}

	for ( int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			switch ( Special )
			{
				case 0xC4:
				case 0xC5:
				case 0xC6:
				case 0xC7:
					{
						int lc3 = this->m_SetAddStat;
						//*Value += lc3 - (int)(lc3 * this->m_CurrentDurabilityState);
						*Value += lc3;
					}
					break;

				default:
					*Value += this->m_Option3*3;
			}
		}
	}
}

int CItem::GetAddStatType()
{
	LPITEM_ATTRIBUTE p;
	p = &ItemAttribute[this->m_Type];
	return p->SetAttr;
}

int CItem::GetWeaponType()
{
	if ( this->m_Type <= ITEMGET(5,0) )
	{
		return 1;
	}

	if ( this->m_Type <= ITEMGET(6,0) )
	{
		return 2;
	}
	return 0;
}


void CItem::SetPetItemInfo(int petlevel, UINT64 petexp)
{
	if ( this->m_Type == ITEMGET(13,4) )
	{
		this->m_PetItem_Level = petlevel;
		this->m_PetItem_Exp = petexp;

		if ( this->m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level] || this->m_PetItem_Exp >= gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level] )
		{
			this->m_PetItem_Exp = gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level];
		}

		this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
			this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
		this->PetValue();
		this->m_IsLoadPetItemInfo = 1;
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		this->m_PetItem_Level = petlevel;
		this->m_PetItem_Exp = petexp;

		if ( this->m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level] || this->m_PetItem_Exp >= gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level] )
		{
			this->m_PetItem_Exp = gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level];
		}

		this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
			this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);

		this->PetValue();
		this->m_IsLoadPetItemInfo = 1;
	}
}



BOOL CItem::AddPetItemExp(UINT64 petexp)
{
	if ( this->m_Durability <= 0.0f )
	{
		return FALSE;
	}

	if ( this->m_IsLoadPetItemInfo == 0 )
	{
		return FALSE;
	}

	if ( this->m_PetItem_Level < 1 )
	{
		this->m_PetItem_Level = 1;
		this->m_PetItem_Exp = 0;
		this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
			this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
		this->PetValue();

		return FALSE;
	}

	if ( this->m_Type == ITEMGET(13,4) )
	{
		if ( this->m_PetItem_Level >= MAX_PET_LEVEL )
		{
			return FALSE;
		}

		BOOL ret = FALSE;
		UINT64 maxexp = gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level];
		this->m_PetItem_Exp += petexp;

		while ( this->m_PetItem_Exp >= maxexp )
		{
			if(this->m_PetItem_Level == MAX_PET_LEVEL)
			{
				ret = TRUE;
				break;
			}
			this->m_PetItem_Level++;
			maxexp = gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level];
			ret = TRUE;
		}

		if (ret != FALSE)
		{
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
				this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			this->PetValue();

			return TRUE;
		}
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		if ( this->m_PetItem_Level >= MAX_PET_LEVEL )
		{
			return FALSE;
		}

		BOOL ret = FALSE;
		UINT64 maxexp = gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level];

		this->m_PetItem_Exp += petexp;

		while ( this->m_PetItem_Exp >= maxexp )
		{
			if(this->m_PetItem_Level == MAX_PET_LEVEL)
			{
				ret = TRUE;
				break;
			}

			this->m_PetItem_Level++;
			maxexp = gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level];
			ret = TRUE;
		}

		if (ret != FALSE)
		{
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
				this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			this->PetValue();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CItem::DecPetItemExp(int percent)
{
	if ( this->m_IsLoadPetItemInfo == 0 )
	{
		return FALSE;
	}

	if ( this->m_PetItem_Level < 1 )
	{
		this->m_PetItem_Level = 1;
		this->m_PetItem_Exp = 0;
		this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
			this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
		this->PetValue();

		return FALSE;
	}

	if ( this->m_Type == ITEMGET(13,4) )
	{
		UINT64 exp = gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level] - gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level];
		exp = exp * percent/100;
		this->m_PetItem_Exp -= exp;

		if ( this->m_PetItem_Exp < 0 )
		{
			this->m_PetItem_Level = 1;
			this->m_PetItem_Exp = 0;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
				this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			this->PetValue();
		}
		else if ( this->m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level] )
		{
			this->m_PetItem_Level--;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
				this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			this->PetValue();
			
			return TRUE;
		}
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		UINT64 exp = gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level] - gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level];
		exp = exp * percent/100;
		this->m_PetItem_Exp -= exp;

		if ( this->m_PetItem_Exp < 0 )
		{
			this->m_PetItem_Level = 1;
			this->m_PetItem_Exp = 0;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
				this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			this->PetValue();
		}
		else if ( this->m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level] )
		{
			this->m_PetItem_Level--;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
				this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			this->PetValue();
			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CItem::PetItemLevelDown(UINT64 exp)
{
	if ( this->m_IsLoadPetItemInfo == FALSE )
	{
		return FALSE;
	}

	if ( this->m_PetItem_Level < 2 )
	{
		return FALSE;
	}

	this->m_PetItem_Exp -= exp;
	this->m_PetItem_Level--;

	if ( this->m_Type == ITEMGET(13,4) )
	{
		int petlevel = this->m_PetItem_Level;
		UINT64 petexp = gPetItemExp.m_DarkHorseExpTable[petlevel];

		while ( this->m_PetItem_Exp <= petexp )
		{
			if ( petlevel <= 1 )
			{
				break;
			}

			petlevel--;
			petexp = gPetItemExp.m_DarkHorseExpTable[petlevel];
		}

		this->m_PetItem_Level = petlevel;
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		int petlevel = this->m_PetItem_Level;
		UINT64 petexp = gPetItemExp.m_DarkSpiritExpTable[petlevel];

		while ( this->m_PetItem_Exp <= petexp )
		{
			if ( petlevel <= 1 )
			{
				break;
			}

			petlevel--;
			petexp = gPetItemExp.m_DarkSpiritExpTable[petlevel];
		}

		this->m_PetItem_Level = petlevel;
	}

	this->Convert(this->m_Type, this->m_Option1, this->m_Option2,
		this->m_Option3, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
	this->PetValue();

	return TRUE;
}



void CItem::PetValue()
{
	if ( this->m_Type == ITEMGET(13,4) )
	{
		this->m_SellMoney = this->m_PetItem_Level * 2000000;
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		this->m_SellMoney = this->m_PetItem_Level * 1000000;
	}

	this->m_BuyMoney = this->m_SellMoney;

	if ( this->m_SellMoney >= 1000 )
	{
		this->m_SellMoney = this->m_SellMoney / 100 * 100;
	}
	else if ( this->m_SellMoney >= 100 )
	{
		this->m_SellMoney = this->m_SellMoney / 10 * 10;
	}

	if ( this->m_BuyMoney >= 1000 )
	{
		this->m_BuyMoney = this->m_BuyMoney / 100 * 100;
	}
	else if ( this->m_BuyMoney >= 100 )
	{
		this->m_BuyMoney = this->m_BuyMoney / 10 * 10;
	}

	if (g_ConfigRead.data.common.joinmuSellPriceZero == true)
	{
		this->m_SellMoney = 0;
	}
}


BOOL CItem::IsExtItem()
{
	if ( this->m_Type == ITEMGET(13,37) )
	{
		return FALSE;
	}

	if ( (this->m_NewOption & 0x7F)!= 0 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CItem::IsCurseSpell()
{
	if ( this->m_IsValidItem == FALSE )
	{
		return 0;
	}

	if ( this->m_Type < 0 )
	{
		return 0;
	}

	return this->m_CurseSpell;
}

BOOL CItem::IsDinorantReduceAttackDamaege()
{
	if ( this->m_Durability  < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption & 1 ) == 1 )
	{
		return 5;
	}
	return 0;
}

int CItem::IsFenrirIncLastAttackDamage()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&1) == 1 )
	{
		return 10;
	}

	return 0;
}

int CItem::IsFenrirDecLastAttackDamage()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&2) == 2 )
	{
		return 10;
	}

	return 0;
}

int CItem::IsFenrirAddExp()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( this->m_NewOption == 0 )
	{
		return g_ConfigRead.pet.RedFenrirAddExp;
	}

	if ( (this->m_NewOption&1) == 1 )
	{
		return g_ConfigRead.pet.BlackFenrirAddExp;
	}

	if ( (this->m_NewOption&2) == 2 )
	{
		return g_ConfigRead.pet.BlueFenrirAddExp;
	}

	if ( (this->m_NewOption&4) == 4 )
	{
		return g_ConfigRead.pet.GoldFenrirAddExp;
	}

	return 0;
}

int CItem::IsFenrirIllusionDecDmg()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&4) == 4 )
	{
		return g_ConfigRead.pet.GoldFenrirDefenseRise; // custom feature dmg dec 
	}

	return 0;
}

int CItem::IsFenrirIllusion()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&4) == 4 )
	{
		return g_ConfigRead.pet.GoldFenrirAttackRise; //Attack %
	}

	return 0;
}

int CItem::IsPeriodItem()
{
	return (this->m_PeriodItemOption & 1) == 1;
}

int CItem::IsPeriodItemExpire()
{
	if (this->IsPeriodItem() == false)
	{
		return false;
	}

	return (this->m_PeriodItemOption & 2) == 2;
}

void CItem::SetPeriodItem()
{
	this->m_PeriodItemOption = 1;
}

void CItem::SetPeriodItemExpire()
{
	this->m_PeriodItemOption |= 2;
	this->m_Durability = 0.0;
}

int CItem::SimpleDurabilityDown(int iDecValue)
{
	if(this->IsPeriodItem() == TRUE)
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	this->m_Durability -= 1.0f ;

	if ( this->m_Durability < 0 )
	{
		this->m_Durability = 0.0f;
		return 0;
	}

	if ( this->CheckDurabilityState() != 0 )
	{
		return 2;
	}

	return this->m_Durability;
}

int CItem::DurabilityDown(int dur, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if ( this->IsPeriodItem() == TRUE )
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	this->m_DurabilitySmall += dur;

	int iBaseDurSmall = 564;

	if (gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur2_Monk > 0.0)
		{
			iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur2_Monk;
		}
	}

	else if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur2 > 0.0)
	{
		iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur2;
	}
	
	if ( this->m_DurabilitySmall > iBaseDurSmall )
	{
		this->m_DurabilitySmall = 0;
		this->m_Durability -= 1.0f;

		if ( this ->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != FALSE )
		{
			return 2;
		}
		return 1;
	}
	
	return this->m_Durability;
}



int CItem::DurabilityDown2(int dur, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if (this->IsPeriodItem() == TRUE)
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	this->m_DurabilitySmall += dur;

	int iBaseDurSmall = 1000;

	if (gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
		{
			iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk;
		}
	}

	else if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
	{
		iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1;
	}
	
	if ( this->m_DurabilitySmall > iBaseDurSmall )
	{
		this->m_DurabilitySmall = 0;
		this->m_Durability -= 1.0f;

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != FALSE )
		{
			return 2;
		}
		return 1;
	}
	
	return 0;
}



int CItem::NormalWeaponDurabilityDown(int defense, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if ( this->IsPeriodItem() == TRUE )
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	int damagemin = this->m_DamageMinOrigin;
	int plusdamage = 0;
	this->PlusSpecial(&plusdamage, 80);

	if ( damagemin == 0 )
	{
		g_Log.Add("MinDamage is 0!");
		return 0;
	}

	int div = damagemin + damagemin/2 + plusdamage;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = (defense*2)/div;

	DecreaseDur -= DecreaseDur*(int)this->m_ImproveDurabilityRate / 100; //season4 add-on

	if(DecreaseDur < 0)
	{
		DecreaseDur = 0;
	}

	this->m_DurabilitySmall += DecreaseDur;

	int iBaseDurSmall = 846;

	if (gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
		{
			iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk;
		}
	}

	else if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
	{
		iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1;
	}

	if ( this->m_DurabilitySmall > iBaseDurSmall )
	{
		this->m_DurabilitySmall = 0;
		this->m_Durability -= 1.0f;

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}


int CItem::BowWeaponDurabilityDown(int defense, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if ( this->IsPeriodItem() == TRUE )
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	int damagemin = this->m_DamageMinOrigin;
	int plusdamage = 0;
	this->PlusSpecial(&plusdamage, 80);
	
	int div = damagemin + damagemin/2 + plusdamage;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = (defense*2)/div;

	DecreaseDur -= DecreaseDur*(int)this->m_ImproveDurabilityRate / 100; //season4 add-on

	if(DecreaseDur < 0)
	{
		DecreaseDur = 0;
	}

	this->m_DurabilitySmall += DecreaseDur;

	int iBaseDurSmall = 1170;

	if (gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
		{
			iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk;
		}
	}

	else if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
	{
		iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1;
	}

	if ( this->m_DurabilitySmall > iBaseDurSmall )
	{
		this->m_DurabilitySmall = 0;
		this->m_Durability -= 1.0f;

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}
		
		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}

int CItem::StaffWeaponDurabilityDown(int defence, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if ( this->IsPeriodItem() == TRUE )
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	int magic = this->m_Magic/2 + this->m_Level*2;
	int plusmagic = 0;
	this->PlusSpecial(&plusmagic, 81);
	int div = magic + magic/3 + plusmagic;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = defence/div;

	DecreaseDur -= DecreaseDur*(int)this->m_ImproveDurabilityRate / 100; //season4 add-on

	if(DecreaseDur < 0)
	{
		DecreaseDur = 0;
	}

	this->m_DurabilitySmall += DecreaseDur;

	int iBaseDurSmall = 1575;

	if (gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
		{
			iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk;
		}
	}

	else if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
	{
		iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1;
	}

	if ( this->m_DurabilitySmall > iBaseDurSmall )
	{
		this->m_DurabilitySmall = 0;
		this->m_Durability -= 1.0f;

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}
		
		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		return 1;
		
	}
	return 0;
}

int CItem::LuckyItemArmorDurabilityDown(int damagemin, int aIndex)
{
	if (this->m_Durability <= 0.0f)
	{
		return 0;
	}

	if (this->m_Type == -1)
	{
		return 0;
	}

	if (this->m_DefenseOrigin == 0)
	{
		g_Log.Add("Defense is 0!");
		return 0;
	}

	this->m_DurabilitySmall += damagemin;

	if (this->m_DurabilitySmall < g_LuckyItemManager.GetLuckyItemDurabilityTime())
	{
		return 0;
	}

	this->m_DurabilitySmall = 0;
	this->m_Durability -= 1.0;

	if (this->m_Durability < 0.0)
	{
		this->m_Durability = 0;
	}

	g_Log.Add("[LuckyItem] Durability Down [%s][%s] [%d][%s] Serial:[%I64d] Dur:[%d/%d]",
		gObj[aIndex].AccountID, gObj[aIndex].Name, this->m_Type,
		this->GetName(), this->m_Number, (int)this->m_Durability, (int)this->m_BaseDurability);

	if (CheckDurabilityState())
	{
		g_LuckyItemManager.GDReqLuckyItemDelete(this->m_Type, this->m_Number, aIndex);
		return 2;
	}
	else
	{
		return 1;
	}

}

int CItem::ArmorDurabilityDown(int damagemin, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}	
	
	if ( this->m_Type == -1 )
	{
		return 0;
	}

	if(g_ConfigRead.data.common.joinmuDurabilityDownDisable == true)
	{
		return 0;
	}

	if (g_LuckyItemManager.IsLuckyItemEquipment(this->m_Type))
	{
		return this->m_Durability;
	}

	if ( this->IsPeriodItem() == TRUE )
	{
		return 0;
	}

	int def = this->m_DefenseOrigin;
	int plusdef = 0;
	int DecreaseDur;

	if ( def == 0 )
	{
		g_Log.Add("def = 0!");
		return 0;
	}

	if ( this->m_Type >= ITEMGET(6,0) && this->m_Type < ITEMGET(7,0) )
	{
		this->PlusSpecial(&plusdef, 82);
		DecreaseDur = damagemin/(def * 5+ plusdef);
	}
	else if ( this->m_RequireClass[0] != 0 )	// Dark Wize
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( this->m_RequireClass[1] != 0 )	// Dark Knight
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( this->m_RequireClass[2] != 0 )	// Elf
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 2 + plusdef);
	}
	else if ( this->m_RequireClass[3] != 0 )	// MAgic Gladiator ( aka Magunsa )
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 7 + plusdef);
	}
	else if ( this->m_RequireClass[4] != 0 )	// Dark Lord
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 6 + plusdef);
	}
	else if ( this->m_RequireClass[5] != 0 )	// Summoner
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( this->m_RequireClass[6] != 0 )	// RF
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}

	if(DecreaseDur < 0)
	{
		DecreaseDur = 0;
	}

	this->m_DurabilitySmall += DecreaseDur;

	int iBaseDurSmall = 69;

	if (gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk > 0.0)
		{
			iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1_Monk;
		}
	}

	else if (gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1 > 0.0)
	{
		iBaseDurSmall += gObj[aIndex].m_PlayerData->m_MPSkillOpt.iMpsDownDur1;
	}

	if ( this->m_DurabilitySmall > iBaseDurSmall )
	{
		this->m_DurabilitySmall = 0;
		this->m_Durability -= 1.0f;

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}
		
		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}



BOOL CItem::CheckDurabilityState()
{
	if ( this->m_Durability == 0.0f )
	{
		if ( this->m_CurrentDurabilityState != 1.0f )
		{
			this->m_CurrentDurabilityState = 1.0f;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2, this->m_Option3, this->m_NewOption,
				this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_Durability < this->m_DurabilityState[2] )
	{
		if ( this->m_CurrentDurabilityState != 0.5f )
		{
			this->m_CurrentDurabilityState = 0.5f;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2, this->m_Option3, this->m_NewOption,
				this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_Durability < this->m_DurabilityState[1] )
	{
		if ( this->m_CurrentDurabilityState != 0.3f )
		{
			this->m_CurrentDurabilityState = 0.3f;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2, this->m_Option3, this->m_NewOption,
				this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_Durability < this->m_DurabilityState[0] )
	{
		if ( this->m_CurrentDurabilityState != 0.2f )
		{
			this->m_CurrentDurabilityState = 0.2f;
			this->Convert(this->m_Type, this->m_Option1, this->m_Option2, this->m_Option3, this->m_NewOption,
				this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_CurrentDurabilityState != 0.0f )
	{
		this->m_CurrentDurabilityState = 0;
		this->Convert(this->m_Type, this->m_Option1, this->m_Option2, this->m_Option3, this->m_NewOption,
			this->m_SetOption, this->m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
		return true;
	}

	return false;
}

bool CItem::IsMuunItemPeriodExpire()
{
	return (this->m_BonusSocketOption & 0x10) != false;
}

void CItem::SetMuunItemPeriodExpire()
{
	this->m_BonusSocketOption |= 0x10;
}

void CItem::SetMuunItemPeriodReset()
{
	this->m_BonusSocketOption &= 0xF;
}

int CItem::GetMuunItemRank()
{
	return this->m_BonusSocketOption & 0xF;
}

//-----------------------------------------------------------
// End of CItem class
//-----------------------------------------------------------

void BufferItemtoConvert3(unsigned char* buf, int& type, BYTE& level, BYTE& op1, BYTE& op2, BYTE& op3, BYTE& dur)
{
	type = buf[0] + ((buf[3]&0x80)*2)+((buf[5]&0xF0)<<5);
	level = (buf[1]>>3) & 15;
	op1 = (buf[1]>>7) & 1;
	op2 = (buf[1]>>2) & 1;
	op3 = (buf[1] & 3 );
	dur = buf[2];
	op3 |= (buf[3] & 0x40) >> 4;
}

void ItemByteConvert(unsigned char* buf, CItem item)
{
	int n=0;

	memset(buf, 0, 12);
	buf[n] = item.m_Type & 0xFF;
	n++;
	buf[n] = 0;
	buf[n] |= item.m_Level * 8;
	buf[n] |= item.m_Option1 * 128;
	buf[n] |= item.m_Option2 * 4;
	buf[n] |= item.m_Option3 & 3;
	n++;
	buf[n] = item.m_Durability;
	n++;
	buf[n] = 0;
	buf[n] |= ((item.m_Type & 0x100) >> 1);

	if ( item.m_Option3 > 3 )
	{
		buf[n] |= 0x40; // item +16 option
	}

	buf[n] |= item.m_NewOption;
	n++;
	buf[n] = item.m_SetOption;
 	n++;
	buf[n] |= (item.m_Type & 0x1E00 ) >> 5;

	BYTE btItemEffectFor380 = 0;
	btItemEffectFor380 = (item.m_ItemOptionEx & 0x80 ) >> 4;
	buf[n] |= btItemEffectFor380;
	buf[n] |= 2 * item.m_PeriodItemOption;
	n++;
	buf[n] = item.m_JewelOfHarmonyOption;

	if (g_SocketOptionSystem.IsSocketItem(&item) == true)
	{
		buf[n] = item.m_BonusSocketOption;
	}

	if (g_PentagramSystem.IsPentagramItem(&item) == true || g_PentagramSystem.IsPentagramJewel(&item) == true)
		buf[n] = item.m_BonusSocketOption;
	if (g_CMuunSystem.IsMuunItem(&item) == 1)
		buf[n] = item.m_BonusSocketOption;
	if (g_CMuunSystem.IsStoneofEvolution(&item) == 1)
		buf[n] = 0;
	n++;
	buf[n] = item.m_SocketOption[0];
	n++;
	buf[n] = item.m_SocketOption[1];
	n++;
	buf[n] = item.m_SocketOption[2];
	n++;
	buf[n] = item.m_SocketOption[3];
	n++;
	buf[n] = item.m_SocketOption[4];
}

void ItemByteConvert(unsigned char* buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, BYTE PeriodItemOption)
{
	memset(buf, 0, 12);
	int n = 0;

	buf[n] = type & 0xFF;
	n++;
	buf[n] = 0;
	buf[n] |= level * 8;
	buf[n] |= Option1 * 128;
	buf[n] |= Option2 * 4;
	buf[n] |= Option3 & 3;
	n++;
	buf[n] = dur;
	n++;
	buf[n] = 0;
	buf[n] |= ((type& 0x100) >> 1);

	if ( Option3 > 3 )
	{
		buf[n] |= 0x40;
	}

	buf[n] |= Noption;
	n++;
	buf[n] = SetOption;
	n++;
	buf[n] |= (type & 0x1E00 ) >> 5;

	BYTE btItemEffectFor380 = 0;
	btItemEffectFor380 = (ItemEffectEx  & 0x80 ) >> 4;
	buf[n] |= btItemEffectFor380;
	buf[n] |= 2 * PeriodItemOption;
	n++;
	buf[n] = JewelOfHarmonyOption;

	if(SocketOption)
	{
		buf[n] = SocketBonusOption;
		n++;
		buf[n] = SocketOption[0];
		n++;
		buf[n] = SocketOption[1];
		n++;
		buf[n] = SocketOption[2];
		n++;
		buf[n] = SocketOption[3];
		n++;
		buf[n] = SocketOption[4];
	}
	else
	{
		n++;
		buf[n] = -1;
		n++;
		buf[n] = -1;
		n++;
		buf[n] = -1;
		n++;
		buf[n] = -1;
		n++;
		buf[n] = -1;
	}
}

void ItemByteConvert32(LPBYTE buf, CItem * const item , int maxitem)
{
	int n=0;
	int count=0;
	DWORD hidword, lodword;

	for (int index=0; index < maxitem ;index++)
	{
		if ( item[index].m_Type == ITEMGET(13,19) )
		{
			if ( CHECK_LIMIT(item[index].m_Level, 3) != FALSE )
			{
				buf[n+0]=-1;
				buf[n+1]=-1;
				buf[n+2]=-1;
				buf[n+3]=-1;
				buf[n+4]=-1;
				buf[n+5]=-1;
				buf[n+6]=-1;
				buf[n+7]=-1;
				buf[n+8]=-1;
				buf[n+9]=-1;
				buf[n+10]=-1;
				buf[n+11]=-1;
				buf[n+12]=-1;
				buf[n+13]=-1;
				buf[n+14]=-1;
				buf[n+15]=-1;
				buf[n+16]=-1;
				buf[n+17]=-1;
				buf[n+18]=-1;
				buf[n+19]=-1;
				buf[n+20]=-1;
				buf[n+21]=-1;
				buf[n+22]=-1;
				buf[n+23]=-1;
				buf[n+24]=-1;
				buf[n+25]=-1;
				buf[n+26]=-1;
				buf[n+27]=-1;
				buf[n+28]=-1;
				buf[n+29]=-1;
				buf[n+30]=-1;
				buf[n+31]=-1;
				n+=32;

				continue;
			}
		}

		if ( item[index].m_Type < ITEMGET(0,0) )
		{
			buf[n+0]=-1;
			buf[n+1]=-1;
			buf[n+2]=-1;
			buf[n+3]=-1;
			buf[n+4]=-1;
			buf[n+5]=-1;
			buf[n+6]=-1;
			buf[n+7]=-1;
			buf[n+8]=-1;
			buf[n+9]=-1;
			buf[n+10]=-1;
			buf[n+11]=-1;
			buf[n+12]=-1;
			buf[n+13]=-1;
			buf[n+14]=-1;
			buf[n+15]=-1;
			buf[n+16]=-1;
			buf[n+17]=-1;
			buf[n+18]=-1;
			buf[n+19]=-1;
			buf[n+20]=-1;
			buf[n+21]=-1;
			buf[n+22]=-1;
			buf[n+23]=-1;
			buf[n+24]=-1;
			buf[n+25]=-1;
			buf[n+26]=-1;
			buf[n+27]=-1;
			buf[n+28]=-1;
			buf[n+29]=-1;
			buf[n+30]=-1;
			buf[n+31]=-1;
			n+=32;
		}
		else
		{
			memset(buf+n, 0, 32);

			buf[n] = (BYTE)item[index].m_Type % 256;
			n++;
			buf[n]=0;
			buf[n] |= item[index].m_Level * 8;
			buf[n] |= item[index].m_Option1 * 128;
			buf[n] |= item[index].m_Option2 * 4;
			buf[n] |= item[index].m_Option3 & 3;
			n++;
			buf[n] = item[index].m_Durability;
			n++;
			hidword = HIDWORD(item[index].m_Number);
			lodword = LODWORD(item[index].m_Number);
			buf[n] = SET_NUMBERH(SET_NUMBERHW(hidword));
			n++;
			buf[n] = SET_NUMBERL(SET_NUMBERHW(hidword));
			n++;
			buf[n] = SET_NUMBERH(SET_NUMBERLW(hidword));
			n++;
			buf[n] = SET_NUMBERL(SET_NUMBERLW(hidword));
			n++;
			buf[n] = 0;
			BYTE btItemType=0;
			btItemType |= (item[index].m_Type & 0x1E00 ) >> 5;
			buf[n] |= ((item[index].m_Type & 0x100) >> 1);

			if ( item[index].m_Option3 > 3 )
			{
				buf[n] |= 0x40; // item +16 option
			}

			buf[n] |= item[index].m_NewOption;
			n++;
			buf[n] = item[index].m_SetOption;
			n++;
			buf[n] = 0;
			buf[n] |= btItemType;
			BYTE btItemEffectFor380 = 0;
			btItemEffectFor380 = (item[index].m_ItemOptionEx & 0x80 ) >> 4;
			buf[n] |= btItemEffectFor380;
			buf[n] |= 2 * item[index].m_PeriodItemOption;
			n++;
			
			if ( item[index].m_JewelOfHarmonyOption == 0xFF )
				item[index].m_JewelOfHarmonyOption = 0;

			buf[n] = item[index].m_JewelOfHarmonyOption;

			if (g_SocketOptionSystem.IsSocketItem(&item[index]) == true)
				buf[n] = item[index].m_BonusSocketOption;

			if (g_PentagramSystem.IsPentagramItem(&item[index]) == true || g_PentagramSystem.IsPentagramJewel(&item[index]) == true)
				buf[n] = item[index].m_BonusSocketOption;

			if (g_CMuunSystem.IsMuunItem(&item[index]) == true)
				buf[n] = item[index].GetMuunItemRank();

			n++;
			buf[n] = item[index].m_SocketOption[0];
			n++;
			buf[n] = item[index].m_SocketOption[1];
			n++;
			buf[n] = item[index].m_SocketOption[2];
			n++;
			buf[n] = item[index].m_SocketOption[3];
			n++;
			buf[n] = item[index].m_SocketOption[4];
			n++;

			buf[n] = SET_NUMBERH(SET_NUMBERHW(lodword));
			n++;
			buf[n] = SET_NUMBERL(SET_NUMBERHW(lodword));
			n++;
			buf[n] = SET_NUMBERH(SET_NUMBERLW(lodword));
			n++;
			buf[n] = SET_NUMBERL(SET_NUMBERLW(lodword));
			n++;

			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;
			buf[n] = -1;
			n++;

			count++;
		}
	}

	//g_Log.AddC(1, __FILE__, __FUNCTION__, TColor::Yellow, "[DEBUG] count:%d", count);
}
	
// ItemAttribute Begin:9439368 END 9446B68 Array[512]


int ItemGetNumberMake(int type, int index)
{
	int make;

	if (type < 0 || index < 0 || type > (MAX_TYPE_ITEMS-1) || index > (MAX_SUBTYPE_ITEMS-1))
	{
		return -1;
	}

	make = type*MAX_SUBTYPE_ITEMS + index;

	if (ItemAttribute[make].Width < 1 ||  ItemAttribute[make].Height < 1)
	{
		return -1;
	}
	return make;
}

void ItemGetSize(int index, int & width, int & height)
{
	if(index < 0 || index > MAX_ITEMS)
	{
		width = -1;
		height = -1;
		return;
	}

	width = ItemAttribute[index].Width;
	height = ItemAttribute[index].Height;
}

BOOL HasItemDurability(int index)
{
	if ( ItemAttribute[index].Durability == 0 && ItemAttribute[index].MagicDurability == 0)
	{
		return FALSE; 
	}
	
	return TRUE;
}

int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem)
{
	 if ( index < 0 || index > MAX_ITEMS )
	 {
		 g_Log.AddC(TColor::Red,"BAD INDEX %d",index);
		 return 0;
	 }

	 if (index == ITEMGET(14, 21) && itemLevel == 3)	// Mark Lord
		 itemLevel = 0;

	 if (index == ITEMGET(14, 29))
		 return 1;

	 if (index == ITEMGET(14, 100)) //season 4.5 lucky coin add-on
		 return 1;

	 if (g_PentagramSystem.IsPentagramItem(index) == true)
	 {
		 return ItemAttribute[index].Durability;
	 }

	 if (index == ITEMGET(12, 144) || index == ITEMGET(12, 146))
	 {
		 return 1;
	 }

	 int dur = 0;

	 if (itemLevel < 5)
	 {
		 dur = ItemAttribute[index].Durability + itemLevel;
	 }

	 else if (itemLevel >= 5)
	 {
		 if (itemLevel == 10)
		 {
			 dur = ItemAttribute[index].Durability + itemLevel * 2 - 3;
		 }
		 else if (itemLevel == 11)
		 {
			 dur = ItemAttribute[index].Durability + itemLevel * 2 - 1;
		 }
		 else if (itemLevel == 12)
		 {
			 dur = ItemAttribute[index].Durability + itemLevel * 2 + 2;
		 }
		 else if (itemLevel == 13)
		 {
			 dur = ItemAttribute[index].Durability + itemLevel * 2 + 6;
		 }
		 else if (itemLevel == 14)
		 {
			 dur = ItemAttribute[index].Durability + 39;
		 }
		 else if (itemLevel == 15)
		 {
			 dur = ItemAttribute[index].Durability + 47;
		 }
		 else
		 {
			 dur = ItemAttribute[index].Durability + itemLevel * 2 - 4;
		 }
	 }

	 if (index == ITEMGET(13, 51)) //Season 2.5 Illusion Temple Blood Scroll Ticket
	 {
		 dur = 1;
	 }

	 if ((index < ITEMGET(12, 3) || index > ITEMGET(12, 6)) &&
		 index != ITEMGET(0, 19) &&
		 index != ITEMGET(4, 18) &&
		 index != ITEMGET(5, 10) &&
		 index != ITEMGET(2, 13) &&
		 index != ITEMGET(13, 30) &&
		 index != ITEMGET(12, 36) &&
		 index != ITEMGET(12, 37) &&
		 index != ITEMGET(12, 38) &&
		 index != ITEMGET(12, 39) &&
		 index != ITEMGET(12, 40) &&
		 index != ITEMGET(12, 41) &&
		 index != ITEMGET(12, 42) &&
		 index != ITEMGET(12, 43) &&
		 index != ITEMGET(12, 49) &&
		 index != ITEMGET(12, 50) &&
		 index != ITEMGET(12, 262) &&
		 index != ITEMGET(12, 263) &&
		 index != ITEMGET(12, 264) &&
		 index != ITEMGET(12, 265))
	 {
		 if (SetItem != 0)
			 dur += 20;
		 else if (ExcellentItem != 0) // Prevent duple if items
			 dur += 15;
	 }

	 if (dur > 255)
		 dur = 255;

	 return dur;
}


int ItemGetAttackDurability(int index)
{
	return ItemAttribute[index].AttackDur;
}

int ItemGetDefenseDurability(int index)
{
	return ItemAttribute[index].DefenceDur;
}

float GetRepairItemRate(int index)
{
	return ItemAttribute[index].RepaireMoneyRate;
}
// ItemAttribute Begin:9439368 END 9446B68 Array[512]

float GetAllRepairItemRate(int index)
{
	return ItemAttribute[index].AllRepaireMoneyRate;
}

void CalRepairRate( int itemtype, int itemsubtype, LPITEM_ATTRIBUTE p)
{
	p->RepaireMoneyRate = (float)0.1;
	p->AllRepaireMoneyRate = (float)1.4;
}

BOOL OpenItemScript(char* FileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(FileName);

	if(res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", FileName, res.description());
		return FALSE;
	}

	memset(ItemAttribute, 0, sizeof(ItemAttribute));

	for (int n = 0; n<MAX_ITEMS; n++)
	{
		ItemAttribute[n].Level = -1;
		ItemAttribute[n].RepaireMoneyRate = 0;
		ItemAttribute[n].AllRepaireMoneyRate = 0;
	}
	
	int iItemCount = 0;

	pugi::xml_node main = file.child("ItemList");

	for(pugi::xml_node section = main.child("Section"); section; section = section.next_sibling())
	{
		int ItemType = section.attribute("Index").as_int();

		for(pugi::xml_node item = section.child("Item"); item; item = item.next_sibling())
		{
			int ItemIndex = item.attribute("Index").as_int();

			if(ITEMGET(ItemType, ItemIndex) < 0 || ITEMGET(ItemType, ItemIndex) >= ITEMGET(16,0))
			{
				g_Log.MsgBox("Error - wrong Item configuration (ItemType:%d ItemIndex:%d) in Section:%d", ItemType, ItemIndex, ItemType);
				return FALSE;
			}

			if (ItemType >= 0 && ItemType < MAX_TYPE_ITEMS)
			{
				if (g_MaxItemIndexOfEachItemType[ItemType] <= ItemIndex)
				{
					g_MaxItemIndexOfEachItemType[ItemType] = ItemIndex;
				}
			}

			LPITEM_ATTRIBUTE p = &ItemAttribute[ItemType*MAX_SUBTYPE_ITEMS + ItemIndex];

			p->ItemSlot = item.attribute("Slot").as_int();
			p->SkillType = item.attribute("SkillIndex").as_int();
			p->Width = item.attribute("Width").as_int();
			p->Height = item.attribute("Height").as_int();
			p->Serial = item.attribute("Serial").as_int();
			p->OptionFlag = item.attribute("Option").as_int();
			p->MondownFlag = item.attribute("Drop").as_int();
			strcpy(p->Name, item.attribute("Name").as_string());

			if (ItemType >= 0 && ItemType <= 5)
			{
				p->Level = item.attribute("DropLevel").as_int();
				p->DamageMin = item.attribute("DamageMin").as_int();
				p->DamageMax = item.attribute("DamageMax").as_int();
				p->AttackSpeed = item.attribute("AttackSpeed").as_int();
				p->Durability = item.attribute("Durability").as_int();
				p->MagicDurability = item.attribute("MagicDurability").as_int();
				p->MagicPW = item.attribute("MagicPower").as_int();
				p->RequireLevel = item.attribute("ReqLevel").as_int();
				p->RequireStrength = item.attribute("ReqStrength").as_int();
				p->RequireDexterity = item.attribute("ReqDexterity").as_int();
				p->RequireEnergy = item.attribute("ReqEnergy").as_int();
				p->RequireVitality = item.attribute("ReqVitality").as_int();
				p->RequireLeadership = item.attribute("ReqCommand").as_int();
				p->TwoHand = item.attribute("TwoHand").as_int();
				
				(BYTE)p->Durability += (BYTE)p->MagicDurability;
			}

			if (ItemType >= 6 && ItemType <= 11)
			{
				p->Level = item.attribute("DropLevel").as_int();

				if (ItemType == 6)
				{
					p->Defense = item.attribute("Defense").as_int();
					p->SuccessfulBlocking = item.attribute("SuccessfulBlocking").as_int();
				}

				else if (ItemType >= 7 && ItemType <= 9)
				{
					p->Defense = item.attribute("Defense").as_int();
					p->MagicDefense = item.attribute("MagicDefense").as_int();
				}

				else if (ItemType == 10)
				{
					p->Defense = item.attribute("Defense").as_int();
					p->AttackSpeed = item.attribute("AttackSpeed").as_int();
				}

				else if (ItemType == 11)
				{
					p->Defense = item.attribute("Defense").as_int();
					p->WalkSpeed = item.attribute("WalkSpeed").as_int();
				}

				p->Durability = item.attribute("Durability").as_int();
				p->RequireLevel = item.attribute("ReqLevel").as_int();
				p->RequireStrength = item.attribute("ReqStrength").as_int();
				p->RequireDexterity = item.attribute("ReqDexterity").as_int();
				p->RequireEnergy = item.attribute("ReqEnergy").as_int();
				p->RequireVitality = item.attribute("ReqVitality").as_int();
				p->RequireLeadership = item.attribute("ReqCommand").as_int();
			}

			if (ItemType == 13)
			{
				p->Level = item.attribute("ReqLevel").as_int();
				p->Durability = item.attribute("Durability").as_int();
				p->Resistance[0] = item.attribute("IceRes").as_int();
				p->Resistance[1] = item.attribute("PoisonRes").as_int();
				p->Resistance[2] = item.attribute("LightRes").as_int();
				p->Resistance[3] = item.attribute("FireRes").as_int();
				p->Resistance[4] = item.attribute("EarthRes").as_int();
				p->Resistance[5] = item.attribute("WindRes").as_int();
				p->Resistance[6] = item.attribute("WaterRes").as_int();

				p->ResistanceType = -1;

				for (int n = 0; n<MAX_ITEM_SPECIAL_ATTRIBUTE - 1; n++)
				{
					if ((p->Resistance[n]) != 0)
					{
						p->ResistanceType = n;
					}
				}

				p->RequireLevel = p->Level;

				if (ItemIndex == 148 || ItemIndex == 149 || ItemIndex == 150 || ItemIndex == 151)
				{
					p->Durability = 1;
				}
			}

			if (ItemType == 14)
			{
				p->Value = item.attribute("Money").as_int();
				p->Level = item.attribute("DropLevel").as_int();

				p->Durability = 1;

				p->ItemCategory = item.attribute("Type").as_int();
				strcpy(p->ModelPath, item.attribute("ModelPath").as_string());
				strcpy(p->ModelFile, item.attribute("ModelFile").as_string());
				p->Dump = item.attribute("Dump").as_int();
				p->Transaction = item.attribute("Transaction").as_int();
				p->PersonalStore = item.attribute("PersonalStore").as_int();
				p->StoreWarehouse = item.attribute("StoreWarehouse").as_int();
				p->SellToNPC = item.attribute("SellToNPC").as_int();
				p->Repair = item.attribute("Repair").as_int();
				p->ItemKindA = item.attribute("KindA").as_int();
				p->ItemKindB = item.attribute("KindB").as_int();
				p->ItemOverlap = item.attribute("Overlap").as_int();
			}
			
			if (ItemType == 12)
			{
				if (ItemIndex >= 200 && ItemIndex <= 220)
				{
					p->RequireLevel = item.attribute("DropLevel").as_int();
				}

				else
				{
					p->Level = item.attribute("DropLevel").as_int();
				}

				p->Defense = item.attribute("Defense").as_int();
				p->Durability = item.attribute("Durability").as_int();

				if (ItemIndex >= 200 && ItemIndex <= 220)
				{
					p->DamageMin = item.attribute("ReqLevel").as_int();
					p->DamageMax = item.attribute("ReqEnergy").as_int();
				}

				else
				{
					p->RequireLevel = item.attribute("ReqLevel").as_int();
					p->RequireEnergy = item.attribute("ReqEnergy").as_int();
				}

				p->RequireStrength = item.attribute("ReqStrength").as_int();
				p->RequireDexterity = item.attribute("ReqDexterity").as_int();
				p->RequireLeadership = item.attribute("ReqCommand").as_int();
				p->BuyMoney = item.attribute("Money").as_int();
			}

			if (ItemType == 15)
			{
				p->Level = item.attribute("DropLevel").as_int();
				p->RequireLevel = item.attribute("ReqLevel").as_int();
				p->RequireEnergy = item.attribute("ReqEnergy").as_int();
				p->BuyMoney = item.attribute("Money").as_int();

				p->DamageMin = p->Level;
				p->DamageMax = p->Level + p->Level / 2;
			}

			if (ItemType <= 11 || ItemType == 13)
			{
				p->SetAttr = item.attribute("SetAttrib").as_int();
			}

			if (ItemType <= 13 || ItemType == 15)
			{
				p->RequireClass[0] = item.attribute("DarkWizard").as_int();
				p->RequireClass[1] = item.attribute("DarkKnight").as_int();
				p->RequireClass[2] = item.attribute("FairyElf").as_int();
				p->RequireClass[3] = item.attribute("MagicGladiator").as_int();
				p->RequireClass[4] = item.attribute("DarkLord").as_int();
				p->RequireClass[5] = item.attribute("Summoner").as_int();
				p->RequireClass[6] = item.attribute("RageFighter").as_int();
				p->RequireClass[7] = item.attribute("GrowLancer").as_int();
				p->ItemCategory = item.attribute("Type").as_int();
				strcpy(p->ModelPath, item.attribute("ModelPath").as_string());
				strcpy(p->ModelFile, item.attribute("ModelFile").as_string());
				p->Dump = item.attribute("Dump").as_int();
				p->Transaction = item.attribute("Transaction").as_int();
				p->PersonalStore = item.attribute("PersonalStore").as_int();
				p->StoreWarehouse = item.attribute("StoreWarehouse").as_int();
				p->SellToNPC = item.attribute("SellToNPC").as_int();
				p->Repair = item.attribute("Repair").as_int();
				p->ItemKindA = item.attribute("KindA").as_int();
				p->ItemKindB = item.attribute("KindB").as_int();
				p->ItemOverlap = item.attribute("Overlap").as_int();
			}

			CalRepairRate(ItemType, ItemIndex, p);

			p->HaveItemInfo = TRUE;

			iItemCount++;
		}
	}

	g_Log.Add("(OpenItemScript) LoadItem complete. (ItemCount:%d)", iItemCount);
	return TRUE;
}

int zzzItemLevel(int type, int index, int level)
{
	int item_num = (type*MAX_SUBTYPE_ITEMS)+index ;

	if (level < 0 )
	{
		level = 0;
	}
	
	if ( ItemAttribute[item_num].Level == (BYTE)-1 )
	{
		return 0;
	}

	if ( ItemAttribute[item_num].Level == 0 )
	{
		return 0;
	}

	if ( ItemAttribute[item_num].Level < level )
	{
		return 1;
	}

	return 0;
}



int GetLevelItem(int type, int index, int level)
{
	int item_num;
	int itemlevel;

	if ( level < 0 )
	{
		level = 0;
	}

	item_num = (type * MAX_SUBTYPE_ITEMS) + index;

	if (ItemAttribute[item_num].MondownFlag == 0)
	{
		return -1;
	}

	if ( ItemAttribute[item_num].Level == 0xFF )
	{
		return -1;
	}

	if ( ItemAttribute[item_num].Level == 0 )
	{
		return -1;
	}

	if ( type == 14 )
	{
		itemlevel = ItemAttribute[item_num].Level;

		if ( index == 15 )
		{
			return -1;
		}

		if ( itemlevel >= (level-8) )
		{
			if ( itemlevel <= level )
			{
				return 0;
			}
		}

		return -1;
	}

	if ( type == 13 && index == 10  )
	{
		int ilevel;

		if ( (rand()%10) == 0 )
		{
			ilevel = 0;

			if ( level < 0 )
			{
				level = 0;
			}

			ilevel= level/10;

			if ( ilevel > 0 )
			{
				ilevel--;
			}

			if ( ilevel > 5)
			{
				ilevel = 5;
			}

			return ilevel;
		}

		return -1;
	}

	if ( type == 12 && index ==  11 )
	{
		int ilevel;

		if ( (rand()%10) == 0 )
		{
			ilevel = 0;

			if ( level < 0 )
			{
				level = 0;
			}

			ilevel= level/10;

			if ( ilevel > 0 )
			{
				ilevel--;
			}

			if ( ilevel > 6)
			{
				ilevel = 6;
			}

			return ilevel;
		}

		return -1;
	}	

	itemlevel = ItemAttribute[item_num].Level;
	
	if ( itemlevel >= level - 18 && itemlevel <= level)
	{
		if ( type == 15 )
		{
			return 0;
		}

		itemlevel = (level - itemlevel)/3;

		if ( type == 13 )
		{
			if ( index == 8 || index == 9 || index == 12 || index == 13 || index == 20 || index == 21 || index == 22 || index == 23 || index == 24 || index == 25 || index == 26 || index == 27 || index == 28 )
			{
				if ( itemlevel > 4 )
				{
					itemlevel=4;
				}
			}
		}

		return itemlevel;
		
	}
	return -1;
}

int GetSerialItem(int type)
{
	int item_num = type;

	if ( ItemAttribute[item_num].Level == 0xFF )
	{
		return -1;
	}
	if ( ItemAttribute[item_num].Serial  == 0 )
	{
		return 0;
	}
	return 1;
}



int IsItem(int item_num)
{
	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return 0;
	}

	return ItemAttribute[item_num].HaveItemInfo;
}



LPITEM_ATTRIBUTE GetItemAttr(int item_num)
{
	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return NULL;
	}

	return &ItemAttribute[item_num];
}

int GetItemGroup(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return -1;
	}

	return pItemAttribute->ItemCategory;
}

BOOL IsDumpItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->Dump ? TRUE : FALSE;
}

BOOL IsTransactionItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->Transaction;// ? TRUE : FALSE;
}

BOOL IsPersonalStoreItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->PersonalStore ? TRUE : FALSE;
}

BOOL IsStoreWarehouseItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->StoreWarehouse ? TRUE : FALSE;
}

BOOL IsSellToNPCItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->SellToNPC ? TRUE : FALSE;
}

BOOL IsRepairItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->Repair ? TRUE : FALSE;
}

int IsOverlapItem(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return FALSE;
	}

	return pItemAttribute->ItemOverlap;
}

int CItem::GetDetailItemType()
{
	LPITEM_ATTRIBUTE p = &ItemAttribute[this->m_Type];

	if ( this->m_Type < 0 )
	{
		return -1;
	}

	if ( this->m_Type >= ITEMGET(0,32) && this->m_Type <= ITEMGET(0,35) )
	{
		return DETAIL_ITEM_TYPE::ITEM_RAGEFIGHTER_WEAPON;
	}

	if ( this->m_Type >= ITEMGET(0,0) && this->m_Type < ITEMGET(1,0) )
	{
		if ( this->m_TwoHand == TRUE )
		{
			return DETAIL_ITEM_TYPE::ITEM_TWOHAND_SWORD;
		}

		else
		{
			return DETAIL_ITEM_TYPE::ITEM_ONEHAND_SWORD;
		}
	}

	if ( this->m_Type >= ITEMGET(2,0) && this->m_Type < ITEMGET(3,0) )
	{
		if ( this->m_Type == ITEMGET(2,5) )
		{
			return DETAIL_ITEM_TYPE::ITEM_TWOHAND_SWORD;
		}

		if ( this->m_Type == ITEMGET(2,6) )
		{
			return -1;
		}

		if ( p->SkillType == 66 )
		{
			return DETAIL_ITEM_TYPE::ITEM_DARKLORD_SCEPTER;
		}

		else
		{
			return DETAIL_ITEM_TYPE::ITEM_MACE;
		}
	}

	if ( this->m_Type >= ITEMGET(3,0) && this->m_Type < ITEMGET(4,0) )
	{
		return DETAIL_ITEM_TYPE::ITEM_SPEAR;
	}

	if ( this->m_Type >= ITEMGET(4,0) && this->m_Type < ITEMGET(5,0) )
	{
		if ( this->m_TwoHand == TRUE && p->ItemSlot == 1 )
		{
			return DETAIL_ITEM_TYPE::ITEM_CROSSBOW;
		}

		else
		{
			return DETAIL_ITEM_TYPE::ITEM_BOW;
		}
	}

	if ( this->m_Type >= ITEMGET(5,0) && this->m_Type < ITEMGET(6,0) )
	{
		int nSum = 0;

		for(int i=0;i<MAX_TYPE_PLAYER;i++)
		{
			if ( this->m_RequireClass[i] != 0 )
				nSum += i;
		}

		if ( nSum == CLASS_SUMMONER )
		{
			if ( this->m_Type != ITEMGET(5,0) )
			{
				if ( p->ItemSlot == 0 )
				{
					return DETAIL_ITEM_TYPE::ITEM_SUMMONER_STAFF;
				}

				else if ( p->ItemSlot == 1 )
				{
					return DETAIL_ITEM_TYPE::ITEM_SUMMONER_BOOK;
				}
			}
		}

		else
		{
			if ( this->m_TwoHand == TRUE )
			{
				return DETAIL_ITEM_TYPE::ITEM_TWOHAND_STAFF;
			}

			else
			{
				return DETAIL_ITEM_TYPE::ITEM_ONEHAND_STAFF;
			}
		}
	}

	if ( this->m_Type >= ITEMGET(6,0) && this->m_Type < ITEMGET(7,0) )
	{
		return DETAIL_ITEM_TYPE::ITEM_SHIELD;
	}

	return -1;
}

void CItem::MakeSocketSlot(BYTE SlotCount)
{
	if (g_SocketOptionSystem.IsEnableSocketItem(this) == FALSE)
	{
		return;
	}

	for (int i = 0; i < SlotCount; i++)
	{
		this->m_SocketOption[i] = 0xFE;
	}

	this->m_BonusSocketOption = 0xFF;
}

void LoadResetItemList(LPSTR szFile)
{
	for(int i = 0; i < 1000; i++ )
	{
		gObjResetItem[i].itemIndex = 0;
		gObjResetItem[i].iResets = 0;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", szFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("Reset");
	int iCount = 0;

	for (pugi::xml_node item = main.child("Item"); item; item = item.next_sibling())
	{
		int iCat = item.attribute("Cat").as_int();
		int iIndex = item.attribute("Index").as_int();
		int Resets = item.attribute("ReqReset").as_int();

		gObjResetItem[iCount].itemIndex = ITEMGET(iCat,iIndex);
		gObjResetItem[iCount].iResets = Resets;
		iCount++;
	}
	
	g_Log.AddC(TColor::DarkOrange, "[%s]::LoadFile() -> Success!", szFile);
}

bool CheckCanWearResetItem(int aIndex, int itemid)
{
	if (!ObjectMaxRange(aIndex))
		return false;

	LPOBJ lpObj = &gObj[aIndex];

	for (int i = 0; i < 1000; i++)
	{
		if (gObjResetItem[i].itemIndex == itemid)
		{
			if (lpObj->m_PlayerData->m_iResets < gObjResetItem[i].iResets)
			{
				MsgOutput(aIndex, Lang.GetText(0,599), gObjResetItem[i].iResets);
				return false;
			}
			else
				return true;
			
		}
	}
	return true;
}

BYTE CalcExcOption(int MaxOpt)
{
	int OptionCount = rand()%6; // Max 5 options;
	BYTE ExcOption = 0;

	if(OptionCount > MaxOpt)
	{
		OptionCount = MaxOpt;
	}

	while ( OptionCount != 0 )
	{
		int iRandOp = rand()%6;

		BYTE btOption = 1 << iRandOp;

		if ( ( ExcOption & btOption ) == 0 )
		{
			ExcOption |= btOption;
		}

		OptionCount--;
	}

	return ExcOption;
}

BOOL IsJumpingEventItem(int iItemCode)
{
	if (GetItemGroup(iItemCode) == ITEMTYPE_EVENT)
	{
		return TRUE;
	}

	else
	{
		return FALSE;
	}
}

int IsExpensiveItem(CItem * item)
{
	if (item->m_Type >= ITEMGET(0,0) && item->m_Type < ITEMGET(12,0))
	{
		if (item->m_Level > 6)
		{
			return 1;
		}

		if ( (item->m_NewOption & 0x3F) != 0)
		{
			return 1;
		}

		if (item->m_SetOption != 0)
		{
			return 1;
		}
	}

	else if (item->m_Type == ITEMGET(13,20) && item->m_Level > 0)
	{
		return 1;
	}

	else if (IsDumpItem(item->m_Type) == FALSE)
	{
		return 1;
	}

	return 0;
}

int GetItemKindA(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return -1;
	}

	return pItemAttribute->ItemKindA;
}

int GetItemKindB(int item_num)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item_num);

	if (pItemAttribute == NULL)
	{
		return -1;
	}

	return pItemAttribute->ItemKindB;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

