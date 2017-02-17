// Fprotocol.cpp: implementation of the Fprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Fprotocol.h"
#include "EDSprotocol.h"
#include "TLog.h"
#include "winutil.h"
#include "giocp.h"
#include "ServerEngine.h"
#include <string.h>
using namespace std;

CFriendSystem::CFriendSystem()
{
	this->m_iMaxFriends = 50;
	InitializeCriticalSection(&m_csMapFriendMaster);
}

CFriendSystem::~CFriendSystem()
{
	DeleteCriticalSection(&m_csMapFriendMaster);
}

LPFRIEND_MASTER CFriendSystem::GetFriendMaster(char *szMaster)
{
	if(szMaster == NULL)
		return NULL;

	if(strlen(szMaster) < 4)
		return NULL;

	char szM[11] = {0};
	strncpy(szM, szMaster, 10);

	std::map<string, FRIEND_MASTER, strCmp>::iterator it;
	it = this->m_MapFriendManager.find(szM);
	if(it == this->m_MapFriendManager.end())
		return NULL;

	return &it->second;
}

BOOL CFriendSystem::FriendExists(char *szMaster, char *szFriend)
{
	int i;
	LPFRIEND_MASTER lpMaster;

	char szM[11] = {0};
	strncpy(szM, szMaster, 10);

	char szF[11] = {0};
	strncpy(szF, szFriend, 10);

	EnterCriticalSection(&m_csMapFriendMaster);
	lpMaster = GetFriendMaster(szM);

	if(lpMaster == NULL)
	{
		LeaveCriticalSection(&m_csMapFriendMaster);
		return FALSE;
	}

	int size = (int)lpMaster->m_vecFriends.size();

	for( i=0; i < size; i++)
	{
		if(strcmp(lpMaster->m_vecFriends[i].m_szName, szF) == 0)
			break;
	}

	LeaveCriticalSection(&m_csMapFriendMaster);
	if(i == size)
		return FALSE;
	else
		return TRUE;
}

BOOL CFriendSystem::CreateFriendMaster(char *szMaster, int iNumber, int iServer)
{

	char szM[11] = {0};
	strncpy(szM, szMaster, 10);

	LPFRIEND_MASTER lpM;
	lpM = GetFriendMaster(szM);
	if(lpM)
	{
		lpM->m_iNumber = iNumber;
		lpM->m_iServer = iServer;
		lpM->m_btState = iServer;
		return TRUE;
	}

	FRIEND_MASTER Master;
	Master.m_iNumber = iNumber;
	Master.m_iServer = iServer;
	Master.m_btState = iServer;

	EnterCriticalSection(&this->m_csMapFriendMaster);
	this->m_MapFriendManager[szM] = Master;
	LeaveCriticalSection(&this->m_csMapFriendMaster);

	return TRUE;
}

BOOL CFriendSystem::DeleteFriendMaster(char *szMaster)
{
	std::map<string, FRIEND_MASTER, strCmp>::iterator it;

	char szM[11] = {0};
	strncpy(szM, szMaster, 10);

	EnterCriticalSection(&m_csMapFriendMaster);
	it = this->m_MapFriendManager.find(szM);
	if(it == this->m_MapFriendManager.end())
	{
		LeaveCriticalSection(&m_csMapFriendMaster);
		return FALSE;
	}

	this->m_MapFriendManager.erase(it);
	LeaveCriticalSection(&m_csMapFriendMaster);

	return TRUE;
		
}

int CFriendSystem::GetFriendGuid(char *szMaster)
{

	char szM[11] = {0};
	strncpy(szM, szMaster, 10);

	int guid = -1;
	LPFRIEND_MASTER lpMaster;
	EnterCriticalSection(&m_csMapFriendMaster);

	lpMaster = this->GetFriendMaster(szM);
	if(lpMaster)
		guid = lpMaster->m_iGUID;
	LeaveCriticalSection(&m_csMapFriendMaster);

	return guid;
}

BYTE CFriendSystem::GetFriendState(char *szMaster)
{
	BYTE btState = -1;
	LPFRIEND_MASTER lpMaster;

	EnterCriticalSection(&m_csMapFriendMaster);

	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster)
		btState = lpMaster->m_btState;
	LeaveCriticalSection(&m_csMapFriendMaster);

	return btState;
}

int CFriendSystem::GetFriendServer(char *szMaster)
{
	int iServer = -1;
	LPFRIEND_MASTER lpMaster;

	EnterCriticalSection(&m_csMapFriendMaster);

	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster)
		iServer = lpMaster->m_iServer;
	LeaveCriticalSection(&m_csMapFriendMaster);

	return iServer;
}

int CFriendSystem::GetFriendNumber(char *szMaster)
{
	int iNumber = -1;
	LPFRIEND_MASTER lpMaster;

	EnterCriticalSection(&m_csMapFriendMaster);

	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster)
		iNumber = lpMaster->m_iNumber;
	LeaveCriticalSection(&m_csMapFriendMaster);

	return iNumber;
}

BOOL CFriendSystem::FriendDBConnect()
{
	if ( this->m_MailDB.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE )
	{
		g_Log.AddC(TColor::Red, "[ERROR] - FRIEND DB CANNOT CONNECT TO MSSQL");
		return FALSE;
	}

	if ( this->m_FriendDB.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE )
	{
		g_Log.AddC(TColor::Red, "[ERROR] - FRIEND DB CANNOT CONNECT TO MSSQL");
		return FALSE;
	}

	g_Log.AddC(TColor::Green, "[SUCCESS] - FRIEND DB CONNECT MSSQL SUCCESS");

	return TRUE;
}

int CFriendSystem::ExDBGetIndexByCode(int iServerCode)
{
	if(iServerCode < 0)
		return -1;

	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD
				&& g_Server[i].m_ServerCode == iServerCode)
		{
			return i;
		}
	}

	return -1;
}

void CFriendSystem::FriendClose(int aIndex, LPBYTE lpMsg)
{
	char szMaster[11] = {0};

	strncpy(szMaster, ((SDHP_USERCLOSE *)lpMsg)->CharName, 10);
	
	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return;

	lpMaster->m_btState = -1;
	SendStateToAllFriends(aIndex, szMaster);
	DeleteFriendMaster(szMaster);
}

BOOL CFriendSystem::UserGuidDBCreate(char *szName)
{
	char Name[MAX_ACCOUNT_LEN+1];
	Name[10] = 0;
	memcpy(Name, szName, MAX_ACCOUNT_LEN);

	BOOL res = this->m_FriendDB.ExecQuery("EXEC WZ_UserGuidCreate '%s'", Name);
	this->m_FriendDB.Close();
	return res;
}

BOOL CFriendSystem::GetDBGuidAndMemoTotal(char *szName, int& guid, int& memo_total)
{
	BOOL res = FALSE;
	if(this->m_FriendDB.ExecQuery("SELECT GUID, MemoTotal FROM T_FriendMain where Name = '%s'", szName))
	{
		if(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			guid = this->m_FriendDB.GetAsInteger("GUID");
			memo_total = this->m_FriendDB.GetAsInteger("MemoTotal");
			res = TRUE;
		}
	}

	this->m_FriendDB.Close();

	return res;
}


BOOL CFriendSystem::GetDBFriendList(char *szMaster)
{
	LPFRIEND_MASTER lpMaster;

	BOOL res = TRUE;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return FALSE;

	if(GetDBGuidAndMemoTotal(szMaster, lpMaster->m_iGUID, lpMaster->m_iMemoTotal) == FALSE)
	{
		res = FALSE;
		if(UserGuidDBCreate(szMaster))
		{
			res = GetDBGuidAndMemoTotal(szMaster, lpMaster->m_iGUID, lpMaster->m_iMemoTotal);
		}
	}

	if(res == FALSE)
		return FALSE;

	res = FALSE;
	int i=0;
	char szFriend[11] = {0};

	if(this->m_FriendDB.ExecQuery("SELECT FriendGuid, FriendName, Del FROM T_FriendList where GUID = %d", lpMaster->m_iGUID))
	{
		while(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			if(i == this->m_iMaxFriends)
			{
				g_Log.AddC(TColor::Red, "DB contains more then %d friends for [%s] character", this->m_iMaxFriends, szMaster);
				break;
			}

			this->m_FriendDB.GetAsString("FriendName", szFriend, 10);
			if(FriendExists(szMaster, szFriend) == FALSE)
			{
				lpMaster->m_vecFriends.push_back(szFriend);
				i++;
			}
		}
	}

	this->m_FriendDB.Close();

	return res;
}

void CFriendSystem::FriendListSend(int aIndex, char *szMaster)
{
	LPFRIEND_MASTER lpMaster;

	g_Log.Add("[Friend List Send] Name[%s].", szMaster);
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
	{
		g_Log.Add("[Friend List Send] FrienMaster[%s] not found.", szMaster);
		return;
	}

	FHP_FRIENDLIST_COUNT* lpFriendListCnt;
	FHP_FRIENDLIST* lpFriendList;

	int cnt = (int)lpMaster->m_vecFriends.size();
	int size = sizeof(*lpFriendListCnt)+sizeof(*lpFriendList)*cnt;
	LPBYTE lpData = new BYTE[size];
	ZeroMemory(lpData, size);

	lpFriendListCnt = (FHP_FRIENDLIST_COUNT*)lpData;
	lpFriendList = (FHP_FRIENDLIST*)(lpData+sizeof(*lpFriendListCnt));
	PHeadSetW(lpData, 0x60, size);
	lpFriendListCnt->Count = cnt;
	lpFriendListCnt->MailCount = lpMaster->m_iMemoTotal;
	lpFriendListCnt->Number = lpMaster->m_iNumber;
	lpFriendListCnt->Server = lpMaster->m_iServer;
	strncpy(lpFriendListCnt->Name, szMaster, 10);
	
	g_Log.Add("[Friend List Send] Friend Count[%d].", cnt);
	for(int i=0; i < cnt; i++)
	{
		LPFRIEND_MASTER lpFr;
		int iServ = -1;
		lpFr = GetFriendMaster(lpMaster->m_vecFriends[i].m_szName);
		if(lpFr)
		{
			// check if friendship is established
			if(FriendExists(lpMaster->m_vecFriends[i].m_szName, szMaster))
				iServ = lpFr->m_btState;
		}

		lpFriendList[i].Server = iServ;
		strncpy(lpFriendList[i].Name, lpMaster->m_vecFriends[i].m_szName, 10);
		
		char szFriend[11] = {0};
		strncpy(szFriend, lpFriendList[i].Name, 10);
		g_Log.Add("[Friend List Send] Friend[%s] send.", szFriend);
	}

	DataSend(aIndex, lpData, size, __FUNCTION__);

	delete [] lpData;
}

BOOL CFriendSystem::GetDBWaitFriend(char *szMaster, OUT char *szWaitFriend)
{
	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);

	if(lpMaster == NULL)
		return FALSE;

	BOOL res = FALSE;
	if(this->m_FriendDB.ExecQuery("SELECT FriendName FROM T_WaitFriend where GUID=%d", lpMaster->m_iGUID))
	{
		if(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			this->m_FriendDB.GetAsString("FriendName", szWaitFriend, 10);
			res = TRUE;
		}
	}

	this->m_FriendDB.Close();

	return res;
}

BOOL CFriendSystem::FriendWaitSend(int aIndex, char *szMaster)
{
	FHP_WAITFRIENDLIST_COUNT WaitFr;
	ZeroMemory(&WaitFr, sizeof(WaitFr));
	PHeadSetB((LPBYTE)&WaitFr, 0x61, sizeof(WaitFr));

	
	char szWaitFriend[11] = {0};

	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return FALSE;

	if(GetDBWaitFriend(szMaster, szWaitFriend) == FALSE)
		return FALSE;

	strncpy(WaitFr.FriendName, szWaitFriend, 10);
	strncpy(WaitFr.Name, szMaster, 10);
	WaitFr.Number = lpMaster->m_iNumber;

	DataSend(aIndex, (LPBYTE)&WaitFr, WaitFr.h.size, __FUNCTION__);

	return TRUE;
}

void CFriendSystem::SendState(int aIndex, char *szMaster, int iNumber, char *szName, BYTE btState)
{
	FHP_FRIEND_STATE State;
	ZeroMemory(&State, sizeof(State));

	PHeadSetB((LPBYTE)&State, 0x62, sizeof(State));

	strncpy(State.Name, szMaster, 10);
	strncpy(State.FriendName, szName, 10);
	State.Number = iNumber;
	State.State = btState;

	DataSend(aIndex, (LPBYTE)&State, State.h.size, __FUNCTION__);
}

void CFriendSystem::SendStateToAllFriends(int aIndex, char *szMaster)
{
	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return;

	int cnt = (int)lpMaster->m_vecFriends.size();

	for(int i=0; i < cnt; i++)
	{
		LPFRIEND_MASTER lpFr;
		lpFr = GetFriendMaster(lpMaster->m_vecFriends[i].m_szName);
		if(lpFr)
		{
			// check if friendship is established
			if(FriendExists(lpMaster->m_vecFriends[i].m_szName, szMaster))
			{
				int index = ExDBGetIndexByCode(lpFr->m_iServer);
				if(index != -1)
				{
					SendState(index, 
						lpMaster->m_vecFriends[i].m_szName, 
						lpFr->m_iNumber, szMaster, lpMaster->m_btState);
				}
			}
		}
	}
}

void CFriendSystem::FriendListRequest(int aIndex, FHP_FRIENDLIST_REQ* lpMsg)
{
	
	char szMaster[11]={0};
	strncpy(szMaster, lpMsg->Name, 10);

	g_Log.Add("[Friend List Request] Name[%s].", szMaster);
	if(CreateFriendMaster(szMaster, lpMsg->Number, lpMsg->pServer) == FALSE)
		return;

	GetDBFriendList(szMaster);
	FriendListSend(aIndex, szMaster);
	FriendWaitSend(aIndex, szMaster);
	SendStateToAllFriends(aIndex, szMaster);

	FHP_FRIEND_MEMO_LIST_REQ req;
	ZeroMemory(&req, sizeof(req));
	PHeadSetB((LPBYTE)&req, 0x71, sizeof(req));

	strncpy(req.Name, lpMsg->Name, 10);
	req.Number = lpMsg->Number;
	FriendMemoListReq(aIndex,&req);

	int ServerIndex = this->ExDBGetIndexByCode(lpMsg->pServer);

	m_EXDSProtocol.GuildMatchingSendAcceptAndRejectInfo(aIndex, szMaster, lpMsg->Number, ServerIndex);

	if (m_EXDSProtocol.m_PartyMatchingEDS.ConnectMember(lpMsg->Name, lpMsg->Number, lpMsg->pServer, ServerIndex) == TRUE)
	{
		PARTYMATCHING_PARTYDATA * lpPartyData = m_EXDSProtocol.m_PartyMatchingEDS.GetPartyDataByMember(lpMsg->Name);

		if (lpPartyData)
		{
			if (strcmp(lpPartyData->szLeaderName, lpMsg->Name))
			{
				if (lpPartyData->m_UserData[0].iServerIndex == ServerIndex)
				{
					m_EXDSProtocol.FixAddPartyMember(lpPartyData->m_UserData[0].iUserIndex, lpMsg->Number, aIndex);
				}
			}

			else
			{
				for (int i = 1; i < 5; i++)
				{
					if (lpPartyData->m_UserData[i].iUsedInfo == TRUE && lpPartyData->m_UserData[i].iUserConnected == TRUE)
					{
						if (lpPartyData->m_UserData[i].iServerIndex == aIndex)
						{
							m_EXDSProtocol.FixAddPartyMember(lpPartyData->m_UserData[0].iUserIndex, lpPartyData->m_UserData[i].iUserIndex, aIndex);
						}
					}
				}
			}

			m_EXDSProtocol.SendPartyMatchingMemberList(lpPartyData->m_UserData[0].szUserName, 0);
		}
	}
}

BOOL CFriendSystem::AddFriend(char *szMaster, char *szFriend)
{
	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return FALSE;

	if( lpMaster->m_vecFriends.size() == this->m_iMaxFriends)
	{
		g_Log.AddC(TColor::Firebrick, "[%s]'s friend list is full - %d", szMaster, this->m_iMaxFriends);
		return FALSE;
	}

	lpMaster->m_vecFriends.push_back(szFriend);

	g_Log.Add("[%s] became [%s]'s friend.", szFriend, szMaster);
	return TRUE;
}

int CFriendSystem::AddDBWaitFriend(char *szMaster, char *szFriend)
{
	int res = 1;
	if(this->m_FriendDB.ExecQuery("EXEC WZ_WaitFriendAdd '%s', '%s'", szMaster, szFriend))
	{
		if(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			res = this->m_FriendDB.GetResult(0);
			if(res != 0)
			{
				if(res == 6)
					g_Log.Add("[%s]'s level is too low.", szFriend);
				else if(res == 3)
					g_Log.Add("GUID for [%s] doesn't exists.", szFriend);
				else if(res != 8)
					g_Log.AddC(TColor::Red, "WZ_WaitFriendAdd returned: %d", res);
			}
		}
		else
			g_Log.AddC(TColor::Red, "WZ_WaitFriendAdd no result returned.");
	}
	else
		g_Log.AddC(TColor::Red, "Error executing WZ_WaitFriendAdd.");

	this->m_FriendDB.Close();

	return res;
}

void CFriendSystem::FriendAddRequest( int aIndex, FHP_FRIEND_ADD_REQ* lpMsg)
{
	FHP_FRIEND_ADD_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));
	PHeadSetB((LPBYTE)&Result, 0x63, sizeof(Result));

	strncpy(Result.Name, lpMsg->Name, 10);
	strncpy(Result.FriendName, lpMsg->FriendName, 10);
	Result.Result = 2;
	Result.Server = -1;
	Result.Number = lpMsg->Number;

	char szMaster[11] = {0};
	char szFriend[11] = {0};
	strncpy(szMaster, Result.Name, 10);
	strncpy(szFriend, Result.FriendName, 10);

	g_Log.Add("[Friend Add Request] Name[%s] FriendName[%s].", szMaster, szFriend);

	if(strcmp(szMaster, szFriend) == 0)
	{
		// own id
		Result.Result = 5;

		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
	{
		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(FriendExists(szMaster, szFriend))
	{
		Result.Result = 4;

		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(lpMaster->m_vecFriends.size() == this->m_iMaxFriends)
	{
		Result.Result = 3;

		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	int res = AddDBWaitFriend(szMaster, szFriend);
	
	// DB result, GUID doesn't exists, or select error
	if(res > 1 && res < 6)
	{
		Result.Result = 0;

		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	// DB result, lvl< 6
	if(res == 6)
	{
		Result.Result = 6;
		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	// DB, other errors
	if(res != 0 && res != 8)
	{
		Result.Result = 2;
		g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	LPFRIEND_MASTER lpFr;
	lpFr = GetFriendMaster(szFriend);

	Result.Server = -1;
	BYTE btState = 0;
	if(lpFr)
	{
		// check if friendship is established
		if(FriendExists(szFriend, szMaster))
		{
			Result.Server = lpFr->m_btState;
			SendState(aIndex, szFriend, lpFr->m_iNumber, szMaster, lpMaster->m_btState);
		}

		int index = ExDBGetIndexByCode(lpFr->m_iServer);
		if(index != -1)
			FriendWaitSend(index, szFriend); // fixed
	}


	AddFriend(szMaster, szFriend);

	Result.Result = 1;

	g_Log.Add("[Friend Add Request] Send Result [%d].", Result.Result);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

}

int CFriendSystem::DelDBWaitFriend(char *szMaster, char *szFriend)
{
	int res = 1;

	if(this->m_FriendDB.ExecQuery("EXEC WZ_WaitFriendDel '%s', '%s'", szMaster, szFriend))
	{
		if(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			res = this->m_FriendDB.GetResult(0);
			if(res != 0)
				g_Log.AddC(TColor::Red, "WZ_WaitFriendDel returned: [%d]", res);
		}
		else
			g_Log.AddC(TColor::Red, "WZ_WaitFriendDel no result returned.");
	}
	else
		g_Log.AddC(TColor::Red, "Error executing WZ_WaitFriendDel.");

	this->m_FriendDB.Close();

	return res;
}

BOOL CFriendSystem::AddDBFriend(char *szMaster, char *szFriend)
{
	int res = -1;

	if(this->m_FriendDB.ExecQuery("EXEC WZ_FriendAdd '%s', '%s'", szMaster, szFriend))
	{
		if(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			res = this->m_FriendDB.GetResult(0);
			if(res != 1)
				g_Log.AddC(TColor::Red, "WZ_FriendAdd returned: %d", res);

		}
		else
			g_Log.AddC(TColor::Red, "WZ_FriendAdd no result returned.");

	}
	else
		g_Log.AddC(TColor::Red, "Error executing WZ_FriendAdd.");

	this->m_FriendDB.Close();

	if(res == 1)
		return TRUE;

	
	return FALSE;
	
}

BOOL CFriendSystem::DelDBFriend(char *szMaster, char *szFriend)
{
	int res = -1;

	if(this->m_FriendDB.ExecQuery("EXEC WZ_FriendDel '%s', '%s'", szMaster, szFriend))
	{
		if(this->m_FriendDB.Fetch() != SQL_NO_DATA)
		{
			res = this->m_FriendDB.GetResult(0);
			if(res != 1)
				g_Log.AddC(TColor::Red, "WZ_FriendDel returned: %d", res);

		}
		else
			g_Log.AddC(TColor::Red, "WZ_FriendDel no result returned.");

	}
	else
		g_Log.AddC(TColor::Red, "Error executing WZ_FriendDel.");

	this->m_FriendDB.Close();

	if(res == 1)
		return TRUE;

	
	return FALSE;
}

int CFriendSystem::WaitFriendAdd(char *szMaster, char *szFriend)
{
	LPFRIEND_MASTER lpMaster;

	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return 0;

	if(FriendExists(szMaster, szFriend))
		return 4;

	if(lpMaster->m_vecFriends.size() == this->m_iMaxFriends)
	{
		g_Log.AddC(TColor::Firebrick, "[%s]'s friend list is full - %d", szMaster, m_iMaxFriends);
		return 3;
	}

	if(AddDBFriend(szMaster, szFriend) == FALSE)
		return 0;

	if(AddFriend(szMaster, szFriend) == FALSE)
		return 0;

	LPFRIEND_MASTER lpFr;
	lpFr = GetFriendMaster(szFriend);
	if(lpFr)
	{
		int index = ExDBGetIndexByCode(lpFr->m_iServer);
		if(index != -1)
			SendState(index, szFriend, lpFr->m_iNumber, szMaster, lpMaster->m_btState);
	}

	return 1;
}

void CFriendSystem::WaitFriendAddRequest( int aIndex,  FHP_WAITFRIEND_ADD_REQ* lpMsg)
{
	FHP_WAITFRIEND_ADD_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0x64, sizeof(Result));
	Result.Number = lpMsg->Number;

	strncpy(Result.Name, lpMsg->Name, 10);
	strncpy(Result.FriendName, lpMsg->FriendName, 10);
	Result.pServer = -1;
	
	char szMaster[11] = {0};
	char szFriend[11] = {0};

	strncpy(szMaster, lpMsg->Name, 10);
	strncpy(szFriend, lpMsg->FriendName, 10);

	g_Log.Add("[WaitFriend Add Request] Name[%s], FriendName[%s].", szMaster, szFriend);

	if(lpMsg->Result == 0)
	{
		Result.Result = 2;
		DelDBWaitFriend(szMaster, szFriend);
		g_Log.Add("[WaitFriend Add Request] [%s] declined [%s]'s request to be a friend.", szMaster, szFriend);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	g_Log.Add("[WaitFriend Add Request] [%s] accepted [%s]'s request to be a friend.", szMaster, szFriend);

	LPFRIEND_MASTER lpFr;
	lpFr = GetFriendMaster(szFriend);
	if(lpFr)
		Result.pServer = lpFr->m_btState;

	Result.Result = WaitFriendAdd(szMaster, szFriend);
	if(Result.Result != 1)
		DelDBWaitFriend(szMaster, szFriend);

	FriendWaitSend(aIndex, szMaster);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
}

void CFriendSystem::FriendStateClientRecv( int aIndex, FHP_FRIEND_STATE_C* lpMsg)
{

	char szMaster[11] = {0};
	strncpy(szMaster, lpMsg->Name, 10);

	g_Log.Add("[Friend State Receive] Name: [%s], State: [%d].", szMaster, lpMsg->State);
	LPFRIEND_MASTER lpMaster; 
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
	{
		g_Log.Add("[Friend State Receive] error: FriendMaster [%s] not found.", szMaster);
		return;
	}

	switch(lpMsg->State)
	{
		case 0:
			// refuse chat
			lpMaster->m_btState = -2;
		break;

		case 2:
			// ???, client shows "Cannot Use"
			lpMaster->m_btState = -3;
		break;

		default:
			// display server
			lpMaster->m_btState = lpMaster->m_iServer;
	}

	SendStateToAllFriends(aIndex, szMaster);
}

BOOL CFriendSystem::DelFriend(char *szMaster, char *szFriend)
{
	LPFRIEND_MASTER lpMaster;
	lpMaster = GetFriendMaster(szMaster);
	if(lpMaster == NULL)
		return FALSE;

	if(FriendExists(szMaster, szFriend) == FALSE)
		return FALSE;

	int cnt = (int)lpMaster->m_vecFriends.size();
	for(int i=0; i < cnt; i++)
	{
		if(strcmp(lpMaster->m_vecFriends[i].m_szName, szFriend) == 0)
		{
			lpMaster->m_vecFriends.erase(lpMaster->m_vecFriends.begin()+i);
			g_Log.Add("[%s] dropped [%s] from his friend list.", szMaster, szFriend);
			break;
		}

	}
	return TRUE;
}

void CFriendSystem::FriendDelRequest( int aIndex, FHP_FRIEND_ADD_REQ* lpMsg)
{
	FHP_FRIEND_DEL_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0x65, sizeof(Result));

	Result.Number = lpMsg->Number;

	char szMaster[11] = {0};
	char szFriend[11] = {0};
	
	strncpy(Result.Name, lpMsg->Name, 10);
	strncpy(Result.FriendName, lpMsg->FriendName, 10);

	strncpy(szMaster, lpMsg->Name, 10);
	strncpy(szFriend, lpMsg->FriendName, 10);

	g_Log.Add("[Friend Delete Request] Name[%s], FriendName[%s].", szMaster, szFriend);

	Result.Result = DelFriend(szMaster, szFriend);
	if(Result.Result)
		Result.Result = DelDBFriend(szMaster, szFriend);

	g_Log.Add("[Friend Delete Request] Send Result [%d].", Result.Result);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

	LPFRIEND_MASTER lpFr;
	lpFr = GetFriendMaster(szFriend);
	if(lpFr)
	{
		int index = ExDBGetIndexByCode(lpFr->m_iServer);

		if(index != -1)
			SendState(index, szFriend, lpFr->m_iNumber, szMaster, -1);
	}
}


void CFriendSystem::MemoHeaderSend(int aIndex, WORD wNumber, MEMO_HEADER * lpMemoHead )
{
	FHP_FRIEND_MEMO_LIST MemoHead;
	ZeroMemory(&MemoHead, sizeof(MemoHead));

	PHeadSetW((LPBYTE)&MemoHead, 0x71, sizeof(MemoHead));
	MemoHead.Number = wNumber;
	memcpy(MemoHead.Date, lpMemoHead->Date, 30);
	memcpy(MemoHead.SendName, lpMemoHead->SendName, 10);
	memcpy(MemoHead.RecvName, lpMemoHead->RecvName, 10);
	memcpy(MemoHead.Subject, lpMemoHead->Subject, 60);
	MemoHead.MemoIndex = lpMemoHead->MemoIndex;
	MemoHead.read = lpMemoHead->read;

	DataSend(aIndex, (LPBYTE)&MemoHead, sizeof(MemoHead), __FUNCTION__);
}

void CFriendSystem::MemoListSend(int aIndex, WORD wNumber, char *szName)
{

	char szMaster[11] = {0};
	strncpy(szMaster, szName, 10);

	g_Log.Add("[Mail List Send] Name: [%s].", szMaster);
	int guid = GetFriendGuid(szMaster);
	if(guid == -1)
	{
		g_Log.Add("[Mail List Send] error: GUID = -1.");
		return;
	}

	int cnt = 0;
	if(this->m_MailDB.ExecQuery("SELECT MemoIndex, FriendName, wDate, Subject, bRead FROM T_FriendMail where GUID=%d", guid))
	{
		while(this->m_MailDB.Fetch() != SQL_NO_DATA)
		{
			MEMO_HEADER head;
			ZeroMemory(&head, sizeof(head));
			strncpy(head.RecvName, szName, 10);

			head.MemoIndex = this->m_MailDB.GetAsInteger("MemoIndex");
			this->m_MailDB.GetAsString("FriendName", head.SendName, 10);
			this->m_MailDB.GetAsString("wDate", head.Date, 30);
			this->m_MailDB.GetAsString("Subject", head.Subject, 60);
			head.read = this->m_MailDB.GetAsInteger("bRead");
			MemoHeaderSend(aIndex, wNumber, &head);
			cnt++;
		}
	}

	this->m_MailDB.Close();

	g_Log.Add("[Mail List Send] Mail Count: %d.", cnt);
}

void CFriendSystem::FriendMemoListReq( int aIndex, FHP_FRIEND_MEMO_LIST_REQ* lpMsg)
{

	char szName[11] = {0};
	strncpy(szName, lpMsg->Name, 10);

	g_Log.Add("[Mail List Request] Name[%s].", szName);
	MemoListSend(aIndex, lpMsg->Number, szName);
}

int CFriendSystem::DBWriteMail(MEMO_SEND_HEADER * lpMemoSendHdr, LPBYTE Photo, BYTE btPhotoSize, char *sMemo, int memo_size)
{
	char szSendName[11] = {0};
	char szRecvName[11] = {0};

	strncpy(szSendName, lpMemoSendHdr->Name, 10);
	strncpy(szRecvName, lpMemoSendHdr->ToName, 10);

	if(this->m_MailDB.ExecQuery("EXEC WZ_WriteMail '%s', '%s', '%s', %d, %d", 
		szSendName, szRecvName, NULL, lpMemoSendHdr->Dir,  lpMemoSendHdr->Action))
	{
		if(this->m_MailDB.Fetch() != SQL_NO_DATA)
		{
			int memo_index;
			int guid;

			memo_index = this->m_MailDB.GetResult(0);
			guid = this->m_MailDB.GetResult(1);
			this->m_MailDB.Close();

			if(memo_index <= 10)
			{
				if(memo_index == 2 || memo_index == 3)
					return -3;

				if(memo_index == 5)
					return -2;

				if(memo_index == 6)
					return -6;

				g_Log.AddC(TColor::Red, "WZ_WriteMail returned: %d", memo_index);
				return - 1;
			}

			if(guid < 0)
			{
				g_Log.AddC(TColor::Red, "WZ_WriteMail returned guid: %d", guid);
				return - 1;
			}

			char szMsg[1500];

			ZeroMemory(szMsg, 1500);
			wsprintf(szMsg, "UPDATE T_FriendMail SET Photo=? where MemoIndex=%d AND GUID=%d", memo_index, guid);
			this->m_MailDB.SetAsBinary(szMsg, Photo, btPhotoSize);

			ZeroMemory(szMsg, 1500);
			wsprintf(szMsg, "UPDATE T_FriendMail SET Subject=? where MemoIndex=%d AND GUID=%d", memo_index, guid);
			this->m_MailDB.SetAsBinary(szMsg, (LPBYTE)lpMemoSendHdr->Subject, 60);

			ZeroMemory(szMsg, 1500);
			wsprintf(szMsg, "UPDATE T_FriendMail SET Memo=? where MemoIndex=%d AND GUID=%d", memo_index, guid);
			this->m_MailDB.SetAsBinary(szMsg, (LPBYTE)sMemo, memo_size);

			return memo_index;
		}
		else
		{
			g_Log.AddC(TColor::Red, "WZ_WriteMail no result returned.");
			this->m_MailDB.Close();
		}
	}
	else
	{
		g_Log.AddC(TColor::Red, "Error executing WZ_WriteMail");
		this->m_MailDB.Close();
	}

	return -1;
}

void CFriendSystem::FriendMemoSend( int aIndex, FHP_FRIEND_MEMO_SEND* lpMsg)
{
	FHP_FRIEND_MEMO_SEND_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));
	PHeadSetB((LPBYTE)&Result, 0x70, sizeof(Result));

	Result.Number = lpMsg->Number;
	Result.WindowGuid = lpMsg->WindowGuid;
	strncpy(Result.Name, lpMsg->Name, 10);
	
	char szName[11] = {0};
	strncpy(szName, Result.Name, 10);

	MEMO_SEND_HEADER head;
	ZeroMemory(&head, sizeof(head));

	memcpy(&head, lpMsg->Name, sizeof(head));
	
	int photo_size;
	int msg_size = MAKEWORD(lpMsg->h.sizeL, lpMsg->h.sizeH);
	LPBYTE lpMemo;
	LPBYTE lpPhoto;
	
	photo_size = 18;
	lpMemo = (LPBYTE)lpMsg->Memo;
	lpPhoto = lpMsg->Photo;

	char szMaster[11]={0};
	strncpy(szMaster, lpMsg->ToName, 10);

	g_Log.Add("[Mail Send Request] Name[%s], ToName[%s], MailSize[%d], PhotoSize[%d].", 
		szName, szMaster, lpMsg->MemoSize, photo_size);

	int res = -1;

	if(lpMsg->MemoSize <= 1000)
		res = DBWriteMail(&head, lpPhoto, photo_size, (char *)lpMemo, lpMsg->MemoSize);
	else
		g_Log.Add("[Mail Send Request] error: Mail Size > 1000.");

	if(res == -2)
		Result.Result = 2;
	else if(res == -3)
		Result.Result = 3;
	else if(res == -6)
		Result.Result = 6;
	else if(res < 0)
		Result.Result = 0;
	else
		Result.Result = 1;

	g_Log.Add("[Mail Send Request] Send Result [%d].", Result.Result);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
	
	if(Result.Result != 1)
		return;
	
	
	LPFRIEND_MASTER lpFr;
	lpFr = GetFriendMaster(szMaster);

	if(!lpFr)
	{
		return;
	}

	MEMO_HEADER memo_head;
	memo_head.MemoIndex = res;
	memo_head.read = 0;
	strncpy(memo_head.Subject, lpMsg->Subject, 60);
	strncpy(memo_head.RecvName, lpMsg->ToName, 10);
	strncpy(memo_head.SendName, lpMsg->Name, 10);
	
	SYSTEMTIME st;
	GetLocalTime(&st);

	wsprintf(memo_head.Date, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	memo_head.read = 2;

	int index = ExDBGetIndexByCode(lpFr->m_iServer);
	if (index != -1)
		MemoHeaderSend(index, lpFr->m_iNumber, &memo_head);
}

BOOL CFriendSystem::DBReadMail(int iMemoId, int iGuid, MEMO_READ * lpMemoRead, LPDWORD lpdwMemoSize, LPBYTE lpPhoto, LPBYTE lpPhotoSize)
{
	if(this->m_MailDB.ExecQuery("SELECT bRead, Dir, Act FROM T_FriendMail where MemoIndex=%d AND GUID =%d", iMemoId, iGuid))
	{
		int bRead = 0;
		if(this->m_MailDB.Fetch() != SQL_NO_DATA)
		{
			bRead = this->m_MailDB.GetAsInteger("bRead");
			lpMemoRead->Dir = this->m_MailDB.GetAsInteger("Dir");
			lpMemoRead->Action = this->m_MailDB.GetAsInteger("Act");
		}

		this->m_MailDB.Close();

		char szMsg[1500];
		ZeroMemory(szMsg, 1500);

		wsprintf(szMsg, "SELECT Photo FROM T_FriendMail where MemoIndex='%d' AND GUID=%d", iMemoId, iGuid);
		*lpPhotoSize = this->m_MailDB.GetAsBinary(szMsg, lpPhoto, 18);

		if(*lpPhotoSize >= 18)
			*lpPhotoSize = 18;

		ZeroMemory(szMsg, 1500);
		wsprintf(szMsg, "SELECT Memo FROM T_FriendMail where MemoIndex='%d' AND GUID=%d", iMemoId, iGuid);
		*lpdwMemoSize = this->m_MailDB.GetAsBinary(szMsg, (LPBYTE)lpMemoRead->Memo,sizeof(lpMemoRead->Memo));

		if(*lpdwMemoSize < 0)
			*lpdwMemoSize = 0;

		if(bRead == 0)
		{
			this->m_MailDB.ExecQuery("UPDATE T_FriendMail SET bRead = 1 where MemoIndex=%d AND GUID=%d", iMemoId, iGuid);
			this->m_MailDB.Close();
		}

		return TRUE;

	}
	else
	{
		this->m_MailDB.Close();
		return FALSE;
	}
}

void CFriendSystem::FriendMemoReadReq( int aIndex, FHP_FRIEND_MEMO_RECV_REQ* lpMsg)
{

	MEMO_READ memo;
	ZeroMemory(&memo, sizeof(memo));
	DWORD memo_size=0;
	BYTE Photo[30];
	memset(Photo, -1, sizeof(Photo));
	BYTE btPhotoSize=18;
	char szMaster[11]={0};
	strncpy(szMaster, lpMsg->Name, 10);

	g_Log.Add("[Mail Read Request] Name: [%s], MemoIndex: [%d].", szMaster, lpMsg->MemoIndex);
	int guid = -1;
	guid = GetFriendGuid(szMaster);
	if(guid < 0)
	{
		g_Log.Add("[Mail Read Request] error: GUID = -1.");
		return;
	}

	if(DBReadMail(lpMsg->MemoIndex, guid, &memo, &memo_size, Photo, &btPhotoSize))
	{
		FHP_FRIEND_MEMO_RECV Result;
		ZeroMemory(&Result, sizeof(Result));

		PHeadSetW((LPBYTE)&Result, 0x72, sizeof(Result)-sizeof(Result.Memo)+memo_size);

		Result.MemoIndex = lpMsg->MemoIndex;
		Result.Number = lpMsg->Number;
		strncpy(Result.Name, lpMsg->Name, 10);

		Result.MemoSize = memo_size;
		memcpy(&Result.Dir, &memo, sizeof(memo));
		memcpy(Result.Photo, Photo, 18);
		
		DataSend(aIndex, (LPBYTE)&Result, sizeof(Result)-sizeof(Result.Memo)+memo_size, __FUNCTION__);

		g_Log.Add("[Mail Read Request] Send Mail: size[%d], photo size[%d].", memo_size, btPhotoSize);
	}
}

BOOL CFriendSystem::DBDelMail(char *szName, int iMemoId)
{
	int res = FALSE;
	if(this->m_MailDB.ExecQuery("EXEC WZ_DelMail '%s', %d", szName, iMemoId))
	{
		if(this->m_MailDB.Fetch() != SQL_NO_DATA)
		{
			res = this->m_MailDB.GetResult(0);
			if(res != 1)
			{
				g_Log.AddC(TColor::Red, "WZ_DelMail returned: [%d]", res);
				res = FALSE;
			}
			else
				res = TRUE;
		}
		else
			g_Log.AddC(TColor::Red, "WZ_DelMail no result returned.");
	}
	else
		g_Log.AddC(TColor::Red, "Error executing WZ_DelMail");

	this->m_MailDB.Close();

	return res;
}

void CFriendSystem::FriendMemoDelReq( int aIndex, FHP_FRIEND_MEMO_DEL_REQ* lpMsg)
{

	FHP_FRIEND_MEMO_DEL_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));
	PHeadSetB((LPBYTE)&Result, 0x73, sizeof(Result));

	Result.MemoIndex = lpMsg->MemoIndex;
	Result.Number = lpMsg->Number;
	strncpy(Result.Name, lpMsg->Name, 10);

	char szName[11]={0};
	strncpy(szName, lpMsg->Name, 10);

	g_Log.Add("[Mail Delete Request] Name[%s], MamoIndex[%d].", szName, lpMsg->MemoIndex);

	Result.Result = DBDelMail(szName, lpMsg->MemoIndex);

	g_Log.Add("[Mail Delete Request] Send Result [%d].", Result.Result);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

