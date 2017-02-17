//////////////////////////////////////////////////////////////////////
// BotSystem.cpp
#include "StdAfx.h"
#include "BotSystem.h"
#include "user.h"
#include "protocol.h"
#include "TLog.h"
#include "GameMain.h"
#include "ObjCalCharacter.h"
#include "configread.h"
#include "BuffEffectSlot.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "VipSys.h"

CBotSystem g_BotSystem;

CBotSystem::CBotSystem()
{

}

CBotSystem::~CBotSystem()
{

}

void CBotSystem::Init()
{
	this->iCount = 0;
	this->LoadData(g_ConfigRead.GetPath("IGC_BotList.xml"));
}

void CBotSystem::LoadData(LPSTR szFile)
{
	this->m_BotData.clear();
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(szFile);

	if( result.status != pugi::status_ok )
	{
		g_Log.Add("[%s]::LoadFile() -> Failed!",szFile);
		return;
	}

	pugi::xml_node botsystem = doc.child("BotSystem");
	pugi::xml_node list = botsystem.child("List");
	for( pugi::xml_node bots = list.child("Bot"); bots; bots = bots.next_sibling())
	{
		_sBOT_SETTINGS sBot;
		sBot.wID = bots.attribute("Id").as_int();
		sBot.btType = bots.attribute("Type").as_int();
		sBot.btClass = bots.attribute("Class").as_int();
		sBot.btMap = bots.attribute("Map").as_int();
		sBot.btX = bots.attribute("PosX").as_int();
		sBot.btY = bots.attribute("PosY").as_int();
		sBot.btDir = bots.attribute("Dir").as_int();
		sBot.btVipType = bots.attribute("VipType").as_int();
		sBot.btCoinType = bots.attribute("CoinType").as_int();
		sBot.iCoinValue = bots.attribute("Money").as_int();
		sBot.wStrength = bots.attribute("Strength").as_int();
		sBot.wDexterity = bots.attribute("Dexterity").as_int();
		sBot.wVitality = bots.attribute("Vitality").as_int();
		sBot.wEnergy = bots.attribute("Energy").as_int();
		sBot.sName = bots.attribute("Name").as_string();
		sBot.sPathActionFile = bots.attribute("File").as_string();
		sBot.bEnabled = bots.attribute("Enable").as_bool();
		this->m_BotData.insert(std::pair<int, _sBOT_SETTINGS>(sBot.wID, sBot));
		this->iCount++;
	}

	for( pugi::xml_node botgear = botsystem.child("BotGear"); botgear; botgear = botgear.next_sibling())
	{
		WORD wBotId = botgear.attribute("Id").as_int();
		for( pugi::xml_node gear = botgear.child("Gear"); gear; gear = gear.next_sibling())
		{
			int Slot = gear.attribute("Slot").as_int();
			_sBOT_INVENTORY_WEAR_ITEMS *sItems = &this->m_BotData[wBotId].m_Items[Slot];
			sItems->wItemID = ITEMGET(gear.attribute("ItemCat").as_int(),gear.attribute("ItemIndex").as_int());
			sItems->btItemLv = gear.attribute("ItemLevel").as_int();
		}
	}
	this->SetAllBots();

}
bool CBotSystem::LoadBotSpecializationData(int aIndex, LPSTR szFile, BYTE type)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(szFile);

	if( result.status != pugi::status_ok )
	{
		MessageBoxA(0,"Failed to load bot specialization. Check your configs","ERROR",0);
		return false;
	}
	LPOBJ lpObj = &gObj[aIndex];
	int iBuffCounter = 0;
	int wBotIndex = lpObj->m_PlayerData->wBotIndex;
	switch(type)
	{
		case BOT_BUFFER:
			{
				pugi::xml_node BuffList = doc.child("BuffList");
				for( pugi::xml_node buffs = BuffList.child("Buff"); buffs; buffs = buffs.next_sibling())
				{
					lpObj->m_BotBuffs[iBuffCounter].wBuffId = buffs.attribute("Id").as_int();
					lpObj->m_BotBuffs[iBuffCounter].wDuration = buffs.attribute("Duration").as_int();
					lpObj->m_BotBuffs[iBuffCounter].iEffect = buffs.attribute("EffectValue").as_int();
					lpObj->m_BotBuffs[iBuffCounter].wEffectType = buffs.attribute("EffectType").as_int();
					//gObjMagicAdd(lpObj,lpObj->m_BotBuffs[iBuffCounter].wBuffId ,0);
					iBuffCounter++;
					
				}

			}
			break;
		case BOT_TRADE:
			{
				sBOT_CRAFTING sCrafting;
			//	sCrafting.m_Requirements.clear();
				pugi::xml_node ItemList = doc.child("ItemList");
				for( pugi::xml_node craft = ItemList.child("Craft"); craft; craft = craft.next_sibling())
				{
					sCrafting.m_Requirements.clear();
					int iIdx = craft.attribute("id").as_int();
					WORD wItemCat = craft.attribute("CraftCat").as_int();
					WORD wItemIdx = craft.attribute("CraftId").as_int();
					sCrafting.RewardCount = craft.attribute("Count").as_int();
					sCrafting.iSuccessRate = craft.attribute("SuccessRate").as_int();
					sCrafting.m_Reward.wItemId = ITEMGET(wItemCat,wItemIdx);
					sCrafting.m_Reward.btLevel = craft.attribute("level").as_int();
					sCrafting.m_Reward.btLuck = craft.attribute("luck").as_int();
					sCrafting.m_Reward.btSkill = craft.attribute("skill").as_int();
					sCrafting.m_Reward.btOpt = craft.attribute("opt").as_int();
					sCrafting.m_Reward.btExc = craft.attribute("exc").as_int();
					sCrafting.m_Reward.btSetItem = craft.attribute("SetItem").as_int();
					sCrafting.iReqCount = 0;
					sCrafting.iTotalReqCount = 0;

					for( pugi::xml_node requirements = craft.child("Requirement"); requirements; requirements = requirements.next_sibling())
					{
						s_BOT_CRAFTING_ITEM_STRUCT sReq;
						int iReqIdx = requirements.attribute("id").as_int();
						wItemCat = requirements.attribute("ItemCat").as_int();
						wItemIdx = requirements.attribute("ItemId").as_int();

						sReq.wItemId = ITEMGET(wItemCat,wItemIdx);
						sReq.btLevel = requirements.attribute("level").as_int();
						sReq.btLuck = requirements.attribute("luck").as_int();
						sReq.btSkill = requirements.attribute("skill").as_int();
						sReq.btOpt = requirements.attribute("opt").as_int();
						sReq.btExc = requirements.attribute("exc").as_int();
						sReq.btSetItem = requirements.attribute("SetItem").as_int();
						sReq.btCount =  requirements.attribute("reqcount").as_int();
						sCrafting.iReqCount++;
						sCrafting.iTotalReqCount += sReq.btCount;
						sCrafting.m_Requirements.insert(std::pair<int, s_BOT_CRAFTING_ITEM_STRUCT>(iReqIdx, sReq));
					}
					this->m_BotData[wBotIndex].m_Crafting.insert(std::pair<int,sBOT_CRAFTING>(iIdx,sCrafting));
				}
			}
			break;
		case BOT_SHOP:
			{
				sBOT_PSHOP sBotShop;
				ZeroMemory(&sBotShop,sizeof(sBotShop));
				
				int index = 0;
				pugi::xml_node xmlShopItemList = doc.child("ShopItemList");
				sBotShop.iCoinType = xmlShopItemList.attribute("CoinType").as_int();
				//sprintf(sBotShop.szBotShopName,"%s",doc.attribute("ShopTitle").as_string());
				sBotShop.szBotShopName = xmlShopItemList.attribute("ShopTitle").as_string();
				for( pugi::xml_node xmlItem = xmlShopItemList.child("Item"); xmlItem; xmlItem = xmlItem.next_sibling())
				{
					s_BOT_SHOP_ITEM sItem;
					WORD wItemCat = xmlItem.attribute("ItemCat").as_int();
					WORD wItemIdx = xmlItem.attribute("ItemIndex").as_int();
					sItem.wItemId = ITEMGET(wItemCat,wItemIdx);
					sItem.iValue = xmlItem.attribute("Value").as_int();
					sItem.btDur = xmlItem.attribute("Dur").as_int();
					sItem.btOpt = xmlItem.attribute("AddOption").as_int();
					sItem.btLuck = xmlItem.attribute("Luck").as_int();
					sItem.btSkill = xmlItem.attribute("Skill").as_int();
					sItem.btExc = xmlItem.attribute("Excellent").as_int();
					sItem.btAnc = xmlItem.attribute("Ancient").as_int();
					sItem.btSocket[0] = xmlItem.attribute("Socket1").as_int();
					sItem.btSocket[1] = xmlItem.attribute("Socket2").as_int();
					sItem.btSocket[2] = xmlItem.attribute("Socket3").as_int();
					sItem.btSocket[3] = xmlItem.attribute("Socket4").as_int();
					sItem.btSocket[4] = xmlItem.attribute("Socket5").as_int();
					sBotShop.pItems[index] = sItem;
					index++;
				}
				sBotShop.iItemCount = index;
				this->m_BotData[wBotIndex].m_Shop = sBotShop;
				this->StoreAddItems(wBotIndex);
				
			}
			break;
		default:
			g_Log.AddC(TColor::Red,"[UNKNOWN SPECIALIZATION] %s %d",this->m_BotData[aIndex].sName.c_str(),type);
			break;
	}
}

void CBotSystem::SetAllBots()
{
	g_Log.AddC(TColor::Aqua,"--- START OF SetAllBots() ---");

	DWORD dwStartTime = GetTickCount();
	int iCount = this->m_BotData.size();
	for(std::map<int,_sBOT_SETTINGS>::iterator It = this->m_BotData.begin(); It != this->m_BotData.end(); It++)
	{
		_sBOT_SETTINGS  * pBot = &It->second;

		if(!pBot->bEnabled)
		{
			continue;
		}

		if (g_MapServerManager.CheckMapCanMove(pBot->btMap) == FALSE) // check if map exists on curr gameserver
		{
			continue;
		}

		int iIndex = this->AddBot(*pBot);

		if (!ObjectMaxRange(iIndex))
		{
			continue;
		}

		pBot->aIndex = iIndex;
		this->LoadBotSpecializationData(pBot->aIndex,g_ConfigRead.GetPath((char*)pBot->sPathActionFile.c_str()),pBot->btType);
		g_Log.AddC(TColor::Aqua,"[%d] BOT [%s] Type: [%d] Added Map [%d][%d][%d]",pBot->wID,pBot->sName,pBot->btType,pBot->btMap,pBot->btX,pBot->btY);
	}

	DWORD dwLoadTime = GetTickCount() - dwStartTime;

	g_Log.AddC(TColor::Aqua,"-- END OF SetAllBots(). Loaded in %d msec ---",dwLoadTime);

}

int CBotSystem::AddBot(_sBOT_SETTINGS pBot)
{
//--- declare
	int aIndex = gObjAddMonster(pBot.btMap);
	LPOBJ lpBotObj = &gObj[aIndex];
//--- valide
	if(aIndex == -1)
	{
		g_Log.AddC(TColor::Red,"[%d][%s] Failed to add bot",pBot.wID,pBot.sName);
		return aIndex;
	}

	if(pBot.sName.size() > 10)
	{
		MessageBoxA(0,"Max Name length is 10 characters","Bot System",0);
		return aIndex;
	}
//--- assign
	strcpy(lpBotObj->Name,pBot.sName.c_str());
	lpBotObj->MapNumber = pBot.btMap;
	lpBotObj->X = pBot.btX;
	lpBotObj->Y = pBot.btY;
	lpBotObj->m_PlayerData = new USER_DATA();
	lpBotObj->ConnectCheckTime = GetTickCount();
	lpBotObj->TargetNumber = -1;
	lpBotObj->TargetNpcNumber = -1;
	lpBotObj->m_RecallMon = -1;
	lpBotObj->Connected = PLAYER_PLAYING;
	lpBotObj->m_PlayerData->ISBOT = true;
	lpBotObj->Live = TRUE;
	lpBotObj->DieRegen = 0;
	lpBotObj->m_State = 1;
	lpBotObj->Type = OBJ_USER;
	lpBotObj->Class = pBot.btClass;	
	lpBotObj->BufferIndex = pBot.wID;
	lpBotObj->m_PlayerData->bt_BotType = 0;
	lpBotObj->m_PlayerData->lpGuild = NULL;
	lpBotObj->TX = lpBotObj->X;
	lpBotObj->TY = lpBotObj->Y;
	lpBotObj->m_OldX = lpBotObj->X;
	lpBotObj->m_OldY = lpBotObj->Y;
	lpBotObj->Dir = pBot.btDir;
	lpBotObj->StartX = lpBotObj->X;
	lpBotObj->StartY = lpBotObj->Y;
	lpBotObj->Level = 400;	
	lpBotObj->m_PlayerData->Energy = pBot.wEnergy;
	lpBotObj->m_PlayerData->Dexterity = pBot.wDexterity;
	lpBotObj->m_PlayerData->Strength = pBot.wStrength;
	lpBotObj->m_PlayerData->Vitality = pBot.wVitality;
	lpBotObj->m_PlayerData->m_MPSkillOpt.Clear();
	lpBotObj->Leadership = 0;
	lpBotObj->m_AttackSpeed = 30;
	lpBotObj->m_PlayerData->m_bSecurityCheck = true;

	lpBotObj->MaxLife = DCInfo.DefClass[lpBotObj->Class].Life + ((lpBotObj->Level+lpBotObj->m_PlayerData->MasterLevel) - 1) * DCInfo.DefClass[lpBotObj->Class].LevelLife  + ((lpBotObj->m_PlayerData->Vitality - DCInfo.DefClass[lpBotObj->Class].Vitality ) * DCInfo.DefClass[lpBotObj->Class].VitalityToLife);
	lpBotObj->MaxMana = DCInfo.DefClass[lpBotObj->Class].Mana + ((lpBotObj->Level+lpBotObj->m_PlayerData->MasterLevel) - 1) * DCInfo.DefClass[lpBotObj->Class].LevelMana  + ((lpBotObj->m_PlayerData->Energy - DCInfo.DefClass[lpBotObj->Class].Energy ) * DCInfo.DefClass[lpBotObj->Class].EnergyToMana);

	lpBotObj->Life = lpBotObj->MaxLife;
	lpBotObj->Mana = lpBotObj->MaxMana;
	lpBotObj->m_PlayerData->wBotIndex = pBot.wID;
	if(pBot.btType == BOT_TRADE)
	{
		lpBotObj->Trade = new CItem[TRADE_BOX_SIZE];
		lpBotObj->TradeMap = new BYTE[TRADE_BOX_MAP_SIZE];
	}
	if(pBot.btType == BOT_SHOP)
	{
		lpBotObj->pInventoryMap = new BYTE[INVENTORY_SIZE];
		lpBotObj->Inventory1 = new CItem[INVENTORY_SIZE];
		for(int n=0;n<INVENTORY_SIZE;n++)
		{
			lpBotObj->Inventory1[n].Clear();
		}
		memset(&lpBotObj->pInventoryMap[0], (BYTE)-1,INVENTORY_SIZE);	//
	}
	this->MakePreviewCharSet(aIndex,pBot.m_Items);
	return aIndex;
}

void CBotSystem::MakePreviewCharSet(int aIndex, _sBOT_INVENTORY_WEAR_ITEMS * pInv)
{
	if ( !ObjectMaxRange(aIndex ))
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return;
	}
	
	BYTE index;
	int itemindex;
	LPOBJ lpObj = &gObj[aIndex];
	
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	if (g_ConfigRead.SeasonX == false)
	{
		lpObj->CharSet[CS_CLASS] = CS_SET_CLASS(lpObj->Class);

		if (lpObj->m_PlayerData->ChangeUP == 1)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP;
		}

		if (lpObj->m_PlayerData->ChangeUP == 2)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP;
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP2;
		}
	}

	else
	{
		lpObj->CharSet[CS_CLASS] = CS_SET_CLASS_SX(lpObj->Class);

		if (lpObj->m_PlayerData->ChangeUP == 1)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP_SX;
		}

		if (lpObj->m_PlayerData->ChangeUP == 2)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP_SX;
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP2_SX;
		}
	}

	// Set Class
	if (g_ConfigRead.SeasonX == false)
	{
		if ( lpObj->m_ActionNumber == 0x80 )
		{
			lpObj->CharSet[CS_CLASS] |= 2;
		}
		else if ( lpObj->m_ActionNumber == 0x81 )
		{
			lpObj->CharSet[CS_CLASS] |= 3;
		}

		else
		{
			lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
		}
	}

	else
	{
		if ( lpObj->m_ActionNumber == 0x80 )
		{
			lpObj->CharSet[CS_CLASS] |= 1;
		}
		else if ( lpObj->m_ActionNumber == 0x81 )
		{
			lpObj->CharSet[CS_CLASS] |= 2;
		}

		else
		{
			lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
		}
	}

	// Set Righth HAnd Item
	if(pInv[0].btItemLv > 0)
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= DBI_GET_TYPE(pInv[0].wItemID);
		lpObj->CharSet[CS_WEAPON1_TYPE] = DBI_GET_INDEX(pInv[0].wItemID);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= (BYTE)DBI_GET_INDEX(-1);
		lpObj->CharSet[CS_WEAPON1_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}

	// Set Left Hand
	if(pInv[1].btItemLv > 0)
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= DBI_GET_TYPE(pInv[1].wItemID);
		lpObj->CharSet[CS_WEAPON2_TYPE] = DBI_GET_INDEX(pInv[1].wItemID);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON2_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}

	// Set Helmet
	if(pInv[2].btItemLv > 0)
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(pInv[2].wItemID);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(pInv[2].wItemID);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(pInv[2].wItemID);
	}
	else
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(-1);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(-1);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(-1);
	}
	// Set Armor
	if(pInv[3].btItemLv > 0)
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(pInv[3].wItemID);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(pInv[3].wItemID);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(pInv[3].wItemID);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(-1);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(-1);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(-1);
	}

	// Set Pants
	if ( pInv[4].btItemLv > 0)
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(pInv[4].wItemID);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(pInv[4].wItemID);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(pInv[4].wItemID);
	}
	else
	{	
		lpObj->CharSet[14] |= CS_SET_PANTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(-1);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(-1);
	}

	// Set Gloves
	if (  pInv[5].btItemLv > 0)
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(pInv[5].wItemID);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(pInv[5].wItemID);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(pInv[5].wItemID);

	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(-1);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(-1);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(-1);
	}

	// Set Boots
	if ( pInv[6].btItemLv > 0 )
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(pInv[6].wItemID);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(pInv[6].wItemID);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(pInv[6].wItemID);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(-1);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(-1);
	}

	index = 0;
	int helperid = pInv[8].wItemID;
	if ( helperid != ITEMGET(13,4) )
	{
		if ( helperid < ITEMGET(0,0) )
		{
			index |= CS_SET_HELPER(-1);
		}
		else
		{
			index |= CS_SET_HELPER(helperid);
		}
	}
	else
	{
		index |= CS_SET_HELPER(-1);
	}

	int levelindex;
	lpObj->CharSet[5] |= index;

	levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(pInv[0].btItemLv));
	levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(pInv[1].btItemLv));
	levelindex |= CS_SET_SMALLLEVEL_HELMET(LevelSmallConvert(pInv[2].btItemLv));
	levelindex |= CS_SET_SMALLLEVEL_ARMOR(LevelSmallConvert(pInv[3].btItemLv));
	levelindex |= CS_SET_SMALLLEVEL_PANTS(LevelSmallConvert(pInv[4].btItemLv));
	levelindex |= CS_SET_SMALLLEVEL_GLOVES(LevelSmallConvert(pInv[5].btItemLv));
	levelindex |= CS_SET_SMALLLEVEL_BOOTS(LevelSmallConvert(pInv[6].btItemLv));

	lpObj->CharSet[6] = CS_SET_SMALLLEVEL1(levelindex);
	lpObj->CharSet[7] = CS_SET_SMALLLEVEL2(levelindex);
	lpObj->CharSet[8] = CS_SET_SMALLLEVEL3(levelindex);

	// Set Excellent Items
	lpObj->CharSet[10] = 0;

	
	// Set Set Items
	lpObj->CharSet[11] = 0;

	gObjCalCharacter.CalcCharacter(aIndex);

	if ( lpObj->m_PlayerData->IsFullSetItem != false )
		lpObj->CharSet[11] |= 0x01;

	if ( (helperid & 0x03) != 0 )
	{
		if ( helperid > 0 )
		{
			lpObj->CharSet[10] |= 0x01;
		}
	}

	if ( helperid == ITEMGET(13,4) )	// Dark Horse
	{
		lpObj->CharSet[12] |= 0x01;
	}

	if ( helperid == ITEMGET(13,37) )	// Fenrir
	{
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 0x04;
		lpObj->CharSet[16] = 0;
		lpObj->CharSet[17] = 0;
	}

	int wingtype = pInv[7].wItemID;
	if((wingtype >= ITEMGET(12,36) && wingtype <= ITEMGET(12,40)) || wingtype == ITEMGET(12,43) || wingtype == ITEMGET(12,50))
	{
		lpObj->CharSet[5] |= CS_SET_WING1(-1);

		if(wingtype == ITEMGET(12,43))
		{
			lpObj->CharSet[9] |= 6;
		}
		else if(wingtype == ITEMGET(12,50))
		{
			lpObj->CharSet[9] |= 7;
		}
		else
		{
			lpObj->CharSet[9] |= (wingtype - 35) & 7;
		}
	}
	else if ((wingtype >= ITEMGET(12,130) && wingtype <= ITEMGET(12,134)))
	{
		lpObj->CharSet[5] |= 12;

		lpObj->CharSet[17] |= 32 * (wingtype + 127) & 7;
	}
	else if (( wingtype >= ITEMGET(12,3) && wingtype <= ITEMGET(12,6)) || wingtype == ITEMGET(13,30) || wingtype == ITEMGET(12,42) || wingtype == ITEMGET(12,49))
	{
		lpObj->CharSet[5] |= 8;

		if(wingtype == ITEMGET(13,30))
		{
			lpObj->CharSet[9] |= 5;
		}
		else if(wingtype == ITEMGET(12,42))
		{
			lpObj->CharSet[9] |= 6;
		}
		else if(wingtype == ITEMGET(12,49))
		{
			lpObj->CharSet[9] |= 7;
		}
		else
		{
			lpObj->CharSet[9] |= (wingtype - 2) & 7;
		}
	}
	else if ((wingtype >= ITEMGET(12,0) && wingtype <= ITEMGET(12,2)) || wingtype == ITEMGET(12,41))
	{
		lpObj->CharSet[5] |= 4;

		if(wingtype == ITEMGET(12,41))
		{
			lpObj->CharSet[9] |= 4;
		}
		else
		{
			lpObj->CharSet[9] |= (wingtype + 1) & 7;
		}
	}

	itemindex = helperid;

	switch ( itemindex )
	{
		case ITEMGET(13,64):
			lpObj->CharSet[16] |= 0x20;
			break;
		case ITEMGET(13,65):
			lpObj->CharSet[16] |= 0x40;
			break;
		case ITEMGET(13,67):
			lpObj->CharSet[5]  &= 0xFC;
			lpObj->CharSet[10] |= 0x01;
			lpObj->CharSet[16] |= 0x80;
			break;
		case ITEMGET(13,80):
			lpObj->CharSet[16] |= 0xE0;
			break;
		case ITEMGET(13,106):
			lpObj->CharSet[16] |= 0xA0;
			break;
		case ITEMGET(13,123):
			lpObj->CharSet[16] |= 0x60;
			lpObj->CharSet[5] -= 0x03;
			break;
		default:
			break;
	}
}

void CBotSystem::BuffPlayer(WORD  wBufferindex,short aIndex)
{
	if(gObj[aIndex].Type != OBJ_USER)
		return;
	_sBOT_SETTINGS *lpBot = &this->m_BotData[wBufferindex];
	LPOBJ gBotObj = &gObj[lpBot->aIndex];
	if(gObj[aIndex].m_PlayerData->Money < lpBot->iCoinValue)
	{
		GSProtocol.ChatTargetSend(&gObj[lpBot->aIndex], Lang.GetText(0,364),aIndex);
		MsgOutput(aIndex, Lang.GetText(0,365), lpBot->iCoinValue);
		return;
	}

	if(gObj[aIndex].m_PlayerData->VipType == 0 && lpBot->btVipType > 0)
	{
		GSProtocol.ChatTargetSend(&gObj[lpBot->aIndex], Lang.GetText(0,366), aIndex);
		return;
	}

	if (gObj[aIndex].m_PlayerData->VipType > 0 && gObj[aIndex].m_PlayerData->VipType < lpBot->btVipType)
	{
		char szTemp[256];
		sprintf(szTemp, Lang.GetText(0,636), g_VipSystem.GetVipName(lpBot->btVipType));

		GSProtocol.ChatTargetSend(&gObj[lpBot->aIndex], szTemp, aIndex);
		return;
	}

	for(int i=0;i<MAX_BUFFS_PER_BOT;i++)
	{
		if(gBotObj->m_BotBuffs[i].wBuffId > 0)
		{
			/*CMagicInf * lpMagic = gObjGetMagicSearch(gBotObj,gBotObj->m_BotBuffs[i].wBuffId);

			if(lpMagic != 0)
			{
				gObj[aIndex].BufferIndex = gBotObj->BufferIndex;
				gObj[aIndex].BuffId = i;
				gObjUseSkill.RunningSkill(lpBot->aIndex,aIndex,lpMagic,0);
			}*/
			gObjAddBuffEffect(&gObj[aIndex], gBotObj->m_BotBuffs[i].wBuffId, gBotObj->m_BotBuffs[i].wEffectType, gBotObj->m_BotBuffs[i].iEffect, 0, 0, gBotObj->m_BotBuffs[i].wDuration);
		}
	}
	GSProtocol.ChatTargetSend(gBotObj, Lang.GetText(0,367), aIndex);
	gObj[aIndex].m_PlayerData->Money -= lpBot->iCoinValue;
	GSProtocol.GCMoneySend(aIndex,gObj[aIndex].m_PlayerData->Money);
}

BYTE CBotSystem::GetBotType(int aIndex)
{
	for(int i=0;i<this->iCount;i++)
	{
		if(this->m_BotData[i].aIndex == aIndex)
			return this->m_BotData[i].btType;
	}
}

int CBotSystem::GetSkillTime(int aIndex, WORD wSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(!lpObj)
		return 0;

	for(int i=0;i<MAX_BUFFS_PER_BOT;i++)
	{
		if(lpObj->m_BotBuffs[i].wBuffId == wSkill)
		{
			return lpObj->m_BotBuffs[i].wDuration;
		}
	}
	return 0;
}
sBOT_REWARD_STRUCT CBotSystem::ConfirmMixSuccess(int aIndex, int botIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpBotObj = &gObj[botIndex];
	sBOT_REWARD_STRUCT m_MixResult;
	int iMixNeedCount = 0;
	int foundItems = 0;
	bool bMixSuccess = false;
	for(std::map<int,sBOT_CRAFTING>::iterator It = this->m_BotData[botIndex].m_Crafting.begin(); It != this->m_BotData[botIndex].m_Crafting.end();It++)
	{
		iMixNeedCount = It->second.iTotalReqCount;
		if(iMixNeedCount == this->AlchemistTradeItemCount(aIndex))
		{

			for(int i=0;i<It->second.iReqCount;i++)
			{
				if(gObjGetItemCountInTradeWindow(aIndex,It->second.m_Requirements[i].wItemId,It->second.m_Requirements[i].btLevel,It->second.m_Requirements[i].btExc,It->second.m_Requirements[i].btSetItem,It->second.m_Requirements[i].btOpt,It->second.m_Requirements[i].btLuck,It->second.m_Requirements[i].btSkill) == It->second.m_Requirements[i].btCount)
				{
					foundItems++;
				}
				if(foundItems == It->second.iReqCount)
				{
					m_MixResult.m_Reward.Convert(It->second.m_Reward.wItemId,It->second.m_Reward.btSkill,It->second.m_Reward.btLuck,It->second.m_Reward.btOpt,0,It->second.m_Reward.btSetItem,0,0,0,0,3);
					m_MixResult.m_Reward.m_Level = It->second.m_Reward.btLevel;
					m_MixResult.m_Reward.m_NewOption = It->second.m_Reward.btExc;
					m_MixResult.iCount = It->second.RewardCount;
					m_MixResult.iSuccessRate = It->second.iSuccessRate;
					return m_MixResult;

				}
			}
		}
	}
}
bool CBotSystem::AlchemistVerifyItem(s_BOT_CRAFTING_ITEM_STRUCT lpReqItem, CItem TradeItem)
{
	if(lpReqItem.wItemId == TradeItem.m_Type &&
		lpReqItem.btLevel == TradeItem.m_Level && 
		lpReqItem.btSkill == TradeItem.m_Option1 && 
		lpReqItem.btLuck == TradeItem.m_Option2 && 
		lpReqItem.btOpt == TradeItem.m_Option3 &&
		lpReqItem.btSetItem == TradeItem.m_SetOption &&
		lpReqItem.btExc == TradeItem.m_NewOption)
	{
		return true;
	}
	else
	{
		return false;
	
	}

}

bool CBotSystem::CheckAlchemist(int aIndex, int botIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpBotObj = &gObj[botIndex];

	CItem rewardItem;
	int iMixNeedCount = 0;
	int foundItems = 0;
	int n=0;
	bool bMixSuccess = false;
	for(std::map<int,sBOT_CRAFTING>::iterator It = this->m_BotData[botIndex].m_Crafting.begin(); It != this->m_BotData[botIndex].m_Crafting.end();It++)
	{
		iMixNeedCount = It->second.iTotalReqCount;
		if(iMixNeedCount == this->AlchemistTradeItemCount(aIndex))
		{

			for(int i=0;i<It->second.iReqCount;i++)
			{
				if(gObjGetItemCountInTradeWindow(aIndex,It->second.m_Requirements[i].wItemId,It->second.m_Requirements[i].btLevel,It->second.m_Requirements[i].btExc,It->second.m_Requirements[i].btSetItem,It->second.m_Requirements[i].btOpt,It->second.m_Requirements[i].btLuck,It->second.m_Requirements[i].btSkill) == It->second.m_Requirements[i].btCount)
				{
					foundItems++;
				}
				if(foundItems == It->second.iReqCount)
				{
					rewardItem.Convert(It->second.m_Reward.wItemId,It->second.m_Reward.btSkill,It->second.m_Reward.btLuck,It->second.m_Reward.btOpt,0,It->second.m_Reward.btSetItem,0,0,0,0,3);
					rewardItem.m_Level = It->second.m_Reward.btLevel;
					rewardItem.m_NewOption = It->second.m_Reward.btExc;
					bMixSuccess = true;
					break;
				}
				if(bMixSuccess)
					break;
			}
		}
		if(bMixSuccess)
			break;
	}

	if(bMixSuccess)
	{
		BYTE ItemBuf[13];
		ItemByteConvert(ItemBuf,rewardItem);
		GSProtocol.GCTradeOtherAdd(aIndex,1,ItemBuf);
		//int GameProtocol::GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo)
	}
	else
	{
		GSProtocol.GCTradeOtherDel(aIndex,1);
	}
	return true;
}
int CBotSystem::AlchemistTradeItemCount(int aIndex)
{
	int Count=0;

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[aIndex].Trade[n].IsItem() == 1)
		{
			Count++;
		}
	}
	return Count;
}

void CBotSystem::AlchemistTradeOk(int aIndex, int botIndex)
{
	LPOBJ lpBot = &gObj[botIndex];
	LPOBJ lpObj = &gObj[aIndex];

	if(!lpObj || !lpBot)
		return;
	sBOT_REWARD_STRUCT reward = ConfirmMixSuccess(aIndex,gObj[botIndex].m_PlayerData->wBotIndex);
	if(reward.m_Reward.m_Type > 0)
	{
		int iEmptyCount = CheckInventoryEmptySpaceCount(&gObj[aIndex], ItemAttribute[reward.m_Reward.m_Type].Width, ItemAttribute[reward.m_Reward.m_Type].Height);

		if(iEmptyCount < reward.iCount)
		{
			MsgOutput(aIndex,"No space in inventory");
			return;
		}
		if(rand()%100 > reward.iSuccessRate)
		{
			MsgOutput(aIndex,"Combination failed! ");
			return;
		}
		for(int i=0;i<reward.iCount;i++)
		{
			ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, reward.m_Reward.m_Type, reward.m_Reward.m_Level, 0, reward.m_Reward.m_Option1,reward.m_Reward.m_Option2, reward.m_Reward.m_Option3, aIndex, reward.m_Reward.m_NewOption, reward.m_Reward.m_SetOption, 0, 0, 0);
		}
		gObjInventoryCommit(aIndex);
		gObjMakePreviewCharSet(aIndex);
		GJSetCharacterInfo(&gObj[aIndex],aIndex,0);
		gObjItemTextSave(&gObj[aIndex]);
		gObjStatTextSave(&gObj[aIndex]);
		gObjSavePetItemInfo(aIndex, 0);
		gObj[aIndex].TargetNumber = -1;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].TradeOk = 0;
		gObj[aIndex].TradeMoney = 0;
		GSProtocol.CGTradeResult(aIndex,1);
	}
	else
	{

	}

}
void CBotSystem::AlchemistTradeOpen(int aIndex, int botIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpBot = &gObj[botIndex];

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 1;
	lpObj->TargetNumber = botIndex;
	lpObj->m_InterfaceTime = GetTickCount64();
	lpObj->TradeMoney = 0;
	lpBot->TargetNumber = aIndex;
	PMSG_TRADE_RESPONSE tr;
	tr.Response = true;
	GSProtocol.CGTradeResponseRecv(&tr,botIndex);

}

bool CBotSystem::StoreAddItems(int botIndex)
{
	int aIndex = this->m_BotData[botIndex].aIndex;
	for(int i=0;i<this->m_BotData[botIndex].m_Shop.iItemCount;i++)
	{
		BYTE blank = this->PShopCheckSpace(&gObj[aIndex],this->m_BotData[botIndex].m_Shop.pItems[i].wItemId,&gObj[aIndex].pInventoryMap[PSHOP_START_RANGE]);
		
		if(blank != 255)
		{
			CItem item;
			item.m_Level = this->m_BotData[botIndex].m_Shop.pItems[i].btLevel;
			item.m_Option1 = this->m_BotData[botIndex].m_Shop.pItems[i].btSkill;
			item.m_Option2 = this->m_BotData[botIndex].m_Shop.pItems[i].btLuck;
			item.m_Option3 = this->m_BotData[botIndex].m_Shop.pItems[i].btOpt;
			item.m_JewelOfHarmonyOption = 0;
			item.m_NewOption = this->m_BotData[botIndex].m_Shop.pItems[i].btExc;
			item.m_SetOption = this->m_BotData[botIndex].m_Shop.pItems[i].btAnc;
			item.m_SocketOption[0] = this->m_BotData[botIndex].m_Shop.pItems[i].btSocket[0];
			item.m_SocketOption[1] = this->m_BotData[botIndex].m_Shop.pItems[i].btSocket[1];
			item.m_SocketOption[2] = this->m_BotData[botIndex].m_Shop.pItems[i].btSocket[2];
			item.m_SocketOption[3] = this->m_BotData[botIndex].m_Shop.pItems[i].btSocket[3];
			item.m_SocketOption[4] = this->m_BotData[botIndex].m_Shop.pItems[i].btSocket[4];
			item.m_iPShopValue = this->m_BotData[botIndex].m_Shop.pItems[i].iValue;
			item.Convert(this->m_BotData[botIndex].m_Shop.pItems[i].wItemId,item.m_Option1,item.m_Option2,item.m_Option3,0,item.m_SetOption,item.m_NewOption,item.m_SocketOption,0,0,3);
			gObj[aIndex].Inventory1[blank+PSHOP_START_RANGE-INVETORY_WEAR_SIZE] = item;
			
		}
		else
		{
			g_Log.MsgBox(1,"ERROR","Bot Shop id:[%d] OUT OF SPACE",botIndex);
		}
	}
	gObj[aIndex].m_bPShopOpen = true;
	//memcpy(gObj[aIndex].m_szPShopText, this->m_BotData[botIndex].m_Shop.szBotShopName, sizeof(gObj[aIndex].m_szPShopText));
	if(this->m_BotData[botIndex].m_Shop.szBotShopName.size() > 36)
	{
		MessageBoxA(0,"Max Shop name length is 36 characters","Bot System",0);
		ExitProcess(0);
	}
	memcpy(gObj[aIndex].m_szPShopText, this->m_BotData[botIndex].m_Shop.szBotShopName.c_str(), sizeof(gObj[aIndex].m_szPShopText));
	return true;
}

BYTE CBotSystem::PShopCheckSpace(LPOBJ lpObj, int type, BYTE * TempMap)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	iwidth=ItemAttribute[type].Width  ;
	iheight=ItemAttribute[type].Height  ;

	for(h = 0; h < 4; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempPShopRectCheck(TempMap,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if(blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap,blank,iwidth,iheight,type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjTempPShopRectCheck(BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x,y,blank = 0;

	if(sx + width > 12)
	{
		return -1;
	}
	if(sy + height > 8)
	{
		return -2;
	}

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(*(BYTE*)(TempMap + (sy+y)*8 + (sx+x))!= 255)
			{
				blank += 1;
				return -1;
			}
		}
	}
	if(blank == 0)
	{
		return sx+sy*8+INVETORY_WEAR_SIZE;
	}
	return -1;
}
/*
		lpReqItem.btLevel == TradeItem.m_Level && 
		lpReqItem.btSkill == TradeItem.m_Option1 && 
		lpReqItem.btLuck == TradeItem.m_Option2 && 
		lpReqItem.btOpt == TradeItem.m_Option3 &&
		lpReqItem.btSetItem == TradeItem.m_SetOption &&
		lpReqItem.btExc == TradeItem.m_NewOption)
*/

int gObjGetItemCountInTradeWindow(int aIndex, WORD itemtype, int itemlevel, BYTE btExc, BYTE btSet, BYTE btOpt, BYTE btLuck, BYTE btSkill)
{
	int Count=0;

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[aIndex].Trade[n].IsItem() == 1 &&  gObj[aIndex].Trade[n].m_Type == itemtype && gObj[aIndex].Trade[n].m_Level == itemlevel  )
		{
			if(gObj[aIndex].Trade[n].m_NewOption == btExc)
			{
				if(gObj[aIndex].Trade[n].m_Option1 == btSkill)
				{
					if(gObj[aIndex].Trade[n].m_Option2 == btLuck)
					{
						if(gObj[aIndex].Trade[n].m_Option3 == btOpt)
						{
							Count++;
						}
					}
				}
			}
		}
	}
	return Count;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

