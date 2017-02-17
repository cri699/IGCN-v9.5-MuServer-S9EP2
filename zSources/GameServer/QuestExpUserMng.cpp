//////////////////////////////////////////////////////////////////////
// QuestExpUserMng.cpp
#include "stdafx.h"
#include "QuestExpUserMng.h"
#include "QuestExpInfo.h"
#include "QuestExpUserInfo.h"
#include "user.h"
#include "TLog.h"
#include "MasterLevelSkillTreeSystem.h"
#include "QuestExpProgMng.h"
#include "DSProtocol.h"
#include "winutil.h"
#include "BuffEffectSlot.h"
#include "GensSystem.h"
#include "GameMain.h"
#include "configread.h"

QuestExpUserMng g_QuestExpUserMng;

QuestExpUserMng::QuestExpUserMng(void)
{
}

QuestExpUserMng::~QuestExpUserMng(void)
{
}

int QuestExpUserMng::IsProgQuestInfo(int iEpisode, int iObjIndex)
{
	if (IsEpisode(iEpisode) == false)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return 0;
	}

	return gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetAskCnt();
}

int QuestExpUserMng::IsProgQuestInfo(DWORD dwQuestInfoIndexID, int iObjIndex)
{
	return this->IsProgQuestInfo(GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID), iObjIndex);
}

bool QuestExpUserMng::IsQuestComplete(int iEpisode, int iObjIndex)
{
	UserQuestInfo* pQuestExpUserMng = this->GetUserQuestInfo(iEpisode, iObjIndex);

	if (pQuestExpUserMng == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return false;
	}

	int iAskCnt = pQuestExpUserMng->GetAskCnt();

	if (iAskCnt >= MAX_QUESTEXP_ASK_INFO)
	{
		g_Log.Add("[QuestExp] - Error - IsQuestComplete [%s][%s] iAskCnt[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iAskCnt);
		return false;
	}

	if (!g_QuestExpProgMng.ChkQuestAsk(iEpisode, iObjIndex))
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return false;
	}

	for (int i = 0; i < iAskCnt; i++)
	{
		if (gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].m_UserQuestAskInfo[i].IsComplete() == false)
		{
			return false;
		}
	}

	if (iAskCnt != 0)
	{
		int iQS = pQuestExpUserMng->GetQuestSwitch();
		g_Log.Add("[QuestExp] AskQuestComplete [%s][%s] Ep[%d] QS[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode, iQS);
		return true;
	}

	else
	{
		int iQS = pQuestExpUserMng->GetQuestSwitch();
		g_Log.Add("[QuestExp] IsQuestComplete [%s][%s] Ep[%d] QS[%d] ***HackUser***", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode, iQS);
		return false;
	}
}

bool QuestExpUserMng::QuestExpExtentCheck(int x, int y, int w, int h)
{
	return (x >= 0 && x < w && y >= 0 && y < h);
}

bool QuestExpUserMng::QuestExpCheckInventoryEmptySpace(BYTE* btTempInventoryMap, int iItemHeight, int iItemWidth, int iObjIndex)
{
	for (int h = 0; h < MAX_INVENTORY_H; ++h)
	{
		for (int w = 0; w < 8; ++w)
		{
			if (btTempInventoryMap[w + h * 8] == 0xFF)
			{
				BYTE blank = this->QuestExpOnlyInventoryRectCheck(btTempInventoryMap, w, h, iItemWidth, iItemHeight, iObjIndex);

				if (blank == 0xFE)
					return false;

				if (blank != 0xFF)
				{
					this->QuestExpTempInventoryItemSet(btTempInventoryMap, blank, iItemWidth, iItemHeight, 1, iObjIndex);
					return true;
				}
			}
		}
	}

	return false;
}

BYTE QuestExpUserMng::QuestExpOnlyInventoryRectCheck(BYTE *btTempInventoryMap, int sx, int sy, int width, int height, int iObjIndex)
{
	if (width + sx > 8)
	{
		return -1;
	}

	if (height + sy > 4 * gObj[iObjIndex].m_PlayerData->m_InventoryExpansion + 8)
	{
		return -2;
	}

	int bResult = CheckOutOfInventory(iObjIndex, sy, height);

	if (bResult == 0)
	{
		return -1;
	}

	if (bResult == -1)
	{
		return -2;
	}

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (InventoryExtentCheck(x + sx, y + sy, 8, MAX_INVENTORY_H) != true)
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return -1;
			}

			if (btTempInventoryMap[x + sx + (y + sy) * 8] != 0xFF)
				return -1;
		}
	}

	BYTE blank = sx + 8 * sy + 12;
	return blank;
}

void QuestExpUserMng::QuestExpTempInventoryItemSet(BYTE *btTempInventoryMap, int itempos, int xl, int yl, BYTE set_byte, int iObjIndex)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	if (this->CheckOutOfInventory(iObjIndex, itemposy, yl) == true)
	{
		for (int y = 0; y < yl; ++y)
		{
			for (int x = 0; x < xl; ++x)
			{
				if (this->InventoryExtentCheck(x + itemposx, y + itemposy, 8, MAX_INVENTORY_H2) != true)
				{
					g_Log.Add("error : %s %d", __FILE__, __LINE__);
					return;
				}

				btTempInventoryMap[x + itemposx + (y + itemposy) * 8] = set_byte;
			}
		}
	}
}

bool QuestExpUserMng::GetRandomRewardItemIndex(int *iSetItem, QuestExpRewardKindRandom *pRewardKindRandom)
{
	if (!pRewardKindRandom)
	{
		g_Log.Add("[QuestExp] - Error - GetRandomRewardItemIndex(NULL)");
		return false;
	}

	int iRewardCnt = pRewardKindRandom->GetRandRewardCnt();
	int iRewardItemTotCnt = pRewardKindRandom->GetRewardItemTotCnt();
	int* iRandRate = pRewardKindRandom->GetRewardRate();
	memset(iSetItem, -1, sizeof(int[5]));

	for (int i = 0; i < iRewardCnt; i++)
	{
		int iSetItemTemp = 0;
		bool bSetItem = false;
		int iRand = rand() % 10000;

		for (int j = 0; j < iRewardItemTotCnt; ++j)
		{
			if (iSetItem[j] == -1 && iRand < iRandRate[j])
			{
				iSetItemTemp = j;
				bSetItem = true;
			}
		}

		if (bSetItem == true)
		{
			iSetItem[iSetItemTemp] = iSetItemTemp;
			g_Log.Add("[QuestExp] - (RandomSetItem) RandomValue [%d] : [%d]", iRand, iSetItem[iSetItemTemp] + 1);
		}

		if (bSetItem == false)
		{
			int iRandRateTemp = 0;
			iSetItemTemp = 0;

			for (int j = 0; j < iRewardItemTotCnt; ++j)
			{
				if (iSetItem[j] == -1 && iRandRateTemp < iRandRate[j])
				{
					iRandRateTemp = iRandRate[j];
					iSetItemTemp = j;
				}
			}

			iSetItem[iSetItemTemp] = iSetItemTemp;
			g_Log.Add("[QuestExp] - (SetItem) RandomValue [%d] : [%d]", iRand, iSetItem[iSetItemTemp] + 1);
		}
	}

	return true;
}

bool QuestExpUserMng::IsRandRewardIndex(int iRewardIndexID, QuestExpRewardKindRandom *pRewardKindRandom)
{
	if (pRewardKindRandom == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - %s(NULL)", __FUNCTION__);
		return false;
	}

	int* iRandRewardIndexID = pRewardKindRandom->GetRewardIndex();
	int iRewardItemTotCnt = pRewardKindRandom->GetRewardItemTotCnt();

	for (int i = 0; i < iRewardItemTotCnt; i++)
	{
		if (iRandRewardIndexID[i] == iRewardIndexID)
		{
			return true;
		}
	}

	return false;
}

bool QuestExpUserMng::IsRandResultReward(int iRewardIndexID, QuestExpRewardKindRandom *pRewardKindRandom, int *iRandResultIndexID)
{
	if (pRewardKindRandom == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - %s(NULL)", __FUNCTION__);
		return false;
	}

	int* iRandRewardIndexID = pRewardKindRandom->GetRewardIndex();
	int iRewardItemTotCnt = pRewardKindRandom->GetRewardItemTotCnt();

	for (int i = 0; i < iRewardItemTotCnt; ++i)
	{
		if (iRandRewardIndexID[i] == iRewardIndexID && iRandResultIndexID[i] == iRewardIndexID)
		{
			return true;
		}
	}

	return false;
}

int QuestExpUserMng::InvenChk_EnableReward(int iEpisode, int iObjIndex, int *pRandResultIndexID)
{
	if (!IsEpisode(iEpisode))
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return 0;
	}

	int iQuestSwitch = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestSwitch();

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);
	QuestExpInfo* pQuestExpInfo = g_QuestExpManager.GetQuestExpInfo(dwQuestInfoIndexID);

	if (pQuestExpInfo == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - InvenChk_EnableReward [%s][%s] Ep[%d] QS[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode, iQuestSwitch);
		return 0;
	}

	auto MapQuestReward = pQuestExpInfo->GetQuestReward();

	BYTE btTempInventoryMap[INVENTORY_MAP_SIZE];
	memcpy(btTempInventoryMap, gObj[iObjIndex].pInventoryMap, INVENTORY_MAP_SIZE);

	int RandomRewardIndex[MAX_QUESTEXP_REWARDKIND_RANDOM] = { 0, 0, 0, 0, 0 };

	QuestExpRewardKindRandom* pRewardKindRandom = nullptr;
	QuestExpRewardKind* pRewardKind = g_QuestExpManager.GetQuestExpRewardKind(dwQuestInfoIndexID);

	if (pRewardKind && pRewardKind->GetRewardKind() == true)
	{
		pRewardKindRandom = static_cast<QuestExpRewardKindRandom*>(pRewardKind);
		bool bGetRandomRewardItemIndex = GetRandomRewardItemIndex(RandomRewardIndex, pRewardKindRandom);

		if (bGetRandomRewardItemIndex == false)
		{
			return 0;
		}

		memcpy(pRandResultIndexID, RandomRewardIndex, sizeof(RandomRewardIndex));
	}

	auto it = MapQuestReward.begin();
	auto end = MapQuestReward.end();

	for (; it != end; ++it)
	{
		QuestExpReward* pQuestExpReward = it->second;

		if (pQuestExpReward == nullptr)
		{
			g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
			return 0;
		}

		int iQuestType = pQuestExpReward->GetRewardType();

		if (pRewardKind)
		{
			bool bRandResultReward = IsRandResultReward(pQuestExpReward->GetIndexID(), pRewardKindRandom, RandomRewardIndex);
			bool bRandIndex = IsRandRewardIndex(pQuestExpReward->GetIndexID(), pRewardKindRandom);

			if (!bRandResultReward && bRandIndex)
			{
				iQuestType = QUESTEXP_REWARD_NONE;
			}
		}

		if (iQuestType == QUESTEXP_REWARD_ZEN)
		{
			RewardZen* pRewardZen = static_cast<RewardZen*>(pQuestExpReward);

			if (!gObjCheckMaxZen(iObjIndex, pRewardZen->GetRewardZen()))
			{
				return 2;
			}
		}

		else if (iQuestType == QUESTEXP_REWARD_ITEM)
		{
			RewardItem* pRewardItem = static_cast<RewardItem*>(pQuestExpReward);

			int iItemCnt = pRewardItem->GetRewardItemCnt();
			int iItemNum = pRewardItem->GetRewardItemNum();

			for (int j = 0; j < iItemCnt; ++j)
			{
				int iItemHeight = 0;
				int iItemWidth = 0;
				ItemGetSize(iItemNum, iItemWidth, iItemHeight);

				if (!QuestExpCheckInventoryEmptySpace(btTempInventoryMap, iItemHeight, iItemWidth, iObjIndex))
				{
					return 3;
				}
			}
		}
	}

	return 1;
}

int QuestExpUserMng::SendQuestReward(int iEpisode, int iObjIndex)
{
	if (!IsEpisode(iEpisode))
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return 0;
	}

	int RandResultIndexId[MAX_QUESTEXP_REWARDKIND_RANDOM];
	int iResult = this->InvenChk_EnableReward(iEpisode, iObjIndex, RandResultIndexId);
	int iQuestSwitch = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestSwitch();

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);

	if (iResult != TRUE)
	{
		g_QuestExpProgMng.GCANSQuestCompleteBtnClick(iObjIndex, dwQuestInfoIndexID, iResult);
		return iResult;
	}

	QuestExpInfo* pQuestExpInfo = g_QuestExpManager.GetQuestExpInfo(dwQuestInfoIndexID);

	if (!pQuestExpInfo)
	{
		g_Log.Add("[QuestExp] - Error - QuestExpInfo [%s][%s] Ep [%d] QS[%d] QuestInfoIndexID [0x%x]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode, iQuestSwitch, dwQuestInfoIndexID);
		return 0;
	}

	QuestExpRewardKindRandom* pRewardKindRandom = nullptr;
	QuestExpRewardKind* pRewardKind = g_QuestExpManager.GetQuestExpRewardKind(dwQuestInfoIndexID);

	if (pRewardKind)
	{
		if (pRewardKind->GetRewardKind() == true)
		{
			pRewardKindRandom = static_cast<QuestExpRewardKindRandom*>(pRewardKind);
		}
	}

	auto MapQuestReward = pQuestExpInfo->GetQuestReward();

	auto iter = MapQuestReward.begin();
	auto end = MapQuestReward.end();

	for (; iter != end; ++iter)
	{
		QuestExpReward* pQuestExpReward = iter->second;
		int iQuestType = pQuestExpReward->GetRewardType();

		char LogBuff[128] = { 0 };
		wsprintf(LogBuff, "[QuestExp] ");

		if (pRewardKind)
		{
			bool bRandResultReward = IsRandResultReward(pQuestExpReward->GetIndexID(), pRewardKindRandom, RandResultIndexId);
			bool bRandIndex = IsRandRewardIndex(pQuestExpReward->GetIndexID(), pRewardKindRandom);

			if (!bRandResultReward && bRandIndex)
			{
				iQuestType = QUESTEXP_REWARD_NONE;
			}

			else
			{
				wsprintf(LogBuff, "[QuestExp] (Random) ");
			}
		}

		switch (iQuestType)
		{
		case QUESTEXP_REWARD_EXP:
		{
			RewardExp* pRewardExp = static_cast<RewardExp*>(pQuestExpReward);

			UINT64 iExp = pRewardExp->GetRewardExp();

			if (gObjCheckUsedBuffEffect(&gObj[iObjIndex], BUFFTYPE_PCS_MARK3) || gObjCheckUsedBuffEffect(&gObj[iObjIndex], BUFFTYPE_PCBANG_POINT_MARK3))
			{
				iExp = 0;
			}

			if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(&gObj[iObjIndex]))
			{
				gObj[iObjIndex].m_PlayerData->MasterExperience += iExp;
			}

			else
			{
				gObj[iObjIndex].m_PlayerData->Experience += iExp;
			}

			if (gObjLevelUp(&gObj[iObjIndex], iExp, 0, Lang.GetText(0,308)) == true)
			{
				GSProtocol.GCKillPlayerMasterExpSend(iObjIndex, -1, iExp, 0, 0);
			}

			strcat(LogBuff, "Reward Exp [%s][%s] Exp[%I64d] Ep[%d] QS[%d]");
			g_Log.Add(LogBuff, gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, pRewardExp->GetRewardExp(), iEpisode, iQuestSwitch);
		}
		break;

		case QUESTEXP_REWARD_ZEN:
		{
			RewardZen* pRewardZen = static_cast<RewardZen*>(pQuestExpReward);

			gObj[iObjIndex].m_PlayerData->Money += pRewardZen->GetRewardZen();
			GSProtocol.GCMoneySend(iObjIndex, gObj[iObjIndex].m_PlayerData->Money);

			strcat(LogBuff, "Reward Money [%s][%s] Money[%d] Ep[%d] QS[%d]");
			g_Log.Add(LogBuff, gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, pRewardZen->GetRewardZen(), iEpisode, iQuestSwitch);
		}
		break;

		case QUESTEXP_REWARD_ITEM:
		{
			RewardItem* pRewardItem = static_cast<RewardItem*>(pQuestExpReward);
			int RewardItemCnt = pRewardItem->GetRewardItemCnt();

			for (int i = 0; i < RewardItemCnt; ++i)
			{
				// Pentagram Jewel Addon (errtels)
				BYTE btPentagramMainAttribute = -1;
				BYTE btSocketOption[5];
				int itemtype = pRewardItem->GetRewardItemNum();

				if (itemtype != ITEMGET(12, 144) && itemtype != ITEMGET(12, 145) && itemtype != ITEMGET(12, 221)
					&& itemtype != ITEMGET(12, 231) && itemtype != ITEMGET(12, 241) && itemtype != ITEMGET(12, 251) && itemtype != ITEMGET(12, 261))
				{
					for (int j = 0; j < 5; j++)
					{
						btSocketOption[j] = 0xFF;
					}
				}

				else
				{
					BYTE btAttribute = rand() % 5 + 1;
					btPentagramMainAttribute = btAttribute | 0x10;

					for (int k = 0; k < 5; k++)
					{
						if (k)
						{
							btSocketOption[k] = -1;
						}

						else
						{
							btSocketOption[k] = 1;
						}
					}
				}

				ItemSerialCreateSend(iObjIndex, 233, gObj[iObjIndex].X, gObj[iObjIndex].Y,
					pRewardItem->GetRewardItemNum(), pRewardItem->GetRewardItemLevel(),
					pRewardItem->GetRewardItemDur(), pRewardItem->GetRewardItemSkill(),
					0, pRewardItem->GetRewardItemOpt(), iObjIndex,
					pRewardItem->GetRewardItemExOpt(), 0, 0, btSocketOption, btPentagramMainAttribute);

				char szRewardItemName[32] = "";

				int reward_item_number = pRewardItem->GetRewardItemNum();
				ITEM_ATTRIBUTE* pItemAttribute = GetItemAttr(reward_item_number);

				if (pItemAttribute)
				{
					strcpy(szRewardItemName, pItemAttribute->Name);
				}

				char ItemLogBuff[128] = "";

				strcpy(ItemLogBuff, LogBuff);
				strcat(ItemLogBuff, "Reward Item [%s][%s] Ep[%d] QS[%d] ItemName[%s] Item Num[%d] Level[%d] Dur[%d] skill[%d]");

				g_Log.Add(ItemLogBuff, gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode,
					iQuestSwitch, szRewardItemName, pRewardItem->GetRewardItemNum(), pRewardItem->GetRewardItemLevel(), pRewardItem->GetRewardItemDur(), pRewardItem->GetRewardItemSkill());
			}
		}
		break;

		case QUESTEXP_REWARD_CONTRIBUTION_POINT:
		{
			RewardContribute* pRewardContribute = static_cast<RewardContribute*>(pQuestExpReward);

			int iContributePt = pRewardContribute->GetRewardContributePoint();

			g_GensSystem.AddContributePoint(&gObj[iObjIndex], iContributePt);

			strcat(LogBuff, "Reward Contribute Point [%s][%s] Point[%d] Ep[%d] QS[%d]");
			g_Log.Add(LogBuff, gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iContributePt, iEpisode, iQuestSwitch);

			char client_msg[128] = "";
			wsprintf(client_msg, Lang.GetText(0,564), iContributePt, g_GensSystem.GetContributePoint(&gObj[iObjIndex]));

			GSProtocol.GCServerMsgStringSend(client_msg, iObjIndex, 1);
		}
		break;

		default:
			continue;
		}
	}

	InitUserQuestAskInfo(iEpisode, iObjIndex);
	g_QuestExpProgMng.GCANSQuestCompleteBtnClick(iObjIndex, dwQuestInfoIndexID, 1);

	return iResult;
}

void QuestExpUserMng::InitUserQuestAskInfo(int iEpisode, int iObjIndex)
{
	if (IsEpisode(iEpisode) == false)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].QuestAskInfoClear();

}

void QuestExpUserMng::SetQuestSwitch(int iEpisode, int iQuestSwitch, int iObjIndex)
{
	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);
	UserQuestInfo* pUserQuestInfo = GetUserQuestInfo(iEpisode, iObjIndex);

	if (!pUserQuestInfo)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	DWORD dwGetUserQuestInfoIndexID = GetQuestInfoIndexId(pUserQuestInfo->GetEpisode(), pUserQuestInfo->GetQuestSwitch());

	if (!g_QuestExpProgMng.ChkQuestIndexIDToEpAndQSLimit(dwQuestInfoIndexID, iObjIndex))
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	if (dwGetUserQuestInfoIndexID == dwQuestInfoIndexID)
	{
		return;
	}

	if (pUserQuestInfo->SetEpisode(iEpisode) == false)
	{
		int iUserQuestSwitch = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestSwitch();
		int iUserEpisode = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetEpisode();

		g_Log.Add("[QuestExp] - Error - SetQuestSwitch [%s][%s] Cur EP[%d] Cur QS[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iUserEpisode, iUserQuestSwitch);
		return;
	}

	if (pUserQuestInfo->SetQuestSwitch(iQuestSwitch) == false)
	{
		int iUserQuestSwitch = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestSwitch();
		int iUserEpisode = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetEpisode();

		g_Log.Add("[QuestExp] - Error - SetQuestSwitch [%s][%s] Cur EP[%d] Cur QS[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iUserEpisode, iUserQuestSwitch);
		return;
	}

	int iUserQuestSwitch = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestSwitch();
	int iUserEpisode = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetEpisode();

	g_Log.Add("[QuestExp] Set Quest Switch [%s][%s] EP[%d] QS[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iUserEpisode, iUserQuestSwitch);

	if (this->AddUserQuestAskInfo(dwQuestInfoIndexID, iObjIndex) == true)
	{
		gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].SetQuestProgState(QUESTEXP_PROG_STATE_ONGOING);
	}
}

bool QuestExpUserMng::AddUserQuestAskInfo(DWORD dwQuestInfoIndexID, int iObjIndex)
{
	QuestExpInfo* pQuestExpInfo = g_QuestExpManager.GetQuestExpInfo(dwQuestInfoIndexID);

	if (!pQuestExpInfo)
	{
		return false;
	}

	int iEpisode = GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID);
	auto MapQuestExpAsk = pQuestExpInfo->GetQuestExpAsk();

	int iAskCnt = 0;
	UserQuestInfo* pUserQuestInfo = &gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode];

	pUserQuestInfo->SetAskCnt(MapQuestExpAsk.size());
	auto map_iter = MapQuestExpAsk.begin();
	auto map_end = MapQuestExpAsk.end();

	while (1)
	{
		if (map_iter == map_end)
			break;

		QuestExpAsk* pQuestExpAsk = map_iter->second;

		if (!pQuestExpAsk)
		{
			g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
			return false;
		}

		if (IsQuestAskInfo(iAskCnt) == false)
		{
			g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
			return false;
		}

		int iQuestType = pQuestExpAsk->GetQuestType();

		switch (iQuestType)
		{
		case QUESTEXP_ASK_NONE:
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(true);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_NONE);
			pUserQuestInfo->SetAskCnt(0);
			break;

		case QUESTEXP_ASK_TUTORIAL_KEY:
		{
			QuestTutorial* pQuestTutorial = static_cast<QuestTutorial*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestTutorial->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_TUTORIAL_KEY);
		}
		break;

		case QUESTEXP_ASK_MONSTER:
			AddUserQuestAskMonsterKill(pQuestExpAsk, iObjIndex, iEpisode, iAskCnt);
			break;

		case QUESTEXP_ASK_SKILL_LEARN:
		{
			QuestSkillLearn* pQuestSkillLearn = static_cast<QuestSkillLearn*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestSkillLearn->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_SKILL_LEARN);
		}
		break;

		case QUESTEXP_ASK_GET_ITEM:
		{
			QuestGetItem* pQuestGetItem = static_cast<QuestGetItem*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestGetItem->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_GET_ITEM);
		}
		break;

		case QUESTEXP_ASK_LEVEL_UP:
		{
			QuestLevelUp* pQuestLevelUp = static_cast<QuestLevelUp*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestLevelUp->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_LEVEL_UP);
		}
		break;

		case QUESTEXP_ASK_BUFF:
		{
			QuestBuff* pQuestBuff = static_cast<QuestBuff*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestBuff->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_BUFF);
		}
		break;

		case QUESTEXP_ASK_CHAOSCASTLE_USER_KILL:
		case QUESTEXP_ASK_CHAOSCASTLE_MON_KILL:
		case QUESTEXP_ASK_BLOODCASTLE_DOOR_KILL:
		{
			QuestEventMapKillPoint* pQuestEventMapKillPoint = static_cast<QuestEventMapKillPoint*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestEventMapKillPoint->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(pQuestEventMapKillPoint->GetQuestType());
		}
		break;

		case QUESTEXP_ASK_BLOODCASTLE_CLEAR:
		case QUESTEXP_ASK_CHAOSCASTLE_CLEAR:
		case QUESTEXP_ASK_DEVILSQUARE_CLEAR:
		case QUESTEXP_ASK_ILLUSIONTEMPLE_CLEAR:
		{
			QuestEventMapClear* pQuestEventMapClear = static_cast<QuestEventMapClear*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestEventMapClear->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(pQuestEventMapClear->GetQuestType());
		}
		break;

		case QUESTEXP_ASK_DEVILSQUARE_POINT_GAIN:
		{
			QuestEventMapDevilPoint* pQuestEventMapDevilPoint = static_cast<QuestEventMapDevilPoint*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestEventMapDevilPoint->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_DEVILSQUARE_POINT_GAIN);
		}
		break;

		case QUESTEXP_ASK_NEED_ZEN:
		{
			QuestNeedZen* pQuestNeedZen = static_cast<QuestNeedZen*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestNeedZen->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_NEED_ZEN);
		}
		break;

		case QUESTEXP_ASK_CONTRIBUTION_POINT:
		{
			QuestPVP_Point* pQuestPVP_Point = static_cast<QuestPVP_Point*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestPVP_Point->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_CONTRIBUTION_POINT);
		}

		break;

		case QUESTEXP_ASK_NPC_TALK:
		{
			QuestNpcTalk* pQuestNpcTalk = static_cast<QuestNpcTalk*>(pQuestExpAsk);

			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestNpcTalk->GetIndexID());
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
			pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_NPC_TALK);
		}
		break;

		default:
			return false;
		}

		++map_iter;
		iAskCnt++;
	}

	return true;
}

void QuestExpUserMng::AddUserQuestAskMonsterKill(QuestExpAsk* pQuestExpAsk, int iObjIndex, int iEpisode, int iAskCnt)
{
    QuestMonsterKill* pQuestMonsterKill = static_cast<QuestMonsterKill*>(pQuestExpAsk);
    UserQuestInfo* pUserQuestInfo = &gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode];

    pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetIndexID(pQuestMonsterKill->GetIndexID());
    pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetComplete(false);
    pUserQuestInfo->m_UserQuestAskInfo[iAskCnt].SetQuestType(QUESTEXP_ASK_MONSTER);
}

int QuestExpUserMng::GetQuestSwitch(int iEpisode, int iObjIndex)
{
	if (IsEpisode(iEpisode) == false)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return -1;
	}

	return gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestSwitch();

}

struct PMSG_QUESTEXP_INFO
{
    PWMSG_HEAD h;
    BYTE btQuestCnt;
    char szCharName[MAX_ACCOUNT_LEN+1]; 
};

struct QUESTEXP_INFO
{
    DWORD dwQuestIndexID;
    WORD wProgState;
    BYTE btAskIndex[MAX_QUESTEXP_ASK_INFO];
    BYTE btAskValue[MAX_QUESTEXP_ASK_INFO];
    BYTE btAskState[MAX_QUESTEXP_ASK_INFO];
    time_t lStartDate;
    time_t lEndDate;

    QUESTEXP_INFO()
    {
        this->dwQuestIndexID = 0;
        this->wProgState = 0;
        this->lStartDate = 0;
        this->lEndDate = 0;

        memset(btAskIndex, 0, sizeof(btAskIndex));
        memset(btAskValue, 0, sizeof(btAskValue));
        memset(btAskState, 0, sizeof(btAskState));
    }

};

void QuestExpUserMng::UserAllQuestInfoSave(int iObjIndex)
{
	BYTE sendBuff[2048] = { 0 };

	int iIndex = iObjIndex;
	QUESTEXP_INFO QuestInfo[MAX_QUESTEXP_EPISODE];
	PMSG_QUESTEXP_INFO pMsg;
	int lOfs = sizeof(pMsg);

	strcpy(pMsg.szCharName, gObj[iIndex].Name);
	int iEpCnt = 1;
	int iQuestCnt = 0;

	while (iEpCnt < MAX_QUESTEXP_EPISODE)
	{
		UserQuestInfo* pUserQuestInfo = &gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpCnt];
		int iEpisode = pUserQuestInfo->GetEpisode();
		int iQuestSwitch = pUserQuestInfo->GetQuestSwitch();

		if (iEpisode > 0)
		{
			QuestInfo[iQuestCnt].dwQuestIndexID = GetQuestInfoIndexId(iEpisode, iQuestSwitch);
			QuestInfo[iQuestCnt].wProgState = pUserQuestInfo->GetQuestProgState();
			QuestInfo[iQuestCnt].lStartDate = pUserQuestInfo->GetStartDate();
			QuestInfo[iQuestCnt].lEndDate = pUserQuestInfo->GetEndDate();
			int iAskCnt = pUserQuestInfo->GetAskCnt();

			if (iAskCnt == 0)
			{
				g_Log.Add("[QuestExp] AllQuestInfoSave [%s][%s] EP[%d] QS[%d]", gObj[iIndex].AccountID, gObj[iIndex].Name, iEpisode, iQuestSwitch);
			}

			for (int i = 0; i < iAskCnt; ++i)
			{
				QuestInfo[iQuestCnt].btAskIndex[i] = pUserQuestInfo->m_UserQuestAskInfo[i].GetIndexID();
				QuestInfo[iQuestCnt].btAskValue[i] = pUserQuestInfo->m_UserQuestAskInfo[i].GetValue();
				QuestInfo[iQuestCnt].btAskState[i] = pUserQuestInfo->m_UserQuestAskInfo[i].IsComplete();

				g_Log.Add("[QuestExp] AllQuestInfoSave [%s][%s] EP[%d] QS[%d] AskIndex[0x%x] AskValue[%d] AskState[%d]",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iEpisode, iQuestSwitch, QuestInfo[iQuestCnt].btAskIndex[i], QuestInfo[iQuestCnt].btAskValue[i], QuestInfo[iQuestCnt].btAskState[i]);
			}

			memcpy(&sendBuff[lOfs], &QuestInfo[iQuestCnt], sizeof(QUESTEXP_INFO));
			lOfs += sizeof(QUESTEXP_INFO);
			iQuestCnt++;
		}

		iEpCnt++;
	}

	if (iQuestCnt > 0)
	{
		pMsg.btQuestCnt = iQuestCnt;
		PHeadSetW(reinterpret_cast<BYTE*>(&pMsg), 0xF6, lOfs);
		memcpy(sendBuff, &pMsg, sizeof(pMsg));

		wsDataCli.DataSend(reinterpret_cast<char*>(sendBuff), lOfs);
	}
}

void QuestExpUserMng::UserQuestInfoSave(DWORD dwQuestInfoIndexID, int iObjIndex)
{
	BYTE send_buf[2048] = { 0 };

	int iIndex = iObjIndex;
	QUESTEXP_INFO QuestInfo;
	PMSG_QUESTEXP_INFO pMsg;
	strcpy(pMsg.szCharName, gObj[iIndex].Name);

	int iEpisode = GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID);
	int iQuestSwitch = GetQuestSwitchFromInfoIndexId(dwQuestInfoIndexID);
	QuestInfo.dwQuestIndexID = dwQuestInfoIndexID;

	int iAskCnt = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetAskCnt();
	WORD wProgState = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetQuestProgState();

	QuestInfo.lStartDate = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetStartDate();
	QuestInfo.lEndDate = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].GetEndDate();
	QuestInfo.wProgState = wProgState;

	if (iAskCnt == 0)
	{
		g_Log.Add("[QuestExp] QuestInfoSave [%s][%s] EP[%d] QS[%d]", gObj[iIndex].AccountID, gObj[iIndex].Name, iEpisode, iQuestSwitch);
	}

	for (int i = 0; i < iAskCnt; i++)
	{
		QuestInfo.btAskIndex[i] = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].m_UserQuestAskInfo[i].GetIndexID();
		QuestInfo.btAskValue[i] = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].m_UserQuestAskInfo[i].GetValue();
		QuestInfo.btAskState[i] = gObj[iIndex].m_PlayerData->m_UserQuestInfo[iEpisode].m_UserQuestAskInfo[i].IsComplete();

		g_Log.Add("[QuestExp] QuestInfoSave [%s][%s] EP[%d] QS[%d] AskIndex[0x%x] AskValue[%d] AskState[%d]",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iEpisode, iQuestSwitch, QuestInfo.btAskIndex[i], QuestInfo.btAskValue[i], QuestInfo.btAskState[i]);
	}

	pMsg.btQuestCnt = 1;
	memcpy(&send_buf[sizeof(pMsg)], &QuestInfo, sizeof(QuestInfo));

	PHeadSetW(reinterpret_cast<BYTE*>(&pMsg), 0xF6, sizeof(pMsg) + sizeof(QuestInfo));
	memcpy(send_buf, &pMsg, sizeof(pMsg));

	wsDataCli.DataSend(reinterpret_cast<char*>(send_buf), sizeof(pMsg) + sizeof(QuestInfo));
}

struct PMSG_REQ_QUESTEXP_INFO
{
    PBMSG_HEAD h;
    int iUserIndex;
    char szCharName[MAX_ACCOUNT_LEN+1];
};

void QuestExpUserMng::DB_ReqUserQuestInfo(int iObjIndex)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		return;
	}

	PMSG_REQ_QUESTEXP_INFO pMsg;

	pMsg.iUserIndex = iObjIndex;
	pMsg.szCharName[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.szCharName, gObj[iObjIndex].Name, MAX_ACCOUNT_LEN);

	PHeadSetB(reinterpret_cast<BYTE*>(&pMsg), 0xF7, sizeof(pMsg));
	wsDataCli.DataSend(reinterpret_cast<char*>(&pMsg), pMsg.h.size);
}

struct PMSG_ANS_QUESTEXP_INFO
{
    PWMSG_HEAD head;
    BYTE btQuestCnt;
    int iUserIndex;
};

void QuestExpUserMng::UserQuestInfoLoad(PMSG_ANS_QUESTEXP_INFO* lpRecv)
{
	int iQuestCnt = lpRecv->btQuestCnt;
	int iObjIndex = lpRecv->iUserIndex;

	if (!ObjectMaxRange(iObjIndex) || !gObjIsConnected(iObjIndex))
	{
		return;
	}

	if(gObj[iObjIndex].Type != OBJ_USER)
	{
		g_Log.Add("[QuestExp] - Error [%d] iObjIndex is not OBJ_USER- [%s] [%d]", iObjIndex, __FILE__, __LINE__);
		return;
	}

	if (IsQuestExpInfo(iQuestCnt) == false)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	QUESTEXP_INFO QuestInfo[MAX_QUESTEXP_INFO];

	memcpy(QuestInfo, reinterpret_cast<BYTE*>(lpRecv) + sizeof(*lpRecv), iQuestCnt * sizeof(QUESTEXP_INFO));

	for (int i = 0; i < iQuestCnt; i++)
	{
		DWORD dwQuestInfoIndexID = QuestInfo[i].dwQuestIndexID;
		int iEpisode = GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID);
		int iQuestSwitch = GetQuestSwitchFromInfoIndexId(dwQuestInfoIndexID);
		int iAskCnt = 0;

		UserQuestInfo* pUserQuestInfo = &gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode];
		pUserQuestInfo->SetEpisode(iEpisode);
		pUserQuestInfo->SetQuestSwitch(iQuestSwitch);

		WORD wProgState = QuestInfo[i].wProgState;
		pUserQuestInfo->SetQuestProgState(wProgState);

		pUserQuestInfo->SetStartDate(QuestInfo[i].lStartDate);
		pUserQuestInfo->SetEndDate(QuestInfo[i].lEndDate);

		for (int j = 0; j < MAX_QUESTEXP_ASK_INFO; j++)
		{
			if (QuestInfo[i].btAskIndex[j])
			{
				QuestExpAsk* pQuestExpAsk = g_QuestExpManager.GetQuestExpAsk(iEpisode, iQuestSwitch, QuestInfo[i].btAskIndex[j]);

				if (!pQuestExpAsk)
				{
					g_Log.Add("[QuestExp] UserQuestInfo DB Error [%s][%s] - pQuestExpAsk NULL", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name);
					return;
				}

				int iQuestType = pQuestExpAsk->GetQuestType();
				BYTE btAskIndex = QuestInfo[i].btAskIndex[j];
				BYTE btAskValue = QuestInfo[i].btAskValue[j];
				BYTE btAskState = (QuestInfo[i].btAskState[j] == 1);

				pUserQuestInfo->m_UserQuestAskInfo[j].SetQuestType(iQuestType);
				pUserQuestInfo->m_UserQuestAskInfo[j].SetIndexID(btAskIndex);
				pUserQuestInfo->m_UserQuestAskInfo[j].SetValue(btAskValue);
				pUserQuestInfo->m_UserQuestAskInfo[j].SetComplete(btAskState);

				g_Log.Add("[QuestExp] Get DB UserQuestInfo [%s][%s] Ep[%d] QS[%d] QuestType[0x%x] AskIndex[%d] AskValue[%d] State[%d] ProgState[%d]",
					gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode, iQuestSwitch, iQuestType, btAskIndex, btAskValue, btAskState, wProgState);

				iAskCnt++;
			}
		}

		pUserQuestInfo->SetAskCnt(iAskCnt);
	}

	if (gObj[iObjIndex].m_PlayerData->m_bUserQuestInfoSent == false)
	{
		gObj[iObjIndex].m_PlayerData->m_bUserQuestInfoSent = true;

		PBMSG_HEAD2 pMsg;
		PHeadSubSetB(reinterpret_cast<BYTE*>(&pMsg), 0xF6, 0x20, sizeof(pMsg));
		IOCP.DataSend(iObjIndex, reinterpret_cast<BYTE*>(&pMsg), pMsg.size);
	}
}

UserQuestInfo* QuestExpUserMng::GetUserQuestInfo(int iEpisode, int iObjIndex)
{
	if (IsEpisode(iEpisode) == false)
	{
		g_Log.Add("[QuestExp] - Error - GetUserQuestInfo [%s][%s] Ep[%d]", gObj[iObjIndex].AccountID, gObj[iObjIndex].Name, iEpisode);
		return nullptr;
	}
		
	return &gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEpisode];
}

int QuestExpUserMng::GetQuestProgState(DWORD dwQuestInfoIndexID, int iObjIndex)
{
    return this->GetQuestProgState(GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID), iObjIndex);
}

int QuestExpUserMng::GetQuestProgState(int iEp, int iObjIndex)
{
	if (gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEp].GetQuestProgState() == QUESTEXP_PROG_STATE_TIME_LIMIT)
    {
		time_t end_date = gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEp].GetEndDate();

		if (g_QuestExpProgMng.CheckExpireDate(end_date) == true)
		{
			gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEp].SetQuestProgState(QUESTEXP_PROG_STATE_NONE);
		}
    }

	return gObj[iObjIndex].m_PlayerData->m_UserQuestInfo[iEp].GetQuestProgState();
}

bool QuestExpUserMng::IsQuestAccept(DWORD dwQuestInfoIndexID, int iObjIndex)
{
    int iProgCnt = 0;

    for(int i = 0; i < MAX_QUESTEXP_EPISODE; i++ )
    {
		if (IsProgQuestInfo(i, iObjIndex) > 0)
		{
			iProgCnt++;
		}
    }

	if (iProgCnt >= MAX_QUESTEXP_ACTIVE_QUEST)
	{
		if (IsProgQuestInfo(dwQuestInfoIndexID, iObjIndex) <= QUESTEXP_PROG_STATE_NONE)
		{
			return false;
		}
	}

	return true;
}

bool QuestExpUserMng::InventoryExtentCheck(int x, int y, int w, int h)
{
    int nEndOfRange;
    int nStartOfRange;

    if( y < 0 )
        return 0;

    if( y < 8 )
    {
        nStartOfRange = 0;
        nEndOfRange = 8;
    }
    else if( y < 12 )
    {
        nStartOfRange = 8;
        nEndOfRange = 12;
    }
    else if( y < 16 )
    {
        nStartOfRange = 12;
        nEndOfRange = 16;
    }
    else if( y < 20 )
    {
        nStartOfRange = 16;
        nEndOfRange = 20;
    }
    else if( y < 24 )
    {
        nStartOfRange = 20;
        nEndOfRange = 24;
    }
    else if( y < 28 )
    {
        nStartOfRange = 24;
        nEndOfRange = 28;
    }
    else
    {
        return false;
    }

    if( nEndOfRange > h )
        nEndOfRange = h;

    return (x >= 0 && x < w && y >= nStartOfRange && y < nEndOfRange);
}

int QuestExpUserMng::CheckOutOfInventory(int aIndex, int sy, int height)
{
    int nEndOfItemY = sy + height - 1;

	if( sy >= 4 * gObj[aIndex].m_PlayerData->m_InventoryExpansion + 8 )
        return -1;

    if( sy < 0 )
        return 0;

    if( sy < 8 )
    {
        if( nEndOfItemY >= 8 )
            return 0;
    }
    else if( sy < 12 )
    {
        if ( nEndOfItemY >= 12 )
            return 0;
    }
    else if( sy < 16 )
    {
        if( nEndOfItemY >= 16 )
            return 0;
    }
    else if( sy < 20 )
    {
        if( nEndOfItemY >= 20 )
            return 0;
    }
    else if( sy < 24 )
    {
        if( nEndOfItemY >= 24 )
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

