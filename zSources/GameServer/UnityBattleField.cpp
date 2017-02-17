//////////////////////////////////////////////////////////////////////
// UnityBattleField.cpp
#include "stdafx.h"
#include "UnityBattleField.h"
#include "TLog.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "winutil.h"
#include "PersonalStore.h"
#include "configread.h"

CUnityBattleField g_UnityBattleField;

CUnityBattleField::CUnityBattleField(void)
{
}

CUnityBattleField::~CUnityBattleField(void)
{
}

void CUnityBattleField::LoadData(char *FileName)
{
	CIniReader ReadBattleCore(FileName);

	this->m_bUBFEnable = ReadBattleCore.ReadInt("BattleCore", "IsBattleCoreEvent", 1);
	this->m_iUBFPopupType = ReadBattleCore.ReadInt("BattleCore", "IsNoticeWindow", 1);
	this->m_bUBFCharacterPromotion = ReadBattleCore.ReadInt("BattleCore", "IsCharacterPromotion", 0);
}

void CUnityBattleField::SendUBFNotice(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (this->m_bUBFEnable == false)
	{
		return;
	}

	GSProtocol.GCSendEventBanner(aIndex, 3);
}

void CUnityBattleField::GDReqJoinUnityBattleField(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (this->m_bUBFEnable == false)
	{
		g_Log.Add("[UBF][GDReqJoinUnityBattleField][%s][%s][%s][ServerCode:%d] UBF not available.",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		return;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		g_Log.Add("[UBF][GDReqJoinUnityBattleField][%s][%s][%s][ServerCode:%d] In UBF, Can not be Requested Join",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		return;
	}

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapSvrMoveReq == true || lpObj->m_bMapSvrMoveReq_2 == true )
	{
		g_Log.Add("[UBF][GDReqJoinUnityBattleField][%s][%s][%s][ServerCode:%d] Map Of Move request was trying to Join UnityBattleField.",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		return;
	}

	if ( lpObj->m_IfState.use && lpObj->m_IfState.type == 1 )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,634), aIndex, 1);
		g_Log.Add("[UBF][GDReqJoinUnityBattleField][%s][%s][%s][ServerCode:%d] Trading can't be to Join UnityBattleField.",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		return;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,633), aIndex, 1);
		g_Log.Add("[UBF][GDReqJoinUnityBattleField][%s][%s][%s][ServerCode:%d] Opened PShop can't be to Join UnityBattleField.",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		return;
	}

	g_Log.Add("[UBF][GDReqJoinUnityBattleField][%s][%s] Request To Join UnityBattleField",
		lpObj->AccountID, lpObj->Name);
	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	PMSG_UBF_REGISTER_ACCOUNT_USER pMsg;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szBattleFieldName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.btRegisterState = 1;
	pMsg.btRegisterMonth = sysTime.wMonth;
	pMsg.btRegisterDay = sysTime.wDay;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x02, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CUnityBattleField::DGAnsJoinUnityBattleField(int aIndex, BYTE Result, WORD LeftSecond)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	g_Log.Add("[UBF][DGAnsJoinUnityBattleField][%s][%s] Answer is To Join UBF ResultCode: %d (0:NewJoin Succ/1:ReJoin Succ/2:Joined State/3:Already apply/4:2 PersonUpper/5:Server Limit)",
		lpObj->AccountID, lpObj->Name, Result);

	PMSG_ANS_UBF_JOIN pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xCD, 0x02, sizeof(pMsg));

	pMsg.btResult = Result;
	pMsg.nLeftSec = LeftSecond;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( Result != 1 && Result != 0)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,632), aIndex, 1);
		return;
	}

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,631), aIndex, 1);
	lpObj->m_PlayerData->m_JoinUnityBattle = true;

	if ( lpObj->m_bPShopOpen == true )
	{
		g_PersonalStore.CGPShopReqClose(aIndex);
	}

	this->GDReqCopyCharacterInfo(aIndex, 0);
}

void CUnityBattleField::GDReqCopyCharacterInfo(int aIndex, BYTE CharacterSlot)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		g_Log.Add("[UBF][GDReqCopyCharcterInfo][%s][%s] Move(Copy Off) Fail, Don't Move because UBFServer",
			lpObj->AccountID, lpObj->Name);
		return;
	}

	PMSG_UBF_ACCOUNT_USER_COPY pMsg;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	pMsg.iUserIndex = aIndex;
	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.btPromotionCode = this->m_bUBFCharacterPromotion;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x03, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	g_Log.Add("[UBF][GDReqCopyCharcterInfo][%s][%s] Move(Copy On) the Character Into UnityBattleField ",
		lpObj->AccountID, lpObj->Name);

	GDReqCopyPetItemInfo(lpObj->m_Index);
}

void CUnityBattleField::DGAnsCopyCharacterInfo(int aIndex, BYTE result, BYTE subResult)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( result != TRUE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,630), aIndex, 1);
		g_Log.Add("[UBF][DGAnsCopyCharcterInfo][%s][%s] Character Move(Copy Fail) Result: %d (2,3:Fail), ErrCode: %d (2:No UserInfo / 3: No CharSlot)",
			lpObj->AccountID, lpObj->Name, result, subResult);
		return;
	}

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,629), aIndex, 1);
	g_Log.Add( "[UBF][DGAnsCopyCharcterInfo][%s][%s] Character Move(Copy) Result: %d (1:Succes) ",
		lpObj->AccountID, lpObj->Name, result);

	g_Log.Add("[UBF][DGAnsCopyCharcterInfo][Copy Complete][%s][%s] CharInfoSave : Class=%d Level=%d LVPoint=%d Exp=%I64d Str=%d Dex=%d Vit=%d Energy=%d Leadership:%d Map=%d Pk=%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->DbClass, lpObj->Level, lpObj->m_PlayerData->LevelUpPoint, lpObj->m_PlayerData->Experience, lpObj->m_PlayerData->Strength, lpObj->m_PlayerData->Dexterity,
		lpObj->m_PlayerData->Vitality, lpObj->m_PlayerData->Energy, lpObj->Leadership, lpObj->MapNumber, lpObj->m_PK_Level);
	gObjItemTextSave(lpObj);
	gObjMagicTextSave(lpObj);

	g_Log.Add("[UBF][DGAnsCopyCharacterInfo][%s][%s] Copy Infomation End", lpObj->AccountID, lpObj->Name);
}

void CUnityBattleField::GDReqCheckJoinedUnityBattleField(int aIndex, int IsUnityBattleFieldServer, BYTE ObServerMode)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	PMSG_REQ_UBF_ACCOUNT_USERINFO pMsg;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	pMsg.iUserIndex = aIndex;
	pMsg.iServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.IsUnityBattleFieldServer = g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE ? TRUE : FALSE;
	pMsg.btObserverMode = ObServerMode;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x01, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}
void CUnityBattleField::DGAnsCheckJoinedUnityBattleField(int aIndex, BYTE btRegisterState)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( btRegisterState == TRUE )
	{
		lpObj->m_PlayerData->m_JoinUnityBattle = true;
		if ( lpObj->m_bPShopOpen == true )
		{
			g_PersonalStore.CGPShopReqClose(lpObj->m_Index);
		}
	}

	else
	{
		lpObj->m_PlayerData->m_JoinUnityBattle = false;
	}

	PMSG_ANS_UBF_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xCD, 0x01, sizeof(pMsg));
	pMsg.btResult = btRegisterState;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CUnityBattleField::GDObserverLogoutManager()
{
	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		LPOBJ lpObj = &gObj[n];

		if ( gObjIsConnected(n) == TRUE && lpObj->Type == OBJ_USER )
		{
			this->GDReqCheckJoinedUnityBattleField(n, g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE ? TRUE : FALSE, TRUE);
		}
	}
}

void CUnityBattleField::GDReqCancelUnityBattleField(int aIndex, BYTE btCancelType)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		g_Log.Add("[UBF][GDReqCancelUnityBattleField][%s][%s] In UBF, Can not be canceled", lpObj->AccountID, lpObj->Name);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,628), aIndex, 1);
		return;
	}

	PMSG_UBF_REQ_CANCEL_REGISTER_USER pMsg;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	pMsg.iUserIndex = aIndex;
	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.btCanceled = btCancelType;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x07, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);;
	
	g_Log.Add("[UBF][GDReqCancelUnityBattleField][%s][%s] Request to Cancel UBF",
		lpObj->AccountID, lpObj->Name);
}

void CUnityBattleField::GDReqCancelUnityBattleField(int aIndex, BYTE btCancelType, const char *name)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		g_Log.Add("[UBF][GDReqCancelUnityBattleField][%s][%s] In UBF, Can not be canceled", lpObj->AccountID, lpObj->Name);
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,628), aIndex, 1);
		return;
	}

	PMSG_UBF_REQ_CANCEL_REGISTER_USER pMsg;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, name, MAX_ACCOUNT_LEN+1);
	pMsg.iUserIndex = aIndex;
	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.btCanceled = btCancelType;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x06, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);;
	
	g_Log.Add("[UBF][GDReqCancelUnityBattleField][%s][%s] Request to Cancel UBF",
		lpObj->AccountID, lpObj->Name);
}

void CUnityBattleField::DGAnsCancelUnityBattleField(int aIndex, BYTE aCanceledResult, BYTE deletedResult)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( aCanceledResult == TRUE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,627), aIndex, 1);
		lpObj->m_PlayerData->m_JoinUnityBattle = false;
	}

	g_Log.Add("[UBF][DGAnsCancelUnityBattleField][%s][%s] Answer CancelUnityBattleField CancelResult:%d (1:Success), deleteResult:%d (1:Success) ",
		lpObj->AccountID, lpObj->Name, aCanceledResult, deletedResult);

	PMSG_ANS_UBF_CANCEL pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xCD, 0x07, sizeof(pResult));
	pResult.btResult = aCanceledResult;

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void CUnityBattleField::GDReqGetRealNameAndServerCode(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE )
	{
		return;
	}

	PMSG_REQ_GET_UBF_REAL_NAME pMsg;
	pMsg.iUserIndex = aIndex;
	memcpy(pMsg.szUBFName, gObj[aIndex].Name, MAX_ACCOUNT_LEN+1);

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x08, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void CUnityBattleField::DGAnsGetRealNameAndServerCode(int aIndex, int nServerCodeOfHomeWorld)
{

}

void CUnityBattleField::GDReqUBFGetReward(int aIndex, BYTE btBattleKind)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		return;
	}

	PMSG_REQ_UBF_GET_REWARD pMsg;
	pMsg.iUserIndex = aIndex;
	pMsg.iServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	pMsg.btBattleKind = btBattleKind;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN+1);

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x06, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	g_Log.Add("[UBF][GDReqUBFGetReward][%d][%s][%s] UnityBattleFiled is asking WinnerItem",
		aIndex, lpObj->AccountID, lpObj->Name);
}

void CUnityBattleField::GDReqSetReceivedWinnerItem(int aIndex, BYTE btBattleKind)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	PMSG_REQ_UBF_SET_RECEIVED_REWARD pMsg;
	pMsg.iUserIndex = aIndex;
	pMsg.iServerCode = g_ConfigRead.server.GetGameServerCode() / 20;
	memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN+1);
	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN+1);
	pMsg.btReceivedReward = TRUE;
	pMsg.btBattleKind = btBattleKind;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x05, sizeof(pMsg));
	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	g_Log.Add("[UBF][GDReqSetReceivedWinnerItem] Index:%d ID:%s Name:%s RequestSetReceived WinnerItem ",
		aIndex, lpObj->AccountID, lpObj->Name);
}

void CUnityBattleField::DGAnsSetReceivedWinnerItem(int aIndex, BYTE btReturn)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	if (btReturn == TRUE)
	{
		g_Log.Add("[UBF][DGAnsSetReceivedWinnerItem] Index:%d ID:%s Name:%s Received WinnerItem Success",
			aIndex, lpObj->AccountID, lpObj->Name);
	}

	else
	{
		g_Log.Add("[UBF][DGAnsSetReceivedWinnerItem] Index:%d ID:%s Name:%s WinnerItem Failed to complete the process ErrCode :%d",
			aIndex, lpObj->AccountID, lpObj->Name, btReturn);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

