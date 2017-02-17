//////////////////////////////////////////////////////////////////////
// OfflineLevelling.cpp
#include "StdAfx.h"
#include "user.h"
#include "ObjUseSkill.h"
#include "OfflineLevelling.h"
#include "TLog.h"
#include "configread.h"
#include "protocol.h"
#include "GameMain.h"
#include "winutil.h"
#include "DSProtocol.h"
#include "ItemSocketOptionSystem.h"
#include "CashShop.h"
#include "LuckyItemManager.h"

COfflineLevelling g_OffLevel;

void OffLevelThread()
{
	while(true)
	{
		for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
		{
			if(!gObjIsConnectedGP(i))
				continue;
			if(g_OffLevel.FindUser(i) != -1)
			{
				g_OffLevel.FindAndAttack(&gObj[i]);
				g_OffLevel.CheckUseTime(i);
				if(g_OffLevel.m_General.AutoRepairItems == 1)
				{
					g_OffLevel.CheckRepairItems(i);
				}
			}
		}
		Sleep(g_OffLevel.m_General.HitInterval);
	}
}

COfflineLevelling::COfflineLevelling()
{

}

COfflineLevelling::~COfflineLevelling()
{

}

void COfflineLevelling::Initiate()
{
	InitializeCriticalSection(&m_OfflevelCriti);
	this->m_OffPlayerData.clear();
	memset(this->m_SkillDef,0,sizeof(this->m_SkillDef));
	this->LoadFile(g_ConfigRead.GetPath("\\Plugins\\IGC_OffLevelling.xml"));
	this->LoadSkillDefinitions(g_ConfigRead.GetPath("\\Skills\\IGC_SkillCategory.xml"));
}

BOOL COfflineLevelling::LoadSkillDefinitions(LPCSTR szFile)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if(res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file load fail (%s)", szFile, res.description());
		return 0;
	}

	pugi::xml_node skillcat = file.child("SkillCategory");
	for (pugi::xml_node skill = skillcat.child("Skill"); skill; skill = skill.next_sibling())
	{
		int skillid = skill.attribute("Index").as_int();
		this->m_SkillCategories[skillid].SkillType = skill.attribute("Type").as_int();
	}
}

BOOL COfflineLevelling::LoadFile(LPCSTR szFile)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if(res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file load fail (%s)", szFile, res.description());
		return 0;
	}

	pugi::xml_node offlevelsettings = file.child("OffLevellingSystem");
	this->b_Enabled = offlevelsettings.attribute("Enable").as_bool(0);

	pugi::xml_node general = offlevelsettings.child("General");

	m_General.VipType = general.attribute("VipType").as_int();
	m_General.CoinType = general.attribute("CoinType").as_int();
	m_General.HitInterval = general.attribute("HitInterval").as_int(1000);
	m_General.ChargeInterval = general.attribute("ChargeInterval").as_int();
	m_General.CoinValue = general.attribute("CoinValue").as_int();
	m_General.Immortal = general.attribute("Immortal").as_int(0);
	m_General.MaxDuration = general.attribute("MaxDuration").as_int(0);
	m_General.MaxVipDuration = general.attribute("MaxVipDuration").as_int(0);
	m_General.AutoRepairItems = general.attribute("AutoRepairItems").as_int(0);
	m_PickItems.clear();

	pugi::xml_node itemlist = offlevelsettings.child("PickItems");

	m_PickItemSettings.bPickUpItems = itemlist.attribute("Enable").as_bool(0);
	m_PickItemSettings.PickUpZen = itemlist.attribute("PickUpZen").as_int(0);
	m_PickItemSettings.PickUpExc = itemlist.attribute("PickUpAllExc").as_int(0);
	m_PickItemSettings.PickUpAncient = itemlist.attribute("PickUpAllAncient").as_int(0);
	m_PickItemSettings.PickUpSocket = itemlist.attribute("PickUpAllSocket").as_int(0);

	for(pugi::xml_node item = itemlist.child("Item"); item; item = item.next_sibling())
	{
		OFF_LEVEL_ITEM_PICK_LIST pickItem; 
		WORD wItemCat = item.attribute("Cat").as_int();
		WORD wItemIndex = item.attribute("Index").as_int();
		pickItem.ItemId = ITEMGET(wItemCat,wItemIndex);
		this->m_PickItems.insert(std::pair<int, OFF_LEVEL_ITEM_PICK_LIST>(pickItem.ItemId, pickItem));
		
	}

	pugi::xml_node permapadjust = offlevelsettings.child("PerMapAdjustments");

	for (pugi::xml_node map = permapadjust.child("Map"); map; map = map.next_sibling())
	{
		int MapNumber = map.attribute("Number").as_int();
		this->m_PerMapAttr[MapNumber].CoinType = map.attribute("CoinType").as_int();
		this->m_PerMapAttr[MapNumber].CoinValue = map.attribute("CoinValue").as_int();
		this->m_PerMapAttr[MapNumber].Disable = map.attribute("Disable").as_int();
	}

	pugi::xml_node skilldef = offlevelsettings.child("ClassSkillSettings");

	for(pugi::xml_node classes = skilldef.child("Class"); classes; classes = classes.next_sibling())
	{
		int ClassType = classes.attribute("Type").as_int();
		for(pugi::xml_node skills = classes.child("Skill"); skills; skills = skills.next_sibling())
		{
			int idx = skills.attribute("ID").as_int();
			this->m_SkillDef[ClassType].SkillNumber[idx] = skills.attribute("Index").as_int();
		}
	}
}
int COfflineLevelling::FindUser(int aIndex)
{
	EnterCriticalSection(&m_OfflevelCriti);
	std::map<int,OFF_LEVEL_PLAYERS>::iterator iter = m_OffPlayerData.find(aIndex);
	
	if(iter == m_OffPlayerData.end())
	{
		//..g_Log.AddC(TColor::Red,"User %d not found LINE: %d",aIndex,__LINE__);
		LeaveCriticalSection(&m_OfflevelCriti);
		return -1;
	}

	LeaveCriticalSection(&m_OfflevelCriti);
	return aIndex;
}

BOOL COfflineLevelling::AddUser(int aIndex, WORD wSkillId)
{
	EnterCriticalSection(&m_OfflevelCriti);

	std::map<int,OFF_LEVEL_PLAYERS>::iterator iter = m_OffPlayerData.find(aIndex);

	if(iter != m_OffPlayerData.end())
	{
		g_Log.Add("[OfflineLevelling] User Already exist %d",aIndex);
		LeaveCriticalSection(&m_OfflevelCriti);
		return false;
	}
	OFF_LEVEL_PLAYERS obj;
	obj.aIndex = aIndex;
	obj.wSkillNumber = wSkillId;
	obj.dwOffTime = GetTickCount();
	m_OffPlayerData.insert(std::pair<int, OFF_LEVEL_PLAYERS>(aIndex,obj));

	g_Log.Add("[OfflineLevelling] Add user %d Skill Id %d",aIndex, wSkillId);

	LeaveCriticalSection(&m_OfflevelCriti);
	return true;
}

BOOL COfflineLevelling::DeleteUser(int aIndex)
{
	EnterCriticalSection(&m_OfflevelCriti);

	std::map<int,OFF_LEVEL_PLAYERS>::iterator iter = m_OffPlayerData.find(aIndex);

	if(iter == m_OffPlayerData.end())
	{
		g_Log.Add("[OfflineLevelling] User does not exist %d",aIndex);
		return false;
	}

	m_OffPlayerData.erase(iter);
	g_Log.Add("[OfflineLevelling] User %d removed from Offlevelling system",aIndex);
	LeaveCriticalSection(&m_OfflevelCriti);
	return true;
}

void COfflineLevelling::FindAndAttack(LPOBJ user)
{
	LPOBJ tObj;
	CMagicInf* lpMagic;
	OFF_LEVEL_PLAYERS obj;
	std::map<int,OFF_LEVEL_PLAYERS>::iterator iter = m_OffPlayerData.find(user->m_Index);
	obj = iter->second;
	switch(this->GetSkillAttackType(obj.wSkillNumber))
	{
		case MAGIC_ATTACK:
			{
				for (int i=0;i<MAX_VIEWPORT;i++)
				{		
					if(user->VpPlayer2[i].type == OBJ_MONSTER && user->VpPlayer2[i].state != 0)
					{
						lpMagic = gObjGetMagicSearch(user,obj.wSkillNumber);
						if(lpMagic == NULL)
						{
							g_Log.Add("[OfflineLevelling] [%s][%s] Skill %d not found", user->AccountID, user->Name,obj.wSkillNumber);
							return;
						}
						
						int tObjNum = user->VpPlayer2[i].number;

						if (!ObjectMaxRange(tObjNum))
						{
							continue;
						}

						tObj = &gObj[tObjNum];

						if(!tObj)
							continue;

						if(tObj->Live == 0 || tObj->DieRegen != FALSE)
							continue;

						if(SkillDistanceCheck(user->m_Index,tObj->m_Index, lpMagic->m_Skill))
						{
							gObjUseSkill.UseSkill(user->m_Index, tObj->m_Index, lpMagic);
							return;
						}
					}
				}
			}
			break;
		case DURATION_MAGIC:
			{
			for (int i=0;i<MAX_VIEWPORT;i++)
				{
					if(user->VpPlayer2[i].type == OBJ_MONSTER && user->VpPlayer2[i].state != 0)
					{
						int tObjNum = user->VpPlayer2[i].number;

						if (!ObjectMaxRange(tObjNum))
						{
							continue;
						}

						tObj = &gObj[tObjNum];

						if(!tObj)
							continue;

						if(tObj->Live == 0 || tObj->DieRegen != FALSE)
							continue;

						lpMagic = gObjGetMagicSearch(user,obj.wSkillNumber);

						if(lpMagic == NULL)
						{
								g_Log.Add("[OfflineLevelling] [%s][%s] Skill %d not found", user->AccountID, user->Name,obj.wSkillNumber);
								return;
						}

						if(SkillDistanceCheck(user->m_Index,tObj->m_Index, lpMagic->m_Skill) && tObj->Live != 0)
						{
							BYTE Dir = GetPathPacketDirPos(tObj->X - user->X,tObj->Y - user->Y);
							user->Dir = Dir;
							PMSG_ACTIONRESULT pActionResult;
				
							PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
							pActionResult.NumberH = SET_NUMBERH(user->m_Index);
							pActionResult.NumberL = SET_NUMBERL(user->m_Index);
							pActionResult.ActionNumber = 0x7A;
							pActionResult.Dir = user->Dir;
							pActionResult.TargetNumberH =SET_NUMBERH(tObj->m_Index);
							pActionResult.TargetNumberL = SET_NUMBERL(tObj->m_Index);

							IOCP.DataSend(user->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
	
							gObjUseSkill.UseSkill(user->m_Index, lpMagic,user->X, user->Y, Dir, 0, tObj->m_Index);
							return;
						}
					}
				}
			}
			break;
		case BE_ATTACK:
			{
				int hitCount = 0;
				for (int i=0;i<MAX_VIEWPORT;i++)
				{
					if(user->VpPlayer2[i].type == OBJ_MONSTER && user->VpPlayer2[i].state != 0)
					{	
						int tObjNum = user->VpPlayer2[i].number;

						if (!ObjectMaxRange(tObjNum))
						{
							continue;
						}

						tObj = &gObj[tObjNum];

						if(!tObj)
							continue;

						if(tObj->Live == 0 || tObj->DieRegen != FALSE)
							continue;

						int DistanceCheck = gObjCalDistance(user, tObj);

						if(DistanceCheck > 13){
							continue;
						}
							
						hitCount++;

						if(hitCount > 5)
						{
							BYTE Dir = GetPathPacketDirPos(tObj->X - user->X,tObj->Y - user->Y);
							gObjUseSkill.UseSkill(user->m_Index, lpMagic,user->X, user->Y, Dir, 0, tObj->m_Index);
							break;
						}
						lpMagic = gObjGetMagicSearch(user,obj.wSkillNumber);
						if(lpMagic == NULL)
						{
							g_Log.Add("[OfflineLevelling] [%s][%s] Skill %d not found", user->AccountID, user->Name,obj.wSkillNumber);
							return;
						}
						gObjAttack(user, tObj, lpMagic, TRUE, 1, 0, FALSE, 0, 0);

					}
				}
			}
			break;
	}
}
bool COfflineLevelling::ChargePlayer(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj == NULL)
		return false;

	if(lpObj->Type != OBJ_USER)
		return false;
	if(this->m_PerMapAttr[lpObj->MapNumber].CoinType == -1)
	{
		switch (this->m_General.CoinType)
		{
			case 0:
			{
				if (lpObj->m_PlayerData->Money < this->m_General.CoinValue)
				{
					return false;
				}
				else
				{
					lpObj->m_PlayerData->Money -= this->m_General.CoinValue;
					return true;
				}
			}
			break;

			case 1:
			{
				if (lpObj->m_PlayerData->m_WCoinC < this->m_General.CoinValue)
				{
					return false;
				}
				else
				{
						lpObj->m_PlayerData->m_WCoinC -= this->m_General.CoinValue;
						GDReqInGameShopPointAdd(lpObj->m_Index, 0, 0.00 - this->m_General.CoinValue);
						return true;
				}
			}
			break;

			case 2:
			{
				if (lpObj->m_PlayerData->m_WCoinP < this->m_General.CoinValue )
				{
					return false;
				}
				else
				{
						lpObj->m_PlayerData->m_WCoinP -= this->m_General.CoinValue;
						GDReqInGameShopPointAdd(lpObj->m_Index, 1, 0.00 - this->m_General.CoinValue);
						return true;
				}
			}
			break;

			case 3:
			{
				if (lpObj->m_PlayerData->m_GoblinPoint < this->m_General.CoinValue)
				{
					return false;
				}
				else
				{
					lpObj->m_PlayerData->m_GoblinPoint -= this->m_General.CoinValue;
					GDReqInGameShopPointAdd(lpObj->m_Index, 2, 0.00 - this->m_General.CoinValue);
					return true;
				}
			}
			break;
		}
	}
	else
	{
		switch (this->m_PerMapAttr[lpObj->MapNumber].CoinType)
		{
			case 0:
			{
				if (lpObj->m_PlayerData->Money < this->m_PerMapAttr[lpObj->MapNumber].CoinValue)
				{
					return false;
				}
				else
				{
					lpObj->m_PlayerData->Money -= this->m_PerMapAttr[lpObj->MapNumber].CoinValue;
					return true;
				}
			}
			break;

			case 1:
			{
				if (lpObj->m_PlayerData->m_WCoinC < this->m_PerMapAttr[lpObj->MapNumber].CoinValue)
				{
					return false;
				}
				else
				{
						lpObj->m_PlayerData->m_WCoinC -= this->m_PerMapAttr[lpObj->MapNumber].CoinValue;
						GDReqInGameShopPointAdd(lpObj->m_Index, 0, 0.00 - this->m_PerMapAttr[lpObj->MapNumber].CoinValue);
						return true;
				}
			}
			break;

			case 2:
			{
				if (lpObj->m_PlayerData->m_WCoinP < this->m_PerMapAttr[lpObj->MapNumber].CoinValue )
				{
					return false;
				}
				else
				{
						lpObj->m_PlayerData->m_WCoinP -= this->m_PerMapAttr[lpObj->MapNumber].CoinValue;
						GDReqInGameShopPointAdd(lpObj->m_Index, 1, 0.00 - this->m_PerMapAttr[lpObj->MapNumber].CoinValue);
						return true;
				}
			}
			break;

			case 3:
			{
				if (lpObj->m_PlayerData->m_GoblinPoint < this->m_PerMapAttr[lpObj->MapNumber].CoinValue)
				{
					return false;
				}
				else
				{
					lpObj->m_PlayerData->m_GoblinPoint -= this->m_PerMapAttr[lpObj->MapNumber].CoinValue;
					GDReqInGameShopPointAdd(lpObj->m_Index, 2, 0.00 - this->m_PerMapAttr[lpObj->MapNumber].CoinValue);
					return true;
				}
			}
			break;
		}
	}
	return false;
}
void COfflineLevelling::Run()
{
	LPOBJ lpObj;

	if (this->b_Enabled == false)
	{
		return;
	}

	EnterCriticalSection(&this->m_OfflevelCriti);
	for(std::map<int, OFF_LEVEL_PLAYERS>::iterator iter = m_OffPlayerData.begin(); iter != m_OffPlayerData.end(); iter++)
	{
		lpObj = &gObj[iter->second.aIndex];
		if(!lpObj){
			this->DeleteUser(iter->second.aIndex);
			continue;
		}
		if(!lpObj->m_bOffLevel){
			this->DeleteUser(lpObj->m_Index);
			continue;
		}

		if(!lpObj->m_bOff){
			this->DeleteUser(lpObj->m_Index);
			continue;
		}

		if(GetTickCount() - iter->second.dwOffTime > this->m_General.ChargeInterval*1000)
		{
			iter->second.dwOffTime = GetTickCount();

			if(!this->ChargePlayer(lpObj->m_Index))
			{
				lpObj->m_bOff = false;
				lpObj->m_bOffLevel = false;
			}
		}
	}
	LeaveCriticalSection(&this->m_OfflevelCriti);
}

int COfflineLevelling::GetSkillAttackType(WORD wSkillID)
{
/*	switch(wSkillID)
	{
	case 24:
	case 41:
		return DURATION_MAGIC;
	case 43:
		return MAGIC_ATTACK;
	}
	*/
	return this->m_SkillCategories[wSkillID].SkillType;
}

BOOL COfflineLevelling::SkillDistanceCheck(int aIndex, int aTargetIndex, int iSkillNum)
{

	if ( iSkillNum == 40 )
		return TRUE;

	if ( iSkillNum == 392 )
		return TRUE;

	int iSkillDistance = MagicDamageC.GetSkillDistance(iSkillNum);

	if ( iSkillDistance == -1 )
		return FALSE;

	iSkillDistance += 1;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( abs(lpObj->X - lpTargetObj->X) > iSkillDistance ||
		 abs(lpObj->Y - lpTargetObj->Y) > iSkillDistance )
	{
		return FALSE;
	}

	return TRUE;
}

int COfflineLevelling::GetOffLevelerCount()
{
	int counter = 0;
	for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if(gObj[i].Connected == 3 && gObj[i].Type == OBJ_USER && gObj[i].m_bOffLevel )
		{
			counter++;
		}
	}
	return counter;
}

bool COfflineLevelling::CheckUseTime(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(!lpObj)
		return false;

	if(this->m_General.MaxDuration == 0 && lpObj->m_PlayerData->VipType == 0 )
	{
		return false;
	}
	else if (this->m_General.MaxVipDuration == 0 && lpObj->m_PlayerData->VipType != 0)
	{
		return false;
	}

	DWORD64 currentTick = GetTickCount64();

	DWORD64 useTime = (currentTick - lpObj->m_dwOffLevelTime);

	if(lpObj->m_PlayerData->VipType == 0)
	{
		if(useTime > (this->m_General.MaxDuration * 1000 * 3600))
		{
			lpObj->m_bOff = false;
			lpObj->m_bOffLevel = false;
			this->DeleteUser(lpObj->m_Index);
			g_Log.Add("[OffLevel][%d][%s][%s] Exceed max use time [Hours:%d] [Vip:%d]",aIndex,lpObj->AccountID,lpObj->Name,(useTime * 1000 *3600), lpObj->m_PlayerData->VipType);
		}
	}
	else
	{
		if(useTime > (this->m_General.MaxVipDuration * 1000 * 3600))
		{
			lpObj->m_bOff = false;
			lpObj->m_bOffLevel = false;
			this->DeleteUser(lpObj->m_Index);
			g_Log.Add("[OffLevel][%d][%s][%s] Exceed max use time [Hours:%d] [Vip:%d]",aIndex,lpObj->AccountID,lpObj->Name,(useTime * 1000 *3600), lpObj->m_PlayerData->VipType);
		}
	}

	return true;
	
	//
	
}

void COfflineLevelling::CheckAndPickUpItem(int aIndex, CMapItem* cMapItem, short tObjNum)
{
	bool bTakeItem = false;

	std::map<int, OFF_LEVEL_ITEM_PICK_LIST>::iterator It = this->m_PickItems.find(cMapItem->m_Type);

	if (It != this->m_PickItems.end())
	{
		bTakeItem = true;
	}

	else if(cMapItem->m_Type == ITEMGET(14,15) && this->m_PickItemSettings.PickUpZen == 1)
	{
		bTakeItem = true;
	}

	else if(cMapItem->m_NewOption > 0 && this->m_PickItemSettings.PickUpExc == 1)
	{
		bTakeItem = true;
	}

	else if(cMapItem->IsSetItem() && this->m_PickItemSettings.PickUpAncient == 1)
	{
		bTakeItem = true;
	}
	else if(g_SocketOptionSystem.IsSocketItem(cMapItem) && m_PickItemSettings.PickUpSocket == 1)
	{
		bTakeItem = true;
	}
	else
	{
		bTakeItem = false;
	}

	if(bTakeItem)
	{
		PMSG_ITEMGETREQUEST pItemGetReq;
		pItemGetReq.NumberH = SET_NUMBERH(tObjNum);
		pItemGetReq.NumberL = SET_NUMBERL(tObjNum);
		GSProtocol.CGItemGetRequest(&pItemGetReq,aIndex);
	}
}

void COfflineLevelling::CheckRepairItems(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int i=0;i<INVETORY_WEAR_SIZE;i++)
	{
		if(lpObj->pInventory[i].IsItem())
		{
			if(lpObj->pInventory[i].m_Durability < lpObj->pInventory[i].m_BaseDurability)
			{
				if ( IsCashItem(lpObj->pInventory[i].m_Type) == TRUE )
					continue;

				if (IsPremiumItem(lpObj->pInventory[i].m_Type) == TRUE)
					continue;

				if ( lpObj->pInventory[i].IsPeriodItem() == TRUE )
					continue;

				if ( lpObj->pInventory[i].IsPeriodItemExpire() == TRUE )
					continue;

				if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[i].m_Type))	//season 6.1 add-on
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,20) && (lpObj->pInventory[i].m_Level == 0 ||lpObj->pInventory[i].m_Level == 1 ))
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,20) && lpObj->pInventory[i].m_Level == 2 )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,4) || lpObj->pInventory[i].m_Type == ITEMGET(13,5) )
					continue;
				
				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,37) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,38) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,40) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,41) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,42) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,51) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,64) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,65) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,67) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,70) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,76) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,77) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,78) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,166) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,80) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,106) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,123) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(13,39) )
					continue;

				if ( lpObj->pInventory[i].m_Type == ITEMGET(12,26) &&
					(lpObj->pInventory[i].m_Level == 1 ||
						lpObj->pInventory[i].m_Level == 2 || 
						lpObj->pInventory[i].m_Level == 3 || 
						lpObj->pInventory[i].m_Level == 4 || 
						lpObj->pInventory[i].m_Level == 5 ))
						continue;

				if (!IsRepairItem(lpObj->pInventory[i].m_Type))
					continue;

				GSProtocol.ItemDurRepaire(lpObj,&lpObj->pInventory[i],i,1);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

