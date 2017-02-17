//////////////////////////////////////////////////////////////////////
// ItemOptionTypeMng.cpp
#include "StdAfx.h"
#include "ItemOptionTypeMng.h"
#include "user.h"
#include "zzzitem.h"
#include "TLog.h"

CItemOptionTypeMng g_ItemOptionTypeMng;

void CItemOptionTypeMng::Initialize()
{
	for (int iClearIndex = 0; iClearIndex < MAX_EXCELLENT_COMMON_OPTIONS; iClearIndex++)
	{
		this->m_CommonExtOptionType[iClearIndex].Clear();
	}

	for (int iClearIndex = 0; iClearIndex < MAX_EXCELLENT_WING_OPTIONS; iClearIndex++)
	{
		this->m_WingExtOptionType[iClearIndex].Clear();
	}
}

bool CItemOptionTypeMng::LoadScript(char *pchFileName)
{
	this->Initialize();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", pchFileName, res.description());
		return false;
	}

	pugi::xml_node main = file.child("ExcellentOptions");
	pugi::xml_node common = main.child("Common");
	int OptionCount = 0;

	for (pugi::xml_node option = common.child("Option"); option; option = option.next_sibling())
	{
		this->m_CommonExtOptionType[OptionCount].OptionID = option.attribute("ID").as_int();
		this->m_CommonExtOptionType[OptionCount].ItemKindA_1 = option.attribute("ItemKindA_1").as_int();
		this->m_CommonExtOptionType[OptionCount].ItemKindA_2 = option.attribute("ItemKindA_2").as_int();
		this->m_CommonExtOptionType[OptionCount].ItemKindA_3 = option.attribute("ItemKindA_3").as_int();
		this->m_CommonExtOptionType[OptionCount].OptionNumber = option.attribute("Number").as_int();
		this->m_CommonExtOptionType[OptionCount].OptionRate = option.attribute("Rate").as_int();
		this->m_CommonExtOptionType[OptionCount].OptionValue = option.attribute("Value").as_int();
		strcpy(this->m_CommonExtOptionType[OptionCount].OptionName, option.attribute("Name").as_string());

		OptionCount++;
	}

	pugi::xml_node wings = main.child("Wings");
	OptionCount = 0;

	for (pugi::xml_node option = wings.child("Option"); option; option = option.next_sibling())
	{
		this->m_WingExtOptionType[OptionCount].OptionID = option.attribute("ID").as_int();
		this->m_WingExtOptionType[OptionCount].ItemKindA = option.attribute("ItemKindA").as_int();
		this->m_WingExtOptionType[OptionCount].ItemKindB = option.attribute("ItemKindB").as_int();
		this->m_WingExtOptionType[OptionCount].OptionNumber = option.attribute("Number").as_int();
		this->m_WingExtOptionType[OptionCount].OptionValue = option.attribute("Value").as_int();
		strcpy(this->m_WingExtOptionType[OptionCount].OptionName, option.attribute("Name").as_string());

		OptionCount++;
	}

	pugi::xml_node optionrate = main.child("OptionDropRate");

	pugi::xml_node commonrate = optionrate.child("Common");
	this->m_ExcellentOptionRate.m_CommonOptionRate[0] = commonrate.attribute("One").as_int();
	this->m_ExcellentOptionRate.m_CommonOptionRate[1] = commonrate.attribute("Two").as_int();
	this->m_ExcellentOptionRate.m_CommonOptionRate[2] = commonrate.attribute("Three").as_int();
	this->m_ExcellentOptionRate.m_CommonOptionRate[3] = commonrate.attribute("Four").as_int();
	this->m_ExcellentOptionRate.m_CommonOptionRate[4] = commonrate.attribute("Five").as_int();
	this->m_ExcellentOptionRate.m_CommonOptionRate[5] = commonrate.attribute("Six").as_int();
}

void CItemOptionTypeMng::Initialize_AccessoryItemOption()
{
	for (int iClearIndex = 0; iClearIndex < 6; iClearIndex++)
	{
		this->m_AccessoryItemOptionValue[iClearIndex].Clear();
	}
}

bool CItemOptionTypeMng::LoadAccessoryItemOptionScript(char *pchFileName)
{
	ACCESSORY_ITEM_OPTION_VALUE AccessoryItemOptionValue; 
	int iValueCount = 0; 

	this->Initialize_AccessoryItemOption();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ItemOptionTypeMng] AccessoryItem Option Script file Load failed. (%s) (%s)", pchFileName, res.description());
		return false;
	}

	pugi::xml_node main = file.child("AccessoryItems");

	for (pugi::xml_node item = main.child("Item"); item; item = item.next_sibling())
	{
		memset((char *)&AccessoryItemOptionValue, 0, sizeof(AccessoryItemOptionValue));
			
		AccessoryItemOptionValue.ItemType = item.attribute("Cat").as_int();
		AccessoryItemOptionValue.ItemIndex = item.attribute("Index").as_int();

		for (int i = 0; i < 16; i++)
		{
			char szTemp[256];
			sprintf(szTemp, "OptValue%d", i);

			AccessoryItemOptionValue.LevelValue[i] = item.attribute(szTemp).as_int();
		}

		memcpy(&this->m_AccessoryItemOptionValue[iValueCount], &AccessoryItemOptionValue, sizeof(AccessoryItemOptionValue));
		iValueCount++;
	}

	return true;
}

int CItemOptionTypeMng::CommonExcOptionRand(int ItemKindA)
{
	BYTE btExcOptions[6];
	DWORD dwExcOptionRate[6];
	int iExcOption = 0;

	for (int i = 0; i < 20; i++)
	{
		if (this->m_CommonExtOptionType[i].ItemKindA_1 == ItemKindA ||
			this->m_CommonExtOptionType[i].ItemKindA_2 == ItemKindA ||
			this->m_CommonExtOptionType[i].ItemKindA_3 == ItemKindA)
		{
			btExcOptions[iExcOption] = this->m_CommonExtOptionType[i].OptionNumber;
			dwExcOptionRate[iExcOption] = this->m_CommonExtOptionType[i].OptionRate;
			iExcOption++;
		}
	}

	int OneOp = this->m_ExcellentOptionRate.m_CommonOptionRate[0];
	int TwoOp = this->m_ExcellentOptionRate.m_CommonOptionRate[1];
	int ThreeOp = this->m_ExcellentOptionRate.m_CommonOptionRate[2];
	int FourOp = this->m_ExcellentOptionRate.m_CommonOptionRate[3];
	int FiveOp = this->m_ExcellentOptionRate.m_CommonOptionRate[4];
	int SixOp = this->m_ExcellentOptionRate.m_CommonOptionRate[5];
	int iRand = rand() % 10000;
	int iOptionCount = 0;

	if (iRand >= OneOp)
	{
		if (iRand < OneOp || iRand >= OneOp + TwoOp)
		{
			if (iRand < OneOp + TwoOp || iRand >= OneOp + TwoOp + ThreeOp)
			{
				if (iRand < OneOp + TwoOp + ThreeOp || iRand >= OneOp + TwoOp + ThreeOp + FourOp)
				{
					if (iRand < OneOp + TwoOp + ThreeOp + FourOp || iRand >= OneOp + TwoOp + ThreeOp + FourOp + FiveOp)
					{
						if (iRand >= OneOp + TwoOp + ThreeOp + FourOp + FiveOp || iRand < OneOp + TwoOp + ThreeOp + FourOp + FiveOp + SixOp)
						{
							iOptionCount = 6;
						}
					}

					else
					{
						iOptionCount = 5;
					}
				}

				else
				{
					iOptionCount = 4;
				}
			}

			else
			{
				iOptionCount = 3;
			}
		}

		else
		{
			iOptionCount = 2;
		}
	}

	else
	{
		iOptionCount = 1;
	}

	if (iOptionCount == 0)
	{
		return 0;
	}

	if (iOptionCount > iExcOption)
	{
		iOptionCount = iExcOption;
	}

	BYTE ExcOption = 0;

	while (iOptionCount != 0)
	{
		BYTE btOptionIndex = rand() % iExcOption;
		BYTE Option = btExcOptions[btOptionIndex];
		DWORD dwRate = dwExcOptionRate[btOptionIndex];

		if ((ExcOption & Option) != Option && (rand() % 10000) < dwRate)
		{
			ExcOption |= Option;
			iOptionCount--;
		}
	}

	return ExcOption;
}

void CItemOptionTypeMng::CalcExcOptionEffect(LPOBJ lpObj)
{
	for (int i = 0; i < INVETORY_WEAR_SIZE; i++)
	{
		if (lpObj->pInventory[i].IsItem() == FALSE)
		{
			continue;
		}

		if (lpObj->pInventory[i].m_Durability == 0.0f)
		{
			continue;
		}

		if (lpObj->pInventory[i].m_IsValidItem == false)
		{
			continue;
		}

		if (lpObj->pInventory[i].IsExtItem() == FALSE)
		{
			continue;
		}

		LPITEM_ATTRIBUTE lpAttr = GetItemAttr(lpObj->pInventory[i].m_Type);

		if (lpAttr == NULL)
		{
			continue;
		}

		for (int j = 0; j < MAX_EXCELLENT_COMMON_OPTIONS; j++)
		{
			if (this->m_CommonExtOptionType[j].OptionID != -1)
			{
				if(this->m_CommonExtOptionType[j].ItemKindA_1 == lpAttr->ItemKindA ||
					this->m_CommonExtOptionType[j].ItemKindA_2 == lpAttr->ItemKindA ||
					this->m_CommonExtOptionType[j].ItemKindA_3 == lpAttr->ItemKindA)
				{
					if ( (lpObj->pInventory[i].m_NewOption & this->m_CommonExtOptionType[j].OptionNumber) == this->m_CommonExtOptionType[j].OptionNumber)
					{
						this->SetExcOptionEffect(lpObj, this->m_CommonExtOptionType[j].OptionID, lpAttr, i);
					}
				}
			}
		}
	}
}

void CItemOptionTypeMng::SetExcOptionEffect(OBJECTSTRUCT *lpObj, int ExcOptionID, LPITEM_ATTRIBUTE lpItem, BYTE btItemPos)
{
	COMMON_EXT_OPTION_TYPE * lpOption = this->GetCommonExcOption(ExcOptionID);

	if (lpOption == NULL)
	{
		return;
	}

	switch ( ExcOptionID )
	{
		case OPTION_INC_MANA_MONSTER_DIE:
			lpObj->MonsterDieGetMana++;
			break;
		case OPTION_INC_LIFE_MONSTER_DIE:
			lpObj->MonsterDieGetLife++;
			break;
		case OPTION_INC_ATTACK_SPEED:
			lpObj->m_AttackSpeed += lpOption->OptionValue;
			lpObj->m_MagicSpeed += lpOption->OptionValue;
			break;
		case OPTION_INC_ATTACK_PERCENT:
			if ( ( lpObj->pInventory[btItemPos].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[btItemPos].m_Type < ITEMGET(6, 0) ) ||
					lpObj->pInventory[btItemPos].m_Type == ITEMGET(13,12) ||
					lpObj->pInventory[btItemPos].m_Type == ITEMGET(13,25) ||
					lpObj->pInventory[btItemPos].m_Type == ITEMGET(13,27) )
			{
				lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * lpOption->OptionValue / 100;
				lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * lpOption->OptionValue / 100;
			}

			else
			{
				if ( btItemPos == 0 || btItemPos == 9 )
				{
					lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * lpOption->OptionValue / 100;
					lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * lpOption->OptionValue / 100;
				}
							
				if ( btItemPos == 1 || btItemPos == 9 )
				{
					lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * lpOption->OptionValue / 100;
					lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * lpOption->OptionValue / 100;
				}
			}
			break;
		case OPTION_INC_ATTACK_LEVEL:
			if ( ( lpObj->pInventory[btItemPos].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[btItemPos].m_Type < ITEMGET(6, 0) ) ||
					lpObj->pInventory[btItemPos].m_Type == ITEMGET(13,12) ||
					lpObj->pInventory[btItemPos].m_Type == ITEMGET(13,25) ||
					lpObj->pInventory[btItemPos].m_Type == ITEMGET(13,27) )
			{
				lpObj->m_MagicDamageMax += (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / lpOption->OptionValue;
				lpObj->m_MagicDamageMin += (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / lpOption->OptionValue;
			}

			else
			{
				if ( btItemPos == 0 || btItemPos == 9 )
				{
					lpObj->m_AttackDamageMinRight += (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / lpOption->OptionValue;
					lpObj->m_AttackDamageMaxRight += (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / lpOption->OptionValue;
				}
							
				if ( btItemPos == 1 || btItemPos == 9 )
				{
					lpObj->m_AttackDamageMinLeft += (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / lpOption->OptionValue;
					lpObj->m_AttackDamageMaxLeft += (lpObj->Level+lpObj->m_PlayerData->MasterLevel) / lpOption->OptionValue;
				}
			}
			break;
		case OPTION_INC_EXCELLENT_DAMAGE:
			lpObj->m_ExcelentDamage += lpOption->OptionValue;
			break;
		case OPTION_INC_ZEN_MONSTER_DIE:
			lpObj->MonsterDieGetMoney += lpOption->OptionValue;
			break;
		case OPTION_INC_DEFENSE_RATE:
			lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking * lpOption->OptionValue / 100;
			break;
		case OPTION_INC_DAMAGE_REFLECT:
			lpObj->DamageReflect += lpOption->OptionValue;
			break;
		case OPTION_DEC_VICTIM_DAMAGE:
			lpObj->DamageMinus += lpOption->OptionValue;
			break;
		case OPTION_INC_MAXIMUM_MANA:
			lpObj->AddMana += (lpObj->MaxMana * lpOption->OptionValue / 100.0f);
			break;
		case OPTION_INC_MAXIMUM_LIFE:
			lpObj->AddLife += (lpObj->MaxLife * lpOption->OptionValue / 100.0f);
			break;
	}
}

COMMON_EXT_OPTION_TYPE * CItemOptionTypeMng::GetCommonExcOption(int ExcOptionID)
{
	for (int i = 0; i < MAX_EXCELLENT_COMMON_OPTIONS; i++)
	{
		if (this->m_CommonExtOptionType[i].OptionID == ExcOptionID)
		{
			return &this->m_CommonExtOptionType[i];
		}
	}

	return NULL;
}

void CItemOptionTypeMng::CalcWingOptionEffect(LPOBJ lpObj)
{
	if (lpObj->pInventory[7].IsItem() == FALSE)
	{
		return;
	}

	if (lpObj->pInventory[7].m_Durability == 0.0f)
	{
		return;
	}

	if (lpObj->pInventory[7].m_IsValidItem == false)
	{
		return;
	}

	if (lpObj->pInventory[7].IsExtItem() == FALSE)
	{
		return;
	}

	LPITEM_ATTRIBUTE lpAttr = GetItemAttr(lpObj->pInventory[7].m_Type);

	if (lpAttr == NULL)
	{
		return;
	}

	for (int i = 0; i < MAX_EXCELLENT_WING_OPTIONS; i++)
	{
		if (this->m_WingExtOptionType[i].OptionID != -1)
		{
			if (this->m_WingExtOptionType[i].ItemKindA == lpAttr->ItemKindA &&
				this->m_WingExtOptionType[i].ItemKindB == lpAttr->ItemKindB)
			{
				if ( (lpObj->pInventory[7].m_NewOption & this->m_WingExtOptionType[i].OptionNumber) == this->m_WingExtOptionType[i].OptionNumber)
				{
					this->SetWingOptionEffect(lpObj, this->m_WingExtOptionType[i].OptionID, lpAttr);
				}
			}
		}
	}
}

void CItemOptionTypeMng::SetWingOptionEffect(OBJECTSTRUCT *lpObj, int ExcOptionID, LPITEM_ATTRIBUTE lpItem)
{
	WING_EXT_OPTION_TYPE * lpOption = this->GetWingOption(ExcOptionID);

	if ( lpOption == NULL )
	{
		return;
	}

	switch ( ExcOptionID )
	{
		case 0:
		case 9:
		case 13:
			lpObj->AddLife += (lpOption->OptionValue + (lpObj->pInventory[7].m_Level * 5));
			break;
		case 1:
		case 10:
		case 14:
			lpObj->AddMana += (lpOption->OptionValue + (lpObj->pInventory[7].m_Level * 5));
			break;
		case 2:
		case 5:
		case 11:
		case 15:
		case 16:
		case 18:
		case 23:
			lpObj->m_PlayerData->m_WingExcOption.iWingOpIgnoreEnemyDefense = lpOption->OptionValue;
			break;
		case 3:
			lpObj->AddBP += lpOption->OptionValue;
			break;
		case 4:
			lpObj->m_AttackSpeed += lpOption->OptionValue;
			lpObj->m_MagicSpeed += lpOption->OptionValue;
			break;
		case 6:
		case 19:
			lpObj->m_PlayerData->m_WingExcOption.iWingOpReturnEnemyDamage = lpOption->OptionValue;
			break;
		case 7:
		case 17:
		case 20:
		case 24:
			lpObj->m_PlayerData->m_WingExcOption.iWingOpRecoveryHP = lpOption->OptionValue;
			break;
		case 8:
		case 21:
			lpObj->m_PlayerData->m_WingExcOption.iWingOpRecoveryMana = lpOption->OptionValue;
			break;
		case 12:
			lpObj->AddLeadership += (lpOption->OptionValue + (lpObj->pInventory[7].m_Level * 5));
			break;
		case 22:
			lpObj->m_PlayerData->m_WingExcOption.iWingOpAddDoubleDamage = lpOption->OptionValue;
			break;
	}
}

WING_EXT_OPTION_TYPE * CItemOptionTypeMng::GetWingOption(int ExcOptionID)
{
	for (int i = 0; i < MAX_EXCELLENT_WING_OPTIONS; i++)
	{
		if (this->m_WingExtOptionType[i].OptionID == ExcOptionID)
		{
			return &this->m_WingExtOptionType[i];
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

