//////////////////////////////////////////////////////////////////////
// QuestExpInfo.cpp
#include "StdAfx.h"
#include "QuestExpInfo.h"
#include "TLog.h"

QuestExpInfo::QuestExpInfo()
{
	this->m_iIndexID = 0;
	this->m_iRewardID = 0;
	this->m_iQuestEpisode = 0;
	this->m_iQuestSwitch = 0;
}

QuestExpInfo::~QuestExpInfo()
{

}

void QuestExpInfo::SetQuestExpAsk(QuestExpAsk *pQuestExpAsk)
{
	int iIndexID = pQuestExpAsk->GetIndexID();

	if (IsQuestAskInfo(iIndexID) == false)
	{
		g_Log.MsgBox("[QuestExp] - Error - AskIndex Overflow iIndexID [%d] [%s] [%d]", iIndexID, __FILE__, __LINE__);
		return;
	}

	this->m_MapQuestExpAsk[iIndexID] = pQuestExpAsk;
}

void QuestExpInfo::SetQuestExpAsk(QuestExpAsk *pQuestExpAsk, int iIndexID)
{
	if (IsQuestAskInfo(iIndexID) == false)
	{
		g_Log.MsgBox("[QuestExp] - Error - AskIndex Overflow iIndexID [%d] [%s] Line:[%d]", iIndexID, __FILE__, __LINE__);
		return;
	}

	this->m_MapQuestExpAsk[iIndexID] = pQuestExpAsk;
	pQuestExpAsk->SetIndexID(iIndexID);
}

std::map<int, QuestExpAsk*> QuestExpInfo::GetQuestExpAsk()
{
	return this->m_MapQuestExpAsk;
}

void QuestExpInfo::SetQuestReward(QuestExpReward *pQuestExpReward)
{
	if (IsQuestRewardInfo(this->m_iRewardID) == false)
	{
		g_Log.MsgBox("[QuestExp] - Error - RewardIndex Overflow m_iRewardID [%d] [%s] Line:[%d]", this->m_iRewardID, __FILE__, __LINE__);
		return;
	}

	this->m_MapQuestReward[this->m_iRewardID] = pQuestExpReward;
	pQuestExpReward->SetIndexID(this->m_iRewardID++);
}

std::map<int, QuestExpReward*> QuestExpInfo::GetQuestReward()
{
    return this->m_MapQuestReward;
}

void QuestExpInfo::Release()
{
	auto ask_it = this->m_MapQuestExpAsk.begin();
	auto ask_end = this->m_MapQuestExpAsk.end();

	for (; ask_it != ask_end; ask_it++)
	{
		delete ask_it->second;
		ask_it->second = nullptr;
	}

	auto reward_it = this->m_MapQuestReward.begin();
	auto reward_end = this->m_MapQuestReward.end();

	for (; reward_it != reward_end; reward_it++)
	{
		delete reward_it->second;
		reward_it->second = nullptr;
	}
}

QuestExpManager g_QuestExpManager;

QuestExpManager::QuestExpManager()
{

}

QuestExpManager::~QuestExpManager()
{
    auto kind_it = this->m_MapQuestExpRewardKind.begin();
    auto kind_end = this->m_MapQuestExpRewardKind.end();

    for(; kind_it != kind_end; ++kind_it)
    {
        delete kind_it->second;
        kind_it->second = nullptr;
    }
}

void QuestExpManager::QuestExpItemInit()
{
    auto vt_it = this->m_vtQuestItemInfo.begin();
    auto vt_end = this->m_vtQuestItemInfo.end();

    for(; vt_it != vt_end; ++vt_it)
    {
        ItemAttribute[vt_it->iItemNum].QuestItem = true;
    }
}

void QuestExpManager::QuestInfoDelete()
{
    auto map_it = this->m_Map_QuestExpInfoList.begin();
    auto map_end = this->m_Map_QuestExpInfoList.end();

    for(; map_it != map_end; ++map_it)
    {
        map_it->second.Release();
		this->m_Map_QuestExpInfoList.erase(map_it);
    }
}

void QuestExpManager::SetQuestExpInfo(QuestExpAsk *pQuestExpAsk, int iEpisodeNum, int iQuestSwitch)
{
	QuestExpAsk* pQuestAsk = nullptr;
	int iQuestType = pQuestExpAsk->GetQuestType();

	switch (iQuestType)
	{
	case QUESTEXP_ASK_NONE:
		pQuestAsk = new QuestProgress(*static_cast<QuestProgress*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_MONSTER:
		pQuestAsk = new QuestMonsterKill(*static_cast<QuestMonsterKill*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_TUTORIAL_KEY:
		pQuestAsk = new QuestTutorial(*static_cast<QuestTutorial*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_GET_ITEM:
		pQuestAsk = new QuestGetItem(*static_cast<QuestGetItem*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_SKILL_LEARN:
		pQuestAsk = new QuestSkillLearn(*static_cast<QuestSkillLearn*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_LEVEL_UP:
		pQuestAsk = new QuestLevelUp(*static_cast<QuestLevelUp*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_BUFF:
		pQuestAsk = new QuestBuff(*static_cast<QuestBuff*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_CHAOSCASTLE_USER_KILL:
	case QUESTEXP_ASK_CHAOSCASTLE_MON_KILL:
	case QUESTEXP_ASK_BLOODCASTLE_DOOR_KILL:
		pQuestAsk = new QuestEventMapKillPoint(*static_cast<QuestEventMapKillPoint*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_BLOODCASTLE_CLEAR:
	case QUESTEXP_ASK_CHAOSCASTLE_CLEAR:
	case QUESTEXP_ASK_DEVILSQUARE_CLEAR:
	case QUESTEXP_ASK_ILLUSIONTEMPLE_CLEAR:
		pQuestAsk = new QuestEventMapClear(*static_cast<QuestEventMapClear*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_DEVILSQUARE_POINT_GAIN:
		pQuestAsk = new QuestEventMapDevilPoint(*static_cast<QuestEventMapDevilPoint*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_NEED_ZEN:
		pQuestAsk = new QuestNeedZen(*static_cast<QuestNeedZen*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_CONTRIBUTION_POINT:
		pQuestAsk = new QuestPVP_Point(*static_cast<QuestPVP_Point*>(pQuestExpAsk));
		break;

	case QUESTEXP_ASK_NPC_TALK:
		pQuestAsk = new QuestNpcTalk(*static_cast<QuestNpcTalk*>(pQuestExpAsk));
		break;

	default:
		break;
	}

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisodeNum, iQuestSwitch);
	QuestExpInfo* pQuestExpInfo = this->GetQuestExpInfo(dwQuestInfoIndexID);

	if (pQuestExpInfo)
	{
		pQuestExpInfo->SetQuestExpAsk(pQuestAsk);
	}

	else
	{
		this->AddQuestExpInfoList(pQuestAsk, dwQuestInfoIndexID);
	}
}

void QuestExpManager::AddQuestExpInfoList(QuestExpAsk* pQuestExpAsk, DWORD dwQuestInfoIndexID)
{
	QuestExpInfo questExpInfo;

	questExpInfo.SetQuestExpAsk(pQuestExpAsk);
	this->AddQuestExpInfoList(&questExpInfo, dwQuestInfoIndexID);
}

void QuestExpManager::SetQuestExpInfo(QuestExpReward *pQuestExpReward, int iEpisodeNum, int iQuestSwitch)
{
	QuestExpReward* pQuestReward = nullptr;

	switch (pQuestExpReward->GetRewardType())
	{
	case QUESTEXP_REWARD_EXP:
		pQuestReward = new RewardExp(*static_cast<RewardExp*>(pQuestExpReward));
		break;

	case QUESTEXP_REWARD_ZEN:
		pQuestReward = new RewardZen(*static_cast<RewardZen*>(pQuestExpReward));
		break;

	case QUESTEXP_REWARD_ITEM:
		pQuestReward = new RewardItem(*static_cast<RewardItem*>(pQuestExpReward));
		break;

	case QUESTEXP_REWARD_CONTRIBUTION_POINT:
		pQuestReward = new RewardContribute(*static_cast<RewardContribute*>(pQuestExpReward));
		break;

	default:
		break;
	}

	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisodeNum, iQuestSwitch);
	QuestExpInfo* pQuestExpInfo = this->GetQuestExpInfo(dwQuestInfoIndexID);

	if (pQuestExpInfo)
	{
		pQuestExpInfo->SetQuestReward(pQuestReward);
	} 

	else
	{
		this->AddQuestExpInfoList(pQuestReward, dwQuestInfoIndexID);
	}
}

void QuestExpManager::AddQuestExpInfoList(QuestExpReward *pQuestExpReward, DWORD dwQuestInfoIndexID)
{
    QuestExpInfo questExpInfo;

    questExpInfo.SetQuestReward(pQuestExpReward);
    this->AddQuestExpInfoList(&questExpInfo, dwQuestInfoIndexID);
}

void QuestExpManager::AddQuestExpInfoList(QuestExpInfo *questExpInfo, DWORD dwQuestInfoIndexID)
{
    int iEpisodeNum = GetQuestEpisodeFromInfoIndexId(dwQuestInfoIndexID);
    int iQuestSwitch = GetQuestSwitchFromInfoIndexId(dwQuestInfoIndexID);

	questExpInfo->SetQuestEpisode(iEpisodeNum);
	questExpInfo->SetQuestSwitch(iQuestSwitch);

    this->m_Map_QuestExpInfoList[dwQuestInfoIndexID] = *questExpInfo;
}

void QuestExpManager::AddQuestExpRewardKind(QuestExpRewardKind *pQuestExpRewardKind, DWORD dwQuestInfoIndexID)
{
	if (pQuestExpRewardKind == nullptr)
	{
		g_Log.MsgBox("[QuestExp] - Error -  AddQuestExpRewardKind dwQuestInfoIndexID 0x%08x", dwQuestInfoIndexID);
		return;
	}

	QuestExpRewardKind* pRewardKind = nullptr;

	if (pQuestExpRewardKind->GetRewardKind() == true)
	{
		pRewardKind = new QuestExpRewardKindRandom(*static_cast<QuestExpRewardKindRandom*>(pQuestExpRewardKind));
	}

	this->m_MapQuestExpRewardKind[dwQuestInfoIndexID] = pRewardKind;
}

void QuestExpManager::AddQuestExpRewardKind(QuestExpRewardKind *pQuestExpRewardKind)
{
	if (pQuestExpRewardKind == nullptr)
	{
		g_Log.MsgBox("[QuestExp] - Error -  AddQuestExpRewardKind");
		return;
	}

	int iEpisodeNum = pQuestExpRewardKind->GetQuestEpisode();
	int iQuestSwitch = pQuestExpRewardKind->GetQuestSwitch();
	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisodeNum, iQuestSwitch);

	this->AddQuestExpRewardKind(pQuestExpRewardKind, dwQuestInfoIndexID);
}

QuestExpRewardKind* QuestExpManager::GetQuestExpRewardKind(DWORD dwQuestInfoIndexID)
{
	auto map_it = this->m_MapQuestExpRewardKind.find(dwQuestInfoIndexID);
	auto map_end = this->m_MapQuestExpRewardKind.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return map_it->second;
	}
}

QuestExpAsk* QuestExpManager::GetQuestExpAsk(int iEpisodeNum, int iQuestSwitch, int iIndexID)
{
	QuestExpInfo* pQuestExpInfo = GetQuestExpInfo(iEpisodeNum, iQuestSwitch);

	if (pQuestExpInfo == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return nullptr;
	}

	auto MapQuestExpAsk = pQuestExpInfo->GetQuestExpAsk();

	auto map_it = MapQuestExpAsk.find(iIndexID);
	auto map_end = MapQuestExpAsk.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return map_it->second;
	}
}

QuestExpAsk* QuestExpManager::GetQuestExpAsk(DWORD dwQuestIndexID, int iIndexID)
{
	QuestExpInfo* pQuestExpInfo = GetQuestExpInfo(dwQuestIndexID);

	if (pQuestExpInfo == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return nullptr;
	}

	auto MapQuestExpAsk = pQuestExpInfo->GetQuestExpAsk();
	auto map_it = MapQuestExpAsk.find(iIndexID);
	auto map_end = MapQuestExpAsk.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return map_it->second;
	}
}

QuestExpReward* QuestExpManager::GetQuestExpReward(int iEpisodeNum, int iQuestSwitch, int iIndexID)
{
	QuestExpInfo* pQuestExpInfo = GetQuestExpInfo(iEpisodeNum, iQuestSwitch);

	if (pQuestExpInfo == nullptr)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return nullptr;
	}

	auto MapQuestReward = pQuestExpInfo->GetQuestReward();

	auto map_it = MapQuestReward.find(iIndexID);
	auto map_end = MapQuestReward.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return map_it->second;
	}
}

QuestExpInfo* QuestExpManager::GetQuestExpInfo(DWORD dwQuestInfoIndexID)
{
	auto map_it = this->m_Map_QuestExpInfoList.find(dwQuestInfoIndexID);
	auto map_end = this->m_Map_QuestExpInfoList.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return &map_it->second;
	}
}

QuestExpInfo* QuestExpManager::GetQuestExpInfo(int iEpisodeNum, int iQuestSwitch)
{
	DWORD dwQuestInfoIndexID = GetQuestInfoIndexId(iEpisodeNum, iQuestSwitch);

	auto map_it = this->m_Map_QuestExpInfoList.find(dwQuestInfoIndexID);
	auto map_end = this->m_Map_QuestExpInfoList.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return &map_it->second;
	}
}

void QuestExpManager::AddQuestDropItemInfo(QuestDropItemInfo *pQuestDropItemInfo, DWORD dwQuestInfoIndexID)
{
	this->m_MapQuestDropItemInfo[dwQuestInfoIndexID] = *pQuestDropItemInfo;
}

QuestDropItemInfo* QuestExpManager::GetQuestDropItemInfo(DWORD dwQuestInfoIndexID)
{
	auto map_it = this->m_MapQuestDropItemInfo.find(dwQuestInfoIndexID);
	auto map_end = this->m_MapQuestDropItemInfo.end();

	if (map_it == map_end)
	{
		return nullptr;
	}

	else
	{
		return &map_it->second;
	}
}

void QuestExpManager::AddQuestItemInfo(QUEST_ITEM_INFO stQuestItemList)
{
	this->m_vtQuestItemInfo.push_back(stQuestItemList);
}

bool QuestExpManager::IsQuestItemInfo(int iItemNum)
{
	auto vt_it = this->m_vtQuestItemInfo.begin();
	auto vt_end = this->m_vtQuestItemInfo.end();

	for (; vt_it != vt_end; vt_it++)
	{
		if (vt_it->iItemNum == iItemNum)
		{
			return true;
		}
	}

	return false;
}

bool QuestExpManager::IsQuestItemAtt(int iItemNum, DWORD dwQuestItemAtt)
{
	auto vt_it = this->m_vtQuestItemInfo.begin();
	auto vt_end = this->m_vtQuestItemInfo.end();

	for (; vt_it != vt_end; vt_it++)
	{
		if (vt_it->iItemNum == iItemNum && (vt_it->dwQuestItemAtt & dwQuestItemAtt) == dwQuestItemAtt)
		{
			return true;
		}
	}

	return false;
}

int QuestExpManager::GetQuestItemEp(int iItemNum)
{
	auto vt_it = this->m_vtQuestItemInfo.begin();
	auto vt_end = this->m_vtQuestItemInfo.end();

	for (; vt_it != vt_end; vt_it++)
	{
		if (vt_it->iItemNum == iItemNum)
		{
			return vt_it->iEp;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

