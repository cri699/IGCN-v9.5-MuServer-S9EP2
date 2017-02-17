//////////////////////////////////////////////////////////////////////
// QuestExpProtocol.cpp
#include "StdAfx.h"
#include "TLog.h"
#include "QuestExpProtocol.h"
#include "QuestExpUserMng.h"
#include "QuestExpLuaBind.h"
#include "user.h"
#include "QuestExpProgMng.h"
#include "NpcTalk.h"
#include "winutil.h"
#include "GameMain.h"

void CGReqQuestSwitch(PMSG_REQ_QUESTEXP *pMsg, int aIndex)
{
    if( !ObjectMaxRange(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->dwQuestInfoIndexID, aIndex) )
    {
        g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

	g_Log.Add("[QuestExp] Selection Episode List Choose One [%s][%s] QuestInfoIndexID[0x%x] Choose[%d]",
		gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg->dwQuestInfoIndexID, pMsg->btResult);

	lua_State* L = g_MuLuaQuestExp.GetLua();

	if (!L)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	if (g_QuestExpUserMng.IsQuestAccept(pMsg->dwQuestInfoIndexID, aIndex))
	{
		g_MuLuaQuestExp.Generic_Call("CGReqQuestSwitch", "iii>", (int)pMsg->dwQuestInfoIndexID, (int)pMsg->btResult, aIndex);
	}

    else
    {
        PMSG_ANS_QUESTEXP pAnsMsg;
        pAnsMsg.btResult = 1;

        PHeadSubSetB((LPBYTE)&pAnsMsg, 0xF6, 0x00, sizeof(pAnsMsg));
        IOCP.DataSend(gObj[aIndex].m_Index, (LPBYTE)&pAnsMsg, sizeof(pAnsMsg));
    }
}

void CGReqQuestProgress(PMSG_REQ_QUESTEXP *pMsg, int aIndex)
{
    if( !ObjectMaxRange(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->dwQuestInfoIndexID, aIndex) )
    {
        g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_Log.Add("[QuestExp] Selection Statements Choose One - User NPC Talking [%s][%s] QuestInfoIndexID[0x%x] Choose[%d]",
		gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg->dwQuestInfoIndexID,pMsg->btResult);

	lua_State* L = g_MuLuaQuestExp.GetLua();

	if (!L)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	g_MuLuaQuestExp.Generic_Call("CGReqQuestProgress", "iii>", (int)pMsg->dwQuestInfoIndexID, (int)pMsg->btResult, aIndex);
}

void CGReqQuestComplete(PMSG_REQ_QUESTEXP_COMPLETE *pMsg, int aIndex)
{
    if( !ObjectMaxRange(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->dwQuestInfoIndexID, aIndex) )
    {
        g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_Log.Add("[QuestExp] ReqQuestComplete [%s][%s] QuestInfoIndexID[0x%x]", gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg->dwQuestInfoIndexID);

	lua_State* L = g_MuLuaQuestExp.GetLua();

	if (!L)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	g_MuLuaQuestExp.Generic_Call("CGReqQuestComplete", "ii>", (int)pMsg->dwQuestInfoIndexID, aIndex);
}

void CGReqQuestGiveUp(PMSG_REQ_QUESTEXP_GIVEUP *pMsg, int aIndex)
{
    if( !ObjectMaxRange(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->dwQuestInfoIndexID, aIndex) )
    {
        g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_QuestExpProgMng.QuestExpGiveUpBtnClick(pMsg->dwQuestInfoIndexID, aIndex);
}

void CGReqTutorialKeyComplete(PMSG_REQ_QUESTEXP_ASK_COMPLETE *pMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->dwQuestInfoIndexID, aIndex) )
    {
        g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_QuestExpProgMng.ReqQuestAskComplete(pMsg->dwQuestInfoIndexID, aIndex);
}

void CGReqProgressQuestList(PMSG_REQ_QUESTEXP_PROGRESS_LIST *pMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    g_QuestExpProgMng.SendProgressQuestList(aIndex);
}

void CGReqProgressQuestInfo(PMSG_REQ_QUESTEXP_PROGRESS_INFO *pMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->dwQuestInfoIndexID, aIndex) )
    {
        g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;

    }    

    g_QuestExpProgMng.SendQuestProgressInfo(pMsg->dwQuestInfoIndexID, aIndex);
}

void CGReqEventItemQuestList(PMSG_REQ_EVENT_ITEM_EP_LIST *pMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
        return;

    if( !gObjIsConnected(aIndex) )
        return;

	lua_State* L = g_MuLuaQuestExp.GetLua();

	if (!L)
	{
		g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	g_MuLuaQuestExp.Generic_Call("ItemAndEvent", "i>", aIndex);
}

void CGReqQuestExp(PMSG_REQ_NPC_QUESTEXP *pMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

	if( !ObjectMaxRange(lpObj->TargetNpcNumber))
		return;

    if( lpObj->m_PlayerData->m_bUserQuestInfoSent == 1 )
    {
		lua_State* L = g_MuLuaQuestExp.GetLua();

		if (!L)
		{
			g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
			return;
		}

		g_MuLuaQuestExp.Generic_Call("NpcTalkClick", "ii>", (int)gObj[lpObj->TargetNpcNumber].Class, lpObj->m_Index);
    }
}

void CGReqAttDefPowerInc(PMSG_REQ_ATTDEF_POWER_INC *pMsg, int aIndex)
{
	if (!ObjectMaxRange(aIndex))
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    NpcShadowPhantom(aIndex);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

